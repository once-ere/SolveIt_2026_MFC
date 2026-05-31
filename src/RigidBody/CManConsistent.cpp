//CManConsistent.cpp
//////////////////////////////////////////////////////////////////////
// CM.cpp : Implementation of CConstraintManager

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "PtToPtConstraint.h"
//#include "RigidBody.h"
#include "ConstraintManager.h"
#include "RigidBodyManager.h"
#include "PtToPtConstraint.h"
#include "parser.h" /* for token definitions and yylval */
#include "MainFrm.h"
///////////////////////////////////////////////////////////////////////////////
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
struct sysVel
{  
	double* vel;//to Velocity component
	double jac;
	long comp, constr_num;
	bool used;
	sysVel(double* v, double& r, const int _comp, int n) : vel(v), jac(r), comp(_comp), constr_num(n), used(false) {}

	sysVel(const sysVel& a) : vel(a.vel), jac(a.jac), comp(a.comp), constr_num(a.constr_num), used(a.used)   {}
	sysVel& operator=(const sysVel& a)
	{
		vel=a.vel, jac=a.jac, comp=a.comp, constr_num=a.constr_num, used=a.used;
		return *this;
	}
};
///////////////////////////////////////////////////////////////////////////////
typedef vector<sysVel> VecSysVel;
///////////////////////////////////////////////////////////////////////////////
struct sysVelCompare
{  
	bool operator()(const sysVel& a, const sysVel& b) const
	{
		if (b.used) return false;
		if (a.used) return true;
		return fabs(a.jac) < fabs(b.jac);
	}
};
struct sysVelCompareEqual
{
	sysVel* b;
	bool operator()(const sysVel& a) const
	{
		return a.vel == b->vel;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct IntCompareEqual
{
	int n;
	IntCompareEqual(int j) : n(j) {}
	bool operator()(const int& a) const
	{
		return a == n;
	}
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::CalculateConsistentPosition(CRigidBody* body)
{
	vector<CRigidBody*>::iterator i = FindBody(body);
	if (i == m_vecBodies.end())
	{
	}

}
///////////////////////////////////////////////////////////////////////////////
//m3.vel=[1,2,3]



void CConstraintManager::CalculateConsistentVelocity(CRigidBody* pbody)
{
	const double smallNum = 0.1e-7;
//	return;
	if (Empty()) return;
	if (m_vecBodies.empty()) return;

	vector<CRigidBody*>::iterator i = FindBody(pbody);
	if (pbody!=0 && i == m_vecBodies.end())
	{
		return;
	}

	AssignVarsToColumns();
//sets m_NumberOfIndependentVariables, m_vecSystemInverseMassMatrix, m_vecSystemVelocity
//sets ud.body?->m_bConstraintForceShouldBeUpdated = true;

	m_ConstraintedBodies.clear();

	
	CSystem* m_system = theApp.GetSystem();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	
	rigidBodyManager->GetConstraintedBodies(m_ConstraintedBodies);

//	ASSERT(m_vecBodies.size() == m_ConstraintedBodies.size());

	long constr_dim		= Size();//m_listPointMassConstraints.size()
	int dim				= m_vecBodies.size();
	int Dim				= 3*dim;

	double t = m_system->m__t;
	double ** J;
	Jacobian(t, J, constr_dim);


//	TNT::Subscript nVars = J.num_cols();
//	ASSERT(nVars == Dim);

	vector<double> Velocity(Dim, 0.0);
	int j=0, a=0, k=0;

	VecSysVel* vec_sysVel	= new VecSysVel[constr_dim];
	sysVel** maxFabs		= new sysVel*[constr_dim];

	double** doublePtrRefs	= new double*[Dim];

	vector<int> vec_constr_avail;
	for (a=0; a < constr_dim; ++a) vec_constr_avail.push_back(a);

	vector<CRigidBody *>::iterator i_vecBodies;

	for (j=0, i_vecBodies = m_vecBodies.begin(); i_vecBodies != m_vecBodies.end(); ++i_vecBodies, ++j)
	{
		CRigidBody *body = *i_vecBodies;
		if (body==0) return;//continue;
		int x = 3*j+0, y = 3*j+1, z = 3*j+2;
		doublePtrRefs[x] = &body->m_vecVelocity.x;
		doublePtrRefs[y] = &body->m_vecVelocity.y;
		doublePtrRefs[z] = &body->m_vecVelocity.z;

		for (a=0; a<constr_dim;++a)
		{
			Vector3D v(J[a][x], J[a][y], J[a][z]);
			if (v.norm()<smallNum)
			{
		//		IntCompareEqual intCompareEqual(a);
		//		vector<int>::iterator j = find_if(vec_constr_avail.begin(), vec_constr_avail.end(), intCompareEqual);
		//		if (j != vec_constr_avail.end()) vec_constr_avail.erase(j);
				continue;
			}
			v.normalize();
			Vector3D vecVelocity = v.vectorProd((body->m_vecVelocity.vectorProd(v)));
			body->set_Velocity(vecVelocity);
			if (fabs(J[a][x]) > smallNum)
			{
				sysVel X ( doublePtrRefs[x], J[a][x], x, a);
				vec_sysVel[a].push_back( X );
			}
			if (fabs(J[a][y]) > smallNum)
			{
				sysVel Y ( doublePtrRefs[y], J[a][y], y, a);
				vec_sysVel[a].push_back( Y );
			}
			if (fabs(J[a][z]) > smallNum)
			{
				sysVel Z ( doublePtrRefs[z], J[a][z], z, a);
				vec_sysVel[a].push_back( Z );
			}
		}
	}

	for (a=0; a<constr_dim;++a)
	{
		if (vec_sysVel[a].empty())
		{
			IntCompareEqual intCompareEqual(a);
			vector<int>::iterator j = find_if(vec_constr_avail.begin(), vec_constr_avail.end(), intCompareEqual);
			if (j != vec_constr_avail.end()) vec_constr_avail.erase(j);
		}
	}
	if (vec_constr_avail.empty()) return;

	sysVelCompareEqual _sysVelCompareEqual;
	vector<sysVel> vec_MxSysVel;
	sysVelCompare _sysVelCompare;
	VecSysVel::iterator ptr;
	vector<int>::iterator K;

	do {
		vec_MxSysVel.clear();
		for (K=vec_constr_avail.begin(); K != vec_constr_avail.end(); ++K)
		{
			sort(vec_sysVel[*K].begin(), vec_sysVel[*K].end(), _sysVelCompare);//depends on .used
			vec_MxSysVel.push_back( vec_sysVel[*K].back() );
		}

		sort(vec_MxSysVel.begin(), vec_MxSysVel.end(), _sysVelCompare);
		k = vec_MxSysVel.back().constr_num;
		{
			IntCompareEqual intCompareEqual(k);
			vector<int>::iterator j = find_if(vec_constr_avail.begin(), vec_constr_avail.end(), intCompareEqual);
			if (j != vec_constr_avail.end())
			{
				vec_constr_avail.erase(j);
			}
		}

		_sysVelCompareEqual.b = maxFabs[k] = &vec_MxSysVel.back();
		ASSERT(maxFabs[k]->jac!=0.0);
		double*& VelocityComp = maxFabs[k]->vel;
		*VelocityComp = 0;
		double velocityComp = 0;
		for (ptr = vec_sysVel[k].begin(); ptr != vec_sysVel[k].end(); ++ptr)
		{
			velocityComp -= (*(ptr->vel))*ptr->jac;
		}
		velocityComp /= maxFabs[k]->jac;
		*VelocityComp = velocityComp;

		for (a=0; a<constr_dim;++a)
		{
			ptr = find_if(vec_sysVel[a].begin(), vec_sysVel[a].end(), _sysVelCompareEqual );
			if ( ptr != vec_sysVel[a].end() )
			{
				ptr->used = true;
			}
		}

	} while (!vec_constr_avail.empty());

	for (j=0, i_vecBodies = m_vecBodies.begin(); i_vecBodies != m_vecBodies.end(); ++i_vecBodies, ++j)
	{
		CRigidBody *body = *i_vecBodies;
		body->set_Velocity(body->m_vecVelocity);
	}

	delete[] doublePtrRefs;
	delete[] maxFabs;
	delete[] vec_sysVel;
}

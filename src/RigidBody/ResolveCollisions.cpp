// ResolveCollisions.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include <functional>
#include <algorithm>

#include "RigidBodyManager.h"
#include "RigidBody.h"
#include "System.h"
#include "Contact.h"
#include "objects.h"
#include "ExamineCollisionTimes.h"

#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */
#include "Matherror.h"

#include "parser.h" /* for token definitions and yylval */
/*
#undef min
#undef max
#include <mtl/mtl.h>
#include <mtl/lapack_interface.h>
#include <mtl/mtl2lapack.h>
#include <mtl/dense1D.h>
#include <mtl/utils.h>
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
	long ToLongSize(size_t value)
	{
		return static_cast<long>(value);
	}

	template <typename Iterator>
	int ToIntDistance(Iterator begin, Iterator current)
	{
		return static_cast<int>(distance(begin, current));
	}
}


const double CVodeRTOL = 1.0e-8;
const double CVodeFLOOR = .01;
const double CVodeATOL = CVodeRTOL*CVodeFLOOR;

typedef void (*ThreadDerivative)( CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data );
//void CVodeDerivative(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data);
//ThreadDerivative FieldLine_Tangent;
///////////////////////////////////////////////////////////////////////////////
//int _matherr( struct _exception *except ){return 1;}
///////////////////////////////////////////////////////////////////////////////
//#include "SWIFT_World.h"
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct ContactsCompareTimes
{  
	bool operator()(const Contact* a, const Contact* b) const
	{
		return a->m_estimatedCollisionTime < b->m_estimatedCollisionTime;
	}
};
///////////////////////////////////////////////////////////////////////////////
//Improve floating-point consistency
#pragma optimize( "p", on )
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
long CVODEStep(	vector<double>& state, double t_in, double t_out);
}
///////////////////////////////////////////////////////////////////////////////



long SolveIt::CVODEStep(vector<double>& state, double t_in, double t_out)
{
const double CVodeRTOL = 1.0e-8;
const double CVodeFLOOR = .01;
const double CVodeATOL = CVodeRTOL*CVodeFLOOR;
	double abstol=CVodeATOL;
	double reltol=CVodeRTOL;
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	long flag = CVODE::ERR_FAILURE;
	double t=t_in, tout=t_out;

//	std::wstring bbuf(g_szWorkDir.c_str());
//	bbuf += L"\\cvode_err.txt";
//	freopen( bbuf, "w", stdout );
//	struct {
//		long	length;
//		double	*data;
//	} X;
///////////////////////////////////////////////////////////////////////////////
	CSystem* m_system					= theApp.GetSystem();
	CSystem::SolveMethod m_SolveMethod	= m_system->m_SolveMethod;
//	vector<double> state				= theApp.GetRigidBodyManager()->m_system_state;
	long m_NumberVariables				= m_system->m_NumberVariables;
///////////////////////////////////////////////////////////////////////////////
//	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();

	long cnt = 0;
tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(m_NumberVariables, NULL);
	if (x->data == 0) return flag;
	x->length	= m_NumberVariables;
//	CopyMemory(x->data, X.data, X.length * sizeof(double) );
	CopyMemory(x->data, &state.front(), m_NumberVariables * sizeof(double) );
	t			= t_in;
	tout		= t_out;

	void *cvode_mem = CVODE::CVodeMalloc(x->length, CVodeDerivative, t, x, 
							CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) 0, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		if (m_SolveMethod == CSystem::direct)
			CVODE::CVDense(cvode_mem, NULL, NULL);
		else if (m_SolveMethod == CSystem::iterative)
			CVODE::CVSpgmr(cvode_mem, CVODE::NONE, CVODE::MODIFIED_GS, 0, 0.0, NULL, NULL, NULL);

		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else
	{
		CVODE::N_VFree(x);
		throw SolveIt_ERROR(L"SolveIt::CVODEStep:  CVODE::CVodeMalloc failed");
//		return (CVODE::N_VFree(x) , -1);
	}

	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(&state.front(), x->data, m_NumberVariables * sizeof(double) );
//		theApp.GetRigidBodyManager()->SetStateOfBodies(state, FALSE);// SetPreviousStateOfBodies done at top of loop
//		CopyMemory(X.data, x->data, X.length * sizeof(double) );
	}
	CVODE::N_VFree(x);
//	if (flag == CVODE::SUCCESS) t_in = t;

	if (flag != CVODE::SUCCESS)
	{
		reltol *= 10;
		abstol *= 10;
		if (cnt++<4) goto tryAgain;
	}

	return flag;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double CRigidBodyManager::RefineCollisionTime(double& t_in,double& t_out,double& timeMin,double& timeMax)
{
//	if (CRigidBody::m_boolSpheresOnly) return 0;
	SetToPreviousState();//calls  CRigidBodyManager::PQP_Update();
	long numberCollisions = NumberCollisions();
	ASSERT(0==numberCollisions);
///////////////////////////////////////////////////////////////////////////////
	CSystem* m_system	= theApp.GetSystem();
	long	iter	= m_system->m_lMaxIterationsForFindingTimeOfCollision;
	do
	{
		m_system_state.clear();
		GetStateOfBodies();
		vector<double> state	= m_system_state;
		if (CVODEStep(state, t_in, t_out) != CVODE::SUCCESS)
		{
			throw SolveIt_ERROR(L"ERROR: CVODEStep(t_in, t_out) != CVODE::SUCCESS");
		}
		SetStateOfBodies(state, FALSE);//do not UpdatePreviousState
	//	PQP_Update();
		numberCollisions = NumberCollisions();
		bool bIntersecting = numberCollisions>0;

		if (bIntersecting)
		{
			timeMax = t_out;
			if ( fabs(t_out - t_in) < m_system->m_fToleranceTimeOfCollision*m_system->m_dt )
			{
				m_system->m_fTimeOfLastCollision = 0.5*(t_in+t_out);
				m_system->m_vecCollisionTimes.push_back(m_system->m_fTimeOfLastCollision);

				if (m_system->examineCollisionTimes!=0 && //.get()
					IsWindow(m_system->examineCollisionTimes->m_hWnd))//.get()
					m_system->examineCollisionTimes->UpdateData();

				break;
			}
			SetToPreviousState();
		//	PQP_Update();moved to SetToPreviousState
		}
		else {
			timeMin = t_in;
			t_in = t_out;
			SetPreviousStateOfBodies();
		}
		t_out = 0.5*(t_in+timeMax);
	}
	while (--iter>0);
///////////////////////////////////////////////////////////////////////////////
	if (iter==0)
	{
		TimedMessageBox(NULL,L"Warning: Unable to determine collision time",L"CRigidBodyManager::RefineCollisionTime",MB_OK,1000);
	}
	return(0.5*(t_in+t_out));
}
///////////////////////////////////////////////////////////////////////////////
#if 10
long CRigidBodyManager::NumberCollisions(bool bBuildLists)
{
	if (CRigidBody::m_boolSpheresOnly) return ResolveSphereCollisions();
	long numCollisions = 0;
	if (empty()) return 0;
	if (m_mapInteractingPair.empty()) return 0;

	if (bBuildLists) ClearCollisionContactsLists();
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
	//	Contact* pc = dynamic_cast<Contact*>(i_mapInteractingPair->second);
	//	vector<Contact*>::iterator i_CollidingContactsList;
	//	for (i_CollidingContactsList = pc->m_CollidingContactsList.begin(); i_CollidingContactsList != pc->m_CollidingContactsList.end(); ++i_CollidingContactsList)
		{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
	//		Contact* ptr = *i_CollidingContactsList;
			if (ptr) 
			{
				if (ptr->IsCollision())
				{
					++numCollisions;
					if (bBuildLists)
					{
						m_CollidingContactsList.push_back(ptr);
						ptr->m_body1->AddContactPoint(ptr);
						ptr->m_body2->AddContactPoint(ptr);
					}
				}
			}
		}
	}
	return numCollisions;
}
//////////////////////////////////////////////////////////////////////
void CRigidBodyManager::GetChangeInNormalRelativeVelocity(vector<double>& nv, int nSize) {
	int i, j, row;
	vector<Contact*>::iterator i_CollidingContactsList;
	vector<double> b(nSize, 0.0);
	for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
			i_CollidingContactsList != m_CollidingContactsList.end();
			++i_CollidingContactsList)
	{
		row = ToIntDistance(m_CollidingContactsList.begin(), i_CollidingContactsList);
		Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
		Vector3D v = ptr->ChangeInNormalRelativeVelocityLab();
		for ( i=0, j=3*row; i<3; ++j, ++i)
			b[j] = v[i];
	}
	nv = b;
}
///////////////////////////////////////////////////////////////////////////////
// CRigidBodyManager
//solve for impulses
//one impulse per contact: J_A_i + J_B_i'= 0
///////////////////////////////////////////////////////////////////////////////
long CRigidBodyManager::ResolveSphereCollisions()
{
	if (!CRigidBody::m_boolSpheresOnly) return -1;
//const double m_MOTION_THRESHOLD		= 0.0005;
//const double m_PROXIMITY_TOLERANCE	= 0.05;
///////////////////////////////////////////////////////////////////////////////
	long numCollisions = 0;
	CRigidBody* ps1 = 0;
	CRigidBody* ps2 = 0;
///////////////////////////////////////////////////////////////////////////////
	ClearCollisionContactsLists();
	if (empty()) return 0;
	if (m_mapInteractingPair.empty()) return 0;

//	ClearCollisionContactsLists();
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
		Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
		if (ptr) 
		{
			ps1 = ptr->m_body1->type==T_SPHERE? ptr->m_body1:0;
			ps2 = ptr->m_body2->type==T_SPHERE? ptr->m_body2:0;
			ASSERT(	ps1);
			assert(		ps1);
			ASSERT(	ps2);
			assert(		ps2);
			Vector3D rel = ps1->x - ps2->x;
			double separationCM = rel.norm();
			double separation = separationCM - (ps1->m_fRadius + ps2->m_fRadius);
			bool bIntersecting = separation < 0.0;
			if (bIntersecting) ptr->m_ContactType = Contact::Penetrating;
			else
			{
				ptr->m_ContactType = Contact::Separated;
				Vector3D normal(rel);
				normal.normalize();
// closest points between bodies, in lab frame
				ptr->normal				= normal;
				ptr->m_pointContact1	= ps1->x - ps1->m_fRadius*normal;
				ptr->m_pointContact2	= ps2->x + ps2->m_fRadius*normal;
				if (ptr->m_bPermute) normal.negate();
			}
/*
		Vector3D v1 = ps1->ComputeVelocityOfPointInBody(cp1g);
		Vector3D v2 = ps2->ComputeVelocityOfPointInBody(cp2g);
		Vector3D relVel = v1 - v2;
		double uz = m_system->m_stepForwardOrBackward*relVel*normal;
*/	
			if (ptr->IsCollision())
			{
				++numCollisions;
				m_CollidingContactsList.push_back(ptr);
				ptr->m_body1->AddContactPoint(ptr);
				ptr->m_body2->AddContactPoint(ptr);
			}
		}
	}

	return numCollisions;
}
///////////////////////////////////////////////////////////////////////////////
long factorial(int n)
{
	static int ntop=4;
	static long a[33]={1,1,2,6,24};
	int j;
   
	if (n < 0) return 1;
	if (n >= sizeof(a)/sizeof(a[0])) return 1;
	while (ntop<n) {
		j=ntop++;
		a[ntop]=a[j]*ntop;
	}
	return a[n];
}
///////////////////////////////////////////////////////////////////////////////
unsigned long pow2(int n)
{
	static int ntop=8;
	static long a[33]={1,2,4,8,16,32,64,128,256};
	int j;
   
	if (n < 0) return 1;
	if (n >= sizeof(a)/sizeof(a[0])) return 1;
	while (ntop<n) {
		j=ntop++;
		a[ntop]=a[j]*2;
	}
	return a[n];
}
///////////////////////////////////////////////////////////////////////////////
struct Add
{  
	Vector3D operator()(Vector3D const& x, Vector3D const& y) const {
		return x+y;
	}
} addv;
///////////////////////////////////////////////////////////////////////////////
long CRigidBodyManager::ResolveCollisions()
{
try{
	if (empty()) return 0;
	if (m_mapInteractingPair.empty()) return 0;
//	if (CRigidBody::m_boolSpheresOnly) return ResolveSphereCollisions();
	long numCollisions = NumberCollisions();
	if (numCollisions == 0) return 0;
	CSystem* m_system	= theApp.GetSystem();

//	if (CRigidBody::m_boolSpheresOnly)
	{
	double t =	m_system->m_target_tout - m_system->m_dt*m_system->m_stepForwardOrBackward;
	double	t_in	= t;
	double	timeMin	= 0.99*t;
	double	timeMax	= 1.1*m_system->m__t;// + 2*m_system->m_dt;//safety factor for upper limit
	double	t_out	= 0.5*(t_in+timeMax);
	t = RefineCollisionTime( t_in, t_out, timeMin, timeMax);
	}


	numCollisions = NumberCollisions(true);
	if (numCollisions == 0) ASSERT(FALSE);
	if (numCollisions == 0) return 0;

	imsl_seed_rand(0);
	double kineticEnergy = ComputeKineticEnergy();
	double kineticEnergyBest = 0;

	int rep=0;
	//	ofstream ofs("out.c", ios::trunc);
		ofstream ofs("out.c", ios::app);

do {
	++rep;
	kineticEnergyBest = -kineticEnergy;
	long NumContacts = ToLongSize(m_CollidingContactsList.size());
	unsigned long bitPatternForContactBodySwap = pow2(NumContacts);//(unsigned long) pow(2,NumContacts);
	unsigned long bitPatternSelected = bitPatternForContactBodySwap;

	{
	//_variant_t v1(numCollisions);
	std::wstring bstr1= std::to_wstring(numCollisions); //v1;
	_variant_t v(NumContacts);
	std::wstring bstr = std::to_wstring(NumContacts); //v;
	std::wstring bstr2(bstr1);
	bstr2 += L": ";
	bstr2 += bstr;
	Fire_WriteStatus(bstr2.c_str());
	ofs << "Num Collisions/Contacts = " << GetC(bstr2) << endl;
	}

	if ( NumContacts == 1)
	{
		Contact* ptr = dynamic_cast<Contact*>(*m_CollidingContactsList.begin());
		if (ptr) 
		{
			if (ptr->IsCollision())
			{
				try
				{
					++numCollisions;
					ptr->ResolveCollisions(m_system->m__t);
				}
				catch( SolveIt_Error& e){e.why();}catch(...)
				{
					TimedMessageBox(NULL,_T("failed"),_T("CRigidBodyManager::ResolveCollisions"),MB_OK);
				}
			}
		}
		ComputeImpulses();
		ComputeMomentumChanges();
		PQP_Update();
		SetPreviousStateOfBodies();
//		return numCollisions;
	}
	else if ( NumContacts > 1)
	{
/*
//		TNT::Array2D<int> permuteRows(NumContacts, NumContacts);
e.g.,
permuteRows =
0 0 0 1
0 1 0 0
0 0 1 0
1 0 0 0

equivalent:
vector<int> permuteRows(NumContacts)=[4,2,3,1]
store the column# that is 1
	m[ptr] = 3*(permuteRows[ToIntDistance(m_CollidingContactsList.begin(), i_CollidingContactsList)]-1);

*/
		int j=0, k=0, rowOffset=0, colOffset=0, row=0, col=0;
		int nSize = 3*NumContacts;
/*
	typedef auto_ptr<double> AD;
	auto_ptr<AD> abuf( new AD[nSize] );
		for ( j=0;j<nSize;++j) {
			abuf[j] = new double[nSize];
		}
	double** a = abuf.get();//
	double** a = new double*[nSize];
		for ( j=0;j<nSize;++j) {
			a[j] = new double[nSize];
		}
*/

//		TNT::Array2D<double> a(nSize, nSize);


		double **a = CVODE::denalloc(nSize);
		if (a == NULL) throw SolveIt_ERROR(L"denalloc memory request failed");


		CVODE::integer *pivot = CVODE::denallocpiv(nSize);
		if (pivot == NULL) throw SolveIt_ERROR(L"denallocpiv memory request failed");
/*
		CVODE::integer ier = CVODE::gefa(a,nSize,pivot);
		if (ier > 0) throw SolveIt_ERROR(L"zero element encountered during");
		if (ier == 0) CVODE::gesl(a,nSize,pivot,b);
*/

		//auto_ptr<double> bbuf( new double[nSize] );
		std::unique_ptr<double> bbuf = std::make_unique<double>(nSize);
		//auto_ptr<double> fbuf( new double[nSize] );
		std::unique_ptr<double> fbuf = std::make_unique<double>(nSize);
		double* b = bbuf.get();
		double* f = fbuf.get();

	//	vector<double> b(nSize, 0.0);
	//	vector<double> f(nSize, 0.0);
		vector<Contact*>::iterator i_CollidingContactsList;
		map< Contact*, long > mapContactPtr_to_ContactPointNumber;//, &m=mapContactPtr_to_ContactPointNumber;

	//	vector<int> permuteRows(NumContacts);
		vector<int> vec_permuteRows;
	//	vec_permuteRows.reserve(NumContacts);
		vector<int> vec_permuteRowsSelected;

///////////////////////////////////////////////////////////////////////////////
		map< Contact*, long > mapContactPtr_to_PermutedContactPointNumber;
		for ( j=0;j<NumContacts;++j)
		{
			vec_permuteRows.push_back(j);//use identity transformation initially;store only the column# that is 1
		}
		vector<int>::iterator start	= vec_permuteRows.begin();
		vector<int>::iterator end	= vec_permuteRows.end();
		std::sort(start, end, less<int>());
///////////////////////////////////////////////////////////////////////////////
		//typedef ostream_iterator< int, wchar_t, char_traits<wchar_t> > IntOstreamIt;
		using IntOstreamIt  = ostream_iterator< int, wchar_t, char_traits<wchar_t> > ;
		std::wostringstream oss2;
		IntOstreamIt outIt(oss2, L" ") ;
///////////////////////////////////////////////////////////////////////////////
		double coefficientOfRestitution = 1;
		for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
				i_CollidingContactsList != m_CollidingContactsList.end();
				++i_CollidingContactsList)
		{
			Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
			int dist = ToIntDistance(m_CollidingContactsList.begin(), i_CollidingContactsList);
			mapContactPtr_to_ContactPointNumber[ptr] = dist;
			mapContactPtr_to_PermutedContactPointNumber[ptr] = dist;

			coefficientOfRestitution *= ptr->m_body1->m_fCoefficientOfRestitution;
			coefficientOfRestitution *= ptr->m_body2->m_fCoefficientOfRestitution;
		}
		ASSERT(coefficientOfRestitution >  0.0);
		ASSERT(coefficientOfRestitution <= 1.0);
		bool bSucceeded = false;
		long nSucceeded = -1;
		int fail=-1;
//		const long citer = 1024;
//		long iter = citer;
//		iter = citer;

		ofs << "full bitPattern = " << bitPatternForContactBodySwap-1 << endl;
		std::wostringstream oss;
///////////////////////////////////////////////////////////////////////////////
		vector<Vector3D> vec_Vector3D;
		vector<Vector3D> vec_Vector3DCompliment;
		vector<double> vec_VaryCompliment;
///////////////////////////////////////////////////////////////////////////////
		bool bSeekingCfg = true;
tryAgain:
	//	bSeekingCfg=false;
//calls  CRigidBodyManager::PQP_Update();
		SetToPreviousState();
//which invokes: if (m_bPermute) normal.negate();
		oss2.str(L"");
		oss2.clear();
		copy(start, end, outIt);
		oss2 << endl << bitPatternForContactBodySwap << endl << ends;
		ofs << GetC(oss2.str());

		for ( j=0;j<nSize;++j) {
			b[j]=0.0;
			f[j]=0.0;
		//	for ( k=0;k<nSize;++k)  a[j][k]=0.0;
		}
		CVODE::denzero(a,nSize);
///////////////////////////////////////////////////////////////////////////////
		vec_Vector3D.clear();
		vec_Vector3DCompliment.clear();
		vec_VaryCompliment.clear();
		for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
				i_CollidingContactsList != m_CollidingContactsList.end();
				++i_CollidingContactsList)
		{
			Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
			Vector3D v = ptr->ChangeInNormalRelativeVelocityLab();//depends on permutation
			vec_Vector3D.push_back(v);
		}
		for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
				i_CollidingContactsList != m_CollidingContactsList.end();
				++i_CollidingContactsList)
		{
			vector<Vector3D>::iterator i_start;
			vector<Vector3D>::iterator i_end;
			vector<Vector3D>::iterator i_v;
			Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
		//	rowOffset = mapContactPtr_to_PermutedContactPointNumber[ptr];
			rowOffset = mapContactPtr_to_ContactPointNumber[ptr];
			Vector3D v;
// find vec_Vector3D[rowOffset] or a numerical copy:
			i_v	= find(vec_Vector3D.begin(),vec_Vector3D.end(), vec_Vector3D[rowOffset]);
			if (i_v	== vec_Vector3D.begin())
			{
				i_start	= ++i_v;
				v = accumulate(i_start, vec_Vector3D.end(), ZERO_3D, addv);
			}
			else if (i_v == vec_Vector3D.end())
			{
				i_end	= --i_v;
				v = accumulate(vec_Vector3D.begin(), i_end, ZERO_3D, addv);
			}
			else
			{
				i_start	= ++i_v;
				i_end	= --i_v;
				v = accumulate(vec_Vector3D.begin(), i_end, ZERO_3D, addv) + accumulate(i_start, vec_Vector3D.end(), ZERO_3D, addv);
			}
	//		double guess = v*vec_Vector3D[rowOffset];
			vec_Vector3DCompliment.push_back(v);
		//	vec_VaryCompliment.push_back(-0.7513087);
			if (bSeekingCfg) vec_VaryCompliment.push_back(-1);
		}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//const double guess = -0.7513087;//OK, -1.00845e-006% error
		for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
				i_CollidingContactsList != m_CollidingContactsList.end();
				++i_CollidingContactsList)
		{
			Contact* ptr	= dynamic_cast<Contact*>(*i_CollidingContactsList);
			int Row			= mapContactPtr_to_PermutedContactPointNumber[ptr];
			rowOffset		= 3*Row;
			if (bSeekingCfg)
			{
				Vector3D& v = vec_Vector3D[Row];
				for ( row=0; row<3; ++row)
				{
					b[rowOffset+row] = v[row];
				}
			}
			else
			{
			//	Vector3D& v = vec_Vector3D[Row];
				Vector3D& vc = vec_Vector3DCompliment[Row];
				double& guess = vec_VaryCompliment[Row];
				Vector3D dv = ptr->ChangeInNormalRelativeVelocityLab(vc, guess);
				for ( row=0; row<3; ++row)
				{
					b[rowOffset+row] = dv[row];
				}
			}

			Point3D contactPointThisImpulse = ptr->m_pointContact();
			vector<Contact*>& contactPointList1 = ptr->m_body1->m_ContactPointList;
			vector<Contact*>& contactPointList2 = ptr->m_body2->m_ContactPointList;
			vector<Contact*>::iterator i_CollidingContactsList1;
			vector<Contact*>::iterator i_CollidingContactsList2;
	
			for (	i_CollidingContactsList1  = contactPointList1.begin();
					i_CollidingContactsList1 != contactPointList1.end();
					++i_CollidingContactsList1)
			{
				Contact* pc1 = dynamic_cast<Contact*>(*i_CollidingContactsList1);
				colOffset = 3*mapContactPtr_to_PermutedContactPointNumber[pc1];
				if (rowOffset == colOffset) ASSERT(pc1 == ptr);
				Mat3 M = ptr->m_body1->ComputeContactMatrix(contactPointThisImpulse, pc1->m_pointContact());
				for ( row=0; row<3; ++row) 
					for ( col=0; col<3; ++col)
					{
				//		a[rowOffset+row][colOffset+col]+=M.m[row][col];
						a[colOffset+col][rowOffset+row]+=M.m[row][col];// for CVODE
			//	oss << "M["<<rowOffset<<"][" << colOffset << "] = " << ptr->m_body1->name << endl << M << endl;
					}
			}
			for (	i_CollidingContactsList2  = contactPointList2.begin();
					i_CollidingContactsList2 != contactPointList2.end();
					++i_CollidingContactsList2)
			{
				Contact* pc2 = dynamic_cast<Contact*>(*i_CollidingContactsList2);
				colOffset = 3*mapContactPtr_to_PermutedContactPointNumber[pc2];
				if (rowOffset == colOffset) ASSERT(pc2 == ptr);
				Mat3 M = ptr->m_body2->ComputeContactMatrix(contactPointThisImpulse, pc2->m_pointContact());
				for ( row=0; row<3; ++row) 
					for ( col=0; col<3; ++col)
					{
					//	a[rowOffset+row][colOffset+col]+=M.m[row][col];
						a[colOffset+col][rowOffset+row]+=M.m[row][col];// for CVODE
			//	oss << "M["<<rowOffset<<"][" << colOffset << "] = " << ptr->m_body2->name << endl << M << endl;
					}
			}
		}

		bSucceeded = false;
		CVODE::integer ier = CVODE::gefa(a,nSize,pivot);
		if (ier > 0) throw SolveIt_ERROR(L"zero element encountered during");
		if (ier == 0) CVODE::gesl(a,nSize,pivot,b);
				bSucceeded = true;
				nSucceeded = 0;

/*
		oss.seekp(0);
	//	oss << m_system->m__t << endl; << endl << endl
//		oss << "b = " << b;
//		oss << "a = " << a;
	//	oss << "==========================" << endl << ends;

		if (nSucceeded==0 || !bSucceeded)
		{
			TNT::Array2D<double> L(nSize, nSize);
			fail = TNT::Cholesky_upper_factorization(a, L);
			ASSERT(!fail);
			if (fail == 0)
			{
// solve Ax =b, as L*L'x =b
//  let y=L'x, then
//   solve L y = b;
//   solve L'x = y;
				vector<double> y = TNT::Lower_triangular_solve(L, b);
				vector<double> F = TNT::Upper_triangular_solve(TNT::Transpose_view(L), y);
				f = F;
				bSucceeded = true;
				nSucceeded = 0;
			}
		}
		if (nSucceeded==1 || !bSucceeded)
		{
			ASSERT(0);
			vector<double> C(nSize), D(nSize);
			fail = TNT::QR_factor(a, C, D);
			ASSERT(!fail);
			if (fail == 0)
		    {
				vector<double> F(b);
				if (TNT::QR_solve(a, C, D, F) == 0) {
					f = F;
					bSucceeded = true;
					nSucceeded = 1;
				}
			}
		}
		if (nSucceeded==2 || !bSucceeded)
		{
			ASSERT(0);
			vector<TNT::Subscript> index(nSize);
			if (TNT::LU_factor(a, index) == 0)
			{
				vector<double> F(b);
				if (TNT::LU_solve(a, index, F) == 0) {
					f = F;
					bSucceeded = true;
					nSucceeded = 2;
				}
			}
		}
		if (nSucceeded==3 || !bSucceeded)
		{
			ASSERT(0);
			vector<double> F = TNT::linear_solve(TNT::Unit_lower_triangular_view(a), b);
			f = F;
			bSucceeded = true;
			nSucceeded = 3;
			TimedMessageBox(NULL,L"not reliable","CRigidBodyManager::ResolveCollisions",MB_OK,100);
		}



	if (nSucceeded==0 || !bSucceeded)
	{
	//	vector<double> B(nSize,b);
//		TNT::Array2D<double> T(nSize, nSize,(double*) a);
		int n = a.dim1();
	//	TNT::Array2D<double> L(n, n);
		TNT::Array2D<double> L;
		vector<TNT::Subscript> index(n);
		vector<double> C(n), D(n);
		JAMA::QR<double> qr(a);
		if (qr.isFullRank())
		{
		//	TNT::Array2D<double> B(nSize,1,(double*) a);
			vector<double> F = qr.solve(b);
		//	Vector3D F(f[0][0],f[1][0],f[2][0]);
				f = F[0];
				bSucceeded = true;
				nSucceeded = 0;
		}
#if defined(not_not_yet_implemented)
		JAMA::Cholesky<double> chol(T);
		if (chol.is_spd())
		{
			TNT::Array2D<double> b(3,1,(double*) cm.m);
			Array2D<double> f = chol.solve(b);
			L = chol.getL();
// solve Ax =b, as L*L'x =b
//  let y=L'x, then
//   solve L y = b;
//   solve L'x = y;
		//	vector<double> y = TNT::Lower_triangular_solve(L, b);
		//	vector<double> f = TNT::Upper_triangular_solve(TNT::Transpose_view(L), y);

			Vector3D F(f[0][0],f[1][0],f[2][0]);
			force = F;
			torque = rel.vectorProd(F);
			return;
		}
#endif
///////////////////////////////////////////////////////////////////////////////
		return 0;
	}

*/

		if (bSucceeded)
		{
			for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
					i_CollidingContactsList != m_CollidingContactsList.end();
					++i_CollidingContactsList)
			{
				Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
				rowOffset = 3*mapContactPtr_to_PermutedContactPointNumber[ptr];
				Vector3D& v = ptr->impulseLab;
				for ( row=0; row<3; ++row) v[row] = b[rowOffset+row];//f[rowOffset+row];
			}
			ComputeImpulses();
			ComputeMomentumChanges();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
			double kenergyTrial = ComputeKineticEnergy()/coefficientOfRestitution;
			if ( bSeekingCfg && (fabs(kineticEnergy - kenergyTrial) < fabs(kineticEnergy - kineticEnergyBest)) )
			{
				bitPatternSelected = bitPatternForContactBodySwap;
				kineticEnergyBest = kenergyTrial;
///////////////////////////////////////////////////////////////////////////////
				vec_permuteRowsSelected.clear();
				vec_permuteRowsSelected=vec_permuteRows;
				ASSERT(vec_permuteRowsSelected.size()==vec_permuteRows.size());
				ASSERT(vec_permuteRowsSelected[vec_permuteRowsSelected.size()-1]==vec_permuteRows[vec_permuteRows.size()-1]);
/*
				vector<int>::iterator i;
				for (i=vec_permuteRows.begin(); i!=vec_permuteRows.end(); ++i)
					vec_permuteRowsSelected.push_back(*i);
*/
			}
///////////////////////////////////////////////////////////////////////////////
#if 0
			vector<double> B = a*f - b;
			oss << (1.0 - kenergyTrial/kineticEnergy)*100 
				<< "% error; #=" << vec_VaryCompliment[0]
				<< "; KE*="<< kineticEnergyBest 
				<< "; bit*="<< bitPatternSelected 
				;//<< "; f = " << f;
//			oss << (1.0 - kenergyTrial/kineticEnergy)*100 << "% error; f = " << f;
//			oss << "a*f - b = " << B;
			oss << "==========================" << endl << ends;
			ofs << buf;// << endl;

#endif
			double tol = bSeekingCfg? 0.01:0.001;
			if ( fabs(kineticEnergy - kenergyTrial) > tol*kineticEnergy )
			{
				if (bSeekingCfg)
				{
					if (0 == --bitPatternForContactBodySwap)
					{
///////////////////////////////////////////////////////////////////////////////
//if all pairs of exchange [body1 with body2] have been exhausted:
//first try next row permutation:
						if ( next_permutation(start, end, less<int>()) )
						{
							bitPatternForContactBodySwap = pow2(NumContacts);//(unsigned long) pow(2,NumContacts);
							mapContactPtr_to_PermutedContactPointNumber.clear();
							unsigned long mask=1;
							for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
									i_CollidingContactsList != m_CollidingContactsList.end();
									++i_CollidingContactsList)
							{
								Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
								ptr->m_bPermute = bitPatternForContactBodySwap & mask? true:false;
								mask*=2;
								mapContactPtr_to_PermutedContactPointNumber[ptr] = vec_permuteRows[mapContactPtr_to_ContactPointNumber[ptr]];
							}
							goto tryAgain;
						}
///////////////////////////////////////////////////////////////////////////////
//if done, select best:
						else
						{
							if (bSeekingCfg)
							{
								bSeekingCfg						= false;
								vec_permuteRowsSelected[0]=0;
								vec_permuteRowsSelected[1]=1;
								bitPatternSelected	= 2;
								bitPatternForContactBodySwap	= bitPatternSelected;
								vec_permuteRows					= vec_permuteRowsSelected;
					//			goto tryAgain;
							}
						//	else return numCollisions;
///////////////////////////////////////////////////////////////////////////////
					//		ofs << "Never get here! Unable to Resolve Collisions" << endl;
					//		TimedMessageBox(NULL,L"Never get here! Unable to Resolve Collisions","CRigidBodyManager::ResolveCollisions",MB_OK,500);
					//		return numCollisions;
						}
						mapContactPtr_to_PermutedContactPointNumber.clear();
						for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
								i_CollidingContactsList != m_CollidingContactsList.end();
								++i_CollidingContactsList)
						{
							Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
							mapContactPtr_to_PermutedContactPointNumber[ptr] = vec_permuteRows[mapContactPtr_to_ContactPointNumber[ptr]];
						}
					//	goto tryAgain;
					}
///////////////////////////////////////////////////////////////////////////////
//exchange body1 with body2 for selected contacts:
					unsigned long mask=1;
					for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
								i_CollidingContactsList != m_CollidingContactsList.end();
								++i_CollidingContactsList)
					{
						Contact* ptr = dynamic_cast<Contact*>(*i_CollidingContactsList);
						ptr->m_bPermute = bitPatternForContactBodySwap & mask? true:false;
						mask*=2;
					}
///////////////////////////////////////////////////////////////////////////////
					goto tryAgain;
				}
				else
				{
					double stop = 0;
					for (	i_CollidingContactsList  = m_CollidingContactsList.begin();
							i_CollidingContactsList != m_CollidingContactsList.end();
							++i_CollidingContactsList)
					{
						Contact* ptr	= dynamic_cast<Contact*>(*i_CollidingContactsList);
						int Row			= mapContactPtr_to_PermutedContactPointNumber[ptr];
					//	rowOffset		= 3*Row;
					//	Vector3D v;
						vec_VaryCompliment[Row]	*= 0.998;
						stop += fabs(vec_VaryCompliment[Row]);
					}
///////////////////////////////////////////////////////////////////////////////
					if (stop < 0.1) return numCollisions;
					goto tryAgain;
				}
///////////////////////////////////////////////////////////////////////////////
			}
///////////////////////////////////////////////////////////////////////////////
			else
			{
				SetToPreviousState();
				ComputeImpulses();
				ComputeMomentumChanges();
				PQP_Update();
				SetPreviousStateOfBodies();
			}
///////////////////////////////////////////////////////////////////////////////
		}
		else
		{
			TimedMessageBox(NULL,L"Unable to Resolve Collisions",L"CRigidBodyManager::ResolveCollisions",MB_OK,1000);
			return numCollisions;
		}
	CVODE::denfreepiv(pivot);
	CVODE::denfree(a);
	}
	else
		return 0;
} while (numCollisions += NumberCollisions(true));//0);
	return numCollisions;
}
catch( SolveIt_Error& e){e.why();}catch(...){
}
		return 0;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D Contact::NormalRelativeVelocityLab()
{
	Vector3D contactPoint = 0.5*(m_pointContact1+m_pointContact2);
	Vector3D vel1 = m_body1->ComputeVelocityOfPointInBody(contactPoint);
	Vector3D vel2 = m_body2->ComputeVelocityOfPointInBody(contactPoint);
	Vector3D vecRelVelocityLab = vel1 - vel2;
	if (m_bPermute) vecRelVelocityLab.negate();
	double relativeVelocityZ = vecRelVelocityLab*normal;
	Vector3D vn(normal);
	vn *= relativeVelocityZ;
//	if (m_bPermute) vn.negate();
	return vn;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D Contact::ChangeInNormalRelativeVelocityLab(const Vector3D& v, double scale)
{
	Vector3D contactPoint = 0.5*(m_pointContact1+m_pointContact2);
	Vector3D vel1 = m_body1->ComputeVelocityOfPointInBody(contactPoint);
	Vector3D vel2 = m_body2->ComputeVelocityOfPointInBody(contactPoint);
	Vector3D vecRelVelocityLab = vel1 - vel2;
	if (m_bPermute) vecRelVelocityLab.negate();
	double relativeNormalVelocity = vecRelVelocityLab*normal;
	Vector3D vn(normal);
	double changeInNormalRelativeVelocity = -(1+coefficientOfRestitution()) * relativeNormalVelocity;
	vn *= changeInNormalRelativeVelocity;
	if (scale == 0.0) return vn;
	double vNormal = v*normal;
	Vector3D v_perp(v);
	v_perp -= vNormal*normal;
	v_perp *= scale;
	return vn+v_perp;
}
///////////////////////////////////////////////////////////////////////////////
long Contact::ResolveCollisions(double t)
{
	Vector3D contactPoint = 0.5*(m_pointContact1+m_pointContact2);
	m_pointContact1 = contactPoint;
	m_pointContact2 = contactPoint;
	ComputeRelativeVelocityLab();
	m_uz = m_vecRelVelocityLab*normal;
	ASSERT(m_uz <= 0.0);
	if (m_uz > 0.0) return 0;

	if (m_body1->m_fCoefficientOfKineticFriction==0.0 ||
		m_body2->m_fCoefficientOfKineticFriction==0.0)
	{
		double changeInRelativeVelocityZ = -(1+coefficientOfRestitution()) * m_uz;
//	double changeInRelativeVelocityZ = -2 * m_uz;
		ComputeCollisionMatrix();
		Mat3		iK;
		int			not_invertible	= iK.invert(K);
		ASSERT(not_invertible != 1);
		if (not_invertible == 1) throw SolveIt_ERROR(L"singular");
	
		impulseLab	= iK*normal;
		impulseLab	*= changeInRelativeVelocityZ;
	}
	else
	{
		ComputeChangeInRelativeVelocity();
		ComputeImpulse();
	}


/*
	m_body1->m_vecImpulseLab +=  impulseLab;
	m_body1->m_vecImpulseTorqueLab +=  (contactPoint-m_body1->x)^impulseLab;

	impulseLab.negate();
	m_body2->m_vecImpulseLab += impulseLab;
	m_body2->m_vecImpulseTorqueLab += (contactPoint-m_body2->x)^impulseLab;

	m_body1->ComputeMomentumChanges();
	m_body2->ComputeMomentumChanges();
	PQP_Update();

	m_body1->m_vecImpulseLab			= ZERO_3D;
	m_body1->m_vecImpulseTorqueLab		= ZERO_3D;
	m_body2->m_vecImpulseLab			= ZERO_3D;
	m_body2->m_vecImpulseTorqueLab		= ZERO_3D;
*/
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
#endif

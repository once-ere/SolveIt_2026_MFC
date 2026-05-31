//MomentumChanges.cpp

#include "stdafx.h"
#include "SolveIt.h"

#include "RigidBodyManager.h"

#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ComputeImpulses() {
	if (empty()) return;
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ComputeImpulses();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ComputeMomentumChanges() {
	if (empty()) return;
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ComputeMomentumChanges();
	}
//return;
#if 0
///////////////////////////////////////////////////////////////////////////////
//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, rigidBodyManager));
	CConstraintManager* c = pThis->constraintManager;
	if (c.m_vecBodies.empty()) return;
	c.AssignVarsToColumns(true);//sets m_NumberOfIndependentVariables, m_vecSystemInverseMassMatrix, m_vecSystemVelocity
	long nVar = c.m_NumberOfIndependentVariables;
	long numConstraints = c.m_vecUniqueConstraints.size();

	TNT::Matrix<double> Jac;
	c.Jacobian(pThis->m__t, Jac, numConstraints);
	TNT::Matrix<double> tJ=TNT::transpose(Jac);

	TNT::Vector<double> V;
	c.GetSystemVelocity(V);


	DATA_SVD_mp_NUMCONSTRAINTS	= nVar;
	double (*A)[SVD_NMAX]		= DATA_SVD_mp_A_;
	double* B					= DATA_SVD_mp_B_;
	double* contactforce		= DATA_SVD_mp_CONTACTFORCE;
///////////////////////////////////////////////////////////////////////////////
	TNT::Subscript I, J;
	long i,j;
///////////////////////////////////////////////////////////////////////////////
	for(i=0; i<SVD_NMAX; i++)
	{
		B[i] = 0;
		for(j=0; j<SVD_NMAX; j++) A[i][j] = 0;
	}
///////////////////////////////////////////////////////////////////////////////
	for(I=0; I<nVar; I++) for(J=0; J<numConstraints; J++) A[long(I)][long(J)] = tJ[I][J];
///////////////////////////////////////////////////////////////////////////////
	DATA_SVD_mp_MY_SVD();// A is singular
///////////////////////////////////////////////////////////////////////////////
	for(J=0; J<numConstraints; J++)
	{
		j = long(J);
		if (j) B[j-1] = 0;
		B[j] = 1;
		DATA_SVD_mp_MY_SVBKSB();
		for(I=0; I<nVar; I++)
		{
			i = long(I);
			tJ[I][J] =contactforce[i];
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	TNT::Matrix<double> T = tJ*Jac;
///////////////////////////////////////////////////////////////////////////////
	TNT::Matrix<double> Id(nVar,nVar);
	{
        for (I=0; I<nVar; I++)
            for (J=0; J<nVar; J++)
                Id[I][J] = I==J? 1:0;
	}
	TNT::Matrix<double> P = Id - T;
	
	TNT::Vector<double> V_perp = P*V;

	TNT::Vector<double> b1 = Jac*V_perp;
	for(J=0; J<numConstraints; J++)
	{
		double chk = b1[J];
	}

	vector<CConstraint*>::iterator it;
///////////////////////////////////////////////////////////////////////////////
	for (it = c.m_vecUniqueConstraints.begin(); it != c.m_vecUniqueConstraints.end(); ++it)
	{
		CConstraint* pc = dynamic_cast<CConstraint*>(*it);
		if (!pc) continue;
		CPointToPointConstraint* ppmc = dynamic_cast<CPointToPointConstraint*>(pc);
		if (ppmc)
		{
			CRigidBody* body1 = ppmc->body1;
			CRigidBody* body2 = ppmc->body2;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//			Mat3 S = GetS(m_ptContactPoint);
			if (body1 && body1->m_bConstraintForceShouldBeUpdated && body1 == ppmc->body1) {
				BodyCompareEqual bodyCompareEqual(body1);
				vector<CRigidBody*>::iterator i = find_if(
													c.m_vecBodiesCurrentTimeStep.begin(),
													c.m_vecBodiesCurrentTimeStep.end(),
													bodyCompareEqual);
				if (i != c.m_vecBodiesCurrentTimeStep.end())
				{
					long n = 3*distance(c.m_vecBodiesCurrentTimeStep.begin(), i);
					Vector3D vel(V_perp[n+0],V_perp[n+1],V_perp[n+2]);

					Point3D pt1 = body1->LocalToLab(ppmc->point1);
					Mat3 S = body1->GetS(pt1);
					Vector3D w = body1->GetAngularVelocity();

					Vector3D v = vel + S*w;
					body1->m_momentumCM = v/body1->m_fMassInverse;
					body1->m_bConstraintForceShouldBeUpdated = false;
				}

			}
			else
			if (body2 && body2->m_bConstraintForceShouldBeUpdated && body2 == ppmc->body2) {
				BodyCompareEqual bodyCompareEqual(body2);
				vector<CRigidBody*>::iterator i = find_if(
													c.m_vecBodiesCurrentTimeStep.begin(),
													c.m_vecBodiesCurrentTimeStep.end(),
													bodyCompareEqual);
				if (i != c.m_vecBodiesCurrentTimeStep.end())
				{
					long n = 3*distance(c.m_vecBodiesCurrentTimeStep.begin(), i);
					Vector3D vel(V_perp[n+0],V_perp[n+1],V_perp[n+2]);

					Point3D pt2 = body2->LocalToLab(ppmc->point2);
					Mat3 S = body2->GetS(pt2);
					Vector3D w = body2->GetAngularVelocity();

					Vector3D v = vel + S*w;
					body2->m_momentumCM = v/body2->m_fMassInverse;
					body2->m_bConstraintForceShouldBeUpdated = false;
				}

			}
		}
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////

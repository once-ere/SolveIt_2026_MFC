// PulleyConstraint.cpp: implementation of the CPulleyConstraint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PulleyConstraint.h"
#include "RigidBodyManager.h"
//#include "System.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPulleyConstraint::CPulleyConstraint(
									CRigidBody* const leftBody,
									CRigidBody* const pulley,
									CRigidBody* const rightBody,
									int nLeftBodyConnectedInQuadrant,
									int nRightBodyConnectedInQuadrant
									) :
	m_leftBody(leftBody),
	m_pulley(pulley),
	m_rightBody(rightBody),
	m_nLeftBodyConnectedInQuadrant(nLeftBodyConnectedInQuadrant),
	m_nRightBodyConnectedInQuadrant(nRightBodyConnectedInQuadrant)
{
	if (leftBody)
	{
		leftBody ->InitializePulleyConnection(pulley->m_fRadius, nLeftBodyConnectedInQuadrant );
	//	leftBody ->set_DisableSelect(true);
		const double &R=m_pulley->m_fRadius;
		leftBody->SetPulleyContactPoint(R);
	}
	if (rightBody)
	{
		rightBody->InitializePulleyConnection(pulley->m_fRadius, nRightBodyConnectedInQuadrant);
	//	rightBody->set_DisableSelect(true);
		const double &R=m_pulley->m_fRadius;
		rightBody->SetPulleyContactPoint(R);
	}
	if (m_pulley)
	{
		m_pulley->set_DisableSelect(true);
	}
}

CPulleyConstraint::~CPulleyConstraint()
{

}

void CPulleyConstraint::DrawState()
{
}


void CPulleyConstraint::Error(double t, vector<double>& vecError)
{
	vecError.push_back(0);
}

void CPulleyConstraint::Jacobian(	double t,
					vector<double>& vecRowOfJacobian
					)
{
}

void CPulleyConstraint::NormalizedJacobian(	double t,
					vector<double>& vecRowOfJacobian
					)
{
}


void CPulleyConstraint::HessianDotVelocity(double t,
							vector<double>& vecRowOfHessianDotVelocity
							)
{

}


void CPulleyConstraint::AddConstraintForce(vector<double>& f, bool bDuringCollision )
{

}

void CPulleyConstraint::AddConstraintImpulse(vector<double>& f)
{

}


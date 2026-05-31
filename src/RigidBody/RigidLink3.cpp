// RigidLink3.cpp : Implementation of CRigidBody

#include "stdafx.h"
#include "SolveIt.h"

#include "RigidBodyManager.h"
#include "RigidBody.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
You cannot change the precedence or grouping of an operator, nor can you change the numbers of operands it accepts
*************************WARNING*************************
*************************WARNING*************************
	m_linkA^B+m_linkC == m_linkA^(B+m_linkC)
*************************WARNING*************************
*************************WARNING*************************
*/
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::updatePoses()
{
	if (CRigidBody::m_baseJoint==0) return;
	if (CRigidBody::m_baseJoint->outboundLink==0) return;
	CRigidBody::m_baseJoint->outboundLink->updatePoses();
	Fire_ViewChange();
/*
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	const double* const & sv = &RigidLinkJointStates.front();
try
{
	while (pb)
	{
		pb->updatePose(sv);
		if (pb->outbounds.size()>0)	pb = pb->outbounds[0];
		else pb = 0;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	m_virtualBodyManager->Stop();
}
*/
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// forward dynamics
void CRigidBodyManager::ForwardDynamics(const double sv[], SpatialMat &Iprop, SpatialVect &Pprop)
{
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	if (pb==0) return;
	pb->ForwardDynamics(sv, Iprop, Pprop);
}
///////////////////////////////////////////////////////////////////////////////
//error if not all 1 DOF
void CRigidBodyManager::compXf(const double sv[])
{
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	if (pb==0) return;
	CRigidBody::m_baseJoint->outboundLink->Xbi = MatX::ID;
	pb->compXf(sv);
//	CRigidBody::m_baseJoint->outboundLink->m_linkV = Vector3D::ZERO;
//	CRigidBody::m_baseJoint->outboundLink->m_linkW = Vector3D::ZERO;
}
//error if not all 1 DOF
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::InitializeLinkIAndZs()
{
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	if (pb==0) return;
	pb->InitIandZ();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ComputeLinkAbsoluteVelocities(const double sv[])
{
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	if (pb==0) return;
	CRigidBody::m_baseJoint->outboundLink->m_linkV = ZERO_3D;
	CRigidBody::m_baseJoint->outboundLink->m_linkW = ZERO_3D;
	pb->compAbsoluteVels(sv);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Initialize()
{
	m_pSelectedRigidBody		= 0;
	m_bShowContacts				= false;
	m_bShowClosestContacts		= false;
	this->Clear();
	CRigidBody::m_baseJoint=0;
	CRigidBody::m_LastLink=0;
	RigidLinkJointStates.clear();
	RigidLinkJointStatesInitial.clear();
	JointForcesTorques.clear();
	Joint1DOFAccelerations.clear();
	partialJoint1DOFAccelerations.clear();
	partialVect6.clear();
	map_isIs.clear();
}
///////////////////////////////////////////////////////////////////////////////

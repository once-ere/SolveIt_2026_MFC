// Joint1DOF.h: interface for the Joint1DOF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINT1DOF_H__F16FE549_15D1_47E9_B328_A0BB85162327__INCLUDED_)
#define AFX_JOINT1DOF_H__F16FE549_15D1_47E9_B328_A0BB85162327__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

#include "SixDim.h"
namespace SolveIt {
///////////////////////////////////////////////////////////////////////////////
class CRigidBody;
//typedef CRigidBody Link;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct pair<Point3D, Point3D> PairPoint3D;
typedef list<PairPoint3D> PairPoint3DList;
typedef vector<PairPoint3D> PairPoint3DVector;
typedef struct std::pair<double, double> PAIR_springConstant_springLength;
typedef vector<PAIR_springConstant_springLength> ListOf_springConstants_springLengths;
///////////////////////////////////////////////////////////////////////////////

class Joint1DOF  
{
public:
	virtual ~Joint1DOF();
	enum JointType {
					NONE,
					REV, // revolute				(1 DOF)
					PRI, // prismatic				(1 DOF)
					SCR, // screw					(1 DOF)
					CYL, // cylindrical				(2 DOF)
					UNI, // universal				(3 DOF)
					SPH, // spherical				(3 DOF)
					FLT, // floating				(6 DOF)
					SPRING, // spring				(3 DOF)
					FIX // fixed attachement (0 DOF)
	} jointType;
	Component1DOF displacement, velocity;
	Component1DOF displacement0, velocity0;
	Vector3D v;			//velocity;
	Vector3D w;			//angular velocity;
	Vect6 S;			// spatial axis
	Vect6 acc;			// spatial acceleration

	CRigidBody* rigidly_attached_to_me;
	CRigidBody* outboundLink;

	Joint1DOF () : jointType(NONE), rigidly_attached_to_me(0), outboundLink(0) {
		displacement.angle=velocity.angle=displacement0.angle=velocity0.angle=0;
	}
	Joint1DOF (const Joint1DOF& r) : rigidly_attached_to_me(r.rigidly_attached_to_me), outboundLink(r.outboundLink), //nextLink(r.nextLink), //RBConfig(r), 
		v(r.v), w(r.w),
		S(r.S), acc(r.acc) {
		displacement0.angle=r.displacement0.angle;
		displacement.angle=r.displacement.angle;
		velocity0.angle=r.velocity0.angle;
		velocity.angle=r.velocity.angle;
	}
	Joint1DOF& operator=(const Joint1DOF& r)
	{
		rigidly_attached_to_me = r.rigidly_attached_to_me;
		outboundLink = r.outboundLink;
//		RBConfig::operator=(*static_cast<RBConfig*>(const_cast<Joint1DOF*>(&r)));
		v=r.v, w=r.w;
		acc=r.acc, S=r.S;
		displacement0.angle=r.displacement0.angle;
		displacement.angle=r.displacement.angle;
		velocity0.angle=r.velocity0.angle;
		velocity.angle=r.velocity.angle;
		return *this;
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void computeAbsoluteJointVelocity(const double state[]);
///////////////////////////////////////////////////////////////////////////////
	void spatJointAcc(const double& qdd);//in outboundLink's FRAME
///////////////////////////////////////////////////////////////////////////////
	MatX TransformAndComputeVelocity (const double*q);//const MatX& m, 
///////////////////////////////////////////////////////////////////////////////
	void SetJoint1DOF(	CRigidBody* rigidlyAttached_to_me, JointType jt, 
						const Vector3D &Position, const Vector3D &Axis,
						Component1DOF displacement, Component1DOF velocity,
			bool bAbsolute=false);//these are Absolute [wrt Lab origin] or Relative displacements?
///////////////////////////////////////////////////////////////////////////////
	void Reset() {
	//	axis					= axis0;
	//	position				= position0;
		displacement.angle		= displacement0.angle;
		velocity.angle			= velocity0.angle;
	}

};

} // namespace SolveIt

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_JOINT1DOF_H__F16FE549_15D1_47E9_B328_A0BB85162327__INCLUDED_)

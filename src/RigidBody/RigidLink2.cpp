// RigidLink2.cpp : Implementation of CRigidBody

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
You cannot change the precedence or grouping of an operator, nor can you change the numbers of operands it accepts
*************************WARNING*************************
*************************WARNING*************************
	A.vectorProd()B+C == A.vectorProd()(B+C)
*************************WARNING*************************
*************************WARNING*************************
*/

//vector<double>& _state,
//						map<long, double>& Joint1DOFAccelerations,
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void xformSpatVect(const MatX &X, const Vect6 &u, Vect6 &v);
void invXformSpatVect(const MatX &X, const Vect6 &u, Vect6 &v);
void xformSpatInertia(const MatX &X, const Mat6 &Ia, Mat6 &Ib);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
¦_¦¦¯aßGpSsµtFTOd8fen=±==()÷˜°··vn²¦ 

e'_ß = e_µ R¯µ_ß  = Mat3::ID

X = e_µ x¯µ = e'_ß x'¯ß = e_µ R¯µ_ß x'¯ß

x¯µ		= R¯µ_ß x'¯ß

x_µ		= R_µß  x'_ß

x'_µ	= R_ßµ  x_ß = x_ß R_ßµ 

x'_ß	= R_µß  x_µ

e'_ß x'_ß	= e_µ R_µß  R_µ'ß  x_µ'

= e_µ x_µ 
	MatX Xij;				 // xform: inboard link to joint coords 
	MatX Xjo;				 // xform: joint to outboard link coords 
//	MatX X = jointFrame(inLoc, inAxis, inRef);
//	Xij.invert(X);
//	Xjo = jointFrame(outLoc, outAxis, outRef);
*/

// e1' = R e1
// e2' = R e2
// e3' = R e3
// R = e1' @ ~e1 + e2' @ ~e2 + e3' @ ~e3; @ = tensor product, ~ = tilde =  transpose
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::compXf(const double sv[])
{
	if (!sv) return;
	try
	{
		const double &q = sv[svIdx];
		if (_isnan( q )) throw SolveIt_ERROR(L"-33");
	
		if (m_inboundJoint.jointType == Joint1DOF::PRI) {
			Xf.R = Mat3::ID;
m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass = parentLinkToThisLink->GetThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(q, m_jointAxis);
		}
		else if (m_inboundJoint.jointType == Joint1DOF::REV) {
			Xf.R.set( m_jointAxis, -q);
m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass = parentLinkToThisLink->GetThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(Xf.R);
		}
		Xf.d = -m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass;
		if (m_parentLink==0) {
			if (m_inboundJoint.jointType == Joint1DOF::FLT) Xbi.set(((RBConfig *) &sv[svIdx])->Tbi);
			else {
				Xb.invert(Xf);
				Xbi = Xb;// xform:	link -> inertial
			}
		}
		else {
			Xb.invert(Xf);
			Xbi = m_parentLink->Xbi*Xb;//Xbi = inboardXbi*Xb;// xform:	link -> inertial
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		//##m_virtualBodyManager->Stop();
		throw SolveIt_ERROR(L"-333");
	}
	vector<Link *>::iterator lpi;
	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi) (*lpi)->compXf(sv); 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Joint1DOF::spatJointAcc(const double& qdd) {//in outboundLink's FRAME
try
{
	if (_isnan( qdd )) throw SolveIt_ERROR(L"NAN");

	const Vector3D& m_jointAxis = outboundLink->m_jointAxis;
	const Vector3D& m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin = outboundLink->m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin;
	switch(jointType) {
		case Joint1DOF::PRI:
			acc.a = ZERO_3D;
			acc.b = qdd*m_jointAxis;
			break;
		case Joint1DOF::REV:
			acc.a = qdd*m_jointAxis;
			acc.b = qdd*m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin;
			break;
		case Joint1DOF::FLT:
			acc.a = acc.b = ZERO_3D;
			break;
		default:
			acc.a = acc.b = ZERO_3D;
			break;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	//##m_virtualBodyManager->Stop();
	throw SolveIt_ERROR(L"-911");
}
}
///////////////////////////////////////////////////////////////////////////////
void Joint1DOF::computeAbsoluteJointVelocity(const double state[])
{
	if (!state) return;
try
{
	if (!outboundLink) throw SolveIt_ERROR(L"-33");
	const int& svIdx = outboundLink->svIdx;
	const double &Q = state[svIdx];
	if (_isnan( Q )) throw SolveIt_ERROR(L"-22");
// joint velocity for 1 dof joints...
	const double &qd = state[svIdx + 1];
	if (_isnan( qd )) throw SolveIt_ERROR(L"-11");

	const Vector3D& m_jointAxis = outboundLink->m_jointAxis;
	const Vector3D& m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin = outboundLink->m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin;
	switch(jointType) {
		case Joint1DOF::PRI:
			w = ZERO_3D;
			v = qd*m_jointAxis;
			break;
		case Joint1DOF::REV:
			w = qd*m_jointAxis;
			v = qd*m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin;
			break;
		case Joint1DOF::FLT:
			w = ((RBConfig *) &state[svIdx])->w;
			((RBConfig *) &state[svIdx])->Tbi.inverseTransform(((RBConfig *) &state[svIdx])->v, v);//invXformVect
			break;
		default:
			w = v = ZERO_3D;
			break;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	//##m_virtualBodyManager->Stop();
	throw SolveIt_ERROR(L"-911");
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	Compute linkV and linkW given the current joint positions and
//	velocities. linkV (resp. linkW) is the absolute linear
//	(resp. angular) velocity (i.e. relative to the inertial frame),
//	but it's expressed in the link frame. If compCor is true, compute
//	the Coriolis vector as well.	This routine assumes compXf called and
//	the link transform Xf is current.
void CRigidBody::compAbsoluteVels(const double sv[]) 
{
	if (!sv) return;
try
{
	const double &Q = sv[svIdx];
	if (_isnan( Q )) throw SolveIt_ERROR(L"-33");
// joint velocity for 1 dof joints...
	const double &qd = sv[svIdx + 1];
	if (_isnan( qd )) throw SolveIt_ERROR(L"-33");

	m_inboundJoint.computeAbsoluteJointVelocity(sv);
	m_linkW = m_inboundJoint.w;
	m_linkV = m_inboundJoint.v;

	if (m_parentLink==0) { 
		switch(m_inboundJoint.jointType) {
		case Joint1DOF::PRI:
			coriolis = Vect6::ZERO;
			break;
		case Joint1DOF::REV:
			coriolis = Vect6(ZERO_3D, (qd*qd)*m_ThisLinkCenterOfMassWithRespectToJointOriginPerpJointAxis);
			break;
		case Joint1DOF::FLT:
// don't compute coriolis if joint 0 is floating since it's not used 
			break;
		}
	}
	else { // not base link
// r = vector from inbrd link origin to outbrd (this) link origin, in outbrd (this) link's coords
	//	const Vector3D& r = -Xf.trans();
		const Vector3D& r = m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass;

		const Vector3D w = Xf * m_parentLink->m_linkW;
		const Vector3D v = Xf * m_parentLink->m_linkV;
		const Vector3D wxr = w.vectorProd(r);
		m_linkW += w;
		m_linkV += v + wxr;

		switch(m_inboundJoint.jointType) {
		case Joint1DOF::PRI:
			coriolis = Vect6(ZERO_3D, w.vectorProd((wxr + (2*qd)*m_jointAxis)));
			break;
		case Joint1DOF::REV:
			coriolis = Vect6( qd*(w.vectorProd(m_jointAxis)), (w.vectorProd(( wxr + (2*qd)*m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin ))) + (qd*qd)*m_ThisLinkCenterOfMassWithRespectToJointOriginPerpJointAxis);
			break;
		}
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	//##m_virtualBodyManager->Stop();
	throw SolveIt_ERROR(L"-333");
}
	vector<Link *>::iterator lpi;
	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi) (*lpi)->compAbsoluteVels(sv); 
}
///////////////////////////////////////////////////////////////////////////////
// Compute spatial quantities needed for the featherstone algo.
// Assumes the CRigidBody's articulated body inertia m_linkI is already computed
// (N.m12. this is spatially symmetric).	The function computes Is, sIs,
// and m_IssI, where s is the spatial vector for the inboard joint to
// the link.	Is and sIs are members of class link; m_IssI is returned
// via the parameter.

void CRigidBody::compSpatialQuants()
{
		Is = m_linkI*m_inboundJoint.S;
		m_IssI = Mat6(Is, Is.xpose());
		double isIs = m_inboundJoint.S^Is;//==m_inboundJoint.S.xpose()*Is;
		sIs  = 1.0/isIs;
	//	m_rigidBodyManager->map_isIs[svIdx] = sIs;
}
///////////////////////////////////////////////////////////////////////////////
	//	Mat3 I, Iinv = R*m_mat3InitialInverseInertiaTensor*Xbi.R;
	//	get_InverseInertiaTensor( Iinv );
	//	I.invert( R*Iinv*Xbi.R );
void CRigidBody::InitIandZ()//const double sv[])
{
	try
	{
		double mass=0;
		get_Mass( mass );
	//	Mat3 I;// = m_linkI.m21;
	//	I.invert(m_mat3InitialInverseInertiaTensor);
	//	m_linkI = Mat6(Mat3( mass ), I);
		m_linkI.m11 = m_linkI.m22 = Mat3::ZERO;
		m_linkI.m12 = Mat3( mass );//.set(Vector3D(mass, mass, mass), ZERO_3D);
		m_linkI.m21.set(_J, ZERO_3D);
	//	double& g	= m_rigidBodyManager->theSystem.theFieldsManager.m_ExternalGravitationalFieldY;
		double g	= -9.81;
// xform:	link -> inertial: Xbi.R
		Mat3 R = ~Xbi.R;// xform:	inertial -> link
		Vector3D gee = R*J_3D;//	Xbi.invXformVect(J_3D, gee);
		m_linkZ = Vect6( -(mass*g)*gee, m_linkW .vectorProd( (m_linkI.m21*m_linkW)));
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
		//##m_virtualBodyManager->Stop();
		throw SolveIt_ERROR(L"-444");;
	}
	vector<Link *>::iterator lpi;
	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi) (*lpi)->InitIandZ(); 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
ForwardDynamics is called after
	CRigidBody::m_baseJoint->outboundLink->compXf(sv);
	CRigidBody::m_baseJoint->outboundLink->compAbsoluteVels(sv);
	CRigidBody::m_baseJoint->outboundLink->InitIandZ();
///////////////////////////////////////////////////////////////////////////////
#define FOR_EACH(iterator) \
  for(iterator = outbounds.begin(); iterator != outbounds.end(); ++iterator)
*/
void CRigidBody::ForwardDynamics(const double sv[], Mat6 &parentLinkI, Vect6 &parentLinkZ)
{
	if (!sv) return;
	vector<Link *>::iterator lpi;
//	Vect6 childZ;	// z.m11. force
//	Mat6 childI;	// spatial inertia
try
{
	const double &Q = sv[svIdx];
	if (_isnan( Q )) throw SolveIt_ERROR(L"-33");
	const double &qd = sv[svIdx + 1];
	if (_isnan( qd )) throw SolveIt_ERROR(L"-33");
// propagate back results from childs
	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi) 
	{
	//	(*lpi)->ForwardDynamics(sv, childI, childZ);
	//	m_linkI+=childI;
	//	m_linkZ+=childZ;
		(*lpi)->ForwardDynamics(sv, m_linkI, m_linkZ);
	}

	if (m_parentLink==0) {	// compute spatial acceleration acc of link
		if (m_inboundJoint.jointType == Joint1DOF::FLT) {
			Mat6 Ii;
			Ii.inertiaInvert(m_linkI);
			acc=Ii*m_linkZ;//	Ii.xform(P, acc);
			acc.negate();
		}
		else {
			if (m_computedTorque) {//spatJointAcc(m_qddDes);
				m_inboundJoint.spatJointAcc(m_qddDes);
				acc = m_inboundJoint.acc;
			}
			else {
				compSpatialQuants();
			//	m_torque = m_tau - m_Ks * (Q - m_Ls) - m_Kd * qd	- (Is.vectorProd()coriolis) - (m_inboundJoint.S.vectorProd()m_linkZ);
				double Is_coriolis = Is^coriolis;
				double m_inboundJointS_m_linkZ = m_inboundJoint.S^m_linkZ;
				m_torque = m_tau - m_Ks * (Q - m_Ls) - m_Kd * qd	- (Is_coriolis) - (m_inboundJointS_m_linkZ);
				m_inboundJoint.spatJointAcc(m_torque * sIs);
				acc = m_inboundJoint.acc;
			}
			acc+=coriolis;
		}
	}
// not base link:	compute parentLinkI and parentLinkZ for link
	else { 
		compSpatialQuants(); // also sets Is, sIs
		Mat6 tmpI;
		if (m_computedTorque) tmpI = m_linkI;
		else {
			tmpI = m_linkI - sIs*m_IssI;//	tmpI.sub(I, tmpI);
		}
		parentLinkI += Xb*tmpI;//	xformSpatInertia(Xb, tmpI, parentLinkI);

		Vect6 tmpV;
		if (m_computedTorque) tmpV = Is*m_qddDes;
		else {
			m_torque = m_tau - m_Ks * (Q - m_Ls) - m_Kd * qd	- (Is^coriolis) - (m_inboundJoint.S^m_linkZ);
			tmpV = (m_torque * sIs)*Is;
		}
		parentLinkZ += Xb*(tmpV + m_linkZ + m_linkI*coriolis);
	//	Vect6 Ic = m_linkI*coriolis;//I.xform(coriolis, Ic);
	//	tmpV += Ic;
	//	tmpV += m_linkZ;
	//	parentLinkZ = Xb*tmpV;//	xformSpatVect(Xb, tmpV, parentLinkZ);
	}

}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	//##m_virtualBodyManager->Stop();
}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::featherstonePass2(
	const double sv[],	// state vector
	double dv[],				// derivative vector
	const Vect6 &inboardAcc)
{
	if (!sv) return;
	double *qdd = dv + svIdx + 1;
	vector<Link *>::iterator lpi;
	Quat qdot;
	Vect6 v;

	if (m_parentLink==0) {
		if (m_inboundJoint.jointType == Joint1DOF::FLT) {
// quaternion derivatives computed from quaternion & angular velocity
// position derivatives specifed by velocity state
			qdot.deriv( ((RBConfig *) &sv[svIdx])->Tbi.rot(), 
						((RBConfig *) &sv[svIdx])->w);
			((RBConfig *) &dv[svIdx])->Tbi.set(qdot, ((RBConfig *) &sv[svIdx])->v);
// lin. and ang. velocity derivatives specified by base's spatial acc.xformVect
			((RBConfig *) &sv[svIdx])->Tbi.transform(acc.a, ((RBConfig *) &dv[svIdx])->v);
			((RBConfig *) &dv[svIdx])->w = acc.b;
		}
		else {
			dv[svIdx] = sv[svIdx + 1];
			if (m_computedTorque) *qdd = m_qddDes;
			else *qdd = m_torque * sIs;
		}
	}
// not base link
	else {
		dv[svIdx] = sv[svIdx + 1];
	//	xformSpatVect(Xf, inboardAcc, v);
		v = Xf*inboardAcc;
		if (m_computedTorque) *qdd = m_qddDes;
		else *qdd = (m_torque - (Is^v)) * sIs;
// We could skip this code for MB leaves if we only cared about
// forward dynamics, but things like contact analysis require the
// acc field of the leaves, so we do it.
	//	spatJointAcc(*qdd);	// computes acc for link
		m_inboundJoint.spatJointAcc(*qdd);
		acc = m_inboundJoint.acc;
		acc += v;
		acc += coriolis;
	}

	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi)
		(*lpi)->featherstonePass2(sv, dv, acc);
}
///////////////////////////////////////////////////////////////////////////////
// Recursively update state of this and
// all descendent links of a MultiBody, based on the State.
//	This routine assumes compXf called previously and
//	the link transform Xf is current.
void CRigidBody::updatePoses()
{
	CSystem* m_system = theApp.GetSystem();
	Lock lock (theApp.protectBodies);
try
{
	if (_isnan( m_linkV.x )) throw 1;
	if (_isnan( m_linkV.y )) throw 2;
	if (_isnan( m_linkV.z )) throw 3;
	if (_isnan( m_linkW.x )) throw 4;
	if (_isnan( m_linkW.y )) throw 5;
	if (_isnan( m_linkW.z )) throw 6;

	x = Xbi.d;//_globalPose.trans();
	if (_isnan( x.x )) throw SolveIt_ERROR(L"-1");
	if (_isnan( x.y )) throw SolveIt_ERROR(L"-2");
	if (_isnan( x.z )) throw SolveIt_ERROR(L"-3");
	q.set(Xbi.R);//_globalPose.rot());
	if (_isnan( q.x_ )) throw SolveIt_ERROR(L"-1");
	if (_isnan( q.y_ )) throw SolveIt_ERROR(L"-2");
	if (_isnan( q.z_ )) throw SolveIt_ERROR(L"-3");
	if (_isnan( q.s_ )) throw SolveIt_ERROR(L"-4");
	m_globalLinkV = Xbi*m_linkV;
	m_globalLinkW = Xbi*m_linkW;

	p						= m_globalLinkV/m_fMassInverse;
	m_vecAngularVelocity	= m_globalLinkW;
	L		= get_InertiaTensor()*m_vecAngularVelocity;
//##	EnforceRigidBoundary();
	PQP_Update();
// recurse through hierarchy
	vector<Link *>::iterator lpi;
	for(lpi = outbounds.begin(); lpi != outbounds.end(); ++lpi) (*lpi)->updatePoses(); 
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	//##m_virtualBodyManager->Stop();
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
void Se3::set(const SoTransform *xform)
{
	const float *quat;
	const float *trans;

	quat = xform->rotation.getValue().getValue();
	q.x_ = quat[0]; q.y_ = quat[1]; q.z_ = quat[2]; q.s_ = quat[3];

	trans = xform->translation.getValue().getValue();
	d.x = trans[0];
	d.y = trans[1];
	d.z = trans[2];
}
#endif
///////////////////////////////////////////////////////////////////////////////
#if 0
void Se3::toSoTransform(SoTransform *xform) const
{
#if OPCOUNTS
	xform->rotation.setValue
		(q.x_.toDouble(), q.y_.toDouble(), q.z_.toDouble(), q.s_.toDouble());
	xform->translation.setValue(d.x.toDouble(), d.y.toDouble(), d.z.toDouble());
#else
	xform->rotation.setValue(q.x_, q.y_, q.z_, q.s_);
	xform->translation.setValue(d.x, d.y, d.z);
#endif
}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Xform a spatial vector u in a source frame to a spatial vector v in
// a destination frame.	X is the transformation from the source frame
// to the destination frame.	u and v must be distinct!

void xformSpatVect(const MatX &X, const Vect6 &u, Vect6 &v)
{
	const Mat3 &R = X.rot();
	v.a = R*u.a;
	v.b = R*u.b;
	v.b += (X.trans() .vectorProd(v.a) );
}
///////////////////////////////////////////////////////////////////////////////
// Xform a spatial vector u in a source frame to a spatial vector v in
// a destination frame.	X is the transformation from the destination frame
// to the source frame. u and v must be distinct!

void invXformSpatVect(const MatX &X, const Vect6 &u, Vect6 &v)
{
	X.rot().invXform(u.a, v.a);
	v.b = u.b + (u.a .vectorProd( X.trans()));
	X.rot().invXform(v.b);
}
///////////////////////////////////////////////////////////////////////////////
// Xform a spatial inertia Ia in a source frame to a spatial vector Ib in
// a destination frame.	X is the transformation from the source frame
// to the destination frame.

void xformSpatInertia(const MatX &X, const Mat6 &Ia, Mat6 &Ib)
{
	const Mat3 &R = X.rot();
	Mat3 Rt = ~R;
	Ib.m12 = (R*Ia.m12)*Rt;
	Mat3 m21 = (R*Ia.m21)*Rt;

	Mat3 U = Mat3(X.trans());
	Mat3 v = Ib.m12*U;
	Mat3 m11 = (R*Ia.m11)*Rt;
	Ib.m11 = m11 -  v;
	Ib.m22 = ~Ib.m11;
	Ib.m21 = U*m11;
	Ib.m21 += ~Ib.m21;//'symmetrize';
	Ib.m21 -= U*v;
	Ib.m21 += m21;
}

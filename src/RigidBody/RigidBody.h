// RigidBody.h: interface for the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGIDBODY_H__64D86A20_5AA2_4996_BF3D_F88BA5471C47__INCLUDED_)
#define AFX_RIGIDBODY_H__64D86A20_5AA2_4996_BF3D_F88BA5471C47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "State.h"
#include "Types.h"
#include "Joint1DOF.h"
/////////////////////////////////////////////////////////////////////////////
class PQP_Model;
/////////////////////////////////////////////////////////////////////////////
#include "TrajectoryRecord.h"

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class instruction;
class Contact;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

class CRigidBody : 
	public State  
{
public:
//	CRigidBody();
	virtual ~CRigidBody();
//	CRigidBody(const wstring& _name);
	CRigidBody(const wstring& _name, int _type);
//	CRigidBody(const CRigidBody& a){}
//	CRigidBody& operator=(const CRigidBody& a){return *this;}
/////////////////////////////////////////////////////////////////////////////
// Construction
	double CalculateMassFromGeometry();//virtual =0;
	double ComputeInertia();// virtual = 0;
	void DefineShape();// virtual = 0;
///////////////////////////////////////////////////////////////////////////////
//Accessors:
//	Point3D& m_ positionCM;//() {return Vector3D(m_CurrentPose.trans().v);}			// center of mass
//	Quat& m_ q;//() {return m_CurrentPose.rot();}			// center of mass
//	Vector3D& m_ momentumCM;
//	Vector3D& m_ angularMomentum;// = Total angular momentum - [(mass) positionCM ^ velocityCM]
	Se3 CurrentPose() {return Se3(q, x);}
	long set_Position(const Point3D& point);
	long set_InitialState(const State& newValue);
	long set_MassInverse(const double& newValue);
	long set_Mass(const double& newValue);
	long set_Quat(const Quat& quat);
	long set_Quat(double angle, const Vector3D& axis);
	long set_Radius(const double& newValue);
	long set_Width(const double& newValue);
	long set_Height(const double& newValue);
	long set_Depth(const double& newValue);
	long set_DrawStyle(const long& newValue);
	long get_DrawStyle(long& newValue);
	long set_Momentum(const Vector3D& newValue);
	long set_Velocity(const Vector3D& newValue);
	long set_Color(const Vector3D& newValue);
	long set_AngularMomentum(const Vector3D& newValue);
	long set_AngularVelocity(const Vector3D& newValue);
	long set_DisableSelect(const bool& newValue);
///////////////////////////////////////////////////////////////////////////////
	long get_fCurrent(double& val);
	long get_fMagneticDipoleMoment(Vector3D& val);//times  mu_0 / (4 PI)


	void OnDrawState();
	void OnDrawShape();
///////////////////////////////////////////////////////////////////////////////
	void Reset();
	void SetPreviousState();
	void SetToPreviousState();
	void SaveState();
	void RestoreSavedState();
///////////////////////////////////////////////////////////////////////////////
	enum VA_Role{ _rhs, _sum, _difference, _crossProduct} m_role;
	vector<CRigidBody *> m_bodiesDependentOn;
	vector<CRigidBody *> m_bodiesToAdvise;
	void OnAdvisePositionChanged();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//dynamic evolution:
	void SetStateOfBody(vector<double>::const_iterator & iy, bool bUpdatePreviousState = true)
	{
		if (bUpdatePreviousState) m_statePrevious = *this;
		State::operator=(iy);
		EnforceRigidBoundary();
		PQP_Update();
	}
///////////////////////////////////////////////////////////////////////////////
//restoring:
	void SetStateOfBody(const State& y)
	{
		State::operator=(y);
			try{
			PQP_Update();
		}
		catch( SolveIt_Error& e){e.why();}catch(...){}
	}
///////////////////////////////////////////////////////////////////////////////
	void GetStateOfBody(vector<double>& y)
	{
		toVector(y);
	}
	void GetTimeDerivativeOfState(StateVector ydot);//vector<double>& ydot);
///////////////////////////////////////////////////////////////////////////////
	void TimeDerivativeSetBodyState(double y[]);//StateVector y
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// tracking
	CTrajectoryRecord m_trajectoryRecord;
	bool m_bTrackingOn;
	bool m_bPlotTrajectory;
	long m_lTrackingOption;
	double m_fTrackingStartTime;
	double m_fTrackingStopTime;
	Vector3D m_vecPlotTrajectoryColor;
	long m_lShowPositionEveryNumSteps;
	long m_lShowVelocityEveryNumSteps;
	double m_fScaleMomentum;
	double m_fScaleVelocity;
	double m_fScaleAngularMomentum;
	double m_fScaleForce;
	double m_fScaleTorque;
	double m_fScaleAcceleration;
	double m_fScaleNormal;
// RecordData w/ t1 > t2 turns tracking off
	void RecordData(long option);//, double t1, double t2);
	void AnalyzeData(const wstring& s);//long option, const Vector3D& n);
	void ClearTrajectoryRecord();
	void ResetTrajectoryRecord();
	void WriteTrack();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// selecting
	bool m_bDisableSelect;
	double m_f_zScreen;
	RECT m_rcScreen;

	BOOL HitTest(const POINT& pt) const;
//	{
//		if (m_bDisableSelect) return FALSE;
//		return PtInRect(&m_rcScreen, pt);
//	}

	void SetScreenRect();


void NormalizeRect(RECT& r)
{
	int nTemp;
	if (r.left > r.right)
	{
		nTemp = r.left;
		r.left = r.right;
		r.right = nTemp;
	}
	if (r.top > r.bottom)
	{
		nTemp = r.top;
		r.top = r.bottom;
		r.bottom = nTemp;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Derived/Computed quantities: temporary variables used by integrator
// temporary variables used for inspection
	Vector3D Compute_UserDefinedExternalForce(double t);
	Vector3D Compute_UserDefinedInternalForce(double t);
	double Compute_UserDefinedPotential(double t);
	double Compute_UserDefinedPotential(const double& t, const double& X,const double& Y,const double& Z);

	vector<Couple> m_CoupleList;
	void ClearForcesTorques();
	void AddForcesTorques();
	void SetPreviousForcesTorques() {
		m_vecPreviousForce = m_vecForce;
		m_vecPreviousTorque = m_vecTorque;
	}
	void Compute_External_Force_and_Torque(double t);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void get_InverseInertiaTensor( Mat3& getValue ) {
		if (m_fMassInverse==0.0) m_mat3InverseInertiaTensor = Mat3::ZERO;
		else
		{
			Mat3 R;
			R.set(q);
			Mat3 tR;
			tR.xpose(R);
			m_mat3InverseInertiaTensor = m_mat3InitialInverseInertiaTensor;	// initial value of the inverse inertia tensor
			m_mat3InverseInertiaTensor.premult(R);
			m_mat3InverseInertiaTensor.postmult(tR);	// inverse inertia tensor
		}
		getValue = m_mat3InverseInertiaTensor;
	}
///////////////////////////////////////////////////////////////////////////////
	Mat3 get_InertiaTensor() {
		Mat3 I;
		get_InverseInertiaTensor( I );
		I.invert();
		return I;
	}
///////////////////////////////////////////////////////////////////////////////
// = Total angular momentum
	Vector3D GetTotalAngularMomentum()
	{
		return ((x.vectorProd(p))+L);
	}
	long get_Velocity( Vector3D& getValue );
	long get_AngularVelocity( Vector3D& getValue );
	long get_Mass( double& getValue );

///////////////////////////////////////////////////////////////////////////////
// Energy
	double m_fEnergy;
	double m_fKineticEnergy;
	double m_fPotentialEnergy;
	long get_KineticEnergy(double& getValue) {
		if (m_fMassInverse==0.0) return (getValue=m_fKineticEnergy = 0, 0);
		else
		{
			Vector3D AngularVelocity;
			get_AngularVelocity( AngularVelocity );
			getValue=m_fKineticEnergy = 0.5*( m_fMassInverse*(p*p)+ L*AngularVelocity );
		}
		return  0;
	}
	long get_PotentialEnergy( double& getValue );
	long get_Energy( double& getValue );
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void PQP_Update()
	{
//		SetScreenRect();
		if (m_boolSpheresOnly) return;
		SetTrn();
		SetRot();
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	Mat3 ComputeCollisionMatrix(const Vector3D& contactPoint);
	Mat3 GetSiI(const Vector3D& contactPoint);
	Mat3 GetW();//W=inverted M
	Mat3 GetS(const Vector3D& contactPoint);
	void ComputeImpulses();
	void ComputeMomentumChanges();
	void EnforceRigidBoundary();
	void EnforceAnalyticalConstraint();
///////////////////////////////////////////////////////////////////////////////
	Vector3D ComputeVelocityOfPointInBody(const Point3D& point);
	Vector3D ComputePreviousVelocityOfPointInBody(const Point3D& point);
	Vector3D ComputeNonContactAndInertialAcceleration(const Point3D& point);
	Mat3 ComputeContactMatrix(const Point3D& point1, const Point3D& point2);
	vector<Contact*> m_ContactPointList;
	vector<Vector3D> m_NonContactAndInertialAccelerationList;
	void ClearContactPointList();
	void AddContactPoint(Contact* c);//const Point3D& point);
	void ComputeNonContactAndInertialAccelerations();
	void ComputeNormalAndFrictionalForcesAndTorque();
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//Transformations:
	Point3D	LabToLocal(const Point3D& point) const;
	Vector3D LabToLocal(const Vector3D& vec) const;
	Point3D	LocalToLab(const Point3D& point) const;
	Vector3D LocalToLab(const Vector3D& vec) const;
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	friend wostream& operator<<(wostream& os, const CRigidBody& a);
	static bool m_boolSpheresOnly;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// data
	wstring	name;	//	name of body
	int	type;		//	SPHERE,BLOCK,CYLINDER,VECTOR_ANALYSIS_OBJECT
	short m_tagImage;
///////////////////////////////////////////////////////////////////////////////
	double m_fCharge;
	double m_fCurrent;
	double m_fMagneticDipoleMoment;//times  mu_0 / (4 PI)
	double m_fMassInverse;

	double m_fCentralForce;// if m_fMassInverse == 0
	bool m_bCentralForce_IsValid;

	double m_fMass;

	double m_fDensity;
	double m_fRadius;
	double m_fWidth;
	double m_fHeight;
	double m_fDepth;
	double m_fCoefficientOfRestitution;
	double m_fCoefficientOfKineticFriction;
	double m_fCoefficientOfStaticFriction;
///////////////////////////////////////////////////////////////////////////////
	Vector3D m_vecMagneticDipoleOrientation;
///////////////////////////////////////////////////////////////////////////////
	Vector3D m_vecForce;
	Vector3D m_vecTorque;
	Vector3D m_vecContactForce;
	Vector3D m_vecContactTorque;
	Vector3D m_vecAngularVelocity;
	Vector3D m_vecPreviousForce;
	Vector3D m_vecPreviousTorque;
	Vector3D m_vecNumericalPositionDamping;
	Vector3D m_vecNumericalVelocityDamping;
	Vector3D m_vecDampingCoefficient;
	Vector3D m_vecImpulseLab;
	Vector3D m_vecImpulseTorqueLab;

	Vector3D m_vecInclineNormalForce;
	Vector3D m_vecInclineFrictionalForce;
	Vector3D m_vecInclineTorque;
///////////////////////////////////////////////////////////////////////////////
	Vector3D m_vecVelocity;
/////////////////////////////////////////////////////////////////////////////
	Vector3D Static_GravitationalField (const Point3D& p)
	{
		Vector3D	r		= p - x;
		double		R		= r.norm();
		if (R < 0.1e-12) return ZERO_3D;
					r /= R;
		double		mass	= 0;
		if (m_bCentralForce_IsValid && (m_fMassInverse < 0.1e-12)) mass	= m_fCentralForce;// if m_fMassInverse == 0
		else if (m_fMassInverse > 0.1e-12) mass	= 1.0/m_fMassInverse;
		else return ZERO_3D;
		Vector3D	G		= (r)*(-mass/(R*R));
		return G;
	}
	Vector3D ElectrostaticField (const Point3D& p)
	{
		Vector3D	r = p - x;
		double		R = r.norm();
		if (R < 0.1e-12) return ZERO_3D;
					r /= R;
		Vector3D	E = (r)*(m_fCharge/(R*R));
		return E;
	}
	Vector3D StaticMagneticField (const Point3D& p)
	{
		Vector3D	m = m_fMagneticDipoleMoment*m_vecMagneticDipoleOrientation;
		Vector3D	r = p - x;
		double		R = r.norm();
		if (R < 0.1e-12) return ZERO_3D;
					r /= R;
		double		a = 3*(r*m);
		Vector3D	B = (a*r - m)/(R*R*R);
		return B;
	}
///////////////////////////////////////////////////////////////////////////////
	Mat3 m_mat3InitialInverseInertiaTensor;
	Mat3 m_mat3InverseInertiaTensor;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	State m_stateInitial;
	State m_statePrevious;
	State m_stateSave;
	State m_stateTimeDerivativeOfState;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	enum _FunctionFor {
		__ExternalForceX,
		__ExternalForceY,
		__ExternalForceZ,
		__InternalForceX,
		__InternalForceY,
		__InternalForceZ,
		__Potential
	};
///////////////////////////////////////////////////////////////////////////////
	bool SetUserDefinedName(const wchar_t* name, _FunctionFor _for);
	bool SetUserDefined_IsTimeDependent(const bool val, _FunctionFor _for);

	double CallUserDefined(_FunctionFor _for, long nargs, ...);
///////////////////////////////////////////////////////////////////////////////
	_CallUserDefined m_uUserDefinedExternalForceX;
	_CallUserDefined m_uUserDefinedExternalForceY;
	_CallUserDefined m_uUserDefinedExternalForceZ;
	_CallUserDefined m_uUserDefinedInternalForceX;
	_CallUserDefined m_uUserDefinedInternalForceY;
	_CallUserDefined m_uUserDefinedInternalForceZ;
	_CallUserDefined m_uUserDefinedPotential;
///////////////////////////////////////////////////////////////////////////////
	bool m_bUserDefinedRedIsValid;
	bool m_bUserDefinedGreenIsValid;
	bool m_bUserDefinedBlueIsValid;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Constraints:
	bool m_bConstraintForceShouldBeUpdated;
	bool m_bConstraintImpulseShouldBeUpdated;
	bool m_bSubjectToActiveConstraint;
	void activate(void){ m_bSubjectToActiveConstraint=true; }
	void deactivate(void){ m_bSubjectToActiveConstraint=false; }
	bool active(void){return m_bSubjectToActiveConstraint && m_fMassInverse>0.0;}
///////////////////////////////////////////////////////////////////////////////
// for connection to pulley:
	void SetPulleyContactPoint(const double& R);
	enum InitialAngleInQuadrant{___Quadrant_One=1,___Quadrant_Two,___Quadrant_Three,___Quadrant_Four};// m_nConnectedInQuadrant;
private:
	InitialAngleInQuadrant m_nConnectedInQuadrant;
public:
///////////////////////////////////////////////////////////////////////////////
	long set_InitialAngleInQuadrant(const InitialAngleInQuadrant& newValue)
	{
		if (int(newValue)>=int(___Quadrant_One) && int(newValue)<=int(___Quadrant_Four) )
		{
			m_nConnectedInQuadrant = newValue;
			return 0;
		}
		return -1;
	}
	long get_InitialAngleInQuadrant(InitialAngleInQuadrant& val)
	{
		val = m_nConnectedInQuadrant;
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
	Vector3D Tension(double g, double R, double InertiaInv, double w);
	_Tension m_Tension;
	_Tension2 m_Tension2;
	_Cos m_Cos;
	_Sin m_Sin;
	void FirstPossibleAngle(double& Cos, double& Sin, double& theta);
	void SecondPossibleAngle(double& Cos, double& Sin, double& theta);
	void InitializePulleyConnection(double radiusOfPulley, long nConnectedInQuadrant);
	Point3D m_PulleyContactPoint;
	double m_fDistanceToFixedPointOnPulley;
///////////////////////////////////////////////////////////////////////////////
	double m_fRadiusOfPulley;
	double m_fAngleOfInitialFixedPointOnPulley;
	double m_fAngleOfLastFixedPointOnPulley;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Constraints:
	ListOfConstraints m_ListOfConstraints;
///////////////////////////////////////////////////////////////////////////////
	void AddForceAndTorqueForZeroAccelerationAtFixedPoint(const Point3D& point);
	void GetForceAndTorqueForZeroAccelerationAtFixedPoint(const Point3D& point, const Vector3D& input_force, const Vector3D& input_torque, Vector3D& force, Vector3D& torque);
///////////////////////////////////////////////////////////////////////////////
	bool AddConstraintToList(CConstraint* pc);//do not use
///////////////////////////////////////////////////////////////////////////////
	bool RemoveConstraintFromList(CConstraint* pc);
///////////////////////////////////////////////////////////////////////////////
	bool m_bConstraintedToPlane;
///////////////////////////////////////////////////////////////////////////////
// if rolling down a plane, with line of contact equivalent to a point contact:
	void ConstrainToPlane(double fAngleOfIncline, double x, double y, double z);
	void FreePlaneConstraint() {
		m_fAngleOfIncline = 0;
		m_bConstraintedToPlane = false;
	}
	void AddInclineForcesTorques() {
		if (!m_bConstraintedToPlane) return;
		m_vecForce	+= m_vecInclineNormalForce;
		m_vecForce	+= m_vecInclineFrictionalForce;
		m_vecTorque += m_vecInclineTorque;
	}
///////////////////////////////////////////////////////////////////////////////
	double m_fAngleOfIncline;
	long set_AngleOfIncline(const double& newValue, bool bUpdateOthers = true) {
		m_fAngleOfIncline = newValue;
		Point3D& point = m_ptPointInPlane;
		ConstrainToPlane(m_fAngleOfIncline, point.x,point.y,point.z);
		return 0;
	}
	long get_AngleOfIncline( double& getValue ) {
		getValue = m_fAngleOfIncline;
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
	Point3D m_ptContactPoint;
	long set_ContactPoint(const Point3D& newValue, bool bUpdateOthers = true) {
		m_ptContactPoint = newValue;
		if (bUpdateOthers) {
		}
		return 0;
	}
	long get_ContactPoint( Point3D& getValue ) {
		getValue = m_ptContactPoint;
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
	Point3D m_ptPointInPlane;
	long set_PointInPlane(const Point3D& newValue, bool bUpdateOthers = true) {
	//	m_ptPointInPlane = newValue;
		Point3D point(newValue);
		ConstrainToPlane(
							m_fAngleOfIncline,
							point.x,
							point.y,
							point.z
							);
		//FireViewChange();//m_pIView->ReDraw();
		if (bUpdateOthers) {
		}
		return 0;
	}
	long get_PointInPlane( Point3D& getValue ) {
		getValue = m_ptPointInPlane;
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool m_bShowMomentum;
	bool m_bShowAngularMomentum;
	bool m_bShowForce;
	bool m_bShowAcceleration;
	bool m_bShowContactForce;
	bool m_bShowTorque;
	bool m_bShowContactTorque;
	bool m_bShowContacts;
	bool m_bShowQuadraticConstraint;
	bool m_bShowNormals;
	bool m_bShowTrajectory;
///////////////////////////////////////////////////////////////////////////////
	long m_lDrawStyle;
	bool m_bHide;
	Vector3D m_vecColor;
///////////////////////////////////////////////////////////////////////////////
	GLUquadricObj *m_pQuadObj;
	PQP_Model* m_pPQP_Model;
///////////////////////////////////////////////////////////////////////////////
	double m_Rot[3][3];
	double m_Trn[3];

	void SetTrn()
	{
		m_Trn[0] = x.x;
		m_Trn[1] = x.y;
		m_Trn[2] = x.z;
	}

	void SetRot()
	{
		Mat3 R;
		R.set(q);
		m_Rot[0][0] = R.m[0][0];
		m_Rot[0][1] = R.m[0][1];
		m_Rot[0][2] = R.m[0][2];
		m_Rot[1][0] = R.m[1][0];
		m_Rot[1][1] = R.m[1][1];
		m_Rot[1][2] = R.m[1][2];
		m_Rot[2][0] = R.m[2][0];
		m_Rot[2][1] = R.m[2][1];
		m_Rot[2][2] = R.m[2][2];
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//mass-spring systems only:
	vector<CRigidBody *> m_bodiesConnectedTo;
//	vector<pair<double, double>> m_ListOf_springConstants_springLengths;
	ListOf_springConstants_springLengths m_ListOf_springConstants_springLengths;
	PairPoint3DVector m_PairPoint3DVector;
///////////////////////////////////////////////////////////////////////////////
// MultiBody
	static Joint1DOF* m_baseJoint;
	static CRigidBody* m_LastLink;
	Joint1DOF m_inboundJoint;
	Joint1DOF* m_ptrOutboundJoint;
typedef CRigidBody Link;
	Link *m_parentLink;	// the unique link m_parentLink to this one (parent in the tree)
	vector<Link *> outbounds; // list of outbound (children) links
///////////////////////////////////////
	Vector3D m_jointAxis;
// springs/dampers/computed torque/explicit torque for 1 d.o.f. joints
	double m_Ks;						// spring stiffness, if spring is present 
	double m_Ls;						// natural length of spring, is spring is present 
	double m_Kd;						// vicous damping coefficient, if damping is present 
	bool m_computedTorque; // true iff doing computed torque ctrl of this jnt 
	double m_qddDes;				// desired acceleration of this jnt if we're doing
											// computed torque control 
	double m_tau;					 // explicit applied external torque to this joint
	long set_MultiBody_SPRING_CONSTANT(const double& newValue, bool bUpdateOthers = true) {m_Ks = newValue;return 0;}
	long get_MultiBody_SPRING_CONSTANT( double& getValue ) {getValue = m_Ks;return 0;}
	long set_MultiBody_SPRING_LENGTH(const double& newValue, bool bUpdateOthers = true) {m_Ls = newValue;return 0;}
	long get_MultiBody_SPRING_LENGTH( double& getValue ) {getValue = m_Ls;return 0;}
	long set_MultiBody_DAMPING_COEFFICIENT(const double& newValue, bool bUpdateOthers = true) {m_Kd = newValue;return 0;}
	long get_MultiBody_DAMPING_COEFFICIENT( double& getValue ) {getValue = m_Kd;return 0;}
	long set_MultiBody_TARGET_ACCELERATION(const double& newValue, bool bUpdateOthers = true) {m_qddDes = newValue;return 0;}
	long get_MultiBody_TARGET_ACCELERATION( double& getValue ) {getValue = m_qddDes;return 0;}
	long set_MultiBody_TORQUE(const double& newValue, bool bUpdateOthers = true) {m_tau = newValue;return 0;}
	long get_MultiBody_TORQUE( double& getValue ) {getValue = m_tau;return 0;}
	long set_MultiBody_CONTROL(const int& newValue, bool bUpdateOthers = true) {m_computedTorque = newValue?true:false;return 0;}
	long get_MultiBody_CONTROL( bool& getValue ) {getValue = m_computedTorque;return 0;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// xformations across the m_parentLink joint
///////////////////////////////////////
// xforms independent of joint angle
//	MatX Xij;				 // xform: m_parentLink link to joint coords 
//	MatX Xjo;				 // xform: joint to outbound link coords 
// pre-compiled (constant) portion of Xf (below)
//	MatX Xf0;				// pre-modified joint xformation (fwd)
	MatX Xf;	// fwd joint xform:	m_parentLink link's frame -> this link's frame
	MatX Xb;			// xform:	Xf.inverse();
	MatX Xbi;			// xform:	link -> inertial
/////////////////////////////////////////////////////////////
// Index of this Link's state in the multibody state vector.	If the
// m_parentLink joint has 1 d.o.f., and sv is the MultiBody state vector,
// then sv[svIdx] is the joint position and sv[svIdx+1] is the joint
// velocity.	If the m_parentLink joint is floating (which is only
// allowed for the base link), then sv[svIdx]..sv[svIdx+12] is the
// RBConfig of this Link.
	int svIdx;
///////////////////////////////////////////////////////////////////////////////
// support routines for forward dynamics
///////////////////////////////////////////////////////////////////////////////
	void compAbsoluteVels(const double state[]);
///////////////////////////////////////////////////////////////////////////////
	void compAbsoluteVels(const double state[], 
			const Vector3D &inboardLinkV, const Vector3D &inboardLinkW,
			Vector3D &linkV, Vector3D &linkW, int compCor);

///////////////////////////////////////////////////////////////////////////////
//	void compAbsoluteVels2(const RBConfig state[], 
//			const Vector3D &inboardLinkV, const Vector3D &inboardLinkW,
//			Vector3D &linkV, Vector3D &linkW, int compCor);

	Mat6 m_IssI;
	void compSpatialQuants();
	double jointComponent(const Vect6 &P);
	void spatJointAcc(double qdd);
///////////////////////////////////////////////////////////////////////////////
// forward dynamics
	void featherstonePass1(const double sv[],
			 Mat6 &Iprop, Vect6 &Pprop);
///////////////////////////////////////////////////////////////////////////////
// forward dynamics
	void ForwardDynamics(const double sv[], Mat6 &parentLinkI, Vect6 &parentLinkZ);
///////////////////////////////////////////////////////////////////////////////
	void featherstonePass1(const double sv[], const MatX &inboardXbi,
			 const Vector3D &inboardLinkV, const Vector3D &inboardLinkW,
			 Mat6 &Iprop, Vect6 &Pprop);
///////////////////////////////////////////////////////////////////////////////
	void featherstonePass2(const double sv[], double dv[], 
			 const Vect6 &inboardAcc);
// forward dynamics
//	void featherstonePass12(const RBConfig sv[], const MatX &inboardXbi,
//			 const Vector3D &inboardLinkV, const Vector3D &inboardLinkW,
//			 Mat6 &Iprop, Vect6 &Pprop);
//	void featherstonePass22(const RBConfig sv[], RBConfig dv[], 
//			 const Vect6 &inboardAcc);
// other
	void updatePoses();
///////////////////////////////////////////////////////////////////////////////
	void updatePose(const double sv[]);
///////////////////////////////////////////////////////////////////////////////
// Let r be vector from joint to the origin of link coords.	For
// revolute joints we compile:
//	Vector3D mom;		// m_jointAxis x r: moment part of the spatial joint 
	Vector3D m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin;
	Vector3D m_ThisLinkCenterOfMassWithRespectToJointOriginPerpJointAxis;
//	Vector3D mom2;	 // m_jointAxis x (m_jointAxis x r): used in calculating coriolis terms 
	Vector3D _J;	 // moments of inertia (diagonal el'ts of mass matrix)

	Vect6 coriolis;	// spatial coriolis term
	Vect6 acc;			 // spatial acceleration
	Vect6 Is;				// Featherstone expression
	double m_torque;					 // torque on joint
	double sIs;							// analog of (1 / moment of inertia)
//	Mat6 I;					// articulated spatial inertia
// poses in the local and global frames
	Se3 _globalPose;	
	Se3 _localPose;
///////////////////////////////////////////////////////////
	Vector3D m_linkV;	// absolute linear velocity (link frame)
	Vector3D m_linkW;	// absolute angular velocity (link frame)
///////////////////////////////////////////////////////////
	Vector3D m_globalLinkV;	// absolute linear velocity (lab frame)
	Vector3D m_globalLinkW;	// absolute angular velocity (lab frame)

	class ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass {	
	private:
		Vector3D m_JointOriginWithRespectToParentLinkCenterOfMass;	
		Vector3D m_ThisLinkCenterOfMassWithRespectToJointOrigin;
		Vector3D m_InitialThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass;
	public:
		ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(const Vector3D &xParent, const Vector3D &xMe):
			m_JointOriginWithRespectToParentLinkCenterOfMass(xParent),
			m_InitialThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(xParent+xMe),
			m_ThisLinkCenterOfMassWithRespectToJointOrigin(xMe)
		{}
		Vector3D GetThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(const Mat3& Rj) {
			return m_ThisLinkCenterOfMassWithRespectToJointOrigin + Rj*m_JointOriginWithRespectToParentLinkCenterOfMass;
		}
		Vector3D GetThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(const double &q, const Vector3D& axis) {
			return m_InitialThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass + q*axis;
		}
	} *parentLinkToThisLink;

//	Vector3D m_ThisLinkCenterOfMassWithRespectToJointOrigin;
	Vector3D m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass;

	Mat6 m_linkI;// articulated spatial inertia
	Vect6 m_linkS, m_linkZ, m_linkC, m_linkA;
	void InitIandZ();//InitIZandC(const double sv[]);
	void ComputeLinkIAndZs(const double sv[]);
	void ComputeLinkAccelerations(int& i, const double t, const double x[], double dx[]);//const vector<double>& _state);
////////////////////////////////////////////////////////////////////////////////
//	MultiBody forward dynamics
///////////////////////////////////////////////////////////////////////////////
	void ComputeLinkFrameAndComputeVelocity(const double t, const double _state[],
			bool bUpdate=false);//const vector<double>& _state)
////////////////////////////////////////////////////////////////////////////////
	void connect(
			const Point3D &attachMeToInboundJointHere,//present coordinates of point of attachment
			Link* _inbound,
			const Point3D &inboundJointLocation,
			Joint1DOF::JointType jointType,
			const Vector3D &jointAxis
			);
////////////////////////////////////////////////////////////////////////////////
	void connect(
			const Point3D &attachMeToInboundJointHere,//present coordinates of point of attachment
			const Vector3D &zFromAxis, const Vector3D &xFromAxis,
			const Vector3D &zToAxis, const Vector3D &xToAxis,
			Link* _inbound,
			const Point3D &inboundJointLocation,
			const Vector3D &in_z_FromAxis, const Vector3D &in_x_FromAxis,
			const Vector3D &in_z_ToAxis, const Vector3D &in_x_ToAxis,
			Joint1DOF::JointType jointType,
			const Vector3D &jointAxis
			);
////////////////////////////////////////////////////////////////////////////////
//vector<double>& _state,
//			 map<long, double>& Joint1DOFAccelerations,
	void connect(
			const Point3D &attachMeToInboundJointHere,//present coordinates of point of attachment
			const Vector3D &zAxis,
			const Vector3D &xAxis,
			Link* _inbound,
			const Point3D &inboundJointLocation,
			const Vector3D &in_z_Axis,
			const Vector3D &in_x_Axis,
			Joint1DOF::JointType jointType, 
			const double& _Kd,
			const double& _Ks,
			const double& _Ls,
			Component1DOF displacement, Component1DOF velocity
			);
////////////////////////////////////////////////////////////////////////////////
//	Compute the Xf matrix of a prismatic or revolute joint from the
//	Xf0 matrix.	This is the forward xformation across the joint, from
//	the m_parentLink link's coords to the outbound link's coords.
/*
nash:
G = R F
F = ( x , y, z ) col vecs
R = m_jointAxis@z~ + ref@x~ + m_jointAxis^ref@y~ = exp( t n*S )
1 + 2 cos(t) = m_jointAxis*z + ref*x +	m_jointAxis^ref*y
2 sin(t) n	= z^m_jointAxis + x^ref +	y^(m_jointAxis^ref)
end nash:
*/
	void compXf(const double sv[]);
///////////////////////////////////////////////////////////////////////////////
	/*static*/ void updateXfs(const double sv[]);//, bool bInitial = false);
////////////////////////////////////////////////////////////////////////////////
/*
		// Use of the OLE allocator is required if the resultant Unicode
		// wstring will be passed to another COM component and if that
		// component will free it. Otherwise you can use your own allocator.
	void* operator new(unsigned int nSize) {
		return CoTaskMemAlloc(nSize);
	}
	void operator delete(void* pObj) {
		CoTaskMemFree(pObj);
	}
*/
///////////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////////
/*
struct RigidBodyListCompareEqualToString
{
	RigidBodyListCompareEqualToString(const wstring& _b) : b(_b) {}
	wstring b;
	bool operator()(const CRigidBody * a) const
	{
		return a->name == b;
	}
};
*/
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

#endif // !defined(AFX_RIGIDBODY_H__64D86A20_5AA2_4996_BF3D_F88BA5471C47__INCLUDED_)

// RigidBody.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "Contact.h"
#include "System.h"
#include "instruction.h"
#include "parser.h"
#include "symbol.h"
#include "SymbolTable.h"
#include <STDARG.H>
#include "RigidBodyManager.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "PQP_Sphere.h"
///////////////////////////////////////////////////////////////////////////////
Joint1DOF* CRigidBody::m_baseJoint=0;
CRigidBody* CRigidBody::m_LastLink=0;
bool CRigidBody::m_boolSpheresOnly = true;
///////////////////////////////////////////////////////////////////////////////
wostream& SolveIt::operator<<(wostream& os, const CRigidBody& a)
{
//	return os << a.name << "\r\n" << a.type;
	return os << a.name << "; 1/mass = " << a.m_fMassInverse;//a.type;
}
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
/*
CRigidBody::CRigidBody() :
	name(""),
	type(-1)
{
}

CRigidBody::CRigidBody(const wstring& _name) :
	name(_name),
	type(-1)
{
}
*/

CRigidBody::CRigidBody(const wstring& _name, int _type) :
//	m_ positionCM(x),
//	m_ q(q),
//	m_ momentumCM(p),
//	m_ angularMomentum(L),
	name(_name),
	type(_type),
	m_pQuadObj(gluNewQuadric()),
	m_pPQP_Model(0),
///////////////////////////////////////////////////////////////////////////////
		parentLinkToThisLink(0),
		m_parentLink(0), m_ptrOutboundJoint(0),
//		jointType(NONE),
		svIdx(-1),
		m_computedTorque(false),m_Ks(0),m_Ls(0),m_Kd(0),m_tau(0),m_qddDes(0),
#pragma warning(disable: 4355) // 'this' used before initialized,
		m_trajectoryRecord(this),
#pragma warning(default: 4355)
		m_fDensity(20),
		m_fMass(1),
		m_fMassInverse(1),
		m_fCentralForce(0),
		m_fCharge(0),
		m_fCurrent(0),
		m_fMagneticDipoleMoment(0),
		m_fRadius(0.1),
		m_fWidth(0),
		m_fHeight(0),
		m_fDepth(0),
		m_fPotentialEnergy(0),
		m_fKineticEnergy(0),
		m_fEnergy(0),
		m_tagImage(18),
		m_fCoefficientOfRestitution(1),
		m_fCoefficientOfKineticFriction(0),
		m_fCoefficientOfStaticFriction(0),
		m_fAngleOfIncline(0),

		m_Cos(0),
		m_Sin(0),
		m_Tension(0),
		m_Tension2(0),
		m_fRadiusOfPulley(0.2),
		m_nConnectedInQuadrant(___Quadrant_One),
		m_fAngleOfInitialFixedPointOnPulley(0),
		m_fAngleOfLastFixedPointOnPulley(0),
		m_fDistanceToFixedPointOnPulley(-1),

		m_torque(0),			// torque on joint
		sIs(0),					// analog of (1 / moment of inertia)

		m_f_zScreen(0),
///////////////////////////////////////////////////////////////////////////////
		m_bCentralForce_IsValid(false),
		m_bSubjectToActiveConstraint(false),
		m_lTrackingOption(0), m_fTrackingStartTime(0), m_fTrackingStopTime(0),
		m_bTrackingOn(false),
		m_bPlotTrajectory(false),

		m_bShowMomentum(false),
		m_bShowAngularMomentum(false),
		m_bShowForce(false),
		m_bShowAcceleration(false),
		m_bShowContactForce(false),
		m_bShowContacts(false),
		m_bShowQuadraticConstraint(false),
		m_bShowTorque(false),
		m_bShowContactTorque(false),
		m_bShowNormals(false),
		m_bShowTrajectory(false),

		m_lShowPositionEveryNumSteps(0),
		m_lShowVelocityEveryNumSteps(0),

		m_fScaleForce(1),
		m_fScaleMomentum(1),
		m_fScaleAngularMomentum(1),
		m_fScaleVelocity(1),
		m_fScaleTorque(1),
		m_fScaleAcceleration(1),
		m_fScaleNormal(1),
		m_bUserDefinedRedIsValid(false),
		m_bUserDefinedGreenIsValid(false),
		m_bUserDefinedBlueIsValid(false),
///////////////////////////////////////////////////////////////////////////////
		m_bConstraintForceShouldBeUpdated(false),
		m_bConstraintImpulseShouldBeUpdated(false),
		m_bConstraintedToPlane(false),
		m_lDrawStyle(GLU_FILL),
		m_bHide(false),
		m_bDisableSelect(false),
		m_vecColor(7490, 0.6353, 0.3843),
		m_role(_rhs)
{
	m_LastLink			= this;
	m_boolSpheresOnly	= m_boolSpheresOnly && type==T_SPHERE;//set before calling CRigidBody::Initialize
//	m_rcScreen			= RECT({0,0,0,0});
	m_rcScreen.bottom=m_rcScreen.left=m_rcScreen.right=m_rcScreen.top=0;
	DefineShape();
	CalculateMassFromGeometry();
	switch(type)
	{
		case T_SPHERE:
				m_tagImage = 18;
			break;
		case T_BLOCK:
				m_tagImage = 19;
			break;
		case T_CYLINDER:
				m_tagImage = 20;
			break;
		case T_VECTOR_ANALYSIS_OBJECT:
				m_tagImage = 19;
///////////////////////////////////////////////////////////////////////////////
	m_role = CRigidBody::_rhs;
	set_MassInverse(0);
	m_lShowPositionEveryNumSteps = 1;
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CRigidBody::~CRigidBody()
{
//	if (m_pQuadObj) gluDeleteQuadric(m_pQuadObj);
//	if (m_pPQP_Model) delete m_pPQP_Model, m_pPQP_Model=0;
//	m_pQuadObj=0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CRigidBody::SetUserDefinedName(const wchar_t* name, _FunctionFor _for)
{
	_CallUserDefined* selected = 0;
	switch(_for)
	{
		case __ExternalForceX:selected	= &m_uUserDefinedExternalForceX;break;
		case __ExternalForceY:selected	= &m_uUserDefinedExternalForceY;break;
		case __ExternalForceZ:selected	= &m_uUserDefinedExternalForceZ;break;
		case __InternalForceX:selected	= &m_uUserDefinedInternalForceX;break;
		case __InternalForceY:selected	= &m_uUserDefinedInternalForceY;break;
		case __InternalForceZ:selected	= &m_uUserDefinedInternalForceZ;break;
		case __Potential:selected		= &m_uUserDefinedPotential;break;
	}
	if (selected == 0) return false;
	CSymbolTable* symbolTable	= theApp.GetSymbolTable();
	symbol* sym	= symbolTable->getsym (name);
	if (sym == 0) return false;
	selected->func					= sym->func;
	selected->indexInDataArray		= sym->indexInDataArray;
	selected->nvargs				= sym->nvargs;
	selected->nargs					= sym->nargs;
	selected->name					= name;
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool CRigidBody::SetUserDefined_IsTimeDependent(const bool val, _FunctionFor _for)
{
	_CallUserDefined* selected = 0;
	switch(_for)
	{
		case __ExternalForceX:selected	= &m_uUserDefinedExternalForceX;break;
		case __ExternalForceY:selected	= &m_uUserDefinedExternalForceY;break;
		case __ExternalForceZ:selected	= &m_uUserDefinedExternalForceZ;break;
		case __InternalForceX:selected	= &m_uUserDefinedInternalForceX;break;
		case __InternalForceY:selected	= &m_uUserDefinedInternalForceY;break;
		case __InternalForceZ:selected	= &m_uUserDefinedInternalForceZ;break;
		case __Potential:selected		= &m_uUserDefinedPotential;break;
	}
	if (selected == 0) return false;
	selected->m_bIsTimeDependent	= val;
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ClearForcesTorques()
{
	m_vecForce					= ZERO_3D;
	m_vecTorque					= ZERO_3D;
	m_vecContactForce			= ZERO_3D;
	m_vecContactTorque			= ZERO_3D;
	m_vecImpulseLab				= ZERO_3D;
	m_vecImpulseTorqueLab		= ZERO_3D;

	m_vecInclineNormalForce		= ZERO_3D;
	m_vecInclineFrictionalForce	= ZERO_3D;
	m_vecInclineTorque			= ZERO_3D;

//	ClearContactPointList();
//	m_ptContactPoint			= x;
	m_CoupleList.clear();
//	m_bConstraintForceShouldBeUpdated	= false;
//	m_bConstraintImpulseShouldBeUpdated	= false;
}
/////////////////////////////////////////////////////////////////////////////////////////
void CRigidBody::AddForcesTorques()
{
	vector<Couple>::iterator ic;
	for (ic=m_CoupleList.begin();ic!=m_CoupleList.end();++ic)
	{
		const Couple& c = *ic;
		m_vecForce  += c.force;
		m_vecTorque += c.torqe;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
void CRigidBody::SetPreviousState()
{
	m_statePrevious = *this;
}
/////////////////////////////////////////////////////////////////////////////////////////
void CRigidBody::SetToPreviousState()
{
//	State save = m_statePrevious;
//	m_stateSave = m_statePrevious;
	SetStateOfBody(m_statePrevious);
//	m_statePrevious = save;
//	PQP_Update();
}
/////////////////////////////////////////////////////////////////////////////////////////
void CRigidBody::SaveState()
{
	m_stateSave = *this;
}
/////////////////////////////////////////////////////////////////////////////////////////
void CRigidBody::RestoreSavedState()
{
	State save = m_statePrevious;
	SetStateOfBody(m_stateSave);
	m_statePrevious = save;
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CRigidBody::Reset()
{
	if (theApp.GetRigidBodyManager()->RigidLinkJointStates.size())
	{
		m_inboundJoint.Reset();
//		ComputeLinkFrameAndComputeVelocity(0, &theApp.theSystem.m_rigidBodyManager.RigidLinkJointStates.front(), true);
	}

	if (m_lTrackingOption!=T_TRAJECTORY) m_bPlotTrajectory = false;
	static_cast<State*>(this)->operator=(m_stateInitial);
	m_statePrevious = m_stateInitial;//*this;
	m_stateTimeDerivativeOfState	= Zero_State;
	m_stateSave						= Unit_State;
	ResetTrajectoryRecord();

	m_bConstraintForceShouldBeUpdated = false;


	if (!CRigidBody::m_boolSpheresOnly)
	{
		//SOLID::dtSelectObject(this);
		//SOLID::dtLoadIdentity();
		//SOLID::dtTranslate(x.x, x.y, x.z);
		//SOLID::dtRotate(q.x_, q.y_, q.z_, q.s_);
	}

///////////////////////////////////////////////////////////////////////////////
#if defined(USE_SWIFT)
	swift::SWIFT_Real R[9];
	swift::SWIFT_Real T[3];
	Mat3 o;
	o.set(q);
	for (int row=0;row<3;row++)	for (int col=0;col<3;col++)	R[3*row+col] = o.m[row][col];
	T[0] = x.x; T[1] = x.y; T[2] = x.z;
	_SWIFT_World->scene->Set_Object_Transformation( m_lKey, R, T );
#endif
///////////////////////////////////////////////////////////////////////////////
	PQP_Update();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::TimeDerivativeSetBodyState(double y[])
{
	double* iy = y;
	long j=0;
	for (j=0; j<3; ++j) x.v[j] = *iy++;
	for (j=0; j<4; ++j) q.v[j] = *iy++;
	for (j=0; j<3; ++j) p.v[j] = *iy++;
	for (j=0; j<3; ++j) L.v[j] = *iy++;
//	PQP_Update();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::GetTimeDerivativeOfState(StateVector ydot)
{

	long i = 0;
	ydot[i++] = m_fMassInverse*p.x;
	ydot[i++] = m_fMassInverse*p.y;
	ydot[i++] = m_fMassInverse*p.z;
	
	Quat dq;
	Vector3D AngularVelocity;
	get_AngularVelocity(AngularVelocity);
	dq.deriv(q, AngularVelocity);

	for (long j = 0; j < 4; j++) ydot[i++] = dq[j];
	
	ydot[i++] = m_vecForce.x;
	ydot[i++] = m_vecForce.y;
	ydot[i++] = m_vecForce.z;
	
	ydot[i++] = m_vecTorque.x;
	ydot[i++] = m_vecTorque.y;
	ydot[i++] = m_vecTorque.z;
/*
	ydot.push_back( m_fMassInverse*p.x );
	ydot.push_back( m_fMassInverse*p.y );
	ydot.push_back( m_fMassInverse*p.z );
	
	Quat dq;
	Vector3D AngularVelocity;
	get_AngularVelocity(AngularVelocity);
	dq.deriv(q, AngularVelocity);

	for (long j = 0; j < 4; j++) ydot.push_back( dq[j] );
	
	ydot.push_back( m_vecForce.x );
	ydot.push_back( m_vecForce.y );
	ydot.push_back( m_vecForce.z );
	
	ydot.push_back( m_vecTorque.x );
	ydot.push_back( m_vecTorque.y );
	ydot.push_back( m_vecTorque.z );
*/

	m_stateTimeDerivativeOfState.x = Point3D((m_fMassInverse*p));
	m_stateTimeDerivativeOfState.q = dq;
	m_stateTimeDerivativeOfState.p = m_vecForce;
	m_stateTimeDerivativeOfState.L = m_vecTorque;
}
/////////////////////////////////////////////////////////////////////////////////////////
//Transformations:
Point3D  CRigidBody::LabToLocal(const Point3D& point) const {
	Se3 T(q, x);
	Point3D pt;
	T.inverseTransform(point, pt);
	return pt;
}
Vector3D CRigidBody::LabToLocal(const Vector3D& vec) const {
	Se3 T(q, x);
	Vector3D v;
	T.inverseTransform(vec, v);
	return v;
}
Point3D  CRigidBody::LocalToLab(const Point3D& point) const {
	Se3 T(q, x);
	Point3D pt;
	T.transform(point, pt);
	return pt;
}
Vector3D CRigidBody::LocalToLab(const Vector3D& vec) const {
	Se3 T(q, x);
	Vector3D v;
	T.transform(vec, v);
	return v;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::EnforceRigidBoundary()
{
	const double limit = 2;
	CSystem* m_system = theApp.GetSystem();
	double& t = m_system->m_fRigidBoundary_top;
	double& b = m_system->m_fRigidBoundary_bottom;
	double& l = m_system->m_fRigidBoundary_left;
	double& r = m_system->m_fRigidBoundary_right;
	double& f = m_system->m_fRigidBoundary_front;
	double& k = m_system->m_fRigidBoundary_back;
//	Vector3D& p=x;
//	Vector3D& v=p;
//L = Total angular momentum - [(mass) positionCM ^ velocityCM]
	double s = m_system->m_stepForwardOrBackward;
	double e = -m_fCoefficientOfRestitution;
	switch (m_system->m_nEnforceRigidBoundary)
	{
		case CSystem::rigidBoundary_off:
			break;
		case CSystem::rigidBoundary_bottom_face_only:
			if (x.y < b  && s*p.y < 0.0 ) p.y *= e; 
			return;
		case CSystem::rigidBoundary_all_faces:
			if (x.x> r && s*p.x > 0.0 ) p.x *= e;//, L.z *= -1; 
			if (x.y> t && s*p.y > 0.0 ) p.y *= e; 
			if (x.z> f && s*p.z > 0.0 ) p.z *= e; 
			if (x.x< l && s*p.x < 0.0 ) p.x *= e;//, L.z *= -1;  
			if (x.y< b && s*p.y < 0.0 ) p.y *= e; 
			if (x.z< k && s*p.z < 0.0 ) p.z *= e; 
		return;
	}

#if 0
	vector<CConstraint*>::iterator j;
	for (j=m_ListOfConstraints.begin();j != m_ListOfConstraints.end();++j)
	{
		CConstraint* pc = dynamic_cast<CConstraint*>(*j);
		if (pc && !pc->m_bActive)
		{
			CSurfaceConstraint* psc = dynamic_cast<CSurfaceConstraint*>(*j);
			if (psc)
			{
				double energy = m_system->m_rigidBodyManager.ComputeEnergy(this);

				double surfaceConstraint = psc->SurfaceConstraint(x);
				if	(
					( psc->m_bNegateGradientForNormal && surfaceConstraint>0.0)||
					(!psc->m_bNegateGradientForNormal && surfaceConstraint<0.0)
					)
				{
					long i=0;
					Vector3D n;// = psc->Jacobian(x);
					do {
						n = psc->Jacobian(x);
						n.normalize();
						x += (0.1*surfaceConstraint)*n;
					}
					while  (i++<32 && fabs(surfaceConstraint = psc->SurfaceConstraint(x))>0.1e-9);
//					v -= n*(2.0*(v*n));
					v += n*(2.0*e*(v*n));
					double energy2 = m_system->m_rigidBodyManager.ComputeEnergy(this);
					v *= sqrt(fabs(energy/energy2));
				}
			}
		}
	}
#endif

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// tracking
void CRigidBody::ClearTrajectoryRecord() {
	m_bTrackingOn = false;
	m_trajectoryRecord.Clear();
}
void CRigidBody::ResetTrajectoryRecord() {
	bool bTrackingOn = m_bTrackingOn;
	ClearTrajectoryRecord();
	if (bTrackingOn) {
		RecordData(m_lTrackingOption);//, m_fTrackingStartTime, m_fTrackingStopTime);
	}
}
void CRigidBody::WriteTrack()
{
	try{
	if (!m_bTrackingOn) return;
	m_trajectoryRecord.AddDataPoint(*static_cast<State*>(this));
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
void CRigidBody::RecordData(long option)//, double t1, double t2)
{
	try{
	ClearTrajectoryRecord();
	if ( m_fTrackingStartTime > m_fTrackingStopTime ) return;
	m_bTrackingOn		= true;
//	m_fTrackingStartTime	= t1;
//	m_fTrackingStopTime	= t2;
	m_lTrackingOption	= option;
	m_trajectoryRecord.SetTimes( m_fTrackingStartTime, m_fTrackingStopTime);
	if (m_fTrackingStartTime == 0.0)
		m_trajectoryRecord.AddDataPoint(m_stateInitial);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::AnalyzeData(const wstring& s)
//long option, const Vector3D& n) 
{
	try{
	wchar_t seps[]   = L":";
	wchar_t *token=0;
	std::vector<wchar_t> tokenBuffer(s.begin(), s.end());
	tokenBuffer.push_back(L'\0');
	wchar_t *context = 0;
	token = wcstok_s(tokenBuffer.data(), seps, &context);
	if ( token != NULL )
	{
		if (! _tcscmp(token, L"period"))//strcmp
		{
			token = wcstok_s(NULL, seps, &context);
			if ( token != NULL )
			{
				Vector3D axis;
				/*istrstream*/ std::wstringstream iss(token);
				if (!(iss >> axis).fail())
				{
					TrajectoryPeriod tp;
					tp.alongThisAxis = axis;
					m_trajectoryRecord.AnalyzeData((AnalyzeTrajectory*) &tp);
				}
			}
		}
	}
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ConstrainToPlane(double fAngleOfIncline, double x_, double y, double z) {
	m_bConstraintedToPlane = true;
	m_fAngleOfIncline = DEG_TO_RAD*fAngleOfIncline;
	m_ptPointInPlane = Point3D(x_, y, z);
	double c = fabs(cos(m_fAngleOfIncline));
	double s = sin(m_fAngleOfIncline);
	if (m_fAngleOfIncline > PI_DIVIDED_BY_2) s *= -1;
	Vector3D normalToIncline(-s,c,0);
	Vector3D  rel = normalToIncline;
	switch(type)
	{
		case T_CYLINDER:
		case T_SPHERE:
		{
			rel *= m_fRadius;
		}
		break;
		case T_BLOCK:
		{
			rel *= m_fHeight/2;
		}
		break;
		case T_VECTOR_ANALYSIS_OBJECT:
		{
		}
		break;
	}
	Point3D point = m_ptPointInPlane + rel;
	x = point;
	m_stateInitial.x = x;

//	if (m_shape && !CRigidBody::m_boolSpheresOnly)
//	{
		//SOLID::dtSelectObject(this);
		//SOLID::dtLoadIdentity();
		//SOLID::dtTranslate(x.x, x.y, x.z);
		//SOLID::dtRotate(q.x_, q.y_, q.z_, q.s_);
//	}

///////////////////////////////////////////////////////////////////////////////
#if defined(USE_SWIFT)
	swift::SWIFT_Real R[9];
	swift::SWIFT_Real T[3];
	Mat3 o;
	o.set(q);
	for (int row=0;row<3;row++)	for (int col=0;col<3;col++)	R[3*row+col] = o.m[row][col];
	T[0] = x.x; T[1] = x.y; T[2] = x.z;
	_SWIFT_World->scene->Set_Object_Transformation( m_lKey, R, T );
#endif
///////////////////////////////////////////////////////////////////////////////
	PQP_Update();
	Fire_ViewChange();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool CRigidBody::AddConstraintToList(CConstraint* pc)//do not use
	{
		if (pc == 0) return false;
		CompareConstraintToMe compareConstraintToMe(pc);
		vector<CConstraint*>::iterator i = find_if(m_ListOfConstraints.begin(), m_ListOfConstraints.end(), compareConstraintToMe);
		if (i == m_ListOfConstraints.end())
		{
			m_ListOfConstraints.push_back(pc);
			return true;
		}
		return false;
	}
///////////////////////////////////////////////////////////////////////////////
	bool CRigidBody::RemoveConstraintFromList(CConstraint* pc)
	{
		if (pc == 0) return false;
		CompareConstraintToMe compareConstraintToMe(pc);
		vector<CConstraint*>::iterator i = find_if(m_ListOfConstraints.begin(), m_ListOfConstraints.end(), compareConstraintToMe);
		if (i != m_ListOfConstraints.end())
		{
			m_ListOfConstraints.erase(i);
			return true;
		}
		return false;
	}
///////////////////////////////////////////////////////////////////////////////

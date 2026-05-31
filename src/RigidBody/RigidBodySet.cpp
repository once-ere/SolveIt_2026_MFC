// RigidBody.cpp : Implementation of CRigidBody

#include "stdafx.h"
#include "SolveIt.h"

#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "FieldsManager.h"
#include "parser.h" /* for token definitions and yylval */
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CRigidBody
///////////////////////////////////////////////////////////////////////////////
#if 0
void	CRigidBody::Set( long nEnum, const _Symbol& newVal )
{
	bool b;// = newVal.boolVal;
	long l;// = newVal.lVal;
	std::wstring bstr=newVal.name.c_str();// = *newVal.pbstrVal;
	wstring s;
	double X, y, z, t, &angle=t;
	Point3D		point;
	Vector3D	vec, &axis = vec;
	Quat		q;
	switch ( nEnum )
	{
	case PULLEY_RADIUS:	X= newVal.Dble();
		set_RadiusOfPulley(X);
		return;
	case BODY_PLANE_ANGLE:
	case ANGLE:	X= newVal.Dble();
		set_AngleOfIncline(X);
		return;
	case DISTANCEToFixedPointOnPulley:	X= newVal.Dble();
		set_DistanceToFixedPointOnPulley(X);
		return;
	case DISTANCE:	X= newVal.Dble();
		set_Distance(X);
		return;
	case SPEED:	X= newVal.Dble();
		set_Speed(X);
		return;
	case TRAJECTORY_START_TIME:	X= newVal.Dble();
		set_TrackingStartTime(X);
		return;
	case TRAJECTORY_STOP_TIME:	X= newVal.Dble();
		set_TrackingStopTime(X);
		return;
	case BODY_SCALE_MOMENTUM:	X= newVal.Dble();
		set_ScaleMomentum(X);
		return;
	case BODY_SCALE_VELOCITY:	X= newVal.Dble();
		set_ScaleVelocity(X);
		return;
	case BODY_SCALE_ANGULARMOMENTUM:	X= newVal.Dble();
		set_ScaleAngularMomentum(X);
		return;
	case BODY_SCALE_FORCE:	X= newVal.Dble();
		set_ScaleForce(X);
		return;
	case BODY_SCALE_TORQUE:	X= newVal.Dble();
		set_ScaleTorque(X);
		return;
	case BODY_SCALE_ACCELERATION:	X= newVal.Dble();
		set_ScaleAcceleration(X);
		return;
	case DENSITY:	X= newVal.Dble();
		set_Density(X);
		return;
	case CHARGE:	X= newVal.Dble();
		set_Charge(X);
		return;
	case RADIUS:	X= newVal.Dble();
		set_Radius(X);
		return;
	case WIDTH:	X= newVal.Dble();
		set_Width(X);
		return;
	case HEIGHT:	X= newVal.Dble();
		set_Height(X);
		return;
	case DEPTH:	X= newVal.Dble();
		set_Depth(X);
		return;

	case ENERGY:
//		set_Energy();
		return;
	case KINETIC_ENERGY:
//		set_KineticEnergy();
		return;
	case COEFFICIENT_OF_RESTITUTION:	X= newVal.Dble();
		set_CoefficientOfRestitution(X);
		return;
	case COEFFICIENT_OF_KINETIC_FRICTION:	X= newVal.Dble();
		set_CoefficientOfKineticFriction(X);
		return;
	case COEFFICIENT_OF_STATIC_FRICTION:	X= newVal.Dble();
		set_CoefficientOfStaticFriction(X);
		return;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	case MultiBody_SPRING_CONSTANT:	X= newVal.Dble();
		set_MultiBody_SPRING_CONSTANT(X);
		return;
	case MultiBody_SPRING_LENGTH:	X= newVal.Dble();
		set_MultiBody_SPRING_LENGTH(X);
		return;
	case MultiBody_DAMPING_COEFFICIENT:	X= newVal.Dble();
		set_MultiBody_DAMPING_COEFFICIENT(X);
		return;
	case MultiBody_TARGET_ACCELERATION:	X= newVal.Dble();
		set_MultiBody_TARGET_ACCELERATION(X);
		return;
	case MultiBody_TORQUE:	X= newVal.Dble();
		set_MultiBody_TORQUE(X);
		return;
	case MultiBody_CONTROL:	b = newVal.boolVal;
		set_MultiBody_CONTROL(b);
		return;
	case MultiBody_LinkInitialJointDisplacement:	
		X= newVal.Dble();
		switch(m_inboundJoint.jointType) {
			case Joint1DOF::REV:
			case Joint1DOF::PRI:
				theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial[svIdx]= (m_inboundJoint.jointType != Joint1DOF::REV ? X : X * DEG_TO_RAD);
			break;
			case Joint1DOF::FLT:
			//	((RBConfig *) &*(//##theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial.end() - 13))->Tbi = Se3::ID;
			break;
		}
	theApp.theSystem.m_rigidBodyManager->RigidLinkJointStates = 	theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial;

	theApp.theSystem.m_rigidBodyManager->compXf(&theApp.theSystem.m_rigidBodyManager->RigidLinkJointStates.front());
	m_baseJoint->outboundLink->updatePoses();
	set_InitialState(*this);

	theApp.theSystem.UpdateTree();
	theApp.theSystem.DisplayData();
	//##m_pIView->ReDraw();
		return;
	case MultiBody_LinkInitialJointVelocity:	X= newVal.Dble();
		switch(m_inboundJoint.jointType) {
			case Joint1DOF::REV:
			case Joint1DOF::PRI:
				theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial[svIdx+1]= (m_inboundJoint.jointType != Joint1DOF::REV ? X : X * DEG_TO_RAD);
			break;
			case Joint1DOF::FLT:
			//	((RBConfig *) &*(//##theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial.end() - 13))->Tbi = Se3::ID;
			break;
		}
	theApp.theSystem.m_rigidBodyManager->RigidLinkJointStates = 	theApp.theSystem.m_rigidBodyManager->RigidLinkJointStatesInitial;

	theApp.theSystem.m_rigidBodyManager->compXf(&theApp.theSystem.m_rigidBodyManager->RigidLinkJointStates.front());
	m_baseJoint->outboundLink->updatePoses();
	set_InitialState(*this);

	theApp.theSystem.UpdateTree();
	theApp.theSystem.DisplayData();
	Fire_ViewChange();
		return;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	case CONSTRAINT_NumericalPositionDamping:
		X= newVal.Dble();
		{
			vector<CConstraint*>::iterator j;
			for (j=m_ListOfConstraints.begin();j != m_ListOfConstraints.end();++j)
			{
			//	CConstraint* pc = dynamic_cast<CConstraint*>(*j);
			//	if (pc)	pc->m_fNumericalPositionDamping = X;
			}
		}
		return;
	case CONSTRAINT_NumericalVelocityDamping:
		X= newVal.Dble();
		{
			vector<CConstraint*>::iterator j;
			for (j=m_ListOfConstraints.begin();j != m_ListOfConstraints.end();++j)
			{
			//	CConstraint* pc = dynamic_cast<CConstraint*>(*j);
			//	if (pc)	pc->m_fNumericalVelocityDamping = X;
			}
		}
		return;
///////////////////////////////////////////////////////////////////////////////
	case DRAWSTYLE:	l = newVal.lVal;
		set_DrawStyle(l);
		Fire_ViewChange();
		return;
///////////////////////////////////////////////////////////////////////////////
	case SHOW:	b = newVal.boolVal;
		set_Hide(b);
		return;
	case SHOW_TRAJECTORY:	b = newVal.boolVal;
		set_ShowTrajectory(b);
		return;
	case SHOW_P:	b = newVal.boolVal;
		set_ShowMomentum(b);
		return;
	case SHOW_L:	b = newVal.boolVal;
		set_ShowAngularMomentum(b);
		return;
	case SHOW_A:	b = newVal.boolVal;
		set_ShowAcceleration(b);
		return;
	case SHOW_FORCE:	b = newVal.boolVal;
		set_ShowForce(b);
		return;
	case SHOW_CONTACT_FORCE:	b = newVal.boolVal;
		set_ShowContactForce(b);
		return;
	case SHOW_TORQUE:	b = newVal.boolVal;
		set_ShowTorque(b);
		return;
	case SHOW_CONTACT_TORQUE:	b = newVal.boolVal;
		set_ShowContactTorque(b);
		return;
///////////////////////////////////////////////////////////////////////////////
	case SET_CONSTRAINT_STATUS:	
		b = newVal.boolVal;
		set_SubjectToActiveConstraint(b);
		return;
///////////////////////////////////////////////////////////////////////////////
	case SHOW_BODY_POTENTIAL:	
		b = newVal.boolVal;
		set_ShowUserDefinedPotential(b);
		if (m_bShowUserDefinedPotential)
		{
			CreateContourPlotBitmap();
/*
			CWinThread* pt = AfxBeginThread(CreateContourPlotBitmapProc,
											this,
											THREAD_PRIORITY_NORMAL, 0,
											CREATE_SUSPENDED);
			pt->ResumeThread();
	CThreadProgressDialog dlg(CreateContourPlotBitmapProc, this, 0, 128, " CString &title");
	dlg.DoModal();    
*/
		}
		return;
//	case BODY_POTENTIAL_RED:
//	case BODY_POTENTIAL_GREEN:
//	case BODY_POTENTIAL_BLUE:
///////////////////////////////////////////////////////////////////////////////
	case BODY_NORMALS:	b = newVal.boolVal;
		set_ShowNormals(b);
		return;
	case SHOW_X:	l = newVal.lVal;
		set_ShowPositionEveryNumSteps(l);
		return;
	case SHOW_V:	l = newVal.lVal;
		set_ShowVelocityEveryNumSteps(l);
		return;

///////////////////////////////////////////////////////////////////////////////
	case BODY_NAME:s = DupString(newVal.bstrVal);//newVal.bstrVal;
		set_Name(s); 
		return;
//	case FORCEX_NAME:s = newVal.bstrVal;
//		set_UserDefinedExternalForceX(s);
//		return;
//	case FORCEY_NAME:s = newVal.bstrVal;
//		set_UserDefinedExternalForceY(s);
//		return;
//	case FORCEZ_NAME:s = newVal.bstrVal;
//		set_UserDefinedExternalForceZ(s);
//		return;
	}

///////////////////////////////////////////////////////////////////////////////
//	case BODY_PLANE_ANGLE:
//	{
//		m_fAngleOfIncline = newVal;
//		Point3D& point = m_ptPointInPlane;
//		ConstrainToPlane(m_fAngleOfIncline, point.x,point.y,point.z);
//		Fire_ViewChange();
//	}
//	return;
///////////////////////////////////////////////////////////////////////////////

	long component = HIWORD(nEnum);
	if (component)
	{
				if (component==XCOMP) component = 0;
		else	if (component==YCOMP) component = 1;
		else	if (component==ZCOMP) component = 2;
		else	if (component==WCOMP) component = 3;
#define VEC_COMP( x ) {x[component] = newVal.Dble();}
	switch ( LOWORD(nEnum) )
	{
	case POSITION:			point = x;
							point[component] = newVal.Dble();
							set_Position(point, true);
		//x.x = (newVal.pdbl)[0], x.y = (newVal.pdbl)[1], x.z = (newVal.pdbl)[2];
		return;
	case MOMENTUM:			vec = p;vec[component] = newVal.Dble();
							set_Momentum(vec);
		return;
	case ANGULARMOMENTUM:	vec = L;vec[component] = newVal.Dble();
							set_AngularMomentum(vec);//GetAngularVelocity()
		return;
	case ROTATE:			q = newVal.pdbl;//double s_, x_, y_, z_
							set_Quat(q);
		return;
	case ORIENTATION:		X = (newVal.pdbl)[1], y = (newVal.pdbl)[2], z = (newVal.pdbl)[3], angle = (newVal.pdbl)[0];
							axis = Vector3D(X,y,z);
							PutOrientation(angle, axis);
		return;
/////////////////////////////////////////////////////////////////////////////

	case PLANE:		q = newVal.pdbl;//double s_, x_, y_, z_
					set_Plane(q);
		return;
	case CONTACT_POINT:
		hr=get_ContactPoint(point);point[component] = newVal.Dble();
		set_ContactPoint(point);
		return;
	case BODY_PLANE_POINT_IN_PLANE:
	case POINT_IN_PLANE:
		hr=get_PointInPlane(point);point[component] = newVal.Dble();
		set_PointInPlane(point);
		return;
	case VELOCITY:
		hr = get_Velocity(vec);vec[component] = newVal.Dble();
		set_Velocity(vec);
//		theSystem->constraintManager.CalculateConsistentVelocity(this);
		return;
	case BODY_FORCE:
		return;
	case BODY_TORQUE:
		return;
	case CONTACT_FORCE:
		return;
	case CONTACT_TORQUE:
		return;
	case COLOR:
		hr = get_Color(vec);
		vec[component] = newVal.Dble();
		set_Color(vec);
		return;
	case DAMPING_COEFFICIENT://times Vx, Vy, Vz
		hr = get_DampingCoefficient(vec);
		vec[component] = newVal.Dble();
		set_DampingCoefficient(vec);
		return;

	case BODY_NumericalPositionDamping:
		hr = get_NumericalPositionDamping(vec);
		vec[component] = newVal.Dble();
		set_NumericalPositionDamping(vec);
		return;
	case BODY_NumericalVelocityDamping:
		hr = get_NumericalVelocityDamping(vec);
		vec[component] = newVal.Dble();
		set_NumericalVelocityDamping(vec);
		return;

	case BODY_IMPULSE:
		return;
	case TRAJECTORY_COLOR:
		hr = get_PlotTrajectoryColor(vec);
		vec[component] = newVal.Dble();
		set_PlotTrajectoryColor(vec);
		return;
	case INCLINE_NORMALFORCE:
		return;
	case INCLINE_FRICTIONFORCE:
		return;
	case INCLINE_TORQUE:
		return;
	case BODY_SCALE_POSITION:
		hr = get_ScalePosition(vec);
		vec[component] = newVal.Dble();
		set_ScalePosition(vec);
		return;
	case ACCELERATION:
		return;
	}
	}
///////////////////////////////////////////////////////////////////////////////
	else
	{
	switch ( LOWORD(nEnum) )
	{
	case POSITION:			point = newVal.pdbl;
							set_Position(point, true);
//theSystem->constraintManager.CalculateConsistentPosition(this);//modifies x
		//x.x = (newVal.pdbl)[0], x.y = (newVal.pdbl)[1], x.z = (newVal.pdbl)[2];
		return;
	case MOMENTUM:			vec = newVal.pdbl;
							set_Momentum(vec);
		return;
	case ANGULARMOMENTUM:	vec = newVal.pdbl;
							set_AngularMomentum(vec);
		return;
	case ROTATE:			q = newVal.pdbl;//double s_, x_, y_, z_
							set_Quat(q);
		return;
	case ORIENTATION:		X = (newVal.pdbl)[1], y = (newVal.pdbl)[2], z = (newVal.pdbl)[3], angle = (newVal.pdbl)[0];
							axis = Vector3D(X,y,z);
							PutOrientation(angle, axis);
		return;
/////////////////////////////////////////////////////////////////////////////
	case PLANE:		q = newVal.pdbl;//double s_, x_, y_, z_
					set_Plane(q);
		return;
	case CONTACT_POINT:
		point = newVal.pdbl;
		set_ContactPoint(point);
		return;
//	case BODY_PLANE_POINT_IN_PLANE:
//	{
//		m_fAngleOfIncline = newVal;
//		Point3D& point = m_ptPointInPlane;
//		ConstrainToPlane(m_fAngleOfIncline, point.x,point.y,point.z);
//		Fire_ViewChange();
//	}
		return;
	case BODY_PLANE_POINT_IN_PLANE:
	case POINT_IN_PLANE:
		point = newVal.pdbl;
		set_PointInPlane(point);
		return;
	case VELOCITY:vec = newVal.pdbl;
		set_Velocity(vec);
//		theSystem->constraintManager.CalculateConsistentVelocity(this);
		return;
	case BODY_FORCE:
		return;
	case BODY_TORQUE:
		return;
	case CONTACT_FORCE:
		return;
	case CONTACT_TORQUE:
		return;
	case COLOR:vec = newVal.pdbl;
		set_Color(vec);
		return;
	case DAMPING_COEFFICIENT:vec = newVal.pdbl;
		set_DampingCoefficient(vec);
		return;


	case BODY_NumericalPositionDamping:vec = newVal.pdbl;
		set_NumericalPositionDamping(vec);
		return;
	case BODY_NumericalVelocityDamping:vec = newVal.pdbl;
		set_NumericalVelocityDamping(vec);
		return;


	case BODY_IMPULSE:
		return;
	case TRAJECTORY_COLOR:vec = newVal.pdbl;
		set_PlotTrajectoryColor(vec);
		return;
	case INCLINE_NORMALFORCE:
		return;
	case INCLINE_FRICTIONFORCE:
		return;
	case INCLINE_TORQUE:
		return;
	case BODY_SCALE_POSITION:vec = newVal.pdbl;
		set_ScalePosition(vec);
		return;
	case ACCELERATION:
		return;
	}
	}
//	map <long, double*>::iterator i = m_mapEnumToVarPtr.find(nEnum);
//	if (i != m_mapEnumToVarPtr.end() && newVal.vt == DATUM_R8) *i->second = newVal.Dble();
}
#endif
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Position(const Point3D& point)
{
		const double tol = 0.1e-4;
		Point3D prevPt(x);
		x = point;
		if (m_bConstraintedToPlane)
		{
			double c = fabs(cos(m_fAngleOfIncline));
			double s = sin(m_fAngleOfIncline);
			if (m_fAngleOfIncline > PI_DIVIDED_BY_2) s *= -1;
			Vector3D normalToIncline(-s,c,0);
			Vector3D rel = point - m_ptPointInPlane;
			double constraint = rel*normalToIncline;
			if (fabs(constraint) > tol)
			{
/*
				Point3D pt(point);
				CConstraintedPosition cp(pt, m_fRadius);
				cp.m_fAngleOfIncline	= m_fAngleOfIncline;
				cp.m_pointInPlane		= m_ptPointInPlane;
				cp.m_normalToIncline	= normalToIncline;
				int ret = cp.DoModal();
				if (IDCANCEL == ret)  {x = prevPt; return;}
				if (IDOK == ret) x = pt;
*/
			}
		}
		m_stateInitial.x = x;
		m_statePrevious = m_stateInitial;
		PQP_Update();
	//	SetScreenRect();put in CRigidBody::OnDrawState()
///////////////////////////////////////////////////////////////////////////////
	//	CVectorAnalysis* pva = dynamic_cast<CVectorAnalysis*>(this);
	//	if (pva) pva->Fire_PositionChanged();
	//	theSystem->ReDraw();
	bool pva = type==T_VECTOR_ANALYSIS_OBJECT;
	if (pva)
	{
		vector<CRigidBody *>::iterator i;
		for (i=m_bodiesToAdvise.begin();i!=m_bodiesToAdvise.end();++i)
		{
			(*i)->OnAdvisePositionChanged();
		}

	}

	CRigidBodyManager*	rigidBodyManager	= theApp.GetRigidBodyManager();
	CFieldsManager*		fieldsManager		= theApp.GetFieldsManager();

	rigidBodyManager->PQP_Update();
	fieldsManager->Clear();
	CSystem* m_system = theApp.GetSystem();
	m_system->UpdateTree();
	m_system->DisplayData();
	Fire_ViewChange();

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::OnAdvisePositionChanged()
{
	switch(m_role)
	{
	case _sum:
		set_Position(m_bodiesDependentOn[0]->x + m_bodiesDependentOn[1]->x);
		break;
	case _difference:
		set_Position(m_bodiesDependentOn[0]->x - m_bodiesDependentOn[1]->x);
		break;
	case _crossProduct:
		set_Position(m_bodiesDependentOn[0]->x .vectorProd( m_bodiesDependentOn[1]->x));
		break;
	default: return;
	}
}
///////////////////////////////////////////////////////////////////////////////
// MassInverse
long CRigidBody::set_MassInverse(const double& newValue)
{
	const double BIG_MASS = 9.e99;
	m_fMassInverse = newValue;
	if (m_fMassInverse == 0.0)
	{
		m_fMass = BIG_MASS;
		m_mat3InitialInverseInertiaTensor = Mat3::ZERO;
		m_mat3InverseInertiaTensor = Mat3::ZERO;
	}
	else m_fMass = 1.0/m_fMassInverse;
//##	ComputeInertia();
	Vector3D velocity;
	get_Velocity(velocity);
	set_Velocity(velocity);
	std::wstring bstr(L"mass recomputed: mass = ");
	//_variant_t v(m_fMass);
	std::wstring bstr2 = std::to_wstring(m_fMass);// v;
	bstr += bstr2;
	CFieldsManager*		fieldsManager		= theApp.GetFieldsManager();
	fieldsManager->Clear();
	Fire_ViewChange();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
// Mass
long CRigidBody::set_Mass(const double& newValue)
{
	const double BIG_MASS = 9.e99;
	m_fMass = fabs(newValue);
	if (m_fMass>BIG_MASS)
	{
		m_fMassInverse = 0;
	}
	else m_fMassInverse = 1.0/m_fMass;
	return set_MassInverse(m_fMassInverse);
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::get_Mass( double& getValue )
{
	const double BIG_MASS = 9.e99;
	getValue = m_fMass = m_fMassInverse > 0.0? 1.0/m_fMassInverse:BIG_MASS;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Quat(const Quat& quat)
{
	q.set(quat);
	q.normalize();	// just to be sure.
	m_stateInitial.q = q;
//	PQP_Update();called by rigidBodyManager->PQP_Update(true);
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	rigidBodyManager->PQP_Update();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Quat(double angle, const Vector3D& axis)
{
	Quat quat;
	quat.set(angle*DEG_TO_RAD, axis);
	set_Quat(quat);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Radius(const double& newValue) {
		m_fRadius = newValue;
		DefineShape();
		CalculateMassFromGeometry();
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Width(const double& newValue) {
		m_fWidth = newValue;
		DefineShape();
		CalculateMassFromGeometry();
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Height(const double& newValue) {
		m_fHeight = newValue;
		DefineShape();
		CalculateMassFromGeometry();
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Depth(const double& newValue) {
		m_fDepth = newValue;
		DefineShape();
		CalculateMassFromGeometry();
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Momentum(const Vector3D& newValue)
{
	p = newValue;
	m_stateInitial.p = p;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Velocity(const Vector3D& newValue)
{
	if (m_fMassInverse==0.0) return -1;
	m_vecVelocity = newValue;
	set_Momentum(m_vecVelocity/m_fMassInverse);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::get_Velocity( Vector3D& getValue )
{
	m_vecVelocity = m_fMassInverse*p;
	getValue = m_vecVelocity;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_Color(const Vector3D& newValue)
{
	m_vecColor = newValue;
	if (m_vecColor.x>1.0) m_vecColor.x=1.0;
	if (m_vecColor.y>1.0) m_vecColor.y=1.0;
	if (m_vecColor.z>1.0) m_vecColor.z=1.0;
	if (m_vecColor.x<0.0) m_vecColor.x=0.0;
	if (m_vecColor.y<0.0) m_vecColor.y=0.0;
	if (m_vecColor.z<0.0) m_vecColor.z=0.0;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_AngularMomentum(const Vector3D& newValue)
{
	L = newValue;
	m_stateInitial.L = L;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_AngularVelocity(const Vector3D& newValue)
{
	m_vecAngularVelocity = newValue;
	Mat3 Iinv;
	get_InverseInertiaTensor( Iinv );
	Iinv.invert();
	set_AngularMomentum(Iinv*m_vecAngularVelocity);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::get_AngularVelocity( Vector3D& getValue )
{
	Mat3 Iinv;
	get_InverseInertiaTensor( Iinv );
	m_vecAngularVelocity = Iinv*L;
	getValue = m_vecAngularVelocity;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_DisableSelect(const bool& newValue)
{
	m_bDisableSelect = newValue;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_InitialState(const State& newValue)
{
	m_stateInitial = newValue;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

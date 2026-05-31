// SetGetSystem.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"

#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "parser.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "StackMachine.h"

#include "LeftView.h"
#include "SolveItDoc.h"
#include "SolveItView.h"
#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
//#include "parser_2020.h" /* for token definitions and yylval */
#include "extend_enum_yytokentype.h"
#include <limits.h>
#include "GLView.h"
//#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Scatt1D.h"
#include "QM/DataQM_Stationary2D.h"
#include "QM/DataQM_Scatt2D.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
static const objects true_object(T_TRUE);
static const objects false_object(T_FALSE);
//static const objects on_object(T__ON);
//static const objects off_object(T_OFF);
static const objects infinity_object(T_INFINITY);
static const objects Nash_Chen_object(T_Nash_Chen);
static const objects PeriodicCayley_object(T_PeriodicCayley);
static const objects Cayley_object(T_Cayley);
static const objects radial_object(T_RADIAL);
static const objects Cartesian_object(T_Cartesian);
/*
32-bit uint
4294967295
42 94 96 72 95
4294 967 295

///////////////////////////////////////////////////////////////////////////////
static const objects true_object((int)T_TRUE);
static const objects false_object((int)T_FALSE);
//static const objects on_object(T__ON);
//static const objects off_object(T_OFF);
static const objects infinity_object((int)T_INFINITY);
static const objects Nash_Chen_object((int)T_Nash_Chen);
static const objects PeriodicCayley_object((int)T_PeriodicCayley);
static const objects Cayley_object((int)T_Cayley);
static const objects radial_object((int)T_RADIAL);
static const objects Cartesian_object((int)T_Cartesian);
*/

#define CONCAT2(s1,s2) s1 ## s2
#define new_case2(s1,s2) \
	case CONCAT2(s1,s2):

#define CONCAT3(s1,s2,s3) s1 ## s2 ## s3
#define new_case3(s1,s2,s3) \
	case CONCAT3(s1,s2,s3):

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//#define CONCAT(s1,s2) #s1 ## #s2

#define __h(x) (x)
//#define sh(x) shift__item(__h(x)-__h(SolveIt_First_System_Token))
#define sh(x) (x - SolveIt_First_System_Token)

//#define CONCAT2(s1,s2) __h(sh(__h(s1))) ## __h(sh(__h(s2)))
// - SolveIt_First_System_Token
#define pos_case(X, w) \
new_case2( T_POSITION, T_ ## X ## COMP) \
		val = obj.rbPtr->x.w;\
		break;

#define pos_cases \
		pos_case(X,x) \
		pos_case(Y,y) \
		pos_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define mom_case(X, w) \
	new_case2( T_MOMENTUM, T_ ## X ## COMP){\
	val=obj.rbPtr->p.w ;\
	break;

#define mom_cases \
		mom_case(X,x) \
		mom_case(Y,y) \
		mom_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define vel_case(X, w) \
	new_case2( T_VELOCITY, T_ ## X ## COMP) \
	Vector3D v;\
	obj.rbPtr->get_Velocity(v);\
	val=v.w ;}\
	break;

#define vel_cases \
		vel_case(X,x) \
		vel_case(Y,y) \
		vel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define color_case(X, w) \
	new_case2( T_COLOR, T_ ## X ## COMP){\
	Vector3D v=obj.rbPtr->m_vecColor;\
	val=v.w ;}\
	break;

#define color_cases \
		color_case(X,x) \
		color_case(Y,y) \
		color_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define amom_case(X, w) \
	new_case2( T_ANGULARMOMENTUM, T_ ## X ## COMP){\
	Vector3D v=obj.rbPtr->L;\
	val=v.w ;}\
	break;

#define amom_cases \
		amom_case(X,x) \
		amom_case(Y,y) \
		amom_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define avel_case(X, w) \
	new_case2( T_ANGULARVELOCITY, T_ ## X ## COMP){\
	Vector3D v;\
	obj.rbPtr->get_AngularVelocity(v);\
	val=v.w ;}\
	break;

#define avel_cases \
		avel_case(X,x) \
		avel_case(Y,y) \
		avel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define ext_grav_case(X, w) \
	new_case2( T_GRAVITATIONAL_FIELD, T_ ## X ## COMP){\
	Vector3D v=fieldsManager->m_ExternalGravitationalField;\
	val=v.w ;}\
	break;

#define ext_grav_cases \
		ext_grav_case(X,x) \
		ext_grav_case(Y,y) \
		ext_grav_case(Z,z)
///////////////////////////////////////////////////////////////////////////////

void CSystem::Get(const objects& obj, unsigned int _enum, objects& val)
{
try {
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	switch(_enum)
	{



	case T_RUN:
/////////////////////////////////////////////////////////////////////////////
		m_bSingleStepping	= false;
		m_bSingleStep_Mode	= false;
		m_bRunning			= true;
///////////////////////////////////////////////////////////////////////////////
		Run();
		val = L"OK";
		break;


	case T_STOP:
		Stop();
		val = L"OK";
		break;
	case T_CLEAR:
		Clear();
		val = L"OK";
		break;
	case T_RESET:
	//	Initialize();performed when bye code is NOT being executed
		val = L"OK";
		break;
	case T_GRAVITATIONAL_COUPLING:
		val = fieldsManager->m_GravitationalCoupling;
		break;
	case T_GRAVITATIONAL_FIELD:
		val=fieldsManager->m_ExternalGravitationalField	;
		break;
	ext_grav_cases


	case T_LENNARD_JONES:
		val = fieldsManager->m_fLennardJones;
		break;
	new_case2( T_LENNARD_JONES, T_XCOMP)
		val = fieldsManager->m_fLennardJones.x;
		break;
	new_case2( T_LENNARD_JONES, T_YCOMP)
		val = fieldsManager->m_fLennardJones.y;
		break;
	new_case2( T_LENNARD_JONES, T_ZCOMP)
		val = fieldsManager->m_fLennardJones.z;
		break;


	case T_CURRENTLOOP_MAGNETICFIELD_STRENGTH:
		val = fieldsManager->m_currentLoopMagneticFieldStrength;
		break;

	
		
	case T_ELECTRIC_FIELD:
		val = fieldsManager->m_ExternalElectricField;
		break;
	new_case2( T_ELECTRIC_FIELD, T_XCOMP)
		val = fieldsManager->m_ExternalElectricField.x;
		break;
	new_case2( T_ELECTRIC_FIELD, T_YCOMP)
		val = fieldsManager->m_ExternalElectricField.y;
		break;
	new_case2( T_ELECTRIC_FIELD, T_ZCOMP)
		val = fieldsManager->m_ExternalElectricField.z;
		break;




	case T_MAGNETIC_FIELD:
		val = fieldsManager->m_ExternalMagneticField;
		break;
	new_case2( T_MAGNETIC_FIELD, T_XCOMP)
		val = fieldsManager->m_ExternalMagneticField.x;
		break;
	new_case2( T_MAGNETIC_FIELD, T_YCOMP)
		val = fieldsManager->m_ExternalMagneticField.y;
		break;
	new_case2( T_MAGNETIC_FIELD, T_ZCOMP)
		val = fieldsManager->m_ExternalMagneticField.z;
		break;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////		
	new_case2(T_STATE , T_ENERGY)
		switch(m_worldDimension)
	{
		case one:
		val = energy_eigenvalue;
			break;
		case two:
		val = data__QM_Stationary2D->energy_eigenvalue;
			break;
		case three:
		val = 0.0;
			break;
	}
		break;
	new_case2(T_STATE , T_EigenNumber)
		switch(m_worldDimension)
	{
		case one:
		val = eigenvalueIndex;
			break;
		case two:
		val = data__QM_Stationary2D->m_nSeekEigenvalues;
			break;
		case three:
		val = 0.0;
			break;
	}
		break;
///////////////////////////////////////////////////////////////////////////////
	new_case2(T_STATE , T_SCALE)
		val = pGLView->m_fScaleWave;
				break;
///////////////////////////////////////////////////////////////////////////////		
///////////////////////////////////////////////////////////////////////////////		
	new_case2(T_STATE , T_MASS)
		switch(m_worldDimension)
	{
		case one:
			val = M;
			break;
		case two:
			break;
		case three:
			break;
	}
		break;
///////////////////////////////////////////////////////////////////////////////		
	new_case2( T_Scattering, T_FourierTransform)
{

	wchar_t buf[256];
	{
		/*istrstream*/ std::wstringstream iss(buf, sizeof(buf));
		double k;
		if ( !(iss >> k).fail() )
		{
			/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf));
			double kx=0, fft=0;
			data__QM_Scatt1D->get_WaveNumber(&kx);
			data__QM_Scatt1D->get_FourierTransformOfWavePacketAtK(kx, k, &fft);
			oss.seekp(0);
			oss << fft << ends;
		}
	}
		val = buf;
}
		break;

	new_case2( T_Scattering, T_WaveNumber)
		data__QM_Scatt1D->put_WaveNumber(val);
		break;
/*
	new_case2( T_Scattering, T_InitialPacketSpread)
		val = "null";
		break;

	new_case2( T_Scattering, T_Center_of_Gaussian)
		val = "null";
		break;

	new_case2( T_Gaussian, T_Spread)
		data__QM_Scatt1D->put_InitialSpread(val);
		break;

	new_case2( T_Gaussian, T_Center)
		data__QM_Scatt1D->put_CenterOfGaussian(val);
		break;
*/


	new_case2( T_Scattering, T_Method)
	new_case2( T_Stationary, T_Method)
//val = m_SolveMethod == radial? radial_object:
//	(m_SolveMethod == Cartesian? Cartesian_object:
//	(m_SolveMethod == Cayley? :Cayley_object
//	(m_SolveMethod == Nash_Chen? Nash_Chen_object:
//	(m_SolveMethod == PeriodicCayley? PeriodicCayley_object:"null"))));
	val = 
		m_SolveMethod == direct? L"direct":
	(	m_SolveMethod == iterative? L"iterative":
	(	m_SolveMethod == radial? L"radial":
	(	m_SolveMethod == pseudoCartesian? L"pseudoCartesian":
	(	m_SolveMethod == Cartesian? L"Cartesian":
	(	m_SolveMethod == PeriodicCayley? L"periodicCayley":
	(	m_SolveMethod == Nash_Chen? L"Nash_Chen":
	(	m_SolveMethod == Cayley? L"Cayley":L"null")))))));
		
		break;

	case T_NODES:
		val = pGLView->m_NDATA;
		break;
///////////////////////////////////////////////////////////////////////////////

	new_case2( T_TITLE, T_XCOMP )
		val = ToWideString(m_xStr.c_str());
		break;

	new_case2( T_TITLE, T_YCOMP )
		val = ToWideString(m_yStr.c_str());
		break;

	new_case2( T_TICMARK, T_XCOMP )
		val = pGLView->m_num_tic_marks_x;//m_tic_mark_spacingStr;
		break;

	new_case2( T_TICMARK, T_YCOMP )
		val = pGLView->m_num_tic_marks_y;//m_tic_mark_spacingStr;
		break;
	
	


	new_case2( T_SCENE, T_SYSTEM_LEFT)
		val = pGLView->glFrustum_left;
		break;
	new_case2( T_SCENE, T_SYSTEM_RIGHT)
		val = pGLView->glFrustum_right;
		break;
	new_case2( T_SCENE, T_SYSTEM_TOP)
		val = pGLView->glFrustum_top;
		break;
	new_case2( T_SCENE, T_SYSTEM_BOTTOM)
		val = pGLView->glFrustum_bottom;
		break;
	new_case2( T_SCENE, T_SYSTEM_NEAR)
		val = pGLView->glFrustum_znear;
		break;
	new_case2( T_SCENE, T_SYSTEM_FAR)
		val = pGLView->glFrustum_zfar;
		break;

	new_case2( T_TRANSLATE, T_XCOMP)
		val = pGLView->m_fTransX;
		break;
	new_case2( T_TRANSLATE, T_YCOMP)
		val = pGLView->m_fTransY;
		break;
	new_case2( T_TRANSLATE, T_ZCOMP)
		val = pGLView->m_fTransZ;
		break;
///////////////////////////////////////////////////////////////////////////////

	case T_SYSTEM_LEFT:		val = m_fRigidBoundary_left;
		break;
	case T_SYSTEM_RIGHT:	val = m_fRigidBoundary_right;
		break;
	case T_SYSTEM_TOP:		val = m_fRigidBoundary_top;
		break;
	case T_SYSTEM_BOTTOM:	val = m_fRigidBoundary_bottom;
		break;
	case T_SYSTEM_NEAR:		val = m_fRigidBoundary_front;
		break;
	case T_SYSTEM_FAR:		val = m_fRigidBoundary_back;


	case T_BOUNDING_BOX:
	{
		VectorND<6>v;
		v.v[0]=pGLView->glFrustum_left;
		v.v[1]=pGLView->glFrustum_right;
		v.v[2]= pGLView->glFrustum_bottom;
		v.v[3]= pGLView->glFrustum_top;
		v.v[4]= pGLView->glFrustum_znear;
		v.v[5]= pGLView->glFrustum_zfar;
		val = v;
	}
		break;

	case T_TRANSLATE:
	{
		Vector3D v(pGLView->m_fTransX,pGLView->m_fTransY,pGLView->m_fTransZ);
		val = v;
	}
		break;

	case T_ROTATE:
		break;
	case T_CENTRAL_FORCE:
		val = obj.rbPtr->m_fCentralForce;
		break;


	new_case2( T_SYSTEM_Background, T_SHOW)
		{
			val = pGLView->m_bShowBackground?L"true":L"false";
		}
		break;

	case T_SYSTEM_TIME_STEP:
		val = m_dt;
		break;
	case T_SYSTEM_TIME:
		val = m__t;
		break;
	new_case2(T_SYSTEM_TIME , T_STOP)
		val = m_fStopTime;
		break;
	case T_SYSTEM_TIMER_INTERVAL:		
		val = int(m_uMilliseconds);
		break;



	case T_COLLISIONDETECTION:
	//	val =m_bCollisionDetectionEnabled?true_object:false_object;
		val =m_bCollisionDetectionEnabled?L"true":L"false";
		break;
///////////////////////////////////////////////////////////////////////////////
	case T_DELETE:
		val = obj.rbPtr->name+L" removed";
		Stop();
		rigidBodyManager->Remove(obj.rbPtr->name);
		break;
///////////////////////////////////////////////////////////////////////////////
	case T_NAME:
		val = obj.rbPtr->name;
		break;
	case T_IMASS:
		val = obj.rbPtr->m_fMassInverse;
		break;
	case T_INVERSE_INERTIA_TENSOR:
		break;
	case T_CHARGE:
		val=obj.rbPtr->m_fCharge;
		break;
	case T_POSITION:
		val = obj.rbPtr->x;
		break;
		pos_cases
	case T_MOMENTUM:
		val = obj.rbPtr->p;
		break;
		mom_cases
	case T_ANGULARMOMENTUM:
		val = obj.rbPtr->L;
		break;
		amom_cases
	case T_ORIENTATION:
	{
	}
		break;
	case T_FORCE:
		val=obj.rbPtr->m_vecForce	;
		break;
	case T_TORQUE:
		val=obj.rbPtr->m_vecTorque	;
		break;
	case T_BODY_FORCE_EXTERNAL:
		break;
//		force_cases
	case T_BODY_FORCE_INTERNAL:
		break;


	case T_contactFORCE:
		if (obj.type == __int)
		{
		}
		else
		{
		val=obj.rbPtr->m_vecContactForce;
		}
	break;

	case T_contactTORQUE:
		if (obj.type == __int)
		{
		}
		else
		{
		val=obj.rbPtr->m_vecContactTorque;
		}
	break;


	new_case2( T_contactFORCE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		val =obj.rbPtr->m_bShowContactForce?L"true":L"false";
		}
	break;

	new_case2( T_FORCE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		val =obj.rbPtr->m_bShowForce?L"true":L"false";
		}
	break;
	new_case2( T_Constraint, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		val =obj.rbPtr->m_bShowQuadraticConstraint?L"true":L"false";
		}
	break;


	case T_VELOCITY:{
		Vector3D v;
		obj.rbPtr->get_Velocity(v);
		val = v;}
		break;
		vel_cases
	case T_ANGULARVELOCITY:
	{
		Vector3D v;
		obj.rbPtr->get_AngularVelocity(v);
		val=v;
	}
		break;
		avel_cases
	case T_ACCELERATION:
		break;
	case T_MASS:
	{
		double getValue=0;
		obj.rbPtr->get_Mass(getValue);
		val = getValue;
	}
		break;
	case T_DENSITY:
		val = obj.rbPtr->m_fDensity;
		break;
	case T_COLOR:
		val = obj.rbPtr->m_vecColor;
		break;
		color_cases
	case T_DRAWSTYLE:
		{
			long Value=0;
			obj.rbPtr->get_DrawStyle(Value);
			val = (int)Value;
		}

		break;
	case T_SCALE:
		break;
	case T_RADIUS:
		val = obj.rbPtr->m_fRadius;
		break;
	case T_WIDTH:
		val = obj.rbPtr->m_fWidth;
		break;
	case T_HEIGHT :
		val = obj.rbPtr->m_fHeight;
		break;
	case T_DEPTH:
		val = obj.rbPtr->m_fDepth;
		break;


	case T_COEFFICIENT_OF_STATIC_FRICTION:
		val	= obj.rbPtr->m_fCoefficientOfStaticFriction;
		break;
	case T_COEFFICIENT_OF_KINETIC_FRICTION:
		val	= obj.rbPtr->m_fCoefficientOfKineticFriction;
		break;
	case T_COEFFICIENT_OF_RESTITUTION:
		val	= obj.rbPtr->m_fCoefficientOfRestitution;
		break;
	case T_DAMPING_COEFFICIENT:
		val	= obj.rbPtr->m_vecDampingCoefficient;
		break;
	case T_ENERGY:
		break;
	case T_KINETIC_ENERGY:
		break;
	case T_POTENTIAL_ENERGY:
		break;
	case T_POTENTIAL:
		break;
	case T_DISTANCE:
		break;
	case T_SPEED:
		{
			Vector3D v;
			obj.rbPtr->get_Velocity( v );
			val	= v.norm();
		}
		break;

	case T_RECORD:
		break;
	case T_TRAJECTORY:
		break;
	case T_ANALYZE:
		break;
	case T_PERIOD:
		break;
	case T_SHOW:
		break;
	case T_CONSTRAINT_STATE:
		break;
	case T_CONNECT:
		break;
	case T_PULLEY:
		break;


	new_case2(T_quadrant,T_ONE)
	{
//		const double radiusOfPulley=0.1;
	//	const_cast<CRigidBody*>(obj.rbPtr)->m_nConnectedInQuadrant=CRigidBody::___Quadrant_One;//CRigidBody::InitialAngleInQuadrant(1);
		obj.rbPtr->set_InitialAngleInQuadrant(CRigidBody::___Quadrant_One);//CRigidBody::InitialAngleInQuadrant(1);
//		InitializePulleyConnection(radiusOfPulley, 1);
		val = obj.rbPtr;
	}
	break;
	new_case2(T_quadrant,T_TWO)
	{
	//	const_cast<CRigidBody*>(obj.rbPtr)->m_nConnectedInQuadrant=CRigidBody::___Quadrant_Two;//CRigidBody::InitialAngleInQuadrant(2);
		obj.rbPtr->set_InitialAngleInQuadrant(CRigidBody::___Quadrant_Two);//CRigidBody::InitialAngleInQuadrant(2);
		val = obj.rbPtr;
	}
	break;
	new_case2(T_quadrant,T_THREE)
	{
	//	const_cast<CRigidBody*>(obj.rbPtr)->m_nConnectedInQuadrant=CRigidBody::___Quadrant_Three;//CRigidBody::InitialAngleInQuadrant(3);
		obj.rbPtr->set_InitialAngleInQuadrant(CRigidBody::___Quadrant_Three);//CRigidBody::InitialAngleInQuadrant(3);
		val = obj.rbPtr;
	}
	break;
	new_case2(T_quadrant,T_FOUR)
	{
	//	const_cast<CRigidBody*>(obj.rbPtr)->m_nConnectedInQuadrant=CRigidBody::___Quadrant_Four;//CRigidBody::InitialAngleInQuadrant(4);
		obj.rbPtr->set_InitialAngleInQuadrant(CRigidBody::___Quadrant_Four);//CRigidBody::InitialAngleInQuadrant(4);
		val = obj.rbPtr;
	}
	break;

	case T_PLANE:
		break;
	case T_ANGLE:
		break;
	case T_POINT_IN_PLANE:
		break;
	case T_LocalVectorToLab:
		break;
	case T_LabVectorToLocal:
		break;
	case T_LabPointToLocal:
		break;
	case T_LocalPointToLab:
		break;

	case T_SYSTEM_MultiBody:
		break;

	new_case2(T_MultiBody_Link_Joint,T_MultiBody_SPRING_LENGTH)
	{
		double getValue=0;
		obj.rbPtr->get_MultiBody_SPRING_LENGTH(getValue);
		val = getValue;
	}
		break;
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_SPRING_CONSTANT)
	{
		double getValue=0;
		obj.rbPtr->get_MultiBody_SPRING_CONSTANT(getValue);
		val = getValue;
	}
		break;
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_CONTROL)
	{
		bool getValue=0;
		obj.rbPtr->get_MultiBody_CONTROL(getValue);
		val = getValue?1:0;
	}
		break;
	case T_MultiBody_Link_Joint:
		break;
		
	new_case2(T_MultiBody_Link_Joint,T_DISPLACEMENT)
	{
	}
		break;
	new_case2(T_MultiBody_Link_Joint,T_VELOCITY)
	{
	}
		break;

	case T_JointType_FIXED:
		break;
	case T_JointType_REVOLUTE:
		break;
	case T_JointType_PRISMATIC:
		break;
	case T_JointType_SCREW:
		break;
	case T_JointType_CYLINDRICAL:
		break;
	case T_JointType_UNIVERSAL:
		break;
	case T_JointType_SPHERICAL:
		break;
	case T_JointType_FLOATING:
		break;
	case T_JointType_SPRING:
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////
	catch (std::runtime_error* exc)
	{
		::MessageBox(0, GetWC(exc->what()), L"CSystem::Get:Error", MB_OK);
		return ;
	}
	catch(struct _exception *except)
	{
		::MessageBox(0, GetWC(except->name), L"CSystem::Get:Error", MB_OK);
		return ;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0, str, L"CSystem::Get:Error", MB_OK);
		return ;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0, L"unknown", L"CSystem::Get:Error", MB_OK);
		return ;
	}
}
///////////////////////////////////////////////////////////////////////////////

#undef pos_case
#undef pos_cases

#undef force_case
#undef force_cases


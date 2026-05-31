// SetGetSystem.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma optimize( "", off )

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
#include "RigidBody/ConstraintManager.h"
//#include "parser_2020.h" /* for token definitions and yylval */
#include "extend_enum_yytokentype.h"
#include <limits.h>
#include "GLView.h"
//#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Scatt1D.h"
#include "QM/QMStationary1D.h"
#include "QM/QMStationary2D.h"

#include "Examples/Superposition/Superposition.h"
#include "Examples/Superposition/WaveNumbers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const wchar_t * approx(const wstring& strDataFileName);
namespace {
	bool ParseTimedCommand(const std::wstring& source, double& time, std::wstring& command)
	{
		std::wstring mutableSource = source;
		wchar_t seps[] = L";";
		wchar_t* context = 0;
		wchar_t* token = wcstok_s(mutableSource.data(), seps, &context);
		if (token == 0)
		{
			return false;
		}

		std::wstringstream iss(token);
		if ((iss >> time).fail())
		{
			return false;
		}

		token = wcstok_s(NULL, seps, &context);
		if (token == 0)
		{
			return false;
		}

		command.assign(token);
		command += L";";
		return true;
	}
}
///////////////////////////////////////////////////////////////////////////////
#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 
///////////////////////////////////////////////////////////////////////////////
static const objects true_object( static_cast<int>(T_TRUE) );
static const objects false_object(static_cast<int>(T_FALSE) );
//static const objects on_object(T__ON);
//static const objects off_object(T_OFF);

static const objects infinity_object(static_cast<int>(T_INFINITY) );
static const objects minus_infinity_object(static_cast<int>(T_MINUS_INFINITY) );

static const objects Nash_Chen_object( static_cast<int>(T_Nash_Chen) );
static const objects PeriodicCayley_object(static_cast<int>(T_PeriodicCayley) );
static const objects Cayley_object(static_cast<int>(T_Cayley) );
static const objects radial_object(static_cast<int>(T_RADIAL) );
static const objects pseudoCartesian_object(static_cast<int>(T_pseudoCartesian) );
static const objects Cartesian_object(static_cast<int>(T_Cartesian) );
//static const objects one_object(T_ONE);
//static const objects two_object(T_TWO);
//static const objects three_object(T_THREE);
/*
32-bit uint
4294967295
42 94 96 72 95
4294 967 295
*/

#define CONCAT2(s1,s2) s1 ## s2
#define new_case2(s1,s2) \
	case CONCAT2(s1,s2):

#define CONCAT3(s1,s2,s3) s1 ## s2 ## s3
#define new_case3(s1,s2,s3) \
	case CONCAT3(s1,s2,s3):

///////////////////////////////////////////////////////////////////////////////
#define pos_case(X, w) \
	new_case2( T_POSITION, T_ ## X ## COMP){\
	Point3D pt=obj.rbPtr->x;\
	pt.w = val;\
	obj.rbPtr->set_Position(pt);}\
	break;

#define pos_cases \
		pos_case(X,x) \
		pos_case(Y,y) \
		pos_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define mom_case(X, w) \
	new_case2( T_MOMENTUM, T_ ## X ## COMP){\
	Vector3D v=obj.rbPtr->p;\
	v.w = val;\
	obj.rbPtr->set_Momentum(v);}\
	break;

#define mom_cases \
		mom_case(X,x) \
		mom_case(Y,y) \
		mom_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define vel_case(X, w) \
	new_case2( T_VELOCITY, T_ ## X ## COMP){\
	Vector3D v;\
	obj.rbPtr->get_Velocity(v);\
	v.w = val;\
	obj.rbPtr->set_Velocity(v);}\
	break;

#define vel_cases \
		vel_case(X,x) \
		vel_case(Y,y) \
		vel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define color_case(X, w) \
	new_case2( T_COLOR, T_ ## X ## COMP){\
		if (obj.type == __int) \
		{\
	switch(T_ ## X ## COMP) \
	{\
	case T_XCOMP:\
			SetUserDefinedName(GetC(*val.sPtr), __Red);\
		break;\
	case T_YCOMP:\
			SetUserDefinedName(GetC(*val.sPtr), __Green);\
		break;\
	case T_ZCOMP:\
			SetUserDefinedName(GetC(*val.sPtr), __Blue);\
		break;\
	}\
		}\
		else\
		{\
	Vector3D v=obj.rbPtr->m_vecColor;\
	v.w = val;\
	obj.rbPtr->set_Color(v);}\
		}\
	break;

#define color_cases \
		color_case(X,x) \
		color_case(Y,y) \
		color_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define amom_case(X, w) \
	new_case2( T_ANGULARMOMENTUM, T_ ## X ## COMP){\
	Vector3D v=obj.rbPtr->L;\
	v.w = val;\
	obj.rbPtr->set_AngularMomentum(v);}\
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
	v.w = val;\
	obj.rbPtr->set_AngularVelocity(v);}\
	break;

#define avel_cases \
		avel_case(X,x) \
		avel_case(Y,y) \
		avel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define ext_grav_case(X, w) \
	new_case2( T_GRAVITATIONAL_FIELD, T_ ## X ## COMP){\
	Vector3D v=fieldsManager->m_ExternalGravitationalField;\
	v.w = val;\
	fieldsManager->m_ExternalGravitationalField=v;}\
	break;

#define ext_grav_cases \
		ext_grav_case(X,x) \
		ext_grav_case(Y,y) \
		ext_grav_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define lennardjones_case(X, w) \
	new_case2( T_LENNARD_JONES, T_ ## X ## COMP){\
	Vector3D v=fieldsManager->m_fLennardJones;\
	v.w = val;\
	fieldsManager->Set_LENNARD_JONES(v);}\
	break;


#define lennardjones_cases \
		lennardjones_case(X,x) \
		lennardjones_case(Y,y) \
		lennardjones_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define forcetime_case(X) \
	new_case3( T_BODY_FORCE_EXTERNAL, T_ ## X ## COMP, T_SYSTEM_TIME) \
	obj.rbPtr->SetUserDefined_IsTimeDependent((val == true_object?true:false), CRigidBody::__ExternalForce ## X);\
	break;

#define forcetime_cases \
		forcetime_case(X) \
		forcetime_case(Y) \
		forcetime_case(Z)
///////////////////////////////////////////////////////////////////////////////
#define force_case(X, w) \
	new_case2( T_BODY_FORCE_EXTERNAL, T_ ## X ## COMP) \
	obj.rbPtr->SetUserDefinedName(val.sPtr->c_str(), CRigidBody::__ExternalForce ## X);\
	break;

//		CRigidBody::__InternalForce ## X,
//		CRigidBody::__Potential

#define force_cases \
		force_case(X,x) \
		force_case(Y,y) \
		force_case(Z,z)
/*
	case T_XCOMP:
		break;
	case T_YCOMP:
		break;
	case T_ZCOMP:
		break;
	case T_WCOMP:
		break;
	case :
		break;
radial		{ field_count+= count(yytext); *yylval = T_RADIAL;		return(T_RADIAL);	}
Cartesian	{ field_count+= count(yytext); *yylval = T_Cartesian;		return(T_Cartesian);	}
	new_case3(T_DOMAIN, T_XCOMP, T_BEGIN)
				break;
	new_case3(T_DOMAIN, T_XCOMP, T_END)
				break;
	new_case3(T_DOMAIN, T_YCOMP, T_BEGIN)
				break;
	new_case3(T_DOMAIN, T_YCOMP, T_END)
				break;
	new_case3(T_DOMAIN, T_ZCOMP, T_BEGIN)
				break;
	new_case3(T_DOMAIN, T_ZCOMP, T_END)
				break;

begin	{ field_count+= count(yytext); *yylval = T_BEGIN;return(T_BEGIN);	}
end		{ field_count+= count(yytext); *yylval = T_END;return(T_END);	}
domain		{ field_count+= count(yytext); *yylval = T_DOMAIN;		return(T_DOMAIN);	}
infinity	{ field_count+= count(yytext); *yylval = T_INFINITY;	return(T_INFINITY);	}

radial		{ field_count+= count(yytext); *yylval = T_RADIAL;		return(T_RADIAL);	}
Cartesian	{ field_count+= count(yytext); *yylval = T_Cartesian;		return(T_Cartesian);	}
*/
///////////////////////////////////////////////////////////////////////////////
void CSystem::Set(objects& obj, unsigned int _enum, const objects& val)
{
try {
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
//	SetUserDefinedPotentialName(val);
	switch(_enum)
	{
		
	case T_DIMENSION:
	{
		if (1 == long(val))	m_worldDimension = one;
		else
		if (2 == long(val))	m_worldDimension = two;
		else
		if (3 == long(val))	m_worldDimension = three;
		else
		if (val.type == __int)//cannot happen
		switch(val.iVal)
		{
			case 1:m_worldDimension = one;
				break;
			case 2:m_worldDimension = two;
				break;
			case 3:m_worldDimension = three;
				break;
		}
/*
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		switch(m_worldDimension)
		{
			case one:
			//	pFrame->SendMessage(WM_COMMAND, ID_VIEW_1DIM, 0);
				pFrame->PostMessage(WM_COMMAND, ID_VIEW_1DIM, 0);
			break;
			case two:
			//	pFrame->SendMessage(WM_COMMAND, ID_VIEW_2DIM, 0);
				pFrame->PostMessage(WM_COMMAND, ID_VIEW_2DIM, 0);
			break;
			case three:
			//	pFrame->SendMessage(WM_COMMAND, ID_VIEW_3DIM, 0);
				pFrame->PostMessage(WM_COMMAND, ID_VIEW_3DIM, 0);
			break;
		}
*/
	}
		break;
///////////////////////////////////////////////////////////////////////////////

	new_case3(T_DOMAIN, T_XCOMP, T_BEGIN)
		m_domainClass = val == minus_infinity_object? infinite:finite;
		switch(m_domainClass)
		{
			case infinite:
				m_domain[0].begin		= 0;
			break;
			case finite:
				m_domain[0].begin		= val;
			break;
		}
		break;

	new_case3(T_DOMAIN, T_XCOMP, T_END)
		m_domainClass = val == infinity_object? infinite:finite;
		switch(m_domainClass)
		{
			case infinite:
				m_domain[0].end		= 0;
			break;
			case finite:
				m_domain[0].end		= val;
			break;
		}
		break;

	new_case3(T_DOMAIN, T_YCOMP, T_BEGIN)
		m_domain[1].begin	= val;
		break;
	new_case3(T_DOMAIN, T_YCOMP, T_END)
				break;
	new_case3(T_DOMAIN, T_ZCOMP, T_BEGIN)
		m_domain[2].begin	= val;
		break;
	new_case3(T_DOMAIN, T_ZCOMP, T_END)
				break;
///////////////////////////////////////////////////////////////////////////////
	new_case2(T_STATE , T_SCALE)
		pGLView->m_fScaleWave	= val;
		data__QM_Scatt1D->put_ScalePsi(val);
		pGLView->Invalidate();
		break;
///////////////////////////////////////////////////////////////////////////////
	new_case3(T_STATE , T_POTENTIAL, T_SCALE)
		data__QM_Scatt1D->put_ScalePotential(val);
		break;
///////////////////////////////////////////////////////////////////////////////		
	new_case2(T_STATE , T_Stationary)
		if (val == true_object)
		switch(m_worldDimension)
	{
		case one:
		//	CreateQMStationary1DThread(true);
			m_bDoQMStationary1D		= true;
		//	Run();
			CreateQMStationary1DThread(true);
			m_pCQMStationary1D->ResumeThread();
			break;
		case two:
			m_bDoQMStationary2D		= true;
			break;
		case three:
			break;
	}
		break;
///////////////////////////////////////////////////////////////////////////////
	new_case2(T_STATE , T_Scattering)
		if (val == true_object)
		switch(m_worldDimension)
	{
		case one:
			m_bDoQM_Scatt1D			= true;
//			pGLView->LoadTrashCan();
			data__QM_Scatt1D->put_InitialSpread(0.25);
			break;
		case two:
			m_bDoQM_Scatt2D			= true;
			break;
		case three:
			break;
	}
		break;
///////////////////////////////////////////////////////////////////////////////		
	new_case2(T_STATE , T_ENERGY)
//		energy_eigenvalue = val;
		if (m_bDoQMStationary2D)
		{
			if (val == true_object)
			{
		//		m_pCQMStationary2D = new CQMStationary2D(_N2, pGLView->m_fScaleWave);//m_fScaleNurbs
			}
		}
		break;


	new_case2(T_STATE , T_EigenNumber)
	{
		long old = eigenvalueIndex;
		eigenvalueIndex	= val;
		if (eigenvalueIndex	!= old)
			energy_eigenvalue = 0;
	}
		break;
	case T_EigenNumber:
		break;
///////////////////////////////////////////////////////////////////////////////		
	new_case2(T_STATE , T_MASS)
		switch(m_worldDimension)
	{
		case one:
//			m_pCQMStationary1D->M = val;
			M = val;
			break;
		case two:
			break;
		case three:
			break;
	}
		break;


///////////////////////////////////////////////////////////////////////////////		
	new_case2( T_STATE, T_Renormalize)
		data__QM_Scatt1D->put_Renormalize((val == true_object? 1:0));
		break;

	new_case2( T_Scattering, T_Renormalize)
		data__QM_Scatt1D->put_Renormalize((val == true_object? 1:0));
		break;
///////////////////////////////////////////////////////////////////////////////		
	new_case2( T_Scattering, T_Incident)
		if (obj.type == __int)
		{
			SetUserDefinedName(GetC(*val.sPtr), __External3);
		}
		else
		{
//			obj.rbPtr->SetUserDefinedName(val, CRigidBody::__External3);
		}
		break;



	new_case2( T_Scattering, T_FourierTransform)
//		data__QM_Scatt1D->(val);
		break;

	new_case2( T_Scattering, T_WaveNumber)
		data__QM_Scatt1D->put_WaveNumber(val);
		break;
/*
	new_case2( T_Scattering, T_InitialPacketSpread)
		data__QM_Scatt1D->put_InitialSpread(val);
		break;

	new_case2( T_Scattering, T_Center_of_Gaussian)
		data__QM_Scatt1D->put_CenterOfGaussian(val);
		break;

	new_case2( T_Gaussian, T_Spread)
		data__QM_Scatt1D->put_InitialSpread(val);
		break;

	new_case2( T_Gaussian, T_Center)
		data__QM_Scatt1D->put_CenterOfGaussian(val);
		break;
*/

	new_case2( T_Scattering, T_Method)
m_SolveMethod= val == radial_object? radial:
	(val == pseudoCartesian_object? pseudoCartesian:
	(val == Cartesian_object? Cartesian:
	(val == Cayley_object? Cayley:
	(val == Nash_Chen_object? Nash_Chen:
	(val == PeriodicCayley_object? PeriodicCayley:direct)))));
	//	data__QM_Scatt1D->put_EvolutionMethod(val);
		break;

	new_case2( T_Stationary, T_Method)
m_SolveMethod= val == radial_object? radial:
	(val == pseudoCartesian_object? pseudoCartesian:
	(val == Cartesian_object? Cartesian:
	(val == Cayley_object? Cayley:
	(val == Nash_Chen_object? Nash_Chen:
	(val == PeriodicCayley_object? PeriodicCayley:direct)))));
		break;
/////////////////////////////////////////////////////////////////////////////
	case T_Superposition:
		{
			bool m_bSuperposition = val == true_object?true:false;
			if (m_bSuperposition)
			{
		//##	m_rigidBodyManager.m_bstrIndexNewWebBrowser	= 3;//8
				m_pCSuperposition	= new CSuperposition(pGLView);
				m_pCWaveNumbers		= new CWaveNumbers(m_pCSuperposition, 0);
			}
		}
		break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////		
	new_case2( T_DATA, T_FILE)
	{
		const wstring& fileName = *val.sPtr;
		CWaitCursor wc;
		const wchar_t*s = approx(fileName);
		if (s) m_strDataFileName = GetC(s);
	}
		break;

	case T_NODES:
		pGLView->m_NDATA		= val;
		if (m_bDoVerhulstDynamics)
		{
			pGLView->glComputeMap(pGLView->glFrustum_top, pGLView->glFrustum_bottom);
	//		pGLView->glDrawMap2(pGLView->glFrustum_top, pGLView->glFrustum_bottom);
			pGLView->Invalidate();
		}
		if (m_pCQMStationary1D)	m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, T_NODES);
		break;

	new_case2( T_TITLE, T_XCOMP )
		m_xStr		= GetC(*val.sPtr);
		pGLView->Invalidate();
		break;

	new_case2( T_TITLE, T_YCOMP )
		m_yStr		= GetC(*val.sPtr);
		pGLView->Invalidate();
		break;


	new_case2( T_TICMARK, T_XCOMP )
	//	m_tic_mark_spacingStr		= val;
		pGLView->m_num_tic_marks_x		= val;
		pGLView->SetPerspectiveOrFrustum();
		pGLView->Invalidate();
		break;

	new_case2( T_TICMARK, T_YCOMP )
	//	m_tic_mark_spacingStr		= val;
		pGLView->m_num_tic_marks_y		= val;
		pGLView->SetPerspectiveOrFrustum();
		pGLView->Invalidate();
		break;


	case T_SYSTEM_LEFT:		m_fRigidBoundary_left = val;
		break;
	case T_SYSTEM_RIGHT:	m_fRigidBoundary_right = val;
		break;
	case T_SYSTEM_TOP:		m_fRigidBoundary_top = val;
		break;
	case T_SYSTEM_BOTTOM:	m_fRigidBoundary_bottom = val;
		break;
	case T_SYSTEM_NEAR:		m_fRigidBoundary_front = val;
		break;
	case T_SYSTEM_FAR:		m_fRigidBoundary_back = val;
		break;
	case T_BOUNDING_BOX:
	{
		VectorND<6>v = *val.v6Ptr;
		pGLView->BoundingBox(v.v[0],v.v[1],v.v[2],v.v[3],v.v[4],v.v[5]);
	}
		break;


	case T_TRANSLATE:
	{
		Vector3D& v	= *val.vPtr;
		if (obj.type == __int)
		{
			pGLView->set_glTrans_X(v.x);
			pGLView->set_glTrans_Y(v.y);
			pGLView->set_glTrans_Z(v.z);
		}
		else
		{
		//	obj.rbPtr->
		}
	}
		break;
	new_case2( T_TRANSLATE, T_XCOMP)
		pGLView->set_glTrans_X(val);
		break;
	new_case2( T_TRANSLATE, T_YCOMP)
		pGLView->set_glTrans_Y(val);
		break;
	new_case2( T_TRANSLATE, T_ZCOMP)
		pGLView->set_glTrans_Z(val);
		break;

///////////////////////////////////////////////////////////////////////////////

	case T_ROTATE:
	{
		Vector3D& v	= *val.vPtr;
		if (obj.type == __int)
		{
			pGLView->set_glRot_X(v.x);
			pGLView->set_glRot_Y(v.y);
			pGLView->set_glRot_Z(v.z);
		}
		else
		{
		//	obj.rbPtr->
		}
	}
		break;
	new_case2( T_ROTATE, T_XCOMP)
		pGLView->set_glRot_X(val);
		break;
	new_case2( T_ROTATE, T_YCOMP)
		pGLView->set_glRot_Y(val);
		break;
	new_case2( T_ROTATE, T_ZCOMP)
		pGLView->set_glRot_Z(val);
		break;
///////////////////////////////////////////////////////////////////////////////
	case T_MAP:
		{
			const wstring ms(L"Mandelbrot Set");
			const wstring sc(L"Logistic Map");
			const wstring& s	= *val.sPtr;

			m_bDoMapper				= s == ms?true:false;
			m_bDoVerhulstDynamics	= s == sc?true:false;
			if (m_bDoMapper)
			{
				pGLView->SetupImage();
				Run();
			}
			if (m_bDoVerhulstDynamics)
			{

				pGLView->SetupImage();
				POINT pt;
				pt.x = pGLView->viewWidth/2;
				pt.y = pGLView->viewHeight/2;
				pGLView->glDrawMap_List2( pt, -1 );

/*
		pGLView->glComputeMap(  pGLView->glFrustum_top, pGLView->glFrustum_bottom);
		pGLView->glDrawMap2(  pGLView->glFrustum_top, pGLView->glFrustum_bottom);
		pGLView->PixelsToTexture(pGLView->PixelsSnapshot(), pGLView->texture);
		pGLView->TextureToDisplayList(pGLView->listBackground, pGLView->texture);
		pGLView->Invalidate();
*/
/*
				RECT rc;
				pGLView->GetClientRect(&rc);
				Point3D p1(0, 0, 0);
				Point3D p2(0, rc.bottom, 0);
				pGLView->ComputeWorldCoords(&p1.x, &p1.y, &p1.z);
				pGLView->ComputeWorldCoords(&p2.x, &p2.y, &p2.z);
				pGLView->glDrawMap_List(p1.y, p2.y, pGLView->listBackground, pGLView->texture);
				pGLView->SetPerspectiveOrFrustum();
*/
			}
		}
		break;

///////////////////////////////////////////////////////////////////////////////		
///////////////////////////////////////////////////////////////////////////////
/*
	new_case2( T_COLOR, T_XCOMP)
	SetUserDefinedName(val, __Red);
	break;
	new_case2( T_COLOR, T_YCOMP)
	SetUserDefinedName(val, __Green);
	break;
	new_case2( T_COLOR, T_ZCOMP)
	SetUserDefinedName(val, __Blue);
	break;
*/
///////////////////////////////////////////////////////////////////////////////

	case T_GRAVITATIONAL_COUPLING:
		fieldsManager->m_GravitationalCoupling	= val;
		break;
	case T_GRAVITATIONAL_FIELD:
		fieldsManager->m_ExternalGravitationalField	= val;
		break;
	ext_grav_cases

	case T_LENNARD_JONES:
		fieldsManager->Set_LENNARD_JONES(val);
		break;
	lennardjones_cases

	case T_CURRENTLOOP_MAGNETICFIELD_STRENGTH:
		fieldsManager->OnCurrentLoopMagneticField( val );
		break;

	
		
	case T_ELECTRIC_FIELD:
		fieldsManager->m_ExternalElectricField = val;
		break;
	new_case2( T_ELECTRIC_FIELD, T_XCOMP)
		fieldsManager->m_ExternalElectricField.x=(val);
		break;
	new_case2( T_ELECTRIC_FIELD, T_YCOMP)
		fieldsManager->m_ExternalElectricField.y=(val);
		break;
	new_case2( T_ELECTRIC_FIELD, T_ZCOMP)
		fieldsManager->m_ExternalElectricField.z=(val);
		break;



	case T_BODY_FORCE_INTERNAL:
		//	if (val == on_object)  rigidBodyManager->m_bInternalForce = true;
		//	if (val == off_object) rigidBodyManager->m_bInternalForce = false;
			if (val == true_object)  rigidBodyManager->m_bInternalForce = true;
			if (val == false_object) rigidBodyManager->m_bInternalForce = false;
		break;


	case T_MAGNETIC_FIELD:
		fieldsManager->m_ExternalMagneticField = val;
		break;
	new_case2( T_MAGNETIC_FIELD, T_XCOMP)
		fieldsManager->m_ExternalMagneticField.x=(val);
		break;
	new_case2( T_MAGNETIC_FIELD, T_YCOMP)
		fieldsManager->m_ExternalMagneticField.y=(val);
		break;
	new_case2( T_MAGNETIC_FIELD, T_ZCOMP)
		fieldsManager->m_ExternalMagneticField.z=(val);
		break;

	case T_SYSTEM_TIME_STEP:
		m_dt = val;
		break;
	case T_SYSTEM_TIME:
		m__t = val;
		break;
	new_case2( T_STOP, T_SYSTEM_TIME)
		m_fStopTime = val;
		break;
	case T_SYSTEM_TIMER_INTERVAL:		
		m_uMilliseconds	= UINT(long(val));
		break;


	new_case2( T_SYSTEM_Background, T_SHOW)
		{
			pGLView->m_bShowBackground = val == true_object?true:false;
		//	bool iCurrentShowBackground = (pGLView->m_FieldDisplay&CGLView::backgroundTexture)?true:false;
		//	if (iCurrentShowBackground != pGLView->m_bShowBackground) pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay^CGLView::backgroundTexture);
			Fire_ViewChange();
		}
		break;
	new_case2( T_SYSTEM_VectorFields, T_SHOW)
		fieldsManager->ShowVectorFields(val == true_object?true:false);
		break;
	new_case2( T_SYSTEM_FieldLines, T_SHOW)
		fieldsManager->ShowFieldLines(val == true_object?true:false);
		break;
	new_case2( T_SYSTEM_Equipotentials, T_SHOW)
		fieldsManager->ShowEquipotentials(val == true_object?true:false);
		break;
///////////////////////////////////////////////////////////////////////////////

	case T_RUN:
		m_lTakeNumSteps = val;
/////////////////////////////////////////////////////////////////////////////
		m_bSingleStepping	= true;
		m_bSingleStep_Mode	= true;
		m_bRunning			= true;
///////////////////////////////////////////////////////////////////////////////
		Run();
		break;



	case T_STOP:
	//	CSystem::Get implements Stop();
		break;
	case T_CLEAR:
	//	CSystem::Get implements	Clear();
		break;
	case T_RESET:
	//	CSystem::Get explains the implementation of	Initialize();
		break;
	case T_COLLISIONDETECTION:
		m_bCollisionDetectionEnabled=(val == true_object?true:false);
		break;
	case T_ENFORCE_RIGID_BOUNDARY:
		m_nEnforceRigidBoundary = RigidBoundary(long(val));
		break;

	case T_DEFERRED_COMMAND:
		{
			double time = 0;
			std::wstring command;
			if (ParseTimedCommand(*val.sPtr, time, command))
			{
				SetDeferredCommand(time, GetC(command));
			}
		}
		break;

///////////////////////////////////////////////////////////////////////////////
	new_case2( T_DEFERRED_COMMAND, T_ONRESET)
		{
			double time = 0;
			std::wstring command;
			if (ParseTimedCommand(*val.sPtr, time, command))
			{
				SetResetDeferredCommand(time, GetC(command));
			}
		}
		break;

	new_case2( T_SCENE, T_SYSTEM_LEFT)
		pGLView->set_glFrustum_left(val);
		if (m_pCQMStationary1D)	m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, T_SYSTEM_LEFT);
		break;
	new_case2( T_SCENE, T_SYSTEM_RIGHT)
		pGLView->set_glFrustum_right(val);
		if (m_pCQMStationary1D)	m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, T_SYSTEM_RIGHT);
		break;
	new_case2( T_SCENE, T_SYSTEM_TOP)
		pGLView->set_glFrustum_top(val);
		break;
	new_case2( T_SCENE, T_SYSTEM_BOTTOM)
		pGLView->set_glFrustum_bottom(val);
		break;
	new_case2( T_SCENE, T_SYSTEM_NEAR)
		pGLView->set_glFrustum_znear(val);
		break;
	new_case2( T_SCENE, T_SYSTEM_FAR)
		pGLView->set_glFrustum_zfar(val);
		break;
	case T_NAME://	title of simulation
		name	= GetC(*val.sPtr);
		break;



/*
		constraintManager.AddTwoPointConstraint(
													obj.rbPtr,
													const Point3D& point1,
													CRigidBody* body2,
													const Point3D& point2,
													double _separation
													)
*/
///////////////////////////////////////////////////////////////////////////////
	case T_IMASS:
		obj.rbPtr->set_MassInverse(val);
		break;

	case T_CENTRAL_FORCE:
		if (obj.type == __int)
		{
			fieldsManager->m_fCentralForce = val;
			if (fieldsManager->m_fCentralForce==0.0)
				fieldsManager->m_bCentralForce_IsValid = false;
			else
				fieldsManager->m_bCentralForce_IsValid = true;
		}
		else
		{
			obj.rbPtr->m_fCentralForce = val;
			if (obj.rbPtr->m_fCentralForce==0.0)
				obj.rbPtr->m_bCentralForce_IsValid = false;
			else
				obj.rbPtr->m_bCentralForce_IsValid = true;
		}
		break;

	case T_INVERSE_INERTIA_TENSOR:
		break;
	case T_CHARGE:
		obj.rbPtr->m_fCharge	= val;
		fieldsManager->Clear();
		break;
	case T_POSITION:
		obj.rbPtr->set_Position(val);
		break;
		pos_cases
	case T_MOMENTUM:
		obj.rbPtr->set_Momentum(val);
		break;
		mom_cases
	case T_ANGULARMOMENTUM:
		obj.rbPtr->set_AngularMomentum(val);
		break;
		amom_cases
	case T_ORIENTATION:
	{
		VectorND<4>v = *val.v4Ptr;
//		X = (newVal.pdbl)[1], y = (newVal.pdbl)[2], z = (newVal.pdbl)[3];
		double angle = v.v[0];
		Vector3D axis(v.v[1],v.v[2],v.v[3]);
		obj.rbPtr->set_Quat(angle, axis);
	}
		break;
	case T_FORCE:
		obj.rbPtr->m_vecForce	= val;
		break;
	case T_TORQUE:
		obj.rbPtr->m_vecTorque	= val;
		break;

	case T_BODY_FORCE_EXTERNAL:
		break;
		force_cases
		forcetime_cases

//	case T_BODY_FORCE_INTERNAL:
//		break;

	case T_VELOCITY:
		obj.rbPtr->set_Velocity(val);
		break;
		vel_cases
	case T_ANGULARVELOCITY:
		obj.rbPtr->set_AngularVelocity(val);
		break;
		avel_cases
	case T_ACCELERATION:
		break;
	case T_MASS:
		obj.rbPtr->set_Mass(val);
		break;
	case T_DENSITY:
		obj.rbPtr->m_fDensity	= val;
		break;



	case T_MagneticDipoleMoment:
		obj.rbPtr->m_fMagneticDipoleMoment	= val;
		break;

	case T_MagneticDipoleOrientation:
		obj.rbPtr->m_vecMagneticDipoleOrientation	= val;
		obj.rbPtr->m_vecMagneticDipoleOrientation.normalize();
		break;



	case T_COLOR:
		obj.rbPtr->set_Color(val);
		break;
		color_cases
	case T_DRAWSTYLE:
			obj.rbPtr->set_DrawStyle(static_cast<long>(val.Long()));
		break;
	case T_SCALE:
		if (obj.type == __int)//T_SYSTEM
		{
			double fScale	= val;//click then zoom by this factor
			if (fScale > 0.0) m_fScale	= fScale;
		}
		else
		{
	//		obj.rbPtr->SetUserDefinedName(val, CRigidBody::__Potential);
		}
		break;
	case T_RADIUS:
		obj.rbPtr->set_Radius(val);
		break;
	case T_WIDTH:
		obj.rbPtr->set_Width(val);
		break;
	case T_HEIGHT :
		obj.rbPtr->set_Height(val);
		break;
	case T_DEPTH:
		obj.rbPtr->set_Depth(val);
		break;
	case T_COEFFICIENT_OF_STATIC_FRICTION:
		obj.rbPtr->m_fCoefficientOfStaticFriction	= val;
		break;
	case T_COEFFICIENT_OF_KINETIC_FRICTION:
		obj.rbPtr->m_fCoefficientOfKineticFriction	= val;
		break;
	case T_COEFFICIENT_OF_RESTITUTION:
		obj.rbPtr->m_fCoefficientOfRestitution	= val;
		break;
	case T_DAMPING_COEFFICIENT:
		obj.rbPtr->m_vecDampingCoefficient	= val;
		break;
	case T_ENERGY:
		break;
	case T_KINETIC_ENERGY:
		break;
	case T_POTENTIAL_ENERGY:
		break;



	case T_POTENTIAL:
	//	if (obj.sym->type == T_SYSTEM)
	//	if (T_SYSTEM == int(obj))
		if (obj.type == __int)
		{
			SetUserDefinedName(GetC(*val.sPtr), __Potential);
			if (pGLView->m_bShowBackground)
			{
		//		CreateContourPlotBitmap();
			}
		}
		else
		{
			obj.rbPtr->SetUserDefinedName(val.sPtr->c_str(), CRigidBody::__Potential);
		}
		break;

	new_case2( T_POTENTIAL, T_SYSTEM_TIME)
	//	if (obj.sym->type == T_SYSTEM)
	//	if (T_SYSTEM == int(obj))
		if (obj.type == __int)
		{
			SetUserDefined_IsTimeDependent((val == true_object?true:false), __Potential);
		}
		else
		{
	obj.rbPtr->SetUserDefined_IsTimeDependent((val == true_object?true:false), CRigidBody::__Potential);
		}
		break;

	new_case2( T_POTENTIAL, T_SHOW)
	//	if (obj.sym->type == T_SYSTEM)
	//	if (T_SYSTEM == int(obj))
		if (obj.type == __int)
		{
			if (val == true_object) CreateContourPlotBitmap();
		}
		else
		{
		}
		break;

		

	new_case2( T_CONTACTS, T_SHOW)
		if (obj.type == __int)
		{
			rigidBodyManager->m_bShowContacts	= val == true_object?true:false;
			rigidBodyManager->m_bShowClosestContacts = val == true_object?true:false;
			if (rigidBodyManager->m_bShowContacts) rigidBodyManager->ExaminePairs();
			else rigidBodyManager->CloseExaminePairs();
		}
		else
		{
			obj.rbPtr->m_bShowContacts	= val == true_object?true:false;
		}
		break;
///////////////////////////////////////////////////////////////////////////////


	case T_DISTANCE:
		break;
	case T_SPEED:
		break;
	case T_RECORD:
		{
			double time = 0;
			std::wstring command;
			if (ParseTimedCommand(*val.sPtr, time, command))
			{
				SetResetDeferredCommand(time, GetC(command));
			}
		}
		break;



	new_case3(T_RECORD, T_SYSTEM_TIME, T_BEGIN)
		obj.rbPtr->m_fTrackingStartTime		= val;
		break;

	new_case3(T_RECORD, T_SYSTEM_TIME, T_END)
		obj.rbPtr->m_fTrackingStopTime = val;
//		obj.rbPtr->RecordData(T_TRAJECTORY, obj.rbPtr->m_fTrackingStartTime, obj.rbPtr->m_fTrackingStopTime);
//		obj.rbPtr->RecordData(T_PERIOD, obj.rbPtr->m_fTrackingStartTime, obj.rbPtr->m_fTrackingStopTime);
		break;



	new_case2(T_RECORD, T_PERIOD)
	//	obj.rbPtr->m_bPlotTrajectory		= val == true_object?true:false;
		obj.rbPtr->RecordData(T_PERIOD);//, obj.rbPtr->m_fTrackingStartTime, obj.rbPtr->m_fTrackingStopTime);
		break;

	new_case2(T_RECORD, T_TRAJECTORY)
		obj.rbPtr->m_bPlotTrajectory		= val == true_object?true:false;
		obj.rbPtr->RecordData(T_TRAJECTORY);//, obj.rbPtr->m_fTrackingStartTime, obj.rbPtr->m_fTrackingStopTime);
		break;

///////////////////////////////////////////////////////////////////////////////
#if 0
void Action::WrapBody_RECORD_DATA_PSIM_OFF () {	
	_Symbol *body = CParser::pop();
	if (body->body==0) return;
	body->body->ClearTrajectoryRecord();
}
#endif
///////////////////////////////////////////////////////////////////////////////

	case T_TRAJECTORY:
		break;

	new_case2(T_TRAJECTORY, T_COLOR)
		obj.rbPtr->m_vecPlotTrajectoryColor = val;
	//	obj.rbPtr->AnalyzeData(T_TRAJECTORY, obj.rbPtr->m_vecPlotTrajectoryColor);
		break;



	case T_ANALYZE:
		{
			std::wstring& s = *val.sPtr;
			obj.rbPtr->AnalyzeData(s);
		}
		break;

	case T_PERIOD:
		break;

	case T_CONSTRAINT_STATE:
		break;
	case T_CONNECT:
		break;
	case T_PULLEY:
		break;
	case T_quadrant:
		break;
	new_case2( T_PULLEY, T_SYSTEM_LEFT)
	{
		CRigidBody::InitialAngleInQuadrant valu;
		CRigidBody*  leftBody	= val.rbPtr;
		CRigidBody*  pulley		= obj.rbPtr;
		CRigidBody*  rightBody	= 0;
		leftBody->get_InitialAngleInQuadrant(valu);
		int nLeftBodyConnectedInQuadrant	= valu;//leftBody->m_nConnectedInQuadrant;
		int nRightBodyConnectedInQuadrant	= 0;
		this->constraintManager->AddPulleyConstraint(leftBody,pulley,rightBody,
									nLeftBodyConnectedInQuadrant,nRightBodyConnectedInQuadrant
									);
	}
	break;
	new_case2( T_PULLEY, T_SYSTEM_RIGHT)
	{
		CRigidBody::InitialAngleInQuadrant valu;
		CRigidBody*  leftBody	= 0;
		CRigidBody*  pulley		= obj.rbPtr;
		CRigidBody*  rightBody	= val.rbPtr;
		rightBody->get_InitialAngleInQuadrant(valu);
		int nLeftBodyConnectedInQuadrant	= 0;
		int nRightBodyConnectedInQuadrant	= valu;//rightBody->m_nConnectedInQuadrant;
		this->constraintManager->AddPulleyConstraint(leftBody,pulley,rightBody,
									nLeftBodyConnectedInQuadrant,nRightBodyConnectedInQuadrant
									);
	}
	break;

	case T_PLANE:
		break;
	case T_ANGLE:
		break;
	case T_POINT_IN_PLANE:
		break;

	new_case2( T_PLANE, T_ANGLE)
	{
		Point3D point			= obj.rbPtr->m_ptPointInPlane;
		double fAngleOfIncline	= val;
		obj.rbPtr->ConstrainToPlane(fAngleOfIncline,point.x,point.y,point.z);
	}
	break;

	new_case2( T_PLANE, T_POINT_IN_PLANE)
	{
		Point3D point			= val;
		double fAngleOfIncline	= obj.rbPtr->m_fAngleOfIncline*RAD_TO_DEG;
		obj.rbPtr->ConstrainToPlane(fAngleOfIncline,point.x,point.y,point.z);
	}
	break;



///////////////////////////////////////////////////////////////////////////////
	new_case2( T_POSITION, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
			obj.rbPtr->m_lShowVelocityEveryNumSteps = val;
		}
	break;
///////////////////////////////////////////////////////////////////////////////
	new_case2( T_VELOCITY, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
			obj.rbPtr->m_lShowVelocityEveryNumSteps = val;
//		m_fScaleVelocity(1),
		}
	break;
///////////////////////////////////////////////////////////////////////////////

	new_case2( T_NORMAL, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleNormal= val;
		}
	break;

	new_case2( T_MOMENTUM, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleMomentum= val;
		}
	break;

	new_case2( T_ANGULARMOMENTUM, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleAngularMomentum= val;
		}
	break;

	new_case2( T_FORCE, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleForce= val;
		}
	break;

	new_case2( T_ACCELERATION, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleAcceleration= val;
		}
	break;

	new_case2( T_contactFORCE, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
//		obj.rbPtr->m_bShowContactForce= val;
		}
	break;

	new_case2( T_TORQUE, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_fScaleTorque= val;
		}
	break;


	new_case2( T_contactTORQUE, T_SCALE)
		if (obj.type == __int)
		{
		}
		else
		{
//		obj.rbPtr->m_bShowContactTorque= val;
		}
	break;




///////////////////////////////////////////////////////////////////////////////
	new_case2( T_POSITION, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
			obj.rbPtr->m_lShowPositionEveryNumSteps = val;
	//		obj.rbPtr->m_bTrackingOn=obj.rbPtr->m_lShowPositionEveryNumSteps>0;
		}
	break;
///////////////////////////////////////////////////////////////////////////////
	new_case2( T_VELOCITY, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
			obj.rbPtr->m_lShowVelocityEveryNumSteps = val;
		}
	break;
///////////////////////////////////////////////////////////////////////////////

	new_case2( T_NORMAL, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowNormals= val == true_object?true:false;
		}
	break;

	new_case2( T_MOMENTUM, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowMomentum= val == true_object?true:false;
		}
	break;

	new_case2( T_ANGULARMOMENTUM, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowAngularMomentum= val == true_object?true:false;
		}
	break;

	new_case2( T_FORCE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowForce= val == true_object?true:false;
		}
	break;

	new_case2( T_ACCELERATION, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowAcceleration= val == true_object?true:false;
		}
	break;

	new_case2( T_contactFORCE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowContactForce= val == true_object?true:false;
		}
	break;

	new_case2( T_Constraint, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowQuadraticConstraint= val == true_object?true:false;
		}
	break;

	new_case2( T_TORQUE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowTorque= val == true_object?true:false;
		}
	break;


	new_case2( T_contactTORQUE, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowContactTorque= val == true_object?true:false;
		}
	break;

	new_case2( T_TRAJECTORY, T_SHOW)
		if (obj.type == __int)
		{
		}
		else
		{
		obj.rbPtr->m_bShowTrajectory= val == true_object?true:false;
		}
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
		obj.rbPtr->set_MultiBody_SPRING_LENGTH(val);
		break;
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_SPRING_CONSTANT)
		obj.rbPtr->set_MultiBody_SPRING_CONSTANT(val);
		break;
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_CONTROL)
		obj.rbPtr->set_MultiBody_CONTROL(static_cast<int>(val.Long()));
		break;
	case T_MultiBody_Link_Joint:
		break;
		
	new_case2(T_MultiBody_Link_Joint,T_DISPLACEMENT)
	{
		double X= val;
		switch(obj.rbPtr->m_inboundJoint.jointType)
		{
			case Joint1DOF::REV:
			case Joint1DOF::PRI:
				rigidBodyManager->RigidLinkJointStatesInitial[obj.rbPtr->svIdx]= (obj.rbPtr->m_inboundJoint.jointType != Joint1DOF::REV ? X : X * DEG_TO_RAD);
			break;
			case Joint1DOF::FLT:
			//	((RBConfig *) &*(//##		rigidBodyManager->RigidLinkJointStatesInitial.end() - 13))->Tbi = Se3::ID;
			break;
		}
		rigidBodyManager->RigidLinkJointStates = 			rigidBodyManager->RigidLinkJointStatesInitial;
		rigidBodyManager->compXf(&		rigidBodyManager->RigidLinkJointStates.front());
		obj.rbPtr->m_baseJoint->outboundLink->updatePoses();
		obj.rbPtr->set_InitialState(*obj.rbPtr);
	//	UpdateTree();
	//	DisplayData();
	}
		break;
	new_case2(T_MultiBody_Link_Joint,T_VELOCITY)
	{
		double X= val;
		switch(obj.rbPtr->m_inboundJoint.jointType) {
			case Joint1DOF::REV:
			case Joint1DOF::PRI:
						rigidBodyManager->RigidLinkJointStatesInitial[obj.rbPtr->svIdx+1]= (obj.rbPtr->m_inboundJoint.jointType != Joint1DOF::REV ? X : X * DEG_TO_RAD);
			break;
			case Joint1DOF::FLT:
			//	((RBConfig *) &*(//##		rigidBodyManager->RigidLinkJointStatesInitial.end() - 13))->Tbi = Se3::ID;
			break;
		}
		rigidBodyManager->RigidLinkJointStates = 	rigidBodyManager->RigidLinkJointStatesInitial;

		rigidBodyManager->compXf(&		rigidBodyManager->RigidLinkJointStates.front());
		obj.rbPtr->m_baseJoint->outboundLink->updatePoses();
		obj.rbPtr->set_InitialState(*obj.rbPtr);
	//	UpdateTree();
	//	DisplayData();
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
//	pGLView->ResetView();
		UpdateTree();
		DisplayData();
	pGLView->Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
	catch (std::runtime_error* exc)
	{
		::MessageBox(0, GetWC(exc->what()), L"CSystem::Set:Error", MB_OK);
		return ;
	}
	catch(struct _exception *except)
	{
		::MessageBox(0, GetWC(except->name), L"CSystem::Set:Error", MB_OK);
		return ;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0, str, L"CSystem::Set:Error", MB_OK);
		return ;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0, L"unknown", L"CSystem::Set:Error", MB_OK);
		return ;
	}
}
///////////////////////////////////////////////////////////////////////////////


#undef pos_case
#undef pos_cases

#undef force_case
#undef force_cases


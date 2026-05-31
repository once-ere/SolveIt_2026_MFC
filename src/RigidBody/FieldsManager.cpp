// FieldsManager.cpp: implementation of the CFieldsManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "RigidBodyManager.h"

#include "FieldsManager.h"
//#include "ODESolver.h"
#include "CurrentLoop.h"
//#include "QMScatt1D/QMStationary1D.h"
//#include "QMScatt2D/QMStationary2D.h"
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DrawCone(double coneRadius, double x, double y, double z, bool bTranslate);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Vector2D CFieldsManager::m_CurrentMousePosition;
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFieldsManager::CFieldsManager() :
	m_pCCurrentLoop(0),
	m_GravitationalCoupling(0),
	m_fCentralForce(0),
//	m_fLennardJones(0),
//	m_ExternalGravitationalFieldY(0),
	m_ExternalElectrostaticFieldX1(0),
	m_ExternalElectrostaticFieldX2(0),
	m_ExternalElectrostaticFieldX3(0),

	m_bShowVectorFields(false),
	m_bShowFieldLines(false),
	m_bShowEquipotentials(false),

	m_bSinusoidalForce_Component_X_IsValid(false),
	m_bSinusoidalForce_Component_Y_IsValid(false),
	m_bSinusoidalForce_Component_Z_IsValid(false),

	m_bCentralForce_IsValid(false),
	m_bLennardJones_IsValid(false),
	m_bShowN_Body_ElectrostaticField(false),//true

	m_bValidCurrentLoopMagneticField(false),
	m_bTimeDependentExternalForce_Component_X_IsValid(false),
	m_bTimeDependentExternalForce_Component_Y_IsValid(false),
	m_bTimeDependentExternalForce_Component_Z_IsValid(false),
	m_currentLoopMagneticFieldStrength(0),
	m_scaleMagneticField(0.05),
	m_fScaleElectricField(0.1),
//	m_RecordPotential(no),
	m_RecordPotentialNumSteps(32),
/////////////////////////////////////////////////////////////////////////////
	m_threadFieldLineElectric2D(0),
	m_threadFieldLineElectric3D(0),
//	m_threadFieldLineMagnetic_current_loop(0),
//	m_threadFieldLineMagnetic(0),
	m_threadFieldLineMagnetostatic(0),
	m_threadFieldLineGravitational(0),
	m_threadFieldLine_Equipotential(0)
/////////////////////////////////////////////////////////////////////////////
//	m_electric_2D_currentFieldLine(2, Vector3D(0.7255, 0.3098, 0.5608)),
//	m_electric_2D_currentFieldLine(2, Vector3D(0.0, 0.5, 0.0)),
//	m_electric_3D_currentFieldLine(3, Vector3D(1, 0.4, 0)),
{

}

CFieldsManager::~CFieldsManager()
{
	Clear();
	if (m_pCCurrentLoop)
	{
	//	delete m_pCCurrentLoop;
		if (::IsWindow(m_pCCurrentLoop->m_hWnd)) ::SendMessage(m_pCCurrentLoop->m_hWnd, WM_CLOSE,0,0);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::OnDraw()
{
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
/*
	CSystem* m_system = theApp.GetSystem();
	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, fieldsManager));
		CRigidBodyManager* rigidBodyManager = *pThis->rigidBodyManager;
*/


	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

// Need to turn on GL_DITHER, GL_SMOOTH for 3D objects and turn them off for lines and points.
	glDisable(GL_DITHER);
	glShadeModel(GL_FLAT);

	CSystem* m_system	= theApp.GetSystem();


///////////////////////////////////////////////////////////////////////////////
	if (m_bShowVectorFields)
	{
		glColor3d (1.0, 0.25, 0.1);
		if (m_system->m_worldDimension==CSystem::two)
		{
			vector<PairVV2D>::const_iterator j;
			for (j=m_vecPair_2Delectrostatic.begin();j!=m_vecPair_2Delectrostatic.end();++j)
			{
				const PairVV2D& pr = *j;
				const Vector2D& p = pr.first;
				const Vector2D& E = pr.second;
				glPushMatrix();
				glTranslated(p.x, p.y, 0);
				glBegin (GL_LINES);
				glVertex2d(  0,   0);
				glVertex2d(E.x, E.y);
				glEnd ();
				DrawCone(0.025, E.x, E.y, 0, true);
				glPopMatrix();
			}
		}
		if (m_system->m_worldDimension==CSystem::three)
		{
			vector<PairPV3D>::const_iterator j;
			for (j=m_vecPair_electric.begin();j!=m_vecPair_electric.end();++j)
			{
				const PairPV3D& pr = *j;
				const Vector3D& p = pr.first;
				const Vector3D& E = pr.second;
				glPushMatrix();
				glTranslated(p.x, p.y, p.z);
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(E.x, E.y, E.z);
				glEnd ();
				DrawCone(0.025, E.x, E.y, E.z, true);
				glPopMatrix();
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_system->m_worldDimension==CSystem::two )
	{
		if ((!rigidBodyManager->empty()) && rigidBodyManager->IsCharged())
		{
			DrawVector2D(m_CurrentMousePosition);
			DrawEquipotential2D(m_CurrentMousePosition, m_RecordPotentialNumSteps, false);
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bValidCurrentLoopMagneticField)
	{
		glPushMatrix();
		glColor3d (0.0, 0.0, 1.0);
		glBegin (GL_LINE_LOOP);
		vector<Vector3D>::iterator i;
		for (i=m_currentLoop.begin();i!=m_currentLoop.end();++i)
		{
			Vector3D& p = *i;
			glVertex3d(p.x,p.y,p.z);
		}
		glEnd ();
		glColor3d (1.0, 0.35, 0.0);
		glPopMatrix();
		if (m_bShowVectorFields)
		{
			vector<PairPV3D>::const_iterator j;
			for (j=m_vecPairMagnetic_current_loop.begin();j!=m_vecPairMagnetic_current_loop.end();++j)
			{
				const PairPV3D& pr = *j;
				const Point3D& p = pr.first;
				const Vector3D& B = pr.second;
				glPushMatrix();
				glTranslated(p.x, p.y, p.z);
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(B.x, B.y, B.z);
				glEnd ();
				DrawCone(0.025, B.x, B.y, B.z, true);
				glPopMatrix();
			}
		}
		if (m_bShowVectorFields)
		{
			vector<PairPV3D>::reverse_iterator j;
			for (j=m_vecPairMagnetic_current_loop.rbegin();j!=m_vecPairMagnetic_current_loop.rend();++j)
			{
				const PairPV3D& pr = *j;
				const Point3D& p = pr.first;
				const Vector3D& B = pr.second;
				glPushMatrix();
				glTranslated(p.x, p.y, p.z);
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(B.x, B.y, B.z);
				glEnd ();
				DrawCone(0.025, B.x, B.y, B.z, true);
				glPopMatrix();
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (m_bShowEquipotentials || m_bShowFieldLines)
	{
//		vector<vector_of_double*>::const_iterator it;
		vector<vector_of_Vector>::const_iterator it;
		Lock lock (protectFieldLines);
		if (!empty())
		{
			glPushMatrix();
		//	glColor3d (0.5, 0.0, 0.6);
			glLineWidth(0.35f);
			for (it=begin(); it!=end(); ++it)
			{
//				vector_of_double& dataFieldLine = *(*it);
//				dataFieldLine.OnDraw();
				vector_of_Vector2D* two		= dynamic_cast<vector_of_Vector2D*>((*it).two);
				vector_of_Vector3D* three	= dynamic_cast<vector_of_Vector3D*>((*it).three);
				if (two) two->OnDraw();
				if (three) three->OnDraw();
			}
			glPopMatrix();
		}
	}
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::Initialize()
{

	Clear();
/*
//f2c hack:
	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
	if (pThis->m_pCQMStationary1D)
	{
		delete pThis->m_pCQMStationary1D;
		pThis->m_pCQMStationary1D = 0;
	}
	if (pThis->m_pCQMStationary2D)
	{
		delete pThis->m_pCQMStationary2D;
		pThis->m_pCQMStationary2D = 0;
	}
// end hack
*/
	m_GravitationalCoupling					= 0;
	m_fCentralForce							= 0;
	m_ExternalGravitationalField			= ZERO_3D;
	m_ExternalElectrostaticFieldX1			= 0;
	m_ExternalElectrostaticFieldX2			= 0;
	m_ExternalElectrostaticFieldX3			= 0;
	m_currentLoopMagneticFieldStrength		= 0;
	m_scaleMagneticField					= 0.05;
	m_fScaleElectricField					= 0.1;

	m_bShowVectorFields						= false;
	m_bShowFieldLines						= false;
	m_bShowEquipotentials					= false;

	m_bValidCurrentLoopMagneticField		= false;
	m_bLennardJones_IsValid					= false;
	m_bSinusoidalForce_Component_X_IsValid	= false;
	m_bSinusoidalForce_Component_Y_IsValid	= false;
	m_bSinusoidalForce_Component_Z_IsValid	= false;
	m_bShowN_Body_ElectrostaticField		= false;//true
	m_bTimeDependentExternalForce_Component_X_IsValid	= false;
	m_bTimeDependentExternalForce_Component_Y_IsValid	= false;
	m_bTimeDependentExternalForce_Component_Z_IsValid	= false;

//	m_RecordPotential						= no;
	m_RecordPotentialNumSteps				= 32;

	m_selectedFieldPoint					= ZERO_3D;
	m_selectedPointForMagneticField			= ZERO_3D;

	m_CurrentMousePosition					= Vector2D(0,0);

	m_ExternalElectricField					= ZERO_3D;
	m_ExternalMagneticField					= ZERO_3D;
	m_SinusoidalForce_Component_X			= ZERO_3D;
	m_SinusoidalForce_Component_Y			= ZERO_3D;
	m_SinusoidalForce_Component_Z			= ZERO_3D;
	m_fLennardJones							= ZERO_3D;


	if (m_pCCurrentLoop)
	{
	//	delete m_pCCurrentLoop;
		if (::IsWindow(m_pCCurrentLoop->m_hWnd)) ::SendMessage(m_pCCurrentLoop->m_hWnd, WM_CLOSE,0,0);
	}
	m_pCCurrentLoop = 0;
//	InitializePath();
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::InitializePath() {
	const long MAXPOINTS = (3*16);
	const double T = 2 * PI / MAXPOINTS;
	m_currentLoop.clear();
	for (int i=0;i< MAXPOINTS;i++)
	{
		Vector3D p(cos(i*T), sin(i*T),0);
		m_currentLoop.push_back(p);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::Reset() {
//	InitializePath();
	Clear();
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::Edit() {
	RecordPath();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ClearVectorFields()
{
	m_currentEquipotentialPoint				= ZERO_3D;
	m_vecPair_2Delectrostatic.clear();
	ShowVectorFields(false);
//	Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ClearVectorFieldLines()
{
	KillThread(m_threadFieldLineElectric3D);
	KillThread(m_threadFieldLineGravitational);
	KillThread(m_threadFieldLineElectric2D);
	m_vecPair_gravitational.clear();
	m_vecPair_2Delectrostatic.clear();
	m_vecPair_electric.clear();

	m_gravitational_currentFieldLine.clear();
	m_electric_3D_currentFieldLine.clear();
	m_electric_2D_currentFieldLine.clear();

	vector<vector_of_Vector>::iterator it;
	for (it=begin(); it!=end(); ++it)
	{
		vector_of_Vector2D* two		= dynamic_cast<vector_of_Vector2D*>((*it).two);
		vector_of_Vector3D* three	= dynamic_cast<vector_of_Vector3D*>((*it).three);
		if (two)
		{
			two->clear();
			delete two;
		}
		if (three)
		{
			three->clear();
			delete three;
		}
	}
	ShowFieldLines(0);
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ClearEquipotentials()
{
	KillThread(m_threadFieldLine_Equipotential);
	m_currentEquipotential.clear();
	ShowEquipotentials(false);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::Clear()
{
	ClearFieldLines();
	ClearVectorFields();
	ShowEquipotentials(false);
	ShowVectorFields(false);
	ShowFieldLines(0);
	theApp.GetGLView()->Clear_fieldLines();
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ClearFieldLines()
{
	KillThreads();
	m_currentEquipotential.clear();

	if (!m_pCCurrentLoop)
	{
		m_vecPairMagnetic_current_loop.clear();

		m_currentLoop.clear();
		m_currentLoopDeltas.clear();
		m_currentLoopDeltasMidpoint.clear();
	}

	m_vecPair_current_loop.clear();

	m_vecPair_gravitational.clear();
	m_vecPair_2Delectrostatic.clear();
	m_vecPair_electric.clear();

	m_gravitational_currentFieldLine.clear();
	m_electric_3D_currentFieldLine.clear();
	m_electric_2D_currentFieldLine.clear();
//	m_magnetic_current_loop_currentFieldLine.clear();
//	m_magnetic_currentFieldLine.clear();
	m_magnetostatic_currentFieldLine.clear();
	m_currentEquipotential.clear();


//	vector<vector_of_double*>::iterator it;
	vector<vector_of_Vector>::iterator it;
	for (it=begin(); it!=end(); ++it)
	{
		vector_of_Vector2D* two		= dynamic_cast<vector_of_Vector2D*>((*it).two);
		vector_of_Vector3D* three	= dynamic_cast<vector_of_Vector3D*>((*it).three);
		if (two)
		{
			two->clear();
			delete two;
		}
		if (three)
		{
			three->clear();
			delete three;
		}
	}
	clear();

//	Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::KillThread(CWinThread* worker)
{
	if (worker)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(worker->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			worker->PostThreadMessage(WM_QUIT, 0, 0); 
			MsgWaitForMultipleObjects(1,&worker->m_hThread,FALSE,INFINITE,QS_ALLINPUT); 
		}
		CloseHandle (worker->m_hThread); 
		worker	= 0; 
	};
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::KillThreads()
{
#define KILL_THREAD(worker) \
	if (worker){\
	DWORD ExitCode=0;\
	BOOL ret = GetExitCodeThread(worker->m_hThread, &ExitCode);\
	if (ExitCode == STILL_ACTIVE)\
	{\
	worker->PostThreadMessage(WM_QUIT, 0, 0); \
		MsgWaitForMultipleObjects(1,&worker->m_hThread,FALSE,INFINITE,QS_ALLINPUT); \
}\
		CloseHandle (worker->m_hThread); \
		worker	= 0; \
	};
///////////////////////////////////////////////////////////////////////////////
//	KILL_THREAD(m_threadFieldLineElectric2D)
	KILL_THREAD(m_threadFieldLineElectric3D)
//	KILL_THREAD(m_threadFieldLineMagnetic_current_loop)
//	KILL_THREAD(m_threadFieldLineMagnetic)
	KILL_THREAD(m_threadFieldLineMagnetostatic)
	KILL_THREAD(m_threadFieldLineGravitational)
	KILL_THREAD(m_threadFieldLine_Equipotential)
///////////////////////////////////////////////////////////////////////////////
#undef KILL_THREAD
	if (m_threadFieldLineElectric2D)
	{
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_threadFieldLineElectric2D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_threadFieldLineElectric2D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_threadFieldLineElectric2D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
		CloseHandle (m_threadFieldLineElectric2D->m_hThread);
		m_threadFieldLineElectric2D	= 0;
	}
//	ShowFieldLines(false);
//	ShowEquipotentials(false);
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ShowEquipotentials(bool bNewValue)
{
	if (m_bShowEquipotentials == bNewValue) return;
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	m_bShowEquipotentials = bNewValue;
//	pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay|CGLView::equipotentials);
//	if (!bNewValue) pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay^CGLView::equipotentials);
	if (m_bShowEquipotentials == false)
	{
		if (m_threadFieldLine_Equipotential)
		{
			m_threadFieldLine_Equipotential->PostThreadMessage(WM_QUIT, 0, 0);
			DWORD dwRet = MsgWaitForMultipleObjects(
				1,							// One event to wait for
				&m_threadFieldLine_Equipotential->m_hThread,		// The array of events
				FALSE,						// Wait for 1 event
				INFINITE,					// Timeout value
				QS_ALLINPUT);				// Any message wakes up
			CloseHandle (m_threadFieldLine_Equipotential->m_hThread);
			m_threadFieldLine_Equipotential	= 0;
		}
	}
	else
	{
		if (m_threadFieldLine_Equipotential) return;
		try {
			m_threadFieldLine_Equipotential = (FieldLine_Equipotential*)AfxBeginThread( RUNTIME_CLASS( FieldLine_Equipotential ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			m_threadFieldLine_Equipotential->m_bStepCompleted	= true;
			m_threadFieldLine_Equipotential->ResumeThread();
		}
		catch (wchar_t * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ShowFieldLines(int nFieldLinesType)
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	m_bShowFieldLines =( (nFieldLinesType&CGLView::electric) || (nFieldLinesType&CGLView::magnetic) || (nFieldLinesType&CGLView::gravitational) ) ;

//	pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay|CGLView::fieldLines);
//	if (!bNewValue) pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay^CGLView::fieldLines);
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_worldDimension==CSystem::two )
	{
		if (m_bShowFieldLines == false)
		{
			if (m_threadFieldLineElectric2D)
			{
				m_threadFieldLineElectric2D->PostThreadMessage(WM_QUIT, 0, 0);
				DWORD dwRet = MsgWaitForMultipleObjects(
					1,							// One event to wait for
					&m_threadFieldLineElectric2D->m_hThread,		// The array of events
					FALSE,						// Wait for 1 event
					INFINITE,					// Timeout value
					QS_ALLINPUT);				// Any message wakes up
				CloseHandle (m_threadFieldLineElectric2D->m_hThread);
				m_threadFieldLineElectric2D	= 0;
			}
		}
		else
		{
			if (m_threadFieldLineElectric2D)
			{
				m_threadFieldLineElectric2D->ResumeThread();
				return;
			}
			try {
				m_threadFieldLineElectric2D = (FieldLineElectric2D*)AfxBeginThread( RUNTIME_CLASS( FieldLineElectric2D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineElectric2D->m_bStepCompleted	= true;
				m_threadFieldLineElectric2D->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
		}
	}
	else
	if (m_system->m_worldDimension==CSystem::three )
	{
		if (m_bShowFieldLines == false)
		{
			if (m_threadFieldLineMagnetostatic)
			{
				m_threadFieldLineMagnetostatic->PostThreadMessage(WM_QUIT, 0, 0);
				DWORD dwRet = MsgWaitForMultipleObjects(
					1,							// One event to wait for
					&m_threadFieldLineMagnetostatic->m_hThread,		// The array of events
					FALSE,						// Wait for 1 event
					INFINITE,					// Timeout value
					QS_ALLINPUT);				// Any message wakes up
				CloseHandle (m_threadFieldLineMagnetostatic->m_hThread);
				m_threadFieldLineMagnetostatic	= 0;
			}
			if (m_threadFieldLineElectric3D)
			{
				m_threadFieldLineElectric3D->PostThreadMessage(WM_QUIT, 0, 0);
				DWORD dwRet = MsgWaitForMultipleObjects(
					1,							// One event to wait for
					&m_threadFieldLineElectric3D->m_hThread,		// The array of events
					FALSE,						// Wait for 1 event
					INFINITE,					// Timeout value
					QS_ALLINPUT);				// Any message wakes up
				CloseHandle (m_threadFieldLineElectric3D->m_hThread);
				m_threadFieldLineElectric3D	= 0;
			}
			if (m_threadFieldLineGravitational)
			{
				m_threadFieldLineGravitational->PostThreadMessage(WM_QUIT, 0, 0);
				DWORD dwRet = MsgWaitForMultipleObjects(
					1,							// One event to wait for
					&m_threadFieldLineGravitational->m_hThread,		// The array of events
					FALSE,						// Wait for 1 event
					INFINITE,					// Timeout value
					QS_ALLINPUT);				// Any message wakes up
				CloseHandle (m_threadFieldLineGravitational->m_hThread);
				m_threadFieldLineGravitational	= 0;
			}
		}
		else
		{
			if ((nFieldLinesType&CGLView::magnetic) && m_threadFieldLineMagnetostatic)
			{
				m_threadFieldLineMagnetostatic->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::magnetic)
			{
			try {
				m_threadFieldLineMagnetostatic =  (FieldLineMagnetostatic*)AfxBeginThread( RUNTIME_CLASS( FieldLineMagnetostatic ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineMagnetostatic->m_bStepCompleted	= true;
				m_threadFieldLineMagnetostatic->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}

			if ((nFieldLinesType&CGLView::electric) && m_threadFieldLineElectric3D)
			{
				m_threadFieldLineElectric3D->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::electric)
			{
			try {
				m_threadFieldLineElectric3D =  (FieldLineElectric3D*)AfxBeginThread( RUNTIME_CLASS( FieldLineElectric3D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineElectric3D->m_bStepCompleted	= true;
				m_threadFieldLineElectric3D->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}

			if ((nFieldLinesType&CGLView::gravitational) && m_threadFieldLineGravitational)
			{
				m_threadFieldLineGravitational->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::gravitational)
			{
			try {
				m_threadFieldLineGravitational =  (FieldLineGravitational*)AfxBeginThread( RUNTIME_CLASS( FieldLineGravitational ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineGravitational->m_bStepCompleted	= true;
				m_threadFieldLineGravitational->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}


		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ComputeFieldLine(int nFieldLinesType)
{
	if (m_bShowFieldLines == false) return;
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_worldDimension==CSystem::two )
	{
		if (m_threadFieldLineElectric2D==0) return;
		try {
			if (m_threadFieldLineElectric2D->m_bStepCompleted)
				m_threadFieldLineElectric2D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
		}
		catch (wchar_t * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
	else
	if (m_system->m_worldDimension==CSystem::three )
	{
			if ((nFieldLinesType&CGLView::magnetic) && m_threadFieldLineMagnetostatic)
			{
				m_threadFieldLineMagnetostatic->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::magnetic)
			{
			try {
				m_threadFieldLineMagnetostatic =  (FieldLineMagnetostatic*)AfxBeginThread( RUNTIME_CLASS( FieldLineMagnetostatic ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineMagnetostatic->m_bStepCompleted	= true;
				m_threadFieldLineMagnetostatic->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}

			if ((nFieldLinesType&CGLView::electric) && m_threadFieldLineElectric3D)
			{
				m_threadFieldLineElectric3D->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::electric)
			{
			try {
				m_threadFieldLineElectric3D =  (FieldLineElectric3D*)AfxBeginThread( RUNTIME_CLASS( FieldLineElectric3D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineElectric3D->m_bStepCompleted	= true;
				m_threadFieldLineElectric3D->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}

			
			if ((nFieldLinesType&CGLView::gravitational) && m_threadFieldLineGravitational)
			{
				m_threadFieldLineGravitational->ResumeThread();
			}
			else if (nFieldLinesType&CGLView::gravitational)
			{
			try {
				m_threadFieldLineGravitational =  (FieldLineGravitational*)AfxBeginThread( RUNTIME_CLASS( FieldLineGravitational ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				m_threadFieldLineGravitational->m_bStepCompleted	= true;
				m_threadFieldLineGravitational->ResumeThread();
			}
			catch (wchar_t * str)
			{
				AfxMessageBox(str);
			}
			catch( SolveIt_Error& e){e.why();}catch(...)
			{
			}
			}

			if (m_threadFieldLineGravitational==0 && m_threadFieldLineElectric3D==0 && m_threadFieldLineMagnetostatic==0) return;
			
			
			
			
		try {
			if (nFieldLinesType&CGLView::electric&&m_threadFieldLineElectric3D && m_threadFieldLineElectric3D->m_bStepCompleted)
				m_threadFieldLineElectric3D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
			if (nFieldLinesType&CGLView::magnetic&&m_threadFieldLineMagnetostatic && m_threadFieldLineMagnetostatic->m_bStepCompleted)
				m_threadFieldLineMagnetostatic->PostThreadMessage(WM_USER_START_PASS, 0, 0);
			if (nFieldLinesType&CGLView::gravitational&&m_threadFieldLineGravitational && m_threadFieldLineGravitational->m_bStepCompleted)
				m_threadFieldLineGravitational->PostThreadMessage(WM_USER_START_PASS, 0, 0);
		}
		catch (wchar_t * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ComputeEquipotentialLine(void)
{
	if (m_bShowEquipotentials == false) return;
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_worldDimension==CSystem::two )
	{
		if (m_threadFieldLine_Equipotential==0) return;
		try {
			if (m_threadFieldLine_Equipotential->m_bStepCompleted)
				m_threadFieldLine_Equipotential->PostThreadMessage(WM_USER_START_PASS, 0, 0);
		}
		catch (wchar_t * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
	else
	if (m_system->m_worldDimension==CSystem::three )
	{
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::ShowVectorFields(bool bNewValue)
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
//	bool bCurrent = (pGLView->m_FieldDisplay&CGLView::vectorFields)?true:false;
//	ASSERT(bCurrent == m_bShowVectorFields);
//	if (bCurrent == bNewValue) return;
	m_bShowVectorFields = bNewValue;
//	pGLView->m_FieldDisplay = (enum CGLView::FieldDisplay)(pGLView->m_FieldDisplay^CGLView::vectorFields);
	if (m_bShowVectorFields == false)
	{
//		Clear();
	}
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::N_Body_GravitationalField (const Point3D& p)
{
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	Vector3D f;
	if (m_GravitationalCoupling == 0.0) return f;
try
{
//	double& g	= m_ExternalGravitationalFieldY;
	ObjectsList::iterator it;
	for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return ZERO_3D;
		if (pbody->m_fMassInverse == 0.0) continue;
		Vector3D rel = p - pbody->x;
		double rCubed = pow(rel*rel,1.5);
		if (rCubed < 1.0e-30) {
			f.normalize();
			return f;
	//		throw CExceptionTerminateFieldLine();
		}
		rel /= rCubed;
		f += (-m_GravitationalCoupling/pbody->m_fMassInverse)*rel;
	}

}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return ZERO_3D;
	}
	return f;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::N_Body_ElectrostaticField (const Point3D& p)
{
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	Vector3D f;
try
{
	ObjectsList::iterator it;
	for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return ZERO_3D;
		if (pbody->m_fCharge == 0.0) continue;
		Vector3D rel = p - pbody->x;
		double rCubed = pow(rel*rel,1.5);
		if (rCubed < 1.0e-30) {
			f.normalize();
			return f;
	//		throw CExceptionTerminateFieldLine();
		}
		rel /= rCubed;
		f += pbody->m_fCharge*rel;
	}
}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return ZERO_3D;
	}
	return f;
}
///////////////////////////////////////////////////////////////////////////////
Vector2D CFieldsManager::E(const Vector2D& pt) {
	const double smallR = 1.0e-6;
	CSystem* m_system = theApp.GetSystem();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();

	Vector2D e;
	try {
    if (rigidBodyManager->empty()) return e;
	ObjectsList::iterator it;
	for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
	{
		CRigidBody* c = dynamic_cast<CRigidBody*>(dynamic_cast<CRigidBody *>((*it).rbPtr));
		if (c!=NULL) {
			Point3D p = c->x;
			Vector2D m_fPt(p.x, p.y);
			Vector2D q= pt - m_fPt;
			double R = q.Norm();
			if (R < smallR) R=smallR;
			q/=R,q/=R,q/=R;q*=c->m_fCharge;
			e += q;
		}
		else throw SolveIt_ERROR(L"pbody == 0");//
	}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	e *= m_fScaleElectricField;
	return e;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::Static_GravitationalField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D G(m_ExternalGravitationalField);

	try {
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	G += m_GravitationalCoupling*rigidBodyManager->Static_GravitationalField (p, pBody);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return G;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::ElectrostaticField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D E(m_ExternalElectricField);

	try {
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	E += rigidBodyManager->ElectrostaticField (p, pBody);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return E;
}
///////////////////////////////////////////////////////////////////////////////

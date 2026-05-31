// System.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "System.h"

#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "parser.h" /* for token definitions and yylval */

#include "LeftView.h"
#include "SolveItDoc.h"
#include "SolveItView.h"
#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
//#include "Worker.h"
#include "Cruncher.h"
#include "GLView.h"
#include "PlayMap.h"
#include "QM/qm_1dscatt.h"
#include "QM/qm_2dscatt.h"
#include "QM/qm_ground_mc_1d.h"
#include "QM/qmstationary1d.h"
#include "QM/DataQM_Scatt1D.h"
#include "QM/DataQM_Scatt2D.h"
#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Stationary2D.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"

#define AfxMessageBox(msg) AfxMessageBox(GetWC(msg))
#include "RigidBody/shapes.h"
#include "RigidBody/ConstraintManager.h"
#include "RigidBody/ExamineCollisionTimes.h"
#include "CalcMap.h"
#include <functional>
#include <algorithm>
#include "QM/QMStationary2D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
	long ToLongSize(size_t value)
	{
		return static_cast<long>(value);
	}
}

//#define data__QM_Stationary1D this
///////////////////////////////////////////////////////////////////////////////
extern GL2PScontext *gl2ps;
extern Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
///////////////////////////////////////////////////////////////////////////////
void CSystem::SingleStep()
{
try {
		m_bSingleStepping	= true;
		m_bSingleStep_Mode	= true;
				{
				Lock lock (theApp.protectWorker);
	if (m_bRunning && worker != NULL)
	{
		SetEvent(g_EventStop);
	}
				}
		m_bRunning			= false;
	if ((m_bDoRigidBody || m_bDoMultiBody) && rigidBodyManager->size())
	{
	//	CreateWorkerThread(true);
		if (worker == NULL) CreateWorkerThread(true);
		if (worker == NULL) throw SolveIt_ERROR("worker == NULL");
	//	worker->ResumeThread();
	//	worker->PostThreadMessage(WM_USER_STEP, 0, m_lTakeNumSteps);
		SetEvent(g_EventStep);
	}
	else
	if (m_bDoQM_Scatt1D)
	{
		CreateQM_Scatt1DThread(true);
		data__QM_Scatt1D->m_pCQM_Scatt1D->ResumeThread();
		data__QM_Scatt1D->m_pCQM_Scatt1D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox("CSystem::SingleStep: unknown exception");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::Run() 
{
try {
	m_bStopIterations	= false;
///////////////////////////////////////////////////////////////////////////////
	if (m_lTakeNumSteps <= 0)
	{
		m_bSingleStepping	= false;
		m_bSingleStep_Mode	= false;
		m_bRunning			= true;
	}
	else
	{
		m_bSingleStepping	= true;
		m_bSingleStep_Mode	= true;
		m_bRunning			= true;
	}
///////////////////////////////////////////////////////////////////////////////
	if ((m_bDoRigidBody || m_bDoMultiBody) && rigidBodyManager->size())
	{
	//	CreateWorkerThread(true);
		if (worker == NULL) CreateWorkerThread(true);
		if (worker == NULL) throw SolveIt_ERROR("worker == NULL");
	//	worker->ResumeThread();
				Lock lock (theApp.protectWorker);
		SetEvent(g_EventRun);
//		worker->PostThreadMessage(WM_USER_START_PASS, 0, m_lTakeNumSteps);
	}
	else
	if (m_bDoVerhulstDynamics)
	{
		CreatePlayerThread(true);
		player->ResumeThread();
		player->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	if (m_bDoMapper)
	{
		CreateMapperThread(true);//
		mapper->ResumeThread();//
	//	mapper->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	if (m_bDoQMStationary1D)
	{
		bool bProvideInitializationEvent = false;
		if (m_pCQMStationary1D == NULL)
		{
			bProvideInitializationEvent = true;
			BOOL manual_reset = TRUE;
			BOOL initial_state_signaled = FALSE;
			g_EventHandle		= CreateEvent (0, manual_reset, initial_state_signaled, 0);
		}

		CreateQMStationary1DThread(true);

		m_pCQMStationary1D->ResumeThread();
		if (bProvideInitializationEvent)
		{
			WaitForSingleObject(g_EventHandle, INFINITE);
			CloseHandle (g_EventHandle);
			g_EventHandle	= 0;
		}
		m_pCQMStationary1D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	if (m_bDoQMStationary2D)
	{
		CreateQMStationary2DThread(true);
		if (data__QM_Stationary2D->m_pCQMStationary2D == NULL) throw SolveIt_ERROR("CSystem::Run(): data__QM_Stationary2D->m_pCQMStationary2D == NULL");
		data__QM_Stationary2D->m_pCQMStationary2D->ResumeThread();
		data__QM_Stationary2D->m_pCQMStationary2D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	if (m_bDoQM_Scatt1D)
	{
		CreateQM_Scatt1DThread(true);
		data__QM_Scatt1D->m_pCQM_Scatt1D->ResumeThread();
		data__QM_Scatt1D->m_pCQM_Scatt1D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	if (m_bDoQM_Scatt2D)
	{
		CreateQM_Scatt2DThread(true);
		m_pCQM_Scatt2D->ResumeThread();
		m_pCQM_Scatt2D->PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox("CSystem::Run: unknown error");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::Stop(void)
{
	m_bSingleStepping	= false;
//	m_bSingleStep_Mode	= true;
	m_bRunning			= false;
	m_bStopIterations	= true;
	if (m_pCQMStationary1D)	m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, WM_CLOSE);

try {
	BOOL ret1=StopWorker();
	BOOL ret2=StopPlayer();
	 ret2|=StopMapper();
	fieldsManager->KillThreads();
	BOOL ret3=1;//StopQMStationary1D();
	BOOL ret4=StopQMStationary2D();
	BOOL ret5=StopQM_Scatt1D();
	BOOL ret6=StopQM_Scatt2D();

//	ASSERT(worker == 0);
	ASSERT(player == 0);
	ASSERT(m_pCQMStationary1D == 0);
	ASSERT(data__QM_Stationary2D->m_pCQMStationary2D == 0);
	ASSERT(data__QM_Scatt1D->m_pCQM_Scatt1D == 0);
	ASSERT(m_pCQM_Scatt2D == 0);
	ASSERT(m_pCQM_Ground_MC_1D == 0);

	return ret1||ret2||ret3||ret4||ret5||ret6;
}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox("CSystem::Stop");
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateWorkerThread(bool bCREATE_SUSPENDED)
{
				Lock lock (theApp.protectWorker);
	m_NumberVariables	= 0;
	if (rigidBodyManager->RigidLinkJointStates.size())
	{
		m_NumberVariables = ToLongSize(rigidBodyManager->RigidLinkJointStates.size());
	}
	else
	if (m_NumberVariables != rigidBodyManager->size())
	{
		m_NumberVariables = DEGREES_FREEDOM * ToLongSize(rigidBodyManager->size());
		rigidBodyManager->m_system_state.clear();
		rigidBodyManager->m_system_state.is(0.0, m_NumberVariables);
	}
	if (m_NumberVariables	== 0) return;
	{
		if (worker)
		{
			DWORD ExitCode=0;
			BOOL ret = GetExitCodeThread(worker->m_hThread, &ExitCode);
			if (ret == 0)
			{
			//	DWORD err = GetLastError();
				CloseHandle (worker->m_hThread);
				worker	= 0;
			//	_variant_t v = long(err);
			//	_bstr_t b(v);
		LPVOID lpMsgBuf= DisplayErrorText();
		_bstr_t b((const char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
				throw SolveIt_ERROR((const char *) b);
			}
			if (ExitCode == STILL_ACTIVE)
			{
			//	if (!bCREATE_SUSPENDED) worker->ResumeThread();
				ASSERT(worker->m_pCSystem ==  this);
				worker->ResumeThread();
//				worker->PostThreadMessage(WM_USER_START_PASS, 0, 0);
				return;
			}
			else
			{
				CloseHandle (worker->m_hThread);
				worker	= 0;
			}
		}
		try {
			if (bCREATE_SUSPENDED) 
worker =  (CCruncher*)AfxBeginThread( RUNTIME_CLASS( CCruncher ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
			worker =  (CCruncher*)AfxBeginThread( RUNTIME_CLASS( CCruncher ));
			worker->m_pCSystem =  this;
			worker->ResumeThread();
		}
		catch (char * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopWorker(void)
{
	if (worker != NULL)
	{
		ASSERT(worker->m_pCSystem == this);
		ASSERT(worker->m_pCSystem == theApp.GetSystem());
	}
	{
	//	Lock lock (theApp.protectBodies);
		m_bSingleStepping	= false;
//		m_bSingleStep_Mode	= true;
	if (m_bRunning && worker != NULL)
	{
		SetEvent(g_EventStop);
	}
		m_bRunning			= false;
		m_bStopIterations	= true;
//		KillWorker();
		if (worker != NULL)
		{
	//		SetEvent(g_EventStop);
	//		worker->PostThreadMessage(WM_USER_TERM, 0, 0);//misnomer; only halts
		}
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::KillWorker(void)
{
try {
//	Lock lock (theApp.protectWorker);	
	m_bStopIterations = true;
	if (worker == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(worker->m_hThread, &ExitCode);
	if (ret == 0)
	{
	//	DWORD err = GetLastError();
		CloseHandle (worker->m_hThread);
		worker	= 0;
	//	_variant_t v = long(err);
	//	_bstr_t b(v);
		LPVOID lpMsgBuf= DisplayErrorText();
		_bstr_t b((const char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
		throw SolveIt_ERROR((const char *) b);
		return TRUE;
	}
	if (ExitCode == STILL_ACTIVE)
	{
		worker->ResumeThread();
		worker->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&worker->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,				// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (worker->m_hThread);
	worker	= 0;
	return TRUE;
}
catch (char * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}
catch(...)
{
		AfxMessageBox("CSystem::StopWorker");
}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreatePlayerThread(bool bCREATE_SUSPENDED) 
{
	m_bStopIterations	= false;
	if (player)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(player->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) player->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (player->m_hThread);
			player	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
player =  (CPlayMap*)AfxBeginThread( RUNTIME_CLASS( CPlayMap ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		player =  (CPlayMap*)AfxBeginThread( RUNTIME_CLASS( CPlayMap ));
	}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopPlayer(void)
{
	m_bStopIterations = true;
	if (player == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(player->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		player->ResumeThread();
		player->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&player->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (player->m_hThread);
	player	= 0;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateMapperThread(bool bCREATE_SUSPENDED)
{
	m_bStopIterations	= false;
	if (mapper)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(mapper->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) mapper->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (mapper->m_hThread);
			mapper	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
mapper =  (CCalcMap*)AfxBeginThread( RUNTIME_CLASS( CCalcMap ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		mapper =  (CCalcMap*)AfxBeginThread( RUNTIME_CLASS( CCalcMap ));
	}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}

///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopMapper(void)
{
	m_bStopIterations = true;
	if (mapper == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(mapper->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		mapper->ResumeThread();
		mapper->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&mapper->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (mapper->m_hThread);
	mapper	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateQM_Scatt1DThread(bool bCREATE_SUSPENDED)
{
	m_bStopIterations	= false;
	data__QM_Scatt1D->CreateQM_Scatt1DThread(bCREATE_SUSPENDED);
/*
	if (m_pCQM_Scatt1D)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCQM_Scatt1D->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCQM_Scatt1D->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (m_pCQM_Scatt1D->m_hThread);
			m_pCQM_Scatt1D	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
m_pCQM_Scatt1D =  (CQM_Scatt1D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt1D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		m_pCQM_Scatt1D =  (CQM_Scatt1D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt1D ));
	}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
*/
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopQM_Scatt1D(void)
{
	m_bStopIterations = true;
	return data__QM_Scatt1D->StopQM_Scatt1D();
/*
	if (m_pCQM_Scatt1D == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQM_Scatt1D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCQM_Scatt1D->ResumeThread();
		m_pCQM_Scatt1D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCQM_Scatt1D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (m_pCQM_Scatt1D->m_hThread);
	m_pCQM_Scatt1D	= 0;
	return TRUE;
*/
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateQM_Scatt2DThread(bool bCREATE_SUSPENDED)
{
	m_bStopIterations	= false;
	if (m_pCQM_Scatt2D)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCQM_Scatt2D->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCQM_Scatt2D->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (m_pCQM_Scatt2D->m_hThread);
			m_pCQM_Scatt2D	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
m_pCQM_Scatt2D =  (CQM_Scatt2D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt2D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		m_pCQM_Scatt2D =  (CQM_Scatt2D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt2D ));
	}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopQM_Scatt2D(void)
{
	m_bStopIterations = true;
	if (m_pCQM_Scatt2D == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQM_Scatt2D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCQM_Scatt2D->ResumeThread();
		m_pCQM_Scatt2D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					2,						// One event to wait for
					&m_pCQM_Scatt2D->m_hThread,		// The array of events
					FALSE,					// Wait for 2 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (m_pCQM_Scatt2D->m_hThread);
	m_pCQM_Scatt2D	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/*
void CSystem::CreateQMStationary1DThread(bool bCREATE_SUSPENDED)
{
	m_bStopIterations	= false;
	CreateQMStationary1DThread(bCREATE_SUSPENDED);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopQMStationary1D(void)
{
	m_bStopIterations = true;
	return StopQMStationary1D();
}
*/
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateQMStationary2DThread(bool bCREATE_SUSPENDED)
{
	m_bStopIterations	= false;
	data__QM_Stationary2D->CreateQMStationary2DThread(bCREATE_SUSPENDED);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopQMStationary2D(void)
{
//	m_bStopIterations = true;
	if (data__QM_Stationary2D->m_pCQMStationary2D == NULL ) return FALSE;
	return data__QM_Stationary2D->StopQMStationary2D();
//	DWORD ExitCode=0;
//	BOOL ret = GetExitCodeThread(m_pCQMStationary2D->m_hThread, &ExitCode);
//	if (ExitCode == STILL_ACTIVE)
//	{
//		m_pCQMStationary2D->PostThreadMessage(WM_QUIT, 0, 0); 
//		DWORD dwRet = MsgWaitForMultipleObjects(
//					2,						// One event to wait for
//					&m_pCQMStationary2D->m_hThread,		// The array of events
//					FALSE,					// Wait for 2 event
//					INFINITE,						// Timeout value
//					QS_ALLINPUT);			// Any message wakes up
//	}
//	CloseHandle (m_pCQMStationary2D->m_hThread);
//	m_pCQMStationary2D	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/*
void CSystem::Stop(void)
{
	m_bStopIterations = true;
	if (worker)
	{
		worker->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&worker->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
		CloseHandle (worker->m_hThread);
		worker	= 0;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
/*
DWORD CSystem::Halt(void)
{
	m_bStopIterations = true;
//	Lock lock (protectBodies);
	if (hWndThread==0) return WAIT_OBJECT_0;
	::PostMessage(hWndThread, WM_USER_TERM, 0, 0);//WM_USER_PASS_DONE
//	DWORD dwRet = MsgWaitForMultipleObjects(
//					1,						// One event to wait for
//					&g_EventHaltThread,		//g_EventHandle The array of eventsG_EventKillThread
//					FALSE,					// Wait for 1 event
//					INFINITE,						// Timeout value
//					QS_ALLINPUT);			// Any message wakes up
	DWORD dwRet = WaitForSingleObject(g_EventThreadHalted, INFINITE);
	ResetEvent(g_EventHaltThread);
	SetWindowText(hWndThread, _T("Stopped...")); // window name
	return dwRet;
}
*/
///////////////////////////////////////////////////////////////////////////////
void CSystem::Halted(void)
{
	m_bStopIterations = true;
//	if (worker) delete(worker);
				Lock lock (theApp.protectWorker);
	worker				= 0;
/*
	CloseHandle (thread_handle);
	CloseHandle (g_EventHaltThread);
	CloseHandle (g_EventThreadHalted);
	thread_handle	= 0;
	hWndThread		= 0;
	thread_tid		= 0;
	g_EventHaltThread	= 0;
	g_EventThreadHalted	= 0;
*/
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::PrepareForStep()
{
/*
	if (rigidBodyManager->RigidLinkJointStates.size())
	{
		m_NumberVariables = rigidBodyManager->RigidLinkJointStates.size();
	//	rigidBodyManager->StepMultibodyState();
		return;//
	}
	if (m_NumberVariables != rigidBodyManager->size())
	{
		m_NumberVariables = DEGREES_FREEDOM*rigidBodyManager->size();
		rigidBodyManager->m_system_state.clear();
		rigidBodyManager->m_system_state.is(0.0, m_NumberVariables);
	}
*/
	rigidBodyManager->SetPreviousStateOfBodies();
	(void) GetTargetTime();
// Set the initial conditions:
	rigidBodyManager->m_system_state.clear();
	rigidBodyManager->GetStateOfBodies();
}
///////////////////////////////////////////////////////////////////////////////
/*
HWND	CSystem::hWndThread			= 0;
HANDLE	CSystem::thread_handle		= 0;
DWORD	CSystem::thread_tid			= 0;
HANDLE	CSystem::g_EventHandle		= 0;
HANDLE	CSystem::g_EventHaltThread	= 0;
bool	CSystem::m_bStopIterations	= false;
	m_bDoQMStationary2D		= false;
				= false;
	m_bDoQM_Scatt2D			= false;
	m_bDoQM_Ground_MC_1D	= false;
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 10
///////////////////////////////////////////////////////////////////////////////
void CSystem::CreateQMStationary1DThread(bool bCREATE_SUSPENDED)
{
//	m_bStopIterations	= false;
	if (m_pCQMStationary1D)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCQMStationary1D->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCQMStationary1D->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (m_pCQMStationary1D->m_hThread);
			m_pCQMStationary1D	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
m_pCQMStationary1D =  (CQMStationary1D*)AfxBeginThread( RUNTIME_CLASS( CQMStationary1D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		m_pCQMStationary1D =  (CQMStationary1D*)AfxBeginThread( RUNTIME_CLASS( CQMStationary1D ));
	}
	catch (char * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSystem::StopQMStationary1D(void)
{
//	m_bStopIterations = true;
	if (m_pCQMStationary1D == NULL ) return FALSE;
	if (m_pCQMStationary1D->m_hThread == NULL ) return FALSE;
	try
	{
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQMStationary1D->m_hThread, &ExitCode);
	if (ret == 0)
	{
		LPVOID lpMsgBuf= DisplayErrorText();
		_bstr_t b((const char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
	//	ret = CloseHandle (m_pCQMStationary1D->m_hThread);
	//	if (ret == 0)
	//	{
	//		LPVOID lpMsgBuf= DisplayErrorText();
	//		b += _bstr_t((const char*)lpMsgBuf);
	//		LocalFree( lpMsgBuf );
	//	}
		m_pCQMStationary1D->m_hThread=0;
		m_pCQMStationary1D	= 0;
		throw SolveIt_ERROR((const char *) b);
	}
	if (ExitCode == STILL_ACTIVE)
	{
	//	HANDLE h = m_pCQMStationary1D->m_hThread;
		m_pCQMStationary1D->ResumeThread();
		m_pCQMStationary1D->PostThreadMessage(WM_QUIT, 0, 0); 
	//	::PostThreadMessage(m_pCQMStationary1D->m_nThreadID, WM_QUIT, 0, 0);
	//	DWORD dwRet = WaitForSingleObject(h, INFINITE);
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCQMStationary1D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
		ret = CloseHandle (m_pCQMStationary1D->m_hThread);
		if (ret == 0)
		{
			m_pCQMStationary1D->m_hThread=0;
			m_pCQMStationary1D	= 0;
			LPVOID lpMsgBuf= DisplayErrorText();
			_bstr_t b((const char*)lpMsgBuf);
			LocalFree( lpMsgBuf );
			throw SolveIt_ERROR((const char *) b);
		}
		m_pCQMStationary1D->m_hThread=0;
		m_pCQMStationary1D	= 0;
		return TRUE;
	}
	else
	{
		m_pCQMStationary1D->m_hThread=0;
		m_pCQMStationary1D	= 0;
	}
	}
	catch (char * str){AfxMessageBox(str);	}//
	catch( SolveIt_Error& e){e.why();}//
	catch(...)	{AfxMessageBox("CSystem::StopQMStationary1D");}//
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////////////




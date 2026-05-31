// Cruncher.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "Cruncher.h"

#include "System.h"
#include "RigidBody/RigidBodyManager.h"
//#include "Matherror.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// _CCruncher

IMPLEMENT_DYNCREATE(_CCruncher, CWinThread)

/////////////////////////////////////////////////////////////////////////////
// CCruncher
CCruncher::CCruncher():
	m_lTakeNumSteps(theApp.GetSystem()->m_lTakeNumSteps),
	m_NumberVariables(theApp.GetSystem()->m_NumberVariables),
	m_lNumStepsTaken(0),
	m_pCSystem(0),
/////////////////////////////////////////////////////////////////////////////
	m_bSingleStepping(theApp.GetSystem()->m_bSingleStepping),
	m_bSingleStep_Mode(theApp.GetSystem()->m_bSingleStep_Mode),
	m_bRunning(theApp.GetSystem()->m_bRunning),
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	m_bSingleStepping(theApp.GetSystem()->m_bSingleStepping),
//	m_bSingleStep(false),
///////////////////////////////////////////////////////////////////////////////
	bStepMultibody(theApp.GetRigidBodyManager()->RigidLinkJointStates.size()>0)
{
}           // protected constructor used by dynamic creation

IMPLEMENT_DYNCREATE(CCruncher, CWinThread)

BOOL CCruncher::InitInstance()
{
	ASSERT(m_pCSystem == theApp.GetSystem());
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0);//misnomer for Wait()
	return TRUE;
}

int CCruncher::ExitInstance()
{
//	ASSERT(0);
	ASSERT(m_pCSystem == theApp.GetSystem());
//	return CWinThread::ExitInstance();

//	Lock lock (theApp.protectWorker);
	if (m_pCSystem != NULL && m_pCSystem->worker != NULL)
	{
		CloseHandle (m_pCSystem->worker->m_hThread);
		m_pCSystem->worker	= 0;
	}

	return CWinThread::ExitInstance();
}

//BEGIN_MESSAGE_MAP(CCruncher, _CCruncher)
BEGIN_MESSAGE_MAP(CCruncher, CWinThread)
	//{{AFX_MSG_MAP(CCruncher)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
ON_REGISTERED_THREAD_MESSAGE(WM_USER_STEP, OnSingleStep)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnRun)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
END_MESSAGE_MAP()

//ON_THREAD_MESSAGE(UINT msg, LRESULT (handler *)(WPARAM, LPARAM))
//ON_REGISTERED_THREAD_MESSAGE(UINT msg, LRESULT (handler *)(WPARAM, LPARAM))
//

/////////////////////////////////////////////////////////////////////////////
// CCruncher message handlers
///////////////////////////////////////////////////////////////////////////////
void CCruncher::Wait()
{
	ASSERT(m_pCSystem == theApp.GetSystem());
	try
	{
		HANDLE events[] = {g_EventRun, g_EventStep, g_EventStop, g_EventKill};
	//	DWORD dwRet = WaitForSingleObject(g_EventStep, INFINITE);
		DWORD dwRet = MsgWaitForMultipleObjects(
				sizeof(events)/sizeof(events[0]),	// # events to wait for
				events,								// The array of events
				FALSE,								// Wait for 1 event
				INFINITE,							// Timeout value
				QS_ALLINPUT);						// Any message wakes up
		if(dwRet == WAIT_OBJECT_0+0)
			{
				OnRun(0, 0);
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+1)
			{
				OnSingleStep(0, 0);
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+2)
			{
				Wait();
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+3)
			{
			//	Lock lock (theApp.protectWorker);
				if(m_pCSystem != NULL && m_pCSystem->worker != NULL)
				{
					CloseHandle (m_pCSystem->worker->m_hThread);
					m_pCSystem->worker	= 0;
				}
				AfxEndThread( 0 );
				return;
			}
		else
			{
				throw SolveIt_ERROR(L"CCruncher::Wait: Invalid return from MsgWaitForMultipleObjects");
				AfxEndThread( 0 );
				return;
			}
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox(L"CCruncher::Wait");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
#define WM_RESULT WM_USER+AnyReasonableValue
When the WM_RESULT synchronization message is called, the handler will get a 
WPARAM and LPARAM value. This lets you know whether or not the message was 
successfully transferred, along with some associated information.


BOOL CMyObj::WaitForResult(WPARAM *pwPar,
                           LPARAM *plPar)
{
 BOOL bRet = FALSE;
 MSG Msg;
 while(TRUE)
 {
   while(::PeekMessage(&Msg,NULL, 0, 0, PM_REMOVE))
   {
     if((Msg.message==WM_RESULT)||
        (Msg.message==WM_QUIT))
     {
       *pwPar = Msg.wParam;
       *plPar = Msg.lParam;
       bRet = TRUE;
       if(Msg.message == WM_QUIT)
         PostQuitMessage(0);
       return bRet;
     }
     TranslateMessage(&Msg);
     ::DispatchMessage(&Msg);
   }
   // Yield control to other threads and processes
   WaitMessage();
 }
 return bRet;
}

Call WaitForResult() whenever you need to wait to finish one or more asynchronous 
actions. Once the whole asynchronous task is finished, post a WM_RESULT message 
containing information about how the task was finished in the WPARAM and LPARAM 
variables. Once the message is captured internally from the WaitForResult(), the 
function will return and stop blocking the line of execution.

#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CCruncher::OnRun(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pCSystem == theApp.GetSystem());
try
{
	m_lNumStepsTaken	= 0;
//	m_lTakeNumSteps		= lParam;
	while (m_bRunning && ((m_lTakeNumSteps==0) || (m_lNumStepsTaken < m_lTakeNumSteps)))
	{
		{
		//	//Lock lock (theApp.protectBodies);
			m_bSingleStepping	= (m_bSingleStep_Mode && (m_lNumStepsTaken < m_lTakeNumSteps) );
			if ((m_pCSystem->m_fStopTime>0.0) && (m_pCSystem->m_fStopTime <= m_pCSystem->m__t) )
			{
				m_pCSystem->m_bStopIterations	= true;
				m_bRunning						= false;
			}
		}
		if (m_bRunning)
		{
			if (MakeTimeStep())//error
			{
			//	Lock lock (theApp.protectWorker);
				if (m_pCSystem->worker)
				{
					CloseHandle (m_pCSystem->worker->m_hThread);
					m_pCSystem->worker	= 0;
				}
				AfxEndThread(0);
			//	PostQuitMessage(0);
			}
			++m_lNumStepsTaken;
			DWORD dwRet = WaitForSingleObject(g_EventKill, 0);
			if(dwRet == WAIT_OBJECT_0)
			{
			//	Lock lock (theApp.protectWorker);
				if (m_pCSystem->worker)
				{
					CloseHandle (m_pCSystem->worker->m_hThread);//
					m_pCSystem->worker	= 0;
				}
				AfxEndThread(0);
				return;
			}
			dwRet = WaitForSingleObject(g_EventStop, 0);
			if(dwRet == WAIT_OBJECT_0)
			{
				goto StepOut;
			}
		}
		else
		{
			break;
		}
	}
StepOut:
	m_lNumStepsTaken	= 0;
	m_bSingleStepping	= false;
	m_bRunning			= false;
	Wait();
}
catch (wchar_t * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}
catch(...)
{
		AfxMessageBox(L"CCruncher::OnRun");
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CCruncher::OnSingleStep(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pCSystem == theApp.GetSystem());
try
{
//	if (!m_bSingleStep_Mode) return;
//	m_lNumStepsTaken	= 0;
//	m_lTakeNumSteps		= lParam;
//	while (m_lNumStepsTaken < m_lTakeNumSteps)
	{
//	{
//		//Lock lock (theApp.protectBodies);
//		m_bSingleStepping	= (m_bSingleStep_Mode && (m_lNumStepsTaken < m_lTakeNumSteps) );
//		m_bRunning			= ( (m_bRunning&&!m_bSingleStep_Mode) || m_bSingleStepping);
//	}
		if (MakeTimeStep())
		{
	//		Lock lock (theApp.protectWorker);
			if (m_pCSystem->worker)
			{
				CloseHandle (m_pCSystem->worker->m_hThread);
				m_pCSystem->worker	= 0;
			}
			AfxEndThread(0);
//		//	PostQuitMessage(0);
		}
		++m_lNumStepsTaken;
		Wait();
	}
}
catch (wchar_t * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}
catch(...)
{
		AfxMessageBox(L"CCruncher::OnSingleStep");
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CCruncher::OnHalt(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pCSystem == theApp.GetSystem());
	Wait();
//	if (m_pCSystem->worker)
//	{
//		CloseHandle (m_pCSystem->worker->m_hThread);
//		m_pCSystem->worker	= 0;
//	}
//	AfxEndThread(0);
////	PostQuitMessage(0);
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SolveIt::CVodeDerivative(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
//	CRigidBodyManager* pRigidBodyManager = (CRigidBodyManager * ) f_data;pRigidBodyManager->
	theApp.GetRigidBodyManager()->TimeDerivativeOfSystemState( t, stateVector->data, dx->data);
}
///////////////////////////////////////////////////////////////////////////////
//		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//		pFrame->PostMessage(WM_USER_PASS_DONE, 0, 0);
///////////////////////////////////////////////////////////////////////////////
long CCruncher::MakeTimeStep()
{
	ASSERT(m_pCSystem == theApp.GetSystem());
{
	//Lock lock (theApp.protectBodies);
	//CSystem* m_system = theApp.GetSystem();
#if 0
	if (m_pCSystem->m_bStopIterations)
	{
		AfxEndThread(0);
//		PostQuitMessage(0);//ExitThread(0);
		return 0;
	}
#endif

//	bool bStepMultibody	= false;
//	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
//	if (rigidBodyManager->RigidLinkJointStates.size())
//	{
//		bStepMultibody	= true;
//	}
	m_pCSystem->PrepareForStep();//exclusive calling rights
	if ((m_pCSystem->m_fStopTime>0.0) && (m_pCSystem->m_fStopTime <= m_pCSystem->m__t) )
	{
		Wait();
	//	AfxEndThread(0);
//	//	PostQuitMessage(0);//ExitThread(0);
		return 0;
	}
}
	long flag = -1;
try {
	flag = bStepMultibody?StepMultibodyState():CVodeStep();
	}
	catch (wchar_t * str)
	{
//		m_pCSystem->m_bStopIterations = true;
		AfxMessageBox(str);
		return -1;
	}
	catch( SolveIt_Error& e)
	{
		e.why();
		return -4;
	}
	catch(...)// there was an error
	{
		MessageBox(0,L"Singularity", L"CCruncher::MakeTimeStep", MB_OK);
		return -2;
	}
	if (flag == 0)//SUCCESS
	{
		//Lock lock (theApp.protectBodies);
		//CSystem* m_system = theApp.GetSystem();
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		if (bStepMultibody)
		{
		}
		else if ( !((m_pCSystem->m_fStopTime>0.0) && (m_pCSystem->m_fStopTime <= m_pCSystem->m__t) ))
		{
//			rigidBodyManager->SetStateOfBodies(FALSE);// SetPreviousStateOfBodies done at top of loop
		}
		else return 0;
///////////////////////////////////////////////////////////////////////////////
		if (m_pCSystem->m_bCollisionDetectionEnabled)
		{
			if (rigidBodyManager->ResolveCollisions())
			{
		//		m_pCSystem->m_rigidBodyManager->ComputeMomentumChanges();
		//		m_pCSystem->m_rigidBodyManager->SetPreviousStateOfBodies();
			}
		}
///////////////////////////////////////////////////////////////////////////////
		rigidBodyManager->SetTime(m_pCSystem->m__t);
		rigidBodyManager->WriteThoseTracking();
		m_pCSystem->DoDeferredCommands(m_pCSystem->m__t);
//		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//		pFrame->PostMessage(WM_USER_PASS_DONE, 0, 0);
		m_pCSystem->UpdateTree();
		m_pCSystem->DisplayData();
//		PostMessage(hwnd, WM_THREAD_STEP_DONE, 0,0);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//		m_bSingleStepping	= true;
//		m_bSingleStep_Mode	= true;
//		m_bRunning			= true;
///////////////////////////////////////////////////////////////////////////////
void CCruncher::OnStepDone(WPARAM wParam, LPARAM lParam)
{	
	ASSERT(m_pCSystem == theApp.GetSystem());
try {
	Wait();
#if 0
#endif
}
catch (wchar_t * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
		AfxMessageBox(L"CCruncher::OnStepDone");
}
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define RTOL  1e-5
#define ATOL  1e-5
const double CVodeRTOL = 1.0e-8;
const double CVodeFLOOR = .01;
const double CVodeATOL = CVodeRTOL*CVodeFLOOR;
///////////////////////////////////////////////////////////////////////////////
// CVodeStep
///////////////////////////////////////////////////////////////////////////////
long CCruncher::CVodeStep()
{

	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];	
//	double abstol=ATOL;
//	double reltol=RTOL;
	double abstol=CVodeATOL;
	double reltol=CVodeRTOL;
	double t=0, tout=0, dt = 0.075;
	long flag = CVODE::ERR_FAILURE;
	long cnt = 0;
///////////////////////////////////////////////////////////////////////////////
	CSystem::SolveMethod m_SolveMethod;
///////////////////////////////////////////////////////////////////////////////
	vector<double> state;//(rigidBodyManager->RigidLinkJointStates);
#if 0
try
#endif
{
//	if (m_NumberVariables == 0) throw SolveIt_ERROR(L"m_NumberVariables==0");
//tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(m_NumberVariables, NULL);
	if (x == 0) throw SolveIt_ERROR(L"CVODE::N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"x->data==0");
	x->length = m_NumberVariables;
{
	//Lock lock (theApp.protectBodies);
	//CSystem* m_system = theApp.GetSystem();
	state			= theApp.GetRigidBodyManager()->m_system_state;
	t				= m_pCSystem->m__t;
	tout			= m_pCSystem->GetTargetTime();
	m_SolveMethod	= m_pCSystem->m_SolveMethod;
}

	CopyMemory(x->data, &state.front(), m_NumberVariables * sizeof(double) );

	void *cvode_mem = CVODE::CVodeMalloc(x->length, CVodeDerivative, t, x, 
		CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) 0, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		if (m_SolveMethod == CSystem::direct)
			CVODE::CVDense(cvode_mem, NULL, NULL);
		else if (m_SolveMethod == CSystem::iterative)
			CVODE::CVSpgmr(cvode_mem, CVODE::NONE, CVODE::MODIFIED_GS, 0, 0.0, NULL, NULL, NULL);

		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else
	{
		CVODE::N_VFree(x);
		throw SolveIt_ERROR(L"CVODE::CVodeMalloc failed");
//		return (CVODE::N_VFree(x) , -1);
	}
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(&state.front(), x->data, m_NumberVariables * sizeof(double) );
		//Lock lock (theApp.protectBodies);
		//CSystem* m_system = theApp.GetSystem();
		m_pCSystem->m__t	= t;
	//	theApp.GetRigidBodyManager()->m_system_state	= state;
		if ( !((m_pCSystem->m_fStopTime>0.0) && (m_pCSystem->m_fStopTime <= m_pCSystem->m__t) ))
		theApp.GetRigidBodyManager()->SetStateOfBodies(state, FALSE);// SetPreviousStateOfBodies done at top of loop
//		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
//		CopyMemory(&rigidBodyManager->m_system_state.front(), x->data, m_NumberVariables * sizeof(double) );
	}

	CVODE::N_VFree(x);

	if (flag != CVODE::SUCCESS)
	{
		{
		//Lock lock (theApp.protectBodies);
		theApp.GetRigidBodyManager()->SetToPreviousState();
		}
		throw SolveIt_ERROR(CVOdeErrors[-flag]);
/*
		reltol *= 10;
		abstol *= 10;
		t		= m_pCSystem->m__t;
		tout	= 0.5*(t + tout);
		if (cnt++<4) goto tryAgain;
		else throw SolveIt_ERROR(CVOdeErrors[-flag]);
*/		
	}

}
#if 0
catch( SolveIt_Error& e){e.why();}catch(...)// there was an error
{
//	m_pCSystem->Stop();
	m_pCSystem->m_bStopIterations = true;
	rigidBodyManager->SetToPreviousState();
	throw CVOdeErrors[-flag];
}
#endif
	return flag;
}
///////////////////////////////////////////////////////////////////////////////
long CCruncher::StepMultibodyState()
{
	ASSERT(m_pCSystem == theApp.GetSystem());
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	long flag = CVODE::ERR_FAILURE;
	if (CRigidBody::m_LastLink==0) return -5;
	if (CRigidBody::m_baseJoint==0) return -4;
	double t=0, tout=0.02;

//	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();

	vector<double> state;//(rigidBodyManager->RigidLinkJointStates);
	{
		//Lock lock (theApp.protectBodies);
		//CSystem* m_system = theApp.GetSystem();
		t			= m_pCSystem->m__t;
		tout		= m_pCSystem->GetTargetTime();
		state=theApp.GetRigidBodyManager()->RigidLinkJointStates;
	}

	long N = static_cast<long>(state.size());
	CVODE::N_Vector x = CVODE::N_VNew(N, 0);
	if (x == 0) throw SolveIt_ERROR(L"CVODE::N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"x->data==0");
	x->length = N;
	CopyMemory(x->data, &state.front(), N * sizeof(double) );
	double abstol=1*ATOL, reltol=1*RTOL;
	void *cvode_mem = 
			CVODE::CVodeMalloc(	x->length, CVodeMultibodyDerivative, t, x, 
								CVODE::BDF, CVODE::NEWTON, CVODE::SS,
								&reltol, &abstol, (void*) 0, 0, FALSE, iopt, ropt, 0);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, 0, 0);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(&state.front(), x->data, N * sizeof(double) );
		//Lock lock (theApp.protectBodies);
		//CSystem* m_system = theApp.GetSystem();
		m_pCSystem->m__t	= t;
		theApp.GetRigidBodyManager()->SetMultibodyState(state);//&state.front()
	}
	CVODE::N_VFree(x);
	if (flag != CVODE::SUCCESS)
	{
		TimedMessageBox(0, GetWC(CVOdeErrors[-flag]), L"::StepMultibodyState", MB_OK, 1000);
		throw SolveIt_ERROR(CVOdeErrors[-flag]);
	}

	return flag;
}


#undef RTOL
#undef FLOOR
#undef ATOL
///////////////////////////////////////////////////////////////////////////////

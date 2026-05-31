// DataQM_Stationary2D.cpp: implementation of the CDataQM_Stationary2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "DataQM_Stationary2D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern "C" 
long				DATA_2D_mp_NEV=0;//# eigenvalues to seek
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataQM_Stationary2D::CDataQM_Stationary2D():
	m_nSeekEigenvalues(DATA_2D_mp_NEV),
	m_pCQMStationary2D(0),
//	eigenvalueIndex(0),
	energy_eigenvalue(0)
{

}

CDataQM_Stationary2D::~CDataQM_Stationary2D()
{

}

///////////////////////////////////////////////////////////////////////////////
void CDataQM_Stationary2D::CreateQMStationary2DThread(bool bCREATE_SUSPENDED)
{
//	m_bStopIterations	= false;
	if (m_pCQMStationary2D)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCQMStationary2D->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCQMStationary2D->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (m_pCQMStationary2D->m_hThread);
			m_pCQMStationary2D	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
m_pCQMStationary2D =  (CQMStationary2D*)AfxBeginThread( RUNTIME_CLASS( CQMStationary2D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		m_pCQMStationary2D =  (CQMStationary2D*)AfxBeginThread( RUNTIME_CLASS( CQMStationary2D ));
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
BOOL CDataQM_Stationary2D::StopQMStationary2D(void)
{
//	m_bStopIterations = true;
	if (m_pCQMStationary2D == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQMStationary2D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCQMStationary2D->ResumeThread();
		m_pCQMStationary2D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCQMStationary2D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	if(m_pCQMStationary2D&&m_pCQMStationary2D->m_hThread)CloseHandle (m_pCQMStationary2D->m_hThread);
	m_pCQMStationary2D	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////

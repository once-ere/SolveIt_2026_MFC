// DataQM_Stationary1D.cpp: implementation of the CDataQM_Stationary1D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "DataQM_Stationary1D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
CDataQM_Stationary1D::CDataQM_Stationary1D():
	m_pCQMStationary1D(0),
	eigenvalueIndex(0),
	M(8+1./3.),
	energy_eigenvalue(0)
{
//	CreateQMStationary1DThread(true);
}
///////////////////////////////////////////////////////////////////////////////
CDataQM_Stationary1D::~CDataQM_Stationary1D()
{
	StopQMStationary1D();
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Stationary1D::CreateQMStationary1DThread(bool bCREATE_SUSPENDED)
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
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CDataQM_Stationary1D::StopQMStationary1D(void)
{
//	m_bStopIterations = true;
	if (m_pCQMStationary1D == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQMStationary1D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCQMStationary1D->ResumeThread();
		m_pCQMStationary1D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCQMStationary1D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	if(m_pCQMStationary1D&&m_pCQMStationary1D->m_hThread)CloseHandle (m_pCQMStationary1D->m_hThread);
	m_pCQMStationary1D	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////

// QM_Scatt1D.cpp : implementation file
//

#include "stdafx.h"
#include "..\SolveIt.h"
#include "QM_1DScatt.h"
#include "Cruncher.h"
#include "System.h"
#include "GLView.h"
#include "DataQM_Scatt1D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//const long _DATA_1D_mp_NDATA = 500;//500;//64
/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt1D

IMPLEMENT_DYNCREATE(CQM_Scatt1D, CWinThread)

CQM_Scatt1D::CQM_Scatt1D():
//	QMEvolve1D(_DATA_1D_mp_NDATA),
//	m_nWidth(theApp.GetGLView()->viewWidth),
//	m_nHeight(theApp.GetGLView()->viewHeight),
//	m_fLambda(QMEvolve1D.Lambda),
//	m_x0(QMEvolve1D.InitialCenter_x),
//	m_fWaveNumber(QMEvolve1D.WaveNumber_x),
//	m_fInitialSpread(QMEvolve1D.Delta_x),
	m_lTakeNumSteps(theApp.GetSystem()->m_lTakeNumSteps),
	m_lNumStepsTaken(0),

//	m_nNumNodes(QMEvolve1D.NDATA),

//	m_nRenormalize(QMEvolve1D.RENORMALIZE),

//	DATA_PASS_mp_PU(QMEvolve1D.u),
//	DATA_PASS_mp_PPSI(QMEvolve1D.psi_xt),
//	DATA_PASS_mp_PPSI0(QMEvolve1D.psi_xt0),
	m_bStepCompleted(true) 
{
}

CQM_Scatt1D::~CQM_Scatt1D()
{
}

BOOL CQM_Scatt1D::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CQM_Scatt1D::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CQM_Scatt1D, CWinThread)
	//{{AFX_MSG_MAP(CQM_Scatt1D)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_STEP, OnSingleStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt1D message handlers
///////////////////////////////////////////////////////////////////////////////
//	CSystem* m_system = theApp.GetSystem();
//wchar_t* hData = m_system->player->SendMessage(WM_USER_SetParam, 'f', 0);
////GlobalFree(hData);
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt1D::OnSetParam(WPARAM wParam, LPARAM lParam)
{
	return ;
	try
	{
		switch (int(wParam))
	{
		case 'f':
			break;
		case 's':
			break;
		case 'u':
			break;
		case 'd':
			break;
	}
/*
		wchar_t buf[64];
		std::wstringstream oss(buf, sizeof buf);//ostrstream
		oss << 23.6  << ends;
		int cb = _tcslen(buf) + 1;
		HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
		std::wstring    lpszNew = (std::wstring )GlobalLock(hgCF_TEXTData);
		if (_tcslen(buf)) _tcscpy(lpszNew, buf);
		GlobalUnlock(hgCF_TEXTData);
		return (long)lpszNew; 
*/
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(exc->what()), MB_OK);
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"OnGetParam", MB_OK);
	}
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt1D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{ 
	m_bStepCompleted=true; 
//	Fire_ViewChange(); 
	bool bSingleStep=false;
	bool bKill=false;
	{
		Lock lock (theApp.protectBodies);
		CSystem* m_system = theApp.GetSystem();
		bSingleStep = m_system->m_bSingleStepping;
		if ((m_system->m_fStopTime>0.0) && (m_system->m_fStopTime <= m_system->m__t) )
		{
			m_system->m_bStopIterations = bKill = true;
		}
	}
	++m_lNumStepsTaken;
	if (bKill)
	{
		PostQuitMessage(0);
	}
	else
	if ( (bSingleStep && (m_lNumStepsTaken < m_lTakeNumSteps) ) || (!bSingleStep) )
	{
		PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	{
		m_lNumStepsTaken = 0;
	}
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt1D::OnStep(WPARAM wParam, LPARAM lParam) 
{ 
//	m_bStepCompleted=false; 
//	MakeTimeStep(); 
	OnSingleStep( wParam, lParam);
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt1D::OnSingleStep(WPARAM wParam, LPARAM lParam)
{
try
{
	m_bStepCompleted=false; 
	MakeTimeStep(); 
}
catch (wchar_t * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
		AfxMessageBox(L"CQM_Scatt1D::OnSingleStep");
}
}
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt1D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0); 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CQM_Scatt1D::MakeTimeStep(void)
{
	CDataQM_Scatt1D* that = reinterpret_cast<CDataQM_Scatt1D*>((BYTE*)this - offsetof(CDataQM_Scatt1D, m_pCQM_Scatt1D));
	CSystem* m_system	= theApp.GetSystem();
	try
	{
	//	that->Step();
		m_system->data__QM_Scatt1D->Step();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox(L"ERROR: ::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////


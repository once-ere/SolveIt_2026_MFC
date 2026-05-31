// QM_Scatt2D.cpp : implementation file
//

#include "stdafx.h"
#include "..\SolveIt.h"
#include "QM_2DScatt.h"
#include "Cruncher.h"
#include "System.h"
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt2D

IMPLEMENT_DYNCREATE(CQM_Scatt2D, CWinThread)

CQM_Scatt2D::CQM_Scatt2D():
	m_bStepCompleted(true) 
{
}

CQM_Scatt2D::~CQM_Scatt2D()
{
}

BOOL CQM_Scatt2D::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CQM_Scatt2D::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CQM_Scatt2D, CWinThread)
	//{{AFX_MSG_MAP(CQM_Scatt2D)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt2D message handlers
///////////////////////////////////////////////////////////////////////////////
//	CSystem* m_system = theApp.GetSystem();
//wchar_t* hData = m_system->player->SendMessage(WM_USER_SetParam, 'f', 0);
////GlobalFree(hData);
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt2D::OnSetParam(WPARAM wParam, LPARAM lParam)
{
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
void CQM_Scatt2D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{ 
	m_bStepCompleted=true; 
	Fire_ViewChange(); 
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt2D::OnStep(WPARAM wParam, LPARAM lParam) 
{ 
	m_bStepCompleted=false; 
	MakeTimeStep(); 
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQM_Scatt2D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0); 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CQM_Scatt2D::MakeTimeStep(void)
{
	CSystem* m_system	= theApp.GetSystem();
	try
	{
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


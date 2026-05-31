// QMStationary2D.cpp: implementation of the CQMStationary2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "QMStationary2D.h"
//#include "..\ScannerParser\Parser.h"
//#include "Help/HtmlCtrl.h"
//#include "Help/TraceWin.h"
//#include "GL_Thread.h"
#include "GLView.h"
//#include "SMutex.h"
#include "instruction.h"
#include "StackMachine.h"
#include "MainFrm.h"
#include "DataQM_Stationary2D.h"
#include "Cruncher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern SMutex g_SMutex_1;
///////////////////////////////////////////////////////////////////////////////
extern "C" void _stdcall	DATA_2D_mp_INIT();
extern "C" void _stdcall	DATA_2D_mp_TERM();
extern "C"  double			DATA_2D_mp_LAMBDA;
extern "C" long				DATA_2D_mp_PPOTENTIAL;
extern "C" long				DATA_2D_mp_PPSI__XY;
extern "C" long				DATA_2D_mp_PWRK;
extern "C" long				DATA_2D_mp_PEIGENVALUES;
extern "C" long				DATA_2D_mp_PIPARAM;
extern "C" long				DATA_2D_mp_NEV;//# eigenvalues to seek

	const long maxncv = 25;
//	typedef long (*MATRIX_2)[2];
//	typedef double DataXY[_N2*_N2];
//	typedef double (**MATRIX_2)[2];
//	typedef double (*MATRIX_2x[2])[maxncv];

//	typedef double (MATRIX_2xy[2])[maxncv];
//	typedef double (MATRIX_2xy[2])[maxncv];
	typedef double (MATRIX_2xy)[2][maxncv];
//	typedef double (MATRIX_2xy)[maxncv][2];

//extern "C" long				DATA_2D_mp_PVEC;
extern "C" MATRIX_2xy				DATA_2D_mp_PVEC;

//double CQMStationary2D::psi_xy[2*max_Num*_N2aN2a];
//double **CQMStationary2D::psi_x				= 0;
//GLuint *CQMStationary2D::listEigenStates	= 0;
//double *CQMStationary2D::eigenvalues		= 0;
//double *CQMStationary2D::eigenvalueErrors	= 0;
///////////////////////////////////////////////////////////////////////////////
// Handle "app:ok" link by closing dialog
//
void CQMStationary2DHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	if (_tcsicmp(lpszWhere,_T("ok"))==0) {
		GetParent()->SendMessage(WM_COMMAND,IDOK);
	}
}

IMPLEMENT_DYNAMIC(CQMStationary2DDialog, CDialog)
BEGIN_MESSAGE_MAP(CQMStationary2DDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////
#include <Mshtml.h>
void CQMStationary2DDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if ( 0 >= cx || 0 >= cy ){
		return;
	}
	if (!::IsWindow(m_page.m_hWnd)) return;
	::MoveWindow(m_page.m_hWnd, 0, 0, cx, cy, TRUE);
	m_page.SendMessage(WM_SIZE,SIZE_RESTORED, MAKELONG(cx, cy));
}

BOOL CQMStationary2DDialog::OnInitDialog()
{

	int ret = -2;
//	CenterWindow(CWnd::GetDesktopWindow());
	RECT Rect, deskRect;
	HWND desk_hwnd = ::GetDesktopWindow();
	::GetWindowRect(desk_hwnd, &deskRect);
	::GetWindowRect(m_hWnd, &Rect);
	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;
	int y = 0; 
	int x = 0;
	::MoveWindow(m_hWnd, x, y, w, h, TRUE);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);//HWND_TOP
	VERIFY(CDialog::OnInitDialog());
	VERIFY(m_page.CreateFromStatic(IDC_HTMLVIEW, this));

	TCHAR buf[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, buf);
	_bstr_t bbuf(buf);
	bbuf += L"\\QMStationary2D.htm";
	m_page.Navigate(bbuf, 0, 0, 0);

	return TRUE;
}
//	CInfoDialog* pCVerhulstDialog = new CInfoDialog(0, 0, _T("QMStationary2D.htm"));
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2DDialog::OnCancel()
{
	DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2DDialog::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////

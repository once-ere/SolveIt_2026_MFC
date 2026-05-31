// TextBoxWindow.cpp : implementation file
//

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"
#include "TextBoxWindow.h"
#include "SolveItDoc.h"
#include "SolveItView.h"
#include <afxpriv.h>//AfxHookWindowCreate
#include "Cruncher.h"
#include "MainFrm.h"
#include "System.h"
#include "QM/QMStationary1D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextBoxWindow
CTextBoxWindow::CTextBoxWindow(CSolveItView* pV, int y, int cy, HWND hWndPrompt):
	m_pCSolveItView(pV),
	m_hWndPrompt(hWndPrompt),
	m_x(0),
	m_y(y),
	m_cx(m_pCSolveItView->m_Width),
	m_cy(cy),
	m_Height(cy),
	nCrLf(1),
	m_bSimulateShift(0),
	m_bSimulateReturn(0),
	m_bLockTop(false)
{
	ASSERT(m_Height == nCrLf*m_cy);
	CRect rc;
	
	
	m_pCSolveItView->GetClientRect(&rc);
	m_cx = rc.Width();

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->GetClientRect(&rc);
	m_cx = rc.Width();

	RECT deskRect={0,0,0,0};
	HWND desk_hwnd		= ::GetDesktopWindow();
	::GetClientRect(desk_hwnd, &deskRect);
	m_cx = deskRect.right;

	::GetClientRect(m_hWndPrompt, &m_rcWndPrompt);
	m_rcWndPrompt.top		= m_y;
	m_rcWndPrompt.bottom	= m_y + m_Height;
	m_cx	-= m_rcWndPrompt.right;
	RECT r ={m_rcWndPrompt.right, m_y, m_cx, m_y+m_Height};
	m_rc = r;
//	::MoveWindow(m_hWndPrompt, 0, m_y, m_rcWndPrompt.right, m_Height, TRUE);
//	MoveWindow(m_rc, TRUE);
}
///////////////////////////////////////////////////////////////////////////////
CTextBoxWindow::CTextBoxWindow(CSolveItView* pV):
#pragma warning(push)
#pragma warning(disable:4244)
	m_pCSolveItView(pV),
	m_hWndPrompt(0),
	m_cx(m_pCSolveItView->m_Width),
	m_x(0),
	m_y(0),
	m_cy(0),
	m_bSimulateShift(0),
	m_bSimulateReturn(0),
	m_bLockTop(false)
{
	_variant_t v((long)m_pCSolveItView->m_entryNumber);
	_bstr_t bstr(v);
	bstr += L"<   ";
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_pCSolveItView->SendMessage(WM_GETFONT, 0, 0);
	//	CClientDC dc(NULL);
	//	if (hFont != NULL) hOldFont = dc.SelectObject(hFont);
//		size = dc.GetTextExtent("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//		size = dc.GetTextExtent("1234>");
//		BOOL res = GetTextExtentPoint32( hdc, "1234>", _tcslen("1234>"), &size );
		HDC hdc = GetDC()->GetSafeHdc();
		if (hFont != NULL) hOldFont = (HFONT)SelectObject(hdc, hFont);
		BOOL res = GetTextExtentPoint32( hdc, bstr, bstr.length(), &size );
		if (hOldFont != NULL) SelectObject(hdc, hOldFont);
		::ReleaseDC(GetSafeHwnd(),hdc);
	//	if (hOldFont != NULL) dc.SelectObject(hOldFont);
	}
	m_cy	= size.cy;
	CRect rc;
	m_pCSolveItView->GetClientRect(&rc);
	m_cx = rc.Width();
	int x = 0;
	int y = 0;
	int cx = size.cx;
	DWORD dwStyle=0, dwExStyle=0;
	HWND hWnd = m_pCSolveItView->m_hWnd;

	dwStyle=WS_CHILD|WS_VISIBLE|SS_LEFT;//SS_SIMPLE|SS_WHITERECT;_T("")
	m_hWndPrompt = CreateWindowEx(dwExStyle, _T("STATIC"), bstr, dwStyle,x,y,cx,m_cy, hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(m_hWndPrompt != NULL);
	ASSERT(::IsWindow(m_hWndPrompt));

	dwExStyle	= WS_EX_TRANSPARENT|WS_EX_CONTROLPARENT;
	dwStyle		= WS_CHILD|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|ES_AUTOHSCROLL;//|WS_BORDER;
	x	= cx;
	cx = m_cx - size.cx;

	HWND hWndEDIT = CreateWindowEx(0,_T("EDIT"),_T(""),dwStyle,x,y,cx,m_cy,hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(hWndEDIT != NULL);
	ASSERT(::IsWindow(hWndEDIT));
	LONG_PTR lStyle = ::GetWindowLongPtr(hWndEDIT, GWL_STYLE);
	lStyle |= ES_MULTILINE|ES_WANTRETURN;
	::SetWindowLongPtr(hWndEDIT, GWL_STYLE, lStyle);

	if (!SubclassWindow(hWndEDIT))
	{
		ASSERT(FALSE);		// possibly trying to subclass twice?
		AfxThrowNotSupportedException();
	}
}



CTextBoxWindow::~CTextBoxWindow()
{
}
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTextBoxWindow, CEdit)
	//{{AFX_MSG_MAP(CTextBoxWindow)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_COPY,	OnPopupCopy)
	ON_COMMAND(ID_POPUP_CUT,	OnPopupCut)
	ON_COMMAND(ID_POPUP_CLEAR,	OnPopupDelete)
	ON_COMMAND(ID_POPUP_PASTE,	OnPopupPaste)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY,		OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CUT,		OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLEAR,	OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PASTE,	OnUpdateNeedClip)
ON_REGISTERED_MESSAGE(WM_USER_COMMAND, OnUserCommand)
END_MESSAGE_MAP()

//_THREAD

///////////////////////////////////////////////////////////////////////////////
// CTextBoxWindow message handlers
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnDestroy() 
{
	try {
	DestroyWindow();

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnDestroy");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::PostNcDestroy()
{
	try {
//	m_pCSolveItView->m_pCTextBoxWindow = 0;
	delete this;

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::PostNcDestroy");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::Resize(int nBias)//int numRows) 
{
	try {
	LONG_PTR duh = ::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (duh==0 && nBias!=0) return;
	if (!m_hWnd || !::IsWindow(m_hWnd)) return;//
	CString text;
	GetWindowText(text);
	int length = text.GetLength();
	if (length <= 0) length=1;

	const wchar_t nl			= L'\n';
	int nCrLfNew			= 1;
	int j					= 0;
//	bool char_on_next_line	= false;//true;
	for (nCrLfNew=nBias, j=0; j<length; ++j)
	{
		if (text[j]==nl)
		{
			++nCrLfNew;
		}
	}

	long nCrLfChange	= nCrLfNew-nCrLf;
	if (nCrLfChange == 0) return;

	nCrLf		= nCrLfNew;
	m_Height	= nCrLf*m_cy;


	::GetClientRect(m_hWndPrompt, &m_rcWndPrompt);
	m_rcWndPrompt.top		= m_y;
	m_rcWndPrompt.bottom	= m_y + m_Height;
	RECT rc={m_rcWndPrompt.right, m_y, m_cx, m_y+m_Height};
	m_rc=rc;

	m_bLockTop = true;
	m_pCSolveItView->vec_pTextBoxWindow.MoveVertically(this, nCrLfChange, m_cy);
	m_bLockTop = false;

//	::MoveWindow(m_hWndPrompt, 0, m_y, m_rcWndPrompt.right, m_Height, TRUE);
//	MoveWindow(m_rc, TRUE);
//	m_pCSolveItView->SendMessage( WM_USER_TextBoxWindow, 0, 0);

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::Resize");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const char* CTextBoxWindow::GetTextBoxText()
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//auto_ptr<char> CTextBoxWindow::GetTextBoxStrings()
char* CTextBoxWindow::GetTextBoxStrings()
{
	try {
	if (!m_hWnd || !::IsWindow(m_hWnd)) throw SolveIt_ERROR("CTextBoxWindow::GetTextBoxStrings: null");//return 0;
	CString text;
	GetWindowText(text);
	const int length = text.GetLength();
//	if (length <= 0) throw SolveIt_ERROR("null");
	if (length <= 0) return 0;

	CStringA textA(text);
	char* buf = new char[textA.GetLength()+1];
	strcpy_s(buf, textA.GetLength()+1, textA.GetString());
	return buf;

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::GetTextBoxStrings");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	try {
	LONG_PTR duh = ::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (duh==0) return;

	HWND hWnd = ::GetFocus();
	if (m_hWnd != hWnd)
	{
		return;
	}
	const MSG& aMSG = *GetCurrentMessage();
	if (m_hWnd != aMSG.hwnd)
	{
		return;
	}
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;
	BOOL bRetn	= GetAsyncKeyState(VK_RETURN)	& 0x8000;
	BOOL bDele1	= GetAsyncKeyState(VK_DELETE)	& 0x8000;
	BOOL bDele2	= GetAsyncKeyState(VK_BACK)		& 0x8000;
//	BOOL bDele	= (bDele1 || bDele2) && (char('\n')==char(aMSG.wParam));
	if (!bShift)
	{
		bShift	= m_bSimulateShift;
//		if (m_bSimulateShift) AfxMessageBox("s");
	}
	m_bSimulateShift	= 0;
	if (!bRetn)
	{
		bRetn	= m_bSimulateReturn;
//		if (m_bSimulateReturn) AfxMessageBox("r");
	}
//	m_bSimulateReturn	= 0;

	POINTL pt;
	pt.x = aMSG.pt.x;
	pt.y = aMSG.pt.y;
//	if ((aMSG.lParam==MAKELONG(1, 28)) && (char(VK_RETURN)==char(aMSG.wParam)))
//	{
//		long g = WM_CHAR;
//		long l = aMSG.message;
//	}
///////////////////////////////////////////////////////////////////////////////
	if ((bDele1 || bDele2) && (!m_bSimulateReturn))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		Resize();
		return;
		WPARAM wParam = 0; // not used 
		POINT pt;
		pt.x = aMSG.pt.x;
		pt.y = aMSG.pt.y;
	//	m_pV->ScreenToClient(&pt);
	//	pt.x = aMSG.pt.x;
	//	pt.y = aMSG.pt.y;
		ScreenToClient(&pt);
	//	pt.x = aMSG.pt.x;
	//	pt.y = aMSG.pt.y;
	//	::ScreenToClient(m_hWnd, &pt);
	//	LPARAM lParam = (LPARAM) (POINTL *) &pt;//Rich edit controls
		LPARAM lParam = MAKELPARAM(pt.x, pt.y);	//Edit controls
		LRESULT result = SendMessage(EM_CHARFROMPOS, wParam, lParam); 
		if (result == -1) return;

		WORD zero_based_index_of_character_nearest_point = LOWORD(result);//Edit controls
		WORD zero_based_index_of_line	 = HIWORD(result);//Edit controls

		HLOCAL h = (HLOCAL) SendMessage(EM_GETHANDLE, 0, 0);
		char* buf = (char*)LocalLock(h);
		char ch = buf[zero_based_index_of_character_nearest_point];
		LocalUnlock(h);

		if ( char('\r')==ch )
			DecrementSize();
		if ( char('\n')==ch )
			DecrementSize();
		return;
	}
	m_bSimulateReturn	= 0;
	if (bRetn && bShift)
	{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//LPSTR dupString(LPCSTR lpsz); //Action::DupString
//requires a call to	GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
//LPSTR DupString(LPCSTR lpsz); //_tcsdup; memory leak
///////////////////////////////////////////////////////////////////////////////
		try{
//		auto_ptr<char>	abuf	= GetTextBoxStrings();
		char*			buf		= GetTextBoxStrings();//abuf.get();

	//	if (buf == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnChar: null");
		if (buf == NULL) return;


//		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//		pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) dupString(buf));
		const int		ch		= ';';
		char* 			pdest	= strrchr( buf, ch );
		if (pdest == NULL )
		{
			delete[] buf;
			return;
		}
		m_pCSolveItView->OnCompile2(buf);
		delete[] buf;
		//SendMessage(WM_CHAR,aMSG.wParam,aMSG.lParam);
		return;
		}catch( SolveIt_Error& e){e.why();}catch(...){return;}
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	if (bRetn)
	{
		IncrementSize();
//		Resize();
	}
//	CEdit::OnChar(nChar, nRepCnt, nFlags);

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnChar");
	}
}
#pragma warning(pop)
///////////////////////////////////////////////////////////////////////////////
BOOL CTextBoxWindow::PreTranslateMessage(MSG* pMsg) 
{
	try {
	if (pMsg->message == WM_COMMAND)
	{

		switch (pMsg->wParam)
		{
			case ID_EDIT_COPY:
			case ID_EDIT_CUT:
			case ID_EDIT_PASTE:
				SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	if (pMsg->message < WM_KEYFIRST) return CEdit::PreTranslateMessage(pMsg);
	if (pMsg->message > WM_KEYLAST ) return CEdit::PreTranslateMessage(pMsg);
	switch (pMsg->message)
	{
		case WM_VSCROLL:
		case WM_HSCROLL:
			m_pCSolveItView->SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
//return 1;
		break;
	//	case WM_KEYUP:
		case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
			case VK_DELETE:
			BOOL res = CEdit::PreTranslateMessage(pMsg);
			Resize(0);
			return res;
#if 0
				SendMessage( WM_CHAR, VK_DELETE, MAKELONG(1, VK_DELETE));
		//		SendMessage( WM_CHAR, VK_DELETE, MAKELONG(1, 14));
		//		SendMessage( WM_CHAR, VK_DELETE, MAKELONG(1, 28));
		//		SendMessage( WM_CHAR, VK_DELETE, 0);
			return 0;//message was not translated and should be dispatched
			return 1;//message was translated and should not be dispatched
{
	//	//	SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
	const MSG& aMSG = *GetCurrentMessage();
		WPARAM wParam = 0; // not used 
		POINT pt;
		pt.x = aMSG.pt.x;
		pt.y = aMSG.pt.y;
		ScreenToClient(&pt);
		LPARAM lParam = MAKELPARAM(pt.x, pt.y);	//Edit controls
		LRESULT result = SendMessage(EM_CHARFROMPOS, wParam, lParam); 
		if (result == -1) return TRUE;
		WORD zero_based_index_of_character_nearest_point = LOWORD(result);//Edit controls
		WORD zero_based_index_of_line	 = HIWORD(result);//Edit controls
		HLOCAL h = (HLOCAL) SendMessage(EM_GETHANDLE, 0, 0);
		char* buf = (char*)LocalLock(h);
		char ch = buf[zero_based_index_of_character_nearest_point];
		LocalUnlock(h);
}
#endif
#if 0
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_HOME:
			case VK_END:
#endif
		}
		break;
	}

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::PreTranslateMessage");
	}
	return CEdit::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CTextBoxWindow::GetBottom()
{
	return m_y + m_Height;
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::MoveVertically(int numRows)
{
	try {
	if (m_bLockTop) return;
	int y		= numRows*m_cy;
	m_y		+= y;

	m_rc.top	+= y;
	m_rc.bottom	+= y;

	m_rcWndPrompt.top	+= y;
	m_rcWndPrompt.bottom+= y;

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::MoveVertically");
	}
}
///////////////////////////////////////////////////////////////////////////////
// CTextBoxWindow message handlers
void CTextBoxWindow::IncrementSize()
{
	try {
	LONG_PTR duh = ::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (duh==0) return;
	if (!m_hWnd || !::IsWindow(m_hWnd)) return;
	++nCrLf;
	m_Height	= nCrLf*m_cy;

	::GetClientRect(m_hWndPrompt, &m_rcWndPrompt);
	m_rcWndPrompt.top		= m_y;
	m_rcWndPrompt.bottom	= m_y + m_Height;
	RECT rc={m_rcWndPrompt.right, m_y, m_cx, m_y+m_Height};
	m_rc=rc;
	m_pCSolveItView->vec_pTextBoxWindow.MoveVertically(this, 1, m_cy);

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::IncrementSize");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::DecrementSize()
{
	try {
	LONG_PTR duh = ::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (duh==0) return;
	if (!m_hWnd || !::IsWindow(m_hWnd)) return;
	--nCrLf;
	m_Height	= nCrLf*m_cy;

	::GetClientRect(m_hWndPrompt, &m_rcWndPrompt);
	m_rcWndPrompt.top		= m_y;
	m_rcWndPrompt.bottom	= m_y + m_Height;
	RECT rc={m_rcWndPrompt.right, m_y, m_cx, m_y+m_Height};
	m_rc=rc;
	m_pCSolveItView->vec_pTextBoxWindow.MoveVertically(this, -1, m_cy);

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::DecrementSize");
	}
}
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnContextMenu(CWnd* pWnd, CPoint ptMousePos) 
{
try {
//	ASSERT(m_system != NULL);

	ASSERT(pWnd->m_hWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));
	if (pWnd->m_hWnd == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: pWnd->m_hWnd == 0");
	if (!::IsWindow(pWnd->m_hWnd)) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: !::IsWindow(pWnd->m_hWnd)");

//	CEdit::OnContextMenu(pWnd, ptMousePos);
//    AppendMenu(hmenuMain, MF_STRING | MF_POPUP, (UINT) hmenuChart, "Chart");
	
	CSystem* m_pCSystem	= theApp.GetSystem();
	if (m_pCSystem!=NULL && m_pCSystem->m_pCQMStationary1D!=NULL)
	{
	//	m_pCSystem->StopQMStationary1D();
		m_pCSystem->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, WM_CLOSE);
	}

//	::ClientToScreen(pWnd->m_hWnd, &ptMousePos);
	HMENU hmenu;            // top-level menu 
	HMENU hmenuTrackPopup;  // shortcut menu 
// Load the menu resource. 
	if ((hmenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_EXAMPLES_MENU))) == NULL) return; 
	ASSERT( ::IsMenu(hmenu) );
	if (hmenu == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: hmenu == 0");
	if (!::IsMenu(hmenu)) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: !::IsMenu(hmenu)");

// TrackPopupMenu cannot display the menu bar so get a handle to the first shortcut menu. 
	hmenuTrackPopup = GetSubMenu(hmenu, 0);
	ASSERT( ::IsMenu(hmenuTrackPopup) );
	if (hmenuTrackPopup == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: hmenuTrackPopup == 0");
	if (!::IsMenu(hmenuTrackPopup)) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: !::IsMenu(hmenuTrackPopup)");

	if (!OnUpdateNeedSel())
	{
		EnableMenuItem(hmenuTrackPopup, 0, MF_BYPOSITION | MF_GRAYED);
		EnableMenuItem(hmenuTrackPopup, 1, MF_BYPOSITION | MF_GRAYED);
		EnableMenuItem(hmenuTrackPopup, 4, MF_BYPOSITION | MF_GRAYED);
	}

	if (IsClipboardFormatAvailable(CF_TEXT) == 0)
	{
		EnableMenuItem(hmenuTrackPopup, 2, MF_BYPOSITION | MF_GRAYED);
	}
///////////////////////////////////////////////////////////////////////////////
//	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->SendMessage(WM_USER_PASS_DONE, 0, 0);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	{
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		if (pFrame == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: pFrame == 0");
		if (pFrame->m_system == NULL) throw SolveIt_ERROR("CTextBoxWindow::OnContextMenu: pFrame->m_system == 0");
//		pFrame->PostMessage(WM_USER_Initialize, 0, 0);//
		pFrame->SendMessage(WM_USER_KillWorker, 0, 0);//
//		pFrame->m_system->KillWorker();
#if 0
		DWORD ExitCode=0;
		bool bEnable = false;
		if (pFrame->m_system->worker == NULL ) bEnable = true;
		else
		{
			BOOL ret = GetExitCodeThread(pFrame->m_system->worker->m_hThread, &ExitCode);
			if (ret == 0)
			{
				DWORD err = GetLastError();
				_variant_t v = long(err);
				_bstr_t b(v);
				throw SolveIt_ERROR((const char *) b);
			}
			if (ExitCode != STILL_ACTIVE) bEnable = true;
		}
		bEnable = !( (ExitCode == STILL_ACTIVE) || (pFrame->m_system->m_bStopIterations==true || pFrame->m_system->m_bSingleStep==true ));
#endif

		if (pFrame->m_system->m_bRunning)//m_bStopIterations==false || pFrame->m_system->m_bSingleStepping)
		{
	//		if (pFrame->m_system->worker && (pFrame->m_system->worker->m_lNumStepsTaken >= pFrame->m_system->worker->m_lTakeNumSteps))
	//		{
	//		}
	//		else
			{
				for ( int j = 6; j<12; ++j) EnableMenuItem(hmenuTrackPopup, j, MF_BYPOSITION | MF_GRAYED);
			}
		}
	}

// Display the shortcut menu. Track the right mouse button. 
	TrackPopupMenu(	hmenuTrackPopup, 
					TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
					ptMousePos.x, ptMousePos.y, 0, pWnd->m_hWnd, NULL); 
 
// Destroy the menu. 
	DestroyMenu(hmenu); 



	Resize();

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnContextMenu");
	}
}
/////////////////////////////////////////////////////////////////////////////
//	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
///////////////////////////////////////////////////////////////////////////////
bool CTextBoxWindow::OnUpdateNeedSel()
{
	try {
	long starting_position	= 0;
	long ending_position	= 0;
	SendMessage(EM_GETSEL, reinterpret_cast<WPARAM> (&starting_position), reinterpret_cast<LPARAM> (&ending_position) );
	return( starting_position != ending_position );

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnUpdateNeedSel");
	}
	return false;
} 
///////////////////////////////////////////////////////////////////////////////
//HOW BIZARRE CAN WINDOWS GET?
//
LRESULT CTextBoxWindow::OnUserCommand(WPARAM wParam, LPARAM lParam)
{
	try {
	_bstr_t bstr(reinterpret_cast<const char*> (lParam));
	WriteLine(bstr, 1);
	Resize(0);

}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnUserCommand");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//HOW BIZARRE CAN WINDOWS GET?
//
int CTextBoxWindow::WriteLine(BSTR strText, long lWriteReturn)
{
	try {
	_bstr_t bstr(strText);
	SetFocus();
	SetWindowText(bstr);

	if (lWriteReturn)
	{
		m_bSimulateShift=1;
		m_bSimulateReturn=1;
		SendMessage(WM_CHAR, 10, MAKELONG(1, 28));
	}
	else
	{
	//	SendMessage(WM_CHAR, VK_RETURN, 0);
	//	SendMessage(WM_CHAR, 10, 0);
	}
#if 0
//A caret is a blinking line, block, or bitmap in the client area of a window.
//The caret typically indicates the place at which text or graphics will be inserted. 
//A cursor is a small picture whose location on the screen is controlled 
//by a pointing device, such as a mouse, pen, or trackball. 

//Move to bottom of text window:
	SetFocus();
	SendMessage(EM_SETSEL, 0, -1);//If the start is 0 and the end is -1, all the text in the edit control is selected. 
	long starting_position	= 0;
	long ending_position	= 0;
	SendMessage(EM_GETSEL, reinterpret_cast<WPARAM> (&starting_position), reinterpret_cast<LPARAM> (&ending_position) );

//long res = 
//	SendMessage(EM_POSFROMCHAR, ending_position,0);
//	POINT pt;
//	pt.x = LOWORD(res);
//	pt.y = HIWORD(res);
/*
Parameters
wParam 
Rich Edit 1.0 and 3.0: Pointer to a POINTL structure that receives the client area coordinates of the character. The coordinates are in screen units and are relative to the upper-left corner of the control's client area. 
Edit controls and Rich Edit 2.0: Specifies the zero-based index of the character. 

lParam 
Rich Edit 1.0 and 3.0: Specifies the zero-based index of the character. 
Edit controls and Rich Edit 2.0: This parameter is not used. 

Return Values
Rich Edit 1.0 and 3.0: The return value is not used.

Edit controls and Rich Edit 2.0: The return value contains the client area coordinates of the character. The low-order word contains the horizontal coordinate and the high-order word contains the vertical coordinate. 

*/
	if (bstr.length() > 0)
	{
		SendMessage(EM_REPLACESEL, 0, reinterpret_cast<LPARAM> ((const char*)bstr));
	}
// CTRL+VK_RETURN
//MAKELONG(1, 28) == 1835009
	LRESULT res = SendMessage(EM_POSFROMCHAR,
				(WPARAM) 0,     // coordinates or index
				(LPARAM) 0      // index or not used
	);

	CPoint pt(GET_X_LPARAM(res), GET_Y_LPARAM(res));//GetCharPos( ending_position );
	SetCaretPos(pt);
	::SetCaretPos(pt.x,pt.y);
//	SetCursorPos(pt.x,pt.y);

	SendMessage(EM_SETSEL, -1, -1);

	SendMessage(EM_SCROLLCARET, 0, 0);
	Resize(0);

#endif


}
	catch (char * str)
	{
		AfxMessageBox(CString(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::WriteLine");
	}
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////

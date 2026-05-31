// NumerovView.cpp : implementation of the CSolveItView class
//

#include "stdafx.h"
#include "SolveIt.h"
#include "TextBoxWindow.h"
#include "MainFrm.h"
#include <afxpriv.h>//AfxHookWindowCreate
#include "SolveItDoc.h"
#include "SolveItView.h"
#include "System.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////
// vecCEdit
void vecCEdit::MoveVertically(CTextBoxWindow* p, int numRows, int cy)
{
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSolveItView* pView = pFrame->GetEditView();
	if (pView != NULL && pView->m_hWnd && ::IsWindow(pView->m_hWnd) )
	{
		vector<CTextBoxWindow*>::iterator j = find(begin(), end(), p);
		if (j != end())
		{
	ASSERT ((*j) == p );//
			for (++j; j != end(); ++j)
			{
	ASSERT ((*j) != p );//
				(*j)->MoveVertically(numRows);
			}
		}
	}
	pView->SizeUpdate(numRows*cy);
//	pView->Invalidate();//m_hWndPrompt not always redrawn unless this is called
	pView->vec_pTextBoxWindow.back()->SetFocus();
}
///////////////////////////////////////////////////////////////////////////////
int vecCEdit::GetBottom()
{
	if (0==size()) return 0;
	return back()->GetBottom();
}
///////////////////////////////////////////////////////////////////////////////
vecCEdit::~vecCEdit()
{
	vector<CTextBoxWindow*>::iterator i;
	for (i=begin();i != end(); ++i)
	{
		*i=0;
//		delete *i;
	}
}
///////////////////////////////////////////////////////////////////////////////
	//	CClientDC dc(NULL);
	//	if (hFont != NULL) hOldFont = dc.SelectObject(hFont);
//		size = dc.GetTextExtent("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//		size = dc.GetTextExtent("1234>");
//		BOOL res = GetTextExtentPoint32( hdc, "1234>", _tcslen("1234>"), &size );
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::CreateTextBoxWindow()
{
	//_variant_t v((long)m_entryNumber);
	//std::wstring bstr(v);
	std::wstring bstr = std::to_wstring((long)m_entryNumber);
	bstr += L"<   ";
	CSize size;
	{
	//	HGDIOBJ hOldFont = NULL;
	//	HFONT hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
		HDC hdc = GetDC()->GetSafeHdc();
	//	if (hFont != NULL) hOldFont = (HFONT)SelectObject(hdc, hFont);
		BOOL res = GetTextExtentPoint32W( hdc, bstr.c_str(), static_cast<int>(bstr.length()), &size );
		::ReleaseDC(GetSafeHwnd(), hdc);
	}

	int cy = size.cy;
	CRect rc;
	GetClientRect(&rc);
	int x = 0;
	ASSERT (vec_pTextBoxWindow.GetBottom() == m_Height );
	int y = vec_pTextBoxWindow.GetBottom();
	ASSERT (y == m_Height );
	int cx = size.cx;
	DWORD dwStyle=0, dwExStyle=0;
	HWND hWnd = m_hWnd;

	dwStyle=WS_CHILD|WS_VISIBLE|SS_LEFT;//SS_SIMPLE|SS_WHITERECT;_T("")
	HWND hWndPrompt = CreateWindowExW(dwExStyle, L"STATIC", bstr.c_str(), dwStyle,x,y,cx, cy, hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(hWndPrompt != NULL);
	ASSERT(::IsWindow(hWndPrompt));

	dwExStyle	= WS_EX_TRANSPARENT|WS_EX_CONTROLPARENT;
	dwStyle		= WS_CHILD|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|ES_AUTOHSCROLL;//|WS_BORDER;
	x	= cx;
//	cx = rc.Width() - size.cx;
//	cx=2048;
//	GetParentFrame()->GetClientRect(&rc);
//	cx = rc.Width();
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->GetClientRect(&rc);
	cx = rc.Width();

	RECT deskRect={0,0,0,0};
	HWND desk_hwnd		= ::GetDesktopWindow();
	::GetClientRect(desk_hwnd, &deskRect);
	cx = deskRect.right;

	HWND hWndEDIT = CreateWindowEx(0,_T("EDIT"),_T(""),dwStyle,x,y, cx, cy,hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(hWndEDIT != NULL);
	ASSERT(::IsWindow(hWndEDIT));
	LONG_PTR lStyle = ::GetWindowLongPtr(hWndEDIT, GWL_STYLE);
	lStyle |= ES_MULTILINE|ES_WANTRETURN;
	::SetWindowLongPtr(hWndEDIT, GWL_STYLE, lStyle);
	::SetFocus(hWndEDIT);

	CTextBoxWindow* pTextBoxWindow = new  CTextBoxWindow(this, y, cy, hWndPrompt);
	if (!pTextBoxWindow->SubclassWindow(hWndEDIT))
	{
		ASSERT(FALSE);		// possibly trying to subclass twice?
		AfxThrowNotSupportedException();
	}

	::SetWindowLongPtr(hWndEDIT, GWLP_USERDATA, m_entryNumber++);//(DWORD)((++colorWindow)%2));

	pTextBoxWindow->SetFocus();
	SizeUpdate(cy);
	::ShowWindow (hWndEDIT, SW_NORMAL );
	vec_pTextBoxWindow.push_back(pTextBoxWindow);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Output
///////////////////////////////////////////////////////////////////////////////
BOOL CSolveItView::CreateOutputWindow(const wstring& text) 
{
	if (text.size() == 0) return false;
//	static DWORD colorWindow = 0;
	int x=0, y=0, cx = 16, cy = 0, cyPrompt = 0;
	y=vec_pTextBoxWindow.GetBottom();
	ASSERT (y == m_Height );

	//_variant_t v((long)(m_entryNumber-1));
	std::wstring bstr = std::to_wstring((long)(m_entryNumber - 1));// (v);
	bstr += L">   ";
//	std::wstring bstrn = v;
//	std::wstring bstr(L"Out[");
//	bstr += bstrn;
//	bstr += L"]% ";
	{
		SIZE size={0,0};
		HDC hdc = GetDC()->GetSafeHdc();
		BOOL res = GetTextExtentPoint32W( hdc, bstr.c_str(), static_cast<int>(bstr.length()), &size );
		::ReleaseDC(GetSafeHwnd(),hdc);
		cx			= size.cx;
		cyPrompt	= size.cy;//
	}
	{
		std::wstring buf(text.c_str());
		SIZE size={0,0};
		HDC hdc = vec_pTextBoxWindow.back()->GetDC()->GetSafeHdc();
		BOOL res = GetTextExtentPoint32W( hdc, buf.c_str(), static_cast<int>(buf.length()), &size );
		::ReleaseDC(GetSafeHwnd(),hdc);
		cy = size.cy;//==16 regardless => call pTextBoxWindow->Resize();
	}


	DWORD dwStyle=WS_CHILD|WS_VISIBLE|SS_SIMPLE;//SS_WHITERECT;_T("")
	HWND hWnd = CreateWindowExW(0,L"STATIC",bstr.c_str(),dwStyle,x,y,cx,cy,m_hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(hWnd != NULL);
	ASSERT(::IsWindow(hWnd));


	dwStyle=WS_CHILD|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_READONLY|ES_AUTOHSCROLL;//|ES_AUTOVSCROLL|WS_BORDER;
//	CRect rc;
//	GetClientRect(&rc);
	x = cx;
//	cx = rc.Width()-cx;
	cx = m_Width-cx;

	CTextBoxWindow* pTextBoxWindow = new  CTextBoxWindow(this, y, cy, hWnd);//, cx

//	int cy = 36;//r.bottom - r.top;
	hWnd = CreateWindowExW(0,L"EDIT",text.c_str(),dwStyle,x,y,cx,cy,m_hWnd,(HMENU)0,AfxGetInstanceHandle(),0);
	ASSERT(hWnd != NULL);
	ASSERT(::IsWindow(hWnd));
//	long lStyle = ::GetWindowLong(hwnd, GWL_STYLE);_T("")
//	lStyle |= ES_MULTILINE|ES_WANTRETURN;
//	::SetWindowLong(hwnd, GWL_STYLE, lStyle);

	if (!pTextBoxWindow->SubclassWindow(hWnd))
	{
		ASSERT(FALSE);		// possibly trying to subclass twice?
		AfxThrowNotSupportedException();
	}

////	::SetWindowText(hWnd, text );
	::ShowWindow (hWnd, SW_NORMAL );
	pTextBoxWindow->Resize(0);
	pTextBoxWindow->SetFocus();

//	vec_hWnd.push_back(pTextBoxWindow->m_hWnd);
	vec_pTextBoxWindow.push_back(pTextBoxWindow);
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);//(DWORD)((++colorWindow)%2));

	SizeUpdate(cy);

	CreateTextBoxWindow();

	return (hWnd != NULL);
}
///////////////////////////////////////////////////////////////////////////////

/*
BOOL CSolveItView::OnEraseBkgnd(CDC* pDC) 
{
	RECT rc;
	GetWindowRect(&rc);
	::FillRect(pDC->m_hDC, &rc, (HBRUSH) GetStockObject(WHITE_BRUSH));
	return FALSE;
	return TRUE; // tell Windows not to erase the background
}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


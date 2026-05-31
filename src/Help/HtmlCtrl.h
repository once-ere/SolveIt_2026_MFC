////////////////////////////////////////////////////////////////
// Microsoft Systems Journal -- December 1999
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HtmlCtrl_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_)
#define AFX_HtmlCtrl_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_

#include <afxhtml.h>
#include "TraceWin.h"
#include "resource.h"       // main symbols

class CHtmlCtrl : public CHtmlView {
public:
	CHtmlCtrl() { }
	~CHtmlCtrl() { }

	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

	// Normally, CHtmlView destroys itself in PostNcDestroy,
	// but we don't want to do that for a control since a control
	// is usually implemented as a stack object in a dialog.
	//
	virtual void PostNcDestroy() {  }

	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// override to trap "app:" pseudo protocol
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel );

	// override to handle links to "app:mumble...". lpszWhere will be "mumble"
	virtual void OnAppCmd(LPCTSTR lpszWhere);

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CHtmlCtrl)
};



//pln:
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// About dialog uses HTML control to display contents.const wchar_t * res_name
//typedef const wchar_t * jjj;
//template <class T, T* res_name>
//template <typename T, T* res_name>
class CInfoDialog : public CDialog {
//typename T	typedef T res_name;
	DECLARE_DYNAMIC(CInfoDialog)
protected:
	wstring res_name;
	wstring title;
	wstring NavigateTo;
	class CXXXHtmlCtrl : public CHtmlCtrl
	{
// Handle "app:ok" link by closing dialog
		virtual void OnAppCmd(LPCTSTR lpszWhere)
		{
			if (_tcsicmp(lpszWhere,_T("ok"))==0) {
				GetParent()->SendMessage(WM_COMMAND,IDOK);
			}
		}
	} m_page;

	virtual BOOL OnInitDialog()
	{
		VERIFY(CDialog::OnInitDialog());
		int ret = -1;
		SetWindowText(title.c_str());
//		CenterWindow(CWnd::GetDesktopWindow());
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
	//	VERIFY(CDialog::OnInitDialog());
		VERIFY(m_page.CreateFromStatic(IDC_HTMLVIEW, this));
		if (res_name.size())
		{
			m_page.LoadFromResource(res_name.c_str());
		}
		else
		if (NavigateTo.size())
		{
		//	TCHAR buf[_MAX_PATH];
		//	GetCurrentDirectory(_MAX_PATH, buf);
		//	_bstr_t bbuf(buf);
		//	bbuf += L"\\";
		//	bbuf += NavigateTo.c_str();
			_bstr_t bbuf(NavigateTo.c_str());
			m_page.Navigate(bbuf, 0, 0, 0);
		}
		else
		return FALSE;
		return TRUE;
	} 
	virtual void PostNcDestroy()
	{
		delete this;
	}
	virtual void OnCancel()
	{
		DestroyWindow();
	}
	afx_msg void OnSize(UINT nType, int cx, int cy)
	{
		CDialog::OnSize(nType, cx, cy);
		if ( 0 >= cx || 0 >= cy ){
			return;
		}
		if (!::IsWindow(m_page.m_hWnd)) return;
		::MoveWindow(m_page.m_hWnd, 0, 0, cx, cy, TRUE);
		m_page.SendMessage(WM_SIZE,SIZE_RESTORED, MAKELONG(cx, cy));
	}
	DECLARE_MESSAGE_MAP();

public:
	CInfoDialog(const wchar_t * _res_name, const wchar_t * _title, CWnd* pParent = NULL, const wchar_t * _NavigateTo=0) :
		CDialog(IDD_HELPBOX, pParent),
		title(_title),
		res_name(L""),
		NavigateTo(L"")
	{
		if (_res_name) res_name = _res_name;
		if (_NavigateTo) NavigateTo = _NavigateTo;
		CDialog::Create(IDD_HELPBOX, pParent);
		ShowWindow(SW_SHOW);
	}
};

//IMPLEMENT_DYNAMIC(CInfoDialog, CDialog)
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_HtmlCtrl_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_)

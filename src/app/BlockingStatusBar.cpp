// BlockingStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "BlockingStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockingStatusBar

CBlockingStatusBar::CBlockingStatusBar():
	m_bIgnoreSetText(false)
{
}

CBlockingStatusBar::~CBlockingStatusBar()
{
}


BEGIN_MESSAGE_MAP(CBlockingStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CBlockingStatusBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SETTEXT, OnSetText)
	ON_UPDATE_COMMAND_UI(ID_SEPARATOR, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, OnUpdateBlockingStatusBar)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
void CBlockingStatusBar::OnUpdateBlockingStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CBlockingStatusBar message handlers
LRESULT CBlockingStatusBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
	//if ( !m_bIgnoreSetText ) return CStatusBar::OnSetText( wParam, lParam );
	if (!m_bIgnoreSetText) return CStatusBar::OnSetText((LPCTSTR)wParam /*, lParam */);
	return 0;  // Same as CStatusBar::OnSetText success
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CBlockingStatusBar::SetPaneOneAndLock(const wstring& str)
{
	m_bIgnoreSetText	= true;
	PreparePane( 0, str);
	return CStatusBar::SetPaneText(0, CString(str.c_str()));
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CBlockingStatusBar::SetPaneOneAndUnlock(const wstring& str)
{
	m_bIgnoreSetText	= false;
	PreparePane( 0, str);
	return CStatusBar::SetPaneText(0, CString(str.c_str()));
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CBlockingStatusBar::SetTextPane( int nIndex, const wstring& str)
{
	if (nIndex == 0) return SetPaneOneAndLock(str);
	PreparePane( nIndex, str);
	return CStatusBar::SetPaneText(nIndex, CString(str.c_str()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CBlockingStatusBar::PreparePane( int nIndex, const wstring& str)
{
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(str.c_str());
		if (hOldFont != NULL) dc.SelectObject(hOldFont);
	}
	UINT nID=-1, nStyle=-1;
	int nWidth=-1;
	GetPaneInfo(nIndex, nID, nStyle, nWidth);
//	ASSERT( nStyle == SBPS_NORMAL);
	nStyle = SBPS_NORMAL;
	SetPaneInfo(nIndex, nID, nStyle, size.cx);
}
///////////////////////////////////////////////////////////////////////////////


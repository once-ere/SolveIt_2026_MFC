// ChildFrm.cpp : implementation of the CChildFrame2 class
//

#include "stdafx.h"
#include "SolveIt.h"

#include "ChildFrm2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame2

IMPLEMENT_DYNCREATE(CChildFrame2, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame2, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame2 construction/destruction

CChildFrame2::CChildFrame2()
{
	// TODO: add member initialization code here
	
}

CChildFrame2::~CChildFrame2()
{
}

BOOL CChildFrame2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		cs.style ^=   WS_SYSMENU ;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}





/////////////////////////////////////////////////////////////////////////////
// CChildFrame2 diagnostics

#ifdef _DEBUG
void CChildFrame2::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame2::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame2 message handlers
///////////////////////////////////////////////////////////////////////////////
#if 0
BOOL CChildFrame2::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message < WM_KEYFIRST) return CMDIChildWnd::PreTranslateMessage(pMsg);
	if (pMsg->message > WM_KEYLAST ) return CMDIChildWnd::PreTranslateMessage(pMsg);

	switch (pMsg->message)
	{
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			return TRUE;
			return FALSE;//enables LEFT and RIGHT arrow keys
		case WM_KEYDOWN:
		case WM_KEYUP:
		switch (pMsg->wParam)
		{
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_HOME:
			case VK_END:
			case VK_PRIOR:
			case VK_NEXT:
			SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
			// Windowless controls won't be able to call SendMessage.
			// Instead, just respond to the message here.
			return TRUE;
			return FALSE;//enables LEFT and RIGHT arrow keys
		}
		break;
	}
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}
#endif
///////////////////////////////////////////////////////////////////////////////

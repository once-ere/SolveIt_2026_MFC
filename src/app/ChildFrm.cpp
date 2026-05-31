// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "SolveIt.h"

#include "ChildFrm.h"
#include "LeftView.h"
#include "SolveItView.h"
#include "MainFrm.h"
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_WM_CLOSE()
///////////////////////////////////////////////////////////////////////////////
void CChildFrame::OnClose() 
{
//	theApp.CloseAllDocuments( TRUE );
//	theApp.m_pGLViewTemplate->CloseAllDocuments(true);
}
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction
///////////////////////////////////////////////////////////////////////////////
CChildFrame::CChildFrame():
	m_pCGLView(0),
	m_pCSolveItView(0),
	theTreeView(0)
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}
///////////////////////////////////////////////////////////////////////////////
//BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
///////////////////////////////////////////////////////////////////////////////
BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs,	CCreateContext* pContext)
{
///////////////////////////////////////////////////////////////////////////////
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
	pFrame->SetChildFrame(this);//
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;


///////////////////////////////////////////////////////////////////////////////
#if 0
	ASSERT(pContext	!= NULL);
	if(pContext	== NULL)		return FALSE;

	BOOL bRet = FALSE;
	CDocument *pDoc=NULL;
	POSITION pos;
	//create the source view for this document at this time
	CRuntimeClass *prc = RUNTIME_CLASS(CGLView);
	ASSERT(prc);
	m_pCGLView = (CGLView*)prc->CreateObject();
	if(m_pCGLView)
		VERIFY(m_pCGLView->CView::Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, NULL));

	//create the doc-template view and save off a pointer to it
//	bRet = CFrameWnd::OnCreateClient(lpcs, pContext);

#endif
///////////////////////////////////////////////////////////////////////////////


	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSolveItView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
///////////////////////////////////////////////////////////////////////////////
#if 0

	bRet = TRUE;

	pDoc = pContext->m_pCurrentDoc;
	ASSERT(pDoc	!= NULL);
	if(pDoc	== NULL)		return FALSE;

	if(bRet && pDoc)
	{
		pDoc->AddView(m_pCGLView);
		pos = pDoc->GetFirstViewPosition();
	}
	else
		return FALSE;

	while (pos) {
		CView *pView = pDoc->GetNextView(pos);
	if(pView)
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CLeftView))) {
			theTreeView = (CLeftView*) pView;
			ASSERT(pView == GetView(0, 0));
		}
		if (pView->IsKindOf(RUNTIME_CLASS(CSolveItView))) {
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CSolveItView)));
			m_pCSolveItView = (CSolveItView*) pView;
			ASSERT(pView == GetView(0, 1));
		}
	}
	}
//	else
//		return FALSE;
//	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->SetChildFrame(this);

	return bRet;

#endif
///////////////////////////////////////////////////////////////////////////////

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
	CDocument *pDoc=NULL;
	POSITION pos;
	BOOL bRet = FALSE;
	//create the source view for this document at this time
	CRuntimeClass *prc = RUNTIME_CLASS(CSourceView);
	ASSERT(prc);
	m_pSrcView = (CSourceView*)prc->CreateObject();
	if(m_pSrcView)
		VERIFY(m_pSrcView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, NULL));


	//create the doc-template view and save off a pointer to it
	bRet = CFrameWnd::OnCreateClient(lpcs, pContext);
	pDoc = pContext->m_pCurrentDoc;
	if(bRet && pDoc)
	{
		pos = pDoc->GetFirstViewPosition();
		pDoc->AddView(m_pSrcView);
	}
	else
		return FALSE;

	CView *pView = pDoc->GetNextView(pos);
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CHTMLEdView)));
	if(pView)
	{
		m_pWebView = (CHTMLEdView*) pView;
	}
	else
		return FALSE;

	return bRet;
}

void CChildFrame::SwapView(int nCmdID)
{
	CDocument *pDoc;

	pDoc = GetActiveDocument();
	if(nCmdID == ID_VIEW_SOURCE)
	{
		//swap the view IDs
		m_pWebView->SetDlgCtrlID(AFX_IDW_PANE_LAST);
		m_pSrcView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

		// show/hide the right view
		m_pWebView->ShowWindow(SW_HIDE);
		m_pSrcView->ShowWindow(SW_SHOW);

		//update the document
		//pDoc->AddView(m_pSrcView);
		//pDoc->RemoveView(m_pWebView);

		//re-layout everything
		SetActiveView(m_pSrcView);
		RecalcLayout();

		m_dwCurrentView = ID_VIEW_SOURCE;

		m_wndStatusBar.SetPaneText(1, "SRC VIEW", TRUE);

		return;
	}
	else if(nCmdID == ID_VIEW_WEB)
	{
		//swap the view IDs
		m_pWebView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		m_pSrcView->SetDlgCtrlID(AFX_IDW_PANE_LAST);

		// show/hide the right view
		m_pWebView->ShowWindow(SW_SHOW);
		m_pSrcView->ShowWindow(SW_HIDE);

		//update the document
		//pDoc->AddView(m_pWebView);
		//pDoc->RemoveView(m_pSrcView);

		//re-layout everything
		SetActiveView(m_pWebView);
		RecalcLayout();

		m_dwCurrentView = ID_VIEW_WEB;
		m_wndStatusBar.SetPaneText(1, "WEB VIEW", TRUE);
		return;
	}

}
void CChildFrame::SwapView()
{
	//if we are already showing the right view, bail, nothing to do.
	DWORD nCmdID = LOWORD(GetCurrentMessage()->wParam);   
	if((nCmdID == ID_VIEW_SOURCE) && (m_dwCurrentView == ID_VIEW_SOURCE))
		return;
	if((nCmdID == ID_VIEW_WEB) && (m_dwCurrentView == ID_VIEW_WEB))
		return;

	SwapView(nCmdID);
}


void CChildFrame::OnViewSource() 
{
	SwapView();	
}

void CChildFrame::OnViewWeb() 
{
	SwapView();
}

void CChildFrame::OnUpdateViewWeb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dwCurrentView==ID_VIEW_WEB);
	
}

void CChildFrame::OnUpdateViewSource(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_dwCurrentView==ID_VIEW_SOURCE);
	
}

CView* CChildFrame::GetWebView()
{
	return DYNAMIC_DOWNCAST(CView,m_pWebView);
}

CView* CChildFrame::GetSourceView()
{
	return DYNAMIC_DOWNCAST(CView,m_pSrcView);
}

DWORD CChildFrame::GetCurrentView()
{
	return m_dwCurrentView;
}
#endif
///////////////////////////////////////////////////////////////////////////////



BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
///////////////////////////////////////////////////////////////////////////////
//		cs.style &= ! WS_SYSMENU ;
		cs.style ^=   WS_SYSMENU ;
///////////////////////////////////////////////////////////////////////////////
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.
//	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetChildFrame(this);
	CLeftView*	tree = (CLeftView*) GetView(0, 0);//pFrame->GetTreeView();
	if (tree != 0)
	{
/*
		tree->PopulateTree();
*/
		tree->UpdateTree();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
/////////////////////////////////////////////////////////////////////////////
CView *CChildFrame::GetView(int row, int col) {
//	if (g_bInitNotComplete) return 0;
	if (!m_wndSplitter.m_hWnd || !::IsWindow(m_wndSplitter.m_hWnd)) return 0;
	if (!m_wndSplitter.GetSafeHwnd() || !::IsWindow(m_wndSplitter.GetSafeHwnd())) return 0;
	return ((CView *) m_wndSplitter.GetPane(row,col));
}
///////////////////////////////////////////////////////////////////////////////

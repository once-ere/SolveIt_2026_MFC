// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"

#include "MainFrm.h"
#include "System.h"

#include "ChildFrm.h"
#include "LeftView.h"
#include "SolveItView.h"
#include "GLView.h"
#include "StackMachine.h"
//#include "Worker.h"
#include "Cruncher.h"
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

const UINT indicators[] =
{
	ID_SEPARATOR,		   // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SINGLE_STEP, OnStep)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_PNG, OnPNG)
	ON_BN_CLICKED(IDC_EPS, OnEPS)
	ON_BN_CLICKED(IDC_PRINT_CODE, OnListCode)
	ON_BN_CLICKED(ID_VIEW_FLYBY, OnFlyBy)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLYBY, OnUpdateFlyBy)
	ON_COMMAND(ID_VIEW_RESET, OnViewReset)
	ON_COMMAND(ID_VIEW_TOGGLE, OnToggleView)
	ON_COMMAND(ID_VIEW_1DIM, OnView1dim)
	ON_UPDATE_COMMAND_UI(ID_VIEW_1DIM, OnUpdateView1dim)
	ON_COMMAND(ID_VIEW_2DIM, OnView2dim)
	ON_UPDATE_COMMAND_UI(ID_VIEW_2DIM, OnUpdateView2dim)
	ON_COMMAND(ID_VIEW_3DIM, OnView3dim)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3DIM, OnUpdateView3dim)
	ON_BN_CLICKED(IDC_BUTTON_Reset, OnReset)
	ON_REGISTERED_MESSAGE(WM_USER_PASS_DONE, OnHalt)
	ON_REGISTERED_MESSAGE(WM_USER_Initialize, OnInitialize)
	ON_REGISTERED_MESSAGE(WM_USER_Message, OnUSER_Message)
	ON_REGISTERED_MESSAGE(WM_USER_KillWorker, OnUSER_KillWorker)
	ON_REGISTERED_MESSAGE(WM_USER_AddCInfoDialog, OnUSER_AddCInfoDialog)
	ON_REGISTERED_MESSAGE(WM_USER_AddCInfoDialog2, OnUSER_AddCInfoDialog2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESET, OnUpdateOnViewReset)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_Reset, OnUpdateOnReset)
	ON_EN_CHANGE(IDC_EDIT_ScanTime, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_ScanStepsize, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_BEGIN_x, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_BEGIN_y, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_END_x, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_END_y, OnEnChange)
	ON_BN_CLICKED(IDC_BUTTON_Scan, OnClickedButton_scan )
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////
#if 0
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_SEPARATOR, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, OnUpdateBlockingStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, OnUpdateBlockingStatusBar)
void CMainFrame::OnUpdateBlockingStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
#endif
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetStatusBarText(const wstring& str, int  nStatusPane)
{
	int nOrigSize = sizeof(indicators) / sizeof(indicators[0]);
	if (nStatusPane >= nOrigSize) return;
	if (nStatusPane == 0) m_wndStatusBar.SetPaneOneAndLock(str);
	else m_wndStatusBar.SetTextPane( nStatusPane, str.c_str());
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnUSER_Message(WPARAM wParam, LPARAM lParam)
{
	wstring str((wchar_t const*const )lParam);
	//GlobalFree((void*)lParam);
	SetStatusBarText( str, static_cast<int>(wParam));
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz); //Action::DupString
//requires a call to	//GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
//std::wstring  DupString(std::wstring const &  lpsz); //_tcsdup; memory leak
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnUSER_AddCInfoDialog(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL) throw SolveIt_ERROR(L"CMainFrame::OnUSER_AddCInfoDialog: null wParam");
	if (lParam == NULL) throw SolveIt_ERROR(L"CMainFrame::OnUSER_AddCInfoDialog: null lParam");
	wstring html_file((wchar_t const* const)wParam);
	////GlobalFree((void*)wParam);
	wstring title((wchar_t const* const)lParam);
	////GlobalFree((void*)lParam);
	CInfoDialog* pDialog = new CInfoDialog(html_file.c_str(), title.c_str());
	m_CleanupOnInitialize.Add(pDialog->m_hWnd);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnUSER_AddCInfoDialog2(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL) throw SolveIt_ERROR(L"CMainFrame::OnUSER_AddCInfoDialog2: null wParam");
	if (lParam == NULL) throw SolveIt_ERROR(L"CMainFrame::OnUSER_AddCInfoDialog2: null lParam");
	wstring text((wchar_t const*const )wParam);
	////GlobalFree((void*)wParam);
	wstring title((wchar_t const*const )lParam);
	////GlobalFree((void*)lParam);
	CInfoDialog* pDialog = new CInfoDialog(0, title.c_str(), 0, text.c_str());
	m_CleanupOnInitialize.Add(pDialog->m_hWnd);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnUSER_KillWorker(WPARAM wParam, LPARAM lParam)
{
	ASSERT (m_system != NULL);
	if (m_system == NULL) throw SolveIt_ERROR(L"CMainFrame::OnUSER_KillWorker: null m_system");
	m_system->StopWorker();//KillWorker();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
extern const wchar_t scratchFileName[];
extern const wchar_t scratchBmpName[];
///////////////////////////////////////////////////////////////////////////////
//	if (::IsWindow(ElectroStaticshWnd)) ::SendMessage(ElectroStaticshWnd, WM_CLOSE,0,0);
//	if (::IsWindow(VectorAnalysis_hWnd)) ::SendMessage(VectorAnalysis_hWnd, WM_CLOSE,0,0);
//	ElectroStaticshWnd	 	  = 0;
//	VectorAnalysis_hWnd 	  = 0;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::Initialize()
{
	m_CleanupOnInitialize.Clear();
	HWND hWnd = ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP);

	ASSERT(hWnd);
	ASSERT(::IsWindow(hWnd));


//	::SendMessage(GetSafeHwnd(), BN_CLICKED,IDC_STOP, ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP));
//	::SendMessage(GetSafeHwnd(), BN_CLICKED, 0, 0);//(LPARAM ) ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP));
//	::SendMessage(m_wndDlgBar.GetSafeHwnd(), BN_CLICKED, 0, 0);
//	HWND hWnd2 = ::SetActiveWindow(m_wndDlgBar.GetSafeHwnd());
	::EnableWindow(hWnd, TRUE);
	HWND hWnd3 = ::SetActiveWindow(hWnd);
	::SendMessage(hWnd, BM_CLICK, 0, 0);
//	OnStop();//LRESULT res = 
//	BM_CLICK: WM_LBUTTONDOWN and WM_LBUTTONUP messages, and the button's parent window to receive a BN_CLICKED 
//	PostMessage(WM_USER_Initialize, 0, 0);
///////////////////////////////////////////////////////////////////////////////
	CSolveItView* pCSolveItView = GetEditView();
	if (pCSolveItView == NULL) return;
	pCSolveItView->SetActiveWindow();

	DWORD res = GetFileAttributes(scratchFileName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}

	delete m_system;
	m_system	= 0;
	m_system	= new CSystem;
	CGLView* gl = GetGLView();
	if (gl==NULL) throw SolveIt_ERROR(L"gl==NULL");
	gl->Initialize();
	UpdateTree();
// Select the projection matrix and clear it
	gl->SetPerspectiveOrFrustum();
	gl->Invalidate();

	SetEvent(g_EventInitialize);
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnInitialize(WPARAM wParam, LPARAM lParam)
{
	try {
	m_CleanupOnInitialize.Clear();
	HWND hWnd = ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP);

	ASSERT(hWnd);
	ASSERT(::IsWindow(hWnd));


//	::SendMessage(GetSafeHwnd(), BN_CLICKED,IDC_STOP, ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP));
//	::SendMessage(GetSafeHwnd(), BN_CLICKED, 0, 0);//(LPARAM ) ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP));
//	::SendMessage(m_wndDlgBar.GetSafeHwnd(), BN_CLICKED, 0, 0);
//	HWND hWnd2 = ::SetActiveWindow(m_wndDlgBar.GetSafeHwnd());
	::EnableWindow(hWnd, TRUE);
	HWND hWnd3 = ::SetActiveWindow(hWnd);
	::SendMessage(hWnd, BM_CLICK, 0, 0);
//	OnStop();//LRESULT res = 
//	BM_CLICK: WM_LBUTTONDOWN and WM_LBUTTONUP messages, and the button's parent window to receive a BN_CLICKED 
//	PostMessage(WM_USER_Initialize, 0, 0);
///////////////////////////////////////////////////////////////////////////////
	CSolveItView* pCSolveItView = GetEditView();
	if (pCSolveItView == NULL) return 0;
	if (pCSolveItView == NULL) return 0;
	pCSolveItView->SetActiveWindow();

/*m_pCSolveItView;//
	if (m_pCGLView == NULL) return;
	if (m_pCSolveItView == NULL) return;
	CGLView *;
	CSolveItView*	;
*/
	DWORD res = GetFileAttributes(scratchFileName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}


	delete m_system;
	m_system	= 0;
	m_system	= new CSystem;
	CGLView* gl = GetGLView();
	if (gl==NULL) throw SolveIt_ERROR(L"gl==NULL");
	gl->Initialize();
	UpdateTree();
// Select the projection matrix and clear it
	gl->SetPerspectiveOrFrustum();
	gl->Invalidate();

	SetEvent(g_EventInitialize);

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CMainFrame::OnInitialize ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CMainFrame::OnInitialize");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnHalt(WPARAM wParam, LPARAM lParam)
{
	OnStop();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():
	m_system(0),//new CSystem
//	m_pCGLView(0),
//	m_pCSolveItView(0),
	m_pMDIActive(0),
///////////////////////////////////////////////////////////////////////////////
	m_TimerScan(2),
	m_uMillisecondsBetweenScanSteps(200),
	m_x1(-2.5/2),
	m_y1(2.5/2),
	m_x2(2.5/2),
	m_y2(-2.5/2),
	m_fScanCurveParameter(0),
	m_fScanDistance(sqrt((m_x2-m_x1)*(m_x2-m_x1)+(m_y2-m_y1)*(m_y2-m_y1))),
	m_fScanStepSize(1.41421356237309505/100.),
///////////////////////////////////////////////////////////////////////////////
	m_hIconPerspective(0),
	m_hIconParallel(0)
{
	// TODO: add member initialization code here
	
}
///////////////////////////////////////////////////////////////////////////////
CMainFrame::~CMainFrame()
{
}
///////////////////////////////////////////////////////////////////////////////

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;	  // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;	  // fail to create
	}
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;	  // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

//	CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

	m_hIconPerspective	= AfxGetApp()->LoadIcon(IDR_GLVIEW_TMPL);//IDI_ICON_Perspective
	m_hIconParallel		= AfxGetApp()->LoadIcon(IDI_ICON1);//IDI_ICON_Parallel
//	OnToggleView();
//	OnToggleView();
	if (0 != m_hIconPerspective)
	{
		m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETSTYLE, BS_ICON|BS_PUSHBUTTON, 0);
		m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(HANDLE) m_hIconPerspective);
	}


	CDialogBar &bar = m_wndDlgBar;
	TCHAR buf[32+1];
	/*ostrstream*/ std::wstringstream oss(buf,32);
	oss.seekp(0);
	oss << m_x1 << ends;
	bar.SetDlgItemText(IDC_EDIT_BEGIN_x, buf);
	oss.seekp(0);
	oss << m_y1 << ends;
	bar.SetDlgItemText(IDC_EDIT_BEGIN_y, buf);
	oss.seekp(0);
	oss << m_x2 << ends;
	bar.SetDlgItemText(IDC_EDIT_END_x, buf);
	oss.seekp(0);
	oss << m_y2 << ends;
	bar.SetDlgItemText(IDC_EDIT_END_y, buf);
	oss.seekp(0);
	oss << m_uMillisecondsBetweenScanSteps << ends;
	bar.SetDlgItemText(IDC_EDIT_ScanTime, buf);
	oss.seekp(0);
	oss << m_fScanStepSize << ends;
	bar.SetDlgItemText(IDC_EDIT_ScanStepsize, buf);






	m_system	= new CSystem;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnDestroy() 
{
	OnStop();
	m_system->KillWorker();
	delete m_system;
//	m_system->Initialize();

	if (0 != m_hIconPerspective) DestroyIcon(m_hIconPerspective);
	if (0 != m_hIconParallel) DestroyIcon(m_hIconParallel);
	CMDIFrameWnd::OnDestroy();
	
//if (m_hBitmapPerspective) DeleteObject(m_hBitmapPerspective);
//if (m_hBitmapParallel) DeleteObject(m_hBitmapParallel);
}
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::CreateOrActivateFrame(CDocTemplate* pTemplate, CRuntimeClass* pViewClass) {
// If a view (specified by pViewClass) already exists, activate the MDI child window containing the view.
// Otherwise, create a new view for the document.
	CMDIChildWnd* pMDIActive = MDIGetActive();
	ASSERT(pMDIActive != NULL);
	if (pMDIActive == NULL) return;
	CDocument* pDoc = pMDIActive->GetActiveDocument();
	ASSERT(pDoc != NULL);

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(pViewClass))
		{
			pView->GetParentFrame()->ActivateFrame();
			return;
		}
	}

	CMDIChildWnd* pNewFrame = (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));

	if (pNewFrame == NULL) return;	 // not created

	ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
	pTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::UpdateTree()
{
	CLeftView*	tree = GetTreeView();
	if (tree != 0)
	{
		tree->PopulateTree();
		tree->UpdateTree();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetChildFrame(CChildFrame* pMDIActive)
{
	m_pMDIActive = pMDIActive;
	ASSERT(m_pMDIActive != NULL);
///////////////////////////////////////////////////////////////////////////////
#if 0
	CGLView*pGLView	= GetGLView();
	ASSERT(pGLView != NULL);
	CSolveItView*pEditView	= GetEditView();
	ASSERT(pEditView != NULL);
	if (pGLView == NULL) return;
	if (pEditView == NULL) return;
//	pGLView->SetDlgCtrlID(AFX_IDW_PANE_LAST);
//	pEditView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

// show/hide the right view
//		m_pWebView->ShowWindow(SW_HIDE);
	pGLView->ShowWindow(SW_SHOW);
	pEditView->ShowWindow(SW_SHOW);


//re-layout everything
	SetActiveView(pGLView);
	RecalcLayout();
	SetActiveView(pEditView);
	RecalcLayout();
#endif
///////////////////////////////////////////////////////////////////////////////

}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetChildFrame()
{
	m_pMDIActive = dynamic_cast<CChildFrame*>(MDIGetActive());
	ASSERT(m_pMDIActive != NULL);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CLeftView*		CMainFrame::theTreeView	= 0;
CSolveItView*	CMainFrame::theEditView	= 0;
CGLView*		CMainFrame::theGLView	= 0;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CLeftView*		CMainFrame::GetTreeView()
{
	return theTreeView;
///////////////////////////////////////////////////////////////////////////////
#if 0
	CChildFrame* pMDIActive = m_pMDIActive;//dynamic_cast<CChildFrame*>(MDIGetActive());
	ASSERT(pMDIActive != NULL);
	if (pMDIActive == NULL) return 0;
	CLeftView* p = dynamic_cast<CLeftView*> (pMDIActive->GetView(0, 0));
#ifdef _DEBUG
	if (p != NULL) p->AssertValid();
#endif
	return p;
#endif
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
CSolveItView*	CMainFrame::GetEditView()
{
	return theEditView;
///////////////////////////////////////////////////////////////////////////////
#if 0
	CChildFrame* pMDIActive = m_pMDIActive;//dynamic_cast<CChildFrame*>(MDIGetActive());//m_pMDIActive;
	ASSERT(pMDIActive != NULL);
	if (pMDIActive == NULL) return 0;
	if (pMDIActive != NULL) return pMDIActive->m_pCSolveItView;
	CSolveItView* p = dynamic_cast<CSolveItView*> (pMDIActive->GetView(0, 1));
#ifdef _DEBUG
	if (p != NULL) p->AssertValid();
#endif
	return p;
//	return dynamic_cast<CSolveItView*> (GetView(0, 1));
#endif
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
CGLView* CMainFrame::GetGLView()
{
	return theGLView;
///////////////////////////////////////////////////////////////////////////////
#if 0
	CGLView* pGLView = NULL;
	CChildFrame* pMDIActive = m_pMDIActive;//dynamic_cast<CChildFrame*>(MDIGetActive());//MDIGetActive();//same doc for all MDI-Children
	ASSERT(pMDIActive != NULL);
#ifdef _DEBUG
	if (pMDIActive != NULL) pMDIActive->AssertValid();
#endif
	if (pMDIActive == NULL) return 0;
	if (pMDIActive != NULL) return pMDIActive->m_pCGLView;
//	CChildFrame* pMDIActive = dynamic_cast<CChildFrame*>(pmDIActive);
//	ASSERT(pMDIActive != NULL);
	CDocument* pDoc = pMDIActive->GetActiveDocument();
	ASSERT(pDoc != NULL);
	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos) {
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGLView))) {
			pGLView = dynamic_cast<CGLView*>(pView);//(CGLView*)pView;
			return pGLView;
		}
	}
	return pGLView;
#endif
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::PositionWindow()
{
	CGLView* glView		= GetGLView();
	ASSERT(glView != NULL);
	if (glView==NULL) return;
	CenterWindow(CWnd::GetDesktopWindow());
	RECT Rect, deskRect, glRect={0,0,0,0};
	HWND desk_hwnd		= ::GetDesktopWindow();
	::GetWindowRect(desk_hwnd, &deskRect);
	HWND hWndGL			= glView->m_hWnd;
	if (hWndGL) ::GetWindowRect(hWndGL, &glRect);
	::GetWindowRect(m_hWnd, &Rect);
	//	GetTopLevelFrame()->GetWindowRect(&Rect);
	//	GetTopLevelParent()->GetWindowRect(&Rect);
	//	int w = Rect.right - Rect.left;
	int w = deskRect.right - ( glRect.right - glRect.left );
	int h = Rect.bottom - Rect.top;
	int y = deskRect.bottom - h 
			- GetSystemMetrics(SM_CYSIZEFRAME) 
		//	- GetSystemMetrics(SM_CYHSCROLL) 
			- GetSystemMetrics(SM_CYEDGE)
			- GetSystemMetrics(SM_CYSIZE)
			; 
	int x = 0;
	::MoveWindow(m_hWnd, x, y, w, h, TRUE);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnListCode()
{
	CStackMachine* stackMachine = m_system->stackMachine;
	std::ostringstream oss;
	stackMachine->print_code(oss);
	AfxMessageBox(GetWC(oss.str().c_str()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRun()
{
//	SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_system->Run();
//	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnStep()
{
//	m_system->m_bSingleStepping=true;
//	OnStop();
	HWND hWnd = ::GetDlgItem(m_wndDlgBar.GetSafeHwnd(), IDC_STOP);
	ASSERT(hWnd);
	ASSERT(::IsWindow(hWnd));
	::EnableWindow(hWnd, TRUE);
	HWND hWnd3 = ::SetActiveWindow(hWnd);
	::SendMessage(hWnd, BM_CLICK, 0, 0);
	m_system->SingleStep();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnStop()
{
try {
	ASSERT(m_system != NULL);
	if (m_system == NULL) return;
	m_system->Stop();
}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CMainFrame::OnStop");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClear()
{
	m_system->Clear();
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnPNG()
{
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->MakePNGImage();
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnEPS()
{
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->OnExportEps();
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClose() 
{
//	theApp.CloseAllDocuments( TRUE );
//	theApp.m_pGLViewTemplate->CloseAllDocuments(true);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFlyBy()
{
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->m_bFlyBy = !pGLView->m_bFlyBy;
	pGLView->Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFlyBy(CCmdUI* pCmdUI) 
{
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pCmdUI->SetCheck( pGLView->m_bFlyBy?1:0);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateOnViewReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_system->m_bStopIterations==true || m_system->m_bSingleStepping==true);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewReset()
{
//	LRESULT res = SendMessage(WM_USER_PASS_DONE, 0, 0);
//	LRESULT res = m_wndDlgBar.GetDlgItem(IDC_STOP)->SendMessage(WM_LBUTTONDOWN, 0, 0);
//	res = m_wndDlgBar.GetDlgItem(IDC_STOP)->SendMessage(WM_LBUTTONUP, 0, 0);

	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
	UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnToggleView()
{
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->ToggleViewTransformation();
	LRESULT res = 0;
		res = m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETSTYLE, BS_ICON|BS_PUSHBUTTON, 0);
	static bool j = false;
	if (j=!j)
		res = m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(HANDLE) m_hIconParallel);
	else
		res = m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(HANDLE) m_hIconPerspective);
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateOnReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(  (m_system->m_bRunning==false ));
	return;
//	DWORD ExitCode=0;
//		Lock lock (theApp.protectWorker);
//	if (m_system->worker == NULL ) ExitCode = TRUE;// FALSE;
//	else
//	{
//		BOOL ret = GetExitCodeThread(m_system->worker->m_hThread, &ExitCode);
//		if (ret == 0)
//		{
//			DWORD err = GetLastError();
//			_variant_t v = long(err);
//			std::wstring b(v);
//			throw SolveIt_ERROR((const wchar_t *) b);
//		}
//		if (ExitCode == STILL_ACTIVE) ExitCode = FALSE;
//	}
//	pCmdUI->Enable( ExitCode && (m_system->m_bStopIterations==true || m_system->m_bSingleStepping==true ));
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnReset() 
{
//	LRESULT res = SendMessage(WM_USER_PASS_DONE, 0, 0);
//	LRESULT res = m_wndDlgBar.GetDlgItem(IDC_STOP)->SendMessage(WM_LBUTTONDOWN, 0, 0);
//	res = m_wndDlgBar.GetDlgItem(IDC_STOP)->SendMessage(WM_LBUTTONUP, 0, 0);
///////////////////////////////////////////////////////////////////////////////

//	m_system->StopWorker();
	m_system->Reset();
	CGLView* pGLView = GetGLView();//theApp.GetGLView();
	if (pGLView == 0) return;
	pGLView->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
	UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
#if 0
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMDIFrameWnd::OnLButtonDblClk(nFlags, point);
}

void CMainFrame::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CMDIFrameWnd::OnRButtonDblClk(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message < WM_KEYFIRST) return CMDIFrameWnd::PreTranslateMessage(pMsg);
	if (pMsg->message > WM_KEYLAST ) return CMDIFrameWnd::PreTranslateMessage(pMsg);

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
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnInitialUpdate()
{
//	not called
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnView1dim() 
{
	m_system->m_worldDimension=CSystem::one;
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateView1dim(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_system->m_worldDimension==CSystem::one );
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnView2dim() 
{
	m_system->m_worldDimension=CSystem::two;
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateView2dim(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_system->m_worldDimension==CSystem::two );
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnView3dim() 
{
	m_system->m_worldDimension=CSystem::three;
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateView3dim(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_system->m_worldDimension==CSystem::three );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnEnChange()
{
	HWND hwnd = ::GetFocus();
	TCHAR buf[64+1];
	if (::GetWindowText(hwnd, buf, 32) > 0 )
	{
		/*istrstream*/ std::wstringstream iss(buf,32);
		double x=0;
		if (!(iss>>x).fail())
		{
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_ScanTime))		m_uMillisecondsBetweenScanSteps = UINT(x+0.5);
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_ScanStepsize))	m_fScanStepSize=x;
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_BEGIN_x))			m_x1=x;
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_BEGIN_y))			m_y1=x;
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_END_x))			m_x2=x;
			if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_END_y))			m_y2=x;
	//		if (hwnd == ::GetDlgItem(m_wndDlgBar.m_hWnd, IDC_EDIT_UI_CP_FILENAME))g_Rate=x;
			
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClickedButton_scan()
{
	Scan();
}
///////////////////////////////////////////////////////////////////////////////
RECT rcOldClip;
///////////////////////////////////////////////////////////////////////////////
void CALLBACK
ScanTimer(HWND hwnd, UINT uiMsg, UINT_PTR idEvent, DWORD dwTime)
{
	const double scan_duration = 10;//10 sec
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CGLView* pGLView = theApp.GetGLView();

	if (pFrame->m_TimerScan==idEvent && pGLView->GetSafeHwnd() == ::GetFocus())
	{
		double& t = pFrame->m_fScanCurveParameter;
		if (t < 0.0 || t > 1.0)
		{
			KillTimer(pFrame->GetSafeHwnd(), pFrame->m_TimerScan);
//			ReleaseCapture();
//			ClipCursor(0);
		//	ClipCursor(&rcOldClip);
		//	pFrame->SetFocus();
		}
		else
		{
			t += pFrame->m_fScanStepSize;//(double(m_uMillisecondsBetweenScanSteps)/1000.)/scan_duration;
			Vector2D p1(pFrame->m_x1, pFrame->m_y1);
			Vector2D p2(pFrame->m_x2, pFrame->m_y2);
			Vector2D Rel = p2 - p1;
			Vector2D p = p1 + t*Rel;
			double z = 0;
		//	pGLView->ComputeScreenCoords(&p.x, &p.y, &z);
			pGLView->ComputeScreenCoords(p.x, p.y, z, false);
			POINT Pt;
			Pt.x = int(p.x);
			Pt.y = int(p.y);
			pGLView->ClientToScreen(&Pt);
			SetCursorPos( Pt.x, Pt.y);
		}
	}
	else
	{
			KillTimer(pFrame->GetSafeHwnd(), pFrame->m_TimerScan);
//			ReleaseCapture();
//			ClipCursor(0);
		//	ClipCursor(&rcOldClip);
		//	pFrame->SetFocus();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CMainFrame::Scan()
{
//	CGLView* pGLView = reinterpret_cast<CGLView*>(CWnd::FromHandle(hWndCtl));
	CGLView* pGLView = GetGLView();//theApp.GetGLView();

	m_fScanCurveParameter			= 0;
	if (m_uMillisecondsBetweenScanSteps == 0 ||
		m_fScanCurveParameter > 1.0)
	{
		::KillTimer(GetSafeHwnd(), m_TimerScan);
//		ClipCursor(0);
		return S_OK;
	}
	m_fScanDistance = sqrt((m_x2-m_x1)*(m_x2-m_x1)+(m_y2-m_y1)*(m_y2-m_y1));
	if (m_fScanDistance < 1.0e-4) return S_OK;


	RECT rc;
	pGLView->GetWindowRect(&rc);
//	GetClientRect(rc);
//	GetClipCursor(&rcOldClip);
//	ClipCursor(&rc);
	Vector2D p(m_x1, m_y1);
	double z=0;
//	pGLView->ComputeScreenCoords(&p.x, &p.y, &z);
	pGLView->ComputeScreenCoords(p.x, p.y, z, false);
	POINT pt;
	pt.x = int(p.x);
	pt.y = int(p.y);
	pGLView->SetFocus();
//	pGLView->SetCapture();
	pGLView->ClientToScreen(&pt);
	SetCursorPos( pt.x, pt.y);

	m_TimerScan = pGLView->SetTimer( m_TimerScan, m_uMillisecondsBetweenScanSteps, ScanTimer);

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#if 0
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	ASSERT(pContext	!= NULL);
	if(pContext	== NULL)		return FALSE;

	BOOL bRet = FALSE;
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif
///////////////////////////////////////////////////////////////////////////////
	CDocument *pDoc=NULL;
	POSITION pos;
	//create the source view for this document at this time
	CRuntimeClass *prc = RUNTIME_CLASS(CGLView);
	ASSERT(prc);
	m_pCGLView = (CGLView*)prc->CreateObject();
	if(m_pCGLView)
		VERIFY(m_pCGLView->CView::Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, NULL));

/*
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);
*/

	//create the doc-template view and save off a pointer to it
	bRet = CFrameWnd::OnCreateClient(lpcs, pContext);
	pDoc = pContext->m_pCurrentDoc;
	if(bRet && pDoc)
	{
		pos = pDoc->GetFirstViewPosition();
		pDoc->AddView(m_pCGLView);
	}
	else
		return FALSE;

	CView *pView = pDoc->GetNextView(pos);
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CSolveItView)));
	if(pView)
	{
		m_pCSolveItView = (CSolveItView*) pView;
	}
	else
		return FALSE;
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif
///////////////////////////////////////////////////////////////////////////////

	return bRet;
	
//	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}
#endif
///////////////////////////////////////////////////////////////////////////////

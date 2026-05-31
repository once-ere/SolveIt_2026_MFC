// SolveItView.cpp : implementation of the CSolveItView class
//

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"

#include "SolveItDoc.h"
//#include "CntrItem.h"
#include "SolveItView.h"
#include "MainFrm.h"
#include "GLView.h"
#include "Matherror.h"
#include "StackMachine.h"
#include "TextBoxWindow.h"
#include "LeftView.h"
#include "Cruncher.h"
#include "resource.h"
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"
//#include "GenExamp.h"
#include "System.h"
#include "QM/QMStationary1D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolveItView

IMPLEMENT_DYNCREATE(CSolveItView, CFormView)


//OnUpdateTextBoxWindow

BEGIN_MESSAGE_MAP(CSolveItView, CFormView)
//{{AFX_MSG_MAP(CSolveItView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_REGISTERED_MESSAGE(WM_USER_TextBoxWindow, DoWriteCommand)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedClip)
// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)

	ON_COMMAND(ID_POPUP_Spline, OnSpline)
	ON_COMMAND(ID_POPUP_Test_Plot, OnTestPlot)
	ON_COMMAND(ID_POPUP_Test_Recursion, OnTestRecursion)
//	ON_COMMAND(ID_POPUP_Test_, OnTest)

	ON_COMMAND(ID_POPUP_VECTORALGEBRA_SUM, OnSum)
	ON_COMMAND(ID_POPUP_VECTORALGEBRA_DIFFERENCE, OnDifference)
	ON_COMMAND(ID_POPUP_VECTORALGEBRA_CROSSPRODUCT, OnCrossProduct)

	ON_COMMAND(ID_POPUP_MECHANICS_MULTIBODY_TEST, OnMultiBodyTest)
	ON_COMMAND(ID_POPUP_MECHANICS_MULTIBODY_CHAIN, OnMultiBodyChain)
	ON_COMMAND(ID_POPUP_MECHANICS_MULTIBODY_SLIDER, OnMultiBodySlider)
	ON_COMMAND(ID_POPUP_MECHANICS_MultiMassSpring, OnMultiMassSpring)

	ON_COMMAND(ID_POPUP_OSCILLATORS_SIMPLEPENDULUM, OnSimplePendulum)
	ON_COMMAND(ID_POPUP_OSCILLATORS_SIMPLEHARMONIC, OnSimpleHarmonicOscillator)
	ON_COMMAND(ID_POPUP_ELECTROMAGNETIC_QINUNIFORMCHARGEDSLAB, OnSimpleHarmonicOscillator)
	ON_COMMAND(ID_POPUP_OSCILLATORS_RESONANTSIMPLEHARMONIC, OnResonantSimpleHarmonicOscillator)
	ON_COMMAND(ID_POPUP_OSCILLATORS_NONLINEARCUBIC, OnCubicAnharmonicOscillator)
	ON_COMMAND(ID_POPUP_OSCILLATORS_ANHARMONIC, OnAnharmonicOscillator)
	ON_COMMAND(ID_POPUP_MECHANICS_TEST, OnTest)
	ON_COMMAND(ID_POPUP_MECHANICS_LENNARDJONESPOTENTIAL, OnLennardJones)
	ON_COMMAND(ID_POPUP_MECHANICS_LNNRDJNSPTNTLHEATEDCOOLED, OnLennardJonesHeatedCooled)
	ON_COMMAND(ID_POPUP_MECHANICS_RAMP, OnRamp)
	ON_COMMAND(ID_POPUP_MECHANICS_PULLEY, OnPulley)
	ON_COMMAND(ID_POPUP_MECHANICS_TWOBODY, OnTwoBodyScattering)
	ON_COMMAND(ID_POPUP_MECHANICS_THREEBODY, OnThreeBodyCollision)
	ON_COMMAND(ID_POPUP_MECHANICS_FOURBODYCOLLISION, OnFourBodyCollision)
	ON_COMMAND(ID_POPUP_MECHANICS_FOURBODYCOLLISION2, OnFourBodyCollision2)
	ON_COMMAND(ID_POPUP_MECHANICS_BALLONSHPERE, OnBallOnShpere)
	ON_COMMAND(ID_POPUP_MECHANICS_SPHEREANDBLOCK, OnSphereAndBlock)
	ON_COMMAND(ID_POPUP_MECHANICS_PULLEYANDTWOBLOCKS, OnTwoBlocksAndPulley)
	ON_COMMAND(ID_POPUP_MECHANICS_BLOCK_AND_PULLEY, OnBlockAndPulley)
	ON_COMMAND(ID_POPUP_MECHANICS_BLOCKANDTACKLE, OnBlockAndTackle)
	ON_COMMAND(ID_POPUP_MECHANICS_INELASTICCOLLISION, OnInelasticCollision)
	ON_COMMAND(ID_POPUP_MECHANICS_INELASTICPENDULUMCOLLISION, OnInelasticPendulumCollision)
	ON_COMMAND(ID_POPUP_MECHANICS_NEWTONSCRADLE, OnNewtonCradle)
	ON_COMMAND(ID_POPUP_MECHANICS_NEWTONSCRADLE2, OnNewtonCradle2)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_XCOMPONENT, OnUserDefinedTimeIndependentX)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_YCOMPONENT, OnUserDefinedTimeIndependentY)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_ZCOMPONENT, OnUserDefinedTimeIndependentZ)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_XCOMPONENT, OnUserDefinedTimeDependentX)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_YCOMPONENT, OnUserDefinedTimeDependentY)
	ON_COMMAND(ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_ZCOMPONENT, OnUserDefinedTimeDependentZ)

	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN1SPATIALDIMENSION,  Scattering1Dim)
	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN2SPATIALDIMENSION,  Scattering2Dim)

	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_Ground_State__IN_1_SPATIALDIMENSION,  QM_Ground_State_1Dim)
	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_Ground_State__BEC,  QM_Ground_State_2DimBEC)


	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_1_SPATIALDIMENSION,  QMStationary1Dim)
	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_1_SPATIALDIMENSIONB,  QMStationary1DimB)
	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_2_SPATIALDIMENSION,  QMStationary2Dim)
	ON_COMMAND(ID_POPUP_QUANTUMMECHANICS_SUPERPOSITION, Superposition)
	ON_COMMAND(ID_POPUP_STATISTICALMECHANICS_NSPHERES, OnN_Body)
	ON_COMMAND(ID_POPUP_GRAVITATION_ORBITALMECHANICS, OnOrbitalMechanics)
	ON_COMMAND(ID_POPUP_GRAVITATION_SUNEARTHMOON, OnSunEarthMoon)
	ON_COMMAND(ID_POPUP_GRAVITATION_EARTHMOON, OnEarthMoon)
	ON_COMMAND(ID_POPUP_PROJECTILE_CANNONBALL, OnCannonBall)
	ON_COMMAND(ID_POPUP_PROJECTILE_TWOBODY, OnTwoBody)
	ON_COMMAND(ID_POPUP_PROJECTILE_THREEBODY, OnThreeBody)
	ON_COMMAND(ID_POPUP_ELECTROMAGNETIC_THREEBODY, OnThreeChargedBodies)
	ON_COMMAND(ID_POPUP_ELECTROMAGNETIC_ELECTROSTATICFIELD, OnElectroStaticField)
	ON_COMMAND(ID_POPUP_OSCILLATORS_CYCLOTRON, OnCyclotron)
	ON_COMMAND(ID_POPUP_ELECTROMAGNETIC_CURRENTLOOP, OnCurrentLoop)
	ON_COMMAND(ID_POPUP_CHAOTIC_VERHULSTDYNAMICS, OnVerhulstDynamics)
	ON_COMMAND(ID_POPUP_CHAOTIC_6BODY, On6Body)
	ON_COMMAND(ID_POPUP_CHAOTIC_CHAIN, OnChain)
	ON_COMMAND(ID_POPUP_CHAOTIC_RANDOMCHAIN, OnRandomChain)
	ON_COMMAND(ID_POPUP_CHAOTIC_COMPOUNDPENDULUM, OnCompoundPendulum)
	ON_COMMAND(ID_POPUP_CHAOTIC_PHYSICAL_PENDULUM, OnPhysicalPendulum)
	ON_COMMAND(ID_POPUP_CHAOTIC_TRAPPING, OnTrapping)
	ON_COMMAND(ID_POPUP_CHAOTIC_TWOBODY, OnTwoNonShpericalBody)
	ON_COMMAND(ID_POPUP_CHAOTIC_PENDULUMANDFIXEDCHARGE, OnPendulumAndFixedCharge)
	ON_COMMAND(ID_POPUP_CHAOTIC_PENDULUMANDPLANARCHARGE, OnPendulumAndPlanarCharge)
	ON_COMMAND(ID_POPUP_CHAOTIC_SUBMERGED, OnSubmerged)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM214, Problem2_14)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM237, Problem2_37)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_EXAMPLE24, Example2_4)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM346, Problem3_46)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM351, Problem3_51)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM360, Problem3_60)
	ON_COMMAND(ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_CHAPTER_7_EXAMPLE78, Example7_8)

END_MESSAGE_MAP()
//	ON_COMMAND(WM_USER+ID_TextBoxWindow, OnUpdateTextBoxWindow)
//	ON_UPDATE_COMMAND_UI(WM_USER+ID_TextBoxWindow, OnUpdateTextBoxWindow)
#if 0
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_POPUP_COPY, OnPopupCopy)
	ON_COMMAND(ID_POPUP_CUT, OnPopupCut)
	ON_COMMAND(ID_POPUP_CLEAR, OnPopupDelete)
	ON_COMMAND(ID_POPUP_PASTE, OnPopupPaste)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY, OnUpdatePopupCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PASTE, OnUpdatePopupPaste)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CUT, OnUpdatePopupCopy)
#endif
/////////////////////////////////////////////////////////////////////////////
// CSolveItView construction/destruction

CSolveItView::CSolveItView():
	CFormView(CSolveItView::IDD),
	m_entryNumber(1),
	m_Height(0),
	m_Width(2048),
	m_lastLineNumber(0),
//	m_pRichEditOle(0),
//	m_pCGenExamp(0),
	m_pCTextBoxWindow(0)
{
	CMainFrame::theEditView	= this;
}

CSolveItView::~CSolveItView()
{
}

BOOL CSolveItView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: Modify the Window class or styles here by modifying
//	the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
#if 0
// Get the Controller Interface from the RichEdit Control
	if (m_pRichEditOle == NULL)
		m_pRichEditOle = GetRichEditCtrl().GetIRichEditOle();
	ASSERT(m_pRichEditOle != NULL);
// Unregister this edit control as a potential target for OLE drag-and-drop
	::RevokeDragDrop(m_hWnd);
// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	CreateTextBoxWindow();

#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 10
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
	CSolveItDoc* pDoc = GetDocument();
	ASSERT(pDoc	!= NULL);
	if(pDoc	== NULL) return;


///////////////////////////////////////////////////////////////////////////////
#if 0
//create the source view for this document at this time
	CRuntimeClass *prc = RUNTIME_CLASS(CGLView);
	ASSERT(prc);
	CGLView* pCGLView = (CGLView*)prc->CreateObject();
	if(pCGLView)
		VERIFY(pCGLView->CView::Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, NULL));
	pDoc->AddView(pCGLView);
#endif
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#if 10
	CMDIChildWnd* pNewFrame = (CMDIChildWnd*)(theApp.m_pGLViewTemplate->CreateNewFrame(pDoc, NULL));

	if (pNewFrame )
	{
		ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
		theApp.m_pGLViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	}
#endif
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#if 10
	POSITION pos = pDoc->GetFirstViewPosition();

	while (pos)
	{
		CView *pView = pDoc->GetNextView(pos);
		if(pView)
		{
			if (pView->IsKindOf(RUNTIME_CLASS(CLeftView)))
			{
				ASSERT(CMainFrame::theTreeView == (CLeftView*) pView);
				CMainFrame::theTreeView = (CLeftView*) pView;
			}
			if (pView->IsKindOf(RUNTIME_CLASS(CSolveItView)))
			{
				ASSERT(CMainFrame::theEditView == (CSolveItView*) pView);
				CMainFrame::theEditView = (CSolveItView*) pView;
			}
			if (pView->IsKindOf(RUNTIME_CLASS(CGLView)))
			{
				ASSERT(CMainFrame::theGLView == (CGLView*) pView);
				CMainFrame::theGLView = (CGLView*) pView;
			}
		}
	}
#endif
///////////////////////////////////////////////////////////////////////////////

#endif
///////////////////////////////////////////////////////////////////////////////


	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PositionWindow();
	CWindowDC dc(NULL);
	dc.SetMapMode(MM_TEXT);
	GetParentFrame()->RecalcLayout();
	CRect rectTemplate;
	GetWindowRect(rectTemplate);
	SetScrollSizes(MM_TEXT, rectTemplate.Size());
	CreateTextBoxWindow();
/*
	CLeftView*	tree = pFrame->GetTreeView();
	if (tree != 0)
	{
		tree->PopulateTree();
		tree->UpdateTree();
	}
*/
//	m_pCTextBoxWindow = new CTextBoxWindow(this);
//	PostMessage( WM_USER_TextBoxWindow, 0, 0);
///////////////////////////////////////////////////////////////////////////////
//const char lpClipboardFormat[] = TEXT("{FF033C60-8F68-4a1b-9F64-178B4C68F3A8}");
//const UINT WM_USER_SetPerspective = RegisterWindowMessage(lpWM_USER_SetPerspective);
//	UINT uiCustomDataFormat = RegisterClipboardFormat(lpClipboardFormat);
///////////////////////////////////////////////////////////////////////////////

	CInfoDialog* pIntroDialog = new CInfoDialog(_T("intro.htm"), _T("Introduction"));
//	IntroDialog_hWnd  = pIntroDialog->m_hWnd;
	pFrame->m_CleanupOnInitialize.Add(pIntroDialog->m_hWnd);//







	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));


	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();	
	gl->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
	gl->m_bPerspectiveTransformation=false;
	gl->SetPerspectiveOrFrustum();


}
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
LRESULT CSolveItView::DoWriteCommand(WPARAM wParam, LPARAM lParam)
{
	try {
	SetFocus();
	_bstr_t bstr((LPSTR)lParam);
	GlobalFree((void*)lParam);
	WriteCommand(bstr);

}
	catch (char * str)
	{
		AfxMessageBox(GetWC(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::DoWriteCommand");
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CSolveItView::OnUpdateTextBoxWindow(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CSolveItView printing

BOOL CSolveItView::OnPreparePrinting(CPrintInfo* pInfo)
{
// default preparation
	return DoPreparePrinting(pInfo);
}


void CSolveItView::OnDestroy()
{
//	StopGenExamp();
	PostQuitMessage(0);
#if 0
	m_pRichEditOle->Release();
// Deactivate the item on destruction; this is important
// when a splitter view is being used.
	CFormView::OnDestroy();
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
			pActiveItem->Deactivate();
			ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CSolveItView diagnostics

#ifdef _DEBUG
void CSolveItView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSolveItView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSolveItDoc* CSolveItView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSolveItDoc)));
	return (CSolveItDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSolveItView message handlers

void CSolveItView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	static char buf[64];
	CFormView::OnLButtonDown(nFlags, point);
#if 0
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;

	const MSG& aMSG = *GetCurrentMessage();
	POINT pt;
	pt.x = aMSG.pt.x;
	pt.y = aMSG.pt.y;
	ScreenToClient(&pt);
	{
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	const int &screen_x = pt.x;
	const int &screen_y = pt.y;
	oss	<< screen_x << " * " << screen_y << ends;
	string str(buf);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetStatusBarText(str, 0);
	}
#endif
#if 0
	CRichEditCtrl& m_ctlEdit = GetRichEditCtrl();
	m_ptCaret = GetCaretPos();
	{
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	const int screen_x = (m_ptCaret.x-9)/8;
	const int screen_y = m_ptCaret.y/16;
	oss	<< m_ptCaret.x << " * " << m_ptCaret.y << " ][ "	<< screen_x << " * " << screen_y << ends;
	string str(buf);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetStatusBarText(str, 3);
	}
//Rich edit controls:aMSG.aMSG.&&
	POINTL ptL;
	ptL.x = pt.x;
	ptL.y = pt.y;
	LRESULT character_index_nearest_character = 
		m_ctlEdit.SendMessage(EM_CHARFROMPOS, 0, (LPARAM) &ptL);
	int nLine =  m_ctlEdit.LineIndex( character_index_nearest_character);
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	oss << nLine << ends;
	string str(buf);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetStatusBarText(str, 0);
#endif
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CFormView::OnMouseMove(nFlags, point);
#if 0
	static char buf[128];
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	const int &screen_x = point.x;
	const int &screen_y = point.y;
	oss	<< screen_x << " * " << screen_y << ends;
	string str(buf);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetStatusBarText(str, 2);
#endif
}
///////////////////////////////////////////////////////////////////////////////
bool CSolveItView::OnCompile2(const char* buf)//auto_ptr<char>& abuf)
{
	try {
		bool bParseCompleted = false;
//		char* buf = abuf.get();
		if (buf[0] == '\"')
		{
			string out(buf+1);
			CreateOutputWindow(ToWideString(out.c_str()));
			return bParseCompleted;
		}
		const int  ch = ';';
//		char* pdest = strrchr( buf, ch );
		const char* pdest = strrchr(buf, ch);
		if (pdest == NULL )
		{
			return false;
		}
		string s(buf);
		std::string::size_type pos = 0;
		const string CrLf("\r\n");
		const string r("\n");
		s += r;
//	replace (s.begin( ), s.end( ), CrLf , r);
// find all occurences of 'CrLf' and replace them by 'r'
		while ( ( pos = s.find( CrLf, pos ) ) != std::string::npos ) {
			s.replace( pos, CrLf.length(), r );
			pos += r.length(); // new starting point of search is just after 'b'
		}
	
		string out;
		CStackMachine* stackMachine	= theApp.GetStackMachine();
		bParseCompleted = stackMachine->Compile_and_Run_and_check_for_reset(s, out);
		if (out.size() > 0) CreateOutputWindow(ToWideString(out.c_str()));
		return bParseCompleted;
	}

#define ERR_RET(x)			string brebuf(x);\
							brebuf+="\r\n";\
		if (!CreateOutputWindow(ToWideString(brebuf.c_str()))) return false;

	catch (std::runtime_error* exc)
	{
	//	AfxMessageBox(exc->what());
		ERR_RET(exc->what())
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
	//	AfxMessageBox(except->name);
		ERR_RET(except->name)
	}
	catch (char * str)
	{
	//	AfxMessageBox(str);
		ERR_RET(str)
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	//	AfxMessageBox("CSolveItView::OnCompile: Unknown error.");
		ERR_RET("CSolveItView::OnCompile: Unknown error.")
	}
#undef ERR_RET
	return false;
}
///////////////////////////////////////////////////////////////////////////////
const char* CSolveItView::OnCompile(const char* buf)
{
	try {
	bool bParseCompleted = false;
	string s(buf);
	std::string::size_type pos = 0;
	const string CrLf("\r\n");
	const string r("\n");
	s += ";";
	s += r;//

//	replace (s.begin( ), s.end( ), CrLf , r);
// find all occurences of 'CrLf' and replace them by 'r'
	while ( ( pos = s.find( CrLf, pos ) ) != std::string::npos ) {
		s.replace( pos, CrLf.length(), r );
		pos += r.length(); // new starting point of search is just after 'b'
	}

	string out;
		CStackMachine* stackMachine	= theApp.GetStackMachine();
		bParseCompleted = stackMachine->Compile_and_Run_and_check_for_reset(s, out);
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	CSystem*		m_system		= pFrame->m_system;
//	CStackMachine*	stackMachine	= *m_system->stackMachine;

//	CSolveItApp::g_boolDoReset	= false;
//	bParseCompleted = stackMachine->Compile_and_Run(s, out);
//	if (CSolveItApp::g_boolDoReset) m_system->Initialize();

	if (out.size() == 0) out = "0";
	return _strdup(out.c_str());
	}

#define ERR_RET(x)			string brebuf(x);\
							brebuf+="\r\n";\
		if (!AfxMessageBox(GetWC(brebuf))) return 0;

	catch (std::runtime_error* exc)
	{
	//	AfxMessageBox(exc->what());
		ERR_RET(exc->what())
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
	//	AfxMessageBox(except->name);
		ERR_RET(except->name)
	}
	catch (char * str)
	{
	//	AfxMessageBox(str);
		ERR_RET(str)
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	//	AfxMessageBox("CSolveItView::OnCompile: Unknown error.");
		ERR_RET("CSolveItView::OnCompile: Unknown error.")
	}
#undef ERR_RET
	return 0;
}
///////////////////////////////////////////////////////////////, int _nCrLf////////////////
BOOL CSolveItView::ClipCopy(const string& out)
{
	size_t count = out.size();
	if (count && OpenClipboard())
	{
		EmptyClipboard();//GMEM_DDESHARE
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, (count + 1) * sizeof(char));
		LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hClipboardData);
		memcpy(lptstrCopy, &out.c_str()[0], count * sizeof(char));
		lptstrCopy[count] = (char) 0;    // null character 
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HBRUSH CSolveItView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CRect rcParent, rcTemp;
//	GetWindowRect(&rcParent);
	GetClientRect(&rcParent);
//Get document size. Assume mapping mode is MM_TEXT and ViewPort
//and Window origins have not been set so we can also assume LP==DP.
	CSize size = GetTotalSize();
// Determine if scroll bars are visible
// and subtract their size from the view.
	if (size.cx > rcParent.right - rcParent.left)
			rcParent.bottom -= GetSystemMetrics(SM_CYHSCROLL);
	if (size.cy > rcParent.bottom - rcParent.top)
			rcParent.right -= GetSystemMetrics(SM_CXVSCROLL);

	CPoint pt = GetScrollPosition();

	vector<CTextBoxWindow*>::iterator j;
	for (j=vec_pTextBoxWindow.begin(); j != vec_pTextBoxWindow.end(); ++j)
	{
		CTextBoxWindow*p = *j;
		ASSERT (p!=NULL);
		CRect m_rc	= p->m_rc;
		CRect m_rcWndPrompt= p->m_rcWndPrompt;
		m_rc.OffsetRect( 0, -pt.y );
		m_rcWndPrompt.OffsetRect( 0, -pt.y );
// Succeed on&&ly if no part of the child appears in the parent.
		rcTemp.IntersectRect(m_rc, rcParent);
		if (!rcTemp.IsRectEmpty())
		{
			::MoveWindow(p->m_hWndPrompt, 0, m_rcWndPrompt.top, m_rcWndPrompt.Width(), m_rcWndPrompt.Height(), TRUE);
			p->MoveWindow(m_rc, TRUE);
		}
	}
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
//// Set the static text to white on blue.
	//	pDC->SetTextColor(RGB(255, 255, 255));
	//	pDC->SetTextColor(RGB(0, 0, 0));
	//	pDC->SetTextColor(RGB(143, 86, 68));
		pDC->SetTextColor(RGB(143/2, 86/2, 68/2));
	//	pDC->SetBkColor(RGB(0, 0, 255));
	//	pDC->SetBkColor(RGB(235, 255, 205));
	//	pDC->SetBkColor(RGB(0xAD, 0xFF, 0x2F));//greenyellow
	//	pDC->SetBkColor(RGB(173, 255, 47));
	//	pDC->SetBkColor(RGB(204, 255, 152));
	//	pDC->SetBkColor(RGB(224, 255, 172));
		pDC->SetBkColor(RGB(0xdc, 0xff, 0xcc));//
	//	pDC->SetBkColor(RGB(0x20, 0xB2, 0xAA));//lightseagreen
	//	pDC->SetBkColor(RGB(0x32, 0xCD, 0x32));//limegreen
	//	pDC->SetBkColor(RGB(0x9A, 0xCD, 0x32));//yellowgreen
//// Drop through to return the background brush.
		case CTLCOLOR_DLG:
			return (HBRUSH)GetStockObject(WHITE_BRUSH);//(COLOR_CAPTIONTEXT+1);//(HBRUSH)(m_pBkBrush.GetSafeHandle());
		default:
			return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	HBRUSH hbr = CFormView::OnCtlColor( pDC,  pWnd, nCtlColor);
	return hbr;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnDraw(CDC* pDC)
{
}
///////////////////////////////////////////////////////////////////////////////
const wchar_t szsz[]=
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
void CSolveItView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	SetFocus();
	GetParentFrame()->SetFocus();
	CWindowDC dc(NULL);
	dc.SetMapMode(MM_TEXT);
	GetParentFrame()->RecalcLayout();

	CRect rc;
	GetClientRect(rc);
//Get document size. Assume mapping mode is MM_TEXT and ViewPort
//and Window origins have not been set so we can also assume LP==DP.
	{
		CSize size = GetTotalSize();
// Determine if scroll bars are visible
// and subtract their size from the view.
		if (size.cx > rc.right - rc.left)
			rc.bottom -= GetSystemMetrics(SM_CYHSCROLL);
		if (size.cy > rc.bottom - rc.top)
			rc.right -= GetSystemMetrics(SM_CXVSCROLL);
	}
	SIZE sizePage={rc.Width(), rc.Height()};
	int cx = rc.Width();
	int cy = rc.Height();
//	cx = 2048; 
	cy = cy>m_Height? cy:m_Height;
	SIZE sizeLine={0,0};
	BOOL res = GetTextExtentPoint32(dc.m_hDC, szsz, static_cast<int>(_countof(szsz) - 1), &sizeLine);
	SIZE sizeTotal={sizeLine.cx, 4*sizeLine.cy+cy};//(int)(1.1*cy)};
//	HDC hdc = GetDC()->GetSafeHdc();
//	m_Width = sizeLine.cx = cx;
//	::ReleaseDC(GetSafeHwnd(),hdc);
	if (res) SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::SizeUpdate(int nAdditionalHeight)
{
	if (nAdditionalHeight != 0)
	{
		m_Height += nAdditionalHeight;
	}
//	GetParentFrame()->RecalcLayout();
//	CSolveItDoc* pDoc = GetDocument();
//	pDoc->UpdateAllViews(NULL);//SetModifiedFlag( TRUE );
	OnUpdate(this, 0, 0);
	Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HANDLE	gl_terminate_event=0;
void CSolveItView::OnClose() 
{
//theApp.CloseAllDocuments( TRUE );
//return;//
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->SendMessage(WM_CLOSE, 0, 0);
	if (gl_terminate_event)
	{
		SetEvent (gl_terminate_event);
		GetParentFrame()->SendMessage(WM_CLOSE, 0, 0);
theApp.CloseAllDocuments( TRUE );
		PostQuitMessage(0);//
		return;
	}
	theApp.m_pGLViewTemplate->CloseAllDocuments(true);
	CGLView* gl = pFrame->GetGLView();
	if (gl==NULL) return;
	if (!gl->m_hWnd || !::IsWindow(gl->GetSafeHwnd())) return;

	gl_terminate_event = CreateEvent(0, FALSE, FALSE, 0);
	{
		::PostMessage(gl->GetSafeHwnd(), WM_CLOSE, 0, 0);//OnClose();
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&gl_terminate_event,		//g_EventHandle The array of eventsG_EventKillThread
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
//	CMDIFrameWnd::OnClose();
theApp.CloseAllDocuments( TRUE );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
void CSolveItView::WriteBegin(BSTR strText)
{
	const _bstr_t CrLf(L"\r\n");
	_bstr_t bstr(strText);
	bstr+=CrLf;
	CreateTextBoxWindow();
	vec_pTextBoxWindow.back()->SetWindowText(bstr);
	vec_pTextBoxWindow.back()->Resize();
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::WriteAdd(BSTR strText)
{
	const _bstr_t CrLf(L"\r\n");
	_bstr_t bstr(strText);
	bstr+=CrLf;

	DWORD length = ::SendMessage(vec_pTextBoxWindow.back()->m_hWnd, WM_GETTEXTLENGTH,0,0);
	if (length <= 0) length=1;
	++length;
	length *= 2;//much padding
	char* buf = new char[length];
	ZeroMemory(buf, length*sizeof(char));
	DWORD length2 = ::GetWindowText(vec_pTextBoxWindow.back()->m_hWnd, buf, length-1);
	_bstr_t bstrw(buf);
	bstrw	+= bstr;
	vec_pTextBoxWindow.back()->SetWindowText(bstrw);
	delete[] buf;
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::WriteEnd(BSTR strText)
{
	WriteAdd(strText);
	vec_pTextBoxWindow.back()->Resize();
	int nRepCnt	= 1;
	int nFlags	= 28;
	vec_pTextBoxWindow.back()->m_bSimulateShift=1;
	vec_pTextBoxWindow.back()->m_bSimulateReturn=1;

	vec_pTextBoxWindow.back()->SendMessage(WM_CHAR, 10, MAKELONG(nRepCnt, nFlags));
}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//HOW BIZARRE CAN WINDOWS GET?
//
void CSolveItView::WriteCommand(BSTR strText)
{
	const _bstr_t CrLf(L"\r\n");
	_bstr_t bstr(strText);
	bstr+=CrLf;
	vec_pTextBoxWindow.back()->SendMessage(WM_USER_COMMAND, 0, reinterpret_cast<LPARAM> ((const char*)bstr));
	return;
//	CString sng;
//	vec_pTextBoxWindow.back()->GetWindowText(sng);
//	if (sng.GetLength() > 0) CreateTextBoxWindow();
	CreateTextBoxWindow();
	vec_pTextBoxWindow.back()->WriteLine(bstr, 1);
	return;
//	SetFocus();
	vec_pTextBoxWindow.back()->SetFocus();
//	SetCapture();
////	ReleaseCapture();
//	vec_pTextBoxWindow.back()->SetCapture();
//	vec_pTextBoxWindow.back()->SetWindowText(bstr);
	vec_pTextBoxWindow.back()->SetFocus();
//	vec_pTextBoxWindow.back()->SetCapture();
	int nRepCnt	= 1;
	int nFlags	= 28;
	vec_pTextBoxWindow.back()->m_bSimulateShift=1;
	vec_pTextBoxWindow.back()->m_bSimulateReturn=1;

//	vec_pTextBoxWindow.back()->SendMessage(WM_CHAR, 10, MAKELONG(nRepCnt, nFlags));
//	vec_pTextBoxWindow.back()->OnChar(10, nRepCnt, nFlags);
	vec_pTextBoxWindow.back()->WriteLine(bstr, 1);
//	vec_pTextBoxWindow.back()->Resize();
//	ReleaseCapture();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnRButtonDown(UINT nFlags, CPoint point) 
{
try {
	OnContextMenu(CWnd::FromHandle(m_hWnd), point);
//	CRichEditView::OnRButtonDown(nFlags, point);
}
	catch (char * str)
	{
		AfxMessageBox(GetWC(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnRButtonDown");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnContextMenu(CWnd* pWnd, CPoint pt) 
{
try {
//	ASSERT(m_system != NULL);

	ASSERT(pWnd->m_hWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));
	if (pWnd->m_hWnd == NULL) throw SolveIt_ERROR("CSolveItView::OnContextMenu: pWnd->m_hWnd == 0");
	if (!::IsWindow(pWnd->m_hWnd)) throw SolveIt_ERROR("CSolveItView::OnContextMenu: !::IsWindow(pWnd->m_hWnd)");


	CSystem* m_pCSystem	= theApp.GetSystem();
	if (m_pCSystem!=NULL && m_pCSystem->m_pCQMStationary1D!=NULL)
	{
	//	m_pCSystem->StopQMStationary1D();
		m_pCSystem->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, WM_CLOSE);
	}
//	if (m_pCSystem->m_pCQMStationary1D)	m_pCSystem->m_pCQMStationary1D->PostThreadMessage(WM_QUIT, 0, 0);

	::ClientToScreen(pWnd->m_hWnd, &pt);
	HMENU hmenu;            // top-level menu 
	HMENU hmenuTrackPopup;  // shortcut menu 
// Load the menu resource. 
	if ((hmenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_EXAMPLES_MENU))) == NULL) return; 
	if (hmenu == NULL) throw SolveIt_ERROR("CSolveItView::OnContextMenu: hmenu == 0");
	if (!::IsMenu(hmenu)) throw SolveIt_ERROR("CSolveItView::OnContextMenu: !::IsMenu(hmenu)");

	// TrackPopupMenu cannot display the menu bar so get a handle to the first shortcut menu. 
	hmenuTrackPopup = GetSubMenu(hmenu, 0);
	if (hmenuTrackPopup == NULL) throw SolveIt_ERROR("CSolveItView::OnContextMenu: hmenuTrackPopup == 0");
	if (!::IsMenu(hmenuTrackPopup)) throw SolveIt_ERROR("CSolveItView::OnContextMenu: !::IsMenu(hmenuTrackPopup)");

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
	{
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		if (pFrame == NULL) throw SolveIt_ERROR("CSolveItView::OnContextMenu: pFrame == 0");
		if (pFrame->m_system == NULL) throw SolveIt_ERROR("CSolveItView::OnContextMenu: pFrame->m_system == 0");
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
		bEnable = !( (ExitCode == STILL_ACTIVE) || (pFrame->m_system->m_bStopIterations==true || pFrame->m_system->m_bSingleStepping==true ));
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
					pt.x, pt.y, 0, pWnd->m_hWnd, NULL); 
 
// Destroy the menu. 
	DestroyMenu(hmenu); 

}
	catch (char * str)
	{
		AfxMessageBox(GetWC(str));
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnContextMenu");
	}
} 
///////////////////////////////////////////////////////////////////////////////
bool CSolveItView::OnUpdateNeedSel()
{
	long starting_position	= 0;
	long ending_position	= 0;
	vec_pTextBoxWindow.back()->SendMessage(EM_GETSEL, reinterpret_cast<WPARAM> (&starting_position), reinterpret_cast<LPARAM> (&ending_position) );
	return( starting_position != ending_position );
} 
///////////////////////////////////////////////////////////////////////////////
BOOL CSolveItView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_COMMAND)
	{
		{
			CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		if (pFrame->m_system->m_bStopIterations==false)
		{
			return CFormView::PreTranslateMessage(pMsg);
		}
			DWORD ExitCode=0;
			if (pFrame->m_system->worker == NULL ) ExitCode = TRUE;// FALSE;
			else
			{
				Lock lock (theApp.protectWorker);
				BOOL ret = GetExitCodeThread(pFrame->m_system->worker->m_hThread, &ExitCode);
				if (ret == 0)
				{
				//	DWORD err = GetLastError();
				//	_variant_t v = long(err);
				//	_bstr_t b(v);
		LPVOID lpMsgBuf= DisplayErrorText();
		_bstr_t b((const char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
					throw SolveIt_ERROR((const char *) b);
				}
				if (ExitCode == STILL_ACTIVE)
				{
					ExitCode = FALSE;
					SetEvent(g_EventKill);
					ExitCode = TRUE;
				}
			}
			bool bEnable = ( ExitCode && (pFrame->m_system->m_bStopIterations==true || pFrame->m_system->m_bSingleStepping==true ));
			if (!bEnable) return CFormView::PreTranslateMessage(pMsg);
		}
		switch (pMsg->wParam)
		{
			case ID_POPUP_VECTORALGEBRA_SUM:
			case ID_POPUP_VECTORALGEBRA_DIFFERENCE:
			case ID_POPUP_VECTORALGEBRA_CROSSPRODUCT:
			case ID_POPUP_MECHANICS_MULTIBODY_TEST:
			case ID_POPUP_MECHANICS_MULTIBODY_CHAIN:
			case ID_POPUP_MECHANICS_MULTIBODY_SLIDER:
			case ID_POPUP_MECHANICS_MultiMassSpring:

			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_XCOMPONENT :
			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_XCOMPONENT :

			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_YCOMPONENT :
			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_YCOMPONENT :

			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEINDEPENDENT_ZCOMPONENT :
			case ID_POPUP_MECHANICS_USERDEFINEDFORCES_TIMEDEPENDENT_ZCOMPONENT :

			case ID_POPUP_MECHANICS_RAMP         :
			case ID_POPUP_MECHANICS_TWOBODY      :
			case ID_POPUP_MECHANICS_BALLONSHPERE :
			case ID_POPUP_MECHANICS_PULLEY       :
			case ID_POPUP_MECHANICS_SPHEREANDBLOCK :
			case ID_POPUP_MECHANICS_PULLEYANDTWOBLOCKS :
			case ID_POPUP_MECHANICS_BLOCK_AND_PULLEY :
			case ID_POPUP_MECHANICS_BLOCKANDTACKLE :
			case ID_POPUP_MECHANICS_TEST         :
			case ID_POPUP_MECHANICS_FOURBODYCOLLISION :
			case ID_POPUP_MECHANICS_FOURBODYCOLLISION2 :
			case ID_POPUP_MECHANICS_LENNARDJONESPOTENTIAL :
			case ID_POPUP_MECHANICS_LNNRDJNSPTNTLHEATEDCOOLED :
			case ID_POPUP_MECHANICS_INELASTICCOLLISION :
			case ID_POPUP_MECHANICS_INELASTICPENDULUMCOLLISION :
			case ID_POPUP_MECHANICS_NEWTONSCRADLE :
			case ID_POPUP_MECHANICS_NEWTONSCRADLE2 :
			case ID_POPUP_MECHANICS_THREEBODY    :

			case ID_POPUP_OSCILLATORS            :
			case ID_POPUP_OSCILLATORS_SIMPLEHARMONIC :
			case ID_POPUP_OSCILLATORS_SIMPLEPENDULUM :
			case ID_POPUP_OSCILLATORS_RESONANTSIMPLEHARMONIC :
			case ID_POPUP_OSCILLATORS_NONLINEARCUBIC :
			case ID_POPUP_OSCILLATORS_ANHARMONIC :
			case ID_POPUP_OSCILLATORS_CYCLOTRON  :
			case ID_POPUP_PROJECTILE_CANNONBALL  :
			case ID_POPUP_PROJECTILE_THREEBODY   :
			case ID_POPUP_PROJECTILE_TWOBODY     :
			case ID_POPUP_GRAVITATION_ORBITALMECHANICS :
			case ID_POPUP_GRAVITATION_SUNEARTHMOON :
			case ID_POPUP_GRAVITATION_EARTHMOON  :
			case ID_POPUP_ELECTROMAGNETIC_CYCLOTRON :
			case ID_POPUP_ELECTROMAGNETIC_QINUNIFORMCHARGEDSLAB :
			case ID_POPUP_ELECTROMAGNETIC_CURRENTLOOP :
			case ID_POPUP_ELECTROMAGNETIC_THREEBODY :
			case ID_POPUP_ELECTROMAGNETIC_ELECTROSTATICFIELD :
			case ID_POPUP_CHAOTIC_TWOBODY        :
			case ID_POPUP_CHAOTIC_6BODY          :
			case ID_POPUP_CHAOTIC_TRAPPING       :
			case ID_POPUP_CHAOTIC_COMPOUNDPENDULUM :
			case ID_POPUP_CHAOTIC_PHYSICAL_PENDULUM :
			case ID_POPUP_CHAOTIC_CHAIN          :
			case ID_POPUP_CHAOTIC_PENDULUMANDFIXEDCHARGE :
			case ID_POPUP_CHAOTIC_PENDULUMANDPLANARCHARGE :
			case ID_POPUP_CHAOTIC_VERHULSTDYNAMICS :
			case ID_POPUP_CHAOTIC_RANDOMCHAIN    :
			case ID_POPUP_CHAOTIC_SUBMERGED      :
			case ID_POPUP_STATISTICALMECHANICS_NSPHERES :


			case ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN2SPATIALDIMENSION :
			case ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN1SPATIALDIMENSION :
			case ID_POPUP_QUANTUMMECHANICS_SUPERPOSITION :
			case ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_1_SPATIALDIMENSION:
			case ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_2_SPATIALDIMENSION:
			case ID_POPUP_QUANTUMMECHANICS_Ground_State__IN_1_SPATIALDIMENSION :
			case ID_POPUP_QUANTUMMECHANICS_Ground_State__BEC :





			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM214 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_EXAMPLE24 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM237 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_CHAPTER_4 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_CHAPTER_5 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_CHAPTER_6 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM346 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM351 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_PROBLEM360 :
			case ID_POPUP_HOMEWORK_COLLEGEPHYSICSSERWAYFAUGHN_CHAPTER_7_EXAMPLE78 :

				SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////
#if 0
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::CreateGenExampThread(bool bCREATE_SUSPENDED)
{
	if (m_pCGenExamp)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCGenExamp->m_hThread, &ExitCode);
	if (ret == 0)
	{
		DWORD err = GetLastError();
		CloseHandle (m_pCGenExamp->m_hThread);
		m_pCGenExamp	= 0;
		_variant_t v = long(err);
		_bstr_t b(v);
		throw SolveIt_ERROR((const char *) b);
	}
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCGenExamp->ResumeThread();
//				m_pCGenExamp->PostThreadMessage(WM_USER_START_PASS, 0, 0);
			return;
		}
		else
		{
			CloseHandle (m_pCGenExamp->m_hThread);
			m_pCGenExamp	= 0;
		}
	}
	try {
/*
			if (bCREATE_SUSPENDED) 
			m_pCGenExamp =  
					(CGenExamp*)AfxBeginThread( RUNTIME_CLASS( CGenExamp ),
					THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
*/
			m_pCGenExamp =  (CGenExamp*)AfxBeginThread( RUNTIME_CLASS( CGenExamp ));
		}
		catch (char * str)
		{
			AfxMessageBox(str);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSolveItView::StopGenExamp(void)
{
try {
	if (m_pCGenExamp == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCGenExamp->m_hThread, &ExitCode);
	if (ret == 0)
	{
	//	DWORD err = GetLastError();
		CloseHandle (m_pCGenExamp->m_hThread);
		m_pCGenExamp	= 0;
	//	_variant_t v = long(err);
	//	_bstr_t b(v);
		LPVOID lpMsgBuf= DisplayErrorText();
		_bstr_t b((const char*)lpMsgBuf);
		LocalFree( lpMsgBuf );
		throw SolveIt_ERROR((const char *) b);
		return TRUE;
	}
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCGenExamp->ResumeThread();
		m_pCGenExamp->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCGenExamp->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (m_pCGenExamp->m_hThread);
	m_pCGenExamp	= 0;
	return TRUE;
}
catch (char * str)
{
	AfxMessageBox(str);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
		AfxMessageBox("CSystem::StopWorker");
}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
#endif

///////////////////////////////////////////////////////////////////////////////

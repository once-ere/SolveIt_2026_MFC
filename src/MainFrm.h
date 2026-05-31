// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4547DF4F_2327_43AD_BF8B_96FC450E7D14__INCLUDED_)
#define AFX_MAINFRM_H__4547DF4F_2327_43AD_BF8B_96FC450E7D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockingStatusBar.h"
//#include "System.h"
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CSystem;
///////////////////////////////////////////////////////////////////////////////
class CGLView;
class CSolveItView;
class CLeftView;
class CChildFrame;
class CSolveItApp;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CleanupOnInitialize
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CCleanupOnInitialize : public vector<HWND>
{
public:
	CCleanupOnInitialize(){}
	virtual ~CCleanupOnInitialize()
	{
		Clear();
	}
///////////////////////////////////////////////////////////////////////////////
	void Clear()
	{
		if (size() == 0) return;
		vector<HWND>::iterator i;
		for (i = begin(); i != end(); ++i)
		{
			HWND hWnd = *i;
			if (hWnd==0) throw SolveIt_ERROR(L"CCleanupOnInitialize::Clear");//return;
			if (::IsWindow(hWnd)) ::SendMessage(hWnd, WM_CLOSE,0,0);
		}
		clear();
	}
///////////////////////////////////////////////////////////////////////////////
	void Add(const HWND& hWnd)
	{
		ASSERT(hWnd!=0);
		ASSERT(::IsWindow(hWnd));
		if (hWnd==0) return;
		push_back( hWnd );
	}
///////////////////////////////////////////////////////////////////////////////
	void Remove(const HWND& hWnd)
	{
		HWNDCompareEqual _HWNDCompareEqual(hWnd);
		vector<HWND>::iterator j = find_if( begin(), end(), _HWNDCompareEqual);
		ASSERT(j != end());
		if (j != end()) erase(j);
	}
};
///////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMDIFrameWnd
{
	friend CSolveItApp;
	friend CGLView;

	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
//	CSystem m_system;
	CSystem* m_system;
	CChildFrame* m_pMDIActive;// MDIGetActive() fails when CGLView is active 
//	CGLView *m_pCGLView;
//	CSolveItView*	m_pCSolveItView;

	CCleanupOnInitialize m_CleanupOnInitialize;


// Operations
public:
	void CreateOrActivateFrame(CDocTemplate* pTemplate, CRuntimeClass* pViewClass);

	void	SetChildFrame();
	void	SetChildFrame(CChildFrame* pMDIActive);

	CLeftView*		GetTreeView();
	CSolveItView*	GetEditView();
	CGLView*		GetGLView();

	static CLeftView*		theTreeView;
	static CSolveItView*	theEditView;
	static CGLView*			theGLView;

	void PopulateTree();
	void UpdateTree();
	void Paint();
	void ViewPotential();
	void PositionWindow();
	void Initialize();

	void SetStatusBarText(const wstring& str, int  nStatusPane);
	virtual void OnInitialUpdate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
///////////////////////////////////////////////////////////////////////////////
// scan
	UINT_PTR m_TimerScan;
	double m_x1, m_y1, m_x2, m_y2;//from (x1,y1) to (x2,y2)
	UINT m_uMillisecondsBetweenScanSteps;
	double m_fScanCurveParameter;
	double m_fScanStepSize;
	double m_fScanDistance;
///////////////////////////////////////////////////////////////////////////////

protected:  // control bar embedded members
	CBlockingStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;


	HICON m_hIconPerspective;
	HICON m_hIconParallel;
// Generated message map functions
protected:
	int Scan();
	//{{AFX_MSG(CMainFrame)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnRun();
	afx_msg void OnStop();
	afx_msg void OnStep();
	afx_msg void OnClear();
	afx_msg void OnPNG();
	afx_msg void OnEPS();
	afx_msg void OnListCode();
	afx_msg void OnViewReset();
	afx_msg void OnToggleView();
	afx_msg void OnFlyBy();
	
	afx_msg void OnReset();
	afx_msg void OnUpdateOnViewReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOnReset(CCmdUI* pCmdUI);

	afx_msg void OnView1dim();
	afx_msg void OnUpdateView1dim(CCmdUI* pCmdUI);

	afx_msg void OnView2dim();
	afx_msg void OnUpdateView2dim(CCmdUI* pCmdUI);
	afx_msg void OnView3dim();
	afx_msg void OnUpdateView3dim(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFlyBy(CCmdUI* pCmdUI);

	afx_msg LRESULT OnHalt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitialize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUSER_Message(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUSER_KillWorker(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUSER_AddCInfoDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUSER_AddCInfoDialog2(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEnChange();
	afx_msg void OnClickedButton_scan();
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnUpdateBlockingStatusBar(CCmdUI* pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_MAINFRM_H__4547DF4F_2327_43AD_BF8B_96FC450E7D14__INCLUDED_)

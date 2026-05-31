// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__5627AFD4_EA82_4BD4_B46C_64D7F8601A1C__INCLUDED_)
#define AFX_CHILDFRM_H__5627AFD4_EA82_4BD4_B46C_64D7F8601A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
class CGLView;
class CSolveItView;
class CLeftView;

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:
	CGLView *m_pCGLView;
	CSolveItView*	m_pCSolveItView;
	CLeftView*		theTreeView;
///////////////////////////////////////////////////////////////////////////////
#if 0
	CSourceView *m_pSrcView;
	CHTMLEdView *m_pWebView;
	DWORD m_dwCurrentView;
	afx_msg void OnViewSource();
	afx_msg void OnViewWeb();
	afx_msg void OnUpdateViewWeb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSource(CCmdUI* pCmdUI);
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Operations
public:
	CView *GetView(int row, int col);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void ActivateFrame(int nCmdShow);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CHILDFRM_H__5627AFD4_EA82_4BD4_B46C_64D7F8601A1C__INCLUDED_)

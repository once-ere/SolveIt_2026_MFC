#if !defined(AFX_BLOCKINGSTATUSBAR_H__7C77D900_DCAF_40A0_BFE3_95698B4DEA29__INCLUDED_)
#define AFX_BLOCKINGSTATUSBAR_H__7C77D900_DCAF_40A0_BFE3_95698B4DEA29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockingStatusBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CBlockingStatusBar window

class CBlockingStatusBar : public CStatusBar
{
// Construction
public:
	CBlockingStatusBar();

// Attributes
public:
	bool m_bIgnoreSetText;
// Operations
public:
	LRESULT SetPaneOneAndLock(const wstring& str);
	LRESULT SetPaneOneAndUnlock(const wstring& str);
	LRESULT SetTextPane( int nIndex, const wstring& str);
	void PreparePane( int nIndex, const wstring& str);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockingStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBlockingStatusBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBlockingStatusBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM, LPARAM);
	afx_msg void OnUpdateBlockingStatusBar(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_BLOCKINGSTATUSBAR_H__7C77D900_DCAF_40A0_BFE3_95698B4DEA29__INCLUDED_)

#if !defined(AFX_TEXTBOXWINDOW_H__C5477B9C_1DCA_4EF4_8FE1_1FEA33795B6D__INCLUDED_)
#define AFX_TEXTBOXWINDOW_H__C5477B9C_1DCA_4EF4_8FE1_1FEA33795B6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextBoxWindow.h : header file
//
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CSolveItView;
/////////////////////////////////////////////////////////////////////////////
// CTextBoxWindow window

class CTextBoxWindow : public CEdit
{
	friend CSolveItView;
// Construction
public:
//	CTextBoxWindow();
	CTextBoxWindow(CSolveItView* pV);
	CTextBoxWindow(CSolveItView* pV, int y, int cy, HWND hWndPrompt);

// Attributes
public:
	CSolveItView* m_pCSolveItView;
	CRect m_rc;
	CRect m_rcWndPrompt;
	HWND m_hWndPrompt;
	int m_x;
	int m_y;
	int& m_cx;
	int m_cy;//single line
	int m_Height;
//	int m_Width;
	long nCrLf;
	bool m_bLockTop;//hack
	BOOL m_bSimulateShift;//hack
	BOOL m_bSimulateReturn;//hack
// Operations
public:
	void Resize(int nBias = 1);
	void IncrementSize();
	void DecrementSize();
	void MoveVertically(int numRows);
	int GetBottom();
	const char* GetTextBoxText();
//	auto_ptr<char> GetTextBoxStrings();
	char* GetTextBoxStrings();
	bool OnUpdateNeedSel();
	int WriteLine(BSTR strText, long lWriteReturn);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextBoxWindow)
	//}}AFX_VIRTUAL
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CTextBoxWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextBoxWindow)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnPopupCopy();
	afx_msg void	OnPopupCut();
	afx_msg void	OnPopupDelete();
	afx_msg void	OnPopupPaste();
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUserCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
//OnUserCommand: HOW BIZARRE CAN WINDOWS GET?
//
//	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}//end namespace SolveIt

#endif // !defined(AFX_TEXTBOXWINDOW_H__C5477B9C_1DCA_4EF4_8FE1_1FEA33795B6D__INCLUDED_)

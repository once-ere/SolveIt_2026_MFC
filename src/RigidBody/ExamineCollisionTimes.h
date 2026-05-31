#if !defined(AFX_EXAMINECOLLISIONTIMES_H__70235FD0_C87B_4617_8855_5735766C2B39__INCLUDED_)
#define AFX_EXAMINECOLLISIONTIMES_H__70235FD0_C87B_4617_8855_5735766C2B39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExamineCollisionTimes.h : header file
//

namespace SolveIt {

class CSystem;
/////////////////////////////////////////////////////////////////////////////
// CExamineCollisionTimes dialog

class CExamineCollisionTimes : public CDialog
{
// Construction
public:
	CExamineCollisionTimes(CSystem* sys, CWnd* pParent = NULL);   // standard constructor


/////////////////////////////////////////////////////////////////////////////

	CSystem* psys;
	vector<double>& m_vecCollisionTimes;
	void UpdateData();

// Dialog Data
	//{{AFX_DATA(CExamineCollisionTimes)
	enum { IDD = IDD_EXAMINECOLLISIONTIMES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamineCollisionTimes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExamineCollisionTimes)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} // namespace SolveIt

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMINECOLLISIONTIMES_H__70235FD0_C87B_4617_8855_5735766C2B39__INCLUDED_)

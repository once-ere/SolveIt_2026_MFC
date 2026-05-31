#if !defined(AFX_QM_Scatt2D_H__0FC88776_E6B2_4C32_98C3_22016CB5A8DE__INCLUDED_)
#define AFX_QM_Scatt2D_H__0FC88776_E6B2_4C32_98C3_22016CB5A8DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QM_Scatt2D.h : header file
//


/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{



/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt2D thread

class CQM_Scatt2D : public CWinThread
{
	DECLARE_DYNCREATE(CQM_Scatt2D)
protected:
	CQM_Scatt2D();           // protected constructor used by dynamic creation

// Attributes
public:
	bool m_bStepCompleted;

// Operations
public:
	long MakeTimeStep();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQM_Scatt2D)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CQM_Scatt2D();

	// Generated message map functions
	//{{AFX_MSG(CQM_Scatt2D)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


	afx_msg void OnStep(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetParam(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_QM_Scatt2D_H__0FC88776_E6B2_4C32_98C3_22016CB5A8DE__INCLUDED_)

#if !defined(AFX_QM_GROUND_MC_1D_H__1F7EB29F_6ECB_4D08_9F5C_33CD2BE6E7F2__INCLUDED_)
#define AFX_QM_GROUND_MC_1D_H__1F7EB29F_6ECB_4D08_9F5C_33CD2BE6E7F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// qm_ground_mc_1d.h : header file
//


/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{



/////////////////////////////////////////////////////////////////////////////
// CQM_Ground_MC_1D thread

class CQM_Ground_MC_1D : public CWinThread
{
	DECLARE_DYNCREATE(CQM_Ground_MC_1D)
protected:
	CQM_Ground_MC_1D();           // protected constructor used by dynamic creation

// Attributes
public:
	bool m_bStepCompleted;

// Operations
public:
	long MakeTimeStep();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQM_Ground_MC_1D)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CQM_Ground_MC_1D();

	// Generated message map functions
	//{{AFX_MSG(CQM_Ground_MC_1D)
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


#endif // !defined(AFX_QM_GROUND_MC_1D_H__1F7EB29F_6ECB_4D08_9F5C_33CD2BE6E7F2__INCLUDED_)

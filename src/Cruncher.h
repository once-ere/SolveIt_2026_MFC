#if !defined(AFX_CRUNCHER_H__BAD8B206_CF5A_4D93_9041_5E290AF29459__INCLUDED_)
#define AFX_CRUNCHER_H__BAD8B206_CF5A_4D93_9041_5E290AF29459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cruncher.h : header file
//

#include "SolveIt.Lib/lib/CVOde/LLNLTYPS.H"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "SolveIt.Lib/lib/CVOde/CVODE.H"		 /* main CVODE header file													*/
#include "SolveIt.Lib/lib/CVOde/ITERATIV.H"	/* contains the enum for types of preconditioning	*/
#include "SolveIt.Lib/lib/CVOde/CVSPGMR.H"	 /* use CVSPGMR linear solver each internal step		*/
#include "SolveIt.Lib/lib/CVOde/DENSE.H"		 /* use generic DENSE solver for preconditioning		*/
#include "SolveIt.Lib/lib/CVOde/CVDENSE.H"  /* use CVDENSE linear solver each internal step */
#include "Types.h"

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
class CSystem;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCruncher thread

class _CCruncher : public CWinThread
{
	DECLARE_DYNCREATE(_CCruncher)
protected:
	_CCruncher(){}           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual long CVodeStep(){return 0;}
	virtual long MakeTimeStep(){return 0;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCruncher)
	//}}AFX_VIRTUAL
	public:
	virtual BOOL InitInstance(){return 0;}
	virtual int ExitInstance(){return 0;}

// Implementation
protected:
	virtual ~_CCruncher(){}

};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCruncher thread

class CCruncher : public CWinThread	//_CCruncher
{
	DECLARE_DYNCREATE(CCruncher)
protected:
	CCruncher();           //{} protected constructor used by dynamic creation

// Attributes
public:
	CSystem* m_pCSystem;
	const bool bStepMultibody;
	const long m_NumberVariables;
	long& m_lTakeNumSteps;
	long m_lNumStepsTaken;
/////////////////////////////////////////////////////////////////////////////
//	bool m_bSingleStepping;
//	bool m_bSingleStep;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	bool& m_bSingleStepping;
	bool& m_bSingleStep_Mode;
	bool& m_bRunning;
///////////////////////////////////////////////////////////////////////////////

// Operations
public:
	long CVodeStep();
	long MakeTimeStep();
	long StepMultibodyState();
	void Wait();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCruncher)
	//}}AFX_VIRTUAL
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
protected:
	virtual ~CCruncher(){}

	// Generated message map functions
	//{{AFX_MSG(CCruncher)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnSingleStep(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRun(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CRUNCHER_H__BAD8B206_CF5A_4D93_9041_5E290AF29459__INCLUDED_)

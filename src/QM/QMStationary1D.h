#if !defined(AFX_QMSTATIONARY1D_H__CFCBB854_79EC_421E_A9EF_D26FA506D8F3__INCLUDED_)
#define AFX_QMSTATIONARY1D_H__CFCBB854_79EC_421E_A9EF_D26FA506D8F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// qmstationary1d.h : header file
//

#include "array.h"
///////////////////////////////////////////////////////////////////////////////
extern "C" /* Subroutine */ int callsledge_(long *numx, long *indexeigen, 
	double *ev, double *xef, double *ef, double *pdef, 
	long *iflag);
//////////////////////////////////////////////////////////////////////
/* ********************************************************************** */
/*      These routines estimate eigenvalues, eigenfunctions and/or     * */
/*      spectral density functions for Sturm-Liouville problems.       * */
/*      The differential equation has the form:                        * */
/*                                                                     * */
/*           -(p(x)u')' + q(x)u  =  EV*r(x)u       for x in [A,B]      * */
/*                                                                     * */
/*      with boundary conditions (at regular points)                   * */
/*                                                                     * */
/*           A1*u - A2*(pu')  =  EV*(A1'*u - A2'*(pu'))    at A        * */
/*           B1*u + B2*(pu')  =  0                         at B .      * */
/*                                                                     * */
/*      The functions p(x) and r(x) are assumed to be positive in      * */
/*      the open interval (A,B).                                       * */
/* ********************************************************************** */

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{




/////////////////////////////////////////////////////////////////////////////
// CQMStationary1D thread

class CQMStationary1D : public CWinThread
{
	DECLARE_DYNCREATE(CQMStationary1D)
protected:
	CQMStationary1D();           // protected constructor used by dynamic creation

// Attributes
public:
//	vector_of_double abscissa;
//	vector_of_double plot_potential;
//	vector_of_double wave_function;
//	vector_of_double dwave_function;
/*     PDEF(*)   = real vector of eigenfunction derivative values: */
/*                 PDEF((k-1)*NUMX+i) is the estimate of (pu')(XEF(i)) */
/*                 corresponding to the eigenvalue in EV(k).  If JOB(2) */
/*                 is False then this vector is not referenced; otherwise, */
/*                 it must be dimensioned as is EF(*). */
	double* abscissa;
	double* wave_function;
	double* dwave_function;
	double* plot_potential;
	double* probability_density;

	int& m_SolveMethod;
	long& eigenvalueIndex;
	long& m_NDATA;
	double& m_fScaleWave;

	bool m_bStepCompleted;
//	bool m_bUseRadial;
	double m_L0;
	double m_L1;
	double Omega0;
	double& x_min;
	double& x_max;
	double& energy_eigenvalue;
	double& M;
	double dx;
	double TwoMOverMu;
	double MuOverTwoM;
//	double PsiAtZero;
//	double PsiPrimeAtZero;

// Operations
public:
	long MakeTimeStep();
	void DrawSystem();
	void Wait();
///////////////////////////////////////////////////////////////////////////////
	void SetTwoMOverMu(double x)
	{
		if (x <= 0.0) return;
		TwoMOverMu	= x;
		MuOverTwoM = 1.0/TwoMOverMu;
		SetM();
	}
///////////////////////////////////////////////////////////////////////////////
	void SetMuOverTwoM(double x)
	{
		if (x <= 0.0) return;
		MuOverTwoM= x;
		TwoMOverMu = 1.0/MuOverTwoM;
		SetM();
	}
///////////////////////////////////////////////////////////////////////////////
	void SetM(void)
	{
		M = (TwoMOverMu*m_L1*m_L1)/Omega0;
		Initialize();
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void SetL0(const double& val)
	{
		m_L0 = val;
		m_L1 = 1.0/ m_L0;
		SetM();
	}
///////////////////////////////////////////////////////////////////////////////
	void SetL1(const double& val)
	{
		m_L1 = val;
		m_L0 = 1.0/ m_L1;
		SetM();
	}
///////////////////////////////////////////////////////////////////////////////
	void Initialize(void);
	void Normalize();
	void NormalizeRADIAL();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQMStationary1D)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CQMStationary1D();

	// Generated message map functions
	//{{AFX_MSG(CQMStationary1D)
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


#endif // !defined(AFX_QMSTATIONARY1D_H__CFCBB854_79EC_421E_A9EF_D26FA506D8F3__INCLUDED_)

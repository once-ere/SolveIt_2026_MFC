// QMStationary2D.h: interface for the CQMStationary2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMSTATIONARY2D_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_)
#define AFX_QMSTATIONARY2D_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"
#include <list>

namespace SolveIt {
/////////////////////////////////////////////////////////////////////////////
struct Vertex
{
	double x,y,z;
};

	const constexpr  long max_NumFound	= 11;
	const constexpr  long max_Num		= (max_NumFound+1);
	const constexpr  long _N2			= 64;
	const constexpr  long _N2a			= (_N2+1);
	const constexpr  long _N2aN2a		= (_N2a*_N2a);

/*
//	static double psi_xy[max_Num*_N2aN2a];
	static GLuint listEigenStates[max_Num];
//	static double EigenValues[max_Num];

	double *__potential		= (double *)DATA_2D_mp_PPOTENTIAL;
	double *__psi_xy		= (double *)DATA_2D_mp_PPSI__XY;
	double *__EigenValues	= (double *)DATA_2D_mp_PEIGENVALUES;
*/

#define psi_xy(j, x, y) __psi_xy[(j)*_N2aN2a+(x)*_N2a+(y)]

/*
#define psi_xy(j, x, y) psi_xy[(j)*_N2aN2a+(x)*_N2a+(y)]
#define EigenValues(x) EigenValues[(x)]

#define EigenValues(x) __EigenValues[(x)]
*/

#define potential(x, y) __potential[(x)*_N2a+(y)]

class CQMStationary2D   : public CWinThread
{
	DECLARE_DYNCREATE(CQMStationary2D)
protected:
	CQMStationary2D();//long _NDATA, double& fScaleNurbs);
	virtual ~CQMStationary2D();
public:
//	GLUnurbsObj*	theNurbs;//	= gluNewNurbsRenderer();
	Mutex mutex;

//	double psi_xy[max_Num*_N2aN2a];
//	static double psi_xy[2*max_Num*_N2aN2a];
//	double EigenValues[max_Num];
	GLuint listEigenStates[max_Num];

//	double *v;
//	static double *eigenvalues;
//	static double *eigenvalueErrors;
//	double eigenvalues[65];
//	double eigenvalueErrors[65];
	double getPotential(int x, int y);
	
//	static GLuint *listEigenStates;
//	GLuint listEigenStates[max_Num];
//	GLuint *listEigenStates;
	GLuint firstInlistEigenStates;

/*
	union {
		double *eigenvectors_block;
		double **eigenvectors;
	};
*/

//	static 
	double **psi_x;
//	double *psi_xy;
//	double psi_xy[max_Num*_N2aN2a];

	double	m_fMaxPotential;
	double	m_fMinPotential;
	double	m_fMaxPsi;
	double	m_fMinPsi;
	double	m_fScalePsi;
	double	m_fScalePotential;
	double&	m_fScaleNurbs;
//	double	m_fScaleNurbs;
	double	&m_fLambda;
//	double	m_fLambda;
	long NDATA;
	long	m_nNumFound;
	long	&m_nSeekEigenvalues;
//	long	m_nSeekEigenvalues;

	long	m_nSelectedEigenstate;
///////////////////////////////////////////////////////////////////////////////
	HWND QMStationary2DhWnd;
	std::wstring QMStationary2Dtxt;//(g_szWorkDir.c_str());

//#define _N2  64

	void FindMinMaxPsi(long j);

	void ComputeEigenState(const double& energy_eigenvalue);
	double CheckEigenState(const double& energy_eigenvalue, const long& index);
	void EigenStates(void);//arpack
	void EigenStatesTRLan();
	void DefinePotential(void);
	void FindMinMaxPotential(void);
	void Draw(void);
	void Draw(const long& j);
	void DrawList(const long& j);
//	static void __cdecl GLViewCallback(void);
	static void GLViewRenderCallback(void);
	static void GLViewNotificationCallback(void);
//	friend void __cdecl GLViewCallback(void);

	std::wstring m_bstrQHullOptions;
	long BuildHull(const long& j);//, list<Vertex>& verts, double max);
	void DrawHull(const long& j);
	void DrawSplineSurface();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQMStationary1D)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL


	afx_msg void OnStep(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetParam(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////
// Specialized derivation to handle "button" click
//
class CQMStationary2DHtmlCtrl : public CHtmlCtrl {
	virtual void OnAppCmd(LPCTSTR lpszWhere);
};


///////////////////////////////////////////////////////////////////////////////
// About dialog uses HTML control to display contents.
//
class CQMStationary2DDialog : public CDialog {
	friend class CGLView;
	DECLARE_DYNAMIC(CQMStationary2DDialog)
protected:
	CQMStationary2DHtmlCtrl m_page;
	virtual BOOL OnInitDialog(); 
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP();
public:
	CQMStationary2DDialog(CWnd* pParent = NULL) : CDialog(IDD_HELPBOX, NULL) {
	CDialog::Create(IDD_HELPBOX, pParent);
	ShowWindow(SW_SHOW);
	}
};


} // namespace SolveIt

#endif // !defined(AFX_QMSTATIONARY2D_H__4AEACDC1_4E18_452A_86EA_C51DCC42F97E__INCLUDED_)

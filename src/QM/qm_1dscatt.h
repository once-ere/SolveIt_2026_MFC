#if !defined(AFX_QM_Scatt1D_H__EFAFB3D1_90D2_464A_9E41_1162DB760EB1__INCLUDED_)
#define AFX_QM_Scatt1D_H__EFAFB3D1_90D2_464A_9E41_1162DB760EB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QM_Scatt1D.h : header file
//
#include "QMEvolve.h"
#include "QMEvolve1D.h"

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{

/////////////////////////////////////////////////////////////////////////////
const double Pi					= 3.14159265358979323846;
const double PiOver2			= 1.57079632679489661923;
const double TwoPiToTheOneFourth= 1.58323348708615953858;
const double TwoOverPiToTheOneFourth=0.893243841738002331401;
const double PiToTheMinusOneFourth= 0.75112554446494248286;

const double J0zero	= 2.4048255576957727686;
const double J1zero	= 3.8317059702075123156;
const double J2zero	= 5.1356223018406825563;
const double J3zero	= 9.7610231299816696785;
const double J4zero	= 11.064709488501184883;
const double J5zero	= 12.338604197466943986;
/////////////////////////////////////////////////////////////////////////////
//const int HITSIZE = 2;
/////////////////////////////////////////////////////////////////////////////
// CQM_Scatt1D thread

class CQM_Scatt1D : 
public CWinThread
{
	DECLARE_DYNCREATE(CQM_Scatt1D)
protected:
	CQM_Scatt1D();           // protected constructor used by dynamic creation

// Attributes
public:
	bool m_bStepCompleted;
//	CQMEvolve1D QMEvolve1D;
//	vector<CBox *> boxes;

// Operations
public:
	long MakeTimeStep();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQM_Scatt1D)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long& m_lTakeNumSteps;
	long m_lNumStepsTaken;
//	HWND m_videoWnd;
//	bool m_paused;
//	bool m_bMouseDown;
//	long m_begin_x, m_begin_y;
//	bool m_bPotentialChanged;

//	bool m_bFirstDraw;
//	bool m_bDrawUsingInitial;
///////////////////////////////////////////////////////////////////////////////

//	double&		m_x0;
//	double		m_fScalePsi;
//	double		m_fScalePotential;

//	long&	m_nNumNodes;
//	long&	m_nRenormalize;
///////////////////////////////////////////////////////////////////////////////
//	int&	m_nWidth;
//	int&	m_nHeight;
	
//	double&	m_fLambda;
//	double&	m_fWaveNumber;


//	double&	m_fInitialSpread;
// rendering background:
//	long DrawBackground(const RECT& rc);

//	void Clear();
//	void OnLoad();

//	CBox* m_pSelectedBox;
//	long m_numRectangles;
//	inline CBox* HitTestRectangleItems(const POINT& point);
//	CBox* AddPot(const RECT& r);//inline 
//	void RemovePot(CBox* pbox);//inline 

//	void Compute();
//	void Compute2D();
//	void DefinePotential();
//	void AVI_Init(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile, const LPBITMAPINFO pbmi, std::wstring bstrHDF5FileName);
//	void AVI_Term(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile);


// Implementation
protected:
	virtual ~CQM_Scatt1D();

	// Generated message map functions
	//{{AFX_MSG(CQM_Scatt1D)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


	afx_msg void OnSingleStep(WPARAM wParam, LPARAM lParam);
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


#endif // !defined(AFX_QM_Scatt1D_H__EFAFB3D1_90D2_464A_9E41_1162DB760EB1__INCLUDED_)

#if !defined(AFX_GENEXAMP_H__25FD9787_1E6E_4D27_989A_FD14E80A2430__INCLUDED_)
#define AFX_GENEXAMP_H__25FD9787_1E6E_4D27_989A_FD14E80A2430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenExamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz); //Action::DupString
//requires a call to	//GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
//std::wstring  DupString(std::wstring const &  lpsz); //_tcsdup; memory leak
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define WM_APP_INIT_2(a,b) \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	if (a) app->PostThreadMessage(WM_APP_Initialize, (WPARAM) GetWC((LPCSTR)(a)), (LPARAM) GetWC((LPCSTR)(b)));\
	else app->PostThreadMessage(WM_APP_Initialize, 0, 0);\
	}
///////////////////////////////////////////////////////////////////////////////
#define WM_APP_INIT \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	app->PostThreadMessage(WM_APP_Initialize, 0, 0);\
	WaitForSingleObject(g_EventInitialize, INFINITE);\
	}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define WM_APP_INIT2(a,b) \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	app->PostThreadMessage(WM_APP_Initialize, (WPARAM) GetWC((LPCSTR)(a)), (LPARAM) GetWC((LPCSTR)(b)));\
	WaitForSingleObject(g_EventInitialize, INFINITE);\
	}

///////////////////////////////////////////////////////////////////////////////
/*
#define WM_APP_INIT \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	CSolveItApp* s = dynamic_cast<CSolveItApp*>(app);\
	if (s == NULL) throw SolveIt_ERROR(L"CSolveItApp::WM_APP_Initialize: CSolveItApp* == 0");\
	s->PostThreadMessage(WM_APP_Initialize, 0, 0);\
	WaitForSingleObject(g_EventInitialize, INFINITE);\
	}

#define Create_GenExampThread(xxx)	\
	{\
#if __use_GenExampThread_ \
	Create_GenExampThread_2(xxx) \
#else \
	WM_APP_INIT_2(0,0) \
#endif \
	}

*/
//	SleepEx(1000, TRUE);
//	SleepEx(1000, TRUE);

#define Create_GenExampThread(xxx)	\
	{\
	WM_APP_INIT_2(0,0) \
	}

/////////////////////////////////////////////////////////////////////////////

#define Create_GenExampThread_2(xxx)	\
	{\
	CreateGenExampThread(true);\
	if (m_pCGenExamp)\
	{\
		m_pCGenExamp->ResumeThread();\
		m_pCGenExamp->PostThreadMessage((xxx), 0, 0);\
	}\
	return;\
	}

/////////////////////////////////////////////////////////////////////////////

#define Create_GenExampThread2(xxx)	\
	{\
	theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);\
	}
//	SleepEx(1000, TRUE);

///////////////////////////////////////////////////////////////////////////////
#define Create_GenExampThread5(xxx) \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	CSolveItApp* s = dynamic_cast<CSolveItApp*>(app);\
	if (s == NULL) throw SolveIt_ERROR(L"CSolveItApp::WM_APP_Initialize: CSolveItApp* == 0");\
	s->PostThreadMessage(WM_APP_Initialize, 0, 0);\
	}

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CGenExamp thread

class CGenExamp : public CWinThread
{
	DECLARE_DYNCREATE(CGenExamp)
protected:
	CGenExamp();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenExamp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGenExamp();

	// Generated message map functions
	//{{AFX_MSG(CGenExamp)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


	afx_msg void OnSpline();

	afx_msg void OnSum();
	afx_msg void OnDifference();
	afx_msg void OnCrossProduct();

	afx_msg void OnMultiBodyTest();
	afx_msg void OnMultiBodySlider();
	afx_msg void OnMultiMassSpring();
	afx_msg void OnMultiBodyChain();

	afx_msg void OnSimplePendulum();
	afx_msg void OnSimpleHarmonicOscillator();
	afx_msg void OnResonantSimpleHarmonicOscillator();
	afx_msg void OnCubicAnharmonicOscillator();
	afx_msg void OnAnharmonicOscillator();
	afx_msg void OnTest();
	afx_msg void OnLennardJones();
	afx_msg void OnLennardJonesHeatedCooled();
	afx_msg void OnRamp();
	afx_msg void OnPulley();
	afx_msg void OnTwoBodyScattering();
	afx_msg void OnThreeBodyCollision();
	afx_msg void OnFourBodyCollision();
	afx_msg void OnFourBodyCollision2();
	afx_msg void OnBallOnShpere();
	afx_msg void OnSphereAndBlock();
	afx_msg void OnTwoBlocksAndPulley();
	afx_msg void OnBlockAndPulley();
	afx_msg void OnBlockAndTackle();
	afx_msg void OnInelasticCollision();
	afx_msg void OnInelasticPendulumCollision();
	afx_msg void OnNewtonCradle();
	afx_msg void OnNewtonCradle2();
	afx_msg void OnUserDefinedTimeIndependentX();
	afx_msg void OnUserDefinedTimeIndependentY();
	afx_msg void OnUserDefinedTimeIndependentZ();
	afx_msg void OnUserDefinedTimeDependentX();
	afx_msg void OnUserDefinedTimeDependentY();
	afx_msg void OnUserDefinedTimeDependentZ();

	afx_msg void Scattering1Dim();
	afx_msg void Scattering2Dim();
	afx_msg void QM_Ground_State_1Dim();
	afx_msg void QMStationary1Dim();
	afx_msg void QMStationary1DimB();
	afx_msg void QMStationary2Dim();
	afx_msg void Superposition();
	afx_msg void OnN_Body();
	afx_msg void OnOrbitalMechanics();
	afx_msg void OnSunEarthMoon();
	afx_msg void OnEarthMoon();
	afx_msg void OnCannonBall();
	afx_msg void OnTwoBody();
	afx_msg void OnThreeBody();
	afx_msg void OnThreeChargedBodies();
	afx_msg void OnElectroStaticField();
	afx_msg void OnCyclotron();
	afx_msg void OnCurrentLoop();
	afx_msg void OnVerhulstDynamics();
	afx_msg void On6Body();
	afx_msg void OnChain();
	afx_msg void OnRandomChain();
	afx_msg void OnCompoundPendulum();
	afx_msg void OnPhysicalPendulum();
	afx_msg void OnTrapping();
	afx_msg void OnTwoNonShpericalBody();
	afx_msg void OnPendulumAndFixedCharge();
	afx_msg void OnPendulumAndPlanarCharge();
	afx_msg void OnSubmerged();
	afx_msg void Problem2_14();
	afx_msg void Problem2_37();
	afx_msg void Example2_4();
	afx_msg void Problem3_46();
	afx_msg void Problem3_51();
	afx_msg void Problem3_60();
	afx_msg void Example7_8();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_GENEXAMP_H__25FD9787_1E6E_4D27_989A_FD14E80A2430__INCLUDED_)

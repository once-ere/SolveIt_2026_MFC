// QMExamples.cpp : Implementation of CIO
#include "stdafx.h"
#include "SolveIt.h"
#include "GenExamp.h"

#include "SolveItView.h"
#include "MainFrm.h"
#include "GLView.h"

#include "resource.h"
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"
//#include "QM/QMStationary2D.h"
//#include "System.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "../ScriptColors.h"

///////////////////////////////////////////////////////////////////////////////

/*
		RECT rc;
		GetWindowRect(&rc);
CComPtr<IAxWinHostWindow> spAxWindow;
hr = QueryHost(&spAxWindow);
wnd.Create(hWnd, rc, _T("Act.CGraph2D.1;"), WS_CHILD|WS_VISIBLE, 0, WM_USER+3);  

		CComQIPtr <IPersistStreamInit> spPerStm(theSystem->m_pIQM_2DScatt);
		spPerStm->InitNew();
		CComPtr<IUnknown> spUnkContainer;
		HWND hWnd = 0;
		hr = theSystem->m_pIUI_ClassicalPhysics->get_Window((long*)&hWnd);//m_pIView
		AtlAxAttachControl( theSystem->m_pIQM_2DScatt, hWnd, &spUnkContainer );
	IUnknown *punk=0;
//	AtlAxCreateControl( (const unsigned short *) &CLSID_Graph2D, hWnd, 0, &punk );
	AtlAxCreateControl( T2COLE(_T("Act.CGraph2D.1;")), hWnd, 0, &spUnk );
*/
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Scattering2Dim()
{
	WM_APP_INIT
	//theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"2-Dim Scattering\";")
	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, -1, 10 ] ;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function V(x$, y$) {"
"q=scaleX*(x);"
"Y=scaleX*(y);"
"	return ( (1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + Y*Y*(0.25+6.25*exp(-q*q)) - 1.0 );"
"	return ( (1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + y*y*(1.0+scaleE*exp(-q*q)) - 1.0 );"
	"}")
	WRITE_LINE("scaleX = 2.0;")
	WRITE_LINE("scaleE = 6.25;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Stretch(x$) {"
"val=1.0/2048.0;"
"if (x <= val) {"
"	return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
"};"
" while(x>val){;"
"	val=2.0*val;"
"};"
"return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Red(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2*r ));"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Green(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2.52*r ));"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Blue(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,3*r ));"
	"}")
//	WRITE_LINE("	return  ( pow(0.0001+x, 1.0/(0.000001+x)) );")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("system.redFunc = Red;")
	WRITE_LINE("system.greenFunc = Green;")
	WRITE_LINE("system.blueFunc = Blue;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("lightenDarken = 0.7;")
//	WRITE_LINE("system.potential = V 2, false;")
	WRITE_LINE("system.potential.name = 'V';")//
	WRITE_LINE("system.potential.nargs = 2;")//
	WRITE_LINE("system.potential.time = false;")//
	WRITE_LINE("system.potential = true;")//
	WRITE_LINE("system.potential.show = true;")//
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.twoDimensionalWorld.qm = true;")
//	NewWebBrowser(7);
//	//Fire_ClearUserDefs();
	WRITE_LINE("system.rotate =  [ -18.3 , 0, 0] ;")
	WRITE_LINE("system.translate.z =  -1.5;")//

	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//L"\r\nif (r == 0.0) return (-9.9e6);"
/////////////////////////////////////////////////////////////////////////////
void CGenExamp::QM_Ground_State_1Dim()
{
	WM_APP_INIT//theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"2D Hydrogen\";")
	WRITE_LINE("system.dimension = 1;")

	_bstr_t bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"\r\nreturn -2.0/r;"
L"\r\n}";

	Fire_WriteCommand(bstrV);
//
	WRITE_LINE("plot(\"V\", \" [x,0.1, 10], [y ,  -10, 1] \");");
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.state.mass = 1;")
	WRITE_LINE("system.state.scale = 30;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.domain.end = infinity;")
	WRITE_LINE("system.domain.begin = -infinity;")
//	WRITE_LINE("system.stationary.method = Cartesian;")
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.boundingBox = [ 0, 10, -5, 5, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")


/*
	new_case2(T_STATE , T_MASS)

	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	WRITE_LINE("system. = ;")
	new_case3(T_DOMAIN, T_XCOMP, T_BEGIN)
		m_domain[0].begin	= val;
		break;
	new_case3(T_DOMAIN, T_XCOMP, T_END)
		m_domainClass = val == infinity_object? infinite:finite;
		switch(m_domainClass)
		{
			case infinite:
*/
	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
return;
//	WM_APP_INIT/*WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///*/WaitForSingleObject(g_EventInitialize, INFINITE);
//	WRITE_LINE("system.name = \"1-Dim Eigenstate\";")
//	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, -2.5, 2.5, -1, 1 ] ;")
//	WRITE_LINE("system.boundingBox = [ -2, 2, -2, 2, -1, 1 ] ;")

	WRITE_LINE("function V(x$) {"
"q=exp(-x*x);"
"	return ( q*(-1+q*x*abs(x)*5) );"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function dVdx(x$) {"
"q=exp(-x*x);"
"if (x >= 0.0) {"
"	return  ( 2 *q *x* ( q*(5 - 10*x*x) + 1.0 ) );"
"	return  ( 2 *q *x* ( 5*q*(1.0 + Sqrt2*x)*(1.0 - Sqrt2*x) + 1.0 ) );"
"};"
"return  (  2 *q *x* ( -q*(5 - 10*x*x) + 1.0 ) );"
"	return  ( 2 *q *x* ( -5*q*(1.0 + Sqrt2*x)*(1.0 - Sqrt2*x) + 1.0 ) );"
	"}")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.potential = V 1, false;")
	WRITE_LINE("system.potential.name = 'V';")
	WRITE_LINE("system.potential.nargs = 1;")
	WRITE_LINE("system.potential.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.gradient.name.x = 'dVdx';")
	WRITE_LINE("system.gradient.nargs.x = 1;")
//	WRITE_LINE("system.gradient.x.time = false;")

	WRITE_LINE("system.numPoints = 101;")
	WRITE_LINE("system.potential = true;")
	WRITE_LINE("system.gradient.x = 'true';")
	WRITE_LINE("system.potential.show = true;")
	WRITE_LINE("system.ansatz = 'Monte Carlo';")
	WRITE_LINE("system.oneDimensionalWorld.qm.groundState = true;")

	_bstr_t bstr = L"Crude Path Integral Monte Carlo approximation for the ground state probability.";
	Fire_WriteResponse(bstr);//Fire_WriteResponse(0);

	bstr = L"Periodic boundary conditions are applied.";
	Fire_WriteResponse(bstr);//Fire_WriteResponse(0);

	bstr = L"To quit this example, press the 'New' key and think positive thoughts.";
	Fire_WriteResponse(bstr);//Fire_WriteResponse(0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::QMStationary1Dim()
{
//	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, -2.5, 2.5, -1, 1 ] ;")
	//Fire_ClearUserDefs();
	
	WM_APP_INIT
	
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"1-Dim Eigenstate\";")
	WRITE_LINE("system.dimension = 1;")
//

	_bstr_t bstrQ=\
L"function Q(r$)"
L"\r\n{"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nreturn 4*a*R/((a+R)^2+d^2);"
L"\r\n}";

	Fire_WriteCommand(bstrQ);
//L"\r\nif (r == 0.0) return (-0.00556);"
//L"\r\nif (r == 0.0) return (-0.00548344);"

	_bstr_t bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"\r\nif (r < 0.08) return (-0.00548344);"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\ndelta      = 0.0802;"
L"\r\nCoulomb    = -0.013;"
L"\r\nB0         = 20;"
L"\r\nB1         = -0.05;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nOmegaB0    = 0.0575567;"
L"\r\nq          = Q(r);"
L"\r\np          = sqrt(q);"
L"\r\nEc         = EllipticEc(q);"
L"\r\nK          = EllipticK(q);"
L"\r\nF1         = (4*(a + R)*a/((a + R)^2 + d^2))* sqrt(a/(R*q)) * ( -Ec + (1 - q/2)*K );"
L"\r\nDeE        = (Ec-K)/(2*q);"
L"\r\nDeK        = (Ec-(1-q)*K)/(2*(1-q)*q);"
L"\r\nF2         = ((a^2 - R^2 + d^2)*a*p^3/(R^2*sqrt(R*a))) *( -DeE - 0.5*K + (1 - 0.5*q)*DeK);"
L"\r\nreturn ((B1/B0)*( OmegaB0 *( 0.5 + delta ) + Coulomb )*( F1 + F2 )  / OmegaB0 );"
L"\r\n}";

	Fire_WriteCommand(bstrV);
	WRITE_LINE("system.boundingBox = [ 0, 1000, -0.05, 0.05, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")
	WRITE_LINE("plot(\"V\", \" [x,0, 1000], [y ,  -.012, .008] \");");
	WRITE_LINE("system.state.mass = 25.0/3;")
	WRITE_LINE("system.state.scale = 0.03;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.domain.end = infinity;")
//	WRITE_LINE("system.domain.begin = -infinity;")//
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = pseudoCartesian;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.energy;")

	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::QMStationary1DimB()
{
//	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, -2.5, 2.5, -1, 1 ] ;")
	//Fire_ClearUserDefs();	

	WM_APP_INIT
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"1-Dim Eigenstate\";")
	WRITE_LINE("system.dimension = 1;")
//

	_bstr_t bstrQ=\
L"function Q(r$)"
L"\r\n{"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nreturn 4*a*R/((a+R)^2+d^2);"
L"\r\n}";

	Fire_WriteCommand(bstrQ);
//L"\r\nif (r == 0.0) return (-0.00556);"

	_bstr_t bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"\r\nif (r == 0.0) return (0.00548344);"
L"\r\nif (r < 0.08) return (0.00548344);"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\ndelta      = 0.0802;"
L"\r\nCoulomb    = -0.013;"
L"\r\nB0         = 20;"
L"\r\nB1         = 0.05;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nOmegaB0    = 0.0575567;"
L"\r\nq          = Q(r);"
L"\r\np          = sqrt(q);"
L"\r\nEc         = EllipticEc(q);"
L"\r\nK          = EllipticK(q);"
L"\r\nF1         = (4*(a + R)*a/((a + R)^2 + d^2))* sqrt(a/(R*q)) * ( -Ec + (1 - q/2)*K );"
L"\r\nDeE        = (Ec-K)/(2*q);"
L"\r\nDeK        = (Ec-(1-q)*K)/(2*(1-q)*q);"
L"\r\nF2         = ((a^2 - R^2 + d^2)*a*p^3/(R^2*sqrt(R*a))) *( -DeE - 0.5*K + (1 - 0.5*q)*DeK);"
L"\r\nreturn ((B1/B0)*( OmegaB0 *( 0.5 + delta ) + Coulomb )*( F1 + F2 )  / OmegaB0 );"
L"\r\n}";

	Fire_WriteCommand(bstrV);
	WRITE_LINE("system.boundingBox = [ 0, 1000, -0.05, 0.05, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")
	WRITE_LINE("plot(\"V\", \" [x,0, 1000], [y ,  -.012, .008] \");");
	WRITE_LINE("system.state.mass = 25.0/3;")
	WRITE_LINE("system.state.scale = 0.03;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.domain.end = infinity;")
//	WRITE_LINE("system.domain.begin = -infinity;")//
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = pseudoCartesian;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.energy;")

	return;

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::QMStationary2Dim()
{
	//Fire_ClearUserDefs();

//	theApp.Initialize();
//	theApp.PostTh readMessage(WM_APP_Initialize, 0, 0);
	WM_APP_INIT

//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"2-Dim Eigenstate\";")
	WRITE_LINE("system.background.show = true;")
	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, 4.8, 20 ] ;")
//	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, 10, 20 ] ")
	WRITE_LINE("sys.translate.z =  -10.1")
//	WRITE_LINE("system.translate.z =  -5.1;")
//	WRITE_LINE("system.boundingBox = [ -3, 3, -3, 3, 5, 10 ] ;")
	WRITE_LINE("scaleX = 3.0;")
	WRITE_LINE("scaleA = 20.0;")
	WRITE_LINE("scaleE = 27.0;")
	WRITE_LINE("lightenDarken = 0.7;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function fx(x$, y$, z$) {"
"	return (0.25*x*(1 - x/4)*(1 + x/4) + 12.5*x*y*y*exp(-x*x));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function fy(x$, y$, z$) {"
"	return (-0.5*y*(1 + 25*exp(-x*x)));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("function V(x$, y$, z$) {"
	WRITE_LINE("function V(x$, y$) {"
"	return ( (1 - x/4)*(1 + x/4)*(1 - x/4)*(1 + x/4) + y*y*(0.25+6.25*exp(-x*x)));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Stretch(x$) \r\n{\r\n"
"val=1.0/2048.0;\r\n"
"if (x <= val) {"
"	return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
"};\r\n"
" while(x>val){"
"	val=2.0*val;"
"};\r\n"
"return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Red(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Green(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2.52*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Blue(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,3*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.color.red = \"Red\";")
	WRITE_LINE("system.color.green = \"Green\";")
	WRITE_LINE("system.color.blue = \"Blue\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
//	WRITE_LINE("system.potential = true;")//
	WRITE_LINE("system.potential.show = true;")//
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.twoDimensionalWorld.qm.stationaryState = true;")
	WRITE_LINE("system.rotate =  [ -18.3 , 0, 0] ;")//
	WRITE_LINE("system.translate.z =  -1.5;")//
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.state.energy = true;")
///////////////////////////////////////////////////////////////////////////////
//	CSystem* m_system = theApp.GetSystem();
//	m_system->m_bDoQMStationary2D	= true;
///////////////////////////////////////////////////////////////////////////////
//	m_system->m_pCQMStationary2D = new CQMStationary2D(_N2, theApp.GetGLView()->m_fScaleWave);//m_fScaleNurbs
//		AfxMessageBox("2");

	return;

	_bstr_t bstr = L"Periodic boundary conditions are applied.";
	Fire_WriteResponse(bstr);//Fire_WriteResponse(0);

	bstr = L"Double-click in the 'System' window [and wait] to toggle through the views of the different eigenstates.";
	Fire_WriteResponse(bstr);////Fire_WriteResponse(0);

//	bstr = L".";
//	Fire_WriteResponse(bstr);//Fire_WriteResponse(0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Scattering1Dim()
{
/*
Evolution Method

Nash-Chen
Periodic Cayley
Cayley

Largest Fourier component of initial wave function  = 
| Maximum Wave Number | without aliasing = 

Calculate Fourier component of initial wave function at wavenumber 
state		{ field_count+= count(yytext); *yylval = T_STATE;		return(T_STATE);	}
stationary	{ field_count+= count(yytext); *yylval = T_Stationary;	return(T_Stationary);	}
scattering	{ field_count+= count(yytext); *yylval = T_Scattering;	return(T_Scattering);	}
waveNumber	{ field_count+= count(yytext); *yylval = T_WaveNumber;	return(T_WaveNumber);	}
method		{ field_count+= count(yytext); *yylval = T_Method;		return(T_Method);	}
initialPacketSpread	{ field_count+= count(yytext); *yylval = T_InitialPacketSpread;	return(T_InitialPacketSpread);	}
center_of_Gaussian	{ field_count+= count(yytext); *yylval = T_Center_of_Gaussian;	return(T_Center_of_Gaussian);	}
fourierTransform	{ field_count+= count(yytext); *yylval = T_FourierTransform;	return(T_FourierTransform);	}
 
number		{ field_count+= count(yytext); *yylval = T_Number;	return(T_Number);	}
frames		{ field_count+= count(yytext); *yylval = T_Frames;	return(T_Frames);	}

Nash_Chen	{ field_count+= count(yytext); *yylval = T_Nash_Chen;	return(T_Nash_Chen);	}
PeriodicCayley	{ field_count+= count(yytext); *yylval = T_PeriodicCayley;	return(T_PeriodicCayley);	}
Cayley	{ field_count+= count(yytext); *yylval = T_Cayley;	return(T_Cayley);	}
*/	
//	theApp.Initialize(_T("Scattering1Dim.htm"), _T("Scattering1Dim"));WaitForSingleObject(g_EventInitialize, INFINITE);

//	AfxGetApp()->PostThreadMessage(WM_APP_Initialize, (WPARAM) dupString("Scattering1Dim.htm"), (LPARAM) dupString("Scattering1Dim"));
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WM_APP_INIT2("Scattering1Dim.htm", "Scattering1Dim")

	WRITE_LINE("system.name = \"1-Dim Scattering\";")
	WRITE_LINE("system.dimension = 1;")
	WRITE_LINE("system.boundingBox = [ -5, 5, -1, 1, -1, 1 ] ;")
	WRITE_LINE("system.translate.z =  0;")//
	WRITE_LINE("system.state.renormalize = true;")
	WRITE_LINE("system.state.renormalize = false;")
	WRITE_LINE("system.state.mass = 1;")
	WRITE_LINE("system.state.scale = 0.3;")
	WRITE_LINE("system.state.potential.scale = 3.0;")
	WRITE_LINE("system.scattering.method = nashChen;")
	WRITE_LINE("system.scattering.method = cayley;")
	WRITE_LINE("system.scattering.method = periodicCayley;")
	WRITE_LINE("GaussianCenter = -2.0;")
	WRITE_LINE("GaussianSpread = 0.25;")

	_bstr_t bstrV=\
L"\r\nfunction Psi0(x$)"
L"\r\n{"
L"\r\nreturn gaussianNormalized(GaussianSpread, x - GaussianCenter);"
L"\r\n}";

	Fire_WriteCommand(bstrV);
	WRITE_LINE("system.scattering.waveNumber = 0.2*PI;")
	WRITE_LINE("system.scattering.incident = \"Psi0\";")
	WRITE_LINE("system.state.scattering = true;")
//	CInfoDialog* pCElectroStaticsDialog = new CInfoDialog(_T("Scattering1Dim.htm"), _T("Scattering1Dim"));
//	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->m_CleanupOnInitialize.Add(pCElectroStaticsDialog->m_hWnd);
///////////////////////////////////////////////////////////////////////////////
#if 0
	WRITE_LINE("function U(x$) {"
"q=6.0*(x);"
"Y=0.1;"
"	return ( (1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + Y*Y*(0.25+6.25*exp(-q*q)) - (1+Y*Y*(0.25+6.25)) );"
	"}")

	WRITE_LINE("system.potential.data = 't.dat';")
//	WRITE_LINE("system.potential.nargs = 1;")


	WRITE_LINE("function V(x$) {"
"	return (Chebev(x));"
"	return (Spline(x));"
	"}")

	WRITE_LINE("function V2(x$) {"
"	return (sign(x)*( unitStep(x) - x*exp(-0.25*x*x) ) );"
	"}")

	WRITE_LINE("function U(x$) {"
"	return (sign(x)*( unitStep(x) - (4.0 - sign(x))* x^2*exp(-0.25*x*x) ) );"
	"}")

	WRITE_LINE("function U3(x$) {"
"	return (sign(x)*( unitStep(x) - (4.0 - sign(x))* x*exp(-0.25*x*x) ) );"
	"}")


	WRITE_LINE("function U2(x$) {"
"	return (x*sign(x)*( unitStep(x) - (4.0 - sign(x))* x*exp(-0.25*x*x) ) );"
	"}")

	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
#endif

//	WRITE_LINE("system.numPoints = 501;")
//	WRITE_LINE("system.potential = true;")//
//	WRITE_LINE("system.potential.show = true;")//
///////////////////////////////////////////////////////////////////////////////
	
}


void CGenExamp::Superposition()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"Superposition\";")
	WRITE_LINE("system.boundingBox = [ -4, 4, -1.5, 2.5, 4, 12 ] ;")
	WRITE_LINE("system.translate.z =  -3;")
	WRITE_LINE("system.rotate.x =  30;")
	WRITE_LINE("system.rotate.y =  -4;")
	WRITE_LINE("system.oneDimensionalWorld.superposition = true;")
#if 0
	if (theSystem->m_pISuperposition)
	{
		theSystem->m_pISuperposition.Release();
		theSystem->m_pISuperposition = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_Superposition,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_ISuperposition,
									(void**)&theSystem->m_pISuperposition);


	if (SUCCEEDED(hr))
	{
		theSystem->m_currentLayout = _Superposition;
		theSystem->m_pISuperposition->SetViewPointer((long) this);
		theSystem->m_pISuperposition->AdviseSource();
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_Superposition));
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Hide(LONG(_Superposition));//hides all of classical physics ui
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrSuperposition2);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrSuperposition);
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif

	
}
///////////////////////////////////////////////////////////////////////////////
#if 0
///////////////////////////////////////////////////////////////////////////////
// Specialized derivation to handle "button" click
//
class CMyHtmlCtrl : public CHtmlCtrl {
	virtual void OnAppCmd(LPCTSTR lpszWhere);
};

///////////////////////////////////////////////////////////////////////////////
// Handle "app:ok" link by closing dialog
//
void CMyHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	if (_tcsicmp(lpszWhere,_T("ok"))==0) {
		GetParent()->SendMessage(WM_COMMAND,IDOK);
	}
}
///////////////////////////////////////////////////////////////////////////////
// About dialog uses HTML control to display contents.
//
class CAboutDialog : public CDialog {
	DECLARE_DYNAMIC(CAboutDialog)
protected:
	CMyHtmlCtrl m_page;
	virtual BOOL OnInitDialog(); 
public:
	CAboutDialog() : CDialog(IDD_HELPBOX, NULL) { }
};

IMPLEMENT_DYNAMIC(CAboutDialog, CDialog)

BOOL CAboutDialog::OnInitDialog()
{
	VERIFY(CDialog::OnInitDialog());
	VERIFY(m_page.CreateFromStatic(IDC_HTMLVIEW, this));
	m_page.LoadFromResource(_T("movie.htm"));
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
//////////////////
HWND ElectroStaticshWnd  = 0;
///////////////////////////////////////////////////////////////////////////////
//	CElectroStaticsDialog* pCElectroStaticsDialog=new CElectroStaticsDialog();
//	HWND ElectroStaticshWnd  = pCElectroStaticsDialog->m_hWnd;
// Specialized derivation to handle "button" click
//
class CElectroStaticsHtmlCtrl : public CHtmlCtrl {
	virtual void OnAppCmd(LPCTSTR lpszWhere);
};

///////////////////////////////////////////////////////////////////////////////
// Handle "app:ok" link by closing dialog
//
void CElectroStaticsHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	if (_tcsicmp(lpszWhere,_T("ok"))==0) {
		GetParent()->SendMessage(WM_COMMAND,IDOK);
	}
}
///////////////////////////////////////////////////////////////////////////////
// About dialog uses HTML control to display contents.
//
class CElectroStaticsDialog : public CDialog {
	friend class CGLView;
	DECLARE_DYNAMIC(CElectroStaticsDialog)
protected:
	CElectroStaticsHtmlCtrl m_page;
	virtual BOOL OnInitDialog(); 
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP();
public:
	CElectroStaticsDialog(CWnd* pParent = NULL) : CDialog(IDD_HELPBOX, NULL) {
	CDialog::Create(IDD_HELPBOX, pParent);
	ShowWindow(SW_SHOW);
	}
};

IMPLEMENT_DYNAMIC(CElectroStaticsDialog, CDialog)
BEGIN_MESSAGE_MAP(CElectroStaticsDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////
void CElectroStaticsDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if ( 0 >= cx || 0 >= cy ){
		return;
	}
	if (!::IsWindow(m_page.m_hWnd)) return;
	::MoveWindow(m_page.m_hWnd, 0, 0, cx, cy, TRUE);
	m_page.SendMessage(WM_SIZE,SIZE_RESTORED, MAKELONG(cx, cy));
}

BOOL CElectroStaticsDialog::OnInitDialog()
{

	int ret = -1;
//	CenterWindow(CWnd::GetDesktopWindow());
	RECT Rect, deskRect;
	HWND desk_hwnd = ::GetDesktopWindow();
	::GetWindowRect(desk_hwnd, &deskRect);
	::GetWindowRect(m_hWnd, &Rect);
	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;
	int y = 0; 
	int x = 0;
	::MoveWindow(m_hWnd, x, y, w, h, TRUE);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);//HWND_TOP
	VERIFY(CDialog::OnInitDialog());
	VERIFY(m_page.CreateFromStatic(IDC_HTMLVIEW, this));
	m_page.LoadFromResource(_T("electrostatics.htm"));
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
void CElectroStaticsDialog::OnCancel()
{
	DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CElectroStaticsDialog::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////
#endif

void CGenExamp::OnElectroStaticField()
{
//	theApp.Initialize(_T("electrostatics.htm"), _T("ElectroStatics"));WaitForSingleObject(g_EventInitialize, INFINITE);
//	AfxGetApp()->PostThreadMessage(WM_APP_Initialize, (WPARAM) dupString("electrostatics.htm"), (LPARAM) dupString("ElectroStatics"));
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WM_APP_INIT2("electrostatics.htm", "ElectroStatics")


	WRITE_LINE("system.name = \"ElectroStatic Field\";")
	WRITE_LINE("system.dimension = 2;")
//	WRITE_LINE("system.rec.potential = true;")
//	theApp.GetGLView()->LoadTrashCan();

#if 0
//	
	CInfoDialog* pCElectroStaticsDialog = new CInfoDialog(_T("electrostatics.htm"), _T("ElectroStatics"));
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->m_CleanupOnInitialize.Add(pCElectroStaticsDialog->m_hWnd);

	if (theSystem->m_pIElectroStatics)
	{
		theSystem->m_pIElectroStatics.Release();
		theSystem->m_pIElectroStatics = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_ElectroStatics,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_IElectroStatics,
									(void**)&theSystem->m_pIElectroStatics);
	if (SUCCEEDED(hr))
	{
//		theSystem->m_pIElectroStatics->SetViewPointer((long) this);
		theSystem->m_pIElectroStatics->AdviseSource();
		theSystem->m_currentLayout = _ClassicalPhysics;
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_ClassicalPhysics));
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrElectroStatics);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrElectroStatics);
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif
	
}

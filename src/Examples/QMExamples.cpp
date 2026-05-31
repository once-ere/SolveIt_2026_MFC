// QMExamples.cpp : Implementation of CIO
#include "stdafx.h"
#include "SolveIt.h"

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
#include "ScriptColors.h"
//#include "GenExamp.h"

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
void CSolveItView::Scattering2Dim()
{
	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN2SPATIALDIMENSION)//theApp.Initialize();
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
//-(0.0304606)*(1.0/r)*(1+(-31.372180524934922 + (1.2086890121425404 - 15.805887623727772*r)*r)/((5.390816078738203 + (-0.9324844369365365 + r)*r)*(5.819560539019174 + r*(4.163008496880381 + r))))
//1.1519656883602023263824206494013678441305244750397

/*
	std::wstring bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"\r\nreturn -(0.0304606)*(1.0/r)*(1+(-31.372180524934922 + (1.2086890121425404 - 15.805887623727772*r)*r)/((5.390816078738203 + (-0.9324844369365365 + r)*r)*(5.819560539019174 + r*(4.163008496880381 + r))));"
L"\r\n}";


	std::wstring bstrV2a=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"if (r <= 1.15196568836020232638242) {"
L"	return  ( -(0.0304606)*0.25*pow(r,-0.25) );"
L"};\r\n"
L"\r\nreturn -(0.0304606)*((0.333333333)*(1.0/r^(0.333333333)) - 0.0766667);"
L"\r\n}";



	std::wstring bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"if (r <= 1.15196568836020232638242) {"
L"	return  ( -(0.0304606)*(1.0/(4.0*pow(r,0.25))) );"
L"};\r\n"
L"\r\nreturn (-(0.0304606)*(1.0/(3.0*pow(r,0.333333333333333333)) - 0.0766666666666666667));"
L"\r\n}";

	std::wstring bstrV2=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"if (r <= 1.15196568836020232638242) {"
L"	return  ( -(1.0/(4.0*pow(r,0.25))) );"
L"};\r\n"
L"\r\nreturn (-(1.0/(3.0*pow(r, 1.0/3.0)) - 23.0/300.0));"
L"\r\n}";

	std::wstring bstrV2=\
L"\r\nfunction V(r$)"
L"\r\n{"
L" q=1.0/3.0;"
L"if (r <= 1.15196568836020232638242) {"
L"	return ( -( (1.0/4.0)*pow(r, -(1.0/4.0)) ));"
L"};\r\n"
L"  return ( -( (1.0/3.0)*pow(r, -(1.0/3.0)) - 23.0/300.0));"
L"\r\n}";


	std::wstring bstrV2=\
L"\r\nfunction V(r$)"
L"\r\n{"
L" q=1.0/3.0; s=1.0/4.0;"
L"if (r <= 1.15196568836020232638242) {"
L"	return ( -( s*pow(r, -s) ));"
L"};\r\n"
L"  return ( -( q*pow(r, -q) - 23.0/300.0));"
L"\r\n}";

	std::wstring bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"if (r <= 1.15196568836020232638242) {"
L"	return  ( -(0.0304606)*(1.0/(4.0*pow(r,0.25))) );"
L"};\r\n"
L"\r\nreturn (-(0.0304606)*(1.0/(3.0*pow(r,0.333333333333333333)) - 0.0766666666666666667));"
L"\r\n}";

	std::wstring bstrV2=\
L"function V(r$)"
L"\r\n{"
L"\r\nif (r <= 1.15196568836020232638242) {"
L" return ( -( (1.0/4.0)*pow(r, -(1.0/4.0)) ));"
L" };\r\n"
L"return ( -( (1.0/3.0)*pow(r, -(1.0/3.0)) - 23.0/300.0));"
L"\r\n}";


*/
/////////////////////////////////////////////////////////////////////////////
void CSolveItView::QM_Ground_State_2DimBEC()
{
//	WM_APP_INIT_2("sledge.htm","Eigenvalue Problem")
	WM_APP_INIT_2(0,0)

	WRITE_LINE("system.name = \"L=0 Eigenvalue Problem\";")
	WRITE_LINE("system.dimension = 1;")

	WRITE_LINE("system.boundingBox = [ 0, 100, -.1, .1, -1, 1 ] ;")
//	WRITE_LINE("system.boundingBox = [ 0, 10, -1, 1, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")
	WRITE_LINE("system.nodes = 128;")//FFT

//0.0931783

	std::wstring bstrV2=\
L"function V(r$)"
L"\r\n{"
L"\r\nif (r <= 1.0) {"
L" return ( -0.5*pow(r, -(19.0/64.0) ) );"
L" };\r\n"
L" return ( -0.5*pow(r, -(49.0/64.0) ) );"
L"\r\n}";

	std::wstring bstrV3=\
L"function V(r$)"
L"\r\n{"
L"\r\nif (r <= 1.0) {"
L" return ( -0.5*0.0931783*pow(r, -(19.0/64.0) ) );"
L" };\r\n"
L" return ( -0.5*0.0931783*pow(r, -(49.0/64.0) ) );"
L"\r\n}";


	std::wstring bstrV=\
L"function V(r$)"
L"\r\n{"
L" return -0.0931783/( 0.005105158639020407*(0.8209178912839936 + r)*(238.6113166148295 + (r - 14.186872366069968)*r));"
L"\r\n}";

//	WriteCommand(bstrV);
	WRITE_LINEW(bstrV.c_str());
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("V(0.007);");
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("plot(\"V\", \" [x, 0.2, 10], [y ,  -10, 1] \");");
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.state.mass;")
	WRITE_LINE("system.state.mass = 1;")
	WRITE_LINE("system.state.scale = 15;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.domain.end = infinity;")
	WRITE_LINE("system.domain.begin = -infinity;")
//	WRITE_LINE("system.stationary.method = Cartesian;")
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.stationary = true;")

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSolveItView::QM_Ground_State_1Dim()
{
	WM_APP_INIT_2("sledge.htm","Eigenvalue Problem")

	WRITE_LINE("system.name = \"2D Hydrogen\";")
	WRITE_LINE("system.dimension = 1;")

	WRITE_LINE("system.boundingBox = [ 0, 10, -5, 5, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")

	std::wstring bstrV=\
L"\r\nfunction V(r$)"
L"\r\n{"
L"\r\nreturn -2.0/r;"
L"\r\n}";

//	WriteCommand(bstrV);
	WRITE_LINEW(bstrV.c_str());
///////////////////////////////////////////////////////////////////////////////
/*
	WRITE_LINE("function V(r$) {"
"	return -2.0/r;"
"}")
*/
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("V(0.7);");
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("plot(\"V\", \" [x, 0.2, 10], [y ,  -10, 1] \");");
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.state.mass;")
	WRITE_LINE("system.state.mass = 1;")
	WRITE_LINE("system.state.scale = 30;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.domain.end = infinity;")
	WRITE_LINE("system.domain.begin = -infinity;")
//	WRITE_LINE("system.stationary.method = Cartesian;")
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.stationary = true;")

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::QMStationary1Dim()
{
//	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, -2.5, 2.5, -1, 1 ] ;")
	//Fire_ClearUserDefs();
	
	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_1_SPATIALDIMENSION)//theApp.Initialize();
	WRITE_LINE("system.name = \"1-Dim Eigenstate\";")
	WRITE_LINE("system.dimension = 1;")
//

	std::wstring bstrQ=\
L"function Q(r$)"
L"\r\n{"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nreturn 4*a*R/((a+R)^2+d^2);"
L"\r\n}";

	WRITE_LINEW(bstrQ.c_str());
//L"\r\nif (r == 0.0) return (-0.00556);"
//L"\r\nif (r == 0.0) return (-0.00548344);"

	std::wstring bstrV=\
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

	WRITE_LINEW(bstrV.c_str());
	WRITE_LINE("system.boundingBox = [ 0, 1000, -0.05, 0.05, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")
	WRITE_LINE("plot(\"V\", \" [x,0, 1000], [y ,  -.012, .008] \");");
	WRITE_LINE("system.state.mass = 25.0/3;")
	WRITE_LINE("system.state.scale = 0.03;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.domain.end = infinity;")
//	WRITE_LINE("system.domain.begin = -infinity;")//
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = pseudoCartesian;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.state.energy;")

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::QMStationary1DimB()
{
//	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, -2.5, 2.5, -1, 1 ] ;")
	//Fire_ClearUserDefs();
	
	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_1_SPATIALDIMENSIONB)//theApp.Initialize();
	WRITE_LINE("system.name = \"1-Dim Eigenstate\";")
	WRITE_LINE("system.dimension = 1;")
//

	std::wstring bstrQ=\
L"function Q(r$)"
L"\r\n{"
L"\r\nL0=0.00573645;"
L"\r\nR=L0*r;"
L"\r\na          = 2;"
L"\r\nd          = 0.2;"
L"\r\nreturn 4*a*R/((a+R)^2+d^2);"
L"\r\n}";

	WRITE_LINEW(bstrQ.c_str());
//L"\r\nif (r == 0.0) return (-0.00556);"

	std::wstring bstrV=\
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

	WRITE_LINEW(bstrV.c_str());
	WRITE_LINE("system.boundingBox = [ 0, 1000, -0.05, 0.05, -1, 1 ] ;")
	WRITE_LINE("system.translate = [0,0,0];")
	WRITE_LINE("plot(\"V\", \" [x,0, 1000], [y ,  -.012, .008] \");");
	WRITE_LINE("system.state.mass = 25.0/3;")
	WRITE_LINE("system.state.scale = 0.03;")
	WRITE_LINE("system.state.eigenNumber = 0;")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.domain.end = infinity;")
//	WRITE_LINE("system.domain.begin = -infinity;")//
	WRITE_LINE("system.domain.begin = 0;")
	WRITE_LINE("system.stationary.method = pseudoCartesian;")
	WRITE_LINE("system.stationary.method = radial;")
	WRITE_LINE("system.state.stationary = true;")
	WRITE_LINE("system.state.energy;")

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::QMStationary2Dim()
{
	//Fire_ClearUserDefs();
//	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_2_SPATIALDIMENSION)
		
/*
	CreateGenExampThread(true);
	if (m_pCGenExamp)
	{
		m_pCGenExamp->ResumeThread();
		m_pCGenExamp->PostThreadMessage((ID_POPUP_QUANTUMMECHANICS_STATIONARY_IN_2_SPATIALDIMENSION), 0, 0);
	}
//		AfxMessageBox(L"1");
	return;
*/

	WM_APP_INIT_2(0,0)

	//theApp.Initialize();
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
#if 0
	WRITE_LINE("lightenDarken = 0.7;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("function V(x$, y$) {"
//"q=scaleX*(x);"
//"Y=scaleX*(y);"
//"	return ( (1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + Y*Y*(0.25+6.25*exp(-q*q)) - 1.0 );"
//"	return ( scaleA*(1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + y*y*(1.0+scaleE*exp(-q*q)) - scaleA );"
//	"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function V(x$, y$) {"
"	return ( (1 - x/4)*(1 + x/4)*(1 - x/4)*(1 + x/4) + y*y*(0.25+6.25*exp(-x*x)));"
"}")
///////////////////////////////////////////////////////////////////////////////
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
#endif
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

	return;

	std::wstring bstr = L"Periodic boundary conditions are applied.";
	Fire_WriteResponse(bstr.c_str());//Fire_WriteResponse(0);

	bstr = L"Double-click in the 'System' window [and wait] to toggle through the views of the different eigenstates.";
	Fire_WriteResponse(bstr.c_str());////Fire_WriteResponse(0);

//	bstr = L".";
//	Fire_WriteResponse(bstr.c_str());//Fire_WriteResponse(0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::Scattering1Dim()
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


//	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_SCATTERINGIN1SPATIALDIMENSION)//theApp.Initialize();


	WM_APP_INIT_2("Scattering1Dim.htm", "Scattering1Dim")



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

	std::wstring bstrV=\
L"\r\nfunction Psi0(x$)"
L"\r\n{"
L"\r\nreturn gaussianNormalized(GaussianSpread, x - GaussianCenter);"
L"\r\n}";

	WRITE_LINEW(bstrV.c_str());
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


void CSolveItView::Superposition()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_QUANTUMMECHANICS_SUPERPOSITION)//theApp.Initialize();
	WRITE_LINE("system.name = \"Superposition\";")
	WRITE_LINE("system.boundingBox = [ -4, 4, -1.5, 2.5, 4, 12 ] ;")
	WRITE_LINE("system.translate.z =  -3;")
	WRITE_LINE("system.rotate.x =  30;")
	WRITE_LINE("system.rotate.y =  -4;")
	WRITE_LINE("system.superposition = true;")
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
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );
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

void CSolveItView::OnElectroStaticField()
{
	//Create_GenExampThread(ID_POPUP_ELECTROMAGNETIC_ELECTROSTATICFIELD)//theApp.Initialize();

	WM_APP_INIT_2("electrostatics.htm", "ElectroStatics")

	WRITE_LINE("system.name = \"ElectroStatic Field\";")
	WRITE_LINE("system.dimension = 2;")
//	WRITE_LINE("system.rec.potential = true;")
//	theApp.GetGLView()->LoadTrashCan();
//	
#if 0
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
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif
	
}

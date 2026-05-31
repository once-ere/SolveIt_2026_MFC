//ScriptColors.h
#if !defined(___ScriptColors_h_)
#define ___ScriptColors_h_
//#include "System.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//#define WRITE_LINE(s)  
//{
//std::wstring bstr(s);
//WriteLine(bstr, 1);
//}
//Lock lock(theApp.theSystem.protectSoTransforms);
//#define WRITE_LINE(s)  {std::wstring bstr(s);Fire_WriteCommand(bstr.c_str());}
///////////////////////////////////////////////////////////////////////////////
#define WRITE_LINE(a) \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	app->PostThreadMessage(WM_USER_POST_COMMAND, 0, (LPARAM) GetWC(a));\
	}

///////////////////////////////////////////////////////////////////////////////
#define WRITE_LINEW(a) \
	{\
	CWinApp* app = AfxGetApp();\
	if (app == NULL) throw SolveIt_ERROR(L"CWinApp::WM_APP_Initialize: CWinApp* == 0");\
	app->PostThreadMessage(WM_USER_POST_COMMAND, 0, (LPARAM) a);\
	}

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

#define Create_GenExampThread(xxx)	\
	{\
	WM_APP_INIT_2(0,0) \
	}

/////////////////////////////////////////////////////////////////////////////


#define WRITE_Begin(s) {std::wstring bstr(s);WriteBegin(bstr);}
#define WRITE_Add(s) {std::wstring bstr(s);WriteAdd(bstr);}
#define WRITE_End(s) {std::wstring bstr(s);WriteEnd(bstr);}
//#define WRITE_LINE(s)  {std::wstring bstr(s);bstr+=CrLf;WriteCommand(bstr);}
#define WRITE_LINES(s)  {std::wstring bstr(s);WriteCommand(bstr);}


//#define WRITE_LINE(s)  {std::wstring bstr(s);WriteCommand(bstr);}
//#define WRITE_LINE(s)  {Fire_WriteCommand(s);}
//#define WRITE_LINE(s)  {wstring bstr(s);Fire_WriteCommand(bstr.c_str());}

///////////////////////////////////////////////////////////////////////////////
#define Gold			".color = [ 0.7490, 0.6353, 0.3843 ];"
#define NeonPurple		".color = [ 0.7255, 0.3098, 0.5608 ];"
#define NeonRed			".color = [ 0.8627, 0.1529, 0.2941 ];"
#define HotPink			".color = [ 0.8941, 0.3412, 0.4118 ];"
#define ElectricBlue	".color = [ 0.4314, 0.3686, 0.6235 ];"
#define PANTONERhodamineRedCV	".color = [ 0.9255, 0.0000, 0.5725 ];"
#define PANTONERubineRedCV	".color = [ 0.8157, 0.0000, 0.3418 ];"
#define PANTONEOrange021CV	".color = [ 0.9372, 0.4235, 0.0000 ];"
#define PANTONEYellowCV		".color = [ 0.9882, 0.8824, 0.0863 ];"
#define PANTONEPurpleCV		".color = [ 0.7529, 0.1882, 0.7059 ];"
//tan:
#define PANTONE731CV	".color = [ 0.4549, 0.2510, 0.0392 ];"
//blues:
#define PANTONE2727CV	".color = [ 0.372549, 0.411765, 0.768627 ];"
#define PANTONE2718CV	".color = [ 0.364706, 0.470588, 0.796078 ];"
#define PANTONE2728CV	".color = [ 0.188235, 0.274510, 0.705882 ];"
#define PANTONEPANTONEBlue072CV		".color = [ 0.215686, 0., 0.592157 ];"
//purples:
#define PANTONE512CV	".color = [ 0.521569, 0.133333, 0.423529 ];"
#define PANTONE513CV	".color = [ 0.619608, 0.141176, 0.529412 ];"
#define PANTONE527CV	".color = [ 0.482353, 0.117647, 0.603922 ];"
#define PANTONE259CV	".color = [ 0.450980, 0.090196, 0.423529 ];"
#define PANTONE2592CV	".color = [ 0.576471, 0.062745, 0.647059 ];"
#define PANTONE2612CV	".color = [ 0.443137, 0.121569, 0.454902 ];"

#define MyBrown			".color = [ 0.5, 0.3, 0.1 ];"
#define MyBrown2		".color = [ 0.5608, 0.3372, 0.2667 ];"
#define MyOrange		".color = [ 1, 0.4, 0 ];"
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif

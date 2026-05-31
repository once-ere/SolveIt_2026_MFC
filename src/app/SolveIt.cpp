/*
IN "parser_2020.h"
YOU MUST PERFORM THIS EDIT:
/# Tokens.  #/
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/# Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  #/
#include <cstdint>
enum yytokentype : std::uint64_t {
	...
*/

// SolveIt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildFrm2.h"
#include "SolveItDoc.h"
#include "LeftView.h"
#include "GLView.h"
#include "StackMachine.h"
#include "SymbolTable.h"
#include "SolveItView.h"
#include "RigidBody/ConstraintManager.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// ./lib
// f2cd.lib ZLibd.lib PNGd.lib opengl32.lib glu32.lib 
// f2c.lib  ZLib.lib  PNG.lib  opengl32.lib glu32.lib 
// MesaGL.lib MesaGLU.lib osmesa.lib
// opengl32.lib glu32.lib 

///////////////////////////////////////////////////////////////////////////////
HANDLE	SolveIt::g_EventHandle		= 0;
HANDLE	SolveIt::g_EventInitialize	= 0;
///////////////////////////////////////////////////////////////////////////////
HANDLE	SolveIt::g_EventStop=0;
HANDLE	SolveIt::g_EventStep=0;
HANDLE	SolveIt::g_EventRun=0;
HANDLE	SolveIt::g_EventKill=0;
///////////////////////////////////////////////////////////////////////////////
wchar_t SolveIt::workingDirectory[32] = L"\\work\\";


extern const wchar_t scratchFileName[];
extern const wchar_t scratchBmpName[];
wstring		SolveIt::g_szWorkDir;		//no terminal '\'
wstring		SolveIt::g_szWorkDir2;		//with terminal '\'
wstring		SolveIt::g_szWorkDirFileName;
///////////////////////////////////////////////////////////////////////////////
// CSolveItApp

BEGIN_MESSAGE_MAP(CSolveItApp, CWinApp)
	//{{AFX_MSG_MAP(CSolveItApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CSolveItApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_REGISTERED_MESSAGE(WM_APP_Initialize, reinterpret_cast<LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM)>(OnUSER_Initialize))
	ON_REGISTERED_MESSAGE(WM_USER_POST_COMMAND, reinterpret_cast<LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM)>(OnUserCommand))
END_MESSAGE_MAP()
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

	//ON_REGISTERED_MESSAGE(WM_APP_Initialize, OnUSER_Initialize)
	//ON_REGISTERED_MESSAGE(WM_USER_POST_COMMAND, OnUserCommand)
/////////////////////////////////////////////////////////////////////////////
// CSolveItApp construction

///////////////////////////////////////////////////////////////////////////////
extern "C" {
	wchar_t DATA_PASS_mp_WORK_DIR[_MAX_PATH] = L"\0";
}
/////////////////////////////////////////////////////////////////////////////////
CSolveItApp::CSolveItApp():
//	m_hLibRichEdit(0),
	m_pEditViewTemplate(0),
	m_pGLViewTemplate(0)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

///////////////////////////////////////////////////////////////////////////////
// The one and only CSolveItApp object
///////////////////////////////////////////////////////////////////////////////
CSolveItApp SolveIt::theApp;
///////////////////////////////////////////////////////////////////////////////
bool CSolveItApp::g_boolDoReset=false;
///////////////////////////////////////////////////////////////////////////////

BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
	DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);


/*
template<typename BaseType, class StringTraits>
class CStringT :
	public CSimpleStringT<BaseType,
		_CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits>

typedef ATL::CStringT< wchar_t, StrTraitMFC< wchar_t > > CStringW;
typedef ATL::CStringT< char, StrTraitMFC< char > > CStringA;
typedef ATL::CStringT< TCHAR, StrTraitMFC< TCHAR > > CString;


// typedef CStringT<TCHAR, StrTraitATL<TCHAR, ChTraitsCRT<TCHAR>>> CAtlString;

CAtlString str(_T("Soccer is best, but hockey is quicker!"));
_tprintf_s(_T("Before: %s\n"), (LPCTSTR)str);

int n = str.Delete(6, 3);
_tprintf_s(_T("After: %s\n"), (LPCTSTR)str);
ASSERT(n == str.GetLength());
*/




void CSolveItApp::OnFileOpen()
{
	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

//	AfxGetApp()->OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted
	GetEditView()->m_ScriptFileName = GetC((LPCTSTR)newName);
//	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
//		return FALSE;
	Initialize();
	GetEditView()->ReadFileScript();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {9ADB6671-C9E2-40E9-B41C-6878164A29B8}
//static const CLSID clsid =
//{ 0x9adb6671, 0xc9e2, 0x40e9, { 0xb4, 0x1c, 0x68, 0x78, 0x16, 0x4a, 0x29, 0xb8 } };

/////////////////////////////////////////////////////////////////////////////
// CSolveItApp initialization

FILE *stream_stdout=0;
FILE *stream_stderr=0;

BOOL CSolveItApp::InitInstance()
{

/*
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(L"IDP_OLE_INIT_FAILED");
		return FALSE;
	}
*/

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();
	// Initialize OLE libraries
//	if (!AfxOleInit())
//	{
//		AfxMessageBox(IDP_OLE_INIT_FAILED);
//		return FALSE;
//	}

	AfxEnableControlContainer();
   INITCOMMONCONTROLSEX icex;
	
// Ensure that the common control DLL is loaded. 
   icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icex.dwICC  = ICC_BAR_CLASSES;
   InitCommonControlsEx(&icex);
/*
*/

//	m_hLibRichEdit = LoadLibrary(_T("RICHED32.DLL"));



	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this wstring to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("SolveIt"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

///////////////////////////////////////////////////////////////////////////////
	DWORD res = 0;

	{
		TCHAR		szAppPath[MAX_PATH];
		TCHAR		ch = (TCHAR)'\\';
		HRSRC		hRC = 0;

		GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
		CString		sAppPath(szAppPath);
		int			n = sAppPath.ReverseFind(ch);
		CString		szDefaultDir = sAppPath.Left(n);			//if n+1 then keep '\'
		szDefaultDir += workingDirectory;
		g_szWorkDir2 = szDefaultDir;		//with terminal '\'
		n = szDefaultDir.GetLength();
		CString		szWorkDir = szDefaultDir.Left(n - 1);		//discard terminal '\'
		g_szWorkDir = szWorkDir;		//no terminal '\'

		res = GetFileAttributes(szWorkDir);
		if (res == INVALID_FILE_ATTRIBUTES)
		{
			res = CreateDirectory(szWorkDir, 0);
			ASSERT(res);
		}
		CString		szWorkDir2 = szDefaultDir;
		g_szWorkDirFileName = szWorkDir2;
		g_szWorkDirFileName += _T("CONOUT.txt");

		CString		sz_stdout = szWorkDir2;
		sz_stdout += _T("stdout.txt");
		CString		sz_stderr = szWorkDir2;
		sz_stderr += _T("stderr.txt");

		freopen_s(&stream_stdout, GetC((wchar_t const*)sz_stdout), "w", stdout);
		freopen_s(&stream_stderr, GetC((wchar_t const*)sz_stderr), "w", stderr);

		//	stream_stdout = freopen( g_szWorkDirFileName.c_str(), "a+", stdout );
		//	stream_stderr = freopen( g_szWorkDirFileName.c_str(), "a+", stderr );

		//	stream_stdout = freopen( "stdout.txt", L"w", stdout );
		//	stream_stderr = freopen( "stderr.txt", L"w", stderr );

		ASSERT(stream_stdout);
		ASSERT(stream_stderr);
		//	fclose( stream_stdout );
		//	fclose( stream_stderr );

		for (auto i = 0; i < g_szWorkDirFileName.size(); i++) DATA_PASS_mp_WORK_DIR[i] = g_szWorkDirFileName[i];
		for (auto i = g_szWorkDirFileName.size(); i < _MAX_PATH; i++) DATA_PASS_mp_WORK_DIR[i] = ' ';
	}

	res = GetFileAttributes(scratchFileName);
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}
	res = GetFileAttributes(_T("CONOUT.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("CONOUT.txt"));
	}
	res = GetFileAttributes(_T("QMStationary1D.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("QMStationary1D.txt"));
	}
	res = GetFileAttributes(_T("QMStationary2D.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("QMStationary2D.txt"));
	}
	res = GetFileAttributes(_T("duh.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("duh.txt"));
	}
	res = GetFileAttributes(_T("duh2.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("duh2.txt"));
	}
	res = GetFileAttributes(_T("CONOUT.txt"));
	if (res != INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(_T("CONOUT.txt"));
	}
	///////////////////////////////////////////////////////////////////////////////
	//	WriteResourceToFile("del_rd.bmp",(LPCTSTR) _T("IDR_del_rd_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("NII_grn.bmp",(LPCTSTR) _T("IDR_NII_grn_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("h_blk.bmp",(LPCTSTR) _T("IDR_h_blk_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("schr_red.bmp",(LPCTSTR) _T("IDR_schr_red_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("psi_blu.bmp",(LPCTSTR) _T("IDR_psi_blu_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("inf_blk.bmp",(LPCTSTR) _T("IDR_inf_blk_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("torus.bmp",(LPCTSTR) _T("IDR_torus_BMP"), (LPCTSTR) _T("GIF"));
	//	WriteResourceToFile("ein_vio.bmp",(LPCTSTR) _T("IDR_ein_vio_BMP"), (LPCTSTR) _T("GIF"));
	WriteResourceToFile(L"t.act", (LPCTSTR)_T("t.act"), (LPCTSTR)_T("TEXT"));
	WriteResourceToFile(L"t.dat", (LPCTSTR)_T("t.dat"), (LPCTSTR)_T("TEXT"));


	{
		BOOL manual_reset = FALSE;
		BOOL initial_state_signaled = FALSE;
		g_EventInitialize = CreateEvent(0, manual_reset, initial_state_signaled, 0);
		g_EventStop = CreateEvent(0, manual_reset, initial_state_signaled, 0);
		g_EventStep = CreateEvent(0, manual_reset, initial_state_signaled, 0);
		g_EventRun = CreateEvent(0, manual_reset, initial_state_signaled, 0);
		g_EventKill = CreateEvent(0, manual_reset, initial_state_signaled, 0);
	}
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
#if 10
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	{	// BLOCK: doc template registration
		// Register the document template.  Document templates serve
		// as the connection between documents, frame windows and views.
		// Attach this form to another document or frame window by changing
		// the document or frame class in the constructor below.
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_GLVIEW_TMPL,
			RUNTIME_CLASS(CSolveItDoc),		// document class
			RUNTIME_CLASS(CChildFrame2),		//CMDIChildWnd frame class
			RUNTIME_CLASS(CGLView));		// view class
		AddDocTemplate(pNewDocTemplate);
		m_pGLViewTemplate = pNewDocTemplate;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SOLVEITYPE,
		RUNTIME_CLASS(CSolveItDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);
	m_pEditViewTemplate = pDocTemplate;



	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);//);
	pMainFrame->UpdateWindow();


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSolveItApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSolveItApp message handlers
/*
///////////////////////////////////////////////////////////////////////////////
int CSolveItApp::ExitInstance() 
{
//	m_pGLViewTemplate->CloseAllDocuments(true);
//	m_pEditViewTemplate->CloseAllDocuments(true);
//	if (m_hLibRichEdit) FreeLibrary(m_hLibRichEdit);
	return CWinApp::ExitInstance();
}
///////////////////////////////////////////////////////////////////////////////
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz); //Action::DupString
//requires a call to	//GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
//std::wstring  DupString(std::wstring const &  lpsz); //_tcsdup; memory leak
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItApp::Initialize(const wchar_t *html_file, const wchar_t *title)
{
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::Initialize: pFrame == 0");
//	pFrame->Initialize();
//	pFrame->SendMessage(WM_USER_Initialize, 0, 0);//
	pFrame->PostMessage(WM_USER_Initialize, 0, 0);//
	if (html_file != NULL)
	{
		pFrame->PostMessage(WM_USER_AddCInfoDialog, (WPARAM) /*GetWC*/(html_file), (LPARAM) /*GetWC*/(title));//
	//	pFrame->SendMessage(WM_USER_AddCInfoDialog, (WPARAM) html_file, (LPARAM)title);//
	}
//	CInfoDialog* pDialog = new CInfoDialog(_T("Mandelbrot.htm"), _T("Mandelbrot Set"));
//	pFrame->m_CleanupOnInitialize.Add(pDialog->m_hWnd);
	return;
//		WaitForSingleObject(g_EventInitialize, INFINITE);
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CSolveItApp::OnUSER_Initialize(WPARAM wParam, LPARAM lParam)
{
	try {
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::Initialize: pFrame == 0");
	wstring html_file(L"");
	wstring title(L"");
	if (wParam != NULL)
	{
		if (lParam == NULL) throw SolveIt_ERROR(L"CSolveItApp::Initialize: No title");
	//	html_file	= wstring((std::wstring )wParam);
	//	title		= wstring((std::wstring )lParam);
	//	//GlobalFree((void*)wParam);CMainFrame frees
	//	//GlobalFree((void*)lParam);CMainFrame frees
	//	pFrame->PostMessage(WM_USER_AddCInfoDialog, (WPARAM) GetWC(html_file), (LPARAM) GetWC(title));//
	}
	pFrame->PostMessage(WM_USER_Initialize, 0, 0);//
	if (wParam != NULL)
	{
		pFrame->PostMessage(WM_USER_AddCInfoDialog, wParam, lParam);//
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CSolveItApp::OnUSER_Initialize ",  GetWC( exc->what() ), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItApp::OnUSER_Initialize");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LRESULT CSolveItApp::OnUserCommand(WPARAM wParam, LPARAM lParam)
{
	try {
		if (lParam == NULL) throw SolveIt_ERROR(L"CSolveItApp::OnUserCommand: null");
	//	wstring command((const wchar_t*)lParam);
		std::wstring bstr((const wchar_t*)lParam);
		////GlobalFree((void*)lParam);
	//	Fire_WriteCommand(bstr.c_str());
		CSolveItView* edit = theApp.GetEditView();
		if (edit == NULL) throw SolveIt_ERROR(L"Fire_WriteCommand::: null CSolveItView*");
		edit->PostMessage(WM_USER_TextBoxWindow, 0, (LPARAM) /*GetWC*/(bstr.c_str()));//
}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CSolveItApp::OnUSER_Initialize ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItApp::OnUserCommand");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
CSymbolTable* CSolveItApp::GetSymbolTable()
{
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	CSystem*		m_system		= GetSystem();
	if (m_system->symbolTable == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null m_system->symbolTable");
	return m_system->symbolTable;
}
///////////////////////////////////////////////////////////////////////////////
CStackMachine* CSolveItApp::GetStackMachine()
{
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	CSystem*		m_system		= GetSystem();
	if (m_system->stackMachine == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null m_system->stackMachine");
	return m_system->stackMachine;
}
///////////////////////////////////////////////////////////////////////////////
bool CSolveItApp::GetSystem(CSystem* sys)
{
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	try{
	ASSERT (pFrame != NULL);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetSystem: null CMainFrame");
	ASSERT (pFrame->m_system != NULL);
	if (pFrame->m_system == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetSystem: null m_system");
	sys=pFrame->m_system;
	return true;
}
catch (wchar_t *str)// )
{
	::MessageBox(0, str, L"CSolveItApp::GetSystem", MB_OK);
		PostQuitMessage(1);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(0, L"error", L"CSolveItApp::GetSystem", MB_OK);
		PostQuitMessage(2);
}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
//CSystem* CSolveItApp::GetSystem()
CSystem* CSolveItApp::GetSystem()
{
//	CSystem* sys = 0;
//	if (!GetSystem(sys)) throw SolveIt_ERROR(L"CSolveItApp::GetSystem(): null");
//	return sys;
 //throw SolveIt_ERROR(L"pbody == 0");

	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	try{
	ASSERT (pFrame != NULL);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetSystem: null CMainFrame");
	ASSERT (pFrame->m_system != NULL);
	if (pFrame->m_system == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetSystem: null m_system");
	return pFrame->m_system;
}
catch (wchar_t *str)// )
{
	::MessageBox(0, str, L"CSolveItApp::GetSystem", MB_OK);
		PostQuitMessage(1);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(0, L"error", L"CSolveItApp::GetSystem", MB_OK);
		PostQuitMessage(2);
}
	return new CSystem;
}
///////////////////////////////////////////////////////////////////////////////
CRigidBodyManager* CSolveItApp::GetRigidBodyManager()
{
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	CSystem*		m_system		= GetSystem();
	if (m_system->rigidBodyManager == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null m_system->rigidBodyManager");
	return m_system->rigidBodyManager;
}
///////////////////////////////////////////////////////////////////////////////
CFieldsManager* CSolveItApp::GetFieldsManager()
{
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	CSystem*		m_system		= GetSystem();
	if (m_system->fieldsManager == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null m_system->fieldsManager");
	return m_system->fieldsManager;
}
///////////////////////////////////////////////////////////////////////////////
CConstraintManager* CSolveItApp::GetConstraintManager()
{
	CSystem*		m_system		= GetSystem();
	if (m_system->constraintManager == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null m_system->constraintManager");
	return m_system->constraintManager;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CGLView* CSolveItApp::GetGLView()
{
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	ASSERT (pFrame != NULL);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetSystem: null CMainFrame");
	CGLView* pv = pFrame->GetGLView();
	if (pv == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null CGLView*");
	return pv;//pFrame->GetGLView();
}
///////////////////////////////////////////////////////////////////////////////
CLeftView*		CSolveItApp::GetTreeView()
{
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	ASSERT (pFrame != NULL);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetTreeView: null CMainFrame");
	CLeftView*		lv = pFrame->GetTreeView();
	if (lv == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null CLeftView*");
	return lv;//pFrame->GetTreeView();
}
///////////////////////////////////////////////////////////////////////////////
CSolveItView*	CSolveItApp::GetEditView()
{
	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
	ASSERT (pFrame != NULL);
	if (pFrame == NULL) throw SolveIt_ERROR(L"CSolveItApp::GetEditView: null CMainFrame");
	CSolveItView*	ev = pFrame->GetEditView();
	if (ev == NULL) throw SolveIt_ERROR(L"CSolveItApp::: null CSolveItView*");
	return ev;//pFrame->GetEditView();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void Fire_WriteStatus(wchar_t* strText);
void Fire_ViewChange(void);
void Fire_ResetView(void);
void Fire_WriteCommand(wchar_t* strText);
void Fire_WriteResponse(wchar_t* strText);
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SolveIt::Fire_WriteStatus(wchar_t const* strText)
{
	try{
	std::wstring bstr(strText);
	((CFrameWnd *) AfxGetMainWnd())->SetMessageText(bstr.c_str());

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"SolveIt::Fire_WriteStatus ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"SolveIt::Fire_WriteStatus");
	}
}
///////////////////////////////////////////////////////////////////////////////
void SolveIt::Fire_ViewChange(void)
{
	try{
	CGLView* gl = theApp.GetGLView();
	if (gl == NULL) return;
	gl->Invalidate();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"SolveIt::Fire_ViewChange ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"SolveIt::Fire_ViewChange");
	}
}
///////////////////////////////////////////////////////////////////////////////
void SolveIt::Fire_ResetView(void)
{
	try{
	CGLView* gl = theApp.GetGLView();
	if (gl == NULL) return;
	gl->ResetView();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"SolveIt::Fire_ResetView ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"SolveIt::Fire_ResetView");
	}
}
///////////////////////////////////////////////////////////////////////////////
//void SolveIt::Fire_WriteCommand(wchar_t* strText)
void SolveIt::FireWriteCommand(const wchar_t* strText)
{
try {
	if (strText == NULL) throw SolveIt_ERROR(L"SolveIt::FireWriteCommand: null strText");
	wstring str(strText);
	//GlobalFree((void*)strText);
	CSolveItView* edit = theApp.GetEditView();
	if (edit == NULL) throw SolveIt_ERROR(L"Fire_WriteCommand::: null CSolveItView*");
	std::wstring bstr(str.c_str());
	edit->PostMessage(WM_USER_TextBoxWindow, 0, (LPARAM) /*GetWC*/(bstr.c_str()));//
//	edit->WriteCommand(strText);

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"SolveIt::Fire_WriteCommand ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"SolveIt::Fire_WriteCommand");
	}
}
///////////////////////////////////////////////////////////////////////////////
void SolveIt::Fire_WriteResponse(wchar_t const* strText)
{
	try {
	std::wstring bstr(strText);
	wstring out(bstr.c_str());
	CStackMachine::receiveCommandResponse = GetC(out);
//	objects o(out);
//	CStackMachine* stackMachine	= theApp.GetStackMachine();
//	stackMachine->stack[--stackMachine->top] = o;
//	theApp.GetEditView()->CreateOutputWindow(out);
//	std::wstring msg(L"\"");
//	msg += bstr;
//	msg += L"\";";
//	Fire_WriteCommand(msg);

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"SolveIt::Fire_WriteResponse ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"SolveIt::Fire_WriteResponse");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CSolveItApp::ExitInstance() 
{
	CloseHandle (g_EventInitialize);
	CloseHandle (g_EventStep);
	CloseHandle (g_EventStop);
	CloseHandle (g_EventRun);
	CloseHandle (g_EventKill);
	g_EventInitialize	= 0;
	g_EventStep	= 0;
	g_EventStop	= 0;
	g_EventRun	= 0;
	g_EventKill	= 0;
	fclose( stream_stdout );
	fclose( stream_stderr );
	FILE* console_stdout = 0;
	freopen_s(&console_stdout, "CON", "w", stdout);
	int j=0;
	DWORD res = GetFileAttributes(scratchFileName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}
/*
	CActionView * pView = GetActionView();
	if (pView && pView->m_hWnd && ::IsWindow(pView->m_hWnd))
	{
		pView->GetParent()->PostMessage(WM_CLOSE, 0, 0);
	//	pView->GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
	//	pView->GetParentFrame()->SendMessage(WM_QUIT, 0, 0);
	}
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;//
	pMainFrame->SendMessage(WM_CLOSE, 0, 0);//
*/
//	pMainFrame->SendMessage(WM_QUIT, 0, 0);
	return CWinApp::ExitInstance();
}
///////////////////////////////////////////////////////////////////////////////

long CSolveItApp::WriteResourceToFile(	LPCTSTR filename, 
										LPCTSTR lpName,  // resource name
										LPCTSTR lpType   // resource type
										)
{
	try{
	TCHAR		szAppPath[MAX_PATH];
	TCHAR		ch				= (TCHAR)'\\';
	HRSRC		hRC				= 0;	

	GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	CString		sAppPath(szAppPath);
	int			n				= sAppPath.ReverseFind( ch );
	CString		szDefaultDir	= sAppPath.Left(n);			//if n+1 then keep '\'
				szDefaultDir	+= workingDirectory;
				CString		sAdminPath		= szDefaultDir;
				sAdminPath		+= filename;

//	AfxMessageBox(sAdminPath);
	if ( GetFileAttributes(sAdminPath) != (DWORD)0xFFFFFFFF ) return 0;


	hRC = FindResource(AfxGetInstanceHandle(), lpName, lpType);	
	if (hRC) {
//		HGLOBAL hgl = LoadResource(NULL, hRC);
		BYTE* hgl = (BYTE*) LoadResource(NULL, hRC);
		UINT nBufferSize = SizeofResource(NULL, hRC);
		if (hgl) {
//			BYTE* str = (BYTE*) LockResource(hgl);//no op
//			if (str) {
			ofstream ofs((const wchar_t*)sAdminPath, ios::out|ios::binary);
			if (ofs) {
				for (UINT i=0;i<nBufferSize;i++) ofs << hgl[i];// << str[i];// << fflush;
				ofs.close();
			}
//			}else {AfxMessageBox(_T(": LockResource failed"));return FALSE;}
		}else {
			AfxMessageBox(_T(": LoadResource failed"));
			return -1;
		}
	}else {
		DWORD n = GetLastError();
		CString sz;
		sz.Format(L"FindResource failed: %u",n);
		AfxMessageBox(sz);
		return -2;
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CSolveItApp::WriteResourceToFile ", GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItApp::WriteResourceToFile");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LPVOID SolveIt::DisplayErrorText()
{
LPVOID lpMsgBuf=0;
if (!FormatMessage( 
	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	FORMAT_MESSAGE_FROM_SYSTEM | 
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	GetLastError(),
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	(LPTSTR) &lpMsgBuf,
	0,
	NULL ))
{
   // Handle the error.
   return 0;
}

// Process any inserts in lpMsgBuf.
// ...

// Display the wstring.
//MessageBox( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );

// Free the buffer.
//LocalFree( lpMsgBuf );
return( lpMsgBuf );
}
///////////////////////////////////////////////////////////////////////////////
#include <lmerr.h>

void
DisplayErrorText(
	DWORD dwLastError
	)
{
	HMODULE hModule = NULL; // default to system source
	std::wstring  MessageBuffer;
	DWORD dwBufferLength;

	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM ;

	//
	// If dwLastError is in the network range, 
	//  load the message source.
	//

	if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
		hModule = LoadLibraryEx(
			TEXT("netmsg.dll"),
			NULL,
			LOAD_LIBRARY_AS_DATAFILE
			);

		if(hModule != NULL)
			dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
	}

	//
	// Call FormatMessage() to allow for message 
	//  text to be acquired from the system 
	//  or from the supplied module handle.
	//

	if(dwBufferLength = FormatMessage(
		dwFormatFlags,
		hModule, // module to get message from (NULL == system)
		dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
		(LPTSTR) &MessageBuffer[0],
		0,
		NULL
		))
	{
		DWORD dwBytesWritten;

		//
		// Output message wstring on stderr.
		//
		WriteFile(
			GetStdHandle(STD_ERROR_HANDLE),
			(LPTSTR)&MessageBuffer[0],
			dwBufferLength,
			&dwBytesWritten,
			NULL
			);

		//
		// Free the buffer allocated by the system.
		//
		//LocalFree(MessageBuffer);
	}

	//
	// If we loaded a message source, unload it.
	//
	if(hModule != NULL)
		FreeLibrary(hModule);
}
///////////////////////////////////////////////////////////////////////////////

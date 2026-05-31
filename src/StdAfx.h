// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__16E865D3_14A8_4BE0_811D_93A19ABE3EA9__INCLUDED_)
#define AFX_STDAFX_H__16E865D3_14A8_4BE0_811D_93A19ABE3EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Define target Windows version (Windows 10)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#define _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING
#define _SILENCE_CXX23_DENORM_DEPRECATION_WARNING

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
/*
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxrich.h>		// MFC rich edit classes
*/
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>		//CTreeView tree control view
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#endif // _AFX_NO_AFXCMN_SUPPORT

///////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4786)
/*
// . , stlport
//#include "stlport/cstdlib"
#include "stlport/stdexcept"
#include "stlport/iostream"
#include "stlport/fstream"
#include "stlport/map"
#include "stlport/vector"
#include "stlport/list"
#include "stlport/string"
#include "stlport/stdexcept"
#include "stlport/strstream"
#include "stlport/fstream"
#include "stlport/iomanip"
#include "stlport/limits"
#include "stlport/functional"
#include "stlport/algorithm"
#include "stlport/memory"
//#include "stlport/cassert"
#include "stlport/numeric"
//#include "stlport/complex"
*/
///////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <fstream>
//#include <map>
#include <vector>
//#include <list>
#include <string>
#include <stdexcept>
#include <strstream>
#include <fstream>
#include <iomanip>
#include <limits>
//#include <functional>
//#include <algorithm>
#include <memory>
#include <cassert>
#include <numeric>
//#include <complex>
///////////////////////////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <comdef.h>
///////////////////////////////////////////////////////////////////////////////
#   pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
#   pragma comment (lib, "glu32.lib")     /* link with Microsoft OpenGL Utility lib */
//#   pragma comment (lib, "glut32.lib")    /* link with Win32 GLUT lib */
///////////////////////////////////////////////////////////////////////////////
#include "gl/gl.h"		// Include the standard OpenGL headers
#include "gl/glu.h"		// Add the utility library
////#include "gl/glaux.h"	// Add in the auxiliary library (optional)
//#include <GL/glut.h>
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//#include "NVector.h"
///////////////////////////////////////////////////////////////////////////////
extern "C" wchar_t	DATA_PASS_mp_WORK_DIR[_MAX_PATH];
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{

#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 
///////////////////////////////////////////////////////////////////////////////
static double dsqrarg=0;
#define SQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)
///////////////////////////////////////////////////////////////////////////////


extern wchar_t workingDirectory[32];// = L"\\work\\";
static _bstr_t bstrWorkingDirectory = L"\\work\\";
extern wstring		g_szWorkDir;		//no terminal '\'
extern wstring		g_szWorkDir2;		//with terminal '\'
extern wstring		g_szWorkDirFileName;
//#define _MAX_PATH   260 /* max. length of full pathname */
///////////////////////////////////////////////////////////////////////////////
LPSTR dupString(LPCSTR lpsz); //Action::DupString
//requires a call to	GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LPSTR DupString(LPCSTR lpsz); //_tcsdup; memory leak

inline LPWSTR dupString(LPCWSTR lpsz)
{
	const wchar_t* source = lpsz ? lpsz : L"";
	const size_t cch = wcslen(source) + 1;
	HGLOBAL hGlobal = GlobalAlloc(GPTR, cch * sizeof(wchar_t));
	if (hGlobal == NULL)
	{
		return NULL;
	}
	LPWSTR copy = static_cast<LPWSTR>(GlobalLock(hGlobal));
	if (copy != NULL)
	{
		wcscpy_s(copy, cch, source);
	}
	GlobalUnlock(hGlobal);
	return copy;
}

inline std::wstring ToWideString(LPCSTR lpsz)
{
	if (lpsz == NULL)
	{
		return std::wstring();
	}
	const int cch = MultiByteToWideChar(CP_ACP, 0, lpsz, -1, NULL, 0);
	if (cch <= 1)
	{
		return std::wstring();
	}
	std::wstring wide(static_cast<size_t>(cch), L'\0');
	MultiByteToWideChar(CP_ACP, 0, lpsz, -1, wide.data(), cch);
	wide.pop_back();
	return wide;
}

inline std::string ToNarrowString(LPCWSTR lpsz)
{
	if (lpsz == NULL)
	{
		return std::string();
	}
	const int cb = WideCharToMultiByte(CP_ACP, 0, lpsz, -1, NULL, 0, NULL, NULL);
	if (cb <= 1)
	{
		return std::string();
	}
	std::string narrow(static_cast<size_t>(cb), '\0');
	WideCharToMultiByte(CP_ACP, 0, lpsz, -1, narrow.data(), cb, NULL, NULL);
	narrow.pop_back();
	return narrow;
}

inline LPWSTR GetWC(LPCWSTR lpsz)
{
	return dupString(lpsz);
}

inline LPWSTR GetWC(const std::wstring& lpsz)
{
	return GetWC(lpsz.c_str());
}

inline LPWSTR GetWC(LPCSTR lpsz)
{
	const std::wstring wide = ToWideString(lpsz);
	return GetWC(wide.c_str());
}

inline LPWSTR GetWC(const std::string& lpsz)
{
	return GetWC(lpsz.c_str());
}

template <size_t N>
inline LPWSTR GetWC(const char (&lpsz)[N])
{
	return GetWC(static_cast<LPCSTR>(lpsz));
}

template <size_t N>
inline LPWSTR GetWC(const wchar_t (&lpsz)[N])
{
	return GetWC(static_cast<LPCWSTR>(lpsz));
}

inline const char* GetC(LPCSTR lpsz)
{
	return lpsz ? lpsz : "";
}

inline const char* GetC(const std::string& lpsz)
{
	return lpsz.c_str();
}

inline const char* GetC(LPCWSTR lpsz)
{
	static thread_local std::string narrow;
	narrow = ToNarrowString(lpsz);
	return narrow.c_str();
}

inline const char* GetC(const std::wstring& lpsz)
{
	return GetC(lpsz.c_str());
}

//auto_ptr<char> NLtoCrNL(const char*buf);
char* NLtoCrNL(const char*buf);
///////////////////////////////////////////////////////////////////////////////
void Fire_WriteStatus(const wchar_t* strText);
void Fire_ViewChange(void);
void Fire_ResetView(void);
//void Fire_WriteCommand(BSTR strText);
void FireWriteCommand(const wchar_t* strText);//strText = GlobalAlloc
#define Fire_WriteCommand(s) FireWriteCommand((const wchar_t*) dupString((const wchar_t*)s));//strText = GlobalAlloc
void Fire_WriteResponse(const wchar_t* strText);
///////////////////////////////////////////////////////////////////////////////
LPVOID DisplayErrorText();
///////////////////////////////////////////////////////////////////////////////
UINT
TimedMessageBox(
       HWND hwndParent,
       LPCTSTR ptszMessage,
       LPCTSTR ptszTitle,
       UINT flags		= MB_OK | MB_ICONINFORMATION,
       DWORD dwTimeout	= 2000);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
UINT
MessageBoxTimed(
       LPCTSTR ptszMessage,
       HWND hwndParent	= 0,
       LPCTSTR ptszTitle= _T("Action"),
       UINT flags		= MB_OK | MB_ICONINFORMATION,
       DWORD dwTimeout	= 2000);
///////////////////////////////////////////////////////////////////////////////
class SolveIt_Error 
{ 
private: 
	string msg; 
	string filename; 
	int linenum;

public:	
	SolveIt_Error(const char *m, const char *fn = NULL, int ln = 0) : 
		msg(m ? m : ""), filename(fn ? fn : ""), linenum(ln)
	{
//		GlobalFree((void*)m);
	};

/*
	SolveIt_Error(const _bstr_t& m, char *fn = NULL, int ln = 0) : 
		msg((const char *)m), filename(fn), linenum(ln)
	{
	};

	SolveIt_Error(CString &m, char *fn = NULL, int ln = 0) : 
		msg(m), filename(fn), linenum(ln)
	{
		// Nothing to do here.
	};
*/

	// Return error message 
	void why()
	{
		string m=message();
		string f=file();
		long l=line();
		_variant_t L=l;
		_bstr_t bstr(L);
		f+="; ";
		f+=(const char*)bstr;
		CString message(m.c_str());
		CString title(f.c_str());
		::MessageBox(0,message,title,MB_OK);
	}


	// Return error message 
	string message()
	{
		if (msg.size())	return msg; 
		return string("Excepts error thrown");
	};

	int line()
	{
		return linenum;
	};

	string file()
	{
		if(filename.size())	return filename;
		return string("Unknown filename!");
	};
	
};

//#define		SolveIt_ERROR(string)	SolveIt_Error(dupString((const char *)string), __FILE__, __LINE__)
#define		SolveIt_ERROR(string)	SolveIt_Error((const char *)string, __FILE__, __LINE__)


///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_INIT[] = TEXT("{E9AB17DE-6D2F-47a5-A866-49BBD7C022DA}");
const UINT WM_USER_INIT = RegisterWindowMessage(lpWM_USER_INIT);
///////////////////////////////////////////////////////////////////////////////
//HOW BIZARRE CAN WINDOWS GET?
//
const wchar_t lp_WM_USER_COMMAND_MessageName[] = TEXT("{3E7D04A8-4B9B-41bf-A25D-24B4F27DE2C4}");
const UINT WM_USER_COMMAND = RegisterWindowMessage(lp_WM_USER_COMMAND_MessageName);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lp_WM_USER_POST_COMMAND_MessageName[] = TEXT("{6B91062D-D0E8-46bf-9F13-8D44BA4745EA}");
const UINT WM_USER_POST_COMMAND = RegisterWindowMessage(lp_WM_USER_POST_COMMAND_MessageName);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_TextBoxWindow[] = TEXT("{D8C74B64-CFA5-4868-B525-48F5474FBDBA}");
const UINT WM_USER_TextBoxWindow = RegisterWindowMessage(lpWM_USER_TextBoxWindow);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_APP_Initialize[] = TEXT("{C52D9488-250E-4fd4-BA4B-F1F9A6C310DF}");
const UINT WM_APP_Initialize = RegisterWindowMessage(lpWM_APP_Initialize);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_Initialize[] = TEXT("{AFA4195D-1B06-4085-B919-0D7EBAAB3CBF}");
const UINT WM_USER_Initialize = RegisterWindowMessage(lpWM_USER_Initialize);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_Message[] = TEXT("{A4902EC9-E211-4999-9230-271F1818CF82}");
const UINT WM_USER_Message = RegisterWindowMessage(lpWM_USER_Message);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_KillWorker[] = TEXT("{A856835D-8ADF-4015-990A-D620C90804C5}");
const UINT WM_USER_KillWorker = RegisterWindowMessage(lpWM_USER_KillWorker);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_AddCInfoDialog[] = TEXT("{14BF64A4-0F44-41a8-A938-305B140A3FE4}");
const UINT WM_USER_AddCInfoDialog = RegisterWindowMessage(lpWM_USER_AddCInfoDialog);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_AddCInfoDialog2[] = TEXT("{12C8B963-0941-4ab4-BCE5-602E7A0D49A0}");
const UINT WM_USER_AddCInfoDialog2 = RegisterWindowMessage(lpWM_USER_AddCInfoDialog2);
///////////////////////////////////////////////////////////////////////////////
static const wchar_t lpWM_USER_Redraw[] = TEXT("{C4FD7D8F-2731-4449-8D2D-BA45C6E9A809}");
const UINT WM_USER_Redraw = RegisterWindowMessage(lpWM_USER_Redraw);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
static const wchar_t lpWM_USER_SetParam[] = TEXT("{FA35717A-89EE-4a3d-81E0-63B40CBB1C2C}");
const UINT WM_USER_SetParam = RegisterWindowMessage(lpWM_USER_SetParam);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
static const wchar_t lpWM_USER_GetParam[] = TEXT("{2038B6E9-01EB-413a-8A19-2F8D7CA662EF}");
const UINT WM_USER_GetParam = RegisterWindowMessage(lpWM_USER_GetParam);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lp_WM_THREAD_STEP_DONE_MessageName[] = TEXT("{DF959BF9-A349-4b99-9EAC-0550DAFF8812}");
const UINT WM_THREAD_STEP_DONE = RegisterWindowMessage(lp_WM_THREAD_STEP_DONE_MessageName);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_START_PASS[] = TEXT("{A1C228E4-8A81-4940-8E8C-5CF57A0585A2}");
const UINT WM_USER_START_PASS = RegisterWindowMessage(lpWM_USER_START_PASS);

///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_STEP[] = TEXT("{STEPA1C228E4-8A81-4940-8E8C-5CF57A0585A2}");
const UINT WM_USER_STEP = RegisterWindowMessage(lpWM_USER_STEP);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


const wchar_t lpWM_USER_PASS_DONE[] = TEXT("{B25FE96A-EE0F-4fc0-9FAB-88D4235FEC11}");
const UINT WM_USER_PASS_DONE = RegisterWindowMessage(lpWM_USER_PASS_DONE);
///////////////////////////////////////////////////////////////////////////////
const wchar_t lpWM_USER_TERM[] = TEXT("{4F1DAA7B-3E32-4d9d-9E30-C17691C3B118}");
const UINT WM_USER_TERM = RegisterWindowMessage(lpWM_USER_TERM);





///////////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt
///////////////////////////////////////////////////////////////////////////////

using namespace SolveIt;
#include "Vector3D.h"

//#define FLEX_NEED_INTEGRAL_TYPE_DEFINITIONS 1

//#include "auto_ref.h"
//#include "quick_mem_manager.h"



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__16E865D3_14A8_4BE0_811D_93A19ABE3EA9__INCLUDED_)

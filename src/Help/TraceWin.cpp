//TraceWin.cpp
////////////////////////////////////////////////////////////////
// TraceWin Copyright 1995-1999 Paul DiLascia
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// NOTE: If you're using PixieLib, you don't need to include this file.
// It's already included by the library.
//
// ***************************************************************************
// TraceWin is a tool that displays MFC diagnostic (afxDump, TRACE) output
// in the window of the TraceWin applet.
//
// To use TraceWin, you must #include this file somewhere in your main program
// file (typically where you implement your CWinApp). Since this file contains
// code, you should #include it in only once--i.e. NOT in StdAfx.h--or you'll
// get multiply-defined symbol errors in the linker. This file contains an
// auto-initializing static variable that works in most cases; but you may miss
// some TRACE output from constructors of static objects. If so, you can
// manually call PxlTraceInit before your first TRACE call.
//
// To see the output, you also need the TraceWin applet, TraceWin.exe, which
// you can download http://pobox.com/~dilascia
//
// ***************************************************************************
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TraceWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG

IMPLEMENT_DYNAMIC(CFileTrace, CFile)

//////////////////
// Override to write to TraceWin applet instead of file.
//
// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
void CFileTrace::Write(const void* lpBuf, UINT nCount)
{
	if (!afxTraceEnabled)
		return;	// MFC tracing not enabled

	HWND hTraceWnd = ::FindWindow(TRACEWND_CLASSNAME, NULL);
	if (hTraceWnd==NULL)
		hTraceWnd = ::FindWindow(OLDTRACEWND_CLASSNAME, NULL);
	if (hTraceWnd) {
		// Found Trace window: send wstring with WM_COPYDATA
		// Must copy to make me the owner of the wstring; otherwise
		// barfs when called from MFC with traceMultiApp on
		//
		static /*wchar_t*/std::wstring mybuf{ LPCWSTR(lpBuf) };// [1024] ;
		std::string myBuf = utf8_encode(mybuf);
//		static wchar_t myBuf[1024] ;
//#ifdef _UNICODE
//		BOOL bDefCharUsed;
//		::WideCharToMultiByte(CP_ACP,0,LPCWSTR(lpBuf), -1, myBuf, 1024, NULL, &bDefCharUsed);
//#else
//		memcpy(mybuf, lpBuf, nCount);
//#endif

		COPYDATASTRUCT cds;
		cds.dwData = ID_COPYDATA_TRACEMSG;
		cds.cbData = nCount;
		cds.lpData = (PVOID)&myBuf[0];// .c_str();
		CWinApp* pApp = AfxGetApp();
		HWND hWnd = pApp ? pApp->m_pMainWnd->GetSafeHwnd() : NULL;
		::SendMessage(hTraceWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
	}
	// Also do normal debug thing
	::OutputDebugString((LPCTSTR)lpBuf);
}

/////////////////
// Initialize tracing. Replace global afxDump.m_pFile with CFileTrace object.
// In VC 5.0, you shouldn't need to call this, since it's called from an
// auto-initializing static object autoInit below. But if you don't see
// any TRACE output in the TraceWin window, you should try calling
// PxlTraceInit any time before your first TRACE message.
//
BOOL CFileTrace::Init()
{
	if (afxDump.m_pFile==NULL) {
		// Initialize tracing: replace afxDump.m_pFile w/my own CFile object
		//
		// It's important to allocate with "new" here, not a static object,
		// because CFileTrace is virtual--i.e., called through a pointer in
		// the object's vtbl--and the compiler will zero out the virtual
		// function table with a NOP function when a static object
		// goes out of scope. But I want my CFileTrace to stay around to
		// display memory leak diagnostics even after all static objects
		// have been destructed. So I allocate the object with new and
		// never delete it. I don't want this allocation to itself generate
		// a reported memory leak, so I turn off memory tracking before I
		// allocate, then on again after.
		//
		BOOL bEnable = AfxEnableMemoryTracking(FALSE);
		afxDump.m_pFile = new CFileTrace;
		AfxEnableMemoryTracking(bEnable);
		return TRUE;
	}
	return FALSE;
}

//////////////////
// This object does nothing but call CFileTrace::Init, so all you have to
// do is #include this file. Because afxDump is defined in a module with
//
// #pragma init_seg(lib)
//
// afxDump gets initialized before the "user" segment which is where your
// app (and autoInit) is by default. If you need to use init_seg(lib),
// or you have other objects whose constructors call TRACE that get
// initialized before CFileTrace::bInitialized, you will have to call
// CFileTrace::Init yourself, before your first TRACE statement.
//
BOOL CFileTrace::autoInit = CFileTrace::Init();

// This symbol defined so you can call it and have it
// compile to nothing in non-debug build
#define PxlTraceInit() CFileTrace::Init();

#else

#define PxlTraceInit()

#endif // _DEBUG

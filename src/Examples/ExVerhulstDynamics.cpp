// VerhulstDynamics.cpp : Implementation of CIO
#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
#include <objbase.h>
#include <initguid.h>
#include <commdlg.h>
#include <mmreg.h>
#include <dsound.h>
#include "DibSectn.h"
#include "HTML_Content.h"
//#include <iomanip.h>
#include "Prefixes.h"
#include "units.h"
UnitsVector uvec(_utab);
UnitsList ulist(_utab);
PrefixVector pvec(_ptab);
PrefixList plist(_ptab);

struct unitlist *
ulookup(LPSTR str)
{
   return ulist[str];
   return uvec[str];
}

//* Lookup a prefix in the prefix table.  Finds the first prefix that
//	matches the beginning of the input string.  Returns NULL if no
//	prefixes match. *

struct prefixlist *
plookup(LPSTR str)
{
   return plist[str];
   return pvec[str];
}
*/


#include "ScriptColors.h"

void CSolveItView::OnVerhulstDynamics()
{
//	ATLASSERT (WaitForSingleObject(m_hmutex, INFINITE) == WAIT_OBJECT_0);
//	ReleaseMutex(m_hmutex);
//	EnterCriticalSection (&a_critSection);
//	LeaveCriticalSection (&a_critSection);
//	ATLASSERT (WaitForSingleObject(m_hmutex, INFINITE) == WAIT_OBJECT_0);
//	ReleaseMutex(m_hmutex);
//	EnterCriticalSection (&a_critSection);
//	LeaveCriticalSection (&a_critSection);
//	NewWebBrowser(1);
	//Fire_ClearUserDefs();
	WRITE_LINE("sys.stop")WRITE_LINE("initialize")
	WRITE_LINE("system.title = \"Logistic Map\"")
//	WRITE_LINE("_ = sphere")
//	WRITE_LINE("_.radius = .01")
//	WRITE_LINE("_.color = [1, 0, 1]")
//	WRITE_LINE("_.ds =  0")
//	WRITE_LINE("system.boundingBox = [-0.05, 1.05, 2.8, 4.2, 5, 10]")
//	WRITE_LINE("system.boundingBox = [-0.05, 1.35, 1.9, 3.0, 5, 10]")
//	WRITE_LINE("system.boundingBox = [-0.1, 1.1, 2.9, 4.1, 5, 10]")

//	WRITE_LINE("system.boundingBox = [-0.1, 1.1, 2.9, 4.1, 2, 5]")
//	WRITE_LINE("system.translate = [ -0.5 ,  -3.5 ,  0.9 ]")

//	WRITE_LINE("system.twoDimensionalWorld = true")
//	_bstr_t bstr = L"\
//Click in the 'Scene' window then press the ESCAPE key to quit .\r\n";
//	WRITE_LINE("system.boundingBox = [-0.1, 1.1, 2.9, 4.1, 5, 10]")
//	WRITE_LINE("system.translate = [ -0.5 ,  -3.5 ,  0.4 ]")
//	WRITE_LINE("system.translate = [ -0.5 ,  -3.5 ,  -3 ]")

//	WRITE_LINE("system.boundingBox = [-0.1, 2.1, 1.9, 4.1, 5, 10]")
//	WRITE_LINE("system.boundingBox = [-0.1, 2.0, 1.9, 4.0, 5, 10]")
//	WRITE_LINE("system.translate = [ -(-0.1 + 2.0)/2 ,  -(1.9 + 4.0)/2 ,  0.3 ]")

	WRITE_LINE("system.boundingBox = [-0.1, 2.0, 1.9, 4.0, -1, 1]")
//	WRITE_LINE("system.translate = [ 0.15 ,  -0.05 ,  4.6 ]")
	WRITE_LINE("system.translate = [ 0.0 ,  0.0 ,  4.6 ]")

//	WRITE_LINE("system.translate")
//	WRITE_LINE("system.boundingBox = [-1, 3.0, 0, 4.0, -1, 1]")

	_bstr_t bstr = L"\
Click in the 'Scene' window then press the ESCAPE key \r\n\
or press 'Stop' to quit;\r\nafter stopping, this example must be reloaded to run again.\r\n\
";
	OnWriteResponse(bstr);OnWriteResponse(0);
	WRITE_LINE("system.twoDW.verDyn = true")
	
//	WRITE_LINE("system.translate")
}


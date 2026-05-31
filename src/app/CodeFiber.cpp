// CodeFiber.cpp: implementation of the CodeFiber class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "CodeFiber.h"

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static unsigned long err_cnt = 0;
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CodeFiber::~CodeFiber()
{
	Clear();
}
///////////////////////////////////////////////////////////////////////////////
void CodeFiber::Clear()
{
try {
	//for (auto& e : *this) {
	//	e.obj = 0;
	//}
	codeFiber::iterator it;
	for (it=begin();it!=end();++it)
	{
		(*it).obj = 0;
	}

	clear();
	ASSERT(size() == 0);
}
catch( SolveIt_Error& e)
{
	e.why();
}
catch(...)
{
	//_variant_t v(long(++err_cnt));
	std::wstring brst = std::to_wstring(long(++err_cnt));// (v);
	wstring str(L"CodeFiber::Clear: ");
	str += /*(const wchar_t*)*/brst;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->SetStatusBarText(str, 3);
	pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) /*GetWC*/(str.c_str()));
//	AfxMessageBox(L"CodeFiber::Clear: nope");
}
}
///////////////////////////////////////////////////////////////////////////////
CodeFibers::~CodeFibers()
{
	Clear();
}
///////////////////////////////////////////////////////////////////////////////
void CodeFibers::Clear()
{
try {
	codeFibers::iterator it;
	for (it=begin();it!=end();++it)
	{
		delete (*it);
	}

	clear();
	ASSERT(size() == 0);
}
catch( SolveIt_Error& e)
{
	e.why();
}
catch(...)
{
	//_variant_t v(long(++err_cnt));
	//std::wstring brst(v);
std::wstring brst = std::to_wstring(long(++err_cnt));// (v);
	wstring str(L"CodeFibers::Clear: ");
	//str += (const wchar_t*)brst;
	str += /*(const wchar_t*)*/brst;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->SetStatusBarText(str, 3);
	pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) /*GetWC*/(str.c_str()));
//	AfxMessageBox(L"CodeFibers::Clear: nope");
}
}
///////////////////////////////////////////////////////////////////////////////
void CodeFibers::initialize_new_cycle()
{
	//push_back(new CodeFiber);
	emplace_back(std::move(new CodeFiber));
}
///////////////////////////////////////////////////////////////////////////////
void CodeFibers::terminate_top_cycle()
{
	if (empty()) return;
	CodeFiber* fiber	= back();
	delete fiber;
	pop_back();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CodeFibers::reset_top_cycle()
{
	if (empty()) return;
	back()->Clear();
//2020 PLN:: added in desperation :
//	if (empty()) initialize_new_cycle();
}
///////////////////////////////////////////////////////////////////////////////

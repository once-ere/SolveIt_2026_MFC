// stdafx.cpp : source file that includes just the standard includes
//	SolveIt.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"




//QUICK_MEM_MANAGER g_the_auto_ref_pool(sizeof(LONG));

///////////////////////////////////////////////////////////////////////////////
void CALLBACK
MessageBoxTimer(HWND hwnd, UINT uiMsg, UINT idEvent, DWORD dwTime)
{
	PostQuitMessage(0);
}
///////////////////////////////////////////////////////////////////////////////
UINT SolveIt::TimedMessageBox(	HWND hwndParent,
						LPCTSTR ptszMessage,
						LPCTSTR ptszTitle,
						UINT flags,
						DWORD dwTimeout)
{
	UINT_PTR idTimer;
	UINT uiResult;
	MSG msg;
// Set a timer to dismiss the message box.
	idTimer = SetTimer(NULL, 0, dwTimeout, (TIMERPROC)MessageBoxTimer);
	uiResult = MessageBox(hwndParent, ptszMessage, ptszTitle, flags);
// Finished with the timer.
	KillTimer(NULL, idTimer);
//See if there is a WM_QUIT message in the queue. If so, then you
//timed out. Eat the message so you don't quit the entire application.
	if (PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE)) {
// If you timed out, then return zero.
	    uiResult = 0;
	}
	if (hwndParent) SetFocus(hwndParent);
	return uiResult;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
UINT SolveIt::MessageBoxTimed(	LPCTSTR ptszMessage, HWND hwndParent,
						LPCTSTR ptszTitle, UINT flags, DWORD dwTimeout)
{
	return TimedMessageBox(	hwndParent, ptszMessage, ptszTitle, flags, dwTimeout);
}

// OnPaste.cpp : implementation of the CSolveItView class
//

#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItDoc.h"
#include "SolveItView.h"
#include "TextBoxWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IsEdit: a helper function to determine if a given CWnd pointer
// points to a CEDit control.
// Use the SDK ::GetClassName() function because MFC IsKindOf
// fails if no CEdit variable has been created for the control you're
// trying to test.

BOOL CSolveItView::IsEdit( CWnd* pWnd )
	{
	try {
		ASSERT( pWnd != NULL );
		HWND hWnd = pWnd->GetSafeHwnd();
		if (hWnd == NULL)
			return FALSE;

		TCHAR szClassName[6];
		return ::GetClassName(hWnd, szClassName, 6) &&
				 _tcsicmp(szClassName, _T("Edit")) == 0;

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
	return 0;
} 


/*
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)


	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedClip)
*/

// UPDATE_COMMAND_UI handler for Edit Copy and Edit Cut which both
// require that the current focus is on an edit control that has
// text selected.

///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	try {
// get the current focus & determine if its on a CEdit control
	CWnd* pWnd = GetFocus();
	if (NULL == pWnd || !IsEdit( pWnd ))
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		CEdit* pEdit = (CEdit*)pWnd;
		int nBeg, nEnd;
		pEdit->GetSel( nBeg, nEnd );
		pCmdUI->Enable( nBeg != nEnd );
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
} 
// UPDATE_COMMAND_UI handlers for Edit Paste requires that focus be
// on an edit control and that the clipboard contains text to be
// pasted into the control.

///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	try {
// get the current focus & determine if its on a CEdit control
// also check to see if the control is read-only.
	CWnd* pWnd = GetFocus();
	if (  NULL == pWnd ||
			!IsEdit( pWnd ) ||
			(pWnd->GetStyle() & ES_READONLY) != 0 )
	{
		pCmdUI->Enable( FALSE );
	}
	else
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
} 

///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnEditCopy()
{
	try {
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	pEdit->Copy();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnEditCut()
{
	try {
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	pEdit->Cut();
	CTextBoxWindow* tb = dynamic_cast<CTextBoxWindow*>(pEdit);
	if (tb) tb->Resize();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnEditPaste()
{
	try {
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
	pEdit->Paste();
	CTextBoxWindow* tb = dynamic_cast<CTextBoxWindow*>(pEdit);
	if (tb) tb->Resize();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CSolveItView::OnStop");
	}
} 
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	try {
// get the current focus & determine if its on a CEdit control
	CWnd* pWnd = GetFocus();
	if (NULL == pWnd)
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		int nBeg, nEnd;
		GetSel( nBeg, nEnd );
		pCmdUI->Enable( nBeg != nEnd );
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnUpdateNeedSel");
	}
} 
// UPDATE_COMMAND_UI handlers for Edit Paste requires that focus be
// on an edit control and that the clipboard contains text to be
// pasted into the control.

///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	try {
// get the current focus & determine if its on a CEdit control
// also check to see if the control is read-only.
	CWnd* pWnd = GetFocus();
	if (  NULL == pWnd ||
			(pWnd->GetStyle() & ES_READONLY) != 0 )
	{
		pCmdUI->Enable( FALSE );
	}
	else
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnUpdateNeedClip");
	}
} 

///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnPopupCopy()
{
	try {
	Copy();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnPopupCopy");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnPopupCut()
{
	try {
	Cut();
	Resize();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnPopupCut");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnPopupDelete()
{
	try {
	Clear();
	Resize();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnPopupDelete");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CTextBoxWindow::OnPopupPaste()
{
	try {
	ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
	Paste();
	Resize();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CTextBoxWindow::OnPopupPaste");
	}
} 
///////////////////////////////////////////////////////////////////////////////
/*
///////////////////////////////////////////////////////////////////////////////
void CActionView::OnPopupCopy()
{//ATLASSERT(FALSE);
	CRichEditCtrl& m_ctlEdit = GetRichEditCtrl();
	m_ctlEdit.SendMessage(WM_COPY, 0, 0 );
}
///////////////////////////////////////////////////////////////////////////////
void CActionView::OnPopupDelete()
{//ATLASSERT(FALSE);
	CRichEditCtrl& m_ctlEdit = GetRichEditCtrl();
	m_ctlEdit.SendMessage(WM_CLEAR, 0, 0 ); 
}
void CActionView::OnPopupCut()
{//ATLASSERT(FALSE);
	CRichEditCtrl& m_ctlEdit = GetRichEditCtrl();
	m_ctlEdit.SendMessage(WM_CUT, 0, 0 ); 
}
///////////////////////////////////////////////////////////////////////////////
void CActionView::OnPopupPaste()
{//ATLASSERT(FALSE);
	CRichEditCtrl& m_ctlEdit = GetRichEditCtrl();
	m_ctlEdit.SendMessage(WM_PASTE, 0, 0 ); 
}
///////////////////////////////////////////////////////////////////////////////
*/

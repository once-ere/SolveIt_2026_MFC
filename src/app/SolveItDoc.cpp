// SolveItDoc.cpp : implementation of the CSolveItDoc class
//

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"

#include "SolveItDoc.h"
//#include "CntrItem.h"
//#include <shellapi.h>
//#include <OLEIMPL2.H>//#define RELEASE
#include "GLView.h"
#include "SolveItView.h"
#include "TextBoxWindow.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolveItDoc

IMPLEMENT_DYNCREATE(CSolveItDoc, CDocument)

BEGIN_MESSAGE_MAP(CSolveItDoc, CDocument)
	//{{AFX_MSG_MAP(CSolveItDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#if 0
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CDocument::OnUpdateObjectVerbMenu)
BEGIN_DISPATCH_MAP(CSolveItDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CSolveItDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CSolveItDoc, CDocument)
	INTERFACE_PART(CSolveItDoc, IID_ISolveIt, Dispatch)
END_INTERFACE_MAP()

// Note: we add support for IID_ISolveIt to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {62E08DAC-32C9-4F18-8D15-E287CEBAC6C3}
static const IID IID_ISolveIt =
{ 0x62e08dac, 0x32c9, 0x4f18, { 0x8d, 0x15, 0xe2, 0x87, 0xce, 0xba, 0xc6, 0xc3 } };

#endif
/////////////////////////////////////////////////////////////////////////////
// CSolveItDoc construction/destruction

CSolveItDoc::CSolveItDoc()
{
	// TODO: add one-time construction code here

//	EnableAutomation();
//	AfxOleLockApp();
}

CSolveItDoc::~CSolveItDoc()
{
//	AfxOleUnlockApp();
}

BOOL CSolveItDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
///////////////////////////////////////////////////////////////////////////////
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
//	CMDIChildWnd* pNewFrame = (CMDIChildWnd*)(theApp.m_pGLViewTemplate->CreateNewFrame(this, NULL));
//	if (pNewFrame )
//	{
//		ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
//		theApp.m_pGLViewTemplate->InitialUpdateFrame(pNewFrame, this);
//	}

	return TRUE;
}

//CRichEditCntrItem* CSolveItDoc::CreateClientItem(REOBJECT* preo) const
//{
//	// cast away constness of this
//	return new CSolveItCntrItem(preo, (CSolveItDoc*) this);
//}



/////////////////////////////////////////////////////////////////////////////
// CSolveItDoc serialization

void CSolveItDoc::Serialize(CArchive& ar)
{
	try{
	if (ar.IsStoring())
	{
		CSolveItView* pView	= theApp.GetEditView();
		if (pView != NULL && pView->m_hWnd && ::IsWindow(pView->m_hWnd) )
		{
			vecCEdit& vec	= pView->vec_pTextBoxWindow;
			for (vector<CTextBoxWindow*>::iterator j = vec.begin(); j != vec.end(); ++j)
			{
				CTextBoxWindow* m	= *j;
				if (m==NULL || !m->m_hWnd || !::IsWindow(m->m_hWnd))
				continue;
				LONG_PTR duh = ::GetWindowLongPtr(m->m_hWnd, GWLP_USERDATA);
				if (duh==0) continue;
				CString text;
				m->GetWindowText(text);
				if (text.IsEmpty()) continue;
				ar.WriteString(text);
			}
		}
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CDocument enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CDocument::m_bRTF = FALSE if you are serializing as text
//	CDocument::m_bRTF = FALSE;
	CDocument::Serialize(ar);
	}
catch (const char* s)
{
	AfxMessageBox(CString(s));
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}
}

/////////////////////////////////////////////////////////////////////////////
// CSolveItDoc diagnostics

#ifdef _DEBUG
void CSolveItDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSolveItDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSolveItDoc commands
/*
#ifndef _DEBUG
// generate smaller code in release build
#define RELEASE(lpUnk) _AfxRelease((LPUNKNOWN*)&lpUnk)
#else
// generate larger but typesafe code in debug build
#define RELEASE(lpUnk) do \
	{ if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)
#endif
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItDoc::OnCloseDocument() 
{
	ASSERT_VALID(this);
///////////////////////////////////////////////////////////////////////////////
#if 10
{
	CView* pView;
	POSITION pos = GetFirstViewPosition();
	while (pos) {
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGLView))) {
			RemoveView(pView);
		}
	}
}
#endif
///////////////////////////////////////////////////////////////////////////////
	CDocument::OnCloseDocument();
#if 0
{
	ASSERT_VALID(this);

	// don't allow in-place active documents to be closed without first
	//  deactivating them!
	if (m_pInPlaceFrame != NULL)
	{
		if (GetFirstViewPosition() != NULL)
			return;

		// no views but currently in-place active indicates that
		//  a WM_ENDSESSION is being processed.
		m_pInPlaceFrame = NULL;
	}

	InternalAddRef();   // keep document stable during shutdown

	// update lock count before sending notifications
	UpdateVisibleLock(FALSE, FALSE);

	// send some notifications to the container
	if (m_lpClientSite != NULL && m_bCntrVisible)
	{
		// allow the container to unshade the object appropriately
		m_lpClientSite->OnShowWindow(FALSE);
		m_bCntrVisible = FALSE;
	}

	// send close notification
	NotifyClosed();

	// finish closing the document (before m_lpClientSite->Release)
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	COleLinkingDoc::OnCloseDocument();
	ASSERT_VALID(this);

	// release client-site pointer
	RELEASE(m_lpClientSite);

	// disconnect the object
	LPUNKNOWN lpUnknown = (LPUNKNOWN)GetInterface(&IID_IUnknown);
	ASSERT(lpUnknown != NULL);
	CoDisconnectObject(lpUnknown, 0);

	// destroy the document if allowed
	InterlockedDecrement(&m_dwRef);              // remove InternalAddRef above
	if (bAutoDelete)
		delete this;
}
#endif
}

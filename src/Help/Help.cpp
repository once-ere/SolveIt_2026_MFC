// Help.cpp : Implementation of CHelp
#include "stdafx.h"
#include "SolveIt.h"
#include "Help.h"
#ifndef VECTOR_ANALYSIS_OBJECT
#include "parser.h"
#endif
#include "SymbolTable.h"
#include "common/Symbols.h"
#include "common/misc_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern UINT WM_ACTION_OnHelpComplete;//for the linker
///////////////////////////////////////////////////////////////////////////////

int CALLBACK SolveIt::HelpCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	PHELPITEM lplvid1 = (PHELPITEM)lParam1;
	PHELPITEM lplvid2 = (PHELPITEM)lParam2;
	LPSTR aCols1 = lplvid1->aCols[0]; 
	LPSTR aCols2 = lplvid2->aCols[0]; 
	return _tcscmp(aCols1,aCols2);
}

/////////////////////////////////////////////////////////////////////////////
// CHelp
CHelp::CHelp(CWnd* pParent)
	: CDialog(CHelp::IDD, pParent),
	parent_hWnd(pParent->m_hWnd),
	m_hBitmapTreeList(0)
{
	//{{AFX_DATA_INIT(CHelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CDialog::Create(IDD, pParent);
	ShowWindow(SW_SHOW);
}

CHelp::~CHelp()
{
	if (::IsWindow(parent_hWnd)) ::SendMessage(parent_hWnd, WM_ACTION_OnHelpComplete,0,0);
	
}
///////////////////////////////////////////////////////////////////////////////



void CHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
void CHelp::OnCancel()
{
	if (IsWindow(m_hWnd)) DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CHelp::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHelp, CDialog)
	//{{AFX_MSG_MAP(CHelp)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_HELP, OnListViewItemText)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_HELP, OnEndlabeleditList_help)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HELP, OnDblclkList_help)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	NOTIFY_HANDLER(IDC_LIST_HELP, LVN_GETDISPINFO, OnGetdispinfoList_inspectstack)

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CHelp::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if ( 0 >= cx || 0 >= cy ){
		return;
	}
	HWND hWndList = GetDlgItem(IDC_LIST_HELP)->GetSafeHwnd();
	if (!::IsWindow(hWndList)) return;
	CRect rc;
	GetDlgItem(IDC_STATIC_BOTTOM)->GetWindowRect(rc);
	ScreenToClient(rc);
//	HWND hWndLastText = GetDlgItem(IDC_STATIC_BOTTOM)->GetSafeHwnd();
	
	::MoveWindow(hWndList, 0, rc.bottom+6, cx, cy-(rc.bottom+6), TRUE);
/*
	HDC hdc = ::GetDC(hWndList); 
// Extract font dimensions from the text metrics.
	TEXTMETRIC tm;
	GetTextMetrics (hdc, &tm); 
	int xChar = tm.tmAveCharWidth; 
	int xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar/2; 
	int yChar = tm.tmHeight + tm.tmExternalLeading; 
	::ReleaseDC (hWndList, hdc); 
 
// Set an arbitrary maximum width for client area. 
// (xClientMax is the sum of the widths of 48 average 
// lowercase letters and 12 uppercase letters.) 
	int xClientMax = 48 * xChar + 12 * xUpper; 

	SCROLLINFO si;
ZeroMemory(&si , sizeof si);
si.cbSize	= sizeof si;
#define LINES 82
//SendMessage( hWndList, SBM_SETSCROLLINFO, (WPARAM) wParam, (LPARAM) &si );
// Set the vertical scrolling range and page size
	si.cbSize = sizeof(si); 
	si.fMask  = SIF_RANGE | SIF_PAGE; 
	si.nMin   = 0; 
	si.nMax   = LINES - 1; 
	si.nPage  = cy-(rc.bottom+6) / yChar; 

	::SetScrollInfo(hWndList, SB_VERT, &si, TRUE); 
 
// Set the horizontal scrolling range and page size. 
	si.cbSize = sizeof(si); 
	si.fMask  = SIF_RANGE | SIF_PAGE; 
	si.nMin   = 0; 
	si.nMax   = 2 + xClientMax / xChar; 
	si.nPage  = cx / xChar; 

	::SetScrollInfo(hWndList, SB_HORZ, &si, TRUE); 
*/


	GetDlgItem(IDC_LIST_HELP)->Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CHelp::OnDestroy()
{

	HWND hWndList = GetDlgItem(IDC_LIST_HELP)->GetSafeHwnd();
	HIMAGELIST h = (HIMAGELIST) ::SendMessage(hWndList, TVM_GETIMAGELIST, LVSIL_STATE, 0);
	if (h != NULL) ::SendMessage(hWndList, TVM_SETIMAGELIST, LVSIL_STATE, 0);

	if (m_hBitmapTreeList) DeleteObject(m_hBitmapTreeList);
	m_hBitmapTreeList = 0;
}

#undef SubclassWindow
///////////////////////////////////////////////////////////////////////////////

BOOL CHelp::OnInitDialog() 
{
	CDialog::OnInitDialog();

//##		if (!m_ctlSysListView32.SubclassWindow(GetDlgItem(IDC_LIST_HELP))) {
//##			ASSERT(FALSE);
//##		}

//	LONG style = //##	m_ctlSysListView32.GetWindowLong(GWL_STYLE);
//	ASSERT(style != 0);
//	//##	m_ctlSysListView32.SetWindowLong( GWL_STYLE, style|LVS_REPORT|LVS_EX_FULLROWSELECT);//
	HWND hWndList = GetDlgItem(IDC_LIST_HELP)->GetSafeHwnd();
	ListView_SetExtendedListViewStyle(hWndList, LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_TRACKSELECT);
//	LONG style = ::GetWindowLong(hWndList, GWL_STYLE);
//	ASSERT(style != 0);
//	::SetWindowLong(hWndList, GWL_STYLE, style| LVS_EX_FULLROWSELECT);
//TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES |
	m_hBitmapTreeList = LoadBitmap(AfxGetInstanceHandle() ,MAKEINTRESOURCE(IDB_TREELIST));
	int NUM_ICONS = 29;
	HIMAGELIST himl = ImageList_Create(	16, 16, ILC_COLOR24, NUM_ICONS, 0); 
	int firstIndex = ImageList_Add (himl, m_hBitmapTreeList, NULL);
	if (firstIndex == -1) return 0;
	ListView_SetImageList(hWndList, himl, LVSIL_SMALL); 

	//##	m_ctlSysListView32.ShowWindow( SW_SHOW );

//	ListView_DeleteAllItems(hWndList);
//	ListView_DeleteColumn(hWndList, 1);
//	ListView_DeleteColumn(hWndList, 0);
// Add the columns.
	char szText [4*MAX_PATH]; // place to store some text
	LVCOLUMN lvC; // list view column structure
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT; // left-align column
	lvC.pszText = szText;
#define AddCol(n,CX) \
	lvC.iSubItem = n;\
	_tcscpy(szText, Help_Cols[n]);\
	lvC.cx = (CX);\
	if (ListView_InsertColumn (hWndList, n, &lvC) == -1) return 1;

/*
System font avg_char_width = 4 dialog base units
lvC.cx = Width of the column, in pixels.

GetDeviceCaps() using LOGPIXELSX and LOGPIXELSY to determine dots per inch.
for a video display, LOGPIXELSX and LOGPIXELSY are defined by the video driver 
and may vary wildly. 
These numbers define a logical inch, which is almost never equal to a real inch. 
		HDC hdc = GetDC();
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hdc);
		int nCharX = tm.tmAveCharWidth;
GetDialogBaseUnits 
*/
	RECT rc = {0,0,400,481};//dialog base units
	::MapDialogRect(m_hWnd, &rc);//pixels
	int W = rc.right - GetSystemMetrics(SM_CXVSCROLL) - GetSystemMetrics(SM_CXHTHUMB)/2;
//	int col0 = 128, col1 = 256, col2 = 256;
	int col0 = 128, col1 = 512, col2 = 512;
	int wW = col0+col1+col2;
	AddCol(0, MulDiv(col0, W, wW))
	AddCol(1, MulDiv(col1, W, wW))
	AddCol(2, MulDiv(col2, W, wW))
#undef AddCol
#if 10
	LVITEM lvItem; // list view item structure

	lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE | LVIF_IMAGE;
	lvItem.state = 0; 
	lvItem.stateMask = 0;
	lvItem.iSubItem = 0;
//	lvItem.cchTextMax = wW;
//	lvItem.cchTextMax = wW*4;
	lvItem.cchTextMax = 4*MAX_PATH;//ignored 
// The parent window is responsible for storing the text.
// The list view control will send an LVN_GETDISPINFO
// when it needs the text to display.
	lvItem.pszText = LPSTR_TEXTCALLBACK; 

	int dIndex=GetSystemMetrics(SM_CYICON);//==32

	long i, j=0;
	for (i = 0; keywords[i].h.symbol; i++, j++)
	{
		HELPITEM *pItem = (HELPITEM *) LocalAlloc(LPTR, sizeof(HELPITEM));
		pItem->aCols[0] = DupString((char*) keywords[i].h.symbol);
		pItem->aCols[1] = DupString((char*) keywords[i].h.role);
		pItem->aCols[2] = DupString((char*) keywords[i].h.example);
		lvItem.iItem	= j;
		lvItem.lParam = (LPARAM) pItem;
		lvItem.iImage	= i>7? 22:21;
		dIndex = ListView_InsertItem(hWndList,&lvItem);
		if (dIndex == -1) return 1;
	}
///////////////////////////////////////////////////////////////////////////////
	lvItem.iImage	= 21;
	for (i = 0; misc_info[i].h.symbol; i++, j++)
	{
		HELPITEM *pItem = (HELPITEM *) LocalAlloc(LPTR, sizeof(HELPITEM));
		pItem->aCols[0] = DupString((char*) misc_info[i].h.symbol);
		pItem->aCols[1] = DupString((char*) misc_info[i].h.role);
		pItem->aCols[2] = DupString((char*) misc_info[i].h.example);
		lvItem.iItem	= j;
		lvItem.lParam = (LPARAM) pItem;
		dIndex = ListView_InsertItem(hWndList,&lvItem);
		if (dIndex == -1) return 1;
	}

///////////////////////////////////////////////////////////////////////////////
	lvItem.iImage	= 23;
	for (i = 0; consts[i].h.symbol; i++, j++)
	{
		HELPITEM *pItem = (HELPITEM *) LocalAlloc(LPTR, sizeof(HELPITEM));
		pItem->aCols[0] = DupString((char*) consts[i].h.symbol);
		pItem->aCols[1] = DupString((char*) consts[i].h.role);
		pItem->aCols[2] = DupString((char*) consts[i].h.example);
		lvItem.iItem	= j;
		lvItem.lParam = (LPARAM) pItem;
		dIndex = ListView_InsertItem(hWndList,&lvItem);
		if (dIndex == -1) return 1;
	}

	lvItem.iImage	= 24;
	for (i = 0; Lib1[i].h.symbol; i++, j++)
	{
		HELPITEM *pItem = (HELPITEM *) LocalAlloc(LPTR, sizeof(HELPITEM));
		pItem->aCols[0] = DupString((char*) Lib1[i].h.symbol);
		pItem->aCols[1] = DupString((char*) Lib1[i].h.role);
		pItem->aCols[2] = DupString((char*) Lib1[i].h.example);
		lvItem.iItem	= j;
		lvItem.lParam = (LPARAM) pItem;
		dIndex = ListView_InsertItem(hWndList,&lvItem);
		if (dIndex == -1) return 1;
	}

	lvItem.iImage	= 25;
	for (i = 0; Lib2[i].h.symbol; i++, j++)
	{
		HELPITEM *pItem = (HELPITEM *) LocalAlloc(LPTR, sizeof(HELPITEM));
		pItem->aCols[0] = DupString((char*) Lib2[i].h.symbol);
		pItem->aCols[1] = DupString((char*) Lib2[i].h.role);
		pItem->aCols[2] = DupString((char*) Lib2[i].h.example);
		lvItem.iItem	= j;
		lvItem.lParam = (LPARAM) pItem;
		dIndex = ListView_InsertItem(hWndList,&lvItem);
		if (dIndex == -1) return 1;
	}


	ListView_SortItems(hWndList, HelpCompareFunc, 0);

#endif


	return 1;  // Let the system set the focus
}
/**/
///////////////////////////////////////////////////////////////////////////////

void CHelp::OnListViewItemText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *pLvdi	= (LV_DISPINFO *)pNMHDR;
	NM_LISTVIEW *pNm	= (NM_LISTVIEW *)pNMHDR;
	HELPITEM *pItem		= (HELPITEM *)(pLvdi->item.lParam);
	pLvdi->item.pszText = pItem->aCols[pLvdi->item.iSubItem];
	return;
	switch (pLvdi->hdr.code)
	{
		case LVN_GETDISPINFO:
				pLvdi->item.pszText = pItem->aCols[pLvdi->item.iSubItem];
		switch (pLvdi->item.iSubItem)
		{
			case 0: // address
				pLvdi->item.pszText = pItem->symbol;
			break;
			case 1: // city
				pLvdi->item.pszText = pItem->role;
			break;
			case 2: // city
				pLvdi->item.pszText = pItem->example;
			break;
		}
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////

void CHelp::OnDblclkList_help(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *pLvdi	= (LV_DISPINFO *)pNMHDR;
	NM_LISTVIEW *pNm	= (NM_LISTVIEW *)pNMHDR;
	HELPITEM *pItem		= (HELPITEM *)(pLvdi->item.lParam);

	char szText [MAX_PATH]; // place to store some text

	HWND hWnd = GetDlgItem(IDC_LIST_HELP)->GetSafeHwnd();
	int iCurSel = ListView_GetNextItem (hWnd, -1, LVNI_SELECTED);
//	int iCurSel = ListView_GetNextItem (hWnd, -1, MAKELPARAM (LVNI_SELECTED, 0));

	LV_ITEM lvItem = pLvdi->item;
//	lvItem.iItem = iCurSel;
//	lvItem.iSubItem = 0;
	lvItem.mask = LVIF_TEXT;
	lvItem.cchTextMax = sizeof (szText);
	lvItem.pszText = szText;
	ListView_GetItem (hWnd,&lvItem);

}



void CHelp::OnEndlabeleditList_help(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *pLvdi	= (LV_DISPINFO *)pNMHDR;
	NM_LISTVIEW *pNm	= (NM_LISTVIEW *)pNMHDR;
	HELPITEM *pItem		= (HELPITEM *)(pLvdi->item.lParam);

//	if (!::IsClipboardFormatAvailable(CF_TEXT)) return 0;
	if (!OpenClipboard()) return;
	EmptyClipboard();

	int cb = _tcslen(pItem->example) + 1;
	if (cb <= 1) return;
	HGLOBAL hgCF_TEXTData = GlobalAlloc(GMEM_MOVEABLE, cb);//GPTR
	if (hgCF_TEXTData == 0)
	{ 
		CloseClipboard(); 
		return; 
	} 
	LPSTR   lpszNew = (LPSTR)GlobalLock(hgCF_TEXTData);
	if (lpszNew != NULL) _tcscpy(lpszNew, pItem->example);
	GlobalUnlock(hgCF_TEXTData);
	SetClipboardData(CF_TEXT, hgCF_TEXTData); 
	CloseClipboard(); 
}

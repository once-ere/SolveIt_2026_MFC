// Help.h : Declaration of the CHelp

#ifndef __HELP_H_
#define __HELP_H_

#include "resource.h"       // main symbols

namespace Action {


/////////////////////////////////////////////////////////////////////////////
// CHTML_Doc2BgndColor

const int Help_COLUMNS = 3; 
static char *Help_Cols[Help_COLUMNS] = {"Symbol:","Role:","Example Code:"};
/////////////////////////////////////////////////////////////////////////////
/*
typedef struct HelpItem {
	union {
		LPSTR aCols[Help_COLUMNS];
		struct {
			LPSTR symbol;
			LPSTR role;
			LPSTR example;
		};
	};

} HELPITEM, *PHELPITEM;
*/

typedef HelpItem HELPITEM;

int CALLBACK HelpCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

///////////////////////////////////////////////////////////////////////////////
// DupString - allocates a copy of a string. 
// lpsz - address of the null-terminated string to copy. 
LPSTR DupString(LPCSTR lpsz);

/////////////////////////////////////////////////////////////////////////////
// CHelp
class CHelp : 
	public CDialog
{
public:
	CHelp(CWnd* pParent = NULL);

	virtual ~CHelp();
	HWND	parent_hWnd;


// Dialog Data
	//{{AFX_DATA(CHelp)
	enum { IDD = IDD_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

//	CContainedWindow m_ctlSysListView32;

///////////////////////////////////////////////////////////////////////////////
	HBITMAP m_hBitmapTreeList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnListViewItemText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList_help(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList_help(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} // namespace Action

#endif //__HELP_H_

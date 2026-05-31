// ChangeCharge.h : Declaration of the CChangeCharge

#ifndef __CHANGECHARGE_H_
#define __CHANGECHARGE_H_

#include "resource.h"       // main symbols
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CChangeCharge
class CChangeCharge : 
	public CDialog
{
// Construction
public:
	CChangeCharge(double fCharge, const wstring& s, CWnd* pParent = NULL);   // standard constructor


	virtual ~CChangeCharge()
	{
	}

// Dialog Data
	//{{AFX_DATA(CChangeCharge)
	enum { IDD = IDD_CHANGECHARGE };
	//}}AFX_DATA


	double m_fCharge;
	wstring m_szCharge;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeCharge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInspectStack)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

#endif //__CHANGECHARGE_H_

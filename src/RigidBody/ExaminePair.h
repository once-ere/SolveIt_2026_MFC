#if !defined(AFX_EXAMINEPAIR_H__B031A614_ED97_4D8F_AE6F_32B10E7A558F__INCLUDED_)
#define AFX_EXAMINEPAIR_H__B031A614_ED97_4D8F_AE6F_32B10E7A558F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExaminePair.h : header file
//
#include "resource.h"       // main symbols

namespace SolveIt {

class Contact;

/////////////////////////////////////////////////////////////////////////////
// CExaminePair dialog

class CExaminePair : public CDialog
{
// Construction
public:
	CExaminePair(Contact* pc, CWnd* pParent = NULL);   // standard constructor

	virtual ~CExaminePair() {}

	Contact* m_pContact;
	void UpdateData();


// Dialog Data
	//{{AFX_DATA(CExaminePair)
	enum { IDD = IDD_EXAMINEPAIR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExaminePair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExaminePair)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} // namespace SolveIt

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMINEPAIR_H__B031A614_ED97_4D8F_AE6F_32B10E7A558F__INCLUDED_)

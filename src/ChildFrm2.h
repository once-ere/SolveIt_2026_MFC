// ChildFrm2.h : interface of the CChildFrame2 class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__75C3AD5E_86C4_4682_AB83_A124853B7D3E__INCLUDED_)
#define AFX_CHILDFRM_H__75C3AD5E_86C4_4682_AB83_A124853B7D3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{

class CChildFrame2 : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame2)
public:
	CChildFrame2();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame2)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
//	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CChildFrame2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame2)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CHILDFRM_H__75C3AD5E_86C4_4682_AB83_A124853B7D3E__INCLUDED_)

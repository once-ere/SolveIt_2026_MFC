// SolveItDoc.h : interface of the CSolveItDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVEITDOC_H__9ED63AF6_B172_444F_9B84_FE1E0E37A56D__INCLUDED_)
#define AFX_SOLVEITDOC_H__9ED63AF6_B172_444F_9B84_FE1E0E37A56D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////


class CSolveItDoc : public CDocument
{
protected: // create from serialization only
	CSolveItDoc();
	DECLARE_DYNCREATE(CSolveItDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolveItDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL
//	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CSolveItDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSolveItDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_SOLVEITDOC_H__9ED63AF6_B172_444F_9B84_FE1E0E37A56D__INCLUDED_)

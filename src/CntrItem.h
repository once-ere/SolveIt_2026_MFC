// CntrItem.h : interface of the CSolveItCntrItem class
//

#if !defined(AFX_CNTRITEM_H__220C043A_D04C_4D20_9E5B_FBA257B51599__INCLUDED_)
#define AFX_CNTRITEM_H__220C043A_D04C_4D20_9E5B_FBA257B51599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
///////////////////////////////////////////////////////////////////////////////

class CSolveItDoc;
class CSolveItView;

class CSolveItCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CSolveItCntrItem)

// Constructors
public:
	CSolveItCntrItem(REOBJECT* preo = NULL, CSolveItDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CSolveItDoc* GetDocument()
		{ return (CSolveItDoc*)CRichEditCntrItem::GetDocument(); }
	CSolveItView* GetActiveView()
		{ return (CSolveItView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolveItCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSolveItCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__220C043A_D04C_4D20_9E5B_FBA257B51599__INCLUDED_)

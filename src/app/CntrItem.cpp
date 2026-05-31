// CntrItem.cpp : implementation of the CSolveItCntrItem class
//

#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItDoc.h"
#include "SolveItView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolveItCntrItem implementation

IMPLEMENT_SERIAL(CSolveItCntrItem, CRichEditCntrItem, 0)

CSolveItCntrItem::CSolveItCntrItem(REOBJECT* preo, CSolveItDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CSolveItCntrItem::~CSolveItCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CSolveItCntrItem diagnostics

#ifdef _DEBUG
void CSolveItCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CSolveItCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

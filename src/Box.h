// Box.h: interface for the CBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOX_H__2ED60D9A_E62D_4F33_B541_84BB047E46DF__INCLUDED_)
#define AFX_BOX_H__2ED60D9A_E62D_4F33_B541_84BB047E46DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////



class CBox  
{
public:
	virtual ~CBox(){}

public:
	POINT p;
	RECT rc;
	int id;
//for rectangular sequence
	CBox(const RECT& _rc, long ID) : rc(_rc), id(ID)
	{
		p.x=(rc.left+rc.right)/2;
		p.y=(rc.top+rc.bottom)/2;
	}
	CBox(const CBox& b) : rc(b.rc), id(b.id), p(b.p) {}
	CBox& operator=(const CBox& b)
	{
		rc = b.rc, id = b.id, p = b.p;
	}

	void Draw(HDC hDC)
	{
//		::FrameRect(hDC, &rc, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	CBox* HitTest(const POINT& point)
	{
		if ( ::PtInRect(&rc, point)) return this;
		return 0;
	}

	void Resize(long x, long y, long zeroRef, BOOL bCtrl)
	{
		rc.right += x;
		if (!bCtrl)
		{
			rc.left += x;
			if (rc.top == zeroRef)
			{
				rc.bottom += y;
			}
			else if (rc.bottom == zeroRef)
			{
				rc.top += y;
			}
			if (rc.bottom < rc.top)
			{
				long t = rc.bottom;
				rc.bottom = rc.top;
				rc.top = t;
			}
		}
	}

};

///////////////////////////////////////////////////////////////////////////////
struct CBoxCompare
{  
	bool operator()(const CBox* a, const CBox* b) const
	{
		return a->rc.left < b->rc.left;
	}
};
extern CBoxCompare boxCompare;
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BOX_H__2ED60D9A_E62D_4F33_B541_84BB047E46DF__INCLUDED_)

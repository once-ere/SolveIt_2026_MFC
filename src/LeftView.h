// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__E0659677_BD8B_4194_A860_72C00555BF8D__INCLUDED_)
#define AFX_LEFTVIEW_H__E0659677_BD8B_4194_A860_72C00555BF8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "StackDatum.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CSolveItDoc;
class CLeftView;

class objects;
/*
typedef long (__stdcall CLeftView::*SetO)(const objects& val);
typedef long (__stdcall CLeftView::*GetO)(objects& val);
typedef pair<SetO, GetO> Set_Get_O;
///////////////////////////////////////////////////////////////////////////////
typedef void (__stdcall CLeftView::*SetD)(const double& val);
typedef void (__stdcall CLeftView::*SetL)(const int& val);
typedef void (__stdcall CLeftView::*SetS)(const wchar_t* val);

typedef double		(__stdcall CLeftView::*GetD)(void);
typedef int			(__stdcall CLeftView::*GetL)(void);
typedef const wchar_t*	(__stdcall CLeftView::*GetS)(void);

typedef pair<SetD, GetD> Set_Get_D;
typedef pair<SetL, GetL> Set_Get_L;
typedef pair<SetS, GetS> Set_Get_S;
*/
///////////////////////////////////////////////////////////////////////////////
//
class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CSolveItDoc* GetDocument();
///////////////////////////////////////////////////////////////////////////////
	CImageList	m_ctlImage;
	HTREEITEM m_htRoot;
	HTREEITEM m_htTrunk;
	HTREEITEM m_htInput;
	HTREEITEM m_htOutput;

	HTREEITEM		m_sel;

	std::vector<HTREEITEM> vec_ht_objects;

//	std::vector<HTREEITEM> vec_ht_long;
//	std::vector<HTREEITEM> vec_ht_double;
//	std::vector<HTREEITEM> vec_ht_string;


	HTREEITEM m_htSystem;
	HTREEITEM m_htSystemTime;
	HTREEITEM m_htSystemEnergy;
	HTREEITEM m_htSystemLinearMomentum;
	HTREEITEM m_htSystemAngularMomentum;
	HTREEITEM m_htBodies;
	HTREEITEM m_htConstraints;
	HTREEITEM m_htEnvironment;

	std::vector<HTREEITEM> vec_ht_System;
	std::vector<HTREEITEM> vec_ht_objectsRigidBody;
	std::vector<HTREEITEM> vec_ht_objects_position;
	std::vector<HTREEITEM> vec_ht_objects_velocity;
	std::vector<HTREEITEM> vec_ht_objects_force;
	std::vector<HTREEITEM> vec_ht_objects_torque;
	std::vector<HTREEITEM> vec_ht_objects_Contact_force;
	std::vector<HTREEITEM> vec_ht_objects_Contact_torque;
	std::vector<HTREEITEM> vec_ht_objects_LinearMomentum;
	std::vector<HTREEITEM> vec_ht_objects_AngularMomentum;
	std::vector<HTREEITEM> vec_ht_Constraints;
	std::vector<HTREEITEM> vec_ht_Environment;



/*
///////////////////////////////////////////////////////////////////////////////
class MapO : public map<HTREEITEM, Set_Get_O>
{
public:
	Set_Get_O& operator()(const HTREEITEM& key)
	{
		map<HTREEITEM, Set_Get_O>::iterator i = find(key);
		if (i != end()) return i->second;
		throw "CLeftView.MapO";
	}
} mo;

///////////////////////////////////////////////////////////////////////////////
class MapD : public map<HTREEITEM, Set_Get_D>
{
public:
	Set_Get_D& operator()(const HTREEITEM& key)
	{
		map<HTREEITEM, Set_Get_D>::iterator i = find(key);
		if (i != end()) return i->second;
		throw "CLeftView.MapD";
	}
} md;

class MapL : public map<HTREEITEM, Set_Get_L>
{
public:
	Set_Get_L& operator()(const HTREEITEM& key)
	{
		map<HTREEITEM, Set_Get_L>::iterator i = find(key);
		if (i != end()) return i->second;
		throw "CLeftView.MapL";
	}
} ml;

class MapS : public map<HTREEITEM, Set_Get_S>
{
public:
	Set_Get_S& operator()(const HTREEITEM& key)
	{
		map<HTREEITEM, Set_Get_S>::iterator i = find(key);
		if (i != end()) return i->second;
		throw "CLeftView.MapS";
	}
} ms;
*/
///////////////////////////////////////////////////////////////////////////////
// Operations
public:
	void PopulateTree();
	void UpdateTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:

#define ACCESS(x) \
	long __stdcall Set##x(const objects& val);\
	long __stdcall Get##x(objects& val);

	ACCESS(NDATA)

	ACCESS(glFrustum_left)
	ACCESS(glFrustum_right)
	ACCESS(glFrustum_bottom)
	ACCESS(glFrustum_top)
	ACCESS(glFrustum_znear)
	ACCESS(glFrustum_zfar)

	ACCESS(fTransX)
	ACCESS(fTransY)
	ACCESS(fTransZ)

	ACCESS(ZoomScale)
	ACCESS(ScaleWave)

	ACCESS(xStr)
	ACCESS(yStr)
	ACCESS(tic_mark_spacingStr)

#undef ACCESS

#define ACCESS(x) \
	void __stdcall Set##x(const int& val);\
	int __stdcall Get##x(void);

	ACCESS(NDATA)
#undef ACCESS

#define ACCESS(x) \
	void __stdcall Set##x(const double& val);\
	double __stdcall Get##x(void);

	ACCESS(glFrustum_left)
	ACCESS(glFrustum_right)
	ACCESS(glFrustum_bottom)
	ACCESS(glFrustum_top)
	ACCESS(glFrustum_znear)
	ACCESS(glFrustum_zfar)

	ACCESS(fTransX)
	ACCESS(fTransY)
	ACCESS(fTransZ)

	ACCESS(ZoomScale)
	ACCESS(ScaleWave)
	
#undef ACCESS

#if 10
#define ACCESS(x) \
	void __stdcall Set##x(const wchar_t* val);\
	const wchar_t* __stdcall Get##x(void);
	ACCESS(xStr)
	ACCESS(yStr)
	ACCESS(tic_mark_spacingStr)
#undef ACCESS
#endif

#if 0
staticstaticstaticstatic
	void __stdcall SetxStr(const wchar_t* val);
	const wchar_t* __stdcall GetxStr(void);

	void __stdcall SetyStr(const wchar_t* val);
	const wchar_t* __stdcall GetyStr(void);
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CSolveItDoc* CLeftView::GetDocument()
   { return (CSolveItDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_LEFTVIEW_H__E0659677_BD8B_4194_A860_72C00555BF8D__INCLUDED_)

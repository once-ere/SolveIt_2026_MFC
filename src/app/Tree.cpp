// Tree.cpp

#include "stdafx.h"

#pragma optimize( "", off )
#include "SolveIt.h"

#include "SolveItDoc.h"
#include "LeftView.h"
#include "MainFrm.h"
#include "GLView.h"
#include "System.h"
#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/RigidBody.h"
//#include "FieldsManager.h"
#include "parser.h" /* for token definitions and yylval */
#include "objects.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define CONCAT2(s1,s2) s1 ## s2
#define new_ID(s1,s2) CONCAT2(s1,s2)
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers
//974 by 28; 14 images 70 by 28
int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

// Create the Image List
	HBITMAP hBitmapTreeList = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TREELIST));
//	HBITMAP hBitmapTreeList = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TREELIST2));
	if (hBitmapTreeList == 0)
	{
		::MessageBox(NULL, L"hBitmapTreeList == NULL", L"CLeftView::OnCreate", MB_ICONWARNING);
		return -1;
	}

	HIMAGELIST himl;
	himl = ImageList_Create(16, 16, ILC_COLOR24, 29, 0);
//	himl = ImageList_Create(70, 28, ILC_COLOR24, 14, 0);
	if (himl == 0)
	{
		::MessageBox(NULL, L"himl == NULL", L"CLeftView::OnCreate", MB_ICONWARNING);
		return -1;
	}

	int indexBegin = ImageList_Add(himl, hBitmapTreeList, (HBITMAP) NULL); 
	ASSERT(indexBegin == 0);
	DeleteObject(hBitmapTreeList);
	hBitmapTreeList = 0;


	HIMAGELIST previous_image_list = TreeView_SetImageList( GetSafeHwnd(), himl, TVSIL_NORMAL);
	GetTreeCtrl().SendMessage(TVM_SETIMAGELIST, (UINT)TVSIL_NORMAL, (LPARAM)himl);
	
	return 0;

	m_ctlImage.Create(IDB_TREELIST,16,0,RGB(255,0,255));
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
/// Attach image list to Tree
	CTreeCtrl& t = GetTreeCtrl();
	t.SetImageList(&m_ctlImage,TVSIL_NORMAL);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	CRigidBody *  pbody = dynamic_cast<CRigidBody * >(pbody);
typedef Vector3D (*Vector3DFunc2)(void);
typedef Vector3D (*Vector3DFunc)(CRigidBody *  pbody);
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
LPSTR SolveIt::dupString(LPCSTR lpsz) //Action::DupString
{
	size_t cb = strlen(lpsz) + 1;
	HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
	LPSTR   lpszNew = (LPSTR)GlobalLock(hgCF_TEXTData);
	if (lpszNew != NULL) strcpy_s(lpszNew, cb, lpsz);
	GlobalUnlock(hgCF_TEXTData);
	return lpszNew; 
}
	string GetProperty(const objects& body, unsigned int property);
//	string GetProperty(CRigidBody *  pbody, unsigned int property);
	void SetProperty(CRigidBody *  pbody, unsigned int property, double* newVal);//, const objects& val
}
///////////////////////////////////////////////////////////////////////////////
LPSTR SolveIt::DupString(LPCSTR lpsz) //Action::DupString
{
	return _strdup(lpsz);
}
///////////////////////////////////////////////////////////////////////////////
string SolveIt::GetProperty(const objects&  body, unsigned int property)//, int component)
{
	CRigidBody *  pbody = dynamic_cast<CRigidBody *>(body.rbPtr);
	if (pbody==0) return string("null");
	CSystem* m_system = theApp.GetSystem();
	objects val;
	m_system->Get(body, property, val);
	std::wostringstream oss;
	oss << val;
	return ToNarrowString(oss.str().c_str());
}
#if 0
string SolveIt::GetProperty(CRigidBody *  pbody, unsigned int property)//, int component)
{
	if (pbody==0) return string("null");
	CSystem* m_system = theApp.GetSystem();
//	objects body(*pbody);
	objects body(pbody);
	objects val;
	m_system->Get(body, property, val);
	char buf[256];
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	oss << val << ends;
	string s(buf);
	return s;
}
///////////////////////////////////////////////////////////////////////////////
void SolveIt::SetProperty(CRigidBody *  pbody, unsigned int property, double* newVal)
{
	if (pbody==0) return;
	CSystem* m_system = theApp.GetSystem();
//	objects body(*pbody);
	objects body(pbody);
	objects val;
	switch (property)
	{
		case	T_POSITION:
		{
			Point3D pt(newVal);
			val = pt;
			break;
		}
		case	T_ROTATE:
		case	T_VELOCITY:
		case	T_MOMENTUM:
		case	T_ANGULARMOMENTUM:
		case	T_COLOR:
		case	T_DAMPING_COEFFICIENT:
		{
			Vector3D v(newVal);
			val = v;
			break;
		}
		case	T_IMASS:
		case	T_MASS:
		case	T_CHARGE:
		case	T_RADIUS:
		case	T_WIDTH:
		case	T_HEIGHT:
		case	T_DEPTH:
		case	T_COEFFICIENT_OF_STATIC_FRICTION:
		case	T_COEFFICIENT_OF_KINETIC_FRICTION:
		case	T_COEFFICIENT_OF_RESTITUTION:
		{
			val = *newVal;
			break;
		}
		case	T_DRAWSTYLE:
		{
			val = int(*newVal);
			break;
		}
		default:
			return;
	}
	m_system->Set(body, property, val);
}
#endif
///////////////////////////////////////////////////////////////////////////////


static struct {		//* Keywords *
	const char	*name;
	int		type;
} body_properties[] = {
	"position",					T_POSITION,
	"rotate",					T_ROTATE,//ORIENTATION,
	"velocity",					T_VELOCITY,
	"momentum",					T_MOMENTUM,
	"angular_momentum",			T_ANGULARMOMENTUM,
	"color",					T_COLOR,
	"coefficientOfDamping",		T_DAMPING_COEFFICIENT,
	"inverseMass",				T_IMASS,
	"mass",						T_MASS,
	"charge",					T_CHARGE,
	"radius",					T_RADIUS,
	"width",					T_WIDTH,
	"height",					T_HEIGHT,
	"depth",					T_DEPTH,
	"drawstyle",				T_DRAWSTYLE,
	"coefficientOfStaticFriction",	T_COEFFICIENT_OF_STATIC_FRICTION,
	"coefficientOfKineticFriction",	T_COEFFICIENT_OF_KINETIC_FRICTION,
	"coefficientOfRestitution",		T_COEFFICIENT_OF_RESTITUTION,
	"force",					T_FORCE,
	"torque",					T_TORQUE,
	"contact force",			T_contactFORCE,
	"contact torque",			T_contactTORQUE,
	0,		0,
};
///////////////////////////////////////////////////////////////////////////////
//#define new_ID(s1,s2) CONCAT2(s1,s2)
///////////////////////////////////////////////////////////////////////////////
void CLeftView::PopulateTree()
{
	try{
	if (!::IsWindow(m_hWnd))
	{
		::MessageBox(NULL, L"IsWindow(m_hWnd", L"xxx::PopulateTree", MB_ICONWARNING);
		return;
	}
//	SetFocus();
	CTreeCtrl& t = GetTreeCtrl();
	CSystem* m_system = theApp.GetSystem();
//	Lock lock (theApp.protectBodies);
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();

	vec_ht_objects.clear();

//	vec_ht_long.clear();
//	vec_ht_double.clear();
//	vec_ht_string.clear();

	vec_ht_objectsRigidBody.clear();
	vec_ht_objects_position.clear();
	vec_ht_objects_velocity.clear();
	vec_ht_Constraints.clear();
	vec_ht_Environment.clear();

	vec_ht_objects_LinearMomentum.clear();
	vec_ht_objects_AngularMomentum.clear();
	
	vec_ht_objects_force.clear();
	vec_ht_objects_torque.clear();
	
	vec_ht_objects_Contact_force.clear();
	vec_ht_objects_Contact_torque.clear();


	HTREEITEM ht;
	t.DeleteAllItems();
	m_htRoot = TVI_ROOT;//GetRootItem();
	ASSERT(m_htRoot==TVI_ROOT);
	t.SetItemData( m_htRoot, 0 );
	long nID = 22;//IDC_RB_TREE;DupString()

	m_htTrunk = t.InsertItem(CString(m_system->name.c_str()), nID, nID, m_htRoot/*TVI_ROOT*/, TVI_LAST);
	t.SetItemData( m_htTrunk, 0 );
	t.SetItemImage(m_htTrunk, nID, nID);
	m_htSystem					= t.InsertItem(_T("System"),				23,	23, m_htTrunk,  TVI_LAST);//__System
	m_htSystemTime				= t.InsertItem(_T("Time"),				25, 25, m_htSystem, TVI_LAST);//__SystemPlus
	m_htSystemEnergy			= t.InsertItem(_T("Energy"),				26, 26, m_htSystem, TVI_LAST);//__SystemPlus
	m_htSystemLinearMomentum	= t.InsertItem(_T("Linear Momentum"),		27, 27, m_htSystem, TVI_LAST);//__SystemPlus
	m_htSystemAngularMomentum	= t.InsertItem(_T("Angular Momentum"),	28, 28, m_htSystem, TVI_LAST);//__SystemPlus

	m_htBodies		= t.InsertItem(_T("Bodies"),		21,	21,	m_htTrunk, TVI_LAST);//__Bodies
	m_htConstraints	= t.InsertItem(_T("Constraints"),	24,	24,	m_htTrunk, TVI_LAST);//__Constraints
	m_htEnvironment	= t.InsertItem(_T("Environment"),	22,	22,	m_htTrunk, TVI_LAST);//__Environment


	m_htInput	=m_htEnvironment;// t.InsertItem(_T("Input"),	23,	23, m_htTrunk,  TVI_LAST);
	m_htOutput	=m_htEnvironment;// t.InsertItem(_T("Output"),		22,	22,	m_htTrunk, TVI_LAST);

/*
	ht	= t.InsertItem(_T("Bg Color"),	24,	24,	m_htEnvironment, TVI_LAST);
	t.SetItemData( ht, (unsigned long) 13 );
	vec_ht_Environment.push_back(ht);
*/
	_bstr_t bstr(L"MathML: Using Native Browser MathML");

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	int nos[]={10,12,25,15,23,22,25,20,19,18};
	int nos2[]={11,17,16};
///////////////////////////////////////////////////////////////////////////////
	struct {
		LPCTSTR s;
		int ID;
		int imageID;
	} keys[]={
		_T("# Steps"),	T_NODES,  28,
		_T("eigenvalue#"),	new_ID(T_STATE , T_EigenNumber),  23,
		_T("eigenvalue"),	new_ID(T_STATE , T_ENERGY),  22,
		_T("scale wave"),	new_ID(T_STATE , T_SCALE),  24,

		_T("X Max"), new_ID( T_SCENE, T_SYSTEM_RIGHT), 20,
		_T("X Min"), new_ID( T_SCENE, T_SYSTEM_LEFT), 21,
		_T("Y Max"), new_ID( T_SCENE, T_SYSTEM_TOP), 22,
		_T("Y Min"), new_ID( T_SCENE, T_SYSTEM_BOTTOM), 23,
		_T("Z far"), new_ID( T_SCENE, T_SYSTEM_FAR), 24,
		_T("Z near"), new_ID( T_SCENE, T_SYSTEM_NEAR), 25,

		_T("transl X"), new_ID( T_TRANSLATE, T_XCOMP), 19,
		_T("transl Y"), new_ID( T_TRANSLATE, T_YCOMP), 18,
		_T("transl Z"), new_ID( T_TRANSLATE, T_ZCOMP), 17,

//		_T("Zoom Scale"), SetZoomScale, GetZoomScale, true, 26,
//		_T("Scale Wave"), SetScaleWave, GetScaleWave, true, 27,

		_T("x-label"),	new_ID( T_TITLE, T_XCOMP),  18,
		_T("y-label"),	new_ID( T_TITLE, T_YCOMP),  19,

		_T("x-tic-marks"),	new_ID( T_TICMARK, T_XCOMP),  16,
		_T("y-tic-marks"),	new_ID( T_TICMARK, T_YCOMP),  15,

//		_T("tic-marks"),	Settic_mark_spacingStr, Gettic_mark_spacingStr, true,  20,

			0,0,0,0,0
	};
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////reinterpret_cast<unsigned long>()//////////////////////////
int j=0;

	for (j=0; keys[j].s; ++j)
	{
		ht= t.InsertItem( keys[j].s, keys[j].imageID, keys[j].imageID, m_htInput, TVI_LAST);
		t.SetItemText( ht, keys[j].s );
		ht= t.InsertItem(keys[j].s,	keys[j].imageID, keys[j].imageID, ht, TVI_LAST);
		t.SetItemData( ht, keys[j].ID);
		t.EnsureVisible(ht);
		vec_ht_objects.push_back(ht);
	}
///////////////////////////////////////////////////////////////////////////////
	ht	= t.InsertItem(bstr,	23,	23,	m_htEnvironment, TVI_LAST);//__System
	t.SetItemData( ht, (unsigned long) 14 );
	vec_ht_Environment.push_back(ht);

	if (rigidBodyManager->empty())
	{
		HTREEITEM hParent = t.GetParentItem(m_htEnvironment);
		if (hParent != 0)
		{
			hParent = t.GetParentItem(hParent);
			if (hParent != 0) t.Expand(hParent, TVE_EXPAND);
		}
		else t.Expand(hParent, TVE_EXPAND);

		t.EnsureVisible(m_htEnvironment);
		return;
	}
	std::wostringstream oss;


	ObjectsList::const_iterator it;
	for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
	{
		CRigidBody *  pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0)  throw SolveIt_ERROR("pbody == 0");//return;
//		string strName = SolveIt::GetProperty( pbody, T_NAME);
		string strName = SolveIt::GetProperty( (*it), T_NAME);
		long key = pbody->m_tagImage;//pbody->m_lKey;
		ht = t.InsertItem(CString(strName.c_str()), 0xFFFF&key, 0xFFFF&key, m_htBodies, TVI_LAST);
		t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(pbody) );
		vec_ht_objectsRigidBody.push_back(ht);
	}


	std::vector<HTREEITEM>::iterator i;
	for (i = vec_ht_objectsRigidBody.begin(); i != vec_ht_objectsRigidBody.end(); ++i)
	{
		CRigidBody *  pbody = reinterpret_cast<CRigidBody*>(t.GetItemData(*i));
	//	objects body(*pbody);
		objects body(pbody);
///////////////////////////////////////////////////////////////////////////////
	for (int j = 0; body_properties[j].name; j++)
	{
		ht = t.InsertItem(CString(GetWC(body_properties[j].name)), j, j, *i, TVI_LAST);
		t.SetItemData( ht, body_properties[j].type );
		objects val;
		m_system->Get(body, body_properties[j].type, val);
		
		oss.str(L"");
		oss.clear();
		oss << val;//GetProperty
		ht = t.InsertItem(CString(oss.str().c_str()), j, j, ht, TVI_LAST);
		if ( body_properties[j].type == T_POSITION )
		{
			vec_ht_objects_position.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->x) );//address
		}
		else
		if ( body_properties[j].type == T_VELOCITY )
		{
			vec_ht_objects_velocity.push_back(ht);
	//		pbody->get_Velocity(&address);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->m_vecVelocity) );
		}
		else
		if ( body_properties[j].type == T_MOMENTUM )
		{
			vec_ht_objects_LinearMomentum.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->p) );
		}
		else
		if ( body_properties[j].type == T_ANGULARMOMENTUM )
		{
			vec_ht_objects_AngularMomentum.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->L) );
		}
		else
		if ( body_properties[j].type ==  T_FORCE)
		{
			vec_ht_objects_force.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->m_vecForce) );
		}
		else
		if ( body_properties[j].type ==  T_TORQUE)
		{
			vec_ht_objects_torque.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->m_vecTorque) );
		}
		else
		if ( body_properties[j].type == T_contactFORCE )
		{
			vec_ht_objects_Contact_force.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->m_vecContactForce) );
		}
		else
		if ( body_properties[j].type == T_contactTORQUE )
		{
			vec_ht_objects_Contact_torque.push_back(ht);
			t.SetItemData( ht, reinterpret_cast<DWORD_PTR>(&pbody->m_vecContactTorque) );
		}
		else
			t.SetItemData( ht, body_properties[j].type  );
	}
	}
///////////////////////////////////////////////////////////////////////////////
}
catch (char *str)// )
{
	::MessageBox(0, GetWC(str), L"PopulateTree", MB_OK);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(0, L"Error", L"CLeftView::PopulateTree", MB_OK);
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CLeftView::UpdateTree()
{
	try{
//	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	if (pFrame == 0) return;
//	pFrame->ViewPotential();
//	CFortranWrapper& m_FortranWrapper = pFrame->m_FortranWrapper;
	if (!::IsWindow(m_hWnd)) return;
//	if (!::IsWindowVisible(m_ctlSysListView32.m_hWnd)) return;
//	SetFocus();
	CTreeCtrl& t = GetTreeCtrl();
	std::wostringstream oss;
	std::vector<HTREEITEM>::iterator i;
	CSystem* m_system = theApp.GetSystem();
//	Lock lock (theApp.protectBodies);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//EnterCriticalSection (&this->lock);
///////////////////////////////////////////////////////////////////////////////
//m_system->Set(objects& obj, unsigned int _enum, const objects& val)
//m_system->Get(const objects& obj, unsigned int _enum, objects& val)
	for (i = vec_ht_objects.begin(); i != vec_ht_objects.end(); ++i)
	{
		HTREEITEM ht = reinterpret_cast<struct _TREEITEM *>(*i);
		int ID = static_cast<int>(t.GetItemData(*i));
		objects obj(T_SYSTEM);
		objects val;
		m_system->Get(obj, ID, val);
	//	Set_Get_L pt = this->ml(ht) ;
	//	GetL gn = pt.second;
	//	oss << (this->*gn)() << ends;
		oss.str(L"");
		oss.clear();
		oss << val;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_position.begin(); i != vec_ht_objects_position.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		CRigidBody *  pbody = dynamic_cast<CRigidBody *>((CRigidBody *)t.GetItemData(t.GetParentItem(t.GetParentItem(*i))));
		if (pbody==0) return;
		Point3D& pt = *(Point3D*) t.GetItemData(*i);//pbody->x;
	//	CRigidBody *  pbody = (CRigidBody * ) t.GetItemData(GetParentItem(GetParentItem(*i)));
		oss << pt;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_LinearMomentum.begin(); i != vec_ht_objects_LinearMomentum.end(); ++i)
	{
		Vector3D& pt = *(Vector3D*) t.GetItemData(*i);
		oss.str(L"");
		oss.clear();
		oss << pt;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_AngularMomentum.begin(); i != vec_ht_objects_AngularMomentum.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& pt = *(Vector3D*) t.GetItemData(*i);
		oss << pt;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_velocity.begin(); i != vec_ht_objects_velocity.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& vel = *(Vector3D*) t.GetItemData(*i);
		oss << vel;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_force.begin(); i != vec_ht_objects_force.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& v = *(Vector3D*) t.GetItemData(*i);
		oss << v;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_torque.begin(); i != vec_ht_objects_torque.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& v = *(Vector3D*) t.GetItemData(*i);
		oss << v;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_Contact_force.begin(); i != vec_ht_objects_Contact_force.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& v = *(Vector3D*) t.GetItemData(*i);
		oss << v;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}
///////////////////////////////////////////////////////////////////////////////
	for (i = vec_ht_objects_Contact_torque.begin(); i != vec_ht_objects_Contact_torque.end(); ++i)
	{
		oss.str(L"");
		oss.clear();
		Vector3D& v = *(Vector3D*) t.GetItemData(*i);
		oss << v;
		t.SetItemText(*i, CString(oss.str().c_str()));
	}


	t.EnsureVisible(m_htSystemAngularMomentum);

}
catch (char *str)// )
{
	::MessageBox(0, CString(str), L"CLeftView::UpdateTree", MB_OK);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(0, L"error", L"CLeftView::UpdateTree", MB_OK);
}


}
///////////////////////////////////////////////////////////////////////////////
void CLeftView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	// NOTE: The NM_CLICK reflected notification only provides a plain NMHDR.
	// It is NOT an NMTVDISPINFO, so the item/label data is unavailable here.
	// Casting pNMHDR to NMTVDISPINFO* and reading item.pszText dereferences
	// memory past the end of the real NMHDR and crashes on every click.
	// Determine the clicked item safely via a hit-test on the cursor instead.
	UNREFERENCED_PARAMETER(pNMHDR);

	CTreeCtrl& t = GetTreeCtrl();

	CPoint pt;
	::GetCursorPos(&pt);
	t.ScreenToClient(&pt);

	UINT flags = 0;
	HTREEITEM hItem = t.HitTest(pt, &flags);
	if (hItem != 0 && (flags & TVHT_ONITEM))
	{
		CString text = t.GetItemText(hItem);
		string news(GetC((LPCWSTR) text));
	}
}
///////////////////////////////////////////////////////////////////////////////
void CLeftView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	static wchar_t buf[256];
	CSystem* m_system = theApp.GetSystem();
	Lock lock (theApp.protectBodies);

try
{
	TV_ITEM & item = pTVDispInfo->item;
	if (item.pszText == 0) return;// editing was cancelled


	wcsncpy_s(buf, item.pszText, 255);
	std::wistringstream iss(buf);

	CTreeCtrl& t = GetTreeCtrl();
	HTREEITEM ht = t.GetSelectedItem();
	ASSERT(ht == m_sel);
	ht = m_sel;
///////////////////////////////////////////////////////////////////////////////
//m_system->Set(objects& obj, unsigned int _enum, const objects& val)
//m_system->Get(const objects& obj, unsigned int _enum, objects& val)
try
{
	vector<HTREEITEM>::iterator i = find(vec_ht_objects.begin(), vec_ht_objects.end(), ht);
	if (i != vec_ht_objects.end())
	{
//		HTREEITEM ht = reinterpret_cast<struct _TREEITEM *>(*i);
		int ID = static_cast<int>(t.GetItemData(*i));
		objects val;
		if (!(iss>>val).fail())
		{
			objects obj(T_SYSTEM);
			m_system->Set(obj, ID, val);
			return;
		}
	}

	HTREEITEM htype = t.GetParentItem(ht);
	HTREEITEM hParent = t.GetParentItem(htype);
	CRigidBody *  pbody = dynamic_cast<CRigidBody *>((CRigidBody *)t.GetItemData(hParent));
	if (pbody)
	{
//		TCHAR s[256];
//		istrstream iss(s,255);
//		ostrstream oss(s,255);
		int ID = static_cast<int>(t.GetItemData(htype));
		if ( ID == T_POSITION ||
			 ID == T_VELOCITY ||
			 ID == T_MOMENTUM ||
			 ID == T_ANGULARMOMENTUM ||
			 ID == T_FORCE||
			 ID == T_TORQUE||
			 ID == T_contactFORCE ||
			 ID == T_contactTORQUE )
		{
			objects val(&ZERO_3D);
		if (!(iss>>val).fail())
		//	oss<<(*(Vector3D *)(t.GetItemData(ht)));
		//	if (!(iss>>val).fail())
			{
			//	objects body(*pbody);
				objects body(pbody);
				m_system->Set(body, ID, val);
			m_system->UpdateTree();
			val=*(Vector3D *)t.GetItemData(ht);//check
			}
		}
		else
		{
			objects val(0.0);
		if (!(iss>>val).fail())
		//	oss<<(*(double *)(t.GetItemData(ht)));
		//	if (!(iss>>val).fail())
			{
			//	objects body(*pbody);
				objects body(pbody);
				m_system->Set(body, ID, val);
			val=*(double *)t.GetItemData(ht);//check
			}
		}
		m_system->UpdateTree();
		m_system->DisplayData();
	}

	return;
}
catch (std::runtime_error*)// exc)
{
//	::MessageBox(0, exc->what(), "Error", MB_OK);
}
catch (char *)// str)
{
//	::MessageBox(0, str, "Error", MB_OK);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}
///////////////////////////////////////////////////////////////////////////////

}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}

}
///////////////////////////////////////////////////////////////////////////////
void CLeftView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CTreeCtrl& t = GetTreeCtrl();
	t.GetEditControl()->LimitText(127);
	m_sel = t.GetSelectedItem();
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////
BOOL CLeftView::PreTranslateMessage(MSG* pMsg) 
{
	CTreeCtrl& t = GetTreeCtrl();

		// GetKeyState( VK_CONTROL):
	// TODO: Add your specialized code here and/or call the base class
	if(t.GetEditControl() && (pMsg->message == WM_KEYDOWN))
	{
		CString m_Test;
		switch(pMsg->wParam)
		{
		case VK_RETURN:
	//		t.GetEditControl()->GetWindowText(m_Test);
//			if(t.SetItemText(t.m_CurItem.p_Item, m_Test) == NULL)
//				TRACE("Unable to Change Item Text(Enter Message)!!!\n");
			break;
		case VK_DELETE:
	//		t.GetEditControl()->SetWindowText("");
			break;
		case VK_ESCAPE:
			break;
		}
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	return CTreeView::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////

void CLeftView::OnDestroy() 
{
	CTreeView::OnDestroy();
//	if (NumerovDialog_hWnd && ::IsWindow(NumerovDialog_hWnd)) ::SendMessage(NumerovDialog_hWnd, WM_CLOSE,0,0);
}


#undef new_ID
#undef CONCAT2

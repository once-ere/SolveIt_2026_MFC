//SetGetParam.cpp
//

#include "stdafx.h"

#include "SolveIt.h"
#include "GLView.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "objects.h"

//#include "StackDatum.h"
//#include "Worker.h"
#include "Matherror.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
/////////////////////////////////////////////////////////////////////////////


#define ACCESS(x) \
	void __stdcall CLeftView::Set##x(const int& val){\
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);\
		CGLView* gl = pFrame->GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return;\
		objects sd=val; ::SendMessage(hWndGL, WM_USER_SetParam, CGLView::_##x, (LPARAM)&sd);}\
	int __stdcall CLeftView::Get##x(void) {\
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);\
		CGLView* gl = pFrame->GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return 0;\
		objects sd; if (0==::SendMessage(hWndGL, WM_USER_GetParam, CGLView::_##x, (LPARAM)&sd)) return sd.iVal;return 0;}

	ACCESS(NDATA)
#undef ACCESS

#define ACCESS(x) \
	void __stdcall CLeftView::Set##x(const double& val){\
		CGLView* gl = theApp.GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return;\
		objects sd=val; ::SendMessage(hWndGL, WM_USER_SetParam, CGLView::_##x, (LPARAM)&sd);}\
	double __stdcall CLeftView::Get##x(void) {\
		CGLView* gl = theApp.GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return 0;\
		objects sd; if (0==::SendMessage(hWndGL, WM_USER_GetParam, CGLView::_##x, (LPARAM)&sd)) return sd.dVal;return 0;}

//	ACCESS(glFrustum_left)
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



	void __stdcall CLeftView::SetglFrustum_left(const double& val){\
		CGLView* gl = theApp.GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return;\
		objects sd=val; ::SendMessage(hWndGL, WM_USER_SetParam, CGLView::_glFrustum_left, (LPARAM)&sd);}\
	double __stdcall CLeftView::GetglFrustum_left(void) {\
		CGLView* gl = theApp.GetGLView();\
		HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;\
		if (hWndGL==NULL) return 0;\
		objects sd; if (0==::SendMessage(hWndGL, WM_USER_GetParam, CGLView::_glFrustum_left, (LPARAM)&sd)) return sd.dVal;return 0;}

///////////////////////////////////////////////////////////////////////////////
// determines tic mark separation:
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void __stdcall CLeftView::Settic_mark_spacingStr(const wchar_t* val)
{
	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSystem* m_system	= theApp.GetSystem();
	m_system->m_tic_mark_spacingStr		= val;
	CGLView* gl = pFrame->GetGLView();
	if (gl==NULL) return;
	gl->SetPerspectiveOrFrustum();
	gl->Invalidate();
}
const wchar_t* __stdcall CLeftView::Gettic_mark_spacingStr(void)
{
	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSystem* m_system	= theApp.GetSystem();
	return _tcsdup(m_system->m_tic_mark_spacingStr.c_str());
}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void __stdcall CLeftView::SetxStr(const wchar_t* val)
{
	USES_CONVERSION;
	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSystem* m_system	= theApp.GetSystem();
	m_system->m_xStr		= CW2A(val);
	CGLView* gl = pFrame->GetGLView();
	if (gl==NULL) return;
	gl->SetPerspectiveOrFrustum();
	gl->Invalidate();
//	HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;
//	if (hWndGL==NULL) return;
//	objects sd(_tcsdup(val));
//	::SendMessage(hWndGL, WM_USER_SetParam, CGLView::_xStr, (LPARAM)&sd);

//	objects obj(val);
//	long res = gl->SetParam(CGLView::_xStr, obj);

}
const wchar_t* __stdcall CLeftView::GetxStr(void)
{
//	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	USES_CONVERSION;
	CSystem* m_system	= theApp.GetSystem();
	return _wcsdup(CA2W(m_system->m_xStr.c_str()));
//	CGLView* gl = pFrame->GetGLView();
//	if (gl==NULL) return 0;
//	objects obj;
//	long res = gl->GetParam(CGLView::_xStr, obj);
//	return _tcsdup(obj);//.sPtr->c_str()

//	HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;
//	if (hWndGL==NULL) return 0;
//	objects sd;
//	if (0==::SendMessage(hWndGL, WM_USER_GetParam, CGLView::_xStr, (LPARAM)&sd))
//		return _tcsdup(sd.sPtr->c_str());
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void __stdcall CLeftView::SetyStr(const wchar_t* val)
{
	USES_CONVERSION;
	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSystem* m_system	= theApp.GetSystem();
	m_system->m_yStr		= CW2A(val);
	CGLView* gl = pFrame->GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();
//	objects obj(val);
//	long res = gl->SetParam(CGLView::_yStr, obj);
//	HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;
//	if (hWndGL==NULL) return;
//	objects sd(_tcsdup(val));
//	::SendMessage(hWndGL, WM_USER_SetParam, CGLView::_yStr, (LPARAM)&sd);
}
const wchar_t* __stdcall CLeftView::GetyStr(void)
{
//	CMainFrame *pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	USES_CONVERSION;
	CSystem* m_system	= theApp.GetSystem();
	return _wcsdup(CA2W(m_system->m_yStr.c_str()));
//	CGLView* gl = pFrame->GetGLView();
//	if (gl==NULL) return 0;
//	objects obj;
//	long res = gl->GetParam(CGLView::_yStr, obj);
//	return _tcsdup(obj);//.sPtr->c_str()
//	HWND hWndGL = (gl==NULL)? 0:gl->m_hWnd;
//	if (hWndGL==NULL) return 0;
//	objects sd;
//	if (0==::SendMessage(hWndGL, WM_USER_GetParam, CGLView::_yStr, (LPARAM)&sd))
//		return _tcsdup(sd.sPtr->c_str());
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
#if 0
#define _A_MSG(x) \
LRESULT CGLView::OnGet_##x##(WPARAM wParam, LPARAM lParam)\
{\
	return (LRESULT) &x;\
}
	_A_MSG(glFrustum_left)
	_A_MSG(glFrustum_right)
	_A_MSG(glFrustum_bottom)
	_A_MSG(glFrustum_top)
#undef _A_MSG
	ON_REGISTERED_MESSAGE(WM_USER_INIT, OnInit)
	afx_msg LRESULT OnInit(WPARAM wParam, LPARAM lParam);
#endif
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnRedraw(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnGetParam(WPARAM wParam, LPARAM lParam)
{
	try
	{
	objects& sd = *reinterpret_cast<objects*>(lParam);
	switch (_paramTypes(wParam))
	{
		case _NDATA:
			sd = m_NDATA;
			break;
		case _glFrustum_left:
			sd = glFrustum_left;
			break;
		case _glFrustum_right:
			sd = glFrustum_right;
			break;
		case _glFrustum_bottom:
			sd = glFrustum_bottom;
			break;
		case _glFrustum_top:
			sd = glFrustum_top;
			break;

		case _glFrustum_znear:
			sd = glFrustum_znear;
			break;
		case _glFrustum_zfar:
			sd = glFrustum_zfar;
			break;

		case _fTransX:
			sd = m_fTransX;
			break;
		case _fTransY:
			sd = m_fTransY;
			break;
		case _fTransZ:
			sd = m_fTransZ;
			break;

		case _ZoomScale:
			sd = m_fZoomScale;
			break;
		case _ScaleWave:
			sd = m_fScaleWave;
			break;
#if 0
		case _xStr:
//			sd = _tcsdup(m_xStr);
			sd = (m_xStr);
			break;
		case _yStr:
//			sd = _tcsdup(m_yStr);
			sd = (m_yStr);
			break;
#endif
	}
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnGetParam Error: ", GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnGetParam Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"OnGetParam", MB_OK);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glTrans_X(const double& val)
{
	orig_fTransX = m_fTransX = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glTrans_Y(const double& val)
{
	orig_fTransY = m_fTransY = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glTrans_Z(const double& val)
{
	orig_fTransZ = m_fTransZ = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glRot_X(const double& val)
{
	orig_fRotX = m_fRotX = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glRot_Y(const double& val)
{
	orig_fRotY = m_fRotY = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glRot_Z(const double& val)
{
	orig_fRotZ = m_fRotZ = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::set_glFrustum_left(const double& val)
{
	glFrustum_left = orig_glFrustum_left = val;
	{
		Lock lock(mutexPlayData);
		m_fdelta = (glFrustum_right - glFrustum_left)/50;
	}
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::set_glFrustum_right(const double& val)
{
	glFrustum_right = orig_glFrustum_right = val;
	{
		Lock lock(mutexPlayData);
		m_fdelta = (glFrustum_right - glFrustum_left)/50;
	}
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::set_glFrustum_bottom(const double& val)
{
	glFrustum_bottom = orig_glFrustum_bottom = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::set_glFrustum_top(const double& val)
{
	glFrustum_top = orig_glFrustum_top = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::set_glFrustum_znear(const double& val)
{
	glFrustum_znear = orig_glFrustum_znear = val;
	if (glFrustum_znear <= 0.0) m_bPerspectiveTransformation = false;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::set_glFrustum_zfar(const double& val)
{
	glFrustum_zfar = orig_glFrustum_zfar = val;
	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
void CGLView::BoundingBox(const double& L,const double& R,const double& B,const double& T,const double& N,const double& F)
{
	glFrustum_left = orig_glFrustum_left = L;
	glFrustum_right = orig_glFrustum_right = R;
	glFrustum_bottom = orig_glFrustum_bottom = B;
	glFrustum_top = orig_glFrustum_top = T;
	glFrustum_znear = orig_glFrustum_znear = N;
	glFrustum_zfar = orig_glFrustum_zfar = F;

	m_bPerspectiveTransformation	= N > 0.0?	true:false;

	SetPerspectiveOrFrustum();
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnSetParam(WPARAM wParam, LPARAM lParam)
{
	try
	{
	objects& sd = *reinterpret_cast<objects*>(lParam);
	switch (_paramTypes(wParam))
	{
		case _NDATA:
			m_NDATA = sd;
			Invalidate();
			break;

		case _glFrustum_left:
			set_glFrustum_left(sd);
			break;
		case _glFrustum_right:
			set_glFrustum_right(sd);
			break;
		case _glFrustum_bottom:
			set_glFrustum_bottom(sd);
			break;
		case _glFrustum_top:
			set_glFrustum_top(sd);
			break;
		case _glFrustum_znear:
			set_glFrustum_znear(sd);
			break;
		case _glFrustum_zfar:
			set_glFrustum_zfar(sd);
			break;

		case _fTransX:set_glTrans_X(sd);
			break;
		case _fTransY:set_glTrans_Y(sd);
			break;
		case _fTransZ:set_glTrans_Z(sd);
			break;


		case _ZoomScale:
			m_fZoomScale = sd;
			Invalidate();
			break;
		case _ScaleWave:
			m_fScaleWave = sd;
			Invalidate();
			break;
#if 0
		case _xStr:
			m_xStr = sd;
		//	m_xStr = _tcsdup(sd);
		//	_tcsncpy(m_xStr, sd.sPtr->c_str(), sizeof(m_xStr)/sizeof(m_xStr[0]) - 1);
			Invalidate();
			break;
		case _yStr:
			m_yStr = sd;
		//	m_yStr = _tcsdup(sd);
		//	_tcsncpy(m_yStr, sd.sPtr->c_str(), sizeof(m_yStr)/sizeof(m_yStr[0]) - 1);
			Invalidate();
			break;
#endif
			
	}
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	CLeftView& m_FortranWrapper = pFrame->m_FortranWrapper;
//	m_FortranWrapper.Initialize();
	::PostMessage(pFrame->m_hWnd, WM_USER_INIT, 0, 0);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ", GetWC(exc->what()), MB_OK);
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnSetParam Error: ", GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"OnGetParam", MB_OK);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#if 0
long CGLView::GetParam(WPARAM wParam, objects& obj)
{
	try
	{
	switch (_paramTypes(wParam))
	{
		case _NDATA:
			obj = m_NDATA;
			break;
		case _glFrustum_left:
			obj = glFrustum_left;
			break;
		case _glFrustum_right:
			obj = glFrustum_right;
			break;
		case _glFrustum_bottom:
			obj = glFrustum_bottom;
			break;
		case _glFrustum_top:
			obj = glFrustum_top;
			break;
		case _ZoomScale:
			obj = m_fZoomScale;
			break;
		case _ScaleWave:
			obj = m_fScaleWave;
			break;
#if 0
		case _xStr:
			obj = m_xStr;
			break;
		case _yStr:
			obj = m_yStr;
			break;
#endif
	}
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(exc->what()), MB_OK);
		return -1;
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnGetParam Error: ",  GetWC(except->name), MB_OK);
		return -2;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnGetParam Error: ", str, MB_OK);
		return -3;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", "OnGetParam", MB_OK);
		return -4;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CGLView::SetParam(WPARAM wParam, const objects& obj)
{
	try
	{
	switch (_paramTypes(wParam))
	{
		case _NDATA:
			m_NDATA = obj;
			Invalidate();
			break;
		case _glFrustum_left:
			glFrustum_left = obj;
			SetPerspectiveOrFrustum();
			break;
		case _glFrustum_right:
			glFrustum_right = obj;
			SetPerspectiveOrFrustum();
			break;
		case _glFrustum_bottom:
			glFrustum_bottom = obj;
			SetPerspectiveOrFrustum();
			break;
		case _glFrustum_top:
			glFrustum_top = obj;
			SetPerspectiveOrFrustum();
			break;
		case _ZoomScale:
			m_fZoomScale = obj;
			Invalidate();
			break;
		case _ScaleWave:
			m_fScaleWave = obj;
			Invalidate();
			break;
#if 0
		case _xStr:
			m_xStr = obj;
			Invalidate();
			break;
		case _yStr:
			m_yStr = obj;
			Invalidate();
			break;
#endif
			
	}
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	::PostMessage(pFrame->m_hWnd, WM_USER_INIT, 0, 0);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(exc->what()), MB_OK);
		return -1;
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(except->name), MB_OK);
		return -2;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
		return -3;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", "OnGetParam", MB_OK);
		return -4;
	}
	return 0;
}
#endif

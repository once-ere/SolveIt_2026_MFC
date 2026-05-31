// Coords.cpp : Helper implementation of COpenGLObj

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"

#include "GLView.h"
#include "MainFrm.h"

#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
#include "ChangeCharge.h"
#include "LeftView.h"
#include "PlayMap.h"
#include "QM/qm_1dscatt.h"
#include "QM/qm_2dscatt.h"
#include "QM/qm_ground_mc_1d.h"
#include "QM/qmstationary1d.h"
//#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Scatt1D.h"
#include "Box.h"
#include "System.h"
#include "CalcMap.h"
#include "SolveItView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// gluPerspective() => left = -right;top = -bottom
///////////////////////////////////////////////////////////////////////////////
extern Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	wchar_t buf[256];
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	const MSG& aMSG = *GetCurrentMessage();
	POINT pt;
	pt.x = aMSG.pt.x;
	pt.y = aMSG.pt.y;
	ScreenToClient(&pt);
	{
	/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	const int &screen_x = pt.x;
	const int &screen_y = pt.y;
	oss << "( "	<< pt.x << " , " << pt.y << ")["<< screen_x << " , " << screen_y << " ]"  << ends;
	wstring str(buf);
	pFrame->SetStatusBarText(str, 1);
	}
///////////////////////////////////////////////////////////////////////////////
// Allow the user to simulate a middle button by chording
// left an right mouse buttons
	if(nFlags & MK_RBUTTON){
// The right button has already been pressed
//		StartMove(point.x, point.y);
		deFormedView::OnLButtonDown(nFlags, point);
		return;
	}

// Grab control of the mouse until the user
// releases the left mouse button.
	SetCapture();
// Remember that the left button is down
	l_button_down = true;
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_bDoVerhulstDynamics && bCtrl)
	{
		glDrawMap_List2( point, (bShift?11:1));
		return;
		CGLView* pGLView = this;//theApp.GetGLView();
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		m_system->m_CurrentMousePosition	= point;
		Vector2D pt(point);
	//	pGLView->ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		pGLView->ComputeWorldCoords(pt.x, pt.y, z_Pos, false);
		if (bShift)
		{
		//	Lock lock(mutexPlayData);
			m_system->player->g_Rate	= pt.y;
		}
		m_system->m_CurrentWorldPosition	= pt;
		double w = (pGLView->glFrustum_right - pGLView->glFrustum_left);
//		double x = (pGLView->glFrustum_right + pGLView->glFrustum_left)/2;
		double h = (pGLView->glFrustum_top - pGLView->glFrustum_bottom);
//		double y = (pGLView->glFrustum_top + pGLView->glFrustum_bottom)/2;
//		double a = w/h;
		double s = 2*m_system->m_fScale;
		pGLView->set_glFrustum_left(pt.x - w/s);
		pGLView->set_glFrustum_right(pt.x + w/s);
		pGLView->set_glFrustum_bottom(pt.y - h/s);
		pGLView->set_glFrustum_top(pt.y + h/s);



		wchar_t buf[128];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
		oss	<< "  ( " << pt.x << " , " << pt.y << " )  " ;
	//	oss<<float(Znear)<< "," <<float(Zfar);
		oss	<< ends;
		wstring str(buf);
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		pFrame->SetStatusBarText(str, 2);

		oss.seekp(0);
		oss	<< m_ZoomLevel << ends;
	//	oss<<float(Znear)<< "," <<float(Zfar);
		str	= buf;
		pFrame->SetStatusBarText(str, 3);
		return;
	}
	if (m_system->m_bDoMapper && !bCtrl)
	{
	//	Invalidate();
		return;
	}
	if (m_system->m_bDoMapper && bCtrl)
	{
		ComputeMapperCoords(pt);
		return;
		Lock lock (theApp.protectMapper);
		++m_ZoomLevel;
		CGLView* pGLView = this;//theApp.GetGLView();
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		m_system->m_CurrentMousePosition	= point;
		Vector2D pt(point);
//		pGLView->ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		pGLView->ComputeWorldCoords(pt.x, pt.y, z_Pos, false);
		m_system->m_CurrentWorldPosition	= pt;
		double w = (pGLView->glFrustum_right - pGLView->glFrustum_left);
//		double x = (pGLView->glFrustum_right + pGLView->glFrustum_left)/2;
		double h = (pGLView->glFrustum_top - pGLView->glFrustum_bottom);
//		double y = (pGLView->glFrustum_top + pGLView->glFrustum_bottom)/2;
//		double a = w/h;
		double s = 2*m_system->m_fScale;
		const double zoomedMax = static_cast<double>(maxi_) + 40.0 * (m_system->m_fScale - 1.0);
		maxi_ = zoomedMax > 0.0 ? static_cast<unsigned int>(zoomedMax) : 0U;


		orig_glFrustum_left=glFrustum_left	=pt.x - w/s;
		orig_glFrustum_right=glFrustum_right	=pt.x + w/s;
		orig_glFrustum_bottom=glFrustum_bottom=pt.y - h/s;
		orig_glFrustum_top=glFrustum_top	=pt.y + h/s;



		
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		//wglMakeCurrent(hDC, m_hRC);

	//	glViewport(0, 0, viewWidth, viewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
	//	glOrtho( 
	//			glFrustum_left,
	//			glFrustum_right,
	//			glFrustum_bottom,
	//			glFrustum_top,
	//			glFrustum_znear,
	//			glFrustum_zfar
	//		);
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
glTranslated(0.375, 0.375, 0.0);
	//	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
	//	glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
	//	glGetIntegerv(GL_VIEWPORT , m_viewport);
		GLenum err = glGetError();
	//	ASSERT(err == GL_NO_ERROR);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		m_bReCalc = true;

		wchar_t buf[128];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
		oss	<< "  ( " << pt.x << " , " << pt.y << " )  " ;
//		oss<<float(Znear)<< "," <<float(Zfar);
		oss	<< ends;
		wstring str(buf);
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		pFrame->SetStatusBarText(str, 2);

		oss.seekp(0);
		oss	<< m_ZoomLevel << ends;
	//	oss<<float(Znear)<< "," <<float(Zfar);
		str	= buf;
		pFrame->SetStatusBarText(str, 3);
		size_x_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		size_y_ = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);
		if (m_system->mapper)
			m_system->mapper->PostThreadMessage(WM_USER_SetParam, 0, 0);
		calcPixelRows(maxi_);

		Invalidate();
		return;

		if (m_system->mapper)
		{
/*
//	m_system->mapper->reshaped_=true;
	m_system->mapper->ax0_ = pt.x - w/s;
	m_system->mapper->ex_ = pt.x + w/s;
	m_system->mapper->ay_ = pt.y - h/s;
	m_system->mapper->ey_ = pt.y + h/s;
	// Recalc real plane parameters
	m_system->mapper->sx0_ = (m_system->mapper->ex_ - m_system->mapper->ax0_) / ((double) m_system->mapper->w0_);
	m_system->mapper->sy_ = (m_system->mapper->ey_ - m_system->mapper->ay_) / ((double) m_system->mapper->h_);
*/
		m_system->mapper->PostThreadMessage(WM_USER_SetParam, 0, 0);
//	//	m_system->mapper->PostThreadMessage(WM_USER_START_PASS, 0, 0);
		}
	//	Fire_ViewChange();
	}


	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	m_nTrueHit = 0;//false
	rigidBodyManager->HitTest(point, m_nTrueHit);
	if (m_system->m_worldDimension==CSystem::two && m_nTrueHit==0 && bCtrl)
	{
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		Vector2D pt(point);
//		ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		ComputeWorldCoords(pt.x, pt.y, z_Pos, false);

		static int m_nID=1;
		TCHAR sz[64];
		/*ostrstream*/ std::wstringstream oss(sz, sizeof(sz)/sizeof(sz[0]));
		oss << 'Q' << m_nID++ << ends;
		std::wstring bstrCharge(sz);
		std::wstring bstr = bstrCharge;
		bstr += L" =  sphere;";
		FireWriteCommand(bstr.c_str());

		bstr = bstrCharge;
		bstr += L".charge = 1;";
		FireWriteCommand(bstr.c_str());

		Point3D point(pt.x, pt.y, 0);
		oss.seekp(0);
		oss << ".position = [" << pt.x << ", " <<  pt.y << ", 0];" <<  ends;
		bstr = bstrCharge;
		bstr += sz;
		FireWriteCommand(bstr.c_str());

		double c = fabs(cos(1));
		double s = fabs(sin(1));
		Vector3D color( 1, c*c, s*s );
		oss.seekp(0);
		oss << ".color = [" << color.x << ", " <<  color.y << ", " <<  color.z << "];" <<  ends;
		bstr = bstrCharge;
		bstr += sz;
		FireWriteCommand(bstr.c_str());
		m_system->UpdateTree();
		m_system->DisplayData();
	}
	if (m_nTrueHit)
	{
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));
	}

	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
//	if (bCtrl && fieldsManager->m_bValidCurrentLoopMagneticField && !m_nTrueHit)
	if (bCtrl && !m_nTrueHit)
	{
		double x = point.x;
		double y = point.y;

		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		Point3D p(x, y, z_Pos);
	//	ComputeWorldCoords(&p.x, &p.y, &p.z);
		ComputeWorldCoords(p.x, p.y, p.z, false);
		
		fieldsManager->m_selectedFieldPoint = p;
		l_button_down = false;
	//	m_system->FieldLine(CSystem::thread_fieldline_magnetic_current_loop);
		if (fieldsManager->m_bShowFieldLines)
		{
			fieldsManager->ComputeFieldLine(m_FieldLinesType);
		}
//		return;
	}
//m_system->m_bDoQM_Scatt1D
	if (m_system->m_worldDimension==CSystem::one)
	{
	l_button_down = true;
	CBox* pBox = HitTestRectangleItems(point);
	long nPotentialWidth = viewWidth/20;//m_NDATA/20;
	if (nPotentialWidth<25) nPotentialWidth  = 25;
	if (!pBox && bCtrl)
	{
		RECT r={point.x, point.y, point.x+nPotentialWidth,viewHeight/2};
		if (r.bottom<r.top)
		{
			r.bottom = r.top;
			r.top = viewHeight/2;//y;
		}
		if (r.bottom != r.top)
		{
			pBox = AddPot(r);
			ASSERT(pBox!=0);
			m_nTrueHit=32;
		}
	}
	else
	{
		m_nTrueHit=64;
	}
	if (pBox) m_pSelectedBox = pBox;
	}

// Now handle rotations
	if ((m_system->m_worldDimension!=CSystem::one || m_system->m_worldDimension!=CSystem::two)&&!bCtrl && !bShift)
	{
		startRotation(point.x, point.y);
	}
	else
	{
		beginx = point.x;
		beginy = point.y;
//	m_begin_x = xPos;
//	m_begin_y = yPos;
	}

	deFormedView::OnLButtonDown(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
// OnLButtonUp
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	bool m_bMouseDown = l_button_down? true:false;
// The left button is no longer down
	l_button_down = false;
	int nTrueHit = m_nTrueHit;//
	m_nTrueHit = 0;//
// Give up control of the mouse
	ReleaseCapture();
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;
///////////////////////////////////////////////////////////////////////////////
// Now handle rotations
	CSystem* m_system	= theApp.GetSystem();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	if ((m_system->m_worldDimension == CSystem::three )&&!bCtrl && !bShift)
	{
	//	rotation(point.x, point.y);
		if (nTrueHit == 0)
		{
			rotation(point.x, point.y);
		}
	}
	if (m_system->m_worldDimension!=CSystem::two)
	{
		if (nTrueHit == 0)
		{
	//		rotation(point.x, point.y);
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	
	if (m_system->m_worldDimension==CSystem::two && bCtrl && nTrueHit)
	{
		if (rigidBodyManager->m_pSelectedRigidBody)
		{
			if (::PtInRect(&m_rcTrash, point))
			{
				std::wstring bstrCharge(rigidBodyManager->m_pSelectedRigidBody->name.c_str());
				bstrCharge += L".delete;";
				FireWriteCommand(bstrCharge.c_str());
				return;
		//		RemovePot(m_pSelectedBox);
		//		rigidBodyManager->m_pSelectedRigidBody = 0;
		//		Fire_ViewChange();
			}
		}
	}


///////////////////////////////////////////////////////////////////////////////

//m_system->m_bDoQM_Scatt1D
	if (m_system->m_worldDimension==CSystem::one)// && bCtrl)
	{
	if (m_pSelectedBox)
	{
		if (::PtInRect(&m_rcTrash, point))
		{
			RemovePot(m_pSelectedBox);
			m_pSelectedBox = 0;
			Fire_ViewChange();
		}
		else
		{
		RECT& r = m_pSelectedBox->rc;
		long h = viewHeight/2;
		if (r.bottom<r.top)
		{
			long y = r.bottom;
			r.bottom = r.top;
			r.top = y;
		}
		if (r.top >= h)
		{
			r.top = h;
		}
		else if (r.top < h)
		{
			r.bottom = h;
		}
//##		if (m_pCView->m_hBitmapBackground) DeleteObject(m_pCView->m_hBitmapBackground);
//##		m_pCView->m_hBitmapBackground=0;
		DefinePotential();
		theApp.GetSystem()->data__QM_Scatt1D->OnResetViewSource();
		}
		Fire_ViewChange();
	}
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_pSelectedBox && m_nTrueHit==32)
	{
		nTrueHit		= 0;
		m_pSelectedBox	= 0;
	}
///////////////////////////////////////////////////////////////////////////////
	if (bCtrl && rigidBodyManager->m_pSelectedRigidBody)
	{
	double z=0;
	if (rigidBodyManager->m_pSelectedRigidBody) z = rigidBodyManager->m_pSelectedRigidBody->m_f_zScreen;
	else
	{
//z=1 <-> m_fFarPlane;z=0 <-> m_fNearPlane;
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
//		ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		z = z_Pos;
	}
	Point3D p(point.x, point.y, z);
//	ComputeWorldCoords(&p.x, &p.y, &p.z);
	ComputeWorldCoords(p.x, p.y, p.z, false);
		Point3D& CM = rigidBodyManager->m_pSelectedRigidBody->x;
//		p.z = CM.z;
		{
			double r = 0.8;
			double& x = p.x;
			double& y = p.y;
		
			double d = sqrt(x * x + y * y);
			if (d < r * 0.70710678118654752440f) {
// Inside sphere
				p.z = (sqrt(r * r - d * d));
			} 
			else {
// On hyperbola
				double t = r / 1.41421356237309504880f;
				p.z = t * t / d;
			}
		}
		Vector3D diff = p - CM;

		if (diff.norm() > 1.0*rigidBodyManager->m_pSelectedRigidBody->m_fRadius )
		{
			if (bShift)
			{
				p.normalize();
				p *= CM.norm();
			}
			rigidBodyManager->m_pSelectedRigidBody->set_Position(p);
			m_system->UpdateTree();
			m_system->DisplayData();
			Fire_ViewChange();
		}
	}
///////////////////////////////////////////////////////////////////////////////
	deFormedView::OnLButtonUp(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
// OnRButtonDown
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnRButtonDown(UINT nFlags, CPoint point)
{

	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;
#if 0
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_bDoMapper && bCtrl)
	{
		Lock lock (theApp.protectBodies);
		CGLView* pGLView = this;//theApp.GetGLView();
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
		//ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		m_system->m_CurrentMousePosition	= point;
		Vector2D pt(point);
		//ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		ComputeWorldCoords(pt.x, pt.y, z_Pos, false);
		m_system->m_CurrentWorldPosition	= pt;
		double w = (pGLView->glFrustum_right - pGLView->glFrustum_left);
		double h = (pGLView->glFrustum_top - pGLView->glFrustum_bottom);
		double s = 2*m_system->m_fScale;
		maxi_	-= 40*(m_system->m_fScale-1);
		if (maxi_< 40) maxi_ = 40;
		glFrustum_left	=pt.x - w*s;
		glFrustum_right	=pt.x + w*s;
		glFrustum_bottom=pt.y - h*s;
		glFrustum_top	=pt.y + h*s;
		if (glFrustum_right<glFrustum_left)
		{
			double x		= glFrustum_left;
			glFrustum_left	= glFrustum_right;
			glFrustum_right	= x;
		}
		if (glFrustum_top<glFrustum_bottom)
		{
			double x			= glFrustum_bottom;
			glFrustum_bottom	= glFrustum_top;
			glFrustum_top		= x;
		}
		if (glFrustum_right==glFrustum_left)
		{
			glFrustum_right	*= 1.01;
			glFrustum_left	*= 0.99;
		}
		if (glFrustum_top==glFrustum_bottom)
		{
			glFrustum_top		*= 1.01;
			glFrustum_bottom	*= 0.99;
		}

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		//wglMakeCurrent(hDC, m_hRC);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		glOrtho( 
				glFrustum_left,
				glFrustum_right,
				glFrustum_bottom,
				glFrustum_top,
				glFrustum_znear,
				glFrustum_zfar
			);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
		glGetIntegerv(GL_VIEWPORT , m_viewport);
		GLenum err = glGetError();
	//	ASSERT(err == GL_NO_ERROR);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		Invalidate();
		deFormedView::OnRButtonDown(nFlags, point);
		return;
}
#endif

// Allow the user to simulate a middle button by chording
// left an right mouse buttons
	if(nFlags & MK_LBUTTON){
// The left button has already been pressed
//		StartMove(point.x, point.y);
		deFormedView::OnRButtonDown(nFlags, point);
		return;
	}
// Grab control of the mouse
	SetCapture();
//	StartScale(point.x);
	deFormedView::OnRButtonDown(nFlags, point);
}

void CGLView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// Give up mouse control
	ReleaseCapture();
	
	deFormedView::OnRButtonUp(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void CGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	CSystem* m_system	= theApp.GetSystem();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	m_pointCurrent=point;//{xPos,yPos};
	deFormedView::OnMouseMove(nFlags, point);
//	m_pointCurrent=point;//{xPos,yPos};
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	wchar_t buf[256];
	wchar_t buf2[128];
	/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
	/*ostrstream*/ std::wstringstream oss2(buf2, sizeof buf2);
	const int &screen_x = point.x;
	const int &screen_y = point.y;
	oss	<< "[ " << screen_x << " , " << screen_y << " ->  " ;

// Now handle rotations
	if (l_button_down && (m_system->m_bDoQMStationary2D||(m_system->m_worldDimension == CSystem::three)))
	if (!bCtrl && !bShift)
	{
		if (m_nTrueHit == 0)
		{
			rotation(point.x, point.y);
		}
	}
	{
//first get the Screen Coords of the origin:
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == gluPerspective_zNear
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		long ret = ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		if (ret == S_OK)
		{

		double x=screen_x;
		double y=screen_y;
		double z=z_Pos;
	//	glReadPixels (x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &z);
	//	GLenum err = glGetError();
	//	ASSERT(err == GL_NO_ERROR);
//		x -= x_Pos;
//		y -= y_Pos;
//		z -= z_Pos;
	//	ComputeWorldCoords(&x, &y, &z);
		ComputeWorldCoords(x, y, z, false);
		oss2	<< "  ( " << float(x) << " , " << float(y)  << " , " << float(z) << " )  " << ends;
//		oss<<float(Znear)<< "," <<float(Zfar);//
//		oss<<float(m_modelMatrix[0])<< "," <<float(m_modelMatrix[5])<< "," <<float(m_modelMatrix[10])<< "," <<float(m_modelMatrix[15]);
//		oss<<float(m_projMatrix[0])<< "," <<float(m_projMatrix[5])<< "," <<float(m_projMatrix[10])<< "," <<float(m_projMatrix[15]);
	//	ComputeScreenCoords(&x, &y, &z);
		ComputeScreenCoords(x, y, z, false);
		oss	<< " " << float(x) << " , " << float(y)  << " , " << float(z) << " ]  " ;
		oss << buf2;
		oss << float(Znear) << "," << float(Zfar);//
		}
	}
	oss	<< ends;
	wstring str(buf);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->SetStatusBarText(str, 2);
	if (m_system->m_bDoMapper)//&& bCtrl)
	{
		return;
	}
	if ( m_system->m_bDoVerhulstDynamics )//&& bCtrl)
	{
//		return;
	}

	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	double z=0;
	if (rigidBodyManager->m_pSelectedRigidBody) z = rigidBodyManager->m_pSelectedRigidBody->m_f_zScreen;
	else
	{
//z=1 <-> m_fFarPlane;z=0 <-> m_fNearPlane;
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		z = z_Pos;
	}
	Point3D p(point.x, point.y, z);
//	ComputeWorldCoords(&p.x, &p.y, &p.z);
	ComputeWorldCoords(p.x, p.y, p.z, false);
	fieldsManager->m_electrostatic_currentFieldLinePoint2D	= Vector2D(p.x, p.y);
	fieldsManager->m_electrostatic_currentFieldLinePoint		= Vector3D(p.x, p.y, p.z);
	fieldsManager->m_CurrentMousePosition = Vector2D (p.x, p.y);
	if (bShift && fieldsManager->m_bShowFieldLines)
	{
		fieldsManager->ComputeFieldLine(m_FieldLinesType);
	}
	if (bShift && fieldsManager->m_bShowEquipotentials)
	{
		fieldsManager->ComputeEquipotentialLine();
	}
	if (m_system->m_worldDimension==CSystem::two)
	{
		if (bShift && fieldsManager->m_bShowVectorFields)//record electric field vector at current position
		{
			Vector2D& pt = fieldsManager->m_CurrentMousePosition;
			Vector2D e(fieldsManager->E(pt));
			PairVV2D pairVV(pt,e);
			fieldsManager->m_vecPair_2Delectrostatic.push_back(pairVV);
		}
		Fire_ViewChange();// show electric field vector at current position
	}
	if (bCtrl && rigidBodyManager->m_pSelectedRigidBody)
	{
		Point3D& CM = rigidBodyManager->m_pSelectedRigidBody->x;
//		p.z = CM.z;
		{
			double r = 0.8;
			double& x = p.x;
			double& y = p.y;
		
			double d = sqrt(x * x + y * y);
			if (d < r * 0.70710678118654752440f) {
// Inside sphere
				p.z = (sqrt(r * r - d * d));
			} 
			else {
// On hyperbola
				double t = r / 1.41421356237309504880f;
				p.z = t * t / d;
			}
		}
		Vector3D diff = p - CM;

		if (diff.norm() > 1.0*rigidBodyManager->m_pSelectedRigidBody->m_fRadius )
		{
			if (bShift)
			{
				p.normalize();
				p *= CM.norm();
			}
			rigidBodyManager->m_pSelectedRigidBody->set_Position(p);
			m_system->UpdateTree();
			m_system->DisplayData();
			Fire_ViewChange();
		}
	}
//m_system->m_bDoQM_Scatt1D
	if (m_system->m_worldDimension==CSystem::one)
	{
	if (m_pSelectedBox && m_nTrueHit==64)
	{
		long x = point.x - beginx;
		long y = point.y - beginy;
		beginx = point.x;
		beginy = point.y;
		long h = viewHeight/2;
		m_pSelectedBox->Resize( x,  y,  viewHeight/2,  bCtrl);
	//	m_bPotentialChanged = true;
		DefinePotential();
//		OnResetViewSource();
		RECT& rc = m_pSelectedBox->rc;
#define ScaledSign(r) (r.top>=h? -1.0/h:(r.bottom<=h? 1.0/h:0.0) )
		double V = ScaledSign(rc)*fabs(rc.bottom-rc.top)*theApp.GetSystem()->data__QM_Scatt1D->m_fScalePotential;
#undef ScaledSign
		double X = (double(point.x))/(m_NDATA-1);

		if (!bCtrl)
	swprintf_s(buf, _countof(buf), L"( x=%lg , U=%lg )( %d , %d )( t=%d , b=%d , l=%d , r=%d )",
					X,V,
					point.x,point.y,
					rc.top, rc.bottom, rc.left, rc.right);

		else
	swprintf_s(buf, _countof(buf), L"( x=%lg , U=%lg , Width=%lg )( %d , %d )( right-left=%d )",
					X,V,
					(double(rc.right-rc.left))/(m_NDATA-1),
					point.x,point.y,
					rc.right-rc.left
					);
/*
#define Y(r) (r.top>=h? h-r.bottom:(r.top<h? h-r.top:0) )
*/
		wstring str(buf);
		pFrame->SetStatusBarText(str, 3);
		Fire_ViewChange();
	}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	deFormedView::OnLButtonDblClk(nFlags, point);
///////////////////////////////////////////////////////////////////////////////
try{
if (notifications_mutex.wait(500))
{
	if (!notifications.empty())
	{
		list<VoidFn_VoidPtr>::iterator inotifications;
		for (inotifications = notifications.begin(); inotifications != notifications.end(); ++inotifications)
		{
			(*inotifications)();
		}
	}
	notifications_mutex.Release();
	Invalidate();
//	Fire_ViewChange();
}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		ASSERT(0);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	m_nTrueHit = 0;//false
	rigidBodyManager->HitTest( point, m_nTrueHit);
	if (m_nTrueHit==0) return;
	if (rigidBodyManager->m_pSelectedRigidBody==0) return;
	{
		CChangeCharge dc(rigidBodyManager->m_pSelectedRigidBody->m_fCharge, rigidBodyManager->m_pSelectedRigidBody->name);
		if (dc.DoModal()==IDOK) {
			TCHAR sz[64];
			/*ostrstream*/ std::wstringstream oss(sz, sizeof(sz)/sizeof(sz[0]));
			double c = fabs(cos(dc.m_fCharge));
			double s = fabs(sin(dc.m_fCharge));
			Vector3D color;
//0.1 == yellow
// -1 == blue
			if ( dc.m_fCharge > 0.0) color = Vector3D ( 1, c*c, s*s );
			else color = Vector3D ( c*c, s*s , 1);
			std::wstring bstrCharge(rigidBodyManager->m_pSelectedRigidBody->name.c_str());
			oss << ".charge = " <<  dc.m_fCharge << ";"  <<  ends;
			std::wstring bstr = bstrCharge;
			bstr += sz;
			FireWriteCommand(bstr.c_str());

			oss.seekp(0);
			oss << ".color = [" << color.x << ", " <<  color.y << ", " <<  color.z << "];" <<  ends;
			bstr = bstrCharge;
			bstr += sz;
			FireWriteCommand(bstr.c_str());
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	deFormedView::OnRButtonDblClk(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// Allow the user to zoom using a wheel mouse
//	ScaleWheel(zDelta/120);
	return /*deFormedView*/deFormedView::OnMouseWheel(nFlags, zDelta, pt);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CGLView::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	//	case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
{
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;

		const MSG& aMSG = *GetCurrentMessage();
		long xPos = aMSG.pt.x;//LOWORD(lParam);  // horizontal position of cursor 
		long yPos =  aMSG.pt.y;//HIWORD(lParam);  // vertical position of cursor 
		POINT point={xPos,yPos};
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_bDoVerhulstDynamics && bCtrl)
	{
		glDrawMap_List2( aMSG.pt, (bShift?10:0) );
		return TRUE;
		CGLView* pGLView = this;//theApp.GetGLView();
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		m_system->m_CurrentMousePosition	= point;
		Vector2D pt(point);
//		pGLView->ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		pGLView->ComputeWorldCoords(pt.x, pt.y, z_Pos, false);
		if (bShift)
		{
//			Lock lock(mutexPlayData);
			m_system->player->g_Rate	= pt.y;
		}
		m_system->m_CurrentWorldPosition	= pt;
		double w = (pGLView->glFrustum_right - pGLView->glFrustum_left);
//		double x = (pGLView->glFrustum_right + pGLView->glFrustum_left)/2;
		double h = (pGLView->glFrustum_top - pGLView->glFrustum_bottom);
//		double y = (pGLView->glFrustum_top + pGLView->glFrustum_bottom)/2;
//		double a = w/h;
		double s = 2*m_system->m_fScale;
		glFrustum_left	=pt.x - w*s;
		glFrustum_right	=pt.x + w*s;
		glFrustum_bottom=pt.y - h*s;
		glFrustum_top	=pt.y + h*s;
		if (glFrustum_right<glFrustum_left)
		{
			double x		= glFrustum_left;
			glFrustum_left	= glFrustum_right;
			glFrustum_right	= x;
		}
		if (glFrustum_top<glFrustum_bottom)
		{
			double x			= glFrustum_bottom;
			glFrustum_bottom	= glFrustum_top;
			glFrustum_top		= x;
		}
		if (glFrustum_right==glFrustum_left)
		{
			glFrustum_right	*= 1.01;
			glFrustum_left	*= 0.99;
		}
		if (glFrustum_top==glFrustum_bottom)
		{
			glFrustum_top		*= 1.01;
			glFrustum_bottom	*= 0.99;
		}
		pGLView->set_glFrustum_left(glFrustum_left);
		pGLView->set_glFrustum_right(glFrustum_right);
		pGLView->set_glFrustum_bottom(glFrustum_bottom);
		pGLView->set_glFrustum_top(glFrustum_top);
			return TRUE;
	}
	if (m_system->m_bDoMapper && bCtrl)
	{
		ComputeMapperCoords(aMSG.pt, false);
		return TRUE;
		Lock lock (theApp.protectMapper);
		--m_ZoomLevel;
		CGLView* pGLView = this;//theApp.GetGLView();
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		m_system->m_CurrentMousePosition	= point;
		Vector2D pt(point);
	//	pGLView->ComputeWorldCoords(&pt.x, &pt.y, &z_Pos);
		pGLView->ComputeWorldCoords(pt.x, pt.y, z_Pos, false);
		m_system->m_CurrentWorldPosition	= pt;
		double w = (pGLView->glFrustum_right - pGLView->glFrustum_left);
		double h = (pGLView->glFrustum_top - pGLView->glFrustum_bottom);
		double s = 2*m_system->m_fScale;
		const double zoomedMax = static_cast<double>(maxi_) - 40.0 * (m_system->m_fScale - 1.0);
		maxi_ = zoomedMax > 40.0 ? static_cast<unsigned int>(zoomedMax) : 40U;
		glFrustum_left	=pt.x - w*s;
		glFrustum_right	=pt.x + w*s;
		glFrustum_bottom=pt.y - h*s;
		glFrustum_top	=pt.y + h*s;
		if (glFrustum_right<glFrustum_left)
		{
			double x		= glFrustum_left;
			glFrustum_left	= glFrustum_right;
			glFrustum_right	= x;
		}
		if (glFrustum_top<glFrustum_bottom)
		{
			double x			= glFrustum_bottom;
			glFrustum_bottom	= glFrustum_top;
			glFrustum_top		= x;
		}
		if (glFrustum_right==glFrustum_left)
		{
			glFrustum_right	*= 1.01;
			glFrustum_left	*= 0.99;
		}
		if (glFrustum_top==glFrustum_bottom)
		{
			glFrustum_top		*= 1.01;
			glFrustum_bottom	*= 0.99;
		}

		orig_glFrustum_left=glFrustum_left;
		orig_glFrustum_right=glFrustum_right;
		orig_glFrustum_bottom=glFrustum_bottom;
		orig_glFrustum_top=glFrustum_top;
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		//wglMakeCurrent(hDC, m_hRC);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
	//	glOrtho( 
	//			glFrustum_left,
	//			glFrustum_right,
	//			glFrustum_bottom,
	//			glFrustum_top,
	//			glFrustum_znear,
	//			glFrustum_zfar
	//		);
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
		glGetIntegerv(GL_VIEWPORT , m_viewport);
		GLenum err = glGetError();
	//	ASSERT(err == GL_NO_ERROR);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		m_bReCalc = true;
		wchar_t buf[128];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
		oss	<< "  ( " << pt.x << " , " << pt.y << " )  " ;
//		oss<<float(Znear)<< "," <<float(Zfar);
		oss	<< ends;
		wstring str(buf);
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		pFrame->SetStatusBarText(str, 2);

		oss.seekp(0);
		oss	<< m_ZoomLevel << ends;
	//	oss<<float(Znear)<< "," <<float(Zfar);
		str	= buf;
		pFrame->SetStatusBarText(str, 3);
		size_x_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		size_y_ = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);
		if (m_system->mapper)
			m_system->mapper->PostThreadMessage(WM_USER_SetParam, 0, 0);
		calcPixelRows(maxi_);
		Invalidate();
		}
			return TRUE;
}
			return FALSE;
	}
	if (pMsg->message < WM_KEYFIRST) return deFormedView::PreTranslateMessage(pMsg);
	if (pMsg->message > WM_KEYLAST ) return deFormedView::PreTranslateMessage(pMsg);

	double delay = 200;
	bool bPaused			= false;
	bool bg_RateChanged		= false;
///////////////////////////////////////////////////////////////////////////////
double g_Rate = 3.59254;//3.58836;//3.86642;//3.85642;//3.7484;
const double max_Rate = 3.9;
const double min_Rate = 1;//.0000001;
///////////////////////////////////////////////////////////////////////////////
	bool bReCalculateMap	= false;

	CSystem* m_system	= theApp.GetSystem();

	if ( pMsg->message == WM_CHAR )
	{
		if (pMsg->wParam == 'r')
		{
			if (m_system->m_pCQMStationary1D)	m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
			if (m_system->m_bDoVerhulstDynamics)
			{
				POINT pt;
				pt.x = viewWidth/2;
				pt.y = viewHeight/2;
				glDrawMap_List2( pt, -1 );
			}
		//	glDrawMap_List();
			Invalidate();
		}
		else
		if (pMsg->wParam == 'c')
		{
			if (m_system->m_pCQMStationary1D)	m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		}
		else
		if (pMsg->wParam == 'R')
		{
			if (m_system->m_pCQMStationary1D)	m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		}
		else
		if (pMsg->wParam == 'm' && m_system->m_pCQMStationary1D)
		{
			wchar_t buf[64];
			/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
			oss << 1.0  << ends;
			if (_tcslen(buf) > 1) {
				size_t cb = (_tcslen(buf) + 1) * sizeof(TCHAR);
				HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);
				//std::wstring    lpszNew = (std::wstring )GlobalLock(hgCF_TEXTData);
				wchar_t* const    lpszNew = (wchar_t* const)GlobalLock(hgCF_TEXTData);
				if (lpszNew != NULL) _tcscpy_s(lpszNew, _tcslen(buf) + 1, buf);
				GlobalUnlock(hgCF_TEXTData);
				m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, reinterpret_cast<LPARAM>(lpszNew));
			}
		}
		else
		if (pMsg->wParam == 'L' && m_system->m_pCQMStationary1D)
		{
			wchar_t buf[64];
			/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
			oss << 1.0  << ends;
			if (_tcslen(buf) > 1) {
				size_t cb = (_tcslen(buf) + 1) * sizeof(TCHAR);
				HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);
				wchar_t* const    lpszNew = (wchar_t* const)GlobalLock(hgCF_TEXTData);
				if (lpszNew != NULL) _tcscpy_s(lpszNew, _tcslen(buf) + 1, buf);
				GlobalUnlock(hgCF_TEXTData);
				m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, reinterpret_cast<LPARAM>(lpszNew));
			}
		}
/*m_system->m_bDoVerhulstDynamics && 		glDrawMap_List();

		CWinThread*			selected	= (CWinThread *) m_system->player;
		if (selected==0)	selected	= (CWinThread *) m_system->m_pCQMStationary1D;
		if (selected==0)	selected	= (CWinThread *) m_system->m_pCQM_Ground_MC_1D;
		if (selected==0)	selected	= (CWinThread *) m_system->m_pCQM_Scatt1D;
		if (selected==0)	selected	= (CWinThread *) m_system->m_pCQM_Scatt2D;
		if (selected==0)	return deFormedView::PreTranslateMessage(pMsg);
		selected->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
*///
		if (m_system->player)				m_system->player->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
	//	if (m_system->m_pCQMStationary1D)	m_system->m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		if (m_system->m_pCQM_Ground_MC_1D)	m_system->m_pCQM_Ground_MC_1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		if (m_system->data__QM_Scatt1D->m_pCQM_Scatt1D)		m_system->data__QM_Scatt1D->m_pCQM_Scatt1D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		if (m_system->m_pCQM_Scatt2D)		m_system->m_pCQM_Scatt2D->PostThreadMessage(WM_USER_SetParam, pMsg->wParam, pMsg->lParam);
		return deFormedView::PreTranslateMessage(pMsg);
	}
	else

	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			return TRUE;
			return FALSE;
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			return TRUE;
			return FALSE;
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
// DO NOT send this message forward if RB thread is active!
			if (m_system->worker)
				return m_system->Stop();

		switch (pMsg->wParam)
		{
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_HOME:
			case VK_END:
			case VK_PRIOR:
			case VK_NEXT:
	//		SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
			// Windowless controls won't be able to call SendMessage.
			// Instead, just respond to the message here.
	//		return TRUE;
	//		return FALSE;//enables LEFT and RIGHT arrow keys
			break;
		}
			}
		break;
	}
	return deFormedView::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define SCALE_X(c_d) \
	{\
				glFrustum_right	-= c_d;\
				glFrustum_left	+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define SCALE_Y(c_d) \
	{\
				glFrustum_top		-= c_d;\
				glFrustum_bottom	+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define SCALE_Z(c_d) \
	{\
				glFrustum_zfar	-= c_d;\
				glFrustum_znear	+= c_d;\
				bReCalculateMap	= true;\
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE3_X(c_d) \
	{\
				m_fTransX		+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE3_Y(c_d) \
	{\
				m_fTransY			+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE_X(c_d) \
	{\
				glFrustum_right	-= c_d;\
				glFrustum_left	-= c_d;\
				m_fTransX		+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE_Y(c_d) \
	{\
				glFrustum_top		-= c_d;\
				glFrustum_bottom	-= c_d;\
				m_fTransY			+= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE_Z(c_d) \
	{\
				glFrustum_zfar	-= c_d;\
				glFrustum_znear	-= c_d;\
				m_fTransZ		+= c_d;\
				bReCalculateMap	= true;\
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE2_X(c_d) \
	{\
				glFrustum_right	-= c_d;\
				glFrustum_left	-= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE2_Y(c_d) \
	{\
				glFrustum_top		-= c_d;\
				glFrustum_bottom	-= c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE2_Z(c_d) \
	{\
				glFrustum_zfar	-= c_d;\
				glFrustum_znear	-= c_d;\
				bReCalculateMap	= true;\
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define TRANSLATE3_Z(c_d) \
	{\
				m_fTransZ		+= c_d;\
				bReCalculateMap	= true;\
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define UR_DIAG(c_d) \
	{\
				glFrustum_right	-= c_d;\
				glFrustum_top		-= gluPerspective_aspect*c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
#define LL_DIAG(c_d) \
	{\
				glFrustum_left	-= c_d;\
				glFrustum_bottom	-= gluPerspective_aspect*c_d;\
				bReCalculateMap = true;\
	}
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) //OnChar
{
	CSystem* m_system	= theApp.GetSystem();
	if (m_system->m_bDoMapper)
	{
		return;
//	Lock lock (theApp.protectBodies);
	}
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;

	const MSG& msg = *GetCurrentMessage();
	POINTL pt;
	pt.x = msg.pt.x;
	pt.y = msg.pt.y;
	bool bReCalculateMap	= false;
	bool bAngleInRadians	= false;
const double sc=1.2;


	if ( msg.message == WM_KEYDOWN )
	{
		Lock lock(mutexPlayData);
bReCalculateMap = true;
//		CSystem* m_system	= theApp.GetSystem();
//		m_system->Stop();

/*
		union {
			int iv[4];
			struct {
				GLint ix;
				GLint iy;
				GLsizei iwidth;
				GLsizei iheight;
			};
		}xxx;

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		//wglMakeCurrent(hDC, m_hRC);
		glGetIntegerv(GL_VIEWPORT, xxx.iv);
		GLenum err = glGetError();
		ASSERT(err == GL_NO_ERROR);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		gluPerspective_aspect	= double(xxx.iwidth)/double(xxx.iheight);
*/


		switch(msg.wParam)
		{
	
			case VK_DELETE:
	if (m_system->m_worldDimension==CSystem::two )
	{
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		if (rigidBodyManager->m_pSelectedRigidBody)
		{
			POINT pt;
			pt.x = msg.pt.x;
			pt.y = msg.pt.y;
			ScreenToClient(&pt);
			long ret=0;
			rigidBodyManager->HitTest(pt, ret);
			if (ret)
			{
				std::wstring bstrCharge(rigidBodyManager->m_pSelectedRigidBody->name.c_str());
				bstrCharge += L".delete;";
			//	theApp.GetEditView()->SetFocus();
			//	theApp.GetEditView()->SetCapture();
				FireWriteCommand(bstrCharge.c_str());
			//	ReleaseCapture();
				return;
			}
		}
	}
	if (m_system->m_worldDimension==CSystem::one)// && bCtrl)
	{
		if (m_pSelectedBox)
		{
//			if (::PtInRect(&m_rcTrash, point))
			{
				RemovePot(m_pSelectedBox);
				m_pSelectedBox = 0;
				::SetCursor(reinterpret_cast<HCURSOR>(GetClassLongPtr(m_hWnd, GCLP_HCURSOR)));
		DefinePotential();
		theApp.GetSystem()->data__QM_Scatt1D->OnResetViewSource();
				Fire_ViewChange();
			}
		}
	}
				break;

			case VK_UP:
				if (bCtrl && bShift)
				{
					SCALE_Z(m_fdelta)
				}
				else
				if (bCtrl)
				{
				//	SCALE_Y(-m_fdelta)
					TRANSLATE3_Y(-m_fdelta)
				}
				else
				if (bShift)
				{
					TRANSLATE_Z(m_fdelta)
				}
				else
				{
					TRANSLATE_Y(-m_fdelta)
				}
				break;
			case VK_DOWN:
				if (bCtrl && bShift)
				{
					SCALE_Z(-m_fdelta)
				}
				else
				if (bCtrl)
				{
				//	SCALE_Y(m_fdelta)
					TRANSLATE3_Y(m_fdelta)
				}
				else
				if (bShift)
				{
					TRANSLATE_Z(-m_fdelta)
				}
				else
				{
					TRANSLATE_Y(m_fdelta)
				}
				break;
			case VK_LEFT:
				if (bCtrl && bShift)
				{
					TRANSLATE3_Z(m_fdelta)
				}
				else
				if (bCtrl)
				{
	//				SCALE_X(-m_fdelta)
					TRANSLATE3_X(m_fdelta)
				}
				else
				if (bShift)
				{
					TRANSLATE2_Z(m_fdelta)
				}
				else
				{
					TRANSLATE_X(m_fdelta)
				}
				break;
			case VK_RIGHT:
				if (bCtrl && bShift)
				{
					TRANSLATE3_Z(-m_fdelta)
				}
				else
				if (bCtrl)
				{
				//	SCALE_X(m_fdelta)
					TRANSLATE3_X(-m_fdelta)
				}
				else
				if (bShift)
				{
					TRANSLATE2_Z(-m_fdelta)
				}
				else
				{
					TRANSLATE_X(-m_fdelta)
				}
				break;
///////////////////////////////////////////////////////////////////////////////
			case VK_PRIOR:
			{
				if (bCtrl && bShift)
				{
					TRANSLATE2_Y(m_fdelta)
				}
				else
				if (bCtrl)
				{
					UR_DIAG(m_fdelta)
				}
				else
				if (bShift)
				{
					UR_DIAG(-m_fdelta)
				}
				else
				{
					SCALE_X(m_fdelta)
					SCALE_Y(m_fdelta)
				}
			}
				break;
			case VK_NEXT:
			{
				if (bCtrl && bShift)
				{
					TRANSLATE2_Y(-m_fdelta)
				}
				else
				if (bCtrl)
				{
					LL_DIAG(-m_fdelta)
				}
				else
				if (bShift)
				{
					LL_DIAG(m_fdelta)
				}
				else
				{
					SCALE_X(-m_fdelta)
					SCALE_Y(-m_fdelta)
				}
			}
				break;
			case VK_HOME:
				ResetView();
		{
			Lock lock(mutexPlayData);
				m_fdelta = (glFrustum_right - glFrustum_left)/50;
		}
				bReCalculateMap = true;
				break;
			default:
				break;
		}
	}
	if (bReCalculateMap)
	{
		if (glFrustum_right<glFrustum_left)
		{
			double x		= glFrustum_left;
			glFrustum_left	= glFrustum_right;
			glFrustum_right	= x;
		}
		if (glFrustum_top<glFrustum_bottom)
		{
			double x			= glFrustum_bottom;
			glFrustum_bottom	= glFrustum_top;
			glFrustum_top		= x;
		}
		if (glFrustum_zfar<glFrustum_znear)
		{
			double x			= glFrustum_znear;
			glFrustum_znear		= glFrustum_zfar;
			glFrustum_zfar		= x;
		}

		if (glFrustum_right==glFrustum_left)
		{
			glFrustum_right	*= 1.01;
			glFrustum_left	*= 0.99;
		}
		if (glFrustum_top==glFrustum_bottom)
		{
			glFrustum_top		*= 1.01;
			glFrustum_bottom	*= 0.99;
		}
		if (glFrustum_zfar==glFrustum_znear)
		{
			glFrustum_zfar		= 4.5;
			glFrustum_znear		= 0.45;//-glFrustum_zfar;
			m_fTransZ			= -0.5;
			m_bPerspectiveTransformation = true;
		}
		if (glFrustum_znear <= 0.0) ASSERT(m_bPerspectiveTransformation == false);

		{
			Lock lock(mutexPlayData);
			m_fdelta = (glFrustum_right - glFrustum_left)/50;
		}
		CLeftView* tree = theApp.GetTreeView();
		tree->UpdateTree();
// Select the projection matrix and clear it
		SetPerspectiveOrFrustum();
//		Invalidate();
		::SetCursor(reinterpret_cast<HCURSOR>(GetClassLongPtr(m_hWnd, GCLP_HCURSOR)));
	}
	
//	if (rigidBodyManager->m_pSelectedRigidBody == 0)
//	{
//		::SetCursor((struct HICON__ *) GetClassLong(m_hWnd, GCL_HCURSOR));
//	}

	{
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		wchar_t buf[256];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf)/sizeof(buf[0]));
		oss	<< m_fTransX << " , " << m_fTransY << " , "	<< m_fTransZ  << ends;
		wstring str(buf);
		pFrame->SetStatusBarText(str, 1);
	}
	deFormedView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#undef TRANSLATE_Z
#undef TRANSLATE_X
#undef TRANSLATE_Y
///////////////////////////////////////////////////////////////////////////////
void CGLView::rotation(int x, int y)
{
	if (beginx == x && beginy == y) return;
	tBall.trackball( (2.0f*beginx - viewWidth) / viewWidth,
						 (viewHeight - 2.0f*beginy) / viewHeight,
						 (2.0f*x - viewWidth) / viewWidth,
						 (viewHeight - 2.0f*y) / viewHeight);
	
	beginx = x;
	beginy = y;
	tBall.update();
	Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	BOOL bShift	= GetAsyncKeyState(VK_SHIFT)	& 0x8000;
	BOOL bCtrl	= GetAsyncKeyState(VK_CONTROL)	& 0x8000;

	CSystem* m_system	= theApp.GetSystem();
	if ((m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics) )//&& bCtrl)
	{
		return;
	}
try {
	if (pWnd != (CWnd*) this) return;
	CPoint pt(point);
//required if called from CGLView::OnRButtonDown:
//	::ClientToScreen(pWnd->m_hWnd, &pt);

	HMENU hmenu;			// top-level menu 
	HMENU hmenuTrackPopup;  // shortcut menu 
	HMENU hmenuTrackPopup2;  // fieldLines menu 
// Load the menu resource. 
	if ((hmenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MENU_FieldDisplay))) == NULL) return; 
	if (!IsMenu(hmenu))  return;
// TrackPopupMenu cannot display the menu bar so get a handle to the first shortcut menu. 
	hmenuTrackPopup = GetSubMenu(hmenu, 0);
	if (!IsMenu(hmenuTrackPopup))  return;


	hmenuTrackPopup2 = GetSubMenu(hmenuTrackPopup, 3);

	if (!IsMenu(hmenuTrackPopup2))  return;


	HMENU h=0;
/*

   CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
   CMenu* file_menu = menu_bar->GetSubMenu(0);    
   ASSERT(file_menu);

   file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, 
	  point.y, this);

*/
///////////////////////////////////////////////////////////////////////////////
	if (m_bShowBackground)
	{
		CheckMenuItem(hmenuTrackPopup, 0, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
		CheckMenuItem(hmenuTrackPopup, 0, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bShow_equipotentials)
	{
//		CheckMenuItem(hmenuTrackPopup, 1, MF_BYPOSITION | MF_CHECKED);
		h = GetSubMenu(hmenuTrackPopup, 1);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
//		CheckMenuItem(hmenuTrackPopup, 1, MF_BYPOSITION | MF_UNCHECKED);
		h = GetSubMenu(hmenuTrackPopup, 1);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bShow_vectorFields)
	{
//		CheckMenuItem(hmenuTrackPopup, 2, MF_BYPOSITION | MF_CHECKED);
		h = GetSubMenu(hmenuTrackPopup, 2);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
//		CheckMenuItem(hmenuTrackPopup, 2, MF_BYPOSITION | MF_UNCHECKED);
		h = GetSubMenu(hmenuTrackPopup, 2);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
/*
	if (m_FieldDisplay&fieldLines)
	{
		CheckMenuItem(hmenuTrackPopup, 2, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
		CheckMenuItem(hmenuTrackPopup, 2, MF_BYPOSITION | MF_UNCHECKED);
	}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (m_bShow_fieldLines_electric)
	{
//		CheckMenuItem(hmenuTrackPopup2, 0, MF_BYPOSITION | MF_CHECKED);
		h = GetSubMenu(hmenuTrackPopup2, 0);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
//		CheckMenuItem(hmenuTrackPopup2, 0, MF_BYPOSITION | MF_UNCHECKED);
		h = GetSubMenu(hmenuTrackPopup2, 0);
		CheckMenuItem(h, 0, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bShow_fieldLines_magnetic)
	{
		CheckMenuItem(hmenuTrackPopup2, 1, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
		CheckMenuItem(hmenuTrackPopup2, 1, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bShow_fieldLines_gravitational)
	{
		CheckMenuItem(hmenuTrackPopup2, 2, MF_BYPOSITION | MF_CHECKED);
	}
	else
	{
		CheckMenuItem(hmenuTrackPopup2, 2, MF_BYPOSITION | MF_UNCHECKED);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	if (!IsMenu(hmenuTrackPopup))  return;
// Display the shortcut menu. Track the right mouse button. pWnd->
	TrackPopupMenu(	hmenuTrackPopup, 
					TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
					pt.x, pt.y, 0, m_hWnd, NULL); 
 
// Destroy the menu. 
	if (!IsMenu(hmenuTrackPopup2))  return;//
	DestroyMenu(hmenuTrackPopup2);//
	if (!IsMenu(hmenuTrackPopup))  return;//
	DestroyMenu(hmenuTrackPopup);//
	if (!IsMenu(hmenu))  return;
	DestroyMenu(hmenu);

//	CWnd::DefWindowProc( WM_CONTEXTMENU , (WPARAM) pWnd->GetSafeHwnd(), MAKELONG(point.x,point.y) );
}
///////////////////////////////////////////////////////////////////////////////
	catch (std::runtime_error* exc)
	{
		::MessageBox(0, GetWC(exc->what()), L"Error", MB_OK);
		return ;
	}
	catch(struct _exception *except)
	{
		::MessageBox(0, GetWC(except->name), L"Error", MB_OK);
		return ;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0, str, L"Error", MB_OK);
		return ;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return ;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnSelectFieldsToDisplay( UINT nID)
{
try {
	ASSERT(nID>=ID__BACKGROUND);
	ASSERT(nID<=ID_clear_gravitational);
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();

	std::wstring bstr(L"");
	switch(nID)
	{
	case ID__BACKGROUND:
	{
		m_bShowBackground = !m_bShowBackground;
//		m_FieldDisplay = (enum CGLView::FieldDisplay)(m_FieldDisplay^CGLView::backgroundTexture);
//		bool iCurrentShowBackground = (m_FieldDisplay&CGLView::backgroundTexture)?true:false;
//		ASSERT(iCurrentShowBackground == m_bShowBackground);
		Invalidate();
	}
		//	m_FieldDisplay = (enum CGLView::FieldDisplay)(m_FieldDisplay^backgroundTexture);
		//	if (m_bShowBackground)
		//		bstr = L"system.background.show = true;";
		//	else
		//	bstr = L"system.background.show = false;";
		break;
	case ID__VECTORFIELDS:
	{
//		bool bCurrent = (m_FieldDisplay&CGLView::vectorFields)?true:false;
		m_bShow_vectorFields	= !m_bShow_vectorFields;
		fieldsManager->ShowVectorFields(m_bShow_vectorFields);
	}
		break;


	case ID__EQUIPOTENTIALS:
	{
		m_bShow_equipotentials	= !m_bShow_equipotentials;
//		bool bCurrent = (m_bShow_equipotentials)?true:false;
		fieldsManager->ShowEquipotentials(m_bShow_equipotentials);
	}
//		m_FieldDisplay = (enum CGLView::FieldDisplay)(m_FieldDisplay^equipotentials);
//		if (m_bShow_equipotentials)
//			bstr = L"system.equipotentials.show = true;";
//		else
//			bstr = L"system.equipotentials.show = false;";
		break;



	
///////////////////////////////////////////////////////////////////////////////	
	case ID_electric:
	{
		m_bShow_fieldLines_electric = !m_bShow_fieldLines_electric;
		int j = 0;
		if (m_bShow_fieldLines_electric) j |= electric;
		if (m_bShow_fieldLines_magnetic) j |= magnetic;
		if (m_bShow_fieldLines_gravitational) j |= gravitational;
		m_FieldLinesType = (enum CGLView::FieldLinesType)(j);
		fieldsManager->ShowFieldLines(m_FieldLinesType);
	}
		break;
//		if (m_FieldDisplay&fieldLines)
///////////////////////////////////////////////////////////////////////////////	
	case ID_magnetic:
	{
		m_bShow_fieldLines_magnetic = !m_bShow_fieldLines_magnetic;
		int j = 0;
		if (m_bShow_fieldLines_electric) j |= electric;
		if (m_bShow_fieldLines_magnetic) j |= magnetic;
		if (m_bShow_fieldLines_gravitational) j |= gravitational;
		m_FieldLinesType = (enum CGLView::FieldLinesType)(j);
		fieldsManager->ShowFieldLines(m_FieldLinesType);
	}
		break;
///////////////////////////////////////////////////////////////////////////////	
	case ID_gravitational:
	{
		m_bShow_fieldLines_gravitational = !m_bShow_fieldLines_gravitational;
		int j = 0;
		if (m_bShow_fieldLines_electric) j |= electric;
		if (m_bShow_fieldLines_magnetic) j |= magnetic;
		if (m_bShow_fieldLines_gravitational) j |= gravitational;
		m_FieldLinesType = (enum CGLView::FieldLinesType)(j);
		fieldsManager->ShowFieldLines(m_FieldLinesType);
	}
		break;


///////////////////////////////////////////////////////////////////////////////	

	case ID__clear_VECTORFIELDS:
	{
		m_bShow_vectorFields	= false;
		fieldsManager->ClearVectorFields();
		Invalidate();
	}
		break;
///////////////////////////////////////////////////////////////////////////////	
	case ID__clear_EQUIPOTENTIALS:
	{
		m_bShow_equipotentials	= false;
		fieldsManager->ClearEquipotentials();
		Invalidate();
	}
		break;
///////////////////////////////////////////////////////////////////////////////	
	case ID_clear_electric:
	{
		m_bShow_fieldLines_electric = false;
		fieldsManager->ClearVectorFieldLines();
		Invalidate();
	}
		break;
///////////////////////////////////////////////////////////////////////////////	
	case ID_clear_magnetic:
	{
	}
		break;
///////////////////////////////////////////////////////////////////////////////	
	case ID_clear_gravitational:
	{
	}
		break;
///////////////////////////////////////////////////////////////////////////////	

	}
//	if (bstr.length()>0)
//	FireWriteCommand(bstr.c_str());
}
///////////////////////////////////////////////////////////////////////////////
	catch (std::runtime_error* exc)
	{
		::MessageBox(0, GetWC(exc->what()), L"CGLView::OnSelectFieldsToDisplay:Error", MB_OK);
		return ;
	}
	catch(struct _exception *except)
	{
		::MessageBox(0, GetWC(except->name), L"CGLView::OnSelectFieldsToDisplay:Error", MB_OK);
		return ;
	}
	catch (wchar_t * str)
	{
		::MessageBox(0, str, L"CGLView::OnSelectFieldsToDisplay:Error", MB_OK);
		return ;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0, L"unknown", L"CGLView::OnSelectFieldsToDisplay:Error", MB_OK);
		return ;
	}
}
///////////////////////////////////////////////////////////////////////////////

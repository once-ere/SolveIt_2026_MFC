// Mapping.cpp
//

#include "stdafx.h"

#pragma optimize( "", off )


#include "SolveIt.h"
#include "GLView.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "System.h"
#include "RigidBody/RigidBodyManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// gluPerspective() => left = -right;top = -bottom


// see ComputeScreenCoords before redefining m_fTransX, ...
///////////////////////////////////////////////////////////////////////////////
// CGLView;
///////////////////////////////////////////////////////////////////////////////
void CGLView::SetPerspectiveOrFrustum()
{
	try
	{
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return;
	
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	if (m_bPerspectiveTransformation)
	{
		pFrame->m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETSTYLE, BS_ICON|BS_PUSHBUTTON, 0);
		pFrame->m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(HANDLE) pFrame->m_hIconPerspective);
	}
	else
	{
		pFrame->m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETSTYLE, BS_ICON|BS_PUSHBUTTON, 0);
		pFrame->m_wndDlgBar.SendDlgItemMessage(ID_VIEW_TOGGLE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(HANDLE) pFrame->m_hIconParallel);
	}
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);
	glViewport(0, 0, viewWidth, viewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics)
	{
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );

		GetMatrices();//

/*
"An optimum compromise that allows all primitives to be specified at integer positions, 
while still ensuring predictable rasterization, is to translate x and y by 0.375, as 
shown in the following code sample. Such a translation keeps polygon and pixel image 
edges safely away from the centers of pixels, while moving line vertices close enough 
to the pixel centers. "
*/
glTranslated(0.375, 0.375, 0.0);
/* render all primitives at integer positions */ 


m_bGetMatricesSucceeded=false;

		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		return;
	}

	if (m_bPerspectiveTransformation)
	{
		ASSERT(glFrustum_znear>0);
		if (glFrustum_znear < 0.0)
		{
			m_bPerspectiveTransformation = false;
			SetPerspectiveOrFrustum();
			return;
		}
		ASSERT(glFrustum_zfar>0);
		if (m_fTransZ > -glFrustum_znear)
		{
			m_fTransZ	= -1.4*glFrustum_znear;
		}

		glFrustum( 
				glFrustum_left,
				glFrustum_right,
				glFrustum_bottom,
				glFrustum_top,
				glFrustum_znear,
				glFrustum_zfar
			);
/*
// bottom == -top for the symmetrical projection that gluPerspective() produces
	gluPerspective(	gluPerspective_fovy,
						gluPerspective_aspect,
						glFrustum_znear,
						glFrustum_zfar);
//	double(xxx.iwidth)/double(xxx.iheight),
*/
	}
	else
	{
		glOrtho( 
				glFrustum_left,
				glFrustum_right,
				glFrustum_bottom,
				glFrustum_top,
				glFrustum_znear,
				glFrustum_zfar
			);
	}
	if (m_bFlyBy)
//	gluLookAt(	-m_fTransX, -m_fTransY, -m_fTransZ,
//				0.0, 0.0, 0.0,
//				0.0, 1.0, 0.0);
	gluLookAt(	
				0.0, 0.0, 0.0,//
				m_fTransX, m_fTransY, m_fTransZ,//
				0.0, 1.0, 0.0);//


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	GLenum err = glGetError();
//	ASSERT(err == GL_NO_ERROR);
	GetMatrices();//
	err = glGetError();
//	ASSERT(err == GL_NO_ERROR);
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);


	CSize size;
	if (m_system->m_xStr.size()==0)
	{
		size.cx = size.cy = 0;
	m_min_tic_mark_spacing_x	= 0;
	m_min_tic_mark_spacing_y	= 0;
	}
	else
	{
/*
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) hOldFont = dc.SelectObject(hFont);
	//	size = dc.GetTextExtent("-0.1234");
	//	size = dc.GetTextExtent("-0.123");
		size = dc.GetTextExtent(m_system->m_tic_mark_spacingStr.c_str());
		if (hOldFont != NULL) dc.SelectObject(hOldFont);
		Point3D p1(0, 0, 0);
		Point3D p2(size.cx, size.cy, 0);
		ComputeWorldCoords(&p1.x, &p1.y, &p1.z);
		ComputeWorldCoords(&p2.x, &p2.y, &p2.z);
		m_min_tic_mark_spacing_x	= p2.x - p1.x;
		m_min_tic_mark_spacing_y	= p1.y - p2.y;
*/
///////////////////////////////////////////////////////////////////////////////
		double	DX = glFrustum_right - glFrustum_left;
		double	DY = glFrustum_top - glFrustum_bottom;
		m_min_tic_mark_spacing_x	= (DX)/m_num_tic_marks_x;
		m_min_tic_mark_spacing_y	= (DY)/m_num_tic_marks_y;

	}


//	theApp.GetRigidBodyManager()->SetScreenRects();

	Invalidate();
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"CGLView::SetPerspectiveOrFrustum Error: ",  GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"CGLView::SetPerspectiveOrFrustum Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::SetPerspectiveOrFrustum", MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::SetPerspectiveSize(int width, int height)
{
	if(height <= 0) return;
	if(width <= 0) return;
	gluPerspective_width	= width;
	gluPerspective_height	= height;
//	gluPerspective_aspect	= ((double)gluPerspective_width)/gluPerspective_height;
	glViewport(0, 0, viewWidth, viewHeight);

	SetPerspectiveOrFrustum();


	CMainFrame*		pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CLeftView*		tree	= pFrame->GetTreeView();
/*
	tree->SetglFrustum_left();
	tree->SetglFrustum_right();
	tree->SetglFrustum_top();
	tree->SetglFrustum_bottom();
	tree->SetglFrustum_zfar();
	tree->SetglFrustum_znear();
*/
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::ToggleViewTransformation()
{
	if (glFrustum_znear <= 0.0)
	{
	//	m_bPerspectiveTransformation = false;
	//	return;
	}
	m_bPerspectiveTransformation = !m_bPerspectiveTransformation;
	if (m_bPerspectiveTransformation)
	{
		if (glFrustum_znear <= 0.0)
		{
///////////////////////////////////////////////////////////////////////////////
			glFrustum_zfar	= fabs(glFrustum_zfar);
			glFrustum_znear	= glFrustum_zfar/10;
			m_fTransZ		= -1.05*glFrustum_znear;
		}
		if (glFrustum_znear >= glFrustum_zfar)
		{
			glFrustum_zfar	= 4.5;
			glFrustum_znear	= glFrustum_zfar/10;
			m_fTransZ		= -1.05*glFrustum_znear;
///////////////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		if (glFrustum_znear >= 0)
		{
			glFrustum_znear	= -10*glFrustum_zfar;
			m_fTransZ		= 0;
///////////////////////////////////////////////////////////////////////////////
		}
	}
	SetPerspectiveOrFrustum();
	return;
	if (m_bPerspectiveTransformation)
	{
		if (glFrustum_znear <= 0.0) glFrustum_znear	= glFrustum_zfar/10;
		if (glFrustum_znear >= glFrustum_zfar)
		{
///////////////////////////////////////////////////////////////////////////////
			glFrustum_zfar	= orig_glFrustum_zfar;
			glFrustum_znear	= glFrustum_zfar/10;
		}
		if (glFrustum_znear >= glFrustum_zfar)
		{
			glFrustum_zfar	= 4.5;
			glFrustum_znear	= glFrustum_zfar/10;
///////////////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
			glFrustum_znear	= orig_glFrustum_znear;
			glFrustum_zfar	= orig_glFrustum_zfar;
		if (glFrustum_znear >= glFrustum_zfar)
		{
			glFrustum_zfar	= 4.5;
			glFrustum_znear	= -glFrustum_zfar;
///////////////////////////////////////////////////////////////////////////////
		}
	}
	SetPerspectiveOrFrustum();
}
///////////////////////////////////////////////////////////////////////////////
int CGLView::ResetView()
{
//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theScene));
//	pThis->theFieldsManager.Clear();
	beginx				= -1;
	beginy				= -1;

	m_pointCurrent.x	= 0;
	m_pointCurrent.y	= 0;

	tBall.Reset();

	Reset();

	{
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		wchar_t buf[256];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf)/sizeof(buf[0]));
		oss	<< m_fTransX << " , " << m_fTransY << " , "	<< m_fTransZ  << ends;
		wstring str(buf);
		pFrame->SetStatusBarText(str, 1);
	}
//	SetPerspective();
//	SendMessage(hWndCtl, WM_ACTION_Reset, 0, 0);
//	m_FieldDisplay		= nothing;
	Invalidate();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////

void SolveIt::InvertMatrix(GLfloat m[][4])
{
	int i, j;
 
	GLfloat c[3][3];

	c[0][0] = (m[1][1]*m[2][2] - m[2][1]*m[1][2]);
	c[0][1] = -(m[1][0]*m[2][2] - m[2][0]*m[1][2]);
	c[0][2] = (m[1][0]*m[2][1] - m[2][0]*m[1][1]);

	c[1][0] = -(m[0][1]*m[2][2] - m[0][2]*m[2][1]);
	c[1][1] = (m[0][0]*m[2][2] - m[0][2]*m[2][0]);
	c[1][2] = -(m[0][0]*m[2][1] - m[0][1]*m[2][0]);

	c[2][0] = (m[0][1]*m[1][2] - m[0][2]*m[1][1]);
	c[2][1] = -(m[0][0]*m[1][2] - m[0][2]*m[1][0]);
	c[2][2] = (m[0][0]*m[1][1] - m[0][1]*m[1][0]);

	GLfloat det_m = m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1]) - 
					m[0][1]*(m[1][0]*m[2][2] - m[1][2]*m[2][0]) + 
					m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);

	det_m = 1.0f/det_m;

	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			// m equals the transpose of cofactor matrix
			m[i][j] = det_m*c[j][i];
}

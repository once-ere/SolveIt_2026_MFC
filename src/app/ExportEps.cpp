// CGLView.cpp : implementation file
//

#include "stdafx.h"

#include "SolveIt.h"
#include "GLView.h"
#include "MainFrm.h"

#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
//#include "StackDatum.h"
//#include "Worker.h"
#include "RigidBody/RigidBodyManager.h"
#include "DibSectn.h"
#include "RigidBody/FieldsManager.h"
#include "RigidBody/ConstraintManager.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const float z_so_GetMatrices_succeeds = -5.40f;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//********************************* 
// OnExportEps 
//********************************* 
void CGLView::OnExportEps() 
{ 
const wchar_t BASED_CODE filter[] = L"EPS Files (*.eps)|*.eps"; 
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return;
	CFileDialog SaveDlg(FALSE, L"*.eps", L"mesh.eps",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter); 
	if(SaveDlg.DoModal() != IDOK) return;


//	CDC *pDC = GetDC(); 
//	:://wglMakeCurrent(pDC->m_hDC, m_hRC); 


	try {
		wstring fn(SaveDlg.GetPathName()); 
		const wchar_t *pFilename = fn.c_str();
		//std::ofstream ostrm(pFilename);
		
		FILE *fp = NULL;
		_wfopen_s(&fp, pFilename, L"w");

		if(fp == NULL) return;
//	static GLfloat m[4][4];
	tBall.build_rotmatrix(m);


		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		CSystem* m_system = theApp.GetSystem();

		size_t buffsize = 0;
		int state = GL2PS_OVERFLOW;

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		while( state == GL2PS_OVERFLOW )
		{ 

			//wglMakeCurrent(hDC, m_hRC);
			buffsize += 6*1024*1024;
			gl2psBeginPage ( const_cast<char*>("MyTitle"), const_cast<char*>("MySoftware"), 
							//		 GL2PS_EPS, GL2PS_BSP_SORT,complex three-dimensional scenes,
									 GL2PS_EPS, GL2PS_SIMPLE_SORT,
									 GL2PS_SIMPLE_LINE_OFFSET | GL2PS_SILENT, 
									 GL_RGBA, 0, NULL, buffsize, fp, NULL );
			glViewport(0, 0, viewWidth, viewHeight);
			glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

			glPushMatrix();
	glTranslated(m_fTransX, m_fTransY, m_fTransZ);
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics)
	{
		glViewport(0, 0, viewWidth, viewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_DITHER);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//
//	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//Background color
//	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

/*
"An optimum compromise that allows all primitives to be specified at integer positions, 
while still ensuring predictable rasterization, is to translate x and y by 0.375, as 
shown in the following code sample. Such a translation keeps polygon and pixel image 
edges safely away from the centers of pixels, while moving line vertices close enough 
to the pixel centers. "
*/
		glTranslated(0.375, 0.375, 0.0);
/* render all primitives at integer positions */ 
	}
	else
	{
		glRotated(m_fRotX, 1.0f, 0.0f, 0.0f);//
		glRotated(m_fRotY, 0.0f, 1.0f, 0.0f);//
		glRotated(m_fRotZ, 0.0f, 0.0f, 1.0f);//

		glMultMatrixd(&m[0][0]);
	}
////	glLoadMatrixf((const float *)m);
	if (m_bShowBackground && listBackground)
	{
		glCallList(listBackground);
	}
///////////////////////////////////////////////////////////////////////////////
	else
	if (m_system->m_bDoMapper)
	{
//		bool useGLArrays = useGLArrays_;
//		useGLArrays_	= false;
//		Lock lock (theApp.protectBodies);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DITHER);
		glDisable(GL_DEPTH_TEST);
		glShadeModel(GL_FLAT);
		glPushMatrix();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_NORMALIZE);
#define RENDER_MODE_FPU_ASM	 1
		renderImage(maxi_, RENDER_MODE_FPU_ASM);
//		useGLArrays_	= useGLArrays;
	}
///////////////////////////////////////////////////////////////////////////////
	else
	{
		Draw3DAxes(GetWC(m_system->m_xStr), GetWC(m_system->m_yStr));
	}
///////////////////////////////////////////////////////////////////////////////
//	else
	if (m_system->m_bDoVerhulstDynamics)
	{
		glDrawMap2(glFrustum_top, glFrustum_bottom);
		glPushMatrix();
#if 10
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);

		for (int y=0; y<viewHeight; y++) {
			renderPixelRow(y, 1);//
		}
#endif

#if 0
		unsigned int* rowBuffer = 0;
		int	ix	= 0;
		glColor3i( 0, 0, 0 );
		glBegin(GL_POINTS);
	for (int	row=0; row<viewHeight; row++)
	{
		rowBuffer = pixBuf_->getRowPointer(row);
		for (int q=0; q<viewWidth; q++)
		{
			ix	= rowBuffer[q];
			if (ix>=0 && ix < viewWidth)
			{
				glVertex2i(ix, row);
			}
		}
	}
	glEnd();
#endif
	glPopMatrix();
//	Draw3DAxes("","");
	}
///////////////////////////////////////////////////////////////////////////////
//	InvertMatrix(m);
//	theApp.theSystem.DoDraw(m);
	else
	if (m_system->m_bDoMapper)
	{
//	Lock lock (theApp.protectBodies);
	}
///////////////////////////////////////////////////////////////////////////////
	else
	{
	CFieldsManager* fieldsManager			= theApp.GetFieldsManager();
	CRigidBodyManager*	rigidBodyManager	= theApp.GetRigidBodyManager();
	CConstraintManager* constraintManager	= theApp.GetConstraintManager();
	constraintManager->OnDraw();
	fieldsManager->OnDraw();
	rigidBodyManager->OnDraw();
	m_system->DoIt();
	}
			glPopMatrix();

			glFlush();//
			glFinish();

// Double buffer 
//			SwapBuffers(pDC->m_hDC); 
			state = gl2psEndPage();
			//wglMakeCurrent(0, 0);
		}

	//:://ReleaseDC(m_hWnd, hDC);
//
	fclose(fp);

	}
	catch (wchar_t * str)
	{
		::MessageBox(0, str, L"Error", MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0, L"Unknown error.", L"Error", MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
LRESULT CGLView::OnGet_glFrustum_left(WPARAM wParam, LPARAM lParam)
{
	return &glFrustum_left;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnGet_glFrustum_right(WPARAM wParam, LPARAM lParam)
{
	return &glFrustum_right;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnGet_glFrustum_bottom(WPARAM wParam, LPARAM lParam)
{
	return &glFrustum_bottom;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CGLView::OnGet_glFrustum_top(WPARAM wParam, LPARAM lParam)
{
	return &glFrustum_top;
}
///////////////////////////////////////////////////////////////////////////////
#endif

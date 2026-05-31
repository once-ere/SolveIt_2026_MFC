// glDrawMap.cpp : Implementation of CGLView
#include "stdafx.h"
#include "SolveIt.h"
#include "GLView.h"
#include "Types.h"
#include "System.h"

//#include "RigidBody/shapes.h"
//#include "DibSectn.h"
//#include "resource.h"
//#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Mutex protectList;
///////////////////////////////////////////////////////////////////////////////
wchar_t* CGLView::PixelsSnapshot()
{
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
//	if(FALSE == ::wglMakeCurrent(hDC , m_hRC)){
//		ASSERT(0);
//		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
//	}
	glGetIntegerv(GL_VIEWPORT, xxx.iv);
	ASSERT(viewWidth == xxx.iwidth);
	ASSERT(viewHeight == xxx.iheight);

	wchar_t *snapshBuf = new wchar_t[xxx.iwidth*xxx.iheight*3];
//	if(FALSE == SwapBuffers( hDC )){
//		throw SolveIt_ERROR(L"SwapBuffers failed");
//	}

	glReadPixels( 0, 0, xxx.iwidth, xxx.iheight, GL_RGB, GL_UNSIGNED_BYTE, snapshBuf );

//	if(FALSE == SwapBuffers( hDC )){
//		throw SolveIt_ERROR(L"SwapBuffers failed");
//	}
	GLenum e=glGetError();
	if ( GL_NO_ERROR !=  e )
	{
		ASSERT(0);
	}

	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
	return snapshBuf;
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::PixelsToTexture(wchar_t *snapshotBuffer, GLuint& texture, bool bFreeBuffer)
{
	ASSERT(snapshotBuffer != NULL);
	if (snapshotBuffer == NULL) return;
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
//	if(FALSE == ::wglMakeCurrent(hDC , m_hRC)){
//		ASSERT(0);
//		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
//	}
	glGetIntegerv(GL_VIEWPORT, xxx.iv);
	ASSERT(viewWidth == xxx.iwidth);
	ASSERT(viewHeight == xxx.iheight);

	GLenum e=glGetError();
	if ( GL_NO_ERROR !=  e )
	{
		ASSERT(0);
	}
	glEnable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	if (texture) glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glGenTextures(1, &texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glBindTexture(GL_TEXTURE_2D, texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//GL_REPEAT
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//GL_MODULATE
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

//This is the default setting on most systems. 
//
//In order for OpenGL to understand BGR use GL_BGR_EXT instead of 
//GL_RGB in your call to glTexImage2D or gluBuild2DMipmaps: 
	gluBuild2DMipmaps(	GL_TEXTURE_2D, 3, xxx.iwidth, xxx.iheight, 
						GL_RGB, GL_UNSIGNED_BYTE,//GL_BGR_EXT
						snapshotBuffer );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	if (bFreeBuffer) delete[] snapshotBuffer;
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::TextureToDisplayList(GLuint& listVerhulst, GLuint& texture, bool bFreeList)
{
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
//	if(FALSE == ::wglMakeCurrent(hDC , m_hRC)){
//			ASSERT(0);
//			throw SolveIt_ERROR(L"//wglMakeCurrent failed");
//		}
	glGetIntegerv(GL_VIEWPORT, xxx.iv);
	ASSERT(viewWidth == xxx.iwidth);
	ASSERT(viewHeight == xxx.iheight);

	GLenum e=glGetError();
	if ( GL_NO_ERROR !=  e )
	{
		ASSERT(0);
	}
		glViewport(0, 0, viewWidth, viewHeight);
		glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

		glPushMatrix();
//	glEnable(GL_TEXTURE_2D);
	{
//	const double &x1 = glFrustum_left;
//	const double &x2 = glFrustum_right;
//	const double &y1 = glFrustum_bottom;
//	const double &y2 = glFrustum_top;
	const double x1 = 0;
	const double x2 = viewWidth;
	const double y1 = 0;
	const double y2 = viewHeight;

	if (bFreeList)
	{
		if(listVerhulst != 0) glDeleteLists(listVerhulst, 1);
		listVerhulst = 0;
	}
	if(listVerhulst == 0) listVerhulst = glGenLists(1);
	glNewList(listVerhulst, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	glBindTexture(GL_TEXTURE_2D, texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	const float f=1.0f;
	glColor3f(f,f,f);
	glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);

		glNormal3d(0, 0, -1);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
	glEnd();
	}
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glPopMatrix();
	glEndList();
//	if (texture) glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
}
///////////////////////////////////////////////////////////////////////////////
#if 0
void CGLView::glDrawMap_List()
{
				POINT pt;
				pt.x = viewWidth/2;
				pt.y = viewHeight/2;
				glDrawMap_List2( pt, -1 );
	return;
/*
	RECT rc;
	GetClientRect(&rc);
	Point3D p1(0, 0, 0);
	Point3D p2(0, rc.bottom, 0);
	ComputeWorldCoords(&p1.x, &p1.y, &p1.z);
	ComputeWorldCoords(&p2.x, &p2.y, &p2.z);
	glDrawMap_List(p2.y, p1.y, listBackground, texture);
*/
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::glDrawMap_List(const double& y_max, const double& y_min, GLuint& listVerhulst, GLuint& texture)
{

//	glClearMap_List( listVerhulst, texture);
	glComputeMap(  y_max,  y_min);
	glDrawMap2(  y_max,  y_min);
	PixelsToTexture(PixelsSnapshot(), texture);
	TextureToDisplayList(listVerhulst, texture);
	return;
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
	SetPerspectiveSize(xxx.iwidth, xxx.iheight);

	GLenum e=GL_NO_ERROR;
	if(FALSE == :://wglMakeCurrent(hDC , m_hRC))
	{
		ASSERT(0);
		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
	}
	glViewport(0, 0, viewWidth, viewHeight);
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	{
		Lock lock(protectList);
		glTranslated(0, 0, m_fTransZ);
	}

	CSystem* m_system = theApp.GetSystem();
	Draw3DAxes("","");//m_system->m_xStr.c_str(), m_system->m_yStr.c_str());

		glColor3d( 0, 0, 0 );
		double y = 0, x=0;
		int q;
		if (y2<3.5)
		{
			for (q=0;q<200;q++)
			{
				y = y2+(3.5-y2)*imsl_rand();
				x = 0.2;
				for (int h=0;h<300;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
			for (q=0;q<2000;q++)
			{
				y = 3.5+(y1 - 3.5)*imsl_rand();
				x = 0.2;
				for (int h=0;h<500;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
		}
		else
		{
			for (q=0;q<2000;q++)
			{
				y = y2+(y1 - y2)*imsl_rand();
				x = 0.2;
				for (int h=0;h<300;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
		}
		glPopMatrix();
		glFlush();
		glFinish();

//	float *snapshBuf = new float[iwidth*iheight*3];
	wchar_t *snapshBuf = new wchar_t[xxx.iwidth*xxx.iheight*3];
		glReadPixels( 0, 0, xxx.iwidth, xxx.iheight, GL_RGB, GL_UNSIGNED_BYTE, snapshBuf );

		if(FALSE == SwapBuffers( hDC )){
			throw SolveIt_ERROR(L"SwapBuffers failed");
		}
	
	GetMatrices();

	glEnable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	if (texture) glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glGenTextures(1, &texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glBindTexture(GL_TEXTURE_2D, texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//GL_REPEAT
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//GL_MODULATE
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

//This is the default setting on most systems. 
//
//In order for OpenGL to understand BGR use GL_BGR_EXT instead of 
//GL_RGB in your call to glTexImage2D or gluBuild2DMipmaps: 
	gluBuild2DMipmaps(	GL_TEXTURE_2D, 3, xxx.iwidth, xxx.iheight, 
						GL_RGB, GL_UNSIGNED_BYTE,//GL_BGR_EXT
						snapshBuf );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

//	glEnable(GL_TEXTURE_2D);
	{
	const double &x1 = glFrustum_left;
	const double &x2 = glFrustum_right;
	const double &y1 = glFrustum_bottom;
	const double &y2 = glFrustum_top;
	if(listVerhulst != 0) glDeleteLists(listVerhulst, 1);
	listVerhulst = 0;
	if(listVerhulst == 0) listVerhulst = glGenLists(1);
	glNewList(listVerhulst, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	glBindTexture(GL_TEXTURE_2D, texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	const float f=1.0f;
	glColor3f(f,f,f);
	glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);

		glNormal3d(0, 0, -1);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
	glEnd();
	}
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glPopMatrix();
	glEndList();
//	if (texture) glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	delete[] snapshBuf;
	//:://ReleaseDC(m_hWnd, hDC);
*/
}
#endif
///////////////////////////////////////////////////////////////////////////////
void CGLView::glClearMap_List(GLuint& listVerhulst, GLuint& texture)
{
	GLenum e=GL_NO_ERROR;
//		HDC hDC = GetDC()->GetSafeHdc();
		HDC hDC = m_pDC->GetSafeHdc();
//	if(FALSE == ::wglMakeCurrent(hDC , m_hRC)){
//		ASSERT(0);
//		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
//	}
	glViewport(0, 0, viewWidth, viewHeight);
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);
	if (texture)	glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	if (listVerhulst)		glDeleteLists(listVerhulst, 1);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glFlush();
	glFinish();
	if(FALSE == SwapBuffers( hDC )){
		throw SolveIt_ERROR(L"SwapBuffers failed");
	}
	glViewport(0, 0, viewWidth, viewHeight);
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);
	if (texture)	glDeleteTextures(1,&texture);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	if (listVerhulst)		glDeleteLists(listVerhulst, 1);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glFlush();
	glFinish();
	if(FALSE == SwapBuffers( hDC )){
		throw SolveIt_ERROR(L"SwapBuffers failed");
	}
	listVerhulst=0;
	texture=0;
	//wglMakeCurrent(0, 0);
//	::ReleaseDC(m_hWnd, hDC);
}
///////////////////////////////////////////////////////////////////////////////
#if 0
void CGLView::glDrawMap(const double& y_max, const double& y_min)
{
		ASSERT(0);
		glDrawMap2(  y_max,  y_min);
return;
	SetCursor(LoadCursor(NULL, IDC_WAIT));

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();

	GLenum e=GL_NO_ERROR;
	if(FALSE == :://wglMakeCurrent(hDC , m_hRC)){
		ASSERT(0);
		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
	}
		glViewport(0, 0, viewWidth, viewHeight);
		glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

		glPushMatrix();
		{
			Lock lock(protectList);
			glTranslated(0, 0, m_fTransZ);
		}

	CSystem* m_system = theApp.GetSystem();
	Draw3DAxes("","");//m_system->m_xStr.c_str(), m_system->m_yStr.c_str());

		glColor3d( 0, 0, 0 );
		double y = 0, x=0, height = y_max - y_min;
		int q;
		if (y_min<3.5)
		{
			for (q=0;q<200;q++)
			{
				y = y_min+(3.5-y_min)*imsl_rand();
				x = 0.2;
				for (int h=0;h<300;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
			for (q=0;q<2000;q++)
			{
				y = 3.5+(y_max - 3.5)*imsl_rand();
				x = 0.2;
				for (int h=0;h<500;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
		}
		else
		{
			for (q=0;q<2000;q++)
			{
				y = y_min + height*imsl_rand();
				x = 0.2;
				for (int h=0;h<300;h++)
				{
					x = y * x * (1 - x);
				}
				glBegin(GL_POINTS);
				for (h=0;h<500;h++)
				{
					x = y * x * (1 - x);
					glVertex2d(x, y);
				}
				glEnd();
			}
		}
		glPopMatrix();
		glFlush();
		glFinish();

	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
#endif
///////////////////////////////////////////////////////////////////////////////

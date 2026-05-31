// DrawSplineSurface.cpp : Implementation of CQMStationary2D


#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "QMStationary2D.h"
#include "GLView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CQMStationary2D
///////////////////////////////////////////////////////////////////////////////
extern SMutex g_SMutex_2;
//extern GLuint texture;
//extern GLuint listBackground;
//extern bool bTextureDefined;
//extern bool bNewTextureDefined;
///////////////////////////////////////////////////////////////////////////////
//#define	imageWidth 64
//#define	imageHeight 64
//extern GLubyte image[3*imageWidth*imageHeight];
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::DrawSplineSurface()
{
	const long N			= QMEvolve2D.NDATA;
	const long S_NUMPOINTS	= _DATA_2D_mp_NDATA;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= _DATA_2D_mp_NDATA;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;



try
{
	if (m_nDefaultQHullOrSurfaceSpline!=2)
	{
		return;
	}

	CGLView *pGLView = theApp.GetGLView();
	CGLView* pGLView = theApp.GetGLView();

	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	const double	&y1	= pGLView->glFrustum_bottom;
	const double	&y2	= pGLView->glFrustum_top;

//	static float	knots[]	= { 0, 0, 0, 0, 1, 1, 1, 1 };
//	static float	ctlpoints[3*_DATA_2D_mp_NDATA*_DATA_2D_mp_NDATA];
	static GLfloat	ctlpoints[_DATA_2D_mp_NDATA][_DATA_2D_mp_NDATA][3];
//		static GLfloat	ctlpoints2[_DATA_2D_mp_NDATA][_DATA_2D_mp_NDATA][3];
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];

	float edgePt[5][2] = /* counter clockwise */
      {{float(x1), float(y1)}, {float(x2), float(y1)}, {float(x2), float(y2)}, {float(x1), float(y2)}, {float(x1), float(y1)}};

	long x, y;
/*
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_NUMKNOTS/2)? 0.0f:1.0f;
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_NUMKNOTS/2)? 0.0f:1.0f;
	}
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? -1.0f:((x<S_ORDER+S_NUMPOINTS/2)? 0.0f:1.0f);
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? -1.0f:((x<T_ORDER+T_NUMPOINTS/2)? 0.0f:1.0f);
	}
*/
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	}


	glPushMatrix();

//#define __use_GL_TEXTURE_2D

#if defined(__use_GL_TEXTURE_2D)
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
#else
#endif
	const float f=0.50f;
	glColor3f(f,f,f);

#define SCALE(x)		2.0*((((double)x)/N)-0.5)
	{
		MATRIX_Complex psi = 0;
		switch(QMEvolve2D.m_DrawSource)
		{
			case QMEvolve2D.Initial: psi = (MATRIX_Complex) QMEvolve2D.psi_xt0; break;
			case QMEvolve2D.Current: psi = (MATRIX_Complex) QMEvolve2D.psi_xt ; break;
			case QMEvolve2D.External:psi = (MATRIX_Complex) m_pExternal; break;
		}
		if (!psi) return;
			
/*
		double X1=-1,Y1=-1,Z1=0;
		double X2=-1,Y2=-1,Z2=0;
		double X3=-1,Y3=-1,Z3=0;
		double X4=-1,Y4=-1,Z4=0;
		glPointSize(1.0f);
*/
		double X=0, Y=0;


	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);

	{
		for (x=0;x<N;x++)
		{
			X = x1 + x*dx;
			for (y=0;y<N;y++)
			{
				Y = y1 + y*dy;
				cdouble& C1 = psi[x][y];

if (fabs(C1.real())>1.e100) {glEnd();return;}
if (fabs(C1.imag())>1.e100) {glEnd();return;}

				double n = C1.real()*C1.real()+C1.imag()*C1.imag();

				ctlpoints[x][y][0] = float(X);
				ctlpoints[x][y][1] = float(Y);
				ctlpoints[x][y][2] = float(m_fScalePsi*sqrt(n));

		//		ctlpoints2[x][y][0] = float(X);
		//		ctlpoints2[x][y][1] = float(Y);
		//		ctlpoints2[x][y][2] = float(m_fScalePotential*QMEvolve2D.v[x][y]);//
/*
				ctlpoints[3*(_DATA_2D_mp_NDATA*x + y) + 0] = float(X);
				ctlpoints[3*(_DATA_2D_mp_NDATA*x + y) + 1] = float(Y);
				ctlpoints[3*(_DATA_2D_mp_NDATA*x + y) + 2] = float(sqrt(n));
				if (x==N-1) continue;
				if (y==N-1) continue;
*/
			}
		}
		}
	}
#undef SCALE

	glPushAttrib(GL_ALL_ATTRIB_BITS);
   GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 100.0f };

   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);
///////////////////////////////////////////////////////////////////////////////
   {
//	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);
//	gluNurbsProperty(theNurbs, GLU_CULLING , GL_TRUE);

	gluBeginSurface(theNurbs);

	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);



//	gluBeginTrim (theNurbs);
//	gluPwlCurve (theNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (theNurbs);

	gluEndSurface(theNurbs);
//	gluDeleteNurbsRenderer(theNurbs);
   }
///////////////////////////////////////////////////////////////////////////////
	glPopAttrib();

#if defined(__use_GL_TEXTURE_2D)
	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
#else
#endif
	glPopMatrix();
	glFlush();
///////////////////////////////////////////////////////////////////////////////

}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}
}

///////////////////////////////////////////////////////////////////////////////

// TransformCoords.cpp : Helper implementation of COpenGLObj

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
#include "QM/DataQM_Stationary1D.h"
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
void CGLView::GetMatrices()
{
	m_bGetMatricesSucceeded=false;
	if ( !m_hWnd || !::IsWindow(m_hWnd) )
	{
		m_bGetMatricesSucceeded = false;
		return;
	}

	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics)
	{
//		return;
	}


//double mModel[16],mPrj[16];
//int	 viewport[4];
  
//qdbg("Unprj: glCtx: %p\n",glXGetCurrentContext());
  // Retrieve matrices and viewport settings
//glGetDoublev(GL_MODELVIEW_MATRIX,mModel);
//glGetDoublev(GL_PROJECTION_MATRIX,mPrj);
//glGetIntegerv(GL_VIEWPORT,viewport);

	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);



//	const GLenum err1= GL_INVALID_OPERATION;//?called between a call to glBegin and the corresponding call to glEnd.
//	const GLenum err2= GL_INVALID_ENUM;
//	glutSetWindow(mainWindowID);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
	err = glGetError();
	if ( err != GL_NO_ERROR )
	{
//GL_INVALID_OPERATION is generated if glPushMatrix or glPopMatrix is executed
//between the execution of glBegin and the corresponding execution of glEnd
		ASSERT(err == GL_INVALID_OPERATION);
		m_bGetMatricesSucceeded = false;
		return;
	}
	ASSERT(err == GL_NO_ERROR);
	ASSERT(err != GL_INVALID_OPERATION);
	ASSERT(err != GL_INVALID_ENUM);
	glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
	err = glGetError();
	if ( err != GL_NO_ERROR )
	{
		m_bGetMatricesSucceeded = false;
		return;
	}
	ASSERT(err == GL_NO_ERROR);

#if 0
// calculate near/far based upon projection type 
	double p11 = m_projMatrix[4*0+0];
	double p22 = m_projMatrix[4*1+1];
	double p33 = m_projMatrix[4*2+2];
	double p44 = m_projMatrix[4*3+3];
	double p13 = m_projMatrix[4*0+2];
	double p23 = m_projMatrix[4*1+2];
	double p14 = m_projMatrix[4*0+3];
	double p24 = m_projMatrix[4*1+3];
	double p34 = m_projMatrix[4*2+3];
	if ( p44 != 0.0 )
	{
// orthographic 
		Znear			= (p34+1)/p33;
		Zfar			= (p34-1)/p33;
		double left		= (-p14+1)/p33;
		double right	= (-p14-1)/p33;
		double bottom	= (-p24+1)/p33;
		double top		= (-p24-1)/p33;
		double h		= top - bottom;
//		gluPerspective_aspect	= (right - left)  / h;
//		gluPerspective_fovy		= -RAD_TO_DEG*2*atan((h/2)/Znear);
/*glFrustum_glFrustum_glFrustum_glFrustum_
		double z		= (Zfar - Znear)/2;
		double left = (-p14+1)/p33/z;
		double right = (-p14-1)/p33/z;
		double bottom = (-p24+1)/p33/z;
		double top = (-p24-1)/p33/z;
*/
	}
	else
	{
// perspective => left = -right;top = -bottom
/*
p13 = (right + left) / (right - left) 
p23 = (top + bottom) / (top - bottom) 
p33 = (far + near) / (far - near) 
p34 = (2 far near) / (far - near) 
p11 = (2 near) / (right - left) 
p22 = (2 near) / (top - bottom) 
Solve[{p33 == (far + near) / (far - near) , p34 == (2 far near) / (far - near) },{near,far}]
Solve[{p11 == (2 near) / (right - left) , p13 == (right + left) / (right - left) },{right , left}]
Solve[{p22 == (2 near) / (top - bottom) , p23 == (top + bottom) / (top - bottom) },{top , bottom}]
*/
		double aspect	= p11/p22;
		double fovy		= RAD_TO_DEG*2*atan(1.0/p22);
		Znear			= p34 / (p33 - 1);
		Zfar			= p34 / (p33 + 1);
		double t		= tan(fovy/2);
	//	glFrustum_top		=  t*Znear-m_fTransY;
	//	glFrustum_bottom	= -t*Znear-m_fTransY;
/*
		double z		=  (Zfar - Znear)/2;
		glFrustum_right		= Znear*(p13 + 1)/p11-m_fTransX;
		glFrustum_left		= Znear*(p13 - 1)/p11-m_fTransX;
		glFrustum_top		= Znear*(p23 + 1)/p22-m_fTransY;
		glFrustum_bottom	= Znear*(p23 - 1)/p22-m_fTransY;
*/
		double left		= Znear*(p13 - 1)/p11-m_fTransX;
		double right	= Znear*(p13 + 1)/p11-m_fTransX;
		double bottom	= Znear*(p23 - 1)/p22-m_fTransY;
		double top		= Znear*(p23 + 1)/p22-m_fTransY;
//		glFrustum_top		=  Zfar/p22-m_fTransY;
//		glFrustum_bottom	= -Zfar/p22-m_fTransY;
//		double h=  (glFrustum_top - glFrustum_bottom)/2;
//		glFrustum_right		=  aspect*h - m_fTransX;
//		glFrustum_left		= -aspect*h - m_fTransX;
	}
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();

#endif

	glGetIntegerv(GL_VIEWPORT , m_viewport);
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	if ( err != GL_NO_ERROR )
	{
		m_bGetMatricesSucceeded = false;
		return;
	}

//	theApp.GetRigidBodyManager()->SetScreenRects();

	m_bGetMatricesSucceeded = true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CGLView::ComputeScreenCoords(double *x, double *y, double *z)
{
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return 0;
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics||!m_bGetMatricesSucceeded)
	{
		double xPos = *x; 
		double yPos = *y;
		double zPos = *z;

		xPos +=m_fTransX;
		yPos +=m_fTransY;
		zPos +=m_fTransZ;

		*x=(xPos-glFrustum_left) * ((double) viewWidth) /(glFrustum_right - glFrustum_left) ;
		*y=(yPos-glFrustum_bottom) *  ((double) viewHeight) /(glFrustum_top - glFrustum_bottom);
		*z=0;
//	*y = viewHeight - *y;
		return 0;
	}

try
{
	RECT rect;
	GetClientRect(&rect);
//	*y = rect.bottom - *y;
	double xPos = *x;// / sc.x;
	if (_isnan( xPos ))  {return (throw SolveIt_ERROR("_isnan( xPos )"),-1);}
	double yPos = *y;// / sc.y; 
	if (_isnan( yPos ))  {return (throw SolveIt_ERROR("_isnan( yPos )"),-1);}
	double zPos = *z;// / sc.z; 
	if (_isnan( zPos ))  {return (throw SolveIt_ERROR("_isnan( zPos )"),-1);}


	xPos +=m_fTransX;
	yPos +=m_fTransY;
	zPos +=m_fTransZ;

/*
	*x	=	m[0][0]*xPos + m[1][0]*yPos + m[2][0]*zPos;
	*y	=	m[0][1]*xPos + m[1][1]*yPos + m[2][1]*zPos;
	*z	=	m[0][2]*xPos + m[1][2]*yPos + m[2][2]*zPos;

	xPos = *x; 
	yPos = *y;
	zPos = *z;
*/

//	double zPos = (*z - m_fNearPlane)/(m_fFarPlane - m_fNearPlane);
	long ret = gluProject(	xPos, yPos, zPos,
							m_modelMatrix,
							m_projMatrix,
							m_viewport,
							x, y, z);
	ASSERT(ret == GL_TRUE);
//	if(ret != GL_TRUE) {return (throw -1,-1);}
	if(ret != GL_TRUE) {return (throw SolveIt_ERROR("CGLView::ComputeScreenCoords"),-1);}

	*y = rect.bottom - *y;
	return ret == GL_TRUE? S_OK:-1;
}
catch( SolveIt_Error& e){e.why();return -2;}catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
long CGLView::ComputeWorldCoords(double *x, double *y, double *z)
{
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return 0;
	double xPos = *x; 
	double yPos = *y;
	double zPos = *z;
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics||!m_bGetMatricesSucceeded)
	{
//		double x = point.x;
//		double y = viewHeight-point.y;//point.y;

		double X = glFrustum_left	+ xPos * (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		double Y = glFrustum_bottom	+ yPos * (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);
		*x=X;
		*y=Y;
		*z=0;
		*x-=m_fTransX;
		*y-=m_fTransY;
		*z-=m_fTransZ;
		return 0;
	}

try
{
	RECT rc;
	GetClientRect(&rc);
	long ret = GL_FALSE;
	ret = gluUnProject(	xPos, rc.bottom - yPos, zPos,
						m_modelMatrix,
						m_projMatrix,
						m_viewport,
						x, y, z);
	ASSERT(ret == GL_TRUE);
	if(ret != GL_TRUE) {return (throw SolveIt_ERROR("CGLView::ComputeWorldCoords"),-1);}

/*
	m[0][0] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[2] * curquat[2]);
	m[0][1] = 2.0 * (curquat[0] * curquat[1] - curquat[2] * curquat[3]);
	m[0][2] = 2.0 * (curquat[2] * curquat[0] + curquat[1] * curquat[3]);
	m[0][3] = 0.0;

	m[1][0] = 2.0 * (curquat[0] * curquat[1] + curquat[2] * curquat[3]);
	m[1][1] = 1.0 - 2.0 * (curquat[2] * curquat[2] + 
							 curquat[0] * curquat[0]);
	m[1][2] = 2.0 * (curquat[1] * curquat[2] - curquat[0] * curquat[3]);
	m[1][3] = 0.0;

	m[2][0] = 2.0 * (curquat[2] * curquat[0] - curquat[1] * curquat[3]);
	m[2][1] = 2.0 * (curquat[1] * curquat[2] + curquat[0] * curquat[3]);
	m[2][2] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[0] * curquat[0]);
	m[2][3] = 0.0;
*/

/*
	xPos = *x; 
	yPos = *y;
	zPos = *z;

	*x	=	m[0][0]*xPos + m[0][1]*yPos + m[0][2]*zPos;
	*y	=	m[1][0]*xPos + m[1][1]*yPos + m[1][2]*zPos;
	*z	=	m[2][0]*xPos + m[2][1]*yPos + m[2][2]*zPos;
*/

	*x-=m_fTransX;
	*y-=m_fTransY;
	*z-=m_fTransZ;
	
	return ret == GL_TRUE? S_OK:-1;
}
catch( SolveIt_Error& e){e.why();return -12;}catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CGLView::ComputeScreenCoords(double& x, double& y, double& z, bool bExplicitSift)
{
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return 0;
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics||!m_bGetMatricesSucceeded)
	{
		double xPos = x; 
		double yPos = y;
		double zPos = z;

		if (bExplicitSift)
		{
			xPos +=m_fTransX;
			yPos +=m_fTransY;
			zPos +=m_fTransZ;
		}

		x=(xPos-glFrustum_left) * ((double) viewWidth) /(glFrustum_right - glFrustum_left) ;
		y=(yPos-glFrustum_bottom) *  ((double) viewHeight) /(glFrustum_top - glFrustum_bottom);
		z=0;
//	y = viewHeight - y;
		return 0;
	}

try
{
	RECT rect;
	GetClientRect(&rect);
//	y = rect.bottom - y;
	double xPos = x;// / sc.x;
	if (_isnan( xPos ))  {return (throw SolveIt_ERROR("_isnan( xPos )"),-1);}
	double yPos = y;// / sc.y; 
	if (_isnan( yPos ))  {return (throw SolveIt_ERROR("_isnan( yPos )"),-1);}
	double zPos = z;// / sc.z; 
	if (_isnan( zPos ))  {return (throw SolveIt_ERROR("_isnan( zPos )"),-1);}



	if (bExplicitSift)
	{
		xPos +=m_fTransX;
		yPos +=m_fTransY;
		zPos +=m_fTransZ;
	}

/*
	x	=	m[0][0]*xPos + m[1][0]*yPos + m[2][0]*zPos;
	y	=	m[0][1]*xPos + m[1][1]*yPos + m[2][1]*zPos;
	z	=	m[0][2]*xPos + m[1][2]*yPos + m[2][2]*zPos;

	xPos = x; 
	yPos = y;
	zPos = z;
*/

//	double zPos = (z - m_fNearPlane)/(m_fFarPlane - m_fNearPlane);
	long ret = gluProject(	xPos, yPos, zPos,
							m_modelMatrix,
							m_projMatrix,
							m_viewport,
							&x, &y, &z);
	ASSERT(ret == GL_TRUE);
//	if(ret != GL_TRUE) {return (throw -1,-1);}
	if(ret != GL_TRUE) {return -1;}
	if(ret != GL_TRUE) {return (throw SolveIt_ERROR("CGLView::ComputeScreenCoords"),-1);}

	y = rect.bottom - y;
	return ret == GL_TRUE? S_OK:-1;
}
catch( SolveIt_Error& e){e.why();return -2;}catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
long CGLView::ComputeWorldCoords(double& x, double& y, double& z, bool bExplicitSift)
{
	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return 0;
	double xPos = x; 
	double yPos = y;
	double zPos = z;
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics||!m_bGetMatricesSucceeded)
	{
//		double x = point.x;
//		double y = viewHeight-point.y;//point.y;

		double X = glFrustum_left	+ xPos * (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		double Y = glFrustum_bottom	+ yPos * (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);
		x=X;
		y=Y;
		z=0;

		if (bExplicitSift)
		{
			x-=m_fTransX;
			y-=m_fTransY;
			z-=m_fTransZ;
		}
		return 0;
	}

try
{
	RECT rc;
	GetClientRect(&rc);
	long ret = GL_FALSE;
	ret = gluUnProject(	xPos, rc.bottom - yPos, zPos,
						m_modelMatrix,
						m_projMatrix,
						m_viewport,
						&x, &y, &z);
	ASSERT(ret == GL_TRUE);
	if(ret != GL_TRUE) {return (throw SolveIt_ERROR("CGLView::ComputeWorldCoords"),-1);}

/*
	m[0][0] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[2] * curquat[2]);
	m[0][1] = 2.0 * (curquat[0] * curquat[1] - curquat[2] * curquat[3]);
	m[0][2] = 2.0 * (curquat[2] * curquat[0] + curquat[1] * curquat[3]);
	m[0][3] = 0.0;

	m[1][0] = 2.0 * (curquat[0] * curquat[1] + curquat[2] * curquat[3]);
	m[1][1] = 1.0 - 2.0 * (curquat[2] * curquat[2] + 
							 curquat[0] * curquat[0]);
	m[1][2] = 2.0 * (curquat[1] * curquat[2] - curquat[0] * curquat[3]);
	m[1][3] = 0.0;

	m[2][0] = 2.0 * (curquat[2] * curquat[0] - curquat[1] * curquat[3]);
	m[2][1] = 2.0 * (curquat[1] * curquat[2] + curquat[0] * curquat[3]);
	m[2][2] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[0] * curquat[0]);
	m[2][3] = 0.0;
*/

/*
	xPos = x; 
	yPos = y;
	zPos = z;

	x	=	m[0][0]*xPos + m[0][1]*yPos + m[0][2]*zPos;
	y	=	m[1][0]*xPos + m[1][1]*yPos + m[1][2]*zPos;
	z	=	m[2][0]*xPos + m[2][1]*yPos + m[2][2]*zPos;
*/


	if (bExplicitSift)
	{
		x-=m_fTransX;
		y-=m_fTransY;
		z-=m_fTransZ;
	}
	
	return ret == GL_TRUE? S_OK:-1;
}
catch( SolveIt_Error& e){e.why();return -12;}catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


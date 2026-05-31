// Scene.cpp : implementation file
#include "stdafx.h"
#include "action.h"
#include "MainFrm.h"
#include "Scene.h"
#include <afxpriv.h>
//#include "ThreadView.h"
//#include "PDBank/Qmol.h"
//#include "PDBank/QmolView.h"
//#include "ActionView.h"
#include "GL_Thread.h"
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern UINT WM_ACTION_Reset;
extern UINT WM_ACTION_Initialize;
extern UINT WM_KILL_VerhulstDynamics;
///////////////////////////////////////////////////////////////////////////////
HANDLE	gl_event=0;
//CGL* CGL::gl=0;
void setObjPos(){}
void setNearFar(){}
void setRotationAngles(){}
void setRotation(){}
///////////////////////////////////////////////////////////////////////////////
int		mainWindowID=0;
//float	view_rotate[16];
///////////////////////////////////////////////////////////////////////////////
Mutex mutexSoQtExaminerViewer;
extern Mutex mutexSoTransforms;
Mutex mutexRootSoSeparator;
//		Lock lock(mutexSoQtExaminerViewer);
///////////////////////////////////////////////////////////////////////////////
HWND _hWndCtl = 0;
///////////////////////////////////////////////////////////////////////////////
const double c_glFrustum			= 2.5;
const double c_glFrustum_znear		= 5;//0.25;//
const double c_glFrustum_zfar		= 10;//15;//
const double c_glTrans_z			= 0;//-5.4;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CScene
CScene::CScene() :
	gluPerspective_zNear(glFrustum_znear),
	gluPerspective_zFar(glFrustum_zfar),
	hWndCtl(0),
	_handle(0),
	_tid(0),
	event(0),
	event2(0),
//	sWindowName(_T("Open Inventor Window")),
	m_bstrCaption(_T("Copyright by Prof. P.L. Nash, UTSA")),
///////////////////////////////////////////////////////////////////////////////
	gluPerspective_fovy(60),
	gluPerspective_aspect(1),
//	gluPerspective_zNear(0.25),
//	gluPerspective_zFar(15),
//	m_fFOVy(15),
//	m_fNearPlane(0.01),	m_fFarPlane(200),
//	m_AspectRatio(1),

	gluPerspective_width(800),
	gluPerspective_height(800),

	glFrustum_left(-c_glFrustum),
	glFrustum_right(c_glFrustum),
	glFrustum_bottom(-c_glFrustum),
	glFrustum_top(c_glFrustum),
	glFrustum_znear(c_glFrustum_znear),
	glFrustum_zfar(c_glFrustum_zfar),

	_glFrustum_left(-c_glFrustum),
	_glFrustum_right(c_glFrustum),
	_glFrustum_bottom(-c_glFrustum),
	_glFrustum_top(c_glFrustum),
	_glFrustum_znear(c_glFrustum_znear),
	_glFrustum_zfar(c_glFrustum_zfar),

//	m_pIPlayer(0),
//	m_pCPlayer(0),
/////////////////////////////////////////////////////////////////////////////
	g_eye_z(32),
//	m_MathEquationOutput(__MathML),
	m_nWidth(800),
	m_nHeight(935),
//	m_begin_x(-1), m_begin_y(-1),
	beginx(0),	beginy(0),
	dazim(0.0),	delev(0.0),
	m_nTrueHit(0),
	m_hStream(0),
	bBeginDrawFirstTime(true),
	m_bstrAVIFileName(L""),
	m_hdcMemAVI(0),
	m_hBitmapAVI(0),
	m_hBitmapOldAVI(0),
	m_pBits(0),
	m_bMakingAVI(false),
	g_pavi(0),	
	g_psCompressed(0),	
	g_pfile(0),
	g_bInitCalled(false),

	m_fRotX(0),
	m_fRotY(0),
	m_fRotZ(0),
	m_fTransX(0),
	m_fTransY(0),
//	m_fTransZ(-5.4),
	m_fTransZ(c_glTrans_z),

	_fRotX(0),
	_fRotY(0),
	_fRotZ(0),
	_fTransX(0),
	_fTransY(0),
//	_fTransZ(-5.4),
	_fTransZ(c_glTrans_z),

///////////////////////////////////////////////////////////////////////////////
	m_TimerScan(2),
	m_uMillisecondsBetweenScanSteps(200),
	m_x1(-c_glFrustum/2),
	m_y1(c_glFrustum/2),
	m_x2(c_glFrustum/2),
	m_y2(-c_glFrustum/2),
	m_fScanCurveParameter(0),
	m_fScanDistance(sqrt((m_x2-m_x1)*(m_x2-m_x1)+(m_y2-m_y1)*(m_y2-m_y1))),
	m_fScanStepSize(1.41421356237309505/100.),

	m_viewOperation(rotation),
/*
	m_pPal(0),
	m_hPal(0),

	m_hRC(0),
	m_hdcMem(0),
	m_hdcMem2(0),
	m_hBitmap(0),
	m_hBitmapOld(0),

	m_hBitmapBackground(0),
	m_hBitmapTrash(0),m_hOldBitmapTrash(0),
	m_hPaletteTrash(0),m_hOldPaletteTrash(0),

	m_bTwoDimensionalWorld(false),
	m_bDo2DElectroStatics(false),
	m_bDoVerhulstDynamics(false),
	m_bSuperposition(false),
	m_bDo1DQuantumMechanics(false),
	m_bDo2DQuantumMechanics(false),
*/
	m_bMouseDown(false),
///////////////////////////////////////////////////////////////////////////////
	m_hLibDFORMD(0),

	m_scalePosition(Vector3D(2,2,2))

{
	rect.left = rect.top = 0;
	rect.right = m_nWidth;
	rect.bottom = m_nHeight;
	event = CreateEvent(0, FALSE, FALSE, 0);
	event2= CreateEvent(0, FALSE, FALSE, 0);
//	m_pCThreadView = new CThreadView;
//	theApp.theSystem.root = new SoSeparator;//
//	theApp.theSystem.root->ref();//
//	SoDBinit();
}
///////////////////////////////////////////////////////////////////////////////
CScene::~CScene()
{
	CloseHandle(event);
	CloseHandle(event2);
//	theApp.theSystem.root->unref();//
//	delete m_pCThreadView;
//	SoDBterm();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CScene::ResetView()
{

	glFrustum_left		= _glFrustum_left;
	glFrustum_right		= _glFrustum_right;
	glFrustum_bottom	= _glFrustum_bottom;
	glFrustum_top		= _glFrustum_top;
	glFrustum_znear		= _glFrustum_znear;
	glFrustum_zfar		= _glFrustum_zfar;

//	for (int j=0; j< sizeof(view_rotate)/sizeof(view_rotate[0]); j++)
//		view_rotate[j] = 0.0f;
//	view_rotate[0]=view_rotate[5]=view_rotate[10]=view_rotate[15]= 1.0f;
//	{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

	m_fRotX				= _fRotX;
	m_fRotY				= _fRotY;
	m_fRotZ				= _fRotZ;
	m_fTransX			= _fTransX;
	m_fTransY			= _fTransY;
	m_fTransZ			= _fTransZ;
/*
	setObjPos();
	setNearFar();
	setRotationAngles();
	setRotation();
*/
//	SetPerspective();

	SendMessage(hWndCtl, WM_ACTION_Reset, 0, 0);
//	Fire_ViewChange();
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CScene::Initialize()
{
	_glFrustum_left			=-c_glFrustum;
	_glFrustum_right		= c_glFrustum;
	_glFrustum_bottom		=-c_glFrustum;
	_glFrustum_top			= c_glFrustum;
	_glFrustum_znear		= c_glFrustum_znear;
	_glFrustum_zfar			= c_glFrustum_zfar;

	_fRotX				= 0;
	_fRotY				= 0;
	_fRotZ				= 0;
	_fTransX			= 0;
	_fTransY			= 0;
	_fTransZ			= c_glTrans_z;


	gluPerspective_fovy		= 60;
	gluPerspective_aspect	= 1;
	ResetView();

	if (::IsWindow(hWndCtl))
	{
		SendMessage(hWndCtl, WM_KILL_VerhulstDynamics, 0, 0);
	}
	CGLView *pView = CGL::gl->GetGLView();
	if (::IsWindow(hWndCtl))
	{
		if (pView) pView->SendMessage( WM_KILL_VerhulstDynamics, 0, 0);
	}
	CGLView* pCGLView = reinterpret_cast<CGLView*>(CWnd::FromHandle(hWndCtl));
	if (::IsWindow(hWndCtl))
	{
		if (pCGLView) pCGLView->SendMessage( WM_KILL_VerhulstDynamics, 0, 0);
	}

	if (::IsWindow(hWndCtl))
	{
		SendMessage(hWndCtl, WM_ACTION_Initialize, 0, 0);
	}
//	SendMessage(hWndCtl, WM_DeleteMolecule, 0, 0);
//	SendMessage(hWndCtl, WM_ACTION_RIGID_BODY, 0, 0);
//	CGLView *pView = CGL::gl->GetGLView();
//	if (pView) pView->Initialize();

	m_uMillisecondsBetweenScanSteps	= 200;
	m_x1					= -c_glFrustum/2;
	m_y1					= c_glFrustum/2;
	m_x2					= c_glFrustum/2;
	m_y2					= -c_glFrustum/2;
	m_fScanCurveParameter	= 0;
	m_fScanDistance			= sqrt((m_x2-m_x1)*(m_x2-m_x1)+(m_y2-m_y1)*(m_y2-m_y1));
	m_fScanStepSize			= 1.41421356237309505/100.;
///////////////////////////////////////////////////////////////////////////////
//	theScaleSoTransform->scaleFactor.setValue(SbVec3f(1.0f, 1.0f, 1.0f));
//	theScaleSoTransform->translation.setValue(SbVec3f(0.0f, 0.0f, 0.0f));
///////////////////////////////////////////////////////////////////////////////

	_bstr_t bstr(_T("Physics in Action"));
//##	Fire_TooltipString(bstr);
//##	if (m_bDo1DQuantumMechanics)	DATA_PASS_mp_TERM ();
//##	if (m_bDo2DQuantumMechanics)	DATA_2D_mp_TERM();
//	Terminate();
	put_TwoDimensionalWorld(FALSE);
	m_bDo1DQuantumMechanics = false;
	m_bDo2DQuantumMechanics = false;
	m_bDo2DElectroStatics = false;
///////////////////////////////////////////////////////////////////////////////
/*
	if ( m_pCQM_1D_Dialog )
	{
		if (::IsWindow(m_pCQM_1D_Dialog->m_hWnd))
			m_pCQM_1D_Dialog->DestroyWindow();
		delete m_pCQM_1D_Dialog;
	}
	m_pCQM_1D_Dialog = 0;
	if ( m_pCQM_1DScatt )	delete m_pCQM_1DScatt;
	m_pCQM_1DScatt = 0;
///////////////////////////////////////////////////////////////////////////////
	if ( m_pCQM_2D_Dialog )
	{
		if (::IsWindow(m_pCQM_2D_Dialog->m_hWnd))
			m_pCQM_2D_Dialog->DestroyWindow();
		delete m_pCQM_2D_Dialog;
	}
	m_pCQM_2D_Dialog = 0;
	if ( m_pCQM_2DScatt )	delete m_pCQM_2DScatt;
	m_pCQM_2DScatt = 0;
///////////////////////////////////////////////////////////////////////////////
	if ( m_pCWaveNumbers )
	{
		if (::IsWindow(m_pCWaveNumbers->m_hWnd))
			m_pCWaveNumbers->DestroyWindow();
		delete m_pCWaveNumbers;
	}
	m_pCWaveNumbers = 0;
	if ( m_pCSuperposition )	delete m_pCSuperposition;
	m_pCSuperposition = 0;
*/
///////////////////////////////////////////////////////////////////////////////
	m_bSuperposition = false;
///////////////////////////////////////////////////////////////////////////////
/*
	if ( m_hBitmapTrash )	DeleteObject( m_hBitmapTrash );
	if ( m_hPaletteTrash ) DeleteObject( m_hPaletteTrash );
	if (m_hBitmapBackground) DeleteObject(m_hBitmapBackground);
	m_hBitmapTrash=0;
	m_hPaletteTrash=0;
	m_hBitmapBackground=0;
	AVI_Term();
*/

	m_pointCurrent.x	= 0;
	m_pointCurrent.y	= 0;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CScene::Reset()
{
	glFrustum_left = _glFrustum_left;
	glFrustum_right = _glFrustum_right;
	glFrustum_bottom = _glFrustum_bottom;
	glFrustum_top = _glFrustum_top;
	glFrustum_znear = _glFrustum_znear;
	glFrustum_zfar = _glFrustum_zfar;

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
//void Fire_ViewChange(void){
//	theApp.theSystem.theScene.ReDraw();
//}
///////////////////////////////////////////////////////////////////////////////
int CScene::ReDraw()
{
//	if ( scenemanager )
//		scenemanager->scheduleRedraw();

//	glutPostRedisplay();
//	glutPostWindowRedisplay(mainWindowID);
	CGLView* pView = reinterpret_cast<CGLView*>(CGLView::FromHandle(hWndCtl));
//	ASSERT(pView);
	if(pView) pView->postRedisplay();
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
int CScene::Invalidate()
{
	ReDraw();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
int CScene::GLCreate()
{
//	return 0;
	gl_event = CreateEvent(0, FALSE, FALSE, 0);
	CGL::gl=CGL::GLBeginThread(THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	CGL::gl->ResumeThread();
	WaitForSingleObject(gl_event, INFINITE);

	hWndCtl = _hWndCtl;//go figger
	ASSERT(hWndCtl);
	ASSERT(::IsWindow(hWndCtl));
	return 0;

	DWORD WINAPI ThreadEntry (void* pArg);
try
{

		_handle = CreateThread (
								0, // Security attributes
								0, // Stack size
								(LPTHREAD_START_ROUTINE)ThreadEntry, 
								(void*) this, 
								CREATE_SUSPENDED, 
								&_tid);
		ResumeThread(_handle);
//force the system to create thread's message queue 
		WaitForSingleObject(event, INFINITE);
		BOOL ret = 0;
		ret = PostThreadMessage(_tid, WM_USER, 0, 0);
		WaitForSingleObject(event2, INFINITE);
//		CloseHandle(event);
//		CloseHandle(event2);
//		DWORD dwRet = MsgWaitForMultipleObjects(
//					1,						// One event to wait for
//					&event,		//g_EventHandle The array of eventsG_EventKillThread
//					FALSE,					// Wait for 1 event
//					INFINITE,				// Timeout value
//					QS_ALLINPUT);			// Any message wakes up

	//	WaitForSingleObject(event2, INFINITE);
	//	while (!(hWndCtl = FindWindow(0,"glut Window")));
	//	return 0;
	//	hWndCtl = FindWindow(0,(string("[Freeware] - ")+theApp.theSystem.sWindowName).c_str());//
		hWndCtl = _hWndCtl;//go figger
		ASSERT(hWndCtl);
		ASSERT(::IsWindow(hWndCtl));
	//	if ( hWndCtl == 0) return -1;
	//	_hWndCtl = hWndCtl;
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}
///////////////////////////////////////////////////////////////////////////////
void CScene::Terminate()
{
	DWORD dwExitCode=0;
	if ( GetExitCodeThread(_handle, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
	//	if (IsWindow(hWndCtl)) glutDestroyWindow(glutWindowID);
	//	while (::PostThreadMessage(_tid, WM_QUIT, 0, 0) == 0) ::Sleep(100); 
		::PostThreadMessage(_tid, WM_QUIT, 0, 0); 
		CloseHandle(_handle);
		_handle=0;
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void CScene::GetMatrices()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
	GLenum err1= GL_INVALID_OPERATION;//?called between a call to glBegin and the corresponding call to glEnd.
	GLenum err2= GL_INVALID_ENUM;
	GLenum err = glGetError();
	ASSERT(err != GL_INVALID_OPERATION);
	ASSERT(err != GL_INVALID_ENUM);
	glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
	glGetIntegerv(GL_VIEWPORT , m_viewport);
}
*/
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	GLdouble	m_modelMatrix[16];
	GLdouble	m_projMatrix[16];
	GLint		m_viewport[4];
///////////////////////////////////////////////////////////////////////////////
void GetMatrices()
{
//	const GLenum err1= GL_INVALID_OPERATION;//?called between a call to glBegin and the corresponding call to glEnd.
//	const GLenum err2= GL_INVALID_ENUM;
//	glutSetWindow(mainWindowID);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	ASSERT(err != GL_INVALID_OPERATION);
	ASSERT(err != GL_INVALID_ENUM);
	glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	glGetIntegerv(GL_VIEWPORT , m_viewport);
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void GetWindowRect(RECT &rect)
{
	GetWindowRect(theApp.theSystem.theScene.hWndCtl, & rect);
	return;
//	glutSetWindow(mainWindowID);
//	GLint i[4];
	struct {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} i;
	glGetIntegerv(GL_VIEWPORT, &i.x);
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
//fake it:
	rect.left	= i.x;
	rect.top	= i.y;
	rect.right	= i.x + i.width;
	rect.bottom	= i.y + i.height;
}
///////////////////////////////////////////////////////////////////////////////
void GetClientRect(RECT &rect)
{
	GetClientRect(theApp.theSystem.theScene.hWndCtl, & rect);
	return;
//	glutSetWindow(mainWindowID);
//	GLint i[4];
	struct {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} i;
	glGetIntegerv(GL_VIEWPORT, &i.x);
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
//fake it:
	rect.left	= 0;
	rect.top	= 0;
	rect.right	= i.width;
	rect.bottom	= i.height;
}
///////////////////////////////////////////////////////////////////////////////
long ComputeScreenCoords(double *x, double *y, double *z)
{
try
{
	RECT rect;
	GetClientRect(rect);
//	*y = rect.bottom - *y;
	double& xPos = *x;// / sc.x;
	if (_isnan( xPos ))  {return (throw -1,-1);}
	double& yPos = *y;// / sc.y; 
	if (_isnan( yPos ))  {return (throw -1,-1);}
	double& zPos = *z;// / sc.z; 
	if (_isnan( zPos ))  {return (throw -1,-1);}
//	double zPos = (*z - m_fNearPlane)/(m_fFarPlane - m_fNearPlane);
	long ret = gluProject(	xPos, yPos, zPos,
							m_modelMatrix,
							m_projMatrix,
							m_viewport,
							x, y, z);
	ASSERT(ret == GL_TRUE);
	if(ret != GL_TRUE) {return (throw -1,-1);}
	*y = rect.bottom - *y;
	return ret == GL_TRUE? S_OK:-1;
}
catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
long ComputeWorldCoords(double *x, double *y, double *z)
{
try
{
	double xPos = *x; 
	double yPos = *y;
	double zPos = *z;
	RECT rc;
	GetClientRect(rc);
	long ret = GL_FALSE;
	ret = gluUnProject(	xPos, rc.bottom - yPos, zPos,
						m_modelMatrix,
						m_projMatrix,
						m_viewport,
						x, y, z);
	ASSERT(ret == GL_TRUE);
//	*x-=theApp.theSystem.theScene.m_fTransX;
//	*y-=theApp.theSystem.theScene.m_fTransY;
//	*z-=theApp.theSystem.theScene.m_fTransZ;
	return ret == GL_TRUE? S_OK:-1;
}
catch(...)
{
	return -11;
}
}
///////////////////////////////////////////////////////////////////////////////
//		glOrtho2D:
//	zNear=-1, zFar=1 
///////////////////////////////////////////////////////////////////////////////
void SetFrustum(
				double& x1, double& x2,
				double& y1, double& y2,
				const double& aspect, const double& fovY,
				const double& z1, const double& x, const double& y,
				bool bAngleInRadians=false
				)
{
	if (z1 == -1) return;
	if (bAngleInRadians)
		if (fovY >= PI)
		{
			y2	=  1;
			y1	= -1;
			x2	=  1;
			x1	= -1;
			return;
		}
	if (!bAngleInRadians)
		if (DEG_TO_RAD*fovY >= PI)
		{
			y2	=  1;
			y1	= -1;
			x2	=  1;
			x1	= -1;
			return;
		}
	double a=  aspect/2;
	double t=  0;
	if (bAngleInRadians)
		t	=  tan(fovY/2);
	else
		t	=  tan(DEG_TO_RAD*fovY/2);
	y2		=  t*z1-y;
	y1		= -t*z1-y;
	double h=  y2 - y1;
	x2		=  a*h - x;
	x1		= -a*h - x;
	if (y2==y1) y2 += 1;
	if (x2==x1) x2 += 1;
}
///////////////////////////////////////////////////////////////////////////////
void SetPerspective(
				const double& x1, const double& x2,
				const double& y1, const double& y2,
				double& aspect, double& fovY,
				const double& z1, double& x, double& y,
				bool bAngleInRadians=false
				)
{
	if (z1 == -1) return;
	double h	=  y2 - y1;
	if (h <= 0.0)
	{
		ASSERT(0);
		return;
	}
	aspect		= (x2 - x1)  / h;
	fovY		= 2*atan(h/(2*z1));
	if (!bAngleInRadians)
		fovY	*= RAD_TO_DEG;
	x			= -(x2 + x1) / 2;
	y			= -(y2 + y1) / 2;
//	z			= -(z2 + z1) / 2;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CScene::SetPerspective()
{
	::SetPerspective(
				glFrustum_left, glFrustum_right,
				glFrustum_bottom, glFrustum_top,
				gluPerspective_aspect, gluPerspective_fovy,
				glFrustum_znear, m_fTransX, m_fTransY
				);
	_fTransX = m_fTransX;
	_fTransY = m_fTransY;
//	m_fTransZ				= -(glFrustum_zfar + glFrustum_znear) / 2;

//force a call to CGLView::SetupGLView in CGLView::RenderScene :
//	gluPerspective_width	= 0;
//	gluPerspective_height	= 0;
//force a call to CGLView::SetupGLView :
//	SendMessage(hWndCtl, WM_ACTION_Reset, 0, 0);

return;

	if (!CGL::gl) return;
	CGLView *pView = CGL::gl->GetGLView();
//	pView->SetFocus();

	RECT rect;
	::GetClientRect(hWndCtl, & rect);
	pView->SetupGLView(rect.right, 0);//rect.bottom);


return;

//	SetSize(width, height);
// Select the projection matrix and clear it
	glMatrixMode(GL_PROJECTION);
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	glLoadIdentity();
// Select the viewing volume
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	gluPerspective(	gluPerspective_fovy,
					gluPerspective_aspect,
					glFrustum_znear,
					glFrustum_zfar);
/*
	glFrustum( 
			glFrustum_left,
			glFrustum_right,
			glFrustum_bottom,
			glFrustum_top,
			glFrustum_znear,
			glFrustum_zfar
			);
*/
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);

// Switch back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
  	glLoadIdentity();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
const float z_so_GetMatrices_succeeds = -5.40f;
	glTranslatef(0, 0, z_so_GetMatrices_succeeds);
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);

	GetMatrices();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);


return;
}
///////////////////////////////////////////////////////////////////////////////
void CScene::SetFrustum()
{
	::SetFrustum(
				glFrustum_left, glFrustum_right,
				glFrustum_bottom, glFrustum_top,
				gluPerspective_aspect, gluPerspective_fovy,
				glFrustum_znear, m_fTransX, m_fTransY
				);
}
///////////////////////////////////////////////////////////////////////////////
void CScene::SetSize(int width, int height)
{
	if(height <= 0) return;
	if(width <= 0) return;
	gluPerspective_width	= width;
	gluPerspective_height	= height;
	gluPerspective_aspect	= ((double)gluPerspective_width)/gluPerspective_height;
//	glFrustum_top		=  height/2- m_fTransY;
//	glFrustum_bottom	= -height/2- m_fTransY;
//	gluPerspective_fovy		= RAD_TO_DEG*2*atan((glFrustum_top - glFrustum_bottom)/(2*glFrustum_znear));
//	glFrustum_right		= ( width/2 - m_fTransX);
//	glFrustum_left		= (-width/2 - m_fTransX);
}
///////////////////////////////////////////////////////////////////////////////
void CScene::SetPerspective(double fovy)
{
	gluPerspective_fovy = fovy;
	if(gluPerspective_fovy > 180.0)
		gluPerspective_fovy = 180.0;
	
	if(gluPerspective_fovy <= 0.0)
		gluPerspective_fovy = 0.0;
	SetFrustum();
//	SetPerspective();
}
///////////////////////////////////////////////////////////////////////////////
/*
// Select the projection matrix and clear it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
// Select the viewing volume
	gluPerspective(
			gluPerspective_fovy,
			gluPerspective_aspect,
			gluPerspective_zNear,
			gluPerspective_zFar
			);
*/
/*
	glFrustum( 
			theApp.theSystem.theScene.glFrustum_left,
			theApp.theSystem.theScene.glFrustum_right,
			theApp.theSystem.theScene.glFrustum_bottom,
			theApp.theSystem.theScene.glFrustum_top,
			theApp.theSystem.theScene.glFrustum_znear,
			theApp.theSystem.theScene.glFrustum_zfar
			);
*/
// Switch back to the modelview matrix
//	glMatrixMode(GL_MODELVIEW);

// GLView.cpp : implementation file
//
#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "GLView.h"
#include <string.h>
#include <memory.h>
#include "MainFrm.h"
//#include "LeftView.h"
#include "RigidBody/RigidBodyManager.h"
#include "DibSectn.h"
#include "RigidBody/FieldsManager.h"
#include "RigidBody/ConstraintManager.h"
#include "Box.h"
#include "CalcMap.h"

#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
///////////////////////////////////////////////////////////////////////////////
//LPSTR dupString(LPCSTR lpsz);
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CGLView

///////////////////////////////////////////////////////////////////////////////
/*
const double c_glFrustum			= 1;
const double small_positive_value	= 0.01;
//const double c_glFrustum_right		= 1000;
//const double c_glFrustum_left		= 0;
//Specify the distances from the viewer to the near	and far	depth clipping
//planes.  Both distances must be positive for glFrustum and gluPerspective;
//These distances are negative if the plane is to be
//behind the viewer for glOrtho-.
const double c_glFrustum_znear		= 1.045;
const double c_glFrustum_zfar		= 45;
*/
const double c_glFrustum			= 2.5;
const double c_glFrustum_znear		= 5;
const double c_glFrustum_zfar		= 10;


/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CGLView, deFormedView)

CGLView::CGLView(): 
	m_pDC(0),
	notifications_mutex(_T("notifications")),
	render_actions_mutex(_T("render_actions")),
//	deFormedView(CGLView::IDD),
//	gluPerspective_zNear(glFrustum_znear),
//	gluPerspective_zFar(glFrustum_zfar),
	m_bGetMatricesSucceeded(false),
	m_bPerspectiveTransformation(true),
	l_button_down(false),
	m_bFlyBy(false),

	m_numRectangles(0),
	m_pSelectedBox(0),
	
//	m_FieldDisplay(nothing),
	m_FieldLinesType(nothing),

	m_bInitialUpdateComplete(false),
	m_bShowBackground(false),
	m_bShow_vectorFields(false),
	m_bShow_equipotentials(false),
	m_bShow_fieldLines_electric(false),
	m_bShow_fieldLines_magnetic(false),
	m_bShow_fieldLines_gravitational(false),
///////////////////////////////////////////////////////////////////////////////
	gluPerspective_width(viewWidth),
	gluPerspective_height(viewHeight),

	gluPerspective_fovy(60),
	gluPerspective_aspect(1),

	glFrustum_left	(-2.5),
	glFrustum_right	(2.5),

	glFrustum_bottom	(-2.5),
	glFrustum_top		(2.5),

	glFrustum_znear	(c_glFrustum_znear),
	glFrustum_zfar	(c_glFrustum_zfar),
	Znear(0),Zfar(0),

	orig_glFrustum_left(-c_glFrustum),
	orig_glFrustum_right(c_glFrustum),
	orig_glFrustum_bottom(-c_glFrustum),
	orig_glFrustum_top(c_glFrustum),
	orig_glFrustum_znear(c_glFrustum_znear),
	orig_glFrustum_zfar(c_glFrustum_zfar),
///////////////////////////////////////////////////////////////////////////////
	m_num_tic_marks_x(101),
	m_num_tic_marks_y(101),

	m_min_tic_mark_spacing_x((orig_glFrustum_right - orig_glFrustum_left)/m_num_tic_marks_x),
	m_min_tic_mark_spacing_y((orig_glFrustum_top - orig_glFrustum_bottom)/m_num_tic_marks_y),

	m_fdelta( (glFrustum_right - glFrustum_left)/50),//for zoom, translate
///////////////////////////////////////////////////////////////////////////////
	m_hRCMem(0),
//	m_hdcMem(0),

//	m_hBitmap(0),
	m_hBitmapOld(0),

	m_hBitmapBackground(0),
	m_hBitmapTrash(0),m_hOldBitmapTrash(0),
	m_hPaletteTrash(0),m_hOldPaletteTrash(0),
///////////////////////////////////////////////////////////////////////////////
	m_ZoomLevel(0),
	pixBuf_(0),
	glArrColor_(0),
	glArrVertex_(0),
	colorTable_(0),
	maxi_(333),
	m_bReCalc(true),
	useGLArrays_(true),
	reshaped_(true),
///////////////////////////////////////////////////////////////////////////////
	listBackground(0),
	texture(0),
	m_bNewBackgroundTextureDefined(false),
/////////////////////////////////////////////////////////////////////////////
	viewWidth(100),
	viewHeight(100),

	m_fRotX(0),
	m_fRotY(0),
	m_fRotZ(0),

	m_fTransX(0),
	m_fTransY(0),

//	m_fTransZ(-0.45),
	m_fTransZ(-1.4*c_glFrustum_znear),
//	m_fTransZ((-glFrustum_znear)+(-small_positive_value)),

	orig_fRotX(0),
	orig_fRotY(0),
	orig_fRotZ(0),
	orig_fTransX(m_fTransX),
	orig_fTransY(m_fTransY),
	orig_fTransZ(m_fTransZ),
///////////////////////////////////////////////////////////////////////////////
	m_nTrueHit(0),
///////////////////////////////////////////////////////////////////////////////
	m_NDATA(101),


	m_fZoomScale(0.01),
	m_fScaleWave(1)
{
	m_pointCurrent.x	= 0;
	m_pointCurrent.y	= 0;
// Initialize the track ball
	tBall.trackball(0.0, 0.0, 0.0, 0.0);
	//{{AFX_DATA_INIT(CGLView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	_tcsncpy(m_xStr, __xStr, sizeof(m_xStr)/sizeof(m_xStr[0]) - 1);
//	_tcsncpy(m_yStr, __yStr, sizeof(m_yStr)/sizeof(m_yStr[0]) - 1);
	CMainFrame::theGLView	= this;
}

CGLView::~CGLView()
{
	if (glArrColor_ != NULL)	delete[] glArrColor_;//free(glArrColor_);
	if (glArrVertex_ != NULL)	delete[] glArrVertex_;//free(glArrVertex_);
	if (pixBuf_)				delete pixBuf_;
	if (colorTable_)			delete[] colorTable_;
	vector<CBox*>::iterator iBox;
	for (iBox = vecBoxes.begin(); iBox != vecBoxes.end(); ++iBox)
	{
		delete *iBox;
	}
	vecBoxes.clear();
}

#if 0
void CGLView::DoDataExchange(CDataExchange* pDX)
{
	deFormedView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
#endif

BEGIN_MESSAGE_MAP(CGLView, deFormedView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_COMMAND_RANGE(ID__BACKGROUND, ID_clear_gravitational, OnSelectFieldsToDisplay)
	ON_REGISTERED_MESSAGE(WM_USER_GetParam, OnGetParam)
	ON_REGISTERED_MESSAGE(WM_USER_SetParam, OnSetParam)
	ON_REGISTERED_MESSAGE(WM_USER_Redraw, OnRedraw)
END_MESSAGE_MAP()
//	ON_WM_CLOSE()
//	ON_WM_RBUTTONDOWN()

/////////////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG
void CGLView::AssertValid() const
{
	deFormedView::AssertValid();
}

void CGLView::Dump(CDumpContext& dc) const
{
	deFormedView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers
///////////////////////////////////////////////////////////////////////////////
BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// An OpenGL window must be created with the following flags and must not
		// include CS_PARENTDC for the class style. 
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return deFormedView::PreCreateWindow(cs);
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::InitializeOpenGL()
{
	glClearDepth(10.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//Background color

// prepare ligth source
//	const GLfloat ambient[] = {0.4f,0.4f,0.4f,1.0f };
	const GLfloat ambient[] = {0.2f,0.2f,0.2f,1.0f };
	const GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f };
//	const GLfloat position[] = {-4.0f, 14.0f, 14.0f, 0.0f };
	const GLfloat position[] = {4.0f,0.0f,6.0f,0.0f };

// Activate lighting and a light source
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
// Need to set glShadeModel to GL_SMOOTH for 3D objects 
// and set it to GL_FLAT for lines and points.
//	glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
// smooth the drawing
	glEnable(GL_NORMALIZE);
	glDisable(GL_FOG);
// Need to turn on GL_DITHER for 3D objects and turn it off for lines and points.
	glDisable(GL_DITHER);
//	glEnable(GL_DITHER);
// Anti-aliasted lines
	glEnable(GL_LINE_SMOOTH);

	GLenum e=GL_NO_ERROR;
	glEnable(GL_TEXTURE_2D);//GL_INVALID_OPERATION
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
//	glEnableClientState(GL_COLOR_ARRAY);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
// Allow translucent spheres to indicate selected atoms
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Cull back faces of polygons
//	glCullFace(GL_BACK);
//	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
// Define material parameters
	const GLfloat glfMatAmbient[] = {.1000f, .1000f, .1000f, 1.0f};
	const GLfloat glfMatDiffuse[] = {1.000f, 1.000f, 1.000f, 1.0f};
//	const GLfloat glfMatSpecular[]= {1.000f, 1.000f, 1.000f, 1.0f};
	const GLfloat glfMatSpecular[]= {0.600f, 0.600f, 0.600f, 1.0f};
	const GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
	const GLfloat fShininess = 40.0f;//128.000f;

//	glMaterialfv(GL_FRONT, GL_AMBIENT,	glfMatAmbient);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE,	glfMatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
//	glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
/*
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,	glfMatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,	glfMatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glfMatSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glfMatEmission);
*/
	glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, fShininess);

}


#undef AMBIENT   
#undef DIFFUSE   
#undef SPECULAR  
#undef EMISSION  
#undef SHININESS 

#undef RED   
#undef GREEN 
#undef BLUE  
#undef ALPHA 

#undef SHINE 

#undef MATERIAL 
#undef LIGHT    

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Global variables controlling the color depth and Z buffer size
int g_bpp	= 16;		// By default, use a 16 bit display for OpenGL
int g_z_buf	= 16;		// By default, use a 16 bit Z buffer for OpenGL

/////////////////////////////////////////////////////////////////////////////
int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//AfxMessageBox("a");
	wglMakeCurrent(0, 0);
	if (deFormedView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// Define pixel format
	m_pDC = new CClientDC(this);//
	int nPixelFormat=0;	
/*
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, NULL, sizeof(pfd));		
	pfd.nSize			= sizeof(pfd);
	pfd.nVersion	 = 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
*/

	static PIXELFORMATDESCRIPTOR pfd = 
	{
				sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
				1,								// version number
				PFD_DRAW_TO_WINDOW |			// support window
				PFD_SUPPORT_OPENGL |			// support OpenGL
				PFD_DOUBLEBUFFER,				// double buffered
				PFD_TYPE_RGBA,					// RGBA type
				g_bpp,							// bpp
				0, 0, 0, 0, 0, 0,				// color bits ignored
				0,								// Don't use the alpha buffer
				0,								// shift bit ignored
				0,								// no accumulation buffer
				0, 0, 0, 0,						// accum bits ignored
				g_z_buf,							// z-buffer
				0,								// no stencil buffer
				0,								// no auxiliary buffer
				PFD_MAIN_PLANE,					// main layer
				0,								// reserved
				0, 0, 0							// layer masks ignored
		};



	// Set pixel format
//		HDC hDC = GetDC()->GetSafeHdc();
	HDC hDC = m_pDC->GetSafeHdc();
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(FALSE == nPixelFormat)
	{
		::ReleaseDC(GetSafeHwnd(), hDC);
		throw SolveIt_Error("Unable to wglMakeCurrent", __FILE__, __LINE__);
	}
	if(FALSE == SetPixelFormat(hDC, nPixelFormat, &pfd))
	{
		::ReleaseDC(GetSafeHwnd(), hDC);
		throw SolveIt_Error("Unable to wglMakeCurrent", __FILE__, __LINE__);
	}
	// What is the final Pixel format?
	if(FALSE == DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
	{
		::ReleaseDC(GetSafeHwnd(), hDC);
		throw SolveIt_Error("Unable to wglMakeCurrent", __FILE__, __LINE__);
	}
	else
	{

		g_bpp = pfd.cColorBits;
		g_z_buf = pfd.cDepthBits;
	}

	// Create RC
	m_hRC = wglCreateContext(hDC);

	if(FALSE == ::wglMakeCurrent(hDC, m_hRC))
	{
		::ReleaseDC(GetSafeHwnd(), hDC);
		throw SolveIt_Error("Unable to wglMakeCurrent", __FILE__, __LINE__);
	}

// make the system font the device context's selected font 
	SelectObject (hDC, GetStockObject (SYSTEM_FONT)); 
 	wglUseFontBitmaps(hDC, 0, 255, 2000); 
	InitializeOpenGL();

	const GLubyte * glVersion_ = glGetString(GL_VERSION); //0x1F02GL_VERSION Get OpenGL Version
	if (glVersion_)
	{
		useGLArrays_ = glVersion_[2] > '0';
	}
	else
	{
		ASSERT(0);
		useGLArrays_=false;
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
		pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) dupString("error GL_VERSION."));
	}

	//wglMakeCurrent(0, 0);
	//::ReleaseDC(m_hWnd, hDC);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
void CGLView::OnInitialUpdate() 
{
	deFormedView::OnInitialUpdate();
	
	RECT deskRect;
	HWND desk_hwnd = ::GetDesktopWindow();
	::GetWindowRect(desk_hwnd, &deskRect);
//	RECT Rect;
//	GetWindowRect(hWndCtl, &Rect);
//	int w = 800;//Rect.right - Rect.left;
//	int h = 800;//Rect.bottom - Rect.top;
//	int w = (deskRect.right - deskRect.left)/2;
	int h = (deskRect.bottom - deskRect.top)/2;
	int w = h;
	int x = deskRect.right - w;
	int y = 0;
	h += 2*GetSystemMetrics(SM_CYSIZEFRAME);
	h += 2*GetSystemMetrics(SM_CYSIZE);
	h += 2*GetSystemMetrics(SM_CYEDGE);
	GetParent()->MoveWindow( x, y, w, h, TRUE);


	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	m_bInitialUpdateComplete=true;
	OnSize(0, rc.right, rc.bottom);
//////
		return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	ResetView();
	pFrame->OnToggleView();
	ResetView();
	pFrame->OnToggleView();
}
/////////////////////////////////////////////////////////////////////////////
void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	/*deFormedView*/CWnd::OnSize(nType, cx, cy);
	if ( 0 >= cx || 0 >= cy ){
		return;
	}
	if ( !m_bInitialUpdateComplete ) return;

	// Save the window dimensions
	viewWidth	= cx;
	viewHeight	= cy;
		gluPerspective_aspect	= double(cx)/double(cy);

	reshaped_ = true;

	if ( !m_hWnd || !::IsWindow(m_hWnd) ) return;
#if 0
	if (m_hdcMem)
	{
		SelectObject(m_hdcMem, m_hBitmapOld); 
		if (m_hBitmap) DeleteObject(m_hBitmap);
	//	ReleaseDC(m_hdcMem);
		DeleteDC(m_hdcMem);
	}
//	//HDC hDC = m_pDC->GetSafeHdc();//::GetDC(GetSafeHwnd());m_hWnd
	HDC hDC = GetDC()->GetSafeHdc();
	m_hdcMem = ::CreateCompatibleDC(hDC);
	if (m_hdcMem==0)  
	{ 
		::MessageBox(NULL, "Cannot CreateCompatibleDC.", "Error", MB_OK); 
		return; 
	}  
	//wglMakeCurrent(hDC, m_hRC);
	glViewport(0, 0, viewWidth, viewHeight);
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	//wglMakeCurrent(0, 0);
//	//:://ReleaseDC(m_hWnd, hDC);
//	::ReleaseDC(GetSafeHwnd(), hDC);
//	CSystem* m_system	= theApp.GetSystem();
//	if (m_system->mapper)	m_system->mapper->PostThreadMessage(WM_USER_SetParam, 0, 0);
	SetPerspectiveOrFrustum();//SetPerspective();
	if (m_hdcMem==0)  
	{ 
		::MessageBox(NULL, "Cannot CreateCompatibleDC.", "Error", MB_OK); 
		return; 
	}  

	m_hBitmap = DSCreateDIBSection( viewWidth, viewHeight, 24);//, m_pBits, bi);
	ASSERT(m_hBitmap != 0);
	m_hBitmapOld = (struct HBITMAP__ *) SelectObject(m_hdcMem, m_hBitmap); 

	BITMAP bmInfo ;

	GetObject(m_hBitmap, sizeof(BITMAP), &bmInfo) ;
	ASSERT(bmInfo.bmPlanes == 1) ;
	ASSERT((bmInfo.bmBitsPixel == 8) || 
		  (bmInfo.bmBitsPixel == 16) ||
		  (bmInfo.bmBitsPixel == 24) ||
		  (bmInfo.bmBitsPixel == 32)
		  );
#endif
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE; // tell Windows not to erase the background
	return FALSE; // tell Windows	to erase the background
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnDestroy() 
{
	glClearMap_List(listBackground, texture);
//	hWndGL = 0;
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);
	//KillScene();
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_hRC);


//	::ReleaseDC(m_hWnd, hDC);
//:://ReleaseDC(m_hWnd, hDC);

//CClientDC takes care of calling the Windows functionsGetDC at construction time andReleaseDC at destruction time
//	::ReleaseDC(GetSafeHwnd(), m_pDC->GetSafeHdc());
	delete m_pDC;
	m_pDC=0;


//	DestroyWindow();
	if (m_hRCMem) wglDeleteContext( m_hRCMem );
#if 0
	if (m_hdcMem)
	{
		SelectObject(m_hdcMem, m_hBitmapOld); 
		if (m_hBitmap) DeleteObject(m_hBitmap);
	//	ReleaseDC(m_hdcMem);
		DeleteDC(m_hdcMem);
	}
#endif
	if ( m_hBitmapTrash )  DeleteObject( m_hBitmapTrash );
	if ( m_hPaletteTrash ) DeleteObject( m_hPaletteTrash );
	if (m_hBitmapBackground) DeleteObject(m_hBitmapBackground);

	m_hBitmapTrash=0;
	m_hPaletteTrash=0;
	m_hBitmapBackground=0;
	deFormedView::OnDestroy();
	PostQuitMessage(0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::ClearBackground() 
{
	m_bNewBackgroundTextureDefined  = false;
	if (texture) glDeleteTextures(1, &texture);
	texture = 0;
	if (listBackground)		glDeleteLists(listBackground, 1);
	listBackground	= 0;
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::ClearBackgroundTexture() 
{
	m_bNewBackgroundTextureDefined  = false;
	if (texture) glDeleteTextures(1, &texture);
	texture = 0;
	if (listBackground)		glDeleteLists(listBackground, 1);
	listBackground	= 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::OnDraw(CDC* pDC) 
{

///////////////////////////////////////////////////////////////////////////////
try{
if (render_actions_mutex.wait(500))
{
	if (!render_actions.empty())
	{
	//	HDC hDC = GetDC()->GetSafeHdc();//m_pDC->GetSafeHdc();//::GetDC(m_hWnd);
	//	//wglMakeCurrent(hDC, m_hRC);
		list<VoidFn_VoidPtr>::iterator irender_actions;
		for (irender_actions = render_actions.begin(); irender_actions != render_actions.end(); ++irender_actions)
		{
			(*irender_actions)();//
		//	glFlush();
		//	glFinish();
		}
	//	//wglMakeCurrent(0, 0);
	//	//:://ReleaseDC(m_hWnd, hDC);
	}
//	render_actions.clear();
	render_actions_mutex.Release();
}
	}
	catch( SolveIt_Error& e)
	{
		e.why();
		return;
	}
	catch(...)
	{
		ASSERT(0);
	}
///////////////////////////////////////////////////////////////////////////////
	try{
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CSystem* m_system = theApp.GetSystem();
//	static GLfloat m[4][4];
	tBall.build_rotmatrix(m);
try{
	if (m_bNewBackgroundTextureDefined)// && render_actions.empty()
	{
		RGBImageRender();//creates new listBackground
	}
	}
	catch( SolveIt_Error& e)
	{
		e.why();
		return;
	}
	catch(...)
	{
		ASSERT(0);
	}

//		HDC hDC = GetDC()->GetSafeHdc();
		HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);
#if 0
1	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics)
	{
		glViewport(0, 0, viewWidth, viewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
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
#endif
	glRenderMode(GL_RENDER);  
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslated(m_fTransX, m_fTransY, m_fTransZ);
#if 10
	if (m_system->m_bDoMapper || m_system->m_bDoVerhulstDynamics)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_DITHER);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DITHER);
		glDisable(GL_NORMALIZE);
		glShadeModel(GL_FLAT);
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//
//		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//Background color
//1		glClear(GL_COLOR_BUFFER_BIT);

#if 0
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
#endif
	}
	else
#endif
	{
	//	glRotated(m_fRotX, 1.0f, 0.0f, 0.0f);
	//	glRotated(m_fRotY, 0.0f, 1.0f, 0.0f);
	//	glRotated(m_fRotZ, 0.0f, 0.0f, 1.0f);

		glMultMatrixd(&m[0][0]);
	}
////	glLoadMatrixf((const float *)m);
	if (m_bShowBackground && listBackground)
	{
		glCallList(listBackground);
#if 0
		glPopMatrix();
		glFlush();
		glFinish();
		SwapBuffers(hDC);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		return;
#endif
	}
///////////////////////////////////////////////////////////////////////////////
//	else
	if (m_system->m_bDoMapper)
	{
//	Lock lock (theApp.protectBodies);
	//	glPushMatrix();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//
		glClear(GL_COLOR_BUFFER_BIT);
#define RENDER_MODE_FPU_ASM	 1
		renderImage(maxi_, RENDER_MODE_FPU_ASM);
		glPopMatrix();
		glFlush();
		glFinish();
		SwapBuffers(hDC);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		return;
	}
///////////////////////////////////////////////////////////////////////////////
	else
	{
	//	Draw3DAxes(m_system->m_xStr.c_str(), m_system->m_yStr.c_str());
		Draw3DAxes(101);
	}
///////////////////////////////////////////////////////////////////////////////

//	else
#if 10
	if (m_system->m_bDoVerhulstDynamics)
	{
		glPushMatrix();
#if 10
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
//	glFlush();
//	glFinish();
//	SwapBuffers(hDC);
//	//wglMakeCurrent(0, 0);
//	//:://ReleaseDC(m_hWnd, hDC);
//	return;
//	Draw3DAxes("","");
	}
#endif

///////////////////////////////////////////////////////////////////////////////
//	InvertMatrix(m);
//	theApp.theSystem.DoDraw(m);
	CFieldsManager* fieldsManager			= theApp.GetFieldsManager();
	CRigidBodyManager*	rigidBodyManager	= theApp.GetRigidBodyManager();
	CConstraintManager* constraintManager	= theApp.GetConstraintManager();
	constraintManager->OnDraw();
	fieldsManager->OnDraw();
	rigidBodyManager->OnDraw();
	m_system->DoIt();
	if (m_system->m_bDoMapper)
	{
//	Lock lock (theApp.protectBodies);
	}
	GetMatrices();//
	glPopMatrix();
//
	glFlush();
	glFinish();
	SwapBuffers(hDC);
//	GetMatrices();
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
///////////////////////////////////////////////////////////////////////////////
//"Playing with fire, as GDI isn't supposed to be mixed with OpenGL double-buffer in a HDC."
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if( m_hBitmapTrash )
	{
		BITMAP		bm;
		GetObject(m_hBitmapTrash, sizeof(BITMAP), &bm );

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		HDC			hMemDC	= CreateCompatibleDC( hDC );
		m_hOldBitmapTrash	= (HBITMAP)SelectObject( hMemDC, m_hBitmapTrash );
//		m_hOldPaletteTrash	= SelectPalette( hDC, m_hPaletteTrash, FALSE );
//		RealizePalette( hDC );

		m_rcTrash.left	= 2;
		m_rcTrash.top	= viewHeight - 32 - 2;
		m_rcTrash.right	= 2 + 32;
		m_rcTrash.bottom= viewHeight - 2;
		
	//	BitBlt( wglGetCurrentDC(), m_rcTrash.left, m_rcTrash.top, bm.bmWidth, bm.bmHeight,
		BitBlt( hDC, m_rcTrash.left, m_rcTrash.top, bm.bmWidth, bm.bmHeight,
					hMemDC, 0, 0, SRCCOPY );

		SelectObject( hMemDC, m_hOldBitmapTrash );
//		SelectPalette( hDC, m_hOldPaletteTrash, FALSE );
		::ReleaseDC(GetSafeHwnd(), hDC);
		::ReleaseDC(GetSafeHwnd(), hMemDC);
		DeleteDC(hMemDC);
		hMemDC = 0;
	}
///////////////////////////////////////////////////////////////////////////////
#if 0
	char szNumber[64];
	wsprintf(szNumber,"%04u",m_FortranWrapper.iter) ;
	CString strCaption(szNumber);
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	SetBkMode(hDC, TRANSPARENT);
	CRect rc;
	GetClientRect(rc);
	int cx = rc.Width()/2, cy = rc.Height()/2;
	CFont* pOldFont = (class CFont *) SelectObject(hDC, &m_Font);
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	SetTextAlign(hDC,	TA_LEFT | TA_TOP);//TA_CENTER
	SetTextColor(hDC, RGB(0, 0, 128));
	CRect r(6, cy-2*tm.tmHeight, cx, tm.tmHeight);//
//	ExtTextOut(cx-2-tm.tmAveCharWidth*m_strCaption.GetLength() , cy - tm.tmHeight, ETO_OPAQUE , NULL, m_strCaption, m_strCaption.GetLength(), NULL);//ETO_CLIPPED
	ExtTextOut(hDC, cx-2-tm.tmAveCharWidth*strCaption.GetLength() , cy - tm.tmHeight, ETO_OPAQUE , NULL, strCaption, strCaption.GetLength(), NULL);//ETO_CLIPPED
	SelectObject(hDC, pOldFont);
	//:://ReleaseDC(m_hWnd, hDC);
#endif
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
extern HANDLE	gl_terminate_event;
void CGLView::OnClose() 
{
	if (gl_terminate_event)
	{
		SetEvent (gl_terminate_event);
theApp.CloseAllDocuments( TRUE );
		PostQuitMessage(0);
		return;
	}
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	if (!pFrame || !::IsWindow(pFrame->m_hWnd)) return;
	gl_terminate_event = CreateEvent(0, FALSE, FALSE, 0);
	::PostMessage(pFrame->GetSafeHwnd(), WM_CLOSE, 0, 0);//OnClose();
	DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&gl_terminate_event,		//g_EventHandle The array of eventsG_EventKillThread
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	deFormedView::OnClose();
theApp.CloseAllDocuments( TRUE );
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::PostNcDestroy()
{
	delete this;//
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
The WM_SETCURSOR message is sent to a window if the mouse causes 
the cursor to move within a window and mouse input is NOT captured
*/
BOOL CGLView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	long nTrueHit = 0;//false
//	theApp.theSystem.m_rigidBodyManager->HitTest((long*) &m_pointCurrent, &nTrueHit);

//	theApp.GetRigidBodyManager()->HitTest(m_pointCurrent, nTrueHit);//sets m_pSelectedRigidBody
	CRigidBody* pbody = theApp.GetRigidBodyManager()->HitTest(m_pointCurrent);
	nTrueHit= pbody? 1:0;


	CBox* pBox = HitTestRectangleItems(m_pointCurrent);
	if (nTrueHit)
	{
		if (l_button_down && nTrueHit)
		{
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));
		}
		else 
		{
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
	}
	else
	if (pBox)
	{
		if (pBox == m_pSelectedBox)
		{
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));
		}
		else 
		{
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
	}
	else
	{
		::SetCursor((struct HICON__ *) GetClassLong(m_hWnd, GCL_HCURSOR));
	}
	return 0;//CView::OnSetCursor(pWnd, nHitTest, message);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::Reset()
{
	glFrustum_left		= orig_glFrustum_left;
	glFrustum_right		= orig_glFrustum_right;
	glFrustum_bottom	= orig_glFrustum_bottom;
	glFrustum_top		= orig_glFrustum_top;
	glFrustum_znear		= orig_glFrustum_znear;
	glFrustum_zfar		= orig_glFrustum_zfar;

	m_fRotX				= orig_fRotX;
	m_fRotY				= orig_fRotY;
	m_fRotZ				= orig_fRotZ;

	m_fTransX			= orig_fTransX;
	m_fTransY			= orig_fTransY;
	m_fTransZ			= orig_fTransZ;

	SetPerspectiveOrFrustum();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::Clear_fieldLines(void)
{
	m_bShowBackground					= false;
	m_bShow_vectorFields				= false;
	m_bShow_equipotentials				= false;
	m_bShow_fieldLines_electric			= false;
	m_bShow_fieldLines_magnetic			= false;
	m_bShow_fieldLines_gravitational	= false;
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::Initialize()
{
	orig_glFrustum_left		= -2.5;
	orig_glFrustum_right	= 2.5;
	orig_glFrustum_bottom	= -2.5;
	orig_glFrustum_top		= 2.5;
	orig_glFrustum_znear	= c_glFrustum_znear;
	orig_glFrustum_zfar		= c_glFrustum_zfar;

	orig_fRotX		= 0;
	orig_fRotY		= 0;
	orig_fRotZ		= 0;

	orig_fTransX		= 0;
	orig_fTransY		= 0;
	orig_fTransZ		= -1.4*c_glFrustum_znear;

	m_num_tic_marks_x			= 101;
	m_num_tic_marks_y			= 101;

	m_min_tic_mark_spacing_x	= (orig_glFrustum_right - orig_glFrustum_left)/m_num_tic_marks_x;
	m_min_tic_mark_spacing_y	= (orig_glFrustum_top - orig_glFrustum_bottom)/m_num_tic_marks_y;
	
	Reset();

	m_bGetMatricesSucceeded		= false;
	m_bPerspectiveTransformation= false;
	l_button_down				= false;
	m_bFlyBy					= false;
	m_NDATA						= 101;
	maxi_						= 333;
	m_ZoomLevel					= 0;
	if (glArrColor_ != NULL)	delete[] glArrColor_;//free(glArrColor_);
	if (glArrVertex_ != NULL)	delete[] glArrVertex_;//free(glArrVertex_);
	if (pixBuf_)				delete pixBuf_;
	if (colorTable_)			delete[] colorTable_;
	pixBuf_			= 0;
	glArrColor_		= 0;
	glArrVertex_	= 0;
	colorTable_		= 0;
	reshaped_		= true;
///////////////////////////////////////////////////////////////////////////////
	Clear_fieldLines();
//	m_FieldDisplay				= nothing;
///////////////////////////////////////////////////////////////////////////////
	m_nTrueHit					= 0;
	m_pointCurrent.x=m_pointCurrent.y=0;
// rotations
	beginx						= 0;
  	beginy						= 0;

	m_numRectangles				= 0;
	m_pSelectedBox				= 0;

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);
	InitializeOpenGL();

	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
	SetPerspectiveOrFrustum();

	try{
	glClearMap_List(listBackground, texture);
	ASSERT(listBackground==0);
	ASSERT(texture==0);
	vector<CBox*>::iterator iBox;
	for (iBox = vecBoxes.begin(); iBox != vecBoxes.end(); ++iBox)
	{
		delete *iBox;
	}
	vecBoxes.clear();
}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		ASSERT(0);
	}


///////////////////////////////////////////////////////////////////////////////
	{
		if (notifications_mutex.wait(500))
		{
			notifications.clear();
/*
			notifications.push_back(&CSystem::GLViewNotificationCallback);
*/
			notifications_mutex.Release();
		}
		if (render_actions_mutex.wait(500))
		{
			render_actions.clear();
		//	render_actions.push_back(&CSystem::GLViewRenderCallback);
			render_actions_mutex.Release();
		}
	}
///////////////////////////////////////////////////////////////////////////////
/*
///////////////////////////////////////////////////////////////////////////////
void __cdecl CSystem::GLViewRenderCallback(void)
{
	GLenum e=GL_NO_ERROR;
	if (theApp.theSystem.m_bDeleteCalllistPotential)
	{
		theApp.theSystem.m_bDeleteCalllistPotential = false;
		if (theApp.theSystem.calllistPotential)
			glDeleteLists(theApp.theSystem.calllistPotential, 1);
		theApp.theSystem.calllistPotential = 0;
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ASSERT(0);
			return;
		}
	}

///////////////////////////////////////////////////////////////////////////////
{
	const double	&x1	= theApp.theSystem.theScene.glFrustum_left;
	const double	&x2	= theApp.theSystem.theScene.glFrustum_right;
	const double	&y1	= theApp.theSystem.theScene.glFrustum_bottom;
	const double	&y2	= theApp.theSystem.theScene.glFrustum_top;
	double fScale=1.0;
	double glColor[3]={1,0,.5};

	LinePlotList::iterator it;
	for (it = theApp.theSystem.m_LinePlotList.begin(); it != theApp.theSystem.m_LinePlotList.end(); ++it)
	{
		ShareHandle<xy_Data>& s = *it;
	//	xy_Data*& s = *it;
		if (!s->plotCalllist)
		{
			s->DefinePlot( x1, x2, y1, y2);// , CompleteEllipticIntegralFirstKind);
			s->DrawNewList(glColor, fScale);
		}
	}

}

	if (theApp.theSystem.m_bUserDefinedPotentialIsValid == false) return;
	if (theApp.theSystem.calllistPotential) return;
	theApp.theSystem.DrawSystemPotential();
}
///////////////////////////////////////////////////////////////////////////////

*/
}

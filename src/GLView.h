#if !defined(AFX_GLVIEW_H__30D05D16_1DBD_4E3F_9D4F_25B944B41123__INCLUDED_)
#define AFX_GLVIEW_H__30D05D16_1DBD_4E3F_9D4F_25B944B41123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLView.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "objects.h"
#include "trackball2.h"

#include "vfw.h"

#include "RigidBody/SMutex.h"

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class objects;
class CBox;
class PixelBuffer;

///////////////////////////////////////////////////////////////////////////////

void InvertMatrix(GLfloat m[][4]);
/////////////////////////////////////////////////////////////////////////////
// CGLView form view
//	#define deFormedView CFormView
#define deFormedView CView
const int	imageWidth	= 64;
const int	imageHeight	= 64;


class CGLView : public deFormedView
{
protected:
	CGLView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGLView)
bool m_bInitialUpdateComplete; 

// Form Data
public:
	//{{AFX_DATA(CGLView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
//	enum { IDD = IDD_GL_VIEW };

// Attributes
public:

// Operations
public:
	void InitializeOpenGL();
	int MakePNGImage();
	void OnExportEps();

	void Destroy();
	int Create();
//	int MakeCurrent();
///////////////////////////////////////////////////////////////////////////////
	void Clear(void);
	void Initialize(void);
///////////////////////////////////////////////////////////////////////////////
	void Draw3DAxes(long nTicks);
	void Draw3DAxes(const wchar_t* xStr=0, const wchar_t* yStr=0);
	void Draw3DAxes(const wchar_t* xStr, const wchar_t* yStr, const RECT& xRC, const RECT& yRC, const wchar_t* xFormatStr=0, const wchar_t* yFormatStr=0);
	void GetStrSize(const wchar_t* str, HFONT hfntIn, int& Cx, int& Cy, int& cx, int& cy, bool bRestoreFont=false);
///////////////////////////////////////////////////////////////////////////////
	GLdouble	m_modelMatrix[16];
	GLdouble	m_projMatrix[16];
	GLint		m_viewport[4];
	void GetMatrices(void);
	bool m_bGetMatricesSucceeded;
// for tic mark spacing:
	double m_min_tic_mark_spacing_x;
	double m_min_tic_mark_spacing_y;
	long m_num_tic_marks_x;
	long m_num_tic_marks_y;
///////////////////////////////////////////////////////////////////////////////
//	void SetPerspective(void);
//	void SetPerspective(double fovy);
	void MapPerspective(bool bAngleInRadians=false);
	void MapFrustum(bool bAngleInRadians=false);

	void SetPerspectiveSize(int width, int height);
	void SetPerspectiveOrFrustum(void);

	bool m_bFlyBy;
	void ToggleViewTransformation();
	int ResetView();
	void Reset();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long ComputeScreenCoords(double *x, double *y, double *z);
	long ComputeWorldCoords(double *x, double *y, double *z);
///////////////////////////////////////////////////////////////////////////////
	long ComputeScreenCoords(double& x, double& y, double& z, bool bExplicitSift=true);
	long ComputeWorldCoords(double& x, double& y, double& z, bool bExplicitSift=true);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long m_nTrueHit;//false == 0
	void ComputeMapperCoords(const POINT& point, bool bZoomIn=true);
///////////////////////////////////////////////////////////////////////////////
	POINT m_pointCurrent;
// rotations
	int beginx;
	int beginy;
	bool l_button_down;
// The virtual track ball
	VirtualTrackBall tBall;
	void startRotation(int x, int y)
	{
		beginx = x;
		beginy = y;
//		stopSpinning();
	}
	void rotation(int x, int y);
///////////////////////////////////////////////////////////////////////////////
	long GetParam(WPARAM wParam, objects& obj);
	long SetParam(WPARAM wParam, const objects& obj);
/*
	enum FieldDisplay {
		nothing				= 0,
		backgroundTexture	= (1<<0),
		vectorFields		= (1<<1),
		fieldLines			= (1<<2),
		equipotentials		= (1<<3)
	} m_FieldDisplay;
*/
	bool m_bShowBackground;
//	bool m_bShow_backgroundTexture;
	bool m_bShow_vectorFields;
	bool m_bShow_equipotentials;
	bool m_bShow_fieldLines_electric;
	bool m_bShow_fieldLines_magnetic;
	bool m_bShow_fieldLines_gravitational;
	enum FieldLinesType {
		nothing				= 0,
		electric		= (1<<0),
		magnetic		= (1<<1),
		gravitational	= (1<<2)
	} m_FieldLinesType;
///////////////////////////////////////////////////////////////////////////////
	void Clear_fieldLines(void);
///////////////////////////////////////////////////////////////////////////////
	void glDrawMap_List2(const POINT& point, int bZoomIn=1);
//	void glDrawMap_List2(const double& y1, const double& y2, GLuint& listVerhulst, GLuint& texture);
	void glDrawMap2(const double& y_max, const double& y_min);
	void glComputeMap(const double& y_max, const double& y_min);
	void calcRasterRow(const unsigned int row, double rate, double x0, double x1);
	void calcRasterRow(unsigned int* rowBuffer, double rate, double x0, double x1);
///////////////////////////////////////////////////////////////////////////////
	void OnVerhulstDynamics();
	wchar_t *PixelsSnapshot();
	void PixelsToTexture(wchar_t *snapshotBuffer, GLuint& texture, bool bFreeBuffer = true);
	void TextureToDisplayList(GLuint& listVerhulst, GLuint& texture, bool bFreeList = true);
	void glDrawMap_List();
//	void glDrawMap(const double& y_max, const double& y_min);
	void glDrawMap_List(const double& y1, const double& y2, GLuint& listVerhulst, GLuint& texture);
	void glClearMap_List(GLuint& listVerhulst, GLuint& texture);
//	void DrawMap(double y1, double y2, GLuint& texture);
	int RenderTextureAsBackground(const GLuint& texture);
	void BitmapTexture(const HBITMAP& hBmp, GLuint& texture );
	void PlayMap(int& j, int& sleep, double& delay);
	HMIDISTRM m_hStream;
	UINT m_uDeviceID;      
///////////////////////////////////////////////////////////////////////////////
PixelBuffer* pixBuf_;// = NULL;
GLubyte* glArrColor_;// = NULL;
GLfloat* glArrVertex_;// = NULL;
GLubyte* colorTable_;// = NULL;
bool useGLArrays_;// = true;
bool reshaped_;// = false;
bool m_bReCalc;
	int m_ZoomLevel;// = 40;
unsigned int maxi_;// = 40;
	bool renderPixelRow(int row, int mode);
	bool renderImage(unsigned int maxi, int mode);
	void prepareColorTable(int numColors, GLubyte startR, GLubyte startG, GLubyte startB);
bool calcPixelRows(unsigned int maxi);
bool calcPixelRow(int row, unsigned int maxi, int mode);
bool calcPixelRow_C(int row, unsigned int maxi);
bool calcPixelRow_FPU_ASM(int row, unsigned int maxi);
void SetupImage();
bool calcPixelRow_FPU_ASM(int row, unsigned int maxi, double ax_, double _cy, int w_, double sx_);
bool calcPixelRow(int row, unsigned int maxi, double ax_, double _cy, int w_, double sx_);
double _cy;
double sx_;
	double size_x_;// = (ex_ - ax_) / ((double) viewWidth);
	double size_y_;// = (ey_ - ay_) / ((double) viewHeight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#if 0
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#endif

// Implementation
protected:
	virtual ~CGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnSelectFieldsToDisplay( UINT nID );
	afx_msg LRESULT OnGetParam(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetParam(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Data
public:
	CDC*	m_pDC;
	HGLRC m_hRC;		  // Handle to RC
	int viewWidth;
	int viewHeight;
	int& gluPerspective_width;
	int& gluPerspective_height;

	enum _paramTypes{
		_NDATA,
		_glFrustum_left,
		_glFrustum_right,
		_glFrustum_bottom,
		_glFrustum_top,
		_glFrustum_znear,
		_glFrustum_zfar,
		_fTransX,
		_fTransY,
		_fTransZ,
		_ZoomScale,
		_ScaleWave,
		_xStr,
		_yStr
	} paramTypes;

	long		m_NDATA;
/*
	objects m_xoStr;
	objects m_yoStr;
	wchar_t m_xStr[256];
	wchar_t m_yStr[256];
	
	static double m[4][4];
*/	
	
	double m[4][4];


	double  m_fRotX;
	double  m_fRotY;
	double  m_fRotZ;

	double  m_fTransX;
	double  m_fTransY;
	double  m_fTransZ;

	double  orig_fRotX;
	double  orig_fRotY;
	double  orig_fRotZ;

	double  orig_fTransX;
	double  orig_fTransY;
	double  orig_fTransZ;

	void set_glTrans_X(const double& val);
	void set_glTrans_Y(const double& val);
	void set_glTrans_Z(const double& val);

	void set_glRot_X(const double& val);
	void set_glRot_Y(const double& val);
	void set_glRot_Z(const double& val);
///////////////////////////////////////////////////////////////////////////////
	double glFrustum_left;
	double glFrustum_right;
	double glFrustum_bottom;
	double glFrustum_top;

//const	double glFrustum_znear;
	double glFrustum_znear;
	double glFrustum_zfar;
	double Znear,Zfar;

	double orig_glFrustum_left;
	double orig_glFrustum_right;
	double orig_glFrustum_bottom;
	double orig_glFrustum_top;
	double orig_glFrustum_znear;
	double orig_glFrustum_zfar;

	void set_glFrustum_left(const double& val);
	void set_glFrustum_right(const double& val);
	void set_glFrustum_bottom(const double& val);
	void set_glFrustum_top(const double& val);
	void set_glFrustum_znear(const double& val);
	void set_glFrustum_zfar(const double& val);

	void BoundingBox(const double& L,const double& R,const double& B,const double& T,const double& N,const double& F);
	double gluPerspective_fovy;		// The field of view angle, in degrees, in the y-direction.
	double gluPerspective_aspect;	// ratio of x (width) to y (height). 

//	double& gluPerspective_zNear;	// The distance from the viewer to the near clipping plane (always positive). 
//	double& gluPerspective_zFar;	// The distance from the viewer to the far clipping plane (always positive).


	bool m_bPerspectiveTransformation;
///////////////////////////////////////////////////////////////////////////////
	Vector3D m_SelectedVector3D;
	double m_fdelta;//for zoom, translate
///////////////////////////////////////////////////////////////////////////////
	double	m_fZoomScale;
	double	m_fScaleWave;
///////////////////////////////////////////////////////////////////////////////
	GLuint listBackground;
	GLuint texture;
	GLubyte image[3*imageWidth*imageHeight];
	int RGBImageRender();
	bool m_bNewBackgroundTextureDefined;
	int NurbsRender64(const double data[64][64]);
	int NurbsRender64(const double X[64], const double Y[64], const double data[64][64]);
	int NurbsRender64(const double X[64], const double Y[64], const double data[64][64], const double data2[64][64], const GLfloat edgePt[64][2]);

// rendering context handle for MemDC:
	HGLRC	m_hRCMem;
//	HDC		m_hdcMem;
///////////////////////////////////////////////////////////////////////////////
	RECT		m_rcTrash;
	HBITMAP		m_hBitmapTrash, m_hOldBitmapTrash;
	HPALETTE	m_hPaletteTrash, m_hOldPaletteTrash;
	void LoadTrashCan();
///////////////////////////////////////////////////////////////////////////////
	HBITMAP CreateBackgroundDIBSection();
	long RenderBackground();
	long DrawBackground();
	HBITMAP m_hBitmapBackground;
//	HBITMAP	m_hBitmap;
	HBITMAP	m_hBitmapOld;
///////////////////////////////////////////////////////////////////////////////
	CBox* m_pSelectedBox;
	long m_numRectangles;
	CBox* HitTestRectangleItems(const POINT& point);
	CBox* AddPot(const RECT& r);//inline 
	void RemovePot(CBox* pbox);//inline 
	void DefinePotential();
	std::vector<CBox *> vecBoxes;
///////////////////////////////////////////////////////////////////////////////
	afx_msg void ClearBackground();
	void ClearBackgroundTexture();
///////////////////////////////////////////////////////////////////////////////
	CString m_strCaption;
	CFont m_Font;
///////////////////////////////////////////////////////////////////////////////
	list<VoidFn_VoidPtr> render_actions;
	list<VoidFn_VoidPtr> notifications;
	SMutex render_actions_mutex;
	SMutex notifications_mutex;
///////////////////////////////////////////////////////////////////////////////

};

///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_GLVIEW_H__30D05D16_1DBD_4E3F_9D4F_25B944B41123__INCLUDED_)

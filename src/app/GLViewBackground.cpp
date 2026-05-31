// GLViewBackground.cpp : implementation file
//

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "GLView.h"
#include <string.h>
#include <memory.h>
#include "MainFrm.h"
#include "DibSectn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//extern const wchar_t scratchFileName[];
//extern const wchar_t scratchBmpName[];

//assumes agent has set image
int CGLView::RGBImageRender()
{
	if (m_bNewBackgroundTextureDefined == false) return 0;

//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
//	if(FALSE == ::wglMakeCurrent(hDC, m_hRC))
//	{
//		::ReleaseDC(GetSafeHwnd(), hDC);
//		throw SolveIt_ERROR(L"Unable to //wglMakeCurrent");
//		return -11;
//	}
	{
		m_bNewBackgroundTextureDefined = false;
		if (texture) glDeleteTextures(1,&texture);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture );

	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//GL_MODULATE
//		glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);//GL_RGB
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imageWidth, imageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
//		glEnable(GL_TEXTURE_2D);

		const double &x1 = glFrustum_left;
		const double &x2 = glFrustum_right;
		const double &y1 = glFrustum_bottom;
		const double &y2 = glFrustum_top;

		if (listBackground)	glDeleteLists(listBackground, 1);
		listBackground = 0;
		GLenum e=GL_NO_ERROR;
		if(listBackground == 0) listBackground = glGenLists(1);
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			//wglMakeCurrent(0, 0);
		//	::ReleaseDC(GetSafeHwnd(), hDC);
			throw SolveIt_ERROR(L"RGBImageRender: Unable to glGenLists");
	//		ASSERT(0);
			return -1;
		}

		glNewList(listBackground, GL_COMPILE);
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			//wglMakeCurrent(0, 0);
		//	::ReleaseDC(GetSafeHwnd(), hDC);
			throw SolveIt_ERROR(L"RGBImageRender: Unable to glNewList");
	//		ASSERT(0);
			return -1;
		}

		glPushMatrix();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);

		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			//wglMakeCurrent(0, 0);
		//	::ReleaseDC(GetSafeHwnd(), hDC);
			throw SolveIt_ERROR(L"RGBImageRender: Unable to glEnable(GL_TEXTURE_2D)");
	//		ASSERT(0);
			return -1;
		}

		glBindTexture(GL_TEXTURE_2D, texture );
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			//wglMakeCurrent(0, 0);
		//	::ReleaseDC(GetSafeHwnd(), hDC);
			throw SolveIt_ERROR(L"RGBImageRender: Unable to glEnable(GL_TEXTURE_2D)");
	//		ASSERT(0);
			return -1;
		}

		const float f=1.0f;
		const float z=0.001f;//so plane is not overwritten by nurbs->0
		glColor3f(f,f,f);
		glBegin(GL_QUADS);
			glNormal3d(0, 0, 1);
			glTexCoord2d(0,0);glVertex3d(x1,y1, -z);
			glTexCoord2d(0,1);glVertex3d(x1,y2, -z);
			glTexCoord2d(1,1);glVertex3d(x2,y2, -z);
			glTexCoord2d(1,0);glVertex3d(x2,y1, -z);

			glNormal3d(0, 0, -1);
			glTexCoord2d(0,1);glVertex3d(x1,y2, z);
			glTexCoord2d(0,0);glVertex3d(x1,y1, z);
			glTexCoord2d(1,0);glVertex3d(x2,y1, z);
			glTexCoord2d(1,1);glVertex3d(x2,y2, z);
		glEnd();

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			ASSERT(0);
			return -1;
		}
		glEndList();
	}
	//wglMakeCurrent(0, 0);
	//::ReleaseDC(GetSafeHwnd(), hDC);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int oglSetPixelFormatOverlay(HDC hDC, BYTE type, DWORD flags)
{
	 int pf, maxpf;
	 PIXELFORMATDESCRIPTOR pfd;
	 LAYERPLANEDESCRIPTOR  lpd;		/* layer plane descriptor */
	 int	nEntries = 2;			/* number of entries in palette */
	 COLORREF crEntries[2] = {		/* entries in custom palette */
		0x00000000,			/* black (ref #0 = transparent) */
		0x00ff0000,			/* blue */
	 };

	 /* get the maximum number of pixel formats */
	 maxpf = DescribePixelFormat(hDC, 0, 0, NULL);
	 
	 /* find an overlay layer descriptor */
	 for(pf = 0; pf < maxpf; pf++) {
		  DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	/* the bReserved field of the PIXELFORMATDESCRIPTOR contains the
		number of overlay/underlay planes */
	if (pfd.bReserved > 0) {
	  /* aha! This format has overlays/underlays */
	  wglDescribeLayerPlane(hDC, pf, 1,
				sizeof(LAYERPLANEDESCRIPTOR), &lpd);
	  if (lpd.dwFlags & LPD_SUPPORT_OPENGL &&
			lpd.dwFlags & flags)
		 {
			goto found;
		 }
	}
	 }
	 /* couldn't find any overlay/underlay planes */
		MessageBeep(MB_OK);
//	 MessageBox(0,L"non-Fatal Error:  Hardware does not support overlay planes.",L"Error", MB_OK);
	 return 0;

found:
	 /* now get the "normal" pixel format descriptor for the layer */
	 DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	 /* set the pixel format */
	 if(SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(0,L"SetPixelFormat() failed:  Cannot set format specified.",L"Error", MB_OK);
		  return 0;
	 }
	 
	 /* set up the layer palette */
	 wglSetLayerPaletteEntries(hDC, 1, 0, nEntries, crEntries);

	 /* realize the palette */
	 wglRealizeLayerPalette(hDC, 1, TRUE);

	 /* announce what we've got */
	 printf("Number of overlays = %d\n", pfd.bReserved);
	 printf("Color bits in the overlay = %d\n", lpd.cColorBits);

	 return pf;
}	 

///////////////////////////////////////////////////////////////////////////////
/*
int CGLView::Initialize()
{
	if ( m_hBitmapTrash )	DeleteObject( m_hBitmapTrash );
	if ( m_hPaletteTrash ) DeleteObject( m_hPaletteTrash );
	if (m_hBitmapBackground) DeleteObject(m_hBitmapBackground);
	m_hBitmapTrash=0;
	m_hPaletteTrash=0;
	m_hBitmapBackground=0;
	AVI_Term();
	return 0;
}
*/
///////////////////////////////////////////////////////////////////////////////
#if 0
HBITMAP CGLView::CreateBackgroundDIBSection()
{
	ASSERT(0);
	ASSERT(m_hBitmapBackground==0);
//	if (theApp.theSystem.m_pCQM_1DScatt==0 && theApp.theSystem.m_pCQM_2DScatt==0) return 0;
	RECT rc;
	GetClientRect(&rc);
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if (m_hdcMem==0) return 0;
	//wglMakeCurrent(m_hdcMem, m_hRC);

	if ( GL_NO_ERROR !=  glGetError() )
	{
		MessageBox("Error while CGLView::CreateBackgroundDIBSection.");
		return 0;
	}

	OnDraw(CDC::FromHandle(m_hdcMem));


	if ( GL_NO_ERROR !=  glGetError() )
	{
		MessageBox("Error while CGLView::CreateBackgroundDIBSection.");
		return 0;
	}
	//wglMakeCurrent(0, 0);

{
	::SetBkMode(m_hdcMem, TRANSPARENT);
/*
	CFont m_font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
	strcpy(lf.lfFaceName, "Arial");
	m_font.CreateFontIndirect(&lf);
*/
	CRect rc;
	GetClientRect(rc);
	int cx = rc.Width(), cy = rc.Height();

	HFONT pOldFont = (struct HFONT__ *) ::SelectObject(m_hdcMem, &m_Font);
	TEXTMETRIC tm;
	::GetTextMetrics(m_hdcMem, &tm);
	::SetTextAlign(m_hdcMem,  TA_LEFT | TA_TOP);//TA_CENTER
	::SetTextColor(m_hdcMem, RGB(0, 0, 128));
	CRect r(6, cy-2*tm.tmHeight, cx, tm.tmHeight);//
	::ExtTextOut(m_hdcMem, 6, cy - tm.tmHeight, ETO_OPAQUE , NULL, m_strCaption, m_strCaption.GetLength(), NULL);//ETO_CLIPPED
	::SelectObject(m_hdcMem, pOldFont);
}

//	long ret = RenderBackground();

		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();

//##	EndDraw(rc.right, rc.bottom, hdc);

//	if (ret) return ::ReleaseDC(GetSafeHwnd(), hdc), 0;


	HDC hMemDC = ::CreateCompatibleDC(hDC);
	ASSERT(hMemDC != 0);
	if (!hMemDC) return ::ReleaseDC(GetSafeHwnd(), hDC), 0;

	HBITMAP hBitmap = DSCreateDIBSection( rc.right-rc.left, rc.bottom-rc.top, 24);
	ASSERT(hBitmap != 0);

	if (hBitmap == 0) return ::ReleaseDC(GetSafeHwnd(),hDC), ::ReleaseDC(GetSafeHwnd(),hMemDC), DeleteDC(hMemDC), 0;
	HBITMAP hBitmapOld = (struct HBITMAP__ *) SelectObject(hMemDC, hBitmap); 

//make a copy of current:
	BitBlt( hMemDC, 0, 0, rc.right, rc.bottom,
			m_hdcMem,
			0, 0, SRCCOPY );

	SelectObject(hMemDC, hBitmapOld); 
	::ReleaseDC(GetSafeHwnd(),hMemDC);
	DeleteDC(hMemDC);
	hMemDC = 0;
	::ReleaseDC(GetSafeHwnd(),hDC);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return hBitmap;
}
///////////////////////////////////////////////////////////////////////////////
long CGLView::RenderBackground()
{
	if (m_hBitmapBackground)
	{
		BITMAP		bm;
		GetObject(m_hBitmapBackground, sizeof(BITMAP), &bm );
//		HDC hDC = GetDC()->GetSafeHdc();
		HDC hDC = m_pDC->GetSafeHdc();
		HDC			hMemDC		= ::CreateCompatibleDC( hDC );
		HBITMAP		hBitmapOld	= (HBITMAP)SelectObject( hMemDC, m_hBitmapBackground );
		BitBlt( m_hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY );
		SelectObject(hMemDC, hBitmapOld);
		::ReleaseDC(GetSafeHwnd(), hMemDC);
		DeleteDC(hMemDC);
	//	::ReleaseDC(GetSafeHwnd(), hDC);
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////// 
void CGLView::LoadTrashCan() 
{
	if(m_hBitmapTrash) return;
	HDC			hMemDC;
	BITMAP		bm;
	m_hBitmapTrash = (HBITMAP)LoadImage(	AfxGetInstanceHandle(),
											MAKEINTRESOURCE(IDB_TRASH),
											IMAGE_BITMAP, 0, 0,
											LR_CREATEDIBSECTION | LR_DEFAULTSIZE
											);
	if(!m_hBitmapTrash) return;

// Get the color depth of the DIBSection
	GetObject(m_hBitmapTrash, sizeof(BITMAP), &bm );
// If the DIBSection is 256 color or less, it has a color table
	if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
	{
		HBITMAP		 hOldBitmap;
		RGBQUAD		 rgb[256];
		LPLOGPALETTE	pLogPal;
		WORD			 i;

// Create a memory DC and select the DIBSection into it
//	HDC hDC = m_pDC->GetSafeHdc();
	HDC hDC = GetDC()->GetSafeHdc();
	hMemDC = CreateCompatibleDC( hDC );
	if (hMemDC==0)  
	{ 
		::MessageBox(NULL, "Cannot CreateCompatibleDC.", L"Error", MB_OK); 
		return; 
	}  
	hOldBitmap = (HBITMAP)SelectObject( hMemDC, m_hBitmapTrash );
// Get the DIBSection's color table
	GetDIBColorTable( hMemDC, 0, 256, rgb );
// Create a palette from the color tabl
	pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	for(i=0;i<256;i++)
	{
		pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}
	m_hPaletteTrash = CreatePalette( pLogPal );
// Clean up
	free( pLogPal );
	SelectObject( hMemDC, hOldBitmap );
	DeleteDC( hMemDC );
	}
}
///////////////////////////////////////////////////////////////////////////////
int CGLView::NurbsRender64(const double data[64][64])//NurbsRender(const long& N, const double *data[])//, GLuint& listBackgroundPotential)
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	try {
	const double &x1 = glFrustum_left;
	const double &x2 = glFrustum_right;
	const double &y1 = glFrustum_bottom;
	const double &y2 = glFrustum_top;
	const long N	= 64;
	typedef double (*MATRIX_64)[64];
	typedef double (*MATRIX_65)[65];

	long x, y;
	const long S_NUMPOINTS	= 64;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= 64;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];
	static GLfloat	ctlpoints[64][64][3];//for potential
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	}

	double X=0, Y=0;

	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);

	for (x=0;x<N;x++)
	{
		X = x1 + x*dx;
		for (y=0;y<N;y++)
		{
			Y = y1 + y*dy;
			ctlpoints[x][y][0] = float(X);
			ctlpoints[x][y][1] = float(Y);
			ctlpoints[x][y][2] = float(((MATRIX_64)data)[x][y]);//m_fScalePotential*
		}
	}
///////////////////////////////////////////////////////////////////////////////
//	HDC hDC = GetDC()->GetSafeHdc();//m_pDC->GetSafeHdc();//::GetDC(m_hWnd);
	//wglMakeCurrent(hDC, m_hRC);
	if (listBackground)		glDeleteLists(listBackground, 1);
	listBackground = 0;
	if(listBackground == 0) listBackground = glGenLists(1);

	glNewList(listBackground, GL_COMPILE);
	glPushMatrix();
//	glDisable(GL_LIGHT0);
//	glDisable(GL_LIGHTING);
//	glEnable(GL_TEXTURE_2D);

	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
		ASSERT(0);
		return -1;
	}
	{
	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);

	gluBeginSurface(theNurbs);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_4);


//	gluBeginTrim (theNurbs);
//	gluPwlCurve (theNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (theNurbs);

	gluEndSurface(theNurbs);
	gluDeleteNurbsRenderer(theNurbs);
	}


//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
		ASSERT(0);
		return -3;
	}
///////////////////////////////////////////////////////////////////////////////
	glPopMatrix();
	glEndList();

	BOOL bMutexAcquired = FALSE;
	m_bNewBackgroundTextureDefined	= false;
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
#if 0
template <long N>
int CGLView::NurbsRender<N>(const double *data[],
	const double &x1,
	const double &x2,
	const double &y1,
	const double &y2
							)
{
/*
	const double &x1 = glFrustum_left;
	const double &x2 = glFrustum_right;
	const double &y1 = glFrustum_bottom;
	const double &y2 = glFrustum_top;
*/
	typedef double (*MATRIX_N)[N];

	long x, y;
	const long S_NUMPOINTS	= N;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= N;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];
	static GLfloat	ctlpoints[N][N][3];
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	}

	double X=0, Y=0;

	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);

	for (x=0;x<N;x++)
	{
		X = x1 + x*dx;
		for (y=0;y<N;y++)
		{
			Y = y1 + y*dy;
			ctlpoints[x][y][0] = float(X);
			ctlpoints[x][y][1] = float(Y);
			ctlpoints[x][y][2] = float(m_fScaleNurbs*((MATRIX_N)data)[x][y]);
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (listBackground)		glDeleteLists(listBackground, 1);
	listBackground = 0;
	if(listBackground == 0) listBackground = glGenLists(1);

	glNewList(listBackground, GL_COMPILE);
	glPushMatrix();
//	glDisable(GL_LIGHT0);
//	glDisable(GL_LIGHTING);
//	glEnable(GL_TEXTURE_2D);

	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
		return -1;
	}
	{
	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);

	gluBeginSurface(theNurbs);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_4);


//	gluBeginTrim (theNurbs);
//	gluPwlCurve (theNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (theNurbs);

	gluEndSurface(theNurbs);
	gluDeleteNurbsRenderer(theNurbs);
	}
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
		return -3;
	}
///////////////////////////////////////////////////////////////////////////////
	glPopMatrix();
	glEndList();
	bTextureDefined		= true;
	m_bNewBackgroundTextureDefined	= false;
	return 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CGLView::NurbsRender64(const double X[64], const double Y[64], const double data[64][64], const double data2[64][64], const GLfloat edgePt[64][2])
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	try {
	const double &x1 = glFrustum_left;
	const double &x2 = glFrustum_right;
	const double &y1 = glFrustum_bottom;
	const double &y2 = glFrustum_top;
	const long N	= 64;
	typedef double (*MATRIX_64)[64];
	typedef double (*MATRIX_65)[65];

	long x, y;
	const long S_NUMPOINTS	= N;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= N;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];
	static GLfloat	ctlpoints[N][N][3];//for potential
	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	}

	for (x=0;x<N;x++)
	{
		for (y=0;y<N;y++)
		{
			ctlpoints[x][y][0] = (const float)(X[x]);
			ctlpoints[x][y][1] = (const float)(Y[y]);
			ctlpoints[x][y][2] = float(((MATRIX_64)data)[x][y]);//m_fScalePotential*
		}
	}
///////////////////////////////////////////////////////////////////////////////
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);
	if (listBackground)		glDeleteLists(listBackground, 1);
	listBackground = 0;
	if(listBackground == 0) listBackground = glGenLists(1);

	glNewList(listBackground, GL_COMPILE);
	glPushMatrix();
	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
		ASSERT(0);
		return -1;
	}
	{
	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	const float f=0.50f;

	glColor3f(f,f,f);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	const GLfloat glfMatSpecular[]= {0.600f, 0.600f, 0.600f, 1.0f};
	const GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
	const GLfloat fShininess = 40.0f;//128.000f;

//	glMaterialfv(GL_FRONT, GL_AMBIENT,	glfMatAmbient);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE,	glfMatDiffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glfMatSpecular);
//	glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
/*
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,	glfMatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,	glfMatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glfMatEmission);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
*/
//	glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, fShininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	{
	const GLfloat ambient[] = {0.2f,0.2f,0.2f,1.0f };
	const GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f };
	const GLfloat position[] = {-0.4f,-44.0f,0.0f,0.0f };

// Activate lighting and a light source
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glEnable(GL_LIGHT1);
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
///////////////////////////////////////////////////////////////////////////////
	{
//	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 50.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);
//	gluNurbsProperty(theNurbs, GLU_CULLING , GL_TRUE);

	gluBeginSurface(theNurbs);

	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);


/*
//	 gluNurbsCurve (theNurbs, S_NUMKNOTS, sknots, 3, (float*)&edgePt[0][0], 4, GLU_MAP1_TRIM_2);
*/
	gluBeginTrim (theNurbs);
	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	gluPwlCurve (theNurbs, N, (float*)&edgePt[0][0], 2, GLU_MAP1_TRIM_2);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	gluEndTrim (theNurbs);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	gluEndSurface(theNurbs);

	
  	for (x=0;x<N;x++)
	{
		for (y=0;y<N;y++)
		{
//			ctlpoints[x][y][0] = (const float)(X[x]);
//			ctlpoints[x][y][1] = (const float)(Y[y]);
			ctlpoints[x][y][2] = float(((MATRIX_64)data2)[x][y]);//m_fScalePotential*
		}
	}

	gluBeginSurface(theNurbs);

	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);


/*
	gluBeginTrim (theNurbs);
	gluPwlCurve (theNurbs, N, (float*)&edgePt[0][0], 2, GLU_MAP1_TRIM_2);
	gluEndTrim (theNurbs);
*/

	gluEndSurface(theNurbs);
 
	}
///////////////////////////////////////////////////////////////////////////////
	glPopAttrib();


/*

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);


  gluBeginSurface(theNurbs);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);


	gluNurbsSurface(theNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_4);


//	gluBeginTrim (theNurbs);
//	gluPwlCurve (theNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (theNurbs);

	gluEndSurface(theNurbs);

*/
	glFlush();

	gluDeleteNurbsRenderer(theNurbs);
	}
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
		ASSERT(0);
		return -3;
	}
///////////////////////////////////////////////////////////////////////////////
	glPopMatrix();
	glEndList();

	BOOL bMutexAcquired = FALSE;
	m_bNewBackgroundTextureDefined	= false;
	//wglMakeCurrent(0, 0);
	//:://ReleaseDC(m_hWnd, hDC);
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return 0;
}

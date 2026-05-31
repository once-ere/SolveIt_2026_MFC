// CalcMap.cpp : implementation file
//

#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
/*
NTL.lib 

#define NTL_NO_MIN_MAX 1
#include <NTL/RR.h>
#include <NTL/version.h>
NTL_CLIENT
*/
#include "SolveIt.h"
#include "System.h"
#include "CalcMap.h"
#include "GLView.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "Cruncher.h"
#include "MainFrm.h"
#include "LeftView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
///////////////////////////////////////////////////////////////////////////////
//Improve floating-point consistency
#pragma optimize( "p", on )

///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz);
///////////////////////////////////////////////////////////////////////////////
#define RENDER_MODE_FPU_ASM	 1
//int w_=0; // Window width
//double ax_ = -2.0;
//double sx_=0;
/////////////////////////////////////////////////////////////////////////////
// CCalcMap

IMPLEMENT_DYNCREATE(CCalcMap, CWinThread)

CCalcMap::CCalcMap():
//	ITER_BLACK(0xFFFFFFFF),
	pixBuf_(0),
	glArrColor_(0),
	glArrVertex_(0),
	colorTable_(0),
	maxi_(40),
	ax_(-2.0),
	ay_(-1.5),
	ex_(1.0), 
	ey_(1.5), 
	w_(0),
	sx_(0),
	sy_(0),
//	four(4.0),
	doublebuf_(true),
	useGLArrays_(true),
	reshaped_(true)
{
	// Initialize colortable
//	prepareColorTable(10000, 70, 50, 200);
}

CCalcMap::~CCalcMap()
{
	if (glArrColor_ != NULL)	delete[] glArrColor_;//free(glArrColor_);
	if (glArrVertex_ != NULL)	delete[] glArrVertex_;//free(glArrVertex_);
	if (pixBuf_)				delete pixBuf_;
	if (colorTable_)			delete[] colorTable_;
}
///////////////////////////////////////////////////////////////////////////////
//const unsigned wchar_t *glVersion_;
//const GLubyte * glVersion_;

BOOL CCalcMap::InitInstance()
{
	CGLView* pGLView = theApp.GetGLView();
#if 0
	HDC hDC = ::GetDC(pGLView->m_hWnd);
	//wglMakeCurrent(hDC, pGLView->m_hRC);
	const GLubyte * glVersion_ = glGetString(GL_VERSION); //0x1F02GL_VERSION Get OpenGL Version
	//wglMakeCurrent(0, 0);
	::ReleaseDC(pGLView->m_hWnd, hDC);
	if (glVersion_)
	{
		useGLArrays_ = glVersion_[2] > '0';
	}
	else
	{
		ASSERT(0);
		useGLArrays_=false;
//		AfxMessageBox(L"error GL_VERSION.\n");=> death
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
		pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) GetWC("error GL_VERSION."));
	}
//	if (!useGLArrays_)
//	{
//		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
//		pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) GetWC("*WARNING* OpenGL arrays not available. Render will be slower."));
//	}
#endif
	w_	= pGLView->viewWidth;
	h_	= pGLView->viewHeight;

//	double z_Pos = 0;//z == 0 => z == m_fNearPlane
	double x1 = pGLView->glFrustum_left;
	double x2 = pGLView->glFrustum_right;
	double y1 = pGLView->glFrustum_bottom;
	double y2 = pGLView->glFrustum_top;
//		pGLView->ComputeScreenCoords(&x1, &y1, &z_Pos);
//		z_Pos = 0;
//		pGLView->ComputeScreenCoords(&x2, &y2, &z_Pos);
	ax_ = x1;
	ex_ = x2;
	ay_ = y1;
	ey_ = y2;
	// Recalc real plane parameters
	sx_ = (ex_ - ax_) / ((double) w_);
	sy_ = (ey_ - ay_) / ((double) h_);
//	sy_ = sx_;
//	OnStep(0, 0);
//	pGLView->SetupImage();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
int CCalcMap::ExitInstance()
{
//	ASSERT(0);
	if (glArrColor_ != NULL)	delete[] glArrColor_;//free(glArrColor_);
	if (glArrVertex_ != NULL)	delete[] glArrVertex_;//free(glArrVertex_);
	if (pixBuf_)				delete pixBuf_;
	if (colorTable_)			delete[] colorTable_;
//	if (pixBuf_)		delete(pixBuf_);
//	if (colorTable_ != NULL) free(colorTable_);
//	if (glArrColor_ != NULL) free(glArrColor_);
//	if (glArrVertex_ != NULL) free(glArrVertex_);
	pixBuf_			= 0;
	glArrColor_		= 0;
	glArrVertex_	= 0;
	colorTable_		= 0;

	return CWinThread::ExitInstance();
}
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCalcMap, CWinThread)
	//{{AFX_MSG_MAP(CCalcMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CCalcMap::OnStep(WPARAM wParam, LPARAM lParam)
{
try {
//	int row		= wParam;
	unsigned int maxi = static_cast<unsigned int>(lParam);
//	bool ret	=	calcPixelRow_C( row,  maxi);
	int endRow = static_cast<int>(wParam);
	for (int y=0; y<endRow; y++)
	{
//		if (!pixBuf_->isRowCalculated(y))
			calcPixelRow_C(y, maxi);
	}
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)// there was an error
	{
		PostQuitMessage(0);
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return ;
}
///////////////////////////////////////////////////////////////////////////////
void CCalcMap::OnSetParam(WPARAM wParam, LPARAM lParam)
{
	try
	{
//		w_	=w0_;
//		ax_ = ax0_;
//		sx_ = sx0_ ;
//		Fire_ViewChange();
//		return 0;
	maxi_+=40;
	CGLView* pGLView = theApp.GetGLView();
	int _w=w_,_h=h_;
	w_	= pGLView->viewWidth;
	h_	= pGLView->viewHeight;
	if (_w!=w_ || _h!=h_) reshaped_=true;

//	double z_Pos = 0;//z == 0 => z == m_fNearPlane
	double x1 = pGLView->glFrustum_left;
	double x2 = pGLView->glFrustum_right;
	double y1 = pGLView->glFrustum_bottom;
	double y2 = pGLView->glFrustum_top;
//		pGLView->ComputeScreenCoords(&x1, &y1, &z_Pos);
//		z_Pos = 0;
//		pGLView->ComputeScreenCoords(&x2, &y2, &z_Pos);
	ax_ = x1;
	ex_ = x2;
	ay_ = y1;
	ey_ = y2;
	// Recalc real plane parameters
//	sx_ = (ex_ - ax_) / ((double) w_);
//	sy_ = (ey_ - ay_) / ((double) h_);
	sx_  = pGLView->size_x_;// = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
	sy_	 = pGLView->size_y_;// = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);

	switch (int(wParam))
	{
		case 'p':
			break;
		case 'f'://faster
			break;
		case 's'://slower
			break;
		case 'u':
		{
		}
			break;
		case 'd':
		{
		}
			break;
			
	}
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"OnSetParam", MB_OK);
		PostQuitMessage(0);
	}
	return ;
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Calculate the given pixel row using C mode
 * @return true if row calc was ok, false otherwise
 */

bool CCalcMap::calcPixelRow_C(int row, unsigned int maxi)
{
//	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
///////////////////////////////////////////////////////////////////////////////
	CGLView* pGLView = theApp.GetGLView();
//	unsigned int* rowBuffer = 0;
	unsigned int rowBuffer[2048];
	unsigned int width = 0;
	{
		Lock lock (theApp.protectMapper);
	//	unsigned int* rowBuffer_ = pGLView->pixBuf_->getRowPointer(row);
	//	if ( rowBuffer_ == NULL) throw SolveIt_ERROR(L"-2");
		width = pGLView->pixBuf_->getWidth();
	//	CopyMemory(rowBuffer, rowBuffer_, sizeof(unsigned int)*width);
	}
	if ( width <= 0) throw SolveIt_ERROR(L"1");
	if ( width > 2048) throw SolveIt_ERROR(L"-1");
//	rowBuffer = new unsigned int[width];
	if ( rowBuffer == NULL) throw SolveIt_ERROR(L"2");
	ZeroMemory(rowBuffer, sizeof(unsigned int)*width);
//	h_ = pGLView->viewHeight;
//	ax_ = pGLView->glFrustum_left;
//	ex_ = pGLView->glFrustum_right;
//	ay_ = pGLView->glFrustum_bottom;
//	ey_ = pGLView->glFrustum_top;
//	sx_ = (ex_ - ax_) / ((double) w_);
//	sy_ = (ey_ - ay_) / ((double) h_);
	double cx = ax_;
	double cy = ay_ + sy_*((double)row);
// Calc Pixel
#if 10
	double zx, zy;
	double zx2, zy2;
	unsigned int i = 0;
	for (unsigned int x = 0; x < width; x++) {
		zx = cx;
		zy = cy;
		for (i=0; i<maxi; i++) {
			zx2 = zx*zx;
			zy2 = zy*zy;
			if ((zx2 + zy2) > 4) break;
			zy = 2*zx*zy;
			zx = zx2 - zy2;
			zx += cx;
			zy += cy;
		}
		cx += sx_;
		if (i == maxi)
		{
		//	*rowBuffer = ITER_BLACK;
			rowBuffer[x] = ITER_BLACK;
		}
		else
		{
	//		*rowBuffer = i;
			rowBuffer[x] = i;
		}
	//		rowBuffer++;
	}

#endif
#if 0
	RR ax__ , ex__, ay__, ey__, zx, zy, zx2, zy2, Four,Two,sx__,sy__,cx__,cy__;
	Four	= 4.0;
	Two		= 2.0;
	ax__ = ax_;
	ex__ = ex_;
	ay__ = ay_;
	ey__ = ey_;
	sx__ = sx_;//(ex__ - ax__) / ((double) pGLView->viewWidth);
	sy__ = sy_;//(ey__ - ay__) / ((double) pGLView->viewHeight);

	cx__ = cx;//ax__;
	cy__ = cy;//ay__ + sy__*((double)row);

	RR::SetPrecision(400);
	for (int x=0; x<width; x++) {
		zx = cx__;
		zy = cy__;
		for (unsigned int i=0; i<maxi; i++) {
			zx2 = zx*zx;
			zy2 = zy*zy;
			if ((zx2 + zy2) > Four) break;
			zy = Two*zx*zy;
			zx = zx2 - zy2;
			zx += cx__;
			zy += cy__;
		}
		cx__ += sx__;
		if (i == maxi)
		{
		//	*rowBuffer = ITER_BLACK;
			rowBuffer[x] = ITER_BLACK;
		}
		else
		{
	//		*rowBuffer = i;
			rowBuffer[x] = i;
		}
	//		rowBuffer++;
	}
#endif
//		unsigned int a =0;
	{
		Lock lock (theApp.protectMapper);
		if (width != pGLView->pixBuf_->getWidth()) throw SolveIt_ERROR(L"-5");
		unsigned int* rowBuffer_ = pGLView->pixBuf_->getRowPointer(row);
		if ( rowBuffer_ == NULL) throw SolveIt_ERROR(L"-4");
	//	for (x=0; x < w_; ++x)
		{
			try{
		//	a = rowBuffer[x];
		//	*(rowBuffer_ + x) = a;
		//	rowBuffer_[x] = rowBuffer[x];
			}catch( SolveIt_Error& e){e.why();}catch(...)
			{
				ASSERT(0);
			}
		}
		CopyMemory(rowBuffer_, rowBuffer, sizeof(unsigned int)*width);
		pGLView->pixBuf_->setRowCalculated(row, true);
		pGLView->Invalidate();
	}
//	delete[]rowBuffer;
	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**
 * Calculate the given pixel row using the given mode
 * @return true if row calc was ok, false otherwise
bool CGLView::calcPixelRow(int row, unsigned int maxi, int mode)
{
	if(pixBuf_ == 0) return false;

//	pixBuf_->setRowCalculated(row, false);
	bool retcode = false;
//	retcode = calcPixelRow_C(row, maxi);
	if (0&& m_ZoomLevel>1)
	{
		CSystem* m_system = theApp.GetSystem();;
		m_system->mapper->PostThreadMessage(WM_USER_START_PASS, row, maxi);
		return true;
	//////	return calcPixelRow_C( row, maxi);//
	}
//	retcode = calcPixelRow_FPU_ASM(row, maxi);
	retcode = calcPixelRow_FPU_ASM(row, maxi, glFrustum_left, glFrustum_bottom + size_y_*((double)row), viewWidth, size_x_);
	if (retcode) pixBuf_->setRowCalculated(row, true);
//	// TODO: Row agent not yet supported
	return retcode;
}
 */
///////////////////////////////////////////////////////////////////////////////
//	double &ax_ = glFrustum_left;
//	double &ex_ = glFrustum_right;
//	double &ay_ = glFrustum_bottom;
//	double &ey_ = glFrustum_top;
// Recalc real plane parameters
//	double sx_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
//	double _cx = ax_;
//	double _cy = glFrustum_bottom + sy_*((double)row);
bool CGLView::calcPixelRow(int row, unsigned int maxi, double _cx, double _cy, int w_, double sx_)
{
	const double two = 2.0;
	const double four = 4.0;
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);

#ifdef _WIN64
	double cx = _cx;
	double cy = _cy;
	double zx;
	double zy;
	double zx2;
	double zy2;
	for (int x = 0; x < w_; ++x)
	{
		unsigned int i = 0;
		zx = cx;
		zy = cy;
		for (i = 0; i < maxi; ++i)
		{
			zx2 = zx * zx;
			zy2 = zy * zy;
			if ((zx2 + zy2) > four)
			{
				break;
			}
			zy = two * zx * zy;
			zx = zx2 - zy2;
			zx += cx;
			zy += cy;
		}
		*rowBuffer++ = (i == maxi) ? ITER_BLACK : i;
		cx += sx_;
	}
	return true;
#else
	
	_asm {

	finit

	mov	edi,maxi		// edi = maxiters
	mov	edx,w_
	mov esi, rowBuffer;

	fld	four			// STACK: 4
	fld	_cx				// STACK: cx 4
	fld	_cy				// STACK: cy cx 4


NextPixel:
	fld st(1)				// STACK: x cy cx 4
	fld st(1)				// STACK: y x cy cx 4
	xor ecx,ecx				// First iter

iterloop:
	fld st(1)				// STACK: x   y x cy cx 4
	fmul st,st				// STACK: x^2 y x cy cx 4
	fld st(1)				// STACK: y x^2 y x cy cx 4
	fmul st,st				// STACK: y^2 x^2 y x cy cx 4
	faddp st(1),st(0)		// STACK: y^2+x^2 y x cy cx 4


	fcomp st(5)				// STACK: y x cy cx 4
	fstsw ax
	test	ax, 4500H		// Check for  x^2+y^2 > 4
	jz	donepix


	fld st(1)				// STACK: x y x cy cx 4
	fld st(1)				// STACK: y x y x cy cx 4
	faddp st(1),st(0)		// STACK: y+x y x cy cx 4
	fld st(2)				// STACK: x y+x y x cy cx 4 
	fld st(2)				// STACK: y x y+x y x cy cx 4 
	fsubp st(1),st(0)		// STACK: x-y y+x y x cy cx 4

	fmulp st(1),st(0)		// STACK: x^2-y^2  y x cy cx 4

	fld st(4)				// STACK: cx x^2-y^2 y x cy cx 4
	faddp st(1),st(0)		// STACK: cx+x^2-y^2 y x cy cx 4
	fxch st(2)				// STACK: x  y cx+x^2-y^2 cy cx 4
	fmulp st(1),st(0)		// STACK: xy cx+x^2-y^2 cy cx 4 
	fld two					// STACK: 2 xy cx+x^2-y^2 cy cx 4 
	fmulp st(1),st(0)		// STACK: 2xy cx+x^2-y^2 cy cx 4 
	fld st(2)				// STACK: cy 2xy cx+x^2-y^2 cy cx 4
	faddp st(1),st(0)		// STACK: cy+2xy cx+x^2-y^2 cy cx 4


/*
	fxch st(2)				// STACK: x  y x^2-y^2 cy cx 4
	fmulp st(1),st(0)		// STACK: xy x^2-y^2 cy cx 4 
	fld two					// STACK: 2 xy x^2-y^2 cy cx 4 
	fmulp st(1),st(0)		// STACK: 2xy x^2-y^2 cy cx 4 
	fld st(2)				// STACK: cy 2xy x^2-y^2  cy cx 4
	faddp st(1),st(0)		// STACK: cy+2xy x^2-y^2  cy cx 4
	fxch st(1)				// STACK: x^2-y^2 cy+2xy  cy cx 4
	fld st(3)				// STACK: cx x^2-y^2 cy+2xy  cy cx 4
	faddp st(1),st(0)		// STACK: cx+x^2-y^2 cy+2xy  cy cx 4
	fxch st(1)				// STACK: cy+2xy cx+x^2-y^2 cy cx 4
*/

	cmp ecx,edi		 // Check for maxiters
	jz donepix2

	inc ecx
	jmp iterloop


donepix:
//	fstp	st
donepix2:
	fstp	st
	fstp	st			// STACK:  cy cx 
	cmp ecx,edi			// Check for maxiters
	jnz	notblack
	mov ecx,ITER_BLACK
notblack:
	mov	dword ptr [esi],ecx	;SET PIXEL
//next x
	fld	sx_				// STACK:  dx cy cx 
	faddp	st(2),st	// STACK:  cy cx+dx 
	add	esi,4		 ;Change to next pixBuf element

	dec	edx
	jnz	NextPixel
	}

	return true;
#endif
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	double &ax_ = glFrustum_left;
//	double &ex_ = glFrustum_right;
//	double &ay_ = glFrustum_bottom;
//	double &ey_ = glFrustum_top;
// Recalc real plane parameters
//	double sx_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
//	double _cx = ax_;
//	double _cy = glFrustum_bottom + sy_*((double)row);
bool CGLView::calcPixelRow_FPU_ASM(int row, unsigned int maxi, double ax_, double _cy, int w_, double sx_)
{
	const double four = 4.0;
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);

#ifdef _WIN64
	return calcPixelRow(row, maxi, ax_, _cy, w_, sx_);
#else
	
	_asm {

	finit

	mov esi, rowBuffer;

	fld	four				// STACK: 4
	fld	ax_				 // STACK: ax 4
	fld	_cy				 // STACK: ay ax 4

	mov	edi,maxi		// edi = maxiters

	//mov	si,600
	mov	edx,w_

nxtpix:
	fld st(1)				// STACK: cx ay ax 4
	fld st(1)				// STACK: cy cx ay ax 4

	xor ecx,ecx		 // First iter

iterloop:
	fld st(1)				// STACK: zx cy cx ay ax 4
	fmul st,st			// STACK: zy*zy zx cy cx ay ax 4
	fld st(1)				// STACK: zy zx cy cx ay ax 4
	fmul st,st			// STACK: zx*zx zy*zy zx cy cx ay ax 4
	fxch st(1)			// STACK: zx zy*zy zx cy cx ay ax 4
	fld st					// STACK: zx*zx zx*zx zy*zy zx cy cx ay ax 4
	fadd st,st(2)	 // STACK: zx*zx+zy*zy zx*zx zy*zy zx cy cx ay ax 4

	fcomp st(7)			// STACK: zx*zx zy*zy zx cy cx ay ax 4
	fnstsw ax
	sahf						// Check for modulo^2 > 4
	fsubrp st(1),st // STACK: zx*zx-zy*zy zx cy cx ay ax 4
	jnc	donepix
	fadd st,st(4)	 // STACK: zx*zx-zy*zy+ay zx cy cx ay ax 4
	cmp ecx,edi		 // Check for maxiters
	fxch st(2)			// STACK: zx*zx-zy*zy+ay zx cy cx ay ax 4
	fadd st,st			// STACK: 2*cy zx zx*zx-zy*zy+ay cx ay ax 4
	jz donepix

	fmulp st(1),st
	inc ecx
	fadd st,st(2)
	jmp iterloop


donepix:
	fstp	st
	fstp	st
	fstp	st
	cmp ecx,edi		 // Check for maxiters
	fld	sx_
	jnz	notblack
	mov ecx,ITER_BLACK
notblack:
	mov	dword ptr [esi],ecx	;SET PIXEL !
	faddp	st(2),st
	add	esi,4		 ;Change to next pixBuf element

	dec	edx
	jnz	NxtPix
	}

	return true;
#endif
}
///////////////////////////////////////////////////////////////////////////////
bool CGLView::calcPixelRows(unsigned int maxi)
{
//	if (m_bReCalc)
	for (int y=0; y<viewHeight; y++) pixBuf_->setRowCalculated(y, false);
//	m_bReCalc = false;
//The master thread (this) calcs+render his slice
	if (0&& m_ZoomLevel>18)
	{
		CSystem* m_system = theApp.GetSystem();;
		m_system->mapper->PostThreadMessage(WM_USER_START_PASS, viewHeight, maxi);
	}
	else
	for (int row=0; row<viewHeight; row++)
	{
//#define RENDER_MODE_FPU_ASM	 1
	//	if (!pixBuf_->isRowCalculated(y))
	//		calcPixelRow(y, maxi, RENDER_MODE_FPU_ASM);
	//	renderPixelRow(y, mode);
		double y = glFrustum_bottom + size_y_*((double)row);
//		calcPixelRow_FPU_ASM(row, maxi, glFrustum_left, y, viewWidth, size_x_);
		calcPixelRow(row, maxi, glFrustum_left, y, viewWidth, size_x_);
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::ComputeMapperCoords(const POINT& point, bool bZoomIn)
{
	try
	{

		CSystem* m_system	= theApp.GetSystem();
		double s = 2*m_system->m_fScale;
		const unsigned int zoomStep = static_cast<unsigned int>(40.0 * (m_system->m_fScale - 1));

		double x = point.x;
		double y = viewHeight-point.y;//point.y;

		double X = glFrustum_left	+ x * (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		double Y = glFrustum_bottom	+ y * (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);

		double w = (glFrustum_right - glFrustum_left);
		double h = (glFrustum_top - glFrustum_bottom);
	//	maxi_ = m_ZoomLevel<30?400:1000;
	if (bZoomIn)
	{
		++m_ZoomLevel;
		maxi_ += zoomStep;
		orig_glFrustum_left=glFrustum_left		= X - w/s;
		orig_glFrustum_right=glFrustum_right	= X + w/s;
		orig_glFrustum_bottom=glFrustum_bottom	= Y - h/s;
		orig_glFrustum_top=glFrustum_top		= Y + h/s;
	}
	else
	{
		--m_ZoomLevel;
		maxi_ -= zoomStep;
		if (maxi_< 333) maxi_ = 333;//
		glFrustum_left	= X - w*s;
		glFrustum_right	= X + w*s;
		glFrustum_bottom= Y - h*s;
		glFrustum_top	= Y + h*s;
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
	}
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
/*
	static double x1 = glFrustum_left;
	static double x2 = glFrustum_right;
	static double y1 = glFrustum_bottom;
	static double y2 = glFrustum_top;
	static double w = (x2 - x1);
	static double h = (y2 - y1);
		RR Four,Two;
		RR x1__ , x2__, y1__, y2__, x__, y__, w__, h__, size_x__,size_y__;
		RR::SetPrecision(40);
		Four	= 4.0;
		Two		= 2.0;


	//	x = ((double) point.x) / ((double) viewWidth);
	//	y = ((double) viewHeight-point.y) / ((double) viewHeight);
		x = point.x;
		y = viewHeight-point.y;//point.y;
	//	y = point.y;

		w = (x2 - x1);
		h = (y2 - y1);

		x1	= x - w/s;
		x2	= x + w/s;
		y1	= y - h/s;
		y2	= y + h/s;

		size_x_ = w / ((double) viewWidth);
		size_y_ = h / ((double) viewHeight);

		double w = (glFrustum_right - glFrustum_left);
		double h = (glFrustum_top - glFrustum_bottom);

		size_x_ = (w) / ((double) viewWidth);
		size_y_ = (h) / ((double) viewHeight);
*/


		size_x_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		size_y_ = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);

		if (m_system->mapper)
			m_system->mapper->PostThreadMessage(WM_USER_SetParam, 0, 0);
		calcPixelRows(maxi_);
/*	
	{
		HDC hDC = ::GetDC(m_hWnd);
		//wglMakeCurrent(hDC, m_hRC);
		glViewport(0, 0, viewWidth, viewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
	}
*/
		wchar_t buf[128];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
		oss	<< "  ( " << X << " , " << Y << " )  " ;
		oss	<< ends;
		wstring str(buf);
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		pFrame->SetStatusBarText(str, 2);

		oss.seekp(0);
		oss	<< m_ZoomLevel << ends;
		str	= buf;
		pFrame->SetStatusBarText(str, 3);
		Invalidate();
/*
//		register double cx = x1;
//		register double cy = y1;// + size_y_*((double)row);
		x__ = x;
		y__ = y;

		x1__ = x1;
		x2__ = x2;
		y1__ = y1;
		y2__ = y2;
		size_x__ = size_x_;
		size_y__ = size_y_;

		w__			=	(x2__	-	x1__);
		h__			=	(y2__	-	y1__);
		x1__		=	x__	-	w__/s;
		x2__		=	x__	+	w__/s;
		y1__		=	y__	-	h__/s;
		y2__		=	y__	+	h__/s;
		size_x__	=	w__	/	((double)	viewWidth);
		size_y__	=	h__	/	((double)	viewHeight);

*/

	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::ComputeMapperCoords Error: ", MB_OK);
				throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::ComputeMapperCoords", MB_OK);
				throw SolveIt_ERROR(L"?????????");
	}
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::SetupImage()
{
	try
	{
	if (viewHeight == 0) return;
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
/*
"An optimum compromise that allows all primitives to be specified at integer positions, 
while still ensuring predictable rasterization, is to translate x and y by 0.375, as 
shown in the following code sample. Such a translation keeps polygon and pixel image 
edges safely away from the centers of pixels, while moving line vertices close enough 
to the pixel centers. "
*/
glTranslated(0.375, 0.375, 0.0);
/* render all primitives at integer positions */ 
	//	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
	//	glGetDoublev(GL_PROJECTION_MATRIX , m_projMatrix);
	//	glGetIntegerv(GL_VIEWPORT , m_viewport);
		GLenum err = glGetError();
	//	ASSERT(err == GL_NO_ERROR);
		//wglMakeCurrent(0, 0);
		//:://ReleaseDC(m_hWnd, hDC);
		size_x_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		size_y_ = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);


		if (theApp.GetSystem()->m_bDoVerhulstDynamics)
		{
			int j=0;
	//		return;
		}

		if (colorTable_ == NULL)
		{
			prepareColorTable(10000, 70, 50, 200);
		}

	{
		if (reshaped_)
		{
			Lock lock (theApp.protectMapper);
			reshaped_ = false;
			if (glArrColor_ != NULL) delete[] glArrColor_;//free(glArrColor_);
			if (glArrVertex_ != NULL) delete[] glArrVertex_;//free(glArrVertex_);
			if(pixBuf_ != NULL) delete pixBuf_;
			glArrColor_ = new GLubyte[viewWidth*3];//(GLubyte*) realloc(glArrColor_, viewWidth*3*sizeof(GLubyte));
			if (glArrColor_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating row color buffer");
			}
			glArrVertex_ = new GLfloat[viewWidth*2];//(GLfloat*) realloc(glArrVertex_, viewWidth*2*sizeof(GLfloat));
			if (glArrVertex_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating vertex buffer");
			}
			pixBuf_ = new PixelBuffer(viewWidth, viewHeight);
			if (pixBuf_ == NULL)
			{
				throw SolveIt_ERROR(L"ERROR: allocating PixelBuffer ");
			}
		}
	}


		if (theApp.GetSystem()->m_bDoVerhulstDynamics)
		{
			return;
		}
		else
		{
			calcPixelRows(maxi_);
		}
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::SetupImage Error: ", MB_OK);
				throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::SetupImage", MB_OK);
				throw SolveIt_ERROR(L"?????????");
	}
}
///////////////////////////////////////////////////////////////////////////////
bool CGLView::renderImage(unsigned int maxi, int mode)
{
	try
	{
	if (viewHeight == 0) return true;
	if (colorTable_ == NULL)
	prepareColorTable(10000, 70, 50, 200);

	glClear(GL_COLOR_BUFFER_BIT);
//	SetCursor(LoadCursor(NULL, IDC_WAIT));
	::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));
{
		if (reshaped_) {
			Lock lock (theApp.protectMapper);
			reshaped_ = false;
			if (glArrColor_ != NULL) delete[] glArrColor_;//free(glArrColor_);
			if (glArrVertex_ != NULL) delete[] glArrVertex_;//free(glArrVertex_);
			if(pixBuf_ != NULL) delete pixBuf_;
			glArrColor_ = new GLubyte[viewWidth*3];//(GLubyte*) realloc(glArrColor_, viewWidth*3*sizeof(GLubyte));
			if (glArrColor_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating row color buffer");
			}
			glArrVertex_ = new GLfloat[viewWidth*2];//(GLfloat*) realloc(glArrVertex_, viewWidth*2*sizeof(GLfloat));
			if (glArrVertex_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating vertex buffer");
			}
			pixBuf_ = new PixelBuffer(viewWidth, viewHeight);
			if (pixBuf_ == NULL)
			{
				throw SolveIt_ERROR(L"ERROR: allocating PixelBuffer ");
			}
		}
	}
	int y = 0;
	bool complete = false;
// After calculating his slice, the master thread renders the slaves' ones
//	while (!complete)
	{
		Lock lock (theApp.protectMapper);
		complete = true;
		for (y=0; y<viewHeight; y++) {
			renderPixelRow(y, mode);
		//	if (pixBuf_->isRowCalculated(y)) renderPixelRow(y, mode);
		//	else complete = false;
		}
	}


//	glFlush();
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::renderImage Error: ", MB_OK);
				throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::renderImage", MB_OK);
				throw SolveIt_ERROR(L"?????????");
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return true;
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Render the given pixel row for the given mode
 * @return true if row calc was ok, false otherwise
 */
bool CGLView::renderPixelRow(int row, int mode)
{
	try
	{
	if (pixBuf_ == NULL) return false;

//	Lock lock (theApp.protectMapper);
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
	if (rowBuffer == NULL) return false;
	unsigned int i;
//	double z_Pos = 0;//z == 0 => z == m_fNearPlane
const	double &ax_ = glFrustum_left;
//	double &ex_ = glFrustum_right;
const	double &ay_ = glFrustum_bottom;
//	double &ey_ = glFrustum_top;
// Recalc real plane parameters

	if (useGLArrays_) {
//** enable arrays client state 
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLubyte* arrC = glArrColor_;
		GLfloat* arrV = glArrVertex_;
	//	GLfloat frow = (GLfloat) (ay_ + size_y_*((double)row));//row;
	//	GLfloat frow = (GLfloat) viewHeight-row;
		GLfloat frow = (GLfloat) row;
		for (int x=0; x < viewWidth; ++x)
		{
			i = rowBuffer[x];//*rowBuffer++;
			if (i == ITER_BLACK)
			{
				*arrC++ = (GLubyte) 0;
				*arrC++ = (GLubyte) 0;
				*arrC++ = (GLubyte) 0;
			}
			else
			if (i == ITER_WHITE)
			{
				*arrC++ = (GLubyte) 255;
				*arrC++ = (GLubyte) 255;
				*arrC++ = (GLubyte) 255;
			}
			else
			{
				ASSERT(i < 10000);
				*arrC++ = colorTable_[i*3];
				*arrC++ = colorTable_[i*3+1];
				*arrC++ = colorTable_[i*3+2];
			}
		//	*arrV++ = (GLfloat)(ax_+x*size_x_);
			*arrV++ = (GLfloat)(x);
			*arrV++ = frow;
		//	rowBuffer++;
		}
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, glArrColor_);
		glVertexPointer(2, GL_FLOAT, 0, glArrVertex_);
		glDrawArrays(GL_POINTS, 0, viewWidth);
//** disable arrays client state 
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else {
//		GLfloat frow = (GLfloat) (ay_ + size_y_*((double)row));//row;
		glBegin(GL_POINTS);
		for (int x=0; x<viewWidth; x++) {
			//_sntprintf(buf, count, "%d\n", i);
			i = *rowBuffer;
			if (i == ITER_BLACK) {
				glColor3d(0.0f, 0.0f, 0.0f);
			}
			else {
				glColor3ub(colorTable_[i*3], colorTable_[i*3+1], colorTable_[i*3+2]);
			}
			glVertex2i(x, row);
		//	glVertex2f((GLfloat)(ax_+x*size_x_), frow);
			rowBuffer++;
		}
		glEnd();
	}
	glFlush();
	glFinish();
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::renderPixelRow Error: ", MB_OK);
//		throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::renderPixelRow", MB_OK);
//		throw SolveIt_ERROR(L"?????????");
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::prepareColorTable(int numColors, GLubyte startR, GLubyte startG, GLubyte startB) {
	// Allocate colortable memory
//	if (colorTable_ != NULL) free(colorTable_);
	if (colorTable_ != NULL) delete[] colorTable_;
//	colorTable_ = (GLubyte*) malloc(numColors*3*sizeof(GLubyte));
	colorTable_ = new GLubyte[numColors*3];
	if (colorTable_ == NULL) return;

	// Fill colortable
	int addR = 3;
	int addG = 4;
	int addB = -2;

	GLubyte r = startR;
	GLubyte g = startG;
	GLubyte b = startB;

	GLubyte* curCol = colorTable_;

	for (int i=0; i<numColors; i++) {
		//printf("Il colore e' R=%d\tG=%d\tB=%d\n", r, g, b);
		*curCol++ = r;
		*curCol++ = g;
		*curCol++ = b;
		r += addR;
		g += addG;
		b += addB;
		if (r < 0) {
			r = -r;
			addR = -addR;
		}
		else if (r > 240) {
			addR = -addR;
			r += addR;
		}
		if (g < 0) {
			g = -g;
			addG = -addG;
		}
		else if (g > 240) {
			addG = -addG;
			g += addG;
		}
		if (b < 0) {
			b = -b;
			addB = -addB;
		}
		else if (b > 240) {
			addB = -addB;
			b += addB;
		}
	}
}

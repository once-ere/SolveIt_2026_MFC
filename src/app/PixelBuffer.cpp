// CalcMap.cpp : implementation file
//

#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
/*
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
PixelBuffer::PixelBuffer(int w, int h):
	width_(w),
	height_(h)
{
	wchar_t buf[512];
//	width_ = 0;
//	height_ = 0;
	rowInfos_ = NULL;
	mem_ = NULL;
	int npixel = w*h;
//	mem_ = (unsigned int*) malloc(npixel * sizeof(unsigned int));
//	rowInfos_ = (RowInfo*) malloc(h * sizeof(RowInfo));
	rowInfos_	= new RowInfo[h];

	mem_		= new unsigned int[npixel];

//	HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, npixel * sizeof(unsigned int));   
//	mem_ = (unsigned int*)GlobalLock(hgCF_TEXTData);
//	GlobalUnlock(hgCF_TEXTData);

	if ((mem_ == NULL) || (rowInfos_ == NULL))
	{
		_sntprintf_s(buf, _countof(buf), _TRUNCATE, L"ERROR: Failed to allocate pixel buffer\n");
//		width_ = 0;
//		height_ = 0;
		throw buf;
	}
	else {
//		width_ = w;
//		height_ = h;
	}
}

PixelBuffer::~PixelBuffer() {
//	if (mem_ != NULL) //GlobalFree(mem_);
//	if (mem_ != NULL) free(mem_);
//	if (rowInfos_ != NULL) free(rowInfos_);
	if (mem_ != NULL) delete[] mem_;
	if (rowInfos_ != NULL) delete[] rowInfos_;
}

unsigned int PixelBuffer::getWidth() {
	return width_;
}

unsigned int PixelBuffer::getHeight() {
	return height_;
}

unsigned int PixelBuffer::getNumPixels() {
	return width_ * height_;
}

unsigned int* PixelBuffer::getRowPointer(int row) {
	//_sntprintf(buf, count, "%p\n",&mem_[row*width_]);
	if (mem_ == NULL) return NULL;
	else return (&mem_[row*width_]);
}

RowInfo* PixelBuffer::getRowInfo(int row) {
	if (rowInfos_ == NULL) return NULL;
	else return (rowInfos_ + row);
}

bool PixelBuffer::isRowCalculated(int row) {
	RowInfo* ri = getRowInfo(row);
	if (ri == NULL) return false;
	else return ri->calculated;
}

void PixelBuffer::setRowCalculated(int row, bool calculated) {
	RowInfo* ri = getRowInfo(row);
	if (ri == NULL) return;
	else ri->calculated = calculated;
}

int PixelBuffer::getRowAgent(int row) {
	RowInfo* ri = getRowInfo(row);
	if (ri == NULL) return ROW_AGENT_NONE;
	else return ri->agent;
}

void PixelBuffer::setRowAgent(int row, int agent) {
	RowInfo* ri = getRowInfo(row);
	if (ri == NULL) return;
	else ri->agent = agent;
}

void PixelBuffer::resetRow(int row) {
	setRowCalculated(row, false);
	setRowAgent(row, ROW_AGENT_NONE);
}

unsigned int PixelBuffer::getTotalIters(unsigned int maxi, unsigned int iterBlack) {
	unsigned int tot = 0;
	unsigned int* rowP;
	unsigned int iters;
	for (int y=0; y<height_; y++) {
		if (!isRowCalculated(y)) continue;
		rowP = PixelBuffer::getRowPointer(y);
		for (int x=0; x<width_; x++) {
			iters = *rowP++;
			if (iters == iterBlack) iters = maxi;
			tot += iters;
		}
	}
	return tot;
}

void PixelBuffer::clearBuffer() {
	unsigned int* p = mem_;
	for (int i=0; i<width_*height_; i++)
	{
		*p++ = 0;
//		*p++ = 0xFFFFFFFF;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define RENDER_MODE_FPU_ASM	 1
//double four = 4.0;
//int w_=0; // Window width
//double ax_ = -2.0;
//double sx_=0;
/////////////////////////////////////////////////////////////////////////////
// CCalcMap

///////////////////////////////////////////////////////////////////////////////
void CCalcMap::OnStepDone(WPARAM wParam, LPARAM lParam)
{
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
#if 0
void CCalcMap::OnStep(WPARAM wParam, LPARAM lParam)
{
try {
	int row		= wParam;
	unsigned int maxi=lParam;
	bool ret	=	calcPixelRow_C( row,  maxi);
//	int& endRow = h_;
//	for (int y=0; y<endRow; y++) {
//		calcPixelRow(y, maxi_, RENDER_MODE_FPU_ASM);
//	}
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
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void CCalcMap::OnHalt(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
//	return 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
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
	sx_ = (ex_ - ax_) / ((double) w_);
	sy_ = (ey_ - ay_) / ((double) h_);
//	sy_ = sx_;
//			Fire_ViewChange();

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
		::MessageBox(0,L"Unknown error.", "OnSetParam", MB_OK);
		PostQuitMessage(0);
	}
	return 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
void CCalcMap::OnDraw()
{
	try
	{
//	Lock lock (theApp.protectMapper);
	CGLView* pGLView = theApp.GetGLView();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DITHER);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	glPushMatrix();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_NORMALIZE);
//	if (doublebuf_) {
//		glDrawBuffer(GL_BACK);
//	}
//	else {
//		glDrawBuffer(GL_FRONT);
//	}

	renderImage(maxi_, RENDER_MODE_FPU_ASM);
	glPopMatrix();
	glFlush();
	glFinish();

/*
	double cx = ax_ + sx_*((double)pGLView->beginx);
	double cy = ay_ + sy_*((double)pGLView->beginy);
	double zx, zy;
	double zx2, zy2;
	// Calc Pixel
	zx = cx;
	zy = cy;
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (unsigned int i=0; i<maxi_; i++) {
		glVertex2i((zx-ax_)/sx_, (zy-ay_)/sy_);
		zx2 = zx*zx;
		zy2 = zy*zy;
		if ((zx2 + zy2) > 4) break;
		zy = 2*zx*zy;
		zx = zx2 - zy2;
		zx += cx;
		zy += cy;
	}
	glEnd();
	glFlush();
*/
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CCalcMap::OnDraw Error: ", MB_OK);
	//	PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CCalcMap::OnDraw", MB_OK);
	//	PostQuitMessage(0);
	}
}
///////////////////////////////////////////////////////////////////////////////
// CCalcMap message handlers
///////////////////////////////////////////////////////////////////////////////
//------------------ Render the image -------------------
bool CCalcMap::renderImage(unsigned int maxi, int mode)
{
	try
	{
	//_sntprintf(buf, count, "%f,%f %d\n", ax_, ay_, maxi);
//	wchar_t buf[512];
//	size_t count = sizeof(buf)/sizeof(buf[0]) - 1;

	if (h_ == 0) return true;
	CGLView* pGLView = theApp.GetGLView();
	int w_	= pGLView->viewWidth;

	glClear(GL_COLOR_BUFFER_BIT);

	// Wait until sync == 0. If > 0, dec sync and begin execution
{
		// Ok use the CPUs, but first check for window reshaped !
		if (reshaped_) {
			reshaped_ = false;
			if (glArrColor_ != NULL) delete[] glArrColor_;//free(glArrColor_);
			if (glArrVertex_ != NULL) delete[] glArrVertex_;//free(glArrVertex_);
			if(pixBuf_ != NULL) delete pixBuf_;
			glArrColor_ = new GLubyte[w_*3];//(GLubyte*) realloc(glArrColor_, w_*3*sizeof(GLubyte));
			if (glArrColor_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating row color buffer");
			//	_sntprintf(buf, count, "ERROR: allocating row color buffer\n");
			//	throw buf;
			}
			glArrVertex_ = new GLfloat[w_*2];//(GLfloat*) realloc(glArrVertex_, w_*2*sizeof(GLfloat));
			if (glArrVertex_ == NULL) {
				throw SolveIt_ERROR(L"ERROR: allocating vertex buffer");
			//	_sntprintf(buf, count, "ERROR: allocating vertex buffer\n");
			//	throw buf;
			}
			// Reallocate pixel buffer
			// In case of quad pixel calculation, pad buffer width to multiple of 4
			int w = w_;
			pixBuf_ = new PixelBuffer(w, h_);
			if (pixBuf_ == NULL)
			{
				throw SolveIt_ERROR(L"ERROR: allocating PixelBuffer ");
			//	_sntprintf(buf, count, "ERROR: allocating PixelBuffer \n");
			//	throw buf;
			}
		}
	}

	// Get the fun !!!
	int y = 0;

	// Mark all rows as not calculated
	for (y=0; y<h_; y++) pixBuf_->setRowCalculated(y, false);

	// Start the slave threads (on SMP machines)
	int endRow = h_;

	// The master thread (this) calcs+render his slice
	for (y=0; y<endRow; y++) {
		calcPixelRow(y, maxi, mode);
		renderPixelRow(y, mode);
	}

	bool complete = false;

	// After calculating his slice, the master thread renders the slaves' ones
	while (!complete) {
		complete = true;
		for (y=0; y<h_; y++) {
			if (pixBuf_->isRowCalculated(y)) renderPixelRow(y, mode);
			else complete = false;
		}
	}
	//slavesWorking_ = 0;
	//while (slavesWorking_ != 0) Sleep(1);

	// Flush damn OpenGL queue (needed!)
//	glFlush();
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CCalcMap::renderImage Error: ", MB_OK);
				throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CCalcMap::renderImage", MB_OK);
				throw SolveIt_ERROR(L"?????????");
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Render the given pixel row for the given mode
 * @return true if row calc was ok, false otherwise
 */
bool CCalcMap::renderPixelRow(int row, int mode) {
	if (pixBuf_ == NULL) return false;
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
	if (rowBuffer == NULL) return false;
	CGLView* pGLView = theApp.GetGLView();
	int w_	= pGLView->viewWidth;
	double ax_ = pGLView->glFrustum_left;
	double ex_ = pGLView->glFrustum_right;
	double sx_ = (ex_ - ax_) / ((double) w_);

	unsigned int i;

	if (useGLArrays_) {
		/* enable arrays client state */
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLubyte* arrC = glArrColor_;
		GLfloat* arrV = glArrVertex_;
		GLfloat frow = (GLfloat) (ay_ + sy_*((double)row));//row;
	//	for (GLfloat x=0; x<(GLfloat)w_; x += 1.0f)
		for (int x=0; x < w_; ++x)
		{
			//_sntprintf(buf, count, "%d\n", i);
			//_sntprintf(buf, count, "%p\t%p\t%p\n", rowBuffer, arrC, arrV);
			i = *rowBuffer;
			if (i == ITER_BLACK) {
				*arrC++ = (GLubyte) 0;
				*arrC++ = (GLubyte) 0;
				*arrC++ = (GLubyte) 0;
			}
			else {
				*arrC++ = colorTable_[i*3];
				*arrC++ = colorTable_[i*3+1];
				*arrC++ = colorTable_[i*3+2];
			}
			*arrV++ = (GLfloat)(ax_+x*sx_);
			*arrV++ = frow;
			rowBuffer++;
		}
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, glArrColor_);
		glVertexPointer(2, GL_FLOAT, 0, glArrVertex_);
		glDrawArrays(GL_POINTS, 0, w_);
		/* disable arrays client state */
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else {
		GLfloat frow = (GLfloat) (ay_ + sy_*((double)row));//row;
		glBegin(GL_POINTS);
		for (int x=0; x<w_; x++) {
			//_sntprintf(buf, count, "%d\n", i);
			i = *rowBuffer;
			if (i == ITER_BLACK) {
				glColor3d(0.0f, 0.0f, 0.0f);
			}
			else {
				glColor3ub(colorTable_[i*3], colorTable_[i*3+1], colorTable_[i*3+2]);
			}
		//	glVertex2i(x, row);
			glVertex2f((GLfloat)(ax_+x*sx_), frow);
			rowBuffer++;
		}
		glEnd();
	}
	glFlush();
	glFinish();

	return true;
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Calculate the given pixel row using the given mode
 * @return true if row calc was ok, false otherwise
 */
bool CCalcMap::calcPixelRow(int row, unsigned int maxi, int mode)
{
//	Lock lock (theApp.protectMapper);
	CGLView* pGLView = theApp.GetGLView();
	if(pGLView->pixBuf_ == 0) return false;

	pGLView->pixBuf_->setRowCalculated(row, false);
	bool retcode = false;
//	retcode = calcPixelRow_C(row, maxi);
	retcode = calcPixelRow_FPU_ASM(row, maxi);
	if (retcode) pGLView->pixBuf_->setRowCalculated(row, true);
//	// TODO: Row agent not yet supported
	return retcode;
}
///////////////////////////////////////////////////////////////////////////////

bool CCalcMap::calcPixelRow_FPU_ASM(int row, unsigned int maxi)
{
#ifdef _WIN64
	return calcPixelRow_C(row, maxi);
#else
/*
	CGLView* pGLView = theApp.GetGLView();

	unsigned int* rowBuffer = pGLView->pixBuf_->getRowPointer(row);

	// C row calculation routine
	// Calc vars
	//double _cx = ax_;
	double _cy = ay_ + sy_*((double)row);
	
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
*/
	return true;
#endif
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Calculate the given pixel row using C mode
 * @return true if row calc was ok, false otherwise
 */

#if 0
bool CCalcMap::calcPixelRow_C(int row, unsigned int maxi)
{
//	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
///////////////////////////////////////////////////////////////////////////////
	CGLView* pGLView = theApp.GetGLView();
	unsigned int* rowBuffer = 0;
	unsigned int width = 0;
	{
//		Lock lock (theApp.protectMapper);
	//	unsigned int* rowBuffer_ = pGLView->pixBuf_->getRowPointer(row);
	//	if ( rowBuffer_ == NULL) throw SolveIt_ERROR(L"-2");
		width = pGLView->pixBuf_->getWidth();
	//	CopyMemory(rowBuffer, rowBuffer_, sizeof(unsigned int)*width);
	}
		if ( width <= 0) throw SolveIt_ERROR(L"1");
		rowBuffer = new unsigned int[width];
		if ( rowBuffer == NULL) throw SolveIt_ERROR(L"2");
	ZeroMemory(rowBuffer, sizeof(unsigned int)*width);
	// C row calculation routine
	// Calc vars
	register double cx = ax_;
	register double cy = ay_ + sy_*((double)row);
	register double zx, zy;
	register double zx2, zy2;
	
	//printf("%f, %f\n", cx, cy);
//	double &ay_ = pGLView->glFrustum_bottom;
//	double &ey_ = pGLView->glFrustum_top;
//	double sy_ = (ey_ - ay_) / ((double) pGLView->viewHeight);
	// C row calculation routine
	// Calc vars
//	register double cx = ax_;
//	register double cy = ay_ + sy_*((double)row);





	for (int x=0; x<w_; x++) {
		// Calc Pixel
		zx = cx;
		zy = cy;
		for (unsigned int i=0; i<maxi; i++) {
			zx2 = zx*zx;
			zy2 = zy*zy;
			if ((zx2 + zy2) > 4) break;
			zy = 2*zx*zy;
			zx = zx2 - zy2;
			zx += cx;
			zy += cy;
		}
		cx += sx_;
		if (i == maxi) *rowBuffer = ITER_BLACK;
		else *rowBuffer = i;
		rowBuffer++;
	}
	// End of calculation routine

	{
//		Lock lock (theApp.protectMapper);
		if (width != pGLView->pixBuf_->getWidth()) throw SolveIt_ERROR(L"-5");
		unsigned int* rowBuffer_ = pGLView->pixBuf_->getRowPointer(row);
		if ( rowBuffer_ == NULL) throw SolveIt_ERROR(L"-4");
//		for (int x=0; x < w_; ++x)
//		{
//			unsigned int a = rowBuffer[x];
//			*(rowBuffer_ + x) = a;
//		}
		CopyMemory(rowBuffer_, rowBuffer, sizeof(unsigned int)*width);
		pGLView->pixBuf_->setRowCalculated(row, true);
	}
	delete[]rowBuffer;
	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////
void CCalcMap::prepareColorTable(int numColors, GLubyte startR, GLubyte startG, GLubyte startB) {
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**
 * Calculate the given pixel row using C mode
 * @return true if row calc was ok, false otherwise
 */

bool CGLView::calcPixelRow_C(int row, unsigned int maxi)
{
#if 0
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
	double &ay_ = glFrustum_bottom;
	double &ey_ = glFrustum_top;
	double sy_ = (ey_ - ay_) / ((double) viewHeight);
	// C row calculation routine
	// Calc vars
	register double cx = ax_;
	register double cy = ay_ + sy_*((double)row);
	register double zx, zy;
	register double zx2, zy2;
	
	//printf("%f, %f\n", cx, cy);

	for (int x=0; x<w_; x++) {
		// Calc Pixel
		zx = cx;
		zy = cy;
		for (unsigned int i=0; i<maxi; i++) {
			zx2 = zx*zx;
			zy2 = zy*zy;
			if ((zx2 + zy2) > 4) break;
			zy = 2*zx*zy;
			zx = zx2 - zy2;
			zx += cx;
			zy += cy;
		}
		cx += sx_;
		if (i == maxi) *rowBuffer = ITER_BLACK;
		else *rowBuffer = i;
		rowBuffer++;
	}
	// End of calculation routine
	pixBuf_->setRowCalculated(row, true);
	return true;
#endif
#if 0
	RR::SetPrecision(100);
	RR ax__ , ex__, ay__, ey__, zx, zy, zx2, zy2, Four;
	Four	= 4.0;
	ax__= glFrustum_left;
	ex__ = glFrustum_right;
	ay__ = glFrustum_bottom;
	ey__ = glFrustum_top;
// Recalc real plane parameters
	RR sx__ = (ex__ - ax__) / ((double) viewWidth);
	RR sy__ = (ey__ - ay__) / ((double) viewHeight);

	RR cx = ax__;
	RR cy = ay__ + sy__*((double)row);

	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
	for (int x=0; x<w_; x++) {
		zx = cx;
		zy = cy;
		for (unsigned int i=0; i<maxi; i++) {
			zx2 = zx*zx;
			zy2 = zy*zy;
			if ((zx2 + zy2) > Four) break;
			zy = 2*zx*zy;
			zx = zx2 - zy2;
			zx += cx;
			zy += cy;
		}
		cx += sx__;
		if (i == maxi) *rowBuffer = ITER_BLACK;
		else *rowBuffer = i;
		rowBuffer++;
	}
	// End of calculation routine
#endif
	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CGLView::calcPixelRow_FPU_ASM(int row, unsigned int maxi)
{
#ifdef _WIN64
	return calcPixelRow_C(row, maxi);
#else
	const double four = 4.0;
	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);
	double &ax_ = glFrustum_left;
	double &ex_ = glFrustum_right;
	double &ay_ = glFrustum_bottom;
	double &ey_ = glFrustum_top;
// Recalc real plane parameters
	double sx_ = (ex_ - ax_) / ((double) viewWidth);
	double sy_ = (ey_ - ay_) / ((double) viewHeight);
	double _cx = ax_;
	double _cy = ay_ + sy_*((double)row);
	int w_=viewWidth;
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

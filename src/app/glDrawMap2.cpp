// glDrawMap2.cpp : Implementation of CGLView
#include "stdafx.h"
#include "SolveIt.h"
#include "GLView.h"
#include "Types.h"
#include "System.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "CalcMap.h"
#include "PlayMap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern Mutex protectList;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::glComputeMap(const double& y_max, const double& y_min)
{
	glFrustum_top		= y_max;
	glFrustum_bottom	= y_min;
	CSystem* m_system = theApp.GetSystem();
	double y = 0, Y=0;
	double height = y_max - y_min;
	double h = (height) / ((double) viewHeight);
//	double width = glFrustum_right - glFrustum_left;
//	double w = ((double) viewWidth) / (width);
//	int ix_max = viewWidth;
	unsigned int* rowBuffer = 0;
	int	ix	= 0;
	pixBuf_->clearBuffer();//
	for (int row=0; row<viewHeight; row++)
	{
		y = row;
		Y = y_min	+ y * h;
		try{
			calcRasterRow( row, Y, 0.2, glFrustum_left);//
			rowBuffer = pixBuf_->getRowPointer(row);
			for (int q=0; q<viewWidth; q++)
			{
				ix	= rowBuffer[q];
				if (ix == 0) rowBuffer[q] = ITER_WHITE;
			}
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			int izx_max = viewWidth;
			ASSERT(0);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CGLView::glDrawMap_List2(const POINT& point, int bZoomIn)
{
	try
	{

		CSystem* m_system	= theApp.GetSystem();
		double s = 2*m_system->m_fScale;

		double x = point.x;
		double y = viewHeight-point.y;//point.y;

		double X = glFrustum_left	+ x * (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		double Y = glFrustum_bottom	+ y * (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);

		double w = (glFrustum_right - glFrustum_left);
		double h = (glFrustum_top - glFrustum_bottom);

//		maxi_ = m_ZoomLevel<30?400:1000;
		maxi_ = 333;
		if (bZoomIn>1)
		{
		//	Lock lock(mutexPlayData);
			m_system->player->g_Rate	= Y;
		}

	if (bZoomIn==1||bZoomIn==11)
	{
		++m_ZoomLevel;
		orig_glFrustum_left=glFrustum_left		= X - w/s;
		orig_glFrustum_right=glFrustum_right	= X + w/s;
		orig_glFrustum_bottom=glFrustum_bottom	= Y - h/s;
		orig_glFrustum_top=glFrustum_top		= Y + h/s;
	}
	else
	if (bZoomIn==0||bZoomIn==10)
	{
		--m_ZoomLevel;
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
		size_x_ = (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		size_y_ = (glFrustum_top - glFrustum_bottom) / ((double) viewHeight);

//		if (m_system->mapper)
//			m_system->player->PostThreadMessage(WM_USER_SetParam, 0, 0);

/*	
	{
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
		HDC hDC = GetDC()->GetSafeHdc();//m_pDC->GetSafeHdc();//::GetDC(m_hWnd);
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

		glComputeMap(glFrustum_top, glFrustum_bottom);
		glDrawMap2(glFrustum_top, glFrustum_bottom);
//		PixelsToTexture(PixelsSnapshot(), texture);
//		TextureToDisplayList(listBackground, texture);
	//	Invalidate();

	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::glDrawMap_List2 Error: ", MB_OK);
	//			throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::glDrawMap_List2", MB_OK);
	//			throw SolveIt_ERROR(L"?????????");
	}
}
///////////////////////////////////////////////////////////////////////////////
/*
void CGLView::glDrawMap_List2(const double& y_max, const double& y_min, GLuint& listVerhulst, GLuint& texture)
{

//	glClearMap_List( listVerhulst, texture);
	glDrawMap2(  y_max,  y_min);
	PixelsToTexture(PixelsSnapshot(), texture);
	TextureToDisplayList(listVerhulst, texture);
}
*/
///////////////////////////////////////////////////////////////////////////////
			//	   unsigned int** _rowBuffer, 
			//	   double** rowBuffer, 
void CGLView::calcRasterRow(
				   unsigned int* rowBuffer, 
			//	   unsigned int row, 
				   double rate, double x0, double x1)
{
try{
// double	x	= x1 + (ix / viewWidth) * (width);
// int		ix	= (x - x1)*viewWidth/width;
	double width		= glFrustum_right - glFrustum_left;
	const double w		= ((double) viewWidth) / (width);
	double x			= 0;
	long temp			= 0;
	long count			= 0;
	long maxi			= maxi_;
	long vwWidth		= viewWidth;
	long vwWidth2		= viewWidth;

#ifdef _WIN64
	double xValue = x0;
	for (long iter = 0; iter < maxi; ++iter)
	{
		xValue = rate * xValue * (1.0 - xValue);
	}
	for (long index = 0; index < vwWidth2; ++index)
	{
		xValue = rate * xValue * (1.0 - xValue);
		rowBuffer[index] = static_cast<unsigned int>(w * (xValue - x1));
	}
	count = vwWidth2;
#else

//	unsigned int* rowBuffer	= *_rowBuffer;//new unsigned int[viewWidth+1];
//	unsigned int rowBuffer[2028];
//	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);

	_asm {

	finit

//LoadPtr es, di, fInt              ; ES:DI points to 1st root
//	LoadPtr ds, si, fInt			//; DS:SI points to 2nd root
//	fstp    DWORD PTR ds:[si]       ; Copy result, pop stack

//	mov	edi,maxi		// edi = maxiters
//transform to int:
	fld	w				// STACK: w
	fld	x1				// STACK: x1 w
// dynamics:
	fld	rate			// STACK: rate x1 w
// dynamics: initial value
	fld	x0				// STACK: x rate x1 w


	xor ecx,ecx

	mov	edx,maxi

NxtPix0:
	fld st(1)			// STACK: copy_rate x rate x1 w
	fld st(1)			// STACK: copy_x copy_rate x rate x1 w
	fld st(0)			// STACK: copy_x copy_x copy_rate x rate x1 w

	fld1				//1 x x rate x rate x1 w

	fsubrp st(1),st(0)	// STACK: 1-x x rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: x*(1-x) rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: rate*x*(1-x)=new_x  x rate x1 w
//	fst		 x
	fxch	st(1)		// STACK: x new_x  rate x1 w
//	fst		 x
	fstp	st			// STACK:  new_x=x  rate x1 w
//	fst		 x

/*
	fld st(2)			// STACK: x1 x rate x1 w
	fld st(1)			// STACK: x x1 x rate x1 w
	fsubrp st(1),st(0)	// STACK: x-x1 x rate x1 w
	fst		 x
	fld st(4)			// STACK: w x-x1  x rate x1 w
	fst		 x
	fmulp st(1),st(0)	// STACK: w*(x-x1) x  rate x1 w
	fst		 x
//	FRNDINT
	fistp   [dword ptr temp]
	mov     ecx,[temp]

//	sahf				//; Load flag register
*/	
	dec	edx
	jnz	NxtPix0

	mov esi, rowBuffer
	xor ecx,ecx		 // First iter
	mov	edx,vwWidth2

NxtPix:
	fld st(1)			// STACK: copy_rate x rate x1 w
	fld st(1)			// STACK: copy_x copy_rate x rate x1 w
	fld st(0)			// STACK: copy_x copy_x copy_rate x rate x1 w
	fld1				//1 x x rate x rate x1 w

	fsubrp st(1),st(0)	// STACK: 1-x x rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: x*(1-x) rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: rate*x*(1-x)=new_x  x rate x1 w
//	fst		 x
	fxch	st(1)		// STACK: x new_x  rate x1 w
//	fst		 x
	fstp	st			// STACK:  new_x=x  rate x1 w
//	fst		 x

	fld st(2)			// STACK: x1 x rate x1 w
	fld st(1)			// STACK: x x1 x rate x1 w
	fsubrp st(1),st(0)	// STACK: x-x1 x rate x1 w
//	fst		 x
	fld st(4)			// STACK: w x-x1  x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: w*(x-x1) x  rate x1 w
//	fst		 x

//	FRNDINT
	fistp   [dword ptr temp]	// STACK: x  rate x1 w
//	fistp   [temp]
	mov     eax,[temp]

	mov		dword ptr [esi],	eax	//;SET PIXEL !
	add	esi,4			;Change to next pixBuf element

//	sahf				//; Load flag register
	inc	ecx
	dec	edx
	jnz	NxtPix
	mov  [count],   ecx
	}
	#endif
	
	ASSERT(count==vwWidth2);
/*
	int	ix	= 0;
	glBegin(GL_POINTS);
	for (int q=0; q<count; q++)
	{
		ix	= rowBuffer[q];
		if (ix>=0 && ix < viewWidth)
		{
		//	glVertex2i(ix, viewHeight-row);
			glVertex2i(ix, row);
		}
	}
	glEnd();
*/
//	delete[]rowBuffer;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		ASSERT(0);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::calcRasterRow(
				   const unsigned int row, 
				   const double rate, const double x0, const double x1)
{
try{
// double	x	= x1 + (ix / viewWidth) * (width);
// int		ix	= (x - x1)*viewWidth/width;
	double width		= glFrustum_right - glFrustum_left;
	const double w		= ((double) viewWidth) / (width);
	double x			= 0;
	const long zero		= 0;
	const double four	= 4.0;
	long temp			= 0;
	long count			= 0;
	long maxi			= maxi_;
	long vwWidth		= viewWidth;
	long vwWidth2		= m_NDATA;//4*viewWidth;
//	vwWidth2 *= (m_ZoomLevel<13?1:(m_ZoomLevel-12));
//	if (m_ZoomLevel>12) vwWidth2 /= 2;
//	if (m_ZoomLevel>16) vwWidth2 /= 2;
//	if (m_ZoomLevel>20) vwWidth2 /= 2;

	unsigned int* rowBuffer = pixBuf_->getRowPointer(row);

#ifdef _WIN64
	double xValue = x0;
	for (long iter = 0; iter < maxi; ++iter)
	{
		xValue = rate * xValue * (1.0 - xValue);
	}
	long remainingInRange = vwWidth;
	long remainingOutOfRange = vwWidth2;
	while (remainingInRange > 0 && remainingOutOfRange > 0)
	{
		xValue = rate * xValue * (1.0 - xValue);
		const long index = static_cast<long>(w * (xValue - x1));
		if (index >= 0 && index < vwWidth)
		{
			++rowBuffer[index];
			++count;
			--remainingInRange;
		}
		else
		{
			--remainingOutOfRange;
		}
	}
#else

	_asm {
//	push  ds;                  // Preserve!
//	les   ebx,this;             // Use ES:BX as "this" pointer
//	mov   edx,ees:[ebx].viewWidth;   // AX is "width" (temporarily)

//	les   di,rowBuffer;                // ES:DI is "rowBuffer"

//mov esi, rowBuffer
 
	finit

		//transform to int:
	fld	w				// STACK: w
	fld	x1				// STACK: x1 w
// dynamics:
	fld	rate			// STACK: rate x1 w
// dynamics: initial value
	fld	x0				// STACK: x rate x1 w


	xor ecx,ecx

	mov	edx,maxi

NxtPix0:
	fld st(1)			// STACK: copy_rate x rate x1 w
	fld st(1)			// STACK: copy_x copy_rate x rate x1 w
	fld st(0)			// STACK: copy_x copy_x copy_rate x rate x1 w

	fld1				//1 x x rate x rate x1 w

	fsubrp st(1),st(0)	// STACK: 1-x x rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: x*(1-x) rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: rate*x*(1-x)=new_x  x rate x1 w
//	fst		 x
	fxch	st(1)		// STACK: x new_x  rate x1 w
//	fst		 x
	fstp	st			// STACK:  new_x=x  rate x1 w
//	fst		 x

/*
	fld st(2)			// STACK: x1 x rate x1 w
	fld st(1)			// STACK: x x1 x rate x1 w
	fsubrp st(1),st(0)	// STACK: x-x1 x rate x1 w
	fst		 x
	fld st(4)			// STACK: w x-x1  x rate x1 w
	fst		 x
	fmulp st(1),st(0)	// STACK: w*(x-x1) x  rate x1 w
	fst		 x
//	FRNDINT
	fistp   [dword ptr temp]
	mov     ecx,[temp]
*/	
	dec	edx
	jnz	NxtPix0

//	mov edi, rowBuffer
	xor ecx,ecx		 // First iter
	xor ebx,ebx
	mov	ebx,vwWidth2
	mov	edx,vwWidth

NextPixel:
	fld st(1)			// STACK: copy_rate x rate x1 w
	fld st(1)			// STACK: copy_x copy_rate x rate x1 w
	fld st(0)			// STACK: copy_x copy_x copy_rate x rate x1 w
	fld1				//1 x x rate x rate x1 w

	fsubrp st(1),st(0)	// STACK: 1-x x rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: x*(1-x) rate x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: rate*x*(1-x)=new_x  x rate x1 w
//	fst		 x
	fxch	st(1)		// STACK: x new_x  rate x1 w
//	fst		 x
	fstp	st			// STACK:  new_x=x  rate x1 w
//	fst		 x

	fld st(2)			// STACK: x1 x rate x1 w
	fld st(1)			// STACK: x x1 x rate x1 w
	fsubrp st(1),st(0)	// STACK: x-x1 x rate x1 w
//	fst		 x
	fld st(4)			// STACK: w x-x1  x rate x1 w
//	fst		 x
	fmulp st(1),st(0)	// STACK: w*(x-x1) x  rate x1 w
//	fst		 x

//	FRNDINT
	fistp   [dword ptr temp]	// STACK: x  rate x1 w
//	fistp   [temp]


/*
	mov     eax,[temp]
	mov		dword ptr [edi],	eax	//;SET PIXEL !
	add	edi,4			;Change to next pixBuf element
*/	

/*
*/
// Check index
	mov     eax,[temp]

	cmp eax,zero		 
	jl	not_in_range
	cmp eax,vwWidth
	jge	not_in_range

	shl	eax, 2
	mov esi, dword ptr rowBuffer
	add	esi,	eax	
	inc	dword ptr [esi]
	inc	ecx
	dec	edx
	jmp _continue

not_in_range:
	dec ebx
//	cmp ebx,vwWidth
	jnz	_continue
	xor edx,edx

//noabort:
_continue:
//	inc	ecx
//	dec	edx
	or	edx, edx
	jnz	NextPixel

	mov  [count],   ecx

//	pop  ds;                  // Preserve!
	}
	#endif
	
//	ASSERT(count==vwWidth2);

	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		int c=0;
		ASSERT(0);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::glDrawMap2(const double& y_max, const double& y_min)
{
try{
	glFrustum_top		= y_max;
	glFrustum_bottom	= y_min;
	CSystem* m_system = theApp.GetSystem();
	double y = 0, x=0;
	double height = y_max - y_min;
	double h = (height) / ((double) viewHeight);
	double width = glFrustum_right - glFrustum_left;
	double w = ((double) viewWidth) / (width);
	int ix_max = viewWidth, row=0;

	SetCursor(LoadCursor(NULL, IDC_WAIT));
//		HDC hDC = GetDC()->GetSafeHdc();
		HDC hDC = m_pDC->GetSafeHdc();

	GLenum e=GL_NO_ERROR;
//	if(FALSE == ::wglMakeCurrent(hDC , m_hRC))
//	{
//		int ix_umax = viewWidth;
//		ASSERT(0);
//		throw SolveIt_Error("//wglMakeCurrent failed", __FILE__, __LINE__);
//	}

		glViewport(0, 0, viewWidth, viewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluOrtho2D(0, (double)viewWidth, 0, (double)viewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		glTranslated(0.375, 0.375, 0.0);
	glRenderMode(GL_RENDER);  
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

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
#if 0
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
#endif

	glPushMatrix();

	glTranslated(m_fTransX, m_fTransY, m_fTransZ);
	for (row=0; row<viewHeight; row++)
	{
		renderPixelRow(row, 1);
	}


#if 0
#if 10
	unsigned int* rowBuffer = 0;
	int	ix	= 0;
	pixBuf_->clearBuffer();//
	for (row=0; row<viewHeight; row++)
	{
		y = row;
		double Y = y_min	+ y * h;
		try{
			calcRasterRow( row, Y, 0.2, glFrustum_left);//
			rowBuffer = pixBuf_->getRowPointer(row);
			for (int q=0; q<viewWidth; q++)
			{
				ix	= rowBuffer[q];
				if (ix == 0) rowBuffer[q] = ITER_WHITE;
			}
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			int izx_max = viewWidth;
			ASSERT(0);
		}
	}
#else
	int q;
	glPushMatrix();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//
	glClear(GL_COLOR_BUFFER_BIT);
//	glDisable(GL_NORMALIZE);
	glColor3d( 0, 1, 0 );
		glBegin(GL_POINTS);
	for (int row=0; row<viewHeight; row++)
	{
		y = row;
		double Y = y_min	+ y * h;
//		double X = glFrustum_left	+ x * (glFrustum_right - glFrustum_left) / ((double) viewWidth);
		x = 0.2;
		for (q=0;q<maxi_;q++)
		{
			x = Y * x * (1 - x);
		}
		for (q=0;q < viewWidth; q++)
		{
			x = Y * x * (1 - x);
			int	ix	= (x - glFrustum_left)*w;
			if (ix>=0 && ix < viewWidth)
			{
				glVertex2i(ix, row);
			}
			else
			{
			//	ASSERT(0);
				if (ix > ix_max) ix_max = ix;
			}
		}
	}
		glEnd();
		glPopMatrix();
#endif
#endif
	glPopMatrix();
	glFlush();
	glFinish();
	SwapBuffers(hDC);
	//wglMakeCurrent(0, 0);
//	::ReleaseDC(m_hWnd, hDC);
	if (ix_max > viewWidth)
	{
		wchar_t buf[128];
		/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
		oss	<< "@@ " << viewWidth << " < " << ix_max << " !" ;
		oss	<< ends;
		wstring str(buf);
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		pFrame->SetStatusBarText(str, 0);
	}

	}
	catch (wchar_t * str)
	{
		::MessageBox(0,str, L"CGLView::glDrawMap2 Error: ", MB_OK);
				throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"CGLView::glDrawMap2", MB_OK);
				throw SolveIt_ERROR(L"?????????");
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
///////////////////////////////////////////////////////////////////////////////


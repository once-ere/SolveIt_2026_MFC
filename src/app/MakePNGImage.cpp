// MakePNGImage.cpp : Implementation of CScene

#include "stdafx.h"
//#include "CGLView.h"
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "DibSectn.h"
#include "hipng.h"
wchar_t *  GetPngFilename (void);
extern wstring __cdecl getPngFilename (void);
/////////////////////////////////////////////////////////////////////////////
// CGLView
int CGLView::MakePNGImage()
{
	const wchar_t strCaption[]	= _T("Action: Copyright by Prof. P. Nash, UTSA    ");
	HBITMAP			hBitmap		= 0;
	HBITMAP			OldhBitmap	= 0;
	HFONT			hFont		= 0;
	HFONT			hOldFont	= 0;
	HDC				hdc			= 0;
	LPBITMAPINFO	pbmi		= 0;
	BYTE*			m_pBits		= 0;
	char*			snapshBuf	= 0;
	wstring			PngFilename;
	GLenum			e			= GL_NO_ERROR;

	union {
		int iv[4];
		struct {
			GLint ix;
			GLint iy;
			GLsizei iwidth;
			GLsizei iheight;
		};
	}xxx;


try{
	PngFilename	= getPngFilename ();
	if(PngFilename.size() == 0)
		return 0;
//	HDC hDC = ::GetDC(m_hWnd);
//		HDC hDC = GetDC()->GetSafeHdc();
		HDC hDC = m_pDC->GetSafeHdc();
	//wglMakeCurrent(hDC, m_hRC);

//	if(FALSE == :://wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
//	{
//		ASSERT(0);
//		throw theError("//wglMakeCurrent failed", __FILE__, __LINE__);
//	}

	glGetIntegerv(GL_VIEWPORT, xxx.iv);
	long W= viewWidth;
	long H= viewHeight;
	ASSERT(W == xxx.iwidth);
	ASSERT(H == xxx.iheight);
	RECT rc = {0,0,W,H};
	hFont = CreateFont(
		-8,
		0,
		0,
		0,
		FW_BOLD,
		0,
		0,
		0,
		ANSI_CHARSET,
		OUT_DEVICE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_SWISS,
		NULL
	);

	snapshBuf = new char[xxx.iwidth*xxx.iheight*3];
	if (snapshBuf==0)
	{
		return 0;
	}

	glReadPixels( 0, 0, xxx.iwidth, xxx.iheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, snapshBuf );//GL_RGB

	hdc		= ::CreateCompatibleDC(hDC);
//	::ReleaseDC(GetSafeHwnd(), hDC);

	WORD bits = 24;
	hBitmap = DSCreateDIBSection( W, H, bits);
	ASSERT(hBitmap != 0);

	if (hBitmap==0)
	{
		//_variant_t v = (long) W*H;
		//std::wstring err=v;
		std::wstring err = std::to_wstring(long(W*H));
		std::wstring b(L"DSCreateDIBSection error: ");
		b+=err;
		MessageBoxTimed(b.c_str());

//		::ReleaseDC(GetSafeHwnd(), hdc);

		::ReleaseDC(GetSafeHwnd(), hdc);
		DeleteDC(hdc);

		delete[] snapshBuf;
		return 0;
	}
	pbmi = DSGetBITMAPINFOForDIBSection( hBitmap );
	ASSERT(pbmi != 0);

	if (pbmi==0)
	{
		//_variant_t v = (long) (hBitmap);
		//std::wstring err=v;
		std::wstring err = std::to_wstring(static_cast<unsigned long long>(reinterpret_cast<ULONG_PTR>(hBitmap)));
		std::wstring b(L"DSGetBITMAPINFOForDIBSection error: ");
		b+=err;
		MessageBoxTimed(b.c_str());

		DeleteObject(hBitmap);

//		::ReleaseDC(GetSafeHwnd(), hdc);

		::ReleaseDC(GetSafeHwnd(), hdc);
		DeleteDC(hdc);
		delete[] snapshBuf;
		return 0;
	}
	long nImageSize = DSImageBitsSize( pbmi );
	ASSERT(nImageSize == xxx.iwidth*xxx.iheight*3);
	long nTotalSize = DSTotalBytesSize( pbmi );
	ASSERT(nImageSize == pbmi->bmiHeader.biSizeImage );

	m_pBits = DSGetPointerToDIBSectionImageBits(hBitmap);
	ASSERT(m_pBits != 0);
//MessageBox("if (m_pBits==0) return 0;");
	if (m_pBits==0)
	{
		//_variant_t v = (long) (hBitmap);
		//std::wstring err=v;
		std::wstring err = std::to_wstring(static_cast<unsigned long long>(reinterpret_cast<ULONG_PTR>(hBitmap)));
		std::wstring b(L"DSGetPointerToDIBSectionImageBits error: ");
		b+=err;
		MessageBoxTimed(b.c_str());

		DeleteObject(hBitmap);
//		::ReleaseDC(GetSafeHwnd(), hdc);
		::ReleaseDC(GetSafeHwnd(), hdc);
		DeleteDC(hdc);
		delete[] snapshBuf;
		return 0;
	}
	else
	{
		memcpy(m_pBits, snapshBuf, nImageSize);
	}
	delete[] snapshBuf;snapshBuf=0;


	OldhBitmap	= (struct HBITMAP__ *)	::SelectObject(hdc, hBitmap );
	hOldFont	= (struct HFONT__ *)	::SelectObject(hdc, hFont);
//	::SetTextColor(hdc, RGB(128,88,255));
	::SetTextColor(hdc, RGB(0,0,128));
	::SetBkMode(hdc, TRANSPARENT);

//	::DrawText(hdc, strCaption, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_BOTTOM);

			HIPNG_DIB s;
			ZeroMemory(&s, sizeof(s));
			s.file_name= new char[PngFilename.size()+1];
			strcpy_s(s.file_name, PngFilename.size() + 1, GetC( PngFilename.c_str() ) );
			s.hbmi=pbmi;
			s.pixels=m_pBits;
	//		s.color_type;	// so the routine can tell gray-scale from color
			s.interlaced=FALSE;
			s.gamma=-1;		// Set to < 0.0 if you don't know
			s.num_text=0;	// # of text entries
			s.pComment=0;	// pntr to an array of ptrs to text
			s.current_time=FALSE;	// put creation time (GMT) in the file
		// pointer to array of transparent entries for images with a palette
			s.transparent_palette=0;
			s.num_transparent=0;	// # of entries in the transparent_palette
			s.RGBtransparent_value=0;
			s.row_callback_fn=0;
			s.color_type = PNG_COLOR_TYPE_RGB;
			png_color_16 rgb24;
			if(false)
			{
				rgb24.index	= 0;
				rgb24.red	= 0xFF;
				rgb24.green	= 0xFF;
				rgb24.blue	= 0xFF;
				rgb24.gray	= 0;
				s.transparent_palette = NULL; // no palette for this color type  
				s.num_transparent = 0;
				s.RGBtransparent_value = &rgb24;  // set transparent color
			}
			else
			{
				s.transparent_palette = 0; // no palette for this color type  
				s.num_transparent = 0;
				s.RGBtransparent_value = 0;  // no transparent color
			}

		if (FALSE==write_dib_to_png (&s))
				MessageBoxTimed(L"write_dib_to_png cannot create image.");

		delete[] s.file_name;
	if (hBitmap)
	{
		SelectObject( hdc, OldhBitmap );
		DeleteObject(hBitmap);
	}
	if (hFont)
	{
		SelectObject( hdc, hOldFont );
		DeleteObject(hFont);
	}
	if (pbmi) free(pbmi);

//	::ReleaseDC(GetSafeHwnd(), hdc);

	if (hdc)
	{
		::ReleaseDC(GetSafeHwnd(), hdc);
		DeleteDC(hdc);
	}
	//wglMakeCurrent(0, 0);
	::ReleaseDC(m_hWnd, hdc);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	try{
		if (snapshBuf) delete[] snapshBuf;
		if (hBitmap)
		{
			SelectObject( hdc, OldhBitmap );
			DeleteObject(hBitmap);
		}
		if (hFont)
		{
			SelectObject( hdc, hOldFont );
			DeleteObject(hFont);
		}
		if (pbmi) free(pbmi);


		if (hdc)
		{
			::ReleaseDC(GetSafeHwnd(), hdc);
			DeleteDC(hdc);
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
		return 0;
}

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////

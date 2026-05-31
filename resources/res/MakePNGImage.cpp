// MakePNGImage.cpp : Implementation of CView

#include "stdafx.h"
#include "Scene.h"
#include "View.h"

/////////////////////////////////////////////////////////////////////////////
#include "DibSectn.h"
#include "hipng.h"
/////////////////////////////////////////////////////////////////////////////
// CUI_ClassicalPhysics
STDMETHODIMP CView::MakePNGImage()
{
	BYTE* m_pBits=0;
	long W= m_nWidth;
	long H= m_nHeight;
	HDC hdc = GetDC();
	HDC hDC = CreateCompatibleDC(hdc);
	long bits = 24;
	HBITMAP hBitmap = DSCreateDIBSection( W, H, bits);
	ATLASSERT(hBitmap != 0);
//MessageBox("if (hBitmap==0) return");
	if (hBitmap==0)
	{
		_variant_t v = (long) W*H;
		_bstr_t err=v;
		_bstr_t b(L"DSCreateDIBSection error: ");
		b+=err;
		MessageBox(b);

		ReleaseDC(hdc);
		ReleaseDC(hDC);

		DeleteDC(hDC);
		return 0;
	}
	LPBITMAPINFO pbmi = DSGetBITMAPINFOForDIBSection( hBitmap );
	ATLASSERT(pbmi != 0);
//MessageBox("if (pbmi==0) return 0;");
	if (pbmi==0)
	{
		_variant_t v = (long) (hBitmap);
		_bstr_t err=v;
		_bstr_t b(L"DSGetBITMAPINFOForDIBSection error: ");
		b+=err;
		MessageBox(b);

		DeleteObject(hBitmap);

		ReleaseDC(hdc);
		ReleaseDC(hDC);

		DeleteDC(hDC);
		return 0;
	}
	long nImageSize = DSImageBitsSize( pbmi );
	long nTotalSize = DSTotalBytesSize( pbmi );
	ATLASSERT(nImageSize == pbmi->bmiHeader.biSizeImage );

	m_pBits = DSGetPointerToDIBSectionImageBits(hBitmap);
	ATLASSERT(m_pBits != 0);
//MessageBox("if (m_pBits==0) return 0;");
	if (m_pBits==0)
	{
		_variant_t v = (long) (hBitmap);
		_bstr_t err=v;
		_bstr_t b(L"DSGetPointerToDIBSectionImageBits error: ");
		b+=err;
		MessageBox(b);

		DeleteObject(hBitmap);
		free(pbmi);

		ReleaseDC(hdc);
		ReleaseDC(hDC);

		DeleteDC(hDC);
		return 0;
	}

	HBITMAP OldhBitmap	= (struct HBITMAP__ *) SelectObject( hDC, hBitmap );

try{
//	Draw(hDC, false);//
//	GdiFlush();
	BitBlt( hDC, 0, 0, W, H,
			m_hdcMem,
			0, 0, SRCCOPY );

	HFONT hStockFont=NULL,hOldFont=NULL;
	CComQIPtr<IFont,&IID_IFont> pFont(m_pFont);
	if(pFont) pFont->get_hFont(&hStockFont);

	if(hStockFont) hOldFont = (HFONT)SelectObject(hDC,hStockFont);

	::SetTextColor(hDC, RGB(128,88,255));
	SetBkMode(hDC, TRANSPARENT);
	USES_CONVERSION;
	LPCTSTR lpsz = OLE2T(m_bstrCaption);
	RECT rc = {0,0,W,H};
	DrawText(hDC, lpsz, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_BOTTOM);
//DT_VCENTER  DT_TOP | DT_CENTER
	if(hOldFont) SelectObject(hDC,hOldFont);
} catch(...)
{
		SelectObject( hDC, OldhBitmap );
		DeleteObject(hBitmap);
		free(pbmi);

		ReleaseDC(hdc);
		ReleaseDC(hDC);

		DeleteDC(hDC);
		return 0;
}
	//		string spng("C:\\My Documents\\");
	//		spng += dsName;
	//		spng += ".png";
	//		MessageBox(spng.c_str());
	//		TCHAR buf[MAX_PATH];
	//		_tcscpy(buf, spng.c_str());
			HIPNG_DIB s;
			ZeroMemory(&s, sizeof(s));
			s.file_name=0;//buf;
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
try {
			if (FALSE==write_dib_to_png (&s))
				MessageBox("write_dib_to_png cannot create image.");
#if 0
			else if (m_pIHTML_pngImageElement)
			{
		//		_bstr_t newHTML(m_bsPngImageElement);
		//		newHTML += buf;
		//		newHTML += L"\"";
		//		MessageBox(newHTML);
//				HRESULT hr = m_pIHTML_pngImageElement->put_innerHTML(BSTR(newHTML));
				_bstr_t strAttributeName(L"img");
				_bstr_t strAttributeValue(L"\"");
				strAttributeValue += buf;
				strAttributeValue += L"\"";
				_variant_t varAttributeValue = strAttributeValue;
				_variant_t fCaseSensitive = false;
				HRESULT hr = 
					m_pIHTML_pngImageElement->setAttribute(
							BSTR(strAttributeName),
							VARIANT(varAttributeValue),
							VARIANT_BOOL(fCaseSensitive));

			}
#endif
} catch(...)
{
	MessageBox("Exception: write_dib_to_png");
}

	if (hBitmap) SelectObject( hDC, OldhBitmap );
	DeleteObject(hBitmap);
	free(pbmi);

	ReleaseDC(hdc);
	ReleaseDC(hDC);

	DeleteDC(hDC);


	return S_OK;
}

// MakePNGImage.cpp : Implementation of CView

#include "stdafx.h"
#include "Act.h"
#include "View.h"
#include "HTMLColorTable2.h"

/////////////////////////////////////////////////////////////////////////////
#include "DibSectn.h"
#include "hipng.h"
/////////////////////////////////////////////////////////////////////////////
// CUserIO

long CView::MakeImageList()
{
//	vector<ColorTableColor2> m_vecColors;
	map<int, _bstr_t> m_mapColors;
	for (int j=0; j < sizeof(HTMLColorTableColors2)/sizeof(HTMLColorTableColors2[0]); j++)
		m_mapColors[HTMLColorTableColors2[j].rgb] = HTMLColorTableColors2[j].name;
	BYTE* m_pBits=0;
	long H= 16;//32;
	long W= m_mapColors.size()*H;
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
	map<int, _bstr_t>::iterator i;
	long index=0;
#define red(c)		(0xFF & ((c)>>16))
#define green(c)	(0xFF & ((c)>>8))
#define blue(c)		(0xFF & (c))
	for (i = m_mapColors.begin(), index=0; i != m_mapColors.end(); i++, index++)
	{
//i->second);
		unsigned long c = i->first;
		HBRUSH	br			= CreateSolidBrush(RGB(red(c), green(c), blue(c)));
		HBRUSH hbrushOld	= (struct HBRUSH__ *) SelectObject( hDC, br );
//		FillRect(hDC, &rc, br);
		Rectangle(hDC, index*H, 0, (index+1)*H, H);
		SelectObject( hDC, hbrushOld );
	}
#undef red
#undef green
#undef blue

	DSStoreDIBSectionInBMPFile( _T("C:\\Projects\\ATL\\Act\\ListView.bmp"),  hBitmap );

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

	if (hBitmap) SelectObject( hDC, OldhBitmap );
	DeleteObject(hBitmap);
	free(pbmi);

	ReleaseDC(hdc);
	ReleaseDC(hDC);

	DeleteDC(hDC);


	return S_OK;
}

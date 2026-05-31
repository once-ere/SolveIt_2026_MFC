// MakeAVI.cpp : Implementation of CView
/*
21 Mar 01:
#include "mshtml.h"
is added as the FIRST file included in stdafx.h
to overcome a strange compiler error
*/
#include "stdafx.h"
#include "Scene.h"
#include "View.h"
#include "DibSectn.h"
///////////////////////////////////////////////////////////////////////////////
#include "Syntax.h"
#include "QM_1DScatt.h"
#include "QM_1D.h"
#include "Player.h"
//

/*
#include "vfw.h"
#include "digitalv.h"
static PAVISTREAM	PAVISTREAM(g_pavi)			= 0;	
static PAVISTREAM	PAVISTREAM(g_psCompressed)	= 0;	
static PAVIFILE		PAVIFILE(g_pfile)			= 0;
static bool			g_bInitCalled	= false;
*/

//void CreatePalette(int nColor, RGBQUAD* m_rgbPalette, HPALETTE& hpal);
//biNew.biSizeImage = ((((UINT)biNew.biBitCount * biNew.biWidth + 31)&~31) / 8) * biNew.biHeight
///////////////////////////////////////////////////////////////////////////////
// CView
///////////////////////////////////////////////////////////////////////////////
//void AVI_Term(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile)
///////////////////////////////////////////////////////////////////////////////
void CView::AVI_Term()
{
try{
	m_bMakingAVI = false;
	if (m_hdcMemAVI)
	{
		ATLASSERT(m_hBitmapOldAVI != 0);
		SelectObject(m_hdcMemAVI, m_hBitmapOldAVI); 
		if (m_hBitmapAVI) DeleteObject(m_hBitmapAVI);
	//	ReleaseDC(m_hdcMemAVI);
		DeleteDC(m_hdcMemAVI);
	}
	m_pBits			= 0;
	m_hBitmapAVI	= 0;
	m_hdcMemAVI		= 0;
	m_hBitmapOldAVI	= 0;
//	::AVI_Term();
//}
//void CView::AVI_Term()
//{
	if (g_aopts[0] != 0) AVISaveOptionsFree( 1, (LPAVICOMPRESSOPTIONS FAR *) &g_aopts);
	g_aopts[0] = 0;
	if (g_pavi) AVIStreamRelease(PAVISTREAM(g_pavi));
	if (g_psCompressed) AVIStreamRelease(PAVISTREAM(g_psCompressed));
	if (g_pfile) AVIFileRelease(PAVIFILE(g_pfile));	// closes the file 
	if (g_bInitCalled) AVIFileExit();			// releases AVIFile library 

	g_pavi			= 0;	
	g_psCompressed	= 0;	
	g_pfile			= 0;
	m_psCompressed		= 0;
	m_timeOfThisFrame	= 0;

	g_bInitCalled	= false;
}
catch (const _com_error &e) {
	if (!e.Error())
	::MessageBox(NULL,e.Description(),e.Source(),MB_ICONWARNING);
}
catch( SolveIt_Error& e){e.why();}catch(...) {
	::TimedMessageBox(NULL,"general exception","CView::AVI_Term",MB_ICONWARNING);
}
}
///////////////////////////////////////////////////////////////////////////////
//void AVI_Init(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile, const LPBITMAPINFO pbmi, _bstr_t bstr_AVI_FileName)
long CView::AVI_Init(const LPBITMAPINFO pbmi, _bstr_t bstr_AVI_FileName)
{
	TCHAR sz[256];
//	AVICOMPRESSOPTIONS	opts;
//	AVICOMPRESSOPTIONS	* aopts[1]		= {&opts};(PAVIFILE*)
	AVISTREAMINFO		strhdr;

	AVIFileInit();			// opens AVIFile library 

	HRESULT hr = AVIFileOpen(&g_pfile, bstr_AVI_FileName, OF_CREATE|OF_WRITE, NULL);
	if (hr != 0)
	{
		switch (hr) {
case AVIERR_BADFORMAT :	_tcscpy(sz,_T("The file couldn't be read, indicating a corrupt file or an unrecognized format."));break;
case AVIERR_MEMORY :	_tcscpy(sz,_T("The file could not be opened because of insufficient memory."));break;
case AVIERR_FILEREAD :	_tcscpy(sz,_T("A disk error occurred while reading the file."));break;
case AVIERR_FILEOPEN :	_tcscpy(sz,_T("A disk error occurred while opening the file."));break;
case REGDB_E_CLASSNOTREG :	_tcscpy(sz,_T("According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it. "));break;
		}
		::MessageBox(NULL,sz,"",MB_OK);
		return 1;
	}
// Fill in the header for the video stream....
// The video stream will run in 15ths of a second....
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType					= streamtypeVIDEO;// stream type
	strhdr.fccHandler				= 0;
	strhdr.dwScale					= 1;
	strhdr.dwRate					= 15;	// 15 fps
	strhdr.dwSuggestedBufferSize	= pbmi->bmiHeader.biSizeImage;
	::SetRect(&strhdr.rcFrame, 0, 0, (int) pbmi->bmiHeader.biWidth, (int) pbmi->bmiHeader.biHeight);
	
	hr = AVIFileCreateStream(g_pfile, &g_pavi, &strhdr);
	ATLASSERT(g_pavi != 0);
	if (hr != 0) {			//Stream created OK? If not, close file. 
		AVI_Term();
		MessageBox("AVIFileCreateStream: Output failed.","",MB_OK);
		return 2;
	} 

/*PAVIFILE()(PAVISTREAM*)PAVISTREAM()
#define MAKEFOURCC(ch0, ch1, ch2, ch3)  \ 
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |  \ 
    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 )); 
	AVICOMPRESSOPTIONS g_opts = {
		streamtypeVIDEO,
		MAKEFOURCC('M','S','V','C'),
		DWORD  (1), 
		DWORD  (100), 
		DWORD  (1), 
		DWORD  (0), 
		LPVOID (0), 
		DWORD  (0), 
		LPVOID (0), 
		DWORD  (0), 
		DWORD  (0)};(PAVISTREAM*)
*/
	memset(&g_opts, 0, sizeof(g_opts));

	g_aopts[0] = &g_opts;

	if (FALSE ==
	AVISaveOptions(m_hWnd, ICMF_CHOOSE_PREVIEW, 1, &g_pavi, (LPAVICOMPRESSOPTIONS FAR *) &g_aopts)
	)
	{
		AVI_Term();
		MessageBox("AVISaveOptions: Output failed.","",MB_OK);
		return 3;
	}

	hr = AVIMakeCompressedStream(&g_psCompressed, g_pavi, &g_opts, NULL);
	if (hr != 0) {			//Stream created OK? If not, close file. 
		AVI_Term();
		MessageBox("AVIMakeCompressedStream: Output failed.","",MB_OK);
		return 4;
	} 
/*(PAVISTREAM*)PAVISTREAM()
//	LONG SizeBytesBlockMemoryReferencedBylpFormat = 
//		bi.bmiHeader.biSize + bi.bmiHeader.biClrUsed * sizeof(RGBQUAD);
//pbmi == &pbmi->bmiHeader
	hr = AVIStreamSetFormat(PAVISTREAM(g_psCompressed), 0, &pbmi->bmiHeader, sizeof(pbmi->bmiHeader));
//	hr = AVIStreamSetFormat(PAVISTREAM(g_psCompressed), 0, pbmi, nTotalSize);BITMAPINFOHEADER
*/
// sizeof(BITMAPINFO) + ((bits <= 8) ? 1<<bits : 0) * sizeof(RGBQUAD)
/////////////////////////////////////////////////////////////////////////////
static bool m_ThisIsWin95 = ( 0xC0000000 == (GetVersion() & 0xC0000000) )? 1:0;
static bool m_ThisIsWinNT = ( 0 == (GetVersion() & 0xC0000000) )? 1:0;
/////////////////////////////////////////////////////////////////////////////
/*
	long nTotalSize = DSTotalBytesSize( pbmi );
if (m_ThisIsWin95)
	hr = AVIStreamSetFormat(PAVISTREAM(g_psCompressed), 0,
					 pbmi,
					 nTotalSize
					 );
/////////////////////////////////////////////////////////////////////////////
if (m_ThisIsWinNT)
	hr = AVIStreamSetFormat(PAVISTREAM(g_psCompressed), 0,
					 &pbmi->bmiHeader,
					 sizeof(BITMAPINFOHEADER)PAVISTREAM()
					 );
*/
///////////////////////////////////////////////////////////////////////////////
	hr = AVIStreamSetFormat(g_psCompressed, 0,
					 &pbmi->bmiHeader,
					 sizeof(pbmi->bmiHeader)
					 );
///////////////////////////////////////////////////////////////////////////////
	if (hr != 0) { 
		AVI_Term();
		MessageBox("AVIStreamSetFormat: Output failed.","",MB_OK);
		return 5; 
	} 
///////////////////////////////////////////////////////////////////////////////
//	SetCursor(LoadCursor(NULL, IDC_WAIT));
///////////////////////////////////////////////////////////////////////////////
	g_bInitCalled	= true;
	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
void CView::AVIStreamWrite()
{
	::AVIStreamWrite(	PAVISTREAM(g_psCompressed),				// stream pointer
						m_timeOfThisFrame,			// time of this frame
						1,							// number to write
						(LPVOID) m_pBits,
						m_nImageSize,				// size of this frame
						AVIIF_KEYFRAME,				// flags....
						NULL, NULL);
	++m_timeOfThisFrame;
//	if (m_timeOfThisFrame>47) 	AVI_Term();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CView::MakeAVI(BSTR strText, long *pVal)
{

UINT_PTR WINAPI SaveAs_OnFileOK (HWND hWnd, OFNOTIFY *pofn);
UINT_PTR WINAPI SaveAs_OnNotify (HWND hWnd, NMHDR *phdr);
UINT_PTR CALLBACK SaveAsHookProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

try{
//toggle:
	if (m_bMakingAVI)
	{
		AVI_Term();
		*pVal = 0;
		return S_OK;
	}

//////////////////////////////////////////////////////////////////////////////
	m_bstrAVIFileName = strText;
//////////////////////////////////////////////////////////////////////////////
	{
//		DWORD res = GetFileAttributes(m_bstrAVIFileName);
//		if (res != 0xFFFFFFFF)
		{

	TCHAR szFileName[MAX_PATH] = TEXT("");
	TCHAR szTitleName[ _MAX_FNAME + _MAX_EXT ] = TEXT("");
	LPSTR szTitle=0;
	GetFullPathName( m_bstrAVIFileName, _MAX_PATH, szFileName, &szTitle );
	strncpy( szTitleName, szTitle, _MAX_FNAME + _MAX_EXT );
	szTitleName[ _MAX_FNAME + _MAX_EXT -1 ] = '\0';
	BOOL bResult = FALSE;
	DWORD dwError = NOERROR;
	OPENFILENAME ofn = {0};

	ZeroMemory(&ofn, sizeof ofn);
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.lpstrFilter = TEXT("All Files\0*.*\0\0");
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | 
				OFN_ENABLEHOOK | 
				OFN_HIDEREADONLY | 
				OFN_NOCHANGEDIR | 
				OFN_PATHMUSTEXIST;


	HWND hWndCtl = m_hWnd;
	if ( ::GetParent(hWndCtl) != 0)
	{
		do
		{
			hWndCtl = ::GetParent(hWndCtl);
		}
		while (::GetParent(hWndCtl) != 0);
	}
	ofn.hwndOwner = hWndCtl; 
  

	ofn.lpfnHook = (LPOFNHOOKPROC) SaveAsHookProc;

	bResult = GetSaveFileName (&ofn);
	if (bResult == 0)
	{
		AVI_Term();
		*pVal = 0;
		return S_OK;
		dwError = CommDlgExtendedError ();
		return E_FAIL;
	}

//	MessageBox (NULL, szFileName, TEXT("SaveAs returned..."), MB_OK);
//	MessageBox (NULL, szTitleName, TEXT("SaveAs returned..."), MB_OK);
	m_bstrAVIFileName=szFileName;
 
		//	WIN32_FIND_DATA FindFileData;
		//	FindClose(FindFirstFile(m_bstrAVIFileName,&FindFileData));
		//	DeleteFile(m_bstrAVIFileName);
		}
//////////////////////////////////////////////////////////////////////////////
	DWORD res = GetFileAttributes(m_bstrAVIFileName);
	if (res != 0xFFFFFFFF)
	{
		WIN32_FIND_DATA FindFileData;
		FindClose(FindFirstFile(m_bstrAVIFileName,&FindFileData));
		DeleteFile(m_bstrAVIFileName);
	}
	}
//////////////////////////////////////////////////////////////////////////////

	HDC hdc = GetDC();
	m_hdcMemAVI = CreateCompatibleDC(hdc);
	ATLASSERT(m_hdcMemAVI != 0);
	ReleaseDC(hdc);

	RECT rc = {0,0,500,500};
	GetClientRect(&rc);
	long wXSize= rc.right;
	long wYSize= rc.bottom;

	m_hBitmapAVI = DSCreateDIBSection( wXSize, wYSize, 24);
	ATLASSERT(m_hBitmapAVI != 0);
	if (m_hBitmapAVI==0) return (*pVal = 0, E_FAIL);
	m_pBits = DSGetPointerToDIBSectionImageBits(m_hBitmapAVI);
	ATLASSERT(m_pBits != 0);
	if (m_pBits==0)
	{
		AVI_Term();
		*pVal = 0;
		return S_OK;
		return E_FAIL;
	}
	m_hBitmapOldAVI = (struct HBITMAP__ *) SelectObject(m_hdcMemAVI, m_hBitmapAVI); 

	BITMAP bmInfo ;

	GetObject(m_hBitmapAVI, sizeof(BITMAP), &bmInfo) ;
	ATLASSERT(bmInfo.bmPlanes == 1) ;
	ATLASSERT((bmInfo.bmBitsPixel == 8) || 
		  (bmInfo.bmBitsPixel == 16) ||
		  (bmInfo.bmBitsPixel == 24) ||
		  (bmInfo.bmBitsPixel == 32)
		  );


		
	LPBITMAPINFO pbmi = DSGetBITMAPINFOForDIBSection( m_hBitmapAVI );
	ATLASSERT(pbmi != 0);
	if (pbmi==0) E_FAIL;
	ATLASSERT (pbmi->bmiHeader.biCompression == BI_RGB);// Wrong compression format? 
	m_nImageSize = DSImageBitsSize( pbmi );
	ATLASSERT(m_nImageSize == pbmi->bmiHeader.biSizeImage);
	long nTotalSize = DSTotalBytesSize( pbmi );
/////////////////////////////////////////////////////////////////////////////
	long ret = AVI_Init( pbmi, m_bstrAVIFileName);
	if (ret)
	{
		AVI_Term();
		*pVal = 0;
		return S_OK;
		return E_FAIL;
	}
//	m_psCompressed		= PAVISTREAM(g_psCompressed);
//	m_timeOfThisFrame	= 0;
	m_bMakingAVI		= true;
/*
// Now write out each video frame, along with a text label.
// The video frames are 2/3 of a second apart, which is 10
// in the video time scale and 40 in the text stream's time scale.
	HRESULT hr = AVIStreamWrite(PAVISTREAM(g_psCompressed),	// stream pointer
			k * 1,				// time of this frame
			1,				// number to write
			(LPVOID) m_pBits,
			nImageSize,//lpbi->bmiHeader.biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
*/
}
catch (const _com_error &e) {
	if (!e.Error())
	::MessageBox(NULL,e.Description(),e.Source(),MB_ICONWARNING);
}
catch( SolveIt_Error& e){e.why();}catch(...) {
	::TimedMessageBox(NULL,"general exception","CView::MakeAVI",MB_ICONWARNING);
}
	*pVal = 1;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
// CQM_1DScatt
#if 0
void CQM_1DScatt::Compute()
{
	ATLASSERT( m_nNumNodes > 0);
try{
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	DATA_PASS_mp_SETUP ();

	HDC hdc = m_pCView->GetDC();
	HDC hDC = CreateCompatibleDC(hdc);

//	_bstr_t m_bstrAVIFileName(L"flic1D.avi");
	DWORD res = GetFileAttributes(m_bstrAVIFileName);
	if (res != 0xFFFFFFFF)
	{
		WIN32_FIND_DATA FindFileData;
		FindClose(FindFirstFile(m_bstrAVIFileName,&FindFileData));
		DeleteFile(m_bstrAVIFileName);
	}
	
	long bits = 24, k=0;
	BYTE* m_pBits=0;
	long wXSize= m_nNumNodes;//==m_WavePacket.size();
	long wYSize= m_nHeight;

	HBITMAP hBitmap = DSCreateDIBSection( wXSize, wYSize, bits);//, m_pBits, bi);
	ATLASSERT(hBitmap != 0);
	if (hBitmap==0) return;
	m_pBits = DSGetPointerToDIBSectionImageBits(hBitmap);
	ATLASSERT(m_pBits != 0);
	if (m_pBits==0) return;

	LPBITMAPINFO pbmi = DSGetBITMAPINFOForDIBSection( hBitmap );
	ATLASSERT(pbmi != 0);
	if (pbmi==0) return;
	long nImageSize = DSImageBitsSize( pbmi );
	long nTotalSize = DSTotalBytesSize( pbmi );
	ATLASSERT(pbmi->bmiHeader.biSizeImage == nImageSize);
/////////////////////////////////////////////////////////////////////////////
	HPEN	penRed		= CreatePen(PS_SOLID,0,RGB(255,0,0));
	HPEN	penGreen	= CreatePen(PS_SOLID,0,RGB(0,255,0));
	HPEN	penGray		= CreatePen(PS_SOLID,0,RGB(200,200,200));
	HPEN	penBlk		= CreatePen(PS_SOLID,0,RGB(0,0,0));
	HPEN	penYel		= CreatePen(PS_SOLID,0,RGB(155,155,0));
	HPEN	penBlue		= CreatePen(PS_SOLID,0,RGB(0,0,255));
	HBRUSH	br			= CreateSolidBrush(RGB(255,255,255));

	HBITMAP OldhBitmap	= (struct HBITMAP__ *) SelectObject( hDC, hBitmap/*hDib*/ );
	HPEN hpenOld		= (struct HPEN__ *) SelectObject( hDC, penRed );
	HBRUSH hbrushOld	= (struct HBRUSH__ *) SelectObject( hDC, br );


	HFONT	font = CreateFont(
		-12,
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
	HFONT	hfontOld = (struct HFONT__ *)SelectObject(hDC, font);
	
	RECT rc = {0,0,wXSize,wYSize};
//	char szNumber[6];
	POINT * ppt =	new POINT[m_nNumNodes];
/////////////////////////////////////////////////////////////////////////////
#if 0
	PAVISTREAM			PAVISTREAM(g_pavi)			= NULL;	
	PAVISTREAM			PAVISTREAM(g_psCompressed)	= NULL;	
	PAVIFILE			PAVIFILE(g_pfile)=0;
	AVI_Init( PAVISTREAM(g_pavi), PAVISTREAM(g_psCompressed), PAVIFILE(g_pfile), pbmi, m_bstrAVIFileName);
#endif
///////////////////////////////////////////////////////////////////////////////
	m_bDrawUsingInitial = false;

	for (k=0; k<m_nNumFrames; k++ )
	{
		Draw(k);
//		GdiFlush();
#if 0
		BitBlt( hDC, 0, 0, wXSize, wYSize,
				m_pCView->m_hdcMem,
				0, 0, SRCCOPY );
		wsprintf(szNumber,"%04u",k) ;

		SetBkColor(hDC, RGB(0,0,255)) ;
		SetTextColor(hDC, RGB(255,255,0)) ;
		ExtTextOut(
			hDC,
			rc.left+(rc.right-rc.left)/20,
			rc.bottom-(rc.bottom-rc.top)/10,
			ETO_CLIPPED,
			&rc,
			szNumber,
			4,
			NULL);

// Now write out each video frame, along with a text label.
// The video frames are 2/3 of a second apart, which is 10
// in the video time scale and 40 in the text stream's time scale.
		hr = AVIStreamWrite(PAVISTREAM(g_psCompressed),	// stream pointer
			k * 1,				// time of this frame
			1,				// number to write
			(LPVOID) m_pBits,
			nImageSize,//lpbi->bmiHeader.biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
#endif
///////////////////////////////////////////////////////////////////////////////
		long rep = 64;
		switch (m_EvolutionMethod)
		{
		case Nash: DATA_PASS_mp_EVOLVE_NASH(&rep);
			break;
		case Cayley: DATA_PASS_mp_EVOLVE_CAYLEY(&rep);
			break;
		case PeriodicCayley: DATA_PASS_mp_EVOLVE_CAYLEY_PERIODIC(&rep);
			break;
		}

		TCHAR buf[256];
	//	if (m_EvolutionMethod == Nash)
		{
			double pr = 0;
			complexdouble* psi = (complexdouble*) DATA_PASS_mp_PPSI;
			for (long x =0;x<m_nNumNodes;++x)
			{
				complexdouble C = psi[x];
				pr += C.real()*C.real() + C.imag()*C.imag();
			}
			_stprintf(buf, "Frame[%d] Integral psi* psi dx = %19.16lg", k,pr/m_nNumNodes);
		}
	//	else
	//	_stprintf(buf, "Frame[%d]", k);
		_bstr_t bstr(buf);
	//	m_pCView->WriteBrowserStatusLine(bstr);
	}
 	m_bDrawUsingInitial = true;
//	//	theSystem->m_pIUserIO->ReDraw();
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#if 0
	AVI_Term( PAVISTREAM(g_pavi), PAVISTREAM(g_psCompressed), PAVIFILE(g_pfile));
#endif
///////////////////////////////////////////////////////////////////////////////
	OnCloseMovie();

	res = GetFileAttributes(m_bstrAVIFileName);
	if (res == 0xFFFFFFFF)
	{
		goto theEnd;
	}
	{
		CQuartzMovieCtl q;
		q.m_bstrFilenameAVI = m_bstrAVIFileName;
		q.m_nNumFrames = m_nNumFrames;
		q.DoModal();
//	CQuartzMovieCtl* pcl = new CQuartzMovieCtl(this);
//	pcl->Create(m_hWnd);
//	pcl->ShowWindow(SW_SHOW);
//	pcl->CenterWindow();
//	delete pcl;
	}
	
#if 0
	HWND hwnd;
	hr = m_pCView->get_HWND((long*)&hwnd);
	if (!SUCCEEDED(hr)) hwnd = 0;
// if the MCIWnd does not exist, create it.
//	if (m_videoWnd == NULL)
		m_videoWnd = MCIWndCreate(	hwnd,
									globalhInstance,
									WS_CHILD | WS_CAPTION| WS_BORDER | WS_VISIBLE | 
									MCIWNDF_SHOWPOS | MCIWNDF_SHOWNAME
									|MCIWNDF_SHOWALL | MCIWNDF_NOTIFYALL,
									m_bstrAVIFileName.c_str()
									);
/*
// Otherwise, pause or resume play as needed
	else
	{
		if (m_paused)
		{
			MCIWndPlay(m_videoWnd);
			m_paused = false;
		}
		else
		{
			MCIWndPause(m_videoWnd);
			m_paused = true;
		}
	}
*/
		{
	::SetWindowPos(m_videoWnd, HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	CWindow w;
	w.Attach(m_videoWnd);
	w.SetFocus();
	w.CenterWindow();
	w.UpdateWindow();
	w.Detach();
		}
	MCIWndPlay(m_videoWnd);
#endif

theEnd:

	SelectObject( hDC, OldhBitmap );
	DeleteObject(hBitmap);
	free(pbmi);

	SelectObject(hDC, hfontOld);
	DeleteObject(font);

	SelectObject( hDC, hpenOld );
	SelectObject( hDC, hbrushOld );
	DeleteObject( penRed );
	DeleteObject( penGreen );
	DeleteObject( penGray );
	DeleteObject( penBlk );
	DeleteObject( penBlue );
	DeleteObject( penYel );
	DeleteObject( br );


	ReleaseDC(m_pCView->m_hWnd, hdc);
	ReleaseDC(m_pCView->m_hWnd, hDC);

	DeleteDC(hDC);

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
catch(...)
{}
}



/*
//	-N <= n <= N,	 dx = L / 2*N
//	PERIODIC:
//	k * L = 2 * Pi * µ, µ = ±1, ±2, ...
//	k = 2 * Pi * µ / L, µ = ±1, ±2, ...
//	MAXIMUM WAVENUMBER:
//	x[n] = n * dx
//	k * x[n] = n * ( k * dx )
//	Given K such that K * dx = 2 * Pi, then exp( i K * x[n] ) = 1
//	and wave function is same as k = 0 wave fn
//	K = 2 * Pi / dx = 2*Pi * 2*N / L = 4*Pi*N / L
//	0 <= k <= K corresponds to 0 <= ß <= 2 Pi
//	0 < k < ½ K corresponds to positive velocities
//	¼ K corresponds to maximum positive velocity
//	¼ K = ¼ [ 2*Pi * 2*N / L ] = 2*Pi * (N/2) / L
//	½ K < k < K corresponds to negative velocities
//	therefore k[n] = n * 2 Pi / L
//	-½N <= n <= ½N
//	gives ½ cycle from min Vx to max Vx

void CQmsView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CString sz;
	int k=m_nNumPosNodes/2;
	if (pScrollBar==(CScrollBar*)GetDlgItem(IDC_SCROLLBAR_WN)) {
	if (&m_sbarWavenumber != pScrollBar) return;
	int iPos = pScrollBar->GetScrollPos();
	switch (nSBCode) {
	case SB_TOP:
		iPos = k;
		break;
	case SB_LINEDOWN:
		iPos++;
		break;
	case SB_PAGEDOWN:
		iPos += 10;
		break;
	case SB_BOTTOM:
		iPos = -k;
		break;
	case SB_LINEUP:
		iPos--;
		break;
	case SB_PAGEUP:
		iPos -= 10;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iPos = nPos;
		break;
	default:
		break;
	}
	if (iPos < -k) iPos = -k;
	if (iPos >	k) iPos =	k;
	pScrollBar->EnableScrollBar( ESB_ENABLE_BOTH );
	pScrollBar->SetScrollPos(iPos, TRUE);
	UpdateData(TRUE);
	m_fWaveNumber = Pi2Over10*m_nWaveNumber;
	sz.Format("%lf",m_fWaveNumber);
	m_editWaveNumber.SetWindowText(sz);
}
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	return;
}
*/

DWORD PlayFlick(LPSTR path,HWND hwnd, long videoID)
{
	MCI_DGV_OPEN_PARMS mciopen;
	MCI_DGV_PLAY_PARMS mciplay;
	MCI_GENERIC_PARMS mcigen;
	DWORD rtrn;
#define STRINGSIZE 1024
	char b[STRINGSIZE+1];

	if(videoID != -1) {
		mcigen.dwCallback=(long)hwnd;
		mciSendCommand(videoID,MCI_STOP,0,
						 (DWORD)(LPVOID)&mcigen);
		videoID=-1;
	}

	mciopen.lpstrDeviceType="avivideo";
	mciopen.lpstrElementName=path;
	if((rtrn=mciSendCommand(0,MCI_OPEN,
			MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
			(DWORD)(LPVOID)&mciopen)) != 0L) {
		mciGetErrorString(rtrn,(LPSTR)b,STRINGSIZE);
		::MessageBox(NULL,b,"",MB_OK);
		return(0L);
	}

	videoID=mciopen.wDeviceID;

	mciplay.dwCallback=(DWORD)hwnd;
	if((rtrn=mciSendCommand(videoID,MCI_PLAY,
			MCI_NOTIFY,(DWORD)(LPVOID)&mciplay)) != 0L) {
		mciSendCommand(videoID,MCI_CLOSE,0,NULL);
		mciGetErrorString(rtrn,(LPSTR)b,STRINGSIZE);
		::MessageBox(NULL,b,"",MB_OK);
		return(0L);
	}
	return(1L);
}

/*

void CVideo2View::OnFileOpen() 
{
	CFileDialog dlg(TRUE, "avi", "*.avi");
	if (dlg.DoModal()==IDOK)
	{
	// if the MCIWnd exists, destroy it
	if (m_videoWnd != NULL)
	{
		MCIWndDestroy(m_videoWnd);
		m_videoWnd = NULL;
	}
	m_videoWnd = MCIWndCreate(this->GetSafeHwnd(),
		AfxGetInstanceHandle(),
		WS_CHILD | WS_CAPTION | WS_VISIBLE | 
MCIWNDF_SHOWPOS | MCIWNDF_SHOWNAME,
		dlg.GetPathName());
	}
}
*/
#endif

// ReadData.cpp : implementation of the CQmsView class
//

#include "stdafx.h"
#include "Action.h"
#include "GLView.h"
#include "QM_2DScatt.h"
//#include "DibSectn.h"
//#include "hipng.h"
//winmm.lib 

#include "hdf5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/*
//Select a hyperslab.
	hid_t filespace = H5Dget_space (dataset_id);
	offset[0] = 0;
	offset[1] = 0;
	status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, NULL, dims1, NULL);

//Compression is not implemented yet. Release 1.2.1

	hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_deflate (plist, 1 - 9);
	status = H5Pclose(plist);

int H5Sget_simple_extent_ndims (hid_t space) 
This function determines the dimensionality (or rank) of a dataspace_id. 
Returns the number of dimensions in the dataspace_id, negative on failure. 
herr_t H5Sget_simple_extent_dims (hid_t space, hsize_t *dims, hsize_t *max) 
The function retrieves the size of the extent of the dataspace_id space by 
placing the size of each dimension in the array dims. Also retrieves the size 
of the maximum extent of the dataspace_id, placing the results in max. 
Returns non-negative on success, negative on failure. 
*/
/////////////////////////////////////////////////////////////////////////////
// CQM_2DScatt
void CQM_2DScatt::ReadData(long nFrame)
{
char *GetHDF5Filename (void);
//typedef cdouble (*MATRIX_Complex)[QMEvolve2D.NDATA];
//	MATRIX_Complex Y = (MATRIX_Complex) DATA_2D_mp_PPSI;
/////////////////////////////////////////////////////////////////////////////
	long k= nFrame;
	char szNumber[16];

	wsprintf(szNumber,"%04u",k) ;

	string hdf5fn((const char*)m_bstrHDF5FileName);
//	string grpName = "/Frames";
	string dsName = "/Frames/";
	dsName += szNumber;

// handles:
	hid_t	file_id=0;
	hid_t	group_id=0;
	hid_t	dataset_id=0;
	hid_t	dataspace_id=0;
	hid_t	datatype_id=0;
	hid_t	memspace_id=0;
	hid_t	attribute_id=0;

//	hsize_t	dimsf[3]={QMEvolve2D.NDATA,QMEvolve2D.NDATA, 2};//dataset_id dimensions
	hsize_t	dimsf[2]={QMEvolve2D.NDATA,QMEvolve2D.NDATA};//dataset_id dimensions
	herr_t	status=0;                             
//	hsize_t		count[2]={QMEvolve2D.NDATA,QMEvolve2D.NDATA};//size of the hyperslab in the file_id
//	hssize_t	offset[2]={0,0};	//hyperslab offset in the file_id
//	hsize_t		count_out[2]={QMEvolve2D.NDATA,QMEvolve2D.NDATA};//size of the hyperslab in memory
//	hssize_t	offset_out[2]={0,0};//hyperslab offset in memory
/////////////////////////////////////////////////////////////////////////////////
try {

	file_id			= H5Fopen(hdf5fn.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	if (file_id<0)
	{
		_variant_t v = (long) (file_id);
		_bstr_t err=v;
		_bstr_t fn(hdf5fn.c_str());
		_bstr_t b = L"H5Fopen error: \n" + fn + L" not found\n";
		b+=err;
		MessageBox(0,b,"",MB_OK);
//		m_bstrHDF5FileName = (const char*) GetHDF5Filename ();
		put_HDF5FileName(BSTR(_bstr_t((const char*) GetHDF5Filename ())) ) ;
		hdf5fn = (const char*) m_bstrHDF5FileName;
		file_id = H5Fopen(hdf5fn.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
		if (file_id<0)
		{
			_bstr_t fn(hdf5fn.c_str());
			_bstr_t b = L"H5Fopen error: Cannot open \n" + fn + L" \n";
			MessageBox(0,b,"",MB_OK);
			return;
		}
	}

	group_id = H5Gopen(file_id, m_groupTag.c_str());
	if (group_id<0)
	{
		_variant_t v = (long) (group_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Gopen(file_id, '/Frames') error:\n");
		b+=err;
		throw CMyException(b);
	}


	attribute_id = H5Aopen_name(group_id, m_attributeTag.c_str());
	if (attribute_id<0)
	{
		_variant_t v = (long) (attribute_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Aopen_name 'Number of Frames' error: ");
		b+=err;
		throw CMyException(b);
	}
	long nNumFrames;
	status  = H5Aread(attribute_id, H5T_NATIVE_INT, &nNumFrames);
	if (status<0)
	{
		_variant_t v = (long) (status);
		_bstr_t err=v;
		_bstr_t b(L"H5Aread(attribute_id, H5T_NATIVE_INT, &nNumFrames) error: ");
		b+=err;
		throw CMyException(b);
	}
/*
	else {
		_variant_t v = (long) (nNumFrames);
		_bstr_t b=v;
		MessageBox(b);
	}
*/
	put_NumberOfFrames(nNumFrames);
	if (nFrame >= m_nNumFrames)
	{
		_variant_t v = (long) (nFrame);
		_bstr_t err=v;
		_bstr_t b(L"Error: nFrame = ");
		b+=err;
		b+=L"  >= m_nNumFrames ";
		throw CMyException(b);
	}
//	status = H5Aclose(attribute_id);
//	status = H5Gclose(group_id);
	memspace_id		= H5Screate_simple(sizeof(dimsf)/sizeof(dimsf[0]), dimsf, NULL);
	if (memspace_id<0)
	{
		_variant_t v = (long) (memspace_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Screate_simple error: ");
		b+=err;
		throw CMyException(b);
	}
//	status			= H5Sselect_hyperslab(memspace_id, H5S_SELECT_SET, offset_out, NULL, count_out, NULL);
	status			= H5Sselect_all(memspace_id);

//	dataset_id		= H5Dopen(file_id, dsName.c_str());
	dataset_id		= H5Dopen(group_id, szNumber);
	if (dataset_id<0)
	{
		_variant_t v = (long) (dataset_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Dopen error: ");
		b+=err;
		throw CMyException(b);
	}
//	else MessageBox(szNumber);

	datatype_id		= H5Dget_type(dataset_id);
	if (datatype_id<0)
	{
		_variant_t v = (long) (datatype_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Dget_type(dataset_id error: ");
		b+=err;
		throw CMyException(b);
	}
	status = H5Tpack(datatype_id);
	size_t datatype_size = H5Tget_size(datatype_id);
	if (datatype_size != sizeof(cdouble))
	{
		_variant_t v = (long) (datatype_size);
		_bstr_t err=v;
		_bstr_t b(L"datatype_size != sizeof(cdouble):\n");
		b+=err;
		throw CMyException(b);
	}

//	status = H5Tpack(datatype_id);


	dataspace_id	= H5Dget_space(dataset_id);
	if (dataspace_id<0)
	{
		_variant_t v = (long) (dataspace_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Dget_space(dataset_id error: ");
		b+=err;
		throw CMyException(b);
	}

	int rank		= H5Sget_simple_extent_ndims (dataspace_id);
	hsize_t max;
//	hsize_t *dims	= new hsize_t[rank];delete[] dims;
//	status			= H5Sget_simple_extent_dims (dataspace_id, dims, &max);
		vector<hsize_t> dims;
		dims.reserve(rank);
		ASSERT(rank==dims.capacity());
	status			= H5Sget_simple_extent_dims (dataspace_id, &dims.front(), &max);

	if (status<0)
	{
		_variant_t v = (long) (status);
		_bstr_t err=v;
		_bstr_t b(L"H5Sget_simple_extent_dims error: ");
		b+=err;
		throw CMyException(b);
	}

		ASSERT(QMEvolve2D.NDATA==dims[0]);
		ASSERT(QMEvolve2D.NDATA==dims[1]);
	//	ASSERT(2==dims[2]);
		if (QMEvolve2D.NDATA!=dims[0]) throw CMyException("QMEvolve2D.NDATA!=dims[0]");
		if (QMEvolve2D.NDATA!=dims[1]) throw CMyException("QMEvolve2D.NDATA!=dims[1]");
	//	if (2!=dims[2]) throw CMyException("2!=dims[2]");

	Clear();
//	m_pExternal = reinterpret_cast<MATRIX_Complex>(new cdouble[QMEvolve2D.NDATA*QMEvolve2D.NDATA]);


//typedef cdouble ComplexRow[QMEvolve2D.NDATA];
//MATRIX_Complex Y = new ComplexRow[QMEvolve2D.NDATA];

//	m_pExternal = reinterpret_cast<cdouble**>(new cdouble[QMEvolve2D.NDATA*QMEvolve2D.NDATA]);
//	m_pExternal = new cdouble*[QMEvolve2D.NDATA];
//	m_pExternal = new ComplexRow[QMEvolve2D.NDATA];
//	if (!m_pExternal) throw -1;//new CMyException;
//_variant_t v = (long) sizeof(ComplexRow);
//_bstr_t b=v;
//MessageBox(b);

/*
	for (long j=0;j<QMEvolve2D.NDATA;++j)
	{
		m_pExternal[j] = new cdouble[QMEvolve2D.NDATA];
		if (!m_pExternal[j]) throw -1;
	}
*/
//Read data from hyperslab in the file_id into the hyperslab in memory and display.
//	status	= H5Dread(dataset_id, datatype_id, memspace_id, dataspace_id, H5P_DEFAULT, m_pExternal);
//	status	= H5Dread(dataset_id, datatype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_pExternal);
//	status	= H5Dread(dataset_id, datatype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, (void*)DATA_2D_mp_PHDF5_FRAME);
//	m_pExternal=(MATRIX_Complex)DATA_2D_mp_PHDF5_FRAME;
	status	= H5Dread(dataset_id, datatype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_pExternal);

	if (status<0)
	{
		_variant_t v = (long) (status);
		_bstr_t err=v;
		_bstr_t b(L"H5Dread error: ");
		b+=err;
		throw CMyException(b);
	}
//	for (long j=0;j<QMEvolve2D.NDATA;++j)
//		for (long k=0;k<QMEvolve2D.NDATA;++k)
//			m_pExternal[j][k].x = m_pExternal[j][k].y = 0;
	QMEvolve2D.m_DrawSource = QMEvolve2D.External;
	Fire_ViewChange();
    /*
     * Close/release resources.
     */
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
//////////////////////////////////////////////////////////////////////////////

}
//catch(CMyException* e){e->Delete();}
catch( CMyException E )
{
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
	MessageBox(0,E.ShowReason(),"",MB_OK);
	return;
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
	return;
}


#if 0
		{
	BYTE* m_pBits=0;
	long W= m_nWidth;
	long H= m_nHeight;
	HDC hdc = GetDC();
	HDC hDC = CreateCompatibleDC(hdc);
	long bits = 24;
	HBITMAP hBitmap = DSCreateDIBSection( W, H, bits);
	ASSERT(hBitmap != 0);
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
		return;
	}
	LPBITMAPINFO pbmi = DSGetBITMAPINFOForDIBSection( hBitmap );
	ASSERT(pbmi != 0);
//MessageBox("if (pbmi==0) return;");
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
		return;
	}
	long nImageSize = DSImageBitsSize( pbmi );
	long nTotalSize = DSTotalBytesSize( pbmi );
	ASSERT(nImageSize == pbmi->bmiHeader.biSizeImage );

	m_pBits = DSGetPointerToDIBSectionImageBits(hBitmap);
	ASSERT(m_pBits != 0);
//MessageBox("if (m_pBits==0) return;");
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
		return;
	}



	DrawSource oldDrawSource = m_DrawSource;
	m_DrawSource = External;
	HBITMAP OldhBitmap	= (struct HBITMAP__ *) SelectObject( hDC, hBitmap );
//CConfigGL::SetUpMemDrawingEnvironment(HDC hdc, int cx, int cy)
//m_hMemDC != hdc;
try{
//	ConfigGL.SetUpMemDrawingEnvironment(hDC, W, H);
	Draw(hDC, true);
		GdiFlush();
	//	BitBlt( hDC, 0, 0, W, H,
	//			ConfigGL.m_hdcMem,
	//			0, 0, SRCCOPY );
} catch( SolveIt_Error& e){e.why();}catch(...)
{
	MessageBox("error: Draw(hDC, true)");
		m_DrawSource = oldDrawSource;

		SelectObject( hDC, OldhBitmap );
		DeleteObject(hBitmap);
		free(pbmi);

		ReleaseDC(hdc);
		ReleaseDC(hDC);

		DeleteDC(hDC);
		return;
}
	m_DrawSource = oldDrawSource;

			string spng("C:\\My Documents\\");
			spng += dsName;
			spng += ".png";
	//		MessageBox(spng.c_str());
			HIPNG_DIB s;
			ZeroMemory(&s, sizeof(s));
			TCHAR buf[MAX_PATH];
			_tcscpy(buf, spng.c_str());
			s.file_name=buf;
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
} catch( SolveIt_Error& e){e.why();}catch(...)
{
	MessageBox("Exception: write_dib_to_png");
}

	SelectObject( hDC, OldhBitmap );
	DeleteObject(hBitmap);
	free(pbmi);

	ReleaseDC(hdc);
	ReleaseDC(hDC);

	DeleteDC(hDC);
		}
#endif
}

//----------------------------------------------------------------
//	Routine to prompt the user for a filename to create
//	If unsuccessful, return FALSE
//	If successful, return TRUE with ptr to filename in fn
//    const char szPngFilter[] = "HDF5 Files (*.h5)\0*.h5\0"    "All Files (*.*)\0*.*\0\0" ;

char *GetHDF5Filename (void)
{

	static	OPENFILENAME	ofn_hdf5;
    const char szPngFilter[] = "HDF5 Files (*.h5)\0*.h5\0\0";

	static char	szFileName [_MAX_PATH];

	szFileName [0] = '\0';
	ofn_hdf5.lStructSize       = sizeof (OPENFILENAME) ;
	ofn_hdf5.hwndOwner         = NULL ;
	ofn_hdf5.hInstance         = NULL ;
	ofn_hdf5.lpstrFilter       = szPngFilter ;
	ofn_hdf5.lpstrCustomFilter = NULL ;
	ofn_hdf5.nMaxCustFilter    = 0 ;
	ofn_hdf5.nFilterIndex      = 0 ;
	ofn_hdf5.lpstrFile         = szFileName;	// Set in Open and Close functions
	ofn_hdf5.nMaxFile          = _MAX_PATH ;
	ofn_hdf5.lpstrFileTitle    = NULL ;		// Set in Open and Close functions
	ofn_hdf5.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
	ofn_hdf5.lpstrInitialDir   = NULL ;
	ofn_hdf5.lpstrTitle        = NULL ;
//	ofn_hdf5.Flags             = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ;
	ofn_hdf5.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
	ofn_hdf5.nFileOffset       = 0 ;
	ofn_hdf5.nFileExtension    = 0 ;
	ofn_hdf5.lpstrDefExt       = "h5" ;
	ofn_hdf5.lCustData         = 0L ;
	ofn_hdf5.lpfnHook          = NULL ;
	ofn_hdf5.lpTemplateName    = NULL ;

//	BOOL ret_val = GetSaveFileName (&ofn_hdf5);
	BOOL ret_val = GetOpenFileName (&ofn_hdf5);
	if (ret_val)
	{
		if (ofn_hdf5.lpstrFile[ofn_hdf5.nFileExtension] != 'h') return 0;
		if (ofn_hdf5.lpstrFile[ofn_hdf5.nFileExtension+1] != '5') return 0;
		return ofn_hdf5.lpstrFile;
	}
	else
		return  NULL;
}
//----------------------------------------------------------------


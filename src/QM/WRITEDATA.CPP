// WriteData.cpp : implementation of the CQmsView class
//

#include "stdafx.h"
#include "Action.h"
#include "GLView.h"
#include "QM_2DScatt.h"
/////////////////////////////////////////////////////////////////////////////
#include "hdf5.h"
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
*/
/////////////////////////////////////////////////////////////////////////////
// CQM_2DScatt
void CQM_2DScatt::WriteData()
{
struct Complexx{ double x,y;};
	string hdf5fn((const char*)m_bstrHDF5FileName);
//	string DATASETNAME("psiX");
//typedef Complexx (*MATRIX_Complex)[QMEvolve2D.NDATA];
	MATRIX_Complex Y = (MATRIX_Complex) QMEvolve2D.psi_xt;
// handles:
	hid_t	file_id=0;
	hid_t	group_id=0;
	hid_t	dataset_id=0;
	hid_t	dataspace_id=0;
	hid_t	datatype_id=0;
//	hid_t	memspace_id=0;
	hid_t	attribute_id=0;
	hid_t   attribute_dataspace_id=0;
// dataset dimensions:
//	hsize_t	dimsf[3]={QMEvolve2D.NDATA, QMEvolve2D.NDATA, 2};
	hsize_t	dimsf[2]={QMEvolve2D.NDATA, QMEvolve2D.NDATA};
	herr_t	status;                             

	SetCursor(LoadCursor(NULL, IDC_WAIT));
/*
	IWebBrowser2* pIWebBrowser2=0;
	IServiceProvider* pISP=0;

	HRESULT hr = m_spClientSite->QueryInterface(IID_IServiceProvider, (void **)&pISP);
	if (SUCCEEDED(hr))
	{
		pISP->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void **)&pIWebBrowser2);
		pISP->Release();
	}
*/
try {
/*
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
*/
	file_id = H5Fcreate(hdf5fn.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (file_id<0)
	{
		_variant_t v = (long) (file_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Fcreate error: ");
		b+=err;
		MessageBox(0,b,"",MB_OK);
		return;
	}

	group_id = H5Gcreate(file_id, m_groupTag.c_str(), 0);
	if (group_id<0)
	{
		_variant_t v = (long) (group_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Gcreate(file_id, '/Frames', 0) error: ");
		b+=err;
		throw CMyException(b);
	}

	attribute_dataspace_id = H5Screate(H5S_SCALAR);
	if (attribute_dataspace_id<0)
	{
		_variant_t v = (long) (attribute_dataspace_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Screate(H5S_SCALAR) error: ");
		b+=err;
		throw CMyException(b);
	}
	attribute_id = H5Acreate (group_id, m_attributeTag.c_str(), H5T_NATIVE_INT, attribute_dataspace_id, H5P_DEFAULT);
	if (attribute_id<0)
	{
		_variant_t v = (long) (attribute_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Acreate error:\n");
		b+=err;
		throw CMyException(b);
	}
	status = H5Awrite(attribute_id, H5T_NATIVE_INT, &m_nNumFrames);
	if (status<0)
	{
		_variant_t v = (long) (status);
		_bstr_t err=v;
		_bstr_t b(L"H5Awrite error: ");
		b+=err;
		throw CMyException(b);
	}
//	status = H5Sclose(attribute_dataspace_id); 
//	status = H5Aclose(attribute_id);
//	status = H5Gclose(group_id);

    /*
     * Describe the size of the array and create the data space for fixed
     * size dataset_id. 
     */
//	dimsf[0] = QMEvolve2D.NDATA;
//	dimsf[1] = QMEvolve2D.NDATA;
	dataspace_id = H5Screate_simple(sizeof(dimsf)/sizeof(dimsf[0]), dimsf, NULL);
	if (dataspace_id<0)
	{
		_variant_t v = (long) (dataspace_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Screate_simple error: ");
		b+=err;
		throw CMyException(b);
	}

//Create the memory data type.
//compound datatypes are created from scratch. 
//First, one creates an empty compound datatype and specifies it's total size. 
//Then members are added to the compound datatype in any order. 
	datatype_id = H5Tcreate (H5T_COMPOUND, sizeof(Complexx));
	if (datatype_id<0)
	{
		_variant_t v = (long) (datatype_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Tcreate error: ");
		b+=err;
		throw CMyException(b);
	}
	H5Tinsert(datatype_id, "x", HOFFSET(Complexx, x), H5T_NATIVE_DOUBLE);
	H5Tinsert(datatype_id, "y", HOFFSET(Complexx, y), H5T_NATIVE_DOUBLE);
//The macro defined in stddef.h does exactly the same thing as the HOFFSET() macro
//#define offsetof(s,m)   (size_t)&(((s *)0)->m)
//in <stddef.h>

	status = H5Tpack(datatype_id);
	size_t datatype_size = H5Tget_size(datatype_id);
	if (datatype_size != sizeof(Complexx))
	{
		_variant_t v = (long) (datatype_size);
		_bstr_t err=v;
		_bstr_t b(L"datatype_size != sizeof(Complexx):\n");
		b+=err;
		throw CMyException(b);
	}

/*
	datatype_id = H5Tcopy(H5T_NATIVE_DOUBLE);
	status = H5Tset_order(datatype_id, H5T_ORDER_LE);//not valid for type H5T_COMPOUND
*/
	QMEvolve2D.DefinePotential();
	QMEvolve2D.DefinePotentialDependentParameters();
	QMEvolve2D.DefineInitialWavefunction();

	long k;
	char szNumber[16];

/////////////////////////////////////////////////////////////////////////////
	for (k=0; k<m_nNumFrames; k++ )
	{
		wsprintf(szNumber,"%04u",k) ;

		string dsName = "/Frames/";
		dsName += szNumber;
	//	dataset_id = H5Dcreate(file_id, dsName.c_str(), datatype_id, dataspace_id, H5P_DEFAULT);
		dataset_id = H5Dcreate(group_id, szNumber, datatype_id, dataspace_id, H5P_DEFAULT);
	if (dataset_id<0)
	{
		_variant_t vk = (long) sizeof(k);
		_bstr_t bk=vk;
		_variant_t v = (long) sizeof(dataset_id);
		_bstr_t err=v;
		_bstr_t b(L"H5Dcreate error: ");
		b+=bk;
		b+=err;
		throw CMyException(b);
	}
//	for (long j=0;j<QMEvolve2D.NDATA;++j)
//		for (long k=0;k<QMEvolve2D.NDATA;++k)
//			m_pExternal[j][k].x = Y[j][k].x, m_pExternal[j][k].y = Y[j][k].y;

//		status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, (const void*)Y);
		status = H5Dwrite(dataset_id, datatype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, (const void*)Y);
	if (status<0)
	{
		_variant_t v = (long) (status);
		_bstr_t err=v;
		_bstr_t b(L"H5Dwrite error: ");
		b+=err;
		throw CMyException(b);
	}
//QMEvolve2D.NDATA * QMEvolve2D.NDATA * sizeof(Complexx)
// 64 * 64 * 16 = 65536 bytes per slice
		H5Dclose(dataset_id);
		dataset_id = 0;
//	MessageBox(dsName.c_str());

///////////////////////////////////////////////////////////////////////////////
		switch (QMEvolve2D.m_EvolutionMethod)
		{
		case QMEvolve2D.Nash: QMEvolve2D.EVOLVE_NASH(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.Cayley: QMEvolve2D.EVOLVE_CAYLEY(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.PeriodicCayley: QMEvolve2D.EVOLVE_CAYLEY(m_nIterationsPerFrame);
			break;
		}

/*
		TCHAR buf[256];
		_stprintf(buf, "Frame[%d]", k);
		_bstr_t bstr(buf);
		if (pIWebBrowser2) pIWebBrowser2->put_StatusText(bstr);
*/
	}
//////////////////////////////////////////////////////////////////////////////
	SetCursor(LoadCursor(NULL, IDC_ARROW));
//	if (pIWebBrowser2) pIWebBrowser2->Release();

    /*
     * Close/release resources.
     */
	if (attribute_dataspace_id>0) H5Aclose(attribute_dataspace_id);
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
//	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
//////////////////////////////////////////////////////////////////////////////

}
//catch(CMyException* e){e->Delete();}
catch( CMyException E )
{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
//		if (pIWebBrowser2) pIWebBrowser2->Release();
	if (attribute_dataspace_id>0) H5Aclose(attribute_dataspace_id);
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
//	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
	MessageBox(0,E.ShowReason(),"",MB_OK);
	return;
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
//		if (pIWebBrowser2) pIWebBrowser2->Release();
	if (attribute_dataspace_id>0) H5Aclose(attribute_dataspace_id);
	if (attribute_id>0) H5Aclose(attribute_id);
	if (datatype_id>0)	H5Tclose(datatype_id);
	if (dataspace_id>0)	H5Sclose(dataspace_id);
//	if (memspace_id>0)	H5Sclose(memspace_id);
	if (dataset_id>0)	H5Dclose(dataset_id);
	if (group_id>0)		H5Gclose(group_id);
	if (file_id>0)		H5Fclose(file_id);
	return;
}

}


// QMExamples.cpp : Implementation of CIO
#include "stdafx.h"
#include "Action.h"

#include "ActionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ScriptColors.h"

///////////////////////////////////////////////////////////////////////////////

/*
		RECT rc;
		GetWindowRect(&rc);
CComPtr<IAxWinHostWindow> spAxWindow;
hr = QueryHost(&spAxWindow);
wnd.Create(hWnd, rc, _T("Act.CGraph2D.1"), WS_CHILD|WS_VISIBLE, 0, WM_USER+3);  

		CComQIPtr <IPersistStreamInit> spPerStm(theSystem->m_pIQM_2DScatt);
		spPerStm->InitNew();
		CComPtr<IUnknown> spUnkContainer;
		HWND hWnd = 0;
		hr = theSystem->m_pIUI_ClassicalPhysics->get_Window((long*)&hWnd);//m_pIView
		AtlAxAttachControl( theSystem->m_pIQM_2DScatt, hWnd, &spUnkContainer );
	IUnknown *punk=0;
//	AtlAxCreateControl( (const unsigned short *) &CLSID_Graph2D, hWnd, 0, &punk );
	AtlAxCreateControl( T2COLE(_T("Act.CGraph2D.1")), hWnd, 0, &spUnk );
*/
///////////////////////////////////////////////////////////////////////////////
void CActionView::Scattering2Dim()
{
//	NewWebBrowser(7);
	//Fire_ClearUserDefs();
	WRITE_LINE("sys.stop")WRITE_LINE("initialize")
	WRITE_LINE("system.title = \"2-Dim Scattering\"")
	WRITE_LINE("system.boundingBox = [ -1.25, 1.25, -1.25, 1.25, 2.5, 13 ] ")
	WRITE_LINE("system.twoDimensionalWorld.qm = true")
//	WRITE_LINE("sys.translate.z =  -5.4")
	WRITE_LINE("sys.translate.z =  -4.1")
	WRITE_LINE("sys.rotate =  [-37.2, -3.2, 0] ")

#if 0
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINES("func V($x, $y, $z) {")
	WRITE_LINES("	return ( (1 - $x/4)*(1 + $x/4)*(1 - $x/4)*(1 + $x/4) + $y*$y*(0.25+6.25*exp(-$x*$x)))")
	WRITE_LINE ("}")
///////////////////////////////////////////////////////////////////////////////
	if (theSystem->m_pIQM_2DScatt)
	{
		theSystem->m_pIQM_2DScatt.Release();
		theSystem->m_pIQM_2DScatt = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_QM_2DScatt,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_IQM_2DScatt,
									(void**)&theSystem->m_pIQM_2DScatt);
	if (SUCCEEDED(hr))
	{
		theSystem->m_pIQM_2DScatt->SetViewPointer((long) this);
		theSystem->m_pIQM_2DScatt->AdviseSource();
		ATLASSERT(3==LONG(_QM_2DScatt));
		theSystem->m_currentLayout = _QM_2DScatt;
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_QM_2DScatt));
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrQM_2DScatt2);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrQM_2DScatt);
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif
	
}
///////////////////////////////////////////////////////////////////////////////
void CActionView::Scattering1Dim()
{
	//Fire_ClearUserDefs();
	WRITE_LINE("sys.stop")WRITE_LINE("initialize")
	WRITE_LINE("system.title = \"1-Dim Scattering\"")
	WRITE_LINE("system.boundingBox = [ -1, 1, -1, 1, 5, 10 ] ")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINES("func V($x) {")
	WRITE_LINES("q=6.0*($x)")
	WRITE_LINES("Y=0.1")
	WRITE_LINES("	return ( (1 - q/4)*(1 + q/4)*(1 - q/4)*(1 + q/4) + Y*Y*(0.25+6.25*exp(-q*q)))")
	WRITE_LINE ("}")
	WRITE_LINE("system.potential = V 1, false")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.oneDimensionalWorld.qm = true")
#if 0
	if (theSystem->m_pIQM_1DScatt)
	{
		theSystem->m_pIQM_1DScatt.Release();
		theSystem->m_pIQM_1DScatt = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_QM_1DScatt,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_IQM_1DScatt,
									(void**)&theSystem->m_pIQM_1DScatt);


	if (SUCCEEDED(hr))
	{
		theSystem->m_pIQM_1DScatt->SetViewPointer((long) this);
		theSystem->m_pIQM_1DScatt->AdviseSource();
		ATLASSERT(2==LONG(_QM_1DScatt));
		theSystem->m_currentLayout = _QM_1DScatt;
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_QM_1DScatt));
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrQM_1DScatt2);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrQM_1DScatt);
		ofstream ofs("duh.htm");
		ofs<<(const char*)bstrQM_1DScatt2;
		ofs.close();
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif
	
}


void CActionView::Superposition()
{
	//Fire_ClearUserDefs();
	WRITE_LINE("sys.stop")WRITE_LINE("initialize")
	WRITE_LINE("system.title = \"Superposition\"")
	WRITE_LINE("system.boundingBox = [ -4, 4, -1.5, 2.5, 4, 12 ] ")
	WRITE_LINE("sys.translate.z =  -6")
	WRITE_LINE("sys.rotate.x =  30")
	WRITE_LINE("sys.rotate.y =  -4")
	WRITE_LINE("system.oneDimensionalWorld.superposition = true")
#if 0
	if (theSystem->m_pISuperposition)
	{
		theSystem->m_pISuperposition.Release();
		theSystem->m_pISuperposition = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_Superposition,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_ISuperposition,
									(void**)&theSystem->m_pISuperposition);


	if (SUCCEEDED(hr))
	{
		theSystem->m_currentLayout = _Superposition;
		theSystem->m_pISuperposition->SetViewPointer((long) this);
		theSystem->m_pISuperposition->AdviseSource();
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_Superposition));
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Hide(LONG(_Superposition));//hides all of classical physics ui
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrSuperposition2);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrSuperposition);
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif

	
}
///////////////////////////////////////////////////////////////////////////////
void CActionView::OnElectroStaticField()
{
	//Fire_ClearUserDefs();
	WRITE_LINE("sys.stop")WRITE_LINE("initialize")
	WRITE_LINE("system.title = \"ElectroStatic Field\"")
	WRITE_LINE("system.twoDimensionalWorld = true")
	WRITE_LINE("system.twoDW.elec = true")
	WRITE_LINE("system.rec.potential = true")
#if 0
	if (theSystem->m_pIElectroStatics)
	{
		theSystem->m_pIElectroStatics.Release();
		theSystem->m_pIElectroStatics = 0;
	}
	HRESULT hr = CoCreateInstance(	CLSID_ElectroStatics,
									0,
									CLSCTX_ALL,//,//CLSCTX_LOCAL_SERVER
									IID_IElectroStatics,
									(void**)&theSystem->m_pIElectroStatics);
	if (SUCCEEDED(hr))
	{
//		theSystem->m_pIElectroStatics->SetViewPointer((long) this);
		theSystem->m_pIElectroStatics->AdviseSource();
		theSystem->m_currentLayout = _ClassicalPhysics;
		theSystem->m_pIUI_ClassicalPhysics->WrapFire_Show(LONG(_ClassicalPhysics));
		if (m_MathEquationOutput==__MathML)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrElectroStatics);
		else if (m_MathEquationOutput==__EzMath_Plugin)
			theSystem->m_pIUI_ClassicalPhysics->NewWebBrowser(bstrElectroStatics);
	}
	else
	{
		ATLASSERT(hr==REGDB_E_CLASSNOTREG);
		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
#endif
	
}

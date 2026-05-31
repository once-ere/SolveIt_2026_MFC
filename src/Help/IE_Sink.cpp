// IE_Sink.cpp : Implementation of CIE_Sink
#include "stdafx.h"
#include "Act.h"
#include "System.h"
#include "IE_Sink.h"
#include "UI_ClassicalPhysics.h"

HRESULT LoadWebOCFromStream(IWebBrowser2 *pWebBrowser, IStream *pStream);

_ATL_FUNC_INFO OnIE_SinkReceiveDataInfo2 = {CC_STDCALL, VT_EMPTY, 0,0};

_ATL_FUNC_INFO OnIE_SinkReceiveDataInfo = {CC_STDCALL, VT_EMPTY, 8,
	{VT_I4,VT_BYREF|VT_CLSID,VT_I4,VT_I4,VT_PTR,VT_VARIANT,VT_PTR,VT_PTR} };
/////////////////////////////////////////////////////////////////////////////
// CIE_Sink
/*
DISPID_BEFORENAVIGATE2
DISPID_COMMANDSTATECHANGE
DISPID_DOCUMENTCOMPLETE
DISPID_DOWNLOADBEGIN
DISPID_DOWNLOADCOMPLETE 
DISPID_NAVIGATECOMPLETE2
DISPID_NEWWINDOW2
DISPID_ONQUIT
DISPID_ONVISIBLE
DISPID_ONTOOLBAR
DISPID_ONMENUBAR
DISPID_ONSTATUSBAR
DISPID_ONFULLSCREEN
DISPID_ONTHEATERMODE
DISPID_PROGRESSCHANGE
DISPID_PROPERTYCHANGE
DISPID_STATUSTEXTCHANGE
DISPID_TITLECHANGE
*/
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */ 

HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL)
    {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.
    *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
                  cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}
///////////////////////////////////////////////////////////////////////////////
void __stdcall CIE_Sink::OnIE_SinkReceiveData2()
{
//	USES_CONVERSION;
	HRESULT			hr				= 0;
	IStreamPtr		pStream			= 0;
	IHTMLDocument2*	pHTMLDocument2	= NULL;
	LPDISPATCH		pHtmlDoc		= NULL;
	SAFEARRAY*		sfArray			= NULL;
	HGLOBAL			hHTMLText		= 0;;
//	_bstr_t&		bstr			= m_pTheControl->m_bstrContent;OLESTR()
	BSTR			bstr			= SysAllocString(m_pTheControl->m_bstrContent);
	VARIANT*		param			= NULL;
//	hHTMLText = GlobalAlloc(GPTR, bstr.length()+1);
//	lstrcpy((CHAR *)hHTMLText, (const char *)bstr);
//	HRESULT hr = CreateStreamOnHGlobal(hHTMLText, TRUE, &pStream);
//	if (SUCCEEDED(hr))
//		LoadWebOCFromStream( m_pTheControl->m_pWebBrowser, pStream);
	hr = m_pTheControl->m_pWebBrowser->get_Document(&pHtmlDoc);
	if (hr) goto Cleanup;
	hr = pHtmlDoc->QueryInterface( IID_IHTMLDocument2, (void **)&pHTMLDocument2 );
	if (hr) goto Cleanup;
// Creates a new one-dimensional array
	sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	if (sfArray == NULL) goto Cleanup;

	hr = SafeArrayAccessData(sfArray,(LPVOID*) & param);
	param->vt = VT_BSTR;
	param->bstrVal = bstr;
	hr = SafeArrayUnaccessData(sfArray);
	hr = pHTMLDocument2->write(sfArray);
Cleanup:
	SysFreeString(bstr);
	if (sfArray != NULL) SafeArrayDestroy(sfArray);
	if (pHtmlDoc)		pHtmlDoc->Release();
	if (pHTMLDocument2)	pHTMLDocument2->Release();
}
///////////////////////////////////////////////////////////////////////////////
#if 0
STDMETHODIMP CIE_Sink::Invoke(	DISPID dispidMember,
								REFIID riid,
								LCID lcid,
								WORD wFlags,
								DISPPARAMS* pDispParams,
								VARIANT* pvarResult,
								EXCEPINFO*  pExcepInfo,
								UINT* puArgErr)
{
	USES_CONVERSION;
	char str[100];	// Not the best way to do this.
	ostrstream strEventInfo(str, sizeof str);
 
//	if (dispidMember == DISPID_STATUSTEXTCHANGE) return 0;
//	if (dispidMember == DISPID_COMMANDSTATECHANGE) return 0;
	if (!pDispParams) return E_INVALIDARG;

   // Get the current URL
   //  

	int j;
	switch (dispidMember)
	{
		// The parameters for this DISPID:
		// [0]: URL navigated to - VT_BYREF|VT_VARIANT
		// [1]: An object that evaluates to the top-level or frame
		//		  WebBrowser object corresponding to the event.
		//
		case DISPID_DOCUMENTCOMPLETE:
		{
		//::MessageBox(NULL, "DISPID_DOCUMENTCOMPLETE", "Invoke", MB_OK);
			IStreamPtr pStream = 0;
			HGLOBAL hHTMLText;
			_bstr_t& bstr = m_pTheControl->m_bstrContent;
			hHTMLText = GlobalAlloc(GPTR, bstr.length()+1);
			lstrcpy((CHAR *)hHTMLText, (const char *)bstr);
			HRESULT hr = CreateStreamOnHGlobal(hHTMLText, TRUE, &pStream);
			if (SUCCEEDED(hr))
				LoadWebOCFromStream( m_pTheControl->m_pWebBrowser, pStream);
		}
			 break;	 
		case DISPID_ONQUIT:
		//::MessageBox(NULL, "DISPID_ONQUIT", "Invoke", MB_OK);
			 break;	 
		case DISPID_ONSTATUSBAR:
		//::MessageBox(NULL, "DISPID_ONSTATUSBAR", "Invoke", MB_OK);
			 break;	 
		case DISPID_BEFORENAVIGATE2:
		//::MessageBox(NULL, "DISPID_BEFORENAVIGATE2", "Invoke", MB_OK);
			 break;

		case DISPID_COMMANDSTATECHANGE:
		//::MessageBox(NULL, "DISPID_COMMANDSTATECHANGE", "Invoke", MB_OK);
			 break;
		case DISPID_DOWNLOADBEGIN:
		//::MessageBox(NULL, "DISPID_DOWNLOADBEGIN", "Invoke", MB_OK);
			 break;
		case DISPID_DOWNLOADCOMPLETE:
		//::MessageBox(NULL, "DISPID_DOWNLOADCOMPLETE", "Invoke", MB_OK);
			 break;
		case DISPID_NEWWINDOW2:
		//::MessageBox(NULL, "DISPID_NEWWINDOW2", "Invoke", MB_OK);
			 break;
		case DISPID_ONVISIBLE:
		//::MessageBox(NULL, "DISPID_ONVISIBLE", "Invoke", MB_OK);
			 break;
		case DISPID_ONTOOLBAR:
		//::MessageBox(NULL, "DISPID_ONTOOLBAR", "Invoke", MB_OK);
			 break;
		case DISPID_ONMENUBAR:
			//::MessageBox(NULL, "DISPID_ONMENUBAR", "Invoke", MB_OK);
			 break;
		case DISPID_ONFULLSCREEN:
			//::MessageBox(NULL, "DISPID_ONFULLSCREEN", "Invoke", MB_OK);
			 break;
		case DISPID_ONTHEATERMODE:
			//::MessageBox(NULL, "DISPID_ONTHEATERMODE", "Invoke", MB_OK);
			 break;
		case DISPID_PROGRESSCHANGE://this fires on loading of page
/*
			{
				LPOLESTR lpURL = NULL;
				m_spWebBrowser2->get_LocationURL(&lpURL);
				TCHAR* sz;
				UnicodeToAnsi(lpURL, &sz);
				::MessageBox(NULL, sz, "Invoke", MB_OK);
				CoTaskMemFree(sz);
			}
*/
			 //::MessageBox(NULL, "DISPID_PROGRESSCHANGE", "Invoke", MB_OK);
		//	 theSystem->Clear();
		//	 theSystem->m_pIUI_ClassicalPhysics->PopulateTree();
		//	if (!theSystem->m_bConnectedToView)
		//	{
		//		theSystem->ConnectToView();
		//	}
			 break;
		case DISPID_PROPERTYCHANGE:
			 //::MessageBox(NULL, "DISPID_PROPERTYCHANGE", "Invoke", MB_OK);
			 break;
		case DISPID_STATUSTEXTCHANGE:
			 //::MessageBox(NULL, "DISPID_STATUSTEXTCHANGE", "Invoke", MB_OK);

	//		LPOLESTR lpStatusText;
	//		m_spWebBrowser2->get_StatusText(&lpStatusText);
			 break;
		case DISPID_TITLECHANGE:
			 //::MessageBox(NULL, "DISPID_TITLECHANGE", "Invoke", MB_OK);
			 break;	 
		case DISPID_NAVIGATECOMPLETE2:
			 //::MessageBox(NULL, "DISPID_NAVIGATECOMPLETE2", "Invoke", MB_OK);
			j=dispidMember;
// Check the argument's type
#if 0
			 if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
			 {
				 CComVariant varURL(*pDispParams->rgvarg[0].pvarVal);
				 varURL.ChangeType(VT_BSTR);
		 
 
				 strEventInfo << "NavigateComplete2: "
							<< OLE2T(varURL.bstrVal)
							<< ends;
 
		//		 ////::MessageBox(NULL, strEventInfo.str(), "Invoke", MB_OK);
		//		IStreamPtr pStream	= 0;
		//		HGLOBAL hGlobal		= GlobalAlloc(GPTR, 0);
		//		CreateStreamOnHGlobal( hGlobal,TRUE, &pStream );
/*				
IStreamPtr pStream = 0;
HGLOBAL hHTMLText;
TCHAR szHTMLText[] = _T("<html><HEAD></HEAD><BODY><h1>Stream Test</h1><p>This HTML content is being loaded from a stream.</BODY></html>");
hHTMLText = GlobalAlloc(GPTR, lstrlen(szHTMLText)+1);
lstrcpy((CHAR *)hHTMLText, szHTMLText);
HRESULT hr = CreateStreamOnHGlobal(hHTMLText, TRUE, &pStream);
if (SUCCEEDED(hr)) LoadWebOCFromStream( m_pTheControl->m_pWebBrowser, pStream);
*/
			 }
#endif
			 break;	 
	  
		default:
			 break;
		  }
	return 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
/*
void __stdcall CIE_Sink::OnIE_SinkReceiveData2()
{
	////::MessageBox(NULL, "OnIE_SinkReceiveData2", "OnIE_SinkReceiveData2", MB_OK);
}

void __stdcall CIE_Sink::OnIE_SinkReceiveData(
									DISPID dispidMember,
							REFIID riid,
							LCID lcid,
									WORD wFlags,
							DISPPARAMS* pDispParams,
							VARIANT* pvarResult,
							EXCEPINFO*  pExcepInfo,
							UINT* puArgErr)
{
	USES_CONVERSION;
	char str[100];	// Not the best way to do this.
	ostrstream strEventInfo(str, sizeof str);
 
		  if (!pDispParams) return;// E_INVALIDARG;
 
		  switch (dispidMember)
		  {
		// The parameters for this DISPID:
		// [0]: URL navigated to - VT_BYREF|VT_VARIANT
		// [1]: An object that evaluates to the top-level or frame
		//		  WebBrowser object corresponding to the event.
		//
		case DISPID_NAVIGATECOMPLETE2:
			 // Check the argument's type
			 if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
			 {
				 CComVariant varURL(*pDispParams->rgvarg[0].pvarVal);
				 varURL.ChangeType(VT_BSTR);
		 
 
				 strEventInfo << "NavigateComplete2: "
							<< OLE2T(varURL.bstrVal)
							<< ends;
 
				 ////::MessageBox(NULL, strEventInfo.str(), "Invoke", MB_OK);
			 }
			 break;	 
	  
		default:
			 break;
		  }
 
}
*/

/*
void SomeFunction([in] long Number, [in] BSTR String);

You would define an _ATL_FUNC_INFO structure like this:

_ATL_FUNC_INFO info = {CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR} };


<SCRIPT LANGUAGE="JScript">
function OnChangeHandler() {
//	 document.write("OnChangeHandler");
var charCode = String.fromCharCode(window.event.keyCode);  
if (charCode == "\n") window.alert(charCode);
if (window.event.keyCode == 13) window.alert("keyCode == 13");
if (window.event.keyCode == 10) window.alert("keyCode == 10");
}
</SCRIPT>

<TEXTAREA id=TEXTAREA1 name=TEXTAREA1 onkeypress=OnChangeHandler() style="HEIGHT: 76px; WIDTH: 183px">
</TEXTAREA> 

<BUTTON ID="btnSave" LANGUAGE="JavaScript" onclick="btnSave_onclick()">

<SCRIPT LANGUAGE="VBScript">
Function window_onload()
   objAdviseTextbox1 = Textbox1.advise("onkeypress", "checkkeys()")
End function
</SCRIPT>

The function you specify in the advise method works like any event handler. 
If the event passes parameters, you can get those using the DHTML window 
object's event method. The following shows the handler for the onkeypress 
event in the previous example. It examines each keystroke that occurs in 
the Textbox1 object and copies only the numbers to the object Textbox2.

Function checkkeys()
   character = Chr(window.event.keycode)
   If character >= "0" and character <= "9" then
      Textbox2.value = Textbox2.value & character
   End if
End function

When you no longer need notification of the event, cancel it by calling the 
object's unadvise method. The unadvise method requires the advise object 
returned by the advise method as well as the name of the event. The following 
shows an example of calling unadvise:

Textbox1.unadvise("onkeypress", objAdviseTextbox1)



DISPID_BEFORENAVIGATE2
DISPID_COMMANDSTATECHANGE
DISPID_DOCUMENTCOMPLETE
DISPID_DOWNLOADBEGIN
DISPID_DOWNLOADCOMPLETE 
DISPID_NAVIGATECOMPLETE2
DISPID_NEWWINDOW2
DISPID_ONQUIT
DISPID_ONVISIBLE
DISPID_ONTOOLBAR
DISPID_ONMENUBAR
DISPID_ONSTATUSBAR
DISPID_ONFULLSCREEN
DISPID_ONTHEATERMODE
DISPID_PROGRESSCHANGE
DISPID_PROPERTYCHANGE
DISPID_STATUSTEXTCHANGE
DISPID_TITLECHANGE


HRESULT CViewSource::Invoke(DISPID dispidMember, REFIID riid, 
	LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, 
	VARIANT* pvarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
  if (dispidMember == DISPID_DOCUMENTCOMPLETE) {
		OnDocumentComplete();
		m_bDocumentCompleted = true;
  }
  :
}


	  STDMETHODIMP CSomeClass::Invoke(DISPID dispidMember,
										REFIID riid,
										LCID lcid,
									 WORD wFlags,
										DISPPARAMS* pDispParams,
										VARIANT* pvarResult,
										EXCEPINFO*  pExcepInfo,
										UINT* puArgErr)
	  {
		  USES_CONVERSION;
		  strstream strEventInfo;
 
		  if (!pDispParams)
		return E_INVALIDARG;
 
		  switch (dispidMember)
		  {
		// The parameters for this DISPID:
		// [0]: URL navigated to - VT_BYREF|VT_VARIANT
		// [1]: An object that evaluates to the top-level or frame
		//		  WebBrowser object corresponding to the event.
		//
		case DISPID_NAVIGATECOMPLETE2:
			 // Check the argument's type
			 if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
			 {
				 CComVariant varURL(*pDispParams->rgvarg[0].pvarVal);
				 varURL.ChangeType(VT_BSTR);
		 
				 char str[100];	// Not the best way to do this.
 
				 strEventInfo << "NavigateComplete2: "
							<< OLE2T(vtURL.bstrVal)
							<< ends;
 
				 ////::MessageBox(NULL, strEventInfo.str(),
							"Invoke", MB_OK);
			 }
			 break;	 
	  
		default:
			 break;
		  }
 
		  return S_OK;
	  }

*/

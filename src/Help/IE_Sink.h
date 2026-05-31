// IE_Sink.h : Declaration of the CIE_Sink

#ifndef __IE_SINK_H_
#define __IE_SINK_H_

#include "resource.h"       // main symbols
#include "ExDispid.h"

extern _ATL_FUNC_INFO OnIE_SinkReceiveDataInfo2;
extern _ATL_FUNC_INFO OnIE_SinkReceiveDataInfo;

class CView;
class CUI_ClassicalPhysics;
/////////////////////////////////////////////////////////////////////////////
// CIE_Sink
class ATL_NO_VTABLE CIE_Sink : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIE_Sink, &CLSID_IE_Sink>,
	public IDispatchImpl<IIE_Sink, &IID_IIE_Sink, &LIBID_ACTLib>,
	public IDispEventSimpleImpl<1, CIE_Sink, &DIID_DWebBrowserEvents2>
{
public:
	CIE_Sink() :
		m_spWebBrowser2(0),
		m_dwAdvise(0)
	{
	}

	virtual ~CIE_Sink() {
		HRESULT hRes = DispEventUnadvise(m_spWebBrowser2);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IE_SINK)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIE_Sink)
	COM_INTERFACE_ENTRY(IIE_Sink)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

  // Event handlers. Make sure the Event Handlers have __stdcall calling convention
   void __stdcall OnIE_SinkReceiveData2();
   void __stdcall OnIE_SinkReceiveData(	DISPID dispidMember,
										REFIID riid,
										LCID lcid,
										WORD wFlags,
										DISPPARAMS* pDispParams,
										VARIANT* pvarResult,
										EXCEPINFO*  pExcepInfo,
										UINT* puArgErr);

//	SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnIE_SinkReceiveData, &OnIE_SinkReceiveDataInfo)

BEGIN_SINK_MAP(CIE_Sink)
	SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnIE_SinkReceiveData2, &OnIE_SinkReceiveDataInfo2)
END_SINK_MAP()
/*
DISPID_NAVIGATECOMPLETE2
	STDMETHOD(Invoke)(DISPID dispidMember,
						REFIID riid,
						LCID lcid,
						WORD wFlags,
						DISPPARAMS* pDispParams,
						VARIANT* pvarResult,
						EXCEPINFO*  pExcepInfo,
						UINT* puArgErr);

	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
*/

// IIE_Sink
public:
	HRESULT SetOwner(CUI_ClassicalPhysics* pTheControl, CComPtr<IWebBrowser2> spWebBrowser2) {
		m_pTheControl = pTheControl;
		m_spWebBrowser2 = spWebBrowser2;
		HRESULT hRes = DispEventAdvise(m_spWebBrowser2);
		return hRes;
	}

	HRESULT SetBrowser(CComPtr<IWebBrowser2> spWebBrowser2) {
		m_pTheControl = 0;
		m_spWebBrowser2 = spWebBrowser2;
		HRESULT hRes = DispEventAdvise(m_spWebBrowser2);
		return hRes;
	}

private:
//advise holder
	DWORD m_dwAdvise;
	//interface pointer to the server
	CComPtr<IWebBrowser2> m_spWebBrowser2;
//	CComQIPtr<IWebBrowser2, &IID_IWebBrowser2> m_spWebBrowser2;


//	CView* m_pTheControl;
	CUI_ClassicalPhysics* m_pTheControl;
};

#endif //__IE_SINK_H_

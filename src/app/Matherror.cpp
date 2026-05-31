// Matherror.cpp: implementation of the CMatherr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matherror.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

emap emap::_emap;

///////////////////////////////////////////////////////////////////////////////
std::wstring  DupString(const wstring& a) //Action::
{ 
	return a;
} 
///////////////////////////////////////////////////////////////////////////////

int _matherr( struct _exception *except )
{
//	static emap _emap;

	_clearfp();
	/*wchar_t*/char buf[512];
		size_t count = sizeof(buf)/sizeof(buf[0]) - 1;
	if( except->type == _DOMAIN )
	{
		if( strcmp( except->name, "log" ) == 0 )
		{
			except->retval = log( -(except->arg1) );
			snprintf(buf, count,  "Special: using absolute value: %s: _DOMAIN error\n",  except->name );
		}
		else if( strcmp( except->name, "log10" ) == 0 )
		{
			except->retval = log10( -(except->arg1) );
			snprintf(buf, count,  "Special: using absolute value: %s: _DOMAIN error\n",  except->name );
		}
		else if( strcmp( except->name, "sqrt" ) == 0 )
		{
			except->retval = sqrt( -(except->arg1) );
			snprintf(buf, count,  "Invalid arg = %lg; Special: using absolute value: %s: _DOMAIN error\n", except->arg1, except->name );
		}
	//	throw except;
	//	theApp.theSystem.Stop();
	//	theApp.theSystem.m_rigidBodyManager.SetToPreviousState();
	//	Fire_ViewChange();
	//	::TimedMessageBox(NULL,buf,"_matherr",MB_ICONWARNING);
		return 1;
	}
	else
	{
		const wchar_t* msg = 0;
		if ((msg = emap::_emap(except->type).c_str()) && *msg)
		{
			// Convert wide string to narrow string properly
			std::wstring wmsg_wide(msg);
			std::string wmsg(wmsg_wide.begin(), wmsg_wide.end());
			snprintf(buf, count,  "FP Exception: %s:  %s: ",  except->name, wmsg.c_str());
		}
		else
			snprintf(buf, count,  "FP Exception: %s:  ?: ",  except->name);
		::MessageBoxA(NULL, buf, "_matherr", MB_ICONWARNING);//Timed
	//	throw except;
		ExitProcess(except->type);
		return 0;	/* Else use the default actions */
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CMatherror::CMatherror()
{

}

CMatherror::~CMatherror()
{

}
///////////////////////////////////////////////////////////////////////////////
std::wstring emap::OnError(struct _exception *except)
{
//	CFortranWrapper::bStopIterations = true;
	_clearfp();
	char buf[512];
	size_t count = sizeof(buf)/sizeof(buf[0]) - 1;
	const wchar_t* msg = 0;
	if ((msg = emap::_emap(except->type).c_str()) && *msg)
	{
		// Convert wide string to narrow string properly
		std::wstring wmsg_wide(msg);
		std::string wmsg(wmsg_wide.begin(), wmsg_wide.end());
		snprintf(buf, count, "FP Exception: %s:  %s: ",  except->name, wmsg.c_str());
	}
	else
		snprintf(buf, count, "FP Exception: %s:  ?: ",  except->name);

	std::wstring bstr(buf, buf + strlen(buf));
//		SetWindowText(hWndMain, bstr);
	return bstr;	/* Else use the default actions */
}

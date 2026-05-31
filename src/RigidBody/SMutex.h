// SMutex.h: interface for the SMutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMUTEX_H__7D19EF79_400B_44AD_A448_3DAEFC5BB43B__INCLUDED_)
#define AFX_SMUTEX_H__7D19EF79_400B_44AD_A448_3DAEFC5BB43B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace SolveIt {
///////////////////////////////////////////////////////////////////////////////
typedef void	(*VoidFn_VoidPtr)		(void); 

class SMutex  
{
public:
	SMutex(const TCHAR* suffix) :
		m_hmutex(0)
	{
		TCHAR clsid[] = _T("{9aa18c98-597e-11d3-bef9-00c04f79dba3}");
		TCHAR mutexObjectName[MAX_PATH];
		_tcscpy_s(mutexObjectName, _countof(mutexObjectName), clsid);
		_tcscat_s(mutexObjectName, _countof(mutexObjectName), _T("_mtx_"));
		_tcscat_s(mutexObjectName, _countof(mutexObjectName), suffix);
		m_hmutex = CreateMutex(0, FALSE, mutexObjectName);//no security attributes; initially not owned
	}
	virtual ~SMutex()
	{
		if (m_hmutex) CloseHandle(m_hmutex);
	}

	void Release() {
		if (! ReleaseMutex(m_hmutex)) { // Deal with error.
			throw SolveIt_ERROR(L"ReleaseMutex");
		} 
	}
	BOOL wait(long time_out_interval) 
	{
		try {
			DWORD dwWaitResult = WaitForSingleObject(m_hmutex, time_out_interval);// Request ownership of mutex.
			switch (dwWaitResult) {
// The thread got mutex ownership.
				case WAIT_OBJECT_0: return TRUE; 
// Cannot get mutex ownership due to time-out.
				case WAIT_TIMEOUT: return FALSE; 
// Got ownership of the abandoned mutex object.
				case WAIT_ABANDONED:  return FALSE; 
			}
		}
		catch( SolveIt_Error& e){e.why();}catch(...) {
			ReleaseMutex(m_hmutex);
			throw;
		}
		return FALSE; 
	}
private:
	HANDLE m_hmutex;
};

} // namespace SolveIt

#endif // !defined(AFX_SMUTEX_H__7D19EF79_400B_44AD_A448_3DAEFC5BB43B__INCLUDED_)

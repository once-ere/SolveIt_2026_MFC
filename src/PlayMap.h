#if !defined(AFX_PLAYMAP_H__F8F73F82_6925_4FD6_BD06_AE53685CD910__INCLUDED_)
#define AFX_PLAYMAP_H__F8F73F82_6925_4FD6_BD06_AE53685CD910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayMap.h : header file
//

#include "vfw.h"


/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
const long lSize = 512;
#define M_ON  (0x7F0090 | (( (wchar_t (0x5F*s_y[j]))& 0x7F) << 8) )
#define M_OFF (0x90     | (( (wchar_t (0x5F*s_y[j]))& 0x7F) << 8) )
const double max_Rate = 3.9;
const double min_Rate = 1;//.0000001;

/////////////////////////////////////////////////////////////////////////////
// CPlayMap thread

class CPlayMap : public CWinThread
{
	DECLARE_DYNCREATE(CPlayMap)
protected:
	CPlayMap();           // protected constructor used by dynamic creation

// Attributes
public:
	void PlayMap(int& j, int& sleep, double& delay);
	HMIDISTRM m_hStream;
	UINT m_uDeviceID;      
	double delay;
	bool bPaused;
	bool bg_RateChanged;
	int j;
	int sleep;
///////////////////////////////////////////////////////////////////////////////
	double g_Rate;
	DWORD s_x[lSize];
	double s_y[lSize/2];
	Point3D point;
	Vector3D color;
///////////////////////////////////////////////////////////////////////////////

// Operations
public:
	long MakeTimeStep();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayMap)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlayMap();

	// Generated message map functions
	//{{AFX_MSG(CPlayMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


	afx_msg void OnStep(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetParam(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_PLAYMAP_H__F8F73F82_6925_4FD6_BD06_AE53685CD910__INCLUDED_)

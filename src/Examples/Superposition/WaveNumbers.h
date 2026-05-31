#if !defined(AFX_WAVENUMBERS_H__17E27731_2D36_4BC9_BC9D_D6DB20D77182__INCLUDED_)
#define AFX_WAVENUMBERS_H__17E27731_2D36_4BC9_BC9D_D6DB20D77182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveNumbers.h : header file
//

namespace SolveIt {
class CSuperposition;

/////////////////////////////////////////////////////////////////////////////
// CWaveNumbers dialog

class CWaveNumbers : public CDialog
{
// Construction
public:
	CWaveNumbers():
		m_bModal(true),
		m_pCSuperposition(0),
		m_Wavelength(0.2), 
		m_dt(0.003125), 
		m_Ratio(1.05),
		m_NumberOfWaveNumbers(3) 
	{
	}
	CWaveNumbers(CSuperposition* pCSuperposition, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CWaveNumbers)
	enum { IDD = IDD_WAVENUMBERS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	bool m_bModal; 
	double m_Wavelength; 
	double m_dt; 
	double m_Ratio; 
	long m_NumberOfWaveNumbers; 
	CSuperposition* m_pCSuperposition;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveNumbers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaveNumbers)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickedApply();
	afx_msg void OnEnChange();
	afx_msg void OnClickedRadio1();
	afx_msg void OnClickedRadio2();
	afx_msg void OnClickedRadio3();
	afx_msg void OnClickedRadio4();
	afx_msg void OnClickedRadio5();
	//}}AFX_MSG
	afx_msg LRESULT Cancel(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace SolveIt

#endif // !defined(AFX_WAVENUMBERS_H__17E27731_2D36_4BC9_BC9D_D6DB20D77182__INCLUDED_)

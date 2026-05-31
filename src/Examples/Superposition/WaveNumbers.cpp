// WaveNumbers.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "WaveNumbers.h"
#include "Superposition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern UINT WM_CWaveNumbers_Cancel;//for the linker
static TCHAR lp_WM_CWaveNumbers_Cancel_MessageName[] = _T("{6E9F2795-3364-4811-889E-4713897567B4}");
UINT WM_CWaveNumbers_Cancel = RegisterWindowMessage(TEXT("{6E9F2795-3364-4811-889E-4713897567B4}"));
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CWaveNumbers dialog


CWaveNumbers::CWaveNumbers(CSuperposition* pCSuperposition, CWnd* pParent /*=NULL*/) :
	CDialog(CWaveNumbers::IDD, pParent),
	m_bModal(false),
	m_pCSuperposition(pCSuperposition),
	m_Wavelength(0.2), 
	m_dt(0.003125), 
	m_Ratio(1.05),
	m_NumberOfWaveNumbers(3) 
{
	//{{AFX_DATA_INIT(CWaveNumbers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CDialog::Create(IDD, pParent);
	::ShowWindow(m_hWnd,SW_SHOW);
	CenterWindow();
}


void CWaveNumbers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveNumbers)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveNumbers, CDialog)
	//{{AFX_MSG_MAP(CWaveNumbers)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_NUMBERWAVENUMBERS, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_WAVELENGTH, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_RATIO, OnEnChange)
	ON_EN_CHANGE(IDC_EDIT_DT, OnEnChange)
	ON_BN_CLICKED(IDC_APPLY, OnClickedApply)
	ON_BN_CLICKED(IDC_RADIO1, OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnClickedRadio5)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_CWaveNumbers_Cancel, Cancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveNumbers message handlers

BOOL CWaveNumbers::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bModal)
	{
		::ShowWindow(GetDlgItem(IDC_APPLY)->m_hWnd,SW_HIDE);
	}
	else
	{
		::ShowWindow(GetDlgItem(IDOK)->m_hWnd,SW_HIDE);
		::ShowWindow(GetDlgItem(IDCANCEL)->m_hWnd,SW_HIDE);
	}

	::SendMessage(GetDlgItem(IDC_RADIO3)->m_hWnd, BM_SETCHECK, BST_CHECKED, 0);

	TCHAR s[1024];
	/*ostrstream*/ std::wstringstream oss(s,1023);

//		oss << "Collision of " << ends;
//		SetWindowText(s);

	oss.seekp(0);
	oss << m_NumberOfWaveNumbers << ends;
	SetDlgItemText(IDC_EDIT_NUMBERWAVENUMBERS, s);

	oss.seekp(0);
	oss << m_Wavelength << ends;
	SetDlgItemText(IDC_EDIT_WAVELENGTH, s);

	oss.seekp(0);
	oss << m_Ratio << ends;
	SetDlgItemText(IDC_EDIT_RATIO, s);

	oss.seekp(0);
	oss << m_dt << ends;
	SetDlgItemText(IDC_EDIT_DT, s);

	if (m_pCSuperposition) m_pCSuperposition->OnUpdate(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaveNumbers::OnDestroy() 
{
	CDialog::OnDestroy();
	
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedApply()
{
	TCHAR s[1024];
	/*ostrstream*/ std::wstringstream oss(s,1023);

	oss.seekp(0);
	oss << m_NumberOfWaveNumbers << ends;
	SetDlgItemText(IDC_EDIT_NUMBERWAVENUMBERS, s);

	oss.seekp(0);
	oss << m_Wavelength << ends;
	SetDlgItemText(IDC_EDIT_WAVELENGTH, s);

	oss.seekp(0);
	oss << m_Ratio << ends;
	SetDlgItemText(IDC_EDIT_RATIO, s);

	oss.seekp(0);
	oss << m_dt << ends;
	SetDlgItemText(IDC_EDIT_DT, s);
	if (m_pCSuperposition) m_pCSuperposition->OnUpdate(this);
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnEnChange()
{
	// TODO : Add Code for control notification handler.
	const double TwoPi = 6.28318530717958647693;
	double wavelength; 
	double ratio; 
	double dt; 
	long numberOfWaveNumbers; 
	HWND hwnd = ::GetFocus();
	TCHAR s[128];
	/*istrstream*/ std::wstringstream iss(s, sizeof(s));
	::GetWindowText(hwnd, s, sizeof(s));
	HWND hwnd1 = ::GetDlgItem(m_hWnd,IDC_EDIT_NUMBERWAVENUMBERS);
	HWND hwnd2 = ::GetDlgItem(m_hWnd,IDC_EDIT_WAVELENGTH);
	HWND hwnd3 = ::GetDlgItem(m_hWnd,IDC_EDIT_RATIO);
	HWND hwnd4 = ::GetDlgItem(m_hWnd,IDC_EDIT_DT);
	if (hwnd==hwnd1)
	{
		if (!(iss>>numberOfWaveNumbers).fail()) m_NumberOfWaveNumbers = numberOfWaveNumbers;
	}
	else if (hwnd==hwnd2)
	{
		if (!(iss>>wavelength).fail()) m_Wavelength = wavelength;
	}
	else if (hwnd==hwnd3)
	{
		if (!(iss>>ratio).fail()) m_Ratio = ratio;
	}
	else if (hwnd==hwnd4)
	{
		if (!(iss>>dt).fail()) m_dt = dt;
	}
//	if (m_pCSuperposition) m_pCSuperposition->OnUpdate(this);
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnOK()
{
	if (m_bModal) EndDialog(IDD_WAVENUMBERS);
	else
		if (::IsWindow(m_hWnd)) DestroyWindow();//EndDialog(wID);
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnCancel()
{
	if (m_bModal) EndDialog(IDD_WAVENUMBERS);
	else
	if (::IsWindow(m_hWnd)) DestroyWindow();//EndDialog(wID);
	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LRESULT CWaveNumbers::Cancel(WPARAM wParam, LPARAM lParam)
{
	OnCancel();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CWaveNumbers::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedRadio1()
{
    m_Ratio = 1.05;
    m_NumberOfWaveNumbers = 1;
    m_Wavelength = 0.2;
    m_dt = 0.002;
	OnClickedApply();
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedRadio2()
{
    m_Ratio = 1.5;
    m_NumberOfWaveNumbers = 2;
    m_Wavelength = 1.0;
    m_dt = 0.01;
	OnClickedApply();
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedRadio3()
{
    m_Ratio = 1.5;
    m_NumberOfWaveNumbers = 3;
    m_Wavelength = 1.5;
    m_dt = 0.003;
	OnClickedApply();
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedRadio4()
{
    m_Ratio = 1.0025;
    m_NumberOfWaveNumbers = 48;
    m_Wavelength = 0.05;
    m_dt = 0.0005;
	OnClickedApply();
	
}
///////////////////////////////////////////////////////////////////////////////
void CWaveNumbers::OnClickedRadio5()
{
    m_Ratio = 1.025;
    m_NumberOfWaveNumbers = 32;
    m_Wavelength = 0.25;
    m_dt = 0.001;
	OnClickedApply();
	
}

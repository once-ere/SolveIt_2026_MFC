// ChangeCharge.cpp : Implementation of CChangeCharge
#include "stdafx.h"
#include "ChangeCharge.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChangeCharge::CChangeCharge(double fCharge, const wstring& s, CWnd* pParent /*=NULL*/)
	: CDialog(CChangeCharge::IDD, pParent),
	m_fCharge(fCharge),
	m_szCharge(s)
{
	//{{AFX_DATA_INIT(CChangeCharge)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	CDialog::Create(IDD, pParent);
//	ShowWindow(SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChangeCharge, CDialog)
	//{{AFX_MSG_MAP(CChangeCharge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CChangeCharge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeCharge)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
//   GetWC(  ).get() 
///////////////////////////////////////////////////////////////////////////////
// CChangeCharge
BOOL CChangeCharge::OnInitDialog()
{
	CDialog::OnInitDialog();
	::SetWindowTextW( m_hWnd, m_szCharge.c_str() );
	wchar_t sz[64];
	swprintf(sz, 64, L"%lg", m_fCharge);
	::SetDlgItemTextW(m_hWnd, IDC_EDIT_CHARGE, sz);
	::SendMessage(GetDlgItem(IDC_EDIT_CHARGE)->m_hWnd, EM_SETSEL, 0, -1);
	::SetFocus(GetDlgItem(IDC_EDIT_CHARGE)->m_hWnd);
	return 0;  //  set the focus
}

void CChangeCharge::OnOK()
{
	wchar_t sz[64];
	::GetDlgItemTextW(m_hWnd, IDC_EDIT_CHARGE, sz, 64);
	double newCharge = _wtof(sz);
	m_fCharge = newCharge;
	///*istrstream*/ std::wstringstream iss(sz);
	//double newCharge=0;
	//if (!(iss >> newCharge).fail())
	//{
	//	m_fCharge = newCharge;
	//}
	CDialog::OnOK();
}

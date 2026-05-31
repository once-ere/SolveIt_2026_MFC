// ExaminePair.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "ExaminePair.h"
#include "Contact.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExaminePair dialog


CExaminePair::CExaminePair(Contact* pContact, CWnd* pParent /*=NULL*/) :
	m_pContact(pContact),
	CDialog(CExaminePair::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExaminePair)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CDialog::Create(IDD, pParent);
	LONG dwNewLong = GetWindowLong(m_hWnd , GWL_EXSTYLE);
	SetWindowLong(m_hWnd , GWL_EXSTYLE, dwNewLong|WS_EX_TOPMOST);
	
//	CenterWindow();
	RECT Rect, deskRect;
	CWnd* desk_hwnd = GetDesktopWindow();
	desk_hwnd->GetWindowRect(&deskRect);
	GetWindowRect(&Rect);
	int w = Rect.right - Rect.left;
	Rect.right = deskRect.right;// += w;
	Rect.left = Rect.right - w;//+= w;
	Rect.top += w/8;
	Rect.bottom += w/8;
	MoveWindow(&Rect);
	ShowWindow(SW_SHOW);
}


void CExaminePair::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExaminePair)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
///////////////////////////////////////////////////////////////////////////////
void CExaminePair::OnCancel()
{
	DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CExaminePair::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CExaminePair, CDialog)
	//{{AFX_MSG_MAP(CExaminePair)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CExaminePair message handlers

BOOL CExaminePair::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
///////////////////////////////////////////////////////////////////////////////
void CExaminePair::UpdateData()
{
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOREPOSITION|SWP_NOSIZE);

	TCHAR s[1024];
	/*ostrstream*/ std::wstringstream oss(s,1023);

	oss << m_pContact->m_body1->name << "|" << m_pContact->m_body2->name << ends;
	SetWindowText(s);

	oss.seekp(0);
	oss << m_pContact->normal << ends;
	SetDlgItemText(IDC_EDIT_normal, s);

	oss.seekp(0);
	oss << m_pContact->m_pointContact1 << ends;
	SetDlgItemText(IDC_EDIT_pointContact1, s);

	oss.seekp(0);
	oss << m_pContact->m_pointContact2 << ends;
	SetDlgItemText(IDC_EDIT_pointContact2, s);
///////////////////////////////////////////////////////////////////////////////

	Vector3D vel1 = m_pContact->m_body1->ComputeVelocityOfPointInBody(m_pContact->m_pointContact1);
	Vector3D vel2 = m_pContact->m_body2->ComputeVelocityOfPointInBody(m_pContact->m_pointContact2);
	oss.seekp(0);
	oss << vel1 << ends;
	SetDlgItemText(IDC_EDIT_VelPointContact1, s);

	oss.seekp(0);
	oss << vel2 << ends;
	SetDlgItemText(IDC_EDIT_VelPointContact2, s);
///////////////////////////////////////////////////////////////////////////////

	oss.seekp(0);
	oss << m_pContact->m_separation << ends;
	SetDlgItemText(IDC_EDIT_separation, s);

	oss.seekp(0);
	switch (m_pContact->m_ContactType)
	{
		case Contact::Separated:	oss << "Separated" << ends;
		break;
		case Contact::InContact:	oss << "InContact" << ends;
		break;
		case Contact::Penetrating:	oss << "Penetrating" << ends;
		break;
	}
	SetDlgItemText(IDC_EDIT_ContactType, s);

	oss.seekp(0);
	switch (m_pContact->m_RelativeNormalMotion)
	{
		case Contact::Stationary:	oss << "Stationary" << ends;
		break;
		case Contact::Separating:	oss << "Separating" << ends;
		break;
		case Contact::Colliding:	oss << "Colliding" << ends;
		break;
		case Contact::Sliding:	oss << "Sliding" << ends;
		break;
	}
	SetDlgItemText(IDC_EDIT_RelativeNormalMotion, s);
}
///////////////////////////////////////////////////////////////////////////////

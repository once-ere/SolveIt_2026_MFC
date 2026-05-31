// ExamineCollisionTimes.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"

//#include "MainFrm.h"
//#include "ChildFrm.h"
//#include "ActionDoc.h"
//#include "ActionView.h"
#include "RigidBodyManager.h"

#include "System.h"
#include "RigidBody.h"
#include "Contact.h"
#include "ExamineCollisionTimes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
	long ToLongSize(size_t value)
	{
		return static_cast<long>(value);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExamineCollisionTimes dialog


CExamineCollisionTimes::CExamineCollisionTimes(CSystem* sys, CWnd* pParent /*=NULL*/)
	: CDialog(CExamineCollisionTimes::IDD, pParent),
	m_vecCollisionTimes(sys->m_vecCollisionTimes),
	psys(sys)
{
	//{{AFX_DATA_INIT(CExamineCollisionTimes)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CDialog::Create(IDD, pParent);
	ShowWindow(SW_SHOW);
}


void CExamineCollisionTimes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExamineCollisionTimes)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
///////////////////////////////////////////////////////////////////////////////
void CExamineCollisionTimes::OnCancel()
{
	DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CExamineCollisionTimes::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CExamineCollisionTimes, CDialog)
	//{{AFX_MSG_MAP(CExamineCollisionTimes)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamineCollisionTimes message handlers

void CExamineCollisionTimes::UpdateData()
{
	OnInitDialog();
LRESULT line = ::SendMessage (GetDlgItem(IDC_EDIT1)->m_hWnd,EM_GETLINECOUNT, 0, 0);
long wParam = (WPARAM) 0; // characters to scroll horizontally 
LPARAM lParam = line; // lines to scroll vertically 
::SendMessage (GetDlgItem(IDC_EDIT1)->m_hWnd,EM_LINESCROLL, wParam, lParam);
}

void CSystem::DestroyExamineCollisionTimes()
{
	if (0==examineCollisionTimes) return;//.get()
	if (!IsWindow(examineCollisionTimes->m_hWnd)) return;//.get()
//	if (examineCollisionTimes->IsWindow()) examineCollisionTimes->DestroyWindow();
//	delete examineCollisionTimes;
//	examineCollisionTimes->PostMessage(WM_CLOSE,0,0);
//	examineCollisionTimes.release();
//	examineCollisionTimes = auto_ptr<CExamineCollisionTimes>(0);
	delete examineCollisionTimes;
	examineCollisionTimes=0;
}


BOOL CExamineCollisionTimes::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	style |= WS_EX_TOPMOST;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, style);

	::SetTimer (m_hWnd, 1, 200, NULL);

	if (m_vecCollisionTimes.empty()) return 1;
	SetDlgItemText(IDC_EDIT1, 0);

	long size = 32 * ToLongSize(m_vecCollisionTimes.size());

	TCHAR *s = new TCHAR[size+1];

	/*ostrstream*/ std::wstringstream oss(s,size);

	vector<double>::iterator it;
	for (it=m_vecCollisionTimes.begin();it!=m_vecCollisionTimes.end();it++)
	{
		oss << setprecision(24) << *it << "\r\n";
	}

	oss << ends;
	SetDlgItemText(IDC_EDIT1, s);

	delete[]s;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExamineCollisionTimes::OnTimer(UINT_PTR nIDEvent) 
{
	::SetWindowPos (m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
						SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	
	CDialog::OnTimer(nIDEvent);
}

void CExamineCollisionTimes::OnDestroy() 
{
	CDialog::OnDestroy();
	::KillTimer (m_hWnd, 1);
}


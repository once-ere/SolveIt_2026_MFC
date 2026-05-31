// CurrentLoop.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "CurrentLoop.h"
#include "FieldsManager.h"

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

/*
static HCURSOR _threeCursors[3] = {
								::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ARROW)),
								::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CROSS)),
								::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SIZEALL))
};
*/

HCURSOR CCurrentLoop::_threeCursors[3];
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCurrentLoop dialog

//CCurrentLoop::CCurrentLoop(CWnd* pParent )	: CDialog(CCurrentLoop::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CCurrentLoop)
//		// NOTE: the ClassWizard will add member initialization here
//	//}}AFX_DATA_INIT
//}
///////////////////////////////////////////////////////////////////////////////
// CCurrentLoop
CCurrentLoop::CCurrentLoop(CFieldsManager* pCFieldsManager, CWnd* pParent):
	CDialog(CCurrentLoop::IDD, pParent),
	m_pCFieldsManager(pCFieldsManager),
	m_currentLoop(pCFieldsManager->m_currentLoop),
	m_currentLoopDeltas(pCFieldsManager->m_currentLoopDeltas),
	m_currentLoopDeltasMidpoint(pCFieldsManager->m_currentLoopDeltasMidpoint),
	m_WindowWidth(318),
	m_WindowHeight(318),
	m_radius(130),
	m_bDragging(false)
{
	m_NumPoints = ToLongSize(m_ptCircuitNodes.size());
//		Create(m_virtualBodyManager->m_hWnd);
//		RECT rc;
//		GetClientRect(&rc);
//		::MoveWindow(m_hWnd, 2*rc.right,0, rc.right, 2*rc.bottom,TRUE);
//		ShowWindow( SW_SHOW );
	CDialog::Create(IDD, pParent);
	ShowWindow(SW_SHOW);
}


void CCurrentLoop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurrentLoop)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
///////////////////////////////////////////////////////////////////////////////
void CCurrentLoop::OnCancel()
{
	DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////
void CCurrentLoop::PostNcDestroy()
{
	delete this;
}
///////////////////////////////////////////////////////////////////////////////
void CCurrentLoop::OnDestroy() 
{
	CDialog::OnDestroy();
	
//	DestroyWindow();
	
}
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCurrentLoop, CDialog)
	//{{AFX_MSG_MAP(CCurrentLoop)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_Reset, OnClickedReset)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCurrentLoop::RecordPath()
{
//	EnterCriticalSection(&m_parent->m_csLock);

	m_currentLoopDeltas.clear();
	m_currentLoopDeltasMidpoint.clear();

	for (long i=0;i< m_NumPoints;i++)
	{
		m_currentLoop[i] = InvTransform(m_ptCircuitNodes[i]);
	}

	for ( auto i=0;i<m_NumPoints;i++) {
		Vector3D p(m_currentLoop[(i+1)%m_NumPoints]-m_currentLoop[i]);
		Vector3D q = (m_currentLoop[(i+1)%m_NumPoints] + m_currentLoop[i])/ 2.0;
		m_currentLoopDeltas.push_back(p);
		m_currentLoopDeltasMidpoint.push_back(q);
	}
//	LeaveCriticalSection(&m_parent->m_csLock);
	Invalidate();
	Fire_ViewChange();
}


vector<RECT>::iterator CCurrentLoop::HitTest(const POINT& pt)
{
	vector<RECT>::iterator i_ptCircuitRects;
	for (i_ptCircuitRects=m_ptCircuitRects.begin();i_ptCircuitRects!=m_ptCircuitRects.end(); ++i_ptCircuitRects)
	{
		if (PtInRect(&(*i_ptCircuitRects), pt)) 
		{
			return i_ptCircuitRects;
		}
	}
	return m_ptCircuitRects.end();//i_ptCircuitRects;
}


const int border = 24;

//RECT CCurrentLoop::TransformToRect(double x, double y){}

POINT CCurrentLoop::Transform(double x, double y)
{
	POINT p;
	p.x = border + int(m_radius*(1.0+x));
	p.y = border + int(m_radius*(1.0-y));
	return p;
}

Vector3D CCurrentLoop::InvTransform(const POINT& pt)
{
	Vector3D p;
	double x = double(pt.x - border);
	double y = double(pt.y - border);
	p.x = x/m_radius-1.0;
	p.y = 1.0-y/m_radius;
	return p;
}
/////////////////////////////////////////////////////////////////////////////////
void CCurrentLoop::Initialize()
{
	const double T = 2 * PI / MAXPOINTS;

//	m_currentLoop.clear();
	m_ptCircuitNodes.clear();
	m_ptCircuitRects.clear();
	vector<Vector3D>::const_iterator i_currentLoop;

	const int w = 4;
//	EnterCriticalSection(&m_parent->m_csLock);
//	for (int i=0;i< MAXPOINTS;i++)
	for (i_currentLoop=m_currentLoop.begin();i_currentLoop!=m_currentLoop.end();++i_currentLoop)
	{
		m_ptTypes.push_back(PT_BEZIERTO);
		const Vector3D& p = *i_currentLoop;//(cos(i*T), sin(i*T),0);
	//	m_currentLoop.push_back(p);
		POINT pt = Transform(p.x, p.y);
		m_ptCircuitNodes.push_back(pt);
		RECT rc = {pt.x-w,pt.y-w,pt.x+w,pt.y+w};
		m_ptCircuitRects.push_back(rc);
	}
//	LeaveCriticalSection(&m_parent->m_csLock);
//	m_ptTypes[0] = PT_MOVETO;
	m_ptTypes[MAXPOINTS-1] |= PT_CLOSEFIGURE;
	m_NumPoints = ToLongSize(m_ptCircuitNodes.size());

	i_SelectedCircuitRect = m_ptCircuitRects.end();

	RecordPath();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCurrentLoop message handlers

void CCurrentLoop::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*
	int xPos = point.x;  // horizontal position of cursor 
	int yPos = point.y;  // vertical position of cursor 
	POINT pt={xPos,yPos};
	ScreenToClient(&pt);
*/

	i_SelectedCircuitRect = HitTest(point);//pt);
	if (i_SelectedCircuitRect!=m_ptCircuitRects.end())
	{
//		::SetCursor(_threeCursors[2]);
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));//IDC_SIZEALL
		m_bDragging = true;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CCurrentLoop::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging && i_SelectedCircuitRect!=m_ptCircuitRects.end())
	{
	//	RECT& rc = *i_SelectedCircuitRect;
	//	long n = distance( m_ptCircuitRects.begin(), i_SelectedCircuitRect);
	//	POINT& pt = m_ptCircuitNodes[n];
	//	pt.x = (rc.left+rc.right)/2;
	//	pt.y = (rc.top+rc.bottom)/2;
	//	i_SelectedCircuitRect = m_ptCircuitRects.end();
		RecordPath();
	}
//	::SetCursor(_threeCursors[0]);
	m_bDragging = false;
	Invalidate();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CCurrentLoop::OnMouseMove(UINT nFlags, CPoint point) 
{
/*
	int xPos = point.x;  // horizontal position of cursor 
	int yPos = point.y;  // vertical position of cursor 
	POINT pt={xPos,yPos};
	ScreenToClient(&pt);
*/

	if (m_bDragging && i_SelectedCircuitRect!=m_ptCircuitRects.end())
	{
		RECT& rc = *i_SelectedCircuitRect;
		long n = ToLongSize(distance(m_ptCircuitRects.begin(), i_SelectedCircuitRect));
		POINT& p = m_ptCircuitNodes[n];
		p.x = (rc.left+rc.right)/2;
		p.y = (rc.top+rc.bottom)/2;
		CPoint &pt=point;
		OffsetRect(&rc,pt.x-p.x,pt.y-p.y);
		Invalidate();
		return;
	}

	vector<RECT>::iterator i_ptCircuitRects = HitTest(point);//pt);
	if (i_ptCircuitRects==i_SelectedCircuitRect &&i_SelectedCircuitRect!=m_ptCircuitRects.end())
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SEL)));//IDC_SIZEALL
	else if (i_ptCircuitRects!=m_ptCircuitRects.end())
		::SetCursor((struct HICON__ *) ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));//IDC_CROSS
	else ::SetCursor(reinterpret_cast<HCURSOR>(GetClassLongPtr(m_hWnd, GCLP_HCURSOR)));//_threeCursors[0]);
	   
	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CCurrentLoop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Initialize();
//	SetWindowPos(&wndTop, 0,0,m_WindowWidth,m_WindowHeight, SWP_NOZORDER|SWP_SHOWWINDOW );
	SetWindowText(_T("Drag the circuit nodes to desired geometrical configuration"));
	CenterWindow();
	ShowWindow( SW_SHOW );
	return 0;  // Let the me set the focus
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurrentLoop::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	WORD nWidth = cx;  // width of client area 
	WORD nHeight = cy; 
	if (nHeight<1) return;
	return;
	m_WindowWidth=nWidth, m_WindowHeight=nHeight;
	m_radius = m_WindowWidth<m_WindowHeight? m_WindowWidth:m_WindowHeight;
	m_radius /= 2.0;
}
///////////////////////////////////////////////////////////////////////////////
void CCurrentLoop::OnClickedReset()
{
	m_pCFieldsManager->InitializePath();
	Initialize();
	Invalidate();
	Invalidate();
}
///////////////////////////////////////////////////////////////////////////////


void CCurrentLoop::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	HDC hdc = dc.m_hDC;
	RECT rc;
	GetClientRect(&rc);

//	FillRect(hdc, &rc, (HBRUSH) (COLOR_BACKGROUND+1));
	FillRect(hdc, &rc, (HBRUSH) GetStockObject(WHITE_BRUSH));

	HPEN hpen = CreatePen(PS_SOLID,2,COLORREF(RGB(0,255,0)));
	void* oldpen = SelectObject(hdc, &hpen );

//	PolyDraw95(hdc, &m_ptCircuitNodes.front(), &m_ptTypes.front(), m_NumPoints);
	vector<POINT>::iterator i=m_ptCircuitNodes.begin();
	POINT pt;
	MoveToEx(hdc, i->x, i->y, &pt);
	for (++i;i!=m_ptCircuitNodes.end();++i)
	{
		LineTo(hdc, i->x, i->y);
	}
	i=m_ptCircuitNodes.begin();
	LineTo(hdc, i->x, i->y);


	::SelectObject(hdc, oldpen );

	
	// Do not call CDialog::OnPaint() for painting messages
}


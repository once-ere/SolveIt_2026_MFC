// MagneticField.cpp : Implementation of CSystem

#include "stdafx.h"
#include "SolveIt.h"
#include "FieldsManager.h"
#include "RigidBodyManager.h"

#include "resource.h"       // main symbols
#include "CurrentLoop.h"

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
/**********************************************************************
 *                                                                    *
 * FUNCTION:  PolyDraw95(HDC, LPPOINT, LPBYTE, int)                   *
 *                                                                    *
 * PURPOSE:   Draws the points returned from a call to GetPath()      *
 *            to an HDC                                               *
 *                                                                    *
 * NOTES:     This function is similar to the Windows NT PolyDraw     *
 *            function, which draws a set of line segments and Bezier *
 *            curves. Because PolyDraw is not supported in Windows 95 *
 *            this PolyDraw95 function is used instead.               *
 *                                                                    *
 *********************************************************************/
void PolyDraw95(HDC  hdc,				//for handle of a device context
                const POINT* lppt,		// array of points
                const BYTE* lpbTypes,	// line and curve identifiers
                int  cCount)			// count of points
{
//	if (m_ThisIsWinNT) {
//		::PolyDraw ( hdc, lppt, lpbTypes, cCount);
//		return;
//	}
//	if (!m_ThisIsWin95) return;

	MoveToEx (hdc, lppt[0].x, lppt[0].y, NULL);//otherwise closed curve
	int i;

	for (i=0; i<cCount; i++)
		switch (lpbTypes[i]) {
			case PT_MOVETO :
				MoveToEx(hdc, lppt[i].x, lppt[i].y, NULL);
			break;
 
			case PT_LINETO | PT_CLOSEFIGURE:
			case PT_LINETO :
				LineTo(hdc, lppt[i].x, lppt[i].y);
			break;
 
			case PT_BEZIERTO | PT_CLOSEFIGURE:
			case PT_BEZIERTO :
				PolyBezierTo(hdc, &lppt[i], 3);
				i+=2;
			break;
	}
	LineTo(hdc, lppt[0].x, lppt[0].y);
}

///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::MagneticField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D B(m_ExternalMagneticField);
	try{
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	B += rigidBodyManager->MagneticField (p, pBody);
	
	
	long NumPoints = ToLongSize(m_currentLoopDeltas.size());
	if (NumPoints<3) return B;
//	EnterCriticalSection(&m_csLock);
	vector<Vector3D>::iterator i_currentLoopDeltas;
	vector<Vector3D>::iterator i_currentLoopDeltasMidpoint = m_currentLoopDeltasMidpoint.begin();
	double R;
	for (	i_currentLoopDeltas=m_currentLoopDeltas.begin();
			i_currentLoopDeltas!=m_currentLoopDeltas.end();
			++i_currentLoopDeltas, ++i_currentLoopDeltasMidpoint)
	{
		Vector3D q = p - *i_currentLoopDeltasMidpoint;
		R = q.norm();
		Vector3D b = (*i_currentLoopDeltas).vectorProd(q);
		b /= (R*R*R);
		b *= m_currentLoopMagneticFieldStrength;
		B += b;
	}
	B /= NumPoints;
//	LeaveCriticalSection(&m_csLock);

/*	
	IWebBrowser2* pIWebBrowser2=0;
	IServiceProvider* pISP=0;
	HRESULT hr = m_spClientSite->QueryInterface(IID_IServiceProvider, (void **)&pISP);
	if (SUCCEEDED(hr))
	{
		pISP->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void **)&pIWebBrowser2);
		pISP->Release();
	}
	wchar_t sz[512];
	std::wstringstream oss(sz,sizeof(sz));//ostrstream
	oss << B.x << "  " << B.y << "  " << B.z << ends;
	std::wstring bstr(sz);
	EnterCriticalSection(&m_csLock);
	if (pIWebBrowser2) pIWebBrowser2->put_StatusText(bstr);
	LeaveCriticalSection(&m_csLock);
	pIWebBrowser2->Release();
*/
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return B;
}

//Vector3D CurrentLoopMagneticField(CSystem* psys, const Point3D& p)//double x, double y, double z)
//{
//	return psys->fieldsManager->MagneticField (p);
//}

void CFieldsManager::OnCurrentLoopMagneticField(double currentLoopMagneticFieldStrength)
{
	m_currentLoopMagneticFieldStrength = currentLoopMagneticFieldStrength;
	m_bValidCurrentLoopMagneticField = m_currentLoopMagneticFieldStrength==0.0? false:true;
	if (m_bValidCurrentLoopMagneticField)
	{
		if (!m_pCCurrentLoop) RecordPath();
	}
}

void CFieldsManager::RecordPath()
{
	InitializePath();
//	CCurrentLoop cl(this);
//	cl.DoModal();
	m_pCCurrentLoop = new CCurrentLoop (this);
//	ReDraw();
//	CCurrentLoop* pcl = new CCurrentLoop(this);
//	pcl->Create(m_hWnd);
//	pcl->ShowWindow(SW_SHOW);
//	pcl->CenterWindow();
//	delete pcl;

/*
	const long MAXPOINTS = (3*16);
	const double T = 2 * PI / MAXPOINTS;
	vector<POINT> m_ptCircuitNodes;
	vector<BYTE> m_ptTypes;
	vector<POINT> m_ptFlatPath;
	vector<BYTE> m_ptFlatPathTypes;
	m_currentLoop.clear();
	for (int i=0;i< MAXPOINTS;i++)
	{
		m_ptTypes.push_back(PT_BEZIERTO);
		Vector3D p(cos(i*T), sin(i*T),0);
		m_currentLoop.push_back(p);
		POINT pt = {int(m_WindowWidth*p.x),int(m_WindowHeight*p.y)};
		m_ptCircuitNodes.push_back(pt);
	}
	m_ptTypes[MAXPOINTS-1] |= PT_CLOSEFIGURE;
	long NumPoints = m_ptCircuitNodes.size();

	CWindow w;
	w.Attach(GetDlgItem(IDC_FRAME));
	HDC hDC = w.GetDC();
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP bm = CreateCompatibleBitmap(hDC, m_WindowWidth, m_WindowHeight);
	void* bmold = ::SelectObject(hMemDC, &bm );
	RECT R={0,0,m_WindowWidth, m_WindowHeight};
	FillRect (hMemDC, &R, (HBRUSH) (COLOR_WINDOW+1));//GetClassLong (m_hWnd, GCL_HBRBACKGROUND));
//	HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
//	FillRect(hdcMem, &rc, hbrBkGnd);
//	DeleteObject(hbrBkGnd);
	if (::BeginPath(hMemDC)) {
		MoveToEx (hMemDC, m_ptCircuitNodes[0].x, m_ptCircuitNodes[0].y, NULL);
//<-otherwise generates a closed curve, not the intended curve
		PolyDraw95(hMemDC, &m_ptCircuitNodes.front(), &m_ptTypes.front(), MAXPOINTS);
//		cMemDC.Polyline ( m_ptCircuitNodes, MAXPOINTS);

		if (::EndPath(hMemDC)) {
			if (::FlattenPath(hMemDC)) NumPoints = ::GetPath( hMemDC, (LPPOINT)NULL, (LPBYTE)NULL, 0);
			if (NumPoints > 0) {
				m_ptFlatPath.reserve(NumPoints);
				m_ptFlatPathTypes.reserve(NumPoints);
				int nNumPoints = ::GetPath( hMemDC, &m_ptFlatPath.front(), &m_ptFlatPathTypes.front(), NumPoints);
				ATLASSERT(NumPoints==nNumPoints);
			}
		}
	}

	m_currentLoopDeltas.clear();
	m_currentLoopDeltasMidpoint.clear();

	for (i=0;i<NumPoints;i++)
	{
		Vector3D p(double(m_ptFlatPath[i].x)/m_WindowWidth, double(m_ptFlatPath[i].y)/m_WindowHeight, 0 );
		m_currentLoop.push_back(p);
	}
*/

/*
	long NumPoints = m_currentLoop.size();
	for (long i=0;i<NumPoints;i++) {
		Vector3D p(m_currentLoop[(i+1)%NumPoints]-m_currentLoop[i]);
		Vector3D q = m_currentLoop[(i+1)%NumPoints].AddDiv(m_currentLoop[i], 2.0);
		m_currentLoopDeltas.push_back(p);
		m_currentLoopDeltasMidpoint.push_back(q);
	}
	{
	wchar_t buf[4096];
	std::wstringstream oss(buf,sizeof(buf));//ostrstream
	oss<<"# points in path = "<<NumPoints<<endl<<endl;
	for (int i=0;i<NumPoints;i++) {
		double x = m_currentLoopDeltas[i].x;
		double y = m_currentLoopDeltas[i].y;
		double X = m_currentLoopDeltasMidpoint[i].x;
		double Y = m_currentLoopDeltasMidpoint[i].y;
		oss<<"("<<x<<","<<y<<")    "<<"    ("<<X<<","<<Y<<")"<<endl;
	}
	oss<<ends;
	MessageBox(buf);
	}

	::SelectObject( hMemDC , &bmold );
	::DeleteObject( &bm);
	::DeleteDC (hMemDC);
	w.ReleaseDC(hDC);
	w.Detach();
*/
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::Set_LENNARD_JONES(const Vector3D& val)//const double& x,const double& y,const double& z)
{
	m_fLennardJones = val;
	if (m_fLennardJones.x==0.0)
		m_bLennardJones_IsValid = false;
	else
	{
		if (m_fLennardJones.z<=0.0)
		{
			m_fLennardJones.x = 0;
			m_bLennardJones_IsValid = false;
			throw SolveIt_ERROR(L"Non-zero repulsive contribution is required.");
		}
		if (m_fLennardJones.y<=0.0)
		{
			m_fLennardJones.x = 0;
			m_bLennardJones_IsValid = false;
			throw SolveIt_ERROR(L"Non-zero attractive contribution is required.");
		}
		m_fLennardJones.x = sqrt(m_fLennardJones.z / m_fLennardJones.y);
		m_bLennardJones_IsValid = true;
	}
}

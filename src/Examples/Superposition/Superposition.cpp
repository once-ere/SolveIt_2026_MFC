// Superposition.cpp : Implementation of CSuperposition
#include "stdafx.h"
#include "SolveIt.h"
#include "GLView.h"
#include "Superposition.h"
#include "WaveNumbers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSuperposition
CSuperposition::CSuperposition(CGLView* pCView) :
	m_pCView(pCView),
	m_bMouseDown(false),
	m_Wavelength(0.2), 
	m_dt(0.003125), 
	m_Ratio(1.05),
	m_NumberOfWaveNumbers(3) 
{
}

CSuperposition::~CSuperposition()
{
	ClearCurves();
}





int CSuperposition::Show()
{
//	::ShowWindow(m_pISuperposition->m_hWnd, SW_SHOW );
	return S_OK;
}

int CSuperposition::Hide()
{
//	::ShowWindow(m_pISuperposition->m_hWnd, SW_HIDE );
	return S_OK;
}

int CSuperposition::Destroy()
{
	m_pCView = 0;
	return S_OK;
}

int CSuperposition::Initialize()
{
	return S_OK;
}



void CSuperposition::Draw()
{
	long H = 500;
	long W = 500;
	RECT rc = {0,0,W,H};


/*
	RECT rect;
	HWND hwnd = 0;
	HRESULT hr = m_pCView->get_Window((long *)&hwnd);
	GetClientRect(hwnd, &rect);
	const double g_eye_z = 10;
	double halfWidth = (rect.right - rect.left)/2.0;
	double ratio = 1.0/halfWidth;
	double z = fabs(g_eye_z+m_pCView->dis+m_pCView->ddis - m_pCView->m_fTransZ);
	double theta = m_pCView->m_fFOVy*DEG_TO_RAD/2;
	double fTan = tan(theta);
	double x_max = z*fTan, &y_max=x_max;

#define SCALEX(xPos)		(-m_pCView->m_fTransX + x_max*(-1+ratio*xPos))
#define SCALEY(yPos)		(-m_pCView->m_fTransY + y_max*(ratio*yPos ))
*/
#define SCALEX(xPos)		(sc*(xPos))
#define SCALEY(yPos)		(sc*(yPos))
const double sc = 0.35;

	m_pCView->GetClientRect(&rc);
	H = rc.bottom;
	W = rc.right;
//##	m_pCView->BeginDraw(W,H);//calls Draw3DAxes

	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);		 
	}
	glPushMatrix();
	FamilyOfCurves::iterator it;
	double n = static_cast<double>(m_curves.size());
	for (it=m_curves.begin(); it!=m_curves.end(); ++it)
	{
		double g =  distance(m_curves.begin(), it)/n;
		glColor3d (1.0-g, g, 1.0);
		glBegin (GL_LINE_STRIP);
		const VertexData& vd = **it;
		vector<_RealVertex>::const_iterator i;
		for (i=vd.begin(); i!=vd.end(); ++i)
		{
			const _RealVertex& v = *i;
			glVertex3d(v.x,SCALEY(v.y),v.z);
		//	glVertex3d( SCALEX(v.x), SCALEY(v.y), v.z);
		}
		glEnd ();
	}
	glPopMatrix();
///////////////////////////////////////////////////////////////////////////////
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//##	HDC hDC = m_pCView->GetDC();
//##	m_pCView->EndDraw(W, H, hDC);
///////////////////////////////////////////////////////////////////////////////
//##	::ReleaseDC(m_pCView->m_hWnd, hDC); 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
	HFONT hStockFont=NULL,hOldFont=NULL;
	CComQIPtr<IFont,&IID_IFont> pFont(m_pCView->m_pFont);
	if(pFont) pFont->get_hFont(&hStockFont);

	if(hStockFont) hOldFont = (HFONT)SelectObject(hDC,hStockFont);

	SetTextColor(hDC, RGB(128,88,255));
	SetBkMode(hDC, TRANSPARENT);
	USES_CONVERSION;
	LPCTSTR lpsz = OLE2T(m_pCView->m_bstrCaption);
	DrawText(hDC, lpsz, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_BOTTOM);
//DT_VCENTER  DT_TOP | DT_CENTER
	if(hOldFont) SelectObject(hDC,hOldFont);
*/
}


///////////////////////////////////////////////////////////////////////////////
void CSuperposition::CalculateCurves()
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
     
//	const double TwoPi = 6.28318530717958647693;
	ClearCurves();
	cdouble zero(0,0);
	const long N_T	= 64;
	const long N_X	= 4*N_T;
	double L		= N_X/32.0;// 8
	double dx		= L/N_X;//    1/32

	const double hackToDealWith_glFrustum_DepthOfField = 3;

	for (long T=0;T<N_T;T++)
	{
		VertexData* vd = new VertexData;
		double  t = T; 
		for (long j=0;j<N_X;j++)
		{
			double x = -L/2 + j*dx;
			m_PlaneWaves.clear();
			RealData::iterator k;
			for (k=m_WaveNumbers.begin();k!=m_WaveNumbers.end();++k)
				m_PlaneWaves.push_back( PlaneWave(*k,  x,  t*m_dt ) );
			double n = static_cast<double>(m_PlaneWaves.size()); 
			cdouble c = accumulate(m_PlaneWaves.begin(), m_PlaneWaves.end(), zero);//, sum<complex>());
			_RealVertex v;
			v.x = x;
			v.y = norm(c)/(pow(n, 1.33));//	v.y = norm(c)/(n*sqrt(n));
			v.z = (t-N_T/2)*4*dx-hackToDealWith_glFrustum_DepthOfField;
			vd->push_back(v);
		}
		m_curves.push_back(vd);
	}

	Fire_ViewChange();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void CSuperposition::Reset() 
{
	m_bMouseDown = false;
	Fire_ViewChange();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void CSuperposition::ClearCurves()
{
	if ( m_curves.empty() ) return;
	FamilyOfCurves::iterator it;
	for (it=m_curves.begin(); it!=m_curves.end(); ++it)
	{
		delete *it;
	}
	m_curves.clear();
}


void CSuperposition::OnNewWaveNumbers() 
{
	const double TwoPi = 6.28318530717958647693;

	m_WaveNumbers.clear();

	double k= TwoPi / m_Wavelength; 
	m_WaveNumbers.push_back(k);
	for (long j=1;j<m_NumberOfWaveNumbers;++j)
	{
		k *= m_Ratio; 
		m_WaveNumbers.push_back(k);
	}

	CalculateCurves();
}


int CSuperposition::OnEditNumberOfWaveNumbers()
{
//	CWaveNumbers wn(this, m_pCView->m_hWnd);
	CWaveNumbers wn;

	wn.m_Wavelength				= m_Wavelength; 
	wn.m_dt						= m_dt; 
	wn.m_Ratio					= m_Ratio; 
	wn.m_NumberOfWaveNumbers	= m_NumberOfWaveNumbers; 

	if (IDOK != wn.DoModal()) return S_OK;

	m_Wavelength			= wn.m_Wavelength; 
	m_dt					= wn.m_dt; 
	m_Ratio					= wn.m_Ratio; 
	m_NumberOfWaveNumbers	= wn.m_NumberOfWaveNumbers; 

	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::OnUpdate(CWaveNumbers* pwn)
{
	CWaveNumbers& wn=*pwn;
	m_Wavelength			= wn.m_Wavelength; 
	m_dt					= wn.m_dt; 
	m_Ratio					= wn.m_Ratio; 
	m_NumberOfWaveNumbers	= wn.m_NumberOfWaveNumbers; 

	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::OnViewReset()
{
	Reset();
	return S_OK;
}

int CSuperposition::OnResetParameters()
{
	m_bMouseDown			= false;
	m_Wavelength			= 0.2; 
	m_dt					= 0.003125;
	m_Ratio					= 1.05;
	m_NumberOfWaveNumbers	= 3; 
	OnNewWaveNumbers();

	return S_OK;
}

int CSuperposition::get_NumberOfWaveNumbers(long *pVal)
{
	*pVal = m_NumberOfWaveNumbers;
	return S_OK;
}

int CSuperposition::put_NumberOfWaveNumbers(long newVal)
{
	m_NumberOfWaveNumbers = newVal;
	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::get_Wavelength(double *pVal)
{
	*pVal = m_Wavelength;
	return S_OK;
}

int CSuperposition::put_Wavelength(double newVal)
{
	m_Wavelength = newVal;
	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::get_Ratio(double *pVal)
{
	*pVal = m_Ratio;
	return S_OK;
}

int CSuperposition::put_Ratio(double newVal)
{
	m_Ratio = newVal;
	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::get_DeltaTime(double *pVal)
{
	*pVal = m_dt;
	return S_OK;
}

int CSuperposition::put_DeltaTime(double newVal)
{
	m_dt = newVal;
	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::Calculate()
{
	OnNewWaveNumbers();
	return S_OK;
}

int CSuperposition::SetParameters(	long newNumberOfWaveNumbers,
											double newWavelength,
											double newRatio,
											double newDeltaTime	)
{
	m_NumberOfWaveNumbers = newNumberOfWaveNumbers;
	m_Wavelength = newWavelength;
	m_Ratio = newRatio;
	m_dt = newDeltaTime;
	OnNewWaveNumbers();
	return S_OK;
}

// qmstationary1d.cpp : implementation file
//

#include "stdafx.h"
#include "..\SolveIt.h"
#include "qmstationary1d.h"
#include "Cruncher.h"
#include "System.h"
#include "GLView.h"
#include "lib/Lib3D/gl2ps.h"
#include "MainFrm.h"
#include "DataQM_Stationary1D.h"

extern GL2PScontext *gl2ps;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQMStationary1D

IMPLEMENT_DYNCREATE(CQMStationary1D, CWinThread)

CQMStationary1D::CQMStationary1D():
	m_SolveMethod((int&)theApp.GetSystem().m_SolveMethod),
	eigenvalueIndex(theApp.GetSystem().data__QM_Stationary1D->eigenvalueIndex),
	energy_eigenvalue(theApp.GetSystem().data__QM_Stationary1D->energy_eigenvalue),
	M(theApp.GetSystem().data__QM_Stationary1D->M),

	m_NDATA(theApp.GetGLView()->m_NDATA),
//	m_fScaleWave(theApp.GetSystem().m_fScaleWave),
	m_fScaleWave(theApp.GetGLView()->m_fScaleWave),
/*
	abscissa(new double[m_NDATA+1]),
	plot_potential(new double[m_NDATA+1]),
	wave_function(new double[m_NDATA+1]),
	dwave_function(new double[m_NDATA+1]),
*/
	abscissa(0),
	plot_potential(0),
	wave_function(0),
	dwave_function(0),

	m_bStepCompleted(false),
//	m_bUseRadial(true),
//	m_NDATA(100),
//	x_max(1000),

	dx(10),
	x_min(theApp.GetGLView()->glFrustum_left),
	x_max(theApp.GetGLView()->glFrustum_right),

//	M(8+1./3.),
//	m_L0(0.00573645),
//	m_L1(1),
//	Omega0(57.5567)
	m_L0(1),
	m_L1(1),
	Omega0(1)
{
	dx		= (x_max - x_min)/(m_NDATA);

//	SetMuOverTwoM(0.12);
//	abscissa.is(0.0, m_NDATA+1);
//	plot_potential.is(0.0, m_NDATA+1);
//	wave_function.is(0.0, m_NDATA+1);
//	dwave_function.is(0.0, m_NDATA+1);
}

///////////////////////////////////////////////////////////////////////////////
CQMStationary1D::~CQMStationary1D()
{
	delete[] abscissa;
	delete[] plot_potential;
	delete[] wave_function;
	delete[] dwave_function;
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::Initialize(void)
{
/*
	abscissa.clear();
	plot_potential.clear();
	wave_function.clear();
	dwave_function.clear();

	wave_function.is(0.0, m_NDATA+1);
	dwave_function.is(0.0, m_NDATA+1);

	vector_of_double X;
	vector_of_double V;
*/
	double x=0;
	CSystem& m_system	= theApp.GetSystem();
	double v=0;
	for (long j=0; j<=m_NDATA; ++j)
	{
		x = j*dx;
	//	X.push_back(x);
		v=m_system.GetUserDefinedPotential(x, m_system.__Potential);
	//	V.push_back(v);
		abscissa[j]			= x;
		plot_potential[j]	= v;
		wave_function[j]	= 0;
		dwave_function[j]	= 0;
	}
//	abscissa		= X;
//	plot_potential	= V;
}


///////////////////////////////////////////////////////////////////////////////
BOOL CQMStationary1D::InitInstance()
{

	abscissa		= new double[m_NDATA+1];
	plot_potential	= new double[m_NDATA+1];
	wave_function	= new double[m_NDATA+1];
	dwave_function	= new double[m_NDATA+1];

	Initialize();
	m_bStepCompleted	= true;
	SetEvent(g_EventHandle);
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int CQMStationary1D::ExitInstance()
{
	// TODO:  perform any per-thread cleanup herem_system.m_pCQMStationary1D->m_system.m_pCQMStationary1D->
	CSystem& m_system	= theApp.GetSystem();
	CloseHandle (m_hThread);
	m_hThread		= 0;
	m_system.data__QM_Stationary1D->m_pCQMStationary1D		= 0;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CQMStationary1D, CWinThread)
	//{{AFX_MSG_MAP(CQMStationary1D)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQMStationary1D message handlers
///////////////////////////////////////////////////////////////////////////////
//	CSystem& m_system = theApp.GetSystem();
//char* hData = m_system.player->SendMessage(WM_USER_SetParam, 'f', 0);
//GlobalFree(hData);
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnSetParam(WPARAM wParam, LPARAM lParam)
{
		return ;
	try
	{
		switch (int(wParam))
	{
		case 'R':
			{
			CGLView* pGLView = theApp.GetGLView();
			if (pGLView == 0) return ;
			pGLView->Invalidate();
			}
			break;
//		case 'r':m_bUseRadial = true;
//			break;
//		case 'c':m_bUseRadial = false;
//			break;
		case 'm':
		{
			char* s =(char*) lParam;
			if (s == NULL) return ;
			istrstream iss(s, _tcslen(s));
			double x=0;
			if(!(iss>>x).fail())
			{
				SetMuOverTwoM(x);
			}
			GlobalFree(s);
		}
			break;
		case 'L':
		{
			char* s =(char*) lParam;
			if (s == NULL) return ;
			istrstream iss(s, _tcslen(s));
			double x=0;
			if(!(iss>>x).fail())
			{
				SetL0(x);
			}
			GlobalFree(s);
		}
			break;
	}
		return ;
/*
		char buf[64];
		ostrstream oss(buf, sizeof buf);
		oss << 23.6  << ends;
		int cb = _tcslen(buf) + 1;
		HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
		LPSTR   lpszNew = (LPSTR)GlobalLock(hgCF_TEXTData);
		if (_tcslen(buf)) _tcscpy(lpszNew, buf);
		GlobalUnlock(hgCF_TEXTData);
		return (long)lpszNew; 
*/
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,"OnSetParam Error: ",  exc->what(), MB_OK);
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,"OnSetParam Error: ", except->name, MB_OK);
	}
	catch (char * str)
	{
		::MessageBox(0,"OnSetParam Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,"Unknown error.", "OnGetParam", MB_OK);
	}
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LPSTR dupString(LPCSTR lpsz);
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{ 
	CSystem& m_system	= theApp.GetSystem();
//	m_system.data__QM_Stationary1D->energy_eigenvalue		= energy_eigenvalue;
		m_system.UpdateTree();
	m_bStepCompleted=true; 
{
//	char buf[256];
//	ostrstream oss(buf, sizeof buf);
//	oss << "Energy eigenvalue #" << eigenvalueIndex << " = " << (float)energy_eigenvalue  << ends;
//	string str(buf);
//	energy_eigenvalue = data__QM_Stationary1D->energy_eigenvalue;
	_variant_t	v_eignum(eigenvalueIndex);
	_bstr_t		b_eignum(v_eignum);
	_bstr_t		bstr1(L"Eigenvalue #");
	bstr1 += b_eignum;
	_variant_t	v_eig(energy_eigenvalue);
	_bstr_t		b_eig(v_eig);
	_bstr_t		bstr2(L"Eigenvalue energy = ");
	bstr2 += b_eig;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PostMessage(WM_USER_Message, 0, (LPARAM) dupString("Done."));//SetStatusBarText("Running ...", 2);
	pFrame->PostMessage(WM_USER_Message, 1, (LPARAM) dupString((const char*)bstr1));
	pFrame->PostMessage(WM_USER_Message, 2, (LPARAM) dupString((const char*)bstr2));
//	pFrame->PostMessage(WM_USER_Message, 2, (LPARAM) dupString(buf));//SetStatusBarText(str, 2);
}
	Fire_ViewChange();
//	SuspendThread();
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnStep(WPARAM wParam, LPARAM lParam) 
{ 
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_bStepCompleted=false; 
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PostMessage(WM_USER_Message, 0, (LPARAM) dupString("Running ..."));//SetStatusBarText("Running ...", 2);
	MakeTimeStep(); 
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
	SetCursor(LoadCursor(NULL, IDC_ARROW));
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0); 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CQMStationary1D::MakeTimeStep(void)
{
//	Initialize();
	CSystem& m_system	= theApp.GetSystem();
	try
	{
	long	numx		= m_NDATA+1;
	long	iflag[1]	= {0};

//double	*xef		= &abscissa.front();
//double	*ef			= &wave_function.front();
//double	*pdef		= &dwave_function.front();

	double	*xef		= abscissa;
	double	*ef			= wave_function;
	double	*pdef		= dwave_function;

callsledge_(&numx, &eigenvalueIndex, &energy_eigenvalue, xef, ef, pdef, iflag);
	long ret = iflag[0];

	if (ret >= 0)
	{
		switch(m_SolveMethod)//m_bUseRadial)
		{
		case CSystem::radial://true:
		case CSystem::pseudoCartesian://false:
			NormalizeRADIAL();
			break;
		case CSystem::Cartesian://false:
			Normalize();
			break;
		}
//		CGLView* pGLView = theApp.GetGLView();
//		if (pGLView == 0) return 1;
//		pGLView->Invalidate();
	}

	}
	catch (char * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox("ERROR: ::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::NormalizeRADIAL()
{
try
{
	double sum = 0;
	for (long j=0;j<=m_NDATA; ++j)
	{
		double amp = wave_function[j]*wave_function[j]*j*dx;
		sum += amp;
	}
//	double y = sqrt(1.0/sum);
	for ( j=0;j<=m_NDATA; ++j)
	{
		wave_function[j] = wave_function[j]*wave_function[j]*j*dx/sum;//probability density
	}
//	sum = 0;
//	for ( j=0;j<=m_NDATA; ++j)
//	{
//		sum +=wave_function[j];
//	}
//	double s=sum;
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	return;
}
/*
	try {
		double x=1;
		vector_of_double r;//(m_NDATA+1)
		vector_of_double t = wave_function;
		for (long j=0; j<=m_NDATA; ++j) t[j] *= sqrt(j*dx);
		double norm = t.norm();
		if (norm > 0.0) {
			x = 1.0/norm;
			t=x*t;
		}
		wave_function=t*t;//probability density
	}
	catch(struct _exception *)//except)
	{
		return;
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {}
*/
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::Normalize()
{
try
{
	double sum = 0;
	for (long j=0;j<=m_NDATA; ++j)
	{
		double amp = wave_function[j]*wave_function[j];
		sum += amp;
	}
//	double y = sqrt(double(1+m_NDATA)/sum);
	for ( j=0;j<=m_NDATA; ++j)
	{
		wave_function[j] = wave_function[j]*wave_function[j]/sum;//probability density
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	return;
}
/*
	try {
		double x=1;
		vector_of_double t = wave_function;
		double norm = t.norm();
		if (norm > 0.0) {
			x = 1.0/norm;
			t=x*t;
		}
		wave_function=t*t;//probability density
//		PsiAtZero=t(0);
	}
	catch(struct _exception *)//except)
	{
//		_emap.OnError(except);
//		throw except;
		return;
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {}
*/
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::DrawSystem()
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	if (m_NDATA == 0) return;
	if(!pGLView->m_hWnd || !::IsWindow(pGLView->m_hWnd))
	{
		return;
	}


	GLenum e=GL_NO_ERROR;
	ASSERT( m_NDATA > 1);
///////////////////////////////////////////////////////////////////////////////
	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	const double	&y1	= pGLView->glFrustum_bottom;
	const double	&y2	= pGLView->glFrustum_top;
	double r = 0;
	double Width	= x2 - x1;
	double Height	= y2 - y1;
	long j=0, x=0;
try
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);	 
	}

// Need to turn on GL_DITHER for 3D objects and turn it off for lines and points.
	glShadeModel(GL_FLAT);
	glDisable(GL_DITHER);

	glLineWidth(2.0f);
	glColor3d(0,0,1);
//	double *v	= &n.ArrayV.front();//v(double *)DATA_PASS_mp_POTENTIAL;
	glBegin(GL_LINE_STRIP);
	for (j=0;j<=m_NDATA; ++j)
	{
		r = j*dx;
	//	glVertex2d(r, m_fScalePotential*n.ArrayV[j]);
		glVertex2d(r, plot_potential[j]);
	}
	glEnd();
///////////////////////////////////////////////////////////////////////////////
//	glColor3d(0.7490, 0.6353, 0.3843);
	glColor3d(0.75,0,1.0);
	glBegin(GL_LINE_STRIP);
	for (j=0;j<m_NDATA; ++j)
	{
		r = j*dx;
		glVertex2d(r, m_fScaleWave*wave_function[j]);
	}
	glEnd();
///////////////////////////////////////////////////////////////////////////////
{
	static char buf[256];
	ostrstream oss(buf, sizeof buf);
	oss << "Energy eigenvalue #" << eigenvalueIndex << " = " << (float)energy_eigenvalue  << ends;
//	string str(buf);

	GLfloat	fX = 0.065f*(pGLView->glFrustum_right + pGLView->glFrustum_left);
	GLfloat	fY = 0.75f*(pGLView->glFrustum_top + pGLView->glFrustum_bottom);
	glRasterPos3f(fX , fY,0);
		if (gl2ps)
		{
		//	gl2psText(str.c_str(), "Courier", 12);
			gl2psText(buf, "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
		//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glCallLists(strlen(buf), GL_BYTE, (GLbyte *)buf);
			glPopAttrib();
		}
}
///////////////////////////////////////////////////////////////////////////////
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
	glPopAttrib();
	glFlush();//With multithread applications, flush the drawing commands in any other threads drawing to the same window before calling SwapBuffers
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ASSERT(0);
		return;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	::MessageBox(NULL,_T("CFortranWrapper::DrawSystemPotential"),_T("General Exception"),MB_OK);
	return;
}
}
///////////////////////////////////////////////////////////////////////////////




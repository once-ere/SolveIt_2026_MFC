// qmstationary1d.cpp : implementation file
//
//#pragma optimize( "", off )

#include "stdafx.h"

#include "SolveIt.h"

#include "..\SolveIt.h"
#include "QMStationary1D.h"
#include "Cruncher.h"
#include "System.h"
#include "GLView.h"
#include "../SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "MainFrm.h"
//#include "DataQM_Stationary1D.h"
#include "parser.h" /* for token definitions and yylval */

extern GL2PScontext *gl2ps;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef long  logical;
/////////////////////////////////////////////////////////////////////////////
// CQMStationary1D
extern "C"  long _stdcall SLEDGE(logical *job, double *cons, logical *endfin, 
	long *invec, double *tol, logical *type__, double *ev, 
	long *numx, double *xef, double *ef, double *pdef, 
	double *t, double *rho, long *iflag, double *store);

extern "C"   int sledge_(logical *job, double *cons, logical *endfin, 
	long *invec, double *tol, logical *type__, double *ev, 
	long *numx, double *xef, double *ef, double *pdef, 
	double *t, double *rho, long *iflag, double *store);

///////////////////////////////////////////////////////////////////////////////

/* Subroutine */ int callsledge_(long *numx, long *indexeigen, 
	double *ev, double *xef, double *ef, double *pdef, 
	long *iflag)
{
	/* Initialized data */

	static double cons[8] = { 0.,0.,1.,0.,1.,0.,0.,0. };
	static logical endfin[2] = { TRUE,FALSE };
	static long invec[4] = { 1,3,1,0 };
	static logical job[5] = { FALSE,TRUE,TRUE,FALSE,FALSE };
	static double tol[6] = { 1e-5,1e-4,1e-5,1e-4,1e-5,1e-4 };
	static double t[3] = { 0.,.5,2. };

	static double rho[3];
	static logical type__[8]	/* was [4][2] */;
	static double store[2450];

/*      INTEGER IFLAG(NUMEigen), INVEC(3+NUMEigen) */

/*     Load the boundary condition information into CONS(*). */
/*     This example has a Neumann condition at A = 1, and a */
/*     singular point at B = +infinity. */

	/* Parameter adjustments */
	--iflag;
	--pdef;
	--ef;
	--xef;
	--ev;

	/* Function Body */

/*     The eigenfunctions will be estimated at 5 points. */

/*      DATA NUMX,XEF/5, 1.0, 1.5D0, 2.0, 4.0, 100.0/ */

/*     Initialize the vector INVEC(*): */
/*        little printing, */
/*        3 output points for the density function Rho(t), */
/*        estimates for the first (index 0) eigenvalue/function. */


/*     Set the JOB(*) vector: */
/*        estimate both eigenvalues and eigenvectors, */
/*        estimate the spectral density function, */
/*        classify, */
/*        force the initial mesh to be the output points. */


/*     Set the tolerances: */


/*     Initialize the 3 output points for the density function. */


/*     Open file for output. */

/*      OPEN(21,FILE = 'sample.out') */
	invec[0] = 0;
	invec[1] = 0;
	invec[2] = 1;
	invec[3] = *indexeigen;
/*      DO I=1, NUMEigen */
/* 		INVEC(3+I) = indexEigen */
/*      enddo */
/*                                  Compute eigenvalue and eigenfunctions */
	job[0] = FALSE;
	job[1] = TRUE;
	job[2] = FALSE;
	job[3] = FALSE;
	job[4] = FALSE;
//SLEDGE
//sledge_
//	SLEDGE(job, cons, endfin, invec, tol, type__, &ev[1], numx, &xef[1], &ef[1], &pdef[1], t, rho, &iflag[1], store);
	return 0;
} /* callsledge_ */
///////////////////////////////////////////////////////////////////////////////
extern "C" long _stdcall COEFF(double *x, double *px, double *qx, double *rx)
{
		double V=0;
	CSystem* m_system	= theApp.GetSystem();
// either call is OK:

		V = m_system->GetUserDefinedPotential(*x, m_system->__Potential);
	//	V = m_system->GetUserDefinedPotential( m_system->__Potential, 1, double(*x) );


/*     Be careful with potential over/underflows; here we assume the */
/*     IEEE double precision exponent range. */

/*      IF (X .LT. 650.0) THEN */
/*         T = EXP(-X) */
/*      ELSE */
/*         T = 0.0 */
/*      ENDIF */
	switch(m_system->m_pCQMStationary1D->m_SolveMethod)//m_bUseRadial)
	{
	case CSystem::radial://true:
		*px = *x;
		*qx = m_system->m_pCQMStationary1D->M*V**x;
		*rx = m_system->m_pCQMStationary1D->M**x;
		break;
	case CSystem::pseudoCartesian://false:
		*px = 1.0;
		*qx = m_system->m_pCQMStationary1D->M*V;
		*rx = m_system->m_pCQMStationary1D->M;
		*qx+= -0.25/(*x**x);
		break;
	case CSystem::Cartesian://false:
		*px = 1.0;
		*qx = m_system->m_pCQMStationary1D->M*V;
		*rx = m_system->m_pCQMStationary1D->M;
		break;
	}
	return 0;
} /* coeff_ */
/***********************************************************************
 *      These routines estimate eigenvalues, eigenfunctions and/or     *
 *      spectral density functions for Sturm-Liouville problems.       *
 *      The differential equation has the form:                        *
 *                                                                     *
 *           -(p(x)u')' + q(x)u  =  EV*r(x)u       for x in [A,B]      *
 *                                                                     *
 *      with boundary conditions (at regular points)                   *
 *                                                                     *
 *           A1*u - A2*(pu')  =  EV*(A1'*u - A2'*(pu'))    at A        *
 *           B1*u + B2*(pu')  =  0                         at B .      *
 *                                                                     *
 *      The functions p(x) and r(x) are assumed to be positive in      *
 *      the open interval (A,B).                                       *
 ***********************************************************************/

///////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CQMStationary1D, CWinThread)

CQMStationary1D::CQMStationary1D():
	m_SolveMethod((int&)theApp.GetSystem()->m_SolveMethod),
	eigenvalueIndex(theApp.GetSystem()->eigenvalueIndex),
	energy_eigenvalue(theApp.GetSystem()->energy_eigenvalue),
	M(theApp.GetSystem()->M),

	m_NDATA(theApp.GetGLView()->m_NDATA),
//	m_fScaleWave(theApp.GetSystem()->m_fScaleWave),
	m_fScaleWave(theApp.GetGLView()->m_fScaleWave),
/*
	abscissa(new double[m_NDATA+1]),
	plot_potential(new double[m_NDATA+1]),
	wave_function(new double[m_NDATA+1]),
	dwave_function(new double[m_NDATA+1]),
*/
	abscissa(0),
	wave_function(0),
	dwave_function(0),
	plot_potential(0),
	probability_density(0),

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
//	Initialize();
//	SetMuOverTwoM(0.12);
//	abscissa.is(0.0, m_NDATA+1);
//	plot_potential.is(0.0, m_NDATA+1);
//	wave_function.is(0.0, m_NDATA+1);
//	dwave_function.is(0.0, m_NDATA+1);
}

///////////////////////////////////////////////////////////////////////////////
CQMStationary1D::~CQMStationary1D()
{
	try
	{
	if (abscissa)delete[] abscissa;
	if (wave_function)delete[] wave_function;
	if (dwave_function)delete[] dwave_function;
	if (plot_potential)delete[] plot_potential;
	if (probability_density)delete[] probability_density;
	}
	catch (...)
	{
		::MessageBox(0,L"Unknown error.", L"CQMStationary1D::~CQMStationary1D", MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::Initialize(void)
{
	try
	{
	m_bStepCompleted=false; 
	if (abscissa)delete[] abscissa;
	if (wave_function)delete[] wave_function;
	if (dwave_function)delete[] dwave_function;
	if (plot_potential)delete[] plot_potential;
	if (probability_density)delete[] probability_density;
	abscissa		= new double[m_NDATA+1];
	if (!abscissa) throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid abscissa");
	wave_function	= new double[m_NDATA+1];
	if (!wave_function) throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid wave_function");
	dwave_function	= new double[m_NDATA+1];
	if (!dwave_function) throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid dwave_function");
	plot_potential	= new double[m_NDATA+1];
	if (!plot_potential) throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid plot_potential");
	probability_density	= new double[m_NDATA+1];
	if (!probability_density) throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid probability_density");

/*
	wave_function.is(0.0, m_NDATA+1);
	dwave_function.is(0.0, m_NDATA+1);

	vector_of_double X;
	vector_of_double V;
*/
	dx		= (x_max - x_min)/(m_NDATA);
	if (dx <= 0.0)
	{
		throw SolveIt_ERROR(L"CQMStationary1D::Initialize: Invalid dx");
	}
	double x=0;
	CSystem* m_system	= theApp.GetSystem();
	double v=0;
	double off=dx/100;
	for (long j=0; j<=m_NDATA; ++j)
	{
		x = x_min + j*dx;
	//	X.push_back(x);
		v=m_system->GetUserDefinedPotential(x+off, m_system->__Potential);
	//	V.push_back(v);
		abscissa[j]			= x;
		plot_potential[j]	= v;
		wave_function[j]	= 0;
		dwave_function[j]	= 0;
	}
//	abscissa		= X;
//	plot_potential	= V;
//	m_bStepCompleted	= false;
	m_bStepCompleted	= true;
	Fire_ViewChange();

//	CGLView* pGLView = theApp.GetGLView();
//	if (pGLView == 0) return;
//	pGLView->Invalidate();
	}
	catch (...)
	{
		::MessageBox(0,L"Unknown error.", L"CQMStationary1D::Initialize", MB_OK);
				AfxEndThread( 0 );
				return;
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CQMStationary1D::InitInstance()
{

	Initialize();
//	SetEvent(g_EventHandle);
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int CQMStationary1D::ExitInstance()
{
//	return CWinThread::ExitInstance();
	// TODO:  perform any per-thread cleanup herem_system->m_pCQMStationary1D->m_system->m_pCQMStationary1D->
	CSystem* m_pCSystem	= theApp.GetSystem();
	if (m_pCSystem != NULL && m_pCSystem->m_pCQMStationary1D != NULL)
	{
		m_pCSystem->m_pCQMStationary1D	= 0;
	}
//	CloseHandle (m_hThread);
//	m_hThread		= 0;
//	m_system->m_pCQMStationary1D		= 0;
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
//	CSystem* m_system = theApp.GetSystem();
//wchar_t* hData = m_system->player->SendMessage(WM_USER_SetParam, 'f', 0);
////GlobalFree(hData);
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnSetParam(WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch (int(lParam))
		{
			case WM_PAINT:
			{
				if (m_bStepCompleted) DrawSystem();
			}
			case T_NODES:
			case T_SYSTEM_RIGHT:
			case T_SYSTEM_LEFT:
			{
				Initialize();
			}
			break;
			case WM_CLOSE:
			{
				AfxEndThread( 0 );
		//		PostQuitMessage(0);
		return ;
	CSystem* m_pCSystem	= theApp.GetSystem();
	if (m_pCSystem != NULL && m_pCSystem->m_pCQMStationary1D != NULL)
	{
	//	CloseHandle (m_pCSystem->m_pCQMStationary1D->m_hThread);
		m_pCSystem->m_pCQMStationary1D	= 0;
	}
//	PostThreadMessage(WM_QUIT, 0, 0);
			}
			break;
		}
		return ;
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
			wchar_t* s =(wchar_t*) lParam;
			if (s == NULL) return ;
			const std::wstring source(s);
			std::wistringstream iss(source);
			double x=0;
			if(!(iss>>x).fail())
			{
				SetMuOverTwoM(x);
			}
			//GlobalFree(s);
		}
			break;
		case 'L':
		{
			wchar_t* s =(wchar_t*) lParam;
			if (s == NULL) return ;
			const std::wstring source(s);
			std::wistringstream iss(source);
			double x=0;
			if(!(iss>>x).fail())
			{
				SetL0(x);
			}
			//GlobalFree(s);
		}
			break;
	}
		return ;
/*
		wchar_t buf[64];
		std::wstringstream oss(buf, sizeof buf);//ostrstream 
		oss << 23.6  << ends;
		int cb = _tcslen(buf) + 1;
		HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
		std::wstring    lpszNew = (std::wstring )GlobalLock(hgCF_TEXTData);
		if (_tcslen(buf)) _tcscpy(lpszNew, buf);
		GlobalUnlock(hgCF_TEXTData);
		return (long)lpszNew; 
*/
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(exc->what()), MB_OK);
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
	}
	catch (...)
	{
		::MessageBox(0,L"Unknown error.", L"OnGetParam", MB_OK);
	}
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{
	try {
		CSystem* m_system	= theApp.GetSystem();
//	m_system->energy_eigenvalue		= energy_eigenvalue;
		m_bStepCompleted=true; 
		m_system->UpdateTree();
{
//	wchar_t buf[256];
//	/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
//	oss << "Energy eigenvalue #" << eigenvalueIndex << " = " << (float)energy_eigenvalue  << ends;
//	wstring str(buf);
//	energy_eigenvalue = energy_eigenvalue;
	_variant_t	v_eignum(eigenvalueIndex);
	std::wstring		b_eignum = std::to_wstring(eigenvalueIndex);// (v_eignum);
	std::wstring		bstr1(L"Eigenvalue #");
	bstr1 += b_eignum;
	_variant_t	v_eig(energy_eigenvalue);
	std::wstring		b_eig = std::to_wstring(energy_eigenvalue);//(v_eig);
	std::wstring		bstr2(L"Eigenvalue energy = ");
	bstr2 += b_eig;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PostMessage(WM_USER_Message, 0, (LPARAM) GetWC("Done."));//SetStatusBarText("Running ...", 2);
	pFrame->PostMessage(WM_USER_Message, 1, (LPARAM)bstr1.c_str());// GetWC((const wchar_t*)bstr1));
	pFrame->PostMessage(WM_USER_Message, 2, (LPARAM)bstr2.c_str());//  GetWC((const wchar_t*)bstr2));
//	pFrame->PostMessage(WM_USER_Message, 2, (LPARAM) GetWC(buf));//SetStatusBarText(str, 2);
}
	Fire_ViewChange();
//	SuspendThread();
//	return 0; 
//	PostQuitMessage(0);//	AfxEndThread( 0 );
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);//	AfxEndThread( 0 );//
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"ERROR: ::MakeTimeStep");
		PostQuitMessage(0);//	AfxEndThread( 0 );//
	}
} 
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnStep(WPARAM wParam, LPARAM lParam) 
{
	try {
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_bStepCompleted=false; 
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PostMessage(WM_USER_Message, 0, (LPARAM) GetWC("Running ..."));//SetStatusBarText("Running ...", 2);
	MakeTimeStep(); 
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
	SetCursor(LoadCursor(NULL, IDC_ARROW));
//	return 0; 
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);//	AfxEndThread( 0 );//
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"ERROR: ::MakeTimeStep");
		PostQuitMessage(0);//	AfxEndThread( 0 );//
	}
} 
///////////////////////////////////////////////////////////////////////////////
void CQMStationary1D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0);//	AfxEndThread( 0 );// 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long CQMStationary1D::MakeTimeStep(void)
{
//	Initialize();
	CSystem* m_system	= theApp.GetSystem();
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
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);//	AfxEndThread( 0 );//
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox(L"ERROR: CQMStationary1D::MakeTimeStep");
		PostQuitMessage(0);//	AfxEndThread( 0 );//
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
	if (sum <= 0.0) return;
	sum *= TwoPI;
	double y = sqrt(1.0/sum);
	for ( auto j=0;j<=m_NDATA; ++j)
	{
		probability_density[j] = wave_function[j]*wave_function[j]*j*dx/sum;//probability density
		wave_function[j] = wave_function[j]*y;
	}
			ofstream ofs((const wchar_t*)"wv.dat");//, ios::out|ios::text);
			if (ofs) {
//				for (j=0;j<=m_NDATA; ++j) ofs << j*dx << "," << wave_function[j] << endl;
				for (auto j=0;j<=m_NDATA; ++j) ofs << j*dx << " " << wave_function[j] << endl;
				ofs.close();
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
	if (sum <= 0.0) return;
	double y = sqrt(1.0/sum);
//	double y = sqrt(double(1+m_NDATA)/sum);probability_density
	for ( auto j=0;j<=m_NDATA; ++j)
	{
//		wave_function[j] = wave_function[j]*wave_function[j]/sum;//probability density
		probability_density[j] = wave_function[j]*wave_function[j]/sum;//probability density
		wave_function[j] = wave_function[j]*y;
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
	if (plot_potential == NULL) return;
	if (probability_density == NULL) return;
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
		glVertex2d(r, m_fScaleWave*probability_density[j]);//wave_function
	}
	glEnd();
///////////////////////////////////////////////////////////////////////////////
{
	static wchar_t buf[256];
	/*ostrstream*/ std::wstringstream oss(buf, sizeof buf);
	oss << "Energy eigenvalue #" << eigenvalueIndex << " = " << (float)energy_eigenvalue  << ends;
//	wstring str(buf);

	GLfloat	fX = static_cast<GLfloat>(0.065f*(pGLView->glFrustum_right + pGLView->glFrustum_left));
	GLfloat	fY = static_cast<GLfloat>(0.2f*pGLView->glFrustum_top + 0.5f*(pGLView->glFrustum_top + pGLView->glFrustum_bottom));
	glRasterPos3f(fX , fY, 0);
		if (gl2ps)
		{
		//	gl2psText(str.c_str(), "Courier", 12);
			gl2psText(GetC(buf), const_cast<char*>("Courier"), 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
		//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glCallLists(static_cast<GLsizei>(_tcslen(buf)), GL_BYTE, (GLbyte *)buf);//strlen
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
///////////////////////////////////////////////////////////////////////////////
#if 0
void CQMStationary1D::Wait()
{
	CSystem* m_pCSystem	= theApp.GetSystem();
//	ASSERT(m_pCSystem == theApp.GetSystem());
	try
	{
		HANDLE events[] = {g_EventRun, g_EventStep, g_EventStop, g_EventKill};
	//	DWORD dwRet = WaitForSingleObject(g_EventStep, INFINITE);
		DWORD dwRet = MsgWaitForMultipleObjects(
				sizeof(events)/sizeof(events[0]),	// # events to wait for
				events,								// The array of events
				FALSE,								// Wait for 1 event
				INFINITE,							// Timeout value
				QS_ALLINPUT);						// Any message wakes up
		if(dwRet == WAIT_OBJECT_0+0)
			{
				OnStep(0, 0);
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+1)
			{
				OnStep(0, 0);
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+2)
			{
				Wait();
				return;
			}
		else
			if(dwRet == WAIT_OBJECT_0+3)
			{
			//	Lock lock (theApp.protectWorker);
				if(m_pCSystem != NULL && m_pCSystem->worker != NULL)
				{
					CloseHandle (m_pCSystem->worker->m_hThread);
					m_pCSystem->worker	= 0;
				}
				AfxEndThread( 0 );
				return;
			}
		else
			{
				throw SolveIt_ERROR(L"CQMStationary1D::Wait: Invalid return from MsgWaitForMultipleObjects");
				AfxEndThread( 0 );
				return;
			}
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox(L"CQMStationary1D::Wait");
	}
}
#endif
///////////////////////////////////////////////////////////////////////////////



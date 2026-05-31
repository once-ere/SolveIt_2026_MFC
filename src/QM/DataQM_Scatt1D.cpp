// DataQM_Scatt1D.cpp: implementation of the CDataQM_Scatt1D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "DataQM_Scatt1D.h"
#include "GLView.h"
#include "System.h"
#include "Box.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
CBoxCompare SolveIt::boxCompare;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
#define m_EvolutionMethod ((int&)theApp.GetSystem()->m_SolveMethod)

CDataQM_Scatt1D::CDataQM_Scatt1D():
//	m_NDATA(theApp.GetGLView()->m_NDATA),
	m_NDATA(101),
	m_pCQM_Scatt1D(0),
	NumOrder(16),
	Jn(new double[NumOrder+1]),
	im(new cdouble[NumOrder+1]),
	imM(new cdouble[NumOrder+1]),

		v(new double[m_NDATA]),
		ev(new cdouble[m_NDATA]),
		evc(new cdouble[m_NDATA]),
		u(new cdouble[m_NDATA]),
		psi_xt(new cdouble[m_NDATA]),
		psi_xt0(new cdouble[m_NDATA]),

	m_DrawSource(Initial),
	m_fWaveNumber_x(64.25),
	m_fScalePotential(1.0),
	m_fLambda(0.92),
	m_fInitialGaussianCenter_x(-2),
	m_fInitialGaussianSpread(0.25),
	m_fScalePsi(1),
	m_quantumModeSuchThatKEequalsPE(0),
	m_nNumberOfIntermediateSteps(2),
	m_nNumFrames(14),
	m_nRenormalize(0),


	g_bInitCalled(false),
	g_pavi(0),	
	g_psCompressed(0),	
	g_pfile(0),


	m_hdcMemAVI(0),
	m_hBitmapAVI(0),
	m_hBitmapOldAVI(0),
	m_pBits(0),
	m_bstrAVIFileName(L""),
	m_bMakingAVI(false),
	m_psCompressed(0),			// stream pointer
	m_timeOfThisFrame(0),		// time of this frame
	m_nImageSize(0)			// size of this frame

//,	m_EvolutionMethod((int&)theApp.GetSystem()->m_SolveMethod)

{
//	m_EvolutionMethod	= CSystem::PeriodicCayley;
//		CSystem::Nash_Chen,
//		CSystem::Cayley
	cdouble i(0.0,  1.0 );
	cdouble j(0.0, -1.0 );
	imM[0] = im[0] = cdouble(1.0, 0.0 );
	im[1]	= i;
	imM[1]	= j;
	for (long k=2;k<=NumOrder;++k)
	{
		im[k]	= i*im[k-1];
		imM[k]	= j*imM[k-1];
	}
//	INIT();
	cdouble z(0.0,  0.0 );
	for ( auto k=0;k<m_NDATA;++k)
	{
		v[k] = 0;
		u[k] = psi_xt[k] = psi_xt0[k] = ev[k] = evc[k] = z;
	}
	DefinePotentialDependentParameters();
}
///////////////////////////////////////////////////////////////////////////////
CDataQM_Scatt1D::~CDataQM_Scatt1D()
{
try
{
		delete[]imM;
		delete[]im;
		delete[]Jn;

		delete[]ev;
		delete[]evc;
		delete[]v;
		delete[]u;
		delete[]psi_xt;
		delete[]psi_xt0;
}
catch( SolveIt_Error& e){e.why();}catch(...)      // Handle all exceptions
{
	ASSERT(0);
}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CDataQM_Scatt1D::StopQM_Scatt1D(void)
{
	if (m_pCQM_Scatt1D == NULL ) return FALSE;
	DWORD ExitCode=0;
	BOOL ret = GetExitCodeThread(m_pCQM_Scatt1D->m_hThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		m_pCQM_Scatt1D->ResumeThread();
		m_pCQM_Scatt1D->PostThreadMessage(WM_QUIT, 0, 0); 
		DWORD dwRet = MsgWaitForMultipleObjects(
					1,						// One event to wait for
					&m_pCQM_Scatt1D->m_hThread,		// The array of events
					FALSE,					// Wait for 1 event
					INFINITE,						// Timeout value
					QS_ALLINPUT);			// Any message wakes up
	}
	CloseHandle (m_pCQM_Scatt1D->m_hThread);
	m_pCQM_Scatt1D	= 0;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::CreateQM_Scatt1DThread(bool bCREATE_SUSPENDED)
{
//	INIT();
	if (m_pCQM_Scatt1D)
	{
		DWORD ExitCode=0;
		BOOL ret = GetExitCodeThread(m_pCQM_Scatt1D->m_hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)
		{
			if (!bCREATE_SUSPENDED) m_pCQM_Scatt1D->ResumeThread();
			return;
		}
		else
		{
			CloseHandle (m_pCQM_Scatt1D->m_hThread);
			m_pCQM_Scatt1D	= 0;
		}
	}
	try {
			if (bCREATE_SUSPENDED) 
m_pCQM_Scatt1D =  (CQM_Scatt1D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt1D ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			else
		m_pCQM_Scatt1D =  (CQM_Scatt1D*)AfxBeginThread( RUNTIME_CLASS( CQM_Scatt1D ));
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// f(x) = (1/TwoPiToTheOneFourth)*[1/sqrt(a)]*e^(i k0 x)*e^[- ((x-x0)/2a)^2]
cdouble CDataQM_Scatt1D::GaussianWavePacket(double a, double x0, double k0, double x)
{
	const double log10			= 2.3025850929940456840;
	const double DBL_MAX_E_EXP	= DBL_MAX_10_EXP*log10;
	const double DBL_MIN_E_EXP	= DBL_MIN_10_EXP*log10;
	const double TwoPiToTheMinusOneFourth= 0.63161877774606470129;
	double xa = (x-x0)/(2*a);
	double b = TwoPiToTheMinusOneFourth/sqrt(a);
	double X = ( -1.0*xa*xa );
	if (X < DBL_MIN_E_EXP/2) return 0;
	double A = exp( X );
	double B = b*A;
	cdouble c(B*cos(k0*x), B*sin(k0*x));
	return c;
}
/////////////////////////////////////////////////////////////////////////////
// F(k) = [2/Pi]^(1/4) sqrt(a)*e^[-((k0+k)*a)^2]*e^(i k x0)
cdouble CDataQM_Scatt1D::FourierTransformGaussianWavePacket(double a, double x0, double k0, double k)
{
	const double log10			= 2.3025850929940456840;
	const double DBL_MAX_E_EXP	= DBL_MAX_10_EXP*log10;
	const double DBL_MIN_E_EXP	= DBL_MIN_10_EXP*log10;
	const double TwoOverPiToTheOneFourth=0.89324384173800233140;
	double ka = (k0+k)*a;
	double b = sqrt(a)*TwoOverPiToTheOneFourth;
	double X = ( -1.0*ka*ka );
	if (X < DBL_MIN_E_EXP/2) return 0;
	double A = exp( X );//Exp( -1.0*ka*ka );
	double B = b*A;
	cdouble c(B*cos(k*x0), B*sin(k*x0));
	return c;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long	CDataQM_Scatt1D::mu(long i, long N)
{
	long _mu = i;
	if (i< 0) _mu = _mu+N;	//enforce perodicity;
	if (i>=N) _mu = _mu-N;
	return _mu;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* for description of method see [Engeln-Mullges + Uhlig, p. 92]
 *
 *		   diag[0]	offdiag[0]				0	 .....
 *		offdiag[0]	   diag[1]		offdiag[1]	 .....
 *				0	offdiag[1]		   diag[2]	
 *				0			0		offdiag[2]	 .....
 */
int CDataQM_Scatt1D::solve_tridiag(
	const cdouble diag[], size_t d_stride,
	const cdouble offdiag[], size_t o_stride,
	const cdouble b[], size_t b_stride,
	cdouble x[], size_t x_stride,
	size_t N)
{
	int status;
//	cdouble *gamma = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble *alpha = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble *c = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble *z = (cdouble *) malloc (N * sizeof (cdouble));
	cdouble * gamma = new cdouble[N];
	cdouble * alpha = new cdouble[N];
	cdouble * c = new cdouble[N];
	cdouble * z = new cdouble[N];

	if (gamma == 0 || alpha == 0 || c == 0 || z == 0)
		{
			status = 1;//GSL_ENOMEM;
		}
	else
		{
			size_t i, j;

			/* Cholesky decomposition
				 A = L.D.L^t
				 lower_diag(L) = gamma
				 diag(D) = alpha
			 */
			alpha[0] = diag[0];
			gamma[0] = offdiag[0] / alpha[0];

			for (i = 1; i < N - 1; i++)
	{
		alpha[i] = diag[d_stride * i] - offdiag[o_stride*(i - 1)] * gamma[i - 1];
		gamma[i] = offdiag[o_stride * i] / alpha[i];
	}

			if (N > 1) 
				{
					alpha[N - 1] = diag[d_stride * (N - 1)] - offdiag[o_stride*(N - 2)] * gamma[N - 2];
				}

			/* update RHS */
			z[0] = b[0];
			for (i = 1; i < N; i++)
	{
		z[i] = b[b_stride * i] - gamma[i - 1] * z[i - 1];
	}
			for (i = 0; i < N; i++)
	{
		c[i] = z[i] / alpha[i];
	}

			/* backsubstitution */
			x[x_stride * (N - 1)] = c[N - 1];
			if (N >= 2)
	{
		for (i = N - 2, j = 0; j <= N - 2; j++, i--)
			{
				x[x_stride * i] = c[i] - gamma[i] * x[x_stride * (i + 1)];
			}
	}

			status = 0;//GSL_SUCCESS;
		}

	if (z != 0) delete[]z;//free (z);
	if (c != 0)delete[]c;//free (c);
	if (alpha != 0)delete[]alpha;//free (alpha);
	if (gamma != 0)delete[]gamma;//free (gamma);

	return status;
}
///////////////////////////////////////////////////////////////////////////////
/* for description of method see [Engeln-Mullges + Uhlig, p. 96]
 *
 *      diag[0]  offdiag[0]             0   .....  offdiag[N-1]
 *   offdiag[0]     diag[1]    offdiag[1]   .....
 *            0  offdiag[1]       diag[2]
 *            0           0    offdiag[2]   .....
 *          ...         ...
 * offdiag[N-1]         ...
 *
 */
int CDataQM_Scatt1D::solve_cyc_tridiag(
	const cdouble diag[], size_t d_stride,
	const cdouble offdiag[], size_t o_stride,
	const cdouble b[], size_t b_stride,
	cdouble x[], size_t x_stride,
	size_t N)
{
	int status;
//	cdouble * delta = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble * gamma = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble * alpha = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble * c = (cdouble *) malloc (N * sizeof (cdouble));
//	cdouble * z = (cdouble *) malloc (N * sizeof (cdouble));
	cdouble * delta = new cdouble[N];
	cdouble * gamma = new cdouble[N];
	cdouble * alpha = new cdouble[N];
	cdouble * c = new cdouble[N];
	cdouble * z = new cdouble[N];

	if (delta == 0 || gamma == 0 || alpha == 0 || c == 0 || z == 0)
		{
			status = 1;//GSL_ENOMEM;
		}
	else
		{
			size_t i, j;
			cdouble sum = 0.0;

			/* factor */

			alpha[0] = diag[0];
			gamma[0] = offdiag[0] / alpha[0];
			delta[0] = offdiag[o_stride * (N-1)] / alpha[0];

			for (i = 1; i < N - 2; i++)
	{
		alpha[i] = diag[d_stride * i] - offdiag[o_stride * (i-1)] * gamma[i - 1];
		gamma[i] = offdiag[o_stride * i] / alpha[i];
		delta[i] = -delta[i - 1] * offdiag[o_stride * (i-1)] / alpha[i];
	}
			for (i = 0; i < N - 2; i++)
	{
		sum += alpha[i] * delta[i] * delta[i];
	}
			alpha[N - 2] = diag[d_stride * (N - 2)] - offdiag[o_stride * (N - 3)] * gamma[N - 3];
			gamma[N - 2] = (offdiag[o_stride * (N - 2)] - offdiag[o_stride * (N - 3)] * delta[N - 3]) / alpha[N - 2];
			alpha[N - 1] = diag[d_stride * (N - 1)] - sum - offdiag[o_stride * (N - 2)] * gamma[N - 2] * gamma[N - 2];

			/* update */
			z[0] = b[0];
			for (i = 1; i < N - 1; i++)
	{
		z[i] = b[b_stride * i] - z[i - 1] * gamma[i - 1];
	}
			sum = 0.0;
			for (i = 0; i < N - 2; i++)
	{
		sum += delta[i] * z[i];
	}
			z[N - 1] = b[b_stride * (N - 1)] - sum - gamma[N - 2] * z[N - 2];
			for (i = 0; i < N; i++)
	{
		c[i] = z[i] / alpha[i];
	}

			/* backsubstitution */
			x[x_stride * (N - 1)] = c[N - 1];
			x[x_stride * (N - 2)] = c[N - 2] - gamma[N - 2] * x[x_stride * (N - 1)];
			if (N >= 3)
	{
		for (i = N - 3, j = 0; j <= N - 3; j++, i--)
			{
				x[x_stride * i] = c[i] - gamma[i] * x[x_stride * (i + 1)] - delta[i] * x[x_stride * (N - 1)];
			}
	}

			status = 0;//GSL_SUCCESS;
		}

	if (z != 0) delete[]z;//free (z);
	if (c != 0)delete[]c;//free (c);
	if (alpha != 0)delete[]alpha;//free (alpha);
	if (gamma != 0)delete[]gamma;//free (gamma);
	if (delta != 0)delete[]delta;//free (delta);

	return status;
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::Clear(void)
{
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::Reset(void)
{
	OnResetViewSource();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::Step()
{
	m_DrawSource = Current;

	++m_timeOfThisFrame;
	switch (m_EvolutionMethod)
	{
		case CSystem::Nash_Chen: EVOLVE_NASH(m_nNumberOfIntermediateSteps);
			break;
		case CSystem::Cayley: EVOLVE_CAYLEY(m_nNumberOfIntermediateSteps);
			break;
		case CSystem::PeriodicCayley: EVOLVE_CAYLEY_PERIODIC(m_nNumberOfIntermediateSteps);
			break;
	}
//	theApp.theSystem.DisplayData();
	{
		Lock lock (theApp.protectBodies);
		CSystem* m_system = theApp.GetSystem();
		m_system->m__t	= m_timeOfThisFrame;
		m_system->DisplayData();
		m_system->UpdateTree();
	}
	Fire_ViewChange();

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::OnCompute()
{
///////////////////////////////////////////////////////////////////////////////
//	m_bDrawUsingInitial = false;
	m_DrawSource = Current;
	int k = 0;
	for ( k=0; k<m_nNumFrames; k++ )
	{
		m_timeOfThisFrame = k;
//		theApp.theSystem.DisplayData();
		Fire_ViewChange();

		switch (m_EvolutionMethod)
		{
		case CSystem::Nash_Chen: EVOLVE_NASH(m_nNumberOfIntermediateSteps);
			break;
		case CSystem::Cayley: EVOLVE_CAYLEY(m_nNumberOfIntermediateSteps);
			break;
		case CSystem::PeriodicCayley: EVOLVE_CAYLEY_PERIODIC(m_nNumberOfIntermediateSteps);
			break;
		}
	}
//	Draw(m_nNumFrames);
	m_timeOfThisFrame = k;
//	theApp.theSystem.DisplayData();
	Fire_ViewChange();

	return S_OK;

/*
	WriteData1D(m_bstrHDF5FileName, //"/Frames", "Number of Frames",
			   m_NDATA, //(void*) &m_nNumFrames,
			   (const void*) DATA_PASS_mp_PPSI,
			   PumpData
			   );

	ReadData1D(m_bstrHDF5FileName, m_NDATA, DoSomething);
*/

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::OnCloseMovie()
{
//	if (m_videoWnd != 0)
//	{
//		MCIWndDestroy(m_videoWnd);
//		m_videoWnd = 0;
//	}
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::get_NumberOfFrames(long *pVal)
{
	*pVal = m_nNumFrames;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::put_NumberOfFrames(long newVal)
{
	m_nNumFrames = newVal;
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::get_NumberOfIntermediateSteps(long *pVal)
{
	*pVal = m_nNumberOfIntermediateSteps;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::put_NumberOfIntermediateSteps(long newVal)
{
	m_nNumberOfIntermediateSteps = newVal;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::get_WaveNumber(double *pVal)
{
	*pVal = m_fWaveNumber_x;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::put_WaveNumber(double newVal)
{
	m_fWaveNumber_x = newVal;
//	DefinePotential();
	DefineInitialWavefunction();
	OnResetViewSource();
//	Fire_ViewChange();
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::get_Lambda(double *pVal)
{
	*pVal = m_fLambda;
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
int CDataQM_Scatt1D::put_Lambda(double newVal)
{
	m_fLambda = newVal;
//	DefinePotential();
	DefinePotentialDependentParameters();
	OnResetViewSource();
	return S_OK;
}

int CDataQM_Scatt1D::get_ScalePotential(double *pVal)
{
	*pVal = m_fScalePotential;
	return S_OK;
}

int CDataQM_Scatt1D::put_ScalePotential(double newVal)
{
	m_fScalePotential = newVal;
	DefinePotential();
	OnResetViewSource();
	return S_OK;
}

int CDataQM_Scatt1D::get_ScalePsi(double *pVal)
{
	*pVal = m_fScalePsi;
	return S_OK;
}

int CDataQM_Scatt1D::put_ScalePsi(double newVal)
{
	m_fScalePsi = newVal;
	Fire_ViewChange();
	return S_OK;
}

int CDataQM_Scatt1D::get_InitialSpread(double *pVal)
{
	*pVal = m_fInitialGaussianSpread;
	return S_OK;
}

int CDataQM_Scatt1D::put_InitialSpread(double newVal)
{
	m_fInitialGaussianSpread = newVal;
	DefineInitialWavefunction();
	OnResetViewSource();
//	Fire_ViewChange();
	return S_OK;
}

int CDataQM_Scatt1D::get_CenterOfGaussian(double *pVal)
{
	*pVal = m_fInitialGaussianCenter_x;
	return S_OK;
}

int CDataQM_Scatt1D::put_CenterOfGaussian(double newVal)
{
	m_fInitialGaussianCenter_x = newVal;
	DefineInitialWavefunction();
	OnResetViewSource();
//	Fire_ViewChange();
	return S_OK;
}


int CDataQM_Scatt1D::get_MaximumWaveNumber(double *pVal)
{
	*pVal = (m_NDATA-1)*Pi;//PiOver2;
	return S_OK;
}

int CDataQM_Scatt1D::get_EvolutionMethod(long *pVal)
{
	*pVal = m_EvolutionMethod;
	return S_OK;
}

int CDataQM_Scatt1D::put_EvolutionMethod(long newVal)
{
	if ( newVal>=CSystem::Nash_Chen && newVal <= CSystem::PeriodicCayley)
	m_EvolutionMethod =  newVal;//(EvolutionMethod)
	return S_OK;
}

int CDataQM_Scatt1D::get_FourierTransformOfWavePacketAtK(double k0, double k, double *pVal)
{
	cdouble C = FourierTransformGaussianWavePacket(m_fInitialGaussianSpread, m_fInitialGaussianCenter_x, k0, k);
	*pVal = sqrt(C.real()*C.real() + C.imag()*C.imag());
	return S_OK;
}

int CDataQM_Scatt1D::get_Renormalize(long *pVal)//wchar_t*
{
	*pVal = m_nRenormalize;
//	std::wstring b = L"off";
//	if (m_nRenormalize) b = L"on";
//	*pVal = b;
	return S_OK;
}

int CDataQM_Scatt1D::put_Renormalize(long newVal)//wchar_t*
{
	long oldVal = m_nRenormalize;
	m_nRenormalize = newVal;
	if (oldVal != m_nRenormalize) DefineInitialWavefunction();
//	const std::wstring on(L"on");
//	std::wstring b;
//	b = newVal;
//	::MessageBox(NULL,b,"",MB_OK);
//	if (b == on) m_nRenormalize=1;
//	else m_nRenormalize = 0;
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

#if 0
	long CDataQM_Scatt1D::PumpData(long& k)
	{
		if (k == -1)
		{
			DefineInitialWavefunction();
			DefinePotential();
			DefinePotentialDependentParameters();
			k = 0;
			return 1;
		}
		if (++k>=m_nNumFrames) return 0;
		switch (m_EvolutionMethod)
		{
		case Nash: EVOLVE_NASH(m_nNumberOfIntermediateSteps);
			break;
		case Cayley: EVOLVE_CAYLEY(m_nNumberOfIntermediateSteps);
			break;
		case PeriodicCayley: EVOLVE_CAYLEY_PERIODIC(m_nNumberOfIntermediateSteps);
			break;
		}
		return 1;
	}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::AVI_Term(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile)
{
	AVIStreamRelease(pavi);
	AVIStreamRelease(psCompressed);
	AVIFileRelease(pfile);  // closes the file 
	AVIFileExit();		  // releases AVIFile library 
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::AVI_Init(PAVISTREAM& pavi,PAVISTREAM& psCompressed,PAVIFILE& pfile, const LPBITMAPINFO pbmi, std::wstring bstrHDF5FileName)
{
	long nImageSize = DSImageBitsSize( pbmi );
	TCHAR sz[256];
//	PAVISTREAM			pavi			= NULL;	
//	PAVISTREAM			psCompressed	= NULL;	
//	PAVIFILE			pfile=0;
	AVICOMPRESSOPTIONS	opts;
	AVICOMPRESSOPTIONS	* aopts[1]		= {&opts};
	AVISTREAMINFO		strhdr;

	AVIFileInit();		  // opens AVIFile library 

	HRESULT hr = AVIFileOpen(&pfile, bstrHDF5FileName, OF_CREATE|OF_WRITE, NULL);
	if (hr != 0)
	{
		switch (hr) {
case AVIERR_BADFORMAT :	_tcscpy(sz,_T("The file couldn't be read, indicating a corrupt file or an unrecognized format."));break;
case AVIERR_MEMORY :	_tcscpy(sz,_T("The file could not be opened because of insufficient memory."));break;
case AVIERR_FILEREAD :	_tcscpy(sz,_T("A disk error occurred while reading the file."));break;
case AVIERR_FILEOPEN :	_tcscpy(sz,_T("A disk error occurred while opening the file."));break;
case REGDB_E_CLASSNOTREG :	_tcscpy(sz,_T("According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it. "));break;
		}
		::MessageBox(NULL,sz,"",MB_OK);
		return;
	}
	// Fill in the header for the video stream....
// The video stream will run in 15ths of a second....
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType					= streamtypeVIDEO;// stream type
	strhdr.fccHandler				= 0;
	strhdr.dwScale					= 1;
	strhdr.dwRate					= 15;	// 15 fps
	strhdr.dwSuggestedBufferSize	= nImageSize;//lpbi->bmiHeader.biSizeImage;
	::SetRect(&strhdr.rcFrame, 0, 0, (int) pbmi->bmiHeader.biWidth, (int) pbmi->bmiHeader.biHeight);
//	::SetRect(&strhdr.rcFrame, 0, 0, wXSize, wYSize);
	
	hr = AVIFileCreateStream(pfile, &pavi, &strhdr);
	ATLASSERT(pavi != 0);
	if (hr != 0) {			//Stream created OK? If not, close file. 
		AVIFileRelease(pfile); 
		MessageBox(0,L"AVIFileCreateStream: Output failed.","",MB_OK);
		return;
	} 

	memset(&opts, 0, sizeof(opts));

	if (FALSE ==
	AVISaveOptions(NULL, 0, 1, &pavi, (LPAVICOMPRESSOPTIONS FAR *) &aopts)
	)
	{

		AVIStreamRelease(pavi);
//		AVIStreamRelease(psCompressed);
		AVIFileRelease(pfile);  // closes the file 
		AVIFileExit();		  // releases AVIFile library 
		MessageBox(0,L"AVISaveOptions: Output failed.","",MB_OK);
		return;
	}

	hr = AVIMakeCompressedStream(&psCompressed, pavi, &opts, NULL);
	if (hr != 0) {			//Stream created OK? If not, close file. 
		AVIStreamRelease(pavi);
		AVIFileRelease(pfile); 
		MessageBox(0,L"AVIMakeCompressedStream: Output failed.","",MB_OK);
		return;
	} 
/*
//	LONG SizeBytesBlockMemoryReferencedBylpFormat = 
//		bi.bmiHeader.biSize + bi.bmiHeader.biClrUsed * sizeof(RGBQUAD);
//pbmi == &pbmi->bmiHeader
	hr = AVIStreamSetFormat(psCompressed, 0, &pbmi->bmiHeader, sizeof(pbmi->bmiHeader));
//	hr = AVIStreamSetFormat(psCompressed, 0, pbmi, nTotalSize);BITMAPINFOHEADER
*/
// sizeof(BITMAPINFO) + ((bits <= 8) ? 1<<bits : 0) * sizeof(RGBQUAD)
/////////////////////////////////////////////////////////////////////////////
static bool m_ThisIsWin95 = ( 0xC0000000 == (GetVersion() & 0xC0000000) )? 1:0;
static bool m_ThisIsWinNT = ( 0 == (GetVersion() & 0xC0000000) )? 1:0;

/////////////////////////////////////////////////////////////////////////////
	long nTotalSize = DSTotalBytesSize( pbmi );
if (m_ThisIsWin95)
	hr = AVIStreamSetFormat(psCompressed, 0,
				   pbmi,
				   nTotalSize
				   );
/////////////////////////////////////////////////////////////////////////////
if (m_ThisIsWinNT)
	hr = AVIStreamSetFormat(psCompressed, 0,
				   &pbmi->bmiHeader,
				   sizeof(BITMAPINFOHEADER)
				   );
///////////////////////////////////////////////////////////////////////////////
	if (hr != 0) { 
		AVIStreamRelease(pavi); 
		AVIStreamRelease(psCompressed); 
		AVIFileRelease(pfile);  // closes the file 
		AVIFileExit();		  // releases AVIFile library 
		MessageBox(0,L"AVIStreamSetFormat: Output failed.","",MB_OK);
		return; 
	} 
///////////////////////////////////////////////////////////////////////////////
//	SetCursor(LoadCursor(NULL, IDC_WAIT));
///////////////////////////////////////////////////////////////////////////////
}
#endif



///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::INIT()
{
	cdouble z(0.0,  0.0 );
	for (int i=0;i<m_NDATA;++i)
	{
		v[i] = 0;
		u[i] = psi_xt[i] = psi_xt0[i] = ev[i] = evc[i] = z;
	}
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	DefineInitialWavefunction();
	DefinePotential();
//	DefinePotentialDependentParameters();
}
///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::EVOLVE_NASH(long StepsPerIteration)
{
	cdouble*  y = new cdouble[m_NDATA];//wavefn; abscissa data points, normalized 0 <= x <= 1;
	long M,i,k;
	for (k=0;k<StepsPerIteration;++k)
	{
		for (i=0;i<m_NDATA;++i) y[i]=psi_xt[i];
		for (i=0;i<m_NDATA;++i)
		{
			psi_xt[i] = Jn[0]*y[i];
			for (M=1;M<=NumOrder;++M)
			{
				psi_xt[i] = psi_xt[i] + im[M]*Jn[M]*(y[mu(i-M,m_NDATA)]+y[mu(i+M,m_NDATA)]);
			}
			psi_xt[i] = u[i]*psi_xt[i];
		}
		if (m_nRenormalize) Renormalize(psi_xt);
	}
	delete[] y;
	return;
}// EVOLVE_NASH;
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::DefineInitialWavefunction()
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	CSystem* m_system	= theApp.GetSystem();
	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	double Width	= x2 - x1;
	double dx		= Width/(m_NDATA-1);
	double X=0;
	double v=0;
	double kx=0;
	try {

//		for (long x=0;x<m_NDATA;++x)
//		{
//			X = x1 + x*dx;
//			psi_xt[x] = GaussianWavePacket(m_fInitialGaussianSpread, m_fInitialGaussianCenter_x, m_fWaveNumber_x, X);
//		}
		for (long x=0;x<m_NDATA;++x)
		{
			X = x1 + x*dx;
			v=m_system->GetUserDefinedPotential(X, m_system->__External3);
			kx		= m_fWaveNumber_x*x;
			cdouble c(v*cos(kx), v*sin(kx));
			psi_xt[x] = c;
		}

		if (m_nRenormalize) Renormalize(psi_xt);

		for ( auto x=0;x<m_NDATA;++x)
		{
			psi_xt0[x] = psi_xt[x];
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
	}
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::DefinePotential()
{
//	CMainFrame*		pFrame			= dynamic_cast<CMainFrame*>(m_pMainWnd);
//	ASSERT (pFrame->m_system != NULL);
	CSystem* m_system	= theApp.GetSystem();
	CGLView* pGLView = theApp.GetGLView();
//-1 <= x <= +1
	ZeroMemory(v, m_NDATA*sizeof(double));
	if (m_system->m_uUserDefinedPotential.func==0 || pGLView==0)
	{
		DefinePotentialDependentParameters();
		return;
	}

	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
//	const double	&y1	= pGLView->glFrustum_bottom;
//	const double	&y2	= pGLView->glFrustum_top;
	double Width	= x2 - x1;
//	double Height	= y2 - y1;
	double dx		= Width/(m_NDATA-1);
//	double dy		= Height/(m_NDATA-1);
	double X=0;//, Y=0, Z=0;

	for(long j=0;j<m_NDATA;j++)
	{
		X = x1 + j*dx;
		v[j]=m_system->GetUserDefinedPotential(X, m_system->__Potential);
	}
	for(auto j=0;j<m_NDATA;j++) v[j] *= m_fScalePotential;
	DefinePotentialDependentParameters();
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::DefinePotentialDependentParameters()
{
	try {
		cdouble  L( 0.0, 4.0/m_fLambda);
		long x;
		for (x=0;x<=NumOrder;++x) Jn[x] = bessj(x, m_fLambda);
		for (x=0;x<m_NDATA;++x)
		{
			cdouble s(2*(1.0 + v[x]),0);
			ev[x] = -s + L;
			evc[x] = s + L;
			u[x] = cdouble( cos(m_fLambda*(1.0 + v[x])), -1.0*sin(m_fLambda*(1.0 + v[x])));
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
	}
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::EVOLVE_CAYLEY(long StepsPerIteration)
{
//gsl_vector_complex	* xxx		= gsl_vector_complex_alloc(m_NDATA);
	int s = 0;
	long k,j;
	cdouble c;
	cdouble one(1.0, 0.0 );
size_t stride = 1;

cdouble	* rhs		= new cdouble[m_NDATA];
cdouble	* x			= new cdouble[m_NDATA];
cdouble	* diag		= new cdouble[m_NDATA];
cdouble	* offdiag	= new cdouble[m_NDATA];

	for (k=0;k<StepsPerIteration;++k)
	{
		for(j=0; j<m_NDATA; j++) {
			diag[j]		= ev[j];
			offdiag[j]	= one;
		}
		for(j=1; j<m_NDATA-1; j++) 
		{
			rhs[j] = evc[j]*psi_xt[j] - psi_xt[j-1] - psi_xt[j+1];
		}
		rhs[0] = evc[0]*psi_xt[0] - psi_xt[1] - psi_xt[m_NDATA-1];
		rhs[m_NDATA-1] = evc[m_NDATA-1]*psi_xt[m_NDATA-1] - psi_xt[0] - psi_xt[m_NDATA-2];

		s +=// gsl_linalg_solve_symm_tridiag((const gsl_vector *) diag, (const gsl_vector *) offdiag, (const gsl_vector *) rhs, (gsl_vector *) x);
		solve_tridiag(	diag, stride,
						offdiag, stride,
						rhs, stride,
						x, stride,
						m_NDATA);

		for (j=0;j<m_NDATA;++j)
		{
			psi_xt[j] = x[j];
		}
		if (m_nRenormalize) Renormalize(psi_xt);
	}

	delete[] offdiag;
	delete[] diag;
	delete[] x;
	delete[] rhs;
}// EVOLVE_CAYLEY;
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::Renormalize(cdouble*psi)
{
try
{
//	cdouble* x= new cdouble[m_NDATA];
	double sum = 0;
	for (long j=0;j<m_NDATA; ++j)
	{
		cdouble C = psi[j];
	//	cdouble c(C.real(),C.imag());
		double amp = C.real()*C.real() + C.imag()*C.imag();
		sum += amp;
	}
	double y = sqrt(double(m_NDATA)/sum);
	for (auto j=0;j<m_NDATA; ++j)
	{
		psi[j] = y*psi[j];
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(NULL,_T("CQM_1DScatt::Renormalize"),_T("General Exception"),MB_OK);
	return;
}
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::EVOLVE_CAYLEY_PERIODIC(long StepsPerIteration)
{
	int s = 0;
	long k,j;
	cdouble c;
	cdouble one(1.0, 0.0 );
	size_t stride = 1;

cdouble	* rhs		= new cdouble[m_NDATA];
cdouble	* x			= new cdouble[m_NDATA];
cdouble	* diag		= new cdouble[m_NDATA];
cdouble	* offdiag	= new cdouble[m_NDATA];

	for (k=0;k<StepsPerIteration;++k)
	{
		for(j=0; j<m_NDATA; j++) {
			diag[j]		= ev[j];
			offdiag[j]	= one;
		}
		for(j=1; j<m_NDATA-1; j++) 
		{
			rhs[j] = evc[j]*psi_xt[j] - psi_xt[j-1] - psi_xt[j+1];
		}
		rhs[0] = evc[0]*psi_xt[0] - psi_xt[1] - psi_xt[m_NDATA-1];
		rhs[m_NDATA-1] = evc[m_NDATA-1]*psi_xt[m_NDATA-1] - psi_xt[0] - psi_xt[m_NDATA-2];

		s +=// gsl_linalg_solve_symm_tridiag((const gsl_vector *) diag, (const gsl_vector *) offdiag, (const gsl_vector *) rhs, (gsl_vector *) x);
		solve_cyc_tridiag(	diag, stride,
						offdiag, stride,
						rhs, stride,
						x, stride,
						m_NDATA);

		for (j=0;j<m_NDATA;++j)
		{
			psi_xt[j] = x[j];
		}
		if (m_nRenormalize) Renormalize(psi_xt);
	}

	delete[] offdiag;
	delete[] diag;
	delete[] x;
	delete[] rhs;
}
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::OnResetViewSource()
{
	m_DrawSource = Initial;
	m_timeOfThisFrame = 0;
	for (int x=0;x<m_NDATA;++x)
		psi_xt[x] = psi_xt0[x];
	Fire_ViewChange();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif
///////////////////////////////////////////////////////////////////////////////
void CDataQM_Scatt1D::Draw(int nFrameNum)
{
try
{
	CSystem* m_system	= theApp.GetSystem();
	CGLView* pGLView = theApp.GetGLView();
	ASSERT( m_NDATA > 1);
	long H = 500;
	long W = 500;
	RECT rc = {0,0,W,H};
	pGLView->GetClientRect(&rc);
	H = rc.bottom;
	W = rc.right;
	double h = H/2;
///////////////////////////////////////////////////////////////////////////////
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

	long j=0;
	double x = 0;
	double y = 0;
	double z = 0, z_Pos = 0;
//	pGLView->ComputeScreenCoords(&x, &y, &z_Pos);
	pGLView->ComputeScreenCoords(x, y, z_Pos, false);

	const double* pot = v;

	glColor3d(0,1,0);
	glLineWidth(3.0f);

	double* X= new double[m_NDATA];
#define redrawSystemPotential 
#if defined(redrawSystemPotential)
	glBegin(GL_LINE_STRIP);
	for (j=0;j<m_NDATA; ++j)
	{
		x = double(j*W)/(m_NDATA-1);
		y = h;
		z = z_Pos;
	//	pGLView->ComputeWorldCoords(&x, &y, &z);
		pGLView->ComputeWorldCoords(x, y, z, false);
		X[j] = x;
		glVertex2d(x, pot[j]/m_fScalePotential);//);
	}
	glEnd();
#else
	for (j=0;j<m_NDATA; ++j)
	{
		x = double(j*W)/(m_NDATA-1);
		y = h;
		z = z_Pos;
	//	pGLView->ComputeWorldCoords(&x, &y, &z);
		pGLView->ComputeWorldCoords(x, y, z, false);
		X[j] = x;
	}
#endif

	const double TwoPiToTheOneFourth=1.58323348708615953858;
	double Delta_x = TwoPiToTheOneFourth*sqrt(m_fInitialGaussianSpread);

	cdouble* psi = psi_xt;
	if (m_DrawSource = Initial)
		psi = (cdouble*) psi_xt0;

#define SCALEY(yPos)		((yPos)*Delta_x)
	glColor3d(1,0,0);
	glLineWidth(1.0f);
	glBegin(GL_LINE_STRIP);
	for (j=0;j<m_NDATA; ++j)
	{
		cdouble C = psi[j];
		cdouble c(C.real(),C.imag());
	//	glVertex2d(X[j],SCALEY(sqrt(C.real()*C.real() + C.imag()*C.imag())));
		double amp = sqrt(C.real()*C.real() + C.imag()*C.imag());
		glVertex2d(X[j],m_fScalePsi*(amp));
	}
	glEnd();

	glColor3d(0,0,1);
	glBegin(GL_LINE_STRIP);
	for (j=0;j<m_NDATA; ++j)
	{
		cdouble C = psi[j];
		glVertex2d(X[j], m_fScalePsi*(C.real()));
	}
	glEnd();

	glColor3d(1,1,0);
	glBegin(GL_LINE_STRIP);
	for (j=0;j<m_NDATA; ++j)
	{
		cdouble C = psi[j];
		glVertex2d(X[j],m_fScalePsi*(C.imag()));
	}
	glEnd();

	delete[] X;

	glColor3d(0,0,0);
	if (pGLView->m_pSelectedBox)
	{
		RECT& rc = pGLView->m_pSelectedBox->rc;
		glRecti(rc.left, rc.top, rc.right, rc.bottom);//Rectangle(hDC,
	}
///////////////////////////////////////////////////////////////////////////////
/*
	HFONT hStockFont=NULL,hOldFont=NULL,hOldFont2=NULL;
	CComQIPtr<IFont,&IID_IFont> pFont(m_pCView->m_pFont);
	if(pFont) pFont->get_hFont(&hStockFont);

	if(hStockFont) hOldFont2= (HFONT)SelectObject(m_pCView->m_hdcMem,hStockFont);

	SetTextColor(m_pCView->m_hdcMem, RGB(128,88,255));
	SetBkMode(m_pCView->m_hdcMem, TRANSPARENT);

	wchar_t szNumber[16];
	wsprintf(szNumber,"%04u",nFrameNum) ;

	SetBkMode(m_pCView->m_hdcMem, OPAQUE);
	SetBkColor(m_pCView->m_hdcMem, RGB(0,0,255)) ;
	SetTextColor(m_pCView->m_hdcMem, RGB(255,255,0)) ;
	ExtTextOut(
			m_pCView->m_hdcMem,
			rc.left+(rc.right-rc.left)/20,
			rc.bottom-(rc.bottom-rc.top)/10,
			ETO_CLIPPED,
			&rc,
			szNumber,
			4,
			NULL);
	if(hOldFont2) SelectObject(m_pCView->m_hdcMem,hOldFont2);
*/
///////////////////////////////////////////////////////////////////////////////
//##	HDC hDC = m_pCView->Get_pDC()->GetSafeHdc();//CGL::gl->GetGLView()
//##	m_pCView->EndDraw(W, H, hDC);

//	BitBlt( hDC, 0, 0, W, H,
//			m_pCView->m_hdcMem,
//			0, 0, SRCCOPY );
///////////////////////////////////////////////////////////////////////////////
//##	::ReleaseDC(m_pCView->m_hWnd, hDC); 
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);     
	}

	glPopMatrix();
	glPopAttrib();
	glFlush();//With multithread applications, flush the drawing commands in any other threads drawing to the same window before calling SwapBuffers

}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	::MessageBox(NULL,_T("CQM_1DScatt::Draw"),_T("General Exception"),MB_OK);
	return;
}
}

#undef SCALEX
#undef SCALEY

///////////////////////////////////////////////////////////////////////////////

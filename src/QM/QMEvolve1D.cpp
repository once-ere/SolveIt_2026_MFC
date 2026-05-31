// QMEvolve1D.cpp: implementation of the CQMEvolve1D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "GLView.h"
#include "QMEvolve1D.h"
//#include "QM_1DScatt.h"
#include "System.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//typedef matrix<cdouble, rectangle<>, dense<>, row_major>::type Matrix;
//typedef dense1D<cdouble> Vector;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::INIT()
{
	cdouble z(0.0,  0.0 );
	for (int i=0;i<NDATA;++i)
	{
		v[i] = 0;
		u[i] = psi_xt[i] = psi_xt0[i] = ev[i] = evc[i] = z;
	}
}// INIT;
///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::EVOLVE_NASH(long StepsPerIteration)
{
	cdouble*  y = new cdouble[NDATA];//wavefn; abscissa data points, normalized 0 <= x <= 1;
	long M,i,k;
	for (k=0;k<StepsPerIteration;++k)
	{
		for (i=0;i<NDATA;++i) y[i]=psi_xt[i];
		for (i=0;i<NDATA;++i)
		{
			psi_xt[i] = Jn[0]*y[i];
			for (M=1;M<=NumOrder;++M)
			{
				psi_xt[i] = psi_xt[i] + im[M]*Jn[M]*(y[mu(i-M,NDATA)]+y[mu(i+M,NDATA)]);
			}
			psi_xt[i] = u[i]*psi_xt[i];
		}
	}
	delete[] y;
	return;
}// EVOLVE_NASH;
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::DefineInitialWavefunction()
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	double Width	= x2 - x1;
	double dx		= Width/(NDATA-1);
	double X=0;
	try {
		for (long x=0;x<NDATA;++x)
		{
			X = x1 + x*dx;
			psi_xt0[x] = psi_xt[x] = GaussianWavePacket(Delta_x, InitialCenter_x, WaveNumber_x, X);
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::DefinePotentialDependentParameters()
{
	try {
		cdouble  L( 0.0, 4.0/Lambda);
		long x;
		for (x=0;x<=NumOrder;++x) Jn[x] = bessj(x, Lambda);
		for (x=0;x<NDATA;++x)
		{
			cdouble s(2*(1.0 + v[x]),0);
			ev[x] = -s + L;
			evc[x] = s + L;
			u[x] = cdouble( cos(Lambda*(1.0 + v[x])), -1.0*sin(Lambda*(1.0 + v[x])));
		}
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::EVOLVE_CAYLEY(long StepsPerIteration)
{
//gsl_vector_complex	* xxx		= gsl_vector_complex_alloc(NDATA);
	int s = 0;
	long k,j;
	cdouble c;
	cdouble one(1.0, 0.0 );
size_t stride = 1;

cdouble	* rhs		= new cdouble[NDATA];
cdouble	* x			= new cdouble[NDATA];
cdouble	* diag		= new cdouble[NDATA];
cdouble	* offdiag	= new cdouble[NDATA];

	for (k=0;k<StepsPerIteration;++k)
	{
		for(j=0; j<NDATA; j++) {
			diag[j]		= ev[j];
			offdiag[j]	= one;
		}
		for(j=1; j<NDATA-1; j++) 
		{
			rhs[j] = evc[j]*psi_xt[j] - psi_xt[j-1] - psi_xt[j+1];
		}
		rhs[0] = evc[0]*psi_xt[0] - psi_xt[1] - psi_xt[NDATA-1];
		rhs[NDATA-1] = evc[NDATA-1]*psi_xt[NDATA-1] - psi_xt[0] - psi_xt[NDATA-2];

		s +=// gsl_linalg_solve_symm_tridiag((const gsl_vector *) diag, (const gsl_vector *) offdiag, (const gsl_vector *) rhs, (gsl_vector *) x);
		solve_tridiag(	diag, stride,
						offdiag, stride,
						rhs, stride,
						x, stride,
						NDATA);

		for (j=0;j<NDATA;++j)
		{
			psi_xt[j] = x[j];
		}
	}

	delete[] offdiag;
	delete[] diag;
	delete[] x;
	delete[] rhs;
}// EVOLVE_CAYLEY;
/////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::EVOLVE_CAYLEY_PERIODIC(long StepsPerIteration)
{
	int s = 0;
	long k,j;
	cdouble c;
	cdouble one(1.0, 0.0 );
	size_t stride = 1;

cdouble	* rhs		= new cdouble[NDATA];
cdouble	* x			= new cdouble[NDATA];
cdouble	* diag		= new cdouble[NDATA];
cdouble	* offdiag	= new cdouble[NDATA];

	for (k=0;k<StepsPerIteration;++k)
	{
		for(j=0; j<NDATA; j++) {
			diag[j]		= ev[j];
			offdiag[j]	= one;
		}
		for(j=1; j<NDATA-1; j++) 
		{
			rhs[j] = evc[j]*psi_xt[j] - psi_xt[j-1] - psi_xt[j+1];
		}
		rhs[0] = evc[0]*psi_xt[0] - psi_xt[1] - psi_xt[NDATA-1];
		rhs[NDATA-1] = evc[NDATA-1]*psi_xt[NDATA-1] - psi_xt[0] - psi_xt[NDATA-2];

		s +=// gsl_linalg_solve_symm_tridiag((const gsl_vector *) diag, (const gsl_vector *) offdiag, (const gsl_vector *) rhs, (gsl_vector *) x);
		solve_cyc_tridiag(	diag, stride,
						offdiag, stride,
						rhs, stride,
						x, stride,
						NDATA);

		for (j=0;j<NDATA;++j)
		{
			psi_xt[j] = x[j];
		}
	}

	delete[] offdiag;
	delete[] diag;
	delete[] x;
	delete[] rhs;
}
/////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::DefinePotential()
{
	CSystem* m_system = theApp.GetSystem();
//-1 <= x <= +1
	ZeroMemory(v, NDATA*sizeof(double));

	if (m_system->m_uUserDefinedExternal1.func == 0)
	{
		DefinePotentialDependentParameters();
		return;
	}


	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;

	double Width	= x2 - x1;
	double dx		= Width/(NDATA-1);
	double X=0;//, Y=0, Z=0;

	for(long j=0;j<NDATA;j++)
	{
		X = x1 + j*dx;
		v[j] = m_system->GetUserDefinedPotential(X, m_system->__Potential);
	}

	for(auto j=0;j<NDATA;j++) v[j] *= m_fScalePotential;
	DefinePotentialDependentParameters();
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMEvolve1D::OnResetViewSource()
{
	m_DrawSource = CQMEvolve::Initial;
	for (int x=0;x<NDATA;++x)
		psi_xt[x] = psi_xt0[x];
}
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif

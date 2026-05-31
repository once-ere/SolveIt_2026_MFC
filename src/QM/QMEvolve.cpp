// QMEvolve.cpp: implementation of the CQMEvolve class.
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolveIt.h"
#include "QMEvolve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CQMEvolve::CQMEvolve(long _NDATA) :
	NDATA(_NDATA),
	RENORMALIZE(0),
	NumOrder(16),
	Jn(new double[NumOrder+1]),
	im(new cdouble[NumOrder+1]),
	imM(new cdouble[NumOrder+1]),

	m_DrawSource(Initial),
	Lambda(0.92),
	InitialCenter_x(-2),
	WaveNumber_x(64.25),
	Delta_x(0.025),

	m_EvolutionMethod(Nash)

{
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
}

CQMEvolve::~CQMEvolve()
{
		delete[]imM;
		delete[]im;
		delete[]Jn;
}
/////////////////////////////////////////////////////////////////////////////
// f(x) = (1/TwoPiToTheOneFourth)*[1/sqrt(a)]*e^(i k0 x)*e^[- ((x-x0)/2a)^2]
cdouble CQMEvolve::GaussianWavePacket(double a, double x0, double k0, double x)
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
cdouble CQMEvolve::FourierTransformGaussianWavePacket(double a, double x0, double k0, double k)
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
long	CQMEvolve::mu(long i, long N)
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
int CQMEvolve::solve_tridiag(
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
int CQMEvolve::solve_cyc_tridiag(
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

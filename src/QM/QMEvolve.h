// QMEvolve.h: interface for the CQMEvolve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMEVOLVE_H__3321045E_7313_4AB3_AF41_C0E8D6CB25CE__INCLUDED_)
#define AFX_QMEVOLVE_H__3321045E_7313_4AB3_AF41_C0E8D6CB25CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000





#include <complex>

namespace SolveIt {

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// QMScatt1D,  QMScatt2D
typedef complex<double> cdouble;
typedef vector<cdouble> ComplexData;











class CQMEvolve  
{
public:
	CQMEvolve(long _NDATA);
	virtual ~CQMEvolve();
///////////////////////////////////////////////////////////////////////////////
	enum EvolutionMethod { Nash, Cayley, PeriodicCayley, LastEvolutionMethod } m_EvolutionMethod;
	enum DrawSource { Initial, Current, External } m_DrawSource;
///////////////////////////////////////////////////////////////////////////////
	long  mu(long i, long N);
/////////////////////////////////////////////////////////////////////////////
double bessj0(double x)
{
	double ax,z;
	double xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {
		y=x*x;
		ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7
			+y*(-11214424.18+y*(77392.33017+y*(-184.9052456)))));
		ans2=57568490411.0+y*(1029532985.0+y*(9494680.718
			+y*(59272.64853+y*(267.8532712+y*1.0))));
		ans=ans1/ans2;
	} else {
		z=8.0/ax;
		y=z*z;
		xx=ax-0.785398164;
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4
			+y*(-0.2073370639e-5+y*0.2093887211e-6)));
		ans2 = -0.1562499995e-1+y*(0.1430488765e-3
			+y*(-0.6911147651e-5+y*(0.7621095161e-6
			-y*0.934935152e-7)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
	}
	return ans;
}
/////////////////////////////////////////////////////////////////////////////
double bessj1(double x)
{
	double ax,z;
	double xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {
		y=x*x;
		ans1=x*(72362614232.0+y*(-7895059235.0+y*(242396853.1
			+y*(-2972611.439+y*(15704.48260+y*(-30.16036606))))));
		ans2=144725228442.0+y*(2300535178.0+y*(18583304.74
			+y*(99447.43394+y*(376.9991397+y*1.0))));
		ans=ans1/ans2;
	} else {
		z=8.0/ax;
		y=z*z;
		xx=ax-2.356194491;
		ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4
			+y*(0.2457520174e-5+y*(-0.240337019e-6))));
		ans2=0.04687499995+y*(-0.2002690873e-3
			+y*(0.8449199096e-5+y*(-0.88228987e-6
			+y*0.105787412e-6)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
		if (x < 0.0) ans = -ans;
	}
	return ans;
}
/////////////////////////////////////////////////////////////////////////////

#define ACC 40.0
#define BIGNO 1.0e10
#define BIGNI 1.0e-10

double bessj(int n, double x)
{
	int j,jsum,m;
	double ax,bj,bjm,bjp,sum,tox,ans;

	if (n == 0) return bessj0(x);
	if (n == 1) return bessj1(x);
	ax=fabs(x);
	if (ax == 0.0)
		return 0.0;
	else if (ax > (double) n) {
		tox=2.0/ax;
		bjm=bessj0(ax);
		bj=bessj1(ax);
		for (j=1;j<n;j++) {
			bjp=j*tox*bj-bjm;
			bjm=bj;
			bj=bjp;
		}
		ans=bj;
	} else {
		tox=2.0/ax;
		m=2*((n+(int) sqrt(ACC*n))/2);
		jsum=0;
		bjp=ans=sum=0.0;
		bj=1.0;
		for (j=m;j>0;j--) {
			bjm=j*tox*bj-bjp;
			bjp=bj;
			bj=bjm;
			if (fabs(bj) > BIGNO) {
				bj *= BIGNI;
				bjp *= BIGNI;
				ans *= BIGNI;
				sum *= BIGNI;
			}
			if (jsum) sum += bj;
			jsum=!jsum;
			if (j == n) ans=bjp;
		}
		sum=2.0*sum-bj;
		ans /= sum;
	}
	return  x < 0.0 && n%2 == 1 ? -ans : ans;
}

#undef ACC
#undef BIGNO
#undef BIGNI
/////////////////////////////////////////////////////////////////////////////
// f(x) = (1/TwoPiToTheOneFourth)*[1/sqrt(a)]*e^(i k0 x)*e^[- ((x-x0)/2a)^2]
cdouble GaussianWavePacket(double a, double x0, double k0, double x);
//{
//	const double TwoPiToTheMinusOneFourth= 0.63161877774606470129;
//	double xa = (x-x0)/(2*a);
//	double b = TwoPiToTheMinusOneFourth/sqrt(a);
//	double A = Exp( -1.0*xa*xa );
//	double B = b*A;
//	cdouble c(B*cos(k0*x), B*sin(k0*x));
//	return c;
//}
/////////////////////////////////////////////////////////////////////////////
// F(k) = [2/Pi]^(1/4) sqrt(a)*e^[-((k0+k)*a)^2]*e^(i k x0)
cdouble FourierTransformGaussianWavePacket(double a, double x0, double k0, double k);
//{
//	const double TwoOverPiToTheOneFourth=0.89324384173800233140;
//	double ka = (k0+k)*a;
//	double b = Sqrt(a)*TwoOverPiToTheOneFourth;
//	double A = Exp( -1.0*ka*ka );
//	double B = b*A;
//	cdouble c(B*cos(k*x0), B*sin(k*x0));
//	return c;
//}
///////////////////////////////////////////////////////////////////////////////
/* for description of method see [Engeln-Mullges + Uhlig, p. 92]
 *
 *     diag[0]  offdiag[0]             0   .....
 *  offdiag[0]     diag[1]    offdiag[1]   .....
 *           0  offdiag[1]       diag[2]
 *           0           0    offdiag[2]   .....
 */
int solve_tridiag(
  const cdouble diag[], size_t d_stride,
  const cdouble offdiag[], size_t o_stride,
  const cdouble b[], size_t b_stride,
  cdouble x[], size_t x_stride,
  size_t N);
/////////////////////////////////////////////////////////////////////////////
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
int solve_cyc_tridiag(
  const cdouble diag[], size_t d_stride,
  const cdouble offdiag[], size_t o_stride,
  const cdouble b[], size_t b_stride,
  cdouble x[], size_t x_stride,
  size_t N);
/////////////////////////////////////////////////////////////////////////////
	virtual void INIT(void)=0;
//	virtual void SETUP(void)=0;
	virtual void OnResetViewSource(void)=0;
	virtual void DefineInitialWavefunction()=0;
	virtual void DefinePotential()=0;
	virtual void DefinePotentialDependentParameters()=0;
/////////////////////////////////////////////////////////////////////////////
	const int NumOrder;
	long NDATA;
	long RENORMALIZE;

	double	*	Jn;//[NumOrder+1];
	cdouble	*	imM;//[NumOrder+1];
	cdouble	*	im;

	double  Lambda;
	double  InitialCenter_x;
	double  WaveNumber_x;
	double  Delta_x;
};

} // namespace SolveIt

#endif // !defined(AFX_QMEVOLVE_H__3321045E_7313_4AB3_AF41_C0E8D6CB25CE__INCLUDED_)

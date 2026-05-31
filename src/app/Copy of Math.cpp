//MathNumerov.cpp

#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "Matherror.h"
//#include <limits.h>
#include <errno.h>
//extern	int	errno;
///////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
///////////////////////////////////////////////////////////////////////////////

#include "math/fit/curfit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)
/////////////////////////////////////////////////////////////////////////////////
//namespace SolveIt
//{
///////////////////////////////////////////////////////////////////////////////

double ran3(int *idum)
{
	static int inext,inextp;
	static long ma[56];
	static int iff=0;
	long mj,mk;
	int i,ii,k;

	if (*idum < 0 || iff == 0) {
		iff=1;
		mj=MSEED-(*idum < 0 ? -*idum : *idum);
		mj %= MBIG;
		ma[55]=mj;
		mk=1;
		for (i=1;i<=54;i++) {
			ii=(21*i) % 55;
			ma[ii]=mk;
			mk=mj-mk;
			if (mk < MZ) mk += MBIG;
			mj=ma[ii];
		}
		for (k=1;k<=4;k++)
			for (i=1;i<=55;i++) {
				ma[i] -= ma[1+(i+30) % 55];
				if (ma[i] < MZ) ma[i] += MBIG;
			}
		inext=0;
		inextp=31;
		*idum=1;
	}
	if (++inext == 56) inext=1;
	if (++inextp == 56) inextp=1;
	mj=ma[inext]-ma[inextp];
	if (mj < MZ) mj += MBIG;
	ma[inext]=mj;
	return mj*FAC;
}

#undef MBIG
#undef MSEED
#undef MZ
#undef FAC
///////////////////////////////////////////////////////////////////////////////
static int imsl_rand_seed = 0;

double imsl_rand() {
	return ran3(&imsl_rand_seed);
}
double imsl_seed_rand(int fseed) {
	imsl_rand_seed = (int) fseed;
	return imsl_rand();
}
//////////////////////////////////////////////////////////////////////
/* Gaussian Random Number Generator (Box-Muller Algorithm)*/ 
double gaussran(int seed) 
{
	static double ondeck;
	static int toggle;
	double x1, x2, fac, rsq;
//	srand48(seed);
//	srand48(lrand48());
	if (toggle == 0)
	{
		do
		{
			x1 = imsl_rand();//(2.0 * imsl_rand()) -1.0;
			x2 = imsl_rand();//(2.0 * imsl_rand()) -1.0;
			rsq = x1*x1 + x2*x2;
		} 
		while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0*log(rsq)/rsq);
		ondeck = x1 * fac;
		toggle = 1;
		return x2 * fac;
	}
	else
	{
		toggle = 0;
		return ondeck;
	}
} 
///////////////////////////////////////////////////////////////////////////////
double Log(const double& x)
{
	return log(x);//errcheck(log(x), "log");
}
double Log10(const double& x)
{
	return log10(x);//errcheck(log10(x), "log10");
}

double Sqrt(const double& x)
{
//	static emap emap::_emap;
	try {
		return sqrt(x);//errcheck(sqrt(x), "sqrt");
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		return 0;
	}
}

double Exp(const double& x)
{
//	static emap emap::_emap;
	try {
		return exp(x);//errcheck(exp(x), "exp");
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		return 0;
	}
}

double Power(const double& x, const double& y)
{
//	static emap emap::_emap;
	if ( y == 0.0) return 1.0;
	if ( x == 1.0) return 1.0;
	try {
	return pow(x,y);
//	return errcheck(pow(x,y), "exponentiation");
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		return 0;
	}
}

double integer(const double& x)
{
	return (double)(long)x;
}

double errcheck(const double& x,  const char *s)	/* check result of library call */
{
	static char buf[256];
	ostrstream oss(buf, sizeof buf);
	oss << s << ": " << x << ends;
	if (errno == EDOM) {
		ASSERT(0);
		errno = 0;
		MessageBox(NULL,buf, "argument out of domain",MB_OK);
		return 0;
	} else if (errno == ERANGE) {
		ASSERT(0);
		errno = 0;
		MessageBox(NULL,buf, "result out of range",MB_OK);//Timed
		return 0;
	}
	return x;
}
///////////////////////////////////////////////////////////////////////////////
struct elapsedTime
{
	elapsedTime()
	{
		time( &start );
	}
	double getElapsedTime()
	{
		time_t	finish;
		time( &finish );
		return difftime( finish, start );
	}

	time_t	start;
} g_elapsedTime;

double Time()
{
	return g_elapsedTime.getElapsedTime();
}
///////////////////////////////////////////////////////////////////////////////
#if 0
void Time()
{
    __time64_t ltime;
    _tzset();
    _time64( &ltime );
}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//CompleteEllipticIntegralFirstKind, "EllipticK","EllipticK(x), -1 <= x <= 1","EllipticK(0.2)",
//CompleteEllipticIntegralSecondKind, "EllipticEc","EllipticEc(x), -1 <= x <= 1","EllipticEc(0.2)",
///////////////////////////////////////////////////////////////////////////////
//IncompleteEllipticIntegralFirstKind, "EllipticF","EllipticF( phi , k )","EllipticF( PI/4, 0.2)",
//IncompleteEllipticIntegralSecondKind, "EllipticE","EllipticE( phi , k )","EllipticE( PI/4, 0.2)",
///////////////////////////////////////////////////////////////////////////////
double rc_(double *x, double *y, double *errtol, long *	ierr);
//INCOMPLETE ELLIPTIC INTEGRAL OF THE FIRST KIND
double rf_(double *x, double *y, double *z__, double *	errtol, long *ierr);
//INCOMPLETE ELLIPTIC INTEGRAL OF THE SECOND KIND
double rd_(double *x, double *y, double *z__, double *	errtol, long *ierr);
//INCOMPLETE ELLIPTIC INTEGRAL OF THE THIRD KIND
double rj_(double *x, double *y, double *z__, double *p, 	double *errtol, long *ierr);
extern "C" double drd_(double *x, double *y, double *z__, long *ier);
extern "C" double drf_(double *x, double *y, double *z__, long *ier);

///////////////////////////////////////////////////////////////////////////////
extern "C" double _stdcall	RF(double* x, double* y, double* z, double* ERRTOL, long* IERR);
extern "C" double _stdcall	RD(double* x, double* y, double* z, double* ERRTOL, long* IERR);
///////////////////////////////////////////////////////////////////////////////
extern "C" double _stdcall	DRF(double* x, double* y, double* z, long* IERR);
extern "C" double _stdcall	DRD(double* x, double* y, double* z, long* IERR);
///////////////////////////////////////////////////////////////////////////////
double SymmetricIncompleteEllipticIntegralFirstKind( double x, double y, double z)
{
	double	ERRTOL	= 1.0e-4;
	long	IERR;
 //	double	ELIPTC	= RF(& x, & y, & z, &ERRTOL, &IERR);
	double	ELIPTC	= drf_(& x, & y, & z, &IERR);
    if (IERR) return 0;
	return ELIPTC;
}
///////////////////////////////////////////////////////////////////////////////
double SymmetricIncompleteEllipticIntegralSecondKind( double x, double y, double z)
{
	double	ERRTOL	= 1.0e-4;
	long	IERR;
 //	double	ELIPTC	= RD(& x, & y, & z, &ERRTOL, &IERR);
	double	ELIPTC	= drd_(& x, & y, & z, &IERR);
	if (IERR) return 0;
	return ELIPTC;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                                             2         2   2
//                  F(PHI,K) = SIN(PHI) DRF(COS (PHI),1-K SIN (PHI),1)
//
//
//                                  2
//                  K(K) = DRF(0,1-K ,1)
//
//
//                         PI/2     2   2      -1/2
//                       = INT  (1-K SIN (PHI) )   D PHI
//                          0
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
double IncompleteEllipticIntegralFirstKind(double phi, double k)
{
	if (k == 0.0) return phi;
	double			s	= sin(phi);
	double			c	= cos(phi);
	double			ks	= k*s;
	double			x	= c*c;
	double			y	= 1.0 - ks*ks;
	const double	z	= 1;
//	if (b < 0.0) return 0;
	double	f	= s*SymmetricIncompleteEllipticIntegralFirstKind( x, y, z );
	return	f;
}
//////////////////////////////////////////////////////////////////////
double CompleteEllipticIntegralFirstKind(double k)
{
	if (k == 0.0) return PI_DIVIDED_BY_2;
	const double	x	= 0;
	double			y	= 1.0 - k*k;
	const double	z	= 1;
	if (y == 0.0) return PI/2;
	if (y <= 0.0) return PI/2;
	double	f	= SymmetricIncompleteEllipticIntegralFirstKind( x, y, z );
	return	f;
}
///////////////////////////////////////////////////////////////////////////////
//                                             2         2   2
//                  E(PHI,K) = SIN(PHI) DRF(COS (PHI),1-K SIN (PHI),1) -
//
//                     2      3             2         2   2
//                  -(K/3) SIN (PHI) DRD(COS (PHI),1-K SIN (PHI),1)
//
//
//                                  2        2            2
//                  E(K) = DRF(0,1-K ,1) - (K/3) DRD(0,1-K ,1)
//
//                         PI/2     2   2      1/2
//                       = INT  (1-K SIN (PHI) )  D PHI
//                          0
double IncompleteEllipticIntegralSecondKind(double phi, double k)
{
	if (k == 0.0) return phi;
	double			s	= sin(phi);
	double			ks	= k*s;
	double			s2	= s*s;
	double			s3	= s*s2;
	double			c	= cos(phi);
	double			x	= c*c;
	double			y	= 1.0 - ks*ks;
	const double	z	= 1;
	double	f	= s*SymmetricIncompleteEllipticIntegralFirstKind( x, y, z );
	double	g	= (k*k/3)*s3*SymmetricIncompleteEllipticIntegralSecondKind( x, y, z );
	return f - g;
}
///////////////////////////////////////////////////////////////////////////////
double CompleteEllipticIntegralSecondKind(double k)
{
	if (k == 0.0) return PI_DIVIDED_BY_2;
	const double	x	= 0;
	double			y	= 1.0 - k*k;
	const double	z	= 1;
	if (y == 0.0) return PI/2;
	if (y <= 0.0) return PI/2;
	double	f	= SymmetricIncompleteEllipticIntegralFirstKind( x, y, z );
	double	g	= (k*k/3)*SymmetricIncompleteEllipticIntegralSecondKind( x, y, z );
	return f - g;
}
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//}//end namespace SolveIt
/*
static vector<double> curfit__x;
static vector<double> curfit__y;
static vector<double> curfit__weight;
static vector<double> spline_knots;
static vector<double> b_spline_coefficients; 
*/

	int	curfit__spline_degree	= 3;
	int	number_knots_spline_approximation	= -1;
double curfit__x[500];
double curfit__y[500];
double curfit__weight[500];
double spline_knots[500];
double b_spline_coefficients[500]; 
double wrk[15000]; 
int iwrk[15000]; 
///////////////////////////////////////////////////////////////////////////////
double spline(double _x)
{
	int	ier					= 1;
	int	number_of_points	= 1;
	double x[1];
	double y[1];
	x[0] = _x;
	int res = splev_(spline_knots, &number_knots_spline_approximation, b_spline_coefficients, 
	&curfit__spline_degree, x, y, &number_of_points, &ier);
	return y[0];
}

void approx(const string& strDataFileName)
{
	try{
	const long maxSize = 200;
	ifstream ifs(strDataFileName.c_str());
	if (!ifs) return;
	double x=0, y=0;
/*
	curfit__x.reserve(maxSize);
	curfit__y.reserve(maxSize);
	curfit__weight.reserve(maxSize);

	curfit__x.clear();
	curfit__y.clear();
	curfit__weight.clear();
	spline_knots.clear();
	b_spline_coefficients.clear();
*/

	number_knots_spline_approximation	= -1;

	long j=0;
	while (!(ifs >> ws >> x >> y).fail())// ' >> ws' is irrelevant
	{
		curfit__x[j]=x;//.push_back(x);
		curfit__y[j]=y;//.push_back(y);
		curfit__weight[j]=1;//.push_back(1.0);
		++j;
	}
	int		m = j;//curfit__x.size();
	if (m == 0) return;
	if (m < 3) return;
	if (m > maxSize) return;

	int		ier			= 1;
	int		iopt		= 0;
	int		nest		= (m+curfit__spline_degree+1);
	int		lwrk		= (m*(curfit__spline_degree+1)+nest*(7+3*curfit__spline_degree));
	double	smoothing	= .00001;
	double	residuals	= 0;
/*
	spline_knots.reserve(nest);
	b_spline_coefficients.reserve(nest);
	vector<int> iwrk;
	iwrk.reserve(nest);
	for (j=0; j<nest; ++j)
	{
		spline_knots.push_back(0.0);
		b_spline_coefficients.push_back(0.0);
		iwrk.push_back(0);
	}
	vector<double> wrk;
	wrk.reserve(lwrk);
	for (j=0; j<lwrk; ++j)
	{
		wrk.push_back(0.0);
	}
*/

	double	xb = curfit__x[0];//.front();
	double	xe = curfit__x[j-1];//.back();


	int res = curfit_(&iopt, &m, curfit__x,curfit__y, 
curfit__weight, &xb, &xe, &curfit__spline_degree,
&smoothing, &nest, &number_knots_spline_approximation, 
spline_knots, b_spline_coefficients,&residuals, 
wrk, &lwrk, iwrk, &ier);
/*
	int res = curfit_(&iopt, &m, &curfit__x.front(), &curfit__y.front(), 
&curfit__weight.front(), &xb, &xe, &curfit__spline_degree,
&smoothing, &nest, &number_knots_spline_approximation, 
&spline_knots.front(), &b_spline_coefficients.front(),&residuals, 
&wrk.front(), &lwrk, &iwrk.front(), &ier);
*/

	string stat(strDataFileName);
	switch(ier)
	{
	case -2:stat += ": normal return";
		break;
	case -1:stat += ": normal return";
		break;
	case 0:stat += ": normal return";
		break;
	case 1:stat += ": error. the required storage space exceeded";
		break;
	case 2:stat += ": error. a theoretically impossible result";
		break;
	case 3:stat += ": error. the maximal number of iterations";
		break;
	case 10:stat += ": error. on entry";
		break;
	}
	if (ier <= 0)
	{
	}
	else
		TimedMessageBox(NULL,stat.c_str(),"approx",MB_ICONWARNING);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		ASSERT(0);
	}
}
///////////////////////////////////////////////////////////////////////////////

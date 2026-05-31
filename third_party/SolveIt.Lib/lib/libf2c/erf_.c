#include <math.h>
#include "f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef REAL
#define REAL double
#endif

#ifdef KR_headers
double erf();
REAL erf_(x) real *x;
#else
extern double erf(double);
REAL erf_(real *x)
#endif
{
return( erf((double)*x) );
}




double gammln(double xx)
{
	double x,tmp,ser;
	static double cof[6]={76.18009173,-86.50532033,24.01409822,
		-1.231739516,0.120858003e-2,-0.536382e-5};
	int j;

	x=xx-1.0;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.0;
	for (j=0;j<=5;j++) {
		x += 1.0;
		ser += cof[j]/x;
	}
	return -tmp+log(2.50662827465*ser);
}


#define ITMAX 100
#define EPS 3.0e-7

void gcf(double* gammcf,double a,double x,double* gln)
{
	int n;
	double gold=0.0,g,fac=1.0,b1=1.0;
	double b0=0.0,anf,ana,an,a1,a0=1.0;

	*gln=gammln(a);
	a1=x;
	for (n=1;n<=ITMAX;n++) {
		an=(double) n;
		ana=an-a;
		a0=(a1+a0*ana)*fac;
		b0=(b1+b0*ana)*fac;
		anf=an*fac;
		a1=x*a0+anf*a1;
		b1=x*b0+anf*b1;
		if (a1) {
			fac=1.0/a1;
			g=b1*fac;
			if (fabs((g-gold)/g) < EPS) {
				*gammcf=exp(-x+a*log(x)-(*gln))*g;
				return;
			}
			gold=g;
		}
	}
}

#undef ITMAX
#undef EPS



#define ITMAX 100
#define EPS 3.0e-7

void gser(double* gamser,double a,double x,double* gln)
{
	int n;
	double sum,del,ap;

	*gln=gammln(a);
	if (x <= 0.0) {
		if (x < 0.0) return;
		*gamser=0.0;
		return;
	} else {
		ap=a;
		del=sum=1.0/a;
		for (n=1;n<=ITMAX;n++) {
			ap += 1.0;
			del *= x/ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPS) {
				*gamser=sum*exp(-x+a*log(x)-(*gln));
				return;
			}
		}
		return;
	}
}

#undef ITMAX
#undef EPS


double gammp(double a,double x)
{
	double gamser,gammcf,gln;

	if (x < 0.0 || a <= 0.0) return 0;
	if (x < (a+1.0)) {
		gser(&gamser,a,x,&gln);
		return gamser;
	} else {
		gcf(&gammcf,a,x,&gln);
		return 1.0-gammcf;
	}
}



double erf(double x)
{
	return x < 0.0 ? -gammp(0.5,x*x) : gammp(0.5,x*x);
}



double gammq(double a,double x)
{
	double gamser,gammcf,gln;

	if (x < 0.0 || a <= 0.0) return 0;
	if (x < (a+1.0)) {
		gser(&gamser,a,x,&gln);
		return 1.0-gamser;
	} else {
		gcf(&gammcf,a,x,&gln);
		return gammcf;
	}
}

double erfc(double x)
{
	double gammp(),gammq();

	return x < 0.0 ? 1.0+gammp(0.5,x*x) : gammq(0.5,x*x);
}

#ifdef __cplusplus
}
#endif

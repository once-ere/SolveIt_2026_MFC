// CTrajectoryRecord.cpp: implementation of the CTrajectoryRecord class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TrajectoryRecord.h"
#include "parser.h"
//////////////////////////////////////////////////////////////////////
// CTrajectoryRecord
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if 0
struct InitOle {
	  InitOle()  { ::CoInitialize(NULL); }
	  ~InitOle() { ::CoUninitialize();   }
	} _init_InitOle_;

//		IOpenGLObjPtr m_glObj;
//		HRESULT hr = m_glObj.CreateInstance( __uuidof( OpenGLObj ) );
//		IOpenGLObjPtr m_glObj(__uuidof(OpenGLObj));
#endif


//DFORMT.LIB 

void spline( double x[] , double y[], long n, double yp1, double ypn, double y2[])
{
	int i,k;
	double p,qn,sig,un,*u=new double[n];

	if (yp1 > 0.99e30)
		y2[0]=u[0]=0.0;
	else {
		y2[0] = -0.5;
		u[0]=(3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
	}
	for (i=1;i<n-1;i++) {
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	if (ypn > 0.99e30)
		qn=un=0.0;
	else {
		qn=0.5;
		un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
	}
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
	for (k=n-2;k>=0;k--)
		y2[k]=y2[k]*y2[k+1]+u[k];
	delete[]u;
}

double CTrajectoryRecord::splint(double t)
{
	long& n=splintN;
	long klo,khi,k;
	double h,b,a;

	klo=0;
	khi=n-1;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (ta[k] > t) khi=k;
		else klo=k;
	}
	h=ta[khi]-ta[klo];
	if (h == 0.0) return 0.99e40;//nrerror("Bad XA input to routine SPLINT");
	a=(ta[khi]-t)/h;
	b=(t-ta[klo])/h;
//	return a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
	return a*ya[klo]+b*ya[khi]+(a*(a+1)*(a-1)*y2a[klo]+b*(b+1)*(b-1)*y2a[khi])*(h*h)/6.0;
}




#define ITMAX 100
#define EPS 3.0e-8

double CTrajectoryRecord::zbrent(  func f, double x1, double x2, double tol)
{
	int iter;
	double a=x1,b=x2,c,d,e,min1,min2;
	double fa=(this->*f)(a),fb=(this->*f)(b),fc,p,q,r,s,tol1,xm;

	if (fb*fa > 0.0) return 0.99e40;// nrerror("Root must be bracketed in ZBRENT");
	fc=fb;
	for (iter=1;iter<=ITMAX;iter++) {
		if (fb*fc > 0.0) {
			c=a;
			fc=fa;
			e=d=b-a;
		}
		if (fabs(fc) < fabs(fb)) {
			a=b;
			b=c;
			c=a;
			fa=fb;
			fb=fc;
			fc=fa;
		}
		tol1=2.0*EPS*fabs(b)+0.5*tol;
		xm=0.5*(c-b);
		if (fabs(xm) <= tol1 || fb == 0.0) return b;
		if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
			s=fb/fa;
			if (a == c) {
				p=2.0*xm*s;
				q=1.0-s;
			} else {
				q=fa/fc;
				r=fb/fc;
				p=s*(2.0*xm*q*(q-r)-(b-a)*(r-1.0));
				q=(q-1.0)*(r-1.0)*(s-1.0);
			}
			if (p > 0.0)  q = -q;
			p=fabs(p);
			min1=3.0*xm*q-fabs(tol1*q);
			min2=fabs(e*q);
			if (2.0*p < (min1 < min2 ? min1 : min2)) {
				e=d;
				d=p/q;
			} else {
				d=xm;
				e=d;
			}
		} else {
			d=xm;
			e=d;
		}
		a=b;
		fa=fb;
		if (fabs(d) > tol1)
			b += d;
		else
			b += (xm > 0.0 ? fabs(tol1) : -fabs(tol1));
		fb=(this->*f)(b);
	}
	 return 0.99e40;//nrerror("Maximum number of iterations exceeded in ZBRENT");
}

#undef ITMAX
#undef EPS

/////////////////////////////////////////////////////////////////////////////
// CTrajectoryRecord
long CTrajectoryRecord::AnalyzeData(AnalyzeTrajectory* specify)
{
	if (m_data.empty()) return -1;
	std::wstringstream oss;
try {
	switch (specify->option)
	{
		case T_PERIOD:
		{
			TrajectoryPeriod* tp = (TrajectoryPeriod*) specify;
			AnalyzePeriod(tp);
		}
			break;
		case T_TRAJECTORY:
		{
			TrajectoryPeriod* tp = (TrajectoryPeriod*) specify;
			AnalyzePeriod(tp);
		}
			break;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	TimedMessageBox(NULL,L"AnalyzeData",L"Exception",MB_OK);
	throw E_FAIL;
}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////

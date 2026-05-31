//Tension.cpp
#include "stdafx.h"


#pragma optimize( "", off )

#define Power(a,b) pow((a),(b))
#define Sqrt(a) sqrt((a))


///////////////////////////////////////////////////////////////////////////////

//Connection on right:
double TensionRight1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=Sqrt(x*x + y*y);
double tension = 
	#include ".\tensions\TensionRight1.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionRight2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=Sqrt(x*x + y*y);
double tension = 
	#include ".\tensions\TensionRight2.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
double TensionLeft1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=Sqrt(x*x + y*y);
double tension = 
	#include ".\tensions\TensionLeft1.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionLeft2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=Sqrt(x*x + y*y);
double tension = 
	#include ".\tensions\TensionLeft2.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define STUFF \
const double smallNumber = 0.01e-6;\
if (r1<smallNumber) return 0;\
if (r2<smallNumber) return 0;
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft11(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsLeft11.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft12(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsLeft12.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft21(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsLeft21.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft22(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsLeft22.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
double TensionsRight11(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsRight11.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight12(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsRight12.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight21(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsRight21.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight22(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
	#include ".\tensions\TensionsRight22.c"
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if 0
#define INCL(x) x
#define TensionsRight(a,b,A,B) \
double TensionsRight##a##b(double R, double g, double InertiaInv, double w,\
					 double m1, double x1, double y1, double v1x, double v1y,\
					 double m2, double x2, double y2, double v2x, double v2y)\
{\
	double r1=Sqrt(x1*x1 + y1*y1);\
	double r2=Sqrt(x2*x2 + y2*y2);\
double tension = \
	INCL(#include) "TensionsRight" A B ".c"\
	;\
	return tension;\
}

///////////////////////////////////////////////////////////////////////////////
TensionsRight(1,1,"1","1")
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	INCL(include)\
	INCL(x) "TensionsRight"##A##B##".c"\

//Connection on right:
double TensionsRight1(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
double tension = 
;
	return tension;
}
/*//////////////////////////////////////////////////////////////////////////////
	_bstr_t bstr(L"TensionsRight");\
	long A = a, B = b;\
	_variant_ va(A), vb(B);\
	_bstr_t bstra(va), bstrb(vb);\
	bstr+=bstra;bstr+=bstrb;bstr+=L".c";
//////////////////////////////////////////////////////////////////////////////*/
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
double TensionsLeft1(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=Sqrt(x1*x1 + y1*y1);
	double r2=Sqrt(x2*x2 + y2*y2);
double tension = 
	#include ".\tensions\TensionsLeft1.c"
;
	return tension;
}
/*//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/
///////////////////////////////////////////////////////////////////////////////
typedef double (*_Tension)(double X, double Y, double vx, double vy, double R, double g);
///////////////////////////////////////////////////////////////////////////////
struct SelectAngle {
	double s;
	double L;
	double theta;
	_Tension f;
	SelectAngle(double _s, double _L, double _theta, _Tension _f) : s(_s), L(_L), theta(_theta), f(_f) {}
};
///////////////////////////////////////////////////////////////////////////////
struct SelectAngleCompare
{  
	bool operator()(const SelectAngle*& a, const SelectAngle*& b) const
	{
		return a->s < b->s;
	}
};
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
// -PI <= theta <= PI
//Connection on right:
// 0 <= theta <= 2 PI

#endif

#undef STUFF


#pragma optimize( "", on )

// Matherror.h: interface for the CMatherr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHERR_H__ED6093A9_A2DC_4F17_B288_2001F3E0E0AD__INCLUDED_)
#define AFX_MATHERR_H__ED6093A9_A2DC_4F17_B288_2001F3E0E0AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

//#include <complex>
using namespace std;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// f(x) = (1/TwoPiToTheOneFourth)*[1/sqrt(a)]*e^[- 0.5* (x/a)^2]
static double gaussianNormalized(double a, double x)
{
	const double log10			= 2.3025850929940456840;
	const double DBL_MAX_E_EXP	= DBL_MAX_10_EXP*log10;
	const double DBL_MIN_E_EXP	= DBL_MIN_10_EXP*log10;
	const double TwoPiToTheMinusOneFourth= 0.63161877774606470129;
	double xa = (x)/(a);
	double b = TwoPiToTheMinusOneFourth/sqrt(fabs(a));
	double X = ( -0.5*xa*xa );
	if (X < DBL_MIN_E_EXP/2) return 0;
	double A = exp( X );
	double B = b*A;
	return B;
}
///////////////////////////////////////////////////////////////////////////////
static double unitStep(double x) {return x > 0.0? 1:(x == 0.0? 0.5:0);}
static double sign(double a) {return (a>0.0? 1:(a<0.0?-1:0));}
///////////////////////////////////////////////////////////////////////////////
static double fmax(double a,double b) {return a>=b? a:b;}
static double fmin(double a,double b) {return a<=b? a:b;}
static double __jn(long n, double x) {return _jn( int(n), x);}
static double __yn(long n, double x) {return _yn( int(n), x);}
static double _ldexp(double x, long n){return ldexp(x, int(n));}

double Time(void);
double imsl_rand(void);
double imsl_seed_rand(int seed);

double CompleteEllipticIntegralFirstKind( double k);
double IncompleteEllipticIntegralFirstKind(double phi, double k);
double IncompleteEllipticIntegralSecondKind(double phi, double k);
double CompleteEllipticIntegralSecondKind(double k);


double spline(double x);
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//LIB dformt.LIB /REMOVE:matherr.obj  /OUT:dformt_no_matherr.LIB
//LIB dformt.LIB /EXTRACT:matherr.obj /OUT:matherr.obj
//ren __matherr in matherr.obj something else 
//link with dformt_no_matherr.LIB, matherr.obj, ...
/*
#include "uint.h"

const wchar_t msg1[] = L"NORMALIZERADIAL: ";
const wchar_t msg2[] = L"array.norm: ";
struct _exception
{
	int    type;   // exception type - see below
	char*  name;   // name of function where error occurred
	double arg1;   // first argument to function
	double arg2;   // second argument (if any) to function
	double retval; // value to be returned by function
};
When an error occurs in a math routine, _matherr is called with a pointer to an _exception type structure (defined in <math.h>) 
as an argument.

struct _exception {
		int type;       //* exception type - see below *
		char *name;     //* name of function where error occured *
		double arg1;    //* first argument to function *
		double arg2;    //* second argument (if any) to function *
		double retval;  //* value to be returned by function *
		} ;

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class emap : public map<long, wstring>
{
public:
	inline emap()
	{
		emplace(EM_INVALID,        L"EM_INVALID");
		emplace(EM_DENORMAL,       L"EM_DENORMAL");
		emplace(EM_ZERODIVIDE,     L"EM_ZERODIVIDE");
		emplace(EM_OVERFLOW,       L"EM_OVERFLOW");
		emplace(EM_UNDERFLOW,      L"EM_UNDERFLOW");
		emplace(EM_INEXACT,        L"EM_INEXACT");
		try_emplace(SW_INVALID,        L"SW_INVALID");
		try_emplace(SW_DENORMAL,       L"SW_DENORMAL");
		try_emplace(SW_ZERODIVIDE,     L"SW_ZERODIVIDE");
		try_emplace(SW_OVERFLOW,       L"SW_OVERFLOW");
		try_emplace(SW_UNDERFLOW,      L"SW_UNDERFLOW");
		try_emplace(SW_INEXACT,        L"SW_INEXACT");
		try_emplace(SW_UNEMULATED,     L"SW_UNEMULATED");
		try_emplace(SW_SQRTNEG,        L"SW_SQRTNEG");
		try_emplace(SW_STACKOVERFLOW,  L"SW_STACKOVERFLOW");
		try_emplace(SW_STACKUNDERFLOW, L"SW_STACKUNDERFLOW");
		try_emplace(FPE_INVALID,       L"FPE_INVALID");
		try_emplace(FPE_DENORMAL,      L"FPE_DENORMAL");
		try_emplace(FPE_ZERODIVIDE,    L"FPE_ZERODIVIDE");
		try_emplace(FPE_OVERFLOW,      L"FPE_OVERFLOW");
		try_emplace(FPE_UNDERFLOW,     L"FPE_UNDERFLOW");
		try_emplace(FPE_INEXACT,       L"FPE_INEXACT");
		try_emplace(FPE_UNEMULATED,    L"FPE_UNEMULATED");
		try_emplace(FPE_SQRTNEG,       L"FPE_SQRTNEG");
		try_emplace(FPE_STACKOVERFLOW, L"FPE_STACKOVERFLOW");
		try_emplace(FPE_STACKUNDERFLOW,L"FPE_STACKUNDERFLOW");
		try_emplace(FPE_EXPLICITGEN,   L"FPE_EXPLICITGEN");
	}
	std::wstring operator()(const long& key) const
	{
		auto i = find(key);
		if (i != end()) return i->second;
		return {};
	}

	static std::wstring OnError(struct _exception *except);//
	static emap _emap;

};
//extern emap _emap;


class CMatherror  
{
public:
	CMatherror();
	virtual ~CMatherror();

};

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_MATHERR_H__ED6093A9_A2DC_4F17_B288_2001F3E0E0AD__INCLUDED_)

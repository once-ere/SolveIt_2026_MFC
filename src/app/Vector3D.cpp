// Vector3D.cpp: implementation of the Vector3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "Vector3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
//const Vector3D	K_3D = I_3D.vectorProd(J_3D);		// unit vector along +z axis
////////////////////////////////////////////////////////////////////////
const Vector3D	ZERO_3D(0,0,0);
const Vector3D	I_3D(1,0,0);		// unit vector along +x axis
const Vector3D	J_3D(0,1,0);		// unit vector along +y axis
const Vector3D	K_3D(0,0,1);		// unit vector along +z axis

}//end namespace SolveIt
/*
vector<Vector3D>	numericalVector3D;
////////////////////////:://////////////////////////////////////////////
// Vector3D
//////////////////////////////////////////////////////////////////////
//using namespace SolveIt;
static int gg=sizeof(K_3D.v);

static Vector3D aa4a=	I_3D - J_3D;		// unit vector along +z axis

static Vector3D aa5a=	3* I_3D.vectorProd(J_3D);		// unit vector along +z axis

static Vector3D a6aa=	K_3D;		// unit vector along +z axis

static Vector3D aaa=	K_3D + 3* I_3D.vectorProd(J_3D);		// unit vector along +z axis
static Vector3D abaa=	aaa;		// unit vector along +z axis
///////////////////////////////////////////////////////////////////////////////
template<int N>
VectorND<N>::VectorND(const double x,...):v(new double[N])
{
	v[0]=x;
	int count = 1;
	double y=0;
	va_list marker;
	va_start( marker, x );	// Initialize variable arguments.
	for ( ; count < N; ++count)
	{
		y = va_arg( marker, double);
		v[count]=y;
	}
	va_end( marker );
}
///////////////////////////////////////////////////////////////////////////////
VectorND<4> v4;
//VectorND<4> v4a(1,20,3,40);
VectorND<4> v4a(1.0,2.0,3.0,4.0);
VectorND<4> v4b=v4a;
double x0=v4b.v[0];
double x1=v4b.v[1];
double x2=v4b.v[2];
double x3=v4b.v[3];
double x4=x3;
*/

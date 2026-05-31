// Vector3D.h: interface for the Vector3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3D_H__385C2DAD_EEFA_40DE_993B_7E0D14F208DA__INCLUDED_)
#define AFX_VECTOR3D_H__385C2DAD_EEFA_40DE_993B_7E0D14F208DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <sstream>
//#include "NVector.h"

//using namespace SolveIt;
///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{

//////////////////////////////////////////////////////////////////////
//const double PI = 3.1415926535897932384626434;
//const double PI			= 3.1415926535897932384626433832795;
const double PI			= 3.1415926535897932384626433832795028841971693993751;
const double TwoPI		= 6.2831853071795864769252867665590057683943387987502;


const double PI_DIVIDED_BY_2 = 1.57079632679489661923;

const double PIOver8	= 0.39269908169872415480783;
const double mPIOver8	= -0.39269908169872415480783;
const double EXP		= 2.7182818284590452353602874713527;
const double GAMMA		= 0.57721566490153286060;//Euler const", "GAMMA*2",
const double RAD_TO_DEG	= 57.295779513082320876798154814105;//== 180.0 / PI: converts radians to degrees", "TO_DEG * atan2(2,1)",
const double DEG_TO_RAD	= 0.017453292519943295769236907684886;//== PI / 180.0;converts degrees to radians", "sin(TO_RAD*45)",
const double PHI		= 1.61803398874989484820;//golden ratio", "PHI*5",
const double Sqrt2		= 1.4142135623730950488;//sqrt(2)", "Sqrt2*5",

///////////////////////////////////////////////////////////////////////////////

class Vector3D //: public Vector3D<3>
{
public:
#define dim 3
//	double v[dim];
	union {
		struct {double x, y, z;};
		double v[3];
	};

	virtual ~Vector3D(){}

	Vector3D() { v[0]=v[1]=v[2]=0; }

	Vector3D(const double& x, const double& y, const double& z) { v[0]=x,v[1]=y,v[2]=z; }

	void set(const double& a){v[0]=v[1]=v[2]=a;}

	Vector3D(const double& a) {set(a);}

	Vector3D(const Vector3D& _v){ CopyMemory((void*)v, (void*)_v.v, sizeof(v));}
	Vector3D(const double _v[])	{ CopyMemory((void*)v, (void*)_v, sizeof(v));}

	Vector3D& operator =(const Vector3D& _v) {
		CopyMemory((void*)v, (void*)_v.v, sizeof(v));
		return *this;
	}

	Vector3D& operator =(const double _v[]) {
		CopyMemory((void*)v, (void*)_v, sizeof(v));
		return *this;
	}

	Vector3D& operator =(const Vector3D* _v) {
		if (_v==NULL) throw SolveIt_ERROR("null vector");
		const Vector3D&a= *_v;
		CopyMemory((void*)v, (void*)a.v, sizeof(v));
		return *this;
	}
	Vector3D(const Vector3D* _v){ operator =(_v);}

///////////////////////////////////////////////////////////////////////////////
	double& operator[](const int i) {return v[i];};
	const double& operator[](const int i) const {return v[i];};

	void negate() {v[0]*=-1,v[1]*=-1,v[2]*=-1;}
	Vector3D operator-() const {return -1.0*(*this);}
///////////////////////////////////////////////////////////////////////////////
	Vector3D& operator+=(const Vector3D& a) {
		for (int j=0;j<dim;++j) v[j] += a.v[j];
		return *this;
	}
	Vector3D& operator-=(const Vector3D& a) {
		for (int j=0;j<dim;++j) v[j] -= a.v[j];
		return *this;
	}
///////////////////////////////////////////////////////////////////////////////
	Vector3D  operator + (const Vector3D &b) const {
		Vector3D s(*this);
		return s.operator+=(b);
	}

	Vector3D  operator - (const Vector3D &b) const {
		Vector3D s(*this);
		return s.operator-=(b);
	}

	Vector3D  operator % (const Vector3D &b) const {
		Vector3D s(fmod(v[0], b.v[0]), fmod(v[1], b.v[1]), fmod(v[2], b.v[2]));
		return s;
	}
///////////////////////////////////////////////////////////////////////////////
	Vector3D& operator*=(const double& a)	{
		for (int j=0;j<dim;++j) v[j] *= a;
		return *this;
	}
	Vector3D& operator+=(const double& a)	{
		for (int j=0;j<dim;++j) v[j] += a;
		return *this;
	}
	Vector3D& operator-=(const double& a)	{
		for (int j=0;j<dim;++j) v[j] -= a;
		return *this;
	}
	Vector3D& operator/=(const double& a)	{
		for (int j=0;j<dim;++j) v[j] /= a;
		return *this;
	}
///////////////////////////////////////////////////////////////////////////////
	friend Vector3D   operator + (const Vector3D& a, const double&   b){
		Vector3D s(a);
		return s.operator+=(b);
	}
	friend Vector3D   operator + (const double&   b, const Vector3D& a){
		Vector3D s(a);
		return s.operator+=(b);
	}
	friend Vector3D   operator - (const Vector3D& a, const double&   b){
		Vector3D s(a);
		return s.operator-=(b);
	}
	friend Vector3D   operator - (const double&   b, const Vector3D& a){
		Vector3D s(a);
		return s.operator-=(b);
	}
	friend Vector3D   operator * (const Vector3D& a, const double&   b){
		Vector3D s(a);
		return s.operator*=(b);
	}
	friend Vector3D   operator * (const double&   b, const Vector3D& a){
		Vector3D s(a);
		return s.operator*=(b);
	}
	friend Vector3D   operator / (const Vector3D& a, const double&   b){
		Vector3D s(a);
		return s.operator/=(b);
	}
///////////////////////////////////////////////////////////////////////////////
	double operator * (const Vector3D &b) const {return dot(b);}
	double dot (const Vector3D& a) const {
		double s=0;
		for (int j=0;j<dim;++j) s += v[j]*a.v[j];
		return s;
	}

	Vector3D times (const Vector3D& a) const {
		Vector3D s(x*a.x,y*a.y,z*a.z);
		return s;
	}

	double norm2() const {return this->dot(*this);}
	double norm () const {return sqrt(norm2());}

	void normalize()
	{
		double n = norm();
		try
		{
			if (n>0.0) n=1.0/n;
//		else  AfxThrowNotSupportedException();
			operator*=(n);
		}
		catch( SolveIt_Error& e){e.why();}catch(...) {}
	}
///////////////////////////////////////////////////////////////////////////////
	void OnDraw(void) const
	{
		glVertex3dv(v);
	}
///////////////////////////////////////////////////////////////////////////////
	int operator==(const Vector3D  &a) const {
		for (int j=0;j<dim;++j) if(v[j] != a.v[j]) return 0;
		return 1;
	}

	int operator!=(const Vector3D  &b) const {
		return !(operator ==(b));
	}

	int operator! () const {
		const Vector3D zero(0,0,0);
		return (operator ==(zero));
	}
///////////////////////////////////////////////////////////////////////////////
//	std::ostream& print(std::ostream &os) const
	void print(std::ostream &os) const
	{
		int oldFlags = os.setf(ios::showpos);
		os << '(';
		const double *begin = &(v[0]);
		const double *end = begin+ dim;
		for (const double* p=begin; p<end; p++) os << *p << ", ";
		os << ')' ;
		os.flags(oldFlags);
//		return os;
	}
///////////////////////////////////////////////////////////////////////////////
	friend ostream& operator<<(ostream& os, const Vector3D&a)
	{
	//	const double *begin = &(a.v[0]);
	//	const double *end = begin+ dim;
		os << '[' << a.v[0] << ", " << a.v[1] << ", " << a.v[2] << " ]";
	//	for (const double* p=begin; p<end; p++) os << *p << ", ";
	//	os << ']' ;
		return os;
	}

	friend wostream& operator<<(wostream& os, const Vector3D&a)
	{
		os << L'[' << a.v[0] << L", " << a.v[1] << L", " << a.v[2] << L" ]";
		return os;
	}

// r^ = R z^
// �^ = R x^ = theta hat
// �^ = R y^ = phi hat
// R = r^ @ z^ + �^ @ x^ + �^ @ y^, @ = tensor product


#define X 0
#define Y 1
#define Z 2

	Vector3D vectorProd (const Vector3D &a) const
	{
#define ALT(i,j) operator[](i)*a[j]-operator[](j)*a[i]
		return Vector3D(ALT(Y,Z),ALT(Z,X),ALT(X,Y));
#undef ALT
	}

#undef X
#undef Y
#undef Z

// bug fountain:
// ^ overloaded operator  precedence is less than +
//	Vector3D  operator ^ (const Vector3D &b) const{return vectorProd (b);}

#undef dim

};
///////////////////////////////////////////////////////////////////////////////
inline	istream& operator>>(istream& _I, Vector3D& a)
	{
		string s;
		getline(_I,s);
		{char* p=0;
			char *begin = (char *)s.c_str();
			char *end = begin + s.size();
			for ( p=begin;p<end; p++) if (*p==',') *p=' ';
			for ( p=begin;p<end; p++) if (*p=='[') *p=' ';
			for ( p=begin;p<end; p++) if (*p==']') *p=' ';
		}

		istringstream iss(s);
		Vector3D b(0.0);
//		_I.eatwhite();
//		_I >> ws;
		double *begin = &(b.v[0]);
		double *end = begin + 3;
		for (double* p=begin; (!iss.fail()) && p<end; p++) iss >> *p;
		if (!iss.fail())
		{
			a=b;
		}
		return (_I);
	}

inline	wistream& operator>>(wistream& _I, Vector3D& a)
	{
		wstring s;
		getline(_I, s);
		for (wstring::iterator it = s.begin(); it != s.end(); ++it)
		{
			if (*it == L',' || *it == L'[' || *it == L']') *it = L' ';
		}

		wistringstream iss(s.c_str());
		Vector3D b(0.0);
		double *begin = &(b.v[0]);
		double *end = begin + 3;
		for (double* p=begin; (!iss.fail()) && p<end; p++) iss >> *p;
		if (!iss.fail())
		{
			a = b;
		}
		return (_I);
	}
///////////////////////////////////////////////////////////////////////////////
// Vector3D  constants 

extern const Vector3D	ZERO_3D;
extern const Vector3D	I_3D;		// unit vector along +x axis
extern const Vector3D	J_3D;		// unit vector along +y axis
extern const Vector3D	K_3D;		// unit vector along +z axis
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//typedef Vector3D Point3D;
#if 10
class Point3D : public Vector3D 
{
public:
	Point3D(): Vector3D() {}

	Point3D(double _x, double _y, double _z) : Vector3D(_x, _y, _z) {}

	Point3D(const Point3D& _v)	: Vector3D(_v) {}
	Point3D(const Vector3D& _v)	: Vector3D(_v) {}
	Point3D(const double _v[3])	: Vector3D(_v) {}
	Point3D(double _v[3])		: Vector3D(_v) {}

	Point3D& operator=(const Point3D& p) {
		Vector3D::operator=(p);
		return *this;
	}

#if 0
	Point3D& operator=(const Vector3D& p) {
		Vector3D::operator=(p);
		return *this;
	}

	Point3D& operator=(const double p[3]) {
		Vector3D::operator=(p);
		return *this;
	}
	Point3D& operator =(const Point3D* _v) {
		if (_v==NULL) throw SolveIt_ERROR("null vector");
		const Vector3D&a= *_v;
		CopyMemory((void*)v, (void*)a.v, sizeof(v));
		return *this;
	}
	Point3D& operator =(const Vector3D* _v) {
		if (_v==NULL) throw SolveIt_ERROR("null vector");
		const Vector3D&a= *_v;
		v[0]=a.v[0];
		v[1]=a.v[1];
		v[2]=a.v[2];
//		CopyMemory((void*)v, (void*)a.v, sizeof(v));
		return *this;
	}
#endif
};
#endif

///////////////////////////////////////////////////////////////////////////////
class Vector2D
{
public:
//	double x,y;
//	struct {double x, y;};
	union {
		struct {double x, y;};
		double v[2];
	};
	Vector2D() : x(0), y(0){}
	Vector2D(double _x,double _y) : x(_x), y(_y){}
	Vector2D(const POINT& pt) : x(pt.x), y(pt.y){}
	Vector2D(const Vector2D &v) : x(v.x), y(v.y){}

	Vector2D(const double v[]) : x(v[0]), y(v[1]){}
	Vector2D(const Vector3D& a) : x(a.x), y(a.y){}

	Vector2D& operator =(const double v[]) {
		x=v[0], y=v[1];
		return *this;
	}

	Vector2D& operator=(const Vector2D &v) {
		x=v.x, y=v.y;
		return *this;
	}

///////////////////////////////////////////////////////////////////////////////
	double& operator[](const int i) {return(i?y:x);};
	const double& operator[](const int i) const {return(i?y:x);};

	Vector2D& operator+=(const Vector2D &v) {
		x+=v.x, y+=v.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D &v) {
		x-=v.x, y-=v.y;
		return *this;
	}
	Vector2D operator-(const Vector2D &v) const {
		Vector2D s(x-v.x, y-v.y);
		return s;
	}
	Vector2D operator+(const Vector2D &v) const {
		Vector2D s(x+v.x, y+v.y);
		return s;
	}
	Vector2D& operator/=(double f) {
		x /= f, y /= f;
		return *this;
	}
	Vector2D& operator*=(double f) {
		x *= f, y *= f;
		return *this;
	}
/*
	Vector2D operator*(double f) {
		Vector2D s(*this);
		s.x *= f, s.y *= f;
		return s;
	}
*/
	int operator==(const Vector2D  &a) const {
		for (int j=0;j<2;++j) if(v[j] != a.v[j]) return 0;
		return 1;
	}

	int operator!=(const Vector2D  &b) const {
		return !(operator ==(b));
	}

	int operator! () const {
		const Vector2D zero(0,0);
		return (operator ==(zero));
	}
///////////////////////////////////////////////////////////////////////////////
	friend Vector2D   operator * (const Vector2D& a, const double&   b){return Vector2D (a.x*b,a.y*b);}
	friend Vector2D   operator * (const double&   b, const Vector2D& a){return Vector2D (a.x*b,a.y*b);}
	double Norm() {return sqrt(x*x+y*y);}
	double operator*(const Vector2D &v) const{
		return x*v.x+y*v.y;
	}
///////////////////////////////////////////////////////////////////////////////
	void OnDraw(void) const
	{
		glVertex2dv(&x);
	}
};

/*
Note   It is meaningless to apply explicit to constructors with 
multiple arguments, since such constructors cannot take part in 
implicit conversions
*/
template<int N>
class VectorND
{
public:
	double* v;
	virtual ~VectorND(){if(v) delete[]v;}

	VectorND(void):v(new double[N]) { for(int j=0;j<N;j++)v[j]=0; }

//WARNING: data corruption if other than double agrs used!
//	explicit VectorND(const double x,...):v(new double[N])
	VectorND( double x,...):v(new double[N])
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

	VectorND(const VectorND& _v):v(new double[N]) { CopyMemory((void*)v, (void*)_v.v,	N*sizeof(double));}
	VectorND(const double _v[])	:v(new double[N]) { CopyMemory((void*)v, (void*)_v,		N*sizeof(double));}

	VectorND& operator =(const VectorND& _v) {
		CopyMemory((void*)v, (void*)_v.v, N*sizeof(double));
		return *this;
	}
	VectorND& operator =(const double _v[]) {
		CopyMemory((void*)v, (void*)_v, N*sizeof(double));
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

}//end namespace SolveIt

#endif // !defined(AFX_VECTOR3D_H__385C2DAD_EEFA_40DE_993B_7E0D14F208DA__INCLUDED_)

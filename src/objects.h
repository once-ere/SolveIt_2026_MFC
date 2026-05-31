// objects.h: interface for the objects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTS_H__DFCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_)
#define AFX_OBJECTS_H__DFCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <complex>
#include "RigidBody/RigidBody.h"
////#include "parser_2020.h" /* for token definitions and yylval */

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class symbol;
class Vector3D;
class Point3D;
class objects;
class instruction;
class CRigidBody;
///////////////////////////////////////////////////////////////////////////////
typedef double	(*F_0)	(void); 
typedef double	(*F_D)	(double); 
typedef double	(*F_DD)	(double,double);
typedef double	(*F_DDD)(double,double,double);
typedef double	(*F_L)	(int);
typedef double	(*F_LD)	(int,double);
typedef double	(*F_DL)	(double, long);
typedef double	(*F_LDD)	(int,double,double);
typedef double	(*F_DLD)	(double,int,double);
typedef double	(*F_DDL)	(double,double,int);
typedef double	(*F_LDDD)	(int,double,double,double);
typedef double	(*F_DLDD)	(double,int,double,double);
typedef double	(*F_DDLD)	(double,double,int,double);
typedef double	(*F_DDDL)	(double,double,double,int);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	enum dataTypes/*:std::uint64_t*/ {
		__undefined=0,		// immediate (no memory allocation)
		__int,				// immediate for token (no memory allocation)
		__long,				// immediate number(no memory allocation)
		__double,			// immediate (no memory allocation)
		__complex,			// immediate (no memory allocation)
		__vector3, 			// pointer, double[3] ...
		__point3, 			// pointer, double[3] ...
		__vector4, 			// double pointer
		__vector5, 			// double pointer
		__vector6, 			// double pointer
		__string,			// pointer, quoted wstring
		__objectPtr,
		__symbolPtr,
		__instructionPtr,
		__RigidBodyPtr,
		__F_0,
		__F_D,
		__F_DD,
		__F_DDD,
		__F_L,
		__F_LD,
		__F_DL,
		__F_LDD,
		__F_DLD,
		__F_DDL,
		__F_LDDD,
		__F_DLDD,
		__F_DDLD,
		__F_DDDL,
		__symbolPtrGlobalVar,		// VAR
		__symbolPtrArgOrLocalVar,	// ARG
		__userFunc,
	};

///////////////////////////////////////////////////////////////////////////////
// errors
	void settypeerr();
	void setsizeerr();
	void setdimerr();
	void settypeerr(const std::wstring & s);
	void setsizeerr(const std::wstring & s);
	void setdimerr(const std::wstring & s);
	void divisionby0err(const objects &);
	void cksignerr(const objects &);
	void invalidserieserr(const std::wstring &);
	void toofewargs(const std::wstring & s);
	void toomanyargs(const std::wstring & s);
	void maxordererr();



#if 0
	template <class T> class dbgprint_vector: public std::vector<T> {
	public:
		// inherited constructors
		dbgprint_vector() : std::vector<T>::vector() { };
		dbgprint_vector(const std::vector<T>::const_iterator & b,const std::vector<T>::const_iterator & e) : std::vector<T>::vector(b,e) { };
		dbgprint_vector(int i) : std::vector<T>::vector(i) { };
		dbgprint_vector(int i,const T & t) : std::vector<T>::vector(i,t) { };
		// ~dbgprint_vector() { };
		// inherited destructors
		void dbgprint() { oss.seekp(0); oss << *this << std::endl; }
		static /*ostrstream*/ std::wstringstream oss;
	};
	
	struct user_function {
		std::wstring s;
		int parser_token;
		user_function(const std::wstring & mys,int i):s(mys),parser_token(i){};
	};

	// gen_op is the type of all functions taking 1 or more objects args 
	// and returning 1 arg of type objects
	// objects argument has atomic type for an unary op, 
	// and vecteur type (compttr) otherwise
	typedef objects ( * gen_op ) (const objects & arg);
	struct monome;

#ifdef DEBUG_SUPPORT
	typedef dbgprint_vector<objects> vecteur; // debugging support
#else
	typedef std::vector<objects> vecteur; // no debug. support
#endif
	vecteur makevecteur(const objects & a,const objects & b){
		vecteur v;
		v.push_back(a);
		v.push_back(b);
		return v;
	}
#endif
///////////////////////////////////////////////////////////////////////////////

class objects	
{
public:
	virtual ~objects();
		dataTypes type;	// see dispatch.h
		unsigned int * ref_count;
	union {
		int					iVal;	/* bison token values			*/
		long				lVal;	/* bison token values			*/
		double				dVal;
//		complex<double>*	cPtr;
		Vector3D*			vPtr;
		Point3D*			ptPtr;
		VectorND<4>*		v4Ptr;
		VectorND<5>*		v5Ptr;
		VectorND<6>*		v6Ptr;
		std::wstring*		sPtr;	/*	quoted STRING			*/
//		wchar_t*				sPtr;	/*	quoted STRING			*/
		objects*			oPtr;	/*	write from stack to *oPtr	*/
		symbol*				sym;	/* VAR: memory address of var data; FUNCTION: code for func */
		instruction*		iPtr;	/* while for if 'cond', ...  */
		CRigidBody*			rbPtr;
		F_0					fn;
		F_L					fnL;
		F_D					fnX;
		F_DD				fnXY;
		F_DDD				fnXYZ;
		F_LD				fnLX;
		F_DL				fnXL;
	};
		objects(): type(__undefined),ref_count(0),	iVal(0) {};

		//explicit
		objects (int a):						ref_count(0),	type(__int),			iVal(a) {}
		//explicit
		objects (long a):						ref_count(0),	type(__long),			lVal(a) {}
		//explicit
		objects (const double& a):				ref_count(0),	type(__double),			dVal(a) {}

		//explicit
		objects (const Vector3D& a):	ref_count(new unsigned int(1)),type(__vector3),	vPtr(new Vector3D(a))	{*ref_count=1;}
		//explicit
		objects (const Point3D& a):		ref_count(new unsigned int(1)),type(__point3),	ptPtr(new Point3D(a))	{*ref_count=1;}

		//explicit
		objects (const VectorND<4>& a):	ref_count(new unsigned int(1)),type(__vector4),	v4Ptr(new VectorND<4>(a))	{*ref_count=1;}
		//explicit
		objects (const VectorND<5>& a):	ref_count(new unsigned int(1)),type(__vector5),	v5Ptr(new VectorND<5>(a))	{*ref_count=1;}
		//explicit
		objects (const VectorND<6>& a):	ref_count(new unsigned int(1)),type(__vector6),	v6Ptr(new VectorND<6>(a))	{*ref_count=1;}



	//	objects (const wchar_t* a):		ref_count(new unsigned int(1)),type(__string),	sPtr(_tcsdup(a))		{*ref_count=1;}
		//explicit
		objects (const wchar_t* a):		ref_count(new unsigned int(1)),type(__string),	sPtr(new wstring(a))		{*ref_count=1;}
		//explicit
		objects (const wstring& a):		ref_count(new unsigned int(1)),type(__string),	sPtr(new wstring(a))		{*ref_count=1;}
//		objects (complex<double>& a):	ref_count(new unsigned int(1)),type(__complex),	cPtr(new complex<double>(a)) {*ref_count=1;}

		//explicit
		objects (F_0 a):	ref_count(0),	type(__F_0),			fn(a) {}
		//explicit
		objects (F_L a):	ref_count(0),	type(__F_L),			fnL(a) {}
		//explicit
		objects (F_D a):	ref_count(0),	type(__F_D),			fnX(a) {}
		//explicit
		objects (F_DD a):	ref_count(0),	type(__F_DD),			fnXY(a) {}
		//explicit
		objects (F_DDD a):	ref_count(0),	type(__F_DDD),			fnXYZ(a) {}
		//explicit
		objects (F_LD a):	ref_count(0),	type(__F_LD),			fnLX(a) {}
		//explicit
		objects (F_DL a):	ref_count(0),	type(__F_DL),			fnXL(a) {}

		//explicit
		objects (instruction* a):			ref_count(0),	type(__instructionPtr),	iPtr(a) {}
		//explicit
		objects (symbol* a):				ref_count(0),	type(__symbolPtr),		sym(a) {}
		//explicit
		objects (objects* a):				ref_count(0),	type(__objectPtr),		oPtr(a) {}//write from stack to *oPtr


		//explicit
//		objects (const CRigidBody& a):			ref_count(new unsigned int(1)),	type(__RigidBodyPtr),	rbPtr(new CRigidBody(a)) {*ref_count=1;}
		objects (CRigidBody* a);//:			ref_count(new unsigned int(1)),	type(__RigidBodyPtr),	rbPtr(a) {*ref_count=1;}

//		//explicit
		objects(const objects & a);
		objects & operator = (const objects & a);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(int a)				{*this=objects(a);return *this;}
		objects& operator=(long a)				{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(const double& a)		{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(const Vector3D& a)	{*this=objects(a);return *this;}
//		objects& operator=(const Point3D& a)	{*this=objects(*static_cast<Vector3D*>(const_cast<Point3D*>(&a)));return *this;}
		objects& operator=(const Point3D& a)	{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(const VectorND<4>& a)	{*this=objects(a);return *this;}
		objects& operator=(const VectorND<5>& a)	{*this=objects(a);return *this;}
		objects& operator=(const VectorND<6>& a)	{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(const wchar_t* a)		{*this=objects(a);return *this;}
		objects& operator=(const wstring& a)		{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(instruction* a)		{*this=objects(a);return *this;}
		objects& operator=(symbol* a)			{*this=objects(a);return *this;}
		objects& operator=(objects* a)			{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
//		objects& operator=(const CRigidBody& a)	{*this=objects(a);return *this;}
//		objects& operator=(CRigidBody* a)		{*this=objects(*a);return *this;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	//	objects& operator=(const CRigidBody& a)	{*this=objects(a);return *this;}
		objects& operator=(CRigidBody* a);//		{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		objects& operator=(F_0 a)	{*this=objects(a);return *this;}
		objects& operator=(F_L a)	{*this=objects(a);return *this;}
		objects& operator=(F_D a)	{*this=objects(a);return *this;}
		objects& operator=(F_DD a)	{*this=objects(a);return *this;}
		objects& operator=(F_DDD a)	{*this=objects(a);return *this;}
		objects& operator=(F_LD a)	{*this=objects(a);return *this;}
		objects& operator=(F_DL a)	{*this=objects(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		int AddRef() {
			if (ref_count) return ++(*ref_count);
			return 0;
		}
		int ReleaseRef();
///////////////////////////////////////////////////////////////////////////////
	friend std::wostream& operator<<(std::wostream& os, const objects& a);
	std::wstring print() const
	{
		std::wstringstream oss;
		oss << *this;
		return oss.str();
	}

	friend std::wistream& operator>>(std::wistream& _I, objects& b);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Comparison operations
//
	int operator==	(const objects& varSrc) const;
	int operator!=	(const objects& varSrc) const;
	int operator>=	(const objects& varSrc) const;
	int operator<=	(const objects& varSrc) const;
	int operator>	(const objects& varSrc) const;
	int operator<	(const objects& varSrc) const;
	int operator&&	(const objects& varSrc) const;
	int operator||	(const objects& varSrc) const;
///////////////////////////////////////////////////////////////////////////////
// arithmetic operations
//
	objects operator-() const;

	double norm() const;

	int friend operator!(const objects& a);

	objects friend operator+(const objects& a, const objects& b);
	objects friend operator-(const objects& a, const objects& b);
	objects friend operator*(const objects& a, const objects& b);
	objects friend operator/(const objects& a, const objects& b);
	objects friend operator^(const objects& a, const objects& b);
	objects friend operator%(const objects& a, const objects& b);

///////////////////////////////////////////////////////////////////////////////
	objects friend operator*(const double& a, const objects& b);
	objects friend operator*(const objects& b, const double& a);
	objects friend operator*(const long& a, const objects& b);
	objects friend operator*(const objects& b, const long& a);

	objects friend operator*(const long& a, const objects& b);
	objects friend operator*(const objects& b, const long& a);
///////////////////////////////////////////////////////////////////////////////
//	friend operator double (const objects& a) { return a.Dble(); }
//	friend operator const double (const objects& a) const { return a.Dble(); }
	operator double () { return Dble(); }
	operator const double () const { return Dble(); }
	double Dble(void) const;
///////////////////////////////////////////////////////////////////////////////
	operator float () { return Float(); }
	operator const float () const { return Float(); }
	float Float(void) const;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	operator long () { return Long(); }
	operator const long () const { return Long(); }
	int Long() const;
///////////////////////////////////////////////////////////////////////////////
	operator wstring () { return wstring (String()); }//
	operator wchar_t* () { return (wchar_t*) String(); }//
	operator const wchar_t* () const { return String(); }
	const wchar_t* String(void) const;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	operator Vector3D* () { return vPtr; }
//	operator const Vector3D* () const { return vPtr; }
	operator const Vector3D& () const { return *vPtr; }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	operator Point3D* () { return ptPtr; }
//	operator const Point3D* () const { return ptPtr; }
	operator const Point3D& () const { return *ptPtr; }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool True() const;
///////////////////////////////////////////////////////////////////////////////

};
//static const objects null_object(T_NULL);

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_OBJECTS_H__DFCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_)

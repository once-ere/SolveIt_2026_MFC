// object.h: interface for the object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__D9FCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_)
#define AFX_OBJECT_H__D9FCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <cstdio>
#include <vector>
#include <iostream>

#include <string>
#include <cstring>
#include <type_traits>
#include <cstdint>
#include <cassert>
#include <memory>

//#include <complex>
#include "RigidBody/RigidBody.h"
//#include "parser.h" /* for token definitions and yylval */

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class symbol;
class Vector3D;
class Point3D;
class object;
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
	enum dataTypes {
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
		__string,			// pointer, quoted string
		__object,
		__symbol,
		__instruction,
		__RigidBody,
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
		__symbolGlobalVar,		// VAR
		__symbolArgOrLocalVar,	// ARG
		__userFunc,
	};

///////////////////////////////////////////////////////////////////////////////
// errors
	void settypeerr();
	void setsizeerr();
	void setdimerr();
	void settypeerr(const std::string & s);
	void setsizeerr(const std::string & s);
	void setdimerr(const std::string & s);
	void divisionby0err(const object &);
	void cksignerr(const object &);
	void invalidserieserr(const std::string &);
	void toofewargs(const std::string & s);
	void toomanyargs(const std::string & s);
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
		static /*ostrstream*/ std::stringstream oss;
	};
	
	struct user_function {
		std::string s;
		int parser_token;
		user_function(const std::string & mys,int i):s(mys),parser_token(i){};
	};

	// gen_op is the type of all functions taking 1 or more object args 
	// and returning 1 arg of type object
	// object argument has atomic type for an unary op, 
	// and vecteur type (compttr) otherwise
	typedef object ( * gen_op ) (const object & arg);
	struct monome;

#ifdef DEBUG_SUPPORT
	typedef dbgprint_vector<object> vecteur; // debugging support
#else
	typedef std::vector<object> vecteur; // no debug. support
#endif
	vecteur makevecteur(const object & a,const object & b){
		vecteur v;
		v.push_back(a);
		v.push_back(b);
		return v;
	}
#endif
//////////////////////////////////////////std::make_shared<int>(2)/////////////////////////////////////


	template<typename Type>
	std::string type_to_string()
	{
#ifdef __FUNCSIG__
		std::string fname(__FUNCSIG__);
		const char* to_str = "to_string<";
		size_t len = strlen(to_str);
		auto pos = fname.find("to_string<");
		fname = fname.substr(pos + len);
		return fname.substr(0, fname.find_last_of('>'));
#else

		std::string fname(__PRETTY_FUNCTION__);

#ifdef __clang_major__
		const char* ftext = "[Type = ";
		auto pos = fname.find(ftext) + strlen(ftext);
		fname = fname.substr(pos);
		fname.pop_back();
		return fname;

#elif defined(__ICL)
		const char* ftext = "type_to_string<";
		auto pos = fname.find(ftext) + strlen(ftext);
		fname = fname.substr(pos);
		pos = fname.find_last_of('>');
		return fname.substr(0, pos);
#else
		const char* ftext = "[with Type = ";
		auto pos = fname.find(ftext) + strlen(ftext);
		fname = fname.substr(pos);
		pos = fname.find_first_of(';');
		return fname.substr(0, pos);
#endif
#endif

	} // end of type_to_string()

	template<typename Type>
	using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<Type>>;


class object	
{
public:
	~object();
	dataTypes type;
	union {
		int					iVal;	/* bison token values			*/
		long				lVal;	/* bison token values			*/
		double				dVal;
//		complex<double>	c;
		Vector3D			v;
		Point3D			pt;
		VectorND<4>		v4;
		VectorND<5>		v5;
		VectorND<6>		v6;
		std::string		s;			/*	quoted STRING			*/
//		char*			s;			/*	quoted STRING			*/
		object			o;			/*	write from stack to *o	*/
		symbol			sym;		/* VAR: memory address of var data; FUNCTION: code for func */
		instruction		i;			/* while for if 'cond', ...  */
		CRigidBody			rb;
		F_0					fn;
		F_L					fnL;
		F_D					fnX;
		F_DD				fnXY;
		F_DDD				fnXYZ;
		F_LD				fnLX;
		F_DL				fnXL;
	};
		object(): type(__undefined),	iVal(0) {};

		//explicit
		object(int a) :  type(__int), iVal(a) {
			auto o = Objects< int >(4);
			std::cout << type_to_string<decltype(o)>() << '\n';
		}
		//explicit
		object (long a):							type(__long),			lVal(a) {}
		//explicit
		object (const double& a):					type(__double),			dVal(a) {}

		//explicit
		object (const Vector3D& a):	type(__vector3),	v{std::move(Vector3D(a))}	{}
		//explicit
		object (const Point3D& a):		type(__point3),	pt{std::move(Point3D(a))}	{}

		//explicit
		object (const VectorND<4>& a):	type(__vector4),	v4{std::move(VectorND<4>(a))}	{}
		//explicit
		object (const VectorND<5>& a):	type(__vector5),	v5{std::move(VectorND<5>(a))}	{}
		//explicit
		object (const VectorND<6>& a):	type(__vector6),	v6{std::move(VectorND<6>(a))}	{}



	//	object (const char* a):		type(__string),	s(_tcsdup(a))		{}
		//explicit
		object (const char* a):		type(__string),	s{std::move(string(a))}		{}
		//explicit
		object (const string& a):		type(__string),	s{std::move(string(a))}		{}
//		object (complex<double>& a):	type(__complex),	c{std::move(complex<double>(a))} {}

		//explicit
		object (F_0 a):		type(__F_0),			fn(a) {}
		//explicit
		object (F_L a):		type(__F_L),			fnL(a) {}
		//explicit
		object (F_D a):		type(__F_D),			fnX(a) {}
		//explicit
		object (F_DD a):		type(__F_DD),			fnXY(a) {}
		//explicit
		object (F_DDD a):		type(__F_DDD),			fnXYZ(a) {}
		//explicit
		object (F_LD a):		type(__F_LD),			fnLX(a) {}
		//explicit
		object (F_DL a):		type(__F_DL),			fnXL(a) {}

		//explicit
		object (instruction* a):				type(__instruction),	i(a) {}
		//explicit
		object (symbol* a):					type(__symbol),		sym(a) {}
		//explicit
		object (object* a):					type(__object),		o(a) {}//write from stack to *o


		//explicit
//		object (const CRigidBody& a):				type(__RigidBody),	rb{std::move()}CRigidBody(a)) {}
		object (CRigidBody* a);//:				type(__RigidBody),	rb(a) {}

//		//explicit
		object(const object & a);
		object & operator = (const object & a);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		object& operator=(int a)				{*this=object(a);return *this;}
		object& operator=(long a)				{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		object& operator=(const double& a)		{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		object& operator=(const Vector3D& a)	{*this=object(a);return *this;}
//		object& operator=(const Point3D& a)	{*this=object(*static_cast<Vector3D*>(const_cast<Point3D*>(&a)));return *this;}
		object& operator=(const Point3D& a)	{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		object& operator=(const VectorND<4>& a)	{*this=object(a);return *this;}
		object& operator=(const VectorND<5>& a)	{*this=object(a);return *this;}
		object& operator=(const VectorND<6>& a)	{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		object& operator=(const char* a)		{*this=object(a);return *this;}
		object& operator=(const string& a)		{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		object& operator=(instruction* a)		{*this=object(a);return *this;}
		object& operator=(symbol* a)			{*this=object(a);return *this;}
		object& operator=(object* a)			{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
//		object& operator=(const CRigidBody& a)	{*this=object(a);return *this;}
//		object& operator=(CRigidBody* a)		{*this=object(*a);return *this;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	//	object& operator=(const CRigidBody& a)	{*this=object(a);return *this;}
		object& operator=(CRigidBody* a);//		{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		object& operator=(F_0 a)	{*this=object(a);return *this;}
		object& operator=(F_L a)	{*this=object(a);return *this;}
		object& operator=(F_D a)	{*this=object(a);return *this;}
		object& operator=(F_DD a)	{*this=object(a);return *this;}
		object& operator=(F_DDD a)	{*this=object(a);return *this;}
		object& operator=(F_LD a)	{*this=object(a);return *this;}
		object& operator=(F_DL a)	{*this=object(a);return *this;}
///////////////////////////////////////////////////////////////////////////////
		int AddRef() {
			if (ref_count) return ++(*ref_count);
			return 0;
		}
		int ReleaseRef();
///////////////////////////////////////////////////////////////////////////////
	friend ostream& operator<<(ostream& os, const object& a);
	std::string print() const
	{
		char buf[256];
		/*ostrstream*/ std::stringstream oss(buf, sizeof(buf));
		oss << *this << ends;
		return string(buf);
	}

	friend istream& operator>>(istream& _I, object& b);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Comparison operations
//
	int operator==	(const object& varSrc) const;
	int operator!=	(const object& varSrc) const;
	int operator>=	(const object& varSrc) const;
	int operator<=	(const object& varSrc) const;
	int operator>	(const object& varSrc) const;
	int operator<	(const object& varSrc) const;
	int operator&&	(const object& varSrc) const;
	int operator||	(const object& varSrc) const;
///////////////////////////////////////////////////////////////////////////////
// arithmetic operations
//
	object operator-() const;

	double norm() const;

	int friend operator!(const object& a);

	object friend operator+(const object& a, const object& b);
	object friend operator-(const object& a, const object& b);
	object friend operator*(const object& a, const object& b);
	object friend operator/(const object& a, const object& b);
	object friend operator^(const object& a, const object& b);
	object friend operator%(const object& a, const object& b);

///////////////////////////////////////////////////////////////////////////////
	object friend operator*(const double& a, const object& b);
	object friend operator*(const object& b, const double& a);
	object friend operator*(const long& a, const object& b);
	object friend operator*(const object& b, const long& a);

	object friend operator*(const long& a, const object& b);
	object friend operator*(const object& b, const long& a);
///////////////////////////////////////////////////////////////////////////////
//	friend operator double (const object& a) { return a.Dble(); }
//	friend operator const double (const object& a) const { return a.Dble(); }
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
	operator string () { return string (String()); }//
	operator char* () { return (char*) String(); }//
	operator const char* () const { return String(); }
	const char* String(void) const;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	operator Vector3D* () { return v; }
//	operator const Vector3D* () const { return v; }
	operator const Vector3D& () const { return *v; }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	operator Point3D* () { return pt; }
//	operator const Point3D* () const { return pt; }
	operator const Point3D& () const { return *pt; }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool True() const;
///////////////////////////////////////////////////////////////////////////////

};
//static const object null_object(T_NULL);

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_OBJECT_H__D9FCEF8F7_4D83_4CF8_90D6_634F3264849C__INCLUDED_)

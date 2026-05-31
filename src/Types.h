// Types.h: interface for the Types class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPES_H__8498B5BE_2B81_453A_A2D8_2312C3F54A0B__INCLUDED_)
#define AFX_TYPES_H__8498B5BE_2B81_453A_A2D8_2312C3F54A0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Matherror.h"
#include "array.h"
#include <functional>

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class instruction;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Mutex
{
	friend class Lock;
public:
	Mutex () { InitializeCriticalSection (&_critSection); }
	~Mutex () { DeleteCriticalSection (&_critSection); }
private:
	void Acquire () 
	{ 
		EnterCriticalSection (&_critSection);
	}
	void Release () 
	{ 
		LeaveCriticalSection (&_critSection);
	}

	CRITICAL_SECTION _critSection;
};

class Lock 
{
public:
	// Acquire the state of the semaphore
	Lock ( Mutex& mutex ) 
		: _mutex(mutex) 
	{
		_mutex.Acquire();
	}
	// Release the state of the semaphore
	~Lock ()
	{
		_mutex.Release();
	}
private:
	Mutex& _mutex;
};

static const char *CVOdeErrors[] = {
	"SUCCESS=0",
	"CVODE_NO_MEM=-1",
	"ILL_INPUT=-2",
	"TOO_MUCH_WORK=-3",
	"TOO_MUCH_ACC=-4",
	"ERR_FAILURE=-5",
	"CONV_FAILURE=-6",
	"SETUP_FAILURE=-7",
	"SOLVE_FAILURE=-8"
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
struct Couple
{
	Vector3D force;
	Point3D point;//point of application of associated force
	Vector3D torqe;//torque, not always (point-cm)^force
	Couple() {}
	Couple(const Vector3D f) : force(f) {}
	Couple(const Vector3D f, const Vector3D r) : force(f), torqe(r.vectorProd(f)) {}
	Couple(const Couple& c) :
		force(c.force),
		torqe(c.torqe),
		point(c.point)
	{}
	Couple& operator=(const Couple& c)
	{
		force = c.force;
		torqe = c.torqe;
		point = c.point;
		return *this;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct _CallUserDefined
{
	_CallUserDefined():func(0),indexInDataArray(-1),nvargs(-1),nargs(-1),m_bIsTimeDependent(false){}
	wstring name;
	instruction*	func;
	unsigned long	indexInDataArray;	//	memory address in 'data'
	unsigned long	nvargs;				//	# function arguments and local vars
	unsigned long	nargs;				//	# function arguments 
	bool m_bIsTimeDependent;
};
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CleanupOnInitialize
///////////////////////////////////////////////////////////////////////////////
struct HWNDCompareEqual
{
	HWND n;
	HWNDCompareEqual(HWND j) : n(j) {}
	bool operator()(const HWND& a) const
	{
		return a == n;
	}
};
///////////////////////////////////////////////////////////////////////////////
#if 0
#include <functional>
#include <algorithm>
class CCleanupOnInitialize : public vector<HWND>
{
public:
	CCleanupOnInitialize(){}
	virtual ~CCleanupOnInitialize()
	{
		Clear();
	}
///////////////////////////////////////////////////////////////////////////////
	void Clear()
	{
		if (size() == 0) return;
		vector<HWND>::iterator i;
		for (i = begin(); i != end(); ++i)
		{
			HWND hWnd = *i;
			if (::IsWindow(hWnd)) ::SendMessage(hWnd, WM_CLOSE,0,0);
		}
		clear();
	}
///////////////////////////////////////////////////////////////////////////////
	void Add(const HWND& hWnd)
	{
		ASSERT(hWnd!=0);
		ASSERT(::IsWindow(hWnd));
		if (hWnd==0) return;
		push_back( hWnd );
	}
///////////////////////////////////////////////////////////////////////////////
	void Remove(const HWND& hWnd)
	{
		HWNDCompareEqual _HWNDCompareEqual(hWnd);
		vector<HWND>::iterator j = find_if( begin(), end(), _HWNDCompareEqual);
		ASSERT(j != end());
		if (j != end()) erase(j);
	}
};
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef double (*_Tension)(double X, double Y, double vx, double vy, double R, double g, double InertiaInv, double w);
typedef double (*_Tension2)(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);

typedef double (*_Cos)(double x, double y, double R);
typedef double (*_Sin)(double x, double y, double R);
///////////////////////////////////////////////////////////////////////////////

class CRigidBody;
class CConstraint;
class CPulleyConstraint;

///////////////////////////////////////////////////////////////////////////////
typedef map< unsigned long, string>	KeyToName;

typedef map< string, unsigned long>	NameToKey;
typedef map< unsigned long,  CConstraint* >	KeyToConstraint_List;

///////////////////////////////////////////////////////////////////////////////
typedef vector<CConstraint*> ListOfConstraints;
typedef map< unsigned long,  CConstraint* >	KeyToConstraint_List;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct pair<CRigidBody*, CConstraint*> PAIR_RigidBody_Constraint;
typedef vector<PAIR_RigidBody_Constraint> ListOf_RigidBody_Constraint_Pairs;

typedef struct pair<CRigidBody*, ListOfConstraints> PAIR_RigidBody_ConstraintList;
typedef vector<PAIR_RigidBody_ConstraintList> ListOf_RigidBody_ConstraintList_Pairs;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct MyCompare
{  
	bool operator()(double *const& x, double *const& y) const {
		return std::less<double*>()(x, y);
	}
};
///////////////////////////////////////////////////////////////////////////////
struct PAIR_RigidBody_ConstraintCompareEqual
{
	PAIR_RigidBody_ConstraintCompareEqual(PAIR_RigidBody_Constraint _b) : b(_b) {}
	PAIR_RigidBody_Constraint b;
	bool operator()(const PAIR_RigidBody_Constraint& a) const
	{
		return a.first == b.first && a.second == b.second;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct BodiesCompareEqual
{
	BodiesCompareEqual(CRigidBody* _b) : b(_b) {}
	CRigidBody* b;
	bool operator()(CRigidBody* a) const
	{
		return a == b;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct CompareConstraintToMe
{
	CompareConstraintToMe(CConstraint* _b) : b(_b) {}
	CConstraint* b;
	bool operator()(CConstraint* a) const
	{
		return a == b;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct ConstraintsCompareEqual
{  
	bool operator()(const CConstraint*& a, const CConstraint*& b) const
	{
		return a == b;
	}
};
///////////////////////////////////////////////////////////////////////////////
struct ConstraintsCompareLess
{  
	bool operator()(const CConstraint*& a, const CConstraint*& b) const
	{
		return std::less<const CConstraint*>()(a, b);
	}
};
///////////////////////////////////////////////////////////////////////////////
class Types  
{
public:
	Types();
	virtual ~Types();

};

////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt



#endif // !defined(AFX_TYPES_H__8498B5BE_2B81_453A_A2D8_2312C3F54A0B__INCLUDED_)

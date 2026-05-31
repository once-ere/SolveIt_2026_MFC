// State.h : Declaration of the CHost

#ifndef __State_H_
#define __State_H_

//#include "Vector3D.h"
#include "Mat3.h"
#include "MatX.h"
#include "Se3.h"

namespace SolveIt {

class CRigidBody;

/*
class CPointMass;
	union {
		struct
		{
			Vector3D x;
			Quat q;
			Vector3D p;
			Vector3D L;
		};
		double v[DEGREES_FREEDOM*sizeof(double)];
	};
		CopyMemory((void*)y, (void*)&x.x, sizeof(State));
		ASSERT(v!=&x.x);
		ASSERT(&v[DEGREES_FREEDOM-1]!=&L.z);
*/
/////////////////////////////////////////////////////////////////////////////
// State
#define DEGREES_FREEDOM 13
typedef double(StateVector)[DEGREES_FREEDOM];

class State
{
public:
	double t;	//time
	Point3D x;	//position
	Quat q;		//quaternion
	Vector3D p;	//linear momentum
	Vector3D L;	//angular momentum
//////////////////////////////////////////////////////////////////////
	State() : t(0), q(Quat::ID) {}

	State(
		double _t,
		Point3D _x,
		Quat _q,
		Vector3D _p,
		Vector3D _L) :
		t(_t), x(_x), q(_q), p(_p), L(_L)
	{	}


	virtual ~State(){}
	
	State(const State& s)
	{
		*this = s;
	}

	State& operator=(const State& s)
	{
		t = s.t;
		x = s.x;
		q = s.q;
		p = s.p;
		L = s.L;
		return *this;
	}

	State& operator=( vector<double>::const_iterator & iy )
	{
		long j=0;
		for (j=0; j<3; ++j) x.v[j] = *iy++;
		for (j=0; j<4; ++j) q.v[j] = *iy++;
		for (j=0; j<3; ++j) p.v[j] = *iy++;
		for (j=0; j<3; ++j) L.v[j] = *iy++;
		return *this;
	}

	void toVector(vector<double>& y)
	{
		long j=0;
		for (j=0; j<3; ++j) y.push_back( x.v[j] );
		for (j=0; j<4; ++j) y.push_back( q.v[j] );
		for (j=0; j<3; ++j) y.push_back( p.v[j] );
		for (j=0; j<3; ++j) y.push_back( L.v[j] );
	}

	void SetTime(double _t) {t=_t;}


};

extern const State Unit_State;
extern const State Zero_State;

//typedef struct _TRIPLE {double x,y,z;} TRIPLE, *PTRIPLE;

} // namespace SolveIt

/////////////////////////////////////////////////////////////////////////////
#endif

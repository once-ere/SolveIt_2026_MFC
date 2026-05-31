// Se3.h: interface for the Se3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SE3_H__770D90A2_D038_11D3_A704_0050045B99C4__INCLUDED_)
#define AFX_SE3_H__770D90A2_D038_11D3_A704_0050045B99C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Mat3.h"
#include <sstream>

namespace SolveIt {
struct MatX;

struct Se3
{
	Quat q;		 // rotation component
	Vector3D d;		// translation component

	// constructors //////////////////////////////////////////////////////////////


	Se3() {}
	Se3(const Quat &q_, const Vector3D &d_) {set(q_, d_);}

	Se3(const Se3& s) : q(s.rot()), d(s.trans()) {}
	Se3(const MatX& s);
	Se3& operator=(const Se3& s) {
		q=s.rot(), d=s.trans();
		return *this;}

	// setters / accessors / translators /////////////////////////////////////////

	void set(const Quat &q_, const Vector3D &d_) {q = q_; d = d_;}

#if INVENTOR
	inline void set(const SoTransform *T);
	inline void toSoTransform(SoTransform *T) const;
#endif


	const Quat	&rot()	 const {return q;}
	const Vector3D &trans() const {return d;}
				Quat	&rot()				 {return q;}
				Vector3D &trans()			 {return d;}

	// input / output ////////////////////////////////////////////////////////////


	inline wostream& print(wostream &os) const;

	// Read Se3 from input stream.	The Se3 specification must be
	// enclosed in curly brackets { }.	Se3's are built up from a
	// sequence of translation and rotation operations.	A translation
	// opereration is specified by "trans v", where v is the translation
	// vector.	A rotation operation is specified by: "rot a v", where a
	// is the scalar rotation in *degrees*, and v is the axis of
	// rotation (a vector).

	wistream& read(wistream &is);

	// operations returning result via this //////////////////////////////////////

	// The operation result indicated by the comments is always returned
	// in this.	The symbol [!] indicates that this must be distinct
	// from all of the operands.

	friend Se3 operator*(const Se3 &M, const Se3 &N) {
		Se3 X;
		X.mult(M, N);
		return X;
	}
	

	inline void mult(const Se3 &T, const Se3 &U);		// T * U		 [!]
	inline void premult(const Se3 &T);							 // T * this	[!]
	inline void postmult(const Se3 &T);							// this * T	[!]
	inline void invert(const Se3 &T);								// T^-1
	inline void invert();														// this^-1

	// Transforming Vect3s ///////////////////////////////////////////////////////

	// Se3s can transform elements of R^3 either as vectors or as
	// points.	Multiple operands need not be distinct.

	inline void transform(const Vector3D &v, Vector3D &xv) const;	// this * (v 0) => xv
	inline void transform(Vector3D &v) const;			 // this * (v 0) => v
	inline void transform(const Point3D &p, Point3D &xp) const; // this * (p 1) => xp
	inline void transform(Point3D &p) const;			 // this * (p 1) => p

	// These are exactly like the above methods, except the inverse
	// transform this^-1 is used.
	inline void inverseTransform(const Vector3D &v, Vector3D &xv) const;
	inline void inverseTransform(Vector3D &v) const;								 
	inline void inverseTransform(const Point3D &p, Point3D &xp) const;
	inline void inverseTransform(Point3D &p) const;								 


	// Se3 constants /////////////////////////////////////////////////////////////

	static const Se3 ID;			// identity Se3
};



///////////////////////////////////////////////////////////////////////////////
//
//	stream operators
//
///////////////////////////////////////////////////////////////////////////////


//inline wostream &operator<<(wostream &os, const Vector3D &v) {return v.print(os);}
inline wostream &operator<<(wostream &os, const Mat3 &M);//	{return M.print(os);}
inline wostream &operator<<(wostream &os, const Quat &q)	{return q.print(os);}
inline wostream &operator<<(wostream &os, const Se3 &T)	 {return T.print(os);}

//inline wistream &operator>>(wistream &os, Vector3D &v)			 {return v.read(os);}
inline wistream &operator>>(wistream &is, Se3 & T)				{return T.read(is);}


///////////////////////////////////////////////////////////////////////////////


wostream& Se3::print(wostream &os) const
{
	q.print(os);
	std::ostringstream tmp;
	d.print(tmp);
	os << tmp.str().c_str();
	return os;
}


void Se3::mult(const Se3 &T, const Se3 &U)
{
	q.mult(T.q, U.q);
	T.q.rotate(U.d, d);
	d += T.d;
}


void Se3::premult(const Se3 &T)
{
	q.premult(T.q);
	T.q.rotate(d);
	d += T.d;
}


void Se3::postmult(const Se3 &T)
{
	Vector3D v;

	q.rotate(T.d, v);
	d += v;
	q.postmult(T.q);
}


void Se3::invert(const Se3 &T)
{
/*
	q.s_ = -T.q.s_;
	q.x_ =	T.q.x_;
	q.y_ =	T.q.y_;
	q.z_ =	T.q.z_;
	q.rotate(T.d, d);
	d.negate(d);
*/
//Quat Q;
	q.invert(T.q);
	q.rotate(T.d, d);
	d.negate();
}


void Se3::invert()
{
//	q.s_ = -q.s_;
	q.invert();
	q.rotate(d);
	d.negate();
}


void Se3::transform(const Vector3D &v, Vector3D &xv) const
{
	q.rotate(v, xv);
}


void Se3::transform(Vector3D &v) const
{
	q.rotate(v);
}


void Se3::transform(const Point3D &p, Point3D &xp) const
{
	q.rotate(p, xp);
	xp += d;
}


void Se3::transform(Point3D &p) const
{
	q.rotate(p);
	p += d;
}


void Se3::inverseTransform(const Vector3D &v, Vector3D &xv) const
{
	q.inverseRotate(v, xv);
}


void Se3::inverseTransform(Vector3D &v) const
{
	q.inverseRotate(v);
}


void Se3::inverseTransform(const Point3D &p, Point3D &xp) const
{
//xp.sub(p, d);
//q.inverseRotate(xp);

	Point3D a;
	q.inverseRotate(d,a);
	q.inverseRotate(p,xp);
	xp -= a;

}


void Se3::inverseTransform(Point3D &p) const
{
//p.sub(d);
//q.inverseRotate(p);
	Vector3D a;
	q.inverseRotate(d,a);
	q.inverseRotate(p);
	p -= a;
}


#if INVENTOR
void Se3::set(const SoTransform *xform)
{
	const float *quat;
	const float *trans;

	quat = xform->rotation.getValue().getValue();
	q.x_ = quat[0]; q.y_ = quat[1]; q.z_ = quat[2]; q.s_ = quat[3];

	trans = xform->translation.getValue().getValue();
	d.x = trans[0];
	d.y = trans[1];
	d.z = trans[2];
}

void Se3::toSoTransform(SoTransform *xform) const
{
	xform->rotation.setValue(q.x_, q.y_, q.z_, q.s_);
	xform->translation.setValue(d.x, d.y, d.z);
}
#endif

} // namespace SolveIt

#endif // !defined(AFX_SE3_H__770D90A2_D038_11D3_A704_0050045B99C4__INCLUDED_)

///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1998 Mitsubishi Electric Information Technology Center
//	America (MEITCA).	All Rights Reserved.
//
//	Permission to use, copy, modify and distribute this software and
//	its documentation for educational, research and non-profit
//	purposes, without fee, and without a written agreement is hereby
//	granted, provided that the above copyright notice and the
//	following three paragraphs appear in all copies.
//
//	Permission to incorporate this software into commercial products
//	may be obtained from MERL - A Mitsubishi Electric Research Lab, 201
//	Broadway, Cambridge, MA 02139.
//
//	IN NO EVENT SHALL MEITCA BE LIABLE TO ANY PARTY FOR DIRECT,
//	INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
//	LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
//	DOCUMENTATION, EVEN IF MEITCA HAS BEEN ADVISED OF THE POSSIBILITY
//	OF SUCH DAMAGES.
//
//	MEITCA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
//	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//	FOR A PARTICULAR PURPOSE.	THE SOFTWARE PROVIDED HEREUNDER IS ON
//	AN "AS IS" BASIS, AND MEITCA HAS NO OBLIGATIONS TO PROVIDE
//	MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
//	Author:
//		Brian Mirtich
//		mirtich@merl.com
//		617.621.7573
//		www.merl.com/people/mirtich
//
//	Library and API:
//		James Kuffner, Jr.
//		kuffner@stanford.edu
//		650.725.8812
//		http://robotics.stanford.edu/~kuffner
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MATX_H
#define MATX_H

#include "Mat3.h"

namespace SolveIt {
struct Se3;


///////////////////////////////////////////////////////////////////////////////
struct PAIR_Vector3D;
struct Mat6;
///////////////////////////////////////////////////////////////////////////////
//
//	class MatX
//
///////////////////////////////////////////////////////////////////////////////


struct MatX {

//	friend class Se3;

//private:

	Mat3 R;
	Vector3D d;

//public:

	// constructors //////////////////////////////////////////////////////////////

	MatX()	{}
	MatX(const Mat3 &R_, const Vector3D &d_)	{set(R_, d_);}
	MatX(const Vector3D &d_)	{set(Mat3::ID, d_);}
	MatX(const Se3 &T)	{set(T);}


	MatX(const MatX &m) {set(m.R, m.d);}
	MatX& operator=(const MatX &m) {
		set(m.R, m.d);
		return *this;
	}

	// setters / accessors / translators /////////////////////////////////////////
	
	inline void set(const Mat3 &R_, const Vector3D &d_) {R = R_; d = d_;}
	void set(const Se3 &T);

	const Mat3	&rot()	 const {return R;}
	const Vector3D &trans() const {return d;}
				Mat3	&rot()				 {return R;}
				Vector3D &trans()			 {return d;}
	
	// input / output ////////////////////////////////////////////////////////////

	inline wostream& print(wostream &os) const;
				 wistream& read(wistream &is);

	// operations returning result via this //////////////////////////////////////

	// The operation result indicated by the comments is always returned
	// in this.	The symbol [!] indicates that this must be distinct
	// from all of the operands.
//	MatX operator*(const MatX &N) const {
//		MatX X;
//		X.mult(*this, N);
//		return X;
//	}

	friend MatX operator*(const MatX &M, const MatX &N) {
		MatX X;
		X.mult(M, N);
		return X;
	}

	friend MatX operator*(const Mat3 &R, const MatX &N) {
		MatX X;
		X.R = R*N.R;
		X.d = R*N.d;
		return X;
	}

	friend MatX operator*(const MatX &N, const Mat3 &R) {
		MatX X;
		X.R = N.R*R;
		X.d = N.d;
		return X;
	}

	void mult(const MatX &M, const MatX &N);		// M * N		 [!]
	void premult(const MatX &M);								// M * this	[!]
	void postmult(const MatX &M);							 // this * M	[!]
	void invert(const MatX &M);								 // M^-1			[!]
	void invert();															// this^-1

	// Transforming Vect3s ///////////////////////////////////////////////////////

	// MatXs can transform elements of R^3 either as vectors or as
	// points.	The [!] indicates that the operands must be distinct.

	inline void xformVect(const Vector3D &v, Vector3D &xv) const; // this*(v 0)=>xv	[!]
	inline void xformVect(Vector3D &v) const;									// this*(v 0)=>v
	inline void xformPoint(const Vector3D &p, Vector3D &xp) const;// this*(p 1)=>xp	[!]
	inline void xformPoint(Vector3D &p) const;								 // this*(p 1)=>p

	Vector3D operator*(const Vector3D &v) const {
		return R*v;
	}

	Point3D operator*(const Point3D &p) const {
		return R*p+d;
	}

///////////////////////////////////////////////////////////////////////////////
	PAIR_Vector3D operator*(const PAIR_Vector3D &v) const;

	PAIR_Vector3D invXformSpatVect(const PAIR_Vector3D &u) const;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	Mat6 operator*(const Mat6 &v) const;
///////////////////////////////////////////////////////////////////////////////
	// These are exactly like the above methods, except the inverse
	// transform this^-1 is used.
	inline void invXformVect(const Vector3D &v, Vector3D &xv) const;
	inline void invXformVect(Vector3D &v) const;								 
	inline void invXformPoint(const Vector3D &p, Vector3D &xp) const;
	inline void invXformPoint(Vector3D &p) const;								 


	// MatX constants ////////////////////////////////////////////////////////////

	static const MatX ID;			// identity matrix

};


///////////////////////////////////////////////////////////////////////////////
//
//	inline function definitions
//
///////////////////////////////////////////////////////////////////////////////

wostream& MatX::print(wostream &os) const
{
	return os << R << d << endl;
}

void MatX::xformVect(const Vector3D &v, Vector3D &xv) const
{
	R.xform(v, xv);
}

	
void MatX::xformVect(Vector3D &v) const
{
	R.xform(v);
}

	
void MatX::xformPoint(const Vector3D &p, Vector3D &xp) const
{
	R.xform(p, xp);
	xp+=(d);
}


void MatX::xformPoint(Vector3D &p) const
{
	R.xform(p);
	p+=(d);
}


void MatX::invXformVect(const Vector3D &v, Vector3D &xv) const
{
	R.invXform(v, xv);
}

	
void MatX::invXformVect(Vector3D &v) const
{
	R.invXform(v);
}

	
void MatX::invXformPoint(const Vector3D &p, Vector3D &xp) const
{
	xp=(p - d);
	R.invXform(xp);
}


void MatX::invXformPoint(Vector3D &p) const
{
	p-=(d);
	R.invXform(p);
}

///////////////////////////////////////////////////////////////////////////////
//
//	stream operators
//
///////////////////////////////////////////////////////////////////////////////

inline wostream &operator<<(wostream &os, const MatX &X)	{return X.print(os);}
inline wistream &operator>>(wistream &is, MatX &X)				{return X.read(is);}


} // namespace SolveIt

#endif	// MATX_H

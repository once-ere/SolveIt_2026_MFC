// Mat3.h: interface for the Mat3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAT3_H__770D90A1_D038_11D3_A704_0050045B99C4__INCLUDED_)
#define AFX_MAT3_H__770D90A1_D038_11D3_A704_0050045B99C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace SolveIt {

struct Quat;
//#ifndef ARRAY3
#if !defined(ARRAY3)
//typedef double(ARRAY3)[3];
#endif

//namespace duh {
//	typedef double(ARRAY3)[3];
//}

struct Mat3	
{
// (stored in row-major order)
	union {
		struct {
			double	xx, xy, xz,
					yx, yy, yz,
					zx, zy, zz;
		};
		double m[3][3];
	};

// constructors //////////////////////////////////////////////////////////////

	Mat3() :
		xx(0), xy(0), xz(0),
		yx(0), yy(0), yz(0),
		zx(0), zy(0), zz(0)
	{
			assert(&m[0][0]==&xx);
			assert(&m[2][2]==&zz);
	}

	Mat3(double	a, double	b, double	c) :
		xx(a), xy(0), xz(0),
		yx(0), yy(b), yz(0),
		zx(0), zy(0), zz(c)
	{
			assert(&m[0][0]==&xx);
			assert(&m[2][2]==&zz);
	}

	Mat3(double	a) :
		xx(a), xy(0), xz(0),
		yx(0), yy(a), yz(0),
		zx(0), zy(0), zz(a)
	{
			assert(&m[0][0]==&xx);
			assert(&m[2][2]==&zz);
	}

	Mat3(const Vector3D &axis, double angle)	{set(axis, angle);}

	Mat3(const Quat &q) {set(q);}
//pln:
	Mat3(const Vector3D &a) :
		xx(0), xy(-a.v[2]), xz(a.v[1]),
		yx(a.v[2]), yy(0), yz(-a.v[0]),
		zx(-a.v[1]), zy(a.v[0]), zz(0)
	{}

	Mat3(const Vector3D &a, const Vector3D &b) :
		xx(a.v[0]*b.v[0]), xy(a.v[0]*b.v[1]), xz(a.v[0]*b.v[2]),
		yx(a.v[1]*b.v[0]), yy(a.v[1]*b.v[1]), yz(a.v[1]*b.v[2]),
		zx(a.v[2]*b.v[0]), zy(a.v[2]*b.v[1]), zz(a.v[2]*b.v[2])
	{}

	Mat3(const Mat3 &m) :
		xx(m.xx), xy(m.xy), xz(m.xz),
		yx(m.yx), yy(m.yy), yz(m.yz),
		zx(m.zx), zy(m.zy), zz(m.zz)
	{}
	Mat3& operator=(const Mat3 &m) {
		xx=m.xx, xy=m.xy, xz=m.xz,
		yx=m.yx, yy=m.yy, yz=m.yz,
		zx=m.zx, zy=m.zy, zz=m.zz;
		return *this;
	}

	Mat3 operator ~() const {
		Mat3 M;
		M.xx = xx;
		M.xy = yx;
		M.xz = zx;

		M.yx = xy;
		M.yy = yy;
		M.yz = zy;

		M.zx = xz;
		M.zy = yz;
		M.zz = zz;
		return M;
	}


	inline void operator+=(const Mat3 &m);
	inline void operator-=(const Mat3 &m);

	inline Mat3 operator+(const Mat3 &m) const;
	inline Mat3 operator-(const Mat3 &m) const;


Vector3D operator*(const Vector3D &v) const {
		Vector3D V(	xx*v.v[0] + xy*v.v[1] + xz*v.v[2],
					yx*v.v[0] + yy*v.v[1] + yz*v.v[2],
					zx*v.v[0] + zy*v.v[1] + zz*v.v[2]);
		return V;
	}

	Mat3 operator*(const Mat3 &N) const;		 // M * N		 [!]
	void premult(const Mat3 &M);								 // M * this	[!]
	void postmult(const Mat3 &M);								// this * M	[!]
// setters / accessors ///////////////////////////////////////////////////////
	// set Mat3 as a rotation of 'angle' radians about 'axis'
	// axis is automatically normalized unless normalizeAxis = 0
	inline void set(const Vector3D &axis, double angle);
	void set(const Quat &q);

void set(const Vector3D &diag, const Vector3D &sym)
{
	xx = diag.v[0];
	yy = diag.v[1];
	zz = diag.v[2];
	yz = zy = sym.v[0];
	zx = xz = sym.v[1];
	xy = yx = sym.v[2];
}

	// index-based access:	0=xrow, 1=yrow, 2=zrow.
	const Vector3D &operator[](int i) const {return *(((Vector3D *) &xx) + i);}
	Vector3D &operator[](int i)			 {return *(((Vector3D *) &xx) + i);}

	// input / output ////////////////////////////////////////////////////////////

	wostream& print(wostream &os) const;

	// operations not returning a Mat3 ///////////////////////////////////////////

	inline double det() const;

	// operations returning result via this //////////////////////////////////////

	// The operation result indicated by the comments is always returned
	// in this.	The symbol [!] indicates that this must be distinct
	// from all of the operands.	The invert() methods are based on the
	// explicit inversion formula from determinants; there are faster
	// and more accurate ways.	The invert() methods return one if the
	// matrix was not invertible, otherwise zero.

	inline void xpose(const Mat3 &M);									 // M^T			 [!]
	inline void xpose();																// this^T

	int	invert(const Mat3 &M);									// M^-1			[!]
	int	invert();															 // this^-1
	inline void negate(const Mat3 &M);									// -M
	inline void negate();															 // -this

	inline void scale(const Mat3 &M, double s);					 // s * M
	inline void scale(double s);													// s * this

	// Transforming Vect3s ///////////////////////////////////////////////////////

	inline void xform(const Vector3D &v, Vector3D &xv) const; // (this)(v) => xv; 
																											// v & xv must be distinct
	inline void xform(Vector3D &v) const;									// (this)(v) => v

	// These are exactly like the above methods, except the inverse
	// transform this^-1 (= this^T) is used.	This can be thought of as
	// a row vector transformation, e.g.: (v^T)(this) => xv^T
	inline void invXform(const Vector3D &v, Vector3D &xv) const;
	inline void invXform(Vector3D &v) const;


	// for reading rows
//	const Vector3D &xrow() const {return *((Vector3D *) m[0]/*xx*/);}
//	const Vector3D &yrow() const {return *((Vector3D *) m[1]/*yx*/);}
//	const Vector3D &zrow() const {return *((Vector3D *) m[2]/*zx*/);}
	// for writing to rows
//	Vector3D &xrow()	{return *((Vector3D *) m[0]/*xx*/);}
//	Vector3D &yrow()	{return *((Vector3D *) m[1]/*yx*/);}
//	Vector3D &zrow()	{return *((Vector3D *) m[2]/*zx*/);}

	// for reading rows
	Vector3D xrow() const {return Vector3D(m[0]);}
	Vector3D yrow() const {return Vector3D(m[1]);}
	Vector3D zrow() const {return Vector3D(m[2]);}
	// for writing to rows
//typedef double(ARRAY3)[3];
typedef double(AfRRAY3)[3];
	AfRRAY3 &xrow()	{return m[0];}
	AfRRAY3 &yrow()	{return m[1];}
	AfRRAY3 &zrow()	{return m[2];}

	// for reading columns
	Vector3D xcol() const {return Vector3D(xx, yx, zx);}
	Vector3D ycol() const {return Vector3D(xy, yy, zy);}
	Vector3D zcol() const {return Vector3D(xz, yz, zz);}
	// for writing to columns
	inline void setXcol(const Vector3D &v);
	inline void setYcol(const Vector3D &v);
	inline void setZcol(const Vector3D &v);

	// for reading a symmetric matrix
	Vector3D diag() const {return Vector3D(xx, yy, zz);}
	Vector3D sym()	const {return Vector3D(yz, zx, xy);}



// Mat3 constants ////////////////////////////////////////////////////////////

	static const Mat3 ZERO;		// zero matrix
	static const Mat3 ID;			// identity matrix

};


void Mat3::setXcol(const Vector3D &v)
{
	xx = v.x;
	yx = v.y;
	zx = v.z;
}


void Mat3::setYcol(const Vector3D &v)
{
	xy = v.x;
	yy = v.y;
	zy = v.z;
}


void Mat3::setZcol(const Vector3D &v)
{
	xz = v.x;
	yz = v.y;
	zz = v.z;
}

///////////////////////////////////////////////////////////////////////////////
//
//	stream operators
//
///////////////////////////////////////////////////////////////////////////////

inline wostream &operator<<(wostream &os, const Mat3 &M)	{return M.print(os);}

///////////////////////////////////////////////////////////////////////////////

void Mat3::xform(const Vector3D &v, Vector3D &xv) const
{
	xv.x = xx * v.x + xy * v.y + xz * v.z;
	xv.y = yx * v.x + yy * v.y + yz * v.z;
	xv.z = zx * v.x + zy * v.y + zz * v.z;
}


void Mat3::xform(Vector3D &v) const
{
	double ox, oy;

	ox = v.x; oy= v.y;
	v.x = xx * ox + xy * oy + xz * v.z;
	v.y = yx * ox + yy * oy + yz * v.z;
	v.z = zx * ox + zy * oy + zz * v.z;
}


/*
//ERROR:
void Mat3::invXform(const Vector3D &v, Vector3D &xv) const
{
	xv.x = xx * v.x + yx * v.y + zx * v.z;
	xv.y = xy * v.x + yy * v.y + zy * v.z;
	xv.z = xz * v.x + yz * v.y + zz * v.z;
}


//ERROR:
void Mat3::invXform(Vector3D &v) const
{
	double ox, oy;

	ox = v.x; oy= v.y;
	v.x = xx * ox + yx * oy + zx * v.z;
	v.y = xy * ox + yy * oy + zy * v.z;
	v.z = xz * ox + yz * oy + zz * v.z;
}
*/

//ERROR:
void Mat3::invXform(const Vector3D &v, Vector3D &xv) const
{
	Mat3 M;
	if (!M.invert(*this)) M.xform(v, xv);
/*
	xv.x = xx * v.x + yx * v.y + zx * v.z;
	xv.y = xy * v.x + yy * v.y + zy * v.z;
	xv.z = xz * v.x + yz * v.y + zz * v.z;
*/
}


//ERROR:
void Mat3::invXform(Vector3D &v) const
{
	Vector3D V(v);
	invXform(V, v);
/*
	double ox, oy;

	ox = v.x; oy= v.y;
	v.x = xx * ox + yx * oy + zx * v.z;
	v.y = xy * ox + yy * oy + zy * v.z;
	v.z = xz * ox + yz * oy + zz * v.z;
*/
}


double Mat3::det() const
{
	return	xx * (yy * zz - yz * zy)
				+ xy * (yz * zx - yx * zz)
				+ xz * (yx * zy - yy * zx);
}


void Mat3::xpose(const Mat3 &M)
{
	xx = M.xx;
	xy = M.yx;
	xz = M.zx;

	yx = M.xy;
	yy = M.yy;
	yz = M.zy;

	zx = M.xz;
	zy = M.yz;
	zz = M.zz;
}


void Mat3::xpose()
{
		Mat3 me(*this);
		xpose(me);
/*	double tmp;

	tmp = xy;
	xy = yx;
	yx = tmp;;

	tmp = yz;
	yz = zy;
	zy = tmp;

	tmp = zx;
	zx = xz;
	xz = tmp;
*/
}

void Mat3::negate(const Mat3 &M)
{
	xx = - M.xx;
	xy = - M.xy;
	xz = - M.xz;

	yx = - M.yx;
	yy = - M.yy;
	yz = - M.yz;

	zx = - M.zx;
	zy = - M.zy;
	zz = - M.zz;
}	


void Mat3::negate()
{
	xx = - xx;
	xy = - xy;
	xz = - xz;

	yx = - yx;
	yy = - yy;
	yz = - yz;

	zx = - zx;
	zy = - zy;
	zz = - zz;
}	


Mat3 Mat3::operator+(const Mat3 &N) const
{
	Mat3 M;
	M.xx = xx + N.xx;
	M.xy = xy + N.xy;
	M.xz = xz + N.xz;

	M.yx = yx + N.yx;
	M.yy = yy + N.yy;
	M.yz = yz + N.yz;

	M.zx = zx + N.zx;
	M.zy = zy + N.zy;
	M.zz = zz + N.zz;

	return M;
}

Mat3 Mat3::operator-(const Mat3 &N) const
{
	Mat3 M;
	M.xx = xx - N.xx;
	M.xy = xy - N.xy;
	M.xz = xz - N.xz;

	M.yx = yx - N.yx;
	M.yy = yy - N.yy;
	M.yz = yz - N.yz;

	M.zx = zx - N.zx;
	M.zy = zy - N.zy;
	M.zz = zz - N.zz;

	return M;
}


void Mat3::operator+=(const Mat3 &M)
{
	xx += M.xx;
	xy += M.xy;
	xz += M.xz;

	yx += M.yx;
	yy += M.yy;
	yz += M.yz;

	zx += M.zx;
	zy += M.zy;
	zz += M.zz;
}


void Mat3::operator-=(const Mat3 &M)
{
	xx -= M.xx;
	xy -= M.xy;
	xz -= M.xz;

	yx -= M.yx;
	yy -= M.yy;
	yz -= M.yz;

	zx -= M.zx;
	zy -= M.zy;
	zz -= M.zz;
}


void Mat3::scale(const Mat3 &M, double s)
{
	xx = s * M.xx; 
	xy = s * M.xy; 
	xz = s * M.xz;
	yx = s * M.yx; 
	yy = s * M.yy; 
	yz = s * M.yz;
	zx = s * M.zx; 
	zy = s * M.zy; 
	zz = s * M.zz;
}


void Mat3::scale(double s)
{
	xx *= s; 
	xy *= s; 
	xz *= s;
	yx *= s; 
	yy *= s; 
	yz *= s;
	zx *= s; 
	zy *= s; 
	zz *= s;
}





struct Quat	
{
	union {
		struct {double s_, x_, y_, z_;};
		double v[4];
	};

	// constructors //////////////////////////////////////////////////////////////


	Quat() : x_(0),y_(0),z_(0),s_(1) {assert(v==&s_);}
	Quat(double s, double x, double y, double z) {assert(v==&s_);set(s, x, y, z);}
	Quat(double angle, const Vector3D &axis)
		{assert(v==&s_);set(angle, axis);}
	Quat(const Mat3 &R) {assert(v==&s_);set(R);}
	Quat(const Quat& q) : s_(q.s_), x_(q.x_), y_(q.y_), z_(q.z_){assert(v==&s_);}

	Quat& operator=(const Quat& q) {
		s_=q.s_, x_=q.x_, y_=q.y_, z_=q.z_;
		return *this;}

	// setters / accessors / translators /////////////////////////////////////////

	double& operator[](const int i) {return v[i];};
	const double& operator[](const int i) const {return v[i];};

	Quat& operator =(const double _v[4]) {
		CopyMemory((void*)v, (void*)_v, sizeof(v));
		return *this;
	}

	void set(double s, double x, double y, double z) {s_=s; x_=x; y_=y; z_=z;}
	// set a quaternion to a rotation of 'angle' radians about 'axis'
	void set(double angle, const Vector3D &axis);
	void set(const Mat3 &R);


	double s() const {return s_;}
	double x() const {return x_;}
	double y() const {return y_;}
	double z() const {return z_;}
	inline Vector3D axis() const;	// normalized axis of rotation
	inline double angle() const;	// angle of rotation in radians, in range [0, 2pi)

	// input / output ////////////////////////////////////////////////////////////
	wistream& read(wistream &is);
	wostream& print(wostream &os) const;

	// operations not returning a Quat ///////////////////////////////////////////

	int operator==(const Quat &other)
		{return s_ == other.s_ && x_ == other.x_ 
				 && y_ == other.y_ && z_ == other.z_;}


	// operations returning result via this //////////////////////////////////////

	// The operation result indicated by the comments is always returned
	// in this.	The symbol [!] indicates that this must be distinct
	// from all of the operands.

	inline void normalize(const Quat &q);							 // q/|q|
	inline void normalize();														// this/|this|
	inline void invert(const Quat &q);									// q^-1
	inline void invert();															 // this^-1
	void mult(const Quat &p, const Quat &q);						// p * q		 [!]
	void premult(const Quat &q);												// q * this	[!]
	void postmult(const Quat &q);											 // this * q	[!]

	// Transforming Vect3s ///////////////////////////////////////////////////////

	void rotate(const Vector3D &u, Vector3D &v) const;		// this (v 0) this^-1 => xv
	void rotate(Vector3D &v) const;										// this (v 0) this^-1 => v

	// These are exactly like the above methods, except the inverse
	// transform is used (i.e. the factors this and this^-1 are swapped).
	void inverseRotate(const Vector3D &v, Vector3D &xv) const;
	void inverseRotate(Vector3D &v) const;


	// miscellaneous /////////////////////////////////////////////////////////////

	// Return the quaternion derivatives in 'this', given a current
	// orientation q and body angular velocity w.	Note that what is
	// returned in 'this' is not a unit quaternion, but the derivative
	// of one!
	inline void deriv(const Quat &q, const Vector3D &w);

	// Quat constants ////////////////////////////////////////////////////////////

	static const Quat ID;	 // identity quaternion
	static const Quat ZERO;	 // identity quaternion
};



Vector3D Quat::axis() const
{
	Vector3D a(x_, y_, z_);
	if (a.norm() == 0.0) a = Vector3D(0,0,1);	// axis is arbitrary here
	else a.normalize();
	return a;
}


void Mat3::set(const Vector3D &axis, double angle)
{
	Quat q;

	q.set(angle, axis);
	set(q);
}

double Quat::angle() const
{
	return 2 * acos(s_);
}


void Quat::normalize(const Quat &q)
{
	double scale;

	scale = 1.0 / sqrt(q.s_*q.s_ + q.x_*q.x_ + q.y_*q.y_ + q.z_*q.z_);
	s_ = scale * q.s_;
	x_ = scale * q.x_;
	y_ = scale * q.y_;
	z_ = scale * q.z_;
}


void Quat::normalize()
{
	double scale;

	scale = 1.0 / sqrt(s_*s_ + x_*x_ + y_*y_ + z_*z_);
	s_ *= scale;
	x_ *= scale;
	y_ *= scale;
	z_ *= scale;
}


//ERROR:
void Quat::invert(const Quat &q)
{
/*
	s_ =	-q.s_;
	x_ =	 q.x_;
	y_ =	 q.y_;
	z_ =	 q.z_;
*/
	s_ =	 q.s_;
	x_ =	-q.x_;
	y_ =	-q.y_;
	z_ =	-q.z_;
	normalize();
}


//ERROR:
void Quat::invert()
{
//	s_ = -s_;
	x_ *=	 -1;
	y_ *=	 -1;
	z_ *=	 -1;

}


//ERROR?:
void Quat::deriv(const Quat &q, const Vector3D &w)
{
/*
	x_ =	0.5 * ( q.s_ * w.x + q.y_ * w.z - q.z_ * w.y);
	y_ =	0.5 * ( q.s_ * w.y + q.z_ * w.x - q.x_ * w.z);
	z_ =	0.5 * ( q.s_ * w.z + q.x_ * w.y - q.y_ * w.x);
	s_ = -0.5 * ( q.x_ * w.x + q.y_ * w.y + q.z_ * w.z);
*/

	x_ =	0.5 * ( w.y * q.z_ - w.z * q.y_ + w.x * q.s_);
	y_ =	0.5 * ( w.z * q.x_ - w.x * q.z_ + w.y * q.s_);
	z_ =	0.5 * ( w.x * q.y_ - w.y * q.x_ + w.z * q.s_);
	s_ = -0.5 * ( w.x * q.x_ + w.y * q.y_ + w.z * q.z_);
}


inline wistream &operator>>(wistream &is, Quat & q) {return q.read(is);}


} // namespace SolveIt

#endif // !defined(AFX_MAT3_H__770D90A1_D038_11D3_A704_0050045B99C4__INCLUDED_)

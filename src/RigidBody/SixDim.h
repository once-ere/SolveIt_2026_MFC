
// SixDim.h : 
#ifndef __SixDim_H_
#define __SixDim_H_

/////////////////////////////////////////////////////////////////////////////

//#include "Vector3D.h"
#include "Mat3.h"
#include "Se3.h"
#include "MatX.h"

namespace SolveIt {
struct Mat6;
struct PAIR_Vector3D;
///////////////////////////////////////////////////////////////////////////////

MatX jointFrame(const Vector3D &loc, const Vector3D &axis_, const Vector3D &ref_);
///////////////////////////////////////////////////////////////////////////////
//	struct RBConfig
///////////////////////////////////////////////////////////////////////////////
struct RBConfig {
	Se3 Tbi;
	Vector3D v;
	Vector3D w;
	RBConfig () : Tbi(Se3::ID), v(ZERO_3D), w(ZERO_3D) {}
	RBConfig (const RBConfig& r) : Tbi(r.Tbi), v(r.v), w(r.w) {}
	RBConfig& operator=(const RBConfig& r) {Tbi=r.Tbi, v=r.v, w=r.w;return *this;}
	RBConfig (const Se3& Tbi_, const Vector3D& v_, const Vector3D& w_) : Tbi(Tbi_), v(v_), w(w_) {}
};
///////////////////////////////////////////////////////////////////////////////
union Component1DOF{
	double trans;
	double angle;
};
///////////////////////////////////////////////////////////////////////////////
struct Mat6;
///////////////////////////////////////////////////////////////////////////////
struct PAIR_Vector3D//:pair<Vector3D, Vector3D>
{
	Vector3D a, b;
	PAIR_Vector3D () {}
	PAIR_Vector3D (const PAIR_Vector3D& r) : a(r.a), b(r.b) {}
	PAIR_Vector3D& operator=(const PAIR_Vector3D& r) {a=r.a, b=r.b;return *this;}
	PAIR_Vector3D (const Vector3D& v, const Vector3D& w) : a(v), b(w) {}

	double dot (const PAIR_Vector3D& s) const {return a*s.a+b*s.b;}
	double operator * (const PAIR_Vector3D &s) const {return dot(s);}

	double dot2 (const PAIR_Vector3D& s) const {return a*s.b+b*s.a;}
	double operator ^ (const PAIR_Vector3D &s) const {return dot2(s);}

	inline PAIR_Vector3D operator*(const Mat6&m) const;
	inline PAIR_Vector3D operator^(const Mat6&m) const;

	PAIR_Vector3D&	operator +=(const PAIR_Vector3D &s) {
		a+=s.a, b+=s.b;
		return *this;
	}

	PAIR_Vector3D&	operator -=(const PAIR_Vector3D &s) {
		a-=s.a, b-=s.b;
		return *this;
	}

	PAIR_Vector3D	operator + (const PAIR_Vector3D &s) const {
		return PAIR_Vector3D(a+s.a, b+s.b);
	}

	PAIR_Vector3D	operator - (const PAIR_Vector3D &s) const {
		return PAIR_Vector3D(a-s.a, b-s.b);
	}
//xpose
	PAIR_Vector3D	xpose() const {
		return PAIR_Vector3D(b, a);
	}

	void negate() {a*=-1.0;b*=-1.0;}

	friend PAIR_Vector3D	operator * (const PAIR_Vector3D &m, const double &s) {
		return PAIR_Vector3D(s*m.a, s*m.b);
	}
	friend PAIR_Vector3D	operator * (const double &s, const PAIR_Vector3D &m) {
		return PAIR_Vector3D(s*m.a, s*m.b);
	}
	static const PAIR_Vector3D ZERO;
};
typedef PAIR_Vector3D Vect6;
///////////////////////////////////////////////////////////////////////////////
struct Mat6//:pair<Vector3D, Vector3D>
{
	Mat3 m11, m12, m21, m22;
	Mat6 ()	{}// : m11(Mat3::ID), m22(Mat3::ID)
	Mat6 (const Mat6& r) : m11(r.m11), m12(r.m12) , m21(r.m21) , m22(r.m22) {}
	Mat6& operator=(const Mat6& r) {m11=r.m11, m12=r.m12 , m21=r.m21 , m22=r.m22;return *this;}
	Mat6 (const Mat3& M11, const Mat3& M12, const Mat3& M21, const Mat3& M22) : m11(M11), m12(M12) , m21(M21) , m22(M22) {}
	Mat6 (const Mat3& M12, const Mat3& M21) : m12(M12) , m21(M21) {}
//tensor product:
	Mat6 (const PAIR_Vector3D& v1, const PAIR_Vector3D& v2) {
		Mat3 M11(v1.a, v2.a);
		Mat3 M12(v1.a, v2.b);
		Mat3 M21(v1.b, v2.a);
		Mat3 M22(v1.b, v2.b);
		*this = Mat6(M11, M12, M21, M22);
	}


	Mat6&	operator +=(const Mat6 &s) {
		m11+=s.m11, m12+=s.m12, m21+=s.m21, m22+=s.m22;
		return *this;
	}

	Mat6&	operator -=(const Mat6 &s) {
		m11-=s.m11, m12-=s.m12, m21-=s.m21, m22-=s.m22;
		return *this;
	}

	PAIR_Vector3D	operator * (const PAIR_Vector3D &s) const {
		return PAIR_Vector3D(m11*s.a+m12*s.b, m21*s.a+m22*s.b);
	}

	Mat6	operator * (const Mat6 &s) const {
		return Mat6(m11*s.m11+m12*s.m21, m11*s.m12+m12*s.m22, m21*s.m11+m22*s.m21, m21*s.m12+m22*s.m22);
	}
	Mat6	operator + (const Mat6 &s) const {
		return Mat6(m11+s.m11, m12+s.m12, m21+s.m21, m22+s.m22);
	}
	Mat6	operator - (const Mat6 &s) const {
		return Mat6(m11-s.m11, m12-s.m12, m21-s.m21, m22-s.m22);
	}
//	Mat6	operator * (const double &s) const {
//		Mat3 tm11, tm12, tm21, tm22;
//		tm11.scale(m11, s);tm12.scale(m12, s);tm21.scale(m21, s);tm22.scale(m22, s);
//		return Mat6(tm11, tm21, tm12, tm22);
//	}
	friend Mat6	operator * (const Mat6 &m, const double &s) {
		Mat3 tm11, tm12, tm21, tm22;
		tm11.scale(m.m11, s);tm12.scale(m.m12, s);tm21.scale(m.m21, s);tm22.scale(m.m22, s);
		return Mat6(tm11, tm12, tm21, tm22);
	}
	friend Mat6	operator * (const double &s, const Mat6 &m) {
		Mat3 tm11, tm12, tm21, tm22;
		tm11.scale(m.m11, s);tm12.scale(m.m12, s);tm21.scale(m.m21, s);tm22.scale(m.m22, s);
		return Mat6(tm11, tm12, tm21, tm22);
	}
//xpose
	Mat6	xpose () const {
		Mat3 tm11=~m11, tm12=~m12, tm21=~m21, tm22=~m22;
		return Mat6(tm11, tm21, tm12, tm22);
	}

	Mat6 operator ~() const {
		return xpose ();
	}

//invert
//	Mat6	operator ! () const {
//	//	Mat3 im11, im12, im21, im22;
//	//	im11.invert(m11);im12.invert(m12);im21.invert(m21);im22.invert(m22);
//		TNT::Matrix<double> a(6, 6);
//		return ZERO;
//	}

// invert spatial articulated inertia matrix

// The inversion method is based on partitioning (see Numerical
// Recipes, p. 77), but differs in two respects.	First, we avoid
// inverting the op-left and bottom-right 3 x 3 submatrices since
// these are often singular.	Second, we exploit the symmetries in the
// 4 3x3 components of the inertia matrix I, namely A^T = D, B^T = B,
// and C^T = C.	See my notes for details.	N.B.	This is not a
// general 6 x 6 matrix inversion routine!

// This routine is only needed for mulibodies which are floating,
// i.e. not anchored to an inertial frame via joint.

void inertiaInvert(const Mat6 &I)
{
	Mat3 Binv, M, N;

	Binv.invert(I.m12);
	Binv.negate();
	M = I.m22 * Binv;
	N = M * I.m11;
	N += I.m21;
	m12.invert(N);

	M = m12 * I.m22;
	m11 = M * Binv;
	
	m22 = ~m11;

	M = I.m11 * m11;
//	M.xrow().x -= 1.0; M.yrow().y -= 1.0; M.zrow().z -= 1.0;
	M.m[0][0] -= 1.0; M.m[1][1] -= 1.0; M.m[2][2] -= 1.0;
	m21 = Binv * M;
}
///////////////////////////////////////////////////////////////////////////////
// xform a spatial vector by a spatial matrix
void	xform(const PAIR_Vector3D &u, PAIR_Vector3D &v) const
{
v.a=m11*u.a+m12*u.b;
v.b=m21*u.a+m22*u.b;
//	Vector3D tmp;
//	m11.xform(u.a, v.a);
//	m12.xform(u.b, tmp);
//	v.a.add(tmp);

//	m21.xform(u.a, v.b);
//	m22.xform(u.b, tmp);
//	v.b.add(tmp);
}


// Mat6 constants ////////////////////////////////////////////////////////////
	static const Mat6 G;		// metric
	static const Mat6 ZERO;		// zero matrix
	static const Mat6 ID;		// identity matrix
};
///////////////////////////////////////////////////////////////////////////////
//const Mat6 Mat6::G(Mat3::ZERO, Mat3::ID, Mat3::ID, Mat3::ZERO);
//const Mat6 Mat6::ZERO();
//const Mat6 Mat6::ID(Mat3::ID, Mat3::ZERO, Mat3::ZERO, Mat3::ID);
PAIR_Vector3D PAIR_Vector3D::operator*(const Mat6&m) const {
	Mat6 M(m);
	M.xpose();
	return PAIR_Vector3D(M*(*this));
//	return PAIR_Vector3D(M*xpose());
//	return PAIR_Vector3D((M*Mat6::G)**this);
}
///////////////////////////////////////////////////////////////////////////////
PAIR_Vector3D PAIR_Vector3D::operator^(const Mat6&m) const {
	Mat6 M(m);
	M.xpose();
	return PAIR_Vector3D(M*xpose());
//	return PAIR_Vector3D((M*Mat6::G)**this);
}
typedef Mat6 SpatialMat;
typedef PAIR_Vector3D SpatialVect;


} // namespace SolveIt

///////////////////////////////////////////////////////////////////////////////
#endif

// Mat3.cpp: implementation of the Mat3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector3D.h"
//#include "Quat.h"
#include "Mat3.h"
//#include "Se3.h"
//#include "State.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


const Mat3 Mat3::ZERO (ZERO_3D);
const Mat3 Mat3::ID   (1, 1, 1);


void Mat3::set(const Quat &q)
{
  xx = 2.0 * (q.s_ * q.s_ + q.x_ * q.x_ - 0.5);
  yy = 2.0 * (q.s_ * q.s_ + q.y_ * q.y_ - 0.5);
  zz = 2.0 * (q.s_ * q.s_ + q.z_ * q.z_ - 0.5);

  xy = 2.0 * (q.y_ * q.x_ - q.z_ * q.s_);
  yx = 2.0 * (q.x_ * q.y_ + q.z_ * q.s_);


  yz = 2.0 * (q.z_ * q.y_ - q.x_ * q.s_);
  zy = 2.0 * (q.y_ * q.z_ + q.x_ * q.s_);

  zx = 2.0 * (q.x_ * q.z_ - q.y_ * q.s_);
  xz = 2.0 * (q.z_ * q.x_ + q.y_ * q.s_);
}


wostream& Mat3::print(wostream &os) const
{
  int oldFlags = os.setf(ios::showpos);
  os << '[' << xx << ' ' << xy << ' ' << xz << ']' << endl;
  os << '[' << yx << ' ' << yy << ' ' << yz << ']' << endl;
  os << '[' << zx << ' ' << zy << ' ' << zz << ']' << endl;
  os.flags(oldFlags);
  return os;
}


int Mat3::invert(const Mat3 &M)
{
  double D, oneOverDet;

  if (fabs(D = M.det()) < 1.0e-12) return 1; // not invertible
  oneOverDet = 1 / D;

  xx = (M.yy * M.zz - M.yz * M.zy) * oneOverDet;
  xy = (M.xz * M.zy - M.xy * M.zz) * oneOverDet;
  xz = (M.xy * M.yz - M.xz * M.yy) * oneOverDet;
  yx = (M.yz * M.zx - M.yx * M.zz) * oneOverDet;
  yy = (M.xx * M.zz - M.xz * M.zx) * oneOverDet;
  yz = (M.xz * M.yx - M.xx * M.yz) * oneOverDet;
  zx = (M.yx * M.zy - M.yy * M.zx) * oneOverDet;
  zy = (M.xy * M.zx - M.xx * M.zy) * oneOverDet;
  zz = (M.xx * M.yy - M.xy * M.yx) * oneOverDet;
  return 0;
}


int Mat3::invert()
{
	Mat3 me(*this);
	return invert(me);
/*
  double D, oneOverDet;
  double oxy, oyz, ozx, oyx, ozy, oxz;

  if (fabs(D = det()) < 1.0e-12) return 1; // not invertible
  oneOverDet = 1 / D;

  oxy = xy; oyx = yx;
  oyz = yz; ozy = zy;
  ozx = zx; oxz = xz;

  xy = (oxz * ozy - zz * oxy) * oneOverDet;
  yz = (oxz * oyx - xx * oyz) * oneOverDet;
  zx = (oyx * ozy - yy * ozx) * oneOverDet;
  yx = (oyz * ozx - oyx * zz) * oneOverDet;
  zy = (oxy * ozx - ozy * xx) * oneOverDet;
  xz = (oxy * oyz - oxz * yy) * oneOverDet;
  xx = (yy * zz - oyz * ozy) * oneOverDet;
  yy = (xx * zz - oxz * ozx) * oneOverDet;
  zz = (xx * yy - oxy * oyx) * oneOverDet;
  return 0;
*/
}

Mat3 Mat3::operator*(const Mat3 &N) const
{
	Mat3 M;
  M.xx = xx * N.xx + xy * N.yx + xz * N.zx;
  M.xy = xx * N.xy + xy * N.yy + xz * N.zy;
  M.xz = xx * N.xz + xy * N.yz + xz * N.zz;
  M.yx = yx * N.xx + yy * N.yx + yz * N.zx;
  M.yy = yx * N.xy + yy * N.yy + yz * N.zy;
  M.yz = yx * N.xz + yy * N.yz + yz * N.zz;
  M.zx = zx * N.xx + zy * N.yx + zz * N.zx;
  M.zy = zx * N.xy + zy * N.yy + zz * N.zy;
  M.zz = zx * N.xz + zy * N.yz + zz * N.zz;
	return M;
}


void Mat3::premult(const Mat3 &M)
{
		Mat3 me(*this);
		Mat3 m = M*me;
		operator=(m);
//		Mat3 m = M.operator*(me);
/*
  double oxy, oyz, ozx, oyx, ozy, oxz;

  oxy = xy; oyx = yx; oyz = yz; ozy = zy; ozx = zx; oxz = xz;

  xy = M.xx * oxy + M.xy * yy  + M.xz * ozy;
  xz = M.xx * oxz + M.xy * oyz + M.xz * zz;
  yx = M.yx * xx  + M.yy * oyx + M.yz * ozx;
  yz = M.yx * oxz + M.yy * oyz + M.yz * zz;
  zx = M.zx * xx  + M.zy * oyx + M.zz * ozx;
  zy = M.zx * oxy + M.zy * yy  + M.zz * ozy;

  xx = M.xx * xx  + M.xy * oyx + M.xz * ozx;
  yy = M.yx * oxy + M.yy * yy  + M.yz * ozy;
  zz = M.zx * oxz + M.zy * oyz + M.zz * zz;
*/
}


void Mat3::postmult(const Mat3 &M)
{
		Mat3 me(*this);
		*this = me*M;
/*
  double oxy, oyz, ozx, oyx, ozy, oxz;

  oxy = xy; oyx = yx; oyz = yz; ozy = zy; ozx = zx; oxz = xz;

  xy = xx *  M.xy + oxy * M.yy + oxz * M.zy;
  xz = xx *  M.xz + oxy * M.yz + oxz * M.zz;
  yx = oyx * M.xx + yy  * M.yx + oyz * M.zx;
  yz = oyx * M.xz + yy  * M.yz + oyz * M.zz;
  zx = ozx * M.xx + ozy * M.yx + zz  * M.zx;
  zy = ozx * M.xy + ozy * M.yy + zz  * M.zy;

  xx = xx  * M.xx + oxy * M.yx + oxz * M.zx;
  yy = oyx * M.xy + yy  * M.yy + oyz * M.zy;
  zz = ozx * M.xz + ozy * M.yz + zz  * M.zz;
*/
}
///////////////////////////////////////////////////////////////////////////////



const Quat Quat::ID   (1, 0, 0, 0);
const Quat Quat::ZERO   (0, 0, 0, 0);



void Quat::set(double angle, const Vector3D &axis)
{
  Vector3D axis0(axis);
  double theta, sine;

  theta = 0.5 * angle;
  sine = sin(theta);
  s_ = cos(theta);

    axis0.normalize();
    x_ = axis0.x * sine;
    y_ = axis0.y * sine;
    z_ = axis0.z * sine;

}

  
void Quat::set(const Mat3 &R)
{
  double qs2, qx2, qy2, qz2;  // squared magniudes of quaternion components
  double tmp;
  int i;

  // first compute squared magnitudes of quaternion components - at least one
  // will be greater than 0 since quaternion is unit magnitude

  qs2 = 0.25 * (R.xx + R.yy + R.zz + 1);
  qx2 = qs2 - 0.5 * (R.yy + R.zz);
  qy2 = qs2 - 0.5 * (R.zz + R.xx);
  qz2 = qs2 - 0.5 * (R.xx + R.yy);

  
  // find maximum magnitude component
  i = (qs2 > qx2 ) ?
    ((qs2 > qy2) ? ((qs2 > qz2) ? 0 : 3) : ((qy2 > qz2) ? 2 : 3)) :
    ((qx2 > qy2) ? ((qx2 > qz2) ? 1 : 3) : ((qy2 > qz2) ? 2 : 3));

  // compute signed quaternion components using numerically stable method
  switch(i) {
  case 0:
    s_ = sqrt(qs2);
    tmp = 0.25 / s_;
    x_ = (R.zy - R.yz) * tmp;
    y_ = (R.xz - R.zx) * tmp;
    z_ = (R.yx - R.xy) * tmp;
    break;
  case 1:
    x_ = sqrt(qx2);
    tmp = 0.25 / x_;
    s_ = (R.zy - R.yz) * tmp;
    y_ = (R.xy + R.yx) * tmp;
    z_ = (R.xz + R.zx) * tmp;
    break;
  case 2:
    y_ = sqrt(qy2);
    tmp = 0.25 / y_;
    s_ = (R.xz - R.zx) * tmp;
    z_ = (R.yz + R.zy) * tmp;
    x_ = (R.yx + R.xy) * tmp;
    break;
  case 3:
    z_ = sqrt(qz2);
    tmp = 0.25 / z_;
    s_ = (R.yx - R.xy) * tmp;
    x_ = (R.zx + R.xz) * tmp;
    y_ = (R.zy + R.yz) * tmp;
    break;
  }
  // for consistency, force positive scalar component [ (s; v) = (-s; -v) ]
  if (s_ < 0) {
    s_ = -s_;
    x_ = -x_;
    y_ = -y_;
    z_ = -z_;
  }
  // normalize, just to be safe
  tmp = 1.0 / sqrt(s_*s_ + x_*x_ + y_*y_ + z_*z_);
  s_ *= tmp;
  x_ *= tmp;
  y_ *= tmp;
  z_ *= tmp;
}


wostream& Quat::print(wostream &os) const
{
  int oldFlags = os.setf(ios::showpos);
  os << '(' << s_ << ' ' << x_ << ' ' << y_ << ' ' << z_ << ')';
  os.flags(oldFlags);
  return os;
}


void Quat::mult(const Quat &p, const Quat &q)
{
  s_ = p.s_ * q.s_ - (p.x_ * q.x_ + p.y_ * q.y_ + p.z_ * q.z_);
  x_ = p.s_ * q.x_ +  q.s_ * p.x_ + p.y_ * q.z_ - p.z_ * q.y_;
  y_ = p.s_ * q.y_ +  q.s_ * p.y_ + p.z_ * q.x_ - p.x_ * q.z_;
  z_ = p.s_ * q.z_ +  q.s_ * p.z_ + p.x_ * q.y_ - p.y_ * q.x_;
}


void Quat::premult(const Quat &q)
{
  double ox, oy, oz;

  ox = x_; oy = y_; oz = z_;

  x_ = q.s_ * ox +  s_ * q.x_ + q.y_ * oz - q.z_ * oy;
  y_ = q.s_ * oy +  s_ * q.y_ + q.z_ * ox - q.x_ * oz;
  z_ = q.s_ * oz +  s_ * q.z_ + q.x_ * oy - q.y_ * ox;
  s_ = q.s_ * s_ - (q.x_ * ox + q.y_ * oy + q.z_ * oz);
}


void Quat::postmult(const Quat &q)
{
  double ox, oy, oz;

  ox = x_; oy = y_; oz = z_;

  x_ = s_ * q.x_ +  ox * q.s_ + oy * q.z_ - oz * q.y_;
  y_ = s_ * q.y_ +  oy * q.s_ + oz * q.x_ - ox * q.z_;
  z_ = s_ * q.z_ +  oz * q.s_ + ox * q.y_ - oy * q.x_;
  s_ = s_ * q.s_ - (ox * q.x_ + oy * q.y_ + oz * q.z_);
}


// The Quat transformation routines use 19 multiplies and 12 adds
// (counting the multiplications by 2.0).  See Eqn (20) of "A
// Comparison of Transforms and Quaternions in Robotics," Funda and
// Paul, Proceedings of International Conference on Robotics and
// Automation, 1988, p. 886-991.

void Quat::rotate(const Vector3D &v, Vector3D &xv) const
{
  Vector3D u, uv, uuv;
  u = ( double *) &x_;
  uv = u .vectorProd(v) ;
  uuv = u .vectorProd(uv) ;
  uv *= 2.0 * s_;
  uuv*= 2.0;
  xv = v + uv;
  xv +=uuv;
}

void Quat::rotate(Vector3D &v) const
{
  Vector3D u, uv, uuv;
  u = ( double *) &x_;
  uv = u .vectorProd(v) ;
  uuv = u .vectorProd(uv) ;
  uv *= (2.0 * s_);
  uuv *= 2.0;
  v+=uv;
  v+=uuv;
}

//ERROR:
void Quat::inverseRotate(const Vector3D &v, Vector3D &xv) const
{
	Quat Q;
	Q.invert(*this);
	Q.rotate(v,xv);
/*
  Vector3D u, uv, uuv;
  
  u = ( double *) &x_;
  uv.cross(u, v);
  uuv.cross(u, uv);
  uv.scale(-2.0 * s_);
  uuv.scale(2.0);
  xv.add(v, uv);
  xv.add(uuv);
*/
}

//ERROR:wistream
void Quat::inverseRotate(Vector3D &v) const
{
	Quat Q;
	Q.invert(*this);
	Q.rotate(v);
/*
  Vector3D u, uv, uuv;
  u = ( double *) &x_;
  uv.cross(u, v);
  uuv.cross(u, uv);
  uv.scale(-2.0 * s_);
  uuv.scale(2.0);
  v.add(uv);
  v.add(uuv);
*/
}


#define STR_LENGTH 1024


wistream& Quat::read(wistream &is)
{
	double x;
	Vector3D vect;
	*this = Quat::ID;

	if (!((is >> x >> vect).fail()))
	{
		x *= DEG_TO_RAD;
		set(x, vect);
		normalize();	// just to be sure.
	}
	return is;
}


//const State State::Unit_State(0, Point3D(0,0,0), Quat::ID, Vector3D::ZERO,Vector3D::ZERO);
//const State State::Zero_State(0, Point3D(0,0,0), Quat::ZERO, Vector3D::ZERO,Vector3D::ZERO);


#undef STR_LENGTH


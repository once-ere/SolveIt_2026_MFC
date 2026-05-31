//Tension.cpp
#include "stdafx.h"
#include "RigidBody.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "System.h"
//Connection on right:
//#include "Tension1.cpp"
//#include "Tension3.cpp"
//#include "Tension1.c"
//#include "Tension3.c"
//Connection on left:
//#include "Tension2.cpp"
//#include "Tension4.cpp"
//#include "Tension2.c"
//#include "Tension4.c"
double TensionsLeft11(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);
double TensionsLeft12(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);
double TensionsLeft21(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);
double TensionsLeft22(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);

double TensionsRight11(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);

double TensionsRight12(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);

double TensionsRight21(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);

double TensionsRight22(double R, double g, double Inertia,
					 double m1, double x1, double y1, double v1x, double v1y, double w1,
					 double m2, double x2, double y2, double v2x, double v2y, double w2);

/*
_Tension2 TensionsRight1, TensionsRight2, TensionsLeft1, TensionsLeft2;
*/

#define Power(a,b) pow((a),(b))
#define Sqrt(a) sqrt((a))

double _Cos1(double x, double y, double R) {
	double r2=x*x+y*y;
	double r2R2 = Sqrt(r2 - R*R);
	return (R*x-y*r2R2)/r2;
}
double _Sin1(double x, double y, double R) {
	double r2=x*x+y*y;
	double r2R2 = Sqrt(r2 - R*R);
	return (R*y+x*r2R2)/r2;
}

double _Cos2(double x, double y, double R) {
	double r2=x*x+y*y;
	double r2R2 = Sqrt(r2 - R*R);
	return (R*x+y*r2R2)/r2;
}
double _Sin2(double x, double y, double R) {
	double r2=x*x+y*y;
	double r2R2 = Sqrt(r2 - R*R);
	return (R*y-x*r2R2)/r2;
}
///////////////////////////////////////////////////////////////////////////////

//Connection on right:
double TensionRight1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w);
double TensionRight2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w);
//Connection on left:
double TensionLeft1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w);
double TensionLeft2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//typedef double (*_Tension)(double X, double Y, double vx, double vy, double R, double g);
///////////////////////////////////////////////////////////////////////////////
struct SelectAngle {
	double s;
	double L;
	double theta;
	_Tension f;
	SelectAngle(double _s, double _L, double _theta, _Tension _f) : s(_s), L(_L), theta(_theta), f(_f) {}
};
///////////////////////////////////////////////////////////////////////////////
struct SelectAngleCompare
{  
	bool operator()(const SelectAngle* a, const SelectAngle* b) const
	{
		return a->s < b->s;
	}
};
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
// -PI <= theta <= PI
//Connection on right:
// 0 <= theta <= 2 PI
void CRigidBody::FirstPossibleAngle(double& Cos, double& Sin, double& theta) {
	double &R=m_fRadiusOfPulley, &X=x.x,&Y=x.y;
	double R2=R*R;
	double r2=X*X+Y*Y;
	double r2R2 = Sqrt(r2 - R2);

	Cos = (R*X-Y*r2R2)/r2;
	Sin = (R*Y+X*r2R2)/r2;

	double theta1 = acos(Cos);// 0 <= theta1 <= PI
	double theta2 = asin(Sin);// -PI/2 <= theta2 <= PI/2

	if (Sin < 0.0 && Cos < 0.0) {
		theta1 *= -1;
		theta2 = PI - theta2 - 2*PI;
	}
	else if (Cos < 0.0) {
		theta2 = PI - theta2;
	}
	else if (Sin < 0.0) {
		theta1 *= -1;
	}
//	ATLASSERT(theta1 == theta2);
	theta = theta2;
	if ((m_nConnectedInQuadrant==___Quadrant_Two || m_nConnectedInQuadrant==___Quadrant_Three) && theta < 0.0)
		theta += 2*PI;
}
void CRigidBody::SecondPossibleAngle(double& Cos, double& Sin, double& theta) {
	double &R=m_fRadiusOfPulley, &X=x.x,&Y=x.y;
	double R2=R*R;
	double r2=X*X+Y*Y;
	double r2R2 = Sqrt(r2 - R2);

	Cos = (R*X+Y*r2R2)/r2;
	Sin = (R*Y-X*r2R2)/r2;

	double theta1 = acos(Cos); 
	double theta2 = asin(Sin); 

	if (Sin < 0.0 && Cos < 0.0) {
		theta1 *= -1;
		theta2 = PI - theta2 - 2*PI;
	}
	else if (Cos < 0.0) {
		theta2 = PI - theta2;
	}
	else if (Sin < 0.0) {
		theta1 *= -1;
	}
//	ATLASSERT(theta1 == theta2);
	theta = theta2;
	if ((m_nConnectedInQuadrant==___Quadrant_Two || m_nConnectedInQuadrant==___Quadrant_Three) && theta < 0.0)
		theta += 2*PI;
}
///////////////////////////////////////////////////////////////////////////////

void CRigidBody::InitializePulleyConnection(double radiusOfPulley, long nConnectedInQuadrant)
{
	m_fRadiusOfPulley		= radiusOfPulley;
	ASSERT(m_nConnectedInQuadrant	== (InitialAngleInQuadrant) nConnectedInQuadrant);

	double	&length=m_fDistanceToFixedPointOnPulley,
			&R=m_fRadiusOfPulley,
			&X=x.x,&Y=x.y;

	if (X*X+Y*Y < R*R) return;

	double Cos1 = 0, Sin1 = 0, Cos2 = 0, Sin2 = 0;
	double theta1 = 0 , theta2 = 0;
	double Cos = 0, Sin = 0, theta = 0;

	FirstPossibleAngle( Cos1, Sin1, theta1);
	SecondPossibleAngle( Cos2, Sin2, theta2);

	switch (m_nConnectedInQuadrant)
	{
	case ___Quadrant_One:
		if (Sin1 >= 0.0 && Cos1 >= 0.0) {
			Cos = Cos1, Sin = Sin1, theta = theta1;
			m_Tension = TensionRight1;
			m_Cos = _Cos1, m_Sin = _Sin1;
		}
		if (Sin2 >= 0.0 && Cos2 >= 0.0) {
			Cos = Cos2, Sin = Sin2, theta = theta2;
			m_Tension = TensionRight2;
			m_Cos = _Cos2, m_Sin = _Sin2;
		}
		break;
	case ___Quadrant_Two:
		if (Sin1 >= 0.0 && Cos1 <= 0.0) {
			Cos = Cos1, Sin = Sin1, theta = theta1;
			m_Tension = TensionLeft1;
			m_Cos = _Cos1, m_Sin = _Sin1;
		}
		if (Sin2 >= 0.0 && Cos2 <= 0.0) {
			Cos = Cos2, Sin = Sin2, theta = theta2;
			m_Tension = TensionLeft2;
			m_Cos = _Cos2, m_Sin = _Sin2;
		}
		break;
	case ___Quadrant_Three:
		if (Sin1 <= 0.0 && Cos1 <= 0.0) {
			Cos = Cos1, Sin = Sin1, theta = theta1;
			m_Tension = TensionLeft1;
			m_Cos = _Cos1, m_Sin = _Sin1;
		}
		if (Sin2 <= 0.0 && Cos2 <= 0.0) {
			Cos = Cos2, Sin = Sin2, theta = theta2;
			m_Tension = TensionLeft2;
			m_Cos = _Cos2, m_Sin = _Sin2;
		}
		break;
	case ___Quadrant_Four:
		if (Sin1 <= 0.0 && Cos1 >= 0.0) {
			Cos = Cos1, Sin = Sin1, theta = theta1;
			m_Tension = TensionRight1;
			m_Cos = _Cos1, m_Sin = _Sin1;
		}
		if (Sin2 <= 0.0 && Cos2 >= 0.0) {
			Cos = Cos2, Sin = Sin2, theta = theta2;
			m_Tension = TensionRight2;
			m_Cos = _Cos2, m_Sin = _Sin2;
		}
		break;
	default:
		throw SolveIt_ERROR(L"-1");
		return;
	}


	double dx = R*Cos - X; 
	double dy = R*Sin - Y; 

	if (m_nConnectedInQuadrant==___Quadrant_One || m_nConnectedInQuadrant==___Quadrant_Four)
		m_fDistanceToFixedPointOnPulley = R*(PI_DIVIDED_BY_2 - theta) + sqrt(dx*dx+dy*dy);
	if (m_nConnectedInQuadrant==___Quadrant_Two || m_nConnectedInQuadrant==___Quadrant_Three)
		m_fDistanceToFixedPointOnPulley = -R*(PI_DIVIDED_BY_2 - theta) + sqrt(dx*dx+dy*dy);

	m_fAngleOfInitialFixedPointOnPulley	= theta;
	m_fAngleOfLastFixedPointOnPulley		= theta;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::SetPulleyContactPoint(const double& R)
{
	double Cos = (*m_Cos)(x.x, x.y, R);
	double Sin = (*m_Sin)(x.x, x.y, R);
	double dx = R*Cos;// - X;
	double dy = R*Sin;// - Y;
	m_PulleyContactPoint = Point3D(dx,dy,0);
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBody::Tension(double g, double R, double InertiaInv, double w)
{
	if (!m_Tension) return ZERO_3D;
	double	&length=m_fDistanceToFixedPointOnPulley,
//			&R=m_fRadiusOfPulley,
			&X=x.x,&Y=x.y;

	if (X*X+Y*Y < R*R) return ZERO_3D;

	Vector3D v;
	get_Velocity(v);
	double& vx=v.x, &vy=v.y;

#if 0
	double Cos1 = 0, Sin1 = 0, Cos2 = 0, Sin2 = 0;
	double theta1 = 0 , theta2 = 0;

	FirstPossibleAngle(  Cos1, Sin1, theta1);
	SecondPossibleAngle( Cos2, Sin2, theta2);

/*
	if (L.z < 0.0 && theta1 > m_angleOfLastFixedPointOnPulley)
		while (theta1 > m_angleOfLastFixedPointOnPulley) theta1 -= 2*PI;
	if (L.z < 0.0 && theta2 > m_angleOfLastFixedPointOnPulley)
		while (theta2 > m_angleOfLastFixedPointOnPulley) theta2 -= 2*PI;

	if (L.z > 0.0 && theta1 < m_angleOfLastFixedPointOnPulley)
		while (theta1 < m_angleOfLastFixedPointOnPulley) theta1 += 2*PI;
	if (L.z > 0.0 && theta2 < m_angleOfLastFixedPointOnPulley)
		while (theta2 < m_angleOfLastFixedPointOnPulley) theta2 += 2*PI;
*/

	double dx1 = R*Cos1 - X; 
	double dy1 = R*Sin1 - Y; 

	double dx2 = R*Cos2 - X;
	double dy2 = R*Sin2 - Y;

//#define SQR(X) ((X)*(X))
#define LENGTH(X) (R*(PI_DIVIDED_BY_2 - theta##X) +sqrt(SQR(dx##X)+SQR(dy##X)) )

	double L1 = LENGTH(1); 
	double L4 = LENGTH(2); 
#undef LENGTH
#define LENGTH(X) ( -R*(PI_DIVIDED_BY_2 - theta##X) +sqrt(SQR(dx##X)+SQR(dy##X)) )
	double L2 = LENGTH(1); 
	double L3 = LENGTH(2); 

	vector<SelectAngle*> vec;

	switch (m_nConnectedInQuadrant)
	{
	case One:
	case Four:
		{
			SelectAngle a1(fabs(L1 - length), L1, theta1, TensionRight1);
			SelectAngle a4(fabs(L4 - length), L4, theta2, TensionRight2);
			vec.push_back(&a1);vec.push_back(&a4);
		}
		break;
	case Two:
	case Three:
		{
			SelectAngle a2(fabs(L2 - length), L2, theta1, TensionLeft1);
			SelectAngle a3(fabs(L3 - length), L3, theta2, TensionLeft2);
			vec.push_back(&a2);vec.push_back(&a3);
		}
		break;
	default: return ZERO_3D;
	}

	SelectAngleCompare selectAngleCompare;
	sort(vec.begin(),vec.end(), selectAngleCompare);
	m_angleOfLastFixedPointOnPulley = vec.front()->theta;
	m_PulleyContactPoint = Point3D(R*cos(m_angleOfLastFixedPointOnPulley),R*sin(m_angleOfLastFixedPointOnPulley),0);
	_Tension f = vec.front()->f;
//	return (*f)(X, Y, vx, vy, R, g);

	double _tension = (*f)(X, Y, vx, vy, R, g);

	double dx = R*cos(m_angleOfLastFixedPointOnPulley) - X;
	double dy = R*sin(m_angleOfLastFixedPointOnPulley) - Y;
	double L = vec.front()->L; 

	Vector3D tension(dx/L,dy/L,0);
	tension *= _tension;//fabs();
	return tension;

#endif

	double _tension = (*m_Tension)(X, Y, vx, vy, R, g, InertiaInv, w);

	double Cos = (*m_Cos)(X, Y, R);
	double Sin = (*m_Sin)(X, Y, R);
	double dx = R*Cos;// - X;
	double dy = R*Sin;// - Y;
	m_PulleyContactPoint = Point3D(dx,dy,0);
	dx -= X;
	dy -= Y;

	double theta1 = acos(Cos);// 0 <= theta1 <= PI
	double theta = asin(Sin);// -PI/2 <= theta2 <= PI/2

	if (Sin < 0.0 && Cos < 0.0) {
		theta1 *= -1;
		theta = PI - theta - 2*PI;
	}
	else if (Cos < 0.0) {
		theta = PI - theta;
	}
	else if (Sin < 0.0) {
		theta1 *= -1;
	}
	double L = sqrt(dx*dx+dy*dy); 
	if (m_nConnectedInQuadrant==___Quadrant_One || m_nConnectedInQuadrant==___Quadrant_Four)
		m_fDistanceToFixedPointOnPulley += R*(PI_DIVIDED_BY_2 - theta);
	if (m_nConnectedInQuadrant==___Quadrant_Two || m_nConnectedInQuadrant==___Quadrant_Three)
		m_fDistanceToFixedPointOnPulley -= R*(PI_DIVIDED_BY_2 - theta);

	if (_tension<= 0.0) return ZERO_3D;//wstring, not stiff wire
	Vector3D tension(dx/L,dy/L,0);
	tension *= _tension;//fabs();
	return tension;

}


#undef LENGTH
//#undef SQR
#undef Power
#undef Sqrt

//Tension.cpp
#include "stdafx.h"

#pragma optimize( "", off )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define Power(a,b) pow(fabs(a),(b))
#define _Sqrt(a) sqrt(fabs(a))


///////////////////////////////////////////////////////////////////////////////

//Connection on right:
double TensionRight1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=_Sqrt(x*x + y*y);
double tension = 
(Power(r,4)*(Power(r,6)*(Power(vx,2) + Power(vy,2) + g*y) + 
      Power(r,4)*R*(-2*R*(Power(vx,2) + Power(vy,2) + g*y) + 
         _Sqrt(Power(r,2) - Power(R,2))*(-(g*x) + vx*w*x + vy*w*y)) + 
      Power(r,2)*R*(g*Power(R,2)*_Sqrt(Power(r,2) - Power(R,2))*x + 
         Power(R,3)*(Power(vx,2) + Power(vy,2) + g*y) + 
         _Sqrt(Power(r,2) - Power(R,2))*
          (-(Power(vx,2)*x*y) + Power(vy,2)*x*y + 
            vx*vy*(Power(x,2) - Power(y,2))) + 
         R*(4*vx*vy*x*y - Power(vy,2)*(Power(x,2) - 3*Power(y,2)) + 
            Power(vx,2)*(3*Power(x,2) - Power(y,2)))) - 
      Power(R,2)*(2*Power(R,2)*Power(vx*x + vy*y,2) + 
         (Power(vx,2) - Power(vy,2))*(Power(x,4) - Power(y,4)) - 
         2*R*_Sqrt(Power(r,2) - Power(R,2))*
          (Power(vx,2)*x*y - Power(vy,2)*x*y + 
            vx*vy*(-Power(x,2) + Power(y,2))))))/
  ((Power(r,2) - Power(R,2))*(Power(r,8)*_Sqrt(Power(r,2) - Power(R,2))*
       (1 + InertiaInv*Power(R,2)) + 
      Power(r,2)*Power(R,2)*(Power(x,2) - Power(y,2))*
       (_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) + 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2)) - 
      Power(R,2)*(_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) + 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2))*(Power(x,4) - Power(y,4)))\
)
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionRight2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=_Sqrt(x*x + y*y);
double tension = 
(Power(r,4)*(Power(r,6)*(Power(vx,2) + Power(vy,2) + g*y) - 
      2*Power(R,3)*(vy*x - vx*y)*
       (_Sqrt(Power(r,2) - Power(R,2))*vx*x + R*vy*x - R*vx*y + 
         _Sqrt(Power(r,2) - Power(R,2))*vy*y) + 
      Power(r,4)*R*(-2*R*(Power(vx,2) + Power(vy,2)) + 
         _Sqrt(Power(r,2) - Power(R,2))*(-(g*x) + vx*w*x + vy*w*y)) + 
      Power(r,2)*R*(g*Power(R,2)*_Sqrt(Power(r,2) - Power(R,2))*x + 
         Power(R,3)*(Power(vx,2) + Power(vy,2) - g*y) + 
         2*R*Power(vy*x - vx*y,2) + 
         _Sqrt(Power(r,2) - Power(R,2))*
          (-(Power(vx,2)*x*y) + Power(vy,2)*x*y + 
            vx*vy*(Power(x,2) - Power(y,2))))))/
  ((Power(r,2) - Power(R,2))*(Power(r,8)*_Sqrt(Power(r,2) - Power(R,2))*
       (1 + InertiaInv*Power(R,2)) + 
      Power(r,2)*Power(R,2)*(Power(x,2) - Power(y,2))*
       (_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) - 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2)) + 
      Power(R,2)*(-(_Sqrt(Power(r,2) - Power(R,2))*Power(x,2)) + 2*R*x*y + 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2))*(Power(x,4) - Power(y,4)))\
)
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
double TensionLeft1(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=_Sqrt(x*x + y*y);
double tension = 
(Power(r,4)*(Power(r,6)*(Power(vx,2) + Power(vy,2) + g*y) - 
      2*Power(R,3)*(-(vy*x) + vx*y)*
       (_Sqrt(Power(r,2) - Power(R,2))*vx*x - R*vy*x + R*vx*y + 
         _Sqrt(Power(r,2) - Power(R,2))*vy*y) - 
      Power(r,4)*R*(2*R*(Power(vx,2) + Power(vy,2)) + 
         _Sqrt(Power(r,2) - Power(R,2))*(-(g*x) + vx*w*x + vy*w*y)) + 
      Power(r,2)*R*(-(g*Power(R,2)*_Sqrt(Power(r,2) - Power(R,2))*x) + 
         Power(R,3)*(Power(vx,2) + Power(vy,2) - g*y) + 
         2*R*Power(vy*x - vx*y,2) + 
         _Sqrt(Power(r,2) - Power(R,2))*
          (Power(vx,2)*x*y - Power(vy,2)*x*y + 
            vx*vy*(-Power(x,2) + Power(y,2))))))/
  ((Power(r,2) - Power(R,2))*(Power(r,8)*_Sqrt(Power(r,2) - Power(R,2))*
       (1 + InertiaInv*Power(R,2)) + 
      Power(r,2)*Power(R,2)*(Power(x,2) - Power(y,2))*
       (_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) + 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2)) - 
      Power(R,2)*(_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) + 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2))*(Power(x,4) - Power(y,4)))\
)
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionLeft2(double x, double y, double vx, double vy, double R, double g, double InertiaInv, double w)
{
	double r=_Sqrt(x*x + y*y);
double tension = 
(Power(r,4)*(Power(r,6)*(Power(vx,2) + Power(vy,2) + g*y) - 
      Power(r,4)*R*(2*R*(Power(vx,2) + Power(vy,2) + g*y) + 
         _Sqrt(Power(r,2) - Power(R,2))*(-(g*x) + vx*w*x + vy*w*y)) + 
      Power(r,2)*R*(-(g*Power(R,2)*_Sqrt(Power(r,2) - Power(R,2))*x) + 
         Power(R,3)*(Power(vx,2) + Power(vy,2) + g*y) + 
         R*(4*vx*vy*x*y - Power(vy,2)*(Power(x,2) - 3*Power(y,2)) + 
            Power(vx,2)*(3*Power(x,2) - Power(y,2))) + 
         _Sqrt(Power(r,2) - Power(R,2))*
          (Power(vx,2)*x*y - Power(vy,2)*x*y + 
            vx*vy*(-Power(x,2) + Power(y,2)))) - 
      Power(R,2)*(2*Power(R,2)*Power(vx*x + vy*y,2) + 
         (Power(vx,2) - Power(vy,2))*(Power(x,4) - Power(y,4)) + 
         2*R*_Sqrt(Power(r,2) - Power(R,2))*
          (Power(vx,2)*x*y - Power(vy,2)*x*y + 
            vx*vy*(-Power(x,2) + Power(y,2))))))/
  ((Power(r,2) - Power(R,2))*(Power(r,8)*_Sqrt(Power(r,2) - Power(R,2))*
       (1 + InertiaInv*Power(R,2)) + 
      Power(r,2)*Power(R,2)*(Power(x,2) - Power(y,2))*
       (_Sqrt(Power(r,2) - Power(R,2))*Power(x,2) - 2*R*x*y - 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2)) + 
      Power(R,2)*(-(_Sqrt(Power(r,2) - Power(R,2))*Power(x,2)) + 2*R*x*y + 
         _Sqrt(Power(r,2) - Power(R,2))*Power(y,2))*(Power(x,4) - Power(y,4)))\
)
;
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define STUFF \
const double smallNumber = 0.01e-6;\
if (r1<smallNumber) return 0;\
if (r2<smallNumber) return 0;
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft11(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-(((R*(2*v2x*x2 + 2*v2y*y2)*(v2y*x2 - v2x*y2 + 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,4) - 
       (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
             _Sqrt(-Power(R,2) + Power(r2,2)) - 
            (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
             (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,2) + 
       (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2))*
             (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*(v2y*x2 - v2x*y2 + 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2))*
             (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
              (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) + 
       (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
               (y2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        (-((R*((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                 (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                 v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                    2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2y*x2*y2 - 
                      R*v2x*(Power(x2,2) - Power(y2,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                 v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                    2*v2y*Power(x2,2)*y2 + 
                    2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                 g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
               (x2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
        (g - (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
               (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
               g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
               v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                  R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                  g*R*(Power(x2,2) - Power(y2,2))) + 
               ((2*v2x*x2 + 2*v2y*y2)*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
               v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                  4*v2y*Power(y2,3) + 2*v2x*x2*(Power(R,2) + Power(y2,2))\
)))/(Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))))/
     (InertiaInv*m1*Power(R,2)*
       _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)))) \
+ ((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
               (v1y*x1 - v1x*y1 + 
                 (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2))\
)*_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)))/Power(r1,4)) + 
          (R*(g*x1 + (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
                _Sqrt(-Power(R,2) + Power(r1,2)) - 
               (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
                (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)))/Power(r1,2) - 
          (R*w*(2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*(v1y*x1 - v1x*y1 + 
               (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
             (2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                  (y1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1x - (R*((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*(-((R*
                  ((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1 + 
                    (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 + 
                    v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                    _Sqrt(-Power(R,2) + Power(r1,2))*
                     (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                       2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                    ((2*v1x*x1 + 2*v1y*y1)*
                       (-2*R*v1y*x1*y1 - 
                       R*v1x*(Power(x1,2) - Power(y1,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                    v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                       2*v1y*Power(x1,2)*y1 + 
                       2*v1x*x1*(-Power(R,2) + Power(y1,2))) - 
                    g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
                (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                  (x1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1y - (R*(-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) + 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
              Power(r1,2))*(g - 
             (R*(-((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1) - 
                  (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 - 
                  g*(-2*Power(R,2) + Power(r1,2))*x1*y1 - 
                  v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                     R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                     g*R*(Power(x1,2) - Power(y1,2))) + 
                  ((2*v1x*x1 + 2*v1y*y1)*
                     (-2*R*v1x*x1*y1 + 
                       R*v1y*(Power(x1,2) - Power(y1,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                  v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                     4*v1y*Power(y1,3) + 
                     2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) + 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
             Power(r2,2),2) + 
          Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*
                       (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) + 
               (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                    (x1*(-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))))/
                _Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) + 
          (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
              Power(r1,2))*((-y1 + 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))) + 
                  ((-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-y1 + (R*
                        (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        ((R*(2*v2x*x2 + 2*v2y*y2)*
             (v2y*x2 - v2x*y2 + 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,4) - 
          (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
                _Sqrt(-Power(R,2) + Power(r2,2)) - 
               (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
                (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,2) + 
          (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*(v2y*x2 - v2x*y2 + 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2))*
                (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*Power(r2,2)*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) + 
          (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                  (y2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))*(-((R*
                  ((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                    (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                    v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                    _Sqrt(-Power(R,2) + Power(r2,2))*
                     (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                       2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                    ((2*v2x*x2 + 2*v2y*y2)*
                       (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                    v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                       2*v2y*Power(x2,2)*y2 + 
                       2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                    g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
                (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                  (x2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
+ v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))*(g - 
             (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
                  (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
                  g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
                  v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                     R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                     g*R*(Power(x2,2) - Power(y2,2))) + 
                  ((2*v2x*x2 + 2*v2y*y2)*
                     (-2*R*v2x*x2*y2 + 
                       R*v2y*(Power(x2,2) - Power(y2,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                  v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                     4*v2y*Power(y2,3) + 
                     2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
+ v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                  v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))*
     (-(InertiaInv*m2*Power(R,2)*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),
             2) + Power(x2 - (R*
                 (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
             2))) - (R*_Sqrt(Power(y2 - 
              (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))*
          ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (y2*(-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) + 
            (R*((y2*(-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 (x2*(-x2 + (R*
                        (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2))))/
        Power(r2,2) + (y2 - (R*
             (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
        ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))) + 
               ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2))*
                  (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((R*(Power(x2,2) - Power(y2,2))*
                     (-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (2*R*x2*y2*(-x2 + 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                  (-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2))*
                  (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((-2*R*x2*y2*(-y2 + 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (R*(Power(x2,2) - Power(y2,2))*
                     (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
   (InertiaInv*m1*Power(R,2)*_Sqrt(Power(y2 - 
         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),2) + 
       Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
          Power(r2,2),2))*(Power(InertiaInv,2)*m1*m2*Power(R,4)*
        _Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2),2) + Power(x1 - 
            (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
        _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
             Power(r2,2),2) + Power(x2 - 
            (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) \
- (-(InertiaInv*m1*Power(R,2)*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) + 
               (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) + 
                    (x1*(-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/Power(r1,2))*
           ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))) + 
                  ((-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-x1 + (R*
                         (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-y1 + (R*
                        (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        (-(InertiaInv*m2*Power(R,2)*
             _Sqrt(Power(y2 - (R*
                    (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))*
             ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (y2*(-x2 + (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               (R*((y2*(-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
                        2)) + 
                    (x2*(-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))*((-y2 + 
                (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*
                     (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                  ((-x2 + (R*
                        (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2))*
                     (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                       Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((R*(Power(x2,2) - Power(y2,2))*
                        (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (2*R*x2*y2*
                        (-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
           ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                     (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  ((-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2))*
                     (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((-2*R*x2*y2*
                        (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (R*(Power(x2,2) - Power(y2,2))*
                        (-x2 + 
                          (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                          (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft12(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-(((R*(2*v2x*x2 + 2*v2y*y2)*(v2y*x2 - v2x*y2 - 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,4) - 
       (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
             _Sqrt(-Power(R,2) + Power(r2,2)) + 
            (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
             (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,2) + 
       (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                    R*y2))/Power(r2,2))*
             (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*_Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                   R*y2))/Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*(v2y*x2 - v2x*y2 - 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2))*
             (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
              (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) + 
       (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
               (y2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        (-((R*(-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                 (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                 v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                    2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2y*x2*y2 - 
                      R*v2x*(Power(x2,2) - Power(y2,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                 v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                    2*v2y*Power(x2,2)*y2 + 
                    2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                 g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
               (x2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
           Power(r2,2))*(g - (R*
             ((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
               (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
               g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
               v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                  R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                  g*R*(Power(x2,2) - Power(y2,2))) + 
               ((2*v2x*x2 + 2*v2y*y2)*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
               v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                  4*v2y*Power(y2,3) + 2*v2x*x2*(Power(R,2) + Power(y2,2))\
)))/(Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))))/
     (InertiaInv*m1*Power(R,2)*
       _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)))) \
+ ((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
               (v1y*x1 - v1x*y1 + 
                 (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2))\
)*_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)))/Power(r1,4)) + 
          (R*(g*x1 + (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
                _Sqrt(-Power(R,2) + Power(r1,2)) - 
               (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
                (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)))/Power(r1,2) - 
          (R*w*(2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*(v1y*x1 - v1x*y1 + 
               (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
             (2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                  (y1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1x - (R*((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*(-((R*
                  ((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1 + 
                    (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 + 
                    v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                    _Sqrt(-Power(R,2) + Power(r1,2))*
                     (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                       2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                    ((2*v1x*x1 + 2*v1y*y1)*
                       (-2*R*v1y*x1*y1 - 
                       R*v1x*(Power(x1,2) - Power(y1,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                    v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                       2*v1y*Power(x1,2)*y1 + 
                       2*v1x*x1*(-Power(R,2) + Power(y1,2))) - 
                    g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
                (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) - 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                  (x1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1y - (R*(-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) + 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
              Power(r1,2))*(g - 
             (R*(-((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1) - 
                  (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 - 
                  g*(-2*Power(R,2) + Power(r1,2))*x1*y1 - 
                  v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                     R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                     g*R*(Power(x1,2) - Power(y1,2))) + 
                  ((2*v1x*x1 + 2*v1y*y1)*
                     (-2*R*v1x*x1*y1 + 
                       R*v1y*(Power(x1,2) - Power(y1,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                  v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                     4*v1y*Power(y1,3) + 
                     2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) + 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                 R*y2))/Power(r2,2),2) + 
          Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*
                       (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) + 
               (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                    (x1*(-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))))/
                _Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) + 
          (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
              Power(r1,2))*((-y1 + 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))) + 
                  ((-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-y1 + (R*
                        (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        ((R*(2*v2x*x2 + 2*v2y*y2)*
             (v2y*x2 - v2x*y2 - 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,4) - 
          (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
                _Sqrt(-Power(R,2) + Power(r2,2)) + 
               (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
                (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,2) + 
          (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*(v2y*x2 - v2x*y2 - 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*Power(r2,2)*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) + 
          (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                  (y2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))*(-((R*
                  (-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                    (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                    v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                    _Sqrt(-Power(R,2) + Power(r2,2))*
                     (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                       2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                    ((2*v2x*x2 + 2*v2y*y2)*
                       (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                    v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                       2*v2y*Power(x2,2)*y2 + 
                       2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                    g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
                (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                  (x2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
- v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))*(g - 
             (R*((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
                  (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
                  g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
                  v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                     R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                     g*R*(Power(x2,2) - Power(y2,2))) + 
                  ((2*v2x*x2 + 2*v2y*y2)*
                     (-2*R*v2x*x2*y2 + 
                       R*v2y*(Power(x2,2) - Power(y2,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                  v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                     4*v2y*Power(y2,3) + 
                     2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
- v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                  v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))*
     (-(InertiaInv*m2*Power(R,2)*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*_Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))*
          ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2))/Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (y2*(-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*((y2*(-y2 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)\
)/Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 (x2*(-x2 + (R*
                        (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2))))/
        Power(r2,2) + (y2 - (R*
             (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2))*
        ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                  (-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2))*
                  (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((R*(Power(x2,2) - Power(y2,2))*
                     (-y2 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (2*R*x2*y2*(-x2 + 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))) + 
               ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2))*
                  (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((-2*R*x2*y2*(-y2 + 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (R*(Power(x2,2) - Power(y2,2))*
                     (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
   (InertiaInv*m1*Power(R,2)*_Sqrt(Power(y2 - 
         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2),2) \
+ Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2))*
     (Power(InertiaInv,2)*m1*m2*Power(R,4)*
        _Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2),2) + Power(x1 - 
            (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
        _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                 R*y2))/Power(r2,2),2) + 
          Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - (R*
                    (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) + 
               (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) + 
                    (x1*(-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/Power(r1,2))*
           ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))) + 
                  ((-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-x1 + (R*
                         (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-y1 + (R*
                        (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        (-(InertiaInv*m2*Power(R,2)*
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))*
             ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (y2*(-x2 + (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (R*((y2*(-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
                        2)) + 
                    (x2*(-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))*((-y2 + 
                (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*
                     (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                     (-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  ((-x2 + (R*
                        (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2))*
                     (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                       Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((R*(Power(x2,2) - Power(y2,2))*
                        (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (2*R*x2*y2*
                        (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
           ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                       (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                  ((-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2))*
                     (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((-2*R*x2*y2*
                        (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (R*(Power(x2,2) - Power(y2,2))*
                        (-x2 + 
                          (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                          (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft21(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-(((R*(2*v2x*x2 + 2*v2y*y2)*(v2y*x2 - v2x*y2 + 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,4) - 
       (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
             _Sqrt(-Power(R,2) + Power(r2,2)) - 
            (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
             (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,2) + 
       (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2))*
             (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*(v2y*x2 - v2x*y2 + 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2))*
             (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
              (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) + 
       (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
               (y2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        (-((R*((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                 (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                 v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                    2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2y*x2*y2 - 
                      R*v2x*(Power(x2,2) - Power(y2,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                 v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                    2*v2y*Power(x2,2)*y2 + 
                    2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                 g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
               (x2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
        (g - (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
               (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
               g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
               v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                  R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                  g*R*(Power(x2,2) - Power(y2,2))) + 
               ((2*v2x*x2 + 2*v2y*y2)*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
               v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                  4*v2y*Power(y2,3) + 2*v2x*x2*(Power(R,2) + Power(y2,2))\
)))/(Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))))/
     (InertiaInv*m1*Power(R,2)*
       _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)))) \
+ ((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
               (v1y*x1 - v1x*y1 - 
                 (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2))\
)*_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)))/Power(r1,4)) + 
          (R*(g*x1 - (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
                _Sqrt(-Power(R,2) + Power(r1,2)) + 
               (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
                (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)))/Power(r1,2) - 
          (R*w*(2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + 
                       R*v1y - 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*(v1y*x1 - v1x*y1 - 
               (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
             (2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + 
                       R*v1y - 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                  (y1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1x - (R*(-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*(-((R*
                  (-((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1) - 
                    (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 - 
                    v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                    _Sqrt(-Power(R,2) + Power(r1,2))*
                     (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                       2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                    ((2*v1x*x1 + 2*v1y*y1)*
                       (-2*R*v1y*x1*y1 - 
                       R*v1x*(Power(x1,2) - Power(y1,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                    v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                       2*v1y*Power(x1,2)*y1 + 
                       2*v1x*x1*(-Power(R,2) + Power(y1,2))) + 
                    g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
                (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                  (x1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1y - (R*((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) - 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
              Power(r1,2))*(g - 
             (R*((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1 + 
                  (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 + 
                  g*(-2*Power(R,2) + Power(r1,2))*x1*y1 + 
                  v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                     R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                     g*R*(Power(x1,2) - Power(y1,2))) + 
                  ((2*v1x*x1 + 2*v1y*y1)*
                     (-2*R*v1x*x1*y1 + 
                       R*v1y*(Power(x1,2) - Power(y1,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                  v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                     4*v1y*Power(y1,3) + 
                     2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) - 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
             Power(r2,2),2) + 
          Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*
                       (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)\
)/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                    (x1*(-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))))/
                _Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) + 
          (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
              Power(r1,2))*((-y1 + 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))) + 
                  ((-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        ((R*(2*v2x*x2 + 2*v2y*y2)*
             (v2y*x2 - v2x*y2 + 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,4) - 
          (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
                _Sqrt(-Power(R,2) + Power(r2,2)) - 
               (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
                (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,2) + 
          (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*(v2y*x2 - v2x*y2 + 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2))*
                (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*Power(r2,2)*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) + 
          (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                  (y2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))*(-((R*
                  ((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                    (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                    v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                    _Sqrt(-Power(R,2) + Power(r2,2))*
                     (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                       2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                    ((2*v2x*x2 + 2*v2y*y2)*
                       (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                    v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                       2*v2y*Power(x2,2)*y2 + 
                       2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                    g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
                (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
- v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                  (x2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
+ v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))*(g - 
             (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
                  (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
                  g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
                  v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                     R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                     g*R*(Power(x2,2) - Power(y2,2))) + 
                  ((2*v2x*x2 + 2*v2y*y2)*
                     (-2*R*v2x*x2*y2 + 
                       R*v2y*(Power(x2,2) - Power(y2,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                  v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                     4*v2y*Power(y2,3) + 
                     2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
+ v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                  v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))*
     (-(InertiaInv*m2*Power(R,2)*
          _Sqrt(Power(y2 - (R*
                 (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),
             2) + Power(x2 - (R*
                 (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
             2))) - (R*_Sqrt(Power(y2 - 
              (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))*
          ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (y2*(-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) + 
            (R*((y2*(-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 (x2*(-x2 + (R*
                        (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2))))/
        Power(r2,2) + (y2 - (R*
             (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
        ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))) + 
               ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2))*
                  (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((R*(Power(x2,2) - Power(y2,2))*
                     (-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (2*R*x2*y2*(-x2 + 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                  (-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2))*
                  (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((-2*R*x2*y2*(-y2 + 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (R*(Power(x2,2) - Power(y2,2))*
                     (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
   (InertiaInv*m1*Power(R,2)*_Sqrt(Power(y2 - 
         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),2) + 
       Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
          Power(r2,2),2))*(Power(InertiaInv,2)*m1*m2*Power(R,4)*
        _Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                 R*y1))/Power(r1,2),2) + 
          Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2),2))*_Sqrt(Power(y2 - 
            (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),2) \
+ Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) \
- (-(InertiaInv*m1*Power(R,2)*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) + 
                    (x1*(-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/Power(r1,2))*
           ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-x1 + (R*
                        (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                  ((-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        (-(InertiaInv*m2*Power(R,2)*
             _Sqrt(Power(y2 - (R*
                    (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*_Sqrt(Power(y2 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))*
             ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (y2*(-x2 + (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               (R*((y2*(-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
                        2)) + 
                    (x2*(-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
              Power(r2,2))*((-y2 + 
                (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*
                     (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                  ((-x2 + (R*
                        (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2))*
                     (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                       Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((R*(Power(x2,2) - Power(y2,2))*
                        (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (2*R*x2*y2*
                        (-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
           ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                     (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  ((-y2 + (R*
                        (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2))*
                     (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                        Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((-2*R*x2*y2*
                        (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (R*(Power(x2,2) - Power(y2,2))*
                        (-x2 + 
                          (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                        Power(x2 - 
                          (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsLeft22(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-(((R*(2*v2x*x2 + 2*v2y*y2)*(v2y*x2 - v2x*y2 - 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,4) - 
       (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
             _Sqrt(-Power(R,2) + Power(r2,2)) + 
            (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
             (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2)))/Power(r2,2) + 
       (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                    R*y2))/Power(r2,2))*
             (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*_Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                   R*y2))/Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*(v2y*x2 - v2x*y2 - 
            (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
          (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2))*
             (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                    (x2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)) \
+ 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
             (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                  (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
               (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                    (y2*(2*v2x*x2 + 2*v2y*y2))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)))\
)/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
              (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) + 
       (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
               (y2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        (-((R*(-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                 (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                 v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                    2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2y*x2*y2 - 
                      R*v2x*(Power(x2,2) - Power(y2,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                 v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                    2*v2y*Power(x2,2)*y2 + 
                    2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                 g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
               v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                  Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
             (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
          (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
               (x2*(2*v2x*x2 + 2*v2y*y2))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
        (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
           Power(r2,2))*(g - (R*
             ((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
               (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
               g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
               v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                  R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                  g*R*(Power(x2,2) - Power(y2,2))) + 
               ((2*v2x*x2 + 2*v2y*y2)*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))))/
                (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
               v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                  4*v2y*Power(y2,3) + 2*v2x*x2*(Power(R,2) + Power(y2,2))\
)))/(Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
          (R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
          (2*R*(2*v2x*x2 + 2*v2y*y2)*
             ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
               v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                  Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
           (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))))/
     (InertiaInv*m1*Power(R,2)*
       _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)))) \
+ ((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
               (v1y*x1 - v1x*y1 - 
                 (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2))\
)*_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)))/Power(r1,4)) + 
          (R*(g*x1 - (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
                _Sqrt(-Power(R,2) + Power(r1,2)) + 
               (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
                (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)))/Power(r1,2) - 
          (R*w*(2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + 
                       R*v1y - 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*(v1y*x1 - v1x*y1 - 
               (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
             (2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + 
                       R*v1y - 
                       (x1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2)) + 
               2*(x1 - (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2))*
                (v1x + (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                     (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                  (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                       (y1*(2*v1x*x1 + 2*v1y*y1))/
                        (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                   Power(r1,2))))/
           (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                  (y1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1x - (R*(-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*(-((R*
                  (-((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1) - 
                    (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 - 
                    v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                    _Sqrt(-Power(R,2) + Power(r1,2))*
                     (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                       2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                    ((2*v1x*x1 + 2*v1y*y1)*
                       (-2*R*v1y*x1*y1 - 
                       R*v1x*(Power(x1,2) - Power(y1,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                    v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                       2*v1y*Power(x1,2)*y1 + 
                       2*v1x*x1*(-Power(R,2) + Power(y1,2))) + 
                    g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
                (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1y*x1*y1 - 
                     R*v1x*(Power(x1,2) - Power(y1,2))) + 
                  v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                     Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)*
                (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
             (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                  (x1*(2*v1x*x1 + 2*v1y*y1))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
           (v1y - (R*((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) - 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
              Power(r1,2))*(g - 
             (R*((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1 + 
                  (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 + 
                  g*(-2*Power(R,2) + Power(r1,2))*x1*y1 + 
                  v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                     R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                     g*R*(Power(x1,2) - Power(y1,2))) + 
                  ((2*v1x*x1 + 2*v1y*y1)*
                     (-2*R*v1x*x1*y1 + 
                       R*v1y*(Power(x1,2) - Power(y1,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                  v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                     4*v1y*Power(y1,3) + 
                     2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
             (R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + 
                     R*v1y*(Power(x1,2) - Power(y1,2))) - 
                  v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
             (2*R*(2*v1x*x1 + 2*v1y*y1)*
                ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                     Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
              (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                 R*y2))/Power(r2,2),2) + 
          Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*
                       (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)\
)/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                    (x1*(-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2))))/
                _Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) + 
          (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
              Power(r1,2))*((-y1 + 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))) + 
                  ((-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        ((R*(2*v2x*x2 + 2*v2y*y2)*
             (v2y*x2 - v2x*y2 - 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,4) - 
          (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
                _Sqrt(-Power(R,2) + Power(r2,2)) + 
               (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
                (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)))/Power(r2,2) + 
          (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*(v2y*x2 - v2x*y2 - 
               (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
             (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2))/Power(r2,2))*
                (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                       R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                       (x2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2)) + 
               2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2))*
                (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                     (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                  (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                       (y2*(2*v2x*x2 + 2*v2y*y2))/
                        (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/
                   Power(r2,2))))/
           (2.*Power(r2,2)*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) + 
          (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                  (y2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))*(-((R*
                  (-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                    (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                    v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                    _Sqrt(-Power(R,2) + Power(r2,2))*
                     (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                       2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                    ((2*v2x*x2 + 2*v2y*y2)*
                       (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                     (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                    v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                       2*v2y*Power(x2,2)*y2 + 
                       2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                    g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
                (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) \
+ v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                     Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
                (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
             (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                  (x2*(2*v2x*x2 + 2*v2y*y2))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
           (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
- v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))*(g - 
             (R*((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
                  (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
                  g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
                  v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                     R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                     g*R*(Power(x2,2) - Power(y2,2))) + 
                  ((2*v2x*x2 + 2*v2y*y2)*
                     (-2*R*v2x*x2*y2 + 
                       R*v2y*(Power(x2,2) - Power(y2,2))))/
                   (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                  v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                     4*v2y*Power(y2,3) + 
                     2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
             (R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) \
- v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
             (2*R*(2*v2x*x2 + 2*v2y*y2)*
                ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                  v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                     Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
              (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))*
     (-(InertiaInv*m2*Power(R,2)*
          _Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))) - 
       (R*_Sqrt(Power(y2 - (R*
                 (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
               Power(r2,2),2) + 
            Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
               Power(r2,2),2))*
          ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2))/Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (y2*(-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2)))/
             _Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*((y2*(-y2 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)\
)/Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 (x2*(-x2 + (R*
                        (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2))))/
        Power(r2,2) + (y2 - (R*
             (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2))*
        ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                  (-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2))*
                  (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - 
                    (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((R*(Power(x2,2) - Power(y2,2))*
                     (-y2 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (2*R*x2*y2*(-x2 + 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
       (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
        ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
              Power(r2,2))/
           _Sqrt(Power(y2 - (R*
                  (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2),2) + 
             Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2),2)) - 
          (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2))) + 
               ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2))*
                  (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) + 
               _Sqrt(-Power(R,2) + Power(r2,2))*
                ((-2*R*x2*y2*(-y2 + 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  (R*(Power(x2,2) - Power(y2,2))*
                     (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)))))/
           (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
   (InertiaInv*m1*Power(R,2)*_Sqrt(Power(y2 - 
         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2),2) \
+ Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2))*
     (Power(InertiaInv,2)*m1*m2*Power(R,4)*
        _Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                 R*y1))/Power(r1,2),2) + 
          Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2),2))*_Sqrt(Power(y2 - 
            (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2),
           2) + Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2),2)) - 
       (-(InertiaInv*m1*Power(R,2)*
             _Sqrt(Power(y1 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))) + 
          (R*_Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2))*
             ((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (y1*(-x1 + (R*
                       (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2)))/
                _Sqrt(Power(y1 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                     Power(r1,2),2) + 
                  Power(x1 - 
                    (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                     Power(r1,2),2)) - 
               (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) + 
                    (x1*(-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/Power(r1,2))*
           ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                     (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2)))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) - 
                  ((-x1 + (R*
                        (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2))*
                     (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                       Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((R*(Power(x1,2) - Power(y1,2))*
                        (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                         2)) - 
                     (2*R*x1*y1*
                        (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
          (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
              Power(r1,2))*((-x1 + 
                (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2))/
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                  ((-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2))*
                     (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                       Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                   _Sqrt(Power(y1 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                     Power(x1 - 
                       (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r1,2))*
                   ((-2*R*x1*y1*
                        (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                     (R*(Power(x1,2) - Power(y1,2))*
                        (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                      _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2),2) + 
                        Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
              (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
        (-(InertiaInv*m2*Power(R,2)*
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))) - 
          (R*_Sqrt(Power(y2 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*
                    (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2))*
             ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (y2*(-x2 + (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2)))/
                _Sqrt(Power(y2 - 
                    (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                     Power(r2,2),2) + 
                  Power(x2 - (R*
                       (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                     Power(r2,2),2)) - 
               (R*((y2*(-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),
                        2)) + 
                    (x2*(-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
              Power(r2,2))*((-y2 + 
                (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*
                     (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                     (-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2)))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) - 
                  ((-x2 + (R*
                        (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2))*
                     (-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                       Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((R*(Power(x2,2) - Power(y2,2))*
                        (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (2*R*x2*y2*
                        (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
          (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
           ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2))/
              _Sqrt(Power(y2 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                   Power(r2,2),2) + 
                Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                   Power(r2,2),2)) - 
             (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                       (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                  ((-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                        Power(r2,2))*
                     (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                       Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                   _Sqrt(Power(y2 - 
                       (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                     Power(x2 - 
                       (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                        Power(r2,2),2)) + 
                  _Sqrt(-Power(R,2) + Power(r2,2))*
                   ((-2*R*x2*y2*
                        (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                     (R*(Power(x2,2) - Power(y2,2))*
                        (-x2 + 
                          (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                      _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                        Power(x2 - 
                          (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
              (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
double TensionsRight11(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
              (v1y*x1 - v1x*y1 + 
                (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)))/Power(r1,4)) + 
         (R*(g*x1 + (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)) - 
              (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
               (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
))/Power(r1,2) - (R*w*(2*(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*_Sqrt(Power(y1 - 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (R*(v1y*x1 - v1x*y1 + (R*(v1x*x1 + v1y*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)))*
            (2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                 (y1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1x - (R*((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*(-((R*
                 ((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1 + 
                   (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 + 
                   v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                   _Sqrt(-Power(R,2) + Power(r1,2))*
                    (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                      2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                   ((2*v1x*x1 + 2*v1y*y1)*
                      (-2*R*v1y*x1*y1 - 
                        R*v1x*(Power(x1,2) - Power(y1,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                   v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                      2*v1y*Power(x1,2)*y1 + 
                      2*v1x*x1*(-Power(R,2) + Power(y1,2))) - 
                   g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
               (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                 (x1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1y - (R*(-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2))*(g - 
            (R*(-((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1) - 
                 (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 - 
                 g*(-2*Power(R,2) + Power(r1,2))*x1*y1 - 
                 v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                    R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                    g*R*(Power(x1,2) - Power(y1,2))) + 
                 ((2*v1x*x1 + 2*v1y*y1)*
                    (-2*R*v1x*x1*y1 + 
                      R*v1y*(Power(x1,2) - Power(y1,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                 v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                    4*v1y*Power(y1,3) + 
                    2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) + 
                 v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) \
- (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
)*((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) + 
              (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                   (x1*(-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/Power(r1,2))*
          ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))) + 
                 ((-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*((-x1 + 
               (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       ((R*(2*v2x*x2 + 2*v2y*y2)*
            (v2y*x2 - v2x*y2 + 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,4) - 
         (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
               _Sqrt(-Power(R,2) + Power(r2,2)) - 
              (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
               (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,2) + 
         (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*(v2y*x2 - v2x*y2 + 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
                (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) + 
         (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                 (y2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2))*(-((R*
                 ((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                   (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                   v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                   _Sqrt(-Power(R,2) + Power(r2,2))*
                    (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                      2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                   ((2*v2x*x2 + 2*v2y*y2)*
                      (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                   v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                      2*v2y*Power(x2,2)*y2 + 
                      2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                   g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
               (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                 (x2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
          (g - (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
                 (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
                 g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
                 v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                    R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                    g*R*(Power(x2,2) - Power(y2,2))) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2)))\
)/(2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                 v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                    4*v2y*Power(y2,3) + 
                    2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
    (Power(InertiaInv,2)*m1*m2*Power(R,4)*
       _Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
            Power(r1,2),2) + Power(x1 - 
           (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
       _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) - 
      (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
            ((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) + 
              (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2)) + (x1*
                      (-x1 + 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))))/_Sqrt(-Power(R,2) + Power(r1,2))\
))/Power(r1,2) + (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2))*((-y1 + 
               (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2))*
          ((-x1 + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - (R*
                    (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-x1 + (R*
                         (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       (-(InertiaInv*m2*Power(R,2)*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*_Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))*
            ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (y2*(-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) + 
              (R*((y2*(-y2 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) + 
                   (x2*(-x2 + 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
             Power(r2,2))*((-y2 + 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2)\
)/_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                      (-y2 + (R*
                         (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                 ((-x2 + (R*(R*x2 - 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2)\
)*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                      Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((R*(Power(x2,2) - Power(y2,2))*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (2*R*x2*y2*
                       (-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
          ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) - 
                 ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2))*
                    (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((-2*R*x2*y2*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (R*(Power(x2,2) - Power(y2,2))*
                       (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight12(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
              (v1y*x1 - v1x*y1 + 
                (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
              _Sqrt(Power(y1 - 
                  (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)))/Power(r1,4)) + 
         (R*(g*x1 + (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)) - 
              (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
               (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
))/Power(r1,2) - (R*w*(2*(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*_Sqrt(Power(y1 - 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (R*(v1y*x1 - v1x*y1 + (R*(v1x*x1 + v1y*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)))*
            (2*(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (v1x + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(R*v1x - _Sqrt(-Power(R,2) + Power(r1,2))*v1y - 
                 (y1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1x - (R*((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*(-((R*
                 ((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1 + 
                   (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 + 
                   v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                   _Sqrt(-Power(R,2) + Power(r1,2))*
                    (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                      2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                   ((2*v1x*x1 + 2*v1y*y1)*
                      (-2*R*v1y*x1*y1 - 
                        R*v1x*(Power(x1,2) - Power(y1,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                   v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                      2*v1y*Power(x1,2)*y1 + 
                      2*v1x*x1*(-Power(R,2) + Power(y1,2))) - 
                   g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
               (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
- v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (v1y + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(_Sqrt(-Power(R,2) + Power(r1,2))*v1x + R*v1y + 
                 (x1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1y - (R*(-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2))*(g - 
            (R*(-((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1) - 
                 (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 - 
                 g*(-2*Power(R,2) + Power(r1,2))*x1*y1 - 
                 v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                    R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                    g*R*(Power(x1,2) - Power(y1,2))) + 
                 ((2*v1x*x1 + 2*v1y*y1)*
                    (-2*R*v1x*x1*y1 + 
                      R*v1y*(Power(x1,2) - Power(y1,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                 v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                    4*v1y*Power(y1,3) + 
                    2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
+ v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) + 
                 v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                R*y2))/Power(r2,2),2) + 
         Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
            Power(r2,2),2)) - (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
)*((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) + 
              (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                   (x1*(-x1 + 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/Power(r1,2))*
          ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))) + 
                 ((-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*((-x1 + 
               (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-x1 + (R*
                        (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       ((R*(2*v2x*x2 + 2*v2y*y2)*
            (v2y*x2 - v2x*y2 - 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,4) - 
         (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
               _Sqrt(-Power(R,2) + Power(r2,2)) + 
              (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
               (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,2) + 
         (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2))/Power(r2,2))*
               (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*_Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                     R*y2))/Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*(v2y*x2 - v2x*y2 - 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) + 
         (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                 (y2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2))*(-((R*
                 (-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                   (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                   v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                   _Sqrt(-Power(R,2) + Power(r2,2))*
                    (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                      2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                   ((2*v2x*x2 + 2*v2y*y2)*
                      (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                   v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                      2*v2y*Power(x2,2)*y2 + 
                      2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                   g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
               (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                 (x2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
             Power(r2,2))*(g - 
            (R*((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
                 (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
                 g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
                 v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                    R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                    g*R*(Power(x2,2) - Power(y2,2))) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2)))\
)/(2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                 v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                    4*v2y*Power(y2,3) + 
                    2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
    (Power(InertiaInv,2)*m1*m2*Power(R,4)*
       _Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
            Power(r1,2),2) + Power(x1 - 
           (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
       _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) - 
      (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 - 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
            ((x1*(-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) + 
              (R*((y1*(-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2)) + (x1*
                      (-x1 + 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))))/_Sqrt(-Power(R,2) + Power(r1,2))\
))/Power(r1,2) + (y1 - (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
             Power(r1,2))*((-y1 + 
               (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-y1 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-x1 + (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                       R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2))*
          ((-x1 + (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - (R*
                    (_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-x1 + (R*
                         (R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-y1 + (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                       Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r1,2))*x1 + R*y1))/
                         Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 - _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       (-(InertiaInv*m2*Power(R,2)*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*_Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))*
            ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (y2*(-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (R*((y2*(-y2 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) + 
                   (x2*(-x2 + 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
             Power(r2,2))*((-y2 + 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) - 
                 ((-x2 + (R*(R*x2 + 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2)\
)*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                      Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((R*(Power(x2,2) - Power(y2,2))*
                       (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (2*R*x2*y2*
                       (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
          ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                      (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                 ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2))*
                    (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((-2*R*x2*y2*
                       (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (R*(Power(x2,2) - Power(y2,2))*
                       (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight21(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
              (v1y*x1 - v1x*y1 - 
                (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)))/Power(r1,4)) + 
         (R*(g*x1 - (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)) + 
              (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
               (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
))/Power(r1,2) - (R*w*(2*(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*_Sqrt(Power(y1 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (R*(v1y*x1 - v1x*y1 - (R*(v1x*x1 + v1y*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)))*
            (2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1))/Power(r1,2))*
               (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                 (y1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1x - (R*(-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*(-((R*
                 (-((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1) - 
                   (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 - 
                   v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                   _Sqrt(-Power(R,2) + Power(r1,2))*
                    (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                      2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                   ((2*v1x*x1 + 2*v1y*y1)*
                      (-2*R*v1y*x1*y1 - 
                        R*v1x*(Power(x1,2) - Power(y1,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                   v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                      2*v1y*Power(x1,2)*y1 + 
                      2*v1x*x1*(-Power(R,2) + Power(y1,2))) + 
                   g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
               (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                 (x1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1y - (R*((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
             Power(r1,2))*(g - 
            (R*((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1 + 
                 (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 + 
                 g*(-2*Power(R,2) + Power(r1,2))*x1*y1 + 
                 v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                    R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                    g*R*(Power(x1,2) - Power(y1,2))) + 
                 ((2*v1x*x1 + 2*v1y*y1)*
                    (-2*R*v1x*x1*y1 + 
                      R*v1y*(Power(x1,2) - Power(y1,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                 v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                    4*v1y*Power(y1,3) + 
                    2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) - 
                 v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       _Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
            Power(r2,2),2) + Power(x2 - 
           (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2),2)) \
- (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
)*((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                   (x1*(-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/Power(r1,2))*
          ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)\
)/Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-x1 + (R*
                        (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*((-x1 + 
               (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-x1 + 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       ((R*(2*v2x*x2 + 2*v2y*y2)*
            (v2y*x2 - v2x*y2 + 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,4) - 
         (R*(g*x2 + (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
               _Sqrt(-Power(R,2) + Power(r2,2)) - 
              (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
               (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,2) + 
         (R*w*(2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*(v2y*x2 - v2x*y2 + 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            (2*(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
                (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) + 
         (v2x + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(R*v2x - _Sqrt(-Power(R,2) + Power(r2,2))*v2y - 
                 (y2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2x - (R*((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2))*(-((R*
                 ((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2 + 
                   (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 + 
                   v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                   _Sqrt(-Power(R,2) + Power(r2,2))*
                    (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                      2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                   ((2*v2x*x2 + 2*v2y*y2)*
                      (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                   v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                      2*v2y*Power(x2,2)*y2 + 
                      2*v2x*x2*(-Power(R,2) + Power(y2,2))) - 
                   g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
               (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) - 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (v2y + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(_Sqrt(-Power(R,2) + Power(r2,2))*v2x + R*v2y + 
                 (x2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2y - (R*(-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2))*
          (g - (R*(-((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2) - 
                 (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 - 
                 g*(-2*Power(R,2) + Power(r2,2))*x2*y2 - 
                 v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                    R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                    g*R*(Power(x2,2) - Power(y2,2))) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2)))\
)/(2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                 v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                    4*v2y*Power(y2,3) + 
                    2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) + 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
    (Power(InertiaInv,2)*m1*m2*Power(R,4)*
       _Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                R*y1))/Power(r1,2),2) + 
         Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
            Power(r1,2),2))*_Sqrt(Power(y2 - 
           (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2),2) + 
         Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
            Power(r2,2),2)) - (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
            ((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2)) + (x1*
                      (-x1 + 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))))/_Sqrt(-Power(R,2) + Power(r1,2))\
))/Power(r1,2) + (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
             Power(r1,2))*((-y1 + 
               (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2))*
          ((-x1 + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-x1 + (R*
                         (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       (-(InertiaInv*m2*Power(R,2)*
            _Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*_Sqrt(Power(y2 - (R*
                   (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))*
            ((x2*(-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (y2*(-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) + 
              (R*((y2*(-y2 + 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                       R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) + 
                   (x2*(-x2 + 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
             Power(r2,2))*((-y2 + 
               (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/Power(r2,2)\
)/_Sqrt(Power(y2 - (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                      (-y2 + (R*
                         (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                 ((-x2 + (R*(R*x2 - 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2)\
)*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                      Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((R*(Power(x2,2) - Power(y2,2))*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (2*R*x2*y2*
                       (-x2 + 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
          ((-x2 + (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) - 
                 ((-y2 + (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2))*
                    (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((-2*R*x2*y2*
                       (-y2 + 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (R*(Power(x2,2) - Power(y2,2))*
                       (-x2 + (R*
                         (R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(_Sqrt(-Power(R,2) + Power(r2,2))*x2 + R*y2))/
                         Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 - _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
double TensionsRight22(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
	STUFF
double tension = 
-((InertiaInv*m1*Power(R,2)*(-((R*(2*v1x*x1 + 2*v1y*y1)*
              (v1y*x1 - v1x*y1 - 
                (R*(v1x*x1 + v1y*y1))/_Sqrt(-Power(R,2) + Power(r1,2)))*
              _Sqrt(Power(y1 - 
                  (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                   Power(r1,2),2) + 
                Power(x1 - (R*
                     (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                   Power(r1,2),2)))/Power(r1,4)) + 
         (R*(g*x1 - (R*(Power(v1x,2) + Power(v1y,2) + g*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)) + 
              (R*(v1x*x1 + v1y*y1)*(2*v1x*x1 + 2*v1y*y1))/
               (2.*Power(-Power(R,2) + Power(r1,2),1.5)))*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
))/Power(r1,2) - (R*w*(2*(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2))*
               (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*_Sqrt(Power(y1 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (R*(v1y*x1 - v1x*y1 - (R*(v1x*x1 + v1y*y1))/
               _Sqrt(-Power(R,2) + Power(r1,2)))*
            (2*(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1))/Power(r1,2))*
               (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                      R*y1)*(2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                      (x1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2)) + 
              2*(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2))*
               (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
                    (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
                 (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                      (y1*(2*v1x*x1 + 2*v1y*y1))/
                       (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/
                  Power(r1,2))))/
          (2.*Power(r1,2)*_Sqrt(Power(y1 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))\
) + (v1x + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(R*v1x + _Sqrt(-Power(R,2) + Power(r1,2))*v1y + 
                 (y1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1x - (R*(-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*(-((R*
                 (-((-2*Power(R,2) + Power(r1,2))*v1x*v1y*x1) - 
                   (-2*Power(R,2) + Power(r1,2))*Power(v1x,2)*y1 - 
                   v1x*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                   _Sqrt(-Power(R,2) + Power(r1,2))*
                    (-2*R*Power(v1y,2)*x1 - 2*R*v1x*v1y*y1 - 
                      2*g*R*x1*y1 - R*v1x*(2*v1x*x1 - 2*v1y*y1)) + 
                   ((2*v1x*x1 + 2*v1y*y1)*
                      (-2*R*v1y*x1*y1 - 
                        R*v1x*(Power(x1,2) - Power(y1,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r1,2))) + 
                   v1y*(4*v1x*Power(x1,3) + 2*Power(R,2)*v1y*y1 + 
                      2*v1y*Power(x1,2)*y1 + 
                      2*v1x*x1*(-Power(R,2) + Power(y1,2))) + 
                   g*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
               (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               (-((-2*Power(R,2) + Power(r1,2))*v1x*x1*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1y*x1*y1 - R*v1x*(Power(x1,2) - Power(y1,2))) \
+ v1y*(Power(x1,4) + Power(R,2)*Power(y1,2) + 
                    Power(x1,2)*(-Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (v1y + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)*
               (2*v1x*x1 + 2*v1y*y1))/Power(r1,4) - 
            (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*v1x) + R*v1y - 
                 (x1*(2*v1x*x1 + 2*v1y*y1))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2)))))/Power(r1,2))*
          (v1y - (R*((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
             Power(r1,2))*(g - 
            (R*((-2*Power(R,2) + Power(r1,2))*Power(v1y,2)*x1 + 
                 (-2*Power(R,2) + Power(r1,2))*v1x*v1y*y1 + 
                 g*(-2*Power(R,2) + Power(r1,2))*x1*y1 + 
                 v1y*x1*y1*(2*v1x*x1 + 2*v1y*y1) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*v1y*x1 - 2*R*Power(v1x,2)*y1 + 
                    R*v1y*(2*v1x*x1 - 2*v1y*y1) + 
                    g*R*(Power(x1,2) - Power(y1,2))) + 
                 ((2*v1x*x1 + 2*v1y*y1)*
                    (-2*R*v1x*x1*y1 + 
                      R*v1y*(Power(x1,2) - Power(y1,2))))/
                  (2.*_Sqrt(-Power(R,2) + Power(r1,2))) - 
                 v1x*(-2*Power(R,2)*v1y*y1 + 2*v1y*Power(x1,2)*y1 + 
                    4*v1y*Power(y1,3) + 
                    2*v1x*x1*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2))) + 
            (R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) \
- v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (2.*Power(r1,4)*Power(-Power(R,2) + Power(r1,2),1.5)) + 
            (2*R*(2*v1x*x1 + 2*v1y*y1)*
               ((-2*Power(R,2) + Power(r1,2))*v1y*x1*y1 + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  (-2*R*v1x*x1*y1 + R*v1y*(Power(x1,2) - Power(y1,2))) - 
                 v1x*(-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                    Power(x1,2)*(Power(R,2) + Power(y1,2)))))/
             (Power(r1,6)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       _Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                R*y2))/Power(r2,2),2) + 
         Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
            Power(r2,2),2)) - (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2)\
)*((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                       R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                        Power(r1,2),2)) + 
                   (x1*(-x1 + 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2))))/_Sqrt(-Power(R,2) + Power(r1,2))))/Power(r1,2) \
+ (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/Power(r1,2))*
          ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1)\
)/Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-x1 + (R*
                        (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                        Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
             Power(r1,2))*((-x1 + 
               (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-x1 + 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       ((R*(2*v2x*x2 + 2*v2y*y2)*
            (v2y*x2 - v2x*y2 - 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,4) - 
         (R*(g*x2 - (R*(Power(v2x,2) + Power(v2y,2) + g*y2))/
               _Sqrt(-Power(R,2) + Power(r2,2)) + 
              (R*(v2x*x2 + v2y*y2)*(2*v2x*x2 + 2*v2y*y2))/
               (2.*Power(-Power(R,2) + Power(r2,2),1.5)))*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2)))/Power(r2,2) + 
         (R*w*(2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2))/Power(r2,2))*
               (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*_Sqrt(Power(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                     R*y2))/Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*(v2y*x2 - v2x*y2 - 
              (R*(v2x*x2 + v2y*y2))/_Sqrt(-Power(R,2) + Power(r2,2)))*
            (2*(y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2))*
               (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                      R*y2)*(2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                      (x2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2)\
) + 2*(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
               (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
                    (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
                 (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                      (y2*(2*v2x*x2 + 2*v2y*y2))/
                       (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))\
))/(2.*Power(r2,2)*_Sqrt(Power(y2 - 
                (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) + 
         (v2x + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(R*v2x + _Sqrt(-Power(R,2) + Power(r2,2))*v2y + 
                 (y2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2x - (R*(-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
             Power(r2,2))*(-((R*
                 (-((-2*Power(R,2) + Power(r2,2))*v2x*v2y*x2) - 
                   (-2*Power(R,2) + Power(r2,2))*Power(v2x,2)*y2 - 
                   v2x*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                   _Sqrt(-Power(R,2) + Power(r2,2))*
                    (-2*R*Power(v2y,2)*x2 - 2*R*v2x*v2y*y2 - 
                      2*g*R*x2*y2 - R*v2x*(2*v2x*x2 - 2*v2y*y2)) + 
                   ((2*v2x*x2 + 2*v2y*y2)*
                      (-2*R*v2y*x2*y2 - 
                        R*v2x*(Power(x2,2) - Power(y2,2))))/
                    (2.*_Sqrt(-Power(R,2) + Power(r2,2))) + 
                   v2y*(4*v2x*Power(x2,3) + 2*Power(R,2)*v2y*y2 + 
                      2*v2y*Power(x2,2)*y2 + 
                      2*v2x*x2*(-Power(R,2) + Power(y2,2))) + 
                   g*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
               (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               (-((-2*Power(R,2) + Power(r2,2))*v2x*x2*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2y*x2*y2 - R*v2x*(Power(x2,2) - Power(y2,2))) + 
                 v2y*(Power(x2,4) + Power(R,2)*Power(y2,2) + 
                    Power(x2,2)*(-Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (v2y + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2)*
               (2*v2x*x2 + 2*v2y*y2))/Power(r2,4) - 
            (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*v2x) + R*v2y - 
                 (x2*(2*v2x*x2 + 2*v2y*y2))/
                  (2.*_Sqrt(-Power(R,2) + Power(r2,2)))))/Power(r2,2))*
          (v2y - (R*((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
             Power(r2,2))*(g - 
            (R*((-2*Power(R,2) + Power(r2,2))*Power(v2y,2)*x2 + 
                 (-2*Power(R,2) + Power(r2,2))*v2x*v2y*y2 + 
                 g*(-2*Power(R,2) + Power(r2,2))*x2*y2 + 
                 v2y*x2*y2*(2*v2x*x2 + 2*v2y*y2) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*v2y*x2 - 2*R*Power(v2x,2)*y2 + 
                    R*v2y*(2*v2x*x2 - 2*v2y*y2) + 
                    g*R*(Power(x2,2) - Power(y2,2))) + 
                 ((2*v2x*x2 + 2*v2y*y2)*
                    (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2)))\
)/(2.*_Sqrt(-Power(R,2) + Power(r2,2))) - 
                 v2x*(-2*Power(R,2)*v2y*y2 + 2*v2y*Power(x2,2)*y2 + 
                    4*v2y*Power(y2,3) + 
                    2*v2x*x2*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2))) + 
            (R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (2.*Power(r2,4)*Power(-Power(R,2) + Power(r2,2),1.5)) + 
            (2*R*(2*v2x*x2 + 2*v2y*y2)*
               ((-2*Power(R,2) + Power(r2,2))*v2y*x2*y2 + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  (-2*R*v2x*x2*y2 + R*v2y*(Power(x2,2) - Power(y2,2))) - 
                 v2x*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                    Power(x2,2)*(Power(R,2) + Power(y2,2)))))/
             (Power(r2,6)*_Sqrt(-Power(R,2) + Power(r2,2))))))/
    (Power(InertiaInv,2)*m1*m2*Power(R,4)*
       _Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                R*y1))/Power(r1,2),2) + 
         Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
            Power(r1,2),2))*_Sqrt(Power(y2 - 
           (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/Power(r2,2),
          2) + Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
            Power(r2,2),2)) - (-(InertiaInv*m1*Power(R,2)*
            _Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                 Power(r1,2),2))) + 
         (R*_Sqrt(Power(y1 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                 Power(r1,2),2) + 
              Power(x1 - (R*(R*x1 + 
                     _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),2))*
            ((x1*(-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
               _Sqrt(Power(y1 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (y1*(-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2)\
))/_Sqrt(Power(y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                    Power(r1,2),2) + 
                 Power(x1 - (R*
                      (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                    Power(r1,2),2)) - 
              (R*((y1*(-y1 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + 
                       _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                       2)) + (x1*
                      (-x1 + 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))))/_Sqrt(-Power(R,2) + Power(r1,2))\
))/Power(r1,2) + (y1 - (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
             Power(r1,2))*((-y1 + 
               (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - 
                 (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*
                    (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                    (-y1 + (R*
                        (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                       Power(r1,2)))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) - 
                 ((-x1 + (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2))*
                    (-(Power(R,2)*Power(y1,2)) + Power(y1,4) + 
                      Power(x1,2)*(Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((R*(Power(x1,2) - Power(y1,2))*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + 
                        _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2),
                        2)) - 
                    (2*R*x1*y1*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))) + 
         (x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/Power(r1,2))*
          ((-x1 + (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                Power(r1,2))/
             _Sqrt(Power(y1 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + R*y1))/
                  Power(r1,2),2) + 
               Power(x1 - (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                  Power(r1,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r1,2))*x1*y1*
                      (-x1 + (R*
                         (R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                    _Sqrt(Power(y1 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                      Power(x1 - 
                        (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2))) + 
                 ((-y1 + (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2))*
                    (Power(x1,4) + Power(R,2)*Power(y1,2) + 
                      Power(x1,2)*(-Power(R,2) + Power(y1,2))))/
                  _Sqrt(Power(y1 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                    Power(x1 - 
                      (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                       Power(r1,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r1,2))*
                  ((-2*R*x1*y1*
                       (-y1 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                        R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)) - 
                    (R*(Power(x1,2) - Power(y1,2))*
                       (-x1 + 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2)))/
                     _Sqrt(Power(y1 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r1,2))*x1) + 
                         R*y1))/Power(r1,2),2) + 
                       Power(x1 - 
                         (R*(R*x1 + _Sqrt(-Power(R,2) + Power(r1,2))*y1))/
                         Power(r1,2),2)))))/
             (Power(r1,4)*_Sqrt(-Power(R,2) + Power(r1,2)))))*
       (-(InertiaInv*m2*Power(R,2)*
            _Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))) - 
         (R*_Sqrt(Power(y2 - (R*
                   (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                 Power(r2,2),2) + 
              Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                 Power(r2,2),2))*
            ((x2*(-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (y2*(-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2)))/
               _Sqrt(Power(y2 - 
                   (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                    Power(r2,2),2) + 
                 Power(x2 - (R*
                      (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                    Power(r2,2),2)) - 
              (R*((y2*(-y2 + 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) + 
                   (x2*(-x2 + 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))))/_Sqrt(-Power(R,2) + Power(r2,2)))\
)/Power(r2,2) + (y2 - (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
             Power(r2,2))*((-y2 + 
               (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                    (-y2 + (R*
                         (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                       Power(r2,2)))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) - 
                 ((-x2 + (R*(R*x2 + 
                        _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2)\
)*(-(Power(R,2)*Power(y2,2)) + Power(y2,4) + 
                      Power(x2,2)*(Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((R*(Power(x2,2) - Power(y2,2))*
                       (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (2*R*x2*y2*
                       (-x2 + 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))) + 
         (x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/Power(r2,2))*
          ((-x2 + (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                Power(r2,2))/
             _Sqrt(Power(y2 - (R*
                    (-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                  Power(r2,2),2) + 
               Power(x2 - (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                  Power(r2,2),2)) - 
            (R*(-(((-2*Power(R,2) + Power(r2,2))*x2*y2*
                      (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2)))/
                    _Sqrt(Power(y2 - 
                        (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                        R*y2))/Power(r2,2),2) + 
                      Power(x2 - 
                        (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2))) + 
                 ((-y2 + (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2))*
                    (Power(x2,4) + Power(R,2)*Power(y2,2) + 
                      Power(x2,2)*(-Power(R,2) + Power(y2,2))))/
                  _Sqrt(Power(y2 - 
                      (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + R*y2))/
                       Power(r2,2),2) + 
                    Power(x2 - 
                      (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                       Power(r2,2),2)) + 
                 _Sqrt(-Power(R,2) + Power(r2,2))*
                  ((-2*R*x2*y2*
                       (-y2 + 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                         Power(r2,2),2)) - 
                    (R*(Power(x2,2) - Power(y2,2))*
                       (-x2 + (R*
                         (R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2)))/
                     _Sqrt(Power(y2 - 
                         (R*(-(_Sqrt(-Power(R,2) + Power(r2,2))*x2) + 
                         R*y2))/Power(r2,2),2) + 
                       Power(x2 - 
                         (R*(R*x2 + _Sqrt(-Power(R,2) + Power(r2,2))*y2))/
                          Power(r2,2),2)))))/
             (Power(r2,4)*_Sqrt(-Power(R,2) + Power(r2,2)))))));
	return tension;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if 0
#define INCL(x) x
#define TensionsRight(a,b,A,B) \
double TensionsRight##a##b(double R, double g, double InertiaInv, double w,\
					 double m1, double x1, double y1, double v1x, double v1y,\
					 double m2, double x2, double y2, double v2x, double v2y)\
{\
	double r1=_Sqrt(x1*x1 + y1*y1);\
	double r2=_Sqrt(x2*x2 + y2*y2);\
double tension = \
	INCL(#include) "TensionsRight" A B ".c"\
	;\
	return tension;\
}

///////////////////////////////////////////////////////////////////////////////
TensionsRight(1,1,"1","1")
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	INCL(include)\
	INCL(x) "TensionsRight"##A##B##".c"\

//Connection on right:
double TensionsRight1(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
double tension = 
;
	return tension;
}
/*//////////////////////////////////////////////////////////////////////////////
	std::wstring bstr(L"TensionsRight");\
	long A = a, B = b;\
	_variant_ va(A), vb(B);\
	std::wstring bstra(va), bstrb(vb);\
	bstr+=bstra;bstr+=bstrb;bstr+=L".c";
//////////////////////////////////////////////////////////////////////////////*/
///////////////////////////////////////////////////////////////////////////////
//Connection on left:
double TensionsLeft1(double R, double g, double InertiaInv, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y)
{
	double r1=_Sqrt(x1*x1 + y1*y1);
	double r2=_Sqrt(x2*x2 + y2*y2);
double tension = 
	#include ".\TensionsLeft1.c"
;
	return tension;
}
/*//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/
///////////////////////////////////////////////////////////////////////////////
typedef double (*_Tension)(double X, double Y, double vx, double vy, double R, double g);
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

#endif

#undef STUFF
#pragma optimize( "", on )

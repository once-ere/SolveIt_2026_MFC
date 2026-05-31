-((-((R*(2*vx*x + 2*vy*y)*(vy*x - vx*y - 
             (R*(vx*x + vy*y))/Sqrt(Power(r,2) - Power(R,2)))*
           Sqrt(Power(y - (R*
                  (-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/Power(r,2),
              2) + Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                Power(r,2),2)))/Power(r,4)) + 
      (R*(g*x - (R*(Power(vx,2) + Power(vy,2) + g*y))/
            Sqrt(Power(r,2) - Power(R,2)) + 
           (R*(vx*x + vy*y)*(2*vx*x + 2*vy*y))/
            (2.*Power(Power(r,2) - Power(R,2),1.5)))*
         Sqrt(Power(y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
              Power(r,2),2) + 
           Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
              Power(r,2),2)))/Power(r,2) + 
      (R*(vy*x - vx*y - (R*(vx*x + vy*y))/Sqrt(Power(r,2) - Power(R,2)))*
         (2*(y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
               Power(r,2))*(vy + 
              (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y)*
                 (2*vx*x + 2*vy*y))/Power(r,4) - 
              (R*(-(Sqrt(Power(r,2) - Power(R,2))*vx) + R*vy - 
                   (x*(2*vx*x + 2*vy*y))/
                    (2.*Sqrt(Power(r,2) - Power(R,2)))))/Power(r,2)) + 
           2*(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/Power(r,2))*
            (vx + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y)*
                 (2*vx*x + 2*vy*y))/Power(r,4) - 
              (R*(R*vx + Sqrt(Power(r,2) - Power(R,2))*vy + 
                   (y*(2*vx*x + 2*vy*y))/
                    (2.*Sqrt(Power(r,2) - Power(R,2)))))/Power(r,2))))/
       (2.*Power(r,2)*Sqrt(Power(y - 
             (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/Power(r,2),2) \
+ Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/Power(r,2),2))) + 
      (vx + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y)*
            (2*vx*x + 2*vy*y))/Power(r,4) - 
         (R*(R*vx + Sqrt(Power(r,2) - Power(R,2))*vy + 
              (y*(2*vx*x + 2*vy*y))/(2.*Sqrt(Power(r,2) - Power(R,2)))))/
          Power(r,2))*(vx - (R*
            (-((Power(r,2) - 2*Power(R,2))*vx*x*y) + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vy*x*y - R*vx*(Power(x,2) - Power(y,2))) + 
              vy*(Power(x,4) + Power(R,2)*Power(y,2) + 
                 Power(x,2)*(-Power(R,2) + Power(y,2)))))/
          (Power(r,4)*Sqrt(Power(r,2) - Power(R,2)))) + 
      (x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/Power(r,2))*
       (-((R*(-((Power(r,2) - 2*Power(R,2))*vx*vy*x) - 
                (Power(r,2) - 2*Power(R,2))*Power(vx,2)*y - 
                vx*x*y*(2*vx*x + 2*vy*y) + 
                Sqrt(Power(r,2) - Power(R,2))*
                 (-2*R*Power(vy,2)*x - 2*R*vx*vy*y - 2*g*R*x*y - 
                   R*vx*(2*vx*x - 2*vy*y)) + 
                ((2*vx*x + 2*vy*y)*
                   (-2*R*vy*x*y - R*vx*(Power(x,2) - Power(y,2))))/
                 (2.*Sqrt(Power(r,2) - Power(R,2))) + 
                vy*(4*vx*Power(x,3) + 2*Power(R,2)*vy*y + 
                   2*vy*Power(x,2)*y + 2*vx*x*(-Power(R,2) + Power(y,2))\
) + g*(Power(x,4) + Power(R,2)*Power(y,2) + 
                   Power(x,2)*(-Power(R,2) + Power(y,2)))))/
            (Power(r,4)*Sqrt(Power(r,2) - Power(R,2)))) + 
         (R*(2*vx*x + 2*vy*y)*
            (-((Power(r,2) - 2*Power(R,2))*vx*x*y) + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vy*x*y - R*vx*(Power(x,2) - Power(y,2))) + 
              vy*(Power(x,4) + Power(R,2)*Power(y,2) + 
                 Power(x,2)*(-Power(R,2) + Power(y,2)))))/
          (2.*Power(r,4)*Power(Power(r,2) - Power(R,2),1.5)) + 
         (2*R*(2*vx*x + 2*vy*y)*
            (-((Power(r,2) - 2*Power(R,2))*vx*x*y) + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vy*x*y - R*vx*(Power(x,2) - Power(y,2))) + 
              vy*(Power(x,4) + Power(R,2)*Power(y,2) + 
                 Power(x,2)*(-Power(R,2) + Power(y,2)))))/
          (Power(r,6)*Sqrt(Power(r,2) - Power(R,2)))) + 
      (vy + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y)*
            (2*vx*x + 2*vy*y))/Power(r,4) - 
         (R*(-(Sqrt(Power(r,2) - Power(R,2))*vx) + R*vy - 
              (x*(2*vx*x + 2*vy*y))/(2.*Sqrt(Power(r,2) - Power(R,2)))))/
          Power(r,2))*(vy - (R*
            ((Power(r,2) - 2*Power(R,2))*vy*x*y + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vx*x*y + R*vy*(Power(x,2) - Power(y,2))) - 
              vx*(-(Power(R,2)*Power(y,2)) + Power(y,4) + 
                 Power(x,2)*(Power(R,2) + Power(y,2)))))/
          (Power(r,4)*Sqrt(Power(r,2) - Power(R,2)))) + 
      (y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/Power(r,2))*
       (g - (R*((Power(r,2) - 2*Power(R,2))*Power(vy,2)*x + 
              (Power(r,2) - 2*Power(R,2))*vx*vy*y + 
              g*(Power(r,2) - 2*Power(R,2))*x*y + 
              vy*x*y*(2*vx*x + 2*vy*y) + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vx*vy*x - 2*R*Power(vx,2)*y + 
                 R*vy*(2*vx*x - 2*vy*y) + g*R*(Power(x,2) - Power(y,2))) \
+ ((2*vx*x + 2*vy*y)*(-2*R*vx*x*y + R*vy*(Power(x,2) - Power(y,2))))/
               (2.*Sqrt(Power(r,2) - Power(R,2))) - 
              vx*(-2*Power(R,2)*vy*y + 2*vy*Power(x,2)*y + 
                 4*vy*Power(y,3) + 2*vx*x*(Power(R,2) + Power(y,2)))))/
          (Power(r,4)*Sqrt(Power(r,2) - Power(R,2))) + 
         (R*(2*vx*x + 2*vy*y)*
            ((Power(r,2) - 2*Power(R,2))*vy*x*y + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vx*x*y + R*vy*(Power(x,2) - Power(y,2))) - 
              vx*(-(Power(R,2)*Power(y,2)) + Power(y,4) + 
                 Power(x,2)*(Power(R,2) + Power(y,2)))))/
          (2.*Power(r,4)*Power(Power(r,2) - Power(R,2),1.5)) + 
         (2*R*(2*vx*x + 2*vy*y)*
            ((Power(r,2) - 2*Power(R,2))*vy*x*y + 
              Sqrt(Power(r,2) - Power(R,2))*
               (-2*R*vx*x*y + R*vy*(Power(x,2) - Power(y,2))) - 
              vx*(-(Power(R,2)*Power(y,2)) + Power(y,4) + 
                 Power(x,2)*(Power(R,2) + Power(y,2)))))/
          (Power(r,6)*Sqrt(Power(r,2) - Power(R,2)))))/
    ((R*Sqrt(Power(y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
              Power(r,2),2) + 
           Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
              Power(r,2),2))*((x*
              (-y + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                 Power(r,2)))/
            Sqrt(Power(y - (R*
                   (-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                 Power(r,2),2) + 
              Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                 Power(r,2),2)) - 
           (y*(-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                 Power(r,2)))/
            Sqrt(Power(y - (R*
                   (-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                 Power(r,2),2) + 
              Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                 Power(r,2),2)) - 
           (R*((y*(-y + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                      Power(r,2)))/
                 Sqrt(Power(y - 
                     (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                      Power(r,2),2) + 
                   Power(x - 
                     (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                      Power(r,2),2)) + 
                (x*(-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                      Power(r,2)))/
                 Sqrt(Power(y - 
                     (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                      Power(r,2),2) + 
                   Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                      Power(r,2),2))))/Sqrt(Power(r,2) - Power(R,2))))/
       Power(r,2) + (y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
          Power(r,2))*((-y + (R*
               (-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/Power(r,2))/
          Sqrt(Power(y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
               Power(r,2),2) + 
            Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
               Power(r,2),2)) - 
         (R*(((Power(r,2) - 2*Power(R,2))*x*y*
                 (-y + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                    Power(r,2)))/
               Sqrt(Power(y - 
                   (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                    Power(r,2),2) + 
                 Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                    Power(r,2),2)) - 
              ((-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                    Power(r,2))*
                 (-(Power(R,2)*Power(y,2)) + Power(y,4) + 
                   Power(x,2)*(Power(R,2) + Power(y,2))))/
               Sqrt(Power(y - 
                   (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                    Power(r,2),2) + 
                 Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                    Power(r,2),2)) + 
              Sqrt(Power(r,2) - Power(R,2))*
               ((R*(Power(x,2) - Power(y,2))*
                    (-y + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2)))/
                  Sqrt(Power(y - 
                      (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2),2) + 
                    Power(x - 
                      (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2),2)) - 
                 (2*R*x*y*(-x + 
                      (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2)))/
                  Sqrt(Power(y - 
                      (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2),2) + 
                    Power(x - 
                      (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2),2)))))/
          (Power(r,4)*Sqrt(Power(r,2) - Power(R,2)))) + 
      (x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/Power(r,2))*
       ((-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/Power(r,2))/
          Sqrt(Power(y - (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
               Power(r,2),2) + 
            Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
               Power(r,2),2)) - 
         (R*(-(((Power(r,2) - 2*Power(R,2))*x*y*
                   (-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                      Power(r,2)))/
                 Sqrt(Power(y - 
                     (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                      Power(r,2),2) + 
                   Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                      Power(r,2),2))) + 
              ((-y + (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                    Power(r,2))*
                 (Power(x,4) + Power(R,2)*Power(y,2) + 
                   Power(x,2)*(-Power(R,2) + Power(y,2))))/
               Sqrt(Power(y - 
                   (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                    Power(r,2),2) + 
                 Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                    Power(r,2),2)) + 
              Sqrt(Power(r,2) - Power(R,2))*
               ((-2*R*x*y*(-y + 
                      (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2)))/
                  Sqrt(Power(y - 
                      (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2),2) + 
                    Power(x - 
                      (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2),2)) - 
                 (R*(Power(x,2) - Power(y,2))*
                    (-x + (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2)))/
                  Sqrt(Power(y - 
                      (R*(-(Sqrt(Power(r,2) - Power(R,2))*x) + R*y))/
                       Power(r,2),2) + 
                    Power(x - (R*(R*x + Sqrt(Power(r,2) - Power(R,2))*y))/
                       Power(r,2),2)))))/
          (Power(r,4)*Sqrt(Power(r,2) - Power(R,2))))))

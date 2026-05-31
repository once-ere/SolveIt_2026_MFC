(Power(r,4)*(Power(r,6)*(Power(vx,2) + Power(vy,2) + g*y) - 
      2*Power(R,3)*(vy*x - vx*y)*
       (Sqrt(Power(r,2) - Power(R,2))*vx*x + R*vy*x - R*vx*y + 
         Sqrt(Power(r,2) - Power(R,2))*vy*y) + 
      Power(r,4)*R*(-2*R*(Power(vx,2) + Power(vy,2)) + 
         Sqrt(Power(r,2) - Power(R,2))*(-(g*x) + vx*w*x + vy*w*y)) + 
      Power(r,2)*R*(g*Power(R,2)*Sqrt(Power(r,2) - Power(R,2))*x + 
         Power(R,3)*(Power(vx,2) + Power(vy,2) - g*y) + 
         2*R*Power(vy*x - vx*y,2) + 
         Sqrt(Power(r,2) - Power(R,2))*
          (-(Power(vx,2)*x*y) + Power(vy,2)*x*y + 
            vx*vy*(Power(x,2) - Power(y,2))))))/
  ((Power(r,2) - Power(R,2))*(Power(r,8)*Sqrt(Power(r,2) - Power(R,2))*
       (1 + InertiaInv*Power(R,2)) + 
      Power(r,2)*Power(R,2)*(Power(x,2) - Power(y,2))*
       (Sqrt(Power(r,2) - Power(R,2))*Power(x,2) - 2*R*x*y - 
         Sqrt(Power(r,2) - Power(R,2))*Power(y,2)) + 
      Power(R,2)*(-(Sqrt(Power(r,2) - Power(R,2))*Power(x,2)) + 2*R*x*y + 
         Sqrt(Power(r,2) - Power(R,2))*Power(y,2))*(Power(x,4) - Power(y,4)))\
)

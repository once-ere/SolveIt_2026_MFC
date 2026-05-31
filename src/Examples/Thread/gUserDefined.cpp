// UserDefined.cpp : Implementation of CIO

#include "stdafx.h"
#include "SolveIt.h"
#include "GenExamp.h"

#include "SolveItView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../ScriptColors.h"

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeIndependentX()
{
///////////////////////////////////////////////////////////////////////////////
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Duffing:a+2*gamma*v + k1*(1+k2*x^2)*x=0\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m1.radius = 0.02;")
	WRITE_LINE("m1.mass = 2./3.;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1"MyBrown2)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("gamma = 0.04;")
	WRITE_LINE("m1.coefficientOfDamping = [ 0, 2*gamma*m1.mass, 0 ];")
	WRITE_LINE("k1 = 4.0*m1.mass;")
	WRITE_LINE("k2 = 0.1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f(y$) {"
"	return -k1*(1.0+k2*y*y)*y;"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.y = \"f\";")
	WRITE_LINE("m1.externalForce.y.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.trajectory.color = m1.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.begin = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.end = 48.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("X = 48.0;")
	WRITE_LINE("Y = 4.0;")
	WRITE_LINE("system.boundingBox = [ 0, X, -Y, Y, -1, 1 ] ;")
	WRITE_LINE("system.translate.z = 0;")
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeDependentX()
{
///////////////////////////////////////////////////////////////////////////////
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"forced Duffing:a+2*gamma*v + k1*(1+k2*x^2)*x=A*cos(w*t)\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("X = 48.0;")
	WRITE_LINE("Y = 4.0;")
	WRITE_LINE("system.boundingBox = [ 0, X, -Y, Y, -1, 1 ] ;")
	WRITE_LINE("system.translate.z = 0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m1.radius = 0.02;")
	WRITE_LINE("m1.mass = 2./3.;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1"PANTONE2728CV)
//	WRITE_LINE("s1"PANTONERubineRedCV)
	WRITE_LINE("m1"MyBrown2)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("gamma = 0.04;")
	WRITE_LINE("m1.coefficientOfDamping = [ 0, 2*gamma*m1.mass, 0 ];")
	WRITE_LINE("k1 = 4.0*m1.mass;")
	WRITE_LINE("k2 = 0.1;")
	WRITE_LINE("A = Y*m1.mass;")
	WRITE_LINE("w = sqrt(k1);")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f(t$, y$) {"
"	return -k1*(1.0+k2*y*y)*y + A*cos(w*t);"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.y = \"f\";")
	WRITE_LINE("m1.externalForce.y.time = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.trajectory.color = m1.color;")
	WRITE_LINE("m1.trajectory.color = [0,0,0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.begin = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.end = 48;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeIndependentY()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"User Defined Time Independent Y\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1"PANTONE2718CV)
//	WRITE_LINE("m2"PANTONE2727CV)
//	WRITE_LINE("m3"PANTONE2728CV)
//	WRITE_LINE("m4"ElectricBlue)
	WRITE_LINE("m1.color = [0.0, 0.0, 1.0];")
//	WRITE_LINE("m2.color = [0.0, 1.0, 0.0];")
	WRITE_LINE("m3.color = [1.0, 0.0, 0.0];")
	WRITE_LINE("m4.color = [0.0, 0.0, 0.0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.radius = 0.2;")
	WRITE_LINE("m2.radius = m1.radius;")
	WRITE_LINE("m3.radius = m1.radius;")
	WRITE_LINE("m4.radius = m1.radius;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("k1 = PI;")
	WRITE_LINE("k2 = 0.1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1(y$) {"
"	return (-y*(k1 + y*y));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2(y$, Vy$) {"
"	return (-y*(k2 + Vy*Vy));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f3(x$, y$, z$) {"
"	r_tmp = sqrt (x*x + y*y + z*z);"
"	return (-y*r_tmp);"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f4(x$, y$, z$, Vx$, Vy$, Vz$) {"
"	r_dot_v_tmp = x*Vx + y*Vy + z*Vz;"
"	return (-y*r_dot_v_tmp);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.y = \"f1\";")
	WRITE_LINE("m2.externalForce.y = \"f2\";")
	WRITE_LINE("m3.externalForce.y = \"f3\";")
	WRITE_LINE("m4.externalForce.y = \"f4\";")
	WRITE_LINE("m1.externalForce.y.time = false;")
	WRITE_LINE("m2.externalForce.y.time = false;")
	WRITE_LINE("m3.externalForce.y.time = false;")
	WRITE_LINE("m4.externalForce.y.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 0];")
	WRITE_LINE("m2.velocity = [1, 1, 0];")
	WRITE_LINE("m3.velocity = [1, 1, 0];")
	WRITE_LINE("m4.velocity = [1, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.trajectory.color = [0.0, 0.0, 1.0];")
//	WRITE_LINE("m2.trajectory.color = [0.0, 1.0, 0.0];")
//	WRITE_LINE("m3.trajectory.color = [1.0, 0.0, 0.0];")
//	WRITE_LINE("m4.trajectory.color = [0.0, 0.0, 0.0];")
	WRITE_LINE("m1.trajectory.color = m1.color;")
	WRITE_LINE("m2.trajectory.color = m2.color;")
	WRITE_LINE("m3.trajectory.color = m3.color;")
	WRITE_LINE("m4.trajectory.color = m4.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.begin = 0.0;")
	WRITE_LINE("m2.record.time.begin = 0.0;")
	WRITE_LINE("m3.record.time.begin = 0.0;")
	WRITE_LINE("m4.record.time.begin = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.end = 15.5;")
	WRITE_LINE("m2.record.time.end = 15.5;")
	WRITE_LINE("m3.record.time.end = 15.5;")
	WRITE_LINE("m4.record.time.end = 15.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.trajectory = true;")
	WRITE_LINE("m2.record.trajectory = true;")
	WRITE_LINE("m3.record.trajectory = true;")
	WRITE_LINE("m4.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ 0, 4, -2, 2, -1, 1 ] ;")
	WRITE_LINE("system.translate.z =  0;")
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeIndependentZ()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"User Defined Time Independent Z\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.radius = 0.01;")
	WRITE_LINE("m2.radius = 0.01;")
	WRITE_LINE("m3.radius = 0.01;")
	WRITE_LINE("m4.radius = 0.01;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 1;")
	WRITE_LINE("m3.mass = 1;")
	WRITE_LINE("m4.mass = 1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1(z) {"
"	return (-z*(k1 + z*z));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2(z, v_z) {"
"	return (-z*(k2 + v_z*v_z));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f3(x$, y$, z$) {"
"	r_tmp = sqrt (x*x + y*y + z*z);"
"	return (-z*r_tmp);"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f4(x$, y$, z$, Vx$, Vy$, Vz$) {"
"	r_dot_v_tmp = x*Vx + y*Vy + z*Vz;"
"	return (-z*r_dot_v_tmp);"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.z = f1 1, false;")
	WRITE_LINE("m2.externalForce.z = f2 2, false;")
	WRITE_LINE("m3.externalForce.z = f3 3, false;")
	WRITE_LINE("m4.externalForce.z = f4 6, false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 1];")
	WRITE_LINE("m2.velocity = [1, 1, 1];")
	WRITE_LINE("m3.velocity = [1, 1, 1];")
	WRITE_LINE("m4.velocity = [1, 1, 1];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.trajectory.color = [0.0, 0.0, 1.0];")
	WRITE_LINE("m2.trajectory.color = [0.0, 1.0, 0.0];")
	WRITE_LINE("m3.trajectory.color = [1.0, 0.0, 0.0];")
	WRITE_LINE("m4.trajectory.color = [0.0, 0.0, 0.0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.trajectory 0.0, 15.5  ,[0.0, 0.0, 1.0];")
	WRITE_LINE("m2.trajectory 0.0, 15.5  ,[0.0, 1.0, 0.0];")
	WRITE_LINE("m3.trajectory 0.0, 15.5  ,[1.0, 0.0, 0.0];")
	WRITE_LINE("m4.trajectory 0.0, 15.5  ,[0.0, 0.0, 0.0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("k1 = PI;")
	WRITE_LINE("k2 = 0.1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -1, 1, 0, 4, -1, 16 ] ;")
	WRITE_LINE("system.translate.z =  5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.rotate = [0, 90, 0];")
}

///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeDependentY()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"User Defined Time Dependent Y\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("k1 = PI;")
	WRITE_LINE("k2 = 0.1;")
	WRITE_LINE("Amp = 0.5;")
	WRITE_LINE("phase = 0.05;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m5 = sphere;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.radius = 0.25;")
	WRITE_LINE("m2.radius = m1.radius;")
	WRITE_LINE("m3.radius = m1.radius;")
	WRITE_LINE("m4.radius = m1.radius;")
	WRITE_LINE("m5.radius = m1.radius;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1"PANTONE512CV)
	WRITE_LINE("m2"PANTONE513CV)
	WRITE_LINE("m3"PANTONE527CV)
	WRITE_LINE("m4"PANTONE259CV)
	WRITE_LINE("m5"PANTONE2592CV)
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1(t) {"
"	return (Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2(t, y) {"
"	return (-y*(k1 + y*y) + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f3(t, y, v_y) {"
"	return (-y*(k2 + v_y*v_y + Amp*sin(k1*t + phase)));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f4(t, x$, y$, z$) {"
"	r_tmp = sqrt (x*x + y*y + z*z);"
"	return (-y*r_tmp + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f5(t, x$, y$, z$, Vx$, Vy$, Vz$) {"
"	r_dot_v_tmp = x*Vx + y*Vy + z*Vz;"
"	return (-y*r_dot_v_tmp + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.externalForce.y = f1 1, true;")
//	WRITE_LINE("m2.externalForce.y = f2 2, true;")
//	WRITE_LINE("m3.externalForce.y = f3 3, true;")
//	WRITE_LINE("m4.externalForce.y = f4 4, true;")
//	WRITE_LINE("m5.externalForce.y = f5 7, true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.y = \"f1\";")
	WRITE_LINE("m2.externalForce.y = \"f2\";")
	WRITE_LINE("m3.externalForce.y = \"f3\";")
	WRITE_LINE("m4.externalForce.y = \"f4\";")
	WRITE_LINE("m5.externalForce.y = \"f5\";")
	WRITE_LINE("m1.externalForce.y.time = true;")
	WRITE_LINE("m2.externalForce.y.time = true;")
	WRITE_LINE("m3.externalForce.y.time = true;")
	WRITE_LINE("m4.externalForce.y.time = true;")
	WRITE_LINE("m5.externalForce.y.time = true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 0];")
	WRITE_LINE("m2.velocity = [1, 1, 0];")
	WRITE_LINE("m3.velocity = [1, 1, 0];")
	WRITE_LINE("m4.velocity = [1, 1, 0];")
	WRITE_LINE("m5.velocity = [1, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.trajectory.color = m1.color;")
	WRITE_LINE("m2.trajectory.color = m2.color;")
	WRITE_LINE("m3.trajectory.color = m3.color;")
	WRITE_LINE("m4.trajectory.color = m4.color;")
	WRITE_LINE("m5.trajectory.color = m5.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.begin = 0.0;")
	WRITE_LINE("m2.record.time.begin = 0.0;")
	WRITE_LINE("m3.record.time.begin = 0.0;")
	WRITE_LINE("m4.record.time.begin = 0.0;")
	WRITE_LINE("m5.record.time.begin = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.time.end = 15.5;")
	WRITE_LINE("m2.record.time.end = 15.5;")
	WRITE_LINE("m3.record.time.end = 15.5;")
	WRITE_LINE("m4.record.time.end = 15.5;")
	WRITE_LINE("m5.record.time.end = 15.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.record.trajectory = true;")
	WRITE_LINE("m2.record.trajectory = true;")
	WRITE_LINE("m3.record.trajectory = true;")
	WRITE_LINE("m4.record.trajectory = true;")
	WRITE_LINE("m5.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ 0, 10, -5, 5, -1, 1 ] ;")
	WRITE_LINE("system.translate.z =  0;")
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnUserDefinedTimeDependentZ()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"User Defined Time Dependent Z\";")
	WRITE_LINE("system.dimension = 2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m5 = sphere;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1(t) {"
"	return (Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2(t, z) {"
"	return (-z*(k1 + z*z) + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f3(t, z, v_z) {"
"	return (-z*(k2 + v_z*v_z + Amp*sin(k1*t + phase)));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f4(t, x$, y$, z$) {"
"	r_tmp = sqrt (x*x + y*y + z*z);"
"	return (-z*r_tmp + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f5(t, x$, y$, z$, Vx$, Vy$, Vz$) {"
"	r_dot_v_tmp = x*Vx + y*Vy + z*Vz;"
"	return (-z*r_dot_v_tmp + Amp*sin(k1*t + phase));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.z = f1 1, true;")
	WRITE_LINE("m2.externalForce.z = f2 2, true;")
	WRITE_LINE("m3.externalForce.z = f3 3, true;")
	WRITE_LINE("m4.externalForce.z = f4 4, true;")
	WRITE_LINE("m5.externalForce.z = f5 7, true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.velocity = [1, 1, 1];")
	WRITE_LINE("m2.velocity = [1, 1, 1];")
	WRITE_LINE("m3.velocity = [1, 1, 1];")
	WRITE_LINE("m4.velocity = [1, 1, 1];")
	WRITE_LINE("m5.velocity = [1, 1, 1];")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.trajectory 0.0, 15.5  ,[0.0, 0.0, 1.0];")
	WRITE_LINE("m2.trajectory 0.0, 15.5  ,[0.0, 1.0, 0.0];")
	WRITE_LINE("m3.trajectory 0.0, 15.5  ,[1.0, 0.0, 0.0];")
	WRITE_LINE("m4.trajectory 0.0, 15.5  ,[0.0, 0.0, 0.0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("k1 = PI;")
	WRITE_LINE("k2 = 0.1;")
	WRITE_LINE("Amp = 0.5;")
	WRITE_LINE("phase = 0.05;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -2.5, 2.5, 0, 5, -1, 16 ] ;")
	WRITE_LINE("system.translate.z =  5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.rotate = [0, 90, 0];")
}

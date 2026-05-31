// Examples2.cpp : Implementation of CIO
#include "stdafx.h"
#include "SolveIt.h"
#include "GenExamp.h"

#include "SolveItView.h"
#include "GLView.h"
#include "MainFrm.h"
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#include "HTML_Content.h"

#include "../ScriptColors.h"

//	WRITE_LINE("x = sphere;")
//	WRITE_LINE("x.position = x.position+x.radius*[  1, 0, 0 ];")
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnOrbitalMechanics()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	Vector3D velocityInterceptor( -0.86602540378443864676372317075294, 0.5, 0 );
	double scale = 0.78;
	Vector3D velocityPhasing = scale*velocityInterceptor;
	TCHAR buf[256];
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	oss << "phasing.velocity = " << velocityPhasing.x << " "  << velocityPhasing.y << " "  << velocityPhasing.z << ends;

	WRITE_LINE("system.name = \"Orbital Mechanics\";")

	WRITE_LINE("phasing = sphere;")
	WRITE_LINE("target = sphere;")
	WRITE_LINE("interceptor = sphere;")

	WRITE_LINE("phasing.color =[ 1, 0, 1 ];")
	WRITE_LINE("interceptor.color =[ 1, 0.4, 0 ];")
	WRITE_LINE("target.color =[ 0.4, 0.8, 1 ];")

	WRITE_LINE("phasing.radius =  .08;")
	WRITE_LINE("target.radius =  .14;")
	WRITE_LINE("interceptor.radius =  .14;")

	WRITE_LINE("phasing.position = [  -1, 0, 0 ];")
	WRITE_LINE("interceptor.position = phasing.position+phasing.radius*[  1, 0, 0 ];")
	WRITE_LINE("target.position = [  -0.5, -0.86602540378443864676372317075294, 0 ];")

//	WRITE_LINE("phasing.mass =  2;")
//	WRITE_LINE("target.mass =  2;")
//	WRITE_LINE("interceptor.mass =  2;")


	WRITE_LINE("phasing.velocity = [0, -0.78, 0];")
	WRITE_LINE("interceptor.velocity = [0, -1, 0];")
	WRITE_LINE("target.velocity = [0.86602540378443864676372317075294, -0.5, 0];")

	WRITE_LINE("interceptor.momentum.scale = 3;")
	WRITE_LINE("phasing.momentum.scale = 3;")
	WRITE_LINE("target.momentum.scale = 3;")

	WRITE_LINE("interceptor.force.scale = 3;")
	WRITE_LINE("phasing.force.scale = 3;")
	WRITE_LINE("target.force.scale = 3;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("phasing.record.time.begin = 0.0;")
	WRITE_LINE("phasing.record.time.end = 3.8;")
	WRITE_LINE("phasing.trajectory.color = phasing.color;")
	WRITE_LINE("phasing.record.trajectory = true;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("target.record.time.begin = 0.0;")
	WRITE_LINE("target.record.time.end = 6.5;")
	WRITE_LINE("target.trajectory.color = target.color;")
	WRITE_LINE("target.record.trajectory = true;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("interceptor.record.time.begin = 0.0;")
	WRITE_LINE("interceptor.record.time.end = 6.5;")
	WRITE_LINE("interceptor.trajectory.color = interceptor.color;")
	WRITE_LINE("interceptor.record.trajectory = true;")

	WRITE_LINE("Sun = sphere;")
	WRITE_LINE("Sun.radius =  .2;")
	WRITE_LINE("Sun.color =[ 0.8, 1, 0 ];")
	WRITE_LINE("Sun.inverseMass =  0;")
	WRITE_LINE("Sun.centralForce = 20;")
	WRITE_LINE("system.gravitationalCoupling = 0.1;")
	WRITE_LINE("system.timeStepSize = 0.005;")

	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  2 , 42  ];")
	WRITE_LINE("system.translate.z =  -2.68;")

}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnSunEarthMoon()
{

	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Sun-Earth-Moon (not to scale)\";")

	WRITE_LINE("Sun = sphere;")
	WRITE_LINE("Earth = sphere;")
	WRITE_LINE("Moon = sphere;")

	WRITE_LINE("Sun.radius =  .2;")
	WRITE_LINE("Earth.radius = 0.08;")
	WRITE_LINE("Moon.radius =  0.04;")

	WRITE_LINE("Sun.color =[ 0.8, 1, 0 ];")
	WRITE_LINE("Moon.color =[  0.4, 0.6, 0 ];")
	WRITE_LINE("Earth.color =[ 0.4, 0.8, 1 ];")


	WRITE_LINE("Sun.position = [  0.0, 0.0, 0.0 ];")
	WRITE_LINE("Moon.position = [   2.25, 0, 0 ];")
	WRITE_LINE("Earth.position = [  2.00, 0.0, 0.0 ];")

	WRITE_LINE("Sun.inverseMass =  0;")
	WRITE_LINE("Earth.mass = 1.00;")
	WRITE_LINE("Moon.mass =  0.125;")

	WRITE_LINE("Moon.velocity  = [0, 2, 0];")
	WRITE_LINE("Earth.velocity = [0, 1, 0];")


///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Earth.record.time.begin = 0.0;")
	WRITE_LINE("Earth.record.time.end = 20.5;")
	WRITE_LINE("Earth.trajectory.color = Earth.color;")
	WRITE_LINE("Earth.record.trajectory = true;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Moon.record.time.begin = 0.0;")
	WRITE_LINE("Moon.record.time.end = 20.5;")
	WRITE_LINE("Moon.trajectory.color = Moon.color;")
	WRITE_LINE("Moon.record.trajectory = true;")


	WRITE_LINE("Sun.centralForce = 2;")
	WRITE_LINE("system.gravitationalCoupling = 1;")
	WRITE_LINE("system.timeStepSize = 0.025;")
//	WRITE_LINE("system.translate.z =  -16;")


	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  2 , 42  ];")
	WRITE_LINE("system.translate.z =  -2.68;")
	
}

///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnEarthMoon()
{

	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Earth-Moon\";")

	WRITE_LINE("Sun = sphere;")
	WRITE_LINE("Earth = sphere;")
	WRITE_LINE("Moon = sphere;")

	WRITE_LINE("Sun.radius =  .2;")
	WRITE_LINE("Earth.radius = 0.08;")
	WRITE_LINE("Moon.radius =  0.04;")

	WRITE_LINE("Sun.color =[ 0.8, 1, 0 ];")
	WRITE_LINE("Moon.color =[  0.4, 0.6, 0 ];")
	WRITE_LINE("Earth.color =[ 0.4, 0.8, 1 ];")


	WRITE_LINE("Sun.position = [  0.0, 0.0, 0.0 ];")
	WRITE_LINE("Moon.position = [   2.25, 0, 0 ];")
	WRITE_LINE("Earth.position = [  2.00, 0.0, 0.0 ];")

	WRITE_LINE("Sun.inverseMass =  0;")
	WRITE_LINE("Earth.mass = 1.00;")
	WRITE_LINE("Moon.mass =  0.125;")

	WRITE_LINE("Moon.velocity  = [0, 2, 0];")
	WRITE_LINE("Earth.velocity = [0.0, 0.0, 0.0];")


///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Earth.record.time.begin = 0.0;")
	WRITE_LINE("Earth.record.time.end = 20.5;")
	WRITE_LINE("Earth.trajectory.color = Earth.color;")
	WRITE_LINE("Earth.record.trajectory = true;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Moon.record.time.begin = 0.0;")
	WRITE_LINE("Moon.record.time.end = 20.5;")
	WRITE_LINE("Moon.trajectory.color = Moon.color;")
	WRITE_LINE("Moon.record.trajectory = true;")


//	WRITE_LINE("Sun.centralForce = 0;")
	WRITE_LINE("system.gravitationalCoupling = 1;")
	WRITE_LINE("system.timeStepSize = 0.025;")

///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  2 , 42  ];")
	WRITE_LINE("system.translate.z =  -2.68;")

}
///////////////////////////////////////////////////////////////////////////////

void CGenExamp::OnCannonBall()
{
//	theApp.Post ThreadMessage(WM_APP_Initialize, 0, 0);
	WM_APP_INIT
//	WaitForSingleObject(g_EventInitialize, INFINITE);


	WRITE_LINE("system.name = \"Cannon Balls\";")
	WRITE_LINE("system.internalForce = false;")	//off;")
	WRITE_LINE("One = sphere;")
	WRITE_LINE("Two = sphere;")
	WRITE_LINE("Target = sphere;")
	WRITE_LINE("One.position  =   [ -1,  0,      0 ];")
	WRITE_LINE("Two.position  =   [ -1,  0,      0 ];")
//	WRITE_LINE("Target.position = [  1, -0.1499, 0 ];")
	WRITE_LINE("Target.inverseMass = 0;")

	WRITE_LINE("One.color = [ 0.5, 0.4, 0 ];")
	WRITE_LINE("Two.color = [  .8, .8, .6  ];")
	WRITE_LINE("Target.color = [ 0, 0.2, 1 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("One.radius = .12 ;")
	WRITE_LINE("Two.radius = .079;")
	WRITE_LINE("Target.radius = .15;")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("One.velocity = [1, 4, 0];")
	WRITE_LINE("Two.velocity = [4, 1, 0];")

	WRITE_LINE("One.position.show = 40 ;")
	WRITE_LINE("One.velocity.show = 40 ;")
	WRITE_LINE("Two.acceleration.show = true;")//
	WRITE_LINE("One.force.scale = 0.3;")
	WRITE_LINE("Two.force.scale = 0.2;")
	WRITE_LINE("One.momentum.scale = 0.5;")
	WRITE_LINE("Two.momentum.scale = 0.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("One.record.time.begin = 0.0;")
	WRITE_LINE("One.record.time.end = 2.5;")
	WRITE_LINE("One.trajectory.color = One.color;")
	WRITE_LINE("One.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Two.record.time.begin = 0.0;")
	WRITE_LINE("Two.record.time.end = 0.55;")
	WRITE_LINE("Two.trajectory.color = Two.color;")
	WRITE_LINE("Two.record.trajectory = true;")


	WRITE_LINE("system.externalGravitationalField.y = -4;")
	WRITE_LINE("system.timeStepSize = 0.005;")
	WRITE_LINE("system.enforceRigidBoundary = 2;")//floor
	WRITE_LINE("system.boundingBox = [ -1.4 ,  1.4 ,  -1.4 ,  1.4 ,  2. ,  10 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  -1.08 ,  -3.05 ] ;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Target.position = [ One.position.x + (One.velocity*One.velocity)* sin(2*(atan2(One.velocity.y,One.velocity.x)))/abs(system.externalGravitationalField.y), -Target.radius, 0 ];")
///////////////////////////////////////////////////////////////////////////////
return;
}
	
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnSpline()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.data.file=\"work/t.dat\";")

	_bstr_t bstrQ=\
L"function Spline(x$)"
L"\r\n{"
L"\r\nreturn spline(x);"
L"\r\n}";

	Fire_WriteCommand(bstrQ);
	WRITE_LINE("plot(\"Spline\", \" [x,-5, 5], [y ,  -1, 1] \");");
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnCrossProduct()
{

	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Vector Cross Product\";")
//	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("A = vectorAnalysisObject;")
	WRITE_LINE("B = vectorAnalysisObject;")

	WRITE_LINE("A.position = [  1.54714, -0.598759, 0.4 ];")
	WRITE_LINE("B.position = [  1.32689,  0.621512, -1 ];")
	WRITE_LINE("C = A * B;")

	WRITE_LINE("A.color = [ 0.65, 0.40, 0.13 ];")//brown
	WRITE_LINE("B.color = [ 0.60, 0.40, 1.00 ];")
	WRITE_LINE("C.color = [ 1.00, 0.00, 0.00 ];")

//	WRITE_LINE("system.translate.z =  2;")
	WRITE_LINE("system.rotate = [4, -8,0];")


//	_bstr_t bstr = L"\
//	Try a command such as 'D = A * C' (without the quotes)\r\n";
//	Fire_WriteResponse(bstr);Fire_WriteResponse(0);

	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5 ] ;")
//	WRITE_LINE("system.translate.z =  5;")
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnSum()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
//	WRITE_LINE("system.re set;")
	WRITE_LINE("system.name = \"Vector Sum\";")
//	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("A = vectorAnalysisObject;")
	WRITE_LINE("B = vectorAnalysisObject;")

	WRITE_LINE("A.position = [  0.5, 0.5, 0.5 ];")
	WRITE_LINE("B.position = [  1.2, 0.5, 0.1 ];")
	WRITE_LINE("C = A + B;")

	WRITE_LINE("A.color = [ 0.85, 0.40, 0.13 ];")
	WRITE_LINE("B.color = [ 0.60, 0.40, 1.00 ];")
	WRITE_LINE("C.color = [ 1.00, 0.00, 0.00 ];")

	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5 ] ;")
//	WRITE_LINE("system.translate.z =  5;")

	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnDifference()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Vector Difference\";")
//	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("A = vectorAnalysisObject;")
	WRITE_LINE("B = vectorAnalysisObject;")

	WRITE_LINE("A.position = [  1, 1, 1 ];")
	WRITE_LINE("B.position = [  2, 1, 0.1 ];")
	WRITE_LINE("C = A - B;")

	WRITE_LINE("A.color = [ 0.40, 0.85, 0.13 ];")
	WRITE_LINE("B.color = [ 0.60, 0.40, 1.00 ];")
	WRITE_LINE("C.color = [ 1.00, 0.00, 0.00 ];")

	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5 ] ;")
//	WRITE_LINE("system.translate.z =  5;")
}

///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnPendulumAndFixedCharge()
{
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Pendulum and Fixed Charge\";")
	WRITE_LINE("One = sphere;")
	WRITE_LINE("One.radius = .1;")
	WRITE_LINE("One.inverseMass = 0;")
	WRITE_LINE("One.charge = 1;")
	WRITE_LINE("One.position =  [  -0.8,  -0.2, 0 ];")
	WRITE_LINE("One.color = [ 0.1, 0.2, 0.4 ];")

	WRITE_LINE("Two = sphere;")
	WRITE_LINE("Two.radius = .1;")
	WRITE_LINE("Two.charge = 0.1;")
	WRITE_LINE("Two.constraint.[0, 0, 0] == [0, 1, 0], 1;")
	WRITE_LINE("Two.position = [  0.98, 0.80100251257867600905310403579976, 0 ];")
	WRITE_LINE("Two.color = [ 1, 0, 1 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Two.record.time.begin = 0.0;")
	WRITE_LINE("Two.record.time.end = 9.0;")
	WRITE_LINE("Two.record.period = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"9.05; system.stop;\";")
	WRITE_LINE("system.deferredCommand = \"9.05; Two.analyze = 'period: [1, 0, 0]';\";")
	WRITE_LINE("system.deferredCommand = \"9.05; Two.analyze = 'period: [0, 1, 0]';\";")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("system.externalGravitationalField.y =  -3;")
	WRITE_LINE("system.timeStepSize = 0.01;")

	_bstr_t bstr = L"\
Charged Simple Pendulum Near a Fixed Charge\r\n\
    To calculate the period execute:\r\n\
\r\n\
Two.analyze \"period: 1, 0, 0\"\r\n\
or\r\n\
Two.analyze \"period: 0, 1, 0\"\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 9.\r\n\
";
//	Fire_WriteResponse(bstr);Fire_WriteResponse(0);
	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnPendulumAndPlanarCharge()
{
	//Fire_ClearUserDefs()PANTONEYellowCV;
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Pendulum and Planar Charge\";")

//	WRITE_LINE("One = sphere;")
	WRITE_LINE("One = block;")
	WRITE_LINE("One.width =  .5;")
	WRITE_LINE("One.height = .4;")
	WRITE_LINE("One.depth =  .6;")
	WRITE_LINE("One.mass = 1;")
	WRITE_LINE("One.charge = 0.6;")
	WRITE_LINE("One.position  = [  0.2,  -0.321,  0];")
//	WRITE_LINE("One.position.x [ =  0.2 ];")
//	WRITE_LINE("One.position.y [ =  0.2 ];")
	WRITE_LINE("One"PANTONEOrange021CV)//Gold

	WRITE_LINE("One.constraint.linear == [0, 1, 0],[ 0, -0.321, 0];")
	WRITE_LINE("One.force.scale = 0.5;")

	WRITE_LINE("Two = sphere;")
	WRITE_LINE("Two.radius = .2;")
	WRITE_LINE("Two.mass = 3;")
	WRITE_LINE("Two.charge = 2;")//
	WRITE_LINE("Two"PANTONE527CV)//PANTONERhodamineRedCV
	WRITE_LINE("Two.drawStyle =  0;")

	WRITE_LINE("lengthPendulum = 1.0;")
	WRITE_LINE("X_Pendulum = 0.9;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Two.position  = [X_Pendulum,  lengthPendulum - sqrt(lengthPendulum^2 - X_Pendulum^2), 0 ];")


	WRITE_LINE(" Two.constraint.[0, 0, 0] == [0, lengthPendulum, 0], lengthPendulum;")

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*	WRITE_LINE("Two.record.time.begin = 0.0;")
	WRITE_LINE("Two.record.time.end = 9.0;")
	WRITE_LINE("Two.record.period = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"9.05; system.stop;\";")
	WRITE_LINE("system.deferredCommand = \"9.05; Two.analyze = 'period: [1, 0, 0]';\";")
	WRITE_LINE("system.deferredCommand = \"9.05; Two.analyze = 'period: [0, 1, 0]';\";")
///////////////////////////////////////////////////////////////////////////////
*/
	WRITE_LINE("system.externalGravitationalField.y =  -3;")
	WRITE_LINE("system.timeStepSize = 0.01;")
	WRITE_LINE("system.enforceRigidBoundary = 1;")//on

	WRITE_LINE("system.boundingBox = [ -2, 2, -2, 2, 2.05, 20 ] ;")
	WRITE_LINE("system.translate = [ 0   ,  -1.3 ,  -3 ] ;")
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnTrapping()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"\";")

	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::On6Body() {
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Chain of Six\";")
// use default sphere radius = 0.1
	WRITE_LINE("m1 = block;")
	WRITE_LINE("m2 = block;")
	WRITE_LINE("m3 = block;")
	WRITE_LINE("m4 = block;")
	WRITE_LINE("m5 = block;")
	WRITE_LINE("m6 = block;")
/*
*/
	WRITE_LINE("m7 = block;")
	WRITE_LINE("m8 = block;")
	WRITE_LINE("m9 = block;")
	WRITE_LINE("m10 = block;")
	WRITE_LINE("m11 = block;")
	WRITE_LINE("m12 = block;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.drawStyle=0;")
	WRITE_LINE("m2.drawStyle=0;")
	WRITE_LINE("m3.drawStyle=0;")
	WRITE_LINE("m4.drawStyle=0;")
	WRITE_LINE("m5.drawStyle=0;")
	WRITE_LINE("m6.drawStyle=0;")
/*
*/
	WRITE_LINE("m7.drawStyle=0;")
	WRITE_LINE("m8.drawStyle=0;")
	WRITE_LINE("m9.drawStyle=0;")
	WRITE_LINE("m10.drawStyle=0;")
	WRITE_LINE("m11.drawStyle=0;")
	WRITE_LINE("m12.drawStyle=0;")

///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m3.color = [ 0.74901960784313725490196078431373, 0.63529411764705882352941176470588, 0.38431372549019607843137254901961 ];")
	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m4.color = [0,1,0];")
	WRITE_LINE("m5"PANTONE527CV)
	WRITE_LINE("m6"Gold)
/*
*/
	WRITE_LINE("m7"PANTONERhodamineRedCV)
	WRITE_LINE("m8"PANTONEOrange021CV)
	WRITE_LINE("m9"PANTONEYellowCV)
	WRITE_LINE("m10.color = [0,1,0];")
	WRITE_LINE("m11"PANTONE527CV)
	WRITE_LINE("m12"Gold)

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.width =  .4;")
	WRITE_LINE("m1.height = .2;")
	WRITE_LINE("m1.depth =  .6;")

	WRITE_LINE("m2.width =  .6;")
	WRITE_LINE("m2.height = .2;")
	WRITE_LINE("m2.depth =  .4;")

	WRITE_LINE("m3.width =  .4;")
	WRITE_LINE("m3.height = .2;")
	WRITE_LINE("m3.depth =  .6;")

	WRITE_LINE("m4.width =  .4;")
	WRITE_LINE("m4.height = .2;")
	WRITE_LINE("m4.depth =  .6;")

	WRITE_LINE("m5.width =  .6;")
	WRITE_LINE("m5.height = .4;")
	WRITE_LINE("m5.depth =  .2;")

	WRITE_LINE("m6.width =  .4;")
	WRITE_LINE("m6.height = .6;")
	WRITE_LINE("m6.depth =  .2;")

/*
*/
	WRITE_LINE("m7.width =  .6;")
	WRITE_LINE("m7.height = .4;")
	WRITE_LINE("m7.depth =  .2;")

	WRITE_LINE("m8.width =  .2;")
	WRITE_LINE("m8.height = .6;")
	WRITE_LINE("m8.depth =  .4;")

	WRITE_LINE("m9.width =  .4;")
	WRITE_LINE("m9.height = .2;")
	WRITE_LINE("m9.depth =  .6;")

	WRITE_LINE("m10.width =  .6;")
	WRITE_LINE("m10.height = .4;")
	WRITE_LINE("m10.depth =  .2;")

	WRITE_LINE("m11.width =  .2;")
	WRITE_LINE("m11.height = .4;")
	WRITE_LINE("m11.depth =  .6;")

	WRITE_LINE("m12.width =  .4;")
	WRITE_LINE("m12.height = .2;")
	WRITE_LINE("m12.depth =  .6;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.position= [ 0.5*m1.width, 0, 0 ];")
	WRITE_LINE("m2.position= m1.position;")
	WRITE_LINE("m3.position= m1.position;")
	WRITE_LINE("m4.position= m1.position;")
	WRITE_LINE("m5.position= m1.position;")
	WRITE_LINE("m6.position= m1.position;")
/*
*/
	WRITE_LINE("m7.position= m1.position;")
	WRITE_LINE("m8.position= m1.position;")
	WRITE_LINE("m9.position= m1.position;")
	WRITE_LINE("m10.position= m1.position;")
	WRITE_LINE("m11.position= m1.position;")
	WRITE_LINE("m12.position= m1.position;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m2.position.x = m1.position.x + 0.5*(m1.width + m2.width);")
	WRITE_LINE("m3.position.x = m2.position.x + 0.5*(m2.width + m3.width);")
	WRITE_LINE("m4.position.x = m3.position.x + 0.5*(m3.width + m4.width);")
	WRITE_LINE("m5.position.x = m4.position.x + 0.5*(m4.width + m5.width);")
	WRITE_LINE("m6.position.x = m5.position.x + 0.5*(m5.width + m6.width);")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m7.position.x = m6.position.x + 0.5*(m6.width + m7.width);")
	WRITE_LINE("m8.position.x = m7.position.x + 0.5*(m7.width + m8.width);")
	WRITE_LINE("m9.position.x = m8.position.x + 0.5*(m8.width + m9.width);")
	WRITE_LINE("m10.position.x = m9.position.x + 0.5*(m9.width + m10.width);")
	WRITE_LINE("m11.position.x = m10.position.x + 0.5*(m10.width + m11.width);")
	WRITE_LINE("m12.position.x = m11.position.x + 0.5*(m11.width + m12.width);")
/*
*/

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.constraint.[-0.5*m1.width, 0., 0] ==    { [0., 0., 0], 1  ;")
	WRITE_LINE("m2.constraint.[-0.5*m2.width, 0., 0] == m1.constraint.[0.5*m1.width, 0., 0], | m2.position - m1.position |;")
	WRITE_LINE("m3.constraint.[-0.5*m3.width, 0., 0] == m2.constraint.[0.5*m2.width, 0., 0], | m3.position - m2.position |;")
	WRITE_LINE("m4.constraint.[-0.5*m4.width, 0., 0] == m3.constraint.[0.5*m3.width, 0., 0], | m4.position - m3.position |;")
	WRITE_LINE("m5.constraint.[-0.5*m5.width, 0., 0] == m4.constraint.[0.5*m4.width, 0., 0], | m5.position - m4.position |;")
	WRITE_LINE("m6.constraint.[-0.5*m6.width, 0., 0] == m5.constraint.[0.5*m5.width, 0., 0], | m6.position - m5.position |;")
	WRITE_LINE("m7.constraint.[-0.5*m7.width, 0., 0] == m6.constraint.[0.5*m6.width, 0., 0], | m2.position - m1.position |;")
	WRITE_LINE("m8.constraint.[-0.5*m8.width, 0., 0] == m7.constraint.[0.5*m7.width, 0., 0], | m3.position - m2.position |;")
	WRITE_LINE("m9.constraint.[-0.5*m9.width, 0., 0] == m8.constraint.[0.5*m8.width, 0., 0], | m4.position - m3.position |;")
	WRITE_LINE("m10.constraint.[-0.5*m10.width, 0., 0] == m9.constraint.[0.5*m9.width, 0., 0], | m5.position - m4.position |;")
	WRITE_LINE("m11.constraint.[-0.5*m11.width, 0., 0] == m10.constraint.[0.5*m10.width, 0., 0], | m6.position - m5.position |;")
	WRITE_LINE("m12.constraint.[-0.5*m12.width, 0., 0] == m11.constraint.[0.5*m11.width, 0., 0], | m6.position - m5.position |;")
/*
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -9.81;")
	WRITE_LINE("system.timeStepSize = 0.05;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  5 ,  10 ];")
	WRITE_LINE("system.translate   = [ 0 ,  2 ,  2.2 ];")

	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnTwoNonShpericalBody()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"TwoNonShpericalBody\";")

	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnThreeChargedBodies()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Three Charged Bodies\";")

	WRITE_LINE("s1 = sphere;")
	WRITE_LINE("s2 = sphere;")
	WRITE_LINE("s3 = sphere;")
	WRITE_LINE("s1.color = [ 1, 0, 1 ];")
	WRITE_LINE("s2.color = [ 1, 0.4, 0 ];")
	WRITE_LINE("s3.color = [ 0.4, 0.8, 1 ];")
	WRITE_LINE("s1.position = [  -.625,  0, 0 ];")
	WRITE_LINE("s2.position = [   .615,  0, 0 ];")
	WRITE_LINE("s3.position = [    0, .5, 0 ];")
	WRITE_LINE("s1.radius =  .10;")
	WRITE_LINE("s2.radius =  .18;")
	WRITE_LINE("s3.radius =  .06;")
	WRITE_LINE("s1.drawStyle =  0;")
	WRITE_LINE("s2.drawStyle =  0;")
	WRITE_LINE("s3.drawStyle =  0;")
	WRITE_LINE("s1.mass =  2;")
	WRITE_LINE("s2.mass =  4;")
	WRITE_LINE("s3.mass =  0.5;")
	WRITE_LINE("s1.charge =  -2;")
	WRITE_LINE("s2.charge =  4;")
	WRITE_LINE("s3.charge =  -0.5;")

	WRITE_LINE("system.boundingBox = [ -1.2, 1.2, -1.2, 1.2, 5, 10 ] ;")
////	WRITE_LINE("system.fieldOfView = 23;")

	WRITE_LINE("system.fieldLines {-1.0, -1.0, -1.0}{1.0, 1.0, 1.0}{ 0.25, 0.25, 0.25};")
	WRITE_LINE("system.timeStepSize = 0.005;")
	
	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnChain()
{
//	theApp.Initialize(_T("Mandelbrot.htm"), _T("Mandelbrot Set"));WaitForSingleObject(g_EventInitialize, INFINITE);

//	AfxGetApp()->PostThreadMessage(WM_APP_Initialize, (WPARAM) dupString("Mandelbrot.htm"), (LPARAM) dupString("Mandelbrot Set"));
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WM_APP_INIT2("Mandelbrot.htm", "Mandelbrot Set")


	WRITE_LINE("system.name = \"Mandelbrot Set\";")
	WRITE_LINE("system.boundingBox = [-2.0, 1.0,-1.5, 1.5,  -1, 1];")
	WRITE_LINE("system.translate = [ 0. ,  0.0 ,  0.0 ];")
	WRITE_LINE("system.background.show = true;")
	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("system.scale = 1;")
	WRITE_LINE("system.scale = 2;")
	
	WRITE_LINE("system.map = \"Mandelbrot Set\";")
//	OnWriteResponse(bstr);OnWriteResponse(0);
//	WRITE_LINE("system.twoDW.verDyn = true;")
	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CInfoDialog* pDialog = new CInfoDialog(_T("Mandelbrot.htm"), _T("Mandelbrot Set"));
	pFrame->m_CleanupOnInitialize.Add(pDialog->m_hWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));



	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();	
	gl->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
	gl->m_bPerspectiveTransformation=false;
	gl->SetPerspectiveOrFrustum();


	return;
	
	
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Chain of Five\";")

// use default sphere radius = 0.1

	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m5 = sphere;")

//	WRITE_LINE("m3.color = [ 0.74901960784313725490196078431373, 0.63529411764705882352941176470588, 0.38431372549019607843137254901961 ];")
	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m4.color = [0,1,0];")
	WRITE_LINE("m5"PANTONE527CV)

/*
PANTONEPANTONEBlue072CV
NeonRed
HotPink  
NeonRed
PANTONERubineRedCV
Gold
ElectricBlue
MyBrown MyOrange PANTONEPANTONEBlue072CV PANTONE731CV    
*/

	WRITE_LINE("m1.position= [ 0, 1, 0 ];")
	WRITE_LINE("m2.position= m1.position;")
	WRITE_LINE("m3.position= m1.position;")
	WRITE_LINE("m4.position= m1.position;")
	WRITE_LINE("m5.position= m1.position;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("R = m1.radius;")
	WRITE_LINE("D = 2.0*R;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m2.position.x = m1.position.x + D + .2;")
	WRITE_LINE("m3.position.x = m2.position.x + D + .3;")
	WRITE_LINE("m4.position.x = m3.position.x + D + .4;")
	WRITE_LINE("m5.position.x = m4.position.x + D + .5;")

	WRITE_LINE("m1.constraint.[0., 0., 0] ==     [0., 2., 0], 1  ;")
	WRITE_LINE("m2.constraint.[0., 0., 0] == m1.constraint.[0., 0., 0], | m2.position - m1.position |;")
	WRITE_LINE("m3.constraint.[0., 0., 0] == m2.constraint.[0., 0., 0], | m3.position - m2.position |;")
	WRITE_LINE("m4.constraint.[0., 0., 0] == m3.constraint.[0., 0., 0], | m4.position - m3.position |;")
	WRITE_LINE("m5.constraint.[0., 0., 0] == m4.constraint.[0., 0., 0], | m5.position - m4.position |;")


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
	WRITE_LINE("m1.numericalConstraintPositionDamping = 100;")
	WRITE_LINE("m2.numericalConstraintPositionDamping = 100;")
	WRITE_LINE("m3.numericalConstraintPositionDamping = 100;")
	WRITE_LINE("m4.numericalConstraintPositionDamping = 100;")
	WRITE_LINE("m5.numericalConstraintPositionDamping = 100;")
*/
///////////////////////////////////////////////////////////////////////////////
/*
	WRITE_LINE("m1.numericalPositionDamping = [0.1, 0.1, 0.1];")
	WRITE_LINE("m2.numericalPositionDamping = [0.1, 0.1, 0.1];")
	WRITE_LINE("m3.numericalPositionDamping = [0.1, 0.1, 0.1];")
	WRITE_LINE("m4.numericalPositionDamping = [0.1, 0.1, 0.1];")
	WRITE_LINE("m5.numericalPositionDamping = [0.1, 0.1, 0.1];")
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.externalGravitationalField.y = -3;")
	WRITE_LINE("system.timeStepSize = 0.05;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////

	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnTwoBody()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Two Body\";")
//exF x
//exF y
//exF z
	WRITE_LINE("One = sphere;")
	WRITE_LINE("Two = sphere;")
	WRITE_LINE("One.color = [ 1, 0, 1 ];")
	WRITE_LINE("Two.color = [ 0, 1, 0 ];")
	WRITE_LINE("One.position = [  0, 0, 0 ];")
	WRITE_LINE("Two.position = [  .5, 0, 0 ];")
	WRITE_LINE("One.radius =  .14;")
	WRITE_LINE("Two.radius =  .08;")
	WRITE_LINE("One.mass =  8;")
	WRITE_LINE("Two.mass =  1;")
	WRITE_LINE("One.drawStyle = 0;")
	WRITE_LINE("Two.drawStyle = 0;")

//	WRITE_LINE("One.velocity.y = -.5;")
//	WRITE_LINE("Two.velocity.y = 4;")

	WRITE_LINE("One.velocity = [ 0, -.5, 0 ];")
	WRITE_LINE("Two.velocity = [ 0, 4, 0 ];")

	WRITE_LINE("system.gravitationalCoupling = 1;")
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.timeStepSize = 0.001;")

	WRITE_LINE("system.boundingBox = [ -1.2, 1.2, -1.2, 1.2, 5, 10 ] ;")
	WRITE_LINE("system.translate   = [ 0 ,  0 ,  0 ];")
//	WRITE_LINE("system.fieldOfView = 23;")
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnThreeBody()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Three Body\";")
//sa .051
//sv .51

	WRITE_LINE("One = sphere;")
	WRITE_LINE("Two = sphere;")
	WRITE_LINE("Three = sphere;")
	WRITE_LINE("One.radius = .1 ;")
	WRITE_LINE("Two.radius = .18;")
	WRITE_LINE("Three.radius = .06;")
	WRITE_LINE("One.mass =  2;")
	WRITE_LINE("Two.mass =  4;")
	WRITE_LINE("Three.mass =  0.5;")
	WRITE_LINE("One.color = [ 1, 0.42, 0.8 ];")
	WRITE_LINE("Two.color = [ 0.6, 0.8, 1 ];")
	WRITE_LINE("Three.color = [ 0.749, 0.635, 0.384 ];")
	WRITE_LINE("One.position = [  -.625,  0, 0 ];")
	WRITE_LINE("Two.position = [   .615,  0, 0 ];")
	WRITE_LINE("Three.position = [    0, .5, 0 ];")

	WRITE_LINE("One.velocity =  [0, -0.022, 0.22];")
	WRITE_LINE("Three.velocity = [0, -0.088,  0.88];")

	WRITE_LINE("system.gravitationalCoupling = 1;")
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.timeStepSize = 0.001;")
	WRITE_LINE("system.boundingBox = [ -0.5, 0.5, -0.5, 0.5, 1.02, 120 ] ;")
	WRITE_LINE("system.translate   = [ 0 ,  0 ,  -3 ];")
	WRITE_LINE("system.rotate =[ 14 ,  16.2 ,  0 ] ;")
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnCurrentLoop()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.name = \"Current Loop\";")

	WRITE_LINE("s = sphere;")
	WRITE_LINE("s.position = [  0, 0, 0 ];")
	WRITE_LINE("s.color = [ 0.4, 0.1, 0.2 ];")
	WRITE_LINE("s.charge =  -1;")
	WRITE_LINE("s.mass =  1;")
	WRITE_LINE("s.velocity = [1, 0, 0];")
//	WRITE_LINE("s.velocity.x = 1;")
	WRITE_LINE("system.timeStepSize = 0.01;")
	WRITE_LINE("system.currentLoopMagneticField = 44;")

	WRITE_LINE("s.magneticDipoleMoment=1;")
	WRITE_LINE("s.magneticDipoleOrientation=[0,1,0];")


	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5 ];")
	WRITE_LINE("system.translate   = [ 0 ,  0 ,  0 ];")

/*
	string& text = theSystem->m_strResponse;
	text = "\
Charged Particle Moving in a Magnetic Field Due to a Current Loop\r\n\
\r\n\
In this example, B = magnetic field strength = 44,\r\n\
s.mass =  = 1, s.charge =  = -1.\r\n\
    To calculate the magnetic fieldlines:\r\n\
SHIFT-LEFT-CLICK\r\n\
";

	WRITE_LINE("system.print;")
*/
//	NewWebBrowser(6);

	
}

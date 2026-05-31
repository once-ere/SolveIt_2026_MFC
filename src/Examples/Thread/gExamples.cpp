// Examples.cpp : implementation of the CGenExamp class
//

#include "stdafx.h"
#include "SolveIt.h"
#include "GenExamp.h"

#include "SolveItDoc.h"
#include "SolveItView.h"
#include "GLView.h"
#include "MainFrm.h"

#include "resource.h"
#include "Help/HtmlCtrl.h"
#include "Help/TraceWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const _bstr_t CrLf(L"\r\n");
#include "../ScriptColors.h"

/////////////////////////////////////////////////////////////////////////////
// CGenExamp

///////////////////////////////////////////////////////////////////////////////
/*
void CGenExamp::OnSum()
{
//	
WRITE_LINE("system.gravitationalCoupling=16;")
WRITE_LINE("m1=cylinder;")
WRITE_LINE("m1.position=[1,0,0.1];")
WRITE_LINE("m2=block;")
WRITE_LINE("m2.position=[-1,1,0];")
WRITE_LINE("m2.momentum=[0,-.1,0];")
WRITE_LINE("m1.momentum=[0,.1,0];")
WRITE_LINE("m2.angular_momentum=[0,-.01,0.003];")
WRITE_LINE("m1.angular_momentum=[0,.01,-0.003];")

}
///////////////////////////////////////////////////////////////////////////////
slider = block;
slider.width =  1.50;
shaft = cylinder;
shaft.inverseMass = 0;
shaft.radius =  .05;
shaft.depth = 4.0;
shaft.orientation= [90, 0, 1, 0];
slider = block;
swinger = block;
spinner = block;
balance = block;
rocker1 = block;
rocker2 = block;
bob = block;
slider.width =  1.50;
slider.height = 0.85;
slider.depth =  0.85;
swinger.width =  4.0;
swinger.height = .35;
swinger.depth =  .1;
spinner.width =  0.5;
spinner.height = 1.0;
spinner.depth =  2.0;
balance.width =  4.0;
balance.height = 0.25;
balance.depth =  0.5;
rocker1.width =  0.5;
rocker1.height = 1.0;
rocker1.depth =  2.0;
rocker2.width =  0.8;
rocker2.height = 1.33;
rocker2.depth =  .5;
bob.width =  1.6;
bob.height = 1.2;
bob.depth =  .6;
swinger.orientation= [45, 0, 0, 1];
theta = -PI/6.0;
slider.link.[0,0,0] == [0,0,0],prismatic,[1,0,0];
swinger.[-0.5*(swinger.width-slider.width),0,-0.5*swinger.depth] ==slider.link.[0,0,0.5*slider.depth],, revolute,[0,0,1];
swinger.link.displacement = theta*RAD_TO_DEG;
*/
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnMultiBodySlider()
{
//	
//	theApp.Initialize();
	WM_APP_INIT//theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"MultiBody Slider\";")
//	WRITE_LINE("system.multiBody = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("shaft = cylinder;")
	WRITE_LINE("shaft.inverseMass = 0;")
	WRITE_LINE("shaft.drawStyle=0;")
	WRITE_LINE("shaft"MyBrown2)
	WRITE_LINE("shaft.radius =  .1;")
	WRITE_LINE("shaft.depth = 14.0;")
	WRITE_LINE("shaft.orientation= [90, 0, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("slider = block;")
	WRITE_LINE("swinger = block;")
//	WRITE_LINE("spinner = cylinder;")
	WRITE_LINE("spinner = block;")
//	WRITE_LINE("balance = cylinder;")
	WRITE_LINE("balance = block;")
	WRITE_LINE("rocker1 = block;")
	WRITE_LINE("rocker2 = block;")
	WRITE_LINE("bob = block;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("slider"PANTONERhodamineRedCV)
	WRITE_LINE("swinger"PANTONEOrange021CV)
	WRITE_LINE("spinner"PANTONEYellowCV)
	WRITE_LINE("balance"PANTONE2727CV)
	WRITE_LINE("rocker1"PANTONE527CV)
	WRITE_LINE("rocker2"PANTONEPANTONEBlue072CV)//Gold
	WRITE_LINE("bob"PANTONE731CV)
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("slider.width =  1.50;")
	WRITE_LINE("slider.height = 0.85;")
	WRITE_LINE("slider.depth =  0.85;")

	WRITE_LINE("swinger.width =  4.0;")
	WRITE_LINE("swinger.height = .35;")
	WRITE_LINE("swinger.depth =  .1;")

	WRITE_LINE("spinner.width =  0.5;")
	WRITE_LINE("spinner.height = 1.0;")
	WRITE_LINE("spinner.depth =  2.0;")

	WRITE_LINE("balance.width =  4.0;")
	WRITE_LINE("balance.height = 0.25;")
	WRITE_LINE("balance.depth =  0.5;")

	WRITE_LINE("rocker1.width =  0.5;")
	WRITE_LINE("rocker1.height = 1.0;")
	WRITE_LINE("rocker1.depth =  2.0;")

	WRITE_LINE("rocker2.width =  0.8;")
	WRITE_LINE("rocker2.height = 1.33;")
	WRITE_LINE("rocker2.depth =  .5;")

	WRITE_LINE("bob.width =  1.6;")
	WRITE_LINE("bob.height = 1.2;")
	WRITE_LINE("bob.depth =  .6;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("swinger.orientation= [45, 0, 0, 1];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("theta = -PI/6.0;")
///////////////////////////////////////////////////////////////////////////////
WRITE_LINE("slider.link.[0,0,0] == [0,0,0],prismatic,[1,0,0];")
WRITE_LINE("swinger.link.[-0.5*(swinger.width-slider.width),0,-0.5*swinger.depth] ==slider.link.[0,0,0.5*slider.depth] , revolute,[0,0,1];")
WRITE_LINE("swinger.joint.displacement = theta*RAD_TO_DEG;")
//return;
WRITE_LINE("spinner.link.[-0.5*spinner.width,0.125*spinner.height,0] ==swinger.link.[(0.5*swinger.width),0,0] , revolute,[0,0,1];")
WRITE_LINE("spinner.joint.displacement = -theta*RAD_TO_DEG;")

WRITE_LINE("balance.link.[0,0,-0.5*balance.depth] ==spinner.link.[0,0, 0.5*spinner.depth] , revolute,[0,0,1];")


WRITE_LINE("rocker1.link.[0.5*rocker1.width, 0,0] ==balance.link.[-0.5*balance.width,0,0] , revolute,[1,0,0];")
WRITE_LINE("rocker1.joint.displacement = 20;")

WRITE_LINE("rocker2.link.[-0.5*rocker2.width, 0.125*rocker2.height,0] ==balance.link.[0.5*balance.width,0,0] , revolute,[1,0,0];")
WRITE_LINE("rocker2.joint.displacement = 30;")

WRITE_LINE("bob.link.[0,0.125*bob.height,0.5*bob.depth] ==spinner.link.[0,0,-0.5*spinner.depth] , revolute,[0,0,1];")
WRITE_LINE("bob.joint.displacement = 45;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -9.81;")
	WRITE_LINE("system.timeStepSize = 0.05;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.boundingBox = [ -1 ,  5 ,  -5 ,  1 ,  20 , 40  ];")
	WRITE_LINE("system.boundingBox = [ 0 ,  5 ,  -5 ,  0 ,  2 , 12  ];")
	WRITE_LINE("system.translate   = [  0 ,  -0.4 ,  -8  ];")
//	WRITE_LINE("system.fieldOfView = 25;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("slider.joint.springConstant;")
///////////////////////////////////////////////////////////////////////////////
	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnMultiMassSpring()
{
	WM_APP_INIT
//	theApp.Initialize();WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"Mass-Spring\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.multiBody = true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [  -5 ,  5 ,  -5 ,  5 ,  20 , 40  ];")
	WRITE_LINE("system.translate   = [  0 , 0 ,  -23  ];")
//	WRITE_LINE("system.fieldOfView = 25;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m0 = sphere;")
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = block;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m5 = block;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m2.width =  .5;")
	WRITE_LINE("m2.height = .7;")
	WRITE_LINE("m2.depth =  .6;")
	WRITE_LINE("m0.radius = m2.height/2;")
	WRITE_LINE("m1.radius = m2.height/2;")
	WRITE_LINE("m3.radius = m2.width/2;")
	WRITE_LINE("m4.radius = m2.depth/2;")
	WRITE_LINE("m5.width =  .5;")
	WRITE_LINE("m5.height = .7;")
	WRITE_LINE("m5.depth =  .6;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m0.mass = 1;")
	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 1;")
	WRITE_LINE("m3.mass = 1;")
	WRITE_LINE("m4.mass = 1;")
	WRITE_LINE("m5.mass = 1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m0"Gold)
	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m4"PANTONE527CV)
	WRITE_LINE("m5"PANTONEPANTONEBlue072CV)
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Time();")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("seed_rand(Time());")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m0.position  = [0, 5, 0 ];")
	WRITE_LINE("m1.position  = [1+1*(rand()-0.5),  3.5, (rand()-0.5) ];")
	WRITE_LINE("m2.position  = [2+2*(rand()-0.5),  2.05,  (rand()-0.5) ];")
	WRITE_LINE("m3.position  = [3+3*(rand()-0.5),  1.025,  (rand()-0.5) ];")
	WRITE_LINE("m4.position  = [4+4*(rand()-0.5),  -0.225,  (rand()-0.5) ];")
	WRITE_LINE("m5.position  = [4+3*(rand()-0.5),  -0.5,  (rand()-0.5) ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("mySpringConstant = 10.0;")
///////////////////////////////////////////////////////////////////////////////
WRITE_LINE("m0.[0,0,0] == [0, m0.position.y, 0] spring,[3*mySpringConstant, 0,  0];")//
WRITE_LINE("m1.[0,0,0] == m0.[0,0,0] spring,[4*mySpringConstant, sqrt((m1.position.x-m0.position.x)^2+(m1.position.y-m0.position.y)^2+(m1.position.z-m0.position.z)^2),  0];")
WRITE_LINE("m2.[m2.width/2, m2.height/2, m2.depth/2] == m1.[0,0,0] spring,[4*mySpringConstant, sqrt((m2.position.x-m1.position.x)^2+(m2.position.y-m1.position.y)^2+(m2.position.z-m1.position.z)^2),  0];")
WRITE_LINE("m3.[0,0,0] == m2.[-m2.width/2, -m2.height/2, -m2.depth/2] spring,[6*mySpringConstant, sqrt((m3.position.x-m2.position.x)^2+(m3.position.y-m2.position.y)^2+(m3.position.z-m2.position.z)^2),  0];")
WRITE_LINE("m4.[0,0,0] == m3.[0,0,0] spring,[2*mySpringConstant, sqrt((m4.position.x-m3.position.x)^2+(m4.position.y-m3.position.y)^2+(m4.position.z-m3.position.z)^2),  0];")
WRITE_LINE("m5.[m5.width/2, m5.height/2, m5.depth/2] == m3.[0,0,0] spring,[mySpringConstant, sqrt((m4.position.x-m5.position.x)^2+(m4.position.y-m5.position.y)^2+(m4.position.z-m5.position.z)^2),  0];")
WRITE_LINE("m5.[-m5.width/2, -m5.height/2, -m5.depth/2] == m4.[0,0,0] spring,[mySpringConstant, sqrt((m5.position.x-m3.position.x)^2+(m5.position.y-m3.position.y)^2+(m5.position.z-m3.position.z)^2),  0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -2.;")
	WRITE_LINE("system.timeStepSize = 0.025;")
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnMultiBodyTest()
{
//	theApp.Initialize();WaitForSingleObject(g_EventInitialize, INFINITE);
	WM_APP_INIT
	WRITE_LINE("system.name = \"no action Test\";")
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnMultiBodyChain()
{
	WM_APP_INIT
//	theApp.Initialize();WaitForSingleObject(g_EventInitialize, INFINITE);

	WRITE_LINE("system.name = \"MultiBody Chain of Six\";")
	WRITE_LINE("system.multiBody = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = block;")
	WRITE_LINE("m2 = block;")
	WRITE_LINE("m3 = block;")
	WRITE_LINE("m4 = block;")
	WRITE_LINE("m5 = block;")
	WRITE_LINE("m6 = block;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.drawStyle=0;")
	WRITE_LINE("m2.drawStyle=0;")
	WRITE_LINE("m3.drawStyle=0;")
	WRITE_LINE("m4.drawStyle=0;")
	WRITE_LINE("m5.drawStyle=0;")
	WRITE_LINE("m6.drawStyle=0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m4.color = [0,1,0];")
	WRITE_LINE("m5"PANTONE527CV)
	WRITE_LINE("m6"Gold)
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
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.[-0.5*m1.width, 0., 0] ==  [0, 0, 0] , revolute,[0,0,1];")
	WRITE_LINE("m2.[-0.5*m2.width, 0., 0] == m1.[0.5*m1.width, 0., 0] , revolute,[0,0,1];")
	WRITE_LINE("m3.[-0.5*m3.width, 0., 0] == m2.[0.5*m2.width, 0., 0] , revolute,[0,0,1];")
	WRITE_LINE("m4.[-0.5*m4.width, 0., 0] == m3.[0.5*m3.width, 0., 0] , revolute,[0,0,1];")
	WRITE_LINE("m5.[-0.5*m5.width, 0., 0] == m4.[0.5*m4.width, 0., 0] , revolute,[0,0,1];")
	WRITE_LINE("m6.[-0.5*m6.width, 0., 0] == m5.[0.5*m5.width, 0., 0] , revolute,[0,0,1];")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -9.81;")
	WRITE_LINE("system.timeStepSize = 0.05;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  5 ,  10 ];")
	WRITE_LINE("system.translate   = [ 0 ,  2 ,  -1.3 ];")
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnLennardJonesHeatedCooled()
{
//	theApp.Initialize();WaitForSingleObject(g_EventInitialize, INFINITE);
	WM_APP_INIT
	
	WRITE_LINE("system.name = \"Lennard-Jones Potential with Heating then Cooling\";")
	WRITE_LINE("system.background.show = true;")
	WRITE_LINE("lightenDarken = 0.7;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function fx(x$, y$, z$) {"
"	return (0.25*x*(1 - x/4)*(1 + x/4) + 12.5*x*y*y*exp(-x*x));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function fy(x$, y$, z$) {"
"	return (-0.5*y*(1 + 25*exp(-x*x)));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function V(x$, y$, z$) {"
"	return ( (1 - x/4)*(1 + x/4)*(1 - x/4)*(1 + x/4) + y*y*(0.25+6.25*exp(-x*x)));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Stretch(x$) \r\n{\r\n"
"val=1.0/2048.0;\r\n"
"if (x <= val) {"
"	return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
"};\r\n"
" while(x>val){"
"	val=2.0*val;"
"};\r\n"
"return  ( pow(0.000001+x, (lightenDarken/val)*x) );"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Red(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Green(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,2.52*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Blue(x$) {"
"	r = Stretch(x);"
"	return  ( 1.0-pow( r,3*r ));"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")

	WRITE_LINE("m1.radius =  .3;")
	WRITE_LINE("m2.radius =  .3;")
	WRITE_LINE("m3.radius =  .3;")

	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 1;")
	WRITE_LINE("m3.mass = 1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.drawStyle = 0;")
	WRITE_LINE("m2.drawStyle = 0;")
	WRITE_LINE("m3.drawStyle = 0;")

	WRITE_LINE("m1.color = [ 0.749, 0.635, 0.384 ];")
	WRITE_LINE("m2.color = [ 0.635, 0.384, 0.749 ];")
	WRITE_LINE("m3.color = [ 0.384, 0.749, 0.635 ];")

	WRITE_LINE("m1.position = [ 4.3,  0.577, 0 ];")
	WRITE_LINE("m2.position = [ 4.3, -0.577, 0 ];")
	WRITE_LINE("m3.position = [ 3.3,      0, 0 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.x = \"fx\";")
	WRITE_LINE("m2.externalForce.x = \"fx\";")
	WRITE_LINE("m3.externalForce.x = \"fx\";")
	WRITE_LINE("m1.externalForce.x.time = false;")
	WRITE_LINE("m2.externalForce.x.time = false;")
	WRITE_LINE("m3.externalForce.x.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.externalForce.y = \"fy\";")
	WRITE_LINE("m2.externalForce.y = \"fy\";")
	WRITE_LINE("m3.externalForce.y = \"fy\";")
	WRITE_LINE("m1.externalForce.y.time = false;")
	WRITE_LINE("m2.externalForce.y.time = false;")
	WRITE_LINE("m3.externalForce.y.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.coefficientOfDamping = [ -0.38, -0.38, 0 ];")
	WRITE_LINE("m2.coefficientOfDamping = [ -0.38, -0.38, 0 ];")
	WRITE_LINE("m3.coefficientOfDamping = [ -0.38, -0.38, 0 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"13.0; m1.coefficientOfDamping = [ 0.38, 0.38, 0 ];\";")
	WRITE_LINE("system.deferredCommand = \"13.0; m2.coefficientOfDamping = [ 0.38, 0.38, 0 ];\";")
	WRITE_LINE("system.deferredCommand = \"13.0; m3.coefficientOfDamping = [ 0.38, 0.38, 0 ];\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; m1.coefficientOfDamping = [ -0.38, -0.38, 0 ];\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; m2.coefficientOfDamping = [ -0.38, -0.38, 0 ];\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; m3.coefficientOfDamping = [ -0.38, -0.38, 0 ];\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= 0.1;")
	WRITE_LINE("system.potentialLennardJones =  [1,1.2599210498948731647672106072782,1.1224620483093729814335330496792];")
	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, 4.8, 20 ] ;")
	WRITE_LINE("system.translate.z =  -5.1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.potential = \"V\";")
	WRITE_LINE("m2.potential = \"V\";")
	WRITE_LINE("m3.potential = \"V\";")
	WRITE_LINE("m1.potential.time = false;")
	WRITE_LINE("m2.potential.time = false;")
	WRITE_LINE("m3.potential.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.potential = \"V\";")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.color.red = \"Red\";")
	WRITE_LINE("system.color.green = \"Green\";")
	WRITE_LINE("system.color.blue = \"Blue\";")
	WRITE_LINE("system.potential.show = true;")
///////////////////////////////////////////////////////////////////////////////
/*
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.redFunc = ;")
	WRITE_LINE("system.greenFunc = ;")
	WRITE_LINE("system.blueFunc = ;")
	WRITE_LINE("system.potential.name = 'V';")//
	WRITE_LINE("system.potential.nargs = 3;")//
	WRITE_LINE("system.potential.time = false;")//
	WRITE_LINE("system.potential = true;")//
	WRITE_LINE("system.potential.show = true;")//
///////////////////////////////////////////////////////////////////////////////
*/
	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::OnVerhulstDynamics()
{
	
//	theApp.Initialize(_T("VerhulstDynamics.htm"), _T("Logistic Map"));WaitForSingleObject(g_EventInitialize, INFINITE);

//	WM_APP_INIT
//	AfxGetApp()->PostThreadMessage(WM_APP_Initialize, (WPARAM) dupString("VerhulstDynamics.htm"), (LPARAM) dupString("Logistic Map"));
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WM_APP_INIT2("VerhulstDynamics.htm","Logistic Map")

	WRITE_LINE("system.name = \"Logistic Map\";")
	WRITE_LINE("system.boundingBox = [-0.1, 2.0, 1.9, 4.0, -1, 1];")
//	WRITE_LINE("system.translate = [ 0.0 ,  0.0 ,  0.08 ];")
//	WRITE_LINE("system.boundingBox = [-2.0, 1.0,-1.5, 1.5,  -1, 1];")
	WRITE_LINE("system.translate = [ 0. ,  0.0 ,  0.0 ];")
//	system.boundingBox = [-0.1, 2.0, 1.9, 4.0, -4.5, 4.5];
//	system.rotate=true;
	WRITE_LINE("system.background.show = true;")
//	WRITE_LINE("system.background.show = false;")
	WRITE_LINE("system.dimension = 2;")
	WRITE_LINE("system.scale = 2;")
	WRITE_LINE("system.nodes = 666;")
//	WRITE_LINE("system.scale = 1;")
	
	WRITE_LINE("system.map = \"Logistic Map\";")

	_bstr_t bstr = L"\
Click in the 'Scene' window then press the ESCAPE key \r\n\
or press 'Stop' to quit;\r\nafter stopping, this example must be reloaded to run again.\r\n\
";
//	OnWriteResponse(bstr);OnWriteResponse(0);
//	WRITE_LINE("system.twoDW.verDyn = true;")
	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CInfoDialog* pDialog = new CInfoDialog(_T("VerhulstDynamics.htm"), _T("Logistic Map"));
	pFrame->m_CleanupOnInitialize.Add(pDialog->m_hWnd);
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();	gl->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)


}





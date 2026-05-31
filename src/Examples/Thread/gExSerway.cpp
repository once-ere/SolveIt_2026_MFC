// ExSerway.cpp : Implementation of CIO

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

#include "../ScriptColors.h"
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Problem2_14()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Problem 2.14\";")

	WRITE_LINE("raceCar=sphere;")
	WRITE_LINE("raceCar.mass = 1;")
	WRITE_LINE("raceCar.color = [0.749, 0.635, 0.384];")
	WRITE_LINE("raceCar.velocity=[1, 5, 0];")

	WRITE_LINE("accScale = 3*2*0.75;")
	WRITE_LINE("function fy(t$) {"
"return(accScale*t)};")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("raceCar.externalForce.y = \"fy\";")
	WRITE_LINE("raceCar.externalForce.y.time = false;")
//	WRITE_LINE("raceCar.externalForce.y=fy 1, true;")
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("raceCar.trajectory.color = [0.0, 0.0, 1.0];")

	WRITE_LINE("raceCar.trajectory.color = raceCar.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("raceCar.record.time.begin = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("raceCar.record.time.end = 5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("raceCar.record.trajectory = true;")
//	WRITE_LINE("raceCar.trajectory 0.0, 5,  [0.0, 0.0, 1.0];")

	WRITE_LINE("raceCar.position.show = 2;")
	WRITE_LINE("raceCar.velocity.show = 10;")
	WRITE_LINE("raceCar.position.scale.y = 5;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.scale.position.y = 17;")
	WRITE_LINE("system.timeStepSize= 0.1;")
	WRITE_LINE("system.stop.time = 4;")

//	WRITE_LINE("system.boundingBox = [  -1, 4, -5, 95, 15, 35 ];")
//	WRITE_LINE("system.translate.z = -12.8;")
	WRITE_LINE("system.boundingBox = [  -1, 4, -5, 95, -1, 1 ];")
	WRITE_LINE("system.translate =  [ -0.7, 0, 5];")
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Problem2_37()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Problem 2.37\";")

	WRITE_LINE("ball=sphere;")
	WRITE_LINE("ball.mass = 1;")
	WRITE_LINE("ball.color = [0.8, 0.8, 0.6];")

	WRITE_LINE("ball.velocity=[1, 25, 0];")


	WRITE_LINE("function fy(t$) {"
"return(-9.8)};")

//	WRITE_LINE("ball.externalForce.y=fy 1, true;")
	WRITE_LINE("ball.trajectory.color = [ 0.694, 0.305, 0.203];")
	WRITE_LINE("ball.trajectory.color = ball.color;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("ball.externalForce.y = \"fy\";")
	WRITE_LINE("ball.externalForce.y.time = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("ball.record.time.begin = 0.0;")
	WRITE_LINE("ball.record.time.end = 6;")
	WRITE_LINE("ball.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("ball.position.scale.y = 10;")
///////////////////////////////////////////////////////////////////////////////

//	WRITE_LINE("system.scale.position = 1.5 10 1;")
	WRITE_LINE("system.timeStepSize= 0.1;")
	WRITE_LINE("system.stop.time = 6;")
//	WRITE_LINE("system.translate.x =  -1.9;")
//	WRITE_LINE("system.translate.y =  -2.1;")
//	WRITE_LINE("system.translate.z =  -18;")
//	WRITE_LINE("system.boundingBox = [  -2.5, 7.5, -15, 35, 5, 10 ];")
	WRITE_LINE("system.boundingBox = [  -2.5, 7.5, -15, 35, -1, 1 ];")
	WRITE_LINE("system.translate.z =  5;")
	WRITE_LINE("system.enforceRigidBoundary = 2;")//floor
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = 5, \"system.timeStepSize= 0.01\";")
	WRITE_LINE("system.deferredCommand.onReset = 0.0, \"system.timeStepSize= 0.1\";")

	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Example2_4()
{
//	theApp.Initialize(_T("Example2_4.htm"), _T("Example 2.4"));

//	AfxGetApp()->PostThreadMessage(WM_APP_Initialize, (WPARAM) dupString("Example2_4.htm"), (LPARAM) dupString("Example 2.4"));
//	WaitForSingleObject(g_EventInitialize, INFINITE);

	WM_APP_INIT2("Example2_4.htm","Example 2.4")


	WRITE_LINE("system.title = \"Example 2.4\";")
	WRITE_LINE("system.title.x = \"Time\";")
	WRITE_LINE("system.title.y = \"Distance\";")

	WRITE_LINE("system.boundingBox = [  -5, 25, -50, 950, -1, 1 ] ;")
//	WRITE_LINE("system.translate =  [ -1.5, 0, 5];")
	WRITE_LINE("system.translate =  [ -1.5, 50, 0];")

	WRITE_LINE("car = sphere;")
	WRITE_LINE("car.mass = 1;")
	WRITE_LINE("car.color = [0.749, 0.635, 0.384];")

	WRITE_LINE("car.velocity=[1, 30, 0];")

///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("moto = sphere;")
	WRITE_LINE("moto.mass = 1;")
	WRITE_LINE("moto.color = [0.384, 0.749, 0.635];")

	WRITE_LINE("moto.position=[0, 0, 0];")
	WRITE_LINE("moto.velocity=[1, 0, 0];")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function motoForce(t$) {"
"	if(t <= 1.0) return (0);"
"	return (3);"
	"}")

//	WRITE_LINE("moto.externalForce.y=motoForce 1, true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("moto.externalForce.y = \"motoForce\";")
	WRITE_LINE("moto.externalForce.y.time = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("moto.trajectory.color = moto.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("moto.record.time.begin = 0.0;")
	WRITE_LINE("moto.record.time.end = 23;")
	WRITE_LINE("moto.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("car.externalForce.y = \"f1\";")
//	WRITE_LINE("car.externalForce.y.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("car.trajectory.color = car.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("car.record.time.begin = 0.0;")
	WRITE_LINE("car.record.time.end = 23;")
	WRITE_LINE("car.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("car.trajectory 0.0, 23,[ 0.0, 0.0, 1.0];")
//	WRITE_LINE("moto.trajectory 0.0, 23,[ 1.0, 0.0, 0.0];")

//	WRITE_LINE("moto.show.position = 10;")


//	WRITE_LINE("system.scale.position = 6 170 1;")
	WRITE_LINE("system.timeStepSize= 0.2;")
	WRITE_LINE("system.stop.time = 23;")

	return;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CInfoDialog* pDialog = new CInfoDialog(_T("Example2_4.htm"), _T("Example 2.4"));
	pFrame->m_CleanupOnInitialize.Add(pDialog->m_hWnd);
	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->m_bPerspectiveTransformation=false;
	gl->SetPerspectiveOrFrustum();
	gl->Invalidate();	
	gl->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
//	pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));

}
///////////////////////////////////////////////////////////////////////////////
// 3
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Problem3_46()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Problem 3.46\";")
//	WRITE_LINE("system.boundingBox = [ -0.75, 11.25, -1.5, 6.5, 15, 35 ] ;")
	WRITE_LINE("system.boundingBox = [ -0.75, 11.25, -2, 7, -1, 1 ] ;")
	WRITE_LINE("system.translate =  [ 0.0, -1.0, 5];")

	WRITE_LINE("goal=sphere;")
	WRITE_LINE("goal.inverseMass = 0;")
	WRITE_LINE("goal.color = [0.4, 1.0, 0.3];")
	WRITE_LINE("goal.position = [10, 3.05, 0];")

	WRITE_LINE("ball=sphere;")
	WRITE_LINE("ball.mass = 1;")
	WRITE_LINE("ball.color = [0.8, 0.8, 0.6];")
	WRITE_LINE("ball.velocity= [7.39923, 7.39923, 0];")
	WRITE_LINE("ball.position = [0, 2, 0];")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("ball.trajectory.color = ball.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("ball.record.time.begin = 0.0;")
	WRITE_LINE("ball.record.time.end = 6;")
	WRITE_LINE("ball.record.trajectory = true;")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("ball.trajectory.color = [ 0.694, 0.305, 0.203];")

//	WRITE_LINE("ball.scale.position.y = 10;")
///////////////////////////////////////////////////////////////////////////////

//	WRITE_LINE("system.scale.position = 2.0 1.0 1;")
	WRITE_LINE("system.timeStepSize= 0.05;")
	WRITE_LINE("system.stop.time = 2;")
//	WRITE_LINE("system.translate.x =  -2.3;")
//	WRITE_LINE("system.translate.y =  -2.4;")
//	WRITE_LINE("system.translate.z =  -24;")
	WRITE_LINE("system.externalGravitationalField.y = -9.8;")
	WRITE_LINE("system.enforceRigidBoundary =2;")//floor

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("magAccGravity = 9.8;")
	WRITE_LINE("function speedInitial(dx$,dy$,theta$) {"
"return(dx*sqrt((magAccGravity/(2*cos(theta)))*(1/(dx*sin(theta)-dy*cos(theta)))))};")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	_bstr_t bstr = L"\
Here dx = 10.0m, dy = 1.05m, theta = PI / 4 .\r\n\
Execute 'speedInitial( 10.0, 1.05, PI / 4)' (without the quotes) to find the speed.\r\n\
speedInitial * cos( PI / 4) = speedInitial( 10.0, 1.05, PI / 4) / sqrt(2) gives the x-comp of the initial velocity .\r\n\
speedInitial * sin( PI / 4) = speedInitial( 10.0, 1.05, PI / 4) / sqrt(2) gives the y-comp of the initial velocity .\r\n\
";
///////////////////////////////////////////////////////////////////////////////
	Fire_WriteResponse(bstr);Fire_WriteResponse(0);
	
}
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Problem3_51()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Problem 3.51\";")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("velBoatWrtRiver = vecObj;")
	WRITE_LINE("velRiver = vecObj;")

	WRITE_LINE("velBoatWrtRiver.position =  [2.35702, 2.35702, 0];")
	WRITE_LINE("velRiver.position =  [0, 1.667, 0];")

	WRITE_LINE("velBoatWrtLand = velBoatWrtRiver + velRiver;")

	WRITE_LINE("velBoatWrtRiver.color = [0.85, 0.40, 0.13];")
	WRITE_LINE("velRiver.color = [0.30, 0.20, 1.00];")
	WRITE_LINE("velBoatWrtLand.color = [1.00, 0.00, 0.00];")

	WRITE_LINE("system.boundingBox = [-2, 8, -5, 5, -1, 1 ] ;")
	WRITE_LINE("system.translate =  [-1.5, 0, 5];")
}
/*
Time bounce = 1.319
Time ball = 1.34
*/
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Problem3_60()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Problem 3.60\";")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("function CoefficientOfRestitution(theta) {"
//	WRITE_LINE("return( 0.5*sqrt(1 - 3*pow( cos(DEG_TO_RAD * theta) / sin(DEG_TO_RAD * theta) , 2 ) ) )   };")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("function CoefficientOfRestitution(theta) {"
//	WRITE_LINE("return( sqrt(speedRatio - (1 - speedRatio)*pow( cos(DEG_TO_RAD * theta) / sin(DEG_TO_RAD * theta) , 2 ) ) )   };")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -0.5, 9.5, -4, 6, -1, 1 ] ;")
	WRITE_LINE("system.translate =  [0, -1.5, 6];")

	WRITE_LINE("ball=sphere;")
	WRITE_LINE("ball.radius = .2;")
	WRITE_LINE("ball.mass = 1;")
	WRITE_LINE("ball.color = [0.749, 0.635, 0.384];")
//initial speed = 10
//	WRITE_LINE("ball.velocity= 7.0710678118654752440084436210485 7.0710678118654752440084436210485 0;")
//initial speed = 9.3914855054991167249185294086714
//range = 9
	WRITE_LINE("ball.velocity= [6.6407830863535965971976764433087, 6.6407830863535965971976764433087, 0];")

	WRITE_LINE("bounce=sphere;")
	WRITE_LINE("bounce.radius = .2;")
	WRITE_LINE("bounce.mass = 1;")
	WRITE_LINE("bounce.color = [0, 0, 1];")
//theta = ½ asin(4/5) = 26.565051177077989351572193720453°
//{±0.894427, 0.447214} initial speed = 10
//	WRITE_LINE("bounce.velocity= 8.9442719099991587856366946749251 4.4721359549995793928183473374626 0;")
	WRITE_LINE("bounce.velocity= [8.4, 4.2, 0];")
//	WRITE_LINE("bounce.angular_momentum.z= 0.5;")

//	WRITE_LINE("bounce.coefficientOfRestitution = 0.4;")
	WRITE_LINE("bounce.coefficientOfKineticFriction = 4;")
	WRITE_LINE("bounce.coefficientOfStaticFriction = 0.5;")
//	WRITE_LINE("bounce.coefficientOfRestitution = 0.75;")
//	WRITE_LINE("bounce.coefficientOfKineticFriction = 0.75;")

	WRITE_LINE("Floor = block;")
	WRITE_LINE("Floor.color = [0.749, 0.635, 0.0384];")
	WRITE_LINE("Floor.drawStyle =  0;")
	WRITE_LINE("Floor.width =  18;")
	WRITE_LINE("Floor.height = .2;")
	WRITE_LINE("Floor.depth =  .4;")
	WRITE_LINE("Floor.inverseMass = 0;")
	WRITE_LINE("Floor.position = [9,  -0.1, 0];")
	WRITE_LINE("Floor.coefficientOfKineticFriction = 4;")
//	WRITE_LINE("Floor.coefficientOfKineticFriction = 1;")
	WRITE_LINE("Floor.coefficientOfRestitution = 0.4;")
/*
	WRITE_LINE("bounce.coefficientOfRestitution = 1/sqrt(2);")
	WRITE_LINE("Floor.coefficientOfRestitution = 1/sqrt(2);")
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("bounce.record.time.begin = 0.0;")
	WRITE_LINE("bounce.record.time.end = 2;")
	WRITE_LINE("bounce.record.trajectory = true;")
	WRITE_LINE("bounce.trajectory.color = bounce.color;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("ball.record.time.begin = 0.0;")
	WRITE_LINE("ball.record.time.end = 5;")
	WRITE_LINE("ball.record.trajectory = true;")
	WRITE_LINE("ball.trajectory.color = ball.color;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("bounce. trajectory.color = [0, 0, 1];")
	WRITE_LINE("ball. trajectory.color = [0, 1, 0];")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function spd() {"
"	return (sqrt ( bounce.velocity % bounce.velocity ) );"
	"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("speedRatio = 0.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= 0.01;")
	WRITE_LINE("system.stop.time = 2;")
	WRITE_LINE("system.externalGravitationalField.y = -9.8;")
	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.contacts.show = true;")
//	WRITE_LINE("system.enforceRigidBoundary = 2;")//floor
//	WRITE_LINE("system.bottomRigidBoundary = 0;")
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.deferredCommand = \"13.0; m3.coefficientOfDamping = [ 0.38, 0.38, 0 ];\";")
//	WRITE_LINE("system.deferredCommand.onReset = \"0.0; m1.coefficientOfDamping = [ -0.38, -0.38, 0 ];\";")

	WRITE_LINE("system.deferredCommand = \"0.5; system.collisionDetection = true\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.collisionDetection = false\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"0.8; system.timeStepSize= 0.001\";")
//	WRITE_LINE("system.deferredCommand = \"0.84; system.timeStepSize= 0.01\";")
//	WRITE_LINE("system.deferredCommand = \"1.3; system.timeStepSize= 0.001\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.timeStepSize= 0.01\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.deferredCommand = \"0.8; system.arc = true\";")
//	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.arc = false\";")
//	WRITE_LINE("system.arc = true;")

	
}

//	WRITE_LINE("system.enforceRigidBoundary =2;")//floor
//	WRITE_LINE("system.scale.position = 2 0.5 1;")
//	WRITE_LINE("system.translate.x =  -2.3;")
//	WRITE_LINE("system.translate.y =  -2.4;")
//	WRITE_LINE("system.translate.z =  -24;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 4
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGenExamp::Example7_8()
{
	//Fire_ClearUserDefs();
	WM_APP_INIT///*theApp.PostThreadMessage(WM_APP_Initialize, 0, 0);*///WaitForSingleObject(g_EventInitialize, INFINITE);
	WRITE_LINE("system.title = \"Example 7.8\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("nope = sphere;")
	WRITE_LINE("nope.radius = .07;")
	WRITE_LINE("nope.color = [ 0,  0.4, 1];")
	WRITE_LINE("nope.drawStyle =  0;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("pendulum = sphere;")
	WRITE_LINE("pendulum.radius = .07;")
	WRITE_LINE("pendulum.color = [ 0.4, 1, 0 ];")
	WRITE_LINE("pendulum.drawStyle =  0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("yes = sphere;")
	WRITE_LINE("yes.radius = .07;")
	WRITE_LINE("yes.position = [ 0, -1, 0 ];")
	WRITE_LINE("yes.color = [ 1, 0.4, 0 ];")
	WRITE_LINE("yes.drawStyle =  0;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
	WRITE_LINE("pendulum.numericalPositionDamping = [ 0 ,  0 ,  0 ];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 0.1,  0.1,  0.1];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-0.1, -0.1, -0.1];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-1.1, -1.1, -1.1];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 1.1,  1.1,  1.1];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 10., 10., 10.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-10.,-10.,-10.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 100., 100., 100.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-100.,-100.,-100.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 20., 20., 20.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-20.,-20.,-20.];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 0.01,  0.01,  0.01];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-0.01, -0.01, -0.01];")
	WRITE_LINE("pendulum.numericalPositionDamping = [-0.001, -0.001, -0.001];")
	WRITE_LINE("pendulum.numericalPositionDamping = [ 0.001,  0.001,  0.001];")
*/

	WRITE_LINE("pendulum.numericalVelocityDamping = [ 0.1,  0.1,  0.1];")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; pendulum.constraint = true\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; yes.constraint = true\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; nope.constraint = true\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("pendulumLength = 1.0;")
	WRITE_LINE("bNegateGradientForNormal = true;")
//	WRITE_LINE("pointOnBody = [ 0, 0, 0 ];")
	WRITE_LINE("system.externalGravitationalField.y =  -2;")
	WRITE_LINE("system.timeStepSize = 0.025;")
	WRITE_LINE("system.boundingBox = [ -2, 2, -2, 2, 5, 10 ] ;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("yes.velocity  = [1.02*sqrt(-system.externalGravitationalField.y*pendulumLength*5),  0, 0 ];")
	WRITE_LINE("nope.velocity  = [0.8*sqrt(-system.externalGravitationalField.y*pendulumLength*5),  0, 0 ];")
	WRITE_LINE("pendulum.velocity  = [-nope.velocity.x,  0, 0 ];")
//	WRITE_LINE(" pendulum.{[0, 0, 0]} = {[0, 1, 0]} 1;"), pointOnBody
	WRITE_LINE(" nope.surface == [1, 1, 1, 0, 0, 0, 0, -2, 0, 0] , bNegateGradientForNormal, \"positive\";")
	WRITE_LINE(" pendulum.surface == [1, 1, 1, 0, 0, 0, 0, -2, 0, 0] , bNegateGradientForNormal, \"both\";")
	WRITE_LINE(" yes.surface == [1, 1, 1, 0, 0, 0, 0, 0, 0, -1] , bNegateGradientForNormal, \"positive\";")
//	WRITE_LINE("yes.normals = true;")
//	WRITE_LINE("nope.normals = true;")
	WRITE_LINE("pendulum.numericalConstraintPositionDamping = 100;")
	
///////////////////////////////////////////////////////////////////////////////
}
//	WRITE_LINE("nope.position = [ 0, -1, 0 ];")
//	WRITE_LINE("nope.position = [ 0, 0, 0 ];")
//	WRITE_LINE(" yes.surface == [1, 1, 1, 0, 0, 0, 0, 0, 0, -1] , [0, 0, 0], false, \"negative\";")
//	WRITE_LINE(" yes.surface == [1, 1, 1, 0, 0, 0, 0, 0, 0, -1] , [0, 0, 0], true, \"positive\";")
//	WRITE_LINE(" yes.surface == [0, 0, 0, 0, 1, 0] , \"negative\";")
//	WRITE_LINE(" nope.surface == [0, 0, 0, 0, 1, 0] , \"positive\";")
//	WRITE_LINE(" pendulum.{[0, 0, 0]} = {[0, 1, 0]} 1, \"positive\";")



#undef WRITE_LINE
#undef WRITE_LINE

// ExTest.cpp : Implementation of CIO

#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ScriptColors.h"
//#include "GenExamp.h"

///////////////////////////////////////////////////////////////////////////////

void CSolveItView::OnRandomChain()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_CHAOTIC_RANDOMCHAIN)//theApp.Initialize();
	WRITE_LINE("system.name = \"Random Chain\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("radScale = 0.1;")
	WRITE_LINE("seed_rand(1);")
///////////////////////////////////////////////////////////////////////////////
//  default sphere radius = 0.1
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -3.2, 3.2, -3.2, 3.2, 5, 10 ] ;")
//	WRITE_LINE("system.fieldOfView = 60;")
	WRITE_LINE("system.translate.z = -3;")

	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m5 = sphere;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.radius = 0.1 + radScale*rand();")
	WRITE_LINE("m2.radius = 0.1 + radScale*rand();")
	WRITE_LINE("m3.radius = 0.1 + 4.0*radScale*rand();")
	WRITE_LINE("m4.radius = 0.1 + radScale*rand();")
	WRITE_LINE("m5.radius = 0.1 + radScale*rand();")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m5"PANTONERhodamineRedCV)
	WRITE_LINE("m4"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m2.color = [0,1,0];")
	WRITE_LINE("m1"PANTONE527CV)


	WRITE_LINE("m1.position= [ 0, 1, 0 ];")
	WRITE_LINE("m2.position= m1.position;")
	WRITE_LINE("m3.position= m1.position;")
	WRITE_LINE("m4.position= m1.position;")
	WRITE_LINE("m5.position= m1.position;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("R = m1.radius;")
	WRITE_LINE("D = 5.0*R;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m2.position.x = m1.position.x + m1.radius + m2.radius + D;")
	WRITE_LINE("m3.position.x = m2.position.x + m2.radius + m3.radius + D;")
	WRITE_LINE("m4.position.x = m3.position.x + m3.radius + m4.radius + D;")
	WRITE_LINE("m5.position.x = m4.position.x + m4.radius + m5.radius + D;")

	WRITE_LINE("m1.constraint.[0., 0., 0] ==     [0., 2., 0], 1  ;")
	WRITE_LINE("m2.constraint.[0., 0., 0] == m1.constraint.[0., 0., 0], m2.position.x - m1.position.x;")
	WRITE_LINE("m3.constraint.[0., 0., 0] == m2.constraint.[0., 0., 0], m3.position.x - m2.position.x;")
	WRITE_LINE("m4.constraint.[0., 0., 0] == m3.constraint.[0., 0., 0], m4.position.x - m3.position.x;")
	WRITE_LINE("m5.constraint.[0., 0., 0] == m4.constraint.[0., 0., 0], m5.position.x - m4.position.x;")


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("fr = -50.0;")
/*
	WRITE_LINE("m1.numericalConstraintPositionDamping = fr;")
	WRITE_LINE("m2.numericalConstraintPositionDamping = fr;")
	WRITE_LINE("m3.numericalConstraintPositionDamping = fr;")
	WRITE_LINE("m4.numericalConstraintPositionDamping = fr;")
	WRITE_LINE("m5.numericalConstraintPositionDamping = fr;")
*/
///////////////////////////////////////////////////////////////////////////////
/*
	WRITE_LINE("m1.numericalPositionDamping = [fr, fr, fr];")
	WRITE_LINE("m2.numericalPositionDamping = [fr, fr, fr];")
	WRITE_LINE("m3.numericalPositionDamping = [fr, fr, fr];")
	WRITE_LINE("m4.numericalPositionDamping = [fr, fr, fr];")
	WRITE_LINE("m5.numericalPositionDamping = [fr, fr, fr];")
*/
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionIterations = 64;")
//	WRITE_LINE("system.collisionTolerance = 0.001;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.resetDeferredCommand = 0.0, \"system.collisionIterations = 64\";")
//	WRITE_LINE("system.resetDeferredCommand = 0.0, \"system.collisionTolerance = 0.001\";")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.externalGravitationalField.y = -3;")
	WRITE_LINE("system.timeStepSize = 0.05;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnN_Body()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_STATISTICALMECHANICS_NSPHERES)//theApp.Initialize();
	WRITE_LINE("system.name = \"N Spheres\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("radScale = 1.1;")
	WRITE_LINE("seed_rand(1);")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.topRigidBoundary = 1.0;")
	WRITE_LINE("system.bottomRigidBoundary = -0.5;")
	WRITE_LINE("system.leftRigidBoundary = -0.75;")
	WRITE_LINE("system.rightRigidBoundary = 0.75;")
	WRITE_LINE("system.frontRigidBoundary = 0.75;")
	WRITE_LINE("system.backRigidBoundary = -0.75;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function MakeSpheres(n) {"
"    BeginWait();"
"	counter = 0;"
"	kounter = 0;"
"	kounter = ((counter-1)%3);"
"	posx = system.leftRigidBoundary;"
"    while(counter < n){;"
"        counter = counter+1;"
"	kounter = ((counter-1)%3);"
"	duh=\"m\"+counter+\" = sphere\";"
"	exec;"

"	duh=\"m\"+counter+\".position.y = 2.0*(m\"+counter+\".radius)*(\"+((counter/3)*radScale )+\")\";"
"	exec ;"
"	duh=\"m\"+counter+\".position.x = 2.0*(m\"+counter+\".radius)*(\"+(((counter-1)%3)*radScale )+\")\";"
/*
"	duh=\"m\"+counter+\".position.x = 2.0*(m\"+counter+\".radius)*(\"+(((counter-1)%3)*radScale )+\")\";"
"	duh=\"m\"+counter+\".position.x = 2.0*(m\"+counter+\".radius)*(\"+((kounter)*radScale )+\")\";"
*/
"	exec ;"
"    };"

"	duh=\"m\"+n+\".velocity.x = \" + -rand();"
"	exec ;"

"    EndWait();"
"	return  ( duh );"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.enforceRigidBoundary =1;")	//rigidBoundary_all_faces
///////////////////////////////////////////////////////////////////////////////[ -2.5, 2.5, -1, 4, 15, 20 ]
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  0.3 ,  2.7 ,  -1 ,  1 ]  ;")
	WRITE_LINE("system.translate=[ 0 ,  0.6 ,  6 ] ;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= 0.025;")
	WRITE_LINE("system.externalGravitationalField.y = -9.8;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.printContactInfo = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("numSpheres = 11;")
	WRITE_LINE("system.wait;")
	WRITE_LINE("MakeSpheres(numSpheres);")
	WRITE_LINE("system.waitEnd;")
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnNewtonCradle()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_NEWTONSCRADLE)//theApp.Initialize();
	WRITE_LINE("system.name = \"Newton's Cradle\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m1.radius = 0.2;")
	WRITE_LINE("m2.radius = 0.2;")
	WRITE_LINE("m3.radius = 0.2;")
	WRITE_LINE("theta = DEG_TO_RAD*20;")
	WRITE_LINE("m1.position  = [-(m1.radius+m2.radius)-sin(theta),  1.0 - cos(theta), 0 ];")
	WRITE_LINE("m2.position  = [0.0,  0, 0 ];")
	WRITE_LINE("m3.position  = [m2.radius+m3.radius,  0, 0 ];")
	WRITE_LINE(" m1.{[0, 0, 0] == {[-(m1.radius+m2.radius), 1, 0], 1;")
	WRITE_LINE(" m2.{[0, 0, 0] == {[0, 1, 0], 1;")
	WRITE_LINE(" m3.{[0, 0, 0] == {[m2.radius+m3.radius, 1, 0], 1;")
	WRITE_LINE("system.externalGravitationalField.y =  -3;")
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.timeStepSize = 0.025;")
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  -1.2 ,  1.2, 5, 10 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  -0.498   ,  -1 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnNewtonCradle2()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_NEWTONSCRADLE2)//theApp.Initialize();
	WRITE_LINE("system.name = \"Newton's Cradle (2) Approximation\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m1.radius = 0.2;")
	WRITE_LINE("m2.radius = 0.2;")
	WRITE_LINE("m2.mass = 2;")
	WRITE_LINE("m3.radius = 0.2;")
	WRITE_LINE("theta = DEG_TO_RAD*20;")
	WRITE_LINE("m1.position  = [-(m1.radius+m2.radius)-sin(theta),  1.0 - cos(theta), 0 ];")
	WRITE_LINE("m2.position  = [ 0, 0, 0 ];")
	WRITE_LINE("m3.position  = [  m2.radius+m3.radius +sin(theta),  1.0 - cos(theta), 0 ];")
	WRITE_LINE(" m1.{[0, 0, 0] == {[-(m1.radius+m2.radius), 1, 0], 1;")
	WRITE_LINE(" m2.{[0, 0, 0] == {[0, 1, 0], 1;")
	WRITE_LINE(" m3.{[0, 0, 0] == {[m2.radius+m3.radius, 1, 0], 1;")
//
/*
	WRITE_LINE("m1.velocity.x  = 1;")
	WRITE_LINE("m3.velocity.x  = - m1.velocity.x;")
	WRITE_LINE("m1.position.y  = 0;")
	WRITE_LINE("m3.position.y  = 0;")
	WRITE_LINE("m1.mue = 1;")
	WRITE_LINE("m3.mue = 1;")
*/
	WRITE_LINE("system.externalGravitationalField.y =  -3;")
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.timeStepSize = 0.01;")
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  -1.2 ,  1.2, 5, 10 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  -0.498   ,  -1 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnThreeBodyCollision()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_THREEBODY)//theApp.Initialize();
	WRITE_LINE("system.name = \"Three Body Collision Approximation\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m1.radius = 0.2;")
	WRITE_LINE("m2.radius = 0.2;")
	WRITE_LINE("m2.mass = 2;")
	WRITE_LINE("m3.radius = 0.2;")

	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
//	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m3"PANTONE527CV)


	WRITE_LINE("theta = DEG_TO_RAD*20;")
	WRITE_LINE("m1.position  = [-(m1.radius+m2.radius)-sin(theta),  1.0 - cos(theta), 0 ];")
	WRITE_LINE("m2.position  = [0.0,  -.1, 0 ];")
	WRITE_LINE("m3.position  = [  m2.radius+m3.radius +sin(theta),  1.0 - cos(theta), 0 ];")

	WRITE_LINE("m1.velocity  = [.1,  0, 0 ];")
//	WRITE_LINE("m3.velocity  = [-.1,  0, 0 ];")
	WRITE_LINE("m3.velocity  = -m1.velocity;")
	WRITE_LINE("system.collisionDetection = true;")
//	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.contacts.show = true;")
	WRITE_LINE("system.timeStepSize = 0.1;")
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  -1.2 ,  1.2, 1.05, 20 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  -0.498   ,  -2 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnFourBodyCollision()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_FOURBODYCOLLISION)//theApp.Initialize();
	WRITE_LINE("system.name = \"Four Body Collision Approximation\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m4 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m1 = sphere;")

	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"PANTONEYellowCV)
	WRITE_LINE("m4"PANTONE527CV)

	WRITE_LINE("m1.radius = 0.5;")
	WRITE_LINE("m2.radius = m1.radius;")
	WRITE_LINE("m3.radius = m1.radius;")
	WRITE_LINE("m4.radius = m1.radius;")

	WRITE_LINE("m1.mass = 5./(2*m1.radius^2);")
	WRITE_LINE("m2.mass = m1.mass;")
	WRITE_LINE("m3.mass = m1.mass;")
	WRITE_LINE("m4.mass = m1.mass;")

	WRITE_LINE("m1.position  = [-2*m1.radius,  0, 0 ];")
	WRITE_LINE("m2.position  = [0, -2*m2.radius,  0 ];")
	WRITE_LINE("m3.position  = -m1.position;")
	WRITE_LINE("m4.position  = -m2.position;")
	
	WRITE_LINE("m1.velocity  = [1, 0, 0 ];")
	WRITE_LINE("m2.velocity  = [0, 1, 0 ];")
	WRITE_LINE("m3.velocity  = -m1.velocity;")
	WRITE_LINE("m4.velocity  = -m2.velocity;")

	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.contacts.show = true;")
	WRITE_LINE("system.timeStepSize = 0.01;")
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  -1.2 ,  1.2, 5, 10 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  0   ,  -3.5 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
//numerical algorithm fails simply because row order changes:
//!even with pivoting!!
void CSolveItView::OnFourBodyCollision2()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_FOURBODYCOLLISION2)//theApp.Initialize();
	WRITE_LINE("system.name = \"Four Body Collision Approximation\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m4 = sphere;")

	WRITE_LINE("m1"PANTONERhodamineRedCV)
	WRITE_LINE("m2"PANTONEOrange021CV)
	WRITE_LINE("m3"Gold)
	WRITE_LINE("m4"PANTONE527CV)
//PANTONEYellowCV
	WRITE_LINE("m1.radius = 0.5;")
	WRITE_LINE("m2.radius = m1.radius;")
	WRITE_LINE("m3.radius = m1.radius;")
	WRITE_LINE("m4.radius = m1.radius;")

	WRITE_LINE("m1.mass = 5./(2*m1.radius^2);")
	WRITE_LINE("m2.mass = m1.mass;")
	WRITE_LINE("m3.mass = m1.mass;")
	WRITE_LINE("m4.mass = m1.mass;")

	WRITE_LINE("m1.position  = [-2*m1.radius,  0, 0 ];")
	WRITE_LINE("m2.position  = [0, -2*m2.radius,  0 ];")
	WRITE_LINE("m3.position  = -m1.position;")
	WRITE_LINE("m4.position  = -m2.position;")
	
	WRITE_LINE("m1.velocity  = [1, 0, 0 ];")
	WRITE_LINE("m2.velocity  = [0, 1, 0 ];")
	WRITE_LINE("m3.velocity  = -m1.velocity;")
	WRITE_LINE("m4.velocity  = -m2.velocity;")

	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.contacts.show = true;")
	WRITE_LINE("system.timeStepSize = 0.01;")
	WRITE_LINE("system.boundingBox = [ -1.2 ,  1.2 ,  -1.2 ,  1.2, 5, 10 ] ;")
	WRITE_LINE("system.translate = [ 0 ,  0   ,  -3.5 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnSubmerged()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_CHAOTIC_SUBMERGED)//theApp.Initialize();
	WRITE_LINE("system.name = \"Submerged\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("radScale = 1.6;")
	WRITE_LINE("seed_rand(1);")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.topRigidBoundary = 1.5;")
	WRITE_LINE("system.bottomRigidBoundary = 0.0;")
	WRITE_LINE("system.leftRigidBoundary =  0.0;")
	WRITE_LINE("system.rightRigidBoundary = 1.0;")
	WRITE_LINE("system.frontRigidBoundary = 0.75;")
	WRITE_LINE("system.backRigidBoundary = -0.75;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function MakeSphereLayers(n, nLay) {"
"BeginWait();"
"	counter = 0;"
"	numSpheresPerLayer = 0;"
"	numSpheresPerLayer = nLay;"
" while(counter < n){;"
"	counter = counter+1;"
"	duh=\"m\"+counter+\" = sphere\";"
"	exec;"

"	duh=\"m\"+counter+\".radius = 0.05\";"
"	exec;"

"	duh=\"m\"+counter+\".position.y = 2.0*(m\"+counter+\".radius)*(\"+ (0.5 + ((counter-1)/nLay)*radScale )+\")\";"
"	exec ;"
"	duh=\"m\"+counter+\".position.x = 2.0*(m\"+counter+\".radius)*(\"+ (0.5 + ((counter-1)%nLay)*radScale )+\")\";"
"	exec ;"

"};"

"	counter = nLay;"
" while(counter < n){;"
"	counter = counter+1;"

"	duh=\"m\"+counter+\".position.y = m\"+counter+\".position.y + 0.226\";"
"	exec ;"

"};"

"	duh=\"m\"+n+\".velocity.x = \" + -rand();"
"	exec ;"

"EndWait();"
"	return  ( duh );"
	"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.enforceRigidBoundary =1;")	//rigidBoundary_all_faces
///////////////////////////////////////////////////////////////////////////////[ -2.5, 2.5, -1, 4, 15, 20 ]
	WRITE_LINE("system.boundingBox = [ -0.4 ,  1.6 ,  0.5 ,  2.5 ,  -1 ,  1 ]  ;")
	WRITE_LINE("system.translate=[ 0 , 0.896 ,  5 ] ;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= 0.025;")
	WRITE_LINE("system.externalGravitationalField.y = -9.8;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.printContactInfo = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1 = sphere;")
	WRITE_LINE("s1.position = [ 0.5, 2.05*s1.radius, 0 ];")
	WRITE_LINE("s1"HotPink)
	WRITE_LINE("numSpheres = 54;")
	WRITE_LINE("numSpheresPerLayer = 6;")
	WRITE_LINE("MakeSphereLayers(numSpheres, numSpheresPerLayer);")
	
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnTest()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_TEST)//theApp.Initialize();
	WRITE_LINE("system.name = \"Test\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -1, 1, PI/2, 2+PI/2, -1, 1 ] ;")
	WRITE_LINE("system.translate=[ 0 , 0,2 ,  5 ] ;")

	WRITE_LINE("function V(x) {"
"	return (EllipticK(x));"
	"}")

	WRITE_LINE("system.potential.name = 'V';")
	WRITE_LINE("system.potential.nargs = 1;")
	WRITE_LINE("system.potential.time = false;")
	WRITE_LINE("system.numPoints = 101;")
	WRITE_LINE("system.potential = true;")
	WRITE_LINE("system.potential.show = true;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("EllipticK(0.5);")
	WRITE_LINE("V(0.85);")
	WRITE_LINE("EllipticK(0.95);")
	WRITE_LINE("EllipticF(PI/4, 0.85);")
	WRITE_LINE("EllipticEc(0.95);")
	WRITE_LINE("EllipticE(PI/4, 0.85);")
#if 0
	WRITE_LINE("print system;")
	WRITE_LINE("system.propPrint;")
///////////////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
}

/*
	WRITE_LINE("	duh=\"m\"+counter+\".position.y = 2.0*(m\"+counter+\".radius)*(\"+ (0.5 + ((counter-1)/numSpheresPerLayer)*radScale )+\")\";")
	WRITE_LINE("	exec ;")
	WRITE_LINE("	duh=\"m\"+counter+\".position.x = 2.0*(m\"+counter+\".radius)*(\"+(((counter-1)%numSpheresPerLayer)*radScale )+\")\";")
	WRITE_LINE("	exec ;")
*/

#undef WRITE_LINE
#undef WRITE_LINE

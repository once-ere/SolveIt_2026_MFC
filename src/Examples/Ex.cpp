// Ex.cpp : Implementation of CIO

#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItView.h"
#include "GLView.h"
#include "MainFrm.h"
//#include "GenExamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ScriptColors.h"
//system.stop reset
void CSolveItView::OnSimpleHarmonicOscillator()
{
//	//Fire_ClearUserDefs();reset
//	WRITE_LINE(Clear_Interpreter)

/*
	CreateGenExampThread(true);
	if (m_pCGenExamp)
	{
		m_pCGenExamp->ResumeThread();
		m_pCGenExamp->PostThreadMessage(ID_POPUP_OSCILLATORS_SIMPLEHARMONIC, 0, 0);
	}

	return;
*/


//	Create_GenExampThread(ID_POPUP_OSCILLATORS_SIMPLEHARMONIC)//theApp.Initialize();


	WM_APP_INIT_2(0,0)

	WRITE_LINE("system.name = \"Simple Harmonic Oscillator\";")
	WRITE_LINE("s1 = sphere;")
	WRITE_LINE("s1.radius = 0.2;")
	WRITE_LINE("s1.mass =  1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("spring_constant = 4.0;")
	WRITE_LINE("function fx(x$) {"
"	return -spring_constant*x;"
"}")


	WRITE_LINE("s1.externalForce.x = \"fx\";")
	WRITE_LINE("s1.externalForce.x.time = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.position =  [0.64, 0, 0];")
	WRITE_LINE("s1"PANTONERubineRedCV)
	WRITE_LINE("s1.momentum.show = true;")
//	WRITE_LINE(" s1.record = \" period 0.0, 5\";")
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5];")
	WRITE_LINE("system.translate.z = -1.463;")
	WRITE_LINE("system.timeStepSize = 0.01;")
//	WRITE_LINE("system.timer = 100;")

	WRITE_LINE("system.tic_mark.x = 51;")
	WRITE_LINE("system.tic_mark.y = 41;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.record.time.begin = 0.0;")
	WRITE_LINE("s1.record.time.end = 6.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.record.period = true;")
	WRITE_LINE("system.deferredCommand = \"6.75; system.stop;\";")
	WRITE_LINE("system.deferredCommand = \"6.75; s1.analyze = 'period: [1, 0, 0]';\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.run = 100;")//

	std::wstring bstr = L"\
Simple Harmonic Oscillator\r\n\
\r\n\
	To calculate the period execute:\r\n\
\r\n\
s1.analyze \"period: 1, 0, 0\"\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 5\r\n\
\r\n\
Here, the x-component of the external electric field is E_x = 4 x = A x,\r\n\
where A = 4 V/m^2. \r\n\
The mass of the moving particle is M =  1.\r\n\
The charge of the moving particle is Q = - 1.\r\n\
The x-component of the force is F_x = Q E_x = - 4 x, \r\n\
Let us write this as F_x = Q E_x = Q A x = - 4 x = - k x , \r\n\
where k = 4 = -Q A = |Q| A > 0 is the effective spring constant.\r\n\
The square of the angular frequency is k / M .\r\n\
The theoretical value of the period is\r\n\
T = 2 PI Sqrt( M / k ) = 2 PI Sqrt( M / |Q| A ).\r\n\
Here A = 4, M = 1, Q = -1 => k = 1,  and \r\n\
T = PI = 3.1415926535897932384626433832795... .\r\n\
";

//	Fire_WriteResponse(bstr.c_str());
	
}

void CSolveItView::OnResonantSimpleHarmonicOscillator()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_OSCILLATORS_RESONANTSIMPLEHARMONIC)//theApp.Initialize();
	WRITE_LINE("system.name = \"Resonant Simple Harmonic Oscillator\";")
	WRITE_LINE("s1 = sphere;")
	WRITE_LINE("s1.charge =  -1;")
	WRITE_LINE("s1.radius =.2;")
	WRITE_LINE("s1.position =  [0.04, 0, 0];")
	WRITE_LINE("s1"PANTONE731CV)
	WRITE_LINE("s1.drawStyle = 0;")
	WRITE_LINE("s1.mass =  1;")
	WRITE_LINE(" s1.record = \" period 4, 11\";")
	WRITE_LINE("system.extEx1 = 4;")
	WRITE_LINE("system.extSin.x = [0.5, 2.0, 1.5707963267948966192313216916398];")
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  5 ,  10];")
	WRITE_LINE("system.timeStepSize = 0.1;")

	std::wstring bstr = L"\
Resonant Simple Harmonic Oscillator\r\n\
\r\n\
The x-component of the force is Fx = -kx = Q A x, \r\n\
where k = |Q| A > 0 and Q = charge of moving particle.\r\n\
The theoretical value of the period is\r\n\
T = 2 PI Sqrt( M / k ),\r\n\
where M = mass of particle.\r\n\
Here A = 4, M = 1, Q = -1 => k = 4,  and \r\n\
T = PI = 3.1415926535897932384626433832795... (omega = 2 PI / T = 2).\r\n\
\r\n\
	A time dependent external force = A cos( W*t + phi)\r\n\
											 = 0.5 cos( 2 PI t / T + 1.570796)\r\n\
											 = 0.5 cos( 2 t + 1.570796)\r\n\
\r\n\
drives the oscillator at its resonant frequency.\r\n\
\r\n\
To give the particle a damping force = - b Vx \r\n\
where b = Damping Coefficient >= 0, you may use the command 'ed 1' then\r\n\
move to 'Scalar Properties' and change the x Damping Coefficient from 0.\r\n\
\r\n\r\n\
At the resonance frequency the theoretical oscillation amplitude is\r\n\
	  A / (omega b)\r\n";
	std::wstring bstr2 = L"\
This gives A = 0.5 XXXXXXXXXXXX ( 2*2.5 ) = 0.1 for b = 2.5  . \r\n\
If b is small it may take many cycles to attain this value\r\n\
(elapsed time ~ 2 M / b if 4 k M > b^2 ).\r\n\r\n\
	To calculate the amplitude and period execute:\r\n\
RecordData 1 4 11\r\n\
	then\r\n\
AnalyzeData 1 period i\r\n\
\r\n\
after the simulator has run from time = 4 to time >= 11\r\n\
\r\n\
You may use the user interface up/down (right/left) controls\r\n\
to vary 'Omega' = angular frequency of external force,\r\n\
'Amplitude' = amplitude of external force \r\n\
and 'Phase' = phase of external force.\r\n\
\r\n\
";

	Fire_WriteResponse( (bstr+bstr2).c_str() );
	
}
void CSolveItView::OnCyclotron()
{
//	NewWebBrowser(2);
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_OSCILLATORS_CYCLOTRON)//theApp.Initialize();
	WRITE_LINE("system.name = \"Cyclotron\";")
	WRITE_LINE("s = sphere;")
	WRITE_LINE("s.color = [ 0.1, 0.2, 0.4 ];")
	WRITE_LINE("s.position = [  0.63, 0, 0 ];")
	WRITE_LINE("s.charge =  -1;")
	WRITE_LINE("s.mass =  0.81;")

	WRITE_LINE("s.velocity = [0, 0.4321, 0];")
//	WRITE_LINE("s.velocity.y = 0.4321;")

	WRITE_LINE("s.drawStyle =  0;")
	WRITE_LINE(" s.record = \" period 0.0, 6.5\";")
	WRITE_LINE("system.timeStepSize = 0.1;")
	WRITE_LINE("system.externalMagneticField = [0, 0, 1];")
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  5 ,  10];")

	std::wstring bstr = L"\
Charged Particle Moving in a Uniform Magnetic Field Along the z-Axis\r\n\
\r\n\
	To calculate the period execute:\r\n\
\r\n\
AnalyzeData 1 period i\r\n\
or\r\n\
AnalyzeData 1 period j\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 6.5\r\n\
\r\n\
The theoretical value of the period is\r\n\
T = 2 PI ( mass / |charge| ) ( 1 / B ) ,\r\n\
where B = magnetic field strength.\r\n\
In this example B = 1, mass = 0.81, charge = -1 and T = 5.08938.\r\n\
";

	Fire_WriteResponse(bstr.c_str());
	
}


void CSolveItView::OnCubicAnharmonicOscillator()
{
//	NewWebBrowser(3);
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_OSCILLATORS_NONLINEARCUBIC)//theApp.Initialize();
	WRITE_LINE("system.name = \"Cubic Anharmonic Oscillator\";")
	WRITE_LINE("m = sphere;")
	WRITE_LINE("m.radius =  0.25;")
	WRITE_LINE("m.mass =  1;")
	WRITE_LINE("m.charge =  -1;")
	WRITE_LINE("m.position = [  0.64, 0, 0 ];")
	WRITE_LINE("m.radius = .1;")
//	WRITE_LINE("m.color = [ 1, 0, 1 ];")
	WRITE_LINE("m"PANTONERhodamineRedCV)
	WRITE_LINE("m.drawStyle = 0;")
	WRITE_LINE(" m.record = \" period 0.0, 5\";")
	WRITE_LINE("system.extEx1 = 4;")
	WRITE_LINE("system.extEx2 = 4;")
	WRITE_LINE("system.extEx3 = 4;")
	WRITE_LINE("system.timeStepSize = 0.05;")

	std::wstring bstr = L"\
Nonlinear Cubic Anharmonic Oscillator\r\n\
\r\n\
	To calculate the period execute:\r\n\
\r\n\
m.analyze \"period: 1, 0, 0\"\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 4\r\n\
\r\n\
The x-component of the external force is Fx = Q(A1 x + A2 x^2 + A3 x^3), \r\n\
In this example the particle starts from rest at Xinitial = 0.64  . \r\n\
Parameters for this example are A1 = 4 = A2 = A3, Q = -1 and M = 1. \r\n\
T = 2.86432 (omega = 2.1936) .\r\n\
";

	Fire_WriteResponse(bstr.c_str());
	
}



void CSolveItView::OnAnharmonicOscillator()
{
//	NewWebBrowser(4);
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_OSCILLATORS_ANHARMONIC)//theApp.Initialize();
	WRITE_LINE("system.name = \"Anharmonic Oscillator\";")
	WRITE_LINE("One = sphere;")
	WRITE_LINE("Two = sphere;")
	WRITE_LINE("Three = sphere;")
	WRITE_LINE("One.color = [ 1, 0, 1 ];")
	WRITE_LINE("Two.color = [ 0, 1, 0 ];")
//	WRITE_LINE("Three.color = [ 0, 0, 0 ];")
	WRITE_LINE("Three.color = [ 0.749, 0.635, 0.384 ];")
	WRITE_LINE("One.position   = [  -.8,   0, 0 ];")
	WRITE_LINE("Two.position   = [   .8,   0, 0 ];")
	WRITE_LINE("Three.position = [   .13,  0, 0 ];")
	WRITE_LINE("One.charge = 1;")
	WRITE_LINE("Two.charge = 1;")
	WRITE_LINE("One.inverseMass = 0;")
	WRITE_LINE("Two.inverseMass = 0;")
	WRITE_LINE("Three.charge =  1;")
	WRITE_LINE("Three.mass =  1;")
	WRITE_LINE("One.drawStyle   = 0;")
	WRITE_LINE("Two.drawStyle   = 0;")
	WRITE_LINE("Three.drawStyle = 0;")
	WRITE_LINE("Three.mass =  1;")
	WRITE_LINE(" Three.record = \" period 0.0, 2.85\";")
	WRITE_LINE("system.timeStepSize = 0.1;")

	std::wstring bstr = L"\
Charged Particle Moving Between Two Fixed Charges\r\n\
\r\n\
	To calculate the period execute:\r\n\
\r\n\
Three.analyze \"period: 1, 0, 0\"\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 2.85\r\n\
\r\n\
In the limit of arbitrarily small amplitude oscillations, the theoretical value\r\n\
of the period T = PI d / 2 Sqrt( m d / q Q 2 ),\r\n\
where the moving charge =  has mass =  m and charge =  q and\r\n\
the fixed charges have charge =  Q and are separated by a distance d.\r\n\
In this example m = 1 = q = Q and T = 2.2479407 as d -> 0.\r\n\
In this example d = 1.6; reduce d to 0.01 using the command \r\n\
'Three s.position.x  =  0.01' (without the quotes),\r\n\
and so forth.\r\n\
";
	Fire_WriteResponse(bstr.c_str());
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnSimplePendulum()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_OSCILLATORS_SIMPLEPENDULUM)//theApp.Initialize();
	WRITE_LINE("system.name = \"Simple Pendulum\";")
	WRITE_LINE("One = sphere;")
	WRITE_LINE("One.radius = .07;")
	WRITE_LINE("lengthPendulum = 1.0;")
	WRITE_LINE("X_Pendulum = 0.98;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("One.position  = [0.98,  0.80100251257867600905310403579976, 0 ];")
	WRITE_LINE("One.position  = [X_Pendulum,  lengthPendulum - sqrt(lengthPendulum^2 - X_Pendulum^2), 0 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("One.color = [ 1, 0, 1 ];")
//	WRITE_LINE("One.numericalPositionDamping = [0.1, 0.1, 0.1];")
//	WRITE_LINE("One.numericalVelocityDamping = [0, 0, 0];")

	WRITE_LINE(" One.constraint.[0, 0, 0] == [0, 1, 0], lengthPendulum;")

	WRITE_LINE("system.externalGravitationalField.y =  -3;")
//	WRITE_LINE("system.boundingBox = [ -1, 1, -1, 1, 5, 10 ] ;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -1. ,  1. ,  -0.5 ,  1.5 ,  -1. ,  1.];")
	WRITE_LINE("system.translate.z = 0;")
	WRITE_LINE("system.timeStepSize = 0.01;")
//	WRITE_LINE("system.timer = 100;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("One.record.time.begin = 0.0;")
	WRITE_LINE("One.record.time.end = 6.5;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("One.record.period = true;")
//	WRITE_LINE(" One.record = \" period 0.0, 6.5\";")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"6.75; system.stop;\";")
	WRITE_LINE("system.deferredCommand = \"6.75; One.analyze = 'period: [1, 0, 0]';\";")
	WRITE_LINE("system.deferredCommand = \"6.75; One.analyze = 'period: [0, 1, 0]';\";")
///////////////////////////////////////////////////////////////////////////////

	std::wstring bstr = L"\
Simple Pendulum\r\n\
\r\n\
	To calculate the period execute:\r\n\
\r\n\
One.analyze \"period: 1, 0, 0\" \r\n\
(Can\r\n\
One.analyze \"period: 0, 1, 0\"\r\n\
yield a valid result? If so, how?)\r\n\
\r\n\
after the simulator has run from time = 0 to time >= 6.5\r\n\
\r\n\
In the limit of arbitrarily small amplitude 'A' oscillations, the theoretical\r\n\
value of the period T = 2 PI Sqrt( L / g),\r\n\
where L = length of pendulum and\r\n\
g = acceleration due to gravity.\r\n\
In this example L = 1, g = 3 and T = 3.6276 as 'A' -> 0.\r\n\
Reduce 'A' to 0.01 using the command 'One.position.x  =  0.01' (without the quotes),\r\n\
and so forth.\r\n\r\n\
In general the theoretical value of the period is\r\n\
T = 4 Sqrt( L / g ) Integral[from 0][to PI/2] {1 / sqrt[ 1 - k^2 sin^2(x) ]}{ dx},\r\n\
where k^2 = 0.5*(1 - sqrt[1 - (Xinitial / L)^2]) .\r\n\
In this example, Xinitial / L = 0.98 => k^2 = 0.400501 and T = 4.10581 .\r\n\
";
//	Fire_WriteResponse(bstr.c_str());
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnPhysicalPendulum()
{
	Create_GenExampThread(ID_POPUP_CHAOTIC_PHYSICAL_PENDULUM)//theApp.Initialize();
	WRITE_LINE("system.name = \"Physical Pendulum\";")
	WRITE_LINE("m = block;")
	WRITE_LINE("m.width =  .4;")
	WRITE_LINE("m.height = .6;")
	WRITE_LINE("m.depth =  .5;")

	WRITE_LINE("m.position = [ 0.2, 1, 0.25 ];")
	WRITE_LINE("m.constraint.[-0.2, 0.3, -0.25]  == [0.0, 1.3, 0.00], 0.0, \"zero velocity\";")
//	WRITE_LINE("m.constraint.[-0.2, 0.3, -0.25]  == [0.0, 1.3, 0.00], 0.0;")
	WRITE_LINE("m.color = [ 0.65, 0.3, 0.1 ];")

	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -2. ,  2. ,  -1.5 ,  2.5 ,  2. ,  4.];")
//	WRITE_LINE("system.boundingBox = [ -2. ,  2. ,  -1.5 ,  2.5 ,  2. ,  40.];")
//	WRITE_LINE("system.translate.z = -2.8;")
	WRITE_LINE("system.translate.z = 0;")
	WRITE_LINE("system.timeStepSize = 0.01;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m.record.time.begin = 0.0;")
//	WRITE_LINE("m.record.time.end = 6.5;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m.record.period = true;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.deferredCommand = \"6.75; system.stop;\";")
//	WRITE_LINE("system.deferredCommand = \"6.75; m.analyze = 'period: [1, 0, 0]';\";")
//	WRITE_LINE("system.deferredCommand = \"6.75; m.analyze = 'period: [0, 1, 0]';\";")
//	WRITE_LINE("system.rotate = [ 0 ,  45. ,  0 ] ;")
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnCompoundPendulum()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_CHAOTIC_COMPOUNDPENDULUM)//theApp.Initialize();
	WRITE_LINE("system.name = \"Compound Pendulum\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("radScale = 0.4;")
//	WRITE_LINE("seed_rand(1.0);")
	WRITE_LINE("seed_rand(0);")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")

	WRITE_LINE("m1.radius= rand()*0.15;")
	WRITE_LINE("m2.radius= rand()*0.15;")
//	WRITE_LINE("m1.radius = 0.1 + radScale*rand();")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.color = [ 0.5, 0.3, 0.1 ];")
//	WRITE_LINE("m2.color = [ 1, 0.4, 0 ];")
//	WRITE_LINE("m1"PANTONE2718CV)
//	WRITE_LINE("m2"PANTONE2727CV)
	WRITE_LINE("m1"PANTONEOrange021CV)
	WRITE_LINE("m2"PANTONEPurpleCV)
// PANTONEYellowCV
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.position = [ 0, 1, 0 ];")
	WRITE_LINE("m2.position= m1.position + [2.0*( rand() - 0.5), radScale*( rand() - 0.5), 0 ];")
//	WRITE_LINE("m2.position= m1.position + [-.4, 0, 0 ];")

	WRITE_LINE("m1.constraint. [0, 0, 0] ==    [0, 2, 0], 1;")
	WRITE_LINE("m2.constraint. [0, 0, 0] == m1.constraint.[0, 0, 0], | m1.position - m2.position | ;")

	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.externalGravitationalField.y = -1;")
	WRITE_LINE("system.timeStepSize = 0.05;")
	WRITE_LINE("system.boundingBox = [ -2. ,  2. ,  -1.5 ,  2.5 ,  2. ,  40.];")
	WRITE_LINE("system.translate.z = -2.8;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnSphereAndBlock()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_SPHEREANDBLOCK)//theApp.Initialize();
	WRITE_LINE("system.name = \"Sphere and Blocks\";")

	WRITE_LINE("s = sphere;")
	WRITE_LINE("s"NeonPurple)
/*
*/
	WRITE_LINE("s.radius = 0.2;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Block = block;")
	WRITE_LINE("Block.width =  .4;")
	WRITE_LINE("Block.height = .2;")
	WRITE_LINE("Block.depth =  .6;")
//	WRITE_LINE("Block.rot = 45 1 2 1;")
	WRITE_LINE("Block.orientation= [45, 1, 2, 1];")
//	WRITE_LINE("Block.color = [ 0.6, 1, 0.8 ];")
	WRITE_LINE("Block"Gold)
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s.mass = 1;")
//	WRITE_LINE("s.position = [  -1.5, 0, 0 ];")
	WRITE_LINE("s.position = [  -1.39152 ,  -0.73632 , 0 ];")
//	WRITE_LINE("s.velocity = [0.5, 0, 0];")
	WRITE_LINE("s.velocity = [0.0, 0, 0];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("Block.orientation= [-45, 0, 0, 1];")
	WRITE_LINE("Block.mass = 1;")
	WRITE_LINE("Block.position = [   0.5, -0.1, 0 ];")
//	WRITE_LINE("Block.velocity = [ -0.5, 0, 0];")
	WRITE_LINE("Block.velocity = [ -0.0, 0, 0];")
	WRITE_LINE("Block.angular_momentum = [ 0, 0.001, 0.02];")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m = block;")
	WRITE_LINE("m.width =  .4;")
	WRITE_LINE("m.height = .3;")
	WRITE_LINE("m.depth =  .5;")
	WRITE_LINE("m.orientation= [45, 1, 2, 1];")
	WRITE_LINE("m"PANTONEYellowCV)
	WRITE_LINE("m.position = [   -0.41, 0.75, 0 ];")
	WRITE_LINE("m.angular_momentum = [   -0.01, 0.0075, 0.031 ];")
	WRITE_LINE("m.mass = 1;")
/*
*/
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize = 0.0025;")
	WRITE_LINE("system.gravitationalCoupling = 1;")

	WRITE_LINE("system.collisionDetection = true;")
//	WRITE_LINE("system.collisionDetection.show = true;")
	WRITE_LINE("system.contacts.show = true;")

	WRITE_LINE("system.boundingBox = [  -1.25, 1.25, -1.25, 1.25, 2, 30 ];")
	WRITE_LINE("system.translate.z = -3.7;")
//	WRITE_LINE("system.rotate = [ -4.8 ,  142.4 ,  0 ] ;")
	
}
///////////////////////////////////////////////////////////////////////////////
// Lennard-Jones
void CSolveItView::OnLennardJones()
{
//	//Fire_ClearUserDefs();
//	WRITE_LINE(Clear_Interpreter)
	Create_GenExampThread(ID_POPUP_MECHANICS_LENNARDJONESPOTENTIAL)//theApp.Initialize();
	WRITE_LINE("system.name = \"Lennard-Jones Potential\";")
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m3 = sphere;")
	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 1;")
	WRITE_LINE("m3.mass = 1;")
	WRITE_LINE("m1.color = [ 0.74901960784313725490196078431373, 0.63529411764705882352941176470588, 0.38431372549019607843137254901961 ];")
	WRITE_LINE("m2.color = [ 0.63529411764705882352941176470588, 0.38431372549019607843137254901961, 0.74901960784313725490196078431373 ];")
	WRITE_LINE("m3.color = [ 0.38431372549019607843137254901961, 0.74901960784313725490196078431373, 0.63529411764705882352941176470588 ];")

	WRITE_LINE("m1.position = [   1, -0.1, 0 ];")
	WRITE_LINE("m2.position = [  -1,  0,   0 ];")
	WRITE_LINE("m3.position = [   0,  1,   0 ];")
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  5 ,  10 ] ;")
//	WRITE_LINE("system.translate.z =  2.2;")
//	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, 10, 20 ] ;")
	WRITE_LINE("system.translate.z =  0;")


	WRITE_LINE("system.timeStepSize= 0.1;")
//	WRITE_LINE("system.potLennardJones =  [1,10.22,2.556];")
//	WRITE_LINE("system.potLennardJones =  [1,4,4];")
	WRITE_LINE("system.potLennardJones =  [1,1.2599210498948731647672106072782,1.1224620483093729814335330496792];")

	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnRamp()
{
	Create_GenExampThread(ID_POPUP_MECHANICS_RAMP)//theApp.Initialize();
	WRITE_LINE("system.name = \"Inclined Plane\";")

	WRITE_LINE("Sphere = sphere;")
	WRITE_LINE("Cylinder = cylinder;")
	WRITE_LINE("Block = block;")

	WRITE_LINE("Block"PANTONERubineRedCV)
	WRITE_LINE("Cylinder"PANTONE2728CV)
	WRITE_LINE("Sphere"PANTONE731CV)

//	WRITE_LINE("Block.drawStyle = 0;")

	WRITE_LINE("Sphere.coefficientOfKineticFriction = 0.3;")
	WRITE_LINE("Sphere.coefficientOfStaticFriction = 0.5;")
	WRITE_LINE("Cylinder.coefficientOfKineticFriction = 0.3;")
	WRITE_LINE("Cylinder.coefficientOfStaticFriction = 0.5;")

	WRITE_LINE("Sphere.radius = 0.2;")
	WRITE_LINE("Block.height = 2*Sphere.radius;")
	WRITE_LINE("Block.width = Block.height;")
	WRITE_LINE("Block.depth = 2*Block.height;")
	WRITE_LINE("Cylinder.radius = Sphere.radius;")
	WRITE_LINE("Cylinder.depth = 2*Block.height;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("AngleOfIncline = PI/4;")
	WRITE_LINE("ramp_x = 0.0;")
	WRITE_LINE("ramp_y = 0.5;")
	WRITE_LINE("phase = 0.05;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function RampY(x$) {"
"	Normal_x = - sin(AngleOfIncline);"
"	Normal_y =   cos(AngleOfIncline);"
"	NormalDotPoint = Normal_x*ramp_x + Normal_y*ramp_y;"
"	return ( (NormalDotPoint - Normal_x*x)/Normal_y);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE(" Sphere.plane.angle  = AngleOfIncline*RAD_TO_DEG;")
	WRITE_LINE(" Cylinder.plane.angle  = AngleOfIncline*RAD_TO_DEG;")

	WRITE_LINE(" Sphere.plane.thruPoint= [1  , RampY(1)  , 0];")
	WRITE_LINE(" Cylinder.plane.thruPoint= [2  , RampY(2)  , 0];")

	WRITE_LINE("Block.orientation= [AngleOfIncline*RAD_TO_DEG, 0, 0, 1];")
	WRITE_LINE(" Block.plane.angle  = AngleOfIncline*RAD_TO_DEG;")

	WRITE_LINE(" Block.plane.thruPoint= [0.5, RampY(0.5), 0];")



	WRITE_LINE("Sphere.normal.show = true;")
	WRITE_LINE("Cylinder.normal.show = true;")

	WRITE_LINE("Block.velocity.show = 1;")
	WRITE_LINE("Cylinder.velocity.show = 1;")
	WRITE_LINE("Sphere.velocity.show = 1;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.externalGravitationalField.y =  -0.2;")
	WRITE_LINE("system.collisionDetection = true;")
//	WRITE_LINE("system.arc = true;")
	WRITE_LINE("system.timeStepSize = 0.075;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.boundingBox = [ -2.5 ,  2.5 ,  -2.5 ,  2.5 ,  -2.5 ,  2.5 ] ;")
//	WRITE_LINE("system.translate.z =  -3;")
	WRITE_LINE("system.translate.z =  0;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnPulley()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_PULLEY)//theApp.Initialize();
	WRITE_LINE("system.name = \"Fixed Pulley and Block\";")

	WRITE_LINE("Pulley = cylinder;")
	WRITE_LINE("Two = block;")
	WRITE_LINE("Pulley.inverseMass = 0;")
	WRITE_LINE("Two.width =  .4;")
	WRITE_LINE("Two.height = .4;")
	WRITE_LINE("Two.depth =  .6;")
	WRITE_LINE("Pulley.radius = 0.2;")
	WRITE_LINE("Pulley.depth =  .4;")

	WRITE_LINE("Pulley.drawStyle = 0;")
	WRITE_LINE("Two.drawStyle = 0;")

	WRITE_LINE("Pulley.color = [ 0.5, 0, 1 ];")
	WRITE_LINE("Two.color = [ 1, 0.6, 0.8 ];")

	WRITE_LINE("Pulley.position = [ 0, 0, 0 ];")
	WRITE_LINE("Two.position = [  -2.0,  0.18, 0 ];")
	WRITE_LINE("Two.velocity = [0, -2.0, 0];")
	WRITE_LINE("Two.velocity.show = 1;")

//	WRITE_LINE("system.pulley Pulley.left == Two.quadrant 2;")
	WRITE_LINE("Pulley.pulley.left = Two.quadrant.two;")


	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.externalGravitationalField.y =  -1;")
	WRITE_LINE("system.timeStepSize = 0.025;")

	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnTwoBodyScattering()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_TWOBODY)//theApp.Initialize();
	WRITE_LINE("system.name = \"Two-Body Scattering\";")
///////////////////////////////////////////////////////////////////////////////
/*
1.	motion parallel to x-axis
2.	y1 = 0
3.	m1 v1 = m2 v2
*/
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("func TimeOfCollision(x1, x2, y2, m1, m2, R1, R2, v2) {"
//	WRITE_LINE("	return ((m1*(-x1 + x2 + sqrt( (R1 + R2)^2 -  y2^2 )))/((m1 + m2)*v2));")
//	WRITE_LINE("};")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function TimeOfCollision(x1$, x2$, y2$, m1$, m2$, R1$, R2$, v2$) {"
"val1 = sqrt( (R1 + R2)^2 -  y2^2 ) ;"
"val2 = x2 - x1 ;"
"if (val2 >= val1) {"
"	return ((m1*(val2 - val1))/((m1 + m2)*v2));"
"};"
"	return ((m1*(val2 + val1))/((m1 + m2)*v2));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("TimeOfCollision(s1.position.x, s2.position.x, s2.position.y, s1.mass, s2.mass, s1.radius, s2.radius, s2.speed);")
///////////////////////////////////////////////////////////////////////////////

	WRITE_LINE("s1 = sphere;")
	WRITE_LINE("s2 = sphere;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1"Gold)
//NeonRed/NeonPurple/PANTONERhodamineRedCV//ElectricBlue/PANTONE2718CV/PANTONE2727CV/PANTONE2728CV/
	WRITE_LINE("s2"PANTONE2728CV)
//	WRITE_LINE("s1.drawStyle = 0;")
//	WRITE_LINE("s2.drawStyle = 0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.radius = .35;")
	WRITE_LINE("s2.radius = .25;")
//	WRITE_LINE("s1.radius = .25;")
//	WRITE_LINE("s2.radius = s1.radius;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.mass = 3.5;")
	WRITE_LINE("s2.mass = 2.5;")
//	WRITE_LINE("s1.mass = 1;")
//	WRITE_LINE("s2.mass = 1;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("s1.velocity = [0.25, 0, 0];")
//	WRITE_LINE("s2.velocity = [-0.35, 0, 0];")
	WRITE_LINE("s1.velocity.x =  1.0/s1.mass;")
	WRITE_LINE("s2.velocity.x = -1.0/s2.mass;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize = 0.005;")
//	WRITE_LINE("s1.position = [    -s1.velocity.x*2,  0.25,  0 ];")
//	WRITE_LINE("s2.position = [    -s2.velocity.x*2, -0.275, 0 ];")
	WRITE_LINE("s1.position.x =  -s1.speed*system.timeStepSize*3.1 - s1.radius;")
	WRITE_LINE("s2.position.x =   s2.speed*system.timeStepSize*3.1 + s2.radius;")
	WRITE_LINE("s2.position.y =   0.68*(s1.radius + s2.radius);")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s1.coefficientOfKineticFriction = 0.4;")
	WRITE_LINE("s1.coefficientOfStaticFriction = 0.6;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("s2.coefficientOfKineticFriction = 0.3;")
	WRITE_LINE("s2.coefficientOfStaticFriction = 0.5;")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.gravitationalCoupling = 0;")
/*
	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.collisionIterations = 256;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.collisionIterations = 256;\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.collisionTolerance = 0.000001;\";")
	WRITE_LINE("system.collisionTolerance = 0.000001;")
*/
	WRITE_LINE("system.enforceRigidBoundary =1;")//on
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.deferredCommand = \"18.2; m3.coefficientOfDamping = [ 0.38, 0.38, 0 ];\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("TimeOfCollision(s1.position.x, s2.position.x, s2.position.y, s1.mass, s2.mass, s1.radius, s2.radius, s2.speed);")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("system.collisionTimes;")
///////////////////////////////////////////////////////////////////////////////
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnBallOnShpere()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_BALLONSHPERE)//theApp.Initialize();
	WRITE_LINE("system.name = \"Ball On Sphere\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.background.show = true;")
	WRITE_LINE("system.internalForce = false;")	//off;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("_ = sphere;")//
	WRITE_LINE("_.inverseMass = 0;")//
	WRITE_LINE("_.radius = 1;")//
	WRITE_LINE("_.color = [ 1, 0.7, 0.7 ];")//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m = sphere;")
	WRITE_LINE("m.radius = 1 / PI ;")
	WRITE_LINE("m.mass = 2*(1 +  1 / PI) ;")
	WRITE_LINE("m.position = [ 0, _.radius+m.radius, 0 ];")
	WRITE_LINE("m.color = [ 0.4, 1, 0.7 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m.coefficientOfKineticFriction = 0.2;")
	WRITE_LINE("m.coefficientOfStaticFriction = 0.75;")
	WRITE_LINE("m.velocity  = [-0.1,  0, 0 ];")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.collisionDetection = false;")
	WRITE_LINE("system.gravitationalCoupling = 1;")
	WRITE_LINE("system.externalGravitationalField.y =  -3;")
	WRITE_LINE("system.timeStepSize = 0.02;")
	WRITE_LINE("system.stop.time = 4;")
	WRITE_LINE("system.boundingBox = [ -2, 2, -2, 2, 2.05, 40 ] ;")
	WRITE_LINE("system.translate.z =  -3.7;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m.constraint.quadratic == [1, 1, 1],[ 0, 0, 0], -pow(_.radius+m.radius,2), true;")
//	WRITE_LINE("_.drawStyle=1;")//
	WRITE_LINE("m.normal.show = true;")
	WRITE_LINE("m.contactForce.show = true;")
	WRITE_LINE("m.constraint.show = false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = \"2.6; system.timeStepSize= 0.005;\";")
	WRITE_LINE("system.deferredCommand.onReset = \"0.0; system.timeStepSize= 0.02;\";")
//	WRITE_LINE("system.deferredCommand = \"2.7; m.drawStyle=1;\";")
//	WRITE_LINE("system.deferredCommand.onReset = \"0.0; m.drawStyle=0;\";")
///////////////////////////////////////////////////////////////////////////////
				Fire_ViewChange();//
				CGLView* pGLView = theApp.GetGLView();
	pGLView->SetFocus();//calls OnReset(WPARAM wParam, LPARAM lParam)
	pGLView->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
			CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
				Fire_ViewChange();//
				pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
				Fire_ViewChange();//
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnTwoBlocksAndPulley()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_PULLEYANDTWOBLOCKS)//theApp.Initialize();
	WRITE_LINE("system.name = \"Two Blocks and Pulley\";")

	WRITE_LINE("system.rotate = [8, -12, 0];")

	WRITE_LINE("Pulley = cylinder;")
	WRITE_LINE("One = block;")
	WRITE_LINE("Two = block;")

	WRITE_LINE("Pulley.color = [ 0.5, 0, 1 ];")
	WRITE_LINE("One.color = [ 0.4, 0.8, 0.6 ];")
	WRITE_LINE("Two.color = [ 1, 0.6, 0.8 ];")

	WRITE_LINE("Pulley.radius = 0.2;")
	WRITE_LINE("Pulley.depth =  1.4;")
/*
	WRITE_LINE("Pulley.normals = true;")
*/
	WRITE_LINE("One.width =  .2;")
	WRITE_LINE("One.height = .4;")
	WRITE_LINE("One.depth =  .2;")

	WRITE_LINE("Two.width =  .4;")
	WRITE_LINE("Two.height = .4;")
	WRITE_LINE("Two.depth =  .6;")

	WRITE_LINE("One.mass = 2;")
	WRITE_LINE("Two.mass = 2.2;")
	WRITE_LINE("Pulley.mass = 50;")

	WRITE_LINE("Pulley.position = [ 0, 0, 0 ];")
	WRITE_LINE("Pulley.constraint.[0, 0, 0] == [0, 1, 0], 1;")

	WRITE_LINE("One.position = [  -0.2,  -1.5, 0 ];")
	WRITE_LINE("Two.position = [   0.2,  -.5, 0 ];")

//	WRITE_LINE("system.pulley Pulley.left == One.quadrant 2 Pulley.right == Two.quadrant 1;")
	WRITE_LINE("Pulley.pulley.left = One.quadrant.two;")
	WRITE_LINE("Pulley.pulley.right = Two.quadrant.one;")

//	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.externalGravitationalField.y =  -1;")
	WRITE_LINE("system.timeStepSize = 0.075;")
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnBlockAndTackle() {
	Create_GenExampThread(ID_POPUP_MECHANICS_BLOCKANDTACKLE)//theApp.Initialize();
	
}
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnBlockAndPulley()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_BLOCK_AND_PULLEY)//theApp.Initialize();
	WRITE_LINE("system.name = \"Block and Pulley\";")

	WRITE_LINE("system.rotate = [8, -12, 0];")

	WRITE_LINE("Pulley = cylinder;")
	WRITE_LINE("Two = block;")
//	WRITE_LINE("Pulley.inverseMass = 0;")
	WRITE_LINE("Pulley.drawStyle = 0;")
	WRITE_LINE("Two.drawStyle = 0;")

	WRITE_LINE("Pulley.color = [ 0.5, 0, 1 ];")
	WRITE_LINE("Two.color = [ 1, 0.6, 0.8 ];")

	WRITE_LINE("Pulley.position = [ 0, 0, 0 ];")
	WRITE_LINE("Two.position = [   1.0,  0.1, 0 ];")
//	WRITE_LINE("Two.connect.pulley == 0.2 1;")

	WRITE_LINE("Pulley.radius = 0.2;")
	WRITE_LINE("Pulley.depth =  1.4;")
	WRITE_LINE("Pulley.mass = 2;")

	WRITE_LINE("Two.width =  .4;")
	WRITE_LINE("Two.height = .4;")
	WRITE_LINE("Two.depth =  .6;")
	
	WRITE_LINE("Pulley.pulley.right = Two.quadrant.one;")

//	WRITE_LINE("Pulley.{[0.0, 0.0, 0.0]} == { [0.0, 0.0, 0.0] } 0.0;")
	WRITE_LINE("Pulley.constraint.[0, 0, 0] == [0, 1, 0], 1;")
//	WRITE_LINE("Pulley.normals = true;")

	WRITE_LINE("system.collisionDetection = true;")
	WRITE_LINE("system.externalGravitationalField.y =  -1;")
	WRITE_LINE("system.timeStepSize = 0.025;")
	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::OnInelasticCollision()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_INELASTICCOLLISION)//theApp.Initialize();
	WRITE_LINE("system.name = \"Inelastic Collision\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function R(x1$, y1$, x2$, y2$) {"
"	return sqrt ((x1 - x2)^2 + (y1 - y2)^2);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function V(x$, y$, z$) {"
"	if (! sticking) {return 0.0};"
"	return V0 + 0.5*(-k1/(a^2 + (r-b)*(r-b)) + k2*(r-b)*(r-b));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Colliding(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	r = R(x1, y1, x2, y2);"
"	fSave = -(r-b)*( k1 * ( a^2. + (r-b)*(r-b) )^(-2.0) + k2 )/r;"
"	if ( r <= b && sticking == false) {"
"		sticking = true;"
"		V0 = -V(0, 0, 0);"
"		print \"V0 = \", -V0  ;"
"	};"
"	return 0;"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1x(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	Colliding(x1, y1, z1, x2, y2, z2);"
"	if (! sticking) {return 0.0};"
"	f1xSave =  (x1 - x2)*fSave;"
"	return f1xSave;"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2x() {"
"	return (-1.0*f1xSave);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1y(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	if (! sticking) {return 0.0};"
"	f1ySave =  (y1 - y2)*fSave;"
"	return f1ySave;"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2y() {"
"	return (-1.0*f1ySave);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m1.radius =  .5;")
	WRITE_LINE("m2.radius =  .5;")
//	WRITE_LINE("m1.radius =  .05;")
//	WRITE_LINE("m2.radius =  .05;")
	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 10;")
	WRITE_LINE("m1.color = [ 0.749, 0.635, 0.384 ];")
	WRITE_LINE("m2.color = [ 0.384, 0.749, 0.635 ];")

	WRITE_LINE("m1.position = [ 1.125,  0, 0 ];")
	WRITE_LINE("m2.position = [ 0,  0, 0 ];")
	WRITE_LINE("m1.position.x = 1.125*(m1.radius+m2.radius);")

	WRITE_LINE("m1.velocity = [-5.0,  0.0, 0 ];")
	WRITE_LINE("m2.velocity = [ 0.0,  0.0, 0 ];")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.mue = 0.01;")
	WRITE_LINE("m2.mue = 0.01;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.internalForce.x = f1x;")
	WRITE_LINE("m2.externalForce.x = f2x 0, false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.internalForce.y = f1y;")
	WRITE_LINE("m2.externalForce.y = f2y 0, false;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.potential = V false;")
//	WRITE_LINE("m2.potential = V false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("b = m1.radius+m2.radius;")
	WRITE_LINE("sticking = false;")
	WRITE_LINE("V0  = 0.0;")
	WRITE_LINE("a  = 0.10;")
	WRITE_LINE("k1 = 16.0;")
	WRITE_LINE("k2 = 24.0;")
	WRITE_LINE("r = R(m1.position.x, 0, 0, 0);")
	WRITE_LINE("fSave = 0.0;")
	WRITE_LINE("f1xSave = 0.0;")
	WRITE_LINE("f1ySave = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = 0.05, \"system.timeStepSize= 0.05\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"system.timeStepSize= b*0.0025\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"sticking = false\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"V0  = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"r = 1.125*(m1.radius+m2.radius)\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"r\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"r = R(m1.position.x, 0, 0, 0)\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"r\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"fSave = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"f1xSave = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"f1ySave = 0.0\";")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE(" m2.{[0, 0, 0]} = {[0, 5, 0]} 5;")
//	WRITE_LINE("system.externalGravitationalField.y =  -0.6;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= b*0.0025;")
//	WRITE_LINE("print \"   system.timeStepSize = \", system.timeStepSize;")
	WRITE_LINE("system.timeStepSize;")
	WRITE_LINE("system.boundingBox = [ -5, 5, -5, 5, 10, 20 ] ;")
	WRITE_LINE("system.translate.z = -5.5;")
	WRITE_LINE("system.collisionDetection = true;")
///////////////////////////////////////////////////////////////////////////////
	
}
void CSolveItView::OnInelasticPendulumCollision()
{
	//Fire_ClearUserDefs();
	Create_GenExampThread(ID_POPUP_MECHANICS_INELASTICPENDULUMCOLLISION)//theApp.Initialize();
	WRITE_LINE("system.name = \"Inelastic Collision with Pendulum\";")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function R(x1$, y1$, x2$, y2$) {"
"	return sqrt ((x1 - x2)^2 + (y1 - y2)^2);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function V(x$, y$, z$) {"
//	WRITE_LINE("	if (not sticking) {return 0.0};")
"	if (! sticking) {return 0.0};"
"	return V0 + 0.5*(-k1/(a^2 + (r-b)*(r-b)) + k2*(r-b)*(r-b));"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function Colliding(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	r = R(x1, y1, x2, y2);"
"	fSave = -(r-b)*( k1 * ( a^2. + (r-b)*(r-b) )^(-2.0) + k2 )/r;"
"	if ( r <= b && sticking == false) {"
"		sticking = true;"
"		V0 = -V(0, 0, 0);"
"		print \"V0 = \", -V0  ;"
"	};"
"	return 0;"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1x(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	Colliding(x1, y1, z1, x2, y2, z2);"
"	if (! sticking) {return 0.0};"
"	f1xSave =  (x1 - x2)*fSave;"
"	return f1xSave;"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2x() {"
"	return (-1.0*f1xSave);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f1y(x1$, y1$, z1$, x2$, y2$, z2$) {"
"	if (! sticking) {return 0.0};"
"	f1ySave =  (y1 - y2)*fSave;"
"	return f1ySave;"
"}")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("function f2y() {"
"	return (-1.0*f1ySave);"
"}")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1 = sphere;")
	WRITE_LINE("m2 = sphere;")
	WRITE_LINE("m1.radius =  .5;")
	WRITE_LINE("m2.radius =  .5;")
//	WRITE_LINE("m1.radius =  .05;")
//	WRITE_LINE("m2.radius =  .05;")
	WRITE_LINE("m1.mass = 1;")
	WRITE_LINE("m2.mass = 10;")
	WRITE_LINE("m1.color = [ 0.749, 0.635, 0.384 ];")
	WRITE_LINE("m2.color = [ 0.384, 0.749, 0.635 ];")

	WRITE_LINE("m1.position = [ 1.125,  0, 0 ];")
	WRITE_LINE("m2.position = [ 0,  0, 0 ];")
	WRITE_LINE("m1.position.x = 1.125*(m1.radius+m2.radius);")

	WRITE_LINE("m1.velocity = [-5.0,  0.0, 0 ];")
	WRITE_LINE("m2.velocity = [ 0.0,  0.0, 0 ];")

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.mue = 0.1;")
	WRITE_LINE("m2.mue = 0.1;")

///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.internalForce.x = f1x;")
	WRITE_LINE("m2.externalForce.x = f2x 0, false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("m1.internalForce.y = f1y;")
	WRITE_LINE("m2.externalForce.y = f2y 0, false;")
///////////////////////////////////////////////////////////////////////////////
//	WRITE_LINE("m1.potential = V false;")
//	WRITE_LINE("m2.potential = V false;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("b = m1.radius+m2.radius;")
	WRITE_LINE("sticking = false;")
	WRITE_LINE("V0  = 0.0;")
	WRITE_LINE("a  = 0.10;")
	WRITE_LINE("k1 = 16.0;")
	WRITE_LINE("k2 = 24.0;")
	WRITE_LINE("r = R(m1.position.x, 0, 0, 0);")
	WRITE_LINE("fSave = 0.0;")
	WRITE_LINE("f1xSave = 0.0;")
	WRITE_LINE("f1ySave = 0.0;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.deferredCommand = 0.05, \"system.timeStepSize= 0.05\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"system.timeStepSize= b*0.0025\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"sticking = false\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"V0  = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"r = 2.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"fSave = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"f1xSave = 0.0\";")
	WRITE_LINE("system.resetDeferredCommand = 0.0, \"f1ySave = 0.0\";")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE(" m2.{[0, 0, 0]} == {[0, 5, 0]} 5;")
	WRITE_LINE("system.externalGravitationalField.y =  -0.6;")
///////////////////////////////////////////////////////////////////////////////
	WRITE_LINE("system.timeStepSize= b*0.0025;")
	WRITE_LINE("system.boundingBox = [  -5, 5, -5, 5, 10, 20 ];")
	WRITE_LINE("system.translate.z = -5.5;")
	WRITE_LINE("system.collisionDetection = true;")
///////////////////////////////////////////////////////////////////////////////
	
}
///////////////////////////////////////////////////////////////////////////////

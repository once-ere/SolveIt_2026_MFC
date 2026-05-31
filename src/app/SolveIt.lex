/***************************************************************************

			Scanner for the SolveIt language
			
win_bison.exe -o parser_2020.cpp .\SolveIt.y

win_bison3_5.exe -o parser_2020.cpp .\SolveIt.y


win_flex --wincompat -o lexer_2020.cpp SolveIt.lex

win_flex3_5 --wincompat -o lexer_2020.cpp SolveIt.lex

Severity	Code	Description	Project	File	Line	Suppression State
Error	C1189	#error:  MFC requires C++ compilation (use a .cpp suffix) (compiling source file SolveIt.tab.c)	SolveIt	C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.24.28314\atlmfc\include\afx.h	15	

***************************************************************************/


%{
/*
!!!!!!!!!!!!!
delete
class istream;
just before
#include <unistd.h>
!!!!!!!!!!!!!!!!!!!!!!
line ~471 of parser.cpp:
*/
#define CONCAT(s1,s2) #s1 ## #s2
#define MSG(n) CONCAT( below line\x20,n)
#pragma message ("move next the line to BOF.")
#pragma message (MSG(__LINE__))

#include "stdafx.h"
//#include "lexer_2020.h"
//#include "parser_2020.hpp" /* for token definitions and yylval */
#include "parser_2020.h" /* for token definitions and *yylval */
/*=========================================================================
			 C-libraries and Token  definitions
=========================================================================*/
/*#include <stdlib.h> */	 /* for atoi				 */

///////////////////////////////////////////////////////////////////////////////
#include "SolveIt.h"
#include "MainFrm.h"
#include "System.h"
#include "SymbolTable.h"
#include "StackMachine.h"
#include "symbol.h"
#include "instruction.h"

#define	YYSTYPE objects
//#define	YYSTYPE wchar_t
extern YYSTYPE yylval;
//#define YY_DECL int yyFlexLexer::yylex(YYSTYPE *lvalp)
//
#define YY_SKIP_YYWRAP 1
int yywrap (void *);



#include <tchar.h>

#include <io.h>
#define isatty _isatty

int yywrap(void*){return 1;}

///////////////////////////////////////////////////////////////////////////////
unsigned int SolveIt::field_count = 0;
///////////////////////////////////////////////////////////////////////////////
int count(const char* not_yytext);
double numeric(char *s);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

%}
%option reentrant
%option bison-bridge
%option nounistd
/*%option header-file="lexer_2020a.hpp"*/
/*%option outfile="lexer_2020a.cpp"*/
%option header-file="lexer_2020.h"
%option outfile="lexer_2020.cpp"
/*%option outfile="lexer_2020.c"*/

/*=========================================================================
%option header-file="scanner.h"
%option outfile="_scan.cpp"
%option reentrant-bison
%option noline
%option c++
%option reentrant-bison
%option yyclass="MyLexer"
				 TOKEN Definitions 
=========================================================================*/
/*DIGIT	 [0-9]*/
/*ID	[a-z][a-z0-9]**/

/* Abbreviations */
D	[0-9]
E	[elEL][-+]?{D}+
A	[a-zA-Z_]
AN	[0-9a-zA-Z_]
ID	[a-z][a-z0-9]*

/*=========================================================================
language
x=sphere;
x.position.x;
x.radius;


initialPacketSpread	{ field_count+= count(yytext); *yylval = T_InitialPacketSpread;	return(T_InitialPacketSpread);	}
center_of_Gaussian	{ field_count+= count(yytext); *yylval = T_Center_of_Gaussian;	return(T_Center_of_Gaussian);	}
gaussian	{ field_count+= count(yytext); *yylval = T_Gaussian;	return(T_Gaussian);	}
center		{ field_count+= count(yytext); *yylval = T_Center;	return(T_Center);	}
spread		{ field_count+= count(yytext); *yylval = T_Spread;	return(T_Spread);	}

=========================================================================*/
%%
if			{ field_count+= count(yytext); *yylval = T_IF;return(T_IF);		}
while		{ field_count+= count(yytext); *yylval = T_WHILE;return(T_WHILE);	}
for			{ field_count+= count(yytext); *yylval = T_FOR;return(T_FOR);	}
then		{ field_count+= count(yytext); *yylval = T_THEN;return(T_THEN);	}
else		{ field_count+= count(yytext); *yylval = T_ELSE;return(T_ELSE);	}

return		{ field_count+= count(yytext); *yylval = T_RETURN;return(T_RETURN);	}
function	{ field_count+= count(yytext); *yylval = T_DEFINE_USER_FUNCTION;return(T_DEFINE_USER_FUNCTION);}
local		{ 
				field_count+= count(yytext); 
				CSymbolTable* symbolTable	= theApp.GetSymbolTable();
				CStackMachine* stackMachine	= theApp.GetStackMachine();
				ASSERT (stackMachine->indef);
				symbolTable->m_b_local = true;
				*yylval = T_LOCAL;
				return(T_LOCAL);
			}


read		{ field_count+= count(yytext); *yylval = T_READ;return(T_READ);	}
write		{ field_count+= count(yytext); *yylval = T_WRITE;return(T_WRITE);	}

plot		{ field_count+= count(yytext); *yylval = T_PLOT;return(T_PLOT);	}

name			{ field_count+= count(yytext); *yylval = T_NAME;return(T_NAME);	}
sphere			{ field_count+= count(yytext); *yylval = T_SPHERE;return(T_SPHERE);	}
block			{ field_count+= count(yytext); *yylval = T_BLOCK;return(T_BLOCK);	}
cylinder		{ field_count+= count(yytext); *yylval = T_CYLINDER;return(T_CYLINDER);	}
cone			{ field_count+= count(yytext); *yylval = T_CONE;return(T_CONE);	}

vectorAnalysisObject	{ field_count+= count(yytext); *yylval = T_VECTOR_ANALYSIS_OBJECT;return(T_VECTOR_ANALYSIS_OBJECT);	}

radius		{ field_count+= count(yytext); *yylval = T_RADIUS;return(T_RADIUS);	}
width		{ field_count+= count(yytext); *yylval = T_WIDTH;return(T_WIDTH);	}
height		{ field_count+= count(yytext); *yylval = T_HEIGHT ;return(T_HEIGHT );	}
depth		{ field_count+= count(yytext); *yylval = T_DEPTH;return(T_DEPTH);	}
color		{ field_count+= count(yytext); *yylval = T_COLOR;return(T_COLOR);	}
drawStyle	{ field_count+= count(yytext); *yylval = T_DRAWSTYLE;return(T_DRAWSTYLE);	}
scale		{ field_count+= count(yytext); *yylval = T_SCALE;return(T_SCALE);	}
delete		{ field_count+= count(yytext); *yylval = T_DELETE;return(T_DELETE);	}

mass		{ field_count+= count(yytext); *yylval = T_MASS;return(T_MASS);	}
charge		{ field_count+= count(yytext); *yylval = T_CHARGE;return(T_CHARGE);	}
inverseMass	{ field_count+= count(yytext); *yylval = T_IMASS;return(T_IMASS);	}
density		{ field_count+= count(yytext); *yylval = T_DENSITY;return(T_DENSITY);	}

magneticDipoleMoment		{ field_count+= count(yytext); *yylval = T_MagneticDipoleMoment;return(T_MagneticDipoleMoment);	}
magneticDipoleOrientation	{ field_count+= count(yytext); *yylval = T_MagneticDipoleOrientation;return(T_MagneticDipoleOrientation);	}

position			{ field_count+= count(yytext); *yylval = T_POSITION;return(T_POSITION);	}
momentum			{ field_count+= count(yytext); *yylval = T_MOMENTUM;return(T_MOMENTUM);	}
angular_momentum	{ field_count+= count(yytext); *yylval = T_ANGULARMOMENTUM;return(T_ANGULARMOMENTUM);	}
orientation			{ field_count+= count(yytext); *yylval = T_ORIENTATION;return(T_ORIENTATION);	}
velocity			{ field_count+= count(yytext); *yylval = T_VELOCITY;return(T_VELOCITY);	}
acceleration		{ field_count+= count(yytext); *yylval = T_ACCELERATION;return(T_ACCELERATION);	}
angular_velocity	{ field_count+= count(yytext); *yylval = T_ANGULARVELOCITY;return(T_ANGULARVELOCITY);	}
force				{ field_count+= count(yytext); *yylval = T_FORCE;return(T_FORCE);	}
torque				{ field_count+= count(yytext); *yylval = T_TORQUE;return(T_TORQUE);	}
contactForce		{ field_count+= count(yytext); *yylval = T_contactFORCE;return(T_contactFORCE);	}
contactTorque		{ field_count+= count(yytext); *yylval = T_contactTORQUE;return(T_contactTORQUE);	}
externalForce		{ field_count+= count(yytext); *yylval = T_BODY_FORCE_EXTERNAL;return(T_BODY_FORCE_EXTERNAL);	}
internalForce		{ field_count+= count(yytext); *yylval = T_BODY_FORCE_INTERNAL;return(T_BODY_FORCE_INTERNAL);	}
energy				{ field_count+= count(yytext); *yylval = T_ENERGY ;return(T_ENERGY );	}
kineticEnergy		{ field_count+= count(yytext); *yylval = T_KINETIC_ENERGY;return(T_KINETIC_ENERGY);	}
potentialEnergy		{ field_count+= count(yytext); *yylval = T_POTENTIAL_ENERGY;return(T_POTENTIAL_ENERGY);	}
potential			{ field_count+= count(yytext); *yylval = T_POTENTIAL;return(T_POTENTIAL);	}

coefficientOfStaticFriction		{ field_count+= count(yytext); *yylval = T_COEFFICIENT_OF_STATIC_FRICTION;return(T_COEFFICIENT_OF_STATIC_FRICTION);	}
coefficientOfKineticFriction	{ field_count+= count(yytext); *yylval = T_COEFFICIENT_OF_KINETIC_FRICTION;return(T_COEFFICIENT_OF_KINETIC_FRICTION);	}
coefficientOfRestitution		{ field_count+= count(yytext); *yylval = T_COEFFICIENT_OF_RESTITUTION;return(T_COEFFICIENT_OF_RESTITUTION);	}
coefficientOfDamping			{ field_count+= count(yytext); *yylval = T_DAMPING_COEFFICIENT;return(T_DAMPING_COEFFICIENT);	}

contacts	{ field_count+= count(yytext); *yylval = T_CONTACTS;return(T_CONTACTS);	}
speed		{ field_count+= count(yytext); *yylval = T_SPEED;return(T_SPEED);	}
distance	{ field_count+= count(yytext); *yylval = T_DISTANCE;return(T_DISTANCE);	}

constraintsActive	{ field_count+= count(yytext); *yylval = T_CONSTRAINT_STATE;return(T_CONSTRAINT_STATE);	}

constraint	{ field_count+= count(yytext); *yylval = T_Constraint;return(T_Constraint);	}

linear		{ field_count+= count(yytext); *yylval = T_Linear;return(T_Linear);	}
quadratic	{ field_count+= count(yytext); *yylval = T_Quadratic;return(T_Quadratic);	}

connect		{ field_count+= count(yytext); *yylval = T_CONNECT;return(T_CONNECT);	}
pulley		{ field_count+= count(yytext); *yylval = T_PULLEY;return(T_PULLEY);	}
quadrant	{ field_count+= count(yytext); *yylval = T_quadrant;return(T_quadrant);	}

one		{ field_count+= count(yytext); *yylval = T_ONE;return(T_ONE);	}
two		{ field_count+= count(yytext); *yylval = T_TWO;return(T_TWO);	}
three	{ field_count+= count(yytext); *yylval = T_THREE;return(T_THREE);	}
four	{ field_count+= count(yytext); *yylval = T_FOUR;return(T_FOUR);	}

plane		{ field_count+= count(yytext); *yylval = T_PLANE;return(T_PLANE);	}
angle		{ field_count+= count(yytext); *yylval = T_ANGLE;return(T_ANGLE);	}
thruPoint	{ field_count+= count(yytext); *yylval = T_POINT_IN_PLANE;return(T_POINT_IN_PLANE);	}
normal		{ field_count+= count(yytext); *yylval = T_NORMAL;return(T_NORMAL);	}

period		{ field_count+= count(yytext); *yylval = T_PERIOD;return(T_PERIOD);	}
trajectory	{ field_count+= count(yytext); *yylval = T_TRAJECTORY;return(T_TRAJECTORY);	}

record		{ field_count+= count(yytext); *yylval = T_RECORD;return(T_RECORD);	}
analyze		{ field_count+= count(yytext); *yylval = T_ANALYZE;return(T_ANALYZE);	}

localPointToLab		{ field_count+= count(yytext); *yylval = T_LocalPointToLab;return(T_LocalPointToLab);	}
labPointToLocal		{ field_count+= count(yytext); *yylval = T_LabPointToLocal;return(T_LabPointToLocal);	}
localVectorToLab	{ field_count+= count(yytext); *yylval = T_LocalVectorToLab;return(T_LocalVectorToLab);	}
labVectorToLocal	{ field_count+= count(yytext); *yylval = T_LabVectorToLocal;return(T_LabVectorToLocal);	}


multiBody	{ field_count+= count(yytext); *yylval = T_SYSTEM_MultiBody;return(T_SYSTEM_MultiBody);	}
link		{ field_count+= count(yytext); *yylval = T_MultiBody_Link;return(T_MultiBody_Link);	}
joint		{ field_count+= count(yytext); *yylval = T_MultiBody_Link_Joint;return(T_MultiBody_Link_Joint);	}
displacement	{ field_count+= count(yytext); *yylval = T_DISPLACEMENT;return(T_DISPLACEMENT);	}

springConstant	{ field_count+= count(yytext); *yylval = T_MultiBody_SPRING_CONSTANT;return(T_MultiBody_SPRING_CONSTANT);	}
springLength	{ field_count+= count(yytext); *yylval = T_MultiBody_SPRING_LENGTH;return(T_MultiBody_SPRING_LENGTH);	}
control			{ field_count+= count(yytext); *yylval = T_MultiBody_CONTROL;return(T_MultiBody_CONTROL);	}
fixed		{ field_count+= count(yytext); *yylval = T_JointType_FIXED;		return(T_JointType_FIXED);	}
revolute	{ field_count+= count(yytext); *yylval = T_JointType_REVOLUTE;	return(T_JointType_REVOLUTE);	}
prismatic	{ field_count+= count(yytext); *yylval = T_JointType_PRISMATIC;	return(T_JointType_PRISMATIC);	}
screw		{ field_count+= count(yytext); *yylval = T_JointType_SCREW;		return(T_JointType_SCREW);	}
cylindrical	{ field_count+= count(yytext); *yylval = T_JointType_CYLINDRICAL;return(T_JointType_CYLINDRICAL);	}
universal	{ field_count+= count(yytext); *yylval = T_JointType_UNIVERSAL;	return(T_JointType_UNIVERSAL);	}
spherical	{ field_count+= count(yytext); *yylval = T_JointType_SPHERICAL;	return(T_JointType_SPHERICAL);	}
floating	{ field_count+= count(yytext); *yylval = T_JointType_FLOATING;	return(T_JointType_FLOATING);	}
spring		{ field_count+= count(yytext); *yylval = T_JointType_SPRING;	return(T_JointType_SPRING);	}

system	{
			field_count+= count(yytext);
			CSymbolTable* symbolTable	= theApp.GetSymbolTable();
			symbol* s = symbolTable->getsym("system");
			*yylval = objects(s);
			return(T_SYSTEM);
		}

reset	{ CSolveItApp::g_boolDoReset=true;field_count+= count(yytext); *yylval = T_RESET;	return(T_RESET);	}

begin	{ field_count+= count(yytext); *yylval = T_BEGIN;return(T_BEGIN);	}
end		{ field_count+= count(yytext); *yylval = T_END;return(T_END);	}

true		{ field_count+= count(yytext); *yylval = T_TRUE;return(T_TRUE);	}
false		{ field_count+= count(yytext); *yylval = T_FALSE;return(T_FALSE);	}

off		{ field_count+= count(yytext); *yylval = T_OFF;return(T_OFF);	}
on		{ field_count+= count(yytext); *yylval = T__ON;return(T__ON);	}

dimension	{ field_count+= count(yytext); *yylval = T_DIMENSION;return(T_DIMENSION);	}

title		{ field_count+= count(yytext); *yylval = T_TITLE;return(T_TITLE);	}
tic_mark	{ field_count+= count(yytext); *yylval = T_TICMARK;return(T_TICMARK);	}
nodes		{ field_count+= count(yytext); *yylval = T_NODES;return(T_NODES);	}

show		{ field_count+= count(yytext); *yylval = T_SHOW;return(T_SHOW);	}
scene		{ field_count+= count(yytext); *yylval = T_SCENE;return(T_SCENE);	}
map			{ field_count+= count(yytext); *yylval = T_MAP;return(T_MAP);	}

file	{ field_count+= count(yytext); *yylval = T_FILE;return(T_FILE);	}
data	{ field_count+= count(yytext); *yylval = T_DATA;return(T_DATA);	}

background		{ field_count+= count(yytext); *yylval = T_SYSTEM_Background;return(T_SYSTEM_Background);	}
vectorFields	{ field_count+= count(yytext); *yylval = T_SYSTEM_VectorFields;return(T_SYSTEM_VectorFields);	}
fieldLines		{ field_count+= count(yytext); *yylval = T_SYSTEM_FieldLines;return(T_SYSTEM_FieldLines);	}
equipotentials	{ field_count+= count(yytext); *yylval = T_SYSTEM_Equipotentials;return(T_SYSTEM_Equipotentials);	}


state		{ field_count+= count(yytext); *yylval = T_STATE;		return(T_STATE);	}
stationary	{ field_count+= count(yytext); *yylval = T_Stationary;	return(T_Stationary);	}
scattering	{ field_count+= count(yytext); *yylval = T_Scattering;	return(T_Scattering);	}
waveNumber	{ field_count+= count(yytext); *yylval = T_WaveNumber;	return(T_WaveNumber);	}
eigenNumber	{ field_count+= count(yytext); *yylval = T_EigenNumber;	return(T_EigenNumber);	}
method		{ field_count+= count(yytext); *yylval = T_Method;		return(T_Method);	}
renormalize	{ field_count+= count(yytext); *yylval = T_Renormalize;		return(T_Renormalize);	}
fourierTransform	{ field_count+= count(yytext); *yylval = T_FourierTransform;	return(T_FourierTransform);	}
superposition	{ field_count+= count(yytext); *yylval = T_Superposition;	return(T_Superposition);	}


incident	{ field_count+= count(yytext); *yylval = T_Incident;	return(T_Incident);	}

number		{ field_count+= count(yytext); *yylval = T_Number;	return(T_Number);	}
frames		{ field_count+= count(yytext); *yylval = T_Frames;	return(T_Frames);	}

nashChen	{ field_count+= count(yytext); *yylval = T_Nash_Chen;	return(T_Nash_Chen);	}
periodicCayley	{ field_count+= count(yytext); *yylval = T_PeriodicCayley;	return(T_PeriodicCayley);	}
cayley	{ field_count+= count(yytext); *yylval = T_Cayley;	return(T_Cayley);	}

left	{ field_count+= count(yytext); *yylval = T_SYSTEM_LEFT;return(T_SYSTEM_LEFT);	}
right	{ field_count+= count(yytext); *yylval = T_SYSTEM_RIGHT;return(T_SYSTEM_RIGHT);	}
top		{ field_count+= count(yytext); *yylval = T_SYSTEM_TOP;return(T_SYSTEM_TOP);	}
bottom	{ field_count+= count(yytext); *yylval = T_SYSTEM_BOTTOM;return(T_SYSTEM_BOTTOM);	}
near	{ field_count+= count(yytext); *yylval = T_SYSTEM_NEAR;return(T_SYSTEM_NEAR);	}
far		{ field_count+= count(yytext); *yylval = T_SYSTEM_FAR;return(T_SYSTEM_FAR);	}

translate	{ field_count+= count(yytext); *yylval = T_TRANSLATE;return(T_TRANSLATE);	}
rotate		{ field_count+= count(yytext); *yylval = T_ROTATE;return(T_ROTATE);	}

run		{ field_count+= count(yytext); *yylval = T_RUN;return(T_RUN);	}
stop	{ field_count+= count(yytext); *yylval = T_STOP;return(T_STOP);	}
clear	{ field_count+= count(yytext); *yylval = T_CLEAR;return(T_CLEAR);	}
time			{ field_count+= count(yytext); *yylval = T_SYSTEM_TIME;return(T_SYSTEM_TIME);	}
timeStepSize	{ field_count+= count(yytext); *yylval = T_SYSTEM_TIME_STEP;return(T_SYSTEM_TIME_STEP);	}
timer			{ field_count+= count(yytext); *yylval = T_SYSTEM_TIMER_INTERVAL;return(T_SYSTEM_TIMER_INTERVAL);	}

externalMagneticField		{ field_count+= count(yytext); *yylval = T_MAGNETIC_FIELD;return(T_MAGNETIC_FIELD);	}
externalElectricField		{ field_count+= count(yytext); *yylval = T_ELECTRIC_FIELD;return(T_ELECTRIC_FIELD);	}
externalGravitationalField	{ field_count+= count(yytext); *yylval = T_GRAVITATIONAL_FIELD;return(T_GRAVITATIONAL_FIELD);	}
gravitationalCoupling		{ field_count+= count(yytext); *yylval = T_GRAVITATIONAL_COUPLING;return(T_GRAVITATIONAL_COUPLING);	}
centralForce				{ field_count+= count(yytext); *yylval = T_CENTRAL_FORCE;return(T_CENTRAL_FORCE);	}
potentialLennardJones		{ field_count+= count(yytext); *yylval = T_LENNARD_JONES;return(T_LENNARD_JONES);	}
currentLoopMagneticField	{ field_count+= count(yytext); *yylval = T_CURRENTLOOP_MAGNETICFIELD_STRENGTH;return(T_CURRENTLOOP_MAGNETICFIELD_STRENGTH);	}

boundingBox			{ field_count+= count(yytext); *yylval = T_BOUNDING_BOX;return(T_BOUNDING_BOX);	}
collisionDetection	{ field_count+= count(yytext); *yylval = T_COLLISIONDETECTION;return(T_COLLISIONDETECTION);	}
enforceRigidBoundary	{ field_count+= count(yytext); *yylval = T_ENFORCE_RIGID_BOUNDARY;return(T_ENFORCE_RIGID_BOUNDARY);	}

onReset				{ field_count+= count(yytext); *yylval = T_ONRESET;	return(T_ONRESET);	}
deferredCommand		{ field_count+= count(yytext); *yylval = T_DEFERRED_COMMAND;return(T_DEFERRED_COMMAND);	}

domain		{ field_count+= count(yytext); *yylval = T_DOMAIN;		return(T_DOMAIN);	}
infinity	{ field_count+= count(yytext); *yylval = T_INFINITY;	return(T_INFINITY);	}

radial		{ field_count+= count(yytext); *yylval = T_RADIAL;		return(T_RADIAL);	}
Cartesian	{ field_count+= count(yytext); *yylval = T_Cartesian;		return(T_Cartesian);	}
pseudoCartesian	{ field_count+= count(yytext); *yylval = T_pseudoCartesian;		return(T_pseudoCartesian);	}

rand		{ field_count+= count(yytext); *yylval = objects((F_0) imsl_rand);	return(T_BLTIN_VOID); }
Time		{ field_count+= count(yytext); *yylval = objects((F_0) Time);	return(T_BLTIN_VOID); }

seed_rand	{ field_count+= count(yytext); *yylval = objects((F_L) imsl_seed_rand);	return(T_BLTIN_L); }

acos		{ field_count+= count(yytext); *yylval = objects((F_D) acos);	return(T_BLTIN_D); }
asin		{ field_count+= count(yytext); *yylval = objects((F_D) asin);	return(T_BLTIN_D); }
atan		{ field_count+= count(yytext); *yylval = objects((F_D) atan);	return(T_BLTIN_D); }
cos			{ field_count+= count(yytext); *yylval = objects((F_D) cos);	return(T_BLTIN_D); }
cosh		{ field_count+= count(yytext); *yylval = objects((F_D) cosh);	return(T_BLTIN_D); }
exp			{ field_count+= count(yytext); *yylval = objects((F_D) exp);	return(T_BLTIN_D); }
abs			{ field_count+= count(yytext); *yylval = objects((F_D) fabs);	return(T_BLTIN_D); }
log			{ field_count+= count(yytext); *yylval = objects((F_D) log);	return(T_BLTIN_D); }
log10		{ field_count+= count(yytext); *yylval = objects((F_D) log10);	return(T_BLTIN_D); }
sin			{ field_count+= count(yytext); *yylval = objects((F_D) sin);	return(T_BLTIN_D); }
sinh		{ field_count+= count(yytext); *yylval = objects((F_D) sinh);	return(T_BLTIN_D); }
tan			{ field_count+= count(yytext); *yylval = objects((F_D) tan);	return(T_BLTIN_D); }
tanh		{ field_count+= count(yytext); *yylval = objects((F_D) tanh);	return(T_BLTIN_D); }
sqrt		{ field_count+= count(yytext); *yylval = objects((F_D) sqrt);	return(T_BLTIN_D); }
ceil		{ field_count+= count(yytext); *yylval = objects((F_D) ceil);	return(T_BLTIN_D); }
floor		{ field_count+= count(yytext); *yylval = objects((F_D) floor);	return(T_BLTIN_D); }
j0			{ field_count+= count(yytext); *yylval = objects((F_D) _j0);	return(T_BLTIN_D); }
j1			{ field_count+= count(yytext); *yylval = objects((F_D) _j1);	return(T_BLTIN_D); }
y0			{ field_count+= count(yytext); *yylval = objects((F_D) _y0);	return(T_BLTIN_D); }
y1			{ field_count+= count(yytext); *yylval = objects((F_D) _y1);	return(T_BLTIN_D); }

sign		{ field_count+= count(yytext); *yylval = objects((F_D) sign);	return(T_BLTIN_D); }
unitStep	{ field_count+= count(yytext); *yylval = objects((F_D) unitStep);	return(T_BLTIN_D); }
spline		{ field_count+= count(yytext); *yylval = objects((F_D) spline);	return(T_BLTIN_D); }
EllipticK	{ field_count+= count(yytext); *yylval = objects((F_D) CompleteEllipticIntegralFirstKind);		return(T_BLTIN_D); }
EllipticEc	{ field_count+= count(yytext); *yylval = objects((F_D) CompleteEllipticIntegralSecondKind);		return(T_BLTIN_D); }

EllipticF	{ field_count+= count(yytext); *yylval = objects((F_DD) IncompleteEllipticIntegralFirstKind); return(T_BLTIN_DD); }
EllipticE	{ field_count+= count(yytext); *yylval = objects((F_DD) IncompleteEllipticIntegralSecondKind);return(T_BLTIN_DD); }

atan2		{ field_count+= count(yytext); *yylval = objects((F_DD) atan2);	return(T_BLTIN_DD); }
fmod		{ field_count+= count(yytext); *yylval = objects((F_DD) fmod);	return(T_BLTIN_DD); }
pow			{ field_count+= count(yytext); *yylval = objects((F_DD) pow);	return(T_BLTIN_DD); }
hypot		{ field_count+= count(yytext); *yylval = objects((F_DD) _hypot);return(T_BLTIN_DD); }
fmax		{ field_count+= count(yytext); *yylval = objects((F_DD) fmax);	return(T_BLTIN_DD); }
fmin		{ field_count+= count(yytext); *yylval = objects((F_DD) fmin);	return(T_BLTIN_DD); }

gaussianNormalized	{ field_count+= count(yytext); *yylval = objects((F_DD) gaussianNormalized);	return(T_BLTIN_DD); }


jn			{ field_count+= count(yytext); *yylval = objects((F_LD) __jn);	return(T_BLTIN_LD); }
yn			{ field_count+= count(yytext); *yylval = objects((F_LD) __yn);	return(T_BLTIN_LD); }

ldexp		{ field_count+= count(yytext); *yylval = objects((F_DL) _ldexp);return(T_BLTIN_DL); }


			/* comparison */
"=="		{ field_count+= count(yytext); *yylval = T_EQ;return(T_EQ);	}
"!="		{ field_count+= count(yytext); *yylval = T_NEQ;return(T_NEQ);	}
"!"			{ field_count+= count(yytext); *yylval = T_NOT;return(T_NOT);	}
"<="		{ field_count+= count(yytext); *yylval = T_LE;return(T_LE);	}
">="		{ field_count+= count(yytext); *yylval = T_GE;return(T_GE);	}
"<"			{ field_count+= count(yytext); *yylval = T_LT;return(T_LT);	}
">"			{ field_count+= count(yytext); *yylval = T_GT;return(T_GT);	}
"&&"		{ field_count+= count(yytext); *yylval = T_AND;return(T_AND);	}
"||"		{ field_count+= count(yytext); *yylval = T_OR;return(T_OR);	}

			/* numbers */
"#"{D}+"R"{AN}+		|
"#b"([01])+		|
"#o"[0-7]+		|
"#x"[0-9a-fA-F]+	|
{D}+"."{D}*({E})?	|
{D}*"."{D}+({E})?	|
{D}+{E}	{ field_count+= count(yytext); *yylval = objects( numeric(_tcsdup(yytext)) ); return(T_RNUMBER);}


PI		{ field_count+= count(yytext); *yylval = objects( PI); return(T_RNUMBER);}
EXP		{ field_count+= count(yytext); *yylval = objects( EXP); return(T_RNUMBER);}
GAMMA	{ field_count+= count(yytext); *yylval = objects( GAMMA); return(T_RNUMBER);}
PHI		{ field_count+= count(yytext); *yylval = objects( PHI); return(T_RNUMBER);}
RAD_TO_DEG		{ field_count+= count(yytext); *yylval = objects( RAD_TO_DEG); return(T_RNUMBER);}
DEG_TO_RAD		{ field_count+= count(yytext); *yylval = objects( DEG_TO_RAD); return(T_RNUMBER);}

"="	{ field_count+= count(yytext); *yylval = T_ASSGNOP;return(T_ASSGNOP);  }
"+"	{ field_count+= count(yytext); *yylval = T_ADD;return(T_ADD);  }
"-"	{ field_count+= count(yytext); *yylval = T_SUBTR;return(T_SUBTR);  }
"*"	{ field_count+= count(yytext); *yylval = T_MULT;return(T_MULT);  }
"/"	{ field_count+= count(yytext); *yylval = T_DIV;return(T_DIV);  }
"%"	{ field_count+= count(yytext); *yylval = T_MOD;return(T_MOD);  }
"^"	{ field_count+= count(yytext); *yylval = T_CARET;return(T_CARET);  }

{D}+	{ field_count+= count(yytext); *yylval = objects( long(atoi( yytext )));
		 return(T_NUMBER);	}

{A}{AN}*"$"	{
				field_count+= count(yytext);	/*in function definition argument list*/
				CSymbolTable* symbolTable	= theApp.GetSymbolTable();
				CStackMachine* stackMachine	= theApp.GetStackMachine();

				stackMachine->indefInArgList=stackMachine->indef=1;
				yytext[count(yytext)-1] = 0;	/* erase the terminal '$'*/

				symbol* s = symbolTable->install(yytext, stackMachine->stack_offset++);
				ASSERT(s->type	== T_ARG);
				*yylval = objects(s);
				return(s->type);
			}

{A}{AN}*	{
				field_count+= count(yytext); 
				CSymbolTable* symbolTable	= theApp.GetSymbolTable();
				CStackMachine* stackMachine	= theApp.GetStackMachine();

				symbol* s = symbolTable->getsym(yytext);
				if (s == 0)
				{
					if (stackMachine->indef)//local vars on stack 
					{
					//	ASSERT(symbolTable->m_b_local);//local vars on stack 
						s = symbolTable->install(yytext, stackMachine->stack_offset++);
						ASSERT(s->type	== T_ARG);
					}
					else //should not get here:
					if (symbolTable->m_b_local)//local vars on stack 
					{
						s = symbolTable->install(yytext, stackMachine->stack_offset++);
						ASSERT(s->type	== T_ARG);
					}
					else 
					{
						s = symbolTable->install(yytext);
						ASSERT(s->type	== T_VAR);
					}
				}
				*yylval = objects(s);
				return(s->type);
			}

\"[^\"]*\"	{
			//	static YYSTYPE buf[1024];
				YYSTYPE buf[1024];
				ZeroMemory( buf, sizeof(buf));
				int len = count(yytext); 
				field_count+= len;
				if (len < 1024)
					_tcsncpy(buf, yytext, len-1);
				else
					_tcsncpy(buf, yytext, 1023);
				*yylval = objects((buf+1));//_tcsdup
				return(T_Q_STRING);
			}

\'[^\"]*\'	{
			//	static YYSTYPE buf[1024];
				YYSTYPE buf[1024];
				ZeroMemory( buf, sizeof(buf));
				int len = count(yytext); 
				field_count+= len;
				if (len < 1024)
					_tcsncpy(buf, yytext, len-1);
				else
					_tcsncpy(buf, yytext, 1023);
				*yylval = objects((buf+1));//_tcsdup
				return(T_Q_STRING);
			}


[ \t\n\r]+ { field_count+= count(yytext); }/*++field_count eat up whitespace */

"."	{ field_count+= count(yytext); return(T_DOT);	 }

".x"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_XCOMP;return(T_XCOMP);	 }
".y"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_YCOMP;return(T_YCOMP);	 }
".z"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_ZCOMP;return(T_ZCOMP);	 }
".w"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_WCOMP;return(T_WCOMP);	 }


".red"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_XCOMP;return(T_XCOMP);	 }
".green"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_YCOMP;return(T_YCOMP);	 }
".blue"/[ ,.;\])=*\/+\-\^%]	{ field_count+= count(yytext); *yylval = T_ZCOMP;return(T_ZCOMP);	 }

";"		{ 
				field_count+= count(yytext); 
				CSymbolTable* symbolTable	= theApp.GetSymbolTable();
				symbolTable->m_b_local = false;
				*yylval = (int)yytext[0];
				return(yytext[0]);
			}

.	{ field_count+= count(yytext);*yylval = (int)yytext[0]; return(yytext[0]);}
%%
int yywrap(void){return 1;}
int count(const YYSTYPE* not_yytext)
{
	int i;

	for (i = 0; not_yytext[i] != '\0'; i++);

	return i;
}
///////////////////////////////////////////////////////////////////////////////
double numeric( YYSTYPE *s)
{
	static YYSTYPE buf[1024];
	_tcsncpy(buf, s, 1023);
	free(s);
//	cout << buf << endl;
	/*istrstream*/ std::wstringstream iss(buf,1023);//sizeof(s)/sizeof(s[0])
	double x=0;
	if (!(iss>>x).fail()) return x;
	return 0;
}
/************************** End Scanner File *****************************/


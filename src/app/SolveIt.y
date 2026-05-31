%{
/*************************************************************************
Operator precedence is determined by the line ordering of the
declarations; the higher the line number of the declaration (lower on
the page or screen), the higher the precedence.  Hence, exponentiation
***************************************************************************/
#include "stdafx.h"

#include "SolveIt.h"
#include "MainFrm.h"
#include "System.h"
#include "SymbolTable.h"

#include "objects.h"
#include "StackMachine.h"
#include "symbol.h"
#include "instruction.h"

#define	YYSTYPE objects
#define YY_NO_GET_LVAL 1
#include "scanner.h"

#include "parser.h" /* for token definitions and yylval */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM   scanner
int yylex ( YYSTYPE * lvalp, yyscan_t scanner );

// must be a reentrant parser:
#define	YYPURE 1
// sm is called inside sm  for 'plot' 

#define	YYDEBUG 1				/* For Debugging*/
#define	YYERROR_VERBOSE 1		/* For Debugging*/
// Last error message returned by parser
static std::string parser_error;


#define gen_code SolveIt::gen_code
#define gen_call SolveIt::gen_call

//static const objects null_ object(T_NULL);

%}
%pure_parser
/*=========================================================================
								TOKENS 
%start input
=========================================================================*/
%token 	 T_NULL
T_IF T_WHILE T_FOR
T_THEN T_ELSE
T_RETURN
T_READ T_WRITE 
T_PLOT
T_DOT
T_Q_STRING		/* quoted string			*/
T_VAR				/* global identifier		*/
T_LOCAL			/*  local var in user-defined function*/
T_ARG				/* argument or local var in user-defined function*/
T_FUNCTION		/* sym->func == user-defined function	*/
T_NUMBER			/* Simple integer			*/
T_RNUMBER			/* decimal dVal				*/
T_BLTIN_VOID		/* f()						*/
T_BLTIN_L		/* f(long)		*/
T_BLTIN_D			/* f(x)						*/
T_BLTIN_DD		/* f(x, y)					*/
T_BLTIN_DDD		/* f(x, y, z)				*/
T_BLTIN_DL		/* f(double,long)			*/
T_BLTIN_LD		/* f(long,double)		*/
T_DEFINE_USER_FUNCTION

SolveIt_First_System_Token
T_NAME
T_SPHERE T_BLOCK T_CYLINDER T_CONE
T_VECTOR_ANALYSIS_OBJECT

T_MASS T_IMASS T_CHARGE T_DENSITY
T_MagneticDipoleMoment
T_MagneticDipoleOrientation

T_INVERSE_INERTIA_TENSOR 
T_RADIUS T_WIDTH T_HEIGHT T_DEPTH T_COLOR T_DRAWSTYLE 
T_SCALE T_DELETE
T_POSITION T_MOMENTUM 
T_ANGULARMOMENTUM T_ORIENTATION
T_VELOCITY 
T_DISPLACEMENT
T_ACCELERATION
T_ANGULARVELOCITY
T_ENERGY T_KINETIC_ENERGY T_POTENTIAL_ENERGY T_POTENTIAL
T_FORCE T_TORQUE T_CONSTRAINT_STATE
T_contactFORCE
T_contactTORQUE
T_BODY_FORCE_EXTERNAL T_BODY_FORCE_INTERNAL 
T_COEFFICIENT_OF_STATIC_FRICTION
T_COEFFICIENT_OF_KINETIC_FRICTION
T_COEFFICIENT_OF_RESTITUTION
T_DAMPING_COEFFICIENT
T_CONTACTS
T_DISTANCE
T_SPEED
T_PERIOD T_TRAJECTORY
T_RECORD
T_ANALYZE
T_PULLEY
T_CONNECT
T_quadrant

T_PLANE
T_POINT_IN_PLANE
T_NORMAL
T_ANGLE
T_LocalVectorToLab
T_LabVectorToLocal
T_LabPointToLocal
T_LocalPointToLab

T_Constraint
T_Linear
T_Quadratic

T_MultiBody_Link
T_MultiBody_Link_Joint
	
T_MultiBody_SPRING_CONSTANT
T_MultiBody_SPRING_LENGTH
T_MultiBody_CONTROL
T_SYSTEM_MultiBody
T_JointType_FIXED
T_JointType_REVOLUTE
T_JointType_PRISMATIC
T_JointType_SCREW
T_JointType_CYLINDRICAL
T_JointType_UNIVERSAL
T_JointType_SPHERICAL
T_JointType_FLOATING
T_JointType_SPRING

T_SYSTEM
T_SYSTEM_LEFT T_SYSTEM_RIGHT T_SYSTEM_TOP T_SYSTEM_BOTTOM T_SYSTEM_NEAR T_SYSTEM_FAR

T_TITLE
T_TICMARK
T_NODES
T_GRAVITATIONAL_COUPLING
T_GRAVITATIONAL_FIELD
T_ELECTRIC_FIELD
T_MAGNETIC_FIELD
T_CENTRAL_FORCE
T_LENNARD_JONES
T_CURRENTLOOP_MAGNETICFIELD_STRENGTH
T_SYSTEM_TIME_STEP
T_SYSTEM_TIME
T_SYSTEM_TIMER_INTERVAL
T_COLLISIONDETECTION
T_BOUNDING_BOX
T_ENFORCE_RIGID_BOUNDARY
T_ONRESET
T_DEFERRED_COMMAND
T_RUN
T_STOP
T_CLEAR
T_TRANSLATE
T_ROTATE
T_TRUE
T_FALSE
T_OFF
T__ON

T_ONE
T_TWO
T_THREE
T_FOUR

T_DIMENSION

T_SHOW

T_SYSTEM_Background
T_SYSTEM_VectorFields
T_SYSTEM_FieldLines
T_SYSTEM_Equipotentials

T_STATE
T_Stationary
T_Scattering

T_Superposition

T_WaveNumber
T_EigenNumber
T_Method
T_Renormalize

//T_InitialPacketSpread
//T_Center_of_Gaussian

T_Incident
//T_Gaussian
//T_Center
//T_Spread

T_FourierTransform
T_Nash_Chen
T_PeriodicCayley
T_Cayley
T_Number
T_Frames
T_DOMAIN
T_RADIAL
T_pseudoCartesian
T_Cartesian
T_INFINITY
T_MINUS_INFINITY


T_XCOMP T_YCOMP T_ZCOMP T_WCOMP
T_SCENE
T_MAP
T_FILE
T_DATA

T_BEGIN
T_END

T_RESET

SolveIt_Last_System_Token


%right	T_ASSGNOP
%left	T_OR
%left	T_AND
%left	T_GT T_GE T_LT T_LE T_EQ T_NEQ
%left	T_ADD T_SUBTR
%left	T_MULT T_DIV T_MOD
%left	T_UNARYMINUS T_NOT 
%right	T_CARET


%start input


/*
 *  Grammar rules
 */


%%
input:/* nothing */{
//terminate the last fetch_execute_cycle(progbase) called for the trailing 'garbage' bytes
				gen_code( instruction::HALT, 0 );
				YYACCEPT;
			}
	|list	{
				gen_code( instruction::HALT, 0 );//terminate OnCompile.fetch_execute_cycle(progbase);
				YYACCEPT;
			}
;

list	: functiondef {
						gen_code( instruction::HALT, 0 );//terminate OnCompile.fetch_execute_cycle(progbase);
					//	YYACCEPT;
					}
	| stmt ';'		{
						gen_code( instruction::HALT, 0 );
					//	YYACCEPT;
					}
	| expr ';'		{
						gen_code( instruction::WRITE_Object, 0 );
						gen_code( instruction::HALT, 0 );
					//	YYACCEPT;
					}
	| sysAsgn ';'	{ gen_code( instruction::HALT, 0 ); }
	| plot ';'		{ gen_code( instruction::HALT, 0 ); YYACCEPT;}
	| individual_commands ';'	{ gen_code( instruction::HALT, 0 ); }
	| error	{
						yyclearin; yyerrok;
					}
	;

asgn: T_VAR T_ASSGNOP expr	{
							gen_code( instruction::STORE, $1 );	//	gen_code( instruction::LD_VAR,	$1 );
// 'cond' should not just return code to 'push' 'var' , but rather to calc var then push
							$$ = $3;// 'cond' must return code to assign new val to var
						}
	|T_ARG T_ASSGNOP expr	{
							gen_code( instruction::STORE_ARG, $1 );	//	gen_code( instruction::LD_ARG,	$1 );
// 'cond' should not just return code to 'push' 'var' , but rather to calc var then push
							$$ = $3;// 'cond' must return code to assign new val to var
						}
	|T_VAR T_ASSGNOP bodyType	{ $$ = gen_code( instruction::Create_Body,	$1 ); }
;

sysAsgn:system_property T_ASSGNOP expr	{
							gen_code( instruction::I_PUSH, $1 );//for debugging; remove later
							gen_code( instruction::SYSTEM_SET, 0 );
						}
;



//If you don'obj specify an action for a rule, Bison supplies a default: $$ = $1

stmt:	  expr	{ gen_code( instruction::XPOP, 0 ); }
	| T_RETURN expr {
					$$ = $2;
					gen_code( instruction::DO_RETURN, 0 );
				}
	| while '(' cond ')' stmt end {		/* while */
		($1).iPtr[1].obj = $5;		/* setjmp [no OP]; body of loop */
		($1).iPtr[2].obj = $6; }	/* setjmp [no OP]; end, if cond fails */
	| for '(' cond ';' cond ';' cond ')' stmt end {		/* for */
		($1).iPtr[1].obj = $5;		/* setjmp [no OP]; condition */
		($1).iPtr[2].obj = $7;		/* setjmp [no OP]; post loop */
		($1).iPtr[3].obj = $9;		/* setjmp [no OP]; body of loop */
		($1).iPtr[4].obj = $10; }	/* end, if cond fails */
	| if '(' cond ')' stmt end {	/* else-less if */
		($1).iPtr[1].obj = $5;		/* thenpart */
		($1).iPtr[2].obj = -1;		/* no elsepart */
		($1).iPtr[3].obj = $6; }	/* end, if cond fails */
	| if '(' cond ')' stmt end T_ELSE stmt end {	/* if with else */
		($1).iPtr[1].obj = $5;		/* thenpart */
		($1).iPtr[2].obj = $8;		/* elsepart */
		($1).iPtr[3].obj = $9; }	/* setjmp [no OP]; end, if cond fails */
	| '{' stmtlist '}'	{ $$ = $2; }
	| T_WRITE expr { $$ = $2; gen_code( instruction::WRITE_Object, 0 );}
	;

cond:	expr	{ gen_code( instruction::HALT, 0 ); }
	;

while:	T_WHILE	{ $$ = Reserve(3, instruction::I_WHILE); }
	;
for:	T_FOR		{ $$ = Reserve(5, instruction::I_FOR); }
	;
if:		T_IF		{ $$ = Reserve(4, instruction::I_IF); }
	;

begin:	  /* nothing */		{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												$$ = stackMachine->next_free_spot_for_code_generation; }
	;
end:	  /* nothing */		{ gen_code( instruction::HALT, 0 );
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												$$ = stackMachine->next_free_spot_for_code_generation; }
	;

stmtlist : /* empty */		{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												$$ = stackMachine->next_free_spot_for_code_generation; }
	| stmtlist  ';' 
	| stmtlist stmt ';'
;
/*
|T_ONE		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_TWO		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_THREE	{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
*/
expr : T_NUMBER			{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
	| T_RNUMBER			{ $$ = gen_code( instruction::I_PUSH,	$1 ); } 
	| T_VAR				{ $$ = gen_code( instruction::LD_VAR,	$1 ); } 
	| T_ARG				{ $$ = gen_code( instruction::LD_ARG,	$1 ); }
	| T_Q_STRING		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }

|T_FALSE	{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_TRUE		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }

|T_OFF		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T__ON		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }

|T_INFINITY			{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_MINUS_INFINITY	{ $$ = gen_code( instruction::I_PUSH,	$1 ); }


|T_Nash_Chen		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_PeriodicCayley	{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_Cayley			{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_RADIAL			{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_pseudoCartesian	{ $$ = gen_code( instruction::I_PUSH,	$1 ); }
|T_Cartesian		{ $$ = gen_code( instruction::I_PUSH,	$1 ); }

	| Vector3D
	| Vector4D
	| Vector5D
	| Vector6D
	| asgn
	| system_property	{
							gen_code( instruction::I_PUSH, $1 );//for debugging; remove later
							$$ = gen_code( instruction::SYSTEM_GET, 0 );
						}

	| property	{
							gen_code( instruction::I_PUSH, $1 );//for debugging; remove later
							$$ = gen_code( instruction::SYSTEM_GET, 0 );
						}

	|Joint_Type			{ $$ = gen_code( instruction::I_PUSH,	$1 ); }

	| T_BLTIN_VOID	'(' ')'							{ $$=$3; gen_code(instruction::CALL_VOID,	$1 );/*->fn*/}
	| T_BLTIN_L		'(' expr ')'					{ $$=$3; gen_code(instruction::CALL_L,		$1 );/*->fnX*/}
	| T_BLTIN_D		'(' expr ')'					{ $$=$3; gen_code(instruction::CALL_D,		$1 );/*->fnX*/}
	| T_BLTIN_DD	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_DD,		$1 );/*->fnXY*/}
	| T_BLTIN_DDD	'(' expr ',' expr ',' expr ')'	{ $$=$3; gen_code(instruction::CALL_DDD,	$1 );/*->fnXYZ*/}
	| T_BLTIN_DL	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_DL,		$1 );/*->fnXL*/}
	| T_BLTIN_LD	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_LD,		$1 );/*->fnLX*/}

	| T_FUNCTION begin '(' arglist ')'	{ $$ = $2; gen_call( $1.sym , $4);	}

	| '(' expr ')'	{ $$ = $2; }

	| expr T_ADD expr		{ gen_code( instruction::ADD,	0 );	}
	| expr T_SUBTR expr		{ gen_code( instruction::SUB,	0 );	}
	| expr T_MULT expr		{ gen_code( instruction::MULT,	0 );	}
	| expr T_DIV expr		{ gen_code( instruction::DIV,	0 );	}
	| expr T_MOD expr		{ gen_code( instruction::MOD,	0 );	}
	| expr T_CARET expr		{ gen_code( instruction::PWR,	0 );		}
	| T_SUBTR expr	%prec T_UNARYMINUS	{ $$=$2; gen_code( instruction::NEG,	0 );	}
	| expr T_GT expr		{ gen_code( instruction::I_GT,	0 );	}
	| expr T_GE expr		{ gen_code( instruction::I_GE,	0 );	}
	| expr T_LT expr		{ gen_code( instruction::I_LT,	0 );	}
	| expr T_LE expr		{ gen_code( instruction::I_LE,	0 );	}
	| expr T_EQ expr		{ gen_code( instruction::I_EQ,	0 );	}
	| expr T_NEQ expr		{ gen_code( instruction::I_NEQ,	0 );	}
	| expr T_AND expr		{ gen_code( instruction::I_AND,	0 );	}
	| expr T_OR expr		{ gen_code( instruction::I_OR,	0 );	}
	| T_NOT expr 			{ gen_code( instruction::I_NOT,	0 );	}
	| '|' expr '|'			{ $$=$2; gen_code( instruction::I_ABS,	0 );	}
;

functiondef :	T_DEFINE_USER_FUNCTION T_VAR	{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->indefInArgList=stackMachine->indef=1;
												stackMachine->nargs	= 0;
												CSymbolTable* symbolTable	= theApp.GetSymbolTable();
												symbolTable->ConvertToFunc( $2.sym );
											}
				'(' arglist ')'				{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->nargs	= stackMachine->stack_offset-1;
												stackMachine->indefInArgList= 0;
											}
				'{' stmtlist '}'			{
												CSymbolTable* symbolTable	= theApp.GetSymbolTable();
												symbolTable->CodeFunc( $2.sym );
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->indef=0;
											}
;


arglist:	/* nothing */	{ $$ = 0;}
	| expr					{ $$ = (long)1;}
	| arglist ',' expr		{ $$ = $1.iVal + (long)1;}
	;


plot:	T_PLOT '(' expr ',' expr ')' 	{ gen_code( instruction::LD_PLOT, 0 );/*plot("func", "options [x,-1,1],...,[xLabel,an_abscissa],[color,0,255,127]");*/ }
	;


Vector3D:'[' expr ',' expr ',' expr ']' { $$ = gen_code( instruction::LD_VECTOR3,	0 ); }
;

Vector4D:'[' expr ',' expr ',' expr  ',' expr ']' { $$ = gen_code( instruction::LD_VECTOR4,	0 ); }
;

Vector5D:'[' expr ',' expr ',' expr  ',' expr  ',' expr ']' { $$ = gen_code( instruction::LD_VECTOR5,	0 ); }
;

Vector6D:'[' expr ',' expr ',' expr  ',' expr  ',' expr  ',' expr ']' { $$ = gen_code( instruction::LD_VECTOR6,	0 ); }
;


bodyTypes:T_SPHERE
|T_BLOCK
|T_CYLINDER
|T_CONE
|T_VECTOR_ANALYSIS_OBJECT
;

bodyType:bodyTypes			{ $$ = gen_code( instruction::I_PUSH,	$1.iVal ); }
;

///////////////////////////////////////////////////////////////////////////////
Joint_Type:
	 T_JointType_FIXED
	|T_JointType_REVOLUTE
	|T_JointType_PRISMATIC
	|T_JointType_SCREW
	|T_JointType_CYLINDRICAL
	|T_JointType_UNIVERSAL
	|T_JointType_SPHERICAL
	|T_JointType_FLOATING
	|T_JointType_SPRING
;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
component:T_XCOMP
|T_YCOMP
|T_ZCOMP
|T_WCOMP
;
basic_property:T_NAME
|T_IMASS
|T_INVERSE_INERTIA_TENSOR 
|T_CHARGE
|T_POSITION
|T_MOMENTUM
|T_ANGULARMOMENTUM
|T_ORIENTATION
|T_FORCE
|T_TORQUE
|T_contactFORCE
|T_contactTORQUE

|T_BODY_FORCE_EXTERNAL
|T_BODY_FORCE_INTERNAL
|T_VELOCITY
|T_ANGULARVELOCITY
|T_ACCELERATION
|T_MASS
|T_DENSITY

|T_MagneticDipoleMoment
|T_MagneticDipoleOrientation

|T_COLOR
|T_DRAWSTYLE
|T_SCALE
|T_DELETE
|T_RADIUS
|T_WIDTH
|T_HEIGHT 
|T_DEPTH
|T_COEFFICIENT_OF_STATIC_FRICTION
|T_COEFFICIENT_OF_KINETIC_FRICTION
|T_COEFFICIENT_OF_RESTITUTION
|T_DAMPING_COEFFICIENT
|T_ENERGY 
|T_KINETIC_ENERGY
|T_POTENTIAL_ENERGY
|T_POTENTIAL
|T_CONTACTS
|T_DISTANCE
|T_SPEED
|T_RECORD
|T_TRAJECTORY
|T_ANALYZE
|T_PERIOD 
|T_SHOW
|T_CONSTRAINT_STATE
|T_CONNECT
|T_PULLEY
|T_quadrant

|T_ONE
|T_TWO
|T_THREE
|T_FOUR


|T_PLANE
|T_POINT_IN_PLANE
|T_NORMAL
|T_ANGLE
|T_LocalVectorToLab
|T_LabVectorToLocal
|T_LabPointToLocal
|T_LocalPointToLab
|T_DIMENSION
//|T_MultiBody_Link
|T_MultiBody_Link_Joint
|T_DISPLACEMENT
|T_MultiBody_SPRING_CONSTANT
|T_MultiBody_SPRING_LENGTH
|T_MultiBody_CONTROL
|T_SYSTEM_MultiBody
|T_SYSTEM_LEFT
|T_SYSTEM_RIGHT
|T_SYSTEM_TOP
|T_SYSTEM_BOTTOM
|T_SYSTEM_NEAR
|T_SYSTEM_FAR
|T_TITLE
|T_TICMARK
|T_NODES
|T_GRAVITATIONAL_COUPLING
|T_GRAVITATIONAL_FIELD
|T_ELECTRIC_FIELD
|T_MAGNETIC_FIELD
|T_CENTRAL_FORCE
|T_LENNARD_JONES
|T_CURRENTLOOP_MAGNETICFIELD_STRENGTH
|T_SYSTEM_TIME_STEP
|T_SYSTEM_TIME
|T_SYSTEM_TIMER_INTERVAL
|T_COLLISIONDETECTION
|T_ENFORCE_RIGID_BOUNDARY
|T_ONRESET
|T_DEFERRED_COMMAND
|T_BOUNDING_BOX
|T_TRANSLATE
|T_ROTATE
|T_SYSTEM_Background
|T_SYSTEM_VectorFields
|T_SYSTEM_FieldLines
|T_SYSTEM_Equipotentials
|T_STATE
|T_Stationary
|T_Scattering
|T_Superposition
|T_WaveNumber
|T_EigenNumber
|T_DOMAIN

|T_Method
|T_Renormalize

//|T_InitialPacketSpread
//|T_Center_of_Gaussian 

|T_Incident
//|T_Gaussian
//|T_Center
//|T_Spread


|T_FourierTransform
|T_Number
|T_Frames

|T_BEGIN
|T_END

|T_RUN
|T_STOP 
|T_CLEAR
|T_SCENE
|T_MAP
|T_FILE
|T_DATA
|T_RESET
;


property:basic_property		{//objects(buf)
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << $1.iVal << ends;
									$$ = buf;
								}
|property component				{//"XXX.x" etc
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << *$1.sPtr << ',' << $2.iVal << ends;
									$$ = buf;
								}
|property T_DOT basic_property	{
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << *$1.sPtr << ',' << $3.iVal << ends;
									$$ = buf;
								}
;


system_property:T_VAR T_DOT	property {
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << $1.sym->name << ':' << *$3.sPtr << ends;
									$$ = buf;

							gen_code( instruction::I_PUSH, $3 );
							gen_code( instruction::LD_VAR, $1 );
						}
|T_SYSTEM T_DOT	property {
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << $1.sym->name << ':' << *$3.sPtr << ends;
									$$ = buf;

							gen_code( instruction::I_PUSH, $3 );
							gen_code( instruction::LD_VAR, $1 );
						}
;

individual_commands:
	rigid_articulated
|rigid_constraint
;

/////////////////////////////////////////////////////////////////////////////
//rigid Constraints
///////////////////////////////////////////////////////////////////////////////
rigid_constraint:
		T_VAR T_DOT T_Constraint T_DOT expr  T_EQ   expr  ','  expr 
		{//body1 connected at expr1 to point=expr2; separation=expr3
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(0)) );
		}
	|	T_VAR T_DOT T_Constraint T_DOT expr  T_EQ   expr  ','  expr   ','  expr 
		{//body1 connected at expr1 to point=expr2; separation=expr3; options=expr4
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(2)) );
		}
	|	T_VAR T_DOT T_Constraint T_DOT expr  T_EQ T_VAR T_DOT T_Constraint T_DOT   expr ','  expr  
		{//body1 connected at point1=expr1 to body2 point2=expr2; separation=expr3
							gen_code( instruction::LD_VAR, $7 );
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(1)) );
		}
	|	T_VAR T_DOT T_Constraint T_DOT expr  T_EQ T_VAR T_DOT T_Constraint T_DOT   expr ','  expr   ','  expr
		{//body1 connected at point1=expr1 to body2 point2=expr2; separation=expr3; options=expr4
							gen_code( instruction::LD_VAR, $7 );
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(3)) );
		}
	|	T_VAR T_DOT T_Constraint T_DOT T_Linear  T_EQ   expr  ','  expr  
		{//body in plane: expr1*x + expr2==0;  positive normal==expr1 [eq [-1,0,0]]
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(4)) );
		}
	|	T_VAR T_DOT T_Constraint T_DOT T_Quadratic  T_EQ   expr  ','  expr   ','  expr  ','  expr  
		{//body in quadratic: expr1*[x.x^2, x.y^2, x.z^2] + expr2*x + expr3 ==0;
		//positive normal==  gradient expr4=true
		//positive normal== -gradient expr4=false
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_Constraint, objects(int(5)) );
		}
;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//rigid articulated CONNECTIONS
///////////////////////////////////////////////////////////////////////////////
rigid_articulated:
		T_VAR T_DOT T_MultiBody_Link T_DOT expr  T_EQ   expr ','  expr ','  expr 
		{
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_CONNECT, objects(int(0)) );
		}
	|	T_VAR T_DOT T_MultiBody_Link T_DOT expr  T_EQ T_VAR T_DOT T_MultiBody_Link T_DOT   expr ','  expr ','  expr  
		{
							gen_code( instruction::LD_VAR, $7 );
							gen_code( instruction::LD_VAR, $1 );
							gen_code( instruction::I_CONNECT, objects(int(1)) );
		}
;
/////////////////////////////////////////////////////////////////////////////

%%
/*
///////////////////////////////////////////////////////////////////////////////
	|	T_MultiBody_Link T_DOT T_MultiBody_SPRING_CONSTANT	{ $$=T_MultiBody_SPRING_CONSTANT; }
	|	T_MultiBody_Link T_DOT T_MultiBody_SPRING_LENGTH	{ $$=T_MultiBody_SPRING_LENGTH; }
	|	T_MultiBody_Link T_DOT T_DISPLACEMENT				{ $$=T_DISPLACEMENT; }
	|	T_MultiBody_Link T_DOT T_VELOCITY					{ $$=T_VELOCITY; }
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
component:XCOMP {$$ = XCOMP - SolveIt_First_System_Token;}
;

basic_property:NAME {$$ = NAME - SolveIt_First_System_Token;}
|POSITION {$$ = POSITION - SolveIt_First_System_Token;}
;

///////////////////////////////////////////////////////////////////////////////
this fails:
plot:	PLOT '(' Q_STRING ',' expr ',' expr ',' NUMBER ')' 	{
								gen_code( instruction::LD_PLOT, 0 );
							}
	;
///////////////////////////////////////////////////////////////////////////////
x=sphere;
y=block;
x.link.[0,0,0] == [0,0,0],prismatic,[1,0,0];
y.link.[0,0,0] == x.link.[0,0,0.5],revolute,[0,0,1];
///////////////////////////////////////////////////////////////////////////////
function Victor(x$) { y=(x)/10.;return cos(2*PI*y);}
potential.name = "Victor";
function v(x$) { y=(x)/10.;return cos(2*PI*y^2);}
plot("v", "  [x-Label,'%f*dog gy'], [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3]");
plot("v", " [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3],[x-Label,'%f*dog gy']");
plot("v", " [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3] ");
plot("v", -PI, 2*PI , 150);
potential.name = "v";

function Q(r$)
{
L0=0.00573645;
R=L0*r;
a          = 2;
d          = 0.2;
return 4*a*R/((a+R)^2+d^2);
}
function V(r$)
{
if (r == 0.0) return (-0.00556);
L0=0.00573645;
R=L0*r;
delta      = 0.0802;
Coulomb    = -0.013;
B0         = 20;
B1         = -0.05;
a          = 2;
d          = 0.2;
OmegaB0    = 0.0575567;
q          = Q(r);
p          = sqrt(q);
Ec         = EllipticEc(q);
K          = EllipticK(q);
F1         = (4*(a + R)*a/((a + R)^2 + d^2))* sqrt(a/(R*q)) * ( -Ec + (1 - q/2)*K );
DeE        = (Ec-K)/(2*q);
DeK        = (Ec-(1-q)*K)/(2*(1-q)*q);
F2         = ((a^2 - R^2 + d^2)*a*p^3/(R^2*sqrt(R*a))) *( -DeE - 0.5*K + (1 - 0.5*q)*DeK);
return ((B1/B0)*( OmegaB0 *( 0.5 + delta ) + Coulomb )*( F1 + F2 )  / OmegaB0 );
}
plot("V", " [x,0, 1000], [y ,  -.012, .008] ");
potential.name = "V";

x=[5,0,0];
x*x;
x^x;
x-3 *x;
y=[5,-3,0];
x*y;
x^y;
x-3 *y;
[0,0,1]*[5,0,0];
[0,0,1]^[5,0,0];

function foo(x$) {
	return x;
}
foo(3.);


function foo(x$, y$) {
	if(y == 0) 	 return x;
	return x/y;
}
foo(3.,4);
foo(foo(1.,2), foo(3.,4));

function ack(x$, y$) {
	if(x == 0) return (y+1);
	if(y == 0) 	 return (ack(x - 1, 1));
	return (ack(x - 1, ack(x, y - 1)));
}
ack(0, 3);
ack(3, 3);


function Ack(x$, y$) {
	if(x == 0) return (y+1);
	return x/y;
}
Ack(0, 3);

	
n=0;
function ack(x$, y$) {
	n = n+1;
	if(x == 0) return (y+1);
	if(y == 0) 	 return (ack(x - 1, 1));
	return (ack(x - 1, ack(x, y - 1)));
}
ack(3, 3);
n;

function foo(x$) { y=(x)/100.;return cos(PI*y);}foo(9.9);

function fo(x$) { y=foo(x)/100.;return cos(PI*y);}fo(9.9);
j=2;while (j<5) { write(j);j=j+1;};
for (j=2;j<5;j=j+1) { write(j);};
if (2>10) { write(9*1.7);};
if (2>1) { write(-PI);x=9*(1.7/0.9);write(x);};
if (2>10) { x=(9*1.7);} else { x=-PI*1.5;};
if (2>10) { write(9*1.7);} else { write(-PI*1.5);};

system.boundingBox = [-0.1, 2.0, 1.9, 4.0, -4.5, 4.5];
system.rotate=true;
system.translate = [ 0.0 ,  0.0 ,  -0.45 ];
system.rotate=true;
system.boundingBox = [-0.1, 2.0, 1.9, 4.0, 2.5, 10];
system.left.right.bottom;
x=sphere;
x.position;
x.position.x;
x.position=[1.56,-.5,.3];
x.position.x/x.position.x;
x.position.w;
x.width;
m1=sphere;
function fx(x$) {return -x;}
m1.externalForce.x = "fx";
m1.position=[1,0,0];
plot("fx", "  [x-Label,'%f*dog gy'], [x,-1.2, 1.3], [y ,  1.2, -1.3]");

*/

////////////////////////////////////////////////////////////////////////

/*=========================================================================
								 YYERROR
=========================================================================*/
using namespace SolveIt;
void yyerror ( char *s )	/* Called by yyparse on error */
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->errors++;
	if (stackMachine->possyyFlexLexer == NULL) return;
	stackMachine->possyyFlexLexer << s << endl;
}
/**************************** End Grammar File ***************************/

%{
/*************************************************************************
#include <FlexLexer.h>
in FlexLexer.h:
//#include <iostream.h>
#include <iostream>

FlexLexer* lexer = new yyFlexLexer;
		while(lexer->yylex() != 0)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
line 686 of parser.cpp:
		yychar = YYLEX;

~ line 698:
change
		yychar = YYLEX;
to
		yychar = lexer->yylex();//YYLEX;
***************************************************************************/
#include "stdafx.h"
#include "Common.h"

#include <FlexLexer.h>
FlexLexer* lexer = 0;			// = new yyFlexLexer;
#undef	YYLEX
#define YYLEX	lexer->yylex()

ostrstream* possyyFlexLexer=0;

#define	YYDEBUG 1				/* For Debugging*/
#define	YYERROR_VERBOSE 1		/* For Debugging*/
// Last error message returned by parser
static std::string parser_error;
%}

%union
{
	symbol*				sym;		/* memory address of var data, code for func */
	unsigned long		p_counter;	/* program counter */
	instruction*		mem_array;	/* while for if 'cond', ...  */
	long				lval;		/* Integer values			*/
	double				real;
	F_D					fnX;
	F_DD				fnXY;
	F_DDD				fnXYZ;
	F_LD				fnLX;
	F_DL				fnXL;
} 
/*=========================================================================
								TOKENS 
=========================================================================*/
%start input
%token /*<keyWord>*/	IF WHILE FOR DO
%token /*<keyWord>*/	THEN ELSE  
%token /*<keyWord>*/	READ WRITE 
%token /*<keyWord>*/	RETURN
%token <sym>		VAR				/* Simple identifier		*/
%token <sym>		FUNCTION		/* sym->func == user-defined function	*/
%token <lval>		NUMBER			/* Simple integer			*/
%token <real>		RNUMBER			/* decimal real				*/
%token <fnX>		BLTIN_D			/* f(x)						*/
%token <fnXY>		BLTIN_DD		/* f(x, y)					*/
%token <fnXYZ>		BLTIN_DDD		/* f(x, y, z)				*/
%token <fnXL>		BLTIN_DL		/* f(double,long)			*/
%token <fnLX>		BLTIN_LD		/* f(long,double)			*/
%token			DEFINE_USER_FUNCTION

%type	<p_counter>	expr stmt asgn stmtlist 
%type	<p_counter>	cond endif endw endfor 
%type	<mem_array>	while for if
%type	<p_counter>	functiondef
%type	<lval>	arglist

%right	ASSGNOP
%left	OR
%left	AND
%left	GT GE LT LE EQ NEQ
%left	'+' '-'
%left	'*' '/' '%'
%left	UNARYMINUS NOT 
%right	'^'
%%

input	:	  /* nothing */
	|expr ';'	{
					gen_code( instruction::WRITE_INT, 0 );
					gen_code( instruction::HALT, 0 );
					YYACCEPT;
				}
	|stmt ';'	{
					gen_code( instruction::HALT, 0 );
					YYACCEPT;
				}
	|functiondef{
					gen_code( instruction::HALT, 0 );
					YYACCEPT;
				}
	|error{
					yyclearin; yyerrok;
			}
	;

/*
functiondef :	DEFINE_USER_FUNCTION VAR	{ indefInArgList=indef=1; ConvertToFunc( $2 );}
				'(' arglist ')'				{ indefInArgList= 0;}
				mark						{ instruction::nInstructionStart = $6;}
				'{' stmtlist '}'			{ gen_code( instruction::I_RETURN, 0 );CodeFunc( $2 );indef=0;}
;
*/

functiondef :	DEFINE_USER_FUNCTION VAR	{
												indefInArgList=indef=1;
												ConvertToFunc( $2 );
											}
				'(' arglist ')'				{
												ReserveIntVal_for_early_Return();
												indefInArgList= 0;
											}
				'{' stmtlist '}'			{
												gen_code( instruction::I_RETURN, 0 );
												CodeFunc( $2 );
												indef=0;
											}
;

asgn: VAR ASSGNOP expr	{
							gen_code( instruction::STORE, $1 );
							$$ = gen_code( instruction::LD_VAR,	$1 );
// 'cond' should not just return code to 'push' 'var' , but rather to calc var then push
							$$ = $3;// 'cond' must return code to assign new val to var
						}
	| FUNCTION ASSGNOP expr	{//end a func def for foo with 'foo = value;'
							gen_code( instruction::STORE, $1 );
							gen_code( instruction::LD_VAR,	$1 );
// 'cond' should not just return code to 'push' 'var' 
							$$ = $3;// 'cond' must return code to assign new val to var
						}
;

//If you don't specify an action for a rule, Bison supplies a default: $$ = $1

stmt:	  expr	{ gen_code( instruction::XPOP, 0 ); }
	| while '(' cond ')' stmt endw {
		($1)[1].t.p_counter = $5;	/* body of loop */
		($1)[2].t.p_counter = $6; }	/* end, if cond fails */
	| for '(' cond ';' cond ';' cond ')' stmt endfor {
		($1)[1].t.p_counter = $5;	/* condition */
		($1)[2].t.p_counter = $7;	/* post loop */
		($1)[3].t.p_counter = $9;	/* body of loop */
		($1)[4].t.p_counter = $10; }	/* end, if cond fails */
	| if '(' cond ')' stmt endif {		/* else-less if */
		($1)[1].t.p_counter = $5;	/* thenpart */
		($1)[2].t.p_counter = -1;	/* no elsepart */
		($1)[3].t.p_counter = $6; }	/* end, if cond fails */
	| if '(' cond ')' stmt endif ELSE stmt endif {	/* if with else */
		($1)[1].t.p_counter = $5;	/* thenpart */
		($1)[2].t.p_counter = $8;	/* elsepart */
		($1)[3].t.p_counter = $9; }	/* end, if cond fails */
	| WRITE expr { gen_code( instruction::WRITE_INT, 0 );}
	| RETURN	{
					gen_code( instruction::DO_RETURN, 0 );
					$$ = next_free_spot_for_code_generation;
				}
	| '{' stmtlist '}'	{ $$ = $2; }
	;

/*
function foo(x$, y$) { foo=x+y;}foo(foo(1,2), foo(3,4));
function foo(x$) { foo=x+1;}foo(foo(99));
function fo(x$) { y=foo(x)/100.;fo=cos(PI*y);}fo(9.9);
j=2;while (j<5) { write(j);j=j+1;};
for (j=2;j<5;j=j+1) { write(j);};
if (2>10) { write(9*1.7);};
if (2>1) { write(-PI);x=9*(1.7/0.9);write(x);};
if (2>10) { x=(9*1.7);} else { x=-PI*1.5;};
if (2>10) { write(9*1.7);} else { write(-PI*1.5);};
*/

cond:	expr	{ gen_code( instruction::HALT, 0 ); }
	;

while:	WHILE	{ $$ = New(3, instruction::I_WHILE); }
	;
for:	FOR		{ $$ = New(5, instruction::I_FOR); }
	;
if:		IF		{ $$ = New(4, instruction::I_IF); }
	;


endif:	  /* nothing */		{ gen_code( instruction::HALT, 0 ); $$ = next_free_spot_for_code_generation; }
	;
endw:	  /* nothing */		{ gen_code( instruction::HALT, 0 ); $$ = next_free_spot_for_code_generation; }
	;
endfor:	  /* nothing */		{ gen_code( instruction::HALT, 0 ); $$ = next_free_spot_for_code_generation; }
	;

stmtlist : /* empty */		{ $$ = next_free_spot_for_code_generation; }
	| stmtlist  ';' 
	| stmtlist stmt ';'
;

expr : NUMBER			{ $$ = gen_code( instruction::LD_INT,	$1 ); }
	| RNUMBER			{ $$ = gen_code( instruction::LD_REAL,	$1 ); } 
	| VAR				{ $$ = gen_code( instruction::LD_VAR,	$1 ); } 

	| asgn

	| BLTIN_D	'(' expr ')'					{ $$=$3; gen_code(instruction::CALL_D,		$1 ); }
	| BLTIN_DD	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_DD,		$1 ); }
	| BLTIN_DDD	'(' expr ',' expr ',' expr ')'	{ $$=$3; gen_code(instruction::CALL_DDD,	$1 ); }
	| BLTIN_DL	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_DL,		$1 ); }
	| BLTIN_LD	'(' expr ',' expr ')'			{ $$=$3; gen_code(instruction::CALL_LD,		$1 ); }

	| FUNCTION  '(' arglist ')'	{ $$=gen_call( $1 , $3);	}

	| '(' expr ')'	{ /*$$ = $2;*/ }

	| expr '+' expr		{ gen_code( instruction::ADD,	0 );		}
	| expr '-' expr		{ gen_code( instruction::SUB,	0 );		}
	| expr '*' expr		{ gen_code( instruction::MULT, 0 );		}
	| expr '/' expr		{ gen_code( instruction::DIV,	0 );		}
	| expr '%' expr		{ gen_code( instruction::MOD,	0 );		}
	| expr '^' expr		{ gen_code( instruction::PWR,	0 );		}
	| '-' expr	%prec UNARYMINUS	{ $$=$2; gen_code( instruction::NEG,	0 );	}
	| expr GT expr	{ gen_code( instruction::I_GT,	0 ); }
	| expr GE expr	{ gen_code( instruction::I_GE,	0 ); }
	| expr LT expr	{ gen_code( instruction::I_LT,	0 ); }
	| expr LE expr	{ gen_code( instruction::I_LE,	0 ); }
	| expr EQ expr	{ gen_code( instruction::I_EQ,	0 ); }
	| expr NEQ expr	{ gen_code( instruction::I_NEQ,	0 ); }
	| expr AND expr	{ gen_code( instruction::I_AND,	0 ); }
	| expr OR expr	{ gen_code( instruction::I_OR,	0 ); }
	| NOT expr 		{ gen_code( instruction::I_NOT,	0 ); }
;


arglist:	/* nothing */	{ $$ = 0;}
	| expr					{ $$ = 1;}
	| arglist ',' expr		{ $$ = $1 + 1;}
	;
%%
/*=========================================================================
								 YYERROR
=========================================================================*/
void yyerror ( char *s )	/* Called by yyparse on error */
{
	errors++;
//printf ("%s\n", s);
	if (possyyFlexLexer == 0) return;
	*possyyFlexLexer << s << endl;
}
/**************************** End Grammar File ***************************/


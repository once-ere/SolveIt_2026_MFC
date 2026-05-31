/* -*- mode:Mail; compile-command: "bison -y -p giac_yy -d input_parser.yy ; mv -f y.tab.c input_parser.cc ; mv -f y.tab.h input_parser.h ; g++ -g -c -I.. -I. input_parser.cc" -*- 
 *
 *  Input grammar definition for reading expressions.
 *  This file must be processed with yacc/bison. */

/*
 *  modified from GiNaC by B. Parisse (X) 2000
 *  GiNaC Copyright (C) 1999-2000 Johannes Gutenberg University Mainz, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*
 *  Definitions
 */

%{
#include "first.h"
#include <stdexcept>
#include <cstdlib>

#include "input_lexer.h"
#include "index.h"
#include "gen.h"
#include "usual.h"
#include "derive.h"
#include "sym2poly.h"
#include "vecteur.h"
#include "modpoly.h"
#include "alg_ext.h"
#include "prog.h"
#include "rpn.h"
#include "intg.h"

using namespace std;
#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

// It seems there is a bison bug when it reallocates space for the stack
// therefore I redefine YYINITDEPTH to 1000 (max size is 10000)
// instead of 200
// Feel free to change if you need but then readjust YYMAXDEPTH
#define YYINITDEPTH 1000
#define YYERROR_VERBOSE 1
// Note that the compilation by bison with -v option generates a file y.output
// to debug the grammar, compile input_parser.yy with bison
// then add yydebug=1 in input_parser.cc at the beginning of yyparse (
// #define YYDEBUG 1

// Parsed output expression
gen parsed_gen;

// Last error message returned by parser
static string parser_error;

gen polynome_or_sparse_poly1(const gen & coeff, const gen & index){
  if (index.type==_VECT){
    index_t i;
    const_iterateur it=index._VECTptr->begin(),itend=index._VECTptr->end();
    i.reserve(itend-it);
    for (;it!=itend;++it){
      if (it->type!=_INT_)
         settypeerr();
      i.push_back(it->val);
    }
    monomial<gen> m(coeff,i);
    return polynome(m);
  }
  else {
    sparse_poly1 res;
    res.push_back(monome(coeff,index));
    return res;
  }
}
%}

/* Tokens (T_LITERAL means a literal value returned by the parser, but not
   of class numeric or symbol (e.g. a constant or the FAIL object)) */
%token 	T_NUMBER T_SYMBOL T_LITERAL T_DIGITS T_STRING T_END_INPUT
	T_EXPRESSION T_UNARY_OP T_OF T_NOT T_TYPE_ID T_VIRGULE
	T_AFFECT T_MAPSTO T_BEGIN_PAR T_END_PAR 
        T_PLUS T_MOINS T_FOIS T_DIV T_MOD T_POW T_QUOTED_BINARY T_QUOTE T_PRIME
	T_TEST_EQUAL T_EQUAL T_NOTEQ T_LESSEQ T_GREATEREQ 
	T_INTERVAL T_UNION T_INTERSECT T_MINUS 
	T_AND_OP T_OR_OP T_XOR_OP T_COMPOSE T_DOLLAR T_DOLLAR_MAPLE 
	T_INDEX_BEGIN T_VECT_BEGIN T_VECT_DISPATCH T_VECT_END T_SET_BEGIN T_SET_END 
	T_SEMI T_DEUXPOINTS T_DOUBLE_DEUX_POINTS
	T_IF T_ELIF T_THEN T_ELSE T_IFTE T_THROW 
	T_SWITCH T_CASE T_DEFAULT T_ENDCASE 
	T_FOR T_FROM T_TO T_DO T_BY T_WHILE T_REPEAT T_UNTIL T_IN
	T_START T_BREAK T_CONTINUE 
	T_TRY T_CATCH T_TRY_CATCH
	T_PROC T_BLOC T_BLOC_BEGIN T_BLOC_END T_RETURN 
	T_LOCAL T_LOCALBLOC T_GLOBAL T_NAME T_OPTION T_PROGRAM 
	T_NULL T_ARGS T_FACTORIAL
	T_RPN_OP T_RPN_BEGIN T_RPN_END T_STACK
	T_GROUPE_BEGIN T_GROUPE_END T_LINE_BEGIN T_LINE_END
	T_VECTOR_BEGIN T_VECTOR_END T_CURVE_BEGIN T_CURVE_END
	T_ROOTOF_BEGIN T_ROOTOF_END 
	T_SPOLY1_BEGIN T_SPOLY1_END T_POLY1_BEGIN T_POLY1_END
	T_MATRICE_BEGIN T_MATRICE_END T_ASSUME_BEGIN T_ASSUME_END T_HELP
	T_EQUAL TI_DEUXPOINTS TI_LOCAL TI_LOOP TI_FOR TI_WHILE TI_STO TI_TRY
	TI_DIALOG T_PIPE T_END_QUOTE TI_DEFINE TI_PRGM TI_SEMI TI_HASH
	T_STRICTINF T_STRICTSUP T_ACCENTGRAVE

/* Operator precedence and associativity */
%nonassoc TI_DEUXPOINTS
%nonassoc T_RETURN
%nonassoc TI_STO
%nonassoc T_PIPE
%nonassoc T_AFFECT
%left TI_SEMI
%left T_VIRGULE
%nonassoc T_MAPSTO
%left T_OR_OP T_XOR_OP
%left T_AND_OP 
%nonassoc T_DOLLAR_MAPLE // not the same precedence than for spreadsheet
%right T_EQUAL 
%left T_TEST_EQUAL T_NOTEQ
%left T_STRICTINF T_STRICTSUP T_LESSEQ T_GREATEREQ
%left T_UNION
%nonassoc T_MINUS
%left T_INTERSECT
%left T_INTERVAL
%left T_PLUS T_MOINS
%nonassoc T_NUMBER
%left T_FOIS 
%left T_DIV 
%nonassoc T_MOD
%nonassoc NEG T_NOT
%nonassoc T_FACTORIAL
%nonassoc T_DOLLAR // this priority for spreadsheet
%right T_POW
%left T_COMPOSE

%start input


/*
 *  Grammar rules
 */

%%
input	: correct_input { if ($1._VECTptr->size()==1)
			     parsed_gen = $1._VECTptr->front();
                          else
			     parsed_gen = gen(*$1._VECTptr,_SEQ__VECT);
			 }
	;

correct_input : exp T_END_INPUT { $$=vecteur(1,$1); }
	      | exp semi T_END_INPUT { $$=vecteur(1,$1); }
	      | exp semi correct_input { $$=mergevecteur(makevecteur($1),*$3._VECTptr); }
	      ;

exp	: T_NUMBER		{$$ = $1;}
	| T_NUMBER exp	{$$=symbolic(at_prod,makevecteur($1,$2));}
	| T_STRING		{ $$=$1; }
	| T_EXPRESSION		{ if ($1.type==_FUNC) $$=symbolic(*$1._FUNCptr,gen(vecteur(0),_SEQ__VECT)); else $$=$1; }
	/* | T_COMMENT		{ $$=symb_comment($1); }
	| T_COMMENT exp		{ $$=$2; } */
	| symbol T_BEGIN_PAR suite T_END_PAR T_AFFECT bloc {$$ = symb_sto(symb_program($3,$3*zero,$6),$1);}
	| symbol T_BEGIN_PAR suite T_END_PAR T_AFFECT exp {$$ = symb_sto(symb_program($3,$3*zero,$6),$1);}
	| exp TI_STO symbol T_BEGIN_PAR suite T_END_PAR {$$ = symb_sto(symb_program($5,$5*zero,$1),$3);}
	| exp TI_STO symbol T_INDEX_BEGIN exp T_VECT_END { gen g=symb_at($3,$5); $$=symb_sto($1,g); }
	| exp TI_STO symbol { $$=symb_sto($1,$3); }
	| TI_DEUXPOINTS symbol T_BEGIN_PAR suite T_END_PAR TI_PRGM prg_suite TI_DEUXPOINTS TI_LOCAL suite TI_DEUXPOINTS prg_suite ti_bloc_end { 
	$$=symb_sto(symb_program($4,$4*zero,symb_local($10,mergevecteur(*$7._VECTptr,*$12._VECTptr))),$2); 
	}
	| TI_DEUXPOINTS symbol T_BEGIN_PAR suite T_END_PAR TI_PRGM prg_suite TI_LOCAL suite TI_DEUXPOINTS prg_suite ti_bloc_end { 
	$$=symb_sto(symb_program($4,$4*zero,symb_local($9,mergevecteur(*$7._VECTptr,*$11._VECTptr))),$2); 
	}
	| TI_DEUXPOINTS symbol T_BEGIN_PAR suite T_END_PAR TI_PRGM TI_DEUXPOINTS TI_LOCAL suite TI_DEUXPOINTS prg_suite ti_bloc_end { 
	$$=symb_sto(symb_program($4,$4*zero,symb_local($9,$11)),$2); 
	}
	| TI_DEUXPOINTS symbol T_BEGIN_PAR suite T_END_PAR TI_PRGM prg_suite ti_bloc_end { 
	$$=symb_sto(symb_program($4,$4*zero,symb_bloc($7)),$2); 
	}
	| TI_DIALOG prg_suite ti_bloc_end { $$=symbolic(*$1._FUNCptr,$2); }
	| TI_DIALOG bloc { $$=symbolic(*$1._FUNCptr,$2); }
	| TI_DEUXPOINTS exp { $$=$2; }
	| TI_DEFINE symbol T_BEGIN_PAR suite T_END_PAR T_EQUAL exp { $$=symb_sto(symb_program($4,$4*zero,$7),$2);}
	| TI_DEFINE symbol T_BEGIN_PAR suite T_END_PAR T_EQUAL TI_PRGM TI_DEUXPOINTS TI_LOCAL suite TI_DEUXPOINTS prg_suite ti_bloc_end { $$=symb_sto(symb_program($4,$4*zero,symb_local($10,$12)),$2);}
	| TI_DEFINE symbol T_BEGIN_PAR suite T_END_PAR T_EQUAL TI_PRGM prg_suite ti_bloc_end { $$=symb_sto(symb_program($4,$4*zero,symb_bloc($8)),$2);}
	| symbol T_BEGIN_PAR suite T_END_PAR {$$ = symb_of($1,$3);}
	| exp T_BEGIN_PAR suite T_END_PAR {$$ = symb_of($1,$3);}
	| symbol 		{$$ = $1;}  
	| T_LITERAL		{$$ = $1;}
	| T_DIGITS		{$$ = $1;}
	| exp T_TEST_EQUAL exp	{$$ = symb_same($1,$3);}
	| exp T_EQUAL exp	        {$$ = equal($1,$3);}
	| T_EQUAL exp { if ($2.type==_SYMB) $$=$2; else $$=symbolic(at_nop,$2); 
		    $$.subtype=_SPREAD__SYMB; }
	| exp T_NOTEQ exp	{$$ = symb_different($1,$3);}
	| exp T_STRICTINF exp		{$$ = symb_inferieur($1,$3);}
	| exp T_LESSEQ exp	{$$ = symb_inf_ou_egal($1,$3);}
	| exp T_STRICTSUP exp		{$$ = symb_superieur($1,$3);}
	| exp T_GREATEREQ exp	{$$ = symb_sup_ou_egal($1, $3);}
	| exp T_PLUS exp	{$$ =symbolic(*$2._FUNCptr,makevecteur($1,$3));}
	| exp T_MOINS exp	{$$ = symb_plus($1,symbolic(at_neg,$3));}
	| exp T_FOIS exp	{$$ =symbolic(*$2._FUNCptr,makevecteur($1,$3));}
	| exp T_DIV exp		{$$ =symbolic(*$2._FUNCptr,makevecteur($1,$3));}
	| exp T_POW exp		{$$ =symbolic(*$2._FUNCptr,makevecteur($1,$3));}
	| exp T_MOD exp		{$$ = makemodquoted($1,$3);}
	| exp T_INTERVAL exp	{$$ = symb_interval($1,$3); }
	| exp T_PLUS T_PLUS		{$$ = symb_sto($1+1,$1);}
	| exp T_MOINS T_MOINS		{$$ = symb_sto($1-1,$1);}
	| exp T_AND_OP exp	{$$ = symb_and($1,$3);}
	| exp T_OR_OP exp	{$$ = symb_ou($1,$3);}
	| exp T_XOR_OP exp	{$$ = symbolic(*$2._FUNCptr,makevecteur($1,$3));}
	| exp T_DEUXPOINTS exp  {$$= symbolic(at_deuxpoints,makevecteur($1,$3));}
	| T_MOINS exp %prec NEG	{ 
					if ($2==unsigned_inf)
						$$ = minus_inf;
					else
						$$ = symbolic(at_neg,$2);
				}
	| T_PLUS exp %prec NEG	{
					if ($2==unsigned_inf)
						$$ = plus_inf;
					else
						$$ = _plus($2);
				}
	| T_SPOLY1_BEGIN exp T_VIRGULE exp T_SPOLY1_END {$$ = polynome_or_sparse_poly1($2,$4);}
	| T_ROOTOF_BEGIN exp T_ROOTOF_END { 
           if ( ($2.type==_SYMB) && ($2._SYMBptr->sommet==at_deuxpoints) )
             $$ = algebraic_EXTension($2._SYMBptr->feuille._VECTptr->front(),$2._SYMBptr->feuille._VECTptr->back());
           else $$=$2;
        }
	| T_ROOTOF_BEGIN exp T_VIRGULE exp T_ROOTOF_END {if ($2.type==_VECT)
     $$ = real_complex_rootof(*$2._VECTptr,$4); else $$=zero;}
	| T_OF { $$=gen(at_of,2); }
	| exp T_AFFECT exp 		{$$ = symb_sto($3,$1);}
	| T_NOT exp	{ $$ = symb_not($2);}
	| T_ARGS T_BEGIN_PAR exp T_END_PAR	{$$ = symb_args($3);}
	| T_ARGS T_INDEX_BEGIN exp T_VECT_END	{$$ = symb_args($3);}
	| T_ARGS { $$=symb_args(vecteur(0)); }
	| T_UNARY_OP T_BEGIN_PAR exp T_END_PAR	{
	$$ = symbolic(*$1._FUNCptr,$3);
	if (*$1._FUNCptr==at_maple_mode)
          maple_mode=$3.val;
	}
	| T_UNARY_OP T_BEGIN_PAR T_END_PAR	{
	$$ = symbolic(*$1._FUNCptr,gen(vecteur(0),_SEQ__VECT));
	if (*$1._FUNCptr==at_rpn)
          rpn_mode=1;
	if (*$1._FUNCptr==at_alg)
          rpn_mode=0;
	}
	| T_UNARY_OP {
	$$ = $1;
	}
	| exp T_PRIME	{$$ = symbolic(at_derive,$1);}
	| exp T_FACTORIAL { $$=symbolic(*$2._FUNCptr,$1); }
	| T_RPN_BEGIN rpn_suite T_RPN_END { $$=symb_rpn_prog($2); }
	| T_PROC T_BEGIN_PAR suite T_END_PAR entete prg_suite T_BLOC_END { $$=symb_program($3,zero*$3,symb_local($5,$6)); }
	| T_PROC T_BEGIN_PAR suite T_END_PAR entete T_BLOC_BEGIN prg_suite T_BLOC_END { $$=symb_program($3,zero*$3,symb_local($5,$7)); } 
	| T_IF T_BEGIN_PAR exp T_END_PAR bloc else {
	$$ = symbolic(*$1._FUNCptr,makevecteur(equaltosame($3),symb_bloc($5),$6));
	}
	| T_IF T_BEGIN_PAR exp T_END_PAR exp T_SEMI else {
        vecteur v=makevecteur(equaltosame($3),$5,$7);
	// cerr << v << endl;
	$$ = symbolic(*$1._FUNCptr,v);
	}
	| T_IF exp T_THEN bloc T_ELSE bloc {$$ = symbolic(*$1._FUNCptr,makevecteur(equaltosame($2),symb_bloc($4),symb_bloc($6)));} 
 	| T_IF exp T_THEN bloc {$$ = symbolic(*$1._FUNCptr,makevecteur(equaltosame($2),$4,undef));} 
 	| T_IF exp TI_DEUXPOINTS exp {$$ = symbolic(*$1._FUNCptr,makevecteur(equaltosame($2),$4,undef));} 
	| T_IF exp T_THEN prg_suite elif {
	$$ = symbolic(*$1._FUNCptr,makevecteur(equaltosame($2),symb_bloc($4),$5));
	} 
	| T_IFTE T_BEGIN_PAR exp T_END_PAR	{$$ = symbolic(*$1._FUNCptr,$3);}
	| T_IFTE {$$ = $1;}
	| T_PROGRAM T_BEGIN_PAR exp T_END_PAR	{$$ = symb_program($3);}
	| T_PROGRAM {$$ = gen(at_program,3);}
	| exp T_MAPSTO bloc	{$$ = symb_program($1,zero*$1,$3);}
	| exp T_MAPSTO exp	{$$ = symb_program($1,zero*$1,$3);}
	| T_BLOC T_BEGIN_PAR exp T_END_PAR	{$$ = symb_bloc($3);}
	| T_BLOC {$$ = at_bloc;}
	| T_LOCALBLOC T_BEGIN_PAR exp T_END_PAR	{$$ = symb_local($3);} 
	| T_LOCALBLOC {$$ = gen(at_local,2);} 
	/* | T_RETURN T_BEGIN_PAR exp T_END_PAR	{$$ = symb_return($3);} */
	| T_RETURN TI_DEUXPOINTS {$$ = gen(*$1._FUNCptr,0);} 
	| T_RETURN exp  { $$=symbolic(*$1._FUNCptr,$2); } 
	| T_RETURN {$$ = gen(*$1._FUNCptr,0);} 
	/* | T_RETURN T_SEMI {$$ = gen(*$1._FUNCptr,0);}  */
	| T_BREAK	{$$ = symbolic(at_break,zero);}
	| T_CONTINUE	{$$ = symbolic(at_continue,zero);}
	| T_FOR T_BEGIN_PAR exp_or_empty T_SEMI exp_or_empty T_SEMI exp_or_empty T_END_PAR bloc {$$ = symbolic(*$1._FUNCptr,makevecteur($3,equaltosame($5),$7,symb_bloc($9)));}
	| T_FOR T_BEGIN_PAR exp_or_empty T_SEMI exp_or_empty T_SEMI exp_or_empty T_END_PAR exp T_SEMI {$$ = symbolic(*$1._FUNCptr,makevecteur($3,equaltosame($5),$7,$9));}
	| T_FOR T_BEGIN_PAR exp T_END_PAR	{$$ = symbolic(*$1._FUNCptr,makevecteur($3));}
	| T_FOR symbol T_IN exp T_DO prg_suite T_BLOC_END { 
	  gen kk(identificateur("index"));
	  vecteur v(*$6._VECTptr);
	  v.insert(v.begin(),symb_sto(symb_at($4,kk),$2));
	  $$=symbolic(*$1._FUNCptr,makevecteur(symb_sto(zero,kk),symb_inferieur(kk,symb_size($4)),symb_sto(symb_plus(kk,plus_one),kk),symb_bloc(v))); 
	  }
	| T_FOR symbol from T_TO exp step T_DO prg_suite T_BLOC_END { if (is_one($4)) $$=symbolic(*$1._FUNCptr,makevecteur(symb_sto($3,$2),symb_inf_ou_egal($2,$5),symb_sto(symb_plus($2,$6),$2),symb_bloc($8))); else $$=symbolic(*$1._FUNCptr,makevecteur(symb_sto($3,$2),symb_sup_ou_egal($2,$5),symb_sto(symb_plus($2,-$6),$2),symb_bloc($8))); }
	| T_FOR symbol from step T_WHILE exp T_DO prg_suite T_BLOC_END { $$=symbolic(*$1._FUNCptr,makevecteur(symb_sto($3,$2),$6,symb_sto(symb_plus($2,$4),$2),symb_bloc($8))); }
	| T_FOR {$$ = gen(*$1._FUNCptr,4);}
	| TI_FOR suite TI_DEUXPOINTS prg_suite ti_bloc_end {
           vecteur & v=*$2._VECTptr;
           if ( (v.size()<3) || v[0].type!=_IDNT){
             cerr << "Syntax For name,begin,end[,step]" << endl;
             $$=undef;
           }
           else {
             gen pas(plus_one);
             if (v.size()==4)
               pas=v[3];
             gen condition;
             if (is_positive(pas))
               condition=symb_inf_ou_egal(v[0],v[2]);
             else
               symb_sup_ou_egal(v[0],v[2]);
             vecteur w=makevecteur(symb_sto(v[1],v[0]),condition,symb_sto(symb_plus(v[0],pas),v[0]),symb_bloc($4));
             $$=symbolic(*$1._FUNCptr,w);
           }
	}
	| TI_WHILE exp TI_DEUXPOINTS prg_suite ti_bloc_end { 
	vecteur v=makevecteur(zero,equaltosame($2),zero,symb_bloc($4));
	$$=symbolic(*$1._FUNCptr,v); 
	}
	| T_WHILE T_BEGIN_PAR exp T_END_PAR bloc { 
	vecteur v=makevecteur(zero,equaltosame($3),zero,symb_bloc($5));
	$$=symbolic(*$1._FUNCptr,v); 
	}
	| T_WHILE T_BEGIN_PAR exp T_END_PAR exp T_SEMI { 
	$$=symbolic(*$1._FUNCptr,makevecteur(zero,equaltosame($3),zero,$5)); 
	}
	| T_WHILE exp T_DO prg_suite T_BLOC_END { $$=symbolic(*$1._FUNCptr,makevecteur(zero,equaltosame($2),zero,symb_bloc($4))); }
	| TI_LOOP prg_suite ti_bloc_end { $$=symbolic(*$1._FUNCptr,makevecteur(zero,plus_one,zero,symb_bloc($2))); }
	| T_TRY bloc T_CATCH T_BEGIN_PAR exp T_END_PAR bloc { $$=symb_try_catch(makevecteur(symb_bloc($2),$5,symb_bloc($7)));}
	| T_TRY_CATCH T_BEGIN_PAR exp T_END_PAR {$$=symb_try_catch(makevecteur($3));}
	| T_TRY_CATCH {$$=gen(at_try_catch,3);}
	| TI_TRY prg_suite T_ELSE prg_suite ti_bloc_end { $$=symb_try_catch(makevecteur(symb_bloc($2),_IDNT_break,symb_bloc($4))); } 
	| TI_TRY prg_suite T_ELSE ti_bloc_end { $$=symb_try_catch(makevecteur(symb_bloc($2),_IDNT_break,undef)); } 
	| TI_TRY prg_suite TI_DEUXPOINTS T_ELSE prg_suite ti_bloc_end { $$=symb_try_catch(makevecteur(symb_bloc($2),_IDNT_break,symb_bloc($5))); } 
	| TI_TRY prg_suite TI_DEUXPOINTS T_ELSE ti_bloc_end { $$=symb_try_catch(makevecteur(symb_bloc($2),_IDNT_break,undef)); } 
	| T_THROW T_BEGIN_PAR exp T_END_PAR { $$ = symb_throw($3); }
	| T_THROW { $$ = at_throw; }
	| T_SWITCH T_BEGIN_PAR exp T_END_PAR T_BLOC_BEGIN switch T_BLOC_END { $$=symb_case($3,$6); }
	| T_CASE T_BEGIN_PAR T_SYMBOL T_END_PAR { $$ = symb_case($3); }
	| T_CASE exp case T_ENDCASE { $$=symb_case($2,$3); }
	| T_TYPE_ID T_SYMBOL { 
	  gen tmp($1); 
	  tmp.subtype=1; 
	  $$=symb_check_type(makevecteur(tmp,$2)); 
	  }
	| T_TYPE_ID { 
	$$=$1; 
	$$.subtype=1; 
	}
	| T_QUOTE T_TYPE_ID T_END_QUOTE { $$=$2; $$.subtype=1; } 
	| exp T_DOLLAR_MAPLE exp { $$ = symb_dollar(makevecteur($1,$3)); } 
	| exp T_DOLLAR exp { $$ = symb_dollar(makevecteur($1,$3)); } 
	| T_DOLLAR T_SYMBOL { $$=symb_dollar($2); }
	| exp T_COMPOSE exp { $$ = symb_compose(makevecteur($1,$3)); }
	| exp T_UNION exp { $$ = symb_union(makevecteur($1,$3)); }
	| exp T_INTERSECT exp { $$ = symb_intersect(makevecteur($1,$3)); }
	| exp T_MINUS exp { $$ = symb_minus(makevecteur($1,$3)); }
	| exp T_PIPE exp { 
	$$=symbolic(*$2._FUNCptr,makevecteur($1,$3)); 
	}
	| T_QUOTED_BINARY { $$ = $1; }
	| T_QUOTE exp T_END_QUOTE		{if ($2.type==_FUNC) $$=$2; else $$=quote($2);}
	| T_ACCENTGRAVE rpn_token T_ACCENTGRAVE { $$=$2; }
	| T_BEGIN_PAR exp T_END_PAR T_BEGIN_PAR suite T_END_PAR {$$ = symb_of($2,$5);}
	| exp T_INDEX_BEGIN exp T_VECT_END	{$$ = symb_at($1,$3);}
	| T_BEGIN_PAR exp T_END_PAR		{$$ = $2;}
	| T_VECT_DISPATCH suite T_VECT_END { $$ = gen(*($2._VECTptr),$1.val);
	if ($2._VECTptr->size()==1 && $2._VECTptr->front().is_symb_of_sommet(at_ti_semi) ) {
	$$=$2._VECTptr->front();
  }
}
	| T_VECT_BEGIN suite T_VECT_END	{$$ = gen(*($2._VECTptr));} 
	| T_SET_BEGIN suite T_SET_END	{$$ = gen(*($2._VECTptr),_SET__VECT);}
	| T_GROUPE_BEGIN suite T_GROUPE_END	{$$ = gen(*($2._VECTptr),_GROUP__VECT);}
	| T_LINE_BEGIN suite T_LINE_END	{$$ = gen(*($2._VECTptr),_LINE__VECT);}
	| T_VECTOR_BEGIN suite T_VECTOR_END	{$$ = gen(*($2._VECTptr),_VECTOR__VECT);}
	| T_CURVE_BEGIN suite T_CURVE_END	{$$ = gen(*($2._VECTptr),_CURVE__VECT);}
	| T_POLY1_BEGIN suite T_POLY1_END	{$$ = gen(*($2._VECTptr),_POLY1__VECT);}
	| T_MATRICE_BEGIN suite T_MATRICE_END	{$$ = gen(*($2._VECTptr),_MATRIX__VECT);}
	| T_ASSUME_BEGIN suite T_ASSUME_END	{$$ = gen(*($2._VECTptr),_ASSUME__VECT);} 
	| exp T_VIRGULE exp           {$$ = makesuite($1,$3); }
	| exp TI_SEMI exp           { $$=symbolic(at_ti_semi,makevecteur($1,$3)); }
	| T_NULL { $$=gen(vecteur(0),_SEQ__VECT); }
	| stack { $$=$1; }
	| T_RPN_OP { $$=$1; }	   
	| T_HELP exp {$$=symb_findhelp($2);} 
	| error	{ 
	messages_to_print += parse_filename + parser_error; 
	/* cerr << messages_to_print; */
	$$=undef; 
	}
	;

symbol	: T_SYMBOL { $$=$1; }
	| T_SYMBOL T_DOUBLE_DEUX_POINTS T_SYMBOL { $$=symb_double_deux_points(makevecteur($1,$3)); }
	| TI_HASH exp {$$=symbolic(*$1._FUNCptr,$2); }
	;

entete	: /* empty */ { $$=vecteur(0); }
	| local entete { $$=mergevecteur(*$1._VECTptr,*$2._VECTptr); }
	| global entete { $$=$2; }
	| option entete { $$=$2; }
	| nom entete { $$=$2; }
	/* | T_COMMENT entete { $$=$2; } */
	;


stack: 	T_STACK T_BEGIN_PAR exp T_END_PAR { if ($3.type==_VECT) $$=gen(*$3._VECTptr,_RPN_STACK__VECT); else $$=gen(vecteur(1,$3),_RPN_STACK__VECT); }
	| T_STACK T_NULL { $$=gen(vecteur(0),_RPN_STACK__VECT); }
	;

local	: T_LOCAL suite T_SEMI  { $$=$2; }
	;

global	: T_GLOBAL suite T_SEMI { $$=vecteur(0); }
	;

option	: T_OPTION exp T_SEMI { $$=vecteur(0); }
	;

nom	: T_NAME exp T_SEMI { $$=$2; }
	;

exp_or_empty: /* empty */ { $$=plus_one;}
	| exp	{ $$=$1; }
	;

suite: /* empty */ { $$=vecteur(0,_SEQ__VECT); }
       | exp { $$=makesuite($1); }
       ;

prg_suite: exp 	{ $$ = makevecteur($1); }
	/* | bloc { $$=makevecteur(symb_bloc($1)); } */
	| prg_suite exp	{ vecteur v(1,$1); 
			  if ($1.type==_VECT) v=*($1._VECTptr); 
			  v.push_back($2); 
			  $$ = v;
			}
	| prg_suite semi		{ $$ = $1;}
	;

rpn_suite  : /* empty */ { $$=vecteur(0); }
	   | rpn_token rpn_suite { $$=mergevecteur(vecteur(1,$1),*($2._VECTptr));}
	   | rpn_token T_VIRGULE rpn_suite { $$=mergevecteur(vecteur(1,$1),*($3._VECTptr));}
	   ;

rpn_token  : T_UNARY_OP { $$=$1; }
	   | T_QUOTE T_UNARY_OP T_END_QUOTE { $$=$2; }
	   | T_NUMBER	{$$ = $1;}
	   | symbol 	{$$ = $1;}  
	   | T_STRING	{ $$=$1; }
	   | T_VECT_DISPATCH rpn_suite T_VECT_END { $$=$2; }
	   | T_PLUS { $$=gen(at_plus,2); }
	   | T_MOINS { $$=gen(at_binary_minus,2); }
	   | T_DIV { $$=gen(at_division,2); }
	   | T_FOIS { $$=gen(at_prod,2); }
	   | T_POW { $$=gen(at_pow,2); }
	   | T_EQUAL { $$=gen(at_equal); }
	   | T_MOD 	 { $$=gen(at_irem,2); }
	   | T_INTERVAL 	 { $$=gen(at_interval,2); }
	   | T_STRICTINF 	 { $$=gen(at_superieur,2); }
	   | T_STRICTSUP 	 { $$=gen(at_inferieur,2); }
	   |  T_AND_OP 	{$$ = gen(at_and,2);}
	   |  T_OR_OP 	{$$ = gen(at_ou,2);}
	   |  T_LESSEQ  { $$=gen(at_inf_ou_egal,2); }
	   |  T_GREATEREQ  { $$=gen(at_sup_ou_egal,2); }
	   |  T_TEST_EQUAL  { $$=gen(at_same,2); }
	   |  T_NOTEQ  { $$=gen(at_different,2); }
	   | T_OF { $$=gen(at_of,2); }
	   |  T_DOLLAR  { $$ = gen(at_dollar,2); }
	   |  T_COMPOSE  { $$ = gen(at_compose,2); }
	   |  T_UNION  { $$ = gen(at_union,2); }
	   |  T_INTERSECT  { $$ = gen(at_intersect,2); }
	   |  T_MINUS  { $$ = gen(at_minus,2); }
	   | T_RPN_OP { $$=$1; }
	   | T_QUOTE T_RPN_OP T_END_QUOTE { $$=$2; }
	   | T_QUOTED_BINARY { $$=$1; }
	   | T_RPN_BEGIN rpn_suite T_RPN_END { $$=gen(*$2._VECTptr,_RPN_FUNC__VECT); }
	   | T_QUOTE exp T_END_QUOTE	{$$ = symb_quote($2);}  
	   | T_IFTE {$$ = gen(at_IFTE,3);}
	   | T_IF rpn_suite T_THEN rpn_suite T_BLOC_END { $$=symb_IFTE(makevecteur($2,$4,symb_NOP(vecteur(0)))); }
	   | T_IF rpn_suite T_THEN rpn_suite T_ELSE rpn_suite T_BLOC_END { $$=symb_IFTE(makevecteur($2,$4,$6)); }
	   | T_START rpn_suite T_BY { vecteur v=*$2._VECTptr; gen step(plus_one); if (!v.empty()) { step=v.back(); v.pop_back();} $$=symb_RPN_FOR(makevecteur(identificateur(" j"),step),gen(v,_RPN_FUNC__VECT)); }
	   | T_START rpn_suite T_CONTINUE { $$=symb_RPN_FOR(makevecteur(identificateur(" j"),plus_one),$2); }
	   | T_FOR symbol rpn_suite T_BY { vecteur v=*$3._VECTptr; gen step(plus_one); if (!v.empty()) { step=v.back(); v.pop_back();} $$=symb_RPN_FOR(makevecteur($2,step),gen(v,_RPN_FUNC__VECT)); }
	   | T_FOR symbol rpn_suite T_CONTINUE { $$=symb_RPN_FOR(makevecteur($2,plus_one),$3); }
	   | T_WHILE rpn_suite T_REPEAT rpn_suite T_BLOC_END { $$=symb_RPN_WHILE($2,$4);}
	   | T_DO rpn_suite T_UNTIL rpn_suite T_BLOC_END { $$=symb_RPN_UNTIL($2,$4); }
	   | T_MAPSTO symbol_suite rpn_sub_prog { $$=symb_RPN_LOCAL($2,$3); }
	   | T_CASE rpn_case T_BLOC_END { $$=symb_RPN_CASE($2); }
	   | T_CASE rpn_case rpn_suite T_BLOC_END { vecteur v(*$2._VECTptr); v.push_back($3); $$=symb_RPN_CASE(v); }
	   | stack { $$=$1; }
	   ;

rpn_sub_prog : T_RPN_BEGIN rpn_suite T_RPN_END { $$=gen(*$2._VECTptr,_RPN_FUNC__VECT); }
	   | T_QUOTE exp T_END_QUOTE	{$$ = symb_quote($2);}  
	   ;

symbol_suite : symbol { $$=vecteur(1,$1); }
	     | symbol_suite symbol { vecteur v=*$1._VECTptr; v.push_back($2); $$=v; }
	     ;

step:	/* empty */ { $$=plus_one; }
	| T_BY exp { $$=$2; }
	;

from:	/* empty */ { $$=plus_one; }
	| T_FROM exp { $$=$2; }
	;

else:	/* empty */ { $$=undef; }
	| ti_else exp T_SEMI { $$=$2; }
	| ti_else bloc { $$=symb_bloc($2); }
	/* | TI_DEUXPOINTS T_ELSE prg_suite {$$=symb_bloc($3); } */
	;

bloc	: T_BLOC_BEGIN prg_suite T_BLOC_END { 
	$$ = $2;
	}
	| T_BLOC_BEGIN local prg_suite T_BLOC_END	{$$ = symb_local($2,$3);}
	/* | T_BLOC_BEGIN T_COMMENT local prg_suite T_BLOC_END	{$$ = symb_local($3,$4);} */
	;

elif:	 ti_bloc_end	{ $$=undef; }
	| ti_else prg_suite ti_bloc_end {
	$$=symb_bloc($2); 
	}
	| T_ELIF exp T_THEN prg_suite elif { 
	  $$=symb_ifte(equaltosame($2),symb_bloc($4),$5);
	  }
	| TI_DEUXPOINTS T_ELIF exp T_THEN prg_suite elif { 
	  $$=symb_ifte(equaltosame($3),symb_bloc($5),$6);
	  } 
	;

ti_bloc_end:	T_BLOC_END { $$=undef; }
	     |	TI_DEUXPOINTS T_BLOC_END { $$=undef; }
	     ;

ti_else:     T_ELSE { $$=undef; }
	     | TI_DEUXPOINTS T_ELSE { $$=undef; }
	     ;

rpn_case: /* empty */ { $$=vecteur(0); }
	  | rpn_case rpn_suite T_THEN rpn_suite T_BLOC_END { 
	  vecteur v(*$1._VECTptr); 
	  v.push_back($2); 
	  v.push_back($4); $$=v; 
	  }
	  ;

switch:	  /* empty */ { $$=vecteur(0); }
	| T_DEFAULT T_DEUXPOINTS bloc { $$=makevecteur(symb_bloc($3));}
	| T_CASE T_NUMBER T_DEUXPOINTS bloc switch { $$=mergevecteur(makevecteur($2,symb_bloc($4)),*($5._VECTptr));}
	;

case:	/* empty */ { $$=vecteur(0); }
	| T_DEFAULT prg_suite { $$=vecteur(1,symb_bloc($2)); }
	| T_OF T_NUMBER T_DO prg_suite case { $$=mergevecteur(makevecteur($2,symb_bloc($4)),*($5._VECTptr));}
	;

semi:	T_SEMI { $$=zero; }
	;

/*
 *  Routines
 */

%%

#ifndef NO_NAMESPACE_GIAC
} // namespace giac

using giac::parser_error;
#endif // ndef NO_NAMESPACE_GIAC

// Error print routine (store error string in parser_error)
int giac_yyerror(char *s)
{
  if ( (*giac_yytext) && (*giac_yytext!='¤')){
	parser_error = ":" + giac::print_INT_(giac::lexer_line_number) + ": " + string(s) + " line " + giac::print_INT_(giac::lexer_line_number) + " at " + string(giac_yytext)+"\n";
        giac::parsed_gen = giac::string2gen(string(giac_yytext),false);
  }
  else {
	parser_error = ":" + giac::print_INT_(giac::lexer_line_number) + ": " +string(s) + " at end of input\n";
        giac::parsed_gen = giac::undef;
  }
  return giac::lexer_line_number;
}

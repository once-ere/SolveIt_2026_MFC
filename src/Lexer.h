// Lexer.h: interface for the Lexer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEXER_H__F1AE2F96_DF70_4B71_856B_F7BB2DB957BB__INCLUDED_)
#define AFX_LEXER_H__F1AE2F96_DF70_4B71_856B_F7BB2DB957BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "objects.h"
#define	YYSTYPE objects
#include <FlexLexer.h>
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{


/* "Now THIS is REALLY STUPID. Why didn't it occur to the Flex people
   that you might want to call a reentrant lexer from a reentrant
   parser? Duh." */
struct MyLexer : public yyFlexLexer 
//struct MyLexer : public FlexLexer 
{
	YYSTYPE yylval;
	YYSTYPE semantic_value;

  MyLexer( istream* arg_yyin = 0, ostream* arg_yyout = 0 )
    : yyFlexLexer(arg_yyin, arg_yyout)
  {
  }

//int yylex(YYSTYPE *lvalp) {yylval = lvalp;return yyFlexLexer::yylex();}
int yylex();

void embedded_yylex(int& result, YYSTYPE *lvalp){
	result=yylex();
*lvalp=yylval;//semantic_value;
}


};

struct PrescanLexer : public yyFlexLexer {
//InternedString pkgName;

PrescanLexer( istream* arg_yyin = 0, ostream* arg_yyout = 0 )
: yyFlexLexer(arg_yyin, arg_yyout)
{
}

int yylex();

};


class ParserExecutionContext
{
public:
	MyLexer& lexer_;
	ParserExecutionContext(MyLexer& _lexer_):lexer_(_lexer_){}

};
//extern "C" 
static int yylex(YYSTYPE *lvalp, void* parm=0)
{
int result;
((ParserExecutionContext*)parm)->lexer_.embedded_yylex(result,lvalp);
return result;
}




////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt



#endif // !defined(AFX_LEXER_H__F1AE2F96_DF70_4B71_856B_F7BB2DB957BB__INCLUDED_)

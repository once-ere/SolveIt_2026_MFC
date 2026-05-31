// symbol.cpp: implementation of the symbol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "StackMachine.h"
#include "symbol.h"
#include "parser.h"// /* for token definitions and yylval */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
symbol::~symbol()
{
	if (type == T_FUNCTION && func && bAutoDelete)
	{
		try {
			delete[] (instruction*) func;//
		} catch( SolveIt_Error& e){e.why();}catch(...) {}
	}
	if (type == T_SYSTEM)
	{
		try {
	//		while(data.ReleaseRef());
	//		data = 0;
			delete &data;
		} catch( SolveIt_Error& e){e.why();}catch(...) {}
	}
}
///////////////////////////////////////////////////////////////////////////////
symbol::symbol(const char *sym_name, const  int L):
		name(sym_name),
		indexInDataArray(L),
		stack_offset(-1),
		data(CStackMachine::data[L]),
		func(0),
		nvargs(-1),
		nargs(-1),
		bAutoDelete(false),
		type(T_VAR)
{
//	CStackMachine::data[L].type = __double;stack_offset
	data.type = __double;
}
///////////////////////////////////////////////////////////////////////////////
symbol::symbol(const char *sym_name, const  int L,  int _stack_offset):
		name(sym_name),
		indexInDataArray(L),
		stack_offset(_stack_offset),
		data(CStackMachine::data[L]),
		func(0),
		nvargs(-1),
		nargs(-1),
		bAutoDelete(false),
		type(T_ARG)
{
//	CStackMachine::data[L].type = __double;
	data.type = __double;
}
///////////////////////////&////////////////////////////////////////////////////
symbol::symbol(instruction* _func, const  int L, const unsigned long _nvargs, const unsigned long _nargs):
//		name(sym_name),
		indexInDataArray(L),
		stack_offset(-1),
		data(CStackMachine::data[L]),
		func(_func),
		nvargs(_nvargs),
		nargs(_nargs),
		bAutoDelete(false),
		type(T_FUNCTION)
{
//	CStackMachine::data[L].type = __double;stack_offset
	data.type = __double;
}
///////////////////////////////////////////////////////////////////////////////
//for "system" ONLY
symbol::symbol(const char *sym_name):
		name(sym_name),
		indexInDataArray(-1),
		stack_offset(-1),
		data(*(new objects(T_SYSTEM))),
		func(0),
		nvargs(-1),
		bAutoDelete(false),
		type(T_SYSTEM)
{
//	data = 0;
}
///////////////////////////////////////////////////////////////////////////////

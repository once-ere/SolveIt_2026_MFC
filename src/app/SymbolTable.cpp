// SymbolTable.cpp: implementation of the CSymbolTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "MainFrm.h"
#include "SymbolTable.h"
#include "StackMachine.h"
#include "symbol.h"
#include "parser.h"// /* for token definitions and yylval */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
//#define ___vec_
////#undef ___vec_
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CSymbolTable::CSymbolTable():
	sym_table(0),
	sym_tableTemp(0),
	m_b_local(false)
{
}
///////////////////////////////////////////////////////////////////////////////
CSymbolTable::~CSymbolTable()
{
		symbol* a = 0;
		symbol* b = 0;
		for ( a =	sym_tableTemp; a != 0;  )
		{
			if(a) b = a->next;
			delete a;
			a = b;
			b = 0;
		}
		for ( a =	sym_table; a != 0;  )
		{
			if(a) b = a->next;
			if (a && a->type == T_FUNCTION && a->func )
			{
				try {
				if (a->bAutoDelete) delete[] (instruction*) a->func;
				a->func = 0;//else 
				} catch( SolveIt_Error& e){e.why();}catch(...) {a->func = 0;}
			}
			delete a;
			a = b;
			b = 0;
		}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSymbolTable::Clear()
{
//	return;
		symbol* a = 0;
		symbol* b = 0;
		for ( a =	sym_tableTemp; a != 0;  )
		{
			if(a) b = a->next;
			delete a;
			a = b;
			b = 0;
		}
		for ( a =	sym_table; a != 0;  )
		{
			if(a) b = a->next;
			if (a && a->type == T_FUNCTION && a->func )
			{
				try {
				if (a->bAutoDelete) delete[] (instruction*) a->func;
				a->func = 0;//else 
				} catch( SolveIt_Error& e){e.why();}catch(...) {a->func = 0;}
			}
			if(a) delete a;
			a = b;
			b = 0;
		}
	sym_tableTemp	= 0;
	sym_table		= 0;
//	symbol* s		= new_symbol("system", false);
//	ASSERT(s->type	== T_SYSTEM);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Install identifier
// for global VAR
symbol* CSymbolTable::install (const char *sym_name )
{
	symbol *s = getsym (sym_name);
	if (s == 0) s = new_symbol (sym_name);
	return s;
}
///////////////////////////////////////////////////////////////////////////////
symbol* CSymbolTable::install(const wchar_t *sym_name)
{
	return install(ToNarrowString(sym_name).c_str());
}
///////////////////////////////////////////////////////////////////////////////
// for global VAR
symbol* CSymbolTable::new_symbol (const char *sym_name, bool bLook_up_data_location)
{
	if (!bLook_up_data_location)
	{
		symbol *ptr = new symbol(sym_name);
		ptr->next = (class symbol *)sym_table;
		sym_table = ptr;
		return ptr;
	}
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	const  int L = CStackMachine::data_location(stackMachine);
	symbol *ptr = new symbol(sym_name, L);
	ptr->next = (class symbol *)sym_table;
	sym_table = ptr;
	return ptr;
//	symbol *ptr = 0;
//	if (indef)//indefInArgList && 
//	{
//		ptr = new_symbol (sym_name, sym_tableTemp);
//	}
//	else
//	{
//		ptr = new_symbol (sym_name, sym_table);
//	}
//	return ptr;
}
///////////////////////////////////////////////////////////////////////////////
// for ARG or local VAR
symbol* CSymbolTable::install (const char *sym_name, const int	stack_offset )
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	ASSERT(stackMachine->indef != 0);
//	ASSERT(m_b_local);//local vars on stack 
	symbol *s = getsym (sym_name);
	if (s == 0)
	{
		const  int L = CStackMachine::data_location(stackMachine);
		s = new symbol(sym_name, L, stack_offset);
		s->next = (class symbol *)sym_tableTemp;
		sym_tableTemp = s;
		return s;
	}
	return s;
}
///////////////////////////////////////////////////////////////////////////////
symbol* CSymbolTable::install(const wchar_t *sym_name, const int stack_offset)
{
	return install(ToNarrowString(sym_name).c_str(), stack_offset);
}
///////////////////////////////////////////////////////////////////////////////
//symbol* CSymbolTable::new_symbol (const char *sym_name, symbol*& sym_Table)
//{
//	const  int L = CStackMachine::data_location(&stackMachine);
//	symbol *ptr = new symbol(sym_name, L);
//	ptr->next = (class symbol *)sym_Table;
//	sym_Table = ptr;
//	return ptr;
//}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
symbol * CSymbolTable::getsym (const char *sym_name)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	symbol *ptr = 0;
	if (stackMachine->indefInArgList && stackMachine->indef)
	{
		return 0;//want new symbols for these vars (ARGs)
	}
	if (stackMachine->indef)//
	{//
//// return local var already in symbol table
		for ( ptr =	sym_tableTemp; ptr != 0; ptr = ptr->next )//
			if (strcmp (ptr->name.c_str(),sym_name) == 0) return ptr;//
////want new symbols for local vars not already in symbol table
//		give globals a try:
	}//
//global?
	for ( ptr =	sym_table; ptr != (symbol *) 0; ptr = (symbol *)ptr->next )
		if (strcmp (ptr->name.c_str(),sym_name) == 0) return ptr;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
symbol * CSymbolTable::getsym(const wchar_t *sym_name)
{
	return getsym(ToNarrowString(sym_name).c_str());
}
///////////////////////////////////////////////////////////////////////////////
const symbol*const CSymbolTable::getsym (const string& sym_name)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	symbol *ptr = 0;
	if (stackMachine->indefInArgList && stackMachine->indef)
	{
		return 0;//want new symbols for these vars (ARGs)
	}
	if (stackMachine->indef)//
	{//
//// return local var already in symbol table
		for ( ptr =	sym_tableTemp; ptr != 0; ptr = ptr->next )//
			if ( ptr->name == sym_name) return ptr;//
////want new symbols for local vars not already in symbol table
//		give globals a try:
	}//
//global?
	for ( ptr =	sym_table; ptr != (symbol *) 0; ptr = (symbol *)ptr->next )
		if ( ptr->name == sym_name) return ptr;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//void ConvertToFunc(const  int	indexInDataArray)
void CSymbolTable::ConvertToFunc(symbol* ptr)
{
	ptr->type = T_FUNCTION;
	ASSERT(ptr->type == T_FUNCTION);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSymbolTable::CodeFunc(symbol* ptr)
{
	if ( ptr == 0) return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec	= *stackMachine->instructionFibers.back();
	int& next_free_spot_for_code_generation	= stackMachine->next_free_spot_for_code_generation;

	int pc	= next_free_spot_for_code_generation-1;
	bool bFoundReturnStatement	= false;
	instruction	ir;
//	for (;codePtr[pc++].op != instruction::I_RETURN && pc < NPROG;);
	do
	{
#if defined(___vec_)
		ir = /*stackMachine->*/codeVec[pc--];
#else
		ir = stackMachine->code[pc--];
#endif
		if (ir.op == instruction::DO_RETURN)
		{
			bFoundReturnStatement	= true;
			break;
		}
	}
	while (ir.op != instruction::DO_RETURN && pc >= 0);

	if (!bFoundReturnStatement)
	{
const char msg[]=
"ERROR: "
"function does not return a value;\n"
"example returns:\n"
"\treturn 0.123456789;\n"
"\treturn x*y+32/PI;\n"
"\treturn 0;";

		AfxMessageBox(GetWC(msg));
//		AfxMessageBox(msg);
		return;
	}

	ptr->func = new instruction[next_free_spot_for_code_generation];//func;//.release();
	if (ptr->func == NULL) throw SolveIt_ERROR("memory allocation error");
	ptr->bAutoDelete = true;
/*stackMachine->*/
#if defined(___vec_)
/*
	CopyMemory(ptr->func, &codeVec.front(), next_free_spot_for_code_generation*sizeof(instruction));
*/
	for (int j=0; j<next_free_spot_for_code_generation; ++j)
	{
		ptr->func[j].obj.ref_count = 0;
		ptr->func[j].obj = 0;
		ptr->func[j].op = instruction::UNDEFINED_OP;
		ASSERT(ptr->func[j].obj.ref_count==0);
		ptr->func[j] = /*stackMachine->*/codeVec[j];
//		ptr->func[j].obj.AddRef();
	}
#else
//	CopyMemory(ptr->func, stackMachine->code, next_free_spot_for_code_generation*sizeof(instruction));
	for (int j=0; j<next_free_spot_for_code_generation; ++j) ptr->func[j] = stackMachine->code[j];
#endif

	ptr->nvargs	= stackMachine->stack_offset-1;
	ptr->nargs	= stackMachine->nargs;

	stackMachine->reset_cycle();
}
///////////////////////////////////////////////////////////////////////////////

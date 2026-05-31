// symbol.h: interface for the symbol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOL_H__52B4C3ED_59D8_484E_B35B_E4CE77BC4772__INCLUDED_)
#define AFX_SYMBOL_H__52B4C3ED_59D8_484E_B35B_E4CE77BC4772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

//#include "StackDatum.h"
///////////////////////////////////////////////////////////////////////////////
//struct function {
//	 int	indexInDataArray;	// memory address of var data
//	instruction*	func;		// code block
//};
///////////////////////////////////////////////////////////////////////////////

//namespace SolveIt
//{
//}//end namespace SolveIt
//using namespace SolveIt;&


class symbol  
{
public:
	symbol(const char *sym_name);//for "system" ONLY
	symbol(const char *sym_name, const  int L);
	symbol(const char *sym_name, const  int L, const  int _stack_offset);
	symbol(instruction*	func, const  int L, const unsigned long _nvargs, const unsigned long _nargs);
	virtual ~symbol();

	string	name;	//	name of symbol
	int	type;		//	 VAR, ARG, LOCAL_VAR [but == ARG], VECTOR3 or FUNCTION
	objects& data;
	const  int	indexInDataArray;	//	if VAR, memory address of var data
	const  int	stack_offset;		//	if  ARG, stack offset of memory address of data
	instruction*	func;			//	if FUNCTION, code block
	unsigned long	nvargs;			//	if FUNCTION, # function arguments and local vars
	unsigned long	nargs;			//	if FUNCTION, # function arguments 
	bool	bAutoDelete;			//	if FUNCTION,  and func = new instruction[nInstructionCounter]; 
///////////////////////////////////////////////////////////////////////////////
	friend ostream& operator<<(ostream& os, const symbol& a)
	{
		os << GetC(a.data.print());
		return os;
/*
		if (a.type == VAR)	return os << a.iVal;
		else
		if (a.type == ARG)	return os << a.dVal;
//		else
//		if (a.type == __complex)	return os << *a.cPtr;
		else
		if (a.type == VECTOR3)	return os << *a.vPtr;
		else
		if (a.type == __string)	return os << a.sPtr;
		return os << "undefined";
*/
	}

	
	class symbol *next;		//	link field
};
///////////////////////////////////////////////////////////////////////////////
/*
symbol* new_symbol (const char *sym_name);
symbol* getsym (const char *sym_name);
symbol* install (const char *sym_name );
symbol* install (const char *sym_name,   int stack_offset );

void ConvertToVector3D(symbol*);

void ConvertToFunc(symbol*);//const  int	indexInDataArray);
void CodeFunc(symbol*);
*/
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SYMBOL_H__52B4C3ED_59D8_484E_B35B_E4CE77BC4772__INCLUDED_)

// SymbolTable.h: interface for the CSymbolTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLTABLE_H__79F01887_1034_4724_9A95_440642BC6B73__INCLUDED_)
#define AFX_SYMBOLTABLE_H__79F01887_1034_4724_9A95_440642BC6B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "symbol.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class symbol;


class CSymbolTable  
{
public:
	CSymbolTable();
	virtual ~CSymbolTable();

	bool m_b_local;//  local var in user-defined function
///////////////////////////////////////////////////////////////////////////////
	symbol* new_symbol (const char *sym_name, bool bLook_up_data_location=true);
	symbol* getsym (const char *sym_name);
	symbol* getsym (const wchar_t *sym_name);
	const symbol*const getsym (const string& sym_name);
	symbol* install (const char *sym_name );
	symbol* install (const wchar_t *sym_name );
	symbol* install (const char *sym_name,   const int stack_offset );
	symbol* install (const wchar_t *sym_name, const int stack_offset );

	void ConvertToVector3D(symbol*);

	void ConvertToFunc(symbol*);//const  int	indexInDataArray);
	void CodeFunc(symbol*);
	void Clear();
///////////////////////////////////////////////////////////////////////////////
//	SYMBOL TABLE
///////////////////////////////////////////////////////////////////////////////
	symbol* sym_table;// = (symbol *)0; /* The pointer to the Symbol Table */
///////////////////////////////////////////////////////////////////////////////
	symbol* sym_tableTemp;// = (symbol *)0; /* for parsing functions */

};



////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt





#endif // !defined(AFX_SYMBOLTABLE_H__79F01887_1034_4724_9A95_440642BC6B73__INCLUDED_)

// StackMachine.h: interface for the CStackMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKMACHINE_H__20AFABE1_7BA0_4099_8C3A_90253E47E5AB__INCLUDED_)
#define AFX_STACKMACHINE_H__20AFABE1_7BA0_4099_8C3A_90253E47E5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "StackDatum.h"
//#include "ConstructType.h"
#include "instruction.h"
#include "CodeFiber.h"
///////////////////////////////////////////////////////////////////////////////
//int yyparse (void);
//int __cdecl yyparse(void);
//You can use `%pure_parser' to request a reentrant parser 
//without also using YYPARSE_PARAM. Then you should call yyparse 
//with no arguments, as usual. 
int yyparse (void *);


void yyerror(char *s);

///////////////////////////////////////////////////////////////////////////////
#if 0
class FlexLexer;
#define	YYSTYPE objects
extern YYSTYPE yylval;

//int yylex(void);//global
int yylex(YYSTYPE *lvalp);//reentrant parser
//int yylex(YYSTYPE *lvalp, YYLTYPE *llocp);//reentrant parser using '@'

#define YY_DECL int yyFlexLexer::yylex(YYSTYPE *lvalp)
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define ___vec_
//#undef ___vec_
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
const  int	NPROG = 16384;
extern unsigned int field_count;
int gen_code(instruction::code_ops operation, const objects& arg);
int gen_call(symbol* sym, const objects& argListCount);
int get_hash(const int numAgsThatFollow , ...);
instruction* Reserve(int nSize, instruction::code_ops o);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CStackMachine  
{
public:
	CStackMachine();
	virtual ~CStackMachine();
	void Clear();
///////////////////////////////////////////////////////////////////////////////
	static string receiveCommandResponse;
///////////////////////////////////////////////////////////////////////////////
	static double receiveStackTopDoubleValue;
	static long receiveStackTopLongValue;
	static Vector3D receiveStackTopVector3DValue;
///////////////////////////////////////////////////////////////////////////////
	static  int data_location(const CStackMachine*sm)	//** Reserves a data location                
	{
		if (sm->indef)
		{
			return NPROG-++local_variables_data_offset;//user func local vars during compilation; reused
		}
		return indexInDataArray++;// func, global vars on data stack
	}
	void initialize_cycle();
	void terminate_cycle();
	void reset_cycle();
	void fetch_execute_cycle(int nCode);
	void fetch_execute_cycle(instruction* code, int nCode);
	bool Compile_and_Run(const string& in, string& out);
	bool Compile_and_Run_and_check_for_reset(const string& in, string& out);
	unsigned int DecodeString(const string& in);
//////////////////////////////////////////////////////////////////////
	friend int gen_code( instruction::code_ops operation, const objects& arg );
	friend int gen_call(symbol* sym, const objects& argListCount);
///////////////////////////////////////////////////////////////////////////////
	friend int get_hash(const int numAgsThatFollow , ...);
///////////////////////////////////////////////////////////////////////////////
	friend instruction* Reserve( int nSize, instruction::code_ops o);
///////////////////////////////////////////////////////////////////////////////
// data
///* CODE Array */
//	vector<instruction> codeVec;
//	instruction* codeVec;


//	CodeFiber codeVec;
//	CodeFiber& codeVec	= *instructionFibers.back();
#if defined(___vec_)
	CodeFibers instructionFibers;
#else
	instruction code[NPROG+1];
#endif


	std::ostream& print_code(std::ostream &os);
/* RUN-TIME stack */
	objects stack[NPROG+1];
	static objects data[NPROG+1];				//for var
	static int indexInDataArray;				// index into objects data[]; for VAR (ARG)
	static int local_variables_data_offset;	// offset from end of data[] for local vars in user funcs
///////////////////////////////////////////////////////////////////////////////
	int errors;							//	Error Count   	
	unsigned long	nargs;		//	if FUNCTION, # function arguments 
	int stack_offset;					// stack offset from top; for ARG; ARG stored on stack AND common storage in data[]
	int	indef;			//** 1 if parsing a function, 0 otherwise 
	int	indefInArgList;	//** 1 if parsing a function & in arg list, 0 otherwise 
	int progbase;
	int next_free_spot_for_code_generation;
	int last_code_generated;
///////////////////////////////////////////////////////////////////////////////
	int fetch_execute_cycle_Level;
//	bool m_bSingleStep;
///////////////////////////////////////////////////////////////////////////////
	instruction* codePtr;//	= code;
	vector<instruction*> codePtrSave;
	int returning;		//** 1 if return stmt seen 
	vector<int> returningSave;
	int	top;
	vector<int> topSave;
	int	pc;	//index into instruction* codePtr[]
	vector<int> pcSave;

	vector<int> field_countSave;
///////////////////////////////////////////////////////////////////////////////
	void SaveCodePtr()		{codePtrSave.push_back(codePtr);}
	void RestoreCodePtr()	{codePtr=codePtrSave.back();codePtrSave.pop_back();}
	void SaveReturning()	{returningSave.push_back(returning);}
	void RestoreReturning()	{returning=returningSave.back();returningSave.pop_back();}
	void SaveTOP()			{topSave.push_back(top);}
	void RestoreTOP()		{top=topSave.back();topSave.pop_back();}
	void SavePC()			{pcSave.push_back(pc);}
	void RestorePC()		{pc=pcSave.back();pcSave.pop_back();}

	void SaveFieldCount()	{field_countSave.push_back(field_count);}
	void RestoreFieldCount(){field_count=field_countSave.back();field_countSave.pop_back();}
///////////////////////////////////////////////////////////////////////////////
	void SaveRegisters()	{SaveCodePtr();SaveTOP();SavePC();SaveReturning();SaveFieldCount();}
	void RestoreRegisters()	{RestoreCodePtr();RestoreTOP();RestorePC();RestoreReturning();RestoreFieldCount();}
///////////////////////////////////////////////////////////////////////////////
//	static auto_ptr<ostrstream> possyyFlexLexer;
//	static ostrstream* possyyFlexLexer;
	static ostrstream possyyFlexLexer;
};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_STACKMACHINE_H__20AFABE1_7BA0_4099_8C3A_90253E47E5AB__INCLUDED_)

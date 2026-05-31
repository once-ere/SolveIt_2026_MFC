// instruction.cpp: implementation of the instruction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "symbol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
//#define ___vec_
////#undef ___vec_
///////////////////////////////////////////////////////////////////////////////
int instruction::operator==	(const instruction& ins) const
{
	if (op == ins.op) return obj == ins.obj;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//int instruction::nInstructionCounter	= 0;
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#if defined(___vec_)
#else
#endif

///////////////////////////////////////////////////////////////////////////////
void LoadReal( int index, const double& val)
{
	gen_code( instruction::I_PUSH,	objects(val) );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec			= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::I_PUSH;
	codeVec[stackMachine->progbase+index].obj	= val;//.dVal
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __double);
}
///////////////////////////////////////////////////////////////////////////////
void LoadLong( int index, const int& val)
{
	gen_code( instruction::I_PUSH,	objects(val) );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::I_PUSH;
	codeVec[stackMachine->progbase+index].obj	= val;//.iVal
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __int);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Load_WRITE_external_int( int index )
{
	gen_code( instruction::WRITE_external_int,	int(0) );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::WRITE_external_int;
	codeVec[stackMachine->progbase+index].obj		= 0;
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __int);
}
///////////////////////////////////////////////////////////////////////////////
void Load_WRITE_external_double( int index )
{
	gen_code( instruction::WRITE_external_double,	int(0) );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::WRITE_external_double;
	codeVec[stackMachine->progbase+index].obj		= 0;
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __int);
}
///////////////////////////////////////////////////////////////////////////////
void Load_WRITE_external_Vector3D( int index )
{
	gen_code( instruction::WRITE_external_Vector3D,	int(0) );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::WRITE_external_Vector3D;
	codeVec[stackMachine->progbase+index].obj		= 0;
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __int);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
symbol* LoadUserFuncCode( int index, instruction* address,  int indexInDataArray,  int nvargs,  int nargs)
{
	symbol* sym	= new symbol(address, indexInDataArray, nvargs, nargs);
	gen_code( instruction::I_PUSH, objects(nargs) );
	gen_code(instruction::CALL,	sym );
	return sym;
#if 0
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].obj		= new symbol(address, indexInDataArray, nvargs, nargs);//.sym
	codeVec[stackMachine->progbase+index].op			= instruction::CALL;
	ASSERT(codeVec[stackMachine->progbase+index].obj.sym->func	== address);
	ASSERT(codeVec[stackMachine->progbase+index].obj.type	== __symbolPtr);
	return codeVec[stackMachine->progbase+index].obj.sym;
#endif
}
///////////////////////////////////////////////////////////////////////////////
void LoadHalt( int index)
{
	gen_code( instruction::HALT, 0 );return;
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec				= *stackMachine->instructionFibers.back();
	codeVec[stackMachine->progbase+index].op		= instruction::HALT;
	codeVec[stackMachine->progbase+index].obj.iVal	= (int)0;//not used
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallRealFunc(_CallUserDefined* func, const double& val)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();

#if 10
int	top	= stackMachine->top;

stackMachine->initialize_cycle();
top	= stackMachine->top;
#endif
///////////////////////////////////////////////////////////////////////////////
		stackMachine->reset_cycle();
	LoadReal(0, val);
//	LoadLong(1, 1);		//nargs
	symbol*s = LoadUserFuncCode(2, func->func, func->indexInDataArray, func->nvargs, 1);
	Load_WRITE_external_double(3);
	LoadHalt(4);
	if (s)
	{
#if 10
		wchar_t rebuf[8192];//static 
		ZeroMemory(rebuf,sizeof(rebuf));

		stackMachine->possyyFlexLexer.seekp(0);
//		auto_ptr</*ostrstream*/ std::wstringstream> a(new /*ostrstream*/ std::wstringstream(rebuf, sizeof(rebuf)/sizeof(rebuf[0])));
//		stackMachine->possyyFlexLexer = a;
		stackMachine->errors = 0;
#endif
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}
#if 10
top	= stackMachine->top;
stackMachine->terminate_cycle();
top	= stackMachine->top;
#endif
}
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallRealFunc(_CallUserDefined* func, const vector<double>& args)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();

#if 10
int	top	= stackMachine->top;

stackMachine->initialize_cycle();
top	= stackMachine->top;
#endif

		stackMachine->reset_cycle();

	int pc = 0;
	vector<double>::const_iterator it = args.begin();
	for (; it != args.end(); ++it)
		LoadReal(pc++, *it);
//	LoadLong(pc++, func->nargs);		//nargs
	symbol*s = LoadUserFuncCode(pc++, func->func, func->indexInDataArray, func->nvargs, func->nargs);
	Load_WRITE_external_double(pc++);
	LoadHalt(pc++);
	if (s)
	{
#if 10
		wchar_t rebuf[8192];//static 
		ZeroMemory(rebuf,sizeof(rebuf));
		stackMachine->possyyFlexLexer.seekp(0);
//		auto_ptr</*ostrstream*/ std::wstringstream> a(new /*ostrstream*/ std::wstringstream(rebuf, sizeof(rebuf)/sizeof(rebuf[0])));
//		stackMachine->possyyFlexLexer = a;
		stackMachine->errors = 0;
#endif
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}

///////////////////////////////////////////////////////////////////////////////
#if 10
top	= stackMachine->top;
stackMachine->terminate_cycle();
top	= stackMachine->top;
#endif
}
///////////////////////////////////////////////////////////////////////////////
#if 0
#endif
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallIntFunc(instruction* address,  int indexInDataArray,  int nvargs, const double& val)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->reset_cycle();
	LoadReal(0, val);
	LoadLong(1, 1);		//nargs
	symbol*s = LoadUserFuncCode(2, address, indexInDataArray, nvargs, 1);
	Load_WRITE_external_int(3);
	LoadHalt(4);
	if (s)
	{
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}
}
////////////////////////////////////////////////////////////////////////////////
void LoadAndCallVector3DFunc(instruction* address,  int indexInDataArray,  int nvargs, const double& val)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->reset_cycle();
	LoadReal(0, val);
	LoadLong(1, 1);		//nargs
	symbol*s = LoadUserFuncCode(2, address, indexInDataArray, nvargs, 1);
	Load_WRITE_external_Vector3D(3);
	LoadHalt(4);
	if (s)
	{
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}
}
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
#if 0
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallRealFunc(instruction* address,  int indexInDataArray,  int nvargs, const double& val)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();

	stackMachine->reset_cycle();
	LoadReal(0, val);
	LoadLong(1, 1);		//nargs
	symbol*s = LoadUserFuncCode(2, address, indexInDataArray, nvargs, 1);
	Load_WRITE_external_double(3);
	LoadHalt(4);
	if (s)
	{
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}
}
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallRealFunc(instruction* address,  int indexInDataArray,  int nvargs, int nargs, const vector<double>& args)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	int pc = 0;
	stackMachine->reset_cycle();
	vector<double>::const_iterator it = args.begin();
	for (; it != args.end(); ++it)
		LoadReal(pc++, *it);
	LoadLong(pc++, nargs);		//nargs
	symbol*s = LoadUserFuncCode(pc++, address, indexInDataArray, nvargs, nargs);
	Load_WRITE_external_double(pc++);
	LoadHalt(pc++);
	if (s)
	{
		stackMachine->fetch_execute_cycle(stackMachine->progbase);
		delete s;
	}
}
#endif






///////////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt
////////////////////////////////////////////////////////////////////////

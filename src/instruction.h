// instruction.h: interface for the instruction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTRUCTION_H__68EF8393_302E_4195_87AD_8DD9EFA6570B__INCLUDED_)
#define AFX_INSTRUCTION_H__68EF8393_302E_4195_87AD_8DD9EFA6570B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//#include "ConstructType.h"
#include "Types.h"
#include "objects.h"

//using namespace std;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
void LoadAndCallRealFunc(_CallUserDefined* func, const double& val);
void LoadAndCallRealFunc(_CallUserDefined* func, const vector<double>& args);


/*
Assignment
The assignment operator (=) is, strictly speaking, a binary operator.  : 

It must be a nonstatic member function.
No operator= can be declared as a nonmember function.

----------------*********** It is not inherited by derived classes ***********----------------
*/
///////////////////////////////////////////////////////////////////////////////
class instruction	//: public CConstructType // <-- useless derivation
{
public:
	virtual ~instruction(){}
	instruction():obj{}
	{
		ASSERT(this->obj.ref_count==0);
	}

	instruction(const instruction& i)
	{
	//	ASSERT(0);
		operator=(i);
	}
	instruction& operator=(const instruction& i)
	{
	//	ASSERT(0);
		op = i.op;
		obj= i.obj;
		return *this;
	}

// data:
	enum code_ops {
				UNDEFINED_OP=0,
				HALT=1,
				STORE,
				STORE_ARG,
				STORE_VEC,
				I_POP,
				I_PUSH,
				LD_COMPLEX,
				LD_VECTOR3,
				LD_VECTOR4,
				LD_VECTOR5,
				LD_VECTOR6,
				LD_VAR,
				LD_ARG,
				SYSTEM_SET,
				SYSTEM_GET,
				READ_NUM, 
				READ_VAR, 
				READ_ARG, 
				WRITE_Object,
				WRITE_external_int,
				WRITE_external_double,
				WRITE_external_Vector3D,
				ADD, 
				SUB, 
				MULT, 
				DIV,
				PWR,
				MOD,
				I_ABS,
				I_EQ,
				I_NEQ,
				I_LT, 
				I_LE, 
				I_GT, 
				I_GE, 
				I_AND, 
				I_OR,
				I_NOT,
				NEG,
				XPOP,
				I_IF,  
				I_WHILE,
				I_FOR,
				DO_RETURN,
				CALL,	// user-defined function call
				CALL_VOID,
				CALL_L,
				CALL_D,
				CALL_DD,
				CALL_LD,
				CALL_DL,
				CALL_DDD,
				LD_PLOT,
				Create_Body,
				I_Constraint,
				I_CONNECT,
				LAST_OP
	} op;

	instruction(code_ops operation, const objects& arg ):op(operation),obj(arg){}
///////////////////////////////////////////////////////////////////////////////
// Comparison operations
//
	int operator==	(const instruction& ins) const;

// common data:
//	static  int nInstructionCounter;	//loc of next byte code stored
	static  int nInstructionStart;	//start of byte code stored for user func

// a friend:
	friend void LoadHalt( int index);
	friend void LoadReal( int index, const double& val);
	friend void LoadLong( int index, const long& val);
///////////////////////////////////////////////////////////////////////////////
	friend void Load_WRITE_external_long( int index );
	friend void Load_WRITE_external_double( int index );
	friend void Load_WRITE_external_Vector3D( int index );
///////////////////////////////////////////////////////////////////////////////
	friend symbol* LoadUserFuncCode( int index, instruction* address,  int indexInDataArray,  int nvargs,  int nargs);
///////////////////////////////////////////////////////////////////////////////
	friend void LoadAndCallLongFunc(instruction* address,  int indexInDataArray,  int nvargs, const double& val);
	friend void LoadAndCallVector3DFunc(instruction* address,  int indexInDataArray,  int nvargs, const double& val);

	friend void LoadAndCallRealFunc(_CallUserDefined* func, const double& val);
	friend void LoadAndCallRealFunc(_CallUserDefined* func, const vector<double>& args);
///////////////////////////////////////////////////////////////////////////////
// remember to also define op:
///////////////////////////////////////////////////////////////////////////////
	instruction& operator=( symbol* val)
	{
		obj.type=__symbolPtr;		
		obj.sym		= const_cast<symbol*>(val);
		return *this;
	}

///////////////////////////////////////////////////////////////////////////////
// data:
	objects obj;
};
///////////////////////////////////////////////////////////////////////////////
static const char* op_name[instruction::LAST_OP+1] = {
	"undefined", "halt", 
	"store", "store_arg", "store_vec",
	"pop", "push", 
	"ld_complex", "ld_vector", "ld_var", "ld_arg",
	"sys_set","sys_get",
	"read_num","read_var","read_arg",
	"print","write int","write double","write vec",
	"add", "sub", "mult", "div", "pwr", "mod", "abs"
	"eq", "neq", "lt", "le", "gt", "ge", "and", "or", "not", "neg",
	"xpop",
	"if", "while", "for", "return", "call"
	"call_null", "call_x", "call_xy", "call_lx", "call_xl", "call_xyz",
	"plot",
	"create body",
	"onstraint",
	"connect",
	"last op"
};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_INSTRUCTION_H__68EF8393_302E_4195_87AD_8DD9EFA6570B__INCLUDED_)

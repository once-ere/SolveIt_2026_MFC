// Frame.h: interface for the Frame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__70A74123_8264_4DBB_A92A_BCB6F312F6D3__INCLUDED_)
#define AFX_FRAME_H__70A74123_8264_4DBB_A92A_BCB6F312F6D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// frame
struct frame
{
	instruction*	codePtr;	// return code block; this code 'CALL's
	int			pc;			// return to instruction* = codePtr + pc
	int			top;		// stack top when created
	long		nargs;		//# function arguments
	long		nvargs;		//# function arguments and local vars
	frame(instruction* pi, const int& _pc, const int& stackTop, const long& n, const long& nv) : codePtr(pi), pc(_pc), top(stackTop), nargs(n), nvargs(nv) {}
	frame(const frame& a) : codePtr(a.codePtr), pc(a.pc), top(a.top), nargs(a.nargs) , nvargs(a.nvargs) {}
	frame& operator =(const frame& a)
	{
		codePtr	= a.codePtr;
		pc		= a.pc;
		top		= a.top;
		nargs	= a.nargs;
		nvargs	= a.nvargs;
		return *this;
	}
};
class _Frame: public vector<frame>
{
public:
//	operator vector<frame>&() {return *static_cast<vector<frame>*>(this);}
	frame pop()
	{
		if (size() <= 0) throw SolveIt_ERROR(L"ERROR: frame underflow");
		frame f = back();
		pop_back();
		return f;
	}
	frame& top()
	{
		if (size() <= 0) throw SolveIt_ERROR(L"ERROR: frame underflow");
		return back();
	}

};
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_FRAME_H__70A74123_8264_4DBB_A92A_BCB6F312F6D3__INCLUDED_)

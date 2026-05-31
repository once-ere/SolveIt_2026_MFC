// CodeFiber.h: interface for the CodeFiber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEFIBER_H__6A215A31_F6B9_47D5_814A_A912F8F7454F__INCLUDED_)
#define AFX_CODEFIBER_H__6A215A31_F6B9_47D5_814A_A912F8F7454F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
#include "instruction.h"
///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////


//typedef vector<instruction> codeFiber;
using codeFiber = std::vector<instruction>;
///////////////////////////////////////////////////////////////////////////////
// CodeFiber
class CodeFiber  :	public codeFiber
{
public:
	CodeFiber()
	{
		reserve(1024);
#if 0
		codeFiber::iterator it;
		for (int j=0;j<1024;++j)
		{
			push_back(instruction());
		}
		for (it=begin();it!=end();++it)
		{
			ASSERT((*it).obj.ref_count == 0);
		}
#endif

	}
	virtual ~CodeFiber();
	void Clear();

};

///////////////////////////////////////////////////////////////////////////////
// CodeFibers
//typedef vector<CodeFiber*> codeFibers;
using codeFibers = std::vector<CodeFiber*>;

class CodeFibers  :	public codeFibers
{
public:
	CodeFibers() {}
	virtual ~CodeFibers();

///////////////////////////////////////////////////////////////////////////////
	void Clear();
	void initialize_new_cycle();
	void terminate_top_cycle();
	void reset_top_cycle();
};
///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt



#endif // !defined(AFX_CODEFIBER_H__6A215A31_F6B9_47D5_814A_A912F8F7454F__INCLUDED_)

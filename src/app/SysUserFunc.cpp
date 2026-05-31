// SysUserFunc.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"

#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "parser.h" /* for token definitions and yylval */



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double CSystem::GetUserDefinedPotential(double _x, _FunctionFor _for)
{
	_CallUserDefined* selected = 0;
	switch(_for)
	{
		case __External1:selected	= &m_uUserDefinedExternal1;break;
		case __External2:selected	= &m_uUserDefinedExternal2;break;
		case __External3:selected	= &m_uUserDefinedExternal3;break;
		case __Red:selected			= &m_uUserDefinedRed;break;
		case __Green:selected		= &m_uUserDefinedGreen;break;
		case __Blue:selected		= &m_uUserDefinedBlue;break;
		case __Potential:selected	= &m_uUserDefinedPotential;break;
	}
	if (selected == 0) return 0;
//	if (m_uUserDefinedExternal1.func == 0) return 0;
	try
	{
		vector<double> args;
		args.push_back(_x);
//		LoadAndCallRealFunc(&m_uUserDefinedExternal1, args);
		LoadAndCallRealFunc(selected, args);
		return CStackMachine::receiveStackTopDoubleValue;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
double CSystem::GetUserDefinedPotential(_FunctionFor _for, long nargs, ... )
{
	_CallUserDefined* selected = 0;
	switch(_for)
	{
		case __External1:selected	= &m_uUserDefinedExternal1;break;
		case __External2:selected	= &m_uUserDefinedExternal2;break;
		case __External3:selected	= &m_uUserDefinedExternal3;break;
		case __Red:selected			= &m_uUserDefinedRed;break;
		case __Green:selected		= &m_uUserDefinedGreen;break;
		case __Blue:selected		= &m_uUserDefinedBlue;break;
		case __Potential:selected	= &m_uUserDefinedPotential;break;
	}
	if (selected == 0) return 0;
	va_list marker;
	va_start( marker, nargs);	// Initialize variable arguments.
	vector<double> args;
	long count=nargs;
//	for (double x = va_arg( marker, double); count>0; --count, x = va_arg( marker, double))
//	{
//		args.push_back(x);
//	}

	for(count=0; count < nargs; count++)
	{
        double x = va_arg( marker, double);// va_arg(marker, VARIANT);
		args.push_back(x);
	}

	va_end( marker );		//Reset variable arguments.
	if (selected->nargs != args.size()) throw SolveIt_ERROR("CSystem::GetUserDefinedPotential::ERROR: wrong number of arguments in function call");
	try	{
		if (selected->func)
			{
/*
				switch(selected->m_bIsTimeDependent)
				{
				case false:
					switch(selected->nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(X);
						break;
					case 2:
						args.push_back(X);args.push_back(vX);
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				case true:
					switch(selected->nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(X);
						break;
					case 3:
						args.push_back(t);
						args.push_back(X);args.push_back(vX);
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				}
*/
			LoadAndCallRealFunc(selected, args);
			}
	}
	catch (char * str)
	{
		const std::wstring message = ToWideString(str);
		::MessageBoxW(0, message.c_str(), L"Error", MB_OK);
		return 0;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
		return CStackMachine::receiveStackTopDoubleValue;
}

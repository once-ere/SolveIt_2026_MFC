// StackMachine.cpp: implementation of the CStackMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "StackMachine.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "System.h"
#include "MainFrm.h"
#include <sstream>

#define	YYSTYPE objects
#define YY_NO_GET_LVAL 1
#include "scanner.h"


#include "Matherror.h"
///////////////////////////////////////////////////////////////////////////////
//#define ___vec_
////#undef ___vec_
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
double		CStackMachine::receiveStackTopDoubleValue=0;
long		CStackMachine::receiveStackTopLongValue=0;
Vector3D	CStackMachine::receiveStackTopVector3DValue;
///////////////////////////////////////////////////////////////////////////////

string CStackMachine::receiveCommandResponse("OK\r\n");


/* CODE Array */
//instruction CStackMachine::code[NPROG+1];
/* RUN-TIME stack */
//objects CStackMachine::stack[NPROG+1];
objects CStackMachine::data[NPROG+1];
///////////////////////////////////////////////////////////////////////////////
int CStackMachine::indexInDataArray				= 0;	// index into objects data[]; for VAR (ARG)
int CStackMachine::local_variables_data_offset	= 0;	// offset from end of data[] for local vars in user funcs
//int CStackMachine::stack_offset					= 1;	// stack offset from top; for ARG; ARG stored on stack AND common storage in data[]
//int CStackMachine::indef			= 0;//** 1 if parsing a function, 0 otherwise 
//int CStackMachine::indefInArgList	= 0;//** 1 if parsing a function & in arg list, 0 otherwise 
//int CStackMachine::progbase	= 0;
//int CStackMachine::next_free_spot_for_code_generation=0;
//int CStackMachine::last_code_generated=0;
///////////////////////////////////////////////////////////////////////////////
//instruction* CStackMachine::codePtr	= 0;//CStackMachine::code;
//int CStackMachine::fetch_execute_cycle_Level	= 0;
//int CStackMachine::returning					= 0;//** 1 if return stmt seen 
//int CStackMachine::top		= NPROG;
//int CStackMachine::pc		= 0;	//index into instruction* codePtr[]
//int CStackMachine::errors	= 0;	//	Error Count
///////////////////////////////////////////////////////////////////////////////
//auto_ptr<ostrstream> CStackMachine::possyyFlexLexer;//=0;
//ostrstream* CStackMachine::possyyFlexLexer;//=0;
static		char rebuf[8192];// 
//		ZeroMemory(rebuf,sizeof(rebuf));
ostrstream CStackMachine::possyyFlexLexer( rebuf, sizeof(rebuf)/sizeof(rebuf[0]) );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CStackMachine::Clear()
{
	reset_cycle();
	instructionFibers.Clear();
	terminate_cycle();
	CStackMachine::indexInDataArray				= 0;	// index into objects data[]; for VAR (ARG)
	CStackMachine::local_variables_data_offset	= 0;	// offset from end of data[] for local vars in user funcs
	field_count							= 0;
	errors								= 0;
	nargs								=-1;
	stack_offset						= 1;
	indef								= 0;
	indefInArgList						= 0;
	progbase							= 0;
	next_free_spot_for_code_generation	= 0;
	last_code_generated					= 0;
///////////////////////////////////////////////////////////////////////////////
	fetch_execute_cycle_Level			= 0;
//	m_bSingleStep						= false;
///////////////////////////////////////////////////////////////////////////////
	codePtr								= 0;
	returning							= 0;
	top									= NPROG;
	pc									= 0;
	initialize_cycle();
	CSymbolTable* symbolTable	= theApp.GetSymbolTable();
	symbolTable->Clear();
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStackMachine::CStackMachine()://code(&codeVec.front())
//	codePtr(code),
//	codeVec(0),
	next_free_spot_for_code_generation(0),
	last_code_generated(0),
	progbase(0),
	errors(0),
//	m_bSingleStep(false),
	fetch_execute_cycle_Level(0),
	returning(0),					// ==1 if return stmt seen 
	top(NPROG),
	pc(0),
	stack_offset(1),				// stack offset from top; for ARG; ARG stored on stack AND common storage in data[]
	nargs(-1),
	indef(0),						// ==1 if parsing a function, 0 otherwise 
	indefInArgList(0)				// ==1 if parsing a function & in arg list, 0 otherwise 
{
//	CStackMachine::codePtr	= code;
//	codeVec.reserve(1024);
	initialize_cycle();
}

CStackMachine::~CStackMachine()
{
		ZeroMemory(rebuf,sizeof(rebuf));
}
///////////////////////////////////////////////////////////////////////////////
void CStackMachine::reset_cycle()
{
//	CSolveItApp::g_boolDoReset	= false;
	CodeFiber& codeVec	= *instructionFibers.back();
	CSymbolTable* symbolTable	= theApp.GetSymbolTable();
		symbol* a = 0;
		symbol* b = 0;
		for ( a =	symbolTable->sym_tableTemp; a != 0;  )
		{
			if(a) b = a->next;
			delete a;
			a = b;
			b = 0;
		}
	pc			= progbase;

	last_code_generated					= progbase;
	next_free_spot_for_code_generation	= progbase;

	top			= NPROG;

//	ZeroMemory(stack, (NPROG+1)*sizeof(objects));
//	ZeroMemory(code+progbase, (NPROG+1-progbase)*sizeof(instruction));
	returning		= 0;
	indef			= 0;
	indefInArgList	= 0;

	stack_offset	= 1;
	local_variables_data_offset=0;//
	symbolTable->sym_tableTemp	= 0;
	fetch_execute_cycle_Level	= 0;
//	field_count	= 0;

/*
	if (codeVec) delete[] codeVec;
	codeVec=0;
	codeVec			= new instruction[NPROG+1];
*/
try {
//	vector<instruction>::iterator it;
//	for (it=codeVec.begin();it!=codeVec.end();++it)
//	{
//		(*it).obj = 0;
//	}

//	codeVec.clear();
	instructionFibers.reset_top_cycle();
	ASSERT(codeVec.size() == 0);
} catch( SolveIt_Error& e){e.why();}catch(...)
{
	AfxMessageBox(L"ERROR: CStackMachine::reset_cycle");
}
//	codePtr		= code;
//	codePtr		= codeVec;
	if (0 == instructionFibers.size())
	{
		codePtr	= 0;
	}
	else
	{
		codePtr		= &codeVec.front();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CStackMachine::initialize_cycle()
{
	SaveRegisters();
	instructionFibers.initialize_new_cycle();
}
///////////////////////////////////////////////////////////////////////////////
void CStackMachine::terminate_cycle()
{
	instructionFibers.terminate_top_cycle();
	RestoreRegisters();
}
///////////////////////////////////////////////////////////////////////////////
std::ostream& CStackMachine::print_code(std::ostream &os)
{  
static const string CrLf("\r\n");
	int i = 0;
	while (i < next_free_spot_for_code_generation && !os.fail())
	{
#if defined(___vec_)
	CodeFiber& codeVec	= *instructionFibers.back();
		os << i << ":: " << op_name[(int) codeVec[i].op] << ": " << GetC(codeVec[i].obj.print()) << CrLf;
#else
		os << i << ":: " << op_name[(int) code[i].op] << ": " << GetC(code[i].obj.print()) << CrLf;
#endif
		i++;
	}
	return os;
}
///////////////////////////////////////////////////////////////////////////////
instruction* SolveIt::Reserve( int nSize, instruction::code_ops o)
{
	CStackMachine* stackMachine						= theApp.GetStackMachine();
	CodeFiber& codeVec	= *stackMachine->instructionFibers.back();
	int n											= stackMachine->next_free_spot_for_code_generation;
	stackMachine->next_free_spot_for_code_generation	+=nSize;
	stackMachine->last_code_generated				= stackMachine->next_free_spot_for_code_generation-1;
	int j = 0;
	for (j=0;j<nSize;++j)
	{
#if defined(___vec_)
		instruction i;
		i.op = instruction::HALT;//not used
		i.obj= 0;
		ASSERT(i.obj.ref_count	== 0);
//		i.obj.AddRef();
		/*stackMachine->*/codeVec.push_back(i);
	//	stackMachine->codeVec[n+j]=(i);
//
		ASSERT(/*stackMachine->*/codeVec[n+j].op == instruction::HALT);
		ASSERT(/*stackMachine->*/codeVec[n+j].obj.type	== __int);
		ASSERT(/*stackMachine->*/codeVec[n+j].obj.ref_count	== 0);
#else
		stackMachine->code[n+j].op = instruction::HALT;//not used
		stackMachine->code[n+j].obj=( int)0;//.iVal = 0;//p_counter
		ASSERT(stackMachine->code[n+j].obj.type	== __int);
#endif

	}

#if defined(___vec_)
	/*stackMachine->*/codeVec[n+0].op = o;
	/*stackMachine->*/codeVec[n+0].obj = &/*stackMachine->*/codeVec[n+0];
	for (j=0;j<nSize;++j)
	{
		ASSERT(/*stackMachine->*/codeVec[n+0].obj.iPtr+j  == &/*stackMachine->*/codeVec[n+j]);
		ASSERT(/*stackMachine->*/codeVec[n+0].obj.iPtr[j] == /*stackMachine->*/codeVec[n+j]);
	}
	ASSERT(/*stackMachine->*/codeVec[n+0].obj.type	== __instructionPtr);
//	ASSERT(/*stackMachine->*/codeVec.size() == stackMachine->next_free_spot_for_code_generation);
#else
	stackMachine->code[n+0].op = o;
//	stackMachine->code[n+0].obj.iVal = n;//p_counter
	stackMachine->code[n+0].obj = &stackMachine->code[n+0];//.iPtr
	ASSERT(stackMachine->code[n+0].obj.type	== __instructionPtr);
//	stackMachine->code[n+0].obj.type = __instructionPtr;
//	stackMachine->code[n+0].obj.iPtr = &stackMachine->code[n+0];
#endif


#if defined(___vec_)
//	ASSERT(&/*stackMachine->*/codeVec.front()+n	== &/*stackMachine->*/codeVec[n]);
	return &/*stackMachine->*/codeVec[n];
#else
	return stackMachine->code+n;
#endif
}
///////////////////////////////////////////////////////////////////////////////
int SolveIt::get_hash(const int numAgsThatFollow , ...)
{
	if( numAgsThatFollow == 0 ) return 0;
	int val[6];
	int*L = val;
	int count = 1, i = 0;
	va_list marker;
	va_start( marker, numAgsThatFollow );	// Initialize variable arguments.
	for ( i = va_arg( marker, int); count <= numAgsThatFollow && count <= sizeof(val)/sizeof(val[0]); i = va_arg( marker, int), ++count)
	{
		*L++ = i;
	}
	va_end( marker );              /* Reset variable arguments.      */
	if( numAgsThatFollow == 2 ) return MAKELONG(val[0],val[1]);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int CStackMachine::DecodeString(const string& in)
{
	string keyBuffer = in;
	unsigned int result			= 0;
	char seps[]			= ":,\t\n \r";
	char* context = 0;
	char* token = strtok_s(keyBuffer.data(), seps, &context);//_tcstok
	while( token != NULL )
	{
		int n = static_cast<int>(strlen(token));
		std::istringstream iss(token);
		unsigned int key=0;
		if(!(iss>>key).fail())
		{
			for (int j=0; j<n; ++j) result *= 10;
			result += key;
		}
		token = strtok_s(NULL, seps, &context);
	}
	return result;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SolveIt::gen_code( instruction::code_ops operation, const objects& arg )
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	CodeFiber& codeVec	= *stackMachine->instructionFibers.back();
// '(' arglist ')' will generate code to push args onto stack 
// incorrect to include as part of function code:
	if (stackMachine->indefInArgList) return 0;
///////////////////////////////////////////////////////////////////////////////
	int& next_free_spot_for_code_generation	= stackMachine->next_free_spot_for_code_generation;

#if defined(___vec_)
	instruction i(operation, arg);
//	i.obj.AddRef();
//	stackMachine->codeVec[next_free_spot_for_code_generation]	= i;
	/*stackMachine->*/codeVec.push_back(i);
	if (arg.type == __int)
		ASSERT(/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj.ref_count	== 0);
//	ASSERT(/*stackMachine->*/codeVec.size() == 1+stackMachine->next_free_spot_for_code_generation);
	ASSERT(/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj == arg);
#else
	stackMachine->code[next_free_spot_for_code_generation].op	= operation;
#endif


	if (operation == instruction::LD_ARG)
	{
		if (stackMachine->indef)
		{
#if defined(___vec_)
			/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj	= (long) arg.sym->stack_offset;
#else
			stackMachine->code[next_free_spot_for_code_generation].obj	= (long) arg.sym->stack_offset;
#endif
		}
		else
		{
#if defined(___vec_)
			/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj.sym		= arg.sym;
#else
			stackMachine->code[next_free_spot_for_code_generation].obj.sym		= arg.sym;
#endif
		}
	}
	else if (operation == instruction::STORE_ARG)
	{
		if (stackMachine->indef)
		{
#if defined(___vec_)
			/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj	= (long) arg.sym->stack_offset;
#else
			stackMachine->code[next_free_spot_for_code_generation].obj	= (long) arg.sym->stack_offset;
#endif
		}
		else
		{
#if defined(___vec_)
			/*stackMachine->*/codeVec[next_free_spot_for_code_generation].obj.sym		= arg.sym;
#else
			stackMachine->code[next_free_spot_for_code_generation].obj.sym		= arg.sym;
#endif
		}
	}
		else
		{
#if defined(___vec_)
#else
	stackMachine->code[next_free_spot_for_code_generation].obj	= arg;
#endif
		}

	++next_free_spot_for_code_generation;
	stackMachine->last_code_generated	= stackMachine->next_free_spot_for_code_generation-1;
	return stackMachine->last_code_generated;// last code generated
}
////////////////CStackMachine::///////////////////////////////////////////////////////////////
//int gen_call(symbol* sym, const CCommon& argListCount)
int SolveIt::gen_call(symbol* sym, const objects& argListCount)
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	gen_code( instruction::I_PUSH, argListCount );
	return gen_code(instruction::CALL,	sym );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//auto_ptr<char> SolveIt::NLtoCrNL(const char*buf)
char* SolveIt::NLtoCrNL(const char*buf)
{
//	if ( buf == 0 ) return 0;
	if ( buf == 0 ) throw SolveIt_ERROR("NLtoCrNL: null");//return 0;
	DWORD length = static_cast<DWORD>(strlen(buf));
	if ( length == 0 ) throw SolveIt_ERROR("NLtoCrNL: 0");//return 0;

	
//	auto_ptr<char> abuf2(new char[2*(length+2)]);
	char* buf2 = new char[2*(length+2)];//abuf2.get();
	ZeroMemory(buf2, 2*(length+2)*sizeof(buf2[0]));
	char* b = buf2;
	for (unsigned int j=0; j<length; ++j)
	{
		*b++ = buf[j];
		if (b[-1] == '\n')
		{
			if (b-2 && b[-2] != '\r')
			{
				b[-1] = '\r';
				*b++ = '\n';
			}
		}
	}
	return buf2;
}
//////////////////////////////////////////////////////////////////////

//extern auto_ptr<yyFlexLexer> lexer;
extern unsigned int field_count;
///////////////////////////////////////////////////////////////////////////////
bool CStackMachine::Compile_and_Run(const string& in, string& out)
{
	try {
//	reset_cycle();
//	char rebuf[8192];//static 
	ZeroMemory(rebuf,sizeof(rebuf));
//	auto_ptr<ostrstream> a(new ostrstream(rebuf, sizeof(rebuf)/sizeof(rebuf[0])));
//	possyyFlexLexer = a;

//	possyyFlexLexer = new ostrstream(rebuf, sizeof(rebuf)/sizeof(rebuf[0]));

	possyyFlexLexer.seekp(0);

//	auto_ptr<yyFlexLexer> b(new yyFlexLexer(&iss, possyyFlexLexer.get()));
//	lexer = b;
//	if (lexer.get() == NULL )
//	{
//		return false;
//	}


//MyLexer lexer(&iss, possyyFlexLexer.get());
//ParserExecutionContext parser_context(lexer);
//
	yyscan_t scanner;
	YY_BUFFER_STATE buf;
	const std::wstring input = ToWideString(in.c_str());
	yylex_init( &scanner );
////	yytext[yyleng-1] = ' ';
	buf = yy_scan_string(input.c_str(), scanner);
//	yylex( scanner );
//

	errors = 0;
	field_count	= 0;
//	bool bDoReset=false;


	while (field_count < in.size())
	{
		reset_cycle();
		yyparse (scanner); 
//		yyparse (); 
//		yyparse((void*)&parser_context);
		if ( errors == 0 )
		{
//			if (CSolveItApp::g_boolDoReset) bDoReset = true;

#if defined(___vec_)
//			fetch_execute_cycle(&codeVec.front(), progbase);
//			fetch_execute_cycle(codeVec, progbase);
			fetch_execute_cycle(progbase);
#else
			fetch_execute_cycle(progbase);
#endif
		}
		else
		{
			possyyFlexLexer << ends;//not required since 'ZeroMemory(rebuf,sizeof(rebuf));' called
			_bstr_t bbuf(L"ERROR:\r\n");
//			auto_ptr<char> b = NLtoCrNL(rebuf);
//			_bstr_t brebuf(b.get());
			char* b = NLtoCrNL(rebuf);
			_bstr_t brebuf(b);
			bbuf += brebuf;
			out = (const char*)bbuf;
			delete b;
			return false;
		}
	}
	yy_delete_buffer(buf,scanner);
	yylex_destroy( scanner );


//	if (bDoReset)
//	{
//		CSystem* m_system = theApp.GetSystem();
//		m_system->Initialize();
//	}


	{
//		_bstr_t brebuf(rebuf);
//		if (_tcslen(rebuf) == 0) brebuf = L"OK\r\n";
		if (strlen(rebuf) == 0) out = receiveCommandResponse;
		else out = (const char*) rebuf;

		receiveCommandResponse = "OK\r\n";
	}
		receiveCommandResponse = "OK\r\n";
	return true;
	}

#define ERR_RET(x)			_bstr_t brebuf(GetWC(x));\
							brebuf += L"\r\n";\
		out = (const char*)brebuf;\
		receiveCommandResponse = "OK\r\n";\
		return true;

	catch (std::runtime_error* exc)
	{
	//	AfxMessageBox(exc->what());
		ERR_RET(exc->what())
	}
	catch(struct _exception *except)
	{
	//	AfxMessageBox(except->name);
		ERR_RET(emap::_emap.OnError(except))
	}
	catch (char * str)
	{
	//	AfxMessageBox(str);
		ERR_RET(str)
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	//	AfxMessageBox("SolveIt::OnCompile: Unknown error.");
		ERR_RET("SolveIt::OnCompile: Unknown error.")
	}
#undef ERR_RET
	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CStackMachine::Compile_and_Run_and_check_for_reset(const string& in, string& out)
{
	CSolveItApp::g_boolDoReset	= false;
	bool bParseCompleted = Compile_and_Run( in, out);
	if (CSolveItApp::g_boolDoReset)
	{
	//	CSystem* m_system = theApp.GetSystem();
	//	m_system->Initialize();
//WARNING: do not programmatically send 'reset' because it will not execute immediately
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
		pFrame->PostMessage(WM_USER_Initialize, 0, 0);//
//		pFrame->SendMessage(WM_USER_Initialize, 0, 0);//
	}
	return bParseCompleted;
}

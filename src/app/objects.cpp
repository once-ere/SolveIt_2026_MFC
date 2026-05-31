// objects.cpp: implementation of the objects class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"

#include "objects.h"
#include "symbol.h"
//#include "parser_2020.h"// /* for token definitions and yylval */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
	wstring last_evaled_function_name;

	bool expand_re_im=true;
	bool is_inevalf=false;

	auto RUNTIME_ERROR = [](auto const& x)/*->(char const*)*//*->void*/ {return std::move( std::runtime_error(GetC(x.c_str())) ); };
	void settypeerr(){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Bad Argument Type"));
	}

	void setsizeerr(){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Bad Argument Value"));
	}

	void setdimerr(){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Invalid dimension"));
	}

	void settypeerr(const wstring & s){
		throw(RUNTIME_ERROR(s+L" Error: Bad Argument Type"));
	}

	void setsizeerr(const wstring & s){
		throw(RUNTIME_ERROR(s+L" Error: Bad Argument Value"));
	}

	void setdimerr(const wstring & s){
		throw(RUNTIME_ERROR(s+L" Error: Invalid dimension"));
	}

	void divisionby0err(const objects & e){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Division of " + e.print()+ L" by 0"));
	}

	void cksignerr(const objects & e){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Unable to check sign: "+e.print()));
	}

	void invalidserieserr(const wstring & s){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Invalid series expansion: "+s));
	}

	void toofewargs(const wstring & s){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Too few arguments: "+s));
	}

	void toomanyargs(const wstring & s){
		throw(RUNTIME_ERROR(last_evaled_function_name+L"Too many arguments: "+s));
	}



/*
	objects chartab2gen(wchar_t * & s){
		objects res;
		// 
		// initialize as a null __int
		// type = __int;
		// iVal = 0;
		if (!*s)
			return res;
		if (s[0]=='#') {
			if (s[1]=='x')
	s[0]='0';
			if (s[1]=='o'){
	s[0]='0';
	s[1]='0';
			}
		}
		errno = 0;
		wchar_t * endchar;
		int ll=strtol(s,&endchar,0);
		if (*endchar) {// non numeric -> use parser
			double d=strtod(s,&endchar);
			if (*endchar)
			return objects(wstring(s));
			return objects(d);
		}
		if (!errno)
			return objects ( int(ll));
		int l =strlen(s);
		// check if a non 0-9 wchar_t is there
		for (int i=0;i<l;++i){
			if ((s[i]<'0') || (s[i]>'9')){
	return objects(wstring(s));
			}
		}
		int maxsize = 5 + (s[0]=='-');
		if (l<maxsize){
			res.type=__int;
			res.iVal = atoi(s);
			return res;
		}
	}

	objects string2gen(const wstring & ss,bool remove_ss_quotes=true){
		objects res;
		res.type=__string;
		res.ref_count = new int(1);
		if (remove_ss_quotes)
			res.sPtr = new wstring(ss.substr(1,ss.size()-2));
		else
			res.sPtr = new wstring(ss);
		return res;
	}



	int try_parse(){
			int res;
		try {
			res=giac_yyparse();
		}
		catch (std::runtime_error & error){
				cerr << "Error " << error.what() << endl;
				messages_to_print += wstring(error.what()) + '\n';
			return 1;
		}
		return res;
	}

	objects::objects(const complex<double> & c) {
		
		type=__complex;
		3;
		cPtr = new objects[2];
		* cPtr = real(c);
		++cPtr;
		* cPtr = imag(c);
		--cPtr;
		ref_count = new int(1);		
	}
*/


}//end namespace SolveIt

objects::objects(const objects & e):ref_count(0) { 
	try {
	// if (e.type==debugtype)
	// cout << e << "Construct @ " << *e.debugtypeptr << "[" << *e.ref_count <<"+1]" <<endl;
	type=e.type;
	if (!type){
		iVal=e.iVal;
		return;
	}
	switch (type) {
		case __long:
			lVal=e.lVal;
			break;
		case __int:
			iVal=e.iVal;
			break;
		case __double:
			dVal=e.dVal;
			break;
		case __complex: 
//			ref_count=e.ref_count;
//			(*ref_count)++;
//			cPtr=e.cPtr;
			break; 

		case __F_0:		fn=e.fn;
			break;
		case __F_L:		fnL=e.fnL;
			break;
		case __F_D:		fnX=e.fnX;
			break;
		case __F_DD:	fnXY=e.fnXY;
			break;
		case __F_DDD:	fnXYZ=e.fnXYZ;
			break;
		case __F_LD:	fnLX=e.fnLX;
			break;
		case __F_DL:	fnXL=e.fnXL;
			break;
		
		case __objectPtr:
			oPtr=e.oPtr;
			break;
		case __instructionPtr:
			iPtr=e.iPtr;
			break;
		case __symbolPtr:
			sym=e.sym;
			break;

		case __RigidBodyPtr: 
			ref_count=e.ref_count;
			(*ref_count)++;
			rbPtr=e.rbPtr;
			break; 

	//	case __symbolPtr:
	//		ref_count=e.ref_count;
	//		(*ref_count)++;
	//		sym=e.sym;
	//		break;
		case __vector3: 
			ref_count=e.ref_count;
			(*ref_count)++;
			vPtr=e.vPtr;
			break; 
		case __point3: 
			ref_count=e.ref_count;
			(*ref_count)++;
			ptPtr=e.ptPtr;
			break;

		case __vector4: 
			ref_count=e.ref_count;
			(*ref_count)++;
			v4Ptr=e.v4Ptr;
			break; 
		case __vector5: 
			ref_count=e.ref_count;
			(*ref_count)++;
			v5Ptr=e.v5Ptr;
			break; 
		case __vector6: 
			ref_count=e.ref_count;
			(*ref_count)++;
			v6Ptr=e.v6Ptr;
			break; 


		case __string:
			ref_count=e.ref_count;
			(*ref_count)++;
			sPtr=e.sPtr;
			break; 
		default: 
			settypeerr(L"Gen constructor");
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects:: ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"objects::objects");
	}
}
///////////////////////////////////////////////////////////////////////////////
	// if (type==debugtype)
	// cout << *this << " ~ Delete @ " << *debugtypeptr << "[" << *ref_count <<"-1]" << endl;
objects::~objects()
{
	try {
	ReleaseRef();
/*
	if (ref_count){
		(*ref_count)--;
		if (!*ref_count){
			delete ref_count;
			ref_count=0;
			switch (type) {
				case __complex: 
			//		delete [] cPtr;
					break; 
				case __symbolPtr: 
//		delete sym;
					break;
				case __vector3: 
					if (vPtr) delete vPtr;
					vPtr = 0;
				break;
				case __string:
					if (sPtr) delete sPtr;
					sPtr = 0;
				//	free(sPtr);
					break;
				case __RigidBodyPtr: 
					if (rbPtr) delete rbPtr;
					rbPtr = 0;
				break;
				default: 
					settypeerr("Gen Destructor");
			}
		}
	}
*/

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects:: ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox(L"objects::~objects");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
objects& objects::operator=(CRigidBody* a)
{
	try {
	*this=objects(a);
	return *this;

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects:: ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"objects::operator=(CRigidBody*");
	}
	return *this;
}
///////////////////////////////////////////////////////////////////////////////
objects::objects(CRigidBody* a):
	ref_count(new unsigned int(1)),	type(__RigidBodyPtr),	rbPtr(a) 
{
	try {
	*ref_count=2;

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects:: ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"objects::objects(CRigidBody*");
	}
}
///////////////////////////////////////////////////////////////////////////////
extern FILE *stream_stderr;
///////////////////////////////////////////////////////////////////////////////
int objects::ReleaseRef()
{
	try {
	if (ref_count)
	{
		--(*ref_count);
		if (*ref_count <= 0)
		{
			ASSERT(*ref_count == 0);
			if (*ref_count < 0) throw SolveIt_ERROR(L"*ref_count < 0");
			delete ref_count;
			ref_count=0;
			switch (type) {
				case __complex: 
			//		delete [] cPtr;
					break; 
				case __symbolPtr: 
//		delete sym;
					break;
				case __vector3: 
					if (vPtr) delete vPtr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__vector3]");
					vPtr = 0;
				break;
				case __point3: 
					if (ptPtr) delete ptPtr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__point3]");
					ptPtr = 0;
				break;

				case __vector4: 
					if (v4Ptr) delete v4Ptr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__vector4]");
					v4Ptr = 0;
				break;
				case __vector5: 
					if (v5Ptr) delete v5Ptr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__vector5]");
					v5Ptr = 0;
				break;
				case __vector6: 
					if (v6Ptr) delete v6Ptr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__vector6]");
					v6Ptr = 0;
				break;


				case __string:
					if (sPtr) delete sPtr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__string]");
					sPtr = 0;
				//	free(sPtr);
					break;
				case __RigidBodyPtr: 
					if (rbPtr) delete rbPtr;
			else throw SolveIt_ERROR(L"objects::ReleaseRef [__RigidBodyPtr]");
					rbPtr = 0;
				break;
				default:
					throw SolveIt_ERROR(L"objects::ReleaseRef [default]");
				//	settypeerr("Gen Destructor");
			}
		}
	}
	if (ref_count) return (*ref_count);
	return 0;

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects:: ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		::MessageBox(0,L"unknown error",  L"objects::ReleaseRef", MB_OK);
		static long int c=0;
		if (stream_stderr) fprintf(stream_stderr, "%s%u", "objects::ReleaseRef: unknown error ", ++c);
	//	cerr << "objects::ReleaseRef: unknown error" << endl;
	//	AfxMessageBox(L"objects::ReleaseRef");
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/*
 * The following values are non-zero, constant, odd, large, and atypical
 *      Non-zero values help find bugs assuming zero filled data.
 *      Constant values are good so that memory filling is deterministic
 *          (to help make bugs reproducable).  Of course it is bad if
 *          the constant filling of weird values masks a bug.
 *      Mathematically odd numbers are good for finding bugs assuming a cleared
 *          lower bit.
 *      Large numbers (byte values at least) are less typical, and are good
 *          at finding bad addresses.
 *      Atypical values (i.e. not too often) are good since they typically
 *          cause early detection in code.
 *      For the case of no-man's land and free blocks, if you store to any
 *          of these locations, the memory integrity checker will detect it.
 */

/* static unsigned wchar_t _bNoMansLandFill = 0xFD;   fill no-man's land with this */
/* static unsigned wchar_t _bDeadLandFill   = 0xDD;   fill free objects with this */
/* static unsigned wchar_t _bCleanLandFill  = 0xCD;   fill new objects with this */
/* static unsigned wchar_t _bAlignLandFill  = 0xBD;   fill no-man's land for
												   aligned routines */
/*
the 
Windows debug C runtime fills the memory formerly occupied by the deleted 
host instances with 0xfeeefeee for just such occasions where future code 
might accidentally refer to deleted hosts.

the list isn't being 
copied to its new memory




Microsoft's memory management functions often initialize memory with special values. The following article describes frequent used variants. 

Microsoft Visual C++ Runtime library
C runtime library provides it own debug codes:

0xCD, 0xCDCDCDCD - New objects. New objects are filled with 0xCD when they are allocated.
0xFD, 0xFDFDFDFD - No-man's land memory. Extra bytes that belong to the internal block allocated, but not the block you requested. They are placed before and after requested blocks and used for data bound checking.
0xDD, 0xDDDDDDDD - Freed blocks. The freed blocks kept unused in the debug heap's linked list when the _CRTDBG_DELAY_FREE_MEM_DF flag is set are currently filled with 0xDD. Although in some cases you won't see magic 0xDDDDDDDD value, as it will be overwritten by another debug function (e.g. 0xFEEEFEEE for HeapFree).

These constants are defined in DbgHeap.c file as

	

  
Compiler initialisations
0xCC, 0xCCCCCCCC - The /GX Microsoft Visual C++ compiler option initialises all local variables not explicitly initialised by the program. It fills all memory used by these variables with 0xCC, 0xCCCCCCCC. 


Windows NT memory codes
0xABABABAB - Memory following a block allocated by LocalAlloc(). 
0xBAADF00D - "Bad Food". This is memory allocated via LocalAlloc( LMEM_FIXED, ... ). 
It is memory that has been allocated but not yet written to.

0xFEEEFEEE - OS fill heap memory, which was marked for usage, 
but wasn't allocated by HeapAlloc() or LocalAlloc(). 
Or that memory just has been freed by HeapFree(). 
	
*/
///////////////////////////////////////////////////////////////////////////////
objects & objects::operator = (const objects & a)
{

	try {
	ASSERT(ref_count != reinterpret_cast<unsigned int *>(static_cast<UINT_PTR>(0xFEEEFEEEu)));
	if (ref_count && ref_count==a.ref_count) return *this;
// Copy before deleting because the target might be embedded in a
// with a ref_count of a equals to 1
	unsigned int * ref_count_save=ref_count;
	ref_count=0;
	dataTypes type_save=type;
	void * ptr_save = vPtr;
	type=a.type;
// if (type==debugtype)
// cout << a << "Overwriting by = " << *a.debugtypeptr << "[" << *(a.ref_count) << "+1]"<< endl ;
	switch (type) {
		case __int:
			iVal=a.iVal;
			break;
		case __long:
			lVal=a.lVal;
			break;

		case __F_0:		fn=a.fn;
			break;
		case __F_L:		fnL=a.fnL;
			break;
		case __F_D:		fnX=a.fnX;
			break;
		case __F_DD:	fnXY=a.fnXY;
			break;
		case __F_DDD:	fnXYZ=a.fnXYZ;
			break;
		case __F_LD:	fnLX=a.fnLX;
			break;
		case __F_DL:	fnXL=a.fnXL;
			break;
		
		case __double:
			dVal=a.dVal;
			break;
		case __complex: 
		//	ref_count=a.ref_count;
		//	(*ref_count)++;
		//	cPtr=a.cPtr;
			break;
		
		case __objectPtr:
			oPtr=a.oPtr;
			break;
		case __instructionPtr:
			iPtr=a.iPtr;
			break;
		
		case __symbolPtr: 
		//	ref_count=a.ref_count;
		//	if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) []");
		//	(*ref_count)++;
			sym=a.sym;
			break;
		case __vector3: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__vector3]");
			(*ref_count)++;
			vPtr=a.vPtr;
			break;
		case __point3: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__point3]");
			(*ref_count)++;
			ptPtr=a.ptPtr;
			break;

		case __vector4: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__vector4]");
			(*ref_count)++;
			v4Ptr=a.v4Ptr;
			break; 
		case __vector5: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__vector5]");
			(*ref_count)++;
			v5Ptr=a.v5Ptr;
			break; 
		case __vector6: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__vector6]");
			(*ref_count)++;
			v6Ptr=a.v6Ptr;
			break; 

		case __string: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__string]");
			(*ref_count)++;
			sPtr=a.sPtr;
			break;
		case __RigidBodyPtr: 
			ref_count=a.ref_count;
			if (ref_count == NULL) throw SolveIt_ERROR(L"objects::operator = (const objects &) [__RigidBodyPtr]");
			(*ref_count)++;
//	hack moved to		case instruction::STORE:
//			if (__double==type_save==__symbolPtr)
//			{
//				a.rbPtr->name = sym->name;
//			}
			rbPtr=a.rbPtr;

			break;

		default: 
			break ;
		}
		// Now we delete the copied object
		if (ref_count_save){
			// if (type==debugtype)
			// cout << *this << " Destruction by = of " << *debugtypeptr << "[" << *ref_count << "-1]" << endl;
			(*ref_count_save)--;
			if (!*ref_count_save){
	delete ref_count_save;
	switch (type_save) {
	case __complex: 
//		delete (complex<double> *) ptr_save ;
		break; 
	case __symbolPtr: 
	//	delete (identificateur *) ptr_save ; []
		break;
	case __vector3: 
	try {
		if ( (Vector3D *) ptr_save != NULL)
		delete (Vector3D *) ptr_save;
}catch(...) {AfxMessageBox(L"objects::operator = (const objects &__vector3");}
		break;
	case __point3: 
	try {
		if ( (Point3D *) ptr_save != NULL)
		delete (Point3D *) ptr_save;
}catch(...) {AfxMessageBox(L"objects::operator = (const objects &__point3");}
		break;

	case __vector4: 
		if ( (VectorND<4> *) ptr_save != NULL)
		delete (VectorND<4> *) ptr_save;
		break;
	case __vector5: 
		if ( (VectorND<5> *) ptr_save != NULL)
		delete (VectorND<5> *) ptr_save;
		break;
	case __vector6: 
		if ( (VectorND<6> *) ptr_save != NULL)
		delete (VectorND<6> *) ptr_save;
		break;


	case __string:
	try {
		if ( (wstring *) ptr_save != NULL)
		delete (wstring *) ptr_save;
	//	delete (wchar_t *) ptr_save;
}catch(...) {AfxMessageBox(L"objects::operator = (const objects &__string");}
		break;
	case __RigidBodyPtr: 
	try {
		if ( (CRigidBody *) ptr_save != NULL)
		delete (CRigidBody *) ptr_save;
}catch(...) {AfxMessageBox(L"objects::operator = (const objects &__RigidBodyPtr");}
		break;
	default: 
		settypeerr(L"Gen Operator =");
	}
			}
	}

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"objects::operator = (const objects & ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"objects::operator = (const objects &");
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////
wostream& SolveIt::operator<<(wostream& os, const objects& a)
{
	try {
		if (a.type == __int)	return os << a.iVal;
		else
		if (a.type == __long)	return os << a.lVal;
		else
		if (a.type == __double)	return os << a.dVal;
//		else
//		if (a.type == __complex)	return os << *a.cPtr;
		else
		if (a.type == __vector3)	return os << *a.vPtr;
		else
		if (a.type == __point3)	return os << *a.ptPtr;
		else
		if (a.type == __string)	return os << a.sPtr->c_str();
		else
		if (a.type == __symbolPtr)	return os << GetWC(a.sym->data.print());
		else
		if (a.type == __RigidBodyPtr)	return os << *a.rbPtr;
		return os << L"undefined";

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"wostream& SolveIt::operator<<");
	}
	return os;
}

// FetchExecute.cpp: implementation of the CStackMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "StackMachine.h"
#include "symbol.h"
#include "MainFrm.h"
#include "Matherror.h"
#include "parser.h"
#include "RigidBody/RigidBodyManager.h"
#include "Frame.h"
#include "RigidBody/ConstraintManager.h"
#include "GLView.h"
#include "System.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char CrLf[] = "\r\n";
static const objects true_object(T_TRUE);//(static_cast<int>(T_TRUE)); 
static const objects false_object(T_FALSE);//(static_cast<int>(T_FALSE)); 
///////////////////////////////////////////////////////////////////////////////
// frame
_Frame Frame;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CStackMachine::fetch_execute_cycle(int nCode)//, objects* stack)
{
	try
	{
		if (codePtr == NULL)
		{
			return;
			throw SolveIt_ERROR(L"CStackMachine::fetch_execute_cycle:codePtr == NULL ");
		}
	}
	catch( SolveIt_Error& )
	{
	//	e.why();
		throw SolveIt_ERROR(L"CStackMachine::fetch_execute_cycle:codePtr == NULL ");
		return;
	}
	try
	{
	++fetch_execute_cycle_Level;
	instruction	ir;
	int L = -1;

		ASSERT (nCode >= 0 && nCode <= NPROG);
		if (nCode < 0 || nCode > NPROG)
		{
//			if (possyyFlexLexer.get() != 0) {
//				possyyFlexLexer << "fetch_execute_cycle ERROR: nCode = " << nCode << CrLf;
//				throw (possyyFlexLexer).rdbuf();//if (bort++ > 3) 
//			}
			return;
		}
		pc = nCode;
	do
	{
		ir = codePtr[pc++];
		switch (ir.op)
		{
			case instruction::HALT: //cout <<	"halt\n";
				break;
			case instruction::READ_NUM: 
			case instruction::READ_VAR: 
			case instruction::READ_ARG: 
				{
				//	cout <<	"Input: ";
					wchar_t buf[1024];
					if (wscanf_s(L"%1023ls", buf, static_cast<unsigned>(_countof(buf))) != 1)
					{
						buf[0] = L'\0';
					}
					/*istrstream*/ std::wstringstream iss(buf, sizeof(buf)/sizeof(buf[0]));
					switch (ir.op)
					{
					case instruction::READ_NUM: 
						iss >> stack[ir.obj.sym->indexInDataArray];
						break;
					case instruction::READ_VAR: 
						iss >> data[ir.obj.sym->indexInDataArray];
						break;
					case instruction::READ_ARG: 
						iss >> stack[Frame.top().top+Frame.top().nargs-ir.obj.sym->stack_offset];
						break;
					}
				}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::WRITE_Object:
				{
					//if (possyyFlexLexer.get() == 0) return;
					if (top <= NPROG)
					possyyFlexLexer << GetC(stack[top++].print()) << CrLf;// << ends;
				}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::WRITE_external_int:
				{
					receiveStackTopLongValue = stack[top++];
				//	*ir.obj.oPtr = stack[top++];
				}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::WRITE_external_double:
				{
					receiveStackTopDoubleValue = stack[top++];
				//	*ir.obj.oPtr = stack[top++];
				}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::WRITE_external_Vector3D:
				{
					receiveStackTopVector3DValue = stack[top++];
				//	*ir.obj.oPtr = stack[top++];
				}
				break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_POP: 
				stack[top++] = 0;
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_PUSH: 
				stack[--top] = ir.obj;
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_VAR:
			//	ASSERT (&data[ir.obj.sym->indexInDataArray] == &ir.obj.sym->data);
			//	stack[--top] = data[ir.obj.sym->indexInDataArray];//obj storage for VAR
				stack[--top] = ir.obj.sym->data;//obj storage for VAR and SYSTEM
				if (ir.obj.sym->type == T_VAR)
				{
					ASSERT (&data[ir.obj.sym->indexInDataArray] == &ir.obj.sym->data);
				}
				break;
///////////////////////////////////////////////////////////////////////////////
//#define addr (f.top+f.nargs-ir.obj.sym->indexInDataArray)
			case instruction::LD_ARG:
				{
//since local symbols and args are destroyed after compilation of user function
// 'sym->stack_offset' no longer exists
					frame& f	= Frame.top();
					long addr	= f.top+f.nvargs-ir.obj.lVal;//sym->stack_offset;
					stack[--top] = stack[addr];
				}
				break;
////////////////////////////////////////////////////////////////////////////////
			case instruction::STORE:
			if (stack[top].type == __RigidBodyPtr && stack[top].rbPtr->type==T_VECTOR_ANALYSIS_OBJECT)
			{
// name should already agree (stack[top].rbPtr->name == ir.obj.sym->name)
// except for vectorAnalysisObjects
				stack[top].rbPtr->name = GetWC(ir.obj.sym->name);
			}
				data[ir.obj.sym->indexInDataArray] = stack[top];//stack[top++];
				break;

//////////////////////////////////////////////////////////////////////////////
			case instruction::STORE_ARG:
				{
//since local symbols and args are destroyed after compilation of user function
// 'sym->stack_offset' no longer exists
					frame& f = Frame.top();
					long addr	= f.top+f.nvargs-ir.obj.lVal;//.sym->stack_offset;
					stack[addr] = stack[top];//stack[top++];
				}
				break;
//#undef addr
///////////////////////////////////////////////////////////////////////////////
			case instruction::SYSTEM_SET: 
			{
				objects debug = stack[top++];
				objects val = stack[top++];
				objects obj = stack[top++];
				objects enm = stack[top++];
				unsigned int i = DecodeString(ToNarrowString(enm.sPtr->c_str()));
				CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
				CSystem* m_system = theApp.GetSystem();
				m_system->Set(obj, i, val);
			}
				break;
			case instruction::SYSTEM_GET:
			{
				objects debug = stack[top++];
				objects obj = stack[top++];
				objects enm = stack[top++];
				unsigned int i = DecodeString(ToNarrowString(enm.sPtr->c_str()));
				objects val;
				CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
				CSystem* m_system = theApp.GetSystem();
				m_system->Get(obj, i, val);
				stack[--top] = val;
			}
				break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_VECTOR3:
			{
				double z	= stack[top++];
				double y	= stack[top++];
				double x	= stack[top++];
				stack[--top] = objects( Vector3D(x,y,z) );
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_VECTOR4:
			{
				double x4	= stack[top++];
				double x3	= stack[top++];
				double x2	= stack[top++];
				double x1	= stack[top++];
				stack[--top] = objects( VectorND<4>(x1,x2,x3,x4) );
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_VECTOR5:
			{
				double x5	= stack[top++];
				double x4	= stack[top++];
				double x3	= stack[top++];
				double x2	= stack[top++];
				double x1	= stack[top++];
				stack[--top] = objects( VectorND<5>(x1,x2,x3,x4,x5) );
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_VECTOR6:
			{
				double x6	= stack[top++];
				double x5	= stack[top++];
				double x4	= stack[top++];
				double x3	= stack[top++];
				double x2	= stack[top++];
				double x1	= stack[top++];
				stack[--top] = objects( VectorND<6>(x1,x2,x3,x4,x5,x6) );
			}
				break;
/*
*/
///////////////////////////////////////////////////////////////////////////////
			case instruction::STORE_VEC:
		//		ir.obj.sym->type = VECTOR3;
				data[ir.obj.sym->indexInDataArray] = stack[top];//stack[top++];
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::LD_PLOT:
			{
				const wchar_t* options	= stack[top++];
				const wchar_t* name	= stack[top++];

				CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
				CSystem* m_system = theApp.GetSystem();
				m_system->Plot(GetC(name), GetC(options));
//				free((void *)name);
//				free((void *)options);
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_LT:	if ( stack[top+1] < stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;
			case instruction::I_LE:	if ( stack[top+1] <= stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;
			case instruction::I_EQ:	if ( stack[top+1] == stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;
			case instruction::I_NEQ:	if ( stack[top+1] != stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;
			case instruction::I_GT:	if ( stack[top+1] > stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;
			case instruction::I_GE:	if ( stack[top+1] >= stack[top] ) stack[++top] = 1;
						else stack[++top] = 0;
				break;



			case instruction::ADD: stack[top+1] = stack[top+1] + stack[top];
				top++;
				break;
			case instruction::SUB: stack[top+1] = stack[top+1] - stack[top];
				top++;
				break;
			case instruction::MULT: 
				stack[top+1] = stack[top+1] * stack[top];
				top++;
				break;
			case instruction::DIV: stack[top+1] = stack[top+1] / stack[top];
				top++;
				break;
			case instruction::PWR: stack[top+1] = stack[top+1] ^ stack[top];//Power(stack[top+1] , stack[top]);
				top++;
				break;
			case instruction::MOD: stack[top+1] = stack[top+1] % stack[top];
				top++;
				break;


			case instruction::I_ABS: stack[top] = stack[top].norm();
				break;


			case instruction::I_AND: stack[top+1] = stack[top+1] && stack[top];
				top++;
				break;
			case instruction::I_OR: stack[top+1] = stack[top+1] || stack[top];
				top++;
				break;





			case instruction::I_NOT: 
				stack[top] = ! stack[top];
				break;
			
			
			case instruction::NEG: 
				stack[top] = -stack[top];
				break;
			
			case instruction::XPOP: 
				++top;
				break;

///////////////////////////////////////////////////////////////////////////////
			case instruction::I_IF:
			{
				int savedpc = pc;	//* then part *
				fetch_execute_cycle(pc+3);//condition
				if (stack[top++].True())
				{
					L = codePtr[savedpc].obj.iVal;//.p_counter;
					if (L==0) throw SolveIt_ERROR(L"total 'if' failure");
					fetch_execute_cycle( L );
				}
				else //** else part? L == -1 if 'else' omitted
				{
					const int no_else_part = -1;
					if (codePtr[savedpc+1].obj.iVal != no_else_part)
					{
					L = codePtr[savedpc+1].obj.iVal;//.p_counter;
					if ( (L>=0)&&(L<=NPROG) )//check for else part
						fetch_execute_cycle( L );
					}
				}
				if (!returning)
			// next stmt		pc = savedpc+2; // next stmt
					pc = codePtr[savedpc+2].obj.iVal;//.p_counter; 
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_WHILE: 
			{
				int savedpc = pc;
				fetch_execute_cycle(pc+2);//* condition *
			//	objects d = ;d
				while (stack[top++].True())
				{
					L = codePtr[savedpc].obj.iVal;//.p_counter;// body
					fetch_execute_cycle( L );
					if (returning) break;
					fetch_execute_cycle(savedpc+2);//* condition *
			//		d = stack[top++];
				}
				if (!returning)
			// next stmt		pc = savedpc+1; // next stmt **
					pc = codePtr[savedpc+1].obj.iVal;//.p_counter; 
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_FOR: 
			{
				int savedpc = pc;
				fetch_execute_cycle(pc+4);// precharge **
				++top;
				L = codePtr[savedpc].obj.iVal;//.p_counter;// condition
				fetch_execute_cycle( L );
				while (stack[top++].True())
				{
					L = codePtr[savedpc+2].obj.iVal;//.p_counter;// body
					fetch_execute_cycle( L );
					if (returning) break;
					L = codePtr[savedpc+1].obj.iVal;//.p_counter;// post loop
					fetch_execute_cycle( L );
					++top;
					L = codePtr[savedpc].obj.iVal;//.p_counter;// condition
					fetch_execute_cycle( L );
				}
				if (!returning)
			// next stmt		pc = savedpc+3; // next stmt
					pc = codePtr[savedpc+3].obj.iVal;//.p_counter; 
			}
				break;
///////////////////////////////////////////////////////////////////////////////
			case instruction::CALL:
				{
					long nargs			= (long)(stack[top++].lVal);
					if (nargs != ir.obj.sym->nargs) throw SolveIt_ERROR(L"ERROR: wrong number of arguments in function call");
					long nlvars		= ir.obj.sym->nvargs - nargs;		//# function  local vars
					for (long j = 0; j < nlvars; ++j)
					{
						stack[--top] = 0;//reserve space on stack for local vars
					}
					Frame.push_back(frame(codePtr, pc, top, nargs, ir.obj.sym->nvargs));
					codePtr						= ir.obj.sym->func;//.get();

			//		 int indexInDataArray	= ir.obj.sym->indexInDataArray;
			//		for ( int j = 0; j < nargs; ++j)
			//		{
			//			data[indexInDataArray+nargs-j] = stack[top+j];//set objects type; leave stack intact
			//	//		data[indexInDataArray+nargs-j] = &stack[top+j];
			//		}
					fetch_execute_cycle(0);
					returning	= 0;
				}
				break;

			case instruction::DO_RETURN:
				{
					returning	= fetch_execute_cycle_Level;
					objects res	 = stack[top++];
					frame f		= Frame.pop();//back();
					codePtr		= f.codePtr;
					pc			= f.pc;
					long nargs	= f.nargs;//# function arguments
					long nvargs	= f.nvargs;//# function arguments and local vars

					for (long j = 0; j < nvargs; j++) ++top;/* pop arguments */

					instruction ir = codePtr[pc-1];
					 int indexInDataArray	= ir.obj.sym->indexInDataArray;
					ir.obj.sym->data = res;//	data[indexInDataArray] = res;
					stack[--top] = res;
				}
				break;

///////////////////////////////////////////////////////////////////////////////
			case instruction::CALL_VOID: 
				stack[--top] = (*ir.obj.fn)();
				break;
			case instruction::CALL_L: 
				stack[top] = (*ir.obj.fnL)(static_cast<int>(stack[top].Long()));
				break;
			case instruction::CALL_D: 
				stack[top] = (*ir.obj.fnX)(stack[top]);
				break;
			case instruction::CALL_DD: stack[top+1] = (*ir.obj.fnXY)(stack[top+1], stack[top]);
				top++;
				break;
			case instruction::CALL_LD: stack[top+1] = (*ir.obj.fnLX)(static_cast<int>(stack[top+1].Long()), stack[top]);
				top++;
				break;
			case instruction::CALL_DL: stack[top+1] = (*ir.obj.fnXL)(stack[top+1], static_cast<long>(stack[top].Long()));
				top++;
				break;
			case instruction::CALL_DDD: stack[top+2] = (*ir.obj.fnXYZ)(stack[top+2], stack[top+1], stack[top]);
				top++;
				top++;
				break;

///////////////////////////////////////////////////////////////////////////////
			case instruction::Create_Body:
			{
				ASSERT (&data[ir.obj.sym->indexInDataArray] == &ir.obj.sym->data);
				ASSERT (ir.obj.type == __symbolPtr);
				ASSERT (ir.obj.sym->data.type == __double);
				objects bodyType = stack[top++];
			//	objects& body = ir.obj;
				objects& body = ir.obj.sym->data;
			//	body.sym->data.type	= __RigidBodyPtr;

			//	body = CRigidBody(ir.obj.sym->name, bodyType);

				body = new CRigidBody(ToWideString(ir.obj.sym->name.c_str()), static_cast<int>(bodyType.Long()));

				ASSERT (body.type == __RigidBodyPtr);
				CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
				rigidBodyManager->add(body);
				CSystem* m_system = theApp.GetSystem();
				m_system->m_bDoRigidBody	= true;
				stack[--top] = body;//.rbPtr->name;
			}
				break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
				
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_Constraint:
			{
				CSystem* m_system = theApp.GetSystem();
				CConstraintManager* constraintManager	= theApp.GetConstraintManager();
				objects body1			= stack[top++];
				ASSERT (body1.type == __RigidBodyPtr);
				if (body1.type != __RigidBodyPtr) throw SolveIt_ERROR(L"error: body 1");
				const int constraintKind = static_cast<int>(ir.obj.Long());
				if (constraintKind < 4)
				{
				objects body2;
				objects options;
				switch (constraintKind)
				{
					case 0:
					{
					}
						break;
					case 1:		
					{
						body2			= stack[top++];
						ASSERT (body2.type == __RigidBodyPtr);
						if (body2.type != __RigidBodyPtr) throw SolveIt_ERROR(L"error: body 2");
					}
						break;
					case 2:
					{
						options			= stack[top++];
					}
						break;
					case 3:		
					{
						body2			= stack[top++];
						options			= stack[top++];
						ASSERT (body2.type == __RigidBodyPtr);
						if (body2.type != __RigidBodyPtr) throw SolveIt_ERROR(L"error: body 2");
					}
						break;
				}
				objects separation	= stack[top++];
				objects body2Pt		= stack[top++];
				objects body1Pt		= stack[top++];
		//		ASSERT (separation.type == __double || separation.type == __int);
		//		ASSERT (body2Pt.type == __vector3);
		//		ASSERT (body1Pt.type == __vector3);
		//		if (body1Pt.type != __vector3) throw SolveIt_ERROR(L"error: body1Pt");
		//		if (body2Pt.type != __vector3) throw SolveIt_ERROR(L"error: body2Pt");
		//		if (separation.type == __double || separation.type == __int) throw SolveIt_ERROR(L"error: separation");
				Point3D point1(*body1Pt.vPtr);
				Point3D point2(*body2Pt.vPtr);
					switch (constraintKind)
				{
					case 0:
					{
						constraintManager->AddTwoPointConstraint(
													body1.rbPtr,
													point1,
													0,
													point2,
													separation
													);
					}
						break;
					case 1:		
					{
						constraintManager->AddTwoPointConstraint(
													body1.rbPtr,
													point1,
													body2.rbPtr,
													point2,
													separation
													);
					}
						break;
					case 2:
					{
						constraintManager->AddTwoPointConstraint(
													body1.rbPtr,
													point1,
													0,
													point2,
													separation,
													options.sPtr
													);
					}
						break;
					case 3:		
					{
						constraintManager->AddTwoPointConstraint(
													body1.rbPtr,
													point1,
													body2.rbPtr,
													point2,
													separation,
													options.sPtr
													);
					}
						break;
				}
			}
			else
			if (constraintKind == 4)//T_Linear
			{
// normal*body1.rbPtr->x + scalar == 0
				objects point	= stack[top++];
				objects normal	= stack[top++];
//				ASSERT (normal.type == __vector3);
//				if (normal.type != __vector3) throw SolveIt_ERROR(L"error: normal");
				Point3D pointInPlane(*point.vPtr);
				constraintManager->AddLinearConstraint(
													body1.rbPtr,
													normal,
													pointInPlane
													);
			}
			else
			if (constraintKind == 5)//T_Quadratic
			{
//normal2*[body1.rbPtr->x.x^2, body1.rbPtr->x.y^2, body1.rbPtr->x.z^2] + normal1*body1.rbPtr->x + scalar == 0
				objects boolean	= stack[top++];
				objects scalar	= stack[top++];
				objects normal1	= stack[top++];
				objects normal2	= stack[top++];
//				ASSERT (normal1.type == __vector3);
//				if (normal1.type != __vector3) throw SolveIt_ERROR(L"error: normal1");
//				ASSERT (normal2.type == __vector3);
//				if (normal2.type != __vector3) throw SolveIt_ERROR(L"error: normal2");
				bool bNoNegateGradientForNormal = true;
				if (boolean == true_object)		bNoNegateGradientForNormal = true;
				if (boolean == false_object)	bNoNegateGradientForNormal = false;
				constraintManager->AddQuadraticConstraint(
													body1.rbPtr,
													normal2,
													normal1,
													scalar,
													bNoNegateGradientForNormal
													);
			}
///////////////////////////////////////////////////////////////////////////////
//			CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//				pFrame->CreateOrActivateFrame(theApp.m_pGLViewTemplate, RUNTIME_CLASS(CGLView));
//				CGLView* pGLView = theApp.GetGLView();
//	pGLView->SetFocus();//calls OnReset(WPARAM wParam, LPARAM lParam)
//	pGLView->ResetView();//calls OnReset(WPARAM wParam, LPARAM lParam)
				m_system->UpdateTree();
//				pGLView->SetPerspectiveOrFrustum();
				Fire_ViewChange();

			//	stack[--top] = body1;
			}
				break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
				
///////////////////////////////////////////////////////////////////////////////
			case instruction::I_CONNECT:
			{
				objects body1			= stack[top++];
				ASSERT (body1.type == __RigidBodyPtr);
				if (body1.type != __RigidBodyPtr) throw SolveIt_ERROR(L"error: body 1");
				objects body2;
				const int connectKind = static_cast<int>(ir.obj.Long());
				switch (connectKind)
				{
					case 0:
					{
					}
						break;
					case 1:		
					{
						body2			= stack[top++];
						ASSERT (body2.type == __RigidBodyPtr);
						if (body2.type != __RigidBodyPtr) throw SolveIt_ERROR(L"error: body 2");
					}
						break;
				}
				objects jointAxis		= stack[top++];
				objects JointType		= stack[top++];
				objects body2JointLoc	= stack[top++];
				objects body1JointLoc	= stack[top++];
				ASSERT (jointAxis.type == __vector3);
				ASSERT (body2JointLoc.type == __vector3);
				ASSERT (body1JointLoc.type == __vector3);
				if (jointAxis.type != __vector3) throw SolveIt_ERROR(L"error: jointAxis");
				if (body2JointLoc.type != __vector3) throw SolveIt_ERROR(L"error: JointLoc 2");
				if (body1JointLoc.type != __vector3) throw SolveIt_ERROR(L"error: JointLoc 1");
				CSystem* m_system = theApp.GetSystem();
				m_system->m_bDoMultiBody	= true;
				Point3D point1(body1JointLoc);
				Point3D point2(body2JointLoc);
				Joint1DOF::JointType jt = Joint1DOF::NONE;
				switch (static_cast<int>(JointType.Long()))
				{
					case T_JointType_FIXED:			jt = Joint1DOF::FIX;break;
					case T_JointType_REVOLUTE:		jt = Joint1DOF::REV;break;
					case T_JointType_PRISMATIC:		jt = Joint1DOF::PRI;break;
					case T_JointType_SCREW:			jt = Joint1DOF::SCR;break;
					case T_JointType_CYLINDRICAL:	jt = Joint1DOF::CYL;break;
					case T_JointType_UNIVERSAL:		jt = Joint1DOF::UNI;break;
					case T_JointType_SPHERICAL:		jt = Joint1DOF::SPH;break;
					case T_JointType_FLOATING:		jt = Joint1DOF::FLT;break;
					case T_JointType_SPRING:		jt = Joint1DOF::SPRING;break;
				}
				switch (connectKind)
				{
					case 0:
					{
						body1.rbPtr->connect(
								point1,
								0,
								point2,
								jt, jointAxis
						);
					}
						break;
					case 1:		
					{
						body1.rbPtr->connect(
								point1,
								body2.rbPtr,
								point2,
								jt, jointAxis
						);
					}
						break;
				}
				m_system->UpdateTree();
				Fire_ViewChange();
			//	stack[--top] = body1;
			}
				break;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


			default:
				returning	= fetch_execute_cycle_Level;
//				if (possyyFlexLexer.get() != 0)
					possyyFlexLexer << "Error: Instruction = " << ir.op << CrLf; 
//				else throw SolveIt_ERROR(L"Internal Error: Memory Dump");
				break;
		}
		if (top < 0 || top > NPROG)
		{
//			if (possyyFlexLexer.get() != 0) 
			possyyFlexLexer << "stack Error: top= " << top  << CrLf; 
//			else throw SolveIt_ERROR(L"Unknown error.");
		}
	}
	while ((top>=0) && (top <= NPROG) && ir.op != instruction::HALT  && !returning);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch (std::runtime_error& exc)
	{
		returning	= 1;
//		if (possyyFlexLexer.get() != 0)
			{possyyFlexLexer << "Error: "   << exc.what() << endl; return;}
//		else throw exc;
	}
	catch (std::runtime_error* exc)
	{
		returning	= 1;
//		if (possyyFlexLexer.get() != 0)
			{possyyFlexLexer << "Error: "   << exc->what() << endl; return;}
//		else throw exc;
	}
	catch(struct _exception *except)
	{
		returning	= 1;
//		if (possyyFlexLexer.get() != 0)
		{possyyFlexLexer << "Error: " << ( GetC( emap::_emap.OnError(except).c_str() ) ) << "; "  << except->name << endl; return;}
//		else throw except;
	}
	catch (wchar_t * str)
	{
		returning	= 1;
//		if (possyyFlexLexer.get() != 0)
		{possyyFlexLexer << "Error: "  << GetC(str) << endl; throw str;}
//		else throw str;
	}
	catch(...)
	{
		returning	= 1;
//		if (possyyFlexLexer.get() != 0)
		{possyyFlexLexer << "Unknown error." << endl; throw (possyyFlexLexer).rdbuf();}
//		else throw SolveIt_ERROR(L"Unknown error.")SolveIt_ERROR(;
	}

	--fetch_execute_cycle_Level;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int get_hash(const int numAgsThatFollow , ...)
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

// System.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "System.h"

#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "parser.h" /* for token definitions and yylval */

#include "LeftView.h"
#include "SolveItDoc.h"
#include "SolveItView.h"
#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
//#include "Worker.h"
#include "Cruncher.h"
#include "GLView.h"
#include "PlayMap.h"
#include "QM/qm_1dscatt.h"
#include "QM/qm_2dscatt.h"
#include "QM/qm_ground_mc_1d.h"
#include "QM/qmstationary1d.h"
#include "QM/DataQM_Scatt1D.h"
#include "QM/DataQM_Scatt2D.h"
#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Stationary2D.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "RigidBody/shapes.h"
#include "RigidBody/ConstraintManager.h"
#include "RigidBody/ExamineCollisionTimes.h"
#include "CalcMap.h"
#include <functional>
#include <algorithm>
#include "QM/QMStationary2D.h"

#include "Examples/Superposition/Superposition.h"
#include "Examples/Superposition/WaveNumbers.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern GL2PScontext *gl2ps;
extern Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
///////////////////////////////////////////////////////////////////////////////
//instruction* CSystem::UserDefinedPotentialAddress	= 0;
//unsigned long	CSystem::indexInDataArray			= 0;
//unsigned long	CSystem::nvargs						= 0;
//string CSystem::UserDefinedPotentialName("");
//symbol* CSystem::g_UserDefinedPotential_symb		= 0;
///////////////////////////////////////////////////////////////////////////////
//const char __xStr[]="%g�R";
//const char __yStr[]="%g�V";
///////////////////////////////////////////////////////////////////////////////
const char __xStr[]="x-axis";
const char __yStr[]="y-axis";
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
//CWorker*	CSystem::worker=0;

symbol* ____s =0;


CSystem::CSystem() :
	stackMachine(new CStackMachine),
	symbolTable(new CSymbolTable),
	rigidBodyManager(new CRigidBodyManager),
	fieldsManager(new CFieldsManager),
	constraintManager(new CConstraintManager),

	data__QM_Scatt1D(new CDataQM_Scatt1D),
	data__QM_Scatt2D(new CDataQM_Scatt2D),
//	data__QM_Stationary1D(new CDataQM_Stationary1D),
	data__QM_Stationary2D(new CDataQM_Stationary2D),
	m_pCQMStationary1D(0),
///////////////////////////////////////////////////////////////////////////////
	eigenvalueIndex(0),
	M(8+1./3.),
	energy_eigenvalue(0),
///////////////////////////////////////////////////////////////////////////////
	m_pCSuperposition(0),
	m_pCWaveNumbers(0),
///////////////////////////////////////////////////////////////////////////////

	m_NumberVariables(0),
	m_SolveMethod(direct),
//	eigenvalueIndex(0),(0),
//	energy_eigenvalue(0),
//	m_fWaveNumber(0),
//	m_fScaleWave(theApp.GetGLView()->m_fScaleWave),
	m_domainClass(infinite),
	m_worldDimension(three),
///////////////////////////////////////////////////////////////////////////////
	m__t(0.0),
	m_dt(0.01),
	m_fStopTime(-1),
	m_stepForwardOrBackward(1),
	m_lTakeNumSteps(0),
	m_fScale(1),
	workingDirectory("./"),
///////////////////////////////////////////////////////////////////////////////
	worker(0),
	player(0),
	mapper(0),

//	m_pCQMStationary1D(0),
//	m_pCQMStationary2D(0),
//	m_pCQM_Scatt1D(0),
	m_pCQM_Scatt2D(0),
	m_pCQM_Ground_MC_1D(0),
	m_nThreadType(thread_RigidBody_time_step),
	examineCollisionTimes(0),
//	hWndThread(0),
//	thread_handle(0),
//	thread_tid(0),
//	g_EventHandle(0),
//	g_EventHaltThread(0),
//	g_EventThreadHalted(0),
	m_bStopIterations(true),
/////////////////////////////////////////////////////////////////////////////
	m_bSingleStepping(false),
	m_bSingleStep_Mode(false),
	m_bRunning(false),
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	m_bDoVerhulstDynamics(false),
	m_bDoMapper(false),
	m_bDoRigidBody(false),
	m_bDoMultiBody(false),
	m_bDoQMStationary1D(false),
	m_bDoQMStationary2D(false),
	m_bDoQM_Scatt1D(false),
	m_bDoQM_Scatt2D(false),
	m_bDoQM_Ground_MC_1D(false),
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	m_nEnforceRigidBoundary(rigidBoundary_off),
	m_fRigidBoundary_top(1),
	m_fRigidBoundary_bottom(-1),
	m_fRigidBoundary_left(-1),
	m_fRigidBoundary_right(1),
	m_fRigidBoundary_front(1),
	m_fRigidBoundary_back(-1),
///////////////////////////////////////////////////////////////////////////////
	m_bCollisionDetectionEnabled(false),
	m_bAllowRestingContacts(false),
	m_fTimeOfLastCollision(-1),
	m_fToleranceTimeOfCollision(0.1),
	m_lMaxIterationsForFindingTimeOfCollision(1024),
/////////////////////////////////////////////////////////////////////////////

	m_xStr(__xStr),
	m_yStr(__yStr),
//	m_tic_mark_spacingStr("-0.12"),

	UserDefinedPotentialAddress(0),
	indexInDataArray(0),
	nvargs(-1),
	nargs(-1),
	UserDefinedPotentialName(""),
	g_UserDefinedPotential_symb(0)
{
	doit.clear();
	____s = symbolTable->new_symbol("system", false);
	ASSERT(____s->type	== T_SYSTEM);
	CRigidBody::m_boolSpheresOnly = true;
	SolveIt::g_EventHandle		= 0;
//	CreateWorkerThread(true);
//	if (worker == NULL) throw SolveIt_ERROR("worker == NULL");
}

CSystem::~CSystem()
{
	Stop();
	KillWorker();
	StopQMStationary1D();

	delete stackMachine;
	delete symbolTable;

//	if (____s) delete ____s;
	____s=0;

//	if (m_pCQMStationary2D) delete m_pCQMStationary2D;
	delete data__QM_Scatt1D;
	delete data__QM_Scatt2D;
//	delete data__QM_Stationary1D;
	delete data__QM_Stationary2D;
	delete constraintManager;
	delete rigidBodyManager;
	delete fieldsManager;


///////////////////////////////////////////////////////////////////////////////
	if (m_pCWaveNumbers)
	{
		SendMessage(m_pCWaveNumbers->m_hWnd, WM_CLOSE, 0, 0);//WM_CWaveNumbers_Cancel
	}
	if (m_pCSuperposition)		delete m_pCSuperposition;
	m_pCSuperposition	= 0;
	m_pCWaveNumbers		= 0;
///////////////////////////////////////////////////////////////////////////////

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CSystem::SetUserDefinedName(const string& name, _FunctionFor _for)
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
	if (selected == 0) return false;
	CSymbolTable* symbolTable	= theApp.GetSymbolTable();
	const symbol*const  sym	= symbolTable->getsym (name);
	if (sym == 0) return false;
	selected->func					= sym->func;
	selected->indexInDataArray		= sym->indexInDataArray;
	selected->nvargs				= sym->nvargs;
	selected->nargs					= sym->nargs;
	selected->name					= ToWideString(name.c_str());
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool CSystem::SetUserDefinedName(const char* name, _FunctionFor _for)
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
	if (selected == 0) return false;
	CSymbolTable* symbolTable	= theApp.GetSymbolTable();
	symbol* sym	= symbolTable->getsym (name);
	if (sym == 0) return false;
	selected->func					= sym->func;
	selected->indexInDataArray		= sym->indexInDataArray;
	selected->nvargs				= sym->nvargs;
	selected->nargs					= sym->nargs;
	selected->name					= ToWideString(name);
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool CSystem::SetUserDefined_IsTimeDependent(const bool val, _FunctionFor _for)
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
	if (selected == 0) return false;
	selected->m_bIsTimeDependent	= val;
	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
struct _state_data
{
	instruction* code;
	objects* objects_stack;
	objects* objects_data;
	int  errors;
	int progbase;
	instruction* codePtr;
	int fetch_execute_cycle_Level;
	int returning;
	int	top;
	int	pc;
	int indexInDataArray;
	int nInstructionCounter;
} state_data;
///////////////////////////////////////////////////////////////////////////////
void CSystem::SaveState()
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	state_data.top							= stackMachine->top;
	state_data.pc							= stackMachine->pc;
	state_data.indexInDataArray				= stackMachine->indexInDataArray;
	state_data.progbase						= stackMachine->progbase;
	state_data.codePtr						= stackMachine->codePtr;

	state_data.code				= new instruction[stackMachine->next_free_spot_for_code_generation];
	state_data.objects_stack	= new objects[NPROG+1-stackMachine->top];
	state_data.objects_data		= new objects[stackMachine->indexInDataArray];
//	CopyMemory(state_data.code			, stackMachine->code,		stackMachine->next_free_spot_for_code_generation*sizeof(instruction));
//	CopyMemory(state_data.objects_stack	, stackMachine->stack+stackMachine->top,	(NPROG+2-stackMachine->top)*	sizeof(objects));
//	CopyMemory(state_data.objects_data	, stackMachine->data,		(stackMachine->indexInDataArray)*sizeof(objects));
	for (int j=0; j<stackMachine->next_free_spot_for_code_generation; ++j)	state_data.code[j]			= stackMachine->code[j];
	for ( j=0; j<NPROG+1-stackMachine->top; ++j)			state_data.objects_stack[j]	= stackMachine->stack[j+stackMachine->top];
	for ( j=0; j<stackMachine->indexInDataArray; ++j)		state_data.objects_data[j]	= stackMachine->data[j];

	state_data.errors						= stackMachine->errors;
	state_data.fetch_execute_cycle_Level	= stackMachine->fetch_execute_cycle_Level;
	state_data.returning					= stackMachine->returning;

	state_data.nInstructionCounter			= stackMachine->next_free_spot_for_code_generation;
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::RestoreState()
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->indexInDataArray				= state_data.indexInDataArray;
	stackMachine->next_free_spot_for_code_generation			= state_data.nInstructionCounter;
	stackMachine->top							= state_data.top;
	stackMachine->pc							= state_data.pc;
	stackMachine->progbase						= state_data.progbase;
	stackMachine->codePtr						= state_data.codePtr;
	stackMachine->errors						= state_data.errors;
	stackMachine->fetch_execute_cycle_Level	= state_data.fetch_execute_cycle_Level;
	stackMachine->returning					= state_data.returning;

//	CopyMemory(stackMachine->code,					state_data.code,	stackMachine->next_free_spot_for_code_generation*sizeof(instruction));
//	CopyMemory(stackMachine->stack+stackMachine->top	,state_data.objects_stack ,	(NPROG+1-stackMachine->top)*	sizeof(objects));
//	CopyMemory(stackMachine->data,					state_data.objects_data,	(stackMachine->indexInDataArray)*sizeof(objects));
	for (int j=0; j<stackMachine->next_free_spot_for_code_generation; ++j)
	{
		stackMachine->code[j]	= state_data.code[j];
		state_data.code[j].obj	= 0;
	}
	for ( j=0; j<NPROG+1-stackMachine->top; ++j)
	{
		stackMachine->stack[j+stackMachine->top]	= state_data.objects_stack[j];
		state_data.objects_stack[j]	= 0;
	}
	for ( j=0; j<stackMachine->indexInDataArray; ++j)
	{
		stackMachine->data[j]	= state_data.objects_data[j];
		state_data.objects_data[j]	= 0;
	}

	delete[]state_data.objects_data;
	delete[]state_data.objects_stack;
	delete[]state_data.code;
	state_data.code			= 0;
	state_data.objects_stack= 0;
	state_data.objects_data	= 0;
}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::UpdateTree()
{
	CLeftView* tree = theApp.GetTreeView();
	tree->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::DisplayData()
{
	CLeftView& tree = *theApp.GetTreeView();

	char buf[128];
	ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	oss.seekp(0);
	_bstr_t bstr;
/*
	if (m_pCQM_1DScatt)
	{
		oss << m_pCQM_1DScatt->m_pCView->m_timeOfThisFrame << ends;
		bstr = buf;
		_bstr_t bstrt(L"Frame ");
		bstrt += bstr;
		tree.GetTreeCtrl().SetItemText(tree.m_htSystemTime, bstrt);
	}
	else
	if (m_pCQM_2DScatt)
	{
		oss << m_pCQM_2DScatt->m_pCView->m_timeOfThisFrame << ends;
		bstr = buf;
		_bstr_t bstrt(L"Frame ");
		bstrt += bstr;
		tree.GetTreeCtrl().SetItemText(tree.m_htSystemTime, bstrt);
	}
	else
*/
	{
		oss << m__t << ends;
		bstr = buf;
		_bstr_t bstrt(L"Time ");
		bstrt += bstr;
		tree.GetTreeCtrl().SetItemText(tree.m_htSystemTime, bstrt);
	}


	if (rigidBodyManager->empty()) return;
	{
//	Lock lock (theApp.protectBodies);
	oss.seekp(0);
	oss << rigidBodyManager->ComputeMomentum() << ends;
	bstr = buf;
	_bstr_t bstrp(L"Linear Momentum ");
	bstrp += bstr;
	tree.GetTreeCtrl().SetItemText(tree.m_htSystemLinearMomentum, bstrp);

	oss.seekp(0);
	oss << rigidBodyManager->ComputeAngularMomentum() << ends;
	bstr = buf;
	_bstr_t bstrL(L"Angular Momentum ");
	bstrL += bstr;
	tree.GetTreeCtrl().SetItemText(tree.m_htSystemAngularMomentum, bstrL);

	oss.seekp(0);
	oss << rigidBodyManager->ComputeEnergy() << ends;
	bstr = buf;
	_bstr_t bstrE(L"Energy ");
	bstrE += bstr;
	tree.GetTreeCtrl().SetItemText(tree.m_htSystemEnergy, bstrE);
	}

	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::Clear()
{
	m_DeferredCommands.clear();
	m_ResetDeferredCommands.clear();
	i_DeferredCommands = m_DeferredCommands.begin();

//	m_CleanupOnInitialize.Clear();

//	rigidBodyManager->Clear();
	fieldsManager->Clear();
//	theConstraintManager.Clear();
	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSystem::Reset()
{
	m__t					= 0;
	m_stepForwardOrBackward	= 1;
	m_lTakeNumSteps			= 0;
	fieldsManager->Clear();
	rigidBodyManager->Reset();
	data__QM_Scatt1D->Reset();
//	data__QM_Scatt2D->Reset();
//	data__QM_Stationary1D->Reset();
//	data__QM_Stationary2D->Reset();

	sort(m_DeferredCommands.begin(),m_DeferredCommands.end(), timeCompare);
	i_DeferredCommands = m_DeferredCommands.begin();
	DoResetDeferredCommands();
	return;
try{
	Stop();
	KillWorker();
	Clear();
	doit.clear();
#if 0
///////////////////////////////////////////////////////////////////////////////
	rigidBodyManager->Initialize();
	fieldsManager->Initialize();
//	theConstraintManager.Initialize();
//	rigidBodyManager->Clear();
///////////////////////////////////////////////////////////////////////////////
	m_fScale	= 1;
///////////////////////////////////////////////////////////////////////////////
	Vector2D pt(0.0, 0.0);
	m_CurrentMousePosition	= pt;
	m_CurrentWorldPosition	= pt;
///////////////////////////////////////////////////////////////////////////////
	m_bDoVerhulstDynamics	= false;
	m_bDoRigidBody			= false;
	m_bDoMultiBody			= false;
	m_bDoQMStationary1D		= false;
	m_bDoQMStationary2D		= false;
	m_bDoQM_Scatt1D			= false;
	m_bDoQM_Scatt2D			= false;
	m_bDoQM_Ground_MC_1D	= false;
///////////////////////////////////////////////////////////////////////////////
#endif
	stackMachine->Clear();
	CStackMachine::indexInDataArray				= 0;	// index into objects data[]; for VAR (ARG)
	CStackMachine::local_variables_data_offset	= 0;	// offset from end of data[] for local vars in user funcs
	delete symbolTable;
	delete stackMachine;
	stackMachine=new CStackMachine;
	symbolTable=new CSymbolTable;
///////////////////////////////////////////////////////////////////////////////
	symbol* s = symbolTable->new_symbol("system", false);
	ASSERT(s->type	== T_SYSTEM);

#if 0
	delete rigidBodyManager;
	delete fieldsManager;
	rigidBodyManager=new CRigidBodyManager;
	fieldsManager=new CFieldsManager;
#endif
	m_NumberVariables=0;
	m_SolveMethod=direct;
	m_worldDimension=three;
///////////////////////////////////////////////////////////////////////////////
	m__t=0.0;
	m_dt=0.01;
	m_fStopTime=-1;
	m_stepForwardOrBackward=1;
	m_fScale=1;
	workingDirectory="./";
///////////////////////////////////////////////////////////////////////////////
	worker=0;
	player=0;
	mapper=0;
//	m_pCQMStationary1D=0;
//	m_pCQMStationary2D=0;
//	m_pCQM_Scatt1D=0;
	m_pCQM_Scatt2D=0;
	m_pCQM_Ground_MC_1D=0;
	m_nThreadType=thread_RigidBody_time_step;
//	hWndThread=0;
//	thread_handle=0;
//	thread_tid=0;
//	g_EventHandle=0;
//	g_EventHaltThread=0;
//	g_EventThreadHalted=0;
	m_bStopIterations=false;
///////////////////////////////////////////////////////////////////////////////
	m_bDoVerhulstDynamics=false;
	m_bDoRigidBody=false;
	m_bDoMultiBody=false;
	m_bDoQMStationary1D=false;
	m_bDoQMStationary2D=false;
	m_bDoQM_Scatt1D=false;
	m_bDoQM_Scatt2D=false;
	m_bDoQM_Ground_MC_1D=false;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	m_nEnforceRigidBoundary=rigidBoundary_off;
	m_fRigidBoundary_top=1;
	m_fRigidBoundary_bottom=-1;
	m_fRigidBoundary_left=-1;
	m_fRigidBoundary_right=1;
	m_fRigidBoundary_front=1;
	m_fRigidBoundary_back=-1;
///////////////////////////////////////////////////////////////////////////////
	m_bCollisionDetectionEnabled=false;
	m_bAllowRestingContacts=false;
	m_fTimeOfLastCollision=-1;
	m_fToleranceTimeOfCollision=0.1;
	m_lMaxIterationsForFindingTimeOfCollision=1024;
/////////////////////////////////////////////////////////////////////////////

	m_xStr=__xStr;
	m_yStr=__yStr;
//	m_tic_mark_spacingStr="-0.12";

	UserDefinedPotentialAddress=0;
	indexInDataArray=0;
	nvargs=-1;
	nargs=-1;
	UserDefinedPotentialName="";
	g_UserDefinedPotential_symb=0;


	DoResetDeferredCommands();

	CGLView* gl = theApp.GetGLView();
	if (gl==NULL) return;
	gl->Initialize();
	gl->Invalidate();
}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		ASSERT(0);
	}
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// declare a comparison function object, to pass to sort and search algorithms
TimeCompare SolveIt::timeCompare;  
/////////////////////////////////////////////////////////////////////////////
void CSystem::SetDeferredCommand(double time, const string& line) {
	DeferredCommand dc;
	dc.time		= time;
	dc.command = line.c_str();
	m_DeferredCommands.push_back(dc);
	sort(m_DeferredCommands.begin(),m_DeferredCommands.end(), timeCompare);
	i_DeferredCommands = m_DeferredCommands.begin();
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::SetResetDeferredCommand(double time, const string& line) {
	DeferredCommand dc;
	dc.time		= time;
	dc.command	= line.c_str();
	m_ResetDeferredCommands.push_back(dc);
	sort(m_ResetDeferredCommands.begin(),m_ResetDeferredCommands.end(), timeCompare);
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::DoResetDeferredCommands()
{
//	CSolveItView& edit = *theApp.GetEditView();
	sort(m_ResetDeferredCommands.begin(),m_ResetDeferredCommands.end(), timeCompare);
	if (!m_ResetDeferredCommands.empty())
	{
		DeferredCommandList::iterator i_ResetDeferredCommands = m_ResetDeferredCommands.begin();
		if (i_ResetDeferredCommands != m_ResetDeferredCommands.end())
		{
			while (	i_ResetDeferredCommands != m_ResetDeferredCommands.end() &&
					(m_stepForwardOrBackward<0.0|| m__t >= i_ResetDeferredCommands->time)
				)
			{
				_bstr_t bstr(i_ResetDeferredCommands->command.c_str());
			//	edit.WriteCommand(bstr);
				Fire_WriteCommand(bstr);
				++i_ResetDeferredCommands;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::DoDeferredCommands(double t) {
//	CSolveItView& edit = *theApp.GetEditView();
	if (m_stepForwardOrBackward==1.0 && !m_DeferredCommands.empty())
	if (i_DeferredCommands != m_DeferredCommands.end())
	{
		while (	i_DeferredCommands != m_DeferredCommands.end() &&
				m__t >= i_DeferredCommands->time
				)
		{
			_bstr_t bstr(i_DeferredCommands->command.c_str());
		//	edit.WriteCommand(bstr);
			Fire_WriteCommand(bstr);
			++i_DeferredCommands;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

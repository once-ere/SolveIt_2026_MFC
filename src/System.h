// System.h: interface for the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEM_H__878F9B3E_96D6_4599_B528_B50AB8DABB48__INCLUDED_)
#define AFX_SYSTEM_H__878F9B3E_96D6_4599_B528_B50AB8DABB48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

#include "Types.h"

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
struct DeferredCommand
{
	double time;
	string command;
	DeferredCommand() : time(0), command("")  {}
	DeferredCommand(double t, const string& cmd) : time(t), command(cmd)  {}
	DeferredCommand(const DeferredCommand& dc) : time(dc.time), command(dc.command)  {}
	DeferredCommand& operator= (const DeferredCommand& dc){time=dc.time, command=dc.command;return *this;}
};
typedef vector<DeferredCommand> DeferredCommandList;
///////////////////////////////////////////////////////////////////////////////
struct TimeCompare
{  
	bool operator()(const DeferredCommand& a, const DeferredCommand& b) const
	{
		return a.time < b.time;
	}
};

// declare a comparison function object, to pass to sort and search algorithms
extern TimeCompare timeCompare;  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//#include "StackDatum.h"

///////////////////////////////////////////////////////////////////////////////
class symbol;  
class objects;  
class CWorker;
class CSystem;
class CCruncher;
class CPlayMap;
class CCalcMap;

class CRigidBody;
class instruction;  
class CSymbolTable;
class CStackMachine;
class CFieldsManager;
class CODESolverThread;
class CRigidBodyManager;
class CConstraintManager;

class CDataQM_Scatt1D;
class CDataQM_Scatt2D;
class CDataQM_Stationary1D;
class CDataQM_Stationary2D;

class CQMStationary1D;
class CQMStationary2D;
class CQM_Scatt1D;
class CQM_Scatt2D;
class CQM_Ground_MC_1D;

class CSuperposition;
class CWaveNumbers;

class CExamineCollisionTimes;
///////////////////////////////////////////////////////////////////////////////
typedef list<CRigidBody *> RigidBodyList;
struct RigidBodyListCompareEqualToString;

///////////////////////////////////////////////////////////////////////////////
//typedef void (CSystem::*Do)(void) ;__stdcall 
typedef void (__stdcall *DoThis)(void);
typedef vector<DoThis> doVector;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CSystem  
{
public:
	CSystem(void);
	virtual ~CSystem(void);
///////////////////////////////////////////////////////////////////////////////
	void Set(		objects& obj, unsigned int _enum, const	objects& val);
	void Get(const	objects& obj, unsigned int _enum,		objects& val);
///////////////////////////////////////////////////////////////////////////////
	void DoIt(void);
//	CCleanupOnInitialize m_CleanupOnInitialize;
///////////////////////////////////////////////////////////////////////////////
	symbol* g_UserDefinedPotential_symb;
	instruction* UserDefinedPotentialAddress;
	unsigned long	indexInDataArray;
	unsigned long	nvargs;		//	if FUNCTION, # function arguments and local vars
	unsigned long	nargs;		//	if FUNCTION, # function arguments 
//	string UserDefinedPotentialName;
	string UserDefinedPotentialName;
	bool SetUserDefinedPotentialName(const char* name);
	bool SetUserDefinedPotential_IsTimeDependent(const bool val);
///////////////////////////////////////////////////////////////////////////////
	enum _FunctionFor {
		__External1,
		__External2,
		__External3,
		__Red,
		__Green,
		__Blue,
		__Potential
	};
///////////////////////////////////////////////////////////////////////////////
//warning:
// call according to V = GetUserDefinedPotential(__Potential, nargs, double( x_1 ), ... , double( x_nargs ));
//or stack will be corrupted
	double GetUserDefinedPotential(_FunctionFor _for, long nargs, ...);
///////////////////////////////////////////////////////////////////////////////
	double GetUserDefinedPotential(double _x, _FunctionFor _for);
	bool SetUserDefinedName(const char* name, _FunctionFor _for);
	bool SetUserDefinedName(const string& name, _FunctionFor _for);
	bool SetUserDefined_IsTimeDependent(const bool val, _FunctionFor _for);
///////////////////////////////////////////////////////////////////////////////
	_CallUserDefined m_uUserDefinedPotential;
	_CallUserDefined m_uUserDefinedRed;
	_CallUserDefined m_uUserDefinedGreen;
	_CallUserDefined m_uUserDefinedBlue;
	_CallUserDefined m_uUserDefinedExternal1;
	_CallUserDefined m_uUserDefinedExternal2;
	_CallUserDefined m_uUserDefinedExternal3;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool Plot(const char* name, const char* options);
//	static void __stdcall PerformPlot(void);
//	friend void __stdcall PerformPlot(void);
	void CreateContourPlotBitmap(void);
	int CreateTexture(void);
///////////////////////////////////////////////////////////////////////////////
// data
	string				name;	//	title of simulation
	CStackMachine*		stackMachine;
	CSymbolTable*		symbolTable;
	CRigidBodyManager*	rigidBodyManager;
	CFieldsManager*		fieldsManager;
	CConstraintManager*	constraintManager;

	CQMStationary1D*	m_pCQMStationary1D;

	CDataQM_Scatt1D*		data__QM_Scatt1D;
	CDataQM_Scatt2D*		data__QM_Scatt2D;
//	CDataQM_Stationary1D*	data__QM_Stationary1D;
	CDataQM_Stationary2D*	data__QM_Stationary2D;

	CSuperposition* m_pCSuperposition;
	CWaveNumbers* m_pCWaveNumbers;

//	string m_tic_mark_spacingStr;
	string m_xStr;
	string m_yStr;
	string m_zStr;
	string m_strDataFileName;
	string workingDirectory;
	doVector doit;
	Vector2D m_CurrentMousePosition;
	Vector2D m_CurrentWorldPosition;
///////////////////////////////////////////////////////////////////////////////
//	int MouseMove(  POINT& pt,  RECT& rc,  long attributes);
///////////////////////////////////////////////////////////////////////////////
	enum SolveMethod {
		direct,
		iterative,
		radial,
		pseudoCartesian,
		Cartesian,
		PeriodicCayley,
		Nash_Chen,
		Cayley
	} m_SolveMethod;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long eigenvalueIndex;
	double energy_eigenvalue;
	double M;
//	double	m_fWaveNumber;
//	double& m_fScaleWave;
///////////////////////////////////////////////////////////////////////////////
	enum SolveDomain {
		finite,
		infinite
	} m_domainClass;
///////////////////////////////////////////////////////////////////////////////
	struct Domain {
		double begin;
		double end;
	} m_domain[3];
///////////////////////////////////////////////////////////////////////////////
	enum WorldDimension {
		one=1,
		two=2,
		three=3
	} m_worldDimension;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	bool m_bRunning;//i.e., should timer be reset if m_bStopped == false?
//	bool m_bRunningDuringThreadStep;
	bool m_bStopped;
	bool m_bNeedRedraw;
	bool m_bStopIterations;
	long m_lTakeNumSteps;
/////////////////////////////////////////////////////////////////////////////
	bool m_bSingleStepping;
	bool m_bSingleStep_Mode;
	bool m_bRunning;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long m_NumberVariables;
//	CODESolverThread* m_threadTimeStep;
///////////////////////////////////////////////////////////////////////////////
	UINT m_TimerTimeStep;
	UINT m_uMilliseconds;
///////////////////////////////////////////////////////////////////////////////
	double m_fScale;//click then zoom by this factor
///////////////////////////////////////////////////////////////////////////////
	double m_dt;
	double m__t;
	double m_target_tout;
	double m_tout_at_time_of_collision;
	double m_stepForwardOrBackward;
	double m_fStopTime;
	double GetTargetTime()
	{
		return m_target_tout = m__t + m_dt*m_stepForwardOrBackward;
	}
///////////////////////////////////////////////////////////////////////////////
//collision detection
	void DisableCollisionDetection();
	void EnableCollisionDetection();
	bool m_bCollisionDetectionEnabled;
///////////////////////////////////////////////////////////////////////////////
// collision time
	double m_fTimeOfLastCollision;
	double m_fToleranceTimeOfCollision;
//if (fabs(t_out - t_in) < theSystem*->m_fToleranceTimeOfCollision*theSystem->m_dt ) break;
	vector<double> m_vecCollisionTimes;
	long m_lMaxIterationsForFindingTimeOfCollision;
///////////////////////////////////////////////////////////////////////////////
	bool m_bAllowRestingContacts;
///////////////////////////////////////////////////////////////////////////////
// collision time
//	auto_ptr<CExamineCollisionTimes> examineCollisionTimes;
	CExamineCollisionTimes* examineCollisionTimes;
	void DestroyExamineCollisionTimes();
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	DeferredCommandList m_DeferredCommands;
	DeferredCommandList m_ResetDeferredCommands;
	DeferredCommandList::iterator i_DeferredCommands;

	void SetDeferredCommand(double time, const string& line);
	void SetResetDeferredCommand(double time, const string& line);
	void DoDeferredCommands(double t);
	void DoResetDeferredCommands(void);
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	enum RigidBoundary {
		rigidBoundary_off,
		rigidBoundary_all_faces,
		rigidBoundary_bottom_face_only
	} m_nEnforceRigidBoundary;
	void EnforceRigidBoundary(RigidBoundary newValue) {
		m_nEnforceRigidBoundary = newValue;
	}
	double m_fRigidBoundary_top;
	double m_fRigidBoundary_bottom;
	double m_fRigidBoundary_left;
	double m_fRigidBoundary_right;
	double m_fRigidBoundary_front;
	double m_fRigidBoundary_back;
///////////////////////////////////////////////////////////////////////////////
	void PrepareForStep(void);
	void UpdateTree(void);
	void DisplayData(void);
	void Run(void);
	void SingleStep(void);
///////////////////////////////////////////////////////////////////////////////
	void Clear(void);
	void Reset(void);
///////////////////////////////////////////////////////////////////////////////
//	void CreateThread(void);
	void CreateWorkerThread(bool bCREATE_SUSPENDED = false);
	void CreatePlayerThread(bool bCREATE_SUSPENDED = false);
	void CreateMapperThread(bool bCREATE_SUSPENDED = false);
	void CreateQMStationary1DThread(bool bCREATE_SUSPENDED = false);
	void CreateQMStationary2DThread(bool bCREATE_SUSPENDED = false);
	void CreateQM_Ground_MC_1DThread(bool bCREATE_SUSPENDED = false);
	void CreateQM_Scatt1DThread(bool bCREATE_SUSPENDED = false);
	void CreateQM_Scatt2DThread(bool bCREATE_SUSPENDED = false);
	BOOL Stop(void);
	BOOL StopWorker(void);
	BOOL StopPlayer(void);
	BOOL StopMapper(void);
	BOOL StopQMStationary1D(void);
	BOOL StopQMStationary2D(void);
	BOOL StopQM_Ground_MC_1D(void);
	BOOL StopQM_Scatt1D(void);
	BOOL StopQM_Scatt2D(void);
///////////////////////////////////////////////////////////////////////////////
	BOOL KillWorker(void);
	BOOL KillPlayer(void);
	BOOL KillMapper(void);
	BOOL KillQMStationary1D(void);
	BOOL KillQMStationary2D(void);
	BOOL KillQM_Ground_MC_1D(void);
	BOOL KillQM_Scatt1D(void);
	BOOL KillQM_Scatt2D(void);
///////////////////////////////////////////////////////////////////////////////
//	Mutex protectBodies;
/////////////////////////////////////////////////////////////////////////////
	bool m_bDoVerhulstDynamics;
	bool m_bDoMapper;
	bool m_bDoRigidBody;
	bool m_bDoMultiBody;
	bool m_bDoQMStationary1D;
	bool m_bDoQMStationary2D;
	bool m_bDoQM_Scatt1D;
	bool m_bDoQM_Scatt2D;
	bool m_bDoQM_Ground_MC_1D;
/////////////////////////////////////////////////////////////////////////////
	CCruncher*			worker;
	CPlayMap*			player;
	CCalcMap*			mapper;
//	CQMStationary1D*	m_pCQMStationary1D;
//	CQMStationary2D*	m_pCQMStationary2D;
//	CQM_Scatt1D*		m_pCQM_Scatt1D;
	CQM_Scatt2D*		m_pCQM_Scatt2D;
	CQM_Ground_MC_1D*	m_pCQM_Ground_MC_1D;
///////////////////////////////////////////////////////////////////////////////
	enum ThreadType {
		thread_RigidBody_time_step,
		thread_PlayMap,
		thread_QMStationary1D,
		thread_QMStationary2D,
		thread_QM_Scatt1D,
		thread_QM_Scatt2D,
		thread_QM_Ground_MC_1D
	} m_nThreadType;
///////////////////////////////////////////////////////////////////////////////
	void KillThreads();
/////////////////////////////////////////////////////////////////////////////////
	void CreateThread(ThreadType threadType);
//	void KillThread(ThreadType threadType);
///////////////////////////////////////////////////////////////////////////////

//	DWORD Halt(void);
	void Halted(void);
///////////////////////////////////////////////////////////////////////////////
//	static HWND		hWndThread;
//	static HANDLE	thread_handle;	// thread handle
//	static DWORD	thread_tid;		// thread id
//	static HANDLE	g_EventHandle;
//	static HANDLE	g_EventHaltThread;
//	static bool m_bStopIterations;

///////////////////////////////////////////////////////////////////////////////
//	CWorker*			worker;
//	HWND	hWndThread;
//	HANDLE	thread_handle;	// thread handle
//	DWORD	thread_tid;		// thread id
//	HANDLE	g_EventHandle;
//	HANDLE	g_EventHaltThread;
//	HANDLE	g_EventThreadHalted;
///////////////////////////////////////////////////////////////////////////////
};



////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_SYSTEM_H__878F9B3E_96D6_4599_B528_B50AB8DABB48__INCLUDED_)

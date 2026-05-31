// FieldLineODE.h: interface for the CFieldLineODE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FieldLine_ODE_H__609BEC05_D544_11D3_A704_0050045B99C4__INCLUDED_)
#define FieldLine_ODE_H__609BEC05_D544_11D3_A704_0050045B99C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Types.h"
#include "Cruncher.h"


namespace SolveIt {

class CFieldsManager;

/////////////////////////////////////////////////////////////////////////////
class FieldLineElectric2D : public CWinThread 
{
//	friend CFieldsManager;
	DECLARE_DYNCREATE(FieldLineElectric2D) 
public: 
	bool m_bStepCompleted; 
	long CVodeStep(); 
	long MakeTimeStep(); 
	long StepMultibodyState(); 
	virtual BOOL InitInstance(); 
	virtual int ExitInstance(); 
protected: 
	FieldLineElectric2D():m_bStepCompleted(false){} 
	virtual ~FieldLineElectric2D(){} 
	afx_msg void OnStep(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP() 
};

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// threads
#define USER_THREAD(class_name) \
class class_name : public CWinThread \
{  \
	DECLARE_DYNCREATE(class_name) \
public: \
	bool m_bStepCompleted; \
	long CVodeStep(); \
	long MakeTimeStep(); \
	long StepMultibodyState(); \
	virtual BOOL InitInstance(); \
	virtual int ExitInstance(); \
protected: \
	class_name():m_bStepCompleted(false){} \
	virtual ~class_name(){} \
	afx_msg void OnStep(WPARAM wParam, LPARAM lParam); \
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam); \
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam); \
	DECLARE_MESSAGE_MAP() \
};

//AfxBeginThread( RUNTIME_CLASS( class_name ),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
///////////////////////////////////////////////////////////////////////////////
//USER_THREAD(FieldLineElectric2D)
USER_THREAD(FieldLineElectric3D)
//USER_THREAD(FieldLineMagnetic_current_loop)
//USER_THREAD(FieldLineMagnetic)
USER_THREAD(FieldLineMagnetostatic)
USER_THREAD(FieldLineGravitational)
USER_THREAD(FieldLine_Equipotential)
//


} // namespace SolveIt

#endif // !defined(AFX_ODESOLVER_H__609BEC05_D544_11D3_A704_0050045B99C4__INCLUDED_)

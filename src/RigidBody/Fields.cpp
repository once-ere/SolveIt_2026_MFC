// Fields.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Action.h"

#include "RigidBodyManager.h"
#include "FieldLineODE.h"
#include "System.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
void CSystem::FieldLine(ThreadType threadType) {
	CFieldLineODEThread** selectedThread = 0;
	switch (threadType)
	{
	//	case thread_time_step:selectedThread =  m_threadTimeStep;break;
		case thread_fieldline_gravitational:			selectedThread =  &m_threadFieldLineGravitational;break;
		case thread_fieldline_electrostatic:			selectedThread =  &m_threadFieldLineElectrostatic;break;
		case thread_fieldline_electric:					selectedThread =  &m_threadFieldLineElectric;break;
		case thread_fieldline_magnetic_current_loop:	selectedThread =  &m_threadFieldLineMagnetic_current_loop;break;
		case thread_fieldline_magnetostatic:			selectedThread =  &m_threadFieldLineMagnetostatic;break;
		case thread_fieldline_magnetic:					selectedThread =  &m_threadFieldLineMagnetic;break;
	//	case thread_fieldline_from_hell:				selectedThread =  &m_threadFieldLineFrom_hell;break;
		default: return;
	}
	if (*selectedThread) KillThread(threadType);
	if (!(*selectedThread))
	{
		*selectedThread = new CFieldLineODEThread(FieldLine_Tangent, &m_rigidBodyManager, (long) threadType);
		WaitForSingleObject(HANDLE ((*selectedThread)->m_hEventThreadInit), INFINITE);
	}
//	else return;

	(*selectedThread)->FlushThread();
}


///////////////////////////////////////////////////////////////////////////////
void CSystem::FieldLines(ThreadType t, Vector3D from, Vector3D to, Vector3D dr) {
	m_fromFieldLines=from;
	m_toFieldLines=to;
	m_drFieldLines=dr;

	FieldLine(t);
}

void Action::FieldLine_Tangent(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
	CFieldLineODE* selectedThread = (CFieldLineODE*) f_data;
	Point3D p(stateVector->data);
	Vector3D tangent = theApp.theSystem.Compute_FieldLine_Tangent((CSystem::ThreadType) selectedThread->m_type, p, t);
	dx->data[0] = tangent.x;
	dx->data[1] = tangent.y;
	dx->data[2] = tangent.z;
}

///////////////////////////////////////////////////////////////////////////////

Vector3D CSystem::Compute_FieldLine_Tangent(ThreadType threadType, const Point3D& p, double t)
{

try {

	Vector3D f;
	Vector3D& B	= theFieldsManager.m_ExternalMagneticField;
	double& g	= theFieldsManager.m_ExternalGravitationalFieldY;
	double& Ex1 = theFieldsManager.m_ExternalElectrostaticFieldX1;
	double& Ex2 = theFieldsManager.m_ExternalElectrostaticFieldX2;
	double& Ex3 = theFieldsManager.m_ExternalElectrostaticFieldX3;
/*
	TimeDependentExternalForceComponent& X = timeDependentExternalForceComponentX;
	TimeDependentExternalForceComponent& Y = timeDependentExternalForceComponentY;
	TimeDependentExternalForceComponent& Z = timeDependentExternalForceComponentZ;
*/
	if (threadType == thread_fieldline_magnetic)
	if (B.x!=0.0 || B.y!=0.0 || B.z!=0.0 )
	{
		f += B;
	}

	if (threadType == thread_fieldline_magnetic_current_loop)
	if (theFieldsManager.m_bValidCurrentLoopMagneticField)
	{
		Vector3D B = theFieldsManager.MagneticField(p);
		f += B;
		f.normalize();
		return f;
	}

	if (threadType == thread_fieldline_magnetic_current_loop)
	if (g!=0.0)
	{
		f += g*Vector3D::J;
	}
	if (threadType == thread_fieldline_electric)
	if (Ex1!=0.0 || Ex2!=0.0 || Ex3!=0.0 )
	{
		const double& x = p.x;
		f += ((x*(Ex1 + Ex2*x + Ex3*x*x))*Vector3D::I);
	}
/*
	if (X.bValid)
	{
		f += (X.Eval(t))*Vector3D::I;
	}
	if (Y.bValid)
	{
		f += (Y.Eval(t))*Vector3D::J;
	}
	if (Z.bValid)
	{
		f += (Z.Eval(t))*Vector3D::K;
	}
*/

/*		if (m_vecDampingCoefficient.x!=0.0)
		{
			f += ((-velocityCM.x*m_vecDampingCoefficient.x)*Vector3D::I);
		}
		if (m_vecDampingCoefficient.y!=0.0)
		{
			f += ((-velocityCM.y*m_vecDampingCoefficient.y)*Vector3D::J);
		}
		if (m_vecDampingCoefficient.z!=0.0)
		{
			f += ((-velocityCM.z*m_vecDampingCoefficient.z)*Vector3D::K);
		}
*/

//	RigidBodyList::iterator it;
	if (threadType == thread_fieldline_gravitational)
		f += theFieldsManager.N_Body_GravitationalField(p);

	if (threadType == thread_fieldline_electrostatic || threadType == thread_fieldline_electric)
		f += theFieldsManager.N_Body_ElectrostaticField(p);


	f.normalize();
	return f;
}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			return Vector3D::ZERO;
		}
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::N_Body_GravitationalField (const Point3D& p)
{
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	Vector3D f;
	if (m_GravitationalCoupling == 0.0) return f;
try
{
//	double& g	= m_ExternalGravitationalFieldY;
	RigidBodyList::iterator it;
	for (it = theApp.theSystem.m_rigidBodyManager.begin(); it != theApp.theSystem.m_rigidBodyManager.end(); ++it)
	{
		CRigidBody* pbody = *it;
		ASSERT(pbody != 0);
		if (pbody == 0) return Vector3D::ZERO;
		if (pbody->m_fMassInverse == 0.0) continue;
		Vector3D rel = p - pbody->m_positionCM;
		double rCubed = pow(rel*rel,1.5);
		if (rCubed < 1.0e-30) {
			f.normalize();
			return f;
	//		throw CExceptionTerminateFieldLine();
		}
		rel /= rCubed;
		f += (-m_GravitationalCoupling/pbody->m_fMassInverse)*rel;
	}

}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return Vector3D::ZERO;
	}
	return f;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CFieldsManager::N_Body_ElectrostaticField (const Point3D& p)
{
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	Vector3D f;
try
{
	RigidBodyList::iterator it;
	for (it = theApp.theSystem.m_rigidBodyManager.begin(); it != theApp.theSystem.m_rigidBodyManager.end(); ++it)
	{
		CRigidBody* pbody = *it;
		ASSERT(pbody != 0);
		if (pbody == 0) return Vector3D::ZERO;
		if (pbody->m_fCharge == 0.0) continue;
		Vector3D rel = p - pbody->m_positionCM;
		double rCubed = pow(rel*rel,1.5);
		if (rCubed < 1.0e-30) {
			f.normalize();
			return f;
	//		throw CExceptionTerminateFieldLine();
		}
		rel /= rCubed;
		f += pbody->m_fCharge*rel;
	}
}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return Vector3D::ZERO;
	}
	return f;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CSystem
void CSystem::KillThread(ThreadType threadType) {
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)this - offsetof(CRigidBodyManager, theSystem));
	CFieldLineODEThread** selectedThread = 0;
	switch (threadType)
	{
//		case thread_time_step:						selectedThread =  &theApp.theSystem.m_rigidBodyManager.m_threadTimeStep;					break;
		case thread_fieldline_gravitational:		selectedThread =  &m_threadFieldLineGravitational;		break;
		case thread_fieldline_electrostatic:		selectedThread =  &m_threadFieldLineElectrostatic;		break;
		case thread_fieldline_electric:				selectedThread =  &m_threadFieldLineElectric;			break;
		case thread_fieldline_magnetic_current_loop:selectedThread =  &m_threadFieldLineMagnetic_current_loop;break;
		case thread_fieldline_magnetostatic:		selectedThread =  &m_threadFieldLineMagnetostatic;		break;
		case thread_fieldline_magnetic:				selectedThread =  &m_threadFieldLineMagnetic;			break;
		case thread_fieldline_from_hell:			selectedThread =  &m_threadFieldLineFrom_hell;			break;
	}
	if ((*selectedThread))
	try {
		(*selectedThread)->Kill();
		{
			delete (*selectedThread);
		}
		*selectedThread = 0;
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
	throw Syntax("CSystem::KillThread: Cannot kill thread.");
	}
	switch (threadType)
	{
//		case thread_time_step:						ASSERT(0 ==m_threadTimeStep);					break;
		case thread_fieldline_gravitational:		ASSERT(0 ==m_threadFieldLineGravitational);		break;
		case thread_fieldline_electrostatic:		ASSERT(0 ==m_threadFieldLineElectrostatic);		break;
		case thread_fieldline_electric:				ASSERT(0 ==m_threadFieldLineElectric);			break;
		case thread_fieldline_magnetic_current_loop:ASSERT(0 ==m_threadFieldLineMagnetic_current_loop);break;
		case thread_fieldline_magnetostatic:		ASSERT(0 ==m_threadFieldLineMagnetostatic);		break;
		case thread_fieldline_magnetic:				ASSERT(0 ==m_threadFieldLineMagnetic);			break;
		case thread_fieldline_from_hell:			ASSERT(0 ==m_threadFieldLineFrom_hell);			break;
	}
}
///////////////////////////////////////////////////////////////////////////////


void CSystem::KillThreads()
{
///////////////////////////////////////////////////////////////////////////////
	m_nThreadType		= thread_fieldline_from_hell;
	KillThread(thread_fieldline_gravitational);
	KillThread(thread_fieldline_electrostatic);
	KillThread(thread_fieldline_electric);
	KillThread(thread_fieldline_magnetic_current_loop);
	KillThread(thread_fieldline_magnetostatic);
	KillThread(thread_fieldline_magnetic);
	KillThread(thread_fieldline_from_hell);

	m_threadFieldLineGravitational	= 0;
	m_threadFieldLineElectrostatic	= 0;
	m_threadFieldLineElectric	= 0;
	m_threadFieldLineMagnetic_current_loop	= 0;
	m_threadFieldLineMagnetostatic	= 0;
	m_threadFieldLineMagnetic	= 0;
	m_threadFieldLineFrom_hell	= 0;
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////




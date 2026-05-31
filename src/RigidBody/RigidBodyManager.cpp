// RigidBodyManager.cpp: implementation of the CRigidBodyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBodyManager.h"
#include "RigidBody.h"
#include "FieldsManager.h"
#include "System.h"
#include "Contact.h"
#include "objects.h"
#include "LeftView.h"
#include "GLView.h"
#include "parser.h" /* for token definitions and yylval */
#include "ConstraintManager.h"
#include <functional>
#include <algorithm>
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct SolveIt::ObjectsListCompareEqual
{
//	ObjectsListCompareEqual(const objects* _b) : b(_b) {ASSERT(b->ref_count != 0);}
//	objects* b;
//	bool operator()(const objects * a) const
//	{
//		return a->ref_count == b->ref_count;
//	}
	ObjectsListCompareEqual(const objects& _b) : b(_b) {ASSERT(b.ref_count != 0);}
	const objects& b;
	bool operator()(const objects & a) const
	{
		if (b.type != __RigidBodyPtr) return false;
		if (a.type != __RigidBodyPtr) return false;
	//	CRigidBody * bBody = b.rbPtr;
	//	CRigidBody * aBody = a.rbPtr;
		return a.rbPtr->name == b.rbPtr->name;
	//	return a.ref_count == b.ref_count;
	}
};
///////////////////////////////////////////////////////////////////////////////


CRigidBodyManager::CRigidBodyManager():
	m_pSelectedRigidBody(0),
	m_bInternalForce(true),
	m_bShowContacts(false),
	m_bShowClosestContacts(false)
{

}

CRigidBodyManager::~CRigidBodyManager()
{
try {
	int sz = static_cast<int>(size());
	for (ObjectsList::iterator it = begin(); it != end(); ++it )
	{
		while((*it).ReleaseRef());
		(*it) = 0;
		if (--sz <= 0) break;
	}
	clear();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CRigidBodyManager::~CRigidBodyManager ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CRigidBodyManager::~CRigidBodyManager");
	}
}
///////////////////////////////////////////////////////////////////////////////
//static long m_InteractingPairKey=0;

void CRigidBodyManager::add(const objects& object)
{
try {
	ASSERT(object.ref_count != 0);
	ObjectsListCompareEqual objectsListCompareEqual(object);
	ObjectsList::iterator it = find_if(begin(), end(), objectsListCompareEqual);
	if ( it != end() )
	{
//		ASSERT(0);
		wchar_t buf[128];
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
		/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf)/sizeof(buf[0]));
		oss	<< "error: " << object.rbPtr->name << " exists" << ends;
		wstring str(buf);
		pFrame->SetStatusBarText(str, 1);
		return;
	}
	push_back(object);
	DefineExistingSpheresShapeInMixedSystem();

	CRigidBody * pBody = object.rbPtr;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* m_body1	= dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(m_body1 != 0);
		if (!m_body1) throw SolveIt_ERROR(L"pbody == 0");//continue;
		if (pBody==m_body1) continue;
		Contact* p = new Contact(m_body1, pBody, m_bShowContacts);
		ASSERT(	p != 0);
		m_mapInteractingPair[p->m_lKey] = p;
	}
//	pBody->PutOrientation(90, Vector3D::I);//using auxSolidCylinder, ...
	theApp.GetFieldsManager()->Clear();
	theApp.GetTreeView()->PopulateTree();
	theApp.GetSystem()->DisplayData();

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CRigidBodyManager::add ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CRigidBodyManager::add");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::OnDraw()
{
try {
//	CSystem* m_system = theApp.GetSystem();
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		if ((*it).type == __RigidBodyPtr)
		{
			dynamic_cast<CRigidBody *>((*it).rbPtr)->OnDrawState();
		}
	}
	if (!m_bShowClosestContacts) return;

	if (m_mapInteractingPair.empty()) return;

	map <unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
			if (ptr) ptr->OnDraw();
	}
//	glFlush();//With multithread applications, flush the drawing commands in any other threads drawing to the same window before calling SwapBuffers

}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"CRigidBodyManager::OnDraw ",  GetWC(exc->what()), MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}
	catch(...)
	{
		AfxMessageBox(L"CRigidBodyManager::OnDraw");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//void SolveIt::CVodeDerivative(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
//{
//	CRigidBodyManager* pRigidBodyManager = (CRigidBodyManager * ) f_data;
//	pRigidBodyManager->TimeDerivativeOfSystemState( t, stateVector->data, dx->data);
//}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::TimeDerivativeOfSystemState(double t, double x[], double dx[]) {
	if (empty()) return;
		Lock lock (theApp.protectBodies);
	try
	{
// Set the initial conditions:
		GetStateOfBodies();
		SaveStateOfBodies();
		TimeDerivative__SetBodyStates(x);
		Compute_Force_and_Torque(t);
		GetTimeDerivativeOfStates(dx);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		RestoreSavedStateOfBodies();
		try {
//			theApp.theSystem.m_threadTimeStep->Kill();
		}
		catch(wchar_t* s)
		{
			TimedMessageBox(NULL, s,L"CSystem::TimeDerivativeOfSystemState",MB_OK);
		}
		TimedMessageBox(NULL,L"General Exception", L"CSystem::TimeDerivativeOfSystemState", MB_OK);
		throw SolveIt_ERROR(L"-1");
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Compute_Force_and_Torque(double t)
{
	if (empty()) return;
	ClearForcesTorques();
	Compute_External_Force_and_Torque(t);
	Compute_Internal_Force_and_Torque(t);
	AddForcesTorques();
	Compute_RestingContact_Forces(t);
	ImposeConstraints(t);//call this last
// CollisionResponse uses this to estimate the time of the next collision:
	SetPreviousForcesTorques();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ClearForcesTorques()
{
	if (empty()) return;
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
		Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
		ptr->impulseLab = ZERO_3D;
	}
	ClearCollisionContactsLists();
	ClearRestingContactsLists();
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ClearForcesTorques();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Compute_External_Force_and_Torque(double t)
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->Compute_External_Force_and_Torque(t);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::AddForcesTorques()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->AddForcesTorques();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Compute_RestingContact_Forces(double time)
{
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Compute_Internal_Force_and_Torque(double t, CRigidBody* pComputeInternal_Force_and_TorqueOfBody)
{
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	double mass1=0, mass2=0;
	iterator it, iT;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody1 = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody1 != 0);
		if (pbody1 == 0) throw SolveIt_ERROR(L"CRigidBodyManager::Compute_Internal_Force_and_Torque");

	//	if (pbody1->m_fMassInverse==0.0) continue;
///////////////////////////////////////////////////////////////////////////////
		if (pComputeInternal_Force_and_TorqueOfBody) {
			if (pbody1 != pComputeInternal_Force_and_TorqueOfBody) continue;
		}
///////////////////////////////////////////////////////////////////////////////
		mass1 = pbody1->m_fMassInverse>0.0? 1.0/pbody1->m_fMassInverse:pbody1->m_fCentralForce;
///////////////////////////////////////////////////////////////////////////////
		if (pComputeInternal_Force_and_TorqueOfBody) {
			iT = begin();
		}
		else {
			iT = it;
		}
///////////////////////////////////////////////////////////////////////////////
		for ( ; iT != end(); ++iT)
		{
			if (it==iT) continue;
			CRigidBody * pbody2 = dynamic_cast<CRigidBody *>((*iT).rbPtr);
			ASSERT(pbody2 != 0);
			if (pbody2 == 0) throw SolveIt_ERROR(L"CRigidBodyManager::Compute_Internal_Force_and_Torque");
			ASSERT(pbody2 != pbody1);
			if (pbody2==pbody1) continue;
			mass2 = pbody2->m_fMassInverse>0.0? 1.0/pbody2->m_fMassInverse:pbody2->m_fCentralForce;
			Vector3D rel = pbody1->x - pbody2->x;
			double		r2	= rel*rel;
			double		r	= sqrt(r2);
			if (r<1.0e-8) continue;




//	Vector3D m_fLennardJones;//[x != 0 => valid; y / r^6 - z / r^12 
	Vector3D LennardJones = fieldsManager->m_fLennardJones;//( (long*) &retu);
	if (LennardJones.x != 0.0)
	{
		const double sqrt2 = 1.4142135623730950488016887242097;
		double		r3	= pow(LennardJones.y/r, 3.0);
		double		r6	= pow(LennardJones.z/r, 6.0)*sqrt2;
		double		R	= 6*(r6-r3)*(r6+r3)/r2;
		Couple couple(R*rel);
		if (pbody1->m_fMassInverse!=0.0)
			pbody1->m_CoupleList.push_back(couple);
		couple.force.negate();
		if (pbody2->m_fMassInverse!=0.0)
			pbody2->m_CoupleList.push_back(couple);
	}


#if defined(at_least_crudely_optimize)


		double r3 = pow(rel*rel,1.5);
		if (r3<1.0e-24) continue;
		rel /= r3;



		double jk = pbody1->m_fCharge*pbody2->m_fCharge;
	if (jk != 0.0)
	{
		Couple couple(jk*rel);
		if (pbody1->m_fMassInverse!=0.0)
			pbody1->m_CoupleList.push_back(couple);
		couple.force.negate();
		if (pbody2->m_fMassInverse!=0.0)
			pbody2->m_CoupleList.push_back(couple);
	}
//	Vector3D m_fLennardJones;//[x != 0 => valid; y / r^6 - z / r^12 

//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, m_rigidBodyManager));
	jk = -fieldsManager->m_GravitationalCoupling*mass1*mass2;
	if (jk != 0.0)
	{
		Couple couple(jk*rel);
		if (pbody1->m_fMassInverse!=0.0)
			pbody1->m_CoupleList.push_back(couple);
		couple.force.negate();
		if (pbody2->m_fMassInverse!=0.0)
			pbody2->m_CoupleList.push_back(couple);
	}
#endif
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetPreviousForcesTorques()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->SetPreviousForcesTorques();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::AddInclineForcesTorques(bool bDuringCollision)
{
	if (bDuringCollision) return;//not yet implemented 
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ComputeNormalAndFrictionalForcesAndTorque();
		pbody->AddInclineForcesTorques();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ImposeConstraints(double t)
{
	if (empty()) return;
	AddInclineForcesTorques(false);
	CConstraintManager* constraintManager	= theApp.GetConstraintManager();
	constraintManager->ImposeConstraints(t);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SaveStateOfBodies()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->SaveState();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::RestoreSavedStateOfBodies()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->RestoreSavedState();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetPreviousStateOfBodies()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->SetPreviousState();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetToPreviousState()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->SetToPreviousState();//calls  CRigidBody::PQP_Update();
	}
	PQP_Update(false);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::PQP_Update(bool bFirstUpdateBodiesThenContacts)
{
	if (empty()) return;
	if (bFirstUpdateBodiesThenContacts)
	{
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
			ASSERT(pbody != 0);
			if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
			pbody->PQP_Update();
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_mapInteractingPair.empty()) return;
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
			if (ptr) ptr->PQP_Update();
	}
/*
*/
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::GetStateOfBodies()
{
	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, rigidBodyManager));
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->GetStateOfBody(m_system_state);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::TimeDerivative__SetBodyStates(double y[])
{
	if (empty()) return;
	iterator it;
	long i=0;
	for (it = begin(); it != end(); ++it, ++i)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->TimeDerivativeSetBodyState( y + i * DEGREES_FREEDOM );
	}
//	PQP_Update(false);
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::GetTimeDerivativeOfStates(double ydot[])
{
	if (empty()) return;
	long i=0;
	iterator it;
	for (it = begin(); it != end(); ++it, ++i)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->GetTimeDerivativeOfState( ydot + i * DEGREES_FREEDOM);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetStateOfBodies(const vector<double>& state, BOOL bUpdatePreviousState)
{
//	CSystem* m_system = theApp.GetSystem();
//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, rigidBodyManager));
try
{
	if (empty()) return;
	vector<double>::const_iterator ic_theVector	= state.begin();
	{
//		ic_theVector = m_system_state.begin();
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
			ASSERT(pbody != 0);
			if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
			pbody->SetStateOfBody(ic_theVector, (bUpdatePreviousState?true:false));
		}
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)//theApp.theSystem. there was an error
{
	ASSERT(0);
	theApp.GetSystem()->Stop();
	SetToPreviousState();
}
try
{
	PQP_Update(false);
}
catch( SolveIt_Error& e){e.why();}catch(...)//theApp.theSystem. there was an error
{
	ASSERT(0);
	theApp.GetSystem()->Stop();
	SetToPreviousState();
}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::WriteThoseTracking()
{
	if (empty()) return;
	iterator it;
//	CSystem* m_system = theApp.GetSystem();
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->WriteTrack();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetTime(double t)
{
	if (empty()) return;
	iterator it;
//	CSystem* m_system = theApp.GetSystem();
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->SetTime(t);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ClearTrajectoryRecords()
{
	if (empty()) return;
	iterator it;
//	CSystem* m_system = theApp.GetSystem();
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody * pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ClearTrajectoryRecord();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CRigidBody* CRigidBodyManager::HitTest(const POINT& pt) //const
{
	if (empty()) return m_pSelectedRigidBody = 0;
	const_iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return 0;
	//	if (dynamic_cast<CRigidBody *>((*it).rbPtr)->HitTest(pt)) return m_pSelectedRigidBody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		if (pbody->HitTest(pt)) return pbody;
	}
//	return m_pSelectedRigidBody = 0;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//long ComputeScreenCoords(double *x, double *y, double *z);
//long ComputeWorldCoords(double *x, double *y, double *z);
///////////////////////////////////////////////////////////////////////////////
int CRigidBodyManager::HitTest(POINT& pt, long& ret)
{
	if (empty()) return S_OK;
//	CSystem::m_CurrentMousePosition = Vector2D (pt);
//	if (HitTest(pt)) ret=1;
//	else ret=0;
//	CRigidBody* pbody = HitTest(pt);
	m_pSelectedRigidBody = HitTest(pt);
	ret= m_pSelectedRigidBody? 1:0;
	CGLView* pGLView = theApp.GetGLView();

	double z=0;
	if (m_pSelectedRigidBody) z = m_pSelectedRigidBody->m_f_zScreen;
	else
	{
//z=1 <-> m_fFarPlane;z=0 <-> m_fNearPlane;
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		z = z_Pos;
	}
	Point3D p(pt.x, pt.y, z);
//	theApp.GetGLView()->ComputeWorldCoords(&p.x, &p.y, &p.z);
	pGLView->ComputeWorldCoords(p.x, p.y, p.z, false);

//	CSystem::m_CurrentMousePosition = Vector2D (p.x, p.y);
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
bool CRigidBodyManager::IsCharged()
{
	if (empty()) return false;
	const_iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody->m_fCharge != 0.0) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetScreenRects()
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		dynamic_cast<CRigidBody *>((*it).rbPtr)->SetScreenRect();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ExaminePairs()
{
	if (m_mapInteractingPair.empty()) return;
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
			if (ptr) ptr->ExaminePair();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::CloseExaminePairs()
{
	if (m_mapInteractingPair.empty()) return;
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
			if (ptr) ptr->CloseExaminePair();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ClearCollisionContactsLists()
{
	m_CollidingContactsList.clear();
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
		Contact* pc = dynamic_cast<Contact*>(i_mapInteractingPair->second);//*i_CollidingContactsList;
		pc->ClearCollisionContactsLists();
	}
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->ClearContactPointList();
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::ClearRestingContactsLists()
{
	if (empty()) return;
	vector<Contact*>::iterator j;
	for (	j=m_ContactPointsAtRelativeRest.begin();
			j!=m_ContactPointsAtRelativeRest.end();++j) delete *j;
	m_ContactPointsAtRelativeRest.clear();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::DefineExistingSpheresShapeInMixedSystem()
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody *body = dynamic_cast<CRigidBody*>(dynamic_cast<CRigidBody *>((*it).rbPtr));
		ASSERT(	body != 0);
		assert(	body != 0);
		if (body->type==T_SPHERE)
		{
			if (body->m_pPQP_Model == 0) body->DefineShape();
		}
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Reset() {
	if (empty()) return;
	if (RigidLinkJointStatesInitial.size()) {
		RigidLinkJointStates.clear();
		RigidLinkJointStates = 	RigidLinkJointStatesInitial;
	}
///////////////////////////////////////////////////////////////////////////////
	ClearCollisionContactsLists();
	map < unsigned long, Contact*>::iterator i_mapInteractingPair;
	for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
			i_mapInteractingPair != m_mapInteractingPair.end();
			++i_mapInteractingPair)
	{
		Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
		ptr->Reset();
		ptr->m_bPermute = false;
	}


	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		pbody->Reset();
	}
//	if (!CRigidBody::m_boolSpheresOnly)
//	{
//		if (!SOLID::dtTest()) SOLID::dtProceed();
//		else
//		{
//			TimedMessageBox(NULL, "pbody->Reset() => SOLID::dtTest() != 0","CRigidBodyManager::Reset",MB_OK);
//		}
//	}
	PQP_Update(false);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Clear() {
	m_system_state.clear();
	m_CollidingContactsList.clear();
	m_ContactPointsAtRelativeRest.clear();
//dealloc in 'blocks' otherwise get heap errors!(but why?)
//	CRigidBody::m_boolSpheresOnly = true;
	try
	{
	//	theApp.theSystem.Clear();
///////////////////////////////////////////////////////////////////////////////
		if (!m_mapInteractingPair.empty())
		{
			map < unsigned long, Contact*>::iterator i_mapInteractingPair;
			for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
					i_mapInteractingPair != m_mapInteractingPair.end();
					++i_mapInteractingPair)
			{
				Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
				ASSERT(	ptr != 0);
				delete ptr;
				ptr = 0;
			}

			m_mapInteractingPair.clear();
		}
///////////////////////////////////////////////////////////////////////////////
		PQP_Update();
///////////////////////////////////////////////////////////////////////////////
	iterator it;
	for (it = begin(); it != end(); ++it)
		{
			CRigidBody *killMe = dynamic_cast<CRigidBody*>(dynamic_cast<CRigidBody *>((*it).rbPtr));
		//	CRigidBody* killMe = *i;
			ASSERT(	killMe != 0);
			assert(		killMe != 0);
		//	std::wstring bstr(killMe->name.c_str());
		//	FireDeleteSymbolNamed(bstr);
			delete killMe;
			killMe = 0;
		//	m_pIScanParse->DeleteSymbolNamed(bstr);
		}
///////////////////////////////////////////////////////////////////////////////
		clear();
		ASSERT(empty());
///////////////////////////////////////////////////////////////////////////////
	}
	catch( ... )
	{
		::MessageBox(NULL,L"Exception",L"CRigidBodyManager::Clear",MB_OK);
		return;
	}
///////////////////////////////////////////////////////////////////////////////
//	PopulateTree();
//	FireViewChange();
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBodyManager::MagneticField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D B;
	if (!m_bInternalForce) return B;
	if (empty()) return B;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return B;
		if (pbody == pBody) continue;
		B += pbody->StaticMagneticField(p);
	}
	return B;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBodyManager::Static_GravitationalField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D G;
	if (!m_bInternalForce) return G;
	if (empty()) return G;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return G;
		if (pbody == pBody) continue;
		G += pbody->Static_GravitationalField(p);
	}
	return G;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBodyManager::ElectrostaticField (const Point3D& p, const CRigidBody* pBody)
{
	Vector3D m_threadFieldLineMagnetostatic;
	if (!m_bInternalForce) return m_threadFieldLineMagnetostatic;
	if (empty()) return m_threadFieldLineMagnetostatic;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return m_threadFieldLineMagnetostatic;
		if (pbody == pBody) continue;
		m_threadFieldLineMagnetostatic += pbody->ElectrostaticField(p);
	}
	return m_threadFieldLineMagnetostatic;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct SolveIt::RigidBodyListCompareEqualToString
{
	RigidBodyListCompareEqualToString(const wstring& _b) : b(_b) {}
	wstring b;
	bool operator()(const objects& a) const
	{
		return a.rbPtr->name == b;
	}
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CRigidBody * CRigidBodyManager::get_Body(const wstring& Name)
{
	if (empty()) return 0;
	if (Name.empty()) return 0;
	RigidBodyListCompareEqualToString rigidBodyListCompareEqualToString(Name);
	iterator it = find_if(begin(), end(), rigidBodyListCompareEqualToString);
	if (it != end ())
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		return pbody;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::GetConstraintedBodies(vector<CRigidBody *>& constraintedBodies)
{
	if (empty()) return;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return;
		if (pbody!=0 && pbody->m_bSubjectToActiveConstraint) constraintedBodies.push_back(pbody);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::Remove(const wstring& name)
{
	if (name.empty()) return;
	std::wstring bstr(name.c_str());
//	theApp.theSystem.theConstraintManager.Remove(name);
	try
	{
		if (empty()) return;
		RigidBodyListCompareEqualToString rigidBodyListCompareEqualToString(name);
		iterator it = find_if(begin(), end(), rigidBodyListCompareEqualToString);
		if (it == end ()) return;
		CRigidBody* killMe = get_Body(name);
		ASSERT(killMe == dynamic_cast<CRigidBody *>((*it).rbPtr));
		ASSERT(killMe != 0);
///////////////////////////////////////////////////////////////////////////////
#if defined(USE_SWIFT)
		_SWIFT_World->scene->Deactivate(Key);
#endif
///////////////////////////////////////////////////////////////////////////////
//		erase( it );
		if (killMe == 0) return;
		if (m_pSelectedRigidBody==killMe) m_pSelectedRigidBody = 0;

//		if (killMe->m_pPQP_Model) delete killMe->m_pPQP_Model, killMe->m_pPQP_Model=0;
//		killMe->m_pPQP_Model = new PQP_Model;
//		killMe->m_pPQP_Model->BeginModel();
//		killMe->m_pPQP_Model->EndModel();
//		PQP_Update();
///////////////////////////////////////////////////////////////////////////////
if (!m_mapInteractingPair.empty())
{
		map < unsigned long, Contact*>::iterator i_mapInteractingPair;
		vector<map < unsigned long, Contact*>::iterator> erase_i_mapInteractingPair;
		for (	i_mapInteractingPair  = m_mapInteractingPair.begin();
				i_mapInteractingPair != m_mapInteractingPair.end();
				++i_mapInteractingPair)
		{
			Contact* ptr = dynamic_cast<Contact*>(i_mapInteractingPair->second);
			if (ptr && (ptr->m_body1==killMe||ptr->m_body2==killMe))
			{
				delete ptr;
				ptr = 0;
				erase_i_mapInteractingPair.push_back(i_mapInteractingPair);
			}
		}

		vector<map < unsigned long, Contact*>::iterator>::iterator i_erase_i_mapInteractingPair;
		for (	i_erase_i_mapInteractingPair  = erase_i_mapInteractingPair.begin();
				i_erase_i_mapInteractingPair != erase_i_mapInteractingPair.end();
				++i_erase_i_mapInteractingPair)
		{
			map < unsigned long, Contact*>::iterator i_mapInteractingPair = m_mapInteractingPair.find((*i_erase_i_mapInteractingPair)->first);
			if (i_mapInteractingPair != m_mapInteractingPair.end())
				m_mapInteractingPair.erase(i_mapInteractingPair);
		}
}
///////////////////////////////////////////////////////////////////////////////
		PQP_Update();

//		long(HIWORD())	CVectorAnalysis* pVectorAnalysis = dynamic_cast<CVectorAnalysis*>(killMe);
//			if (pVectorAnalysis) pVectorAnalysis->DispEventUnadvise();

//			if (pVectorAnalysis) pVectorAnalysis->Destroy();
//			else

		while((*it).ReleaseRef());
		(*it)=0;
		erase( it );
	//	delete killMe;
		killMe = 0;

		theApp.GetFieldsManager()->Clear();
		theApp.GetTreeView()->PopulateTree();
		theApp.GetSystem()->DisplayData();
		Fire_ViewChange();
//incorrect placement (timing) of this call generates heap error:
	//	FireDeleteSymbolNamed(bstr);
	}
	catch( ... )
	{
		::MessageBox(NULL,bstr.c_str(),L"CRigidBodyManager::Remove""Exception",MB_OK);
		return;
	}
}


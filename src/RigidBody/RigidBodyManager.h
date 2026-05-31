// RigidBodyManager.h: interface for the CRigidBodyManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGIDBODYMANAGER_H__5A0C56D8_53D9_4EF6_B4A4_C49A2C98A63E__INCLUDED_)
#define AFX_RIGIDBODYMANAGER_H__5A0C56D8_53D9_4EF6_B4A4_C49A2C98A63E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "objects.h"
#include "Joint1DOF.h"
#include "array.h"
#include "Constraint.h"

#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class objects;
typedef list<objects> ObjectsList;
struct ObjectsListCompareEqual;
class Contact;

void CVodeDerivative(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data);
///////////////////////////////////////////////////////////////////////////////

	
class CRigidBodyManager	:
	public ObjectsList
{
public:
	CRigidBodyManager();
	virtual ~CRigidBodyManager();

	void add(const objects& object);
	void Remove(const wstring& name);
	void DefineExistingSpheresShapeInMixedSystem();
	void Initialize();

	void Clear();
	void Reset();
///////////////////////////////////////////////////////////////////////////////
	CRigidBody* HitTest(const POINT& pt);// const;
	int HitTest(  POINT& pt,  long& ret);
	void SetScreenRects();
	CRigidBody* m_pSelectedRigidBody;
	CRigidBody * get_Body(const wstring& Name);
///////////////////////////////////////////////////////////////////////////////
//	int MouseMove(  long* ppoint,  long* pRECT,  long attributes);
	bool IsCharged();
	void OnDraw();
///////////////////////////////////////////////////////////////////////////////
	bool m_bInternalForce;
	void Compute_Force_and_Torque(double t);
///////////////////////////////////////////////////////////////////////////////
	void ClearForcesTorques();
	void Compute_External_Force_and_Torque(double t);
	void Compute_Internal_Force_and_Torque(double t, CRigidBody* pComputeInternal_Force_and_TorqueOfBody = 0);
	void Compute_RestingContact_Forces(double t);
	void AddForcesTorques();
	void AddInclineForcesTorques(bool bDuringCollision = false);
	void ImposeConstraints(double t);
	void SetPreviousForcesTorques();
//	void ComputeImpulses();
//	void ComputeMomentumChanges();
/////////////////////////////////////////////////////////////////////////////////
	void TimeDerivativeOfSystemState(double t, double x[], double dx[]);
	friend void CVodeDerivative(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data);
///////////////////////////////////////////////////////////////////////////////
	void TimeDerivative__SetBodyStates(double y[]);
	void GetTimeDerivativeOfStates(double ydot[]);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void GetConstraintedBodies(vector<CRigidBody *>& constraintedBodies);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void GetChangeInNormalRelativeVelocity(vector<double>& nv, int nSize);
	vector<CRigidBody*> m_ListOfCollidingRigidBodies;
	void GetListOfCollidingRigidBodies();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	vector<Contact*> m_CollidingContactsList;
	void ClearCollisionContactsLists();
	void ClearRestingContactsLists();
	void ComputeNonContactAndInertialAccelerations();
	void ComputeImpulses();
	void ComputeMomentumChanges();
///////////////////////////////////////////////////////////////////////////////
	void Compute_A_Matrix();
	void Compute_B_Vector(double *b);
	void Compute_b_Vector(double *b);
	vector<Contact*> m_ContactPointsAtRelativeRest;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	long ResolveCollisions();
	long ResolveSphereCollisions();
///////////////////////////////////////////////////////////////////////////////
	double RefineCollisionTime(double& t_in,double& t_out,double& timeMin,double& timeMax);
	long NumberCollisions(bool bBuildLists = false);

	void ResolveCollisions(Contact& pair, double time);
	void ResolveTwoCollisionPairs(vector <Contact*>& m_vecContact, double time);
	void ResolveManyCollisionPairs(vector <Contact*>& m_vecContact, double time);
///////////////////////////////////////////////////////////////////////////////
	map <unsigned long, Contact*> m_mapInteractingPair;
/////////////////////////////////////////////////////////////////////////////////
	bool m_bShowContacts;
	bool m_bShowClosestContacts;
	void ExaminePairs();
	void CloseExaminePairs();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	void SetStateOfBodies(BOOL bUpdatePreviousState = TRUE);
	void SetStateOfBodies(const vector<double>& state, BOOL bUpdatePreviousState = TRUE);//double state[]
	void GetStateOfBodies();
	void SetPreviousStateOfBodies();
	void SetToPreviousState();
	void SaveStateOfBodies();
	void RestoreSavedStateOfBodies();
///////////////////////////////////////////////////////////////////////////////
	void PQP_Update(bool bFirstUpdateBodiesThenContacts = true);
///////////////////////////////////////////////////////////////////////////////
	void SetTime(double t);
	void WriteThoseTracking();
	void RecordData(CRigidBody* body, long option, double t1, double t2);
	void AnalyzeData(CRigidBody* body, long option, const Vector3D& n);
	void ClearTrajectoryRecords();
///////////////////////////////////////////////////////////////////////////////
	double ComputePotentialEnergy(CRigidBody* pComputeEnergyOfBody = 0);
	double ComputeEnergy(CRigidBody* pComputeEnergyOfBody = 0);
	double ComputeKineticEnergy(CRigidBody* pComputeEnergyOfBody = 0);
	Vector3D ComputeMomentum();
	Vector3D ComputeAngularMomentum();
///////////////////////////////////////////////////////////////////////////////
	Vector3D Static_GravitationalField (const Point3D& p, const CRigidBody* pBody = 0);
	Vector3D ElectrostaticField (const Point3D& p, const CRigidBody* pBody = 0);
	Vector3D MagneticField (const Point3D& p, const CRigidBody* pBody = 0);
// data:
	vector_of_double m_system_state;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// MultiBody
	vector<double> RigidLinkJointStates;
	vector<double> RigidLinkJointStatesInitial;
	vector<double> JointForcesTorques;
	map<long, double> Joint1DOFAccelerations;
	map<long, double> partialJoint1DOFAccelerations;
	map<long, Vect6> partialVect6;
	map<long, double> map_isIs;
	void InitMultibodyState(vector<double>& state);////double state[]
	void SetMultibodyState(const vector<double>& state);//double state[]
	int StepMultibodyState();
///////////////////////////////////////////////////////////////////////////////
// forward dynamics
	void ForwardDynamics(const double sv[], SpatialMat &Iprop, SpatialVect &Pprop);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void updatePoses();
	void compXf(const double sv[]);
	void InitializeLinkIAndZs();//const vector<double>& _state);
	void ComputeLinkAbsoluteVelocities(const double sv[]);//const vector<double>& _state);
///////////////////////////////////////////////////////////////////////////////
	void ComputeLinkFramesAndComputeVelocities(const double t=0, const double x[]=0, bool bUpdate=false);//const vector<double>& _state);
	void ComputeLinkIAndZs(const double sv[]);
	void ComputeLinkAccelerations(const double t, const double x[], double dx[]);//const vector<double>& _state);
	friend void CVodeMultibodyDerivative(CVODE::integer N,
							  CVODE::real t,
							  CVODE::N_Vector stateVector,
							  CVODE::N_Vector dx,
							  void *f_data);
///////////////////////////////////////////////////////////////////////////////
};

void CVodeMultibodyDerivative(CVODE::integer N,
						  CVODE::real t,
						  CVODE::N_Vector stateVector,
						  CVODE::N_Vector dx,
						  void *f_data);



////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

#endif // !defined(AFX_RIGIDBODYMANAGER_H__5A0C56D8_53D9_4EF6_B4A4_C49A2C98A63E__INCLUDED_)

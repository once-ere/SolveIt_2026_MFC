// Contact.h: interface for the Contact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTACT_H__6968F661_2568_11D4_A704_0050045B99C4__INCLUDED_)
#define AFX_CONTACT_H__6968F661_2568_11D4_A704_0050045B99C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CInteractingPair;

#include "RigidBody.h"
#include "PQP_Sphere.h"
#include "ExaminePair.h"
#include "State.h"

namespace SolveIt {

class CRigidBody;
class CExaminePair;


class Contact  
{
public:
/*
    // Use of the OLE allocator is required if the resultant Unicode
    // wstring will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
	void* operator new(unsigned int nSize) {
		return CoTaskMemAlloc(nSize);
	}
	void operator delete(void* pObj) {
		CoTaskMemFree(pObj);
	}
*/
///////////////////////////////////////////////////////////////////////////////
public:
	virtual ~Contact();
	Contact(CRigidBody* body1=0, CRigidBody* body2=0, bool bShowContacts=false);
	CExaminePair* m_pCExaminePair;
//	auto_ptr<CExaminePair> m_pCExaminePair;

	static unsigned long m_InteractingPairKey;
	unsigned long m_lKey;
	double m_uz;

	enum TypeRelativeNormalMotion {
		Stationary,
		Separating,
		Colliding,
		Sliding
	} m_RelativeNormalMotion;

	enum ContactType {
		Separated,
		InContact,
		Penetrating
	} m_ContactType;


	CRigidBody* m_body1;
	CRigidBody* m_body2;
//	Point3D contact_point;
//	CRigidBody* m_altbody1;
//	CRigidBody* m_altbody2;
	bool m_bPermute;

	Vector3D normal;
	vector<Vector3D> m_listNormals;
	vector<TriangleVerts> m_listTriangles1;
	vector<TriangleVerts> m_listTriangles2;


	Vector3D m_CollisionImpulse;
	Vector3D impulseLab;
	Vector3D m_vecRelVelocityLocal;
	Vector3D m_vecRelVelocityLab;
	Vector3D changeInRelativeVelocityLocal;
	Vector3D changeInRelativeVelocityLab;

	Vector3D m_ContactForce;
	double m_fContactForce;
	double m_fNonContactAndInertialAcceleration;
// if only (among) the closest points:
	Point3D m_pointContact1;	// closest point on 1; contact point during a binary collision 
	Point3D m_pointContact2;	// closest point on 2; contact point during a binary collision
	Point3D m_pointContact() { return 0.5*(m_pointContact1+m_pointContact2);}
	void Reset()
	{
		m_pointContact1 = ZERO_3D;
		m_pointContact2 = ZERO_3D;
	}
///////////////////////////////////////////////////////////////////////////////
	vector<Contact*> m_CollidingContactsList;
	void ClearCollisionContactsLists();

	double m_estimatedCollisionTime;
//approximate time of collision since beginning of last time step;

	Vector3D m_nonContactAcceleration1;
	Vector3D m_nonContactAcceleration2;
	void ComputeNonContactAndInertialAccelerations();

//	double m_fProposedTimeStep;
	double m_separation;

	void PQP_Update();
	void SpheresOnly_Update();
	void OnDraw();
	void clear();
	void ExaminePair();
	void CloseExaminePair();

	void ComputeRelativeVelocityLab();
	Vector3D NormalRelativeVelocityLab();
	Vector3D ChangeInNormalRelativeVelocityLab(const Vector3D& v = ZERO_3D, double scale = 0);

	bool IsCollision();

	Contact(const Contact& c) :
		m_body1(c.m_body1),
		m_body2(c.m_body2),
		m_pCExaminePair(c.m_pCExaminePair),
//		m_altbody1(c.m_altbody1),
//		m_altbody2(c.m_altbody2),
		m_bPermute(c.m_bPermute),
		normal(c.normal),
		m_separation(c.m_separation),
		m_estimatedCollisionTime(c.m_estimatedCollisionTime),
		m_pointContact1(c.m_pointContact1),
		m_pointContact2(c.m_pointContact2),
		m_ContactForce(c.m_ContactForce),
		m_RelativeNormalMotion(c.m_RelativeNormalMotion),
		m_ContactType(c.m_ContactType)
	{}

	Contact& operator=(const Contact& c)
	{
		m_body1					= c.m_body1;
		m_body2					= c.m_body2;
		m_pCExaminePair			= c.m_pCExaminePair;
//		m_altbody1				= c.m_altbody1;
//		m_altbody2				= c.m_altbody2;
		m_bPermute				= c.m_bPermute;
		normal					= c.normal;
		m_separation			= c.m_separation;
		m_estimatedCollisionTime= c.m_estimatedCollisionTime;
		m_pointContact1			= c.m_pointContact1;
		m_pointContact2			= c.m_pointContact2;
		m_ContactForce			= c.m_ContactForce;
		m_RelativeNormalMotion	= c.m_RelativeNormalMotion;
		m_ContactType			= c.m_ContactType;
		return *this;
	}
	Vector3D ComputeD_Dt_of_LocalZaxis();


	TypeRelativeNormalMotion ClassifyRelativeNormalMotion(double t);
	ContactType ClassifyInteraction(double t = 0.0);


	Mat3 R;		// frame transformation matrix
	Mat3 iR;	// transpose of frame transformation matrix
	Mat3 K;		// collision matrix
	Mat3 iK;	// collision matrix inverse
	Mat3 tK;	// collision matrix transformed to local frame
	Mat3 tiK;	// collision matrix inverse transformed to local frame


//	CExamine/*Collision*/ *m_pCExamineCollision;
	static UINT s_ExamineCollisionID;
///////////////////////////////////////////////////////////////////////////////
//
	double Compute_b_VectorComponent(Contact& c);

	long ResolveCollisions(double t);
	long RefineContact(Contact& contact, double t);

	void DetermineRestingContactPointsAndEstimatedCollisionTimes(double t, vector<double>& estimatedCollisionTimes);

	long OnCollision(const Contact& c);
//	long OnCollision(const Vector3D& point1, const Vector3D& point2, const Vector3D& localZaxis);
	Vector3D GetImpulse(const Contact& c);


	void ComputeCollisionMatrix();
//	void ComputeCollisionMatrix(const Vector3D& point1, const Vector3D& point2);

	void ComputeRotation();


//	void ComputeRelativeVelocityLab();
	void ComputeRelativeVelocityLocal();
//	Vector3D ComputeRelativeVelocity(const Vector3D& point1, const Vector3D& point2);
	long ComputeChangeInRelativeVelocity();
//	long ComputeChangeInRelativeVelocity(const Contact& c);

	void ComputeImpulse();
	void ComputeMomentumChanges(const Vector3D& impulseLab, const Contact& c);

	double coefficientOfKineticFriction();
	double coefficientOfRestitution();
	double StableFriction();

};


///////////////////////////////////////////////////////////////////////////////
struct ContactPtrsCompare
{  
	bool operator()(const Contact* a, const Contact* b) const
	{
		return a->m_separation < b->m_separation;
	}
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

struct ContactsCompare
{  
	bool operator()(const Contact& a, const Contact& b) const
	{
		return a.m_separation < b.m_separation;
	//	return fabs(a.separation) < fabs(b.separation);
	}
};
///////////////////////////////////////////////////////////////////////////////

} // namespace SolveIt

#endif // !defined(AFX_CONTACT_H__6968F661_2568_11D4_A704_0050045B99C4__INCLUDED_)

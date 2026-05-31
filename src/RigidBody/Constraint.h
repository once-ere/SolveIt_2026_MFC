// Constraint.h: interface for the CConstraint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSTRAINT_H__0E20B08B_A5FB_4D35_AEA3_BA43B956793E__INCLUDED_)
#define AFX_CONSTRAINT_H__0E20B08B_A5FB_4D35_AEA3_BA43B956793E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
//#include "RigidBody.h"
/*
#undef min
#undef max
#include "../SolveIt.Lib/lib/tnt/tnt.h"
#include "../SolveIt.Lib/lib/tnt/jama_qr.h"
#include "../SolveIt.Lib/lib/tnt/jama_cholesky.h"
#include "../SolveIt.Lib/lib/tnt/jama_eig.h"
#include "../SolveIt.Lib/lib/tnt/jama_svd.h"
///////////////////////////////////////////////////////////////////////////////
*/
namespace SolveIt
{

class CRigidBody;
///////////////////////////////////////////////////////////////////////////////
struct BodyCompareEqual
{
	CRigidBody* b;
	BodyCompareEqual(CRigidBody* _b): b(_b){}
	bool operator()(const CRigidBody* a) const
	{
		return a == b;
	}
};
///////////////////////////////////////////////////////////////////////////////


class CConstraint  
{
public:
	CConstraint();
	virtual ~CConstraint();

public:
	CConstraint(long Key, const wstring& strName) : m_Key(Key), name(strName),
	m_bDeactivateIfPositive(false),
	m_bDeactivateIfNegative(false),
	m_fNumericalPositionDamping(0),
	m_fNumericalVelocityDamping(0),
	m_bActive(true)
	{}

	CConstraint(const CConstraint& c) : m_Key(c.m_Key), name(c.name),
	m_bDeactivateIfPositive(c.m_bDeactivateIfPositive),
	m_bDeactivateIfNegative(c.m_bDeactivateIfNegative),
	m_fNumericalPositionDamping(c.m_fNumericalPositionDamping),
	m_fNumericalVelocityDamping(c.m_fNumericalVelocityDamping),
	m_bActive(c.m_bActive)
	{}

	CConstraint& operator=(const CConstraint& c) {
		m_Key=c.m_Key, name=c.name;
		m_bDeactivateIfPositive=c.m_bDeactivateIfPositive;
		m_bDeactivateIfNegative=c.m_bDeactivateIfNegative;
		m_fNumericalPositionDamping=c.m_fNumericalPositionDamping;
		m_fNumericalVelocityDamping=c.m_fNumericalVelocityDamping;
		m_bActive=c.m_bActive;
		return *this;
	}

	virtual Point3D		GetConsistentPosition(CRigidBody* body, const Point3D& pt) {return Point3D(ZERO_3D);}
	virtual Vector3D	GetConsistentVelocity(CRigidBody* body, const Point3D& pt) {return ZERO_3D;}

	virtual void DrawState()=0;
	virtual void Error   (double t, vector<double>& vecError)=0;

	virtual void Jacobian(	double t,
							vector<double>& vecRowOfJacobian
							)=0;

	virtual void NormalizedJacobian(	double t,
							vector<double>& vecRowOfJacobian
							)=0;

	virtual void HessianDotVelocity(double t,
							vector<double>& vecRowOfHessianDotVelocity
							)=0;

///////////////////////////////////////////////////////////////////////////////
//	virtual void AddConstraintForce(TNT::Array1D<double>& f, bool bDuringCollision = false)=0;
//	virtual void AddConstraintImpulse(TNT::Array1D<double>& f)=0;
///////////////////////////////////////////////////////////////////////////////
	virtual void AddConstraintForce(vector<double>& f, bool bDuringCollision = false)=0;
	virtual void AddConstraintImpulse(vector<double>& f)=0;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


	long m_Key;
	wstring name;

	bool m_bDeactivateIfPositive;
	bool m_bDeactivateIfNegative;
	bool m_bActive;
	bool Active() {return m_bActive;}
///////////////////////////////////////////////////////////////////////////////
//numerical damping:
	double m_fNumericalPositionDamping;
	double m_fNumericalVelocityDamping;

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CONSTRAINT_H__0E20B08B_A5FB_4D35_AEA3_BA43B956793E__INCLUDED_)

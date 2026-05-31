// PulleyConstraint.h: interface for the CPulleyConstraint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PULLEYCONSTRAINT_H__2ADFEA3E_CC8A_4BFA_ABFA_8AF503F8296E__INCLUDED_)
#define AFX_PULLEYCONSTRAINT_H__2ADFEA3E_CC8A_4BFA_ABFA_8AF503F8296E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Constraint.h"

namespace SolveIt {


class CPulleyConstraint   : public CConstraint
{
public:
	CPulleyConstraint(
									CRigidBody* const leftBody,
									CRigidBody* const pulley,
									CRigidBody* const rightBody,
									int nLeftBodyConnectedInQuadrant,
									int nRightBodyConnectedInQuadrant
									);
	virtual ~CPulleyConstraint();

	void DrawState();
	void Error   (double t, vector<double>& vecError);
	void Jacobian(	double t,
					vector<double>& vecRowOfJacobian
					);

	void NormalizedJacobian(	double t,
					vector<double>& vecRowOfJacobian
					);

	void HessianDotVelocity(double t,
							vector<double>& vecRowOfHessianDotVelocity
							);

	void AddConstraintForce(vector<double>& f, bool bDuringCollision = false);
	void AddConstraintImpulse(vector<double>& f);

//	CRigidBody* const m_leftBody;
	CRigidBody* m_leftBody;
	CRigidBody* const m_pulley;
	CRigidBody* m_rightBody;
//	CRigidBody* const m_rightBody;
	int m_nLeftBodyConnectedInQuadrant;
	int m_nRightBodyConnectedInQuadrant;
};

} // namespace SolveIt

#endif // !defined(AFX_PULLEYCONSTRAINT_H__2ADFEA3E_CC8A_4BFA_ABFA_8AF503F8296E__INCLUDED_)

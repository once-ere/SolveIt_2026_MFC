// AddConstraint.cpp : Implementation of CConstraintManager

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
//#include "PtToPtConstraint.h"
//#include "RigidBody.h"
#include "ConstraintManager.h"
#include "parser.h" /* for token definitions and yylval */
#include "RigidBodyManager.h"
//#include "parser_2020.h" /* for token definitions and yylval */
#include "MainFrm.h"
///////////////////////////////////////////////////////////////////////////////
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
	long ToLongSize(size_t value)
	{
		return static_cast<long>(value);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddLinearConstraint(
													CRigidBody* body,
													const Vector3D& normal,
													const Point3D& point
													)
{
	try
	{
		CLinearConstraint * c = new CLinearConstraint(body, normal, point);
		m_vecLinearConstraints.push_back(c);
	}
	catch( ... )
	{
		::MessageBoxW(NULL,L"AddLinearConstraint",L"exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddQuadraticConstraint(
													CRigidBody* body,
													const Vector3D& normal2,
													const Vector3D& normal1,
													double scalar,
													bool bNoNegateGradientForNormal
													)
{
	try
	{
		CQuadraticConstraint * c = new CQuadraticConstraint(body, normal2, normal1, scalar, bNoNegateGradientForNormal);
		m_vecQuadraticConstraints.push_back(c);
	}
	catch( ... )
	{
		::MessageBoxW(NULL,L"AddQuadraticConstraint",L"exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddTwoPointConstraint(
													CRigidBody* body1,
													const Point3D& point1,
													CRigidBody* body2,
													const Point3D& point2,
													double _separation,
													const wstring* options
													)
{
	try
	{
	CPointToPointConstraint * c = 
							new CPointToPointConstraint(	body1, point1,
															body2, point2,
															_separation);
	m_vecTwoPointConstraints.push_back(c);
///////////////////////////////////////////////////////////////////////////////
//static const wstring _zeroVelocity("zero velocity");
/*
static const wstring _positive("positive");
static const wstring _negative("negative");
	c->m_Key = m_keyCount++;
		bool bDeactivateIfPositive	= false;
		bool bDeactivateIfNegative	= false;
		if (options && *options == _positive) bDeactivateIfNegative = true;
		if (options && *options == _negative) bDeactivateIfPositive = true;
*/
		bool bZeroVelocity			= false;
		if (options && *options == _zeroVelocity) bZeroVelocity = true;

	if (bZeroVelocity)
	{
	m_vecZeroVelocityAtFixedPointConstraints.push_back(c);
	}
	}

	catch( ... )
	{
		::MessageBoxW(NULL,L"AddTwoPointConstraint",L"exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::JacobianMatrixForTwoPointConstraints(vector<vector_of_Vector3D>& v, double t)
{
	;
	long n_rows = ToLongSize(m_vecTwoPointConstraints.size());
	if (n_rows == 0) return;
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	long n_cols = ToLongSize(rigidBodyManager->size());
	if (n_cols == 0) return;
	vector<CPointToPointConstraint*>::iterator it;
	for (it = m_vecTwoPointConstraints.begin(); it != m_vecTwoPointConstraints.end(); ++it)
	{
		vector_of_Vector3D row;
		CPointToPointConstraint& c = *(*it);
		Point3D pt1 = c.body1? c.body1->LocalToLab(c.point1):c.point1;
		Point3D pt2 = c.body2? c.body2->LocalToLab(c.point2):c.point2;
// this is valid only FOR POINT MASSES:
//	if (c.body1) ASSERT(c.body1->x == pt1);
//	if (c.body2) ASSERT(c.body2->x == pt2);
		Vector3D rel = (pt1 - pt2);
		if (c.separation > 0.01e-12)
			rel /= c.separation;
		for (ObjectsList::iterator it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it )
		{
			CRigidBody* body	= dynamic_cast<CRigidBody *>((*it).rbPtr);
			if (body==c.body1)
			{
				row.push_back(rel);
			}
			else
			if (body==c.body2)
			{
				row.push_back(-rel);
			}
			else
			{
				row.push_back(ZERO_3D);
			}
		}
		v.push_back(row);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::JacobianMatrixForQuadraticConstraints(vector<vector_of_Vector3D>& J, double t)
{
	long n_rows = ToLongSize(m_vecQuadraticConstraints.size());
	if (n_rows == 0) return;
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	long n_cols = ToLongSize(rigidBodyManager->size());
	if (n_cols == 0) return;

	vector<CQuadraticConstraint*>::iterator it;
	for (it = m_vecQuadraticConstraints.begin(); it != m_vecQuadraticConstraints.end(); ++it)
	{
		vector_of_Vector3D row;
		CQuadraticConstraint& c = *(*it);
		for (ObjectsList::iterator it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it )
		{
			CRigidBody* body	= dynamic_cast<CRigidBody *>((*it).rbPtr);
			if (body==c.body)
			{
				Vector3D gradient = 2.0*(c.normal2.times(body->x))+c.normal1;
				row.push_back((c.bNoNegateGradientForNormal? gradient:-gradient));
			}
			else
			{
				row.push_back(ZERO_3D);
			}
		}
		J.push_back(row);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::JacobianMatrixForLinearConstraints(vector<vector_of_Vector3D>& J, double t)
{
	long n_rows = ToLongSize(m_vecLinearConstraints.size());
	if (n_rows == 0) return;
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	long n_cols = ToLongSize(rigidBodyManager->size());
	if (n_cols == 0) return;

	vector<CLinearConstraint*>::iterator it;
	for (it = m_vecLinearConstraints.begin(); it != m_vecLinearConstraints.end(); ++it)
	{
		vector_of_Vector3D row;
		CLinearConstraint& c = *(*it);
		for (ObjectsList::iterator it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it )
		{
			CRigidBody* body	= dynamic_cast<CRigidBody *>((*it).rbPtr);
			if (body==c.body)
			{
				row.push_back(c.normal);
			}
			else
			{
				row.push_back(ZERO_3D);
			}
		}
		J.push_back(row);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::HessianKineticEnergyTwoPoint(vector<double>& e, double t)
{
	long n_rows = ToLongSize(m_vecTwoPointConstraints.size());
	vector<CPointToPointConstraint*>::iterator it;
	for (it = m_vecTwoPointConstraints.begin(); it != m_vecTwoPointConstraints.end(); ++it)
	{
		CPointToPointConstraint& c = *(*it);
		Point3D		pt1 = c.body1? c.body1->LocalToLab(c.point1):c.point1;
		Point3D		pt2 = c.body2? c.body2->LocalToLab(c.point2):c.point2;
// this is valid only FOR POINT MASSES:
//	if (c.body1) ASSERT(c.body1->x == pt1);
//	if (c.body2) ASSERT(c.body2->x == pt2);
		Vector3D	v1	= c.body1? c.body1->ComputeVelocityOfPointInBody(pt1):ZERO_3D;
		Vector3D	v2	= c.body2? c.body2->ComputeVelocityOfPointInBody(pt2):ZERO_3D;
// this is valid only FOR POINT MASSES:
//	if (c.body1) ASSERT(c.body1->m_fMassInverse*c.body1->p == v1);
//	if (c.body2) ASSERT(c.body2->m_fMassInverse*c.body2->p == v2);
		Vector3D	V1	= v1;
		Vector3D	V2	= v2;
		if (c.separation > 0.01e-12)
		{
			Vector3D	rel = (pt1 - pt2)/c.separation;
			v1	-= (rel*v1)*rel;
			v2	-= (rel*v2)*rel;
			v1	/= c.separation;
			v2	/= c.separation;
			e.push_back((V1-V2)*(v1-v2));
		}
		else
		{
			e.push_back((V1-V2)*(v1-v2));
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::HessianKineticEnergyLinear(vector<double>& e, double t)
{
	long n_rows = ToLongSize(m_vecLinearConstraints.size());
	if (n_rows == 0) return;
	vector<CLinearConstraint*>::iterator it;
	for (it = m_vecLinearConstraints.begin(); it != m_vecLinearConstraints.end(); ++it)
	{
			e.push_back(0);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::HessianKineticEnergyQuadratic(vector<double>& e, double t)
{
	long n_rows = ToLongSize(m_vecQuadraticConstraints.size());
	if (n_rows == 0) return;
	vector<CQuadraticConstraint*>::iterator it;
	for (it = m_vecQuadraticConstraints.begin(); it != m_vecQuadraticConstraints.end(); ++it)
	{
		CQuadraticConstraint& c = *(*it);
		double E = 2.0*c.body->m_fMassInverse*c.body->m_fMassInverse*(c.normal2*((c.body->p.times(c.body->p))));
		e.push_back((c.bNoNegateGradientForNormal? E:-E));
	}
}
///////////////////////////////////////////////////////////////////////////////

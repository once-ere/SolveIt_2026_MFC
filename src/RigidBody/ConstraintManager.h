// ConstraintManager.h: interface for the CConstraintManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSTRAINTMANAGER_H__E68F67C9_BB0D_4593_A9C8_BA4B4B0BBC25__INCLUDED_)
#define AFX_CONSTRAINTMANAGER_H__E68F67C9_BB0D_4593_A9C8_BA4B4B0BBC25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#include "Types.h"
#include "Mat3.h"
#include "Constraint.h"
//#include "PointMassConstraint.h"
//#include "SurfaceConstraint.h"
//#include "PtToPtConstraint.h"
///////////////////////////////////////////////////////////////////////////////
#include <functional>
#include <algorithm>


namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const wstring _positive(L"positive");
const wstring _negative(L"negative");
const wstring _zeroVelocity(L"zero velocity");
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
class CPointToPointConstraint
{
public:
//	virtual ~CPointToPointConstraint();
public:
	CRigidBody*const body1;
	CRigidBody*const body2;
	Point3D point1;		//local coords if body1 != 0; lab coords otherwise
	Point3D point2;		//local coords if body2 != 0; lab coords otherwise
	double separation;
	double eval(const Point3D& point);
	void DrawState();


	CPointToPointConstraint(CRigidBody *const _body1,
							const Point3D& _point1,
							CRigidBody *const _body2,
							const Point3D& _point2,
							double _separation=0):
	body1(_body1),body2(_body2),
	point1(_point1),point2(_point2),
	separation(_separation){}

};
///////////////////////////////////////////////////////////////////////////////
class CLinearConstraint
{
public:
	CRigidBody* body;
	const Vector3D normal;
	const Point3D point;
	const double scalar;
	CLinearConstraint(CRigidBody* _body, const Vector3D& _normal, const Point3D& _point):
		body(_body), normal(_normal), point(_point), scalar(normal*point){}

	double eval(const Point3D& pt){return normal*(pt - point);}
	void DrawState();
};
///////////////////////////////////////////////////////////////////////////////
class CQuadraticConstraint
{
public:
	CRigidBody* body;
	const Vector3D normal2;
	const Vector3D normal1;
	const double scalar;
	const bool bNoNegateGradientForNormal;

	bool m_bDrawn;

	CQuadraticConstraint(CRigidBody* _body, const Vector3D& _normal2, const Vector3D& _normal1, double s, bool yn):
		body(_body), normal2(_normal2), normal1(_normal1), scalar(s), bNoNegateGradientForNormal(yn), m_bDrawn(false){}
	
	double eval(const Point3D& pt){return pt*((normal2.times(pt))+normal1) + scalar;}
	bool eval_z(const double& x, const double& y, double& z1, double& z2);
	bool eval_y(const double& x, const double& z, double& y1, double& y2);
	bool eval_x(const double& y, const double& z, double& x1, double& x2);
	void DrawState();
};
///////////////////////////////////////////////////////////////////////////////
class CPulleyConstraint;
///////////////////////////////////////////////////////////////////////////////
static const string constraint_prefix("CnstrndBDY_");

typedef map< unsigned long, string>	ConstraintKeyToName;
typedef map< string, unsigned long>	ConstraintNameToKey;
typedef map< unsigned long,  CConstraint* >	KeyToConstraint_List;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct pair<CRigidBody*, CConstraint*> PAIR_RigidBody_Constraint;
typedef vector<PAIR_RigidBody_Constraint> ListOf_RigidBody_Constraint_Pairs;

typedef struct pair<CRigidBody*, ListOfConstraints> PAIR_RigidBody_ConstraintList;
typedef vector<PAIR_RigidBody_ConstraintList> ListOf_RigidBody_ConstraintList_Pairs;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
class CConstraintManager : public vector<KeyToConstraint_List*>
{
public:
	CConstraintManager();
	virtual ~CConstraintManager();
// [# Two-Point Constraints] by [3* (# RigidBodies)]
//	vector<vector_of_Vector3D> JacobianMatrixForTwoPointConstraints(double t);
	void JacobianMatrix(vector<vector_of_Vector3D>& J, double t)
	{
		JacobianMatrixForTwoPointConstraints(J, t);
		JacobianMatrixForLinearConstraints( J, t);
		JacobianMatrixForQuadraticConstraints( J, t);
	}
	void JacobianMatrixForTwoPointConstraints(vector<vector_of_Vector3D>& J, double t);
	void JacobianMatrixForLinearConstraints(vector<vector_of_Vector3D>& J, double t);
	void JacobianMatrixForQuadraticConstraints(vector<vector_of_Vector3D>& J, double t);

	vector<CPointToPointConstraint*> m_vecTwoPointConstraints;
	vector<CPointToPointConstraint*> m_vecZeroVelocityAtFixedPointConstraints;
	vector<CLinearConstraint*> m_vecLinearConstraints;
	vector<CQuadraticConstraint*> m_vecQuadraticConstraints;

	vector<CPulleyConstraint*> m_vecPulleyConstraints;
	
	void HessianKineticEnergy(vector<double>& e, double t)
	{//kepp same order as JacobianMatrix:
		HessianKineticEnergyTwoPoint( e, t);
		HessianKineticEnergyLinear( e, t);
		HessianKineticEnergyQuadratic( e, t);
	}
	void HessianKineticEnergyTwoPoint(vector<double>& e, double t);
	void HessianKineticEnergyLinear(vector<double>& e, double t);
	void HessianKineticEnergyQuadratic(vector<double>& e, double t);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void AddPointToPointConstraint(
						//			CRigidBody* const body1,
									CRigidBody* body1,
									const Point3D& point1,
									CRigidBody* body2,
									const Point3D& point2,
									double _separation=0,
									bool _bDeactivateIfPositive = false,
									bool _bDeactivateIfNegative = false
									);
///////////////////////////////////////////////////////////////////////////////
	void AddPointToPointConstraint(
						//			CRigidBody* const body1,
									CRigidBody* body1,
									const Point3D& point1,
									CRigidBody* body2,
									const Point3D& point2,
									double _separation,
									const wstring& option
									);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void AddTwoPointConstraint(
													CRigidBody* body1,
													const Point3D& point1,
													CRigidBody* body2,
													const Point3D& point2,
													double _separation,
													const wstring* options=0
													);
///////////////////////////////////////////////////////////////////////////////
	void AddLinearConstraint(
													CRigidBody* body,
													const Vector3D& normal,
													const Point3D& point
													);
///////////////////////////////////////////////////////////////////////////////
	void AddQuadraticConstraint(
													CRigidBody* body,
													const Vector3D& normal2,
													const Vector3D& normal1,
													double scalar,
													bool bNoNegateGradientForNormal
													);
///////////////////////////////////////////////////////////////////////////////
	void AddPulleyConstraint(
									CRigidBody* leftBody,
									CRigidBody* pulley,
									CRigidBody* rightBody,
									int nLeftBodyConnectedInQuadrant,
									int nRightBodyConnectedInQuadrant
									);
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void PulleyConstraintForce(const double& fRadius,
											   const double& x_right, const double& y_right,
											   const double& Vx_right, const double& Vy_right,
											   const double& Fx_right, const double& Fy_right,
											   const double& x_left, const double& y_left,
											   const double& Vx_left, const double& Vy_left,
											   const double& Fx_left, const double& Fy_left,
											   const double& mass_right, const double& mass_left,
											   double& Jx, double& Jy, double& JX, double& JY, double& lambda
											   );
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	vector_of_Vector3D m_vec3SystemVelocity;
	vector_of_Vector3D m_vec3SystemInverseMassMatrix;
	vector_of_Vector3D m_vec3UnconstrainedResultantForce;
	vector_of_Vector3D m_vec3ConstraintForce;
	vector_of_Vector3D m_vec3UnconstrainedCollisionImpulse;
///////////////////////////////////////////////////////////////////////////////
//numerical damping:
	vector_of_Vector3D m_vec3NumericalPositionDamping;//per body
	vector_of_Vector3D m_vec3NumericalVelocityDamping;//per body

	vector_of_Vector3D m_vec3NumericalDampingPosition;//per constraint
	vector_of_Vector3D m_vec3NumericalDampingVelocity;//per constraint
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void OnDraw();
	void Clear();
/////////////////////////////////////////////////////////////////////////////
	void ListConstraints();
	void FindConstraints(const wstring& name, vector<CConstraint*>& vec);
/////////////////////////////////////////////////////////////////////////////////
	ConstraintNameToKey m_nameToKey;
	unsigned long m_keyCount;
	void Remove(const wstring& name);
///////////////////////////////////////////////////////////////////////////////
	bool Empty() {
		bool bEmpty = true;
		vector<KeyToConstraint_List*>::iterator i;
		for (i = begin(); i != end(); ++i)
		{
			KeyToConstraint_List& k = *(*i);
			bEmpty &= k.empty();
		}
		return bEmpty;
		}
///////////////////////////////////////////////////////////////////////////////
	long Size(){
		long n = 0;
		vector<KeyToConstraint_List*>::iterator i;
		for (i = begin(); i != end(); ++i)
		{
			KeyToConstraint_List& k = *(*i);
			n += static_cast<long>(k.size());
		}
		return n;
	}
///////////////////////////////////////////////////////////////////////////////
	long dim(){
		return static_cast<long>(size());
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	map< unsigned long,  CConstraint*> m_listPointMassConstraints;
	map< unsigned long,  CConstraint*> m_listNonPointMassConstraints;
	map< unsigned long,  CConstraint*> m_listPulleyConstraints;
	map< unsigned long,  CConstraint*> m_listSurfaceConstraints;
	map< unsigned long,  CConstraint*> m_listZeroVelocityAtFixedPoint;

	vector<CRigidBody*> m_vecBodies;
	vector<CRigidBody*> m_vecBodiesCurrentTimeStep;
	vector<CConstraint*> m_vecConstraints;//for drawing
	vector<CConstraint*> m_vecUniqueConstraints;
///////////////////////////////////////////////////////////////////////////////
	ListOf_RigidBody_Constraint_Pairs m_listAll;
	ListOf_RigidBody_Constraint_Pairs m_listCurrentTimeStep;
	ListOf_RigidBody_ConstraintList_Pairs  m_listOf_RigidBody_ConstraintList_Pairs_CurrentTimeStep;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	ListOf_RigidBody_Constraint_Pairs::iterator Find(PAIR_RigidBody_Constraint bc)
	{
		if (m_listAll.empty()) return m_listAll.end();
		PAIR_RigidBody_ConstraintCompareEqual _PAIR_RigidBody_ConstraintCompareEqual(bc);
		ListOf_RigidBody_Constraint_Pairs::iterator i = find_if(m_listAll.begin(), m_listAll.end(), _PAIR_RigidBody_ConstraintCompareEqual);
		return i;
	}
///////////////////////////////////////////////////////////////////////////////
	vector<CRigidBody*>::iterator FindBody(CRigidBody* body)
	{
		if (body == 0) return m_vecBodies.end();
		if (m_vecBodies.empty()) return m_vecBodies.end();
		BodiesCompareEqual bodiesCompareEqual(body);
		vector<CRigidBody*>::iterator i = find_if(m_vecBodies.begin(), m_vecBodies.end(), bodiesCompareEqual);
		return i;
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool FindActiveBody(CRigidBody* body)
	{
		if (body == 0) return false;
		if (m_vecBodiesCurrentTimeStep.empty()) return false;
		BodiesCompareEqual bodiesCompareEqual(body);
		vector<CRigidBody*>::iterator i = find_if(m_vecBodiesCurrentTimeStep.begin(), m_vecBodiesCurrentTimeStep.end(), bodiesCompareEqual);
		return i!=m_vecBodiesCurrentTimeStep.end();
	}
///////////////////////////////////////////////////////////////////////////////
	bool AddBodyToVecList(CRigidBody* body)
	{
		if (body == 0) return false;
		vector<CRigidBody*>::iterator i = FindBody(body);
		if (i == m_vecBodies.end())
		{
			m_vecBodies.push_back(body);
			return true;
		}
		return false;
	}
///////////////////////////////////////////////////////////////////////////////
//	bool AddBodyToVecList(CRigidBody* body, CConstraint* c)
//	{
//		if (body == 0) return false;
//		vector<CRigidBody*>::iterator i = FindBody(body);
//		if (i == m_vecBodies.end())
//		{
//			m_vecBodies.push_back(body);
//			m_vecConstraints.push_back(c);
//			return true;
//		}
//		return false;
//	}
///////////////////////////////////////////////////////////////////////////////
	bool AddConstraintToVecList(CConstraint* pc)
	{
		if (pc == 0) return false;
		CompareConstraintToMe compareConstraintToMe(pc);
		vector<CConstraint*>::iterator i = find_if(m_vecConstraints.begin(), m_vecConstraints.end(), compareConstraintToMe);
		if (i == m_vecConstraints.end())
		{
			m_vecConstraints.push_back(pc);
			return true;
		}
		return false;
	}
///////////////////////////////////////////////////////////////////////////////
	bool RemoveBodyFromVecList(CRigidBody* body)
	{
		if (body == 0) return false;
		if (m_vecBodies.empty()) return false;
		vector<CRigidBody*>::iterator i = FindBody(body);
		if (i != m_vecBodies.end())
		{
	//		long n = distance(m_vecBodies.begin(), i);
	//		vector<CConstraint*>::iterator j = find(m_vecConstraints.begin(), m_vecConstraints.end(), m_vecConstraints[n]);
	//		if (j != m_vecConstraints.end()) m_vecConstraints.erase(j);
			m_vecBodies.erase(i);
			return true;
		}
		return false;
	}
///////////////////////////////////////////////////////////////////////////////
	bool Active(long n = -1) {
		if (n == -1)
		{
			bool bActive = true;
			vector<CConstraint*>::iterator i;
			for (i = m_vecUniqueConstraints.begin(); i != m_vecUniqueConstraints.end(); ++i)
			{
				CConstraint& k = *(*i);
				bActive &= k.Active();
			}
			return bActive;
		}
		vector<CConstraint*>::iterator j = find(m_vecUniqueConstraints.begin(), m_vecUniqueConstraints.end(), m_vecUniqueConstraints[n]);
		if (j != m_vecUniqueConstraints.end()) return (*j)->Active();
		return false;
	}
///////////////////////////////////////////////////////////////////////////////
//	bool DeactivateIfPositive(long n) {
//		vector<CConstraint*>::iterator j = find(m_vecConstraints.begin(), m_vecConstraints.end(), m_vecConstraints[n]);
//		if (j != m_vecConstraints.end()) return (*j)->m_bDeactivateIfPositive;
//		return false;
//	}
/////////////////////////////////////////////////////////////////////////////////
//	bool DeactivateIfNegative(long n) {
//		vector<CConstraint*>::iterator j = find(m_vecConstraints.begin(), m_vecConstraints.end(), m_vecConstraints[n]);
//		if (j != m_vecConstraints.end()) return (*j)->m_bDeactivateIfNegative;
//		return false;
//	}
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	void GetUnconstrainedResultantForce(vector<double>& F);//TNT::Vector<double>& F);
	void GetSystemVelocity(vector<double>& V);//TNT::Vector<double>& V);
//	void GetSystemInverseMassMatrix(TNT::Matrix<double>& M);


	long GetNumberOfIndependentVariables();

	long AssignVarsToColumns(bool bDuringCollision = false);
	void ImposeConstraints(double t, bool bDuringCollision = false);
	bool ImposeZeroVelocityAtFixedPoint(double t, bool bDuringCollision);
	void InitializePulleyConstraints(CPulleyConstraint *const c, bool bDuringCollision = false);
	void ImposePulleyConstraints(double t, bool bDuringCollision = false);
///////////////////////////////////////////////////////////////////////////////

	void CalculateConsistentPosition(CRigidBody* body = 0);
	void CalculateConsistentVelocity(CRigidBody* body = 0);

//	void AddConstraintForces(TNT::Vector<double>& f, bool bDuringCollision = false);
///////////////////////////////////////////////////////////////////////////////
	void GetNumericalDamping(vector<double>& pos, vector<double>& vel);//TNT::Vector<double>& pos, TNT::Vector<double>& vel);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	vector<double> m_vecSystemVelocity;
	vector<double> m_vecSystemInverseMassMatrix;
	vector<double> m_vecUnconstrainedResultantForce;
	vector<double> m_vecUnconstrainedCollisionImpulse;
///////////////////////////////////////////////////////////////////////////////
//numerical damping:
	vector<double> m_vecNumericalPositionDamping;//per body
	vector<double> m_vecNumericalVelocityDamping;//per body

	vector<double> m_vecNumericalDampingPosition;//per constraint
	vector<double> m_vecNumericalDampingVelocity;//per constraint


	map<int, double*> col2var;
	map<double*, int, MyCompare> var2col;//myless<const double*>

	long m_NumberOfIndependentVariables;

	vector<CRigidBody *> m_ConstraintedBodies;


};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CONSTRAINTMANAGER_H__E68F67C9_BB0D_4593_A9C8_BA4B4B0BBC25__INCLUDED_)

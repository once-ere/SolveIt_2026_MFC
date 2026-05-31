// PointToPointConstraint.h: interface for the CPointToPointConstraint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTTOPOINTCONSTRAINT_H__E61FC46A_B778_401E_84B1_6592C4BADA56__INCLUDED_)
#define AFX_POINTTOPOINTCONSTRAINT_H__E61FC46A_B778_401E_84B1_6592C4BADA56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointMassConstraint.h"
///////////////////////////////////////////////////////////////////////////////
/*
To declare the object pointed to by the pointer as const:
const    wchar_t *cpch;
To declare the value of the pointer(that is, the actual address stored in the pointer) as const:
wchar_t * const    pchc;
*/
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{


struct PointToPointUser_Data {
	Point3D point1;		//local coords if body1 != 0; lab coords otherwise
	Point3D point2;		//local coords
	CRigidBody*const body1;	//may be null
	CRigidBody*const body2;	//never null
	double separation2;// separation²
	PointToPointUser_Data(	CRigidBody *const b1,
							const Point3D& p1,
							CRigidBody *const b2,
							const Point3D& p2,
							double separation):
		body1(b1),body2(b2),point1(p1),point2(p2),
		separation2(separation*separation)
	{}
};

class CPointToPointConstraint   : public CPointMassConstraint	
{
public:
//	CPointToPointConstraint();
	virtual ~CPointToPointConstraint();
public:
	CRigidBody*const body1;
	CRigidBody*const body2;
	Point3D point1;		//local coords if body1 != 0; lab coords otherwise
	Point3D point2;		//local coords if body2 != 0; lab coords otherwise
	double separation;
	double separation2;// separation²

//	PointToPointUser_Data ud;

	CPointToPointConstraint(CRigidBody *const body1,
							const Point3D& point1,
							CRigidBody *const body2,
							const Point3D& point2,
							double _separation=0);

	double eval(const Point3D& point);
	void Destroy ();
	void DrawState();

	void RollWithoutSlipping();


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


	void Jacobian(	double t,
					vector<double>& vecRowOfJacobian,
					vector<int>& cols,
					map<double *, int, MyCompare>& var2col
					);
	void D_Jacobian_dt(	double t,
							vector<double>& vecRowOfJacobian,
							vector<int>& cols,
							map<double *, int, MyCompare>& var2col
							);
//	void Hessian (double t, Coord_Mat_double& A, int row, int& col, int& depth);
	long GetNumberOfConstrainedBodies();
	void AssignVarsToColumns(	map<int, double *>& col2var,
								map<double *, int, MyCompare>& var2col,
								int& col
								);

	void AddConstraintForce(vector<double>& f,
							map<double *, int, MyCompare>& var2col
							);

	void AddConstraintImpulse(vector<double>& f,
							map<double *, int, MyCompare>& var2col
							);

	void AssignVarsToColumnsDuringCollision(	map<int, double *>& col2var,
								map<double *, int, MyCompare>& var2col,
								int& col
								);
//helper functions:
	Vector3D DPointToPointConstraint_dx2();
	Vector3D DPointToPointConstraint_dx1();
	double PointToPointConstraint();

	Point3D		GetConsistentPosition(CRigidBody* body, const Point3D& pt);
	Vector3D	GetConsistentVelocity(CRigidBody* body, const Point3D& pt);


};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_POINTTOPOINTCONSTRAINT_H__E61FC46A_B778_401E_84B1_6592C4BADA56__INCLUDED_)

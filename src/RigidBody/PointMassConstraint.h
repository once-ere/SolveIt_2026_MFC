// PointMassConstraint.h: interface for the CPointMassConstraint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTMASSCONSTRAINT_H__3B83322E_04CB_4E7E_BC8B_B3CA4BBC6A67__INCLUDED_)
#define AFX_POINTMASSCONSTRAINT_H__3B83322E_04CB_4E7E_BC8B_B3CA4BBC6A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
#include "RigidBody.h"
#include "Constraint.h"
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
//using namespace std;
//using namespace TNT;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CPointMassConstraint  : public CConstraint
{
public:
	CPointMassConstraint();
	virtual ~CPointMassConstraint();
	virtual void Jacobian(	double t,
							vector<double>& vecRowOfJacobian
							)=0;
	virtual void NormalizedJacobian(	double t,
							vector<double>& vecRowOfJacobian
							)=0;

	virtual void HessianDotVelocity(double t,
							vector<double>& vecRowOfHessianDotVelocity
							)=0;


//	virtual void AddConstraintForce(TNT::Array1D<double>& f, bool bDuringCollision = false)=0;
//	virtual void AddConstraintImpulse(TNT::Array1D<double>& f)=0;
///////////////////////////////////////////////////////////////////////////////
	virtual void AddConstraintForce(vector<double>& f, bool bDuringCollision = false)=0;
	virtual void AddConstraintImpulse(vector<double>& f)=0;

	virtual void Destroy  ()=0;
	virtual void DrawState()=0;
	virtual void Error   (double t, vector<double>& vecError)=0;
	virtual void Jacobian(	double t,
							vector<double>& vecRowOfJacobian,
							vector<int>& cols,
							map<double *, int, MyCompare>& var2col
							)=0;
	virtual void D_Jacobian_dt(	double t,
							vector<double>& vecRowOfJacobian,
							vector<int>& cols,
							map<double *, int, MyCompare>& var2col
							)=0;
//	virtual void Hessian (double t, Coord_Mat_double& A, int row, int& col, int& depth)=0;
	virtual long GetNumberOfConstrainedBodies()=0;
	virtual void AssignVarsToColumns(	map<int,
										double *>& col2var,
										map<double *, int,MyCompare >& var2col,
										int& col
										)=0;
	virtual void AddConstraintForce(vector<double>& f,
							map<double *, int, MyCompare>& var2col
							)=0;

	virtual void AddConstraintImpulse(vector<double>& f,
							map<double *, int, MyCompare>& var2col
							)=0;
	virtual void AssignVarsToColumnsDuringCollision(	map<int, double *>& col2var,
								map<double *, int, MyCompare>& var2col,
								int& col
								)=0;


};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_POINTMASSCONSTRAINT_H__3B83322E_04CB_4E7E_BC8B_B3CA4BBC6A67__INCLUDED_)

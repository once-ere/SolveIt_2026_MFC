// OdeSolverCollision.h: interface for the COdeSolverCollision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODESOLVERCOLLISION_H__88CA1A48_6F5A_11D3_BF01_00C04F79DBA3__INCLUDED_)
#define AFX_ODESOLVERCOLLISION_H__88CA1A48_6F5A_11D3_BF01_00C04F79DBA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RigidBodyManager.h"
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

/*
double tK[45][3] ->
double tK[  ][3] -> 
double (*tK)[3]
*/
struct UserDataCollision
{
	double coefficientOfKineticFriction;
	double (*tK)[3];	// collision matrix transformed to local frame
	long bCompression;
};

class COdeSolverCollision  
{
public:
	COdeSolverCollision();
	virtual ~COdeSolverCollision();
	long CollisionCVODE(double& ux, double& uy, double& uz,
						double& Wz, double& Jz,
						double coefficientOfKineticFriction,
						double coefficientOfRestitution,
						double (*tK)[3]);

	friend 
	void CollisionJac1(CVODE::integer N, CVODE::DenseMat J, CVODE::RhsFn f, void *f_data, CVODE::real tn,
		 CVODE::N_Vector x, CVODE::N_Vector fy, CVODE::N_Vector ewt, CVODE::real h, CVODE::real uround,
		 void *jac_data, long int *nfePtr, CVODE::N_Vector vtemp1,
		 CVODE::N_Vector vtemp2, CVODE::N_Vector vtemp3);

	friend
		void CollisionDerivs(CVODE::integer N, CVODE::real t, CVODE::N_Vector x, CVODE::N_Vector xdot, void *f_data);

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_ODESOLVERCOLLISION_H__88CA1A48_6F5A_11D3_BF01_00C04F79DBA3__INCLUDED_)

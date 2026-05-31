// ImposeConstraints.cpp: implementation of the CConstraintManager class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#pragma optimize( "p", on ) 

#include "stdafx.h"
#include "SolveIt.h"
#include "ConstraintManager.h"
#include "RigidBodyManager.h"
#include "Contact.h"
#include "System.h"
//#include "PtToPtConstraint.h"
//#include "PulleyConstraint.h"
#include <functional>
#include <algorithm>
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
#if defined(UseFortran)
const long SVD_NMAX = 128;
extern "C" long				DATA_SVD_mp_SVD_NMAX;
extern "C" long				DATA_SVD_mp_NUMCONSTRAINTS;
extern "C" void _stdcall	DATA_SVD_mp_INIT();
extern "C" void _stdcall	DATA_SVD_mp_TERM();
extern "C" void _stdcall	DATA_SVD_mp_MYSVD();
extern "C" double			DATA_SVD_mp_FVALUE;
extern "C" double			DATA_SVD_mp_A_[SVD_NMAX][SVD_NMAX];
extern "C" double			DATA_SVD_mp_B_[SVD_NMAX];
extern "C" double			DATA_SVD_mp_FORCE[SVD_NMAX];


extern "C" void _stdcall DATA_SVD_mp_EVOLVE_NASH(long* StepsPerIteration);
extern "C" double _stdcall DATA_SVD_mp_EVALTENSORPRODUCTSPLINE(double* x, double* y);

typedef double (*FORT)[SVD_NMAX];
#endif
/*
*/
///////////////////////////////////////////////////////////////////////////////
// CConstraintManager

/*
-++++++++�_���a�GpSs�tFTOd8fen=�==()�����vn���� � � � � �
????????????????????????

FOR POINT MASSES:

C_�			= 0
C_�,j x_j'	= 0

Let	C_�,j = C_�j = Transpose{C_j�}

C_�,jk x_j' x_k' + C_�,j x_j''		= 0
W = 1 / M
C_�,jk x_j' x_k' + C_�,j W_jk F_k	= 0

Introduce constraint-enforcing Q_j:
C_�,jk x_j' x_k' + C_�,j W_jk { F_k + Q_k }			= 0
Apply the 'Principle of Virtual Work':	Q_j x_j'	= 0 :
=> 
Q_j = �_� C_�,j
Whence
C_�,jk x_j' x_k' + C_�,j W_jk { F_k + �_�' C_�',k }	= 0

Generalized constraint forces = �_�

Let	T_��'	= C_�,j W_jk C_�',k = C_�j W_jk C_k�'	(	=	W_��')

T_��' �_�'	= -C_�,jk x_j' x_k' - C_�,j W_jk F_k
--OR--
W_�	= W_��' �_�'	= -C_�,jk x_j' x_k' - C_�,j W_jk F_k
W_�	+ C_�,jk x_j' x_k' + C_�,j W_jk F_k = 0

Collision:
C_�,jk �[x_j' dx_k + x_k' dx_j ] + C_�,j W_jk { F_k + �_�' C_�',k } dt	= 0

Let	J_k = collision impulse
	H_k = constraint impulse = h_� C_�,k

IF  Integral[C_�,jk x_j'] dx_k = 0 THEN
C_�,j W_jk { J_k + h_�' C_�',k } 	= 0
T_��' h_�'	=  - C_�,j W_jk J_k

*/
///////////////////////////////////////////////////////////////////////////////
struct TwoPointConstrainedRigidBodyCompareEqual
{
	TwoPointConstrainedRigidBodyCompareEqual(const CRigidBody* _b) : b(_b) {}
	const CRigidBody* b;
	bool operator()(const CPointToPointConstraint * a) const
	{
		return ((a->body1 == b)||(a->body2 == b));
	}
///////////////////////////////////////////////////////////////////////////////
	bool get_BodyPoint3D(const CRigidBody* _b, vector<CPointToPointConstraint*>& vecTwoPointConstraints, Point3D& pt)
	{
		bool ret = false;
		if (_b==0) return ret;
		if (vecTwoPointConstraints.empty()) return ret;
		TwoPointConstrainedRigidBodyCompareEqual twoPointConstrainedRigidBodyCompareEqual(_b);
		vector<CPointToPointConstraint*>::iterator it = find_if(vecTwoPointConstraints.begin(), vecTwoPointConstraints.end(), twoPointConstrainedRigidBodyCompareEqual);
		if (it == vecTwoPointConstraints.end ()) return ret;
		CPointToPointConstraint& c = *(*it);
		if (_b == c.body1) pt = c.body1->LocalToLab(c.point1), ret=true;
		if (_b == c.body2) pt = c.body2->LocalToLab(c.point2), ret=true;
		return ret;
	}
///////////////////////////////////////////////////////////////////////////////
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct LinearConstrainedRigidBodyCompareEqual
{
	LinearConstrainedRigidBodyCompareEqual(const CRigidBody* _b) : b(_b) {}
	const CRigidBody* b;
	bool operator()(const CLinearConstraint * a) const
	{
		return ((a->body == b));
	}
///////////////////////////////////////////////////////////////////////////////
	CLinearConstraint * get(const CRigidBody* _b, vector<CLinearConstraint*>& vecConstraints)
	{
		if (_b==0) return 0;
		if (vecConstraints.empty()) return 0;
		LinearConstrainedRigidBodyCompareEqual t(_b);
		vector<CLinearConstraint*>::iterator it = find_if(vecConstraints.begin(), vecConstraints.end(), t);
		if (it == vecConstraints.end ()) return 0;
		return (*it);
	}
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct QuadraticConstrainedRigidBodyCompareEqual
{
	QuadraticConstrainedRigidBodyCompareEqual(const CRigidBody* _b) : b(_b) {}
	const CRigidBody* b;
	bool operator()(const CQuadraticConstraint * a) const
	{
		return ((a->body == b));
	}
///////////////////////////////////////////////////////////////////////////////
	CQuadraticConstraint * get(const CRigidBody* _b, vector<CQuadraticConstraint*>& vecConstraints)
	{
		if (_b==0) return 0;
		if (vecConstraints.empty()) return 0;
		QuadraticConstrainedRigidBodyCompareEqual t(_b);
		vector<CQuadraticConstraint*>::iterator it = find_if(vecConstraints.begin(), vecConstraints.end(), t);
		if (it == vecConstraints.end ()) return 0;
		return (*it);
	}
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::ImposeConstraints(double t, bool bDuringCollision)
{
//	AssignVarsToColumns(bDuringCollision);//sets m_NumberOfIndependentVariables, m_vecSystemInverseMassMatrix, m_vecSystemVelocity
	ImposePulleyConstraints(t, bDuringCollision);
	if (ImposeZeroVelocityAtFixedPoint(t, bDuringCollision)) return;

	long nSizeTwoPointConstraints	= ToLongSize(m_vecTwoPointConstraints.size());
	long nSizeLinearConstraints		= ToLongSize(m_vecLinearConstraints.size());
	long nSizeQuadraticConstraints	= ToLongSize(m_vecQuadraticConstraints.size());
	long nSize = nSizeTwoPointConstraints+nSizeLinearConstraints+nSizeQuadraticConstraints;

	if (nSize == 0) return;
	
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	if (rigidBodyManager->size() == 0) return;

	vector<vector_of_Vector3D> J;
	JacobianMatrix( J, t);


	ASSERT(nSize == J.size());
	ASSERT(rigidBodyManager->size() == J[0].size());

	vector_of_Vector3D& W	= m_vec3SystemInverseMassMatrix;
	vector_of_Vector3D& f	= m_vec3UnconstrainedResultantForce;
//	vector_of_Vector3D& v	= m_vec3SystemVelocity;
//	v.clear();
//		v.push_back(body->m_fMassInverse*body->p);
//				vel = body->ComputeVelocityOfPointInBody(pt);

	W.clear();
	f.clear();
	m_vec3ConstraintForce.clear();

	const Vector3D	U3D=I_3D+J_3D+K_3D;
	for (ObjectsList::iterator it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it )
	{
		CRigidBody* body	= dynamic_cast<CRigidBody *>((*it).rbPtr);
		TwoPointConstrainedRigidBodyCompareEqual twoPointConstrainedRigidBodyCompareEqual(body);
		W.push_back(body->m_fMassInverse*U3D);
// this is valid only FOR POINT MASSES:ZERO_3D
		Vector3D& F=bDuringCollision?  body->m_vecImpulseLab: body->m_vecForce;
		Point3D pt;
		bool ret = twoPointConstrainedRigidBodyCompareEqual.get_BodyPoint3D(body, m_vecTwoPointConstraints, pt);
		Vector3D acceleration	= ret? body->ComputeNonContactAndInertialAcceleration(pt):body->m_fMassInverse*F;
	//	f.push_back(body->m_fMassInverse*F);//use acceleration
		f.push_back(acceleration);//use acceleration
		m_vec3ConstraintForce.push_back(ZERO_3D);
	}
	vector<vector_of_Vector3D> JW;
	vector<vector_of_Vector3D>::iterator r;
	for (r = J.begin(); r != J.end(); ++r)
	{
		vector_of_Vector3D& row = (*r);
	//	for (vector_of_Vector3D::iterator i =row.begin(); i != row.end(); ++i)
	//	{
	//		 	Vector3D& v = (*i);
	//	}
		vector_of_Vector3D	rowW	= row.times(W);
		JW.push_back(rowW);
	}
/*
	for (r = JW.begin(); r != JW.end(); ++r)
	{
		vector_of_Vector3D& row = (*r);
		for (vector_of_Vector3D::iterator i =row.begin(); i != row.end(); ++i)
		{
			 	Vector3D& v = (*i);
		}
	}
*/
	//auto_ptr<double> bbuf( new double[nSize] );
	std::unique_ptr<double> bbuf = std::make_unique<double>(nSize);
	double* b = bbuf.get();

	vector<double> e;
	HessianKineticEnergy(e, t);

	ASSERT(e.size() == J.size());
	for ( r = J.begin(); r != J.end(); ++r)
	{
		vector_of_Vector3D& row = (*r);
		int Row = static_cast<int>(distance(J.begin(), r));
		b[Row]	= -(row.dot(f)) - e[Row];
	}



//T = J*W*tJ;
	double ** T = CVODE::denalloc(nSize);
	if (T == NULL) throw SolveIt_ERROR(L"CConstraintManager::ImposeConstraints: denalloc memory request failed");
	CVODE::denzero(T,nSize);
	for ( r = JW.begin(); r != JW.end(); ++r)
	{
		vector_of_Vector3D& row = (*r);
		int Row = static_cast<int>(distance(JW.begin(), r));
		for (vector<vector_of_Vector3D>::iterator c = J.begin(); c != J.end(); ++c)
		{
			vector_of_Vector3D&	col	= *c;
			int Col		= static_cast<int>(distance(J.begin(), c));
			T[Col][Row]	= row.dot(col);
		}
	}
	if (10)//nSize==1)
	{
		if (T[0][0] == 0.0) throw SolveIt_ERROR(L"CVODE::J*W*tJ == 0");
		b[0] /= T[0][0];
	}
	else
	{
		CVODE::integer *pivot = CVODE::denallocpiv(nSize);
		if (pivot == NULL) throw SolveIt_ERROR(L"denallocpiv memory request failed");

		CVODE::integer ier = CVODE::gefa(T, nSize, pivot);
		if (ier > 0)
		{
			throw SolveIt_ERROR(L"CVODE::gefa error");
		}
		if (ier == 0) CVODE::gesl(T, nSize, pivot, b);
		CVODE::denfreepiv(pivot);
	}

	for (r = J.begin(); r != J.end(); ++r)
	{
		int Row = static_cast<int>(distance(J.begin(), r));
		vector_of_Vector3D row = (*r);
		double ConstraintForce = b[Row];
		if (Row>=nSizeTwoPointConstraints && ConstraintForce < 0.0) ConstraintForce = 0.0;
		row *= ConstraintForce;
		m_vec3ConstraintForce += row;
	}


	CVODE::denfree(T);

	for (auto it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it )
	{
		int col = static_cast<int>(distance(rigidBodyManager->begin(), it));
		CRigidBody* body	= dynamic_cast<CRigidBody *>((*it).rbPtr);
		Vector3D& F=bDuringCollision?  body->m_vecImpulseLab: body->m_vecForce;
		F += m_vec3ConstraintForce[col];
		body->m_vecContactForce		+= m_vec3ConstraintForce[col];
///////////////////////////////////////////////////////////////////////////////
		LinearConstrainedRigidBodyCompareEqual L(body);
		QuadraticConstrainedRigidBodyCompareEqual Q(body);
		CLinearConstraint*		cL	= L.get(body, m_vecLinearConstraints);
		CQuadraticConstraint*	cQ	= Q.get(body, m_vecQuadraticConstraints);
///////////////////////////////////////////////////////////////////////////////
	if ((cQ||cL) && body->m_fCoefficientOfStaticFriction != 0.0||body->m_fCoefficientOfKineticFriction != 0.0)
	{
///////////////////////////////////////////////////////////////////////////////
		double		R			= body->m_fRadius;
		Vector3D	normal;
			if (cQ)
			{
				Vector3D gradient = 2.0*(cQ->normal2.times(body->x))+cQ->normal1;
				normal = (cQ->bNoNegateGradientForNormal? gradient:-gradient);
			}
			if (cL)
			{
				normal = cL->normal;
			}
		normal.normalize();
#if 0
		Vector3D force,  torque;
		const Vector3D& input_force	= body->m_vecForce;
		const Vector3D& input_torque= body->m_vecTorque;

		Point3D point=body->x - R*normal;
		body->GetForceAndTorqueForZeroAccelerationAtFixedPoint( point,  input_force,  input_torque,  force,  torque);
		double Fnorm = m_vec3ConstraintForce[col].norm();
		force = normal.vectorProd(force.vectorProd(normal));
		torque = normal.vectorProd(torque.vectorProd(normal));
		if (force.norm()<= body->m_fCoefficientOfStaticFriction*Fnorm)
		{
			body->m_vecForce			+= force;
			body->m_vecContactForce		+= force;
			force.normalize();
			torque = force.vectorProd(torque.vectorProd(force));
			body->m_vecTorque			+= torque;
			body->m_vecContactTorque	+= torque;
		}
		else
		{
			force.normalize();
			torque = force.vectorProd(torque.vectorProd(force));
			force						*= body->m_fCoefficientOfKineticFriction*Fnorm;
			torque.normalize();
			torque						*= body->m_fCoefficientOfKineticFriction*R*Fnorm;
			body->m_vecForce			+= force;
			body->m_vecTorque			+= torque;
			body->m_vecContactForce		+= force;
			body->m_vecContactTorque	+= torque;
		}
#endif
#if 10
		Vector3D	velocity;
		body->get_Velocity(velocity);
//approx. to velocity at END of time step:
		velocity	+=body->m_fMassInverse*theApp.GetSystem()->m_dt*body->m_vecForce;
//approx. to angular velocity at end of time step if no slipping occurs:
		Vector3D	w			= normal.vectorProd(velocity/R);
		Mat3		ii;
		body->get_InverseInertiaTensor(ii);
		ii.invert();
//approx. to angular momentum about CM at end of time step if no slipping occurs:
		Vector3D	L			= ii*w;
		Vector3D Constraint_torque((L - body->L)/theApp.GetSystem()->m_dt);
		Vector3D InclineFrictionalForce(normal.vectorProd(Constraint_torque/R));
		double Fnorm = m_vec3ConstraintForce[col].norm();
		if (InclineFrictionalForce.norm()<= body->m_fCoefficientOfStaticFriction*Fnorm)
		{
			body->m_vecForce			+= InclineFrictionalForce;
			body->m_vecContactForce		+= InclineFrictionalForce;
			body->m_vecTorque			+= Constraint_torque;
			body->m_vecContactTorque	+= Constraint_torque;
		}
		else
		{
			InclineFrictionalForce.normalize();
			InclineFrictionalForce		*= body->m_fCoefficientOfKineticFriction*Fnorm;
			Constraint_torque.normalize();
			Constraint_torque			*= body->m_fCoefficientOfKineticFriction*R*Fnorm;
			body->m_vecForce			+= InclineFrictionalForce;
			body->m_vecContactForce		+= InclineFrictionalForce;
			body->m_vecTorque			+= Constraint_torque;
			body->m_vecContactTorque	+= Constraint_torque;
		}
#endif

	}
	}
}
///////////////////////////////////////////////////////////////////////////////
//physical  pendulums
bool CConstraintManager::ImposeZeroVelocityAtFixedPoint(double t, bool bDuringCollision)
{
	if (bDuringCollision) return false;//not yet implemented 
//	if (!Active()) return false;
	if (m_vecZeroVelocityAtFixedPointConstraints.size() == 0) return false;

	vector<CPointToPointConstraint*>::iterator row ;
	for (row = m_vecZeroVelocityAtFixedPointConstraints.begin(); row != m_vecZeroVelocityAtFixedPointConstraints.end(); ++row)
	{
		CPointToPointConstraint* ppmc = (*row);//dynamic_cast<CPointToPointConstraint*>(row->second);
		if (ppmc)
		{
			Point3D		pt1			= ppmc->body1? ppmc->body1->LocalToLab(ppmc->point1):ppmc->point1;
			Point3D		pt2			= ppmc->body2? ppmc->body2->LocalToLab(ppmc->point2):ppmc->point2;
			if (ppmc->body1)
			{
				ppmc->body1->AddForceAndTorqueForZeroAccelerationAtFixedPoint(pt1);
			}
			if (ppmc->body2)
			{
				ppmc->body2->AddForceAndTorqueForZeroAccelerationAtFixedPoint(pt2);
			}
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////

// UserDefinedExternalForce.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "instruction.h"
#include "System.h"
#include "StackMachine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CRigidBody

Vector3D CRigidBody::Compute_UserDefinedExternalForce(double t)
{
	Vector3D udf;
	if (m_fMassInverse==0.0) return udf;
#if 10
	double mass = 1.0/m_fMassInverse;
	Vector3D velocityCM	= m_fMassInverse*p;
	vector<double> args;

	if (m_uUserDefinedExternalForceX.func || m_uUserDefinedExternalForceY.func || m_uUserDefinedExternalForceZ.func)
	{
		try {
			double Fx=0, Fy=0, Fz=0;
			double X=x.x, Y=x.y, Z=x.z;
			double vX=velocityCM.x, vY=velocityCM.y, vZ=velocityCM.z;
			if (m_uUserDefinedExternalForceX.func)
			{
				switch(m_uUserDefinedExternalForceX.m_bIsTimeDependent)
				{
				case false:
					switch(m_uUserDefinedExternalForceX.nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(X);
						break;
					case 2:
						args.push_back(X);args.push_back(vX);
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
//		LoadAndCallRealFunc(&m_uUserDefinedExternalForceX, args);
				case true:
					switch(m_uUserDefinedExternalForceX.nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(X);
						break;
					case 3:
						args.push_back(t);
						args.push_back(X);args.push_back(vX);
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				}
				LoadAndCallRealFunc(&m_uUserDefinedExternalForceX, args);
				Fx = CStackMachine::receiveStackTopDoubleValue;
			}
			if (m_uUserDefinedExternalForceY.func)
			{
				switch(m_uUserDefinedExternalForceY.m_bIsTimeDependent)
				{
				case false:
					switch(m_uUserDefinedExternalForceY.nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(Y);
						break;
					case 2:
						args.push_back(Y);args.push_back(vY);
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedExternalForceY.nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(Y);
						break;
					case 3:
						args.push_back(t);
						args.push_back(Y);args.push_back(vY);
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				}
				LoadAndCallRealFunc(&m_uUserDefinedExternalForceY, args);
				Fy = CStackMachine::receiveStackTopDoubleValue;
			}
			if (m_uUserDefinedExternalForceZ.func)
			{
				switch(m_uUserDefinedExternalForceZ.m_bIsTimeDependent)
				{
				case false:
					switch(m_uUserDefinedExternalForceZ.nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(Z);
						break;
					case 2:
						args.push_back(Z);args.push_back(vZ);
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedExternalForceZ.nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(Z);
						break;
					case 3:
						args.push_back(t);
						args.push_back(Z);args.push_back(vZ);
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				}
				LoadAndCallRealFunc(&m_uUserDefinedExternalForceZ, args);
				Fz = CStackMachine::receiveStackTopDoubleValue;
			}
			udf = Vector3D(Fx, Fy, Fz);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
#endif
	return udf;
}
///////////////////////////////////////////////////////////////////////////////




Vector3D CRigidBody::Compute_UserDefinedInternalForce(double t)
{
	Vector3D udf;
	if (m_fMassInverse==0.0) return udf;
//	CSystem& m_system = theApp.GetSystem();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
#if 0
	double Fx=0, Fy=0, Fz=0;
	double mass = 1.0/m_fMassInverse;

	Vector3D velocityCM	= m_fMassInverse*p;
	long bodyOffset = rigidBodyManager->GetOffsetOfBody(this);

	long nBodies = rigidBodyManager->size();
	long numberVariables = DEGREES_FREEDOM*nBodies;
	vector<double> aVector;
	aVector.reserve(numberVariables);
	rigidBodyManager->GetStateOfBodies(aVector);


	if (m_uUserDefinedInternalForceX.func || m_uUserDefinedInternalForceY.func || m_uUserDefinedInternalForceZ.func)
	{
		try {
			if (m_uUserDefinedInternalForceX.func)
			{
				Fx = CallUserDefined(__InternalForceX, bodyOffset, nBodies, (long*) &aVector);
			}
			if (m_uUserDefinedInternalForceY.func)
			{
				Fy = CallUserDefined(__InternalForceY, bodyOffset, nBodies, (long*) &aVector);
			}
			if (m_uUserDefinedInternalForceZ.func)
			{
				Fz = CallUserDefined(__InternalForceZ, bodyOffset, nBodies, (long*) &aVector);
			}

			udf = Vector3D(Fx, Fy, Fz);
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			throw Syntax("Compute_UserDefinedInternalForce failed");
		}
	}
#endif
	return udf;
}
///////////////////////////////////////////////////////////////////////////////
double CRigidBody::Compute_UserDefinedPotential(double t)
{
	double U=0;
	if (m_fMassInverse==0.0) return U;
	double mass = 1.0/m_fMassInverse;
	Vector3D velocityCM	= m_fMassInverse*p;
	vector<double> args;

	if (m_uUserDefinedPotential.func)
	{
		try {
			double X=x.x, Y=x.y, Z=x.z;
			double vX=velocityCM.x, vY=velocityCM.y, vZ=velocityCM.z;

			switch(m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				case false:
					switch(m_uUserDefinedPotential.nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(X);
						break;
					case 2:
						args.push_back(X);args.push_back(vX);
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedPotential.nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(X);
						break;
					case 3:
						args.push_back(t);
						args.push_back(X);args.push_back(vX);
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						args.push_back(vX);args.push_back(vY);args.push_back(vZ);
						break;
					}
				break;
				}
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				U = CStackMachine::receiveStackTopDoubleValue;
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
	return U;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double CRigidBody::Compute_UserDefinedPotential(const double& t, const double& X,const double& Y,const double& Z)
{
	double U=0;
	if (m_fMassInverse==0.0) return U;
	double mass = 1.0/m_fMassInverse;
	Vector3D velocityCM	= m_fMassInverse*p;
	vector<double> args;

	if (m_uUserDefinedPotential.func)
	{
		try {
			switch(m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				case false:
					switch(m_uUserDefinedPotential.nargs)
					{
					case 0:
						break;
					case 1:
						args.push_back(X);
						break;
					case 2:
						return U;
						break;
					case 3:
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 6:
						return U;
					}
				break;
				case true:
					switch(m_uUserDefinedPotential.nargs)
					{
					case 1:
						args.push_back(t);
						break;
					case 2:
						args.push_back(t);
						args.push_back(X);
						break;
					case 3:
						args.push_back(t);
						return U;
						break;
					case 4:
						args.push_back(t);
						args.push_back(X);args.push_back(Y);args.push_back(Z);
						break;
					case 7:
						return U;
					}
				break;
				}
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				U = CStackMachine::receiveStackTopDoubleValue;
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
		}
	}
	return U;
}
///////////////////////////////////////////////////////////////////////////////



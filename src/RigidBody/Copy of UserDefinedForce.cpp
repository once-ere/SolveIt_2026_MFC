// UserDefinedExternalForce.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "RigidBodyManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CRigidBody

Vector3D CRigidBody::Compute_UserDefinedExternalForce(double t)
{
	Vector3D udf;
	if (m_fMassInverse==0.0) return udf;
#if 10
	double mass = 1.0/m_fMassInverse;
	Vector3D velocityCM	= m_fMassInverse*p;

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
						Fx = CallUserDefined(__ExternalForceX);
						break;
					case 1:
						Fx = CallUserDefined(__ExternalForceX , X);
						break;
					case 2:
						Fx = CallUserDefined(__ExternalForceX , X, vX);
						break;
					case 3:
						Fx = CallUserDefined(__ExternalForceX , X,Y,Z);
						break;
					case 6:
						Fx = CallUserDefined(__ExternalForceX , X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedExternalForceX.nargs)
					{
					case 1:
						Fx = CallUserDefined(__ExternalForceX , t );
						break;
					case 2:
						Fx = CallUserDefined(__ExternalForceX , t, X );
						break;
					case 3:
						Fx = CallUserDefined(__ExternalForceX , t, X, vX );
						break;
					case 4:
						Fx = CallUserDefined(__ExternalForceX , t, X,Y,Z );
						break;
					case 7:
						Fx = CallUserDefined(__ExternalForceX , t, X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				}
			}
			if (m_uUserDefinedExternalForceY.func)
			{
				switch(m_uUserDefinedExternalForceY.m_bIsTimeDependent)
				{
				case false:
					switch(m_uUserDefinedExternalForceY.nargs)
					{
					case 0:
						Fy = CallUserDefined(__ExternalForceY);
						break;
					case 1:
						Fy = CallUserDefined(__ExternalForceY , Y);
						break;
					case 2:
						Fy = CallUserDefined(__ExternalForceY , Y, vY);
						break;
					case 3:
						Fy = CallUserDefined(__ExternalForceY , X,Y,Z);
						break;
					case 6:
						Fy = CallUserDefined(__ExternalForceY , X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedExternalForceY.nargs)
					{
					case 1:
						Fy = CallUserDefined(__ExternalForceY , t );
						break;
					case 2:
						Fy = CallUserDefined(__ExternalForceY , t, Y );
						break;
					case 3:
						Fy = CallUserDefined(__ExternalForceY , t, Y, vY );
						break;
					case 4:
						Fy = CallUserDefined(__ExternalForceY , t, X,Y,Z );
						break;
					case 7:
						Fy = CallUserDefined(__ExternalForceY , t, X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				}
			}
			if (m_uUserDefinedExternalForceZ.func)
			{
				switch(m_uUserDefinedExternalForceZ.m_bIsTimeDependent)
				{
				case false:
					switch(m_uUserDefinedExternalForceZ.nargs)
					{
					case 0:
						Fz = CallUserDefined(__ExternalForceZ);
						break;
					case 1:
						Fz = CallUserDefined(__ExternalForceZ , Z);
						break;
					case 2:
						Fz = CallUserDefined(__ExternalForceZ , Z, vZ);
						break;
					case 3:
						Fz = CallUserDefined(__ExternalForceZ , X,Y,Z);
						break;
					case 6:
						Fz = CallUserDefined(__ExternalForceZ , X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				case true:
					switch(m_uUserDefinedExternalForceZ.nargs)
					{
					case 1:
						Fz = CallUserDefined(__ExternalForceZ , t );
						break;
					case 2:
						Fz = CallUserDefined(__ExternalForceZ , t, Z );
						break;
					case 3:
						Fz = CallUserDefined(__ExternalForceZ , t, Z, vZ );
						break;
					case 4:
						Fz = CallUserDefined(__ExternalForceZ , t, X,Y,Z );
						break;
					case 7:
						Fz = CallUserDefined(__ExternalForceZ , t, X,Y,Z, vX,vY,vZ);
						break;
					}
				break;
				}
			}

			udf = Vector3D(Fx, Fy, Fz);
		}
		catch(...)
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
	CRigidBodyManager&	rigidBodyManager = theApp.GetRigidBodyManager();
#if 0
	double Fx=0, Fy=0, Fz=0;
	double mass = 1.0/m_fMassInverse;

	Vector3D velocityCM	= m_fMassInverse*p;
	long bodyOffset = rigidBodyManager.GetOffsetOfBody(this);

	long nBodies = rigidBodyManager.size();
	long numberVariables = DEGREES_FREEDOM*nBodies;
	vector<double> aVector;
	aVector.reserve(numberVariables);
	rigidBodyManager.GetStateOfBodies(aVector);


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
		catch(...)
		{
			throw Syntax("Compute_UserDefinedInternalForce failed");
		}
	}
#endif
	return udf;
}


// ComputeExtForcesTorques.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "FieldsManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct SinusoidalForce {
	double m_fAmplitude;
	double m_fAngularFrequency;
	double m_fPhase;
	double& A;
	double& w;
	double& p;
	SinusoidalForce(double amp,double omega, double phi ) :
		A(m_fAmplitude),
		w(m_fAngularFrequency),
		p(m_fPhase),
		m_fAmplitude(amp),
		m_fAngularFrequency(omega),
		m_fPhase(phi)
	{}
	SinusoidalForce(const Vector3D& params) :
		A(m_fAmplitude),
		w(m_fAngularFrequency),
		p(m_fPhase),
		m_fAmplitude(params.x),
		m_fAngularFrequency(params.y),
		m_fPhase(params.z)
	{}
	;
	double force(double t) {
		return m_fAmplitude*cos(m_fAngularFrequency*t+m_fPhase);
	}
};

//typedef Vector3D (CALLBACK *MagneticFieldPtr) (CFieldsManager* pfm, const Point3D& p);
//MagneticFieldPtr MagneticField = 0;
//////////////////////////////////////////////////////////////////////
// CRigidBody

//	double Call(const wchar_t* f,..);
void CRigidBody::Compute_External_Force_and_Torque(double t)
{
//	Vector3D Tension(double length, double x, double y, double vx, double vy, double R, double g);
	if (m_fMassInverse==0.0) return;

	try{
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();

	Vector3D velocityCM	= m_fMassInverse*p;
	double mass = 1.0/m_fMassInverse;

///////////////////////////////////////////////////////////////////////////////
	Vector3D G = fieldsManager->Static_GravitationalField (x, this);
	Vector3D E = fieldsManager->ElectrostaticField (x, this);
	Vector3D B = fieldsManager->MagneticField(x, this);
	if (m_fCharge!=0.0)
	{
		Couple couple(m_fCharge*E);
		m_CoupleList.push_back(couple);
	}

	if (B.norm2()!=0.0)
	{
		Couple couple(m_fCharge*(velocityCM.vectorProd(B)));
		m_CoupleList.push_back(couple);
	}

	if (G.norm2()!=0.0)
	{
		Couple couple(mass*G);
		m_CoupleList.push_back(couple);
	}

	if (m_fMagneticDipoleMoment!=0.0 )
	{
		Vector3D	m = m_fMagneticDipoleMoment*m_vecMagneticDipoleOrientation;
		Couple couple;
		couple.torqe = m.vectorProd(B);
		m_CoupleList.push_back(couple);
	}
/////////////////////////////////////////////////////////////////////////////////



	if (m_uUserDefinedExternalForceX.func || m_uUserDefinedExternalForceY.func || m_uUserDefinedExternalForceZ.func)
	{
		Vector3D udf = Compute_UserDefinedExternalForce(t);
		Couple coupleExt(udf);
		m_CoupleList.push_back(coupleExt);
	}


	if (m_uUserDefinedInternalForceX.func || m_uUserDefinedInternalForceY.func || m_uUserDefinedInternalForceZ.func)
	{
		Vector3D udf = Compute_UserDefinedInternalForce(t);
		Couple coupleInt(udf);
		m_CoupleList.push_back(coupleInt);
	}
/*
	if (m_inboundJoint.jointType == Joint1DOF::SPRING)
	{
		const double smallDist = 0.01e-6;
		ListOf_springConstants_springLengths::iterator	ikL		= m_ListOf_springConstants_springLengths.begin();
		PairPoint3DVector::iterator						iptpt	= m_PairPoint3DVector.begin();
		vector<CRigidBody *>::iterator					irb		= m_bodiesConnectedTo.begin();

		Vector3D		f;

		for (
			irb		=	m_bodiesConnectedTo.begin(),
			ikL		=	m_ListOf_springConstants_springLengths.begin(),
			iptpt	=	m_PairPoint3DVector.begin();
			((irb		!=	m_bodiesConnectedTo.end())&&
			(ikL		!=	m_ListOf_springConstants_springLengths.end())&&
			(iptpt	!=	m_PairPoint3DVector.end()));
			++irb,
			++iptpt,
			++ikL
			)
		{
				CRigidBody*	body2	=	*irb;
				double&			k		= (*ikL).first;
				double&			L		= (*ikL).second;
				PairPoint3D&	pts		= *iptpt;
				Point3D&		point1	= pts.first;
				Point3D&		point2	= pts.second;
				Point3D			pt1		= LocalToLab(point1);
				Point3D			pt2		= body2? body2->LocalToLab(point2):point2;
				Vector3D		rel		= pt1 - pt2;
				double			dist	= rel.norm();
				if (L == 0.0)
				{
					f		= -k*rel;
				}
				else
				{
					double			K		= 1;
					if (dist > smallDist)
					{
						K	= -k*(1.0 - L/dist);
					}
					else
					{
						K		= -k*L;
						rel		= velocityCM - (body2? body2->m_fMassInverse*body2->p:ZERO_3D);
						rel.normalize();
					}
					f		= K*rel;
				}
				Couple couple(f, pt1-x);
			//	Couple couple(f);
				m_CoupleList.push_back(couple);
		}
	}
*/

	double& Ex1	= fieldsManager->m_ExternalElectrostaticFieldX1;
	double& Ex2	= fieldsManager->m_ExternalElectrostaticFieldX2;
	double& Ex3	= fieldsManager->m_ExternalElectrostaticFieldX3;

///////////////////////////////////////////////////////////////////////////////
	if (fieldsManager->m_bCentralForce_IsValid)
	{
		Vector3D rel = x;
		double r = pow(rel*rel,1.5);
		if (r>1.0e-30) {
			rel /= r;
			Couple couple((-mass*fieldsManager->m_fCentralForce)*rel);
			m_CoupleList.push_back(couple);
		}
	}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
	if (fieldsManager->m_bSinusoidalForce_Component_X_IsValid)
	{
		SinusoidalForce f(fieldsManager->m_SinusoidalForce_Component_X);//0.5, 2.0, 1.5707963267948966192313216916398);
		double fx = f.force(t);
	//	double fx = Call("SinusoidalForce",4, f.A, f.w, f.p, t);

		Couple couple;
		couple.force = fx*I_3D;
		m_CoupleList.push_back(couple);
	}

///////////////////////////////////////////////////////////////////////////////
	if (fieldsManager->m_bSinusoidalForce_Component_Y_IsValid)
	{
		SinusoidalForce f(fieldsManager->m_SinusoidalForce_Component_Y);//0.5, 2.0, 1.5707963267948966192313216916398);
		double fx = f.force(t);
	//	double fx = Call("SinusoidalForce",4, f.A, f.w, f.p, t);

		Couple couple;
		couple.force = fx*J_3D;
		m_CoupleList.push_back(couple);
	}

///////////////////////////////////////////////////////////////////////////////
	if (fieldsManager->m_bSinusoidalForce_Component_Z_IsValid)
	{
		SinusoidalForce f(fieldsManager->m_SinusoidalForce_Component_Z);//0.5, 2.0, 1.5707963267948966192313216916398);
		double fx = f.force(t);
	//	double fx = Call("SinusoidalForce",4, f.A, f.w, f.p, t);

		Couple couple;
		couple.force = fx*K_3D;
		m_CoupleList.push_back(couple);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (Ex1!=0.0 || Ex2!=0.0 || Ex3!=0.0 )
	{
		Couple couple((x*(Ex1 + Ex2*x + Ex3*x*x)*m_fCharge)*I_3D);
		m_CoupleList.push_back(couple);
	}
///////////////////////////////////////////////////////////////////////////////
	if (B.x!=0.0 || B.y!=0.0 || B.z!=0.0 )
	{
		Couple couple(m_fCharge*(velocityCM.vectorProd(B)));
		m_CoupleList.push_back(couple);
		if (m_fMagneticDipoleMoment!=0.0 )
		{
			Vector3D	m = m_fMagneticDipoleMoment*m_vecMagneticDipoleOrientation;
			Couple couple;
			couple.torqe = m.vectorProd(B);
			m_CoupleList.push_back(couple);
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (fieldsManager->m_bValidCurrentLoopMagneticField)
	{
	//	Vector3D MagneticField(CFieldsManager* pfm, const Point3D& p);
		Vector3D B = fieldsManager->MagneticField(x, this);//x,y,z);
		Couple couple(m_fCharge*(velocityCM.vectorProd(B)));
		m_CoupleList.push_back(couple);
		if (m_fMagneticDipoleMoment!=0.0 )
		{
			Vector3D	m = m_fMagneticDipoleMoment*m_vecMagneticDipoleOrientation;
			Couple couple;
			couple.torqe = m.vectorProd(B);
			m_CoupleList.push_back(couple);
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		if (m_vecDampingCoefficient.x!=0.0)
		{
			Couple couple((-velocityCM.x*m_vecDampingCoefficient.x)*I_3D);
			m_CoupleList.push_back(couple);
		}
///////////////////////////////////////////////////////////////////////////////
		if (m_vecDampingCoefficient.y!=0.0)
		{
			Couple couple((-velocityCM.y*m_vecDampingCoefficient.y)*J_3D);
			m_CoupleList.push_back(couple);
		}
///////////////////////////////////////////////////////////////////////////////
		if (m_vecDampingCoefficient.z!=0.0)
		{
			Couple couple((-velocityCM.z*m_vecDampingCoefficient.z)*K_3D);
			m_CoupleList.push_back(couple);
		}
///////////////////////////////////////////////////////////////////////////////
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}


///////////////////////////////////////////////////////////////////////////////

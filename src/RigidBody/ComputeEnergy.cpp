// ComputeEnergy.cpp

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "RigidBodyManager.h"
#include "FieldsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 
Vector3D CRigidBodyManager::ComputeMomentum()
{
	Vector3D momentumCM;
	if (empty()) return ZERO_3D;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return ZERO_3D;
		momentumCM += pbody->p;
	}
	return momentumCM;
}

Vector3D CRigidBodyManager::ComputeAngularMomentum()
{
	Vector3D angularMomentum;
	if (empty()) return angularMomentum;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return ZERO_3D;
		angularMomentum += pbody->GetTotalAngularMomentum();
	}
	return angularMomentum;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
double CRigidBodyManager::ComputeKineticEnergy(CRigidBody* pComputeEnergyOfBody)
{
	if (empty()) return 0;
	double kineticenergy = 0;
	double kineticEnergy = 0;
	iterator it;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody != 0);
		if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return 0;
		if (pComputeEnergyOfBody) {
			if (pbody != pComputeEnergyOfBody) continue;
		}
		pbody->get_KineticEnergy(kineticenergy);
		if (kineticenergy<0.0) throw SolveIt_ERROR(L"KE < )");
		kineticEnergy += kineticenergy;
	}
	ASSERT(kineticEnergy >= 0.0);
	return kineticEnergy;
}
///////////////////////////////////////////////////////////////////////////////
double CRigidBodyManager::ComputeEnergy(CRigidBody* pComputeEnergyOfBody)
{
	if (empty()) return 0;
	double energy = 0;
	try{
	double KineticEnergy = ComputeKineticEnergy(pComputeEnergyOfBody);
	double PotentialEnergy = ComputePotentialEnergy(pComputeEnergyOfBody);
//for a system of N particles with N = size() > 1
//arbitrarily give a particle 1/2 of its N=1 PE:
	if (pComputeEnergyOfBody && size()>1)
		energy = KineticEnergy + PotentialEnergy/2;
	else
		energy = KineticEnergy + PotentialEnergy;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return energy;
}
///////////////////////////////////////////////////////////////////////////////
double CRigidBodyManager::ComputePotentialEnergy(CRigidBody* pComputeEnergyOfBody)
{
	if (empty()) return 0;
	CSystem* m_system = theApp.GetSystem();
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	static double Energy = 0;
	double energy = 0;
	double action = 0;
	double altAction = 0;
	static double Action = 0;
	static double AltAction = 0;
	double gamma = 0;
	double&   g = fieldsManager->m_ExternalGravitationalField.y;
	double& Ex1 = fieldsManager->m_ExternalElectrostaticFieldX1;
	double& Ex2 = fieldsManager->m_ExternalElectrostaticFieldX2;
	double& Ex3 = fieldsManager->m_ExternalElectrostaticFieldX3;
/*//////////////////////////////////////////////////////////////////////////////
	Vector3D& B		= m_system->m_ExternalMagneticField;
	TimeDependentExternalForceComponent& X = m_system->timeDependentExternalForceComponentX;
	TimeDependentExternalForceComponent& Y = m_system->timeDependentExternalForceComponentY;
	TimeDependentExternalForceComponent& Z = m_system->timeDependentExternalForceComponentZ;
//////////////////////////////////////////////////////////////////////////////*/
	double mass1=0, mass2=0;
	double double_count = 0.5;
	if (pComputeEnergyOfBody) double_count = 1;
	iterator it, iT;
	for (it = begin(); it != end(); ++it)
	{
		CRigidBody* pbody1 = dynamic_cast<CRigidBody *>((*it).rbPtr);
		ASSERT(pbody1 != 0);
		if (pbody1 == 0) throw SolveIt_ERROR(L"pbody == 0");//return 0;

		if (pComputeEnergyOfBody) {
			if (pbody1 != pComputeEnergyOfBody) continue;
		}

#if 10
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		if (m_system->m_bDoMultiBody && pbody1->m_inboundJoint.jointType == Joint1DOF::SPRING)
		{
			const double smallDist = 0.01e-6;
			ListOf_springConstants_springLengths::iterator	ikL;
			PairPoint3DVector::iterator						iptpt;
			vector<CRigidBody *>::iterator					irb;
		for (
			irb		=	pbody1->m_bodiesConnectedTo.begin(),
			ikL		=	pbody1->m_ListOf_springConstants_springLengths.begin(),
			iptpt	=	pbody1->m_PairPoint3DVector.begin();
			((irb		!=	pbody1->m_bodiesConnectedTo.end())&&
			(ikL		!=	pbody1->m_ListOf_springConstants_springLengths.end())&&
			(iptpt	!=	pbody1->m_PairPoint3DVector.end()));
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
				Point3D			pt1		= pbody1->LocalToLab(point1);
				Point3D			pt2		= body2? body2->LocalToLab(point2):point2;
				Vector3D		rel		= pt1 - pt2;
				double			dist	= rel.norm();
				double			defm	= dist - L;
				double			U		= 0.5*k*defm*defm;
				energy += body2? 0.5*U: U;
		}
		}
///////////////////////////////////////////////////////////////////////////////
		if (m_system->m_bDoMultiBody && RigidLinkJointStates.size() > pbody1->svIdx)
		{
			const double& Q = RigidLinkJointStates[pbody1->svIdx];
			if (_isnan( Q )) throw SolveIt_ERROR(L"_isnan");
			double dx = Q - pbody1->m_Ls;
			energy += 0.5*pbody1->m_Ks * (dx) * (dx);
		}
///////////////////////////////////////////////////////////////////////////////SolveIt_ERROR(L"")
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif
		if (pbody1->m_fMagneticDipoleMoment!=0.0 )
		{
///////////////////////////////////////////////////////////////////////////////
			Vector3D	m = pbody1->m_fMagneticDipoleMoment*pbody1->m_vecMagneticDipoleOrientation;
	{
		Vector3D B	= fieldsManager->MagneticField (pbody1->x, pbody1);
		if (B.x!=0.0 || B.y!=0.0 || B.z!=0.0 )
		{
			energy -= m*B;
		}
	}
///////////////////////////////////////////////////////////////////////////////
//	if (fieldsManager->m_bValidCurrentLoopMagneticField)
//	{
//		Vector3D B = fieldsManager->MagneticField(pbody1->x);//x,y,z);
//		if (B.x!=0.0 || B.y!=0.0 || B.z!=0.0 )
//		{
//			energy -= m*B;
//		}
//	}
///////////////////////////////////////////////////////////////////////////////
		}
		if (pbody1->m_uUserDefinedPotential.func)
		{
			energy += pbody1->Compute_UserDefinedPotential(m_system->m__t);
/*
			Point3D& x = pbody1->x;
			double X=x.x, Y=x.y, Z=x.z;
			if (pbody1->m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				double e = pbody1->CallUserDefined(CRigidBody::__Potential, m_system->m__t, X,Y,Z);
				energy += e;
			}
			else
			{
				double e = pbody1->CallUserDefined(CRigidBody::__Potential, X,Y,Z);
				energy += e;
			}
*/
		}
///////////////////////////////////////////////////////////////////////////////
		if (pbody1->m_fMassInverse!=0.0) mass1 = 1.0/pbody1->m_fMassInverse;
		else mass1 = pbody1->m_fCentralForce;
		
		if (m_bInternalForce)
		for (iT = begin(); iT != end(); ++iT)
		{
			if (it==iT) continue;
			CRigidBody* pbody2 = dynamic_cast<CRigidBody *>(dynamic_cast<CRigidBody *>((*iT).rbPtr));
			ASSERT(pbody2 != 0);
			if (pbody2 == 0) throw SolveIt_ERROR(L"pbody2 == 0");//return 0;
			ASSERT(pbody2 != pbody1);
			if (pbody2==pbody1) continue;
			if (pbody2->m_fMassInverse!=0.0) mass2 = 1.0/pbody2->m_fMassInverse;
			else mass2 = pbody2->m_fCentralForce;
			Vector3D rel = pbody1->x - pbody2->x;
			double r = rel.norm();
			if (r<1.0e-8) continue;
			double jk = pbody1->m_fCharge*pbody2->m_fCharge;
			if (jk != 0.0)
			{
				energy += double_count*jk/r;
			}

			jk = -fieldsManager->m_GravitationalCoupling*mass1*mass2;
			if (jk != 0.0)
			{
				energy += double_count*jk/r;
			}

		//	jk = 4*theFieldsManager.m_fLennardJones;
//m_fLennardJones.x = sqrt(m_fLennardJones.z / m_fLennardJones.y)
	//		jk = theFieldsManager.m_fLennardJones.y;
			if (fieldsManager->m_bLennardJones_IsValid)// && jk != 0.0)
			{
	//			double r3 = pow(r,3.0);
	//			energy += jk*(theFieldsManager.m_fLennardJones.x-r3)*(theFieldsManager.m_fLennardJones.x+r3)/pow(r3,4.0);
				double	r3	= pow(fieldsManager->m_fLennardJones.y/r, 3.0);
				double	r6	= pow(fieldsManager->m_fLennardJones.z/r, 6.0);
				energy += double_count*(r6-r3)*(r6+r3);
			}
		}
///////////////////////////////////////////////////////////////////////////////
		if (mass1==0.0) continue;
///////////////////////////////////////////////////////////////////////////////
		if (g!=0.0) energy += -mass1*g*pbody1->x.y;
		if (Ex1!=0.0 || Ex2!=0.0 || Ex3!=0.0 )
		{
			double& x = pbody1->x.x;
			energy += -0.5*x*x*(Ex1 + (2.0/3.0)*Ex2*x + 0.5*Ex3*x*x)*pbody1->m_fCharge;
		}

//		if (fieldsManager->m_bCentralForce_IsValid)
//		{
//			Vector3D rel = pbody1->x;
//			double r = rel.norm();
//			if (r>1.0e-30) energy += -mass1*fieldsManager->m_fCentralForce/r;
//		}
///////////////////////////////////////////////////////////////////////////////
#if 10
		if (pComputeEnergyOfBody == 0) {
/*
// mass1*pbody1->m_vecAcceleration + ß pbody1->m_vecVelocity + V'[x(t)]
//		pbody1->m_vecAcceleration = pbody1->m_fMassInverse*pbody1->m_vecForce;
//		pbody1->m_vecVelocity = pbody1->m_fMassInverse*pbody1->p;
*/
		gamma = fabs(pbody1->m_vecDampingCoefficient.x);
///////////////////////////////////////////////////////////////////////////////
/*
			Vector3D	(	-p.x*m_vecDampingCoefficient.x,
							-p.y*m_vecDampingCoefficient.y,
							-p.z*m_vecDampingCoefficient.z);
*/
///////////////////////////////////////////////////////////////////////////////
		Vector3D vel = (gamma*pbody1->m_fMassInverse)*pbody1->p;
		Vector3D udf = pbody1->Compute_UserDefinedExternalForce(m_system->m__t);
		iterator i;
		if (m_bInternalForce)
		for (i = begin(); i != end(); ++i)
		{
			CRigidBody* pbody2 = (*i).rbPtr;
			ASSERT( pbody2 != 0);
			if (pbody2 == 0) return 0;
			if (pbody1==pbody2) continue;
			Vector3D rel = pbody1->x - pbody2->x;
			double r3 = pow(rel*rel,1.5);
			if (r3<1.0e-24) continue;
			rel /= r3;
		//	double jk = 24*theFieldsManager.m_fLennardJones;
		//	double jk = 6*theFieldsManager.m_fLennardJones.y;
			if (fieldsManager->m_bLennardJones_IsValid)// && jk != 0.0)
			{
		//		double sqrt2 = theFieldsManager.m_fLennardJones.x*1.4142135623730950488016887242097;
		//		double R = jk*(sqrt2-r3)*(sqrt2+r3)/pow(r3,3.6666666666666666666666666666667);
				const double sqrt2 = 1.4142135623730950488016887242097;
				Vector3D	rel	= pbody1->x - pbody2->x;
				double		r2	= rel*rel;
				double		r	= sqrt(r2);
				double		r3	= pow(fieldsManager->m_fLennardJones.y/r, 3.0);
				double		r6	= pow(fieldsManager->m_fLennardJones.z/r, 6.0)*sqrt2;
				double		R	= 6*(r6-r3)*(r6+r3)/r2;
				udf += double_count*R*rel;
			}
		}
//		Vector3D acc  = pbody1->m_fMassInverse*pbody1->m_vecForce;//m_vecPreviousForce;
		Vector3D acc  = (pbody1->m_fMassInverse/m_system->m_dt)*(pbody1->p - pbody1->m_statePrevious.p);
		Vector3D Acc  = vel + udf;// <- equations of motion
		Vector3D eL  = acc + vel - udf;
		action += eL*eL*m_system->m_dt;
		altAction += (4*m_system->m_dt)*(vel*vel);
	}
#endif
		}

#if 0
	if (pComputeEnergyOfBody == 0) {
		if (Energy == 0.0) Energy = energy;
		Action += action;
		AltAction += altAction;
	std::wstring bbuf(g_szWorkDir.c_str());//buf
	bbuf += L"\\sd.txt";
		ofstream ofs(bbuf, ios::app );
		ofs << m_system->m__t << '\t' << m_system->m_dt << '\t' << energy << '\t' << action << '\t' << Action << '\t' << AltAction << '\t' <<  4*gamma*(energy-Energy) <<endl;
		ofs.close();
	}
#endif

	return energy;
}
///////////////////////////////////////////////////////////////////////////////

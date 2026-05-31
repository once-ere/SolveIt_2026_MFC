// Contact.cpp: implementation of the Contact class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"


#include "System.h"
#include "Contact.h"
#include "OdeSolverCollision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void Contact::ComputeRelativeVelocityLab()
{
	Vector3D vel1 = m_body1->ComputeVelocityOfPointInBody(m_pointContact1);
	Vector3D vel2 = m_body2->ComputeVelocityOfPointInBody(m_pointContact2);
	m_vecRelVelocityLab = vel1 - vel2;
	if (m_bPermute) m_vecRelVelocityLab.negate();
}

void Contact::ComputeRelativeVelocityLocal()
{
	m_vecRelVelocityLocal = R*m_vecRelVelocityLab;
}

//const Contact& c
void Contact::ComputeCollisionMatrix()
{
//	Mat3 K1 = m_body1->ComputeCollisionMatrix(m_pointContact1);
//	Mat3 K2 = m_body2->ComputeCollisionMatrix(m_pointContact2);
	Mat3 K1 = m_body1->ComputeCollisionMatrix(m_pointContact());
	Mat3 K2 = m_body2->ComputeCollisionMatrix(m_pointContact());
#define X 0
#define Y 1
#define Z 2
	K.m[X][X] = K1.m[X][X]+K2.m[X][X];
	K.m[X][Y] = K1.m[X][Y]+K2.m[X][Y];
	K.m[X][Z] = K1.m[X][Z]+K2.m[X][Z];
	K.m[Y][X] = K1.m[Y][X]+K2.m[Y][X];
	K.m[Y][Y] = K1.m[Y][Y]+K2.m[Y][Y];
	K.m[Y][Z] = K1.m[Y][Z]+K2.m[Y][Z];
	K.m[Z][X] = K1.m[Z][X]+K2.m[Z][X];
	K.m[Z][Y] = K1.m[Z][Y]+K2.m[Z][Y];
	K.m[Z][Z] = K1.m[Z][Z]+K2.m[Z][Z];
#undef Z
#undef Y
#undef X
	return;

/*
	iK.invert(K);
	tK = K;
	tiK = iK;
	tK.postmult(iR);
	tK.premult(R);
	tiK.postmult(iR);
	tiK.premult(R);
*/
/*
#define ADD(a,b) K.m[a][b] = K1.m[a][b]+K2.m[a][b];
		ADD(X,X) ADD(X,Y) ADD(X,Z)
		ADD(Y,X) ADD(Y,Y) ADD(Y,Z)
		ADD(Z,X) ADD(Z,Y) ADD(Z,Z)
#undef ADD
*/

	//	INVERSE( K.m , iK )
	//	determinantK =	det;
/*
#define TEST(a,b) (K.m[a][X]*iK[X][b] + K.m[a][Y]*iK[Y][b] + K.m[a][Z]*iK[Z][b])
	TCHAR s[512];
	std::wstringstream oss(s,511);//ostrstream
oss<<TEST(X,X)<<" "<<TEST(X,Y)<<" "<<TEST(X,Z)<<endl<<
	TEST(Y,X)<<" "<<TEST(Y,Y)<<" "<<TEST(Y,Z)<<endl<<
	TEST(Z,X)<<" "<<TEST(Z,Y)<<" "<<TEST(Z,Z)<<endl<<determinantK<<ends;
	::MessageBox(NULL, s, "K.m iK", MB_OK);
#undef TEST
*/
//		double tmp[3][3];
//		MMPROD( R.m, K.m, tmp )
//		MMPROD( tmp, iR, tK )
//		MMPROD( R.m, iK, tmp )
//		MMPROD( tmp, iR, tiK ) 
}


//const Contact& c

void Contact::ComputeRotation()
{
	Vector3D localZaxis = normal;
	Vector3D localXaxis = J_3D .vectorProd(localZaxis) ;
	if (localXaxis == ZERO_3D) localXaxis = K_3D;
	localXaxis.normalize();
	Vector3D localYaxis = localZaxis .vectorProd(localXaxis) ;
	localYaxis.normalize();

#define X 0
#define Y 1
#define Z 2
	R.m[X][X]= localXaxis.x,	R.m[X][Y]= localXaxis.y,	R.m[X][Z]= localXaxis.z;
	R.m[Y][X]= localYaxis.x,	R.m[Y][Y]= localYaxis.y,	R.m[Y][Z]= localYaxis.z;
	R.m[Z][X]= localZaxis.x,	R.m[Z][Y]= localZaxis.y,	R.m[Z][Z]= localZaxis.z;
#undef Z
#undef Y
#undef X

	iR.xpose(R);
}
/**/
#if 10
long Contact::ComputeChangeInRelativeVelocity()
{
	ComputeRotation();
	ComputeCollisionMatrix();

	iK.invert(K);
	tK = K;
	tiK = iK;
	tK.postmult(iR);
	tK.premult(R);
	tiK.postmult(iR);
	tiK.premult(R);

	ComputeRelativeVelocityLab();
	ComputeRelativeVelocityLocal();
	double uZ = theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLab*normal;
	double uz = theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLocal.z;
ASSERT(float(uz)==float(uZ));
ASSERT(theApp.GetSystem()->m_stepForwardOrBackward*uz<=0.0);
//	changeInRelativeVelocityLocal.x = 0;
//	changeInRelativeVelocityLocal.y = 0;
//	changeInRelativeVelocityLocal.z = -(1+coefficientOfRestitution()) * uz * theApp.GetSystem()->m_stepForwardOrBackward;
//	changeInRelativeVelocityLab = iR * changeInRelativeVelocityLocal;
//	return 0;

	if (theApp.GetSystem()->m_stepForwardOrBackward*uz>=0.0) throw SolveIt_ERROR(L"positive relative velocity");//return ResolveUzPos;
//	if (theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLocal.z>=0.0)  return 1;//throw -1;
	double ux = /**/theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLocal.x;
	double uy = /**/theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLocal.y;
//	double uz = /**/theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLocal.z;
	double uz0 = uz;
	Vector3D tmp0(ux,uy,uz);
	double Wz = 0;
	double Jz = 0;
	long res = 0;

	COdeSolverCollision coll;
	try
	{
	res = coll.CollisionCVODE( ux, uy, uz, Wz, Jz,
						coefficientOfKineticFriction(),
						coefficientOfRestitution(),
						tK.m);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		changeInRelativeVelocityLocal.x = 0;
		changeInRelativeVelocityLocal.y = 0;
		changeInRelativeVelocityLocal.z = -(1+coefficientOfRestitution()) * uz0 * theApp.GetSystem()->m_stepForwardOrBackward;
		changeInRelativeVelocityLab = iR * changeInRelativeVelocityLocal;
		return -1;
	}

/*

	CODESolver colli;
	res = colli.Collision( ux, uy, uz, Wz, Jz,
						coefficientOfKineticFriction(),
						coefficientOfRestitution(),
						tK.m);

*/


	if (res)
	{
		changeInRelativeVelocityLocal.x = 0;
		changeInRelativeVelocityLocal.y = 0;
		changeInRelativeVelocityLocal.z = -(1+coefficientOfRestitution()) * uz0 * theApp.GetSystem()->m_stepForwardOrBackward;
	}
	else
	{
		ux *= theApp.GetSystem()->m_stepForwardOrBackward;
		uy *= theApp.GetSystem()->m_stepForwardOrBackward;
		uz *= theApp.GetSystem()->m_stepForwardOrBackward;
		changeInRelativeVelocityLocal.x = ux-m_vecRelVelocityLocal.x;
		changeInRelativeVelocityLocal.y = uy-m_vecRelVelocityLocal.y;
		changeInRelativeVelocityLocal.z = uz-m_vecRelVelocityLocal.z;

//		wchar_t buf[1024];
//		/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf));
//		Vector3D tmp(ux,uy,uz);
//		oss << tmp0  << " <- (ux,uy,uz) -> " << tmp << " ; Jz = "  << Jz	<< "; Wz = "  << Wz << ends;
//	::MessageBox(NULL, buf, L"ComputeChangeInRelativeVelocity", MB_OK);
// in local, not lab, frame:
//	Vector3DMVPROD( changeInRelativeVelocityLocal , changeInRelativeVelocityLab , iR )
	}
/*
		changeInRelativeVelocityLocal.x = 0;
		changeInRelativeVelocityLocal.y = 0;
		double e = coefficientOfRestitution();
		changeInRelativeVelocityLocal.z = -(1+e) * uz0 * theApp.GetSystem()->m_stepForwardOrBackward;
*/
	changeInRelativeVelocityLab = iR * changeInRelativeVelocityLocal;
	return 0;
}
#endif

void Contact::ComputeImpulse()
{
	impulseLab = iK * changeInRelativeVelocityLab;
}


Vector3D Contact::ComputeD_Dt_of_LocalZaxis()
{
	if (m_listNormals.empty()) return ZERO_3D;
	Vector3D normal1;
	Vector3D normal2;
//	vector<Vector3D>::iterator i_vecNormals;
	vector<TriangleVerts>::iterator i_vecTriangles1;
	vector<TriangleVerts>::iterator i_vecTriangles2;
	for (i_vecTriangles1 = m_listTriangles1.begin(); i_vecTriangles1 != m_listTriangles1.end(); ++i_vecTriangles1)
	{
		TriangleVerts& v = *i_vecTriangles1;
		normal1=v.data[3];
	}
	for (i_vecTriangles2 = m_listTriangles2.begin(); i_vecTriangles2 != m_listTriangles2.end(); ++i_vecTriangles2)
	{
		TriangleVerts& v = *i_vecTriangles2;
		normal2=v.data[3];
	}
//	if (m_bVertexFaceContact && m_bodyWithClosestFace)
//	{
//		Vector3D AngularVelocity = m_bodyWithClosestFace->GetAngularVelocity();
//		return AngularVelocity .vectorProd() normal;
//	}

	Vector3D AngularVelocityA;
	Vector3D AngularVelocityB;
	m_body1->get_AngularVelocity(AngularVelocityA);
	m_body2->get_AngularVelocity(AngularVelocityB);
	Vector3D e1dot = AngularVelocityA .vectorProd(normal1) ;
	Vector3D e2dot = AngularVelocityB .vectorProd(normal2) ;
	Vector3D z = e1dot + e2dot;
	return normal .vectorProd( ( z .vectorProd(normal) ) );

	return ZERO_3D;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
	//						  CSphericalBody* const body1,
	//						  CSphericalBody* const body2,

double Contact::Compute_b_VectorComponent(Contact& c)
{
	ASSERT(m_body1==c.m_body1);
	ASSERT(m_body2==c.m_body2);

	Vector3D rel1 = c.m_pointContact1 - m_body1->x;
	Vector3D rel2 = c.m_pointContact2 - m_body2->x;
	Vector3D a_ext_part, a_vel_part, b_ext_part, b_vel_part;
	Vector3D force_on_1 = m_body1->m_vecForce;
	Vector3D force_on_2 = m_body2->m_vecForce;
	Vector3D torque_on_1 = m_body1->m_vecTorque;
	Vector3D torque_on_2 = m_body2->m_vecTorque;
	Vector3D	r1 = m_body1->x,
				r2 = m_body2->x;
/*
	if (c.m_body1->m_pCSurface)
	{
		Vector3D normal = c.m_body1->m_pCSurface->SurfaceNormal(r1);
		double fn = normal*force_on_1;
		Mat3 P1;
		c.m_body1->m_pCSurface->Projctn(r1.x,r1.y,r1.z, P1.m);
		force_on_1 = P1*force_on_1;
		torque_on_1 -= fn*(rel1.vectorProd()normal);
	}
	if (c.m_body2->m_pCSurface)
	{
		Vector3D normal = c.m_body2->m_pCSurface->SurfaceNormal(r2);
		double fn = normal*force_on_2;
		Mat3 P2;
		c.m_body2->m_pCSurface->Projctn(r2.x,r2.y,r2.z, P2.m);
		force_on_2 = P2*force_on_2;
		torque_on_2 -= fn*(rel2.vectorProd()normal);
	}
*/
	if (m_body1->m_fMassInverse == 0.0) force_on_1 = torque_on_1 = ZERO_3D;
	if (m_body2->m_fMassInverse == 0.0) force_on_2 = torque_on_2 = ZERO_3D;
//Compute the part of R pa(t0) due to the external force and torque, and similarly for R pb(t0)

	Mat3 Iinv1;// = m_body1->GetInverseInertiaTensor();//m_Iinv;
	Mat3 Iinv2;// = m_body2->GetInverseInertiaTensor();//m_Iinv;
	m_body1->get_InverseInertiaTensor(Iinv1);
	m_body2->get_InverseInertiaTensor(Iinv2);

	Vector3D AngularVelocity1;
	Vector3D AngularVelocity2;
	m_body1->get_AngularVelocity(AngularVelocity1);
	m_body2->get_AngularVelocity(AngularVelocity2);

	a_ext_part =	force_on_1 * m_body1->m_fMassInverse +
					((Iinv1 * torque_on_1) .vectorProd(rel1) ),
	b_ext_part =	force_on_2 * m_body2->m_fMassInverse +
					((Iinv2 * torque_on_2) .vectorProd(rel2) );
// Compute the part of R pa(t0) due to velocity, and similarly for R pb(t0)
	a_vel_part = (AngularVelocity1 .vectorProd( (AngularVelocity1 .vectorProd(rel1) ))) +
		((Iinv1 * (m_body1->L .vectorProd(AngularVelocity1) )) .vectorProd(rel1) );
	b_vel_part = (AngularVelocity2 .vectorProd( (AngularVelocity2 .vectorProd(rel2) ))) +
		((Iinv2 * (m_body2->L .vectorProd(AngularVelocity2) )) .vectorProd(rel2) );
// Combine the above results, and dot with ni(t0):
	double k1 = c.normal * ((a_ext_part + a_vel_part) - (b_ext_part + b_vel_part));

//	if (c.m_bVertexFaceContact && m_body2 == c.m_bodyWithClosestVertex) k1 *= -1;
//c.m_bodyWithClosestFace;


	Vector3D ndot = c.ComputeD_Dt_of_LocalZaxis();

//	Vector3D point(RestingContactPoints.front().contact_point);
//	double k2 = 2 * ndot * ComputeRelativeVelocity(point, point);


	Vector3D v1 = m_body1->ComputeVelocityOfPointInBody(c.m_pointContact1);
	Vector3D v2 = m_body2->ComputeVelocityOfPointInBody(c.m_pointContact2);
	Vector3D relVel = v1 - v2;
	double k2 = 2 * (ndot * relVel);

	c.ComputeNonContactAndInertialAccelerations();
	ASSERT(fabs(k1 + k2 - c.m_fNonContactAndInertialAcceleration) < 0.1e-14);

	return k1 + k2;
}


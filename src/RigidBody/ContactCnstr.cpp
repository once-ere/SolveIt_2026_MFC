// InteractingPair.cpp: implementation of the Contact class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"


#include "Contact.h"
#include "../SolveIt.Lib/lib/pqp/PQP.h"
#include "../SolveIt.Lib/lib/pqp/MatVec.h"
#include "PQP_Sphere.h"
#include "RigidBody.h"
#include "System.h"
#include "ExaminePair.h"
#include "shapes.h"
#include "parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//Improve floating-point consistency
#pragma optimize( "p", on )

const double m_MOTION_THRESHOLD		= 0.0005;
const double m_PROXIMITY_TOLERANCE	= 0.05;
///////////////////////////////////////////////////////////////////////////////
void DrawTri(double n[3], double p0[3], double p1[3], double p2[3])
{
	const double y=PI/4000;
	static double a=PI/2;
	static double b=PI/2;
	a+=2*y;
	b-=3*y;
	double ca=cos(a);
	double sa=sin(a);
	double cb=cos(b);
	double sb=sin(b);

	glColor3d(fabs(sa*cb),fabs(sa*sb),fabs(ca));
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glNormal3dv(n);
	glVertex3dv(p0);
	glVertex3dv(p1);
	glVertex3dv(p2);
	glEnd();
	glPopMatrix();
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
unsigned long Contact::m_InteractingPairKey=0;
//////////////////////////////////////////////////////////////////////
Contact::Contact(CRigidBody* body1, CRigidBody* body2, bool bShowContacts) :
	m_lKey(m_InteractingPairKey++),
	m_body1(body1),
	m_body2(body2),
	m_pCExaminePair(0),
//	m_altbody1(body2),
//	m_altbody2(body1),
	m_bPermute(false),
	m_RelativeNormalMotion(Separating),
	m_ContactType(Separated)
{
	if (bShowContacts)
		ExaminePair();
}
///////////////////////////////////////////////////////////////////////////////
Contact::~Contact()
{
	CloseExaminePair();
	clear();
}
///////////////////////////////////////////////////////////////////////////////
void Contact::ExaminePair()
{
//	if (m_pCExaminePair.get() == 0)
//	{
//		m_pCExaminePair = auto_ptr<CExaminePair>(new CExaminePair(this));
//	}
	if (m_pCExaminePair == 0)
	{
		m_pCExaminePair = (new CExaminePair(this));
	}
}
///////////////////////////////////////////////////////////////////////////////
void Contact::CloseExaminePair()
{
//	if (m_pCExaminePair.get() != 0 && IsWindow(m_pCExaminePair->m_hWnd) )
	if (m_pCExaminePair != 0 && IsWindow(m_pCExaminePair->m_hWnd) )
	{
		m_pCExaminePair->DestroyWindow();
		delete m_pCExaminePair;
		m_pCExaminePair = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////
void Contact::OnDraw()
{
	if (m_pointContact1 == ZERO_3D&&m_pointContact2 == ZERO_3D) return;

	glColor3d(1.0,0.0,0.0);

	glPushMatrix();
	glTranslated(m_pointContact1[0],m_pointContact1[1],m_pointContact1[2]);
	shapes::auxSolidSphere(.05);
//	glutSolidSphere(.05,15,15);
	glPopMatrix();


	glColor3d(0.0,0.0,1.0);
	glPushMatrix();
	glTranslated(m_pointContact2[0],m_pointContact2[1],m_pointContact2[2]);
	shapes::auxSolidSphere(.05);
//	glutSolidSphere(.05,15,15);
	glPopMatrix();

// draw the line between the closest points
	glColor3d(0.5,0.0,0.5);

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glVertex3v(m_pointContact1.v);
	glVertex3v(m_pointContact2.v);
	glEnd();
	glEnable(GL_LIGHTING);

	if (m_listTriangles1.empty()) return;
//	vector<Vector3D>::iterator i_vecNormals;
	vector<TriangleVerts>::iterator i_vecTriangles1;
	vector<TriangleVerts>::iterator i_vecTriangles2;
	for (i_vecTriangles1 = m_listTriangles1.begin(); i_vecTriangles1 != m_listTriangles1.end(); ++i_vecTriangles1)
	{
		TriangleVerts& v = *i_vecTriangles1;
		DrawTri(v.data[3], v.data[0], v.data[1], v.data[2]);
	}
	for (i_vecTriangles2 = m_listTriangles2.begin(); i_vecTriangles2 != m_listTriangles2.end(); ++i_vecTriangles2)
	{
		TriangleVerts& v = *i_vecTriangles2;
		DrawTri(v.data[3], v.data[0], v.data[1], v.data[2]);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Contact::ClearCollisionContactsLists()
{
	vector<Contact*>::iterator i_CollidingContactsList;
	for (i_CollidingContactsList = m_CollidingContactsList.begin(); i_CollidingContactsList != m_CollidingContactsList.end(); ++i_CollidingContactsList)
	{
		Contact* pc = *i_CollidingContactsList;
		delete pc;
	}
	m_CollidingContactsList.clear();
//	m_pointContact1 = ZERO_3D;
//	m_pointContact2 = ZERO_3D;
}
///////////////////////////////////////////////////////////////////////////////
void Contact::clear()
{
	m_listNormals.clear();
	m_listTriangles1.clear();
	m_listTriangles2.clear();
	ClearCollisionContactsLists();//
}
///////////////////////////////////////////////////////////////////////////////
void Contact::PQP_Update()
{
	if (theApp.GetSystem()->m__t == 0.0) return;

	if (CRigidBody::m_boolSpheresOnly)
	{
		SpheresOnly_Update();
		return;
	}
	clear();
///////////////////////////////////////////////////////////////////////////////
	if (!m_body1->m_pPQP_Model) return;
	if (!m_body2->m_pPQP_Model) return;
///////////////////////////////////////////////////////////////////////////////
	bool ps1 = m_body1->type==T_SPHERE;
	bool ps2 = m_body2->type==T_SPHERE;

	bool pb1 = m_body1->type==T_BLOCK;
	bool pb2 = m_body2->type==T_BLOCK;

	bool pc1 = m_body1->type==T_CYLINDER;
	bool pc2 = m_body2->type==T_CYLINDER;

	bool pv1 = m_body1->type==T_VECTOR_ANALYSIS_OBJECT;
	bool pv2 = m_body2->type==T_VECTOR_ANALYSIS_OBJECT;
///////////////////////////////////////////////////////////////////////////////
	PQP_CollideResult cres;
	PQP_Collide(	&cres,
					m_body1->m_Rot,m_body1->m_Trn, m_body1->m_pPQP_Model,
					m_body2->m_Rot,m_body2->m_Trn, m_body2->m_pPQP_Model,
					PQP_ALL_CONTACTS);

///////////////////////////////////////////////////////////////////////////////
	if (cres.NumPairs()==0)
	{
		m_ContactType = Separated;

		PQP_DistanceResult dres;
		PQP_Distance(	&dres,
						m_body1->m_Rot,m_body1->m_Trn, m_body1->m_pPQP_Model,
						m_body2->m_Rot,m_body2->m_Trn, m_body2->m_pPQP_Model,
						0.0,0.0);

		m_separation = dres.Distance();
/*
	{
	_variant_t v(m_separation);
	std::wstring bstr=v;
	FireWriteStatus(bstr);
	}
*/
		PQP_REAL P1[3],P2[3];
		VcV(P1,dres.P1());
		VcV(P2,dres.P2());
// each point is in the space of its model;
// transform to world space
		PQP_REAL V1[3],V2[3];

		MxVpV(V1,m_body1->m_Rot,P1,m_body1->m_Trn);
		MxVpV(V2,m_body2->m_Rot,P2,m_body2->m_Trn);
		m_pointContact1 = V1;	// closest point on 1; contact point during a binary collision 
		m_pointContact2 = V2;	// closest point on 2; contact point during a binary collision

		normal = m_pointContact1-m_pointContact2;
		if (m_bPermute) normal.negate();
		normal.normalize();

		ComputeRelativeVelocityLab();//m_vecRelVelocityLab
		m_uz = theApp.GetSystem()->m_stepForwardOrBackward*(m_vecRelVelocityLab*normal);
		if (m_uz > m_MOTION_THRESHOLD)
		{
			m_RelativeNormalMotion = Separating;
		}
		else if (m_uz > -m_MOTION_THRESHOLD)// && m_separation < m_PROXIMITY_TOLERANCE)
		{
			m_RelativeNormalMotion = Stationary;
		}
		else if (m_uz <= -m_MOTION_THRESHOLD)
		{
			m_RelativeNormalMotion = Colliding;
		}

	}
///////////////////////////////////////////////////////////////////////////////
// intersecting bodies:
#define DO_SELF_CONSISTENCY_CHECK
#if defined(DO_SELF_CONSISTENCY_CHECK)
#endif
	else
	{
		Contact* pc = 0;
		m_ContactType = Penetrating;
		int numPenetratingPairs = cres.NumPairs();
		if (numPenetratingPairs==1)
		{
			pc = new Contact(*this);//
			m_CollidingContactsList.push_back(pc);//
		}

		PQP_REAL P11[3],P12[3],P13[3];
		PQP_REAL V11[3],V12[3],V13[3];
		PQP_REAL P21[3],P22[3],P23[3];
		PQP_REAL V21[3],V22[3],V23[3];
#if defined(DO_SELF_CONSISTENCY_CHECK)
		Point3D a1, a2;
		Point3D b1, b2;
		Point3D c1, c2;
#endif
		for(int i = 0; i < numPenetratingPairs; i++)
		{
			if (numPenetratingPairs > 1)
			{
//				pc = new Contact(m_body1, m_body2);//
			}
			if (ps1) {
#if defined(DO_SELF_CONSISTENCY_CHECK)
				(Vector3D&)a1 = m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][0]];
				(Vector3D&)b1 = m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][1]];
				(Vector3D&)c1 = m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][2]];
#endif
				VcV(P11,(PQP_REAL*)(m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][0]]));
				VcV(P12,(PQP_REAL*)(m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][1]]));
				VcV(P13,(PQP_REAL*)(m_body1->m_fRadius*verts[triIndices[cres.Id1(i)][2]]));
			}

			if (ps2) {
#if defined(DO_SELF_CONSISTENCY_CHECK)
				(Vector3D&)a2 = m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][0]];
				(Vector3D&)b2 = m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][1]];
				(Vector3D&)c2 = m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][2]];
#endif
				VcV(P21,(PQP_REAL*)(m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][0]]));
				VcV(P22,(PQP_REAL*)(m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][1]]));
				VcV(P23,(PQP_REAL*)(m_body2->m_fRadius*verts[triIndices[cres.Id2(i)][2]]));
			}

			if (pb1) {
				double r[3] = { m_body1->m_fWidth/2, m_body1->m_fHeight/2, m_body1->m_fDepth/2 };
#if defined(DO_SELF_CONSISTENCY_CHECK)
				const PQP_Array3& y=vertsBlock[blkIndices[cres.Id1(i)][0]];
				(Vector3D&)a1 = r*vertsBlock[blkIndices[cres.Id1(i)][0]];
				(Vector3D&)b1 = r*vertsBlock[blkIndices[cres.Id1(i)][1]];
				(Vector3D&)c1 = r*vertsBlock[blkIndices[cres.Id1(i)][2]];
#endif
				VcV(P11,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id1(i)][0]]));
				VcV(P12,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id1(i)][1]]));
				VcV(P13,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id1(i)][2]]));
			}

			if (pb2) {
				double r[3] = { m_body2->m_fWidth/2, m_body2->m_fHeight/2, m_body2->m_fDepth/2 };
#if defined(DO_SELF_CONSISTENCY_CHECK)
				(Vector3D&)a2 = r*vertsBlock[blkIndices[cres.Id2(i)][0]];
				(Vector3D&)b2 = r*vertsBlock[blkIndices[cres.Id2(i)][1]];
				(Vector3D&)c2 = r*vertsBlock[blkIndices[cres.Id2(i)][2]];
#endif
				VcV(P21,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id2(i)][0]]));
				VcV(P22,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id2(i)][1]]));
				VcV(P23,(PQP_REAL*)(r*vertsBlock[blkIndices[cres.Id2(i)][2]]));
			}


			if (pc1) {
				double r[3] = { m_body1->m_fRadius, m_body1->m_fRadius, m_body1->m_fDepth/2 };
#if defined(DO_SELF_CONSISTENCY_CHECK)
				(Vector3D&)a1 = r*vertsCylinder[cylinderIndices[cres.Id1(i)][0]];
				(Vector3D&)b1 = r*vertsCylinder[cylinderIndices[cres.Id1(i)][1]];
				(Vector3D&)c1 = r*vertsCylinder[cylinderIndices[cres.Id1(i)][2]];
#endif
				VcV(P11,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id1(i)][0]]));
				VcV(P12,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id1(i)][1]]));
				VcV(P13,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id1(i)][2]]));
			}

			if (pc2) {
				double r[3] = { m_body2->m_fRadius, m_body2->m_fRadius, m_body2->m_fDepth/2 };
#if defined(DO_SELF_CONSISTENCY_CHECK)
				(Vector3D&)a2 = r*vertsCylinder[cylinderIndices[cres.Id2(i)][0]];
				(Vector3D&)b2 = r*vertsCylinder[cylinderIndices[cres.Id2(i)][1]];
				(Vector3D&)c2 = r*vertsCylinder[cylinderIndices[cres.Id2(i)][2]];
#endif
				VcV(P21,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id2(i)][0]]));
				VcV(P22,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id2(i)][1]]));
				VcV(P23,(PQP_REAL*)(r*vertsCylinder[cylinderIndices[cres.Id2(i)][2]]));
			}

///////////////////////////////////////////////////////////////////////////////

			MxVpV(V11,m_body1->m_Rot,P11,m_body1->m_Trn);
			MxVpV(V12,m_body1->m_Rot,P12,m_body1->m_Trn);
			MxVpV(V13,m_body1->m_Rot,P13,m_body1->m_Trn);

			MxVpV(V21,m_body2->m_Rot,P21,m_body2->m_Trn);
			MxVpV(V22,m_body2->m_Rot,P22,m_body2->m_Trn);
			MxVpV(V23,m_body2->m_Rot,P23,m_body2->m_Trn);

#if defined(DO_SELF_CONSISTENCY_CHECK)
			Se3 body1CurrentPose=m_body1->CurrentPose();
			body1CurrentPose.transform(a1);
			body1CurrentPose.transform(b1);
			body1CurrentPose.transform(c1);

			Se3 body2CurrentPose=m_body2->CurrentPose();
			body2CurrentPose.transform(a2);
			body2CurrentPose.transform(b2);
			body2CurrentPose.transform(c2);
#endif

			Vector3D v11(V11);
			Vector3D v12(V12);
			Vector3D v13(V13);

#if defined(DO_SELF_CONSISTENCY_CHECK)
			a1-=v11;
			b1-=v12;
			c1-=v13;
const double ztol = 0.0001;
			ASSERT(a1.norm()<ztol);
			ASSERT(b1.norm()<ztol);
			ASSERT(c1.norm()<ztol);
#endif

			v12-=v11;
			v13-=v11;
			Vector3D n1(v12.vectorProd(v13));
			n1.normalize();

			TriangleVerts v1(V11,V12,V13, n1.v);
			m_listTriangles1.push_back(v1);

			Vector3D v21(V21);
			Vector3D v22(V22);
			Vector3D v23(V23);

#if defined(DO_SELF_CONSISTENCY_CHECK)
			a2-=v21;
			b2-=v22;
			c2-=v23;

			ASSERT(a2.norm()<ztol);
			ASSERT(b2.norm()<ztol);
			ASSERT(c2.norm()<ztol);
#endif

			v22-=v21;
			v23-=v21;
			Vector3D n2(v22.vectorProd(v23));
			n2.normalize();

			TriangleVerts v2(V21,V22,V23, n2.v);
			m_listTriangles2.push_back(v2);
			
/* 
			n1.negate();
			if (n1*normal < 0.0) n1.negate();
			if (n2*normal < 0.0) n2.negate();

			if (numPenetratingPairs == 1)
			{
				if (pb2) normal = n2;
				else if (pb1) normal = n1;
				else if (ps2) normal = m_pointContact2 - m_body2->x;
				else if (ps1) normal = m_body1->x - m_pointContact1;
			}
			else if (numPenetratingPairs > 1)
			{
				if (pb2) normal += n2;
				else if (pb1) normal += n1;
				else if (ps2) normal = m_pointContact2 - m_body2->x;
				else if (ps1) normal = m_body1->x - m_pointContact1;
			}

			const double tol = 0.88;
			double duh=n2*n1;

			double duh1= n1*normal;
			double duh2= n2*normal;
			if (duh1>tol)
			{
		//		CRigidBody* body = m_body1;
		//		m_body1 = m_body2;
		//		m_body2 = body;
		//		normal = n1;
				m_listNormals.push_back(n1);
			}
			else if (duh2>tol)
			{
		//		normal = n2;
				m_listNormals.push_back(n2);
			}
			else
			{
				m_listNormals.push_back(normal);
			}
*/
			if (pc && numPenetratingPairs > 1)
			{
				pc->m_ContactType = Penetrating;
				pc->m_pointContact1 = v11+v12+v13;
				pc->m_pointContact2 = v21+v22+v23;
				pc->m_pointContact1 /= 3;
				pc->m_pointContact2 /= 3;
				pc->normal = n1;
				pc->ComputeRelativeVelocityLab();//m_vecRelVelocityLab
				pc->m_uz = theApp.GetSystem()->m_stepForwardOrBackward*pc->m_vecRelVelocityLab*pc->normal;
				if (pc->m_uz > m_MOTION_THRESHOLD)
				{
					pc->m_RelativeNormalMotion = Separating;
				}
				else if (pc->m_uz > -m_MOTION_THRESHOLD)// && m_separation < m_PROXIMITY_TOLERANCE)
				{
					pc->m_RelativeNormalMotion = Stationary;
				}
				else if (pc->m_uz <= -m_MOTION_THRESHOLD)
				{
					pc->m_RelativeNormalMotion = Colliding;
				}
				m_CollidingContactsList.push_back(pc);
			}
		}
	}
//	normal.normalize();
//	if (m_pCExaminePair.get() != 0 
	if (m_pCExaminePair != 0 
		&& IsWindow(m_pCExaminePair->m_hWnd) && IsWindowVisible(m_pCExaminePair->m_hWnd) )
	{
		m_pCExaminePair->UpdateData();
	}
}
///////////////////////////////////////////////////////////////////////////////
void Contact::SpheresOnly_Update()
{
	ASSERT(CRigidBody::m_boolSpheresOnly);
	CRigidBody* ps1 = m_body1->type==T_SPHERE? m_body1:0;
	CRigidBody* ps2 = m_body2->type==T_SPHERE? m_body2:0;
	ASSERT(	ps1);
	assert(		ps1);
	ASSERT(	ps2);
	assert(		ps2);
	Vector3D rel = ps1->x - ps2->x;
	double separationCM = rel.norm();
	m_separation = separationCM - (ps1->m_fRadius + ps2->m_fRadius);
	bool bIntersecting = m_separation < 0.0;
	if (bIntersecting) m_ContactType = Contact::Penetrating;
	else
	{
		m_ContactType = Contact::Separated;
		normal = rel;
		normal.normalize();
// closest points between bodies, in lab frame
		m_pointContact1	= ps1->x - ps1->m_fRadius*normal;
		m_pointContact2	= ps2->x + ps2->m_fRadius*normal;
		if (m_bPermute) normal.negate();
	}
///////////////////////////////////////////////////////////////////////////////
	ComputeRelativeVelocityLab();//m_vecRelVelocityLab
	m_uz = theApp.GetSystem()->m_stepForwardOrBackward*m_vecRelVelocityLab*normal;
	if (m_uz > m_MOTION_THRESHOLD)
	{
		m_RelativeNormalMotion = Separating;
	}
	else if (m_uz > -m_MOTION_THRESHOLD)// && m_separation < m_PROXIMITY_TOLERANCE)
	{
		m_RelativeNormalMotion = Stationary;
	}
	else if (m_uz <= -m_MOTION_THRESHOLD)
	{
		m_RelativeNormalMotion = Colliding;
	}
//	if (m_pCExaminePair.get() != 0 
	if (m_pCExaminePair != 0 
		&& IsWindow(m_pCExaminePair->m_hWnd) && IsWindowVisible(m_pCExaminePair->m_hWnd))
	{
		m_pCExaminePair->UpdateData();
	}
}
///////////////////////////////////////////////////////////////////////////////
double Contact::StableFriction() {
//#ifndef SQR
//#define SQR(x) ((x)*(x))
//#endif
		return SQR(coefficientOfKineticFriction()*tiK[2][2])-SQR(tiK[0][2])-SQR(tiK[1][2]);
//#undef SQR
}
///////////////////////////////////////////////////////////////////////////////
double Contact::coefficientOfKineticFriction()
{
		if (m_body1->m_fCoefficientOfKineticFriction==0.0 ||
			m_body2->m_fCoefficientOfKineticFriction==0.0)
		{
			return 0;
		}
		else
		{
			return (m_body1->m_fCoefficientOfKineticFriction*m_body2->m_fCoefficientOfKineticFriction)/(m_body1->m_fCoefficientOfKineticFriction+m_body2->m_fCoefficientOfKineticFriction);
		}

	}
///////////////////////////////////////////////////////////////////////////////
	double Contact::coefficientOfRestitution()
	{
		return m_body1->m_fCoefficientOfRestitution*m_body2->m_fCoefficientOfRestitution;///(m_body1->m_fCoefficientOfRestitution+m_body2->m_fCoefficientOfRestitution);
	}
///////////////////////////////////////////////////////////////////////////////
void Contact::ComputeNonContactAndInertialAccelerations()
{
	m_nonContactAcceleration1 = ZERO_3D;
	m_nonContactAcceleration2 = ZERO_3D;
	m_fNonContactAndInertialAcceleration = 0;
	if (InContact != m_ContactType) return;
	if (Stationary != m_RelativeNormalMotion) return;
	m_nonContactAcceleration1 = m_body1->ComputeNonContactAndInertialAcceleration(m_pointContact1);
	m_nonContactAcceleration2 = m_body2->ComputeNonContactAndInertialAcceleration(m_pointContact2);

	Vector3D v1 = m_body1->ComputeVelocityOfPointInBody(m_pointContact1);
	Vector3D v2 = m_body2->ComputeVelocityOfPointInBody(m_pointContact2);
	Vector3D ndot = ComputeD_Dt_of_LocalZaxis();
	m_fNonContactAndInertialAcceleration = normal*(m_nonContactAcceleration1-m_nonContactAcceleration2);
	double dn2v = 2 * ndot * (v1 - v2);
	m_fNonContactAndInertialAcceleration += dn2v;
}
///////////////////////////////////////////////////////////////////////////////
bool Contact::IsCollision()
{
	ComputeRelativeVelocityLab();
	m_uz = theApp.GetSystem()->m_stepForwardOrBackward*(m_vecRelVelocityLab*normal);

	if (m_uz > m_MOTION_THRESHOLD)
	{
		m_RelativeNormalMotion = Separating;
	}
	else if (m_uz > -m_MOTION_THRESHOLD)// && m_separation < m_PROXIMITY_TOLERANCE)
	{
		m_RelativeNormalMotion = Stationary;
	}
	else if (m_uz <= -m_MOTION_THRESHOLD)
	{
		m_RelativeNormalMotion = Colliding;
	}
	if ( m_RelativeNormalMotion == Colliding && m_ContactType == Penetrating )
		return true;
//	if ( m_RelativeNormalMotion == Colliding && m_separation < m_PROXIMITY_TOLERANCE )
//		return true;
	return false;
}



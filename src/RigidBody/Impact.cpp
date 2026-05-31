// Impact.cpp: implementation of the CRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "Contact.h"
#include "System.h"
#include "instruction.h"
#include "parser.h"
#include "symbol.h"
#include "SymbolTable.h"
#include <STDARG.H>
#include "RigidBodyManager.h"
///////////////////////////////////////////////////////////////////////////////
/*
#undef min
#undef max
#undef Real
#include "./lib/tnt/tnt.h"
#include "./lib/tnt/jama_qr.h"
#include "./lib/tnt/jama_cholesky.h"
#include "./lib/tnt/jama_eig.h"
#include "./lib/tnt/jama_svd.h"
*/
///////////////////////////////////////////////////////////////////////////////
/*
*/
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	//** definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE 
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 //** main CVODE header file													
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	//** contains the enum for types of preconditioning	
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 //** use CVSPGMR linear solver each internal step		
#include "../SolveIt.Lib/lib/cvode/dense.h"		 //** use generic DENSE solver for preconditioning		
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  //** use CVDENSE linear solver each internal step 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ClearContactPointList()
{
		m_ContactPointList.clear();
}
void CRigidBody::AddContactPoint(Contact* c)//const Point3D& point)
{
		m_ContactPointList.push_back(c);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double CRigidBody::CalculateMassFromGeometry() {
	const double BIG_MASS = 9.e99;
	m_fMass = BIG_MASS;
	if (m_fMassInverse == 0.0)
	{
		m_mat3InitialInverseInertiaTensor = Mat3::ZERO;
		m_mat3InverseInertiaTensor = Mat3::ZERO;
		return m_fMass;
	}
	switch(type)
	{
		case T_SPHERE:
			m_fMass = 4.0*(m_fDensity*PI*m_fRadius*m_fRadius*m_fRadius)/3;
		break;
		case T_BLOCK:
			m_fMass = (m_fDensity*m_fWidth*m_fHeight*m_fDepth);
		break;
		case T_CYLINDER:
			m_fMass = m_fDensity*PI*m_fRadius*m_fRadius*m_fDepth;
		break;
		case T_VECTOR_ANALYSIS_OBJECT:
			m_fMass = BIG_MASS;
			m_fMassInverse = 0;
		break;
		default:
			m_fMass = BIG_MASS;
			m_fMassInverse = 0;
		//	return m_fMass;
		break;
	}
	if (m_fMass >= BIG_MASS)
	{
		m_mat3InitialInverseInertiaTensor = Mat3::ZERO;
		m_mat3InverseInertiaTensor = Mat3::ZERO;
		m_fMassInverse = 0;
		return m_fMass;
	}
	else m_fMassInverse = 1.0/m_fMass;
	ComputeInertia();
	Vector3D velocity;
	get_Velocity(velocity);
	set_Velocity(velocity);
//	std::wstring bstr("mass recomputed: mass = ");
//	_variant_t v(m_fMass);
		std::wstring bstr(L"1/mass recomputed: 1/mass = ");
		//_variant_t v(m_fMassInverse);
		std::wstring bstr2= std::to_wstring(m_fMassInverse); //v;
		bstr += bstr2;
		Fire_WriteResponse(bstr.c_str());
	return m_fMass;
}
///////////////////////////////////////////////////////////////////////////////
double CRigidBody::ComputeInertia()
{
	m_mat3InitialInverseInertiaTensor = Mat3::ZERO;
	if (m_fMassInverse == 0.0)
	{
		m_mat3InverseInertiaTensor = m_mat3InitialInverseInertiaTensor;
		return 0;
	}
	switch(type)
	{
		case T_SPHERE:
		{
			double JJ = 2.5*m_fMassInverse/(m_fRadius*m_fRadius);
			m_mat3InitialInverseInertiaTensor.m[0][0] = JJ;
			m_mat3InitialInverseInertiaTensor.m[1][1] = JJ;
			m_mat3InitialInverseInertiaTensor.m[2][2] = JJ;
			_J = Vector3D(1.0/JJ, 1.0/JJ, 1.0/JJ);
		}
		break;
		case T_BLOCK:
		{
//#define SQR(x) ((x)*(x))
#define SUMSQR(x,y) ((SQR(x)+SQR(y))/12.0)
			double a = SUMSQR( m_fHeight, m_fDepth );
			double b = SUMSQR( m_fDepth , m_fWidth );
			double c = SUMSQR( m_fWidth , m_fHeight);
			m_mat3InitialInverseInertiaTensor.m[0][0] = m_fMassInverse/a;
			m_mat3InitialInverseInertiaTensor.m[1][1] = m_fMassInverse/b;
			m_mat3InitialInverseInertiaTensor.m[2][2] = m_fMassInverse/c;
			_J = Vector3D(a/m_fMassInverse, b/m_fMassInverse, c/m_fMassInverse);
//#undef SQR
#undef SUMSQR
		}
		break;
		case T_CYLINDER:
		{
//#define SQR(x) ((x)*(x))
#define SUMSQR(x,y) ((SQR(x)/4.0 +SQR(y))/12.0)
			double a = SQR( m_fRadius/2 ) + SQR( m_fDepth/2 )/3;
			double c = 0.5*SQR( m_fRadius );
			m_mat3InitialInverseInertiaTensor.m[0][0] = m_fMassInverse/a;
			m_mat3InitialInverseInertiaTensor.m[1][1] = m_fMassInverse/a;
			m_mat3InitialInverseInertiaTensor.m[2][2] = m_fMassInverse/c;
			_J = Vector3D(a/m_fMassInverse, a/m_fMassInverse, c/m_fMassInverse);
//#undef SQR
#undef SUMSQR
		}
		break;
		case T_VECTOR_ANALYSIS_OBJECT:
			return 0;
		default:
			return 0;
	}

	m_mat3InverseInertiaTensor = m_mat3InitialInverseInertiaTensor;
	return 1.0/m_mat3InitialInverseInertiaTensor.det();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBody::ComputeNonContactAndInertialAcceleration(const Point3D& point)
{
	if (m_fMassInverse==0.0) return ZERO_3D;
	Point3D pt = point;//LocalToLab(point);
	Vector3D rel = pt - x;
	Mat3 S(rel);
	Vector3D angularVelocity;
	get_AngularVelocity(angularVelocity);
	Vector3D c = angularVelocity.vectorProd((angularVelocity.vectorProd(rel)));
	Vector3D b = L.vectorProd(angularVelocity);
	b += m_vecTorque;
/////////////////////assumption: ode solver has already updated m_Iinv:
	Mat3 Iinv;
	get_InverseInertiaTensor(Iinv);
	b  = Iinv*b;
	b  = S*b;
	Vector3D a = m_fMassInverse*m_vecForce;
	a -= b;
	a += c;

	return a;
}
///////////////////////////////////////////////////////////////////////////////
Mat3 CRigidBody::ComputeContactMatrix(const Point3D& point1, const Point3D& point2)
{
	if (m_fMassInverse==0.0) return Mat3::ZERO;
//assumption: ode solver has already updated m_Iinv:
	Mat3 SiIS;
	get_InverseInertiaTensor(SiIS);//m_Iinv;// inverse inertia tensor;result stored in m_Iinv 

	Vector3D rel1 = point1 - x;
	Vector3D rel2 = point2 - x;
	Mat3 S1(rel1);
	Mat3 S2(rel2);

	SiIS.postmult(S2);
	SiIS.premult(S1);
	SiIS.scale(-1.0);

	SiIS.m[0][0] += m_fMassInverse;
	SiIS.m[1][1] += m_fMassInverse;
	SiIS.m[2][2] += m_fMassInverse;
	return SiIS;
/*
	Mat3 id(Mat3::ID);
	id.scale(m_fMassInverse);

	id.add(SiIS);
	return id;
*/
}
///////////////////////////////////////////////////////////////////////////////
//binary collision:
Mat3 CRigidBody::ComputeCollisionMatrix(const Vector3D& contactPoint)
//==ComputeContactMatrix(contactPoint, contactPoint)
{
	if (m_fMassInverse==0.0)
	{
//		m_SiIS = Mat3::ZERO;
//		m_S = Mat3::ZERO;
		return Mat3::ZERO;
	}
/*
//assumption: ode solver has already updated m_Iinv:
	Mat3 R;
	R.set(q);
	Mat3 tR;
	tR.xpose(R);
	Vector3D rel = contactPoint - x;
	Mat3 S(rel);
	m_S = S;
*/
//	Mat3 GetI();

	Mat3 SiIS;
	get_InverseInertiaTensor(SiIS);//GetSiI(contactPoint);
	Mat3 S = GetS(contactPoint);

	SiIS.premult(S);
	SiIS.postmult(S);
	SiIS.scale(-1.0);

/*
//contactPoint - x = 0 gives correct result for resting contacts:
	Mat3 SiIS;
*/
	SiIS.m[0][0] += m_fMassInverse;
	SiIS.m[1][1] += m_fMassInverse;
	SiIS.m[2][2] += m_fMassInverse;

	return SiIS;//m_SiIS stored for inspection
}
///////////////////////////////////////////////////////////////////////////////
Mat3 CRigidBody::GetSiI(const Vector3D& contactPoint)
{
	if (m_fMassInverse==0.0) return Mat3::ZERO;
//assumption: ode solver has already updated m_Iinv:
//	Mat3 SiI = m_Iinv;// inverse inertia tensor;result stored in m_Iinv 

	Mat3 SiI;
	get_InverseInertiaTensor(SiI);

//	Vector3D rel = contactPoint - x;
	Mat3 S = GetS(contactPoint);//(rel);

	SiI.premult(S);
	SiI.scale(-1.0);
	return SiI;
}
///////////////////////////////////////////////////////////////////////////////
Mat3 CRigidBody::GetS(const Vector3D& contactPoint)
{
	Vector3D rel = contactPoint - x;
	Mat3 S(rel);
//	m_S = S;
	return S;
}
///////////////////////////////////////////////////////////////////////////////
Mat3 CRigidBody::GetW()
{
	Mat3 id(Mat3::ID);
	id.scale(m_fMassInverse);
	return id;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CRigidBody::ComputeVelocityOfPointInBody(const Point3D& point)
{
	Vector3D AngularVelocity;
	get_AngularVelocity(AngularVelocity);
	Vector3D velocityCM;
	get_Velocity(velocityCM);
	Vector3D relpos		= point - x;
	Vector3D velocity	= AngularVelocity .vectorProd(relpos) ;
	velocity			+= velocityCM;
	return velocity;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ComputeImpulses()
{
	m_vecImpulseLab				= ZERO_3D;
	m_vecImpulseTorqueLab		= ZERO_3D;
	if (m_fMassInverse==0.0) return;
	vector<Contact*>::iterator j;
	for (j=m_ContactPointList.begin();j != m_ContactPointList.end();++j)
	{
		Contact* ptr = dynamic_cast<Contact*>(*j);
		if (this == ptr->m_body1)
		{
	//		m_vecImpulseLab			+= ptr->impulseLab;
	//		m_vecImpulseTorqueLab	+= (ptr->m_pointContact()-x).vectorProd()ptr->impulseLab;
			if (ptr->m_bPermute == false)
			{
				m_vecImpulseLab			+= ptr->impulseLab;
				m_vecImpulseTorqueLab	+= (ptr->m_pointContact()-x).vectorProd(ptr->impulseLab);
			}
			else
			{
				m_vecImpulseLab			-= ptr->impulseLab;
				m_vecImpulseTorqueLab	-= (ptr->m_pointContact()-x).vectorProd(ptr->impulseLab);
			}
		}
		else
		if (this == ptr->m_body2)
		{
	//		m_vecImpulseLab			-= ptr->impulseLab;
	//		m_vecImpulseTorqueLab	-= (ptr->m_pointContact()-x).vectorProd()ptr->impulseLab;
			if (ptr->m_bPermute == false)
			{
				m_vecImpulseLab			-= ptr->impulseLab;
				m_vecImpulseTorqueLab	-= (ptr->m_pointContact()-x).vectorProd(ptr->impulseLab);
			}
			else
			{
				m_vecImpulseLab			+= ptr->impulseLab;
				m_vecImpulseTorqueLab	+= (ptr->m_pointContact()-x).vectorProd(ptr->impulseLab);
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ComputeMomentumChanges()
{
	if (m_fMassInverse==0.0) return;
/*
	m_vecImpulseLab				= ZERO_3D;
	m_vecImpulseTorqueLab		= ZERO_3D;
	vector<Contact*>::iterator j;
	for (j=m_ContactPointList.begin();j != m_ContactPointList.end();++j)
	{
		Contact* ptr = dynamic_cast<Contact*>(*j);
		if (this == ptr->m_body1)
		{
			m_vecImpulseLab			+= ptr->impulseLab;
			m_vecImpulseTorqueLab	+= (ptr->m_pointContact()-x).vectorProd()ptr->impulseLab;
		}
		else
		if (this == ptr->m_body2)
		{
			m_vecImpulseLab			-= ptr->impulseLab;
			m_vecImpulseTorqueLab	-= (ptr->m_pointContact()-x).vectorProd()ptr->impulseLab;
		}
	}
*/
	p		+= m_vecImpulseLab;
	L	+= m_vecImpulseTorqueLab;
	m_vecImpulseLab				= ZERO_3D;
	m_vecImpulseTorqueLab		= ZERO_3D;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::DefineShape()
{
	const double scale = 1.0;
	if (CRigidBody::m_boolSpheresOnly)
	{
		return;
	}
	if (m_pPQP_Model) delete m_pPQP_Model, m_pPQP_Model=0;
//	PQP_Update();
///////////////////////////////////////////////////////////////////////////////
	switch(type)
	{
		case T_SPHERE:
		{
			PQP_Sphere(m_pPQP_Model, m_fRadius);
		}
		break;
///////////////////////////////////////////////////////////////////////////////
		case T_BLOCK:
		{
			if (m_fWidth==0.0) m_fWidth=0.2;
			if (m_fHeight==0.0) m_fHeight=0.2;
			if (m_fDepth==0.0) m_fDepth=0.2;
			m_pPQP_Model = new PQP_Model;
			m_pPQP_Model->BeginModel();//for PQP
			double r[3] = { m_fWidth/2, m_fHeight/2, m_fDepth/2 };
			for (int i = 0; i < sizeof(blkIndices)/sizeof(blkIndices[0]); i++)
			{
				m_pPQP_Model->AddTri(	(PQP_REAL*)(r*vertsBlock[blkIndices[i][0]]),
										(PQP_REAL*)(r*vertsBlock[blkIndices[i][1]]),
										(PQP_REAL*)(r*vertsBlock[blkIndices[i][2]]),
										i);
			}
			m_pPQP_Model->EndModel();
///////////////////////////////////////////////////////////////////////////////
		}
		break;
///////////////////////////////////////////////////////////////////////////////
		case T_CYLINDER:
		{
			if (m_fRadius==0.0) m_fRadius=0.2;
			if (m_fDepth==0.0) m_fDepth=0.2;
			m_pPQP_Model = new PQP_Model;
			m_pPQP_Model->BeginModel();//for PQP
			double r[3] = { m_fRadius, m_fRadius, m_fDepth/2 };
			for (int i = 0; i < sizeof(cylinderIndices)/sizeof(cylinderIndices[0]); i++)
			{
				m_pPQP_Model->AddTri(	(PQP_REAL*)(r*vertsCylinder[cylinderIndices[i][0]]),
										(PQP_REAL*)(r*vertsCylinder[cylinderIndices[i][1]]),
										(PQP_REAL*)(r*vertsCylinder[cylinderIndices[i][2]]),
										i);
			}
			m_pPQP_Model->EndModel();
		}
		break;
///////////////////////////////////////////////////////////////////////////////
#if 0
		case coneBody:
		{
			if (m_fRadius==0.0) m_fRadius=0.2;
			if (m_fDepth==0.0) m_fDepth=0.2;
		}
			return;
///////////////////////////////////////////////////////////////////////////////
		case nurbsSurfaceBody:
		{
//			m_pSoNurbsSurface->.setValue();
//	SoSFInt32		numUControlPoints; // Number of control points in
//	SoSFInt32		numVControlPoints; // the U and V directions.
//	SoSFInt32		numSControlPoints; // Number of control points in
//	SoSFInt32		numTControlPoints; // the S and T directions
//	SoMFFloat		uKnotVector;	   // The knot vectors in the U and
//	SoMFFloat		vKnotVector;	   // V directions.
//	SoMFFloat		sKnotVector;	   // The knot vectors in the S and
//	SoMFFloat		tKnotVector;	   // T directions.
		}
			return;
///////////////////////////////////////////////////////////////////////////////
		case nurbsCurveBody:
		{
//			m_pSoNurbsCurve->.setValue();
//	SoSFInt32		numControlPoints; // Number of control points
//	SoMFFloat		knotVector;	   // The knot vector
		}
			return;
///////////////////////////////////////////////////////////////////////////////
		case indexedNurbsSurfaceBody:
		{
//			m_pSoIndexedNurbsSurface->.setValue();
//	SoSFInt32		numUControlPoints; // Number of control points in
//	SoSFInt32		numVControlPoints; // the U and V directions.
//	SoMFInt32		coordIndex;	   // Coordinate indices
//	SoMFFloat		uKnotVector;	   // The knot vectors in the U and
//	SoMFFloat		vKnotVector;	   // V directions.
//	SoSFInt32		numSControlPoints; // Number of control points in
//	SoSFInt32		numTControlPoints; // the S and T directions
//	SoMFInt32		textureCoordIndex; // Texture Coordinate indices
//	SoMFFloat		sKnotVector;	   // The knot vectors in the S and
//	SoMFFloat		tKnotVector;	   // V directions.
		}
			return;
///////////////////////////////////////////////////////////////////////////////
		case indexedNurbsCurveBody:
		{
//			m_pSoIndexedNurbsCurve->.setValue();
//	SoSFInt32		numControlPoints;  // Number of control points in
//	SoMFInt32		coordIndex;	   // Coordinate indices
//	SoMFFloat		knotVector;		// The knot vectors in the U and
		}
			return;
///////////////////////////////////////////////////////////////////////////////
		case vertexShapeBody:
		{
//			m_pSoVertexShape->.setValue();
//protected:
//// Constructor - makes this abstract
//	SoVertexShape();
		}
			return;
///////////////////////////////////////////////////////////////////////////////
#endif
		case T_VECTOR_ANALYSIS_OBJECT:
			return;
		default:
			return;
	}
	theApp.GetRigidBodyManager()->PQP_Update();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::ComputeNormalAndFrictionalForcesAndTorque() {
	const double speedTol = 0.0001;
	if (m_fMassInverse==0.0) return;
	if (!m_bConstraintedToPlane) return;
	Vector3D f= -m_vecForce;
	Vector3D t= m_vecTorque;
	double c = fabs(cos(m_fAngleOfIncline));
	double s = sin(m_fAngleOfIncline);
	if (m_fAngleOfIncline > PI_DIVIDED_BY_2) s *= -1;
	Vector3D normalToIncline(-s,c,0);

	Vector3D v;
	get_Velocity(v);
	double speed = v.norm();

	bool pbb = type==T_BLOCK;
	if (pbb)
	{
		Vector3D tangentToIncline(c,s,0);
		double normalForce = f*normalToIncline;
		if (speed < speedTol)
		{
			if ( m_fCoefficientOfStaticFriction > fabs(tan(m_fAngleOfIncline)) )
			{
				m_vecForce = ZERO_3D;
				m_vecTorque= ZERO_3D;
				return;
			}
		}
//		double& g	= -theApp.theSystem.m_rigidBodyManager.fieldsManager->m_ExternalGravitationalFieldY;
//		double mass = 1.0/m_fMassInverse;
//		double normalForce = mass*g*c;
		m_vecInclineNormalForce = normalForce*normalToIncline;
		m_vecInclineFrictionalForce = m_fCoefficientOfKineticFriction*normalForce*tangentToIncline;
		if (m_vecInclineFrictionalForce*v > 0.0) m_vecInclineFrictionalForce.negate();
		return;
	}

#if 0
	Mat3 i;
	get_InverseInertiaTensor(i);
	double a = (m_fMassInverse/(m_fRadius*m_fRadius))/i.xx;
	double b = (m_fMassInverse/(m_fRadius*m_fRadius))/i.zz;
	double d = normalToIncline*f;

/*
	List((b + b*c*c + s*s)/(2.*(1 + b)),((-1 + b)*2*s*c)/(4.*(1 + b)),0,s/2.),
	List(((-1 + b)*2*s*c)/(4.*(1 + b)),(b + c*c + b*s*s)/(2.*(1 + b)),0,-c/2.),
	List(0,0,a/(1 + a),0),
	List(-s/2.,c/2.,0,0.5)
*/

	Vector3D X((b + b*c*c + s*s)/(2.*(1 + b)),((-1 + b)*2*s*c)/(4.*(1 + b)),0);
	Vector3D Y(((-1 + b)*2*s*c)/(4.*(1 + b)),(b + c*c + b*s*s)/(2.*(1 + b)),0);
	Vector3D Z(0,0,a/(1 + a));
	Vector3D W(-s/2.,c/2.,0);

	m_vecInclineNormalForce = ( W*f + 0.5*d )*normalToIncline;
//	m_vecInclineFrictionalForce = Vector3D( X*f + d*s/2., Y*f - d*c/2., Z*f );
	Vector3D inclineFrictionalForce( X*f + d*s/2., Y*f - d*c/2., Z*f );
	double frictionalForce = inclineFrictionalForce.norm();
	if (speed < speedTol)
	{
		m_vecInclineFrictionalForce = inclineFrictionalForce;
	}
	else
	{
		m_vecInclineFrictionalForce = (-frictionalForce/speed)*v;
	}
	Vector3D g= m_vecForce+m_vecInclineNormalForce+m_vecInclineFrictionalForce;
	m_vecInclineTorque	= (m_fRadius*-normalToIncline).vectorProd()m_vecInclineFrictionalForce;
#endif
#if 10
	Vector3D	w;
	get_AngularVelocity(w);
	Mat3		ii;
	get_InverseInertiaTensor(ii);
	double		Qnormal			= normalToIncline*f;//f= -m_vecForce;
	m_vecInclineNormalForce		= Qnormal*normalToIncline;

	Vector3D	rel				= -m_fRadius*normalToIncline;
	Point3D		contactPoint	= x + rel;
	Mat3		cm				= ComputeCollisionMatrix(contactPoint);
	Mat3		icm;
	int			not_invertible	= icm.invert(cm);
	if (not_invertible == 1)
	{
		ASSERT(0);
		return;
	}
//	�r == delta r = position[contact point] - position[center of mass]
	Vector3D	tLw								= m_vecTorque + L.vectorProd(w);
	Vector3D	dw								= ii*tLw;//angular acceleration about the center of mass
	Mat3		S(rel);
//	Mat3		Sii								= S*ii;
//	Vector3D	SiitLw							= Sii*tLw;
//	velocity of contact point v = v_cm + w @ �r 
//	acceleration of contact point a = a_cm + dw @ �r + w @ ( w @ �r )
	Vector3D	SiitLw							= S*dw;// == �r @ dw == - dw @ �r
	Vector3D	wrw								= w.vectorProd((rel.vectorProd(w)));// == - w @ ( w @ �r )
//	- a_cm - [ dw @ �r + w @ ( w @ �r )] = - acceleration[contact point]
	Vector3D	acceleration_contact_point_m	= m_fMassInverse*f + ( SiitLw + wrw );
//	Vector3D	ConstraintAndFrictionalForce	= icm*acceleration_contact_point_m;// publish this?
//	double		checkNormalForce				= (normalToIncline*ConstraintAndFrictionalForce) - Qnormal;
//	ASSERT(fabs(checkNormalForce) < 0.1e-2);

//	component parallel to incline [in tangent plane]
	f											= normalToIncline.vectorProd((f.vectorProd(normalToIncline)));
//	- a_cm - [ dw @ �r + w @ ( w @ �r )] = - acceleration[contact point]
	Vector3D	a_p_m			= m_fMassInverse*f + ( SiitLw + wrw );
	Vector3D	FrictionalForce	= icm*a_p_m;// publish this?

	m_vecInclineFrictionalForce	= normalToIncline.vectorProd((FrictionalForce.vectorProd(normalToIncline)));
	double		frictionalForce	= m_vecInclineFrictionalForce.norm();
	double		frictioMaxForce	= m_fCoefficientOfStaticFriction * fabs(Qnormal);
	if ( frictionalForce> frictioMaxForce )
	{
		m_vecInclineFrictionalForce.normalize();
		m_vecInclineFrictionalForce	*= frictioMaxForce;
	}
	m_vecInclineTorque			= rel.vectorProd(m_vecInclineFrictionalForce);
#endif
}
///////////////////////////////////////////////////////////////////////////////
//dformt_no_matherr.LIB SMATHD.LIB slatecd.lib BLASd.lib LAPACKd.lib arpackd.lib  QHullD.lib 
//gsld.lib gslcblasd.lib myGLAUXD.LIB GLU32.LIB OPENGL32.LIB CVOdeD.lib hdf5d.lib 
//quartz.lib strmbasd.lib WinMM.lib vfw32.lib ZLibd.lib

//dformt.lib,LIBC.lib,GLAUX.LIB,glut32.lib,libcmt.lib
//
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::AddForceAndTorqueForZeroAccelerationAtFixedPoint(const Point3D& point)
{
	Vector3D force;
	Vector3D torque;
	GetForceAndTorqueForZeroAccelerationAtFixedPoint( point, m_vecForce, m_vecTorque, force, torque);
	m_vecForce	+= force;
	m_vecTorque	+= torque;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::GetForceAndTorqueForZeroAccelerationAtFixedPoint(const Point3D& point, const Vector3D& input_force, const Vector3D& input_torque, Vector3D& force, Vector3D& torque)
{
	Vector3D	rel				= point - x;
	Vector3D	w;
	get_AngularVelocity(w);
	Mat3		ii;
	get_InverseInertiaTensor(ii);
	Vector3D	wrw				= w.vectorProd((rel.vectorProd(w)));
	Mat3		S(rel);
	Mat3		Sii				= S*ii;
	Vector3D	tLw				= input_torque + L.vectorProd(w);
	Vector3D	SiitLw			= Sii*tLw;

	Vector3D	B				= -m_fMassInverse*input_force + SiitLw + wrw;
	Mat3		cm				= ComputeCollisionMatrix(point);
	Mat3		icm;
	int			not_invertible	= icm.invert(cm);
	if (!not_invertible)
	{

try{
		const int nSize = 3;

#if 10
		double **a = CVODE::denalloc(nSize);
		if (a == NULL) throw SolveIt_ERROR(L"denalloc memory request failed");

		CVODE::integer *pivot = CVODE::denallocpiv(nSize);
		if (pivot == NULL) throw SolveIt_ERROR(L"denallocpiv memory request failed");
/*
		CVODE::integer ier = CVODE::gefa(a,nSize,pivot);
		if (ier > 0) throw SolveIt_ERROR(L"zero element encountered during");
		if (ier == 0) CVODE::gesl(a,nSize,pivot,b);
*/

		//auto_ptr<double> bbuf( new double[nSize] );
		std::unique_ptr<double> bbuf = std::make_unique<double>(nSize);
		double* b = bbuf.get();

		for (int row=0; row<nSize; ++row)
		{
			b[row]	= B.v[row];
			for (int col=0; col<nSize; ++col)
			{
				a[col][row] = cm.m[row][col];// for CVODE
			//	a[row][col] = cm.m[row][col];// for CVODE
			}
		}

		CVODE::integer ier = CVODE::gefa(a,nSize,pivot);
		if (ier > 0) throw SolveIt_ERROR(L"CVODE::gefa error");
		if (ier == 0) CVODE::gesl(a,nSize,pivot,b);


			force = b;
			torque = rel.vectorProd(force);


		CVODE::denfreepiv(pivot);
		CVODE::denfree(a);
			return;//
#endif


#if 0
	1	{
using namespace JAMA;
using namespace TNT;

		TNT::Array1D<double> b(3,B.v);
		TNT::Array2D<double> T(3,3,(double*) cm.m);
		int n = T.dim1();
	//	TNT::Array2D<double> L(n, n);
		TNT::Array2D<double> L;
		TNT::Array1D<TNT::Subscript> index(n);
		TNT::Array1D<double> C(n), D(n);
		JAMA::QR<double> qr(T);
		if (qr.isFullRank())
		{
			TNT::Array2D<double> b(3,1,(double*) cm.m);
			TNT::Array2D<double> f = qr.solve(b);
			Vector3D F(f[0][0],f[1][0],f[2][0]);
			force = F;
			torque = rel.vectorProd(F);
		//	return;
		}
#define not_not_yet_implemented
#if defined(not_not_yet_implemented)
		{
		TNT::Array1D<double> b(3,B.v);
		TNT::Array2D<double> T(3,3,(double*) cm.m);
		JAMA::Cholesky<double> chol(T);
		if (!chol.is_spd())
		{
			TNT::Array2D<double> b(3,1,(double*) cm.m);
			TNT::Array2D<double> f = chol.solve(b);
			L = chol.getL();
// solve Ax =b, as L*L'x =b
//  let y=L'x, then
//   solve L y = b;
//   solve L'x = y;
		//	TNT::Array1D<double> y = TNT::Lower_triangular_solve(L, b);
		//	TNT::Array1D<double> f = TNT::Upper_triangular_solve(TNT::Transpose_view(L), y);

			Vector3D F(f[0][0],f[1][0],f[2][0]);
			force = F;
			torque = rel.vectorProd(F);
			return;
		}
		}
		}
#endif
#endif
///////////////////////////////////////////////////////////////////////////////
		return;
}
catch( SolveIt_Error& e){e.why();}catch(...){
}
	}

	force = icm*B;
	torque= rel.vectorProd(force);
}
///////////////////////////////////////////////////////////////////////////////

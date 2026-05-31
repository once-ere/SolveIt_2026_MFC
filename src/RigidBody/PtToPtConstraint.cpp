// PtToPtConstraint.cpp: implementation of the CPointToPointConstraint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "PtToPtConstraint.h"
#include "ConstraintManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
double CPointToPointConstraint::PointToPointConstraint() {
	Point3D		pt1	= body1? body1->LocalToLab(point1):point1;
	Point3D		pt2	= body2? body2->LocalToLab(point2):point2;
	Vector3D	rel	= pt1 - pt2;
	double		r	= rel.norm();
	double		rs	= r - separation;
	if ( separation > 1.0e-6 ) return rs;
	return  rs*(r + separation);
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CPointToPointConstraint::DPointToPointConstraint_dx1() {
	if (!body1) throw SolveIt_ERROR("body1==0");
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt1 - pt2;
	if ( separation > 1.0e-6 )
	{
		rel.normalize();
		return rel;
	}
	else
	{
	}
	return 2*rel;
}
///////////////////////////////////////////////////////////////////////////////
Vector3D CPointToPointConstraint::DPointToPointConstraint_dx2() {
	if (!body2) throw SolveIt_ERROR("body2==0");
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt2 - pt1;
	if ( separation > 1.0e-6 )
	{
		rel.normalize();
		return rel;
	}
	else
	{
	}
	return 2*rel;
}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CPointToPointConstraint::CPointToPointConstraint(
													CRigidBody* const _body1,
													const Point3D& _point1,
													CRigidBody* const _body2,
													const Point3D& _point2,
													double _separation) :
	body1(_body1),body2(_body2),
	point1(_point1),point2(_point2),
	separation(_separation),
	separation2(_separation*_separation)
//	ud(body1,point1,body2,point2,_separation)
{
//	f = PointToPointConstraint;
//	if (body1) df_dx1 = DPointToPointConstraint_dx1;
//	if (body2) df_dx2 = DPointToPointConstraint_dx2;
	if (body1)
	{
		body1->activate();
		body1->set_Velocity(GetConsistentVelocity( body1, ZERO_3D));
		body1->AddConstraintToList(this);
	}
	if (body2)
	{
		body2->activate();
		body2->set_Velocity(GetConsistentVelocity( body2, ZERO_3D));
		body2->AddConstraintToList(this);
	}
//	vec_df_dx.push_back(DPointToPointConstraint_dx1);
//	vec_df_dx.push_back(DPointToPointConstraint_dx2);
}
///////////////////////////////////////////////////////////////////////////////
CPointToPointConstraint::~CPointToPointConstraint() {
	Destroy();
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::AddConstraintForce(	vector<double>& f,
													map<double *, int, MyCompare>& var2col
												) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
//	Vector3D rel = pt2 - pt1;

	if (body1 && body1->m_bConstraintForceShouldBeUpdated) {
		State& s = *static_cast<State*const>(body1);
		Vector3D F(f[var2col[&s.x.x]],f[var2col[&s.x.y]],f[var2col[&s.x.z]]);
		body1->m_vecForce += F;
	//	Vector3D rel = pt1 - body1->m_positionCM;
	//	body1->m_vecTorque += rel^F;
		body1->m_bConstraintForceShouldBeUpdated = false;
	}

	if (body2 && body2->m_bConstraintForceShouldBeUpdated) {
		State& s = *static_cast<State*const>(body2);
		Vector3D F(f[var2col[&s.x.x]],f[var2col[&s.x.y]],f[var2col[&s.x.z]]);
		body2->m_vecForce += F;
	//	Vector3D rel = pt2 - body2->m_positionCM;
	//	body2->m_vecTorque += rel^F;
		body2->m_bConstraintForceShouldBeUpdated = false;
	}
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::Error(double t, vector<double>& vecError) {
	double error = PointToPointConstraint();//f((void*)&ud);
	vecError.push_back(error);
//	vecError.push_back(error);
//	vecError.push_back(error);
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::Jacobian(double t,
									   vector<double>& vecRowOfJacobian,
									   vector<int>& cols,
									   map< double *, int, MyCompare>& var2col
									   ) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt1 - pt2;
	rel.normalize();
	long j;
/*
	if (body1) {
		State& s = *static_cast<State*const>(body1);
		for (j=0;j<3;++j) {
			cols.push_back(var2col[&s.x[j]]);
			vecRowOfJacobian.push_back(rel[j]);
		}
	}
	if (body2) {
		State& s = *static_cast<State*const>(body2);
		for (j=0;j<3;++j) {
			cols.push_back(var2col[&s.x[j]]);//col++
			vecRowOfJacobian.push_back(-rel[j]);
		}
	}theApp.GetConstraintManager()
*/
	vector<CRigidBody*>::iterator i;
	for (i = theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(); i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(); ++i)
	{
		CRigidBody* body = *i;
		if (body==body1 || body==body2)
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			if (body==body1)
			{
				for (j=0;j<3;++j) {
//					vecRowOfJacobian.push_back(rel[j]);
					vecRowOfJacobian[n+j] = rel[j];
				}
			}
			if (body==body2)
			{
				for (j=0;j<3;++j) {
//					vecRowOfJacobian.push_back(-rel[j]);
					vecRowOfJacobian[n+j] = -rel[j];
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
Point3D CPointToPointConstraint::GetConsistentPosition(CRigidBody* body, const Point3D& pt) {
	if (body==0) return ZERO_3D;
	return ZERO_3D;
}
///////////////////////////////////////////////////////////////////////////////
struct _rv
{  
//	Vector3D* vel;//Velocity
//	Vector3D* rel;
	double vel;//Velocity component
	double rel;
	long x;
//	_rv(Vector3D*v,Vector3D*r) : vel(v), rel(r) {}
	_rv(double& v, double& r, const int n) : vel(v), rel(r), x(n) {}
	_rv(const _rv& a) : vel(a.vel), rel(a.rel), x(a.x)   {}
	_rv& operator=(const _rv& a)
	{
		vel=a.vel, rel=a.rel, x=a.x;
		return *this;
	}
};
struct _rvCompare
{  
	bool operator()(const _rv& a, const _rv& b) const
	{
		if (a.vel==0.0) return true;
		if (b.vel==0.0) return false;
		return fabs(a.rel) < fabs(b.rel);
	}
};
struct _rvCompare2
{  
	bool operator()(const _rv& a, const _rv& b) const
	{
		return a.x < b.x;
	}
};
///////////////////////////////////////////////////////////////////////////////


Vector3D CPointToPointConstraint::GetConsistentVelocity(CRigidBody* body, const Point3D& pt) {
	if (body==0) return ZERO_3D;
//	Point3D pT = body->LocalToLab(pt);
//	Vector3D v = body->ComputeVelocityOfPointInBody(pT);
	Vector3D Velocity;
	body->get_Velocity(Velocity);
	if (Velocity==ZERO_3D) return ZERO_3D;
//	TNT::Matrix<double> J;
//	theApp.GetConstraintManager().Jacobian(theSystem->m__t, J, theApp.GetConstraintManager().m_listPointMassConstraints.size());
//	vector<double> V;
//	GetSystemVelocity(V);

	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt1 - pt2;
	rel.normalize();
	if (rel*Velocity == 0.0) {
		return Velocity;
	}
	return rel.vectorProd((Velocity.vectorProd(rel)));
//	if (body == body1) {
//	}
//	if (body == body2) {
//	}
	vector<_rv> vec_rv;
	_rv X ( Velocity.x, rel.x, 0);
	_rv Y ( Velocity.y, rel.y, 1);
	_rv Z ( Velocity.z, rel.z, 2);
	vec_rv.push_back(X ); vec_rv.push_back(Y ); vec_rv.push_back(Z );
	_rvCompare __rvCompare;
	sort(vec_rv.begin(), vec_rv.end(), __rvCompare);
	_rv& mx = vec_rv[2];//.back();
	mx.vel = -(vec_rv[0].vel*vec_rv[0].rel+vec_rv[1].vel*vec_rv[1].rel)/mx.rel;
	_rvCompare2 __rvCompare2;
	sort(vec_rv.begin(), vec_rv.end(), __rvCompare2);
	Vector3D ConstrainedVelocity(vec_rv[0].vel,vec_rv[1].vel,vec_rv[2].vel);
	ASSERT (rel*ConstrainedVelocity == 0.0);
	return ConstrainedVelocity;
}
///////////////////////////////////////////////////////////////////////////////
// D_Jacobian_dt(a)_i = Sum_j J_(a)__i_j * V_j
void CPointToPointConstraint::D_Jacobian_dt(	double t,
							vector<double>& vecRowOfDJacobian,
							vector<int>& cols,
							map<double *, int, MyCompare>& var2col
							) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D v1 = body1? body1->ComputeVelocityOfPointInBody(pt1):ZERO_3D;
	Vector3D v2 = body2? body2->ComputeVelocityOfPointInBody(pt2):ZERO_3D;
	Vector3D v = v1 - v2;

	Vector3D rel = pt1 - pt2;
	double r2	= rel*rel;
	double r	= sqrt(r2);
	const double c_small = 0.1e-33;
	ASSERT (r > c_small);
	if (r < c_small) return;
	v /= r;
	double rv	= rel*v;
	rel *= rv/r2;
	v -= rel;


	long j;
	if (body1) {
		State& s = *static_cast<State*const>(body1);
		for (j=0;j<3;++j) {
			cols.push_back(var2col[&s.x[j]]);
			vecRowOfDJacobian.push_back(v[j]);
		}
	}
//	col += DEGREES_FREEDOM - sizeof(f1)/sizeof(f1.x);
	if (body2) {
		State& s = *static_cast<State*const>(body2);
		for (j=0;j<3;++j) {
			cols.push_back(var2col[&s.x[j]]);//col++
			vecRowOfDJacobian.push_back(-v[j]);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
long CPointToPointConstraint::GetNumberOfConstrainedBodies() {
	return body1? 2:1;
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::AssignVarsToColumns(map<int, double *>& col2var,
												  map<double *, int, MyCompare>& var2col,
												  int& col) {
	vector<double>& W =
		theApp.GetConstraintManager().m_vecSystemInverseMassMatrix;
	vector<double>& f =
		theApp.GetConstraintManager().m_vecUnconstrainedResultantForce;
	vector<double>& v =
		theApp.GetConstraintManager().m_vecSystemVelocity;

	map<double *, int, MyCompare>::iterator i;

	if (body1) {
		State& s = *static_cast<State* const>(body1);
		Point3D pt1 = body1->LocalToLab(point1);
		Vector3D V = body1->ComputeVelocityOfPointInBody(pt1);
		i = var2col.find(&s.x.x);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.x), f.push_back(body1->m_vecForce.x), var2col[&s.x.x] = col, col2var[col++] = &s.x.x;
		i = var2col.find(&s.x.y);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.y), f.push_back(body1->m_vecForce.y), var2col[&s.x.y] = col, col2var[col++] = &s.x.y;
		i = var2col.find(&s.x.z);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.z), f.push_back(body1->m_vecForce.z), var2col[&s.x.z] = col, col2var[col++] = &s.x.z;
		body1->m_bConstraintForceShouldBeUpdated = true;
	}
	if (body2) {
		State& s = *static_cast<State* const>(body2);
		Point3D pt2 = body2->LocalToLab(point2);
		Vector3D V = body2->ComputeVelocityOfPointInBody(pt2);
		i = var2col.find(&s.x.x);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.x), f.push_back(body2->m_vecForce.x), var2col[&s.x.x] = col, col2var[col++] = &s.x.x;
		i = var2col.find(&s.x.y);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.y), f.push_back(body2->m_vecForce.y), var2col[&s.x.y] = col, col2var[col++] = &s.x.y;
		i = var2col.find(&s.x.z);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.z), f.push_back(body2->m_vecForce.z), var2col[&s.x.z] = col, col2var[col++] = &s.x.z;
		body2->m_bConstraintForceShouldBeUpdated = true;
	}
}
//		if (pbody->m_bSubjectToActiveConstraint && pbody->m_fMassInverse>0.0) {
//			rhs.push_back(pbody->m_vecForce.x);
//			rhs.push_back(pbody->m_vecForce.y);
//			rhs.push_back(pbody->m_vecForce.z);
	//	if (pbody->active()) {
	//		Vector3D v = pbody->GetVelocity();
	//		rhs.push_back(v.x);
	//		rhs.push_back(v.y);
	//		rhs.push_back(v.z);


///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::Destroy() {
	if (body1) body1->deactivate();
	if (body2) body2->deactivate();
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::DrawState()
{
//	glPushMatrix();
//	glTranslated(x, y, z);
//draw m_velocityCM and m_acceleration vectors
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	glLineWidth(3.0f);
glBegin( GL_LINES);
	glColor3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f(GLfloat(pt1.x), GLfloat(pt1.y), GLfloat(pt1.z));
	glVertex3f(GLfloat(pt2.x), GLfloat(pt2.y), GLfloat(pt2.z));
glEnd();

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glLineWidth(1.0f);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::AddConstraintImpulse(	vector<double>& f,
													map<double *, int, MyCompare>& var2col
												) {
	if (body1 && body1->m_bConstraintImpulseShouldBeUpdated) {
		State& s = *static_cast<State*const>(body1);
		body1->m_vecImpulseLab.x += f[var2col[&s.x.x]];
		body1->m_vecImpulseLab.y += f[var2col[&s.x.y]];
		body1->m_vecImpulseLab.z += f[var2col[&s.x.z]];
		body1->m_bConstraintImpulseShouldBeUpdated = false;
	}

	if (body2 && body2->m_bConstraintImpulseShouldBeUpdated) {
		State& s = *static_cast<State*const>(body2);
		body2->m_vecImpulseLab.x += f[var2col[&s.x.x]];
		body2->m_vecImpulseLab.y += f[var2col[&s.x.y]];
		body2->m_vecImpulseLab.z += f[var2col[&s.x.z]];
		body2->m_bConstraintImpulseShouldBeUpdated = false;
	}
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::AssignVarsToColumnsDuringCollision(map<int, double *>& col2var,
												  map<double *, int, MyCompare>& var2col,
												  int& col) {
	vector<double>& W =
		theApp.GetConstraintManager().m_vecSystemInverseMassMatrix;
	vector<double>& f =
		theApp.GetConstraintManager().m_vecUnconstrainedCollisionImpulse;
	vector<double>& v =
		theApp.GetConstraintManager().m_vecSystemVelocity;

	map<double *, int, MyCompare>::iterator i;

	if (body1) {
		State& s = *static_cast<State* const>(body1);
		Point3D pt1 = body1->LocalToLab(point1);
		Vector3D V = body1->ComputeVelocityOfPointInBody(pt1);
		i = var2col.find(&s.x.x);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.x), f.push_back(body1->m_vecImpulseLab.x), var2col[&s.x.x] = col, col2var[col++] = &s.x.x;
		i = var2col.find(&s.x.y);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.y), f.push_back(body1->m_vecImpulseLab.y), var2col[&s.x.y] = col, col2var[col++] = &s.x.y;
		i = var2col.find(&s.x.z);
		if (i==var2col.end()) W.push_back(body1->m_fMassInverse), v.push_back(V.z), f.push_back(body1->m_vecImpulseLab.z), var2col[&s.x.z] = col, col2var[col++] = &s.x.z;
		body1->m_bConstraintImpulseShouldBeUpdated = true;
	}
	if (body2) {
		State& s = *static_cast<State* const>(body2);
		Point3D pt2 = body2->LocalToLab(point2);
		Vector3D V = body2->ComputeVelocityOfPointInBody(pt2);
		i = var2col.find(&s.x.x);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.x), f.push_back(body2->m_vecImpulseLab.x), var2col[&s.x.x] = col, col2var[col++] = &s.x.x;
		i = var2col.find(&s.x.y);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.y), f.push_back(body2->m_vecImpulseLab.y), var2col[&s.x.y] = col, col2var[col++] = &s.x.y;
		i = var2col.find(&s.x.z);
		if (i==var2col.end()) W.push_back(body2->m_fMassInverse), v.push_back(V.z), f.push_back(body2->m_vecImpulseLab.z), var2col[&s.x.z] = col, col2var[col++] = &s.x.z;
		body2->m_bConstraintImpulseShouldBeUpdated = true;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::Jacobian(double t, vector<double>& vecRowOfJacobian) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt1 - pt2;
	rel.normalize();
	long j;


	if (body1) {
		BodyCompareEqual bodyCompareEqual(body1);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfJacobian.push_back(rel[j]);
				vecRowOfJacobian[n+j] = rel[j];
			}
		}
	}



	if (body2) {
		BodyCompareEqual bodyCompareEqual(body2);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfJacobian.push_back(-rel[j]);
				vecRowOfJacobian[n+j] = -rel[j];
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::NormalizedJacobian(double t, vector<double>& vecRowOfJacobian) {
	Jacobian( t, vecRowOfJacobian);
/*//////////////////////////////////////////////////////////////////////////////
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	Vector3D rel = pt1 - pt2;
	rel.normalize();
	long j;
///////////////////////////////////////////////////////////////////////////////
	if (body1) {
		BodyCompareEqual bodyCompareEqual(body1);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfJacobian.push_back(rel[j]);
				vecRowOfJacobian[n+j] = rel[j];
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (body2) {
		BodyCompareEqual bodyCompareEqual(body2);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfJacobian.push_back(-rel[j]);
				vecRowOfJacobian[n+j] = -rel[j];
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////*/
}
///////////////////////////////////////////////////////////////////////////////
// $r'/r - ($r/r^2)(r'); $r.$r		= r^2, $r.$r' = r r'; r' = $r.$r'/ r 
// $r'/r - ($r/r^2)( $r.$r'/ r )	= $r'/r - ($r/r^3)( $r.$r' )
//									= 1/r [ I - ($r $r/r^2) ] $r'
void CPointToPointConstraint::HessianDotVelocity(double t,
							vector<double>& vecRowOfHessianDotVelocity
							)
{
	Point3D		pt1	= body1? body1->LocalToLab(point1):point1;
	Point3D		pt2	= body2? body2->LocalToLab(point2):point2;
	Vector3D	rel	= pt1 - pt2;
	double		r	= rel.norm();
	rel.normalize();

	Vector3D	v1	= body1? body1->ComputeVelocityOfPointInBody(pt1):ZERO_3D;
	Vector3D	v2	= body2? body2->ComputeVelocityOfPointInBody(pt2):ZERO_3D;
	Vector3D	vel	= v1 - v2;

	Vector3D	v	= (rel.vectorProd((vel.vectorProd(rel)   )      ))/r;
//	double r2	= rel*rel;
//	double r	= sqrt(r2);
//	v /= r;
//	double rv	= rel*v;
//	rel *= rv/r2;
//	v -= rel;

	long j;
	if (body1) {
		BodyCompareEqual bodyCompareEqual(body1);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfHessianDotVelocity.push_back(v[j]);
				vecRowOfHessianDotVelocity[n+j] = v[j];
			}
		}
	}

	if (body2) {
		BodyCompareEqual bodyCompareEqual(body2);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			for (j=0;j<3;++j) {
			//	vecRowOfHessianDotVelocity.push_back(-v[j]);
				vecRowOfHessianDotVelocity[n+j] = -v[j];
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////*/
void CPointToPointConstraint::AddConstraintForce( vector<double>& f , bool bDuringCollision ) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
//	Vector3D rel = pt2 - pt1;

	if (body1 && body1->m_bConstraintForceShouldBeUpdated) {
		BodyCompareEqual bodyCompareEqual(body1);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			Vector3D F(f[n+0],f[n+1],f[n+2]);

			if (bDuringCollision) body1->m_vecImpulseLab += F;
			else body1->m_vecForce += F;
	//		Vector3D rel = pt1 - body1->m_positionCM;
	//		body1->m_vecTorque += rel^F;
			body1->m_bConstraintForceShouldBeUpdated = false;
		}
	}

	if (body2 && body2->m_bConstraintForceShouldBeUpdated) {
		BodyCompareEqual bodyCompareEqual(body2);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			Vector3D F(f[n+0],f[n+1],f[n+2]);

			if (bDuringCollision) body2->m_vecImpulseLab += F;
			else body2->m_vecForce += F;

	//		Vector3D rel = pt2 - body2->m_positionCM;
	//		body2->m_vecTorque += rel^F;
			body2->m_bConstraintForceShouldBeUpdated = false;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CPointToPointConstraint::AddConstraintImpulse(	vector<double>& f) {
	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;

	if (body1 && body1->m_bConstraintImpulseShouldBeUpdated) {
		BodyCompareEqual bodyCompareEqual(body1);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			Vector3D F(f[n+0],f[n+1],f[n+2]);
			body1->m_vecImpulseLab += F;
	//		body1->m_vecTorque += ;
			body1->m_bConstraintImpulseShouldBeUpdated = false;
		}
	}

	if (body2 && body2->m_bConstraintImpulseShouldBeUpdated) {
		BodyCompareEqual bodyCompareEqual(body2);
		vector<CRigidBody*>::iterator i = find_if(
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(),
			theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end(),
			bodyCompareEqual);
		if (i != theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.end())
		{
			long n = 3*distance(theApp.GetConstraintManager().m_vecBodiesCurrentTimeStep.begin(), i);
			Vector3D F(f[n+0],f[n+1],f[n+2]);
			body2->m_vecImpulseLab += F;
	//		body2->m_vecTorque += ;
			body2->m_bConstraintImpulseShouldBeUpdated = false;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
double PointToPointConstraint(void* user_data) {
	PointToPointUser_Data* ud = (PointToPointUser_Data*) user_data;
	CRigidBody* body1 = ud->body1;
	CRigidBody* body2 = ud->body2;
	Point3D pt1 = body1? body1->LocalToLab(ud->point1):ud->point1;
	Point3D pt2 = body2? body2->LocalToLab(ud->point2):ud->point2;
	Vector3D rel = pt1 - pt2;
	return  (rel.norm2() - ud->separation2)/2;
}
///////////////////////////////////////////////////////////////////////////////
static Vector3D DPointToPointConstraint_dx1(void* user_data) {
	PointToPointUser_Data* ud = (PointToPointUser_Data*) user_data;
	CRigidBody* body1 = ud->body1;
	if (!body1) throw -1;
	CRigidBody* body2 = ud->body2;
	Point3D pt1 = body1? body1->LocalToLab(ud->point1):ud->point1;
	Point3D pt2 = body2? body2->LocalToLab(ud->point2):ud->point2;
	Vector3D rel = pt1 - pt2;
	return rel;
}
///////////////////////////////////////////////////////////////////////////////
static Vector3D DPointToPointConstraint_dx2(void* user_data) {
	PointToPointUser_Data* ud = (PointToPointUser_Data*) user_data;
	CRigidBody* body1 = ud->body1;
	CRigidBody* body2 = ud->body2;
	if (!body2) throw -1;
	Point3D pt1 = body1? body1->LocalToLab(ud->point1):ud->point1;
	Point3D pt2 = body2? body2->LocalToLab(ud->point2):ud->point2;
	Vector3D rel = pt2 - pt1;
	return rel;
}
///////////////////////////////////////////////////////////////////////////////

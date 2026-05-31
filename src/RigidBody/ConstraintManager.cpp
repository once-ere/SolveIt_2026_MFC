// ConstraintManager.cpp: implementation of the CConstraintManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "ConstraintManager.h"
#include "RigidBodyManager.h"
//#include "PtToPtConstraint.h"
#include "GLView.h"
#include "System.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CConstraintManager::CConstraintManager() //:	m_NumberOfIndependentVariables(0)
{
//	push_back(&m_listPointMassConstraints);
//	push_back(&m_listNonPointMassConstraints);
//	push_back(&m_listPulleyConstraints);
//	push_back(&m_listSurfaceConstraints);
//	push_back(&m_listZeroVelocityAtFixedPoint);
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::Clear()
{
	{
		vector<CPointToPointConstraint*>::iterator it;
		for (it = m_vecTwoPointConstraints.begin(); it != m_vecTwoPointConstraints.end(); ++it)
		{
			delete(*it);
		}
		m_vecTwoPointConstraints.clear();
	}

	{
		vector<CLinearConstraint*>::iterator it;
		for (it = m_vecLinearConstraints.begin(); it != m_vecLinearConstraints.end(); ++it)
		{
			delete(*it);
		}
		m_vecLinearConstraints.clear();
	}

	{
		vector<CQuadraticConstraint*>::iterator it;
		for (it = m_vecQuadraticConstraints.begin(); it != m_vecQuadraticConstraints.end(); ++it)
		{
			delete(*it);
		}
		m_vecQuadraticConstraints.clear();
	}
}
///////////////////////////////////////////////////////////////////////////////
CConstraintManager::~CConstraintManager()
{
	Clear();
}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::OnDraw()
{
	{
		vector<CPointToPointConstraint*>::iterator it;
		for (it = m_vecTwoPointConstraints.begin(); it != m_vecTwoPointConstraints.end(); ++it)
		{
			CPointToPointConstraint* c = (*it);
			if (c) c->DrawState();
		}
	}

	{
		vector<CLinearConstraint*>::iterator it;
		for (it = m_vecLinearConstraints.begin(); it != m_vecLinearConstraints.end(); ++it)
		{
			CLinearConstraint* c = (*it);
			if (c) c->DrawState();
		}
	}

	{
		vector<CQuadraticConstraint*>::iterator it;
		for (it = m_vecQuadraticConstraints.begin(); it != m_vecQuadraticConstraints.end(); ++it)
		{
			CQuadraticConstraint* c = (*it);
			if (c) c->DrawState();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
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

	glPushMatrix();

	Point3D pt1 = body1? body1->LocalToLab(point1):point1;
	Point3D pt2 = body2? body2->LocalToLab(point2):point2;
	glLineWidth(3.0f);
glBegin( GL_LINES);
	glColor3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f(GLfloat(pt1.x), GLfloat(pt1.y), GLfloat(pt1.z));
	glVertex3f(GLfloat(pt2.x), GLfloat(pt2.y), GLfloat(pt2.z));
glEnd();

	glPopMatrix();

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glLineWidth(1.0f);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CLinearConstraint::DrawState()
{
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

	const Point3D& x = point;
	Vector3D a = x-body->x;
	a = normal.vectorProd(a.vectorProd(normal));
	Vector3D b = normal.vectorProd(a);
	Vector3D d = a-b;
	Vector3D s = a+b;
	Vector3D D = -d;
	Vector3D S = -s;

	glPushMatrix();

	glTranslated(x.x, x.y, x.z);
	const double th=0.75;
	glColor3d( th, th, th );


	glBegin(GL_POLYGON);
	glVertex3d( d.x, d.y, d.z );
	glVertex3d( s.x, s.y, s.z );
	glVertex3d( D.x, D.y, D.z );
	glVertex3d( S.x, S.y, S.z );
	glEnd();

	glPopMatrix();

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQuadraticConstraint::DrawState()
{
	if (body->m_bShowQuadraticConstraint == false) return;

	try {
	if (m_bDrawn) return;
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	const double& x1= pGLView->glFrustum_left;
	const double& x2= pGLView->glFrustum_right;
	const double& y1= pGLView->glFrustum_bottom;
	const double& y2= pGLView->glFrustum_top;
	const int N			= 64;
	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);
	double dz = 0.5*(w+h);
	const double z1= -dz;
	const double z2= dz;
	dz/=N/2;
	glPushMatrix();
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0); 
	}
//	glLineWidth(2.0f);
//	glColor3dv (body->m_vecColor.v);
	static double scale = 1;
	if (theApp.GetSystem()->m_bStopIterations) scale += 1.0/PI;
	static double red = fabs(cos(scale*body->m_vecColor.x));
	static double grn = fabs(cos(scale*body->m_vecColor.y));
	static double blu = fabs(cos(scale*body->m_vecColor.z));
	if (theApp.GetSystem()->m_bStopIterations)
	{
		red = fabs(cos(scale*body->m_vecColor.x));
		grn = fabs(cos(scale*body->m_vecColor.y));
		blu = fabs(cos(scale*body->m_vecColor.z));

//		double mx=red;
//		if (grn>mx) mx=grn;
//		if (blu>mx) mx=blu;
//		double add=1-mx;
//		red += add;
//		grn += add;
//		blu += add;
		const double ex= 0.2;
		const double th= 0.3;
		const double am= 0.2;
		if (red<th) red += am;
		if (grn<th) grn += am;
		if (blu<th) blu += am;

		red = pow(red, ex);
		grn = pow(grn, ex);
		blu = pow(blu, ex);
	}
	glColor3d(red, grn, blu);

	double x=0, y=0, z=0;


	int type = GL_LINE_STRIP;
//	int type = GL_LINE_LOOP;

	vector<double> re_a;
	vector<double> re_b1;
	vector<double> re_b2;
	double _y1;
	double _y2;
	double _x1;
	double _x2;
	for (z=z1;z<=z2;z+=dz)
	{

	re_a.clear();
	re_b1.clear();
	re_b2.clear();
	for (x=x1;x<=x2;x+=dx)
	{
		if (eval_y(x, z, _y1, _y2))
		{
			re_a.push_back(x);
			Vector3D v1(x, _y1, z);
			Vector3D v2(x, _y2, z);
			re_b2.push_back( body->x*v2>body->x*v1? _y2:_y1);
			re_b1.push_back( body->x*v2>body->x*v1? _y1:_y2);
		}
	}

	glBegin(type);vector<double>::iterator i;
	for ( i= re_a.begin(); i!=re_a.end();++i)
	{
		int j = static_cast<int>(distance(re_a.begin(), i));
		glVertex3d(*i, re_b1[j], z);
	}
	glEnd();

	glBegin(type);
	for ( i= re_a.begin(); i!=re_a.end();++i)
	{
		int j = static_cast<int>(distance(re_a.begin(), i));
		glVertex3d(*i, re_b2[j], z);
	}
	glEnd();

	re_a.clear();
	re_b1.clear();
	re_b2.clear();
	for (y=y1;y<=y2;y+=dy)
	{
		if (eval_x(y, z, _x1, _x2))
		{
			re_a.push_back(y);
			Vector3D v1(_x1, y, z);
			Vector3D v2(_x2, y, z);
			re_b2.push_back( body->x*v2>body->x*v1? _x2:_x1);
			re_b1.push_back( body->x*v2>body->x*v1? _x1:_x2);
		}
	}
	glBegin(type);
	for ( i= re_a.begin(); i!=re_a.end();++i)
	{
		int j = static_cast<int>(distance(re_a.begin(), i));
		glVertex3d(re_b1[j], *i, z);
	}
	glEnd();
	glBegin(type);
	for ( i= re_a.begin(); i!=re_a.end();++i)
	{
		int j = static_cast<int>(distance(re_a.begin(), i));
		glVertex3d(re_b2[j], *i, z);
	}
	glEnd();
	}

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}






	return;













	try {
	if (m_bDrawn) return;
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
	const int N			= 64;
	double data[N][N];
	double data2[N][N];
	GLfloat edgePt[N][2];
	const double& x1= pGLView->glFrustum_left;
	const double& x2= pGLView->glFrustum_right;
	const double& y1= pGLView->glFrustum_bottom;
	const double& y2= pGLView->glFrustum_top;
	const double& a1= normal1.x;
	const double& a2= normal2.x;
	const double& b1= normal1.y;
	const double& b2= normal2.y;
	const double& c1= normal1.z;
	const double& c2= normal2.z;
	if (a2*b2*c2 == 0.0) return;
	const double x0	= -a1/(2*a2);
	const double y0	= -b1/(2*b2);
	const double z0	= -c1/(2*c2);
	const double R2	= ((0.25*(a1*a1/a2+b1*b1/b2+c1*c1/c2) - scalar)/(a2*b2*c2));
	if (R2<0.0) return;
	const double R	= sqrt(R2);
	const double xR	= R*sqrt(b2*c2);
	const double yR	= R*sqrt(a2*c2);
	const double zR	= R*sqrt(a2*b2);
	int x, y;
	vector<int> re_x;
	vector<int> re_y;
	double  Z=0;
	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);
//	double dz = 0.5*(w+h);
//			Z=imsl_rand()*dz;
//			Point3D point(X, Y, Z);
	double z1=0;
	double z2=0;
	double X[64];
	double Y[64];
//	double _y1;
//	double _y2;
	for (x=0;x<N;x++)
	{
		edgePt[x][0]=edgePt[x][1]=0;
	}
	for (x=0;x<N;x++)
	{
			double  p = x*2.0*PI/(N-1);//atan2(Y,X);
			double sp = sin(p);
			double cp = cos(p);
			edgePt[x][0]=(GLfloat)( R*cp);
			edgePt[x][1]=(GLfloat)( R*sp);
			X[x] = x1 + x*dx;
		//	if (eval_y(X[x], 0, _y1, _y2))
		//	{
		//		edgePt[N-x][0]=edgePt[x][0]=(float)X[x];
		//		edgePt[x][1]	= (float)_y1;
		//		edgePt[N-x][1]	= (float)_y2;
		//	}
		for (y=0;y<N;y++)
		{
			Y[y] = y1 + y*dy;
	//		double st = sin(y*PI/N);//atan2(sqrt(X*X+Y*Y),Z));
	//		double ct = cos();
			data[x][y] = 0;
			data2[x][y] = 0;
//			X[x] = x0 + xR*st*cp;
//			Y[y] = y0 + yR*st*sp;
			bool ret = eval_z(X[x], Y[y], z1, z2);
			if(ret)
			{
				Vector3D v1(X[x], Y[y], z1);
				Vector3D v2(X[x], Y[y], z2);
				data[x][y] = body->x*v2>body->x*v1? z2:z1;
				data2[x][y] = body->x*v2>body->x*v1? z1:z2;
			}
			else
			{
				re_x.push_back(x);
				re_y.push_back(y);
			}
		}
	}
	if (re_x.size())
	{
		vector<int>::iterator i_x;
		vector<int>::iterator i_y;
		for (i_x = re_x.begin();i_x != re_x.end();++i_x)
		{
			for (i_y = re_y.begin();i_y != re_y.end();++i_y)
			{
			}
		}
	}
	pGLView->NurbsRender64( X, Y, data, data2, edgePt );
	m_bDrawn = true;
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
bool CQuadraticConstraint::eval_z(const double& x, const double& y, double& z1, double& z2)
{
	const double& a1= normal1.x;
	const double& a2= normal2.x;
	const double& b1= normal1.y;
	const double& b2= normal2.y;
	const double& c1= normal1.z;
	const double& c2= normal2.z;
	const Vector3D normal2;
	const Vector3D normal1;
	const double d=((c1*c1) - 4*c2*(scalar + a1*x + a2*(x*x) + b1*y + b2*(y*y)));
	if (d<0.0)
	{
	//	z1=z2=0;
		return false;
	}
	const double D= sqrt(d);
	z1=(-c1 + D)/(2.*c2);
	z2=(-c1 - D)/(2.*c2);
	return true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CQuadraticConstraint::eval_y(const double& x, const double& z, double& y1, double& y2)
{
	const double& a1= normal1.x;
	const double& a2= normal2.x;
	const double& b1= normal1.y;
	const double& b2= normal2.y;
	const double& c1= normal1.z;
	const double& c2= normal2.z;
	const Vector3D normal2;
	const Vector3D normal1;
	const double d=((b1*b1) - 4*b2*(scalar + a1*x + a2*(x*x) + c1*z + c2*(z*z)));
	if (d<0.0)
	{
	//	z1=z2=0;
		return false;
	}
	const double D= sqrt(d);
	y1=(-b1 + D)/(2.*b2);
	y2=(-b1 - D)/(2.*b2);
	return true;
}
///////////////////////////////////////////////////////////////////////////////
	bool eval_x(const double& y, const double& z, double& x1, double& x2);
///////////////////////////////////////////////////////////////////////////////
bool CQuadraticConstraint::eval_x(const double& y, const double& z, double& x1, double& x2)
{
	const double& a1= normal1.x;
	const double& a2= normal2.x;
	const double& b1= normal1.y;
	const double& b2= normal2.y;
	const double& c1= normal1.z;
	const double& c2= normal2.z;
	const Vector3D normal2;
	const Vector3D normal1;
	const double d=((a1*a1) - 4*a2*(scalar + c1*z + c2*(z*z) + b1*y + b2*(y*y)));
	if (d<0.0)
	{
	//	z1=z2=0;
		return false;
	}
	const double D= sqrt(d);
	x1=(-a1 + D)/(2.*a2);
	x2=(-a1 - D)/(2.*a2);
	return true;
}
///////////////////////////////////////////////////////////////////////////////

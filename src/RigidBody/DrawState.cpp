// RigidBody.cpp: implementation of the CRigidBody class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RigidBody.h"
#include "SMutex.h"
#include "shapes.h"
#include "parser.h" /* for token definitions and yylval */

SMutex g_SMutex_1(_T("1"));

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
void printStrokedString(const wchar_t *s)
{
	GLsizei len = static_cast<GLsizei>(_tcslen(s) + 1); //strlen(s);
	glCallLists(len, GL_BYTE, (GLbyte *)s);
}
///////////////////////////////////////////////////////////////////////////////
void DrawArrow(double X, double Y)	{
//	const Vector3D& sc = theSystem->m_fScalePosition;
	const double scale	= 0.15;
	double c	= scale*cos(PI/6.0);
	double s	= scale*sin(PI/6.0);
	double dx	= X;// / sc.x;
	double dy	= Y;// / sc.y;
	double R	= sqrt(dx*dx+dy*dy);
	if ( R > 0.0 ) {
		dx /= R;
		dy /= R;
	}
	
	glBegin(GL_POLYGON);
	glVertex2d( X, Y );
	glVertex2d( X - ( c*dx-s*dy ), Y - ( s*dx+c*dy ) );
	glVertex2d( X - ( c*dx+s*dy ),  Y - (-s*dx+c*dy ));
	glEnd();
}
///////////////////////////////////////////////////////////////////////////////
void DrawCone(double coneRadius, double x, double y, double z, bool bTranslate)
{
	const double smallNum = 0.1e-6;
//	const double coneRadius = 0.025;
	/*const*/ double coneHeight = 2*coneRadius;
//	auxSolidCone(coneRadius, coneHeight);
//	auxWireCone(coneRadius, coneHeight);-coneHeight
	glPushMatrix();
	if (bTranslate) glTranslated(x, y, z);
	double R2 = x*x+y*y;
	double r2 = R2 + z*z;
	double R = sqrt(R2);
	double r = sqrt(r2);
	bool bDraw = true;
	if (r < smallNum)
	{
		bDraw = false;
	}
	else if (R < smallNum)
	{
		glTranslated(-x*coneHeight/r, -y*coneHeight/r, -z*coneHeight/r);
		if (z < 0.0)
			glRotated(180, 1, 0, 0);
	}
	else if (fabs(z) < smallNum)
	{
		glTranslated(-x*coneHeight/R, -y*coneHeight/R, 0);
		glRotated(90, -y/R, x/R, 0);
	}
	else
	{
		Vector3D p(x,y,z);
		Vector3D n = K_3D.vectorProd(p);
		n.normalize();
		double t = RAD_TO_DEG*acos(z/r);
		glTranslated(-x*coneHeight/r, -y*coneHeight/r, -z*coneHeight/r);
		glRotated(t, n.x, n.y, n.z);
	}
	if (bDraw)
	{
		GLint slices = 64;//	
		GLint stacks = 64;//	
//		glutSolidCone(coneRadius, coneHeight,  slices,  stacks);
		shapes::auxSolidCone(coneRadius, coneHeight);
	}
	glPopMatrix();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::OnDrawState()
{
try{
	const double coneRadius = 0.05;
	if (m_bHide) return;
	double	&Red	= m_vecColor.x,
			&Green	= m_vecColor.y,
			&Blue	= m_vecColor.z;
if (g_SMutex_1.wait(500))
{
#if 0
	glPushMatrix();
	glTranslated(x.x, x.y, x.z);
	glScalef(m_fRadius, m_fRadius, m_fRadius);
// **** "Undo" any perspective rotation ****
GLfloat m[][4]	glMultMatrixf(&m[0][0]);
//glColor4ubv(atom_ref.rgb_color);
	glColor3d( Red, Green, Blue );
	glCallList(ATOM_SPHERE);
	glPopMatrix();
	return;
#endif
#if 10
	glPushMatrix();
	glShadeModel(GL_FLAT);

// Need to turn on GL_DITHER for 3D objects and turn it off for lines and points.
	glDisable(GL_DITHER);

//draw m_velocityCM and m_acceleration vectors
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}
///////////////////////////////////////////////////////////////////////////////
/*
	if( hDC && m_bUserDefinedPotentialIsValid && m_hBitmapPotential && m_bShowUserDefinedPotential)
	{
		HDC			hMemDC;
		BITMAP		bm;
		GetObject(m_hBitmapPotential, sizeof(BITMAP), &bm );
		hMemDC = CreateCompatibleDC( hDC );
		m_hOldBitmapPotential = (HBITMAP)SelectObject( hMemDC, m_hBitmapPotential );
		BitBlt( hMemDC, 0, 0, bm.bmWidth, bm.bmHeight,
					hDC, 0, 0,  SRCAND);
		BitBlt( hDC, 0, 0, bm.bmWidth, bm.bmHeight,
					hMemDC, 0, 0,  SRCAND);

		SelectObject( hMemDC, m_hOldBitmapPotential );
	}
*/
///////////////////////////////////////////////////////////////////////////////
#if 10
	if (m_inboundJoint.jointType == Joint1DOF::SPRING)
	{
		ListOf_springConstants_springLengths::iterator	ikL		= m_ListOf_springConstants_springLengths.begin();
		PairPoint3DVector::iterator						iptpt	= m_PairPoint3DVector.begin();
		vector<CRigidBody *>::iterator					irb		= m_bodiesConnectedTo.begin();

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
				PairPoint3D&	pts		= *iptpt;
				Point3D&		point1	= pts.first;
				Point3D&		point2	= pts.second;
				Point3D			pt1		= LocalToLab(point1);
				Point3D			pt2		= body2? body2->LocalToLab(point2):point2;
			glBegin( GL_LINES);
			glColor3f ( 0.4f, 0.0f, 0.4f );
			glVertex3dv(pt1.v);
			glVertex3dv(pt2.v);
			glEnd();
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bConstraintedToPlane)
	{
		Point3D& p = m_ptPointInPlane;
		const double scale = 2;
		double c = scale*fabs(cos(m_fAngleOfIncline));
		double s = scale*sin(m_fAngleOfIncline);
		if (m_fAngleOfIncline > PI/2) s *= -1;
		glBegin( GL_LINES);
		glColor3f ( 0.4f, 0.0f, 0.4f );
		glVertex3f( GLfloat(p.x), GLfloat(p.y), GLfloat(p.z) );
		glVertex3f(GLfloat(p.x+c), GLfloat(p.y+s), GLfloat(p.z));
		glVertex3f( GLfloat(p.x), GLfloat(p.y), GLfloat(p.z) );
		glVertex3f(GLfloat(p.x-c), GLfloat(p.y-s), GLfloat(p.z));
		glEnd();
	}

	if (m_fDistanceToFixedPointOnPulley != -1.0)
	{
		Point3D& p = m_PulleyContactPoint;
		glBegin( GL_LINES);
		glColor3f ( 0.4f, 0.0f, 0.4f );
		glVertex3f( GLfloat(p.x), GLfloat(p.y), GLfloat(p.z) );
		glVertex3f( GLfloat(x.x), GLfloat(x.y), GLfloat(x.z) );
		glEnd();
	}

	if (m_bPlotTrajectory||m_lShowPositionEveryNumSteps||m_lShowVelocityEveryNumSteps)
	{
		m_trajectoryRecord.PlotTrajectory(m_vecPlotTrajectoryColor);
	}

	if (m_lShowPositionEveryNumSteps>0 && x != ZERO_3D)
	{
		if (type == T_VECTOR_ANALYSIS_OBJECT)
			glLineWidth(2.50f);
		else
			glLineWidth(2.0f);
		glBegin( GL_LINES);

		if (type == T_VECTOR_ANALYSIS_OBJECT)
		{
			const double y=PI/800;
			static double a=PI/2;
			static double b=PI/2;
			a+=2*y;
			b-=3*y;
			double ca=cos(a);
			double sa=sin(a);
			double cb=cos(b);
			double sb=sin(b);

			const double th=0.45;
			double red		= fabs(sa*cb+Red);
			double green	= fabs(sa*sb+Green);
			double blue		= fabs(ca+Blue);
			if (red		> th) red = th;
			if (green	> th) green = th;
			if (blue	> th) blue = th;
			glColor3d( red, green, blue );
		}
		else
		{
			glColor3d( Red, Green, Blue );
		}

		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(x.x), GLfloat(x.y), GLfloat(x.z));
		glEnd();
		{
			DrawCone( coneRadius, x.x, x.y, x.z, true);
		}
	}
#endif

	glTranslated(x.x, x.y, x.z);
#if 10
	if (m_fMassInverse!=0.0)
	{
		Vector3D momentum		= m_fScaleMomentum*p;
		Vector3D force			= m_fScaleForce*m_stateTimeDerivativeOfState.p;
		Vector3D amomentum		= m_fScaleAngularMomentum*L;

		Vector3D velocity		= m_fMassInverse*momentum;
		Vector3D acceleration	= m_fMassInverse*force;


	if (m_lShowVelocityEveryNumSteps>0 && velocity != ZERO_3D)
	{
		glLineWidth(2.0f);
		glBegin( GL_LINES);
		glColor3f ( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(velocity.x), GLfloat(velocity.y), GLfloat(velocity.z));
		glEnd();
		DrawCone( coneRadius, velocity.x, velocity.y, velocity.z, true);
	}
	if (m_bShowMomentum && momentum != ZERO_3D)
	{
		glLineWidth(3.0f);
		glBegin( GL_LINES);
		glColor3f ( 0.7f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(momentum.x), GLfloat(momentum.y), GLfloat(momentum.z));
		glEnd();
		DrawCone( coneRadius, momentum.x, momentum.y, momentum.z, true);
	}
	if (m_bShowAngularMomentum && amomentum != ZERO_3D)
	{
		glLineWidth(3.0f);
		glBegin( GL_LINES);
		glColor3f ( 0.0f, 0.0f, 0.7f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(amomentum.x), GLfloat(amomentum.y), GLfloat(amomentum.z));
		glEnd();
		DrawCone( coneRadius, amomentum.x, amomentum.y, amomentum.z, true);
	}
	if (m_bShowAcceleration && acceleration != ZERO_3D)
	{
		glLineWidth(2.0f);
		glBegin( GL_LINES);
		glColor3f ( 0.0f, 1.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(acceleration.x), GLfloat(acceleration.y), GLfloat(acceleration.z));
		glEnd();
		DrawCone( coneRadius, acceleration.x, acceleration.y, acceleration.z, true);
	}
	if (m_bShowForce && force != ZERO_3D)
	{
		glLineWidth(3.0f);
		glBegin( GL_LINES);
		glColor3f ( 0.0f, 0.7f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f(GLfloat(force.x), GLfloat(force.y), GLfloat(force.z));
		glEnd();
		DrawCone( coneRadius, force.x, force.y, force.z, true);
	}

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glLineWidth(1.0f);
	}

///////////////////////////////////////////////////////////////////////////////
	if (m_bShowContactForce && m_vecContactForce != ZERO_3D)
	{
	glLineWidth(3.0f);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
		glDisable(GL_LIGHT1);     
	}

	Vector3D V = m_fScaleForce*m_vecContactForce;
glBegin( GL_LINES);
	glColor3f ( 0.6f, 0.0f, 0.4f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3dv(V.v);
glEnd();
		DrawCone( coneRadius, V.x, V.y, V.z, true);

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	glLineWidth(1.0f);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (m_bShowContactTorque && m_vecContactTorque != ZERO_3D)
	{
	glLineWidth(3.0f);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

	Vector3D V = m_fScaleForce*m_vecContactTorque;
glBegin( GL_LINES);
	glColor3f ( 0.4f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f(GLfloat(V.x), GLfloat(V.y), GLfloat(V.z));
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
	if (m_bShowNormals)
	{
	glLineWidth(3.0f);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}

glBegin( GL_LINES);
	glColor3f ( 1.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
		Vector3D x = LocalToLab(I_3D);
	glVertex3f(GLfloat(x.x), GLfloat(x.y), GLfloat(x.z));
	glVertex3f( 0.0f, 0.0f, 0.0f );
		Vector3D y = LocalToLab(J_3D);
	glVertex3f(GLfloat(y.x), GLfloat(y.y), GLfloat(y.z));
	glVertex3f( 0.0f, 0.0f, 0.0f );
		Vector3D z = LocalToLab(K_3D);
	glVertex3f(GLfloat(z.x), GLfloat(z.y), GLfloat(z.z));
glEnd();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glLineWidth(1.0f);
	}
///////////////////////////////////////////////////////////////////////////////
#endif

	glColor3d( Red, Green, Blue );

	const Vector3D axis = q.axis();//lab
	const double a = RAD_TO_DEG*q.angle();

	glRotated(a, axis.x, axis.y, axis.z);
// Need to turn on GL_DITHER for 3D objects and turn it off for lines and points.
//	glEnable(GL_DITHER);
	OnDrawShape();
	glDisable(GL_DITHER);

	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}
	glRasterPos2d(m_fRadius*1.19, -m_fRadius*1.19);

	glListBase(2000);
	glCallLists(static_cast<GLsizei>(name.size()), GL_BYTE, (GLbyte *)name.c_str());

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
#endif
	g_SMutex_1.Release();
}
	SetScreenRect();
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}
}

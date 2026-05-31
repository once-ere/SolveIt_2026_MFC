// DrawShape.cpp: implementation of the CRigidBody class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "System.h"
#include "shapes.h"
//#include <GL/glut.h>
#include "parser.h" /* for token definitions and yylval */
#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
	switch(type)
	{
		case T_SPHERE:
		break;
		case T_BLOCK:
		break;
		case T_CYLINDER:
		break;
	}

void APIENTRY auxWireSphere(GLdouble);
void APIENTRY auxSolidSphere(GLdouble);
void APIENTRY auxWireCube(GLdouble);
void APIENTRY auxSolidCube(GLdouble);
void APIENTRY auxWireBox(GLdouble, GLdouble, GLdouble);
void APIENTRY auxSolidBox(GLdouble, GLdouble, GLdouble);
void APIENTRY auxWireTorus(GLdouble, GLdouble);
void APIENTRY auxSolidTorus(GLdouble, GLdouble);
void APIENTRY auxWireCylinder(GLdouble, GLdouble);
void APIENTRY auxSolidCylinder(GLdouble, GLdouble);
void APIENTRY auxWireIcosahedron(GLdouble);
void APIENTRY auxSolidIcosahedron(GLdouble);
void APIENTRY auxWireOctahedron(GLdouble);
void APIENTRY auxSolidOctahedron(GLdouble);
void APIENTRY auxWireTetrahedron(GLdouble);
void APIENTRY auxSolidTetrahedron(GLdouble);
void APIENTRY auxWireDodecahedron(GLdouble);
void APIENTRY auxSolidDodecahedron(GLdouble);
void APIENTRY auxWireCone(GLdouble, GLdouble);
void APIENTRY auxSolidCone(GLdouble, GLdouble);
void APIENTRY auxWireTeapot(GLdouble);
void APIENTRY auxSolidTeapot(GLdouble);
*/

extern GLuint texNames[2];
///////////////////////////////////////////////////////////////////////////////
// DrawStyle
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::get_DrawStyle(long& value)
{
	switch (m_lDrawStyle)
	{
		case GLU_FILL:value=0;
		break;
		case GLU_SILHOUETTE:value=2;
		break;
		case GLU_POINT:value=3;
		break;
		case GLU_LINE:
		default:
		value=1;
		break;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CRigidBody::set_DrawStyle(const long& newValue)
{
	switch (newValue)
	{
		case 0:m_lDrawStyle=GLU_FILL;
		break;
		case 2:m_lDrawStyle=GLU_SILHOUETTE;
		break;
		case 3:m_lDrawStyle=GLU_POINT;
		break;
		case 1:
		default:
		m_lDrawStyle=GLU_LINE;
		break;
	}
	if (m_pQuadObj) gluQuadricDrawStyle (m_pQuadObj, (GLenum) m_lDrawStyle);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::OnDrawShape()
{
	if (m_bHide) return;
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
//TimedMessageBox(NULL,name.c_str(), "CRigidBody::OnDrawShape", MB_OK);
	GLint slices = 64;
	GLint stacks = 64;
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, texNames[1] );//space_tex );13//
//	CSystem* m_system = theApp.GetSystem();
	switch(type)
	{
		case T_SPHERE:
		{
			switch(m_lDrawStyle)
			{
				case GLU_FILL:slices = stacks = 64;
				break;
				case GLU_LINE:slices = stacks = 16;
				break;
				case GLU_SILHOUETTE:slices = stacks = 12;
				break;
				case GLU_POINT:slices = stacks = 48;
				break;
			}
			gluSphere (m_pQuadObj, m_fRadius, slices, stacks);
/*
			switch(m_lDrawStyle)
			{
				case GLU_FILL://slices = stacks = 64;
		//			shapes::auxSolidSphere(m_fRadius);
					gluSphere (m_pQuadObj, m_fRadius, slices, stacks);
		//			glutSolidSphere(m_fRadius, slices, stacks);
				return;
				case GLU_LINE://slices = stacks = 16;
		//			shapes::auxWireSphere(m_fRadius);
					gluSphere (m_pQuadObj, m_fRadius, slices, stacks);
		//			glutWireSphere(m_fRadius, slices, stacks);
				return;
				case GLU_SILHOUETTE://slices = stacks = 12;
		//			shapes::auxWireSphere(m_fRadius);
					gluSphere (m_pQuadObj, m_fRadius, slices, stacks);
				return;
				case GLU_POINT://slices = stacks = 48;
		//			shapes::auxWireSphere(m_fRadius);
					gluSphere (m_pQuadObj, m_fRadius, slices, stacks);
				return;
			}
*/
		}
		return;
		case T_BLOCK:
		{
			GLenum type = GL_LINE_LOOP;
			switch(m_lDrawStyle)
			{
				case GLU_FILL:type = GL_QUADS;
					shapes::auxSolidBox(m_fWidth, m_fHeight, m_fDepth);
				return;
				case GLU_LINE:type = GL_LINE_LOOP;
				case GLU_SILHOUETTE:type = GL_LINE_LOOP;
					shapes::auxWireBox(m_fWidth, m_fHeight, m_fDepth);
				return;
				case GLU_POINT:type = GL_LINE_LOOP;
					shapes::auxWireBox(m_fWidth, m_fHeight, m_fDepth);
				return;
			}
//				shapes::auxSolidBox(m_fWidth, m_fHeight, m_fDepth);
//				drawbox(-m_fWidth/2, m_fWidth/2, -m_fHeight/2, m_fHeight/2,-m_fDepth/2, m_fDepth/2, type);
		}
		return;
		case T_CYLINDER:
		{
			switch(m_lDrawStyle)
			{
				case GLU_FILL:slices = stacks = 64;
				break;
				case GLU_LINE:slices = stacks = 16;
				break;
				case GLU_SILHOUETTE:slices = stacks = 12;
				break;
				case GLU_POINT:slices = stacks = 48;
				break;
			}
			glTranslated(0, 0, -m_fDepth/2);
			gluDisk(m_pQuadObj, 0, m_fRadius, slices, stacks);
			gluCylinder (m_pQuadObj, m_fRadius, m_fRadius, m_fDepth, slices, stacks);
			glTranslated(0, 0,  m_fDepth/2);
			glTranslated(0, 0,  m_fDepth/2);
			gluDisk(m_pQuadObj, 0, m_fRadius, slices, stacks);
			glTranslated(0, 0,  -m_fDepth/2);
/*
			switch(m_lDrawStyle)
			{
				case GLU_FILL://slices = stacks = 64;
					shapes::auxSolidCylinder(m_fRadius, m_fDepth);
				return;
				case GLU_LINE://slices = stacks = 16;
					shapes::auxWireCylinder(m_fRadius, m_fDepth);
				return;
				case GLU_SILHOUETTE://slices = stacks = 12;
					shapes::auxWireCylinder(m_fRadius, m_fDepth);
				return;
				case GLU_POINT://slices = stacks = 48;
					shapes::auxWireCylinder(m_fRadius, m_fDepth);
				return;
			}
			glTranslated(0, 0, -m_fDepth/2);
			gluDisk(m_pQuadObj, 0, m_fRadius, slices, stacks);
			gluCylinder (m_pQuadObj, m_fRadius, m_fRadius, m_fDepth, slices, stacks);
			glTranslated(0, 0,  m_fDepth/2);
			glTranslated(0, 0,  m_fDepth/2);
			gluDisk(m_pQuadObj, 0, m_fRadius, slices, stacks);
			glTranslated(0, 0,  -m_fDepth/2);
		return;
*/
		}

		case T_VECTOR_ANALYSIS_OBJECT:
		{
			typedef CRigidBody CVectorAnalysis;
			void DrawCone(double coneRadius, double x, double y, double z, bool bTranslate);

	if (m_bodiesDependentOn.empty()) return;

	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	glPushMatrix();
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}
	double	&Red	= m_vecColor.x,
			&Green	= m_vecColor.y,
			&Blue	= m_vecColor.z;




//	glColor3d( Red, Green, Blue );
//	glPushMatrix();
//	glTranslated(x.x, x.y, x.z);
//	shapes::auxSolidSphere(.05);
//	glPopMatrix();



	CVectorAnalysis* A = m_bodiesDependentOn[0];
	CVectorAnalysis* B = m_bodiesDependentOn[1];

//	CVectorAnalysis* A = m_bodiesDependentOn.front();
//	CVectorAnalysis* B = m_bodiesDependentOn.back();

	const Vector3D& a = A->x;
	const Vector3D& b = B->x;

	switch(m_role)
	{
	case _sum:
	{
		glTranslated(-x.x, -x.y, -x.z);//undo CRigidBody::DrawState() translation
		double	&RedA	= A->m_vecColor.x,
				&GreenA	= A->m_vecColor.y,
				&BlueA	= A->m_vecColor.z;
		double	&RedB	= B->m_vecColor.x,
				&GreenB	= B->m_vecColor.y,
				&BlueB	= B->m_vecColor.z;
		glLineWidth(1.0f);
		const double f = 0.85;
		glColor3d( f*RedA, f*GreenA, f*BlueA );
		glTranslated(b.x, b.y, b.z);
	//	shapes::auxSolidSphere(.05);
		glBegin( GL_LINES);
		glVertex3d( 0, 0, 0 );
		glVertex3d(a.x, a.y, a.z);
		glEnd();
		glTranslated(-b.x, -b.y, -b.z);

		glColor3d( f*RedB, f*GreenB, f*BlueB );
		glTranslated(a.x, a.y, a.z);
	//	shapes::auxSolidSphere(.05);
		glBegin( GL_LINES);
		glVertex3d( 0, 0, 0 );
		glVertex3d(b.x, b.y, b.z);
		glEnd();
		glTranslated(-a.x, -a.y, -a.z);

	}
		break;
	case _difference:
	{
		const double f = 0.75;
	//	glColor3d( f*Red, f*Green, f*Blue );
		glColor3d( 1.00, 0.71, 0 );
		glLineWidth(1.0f);

		glTranslated(-x.x, -x.y, -x.z);//undo CRigidBody::DrawState() translation
		glTranslated(b.x, b.y, b.z);
		glBegin( GL_LINES);
		glVertex3d( 0, 0, 0 );
		glVertex3d(x.x, x.y, x.z);
		glEnd();

//		DrawCone(0.025, x.x, x.y, x.z, true);
	}
		break;
	case _crossProduct:
	{
	Vector3D d = a-b;
	Vector3D s = a+b;
	Vector3D D = -d;
	Vector3D S = -s;
	glTranslated(-x.x, -x.y, -x.z);
//	glColor3d( 0.492, 0.49, 0.486 );

			const double th=0.75;
/*
			const double y=PI/1600;
			static double a=PI/2;
			static double b=PI/2;
			a+=3*y;
			b-=2*y;
			double ca=cos(a);
			double sa=sin(a);
			double cb=cos(b);
			double sb=sin(b);

			double red		= fabs(sa*cb);
			double green	= fabs(sa*sb);
			double blue		= fabs(ca);
			if (red		< th) red = th;
			if (green	< th) green = th;
			if (blue	< th) blue = th;
			glColor3d( red, green, blue );
*/
			glColor3d( th, th, th );


	glBegin(GL_POLYGON);
	glVertex3d( d.x, d.y, d.z );
	glVertex3d( s.x, s.y, s.z );
	glVertex3d( D.x, D.y, D.z );
	glVertex3d( S.x, S.y, S.z );
	glEnd();
	}
		break;
	default: break;
	}

	glColor3d( Red, Green, Blue );

		DrawCone(0.075, x.x, x.y, x.z, true);

	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
		}
		break;
#if 0
		case coneBody:
		{
			switch(m_lDrawStyle)
			{
				case GLU_FILL://slices = stacks = 64;
					shapes::auxSolidCone(m_fRadius, m_fDepth);
				return;
				case GLU_LINE://slices = stacks = 16;
					shapes::auxWireCone(m_fRadius, m_fDepth);
				return;
				case GLU_SILHOUETTE://slices = stacks = 12;
					shapes::auxWireCone(m_fRadius, m_fDepth);
				return;
				case GLU_POINT://slices = stacks = 48;
					shapes::auxWireCone(m_fRadius, m_fDepth);
				return;
			}
		}
		return;
#endif
	}

}
///////////////////////////////////////////////////////////////////////////////
BOOL CRigidBody::HitTest(const POINT& pt) const
{
	if (m_bDisableSelect) return FALSE;
	return PtInRect(&m_rcScreen, pt);

#if 0
	CGLView* pGLView = theApp.GetGLView();
	Vector3D p(pt.x, pt.y, 0);
	pGLView->ComputeWorldCoords(&p.x, &p.y, &p.z);
	double x_Pos = x.x;// + X;
	double y_Pos = x.y;// + Y;
	double z_Pos = x.z;// + Z;
//	pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
	const double fRadius = 0.1;
	double r = m_fRadius>fRadius? m_fRadius:fRadius;
	double xPos = r/1.73205080756887729353;
	double yPos = xPos;
	double zPos = xPos;
//	pGLView->ComputeScreenCoords(&xPos, &yPos, &zPos);
	int hrR = pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
	r = xPos*xPos + yPos*yPos + zPos*zPos;
	double R = (x_Pos-pt.x)*(x_Pos-pt.x) + (y_Pos-pt.y)*(y_Pos-pt.y);
	return (R*R <= r)? 1:0;
#endif
}
///////////////////////////////////////////////////////////////////////////////
#if 10
void CRigidBody::SetScreenRect()
{
	CGLView* pGLView = theApp.GetGLView();
#if 0
	Vector3D X, Y, Z;
	{
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		Vector3D O(x_Pos, y_Pos, z_Pos);
		x_Pos = 1;
		y_Pos = 0;
		z_Pos = 0;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		 X = Vector3D(x_Pos, y_Pos, z_Pos);
		 X	-= O;
		x_Pos = 0;
		y_Pos = 1;
		z_Pos = 0;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		 Y = Vector3D(x_Pos, y_Pos, z_Pos);
		 Y	-= O;
		x_Pos = 0;
		y_Pos = 0;
		z_Pos = 1;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		 Z = Vector3D(x_Pos, y_Pos, z_Pos);
		 Z	-= O;
	}
#endif

//	const double &X = theApp.theSystem.theScene.m_fTransX;
//	const double &Y = theApp.theSystem.theScene.m_fTransY;
//	const double &Z = theApp.theSystem.theScene.m_fTransZ;

	double x_Pos = x.x;// + X;
	double y_Pos = x.y;// + Y;
	double z_Pos = x.z;// + Z;

/*
	double xPos = x.x;// + X;
	double yPos = x.y;// + Y;
	double zPos = x.z;// + Z;

	xPos = *x; 
	yPos = *y;
	zPos = *z;

	x_Pos	=	pGLView->m[0][0]*xPos + pGLView->m[0][1]*yPos + pGLView->m[0][2]*zPos;
	y_Pos	=	pGLView->m[1][0]*xPos + pGLView->m[1][1]*yPos + pGLView->m[1][2]*zPos;
	z_Pos	=	pGLView->m[2][0]*xPos + pGLView->m[2][1]*yPos + pGLView->m[2][2]*zPos;

	x_Pos-=pGLView->m_fTransX;
	y_Pos-=pGLView->m_fTransY;
	z_Pos-=pGLView->m_fTransZ;

	Vector3D XYZ;
	{
		double x_Pos = pGLView->m_fTransX;
		double y_Pos = pGLView->m_fTransY;
		double z_Pos = pGLView->m_fTransZ;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		XYZ = Vector3D(x_Pos, y_Pos, z_Pos);
	}
	{
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;
		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
		Vector3D O(x_Pos, y_Pos, z_Pos);
		XYZ	-= O;
	}
*/

	switch(type)
	{
		case T_SPHERE:
		case T_CYLINDER:
		case T_VECTOR_ANALYSIS_OBJECT:
		{
				RECT rc;
				pGLView->GetClientRect(&rc);
		//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
/*
			if (m_fRadius <= 0.1)
			{
				const double w = 8;
				if (!hr)
				{
					m_rcScreen.left		= (long) (x_Pos + 0.5 - w);
					m_rcScreen.right	= (long) (x_Pos + 0.5 + w);
					m_rcScreen.top		= (long) (y_Pos + 0.5 - w);
					m_rcScreen.bottom	= (long) (y_Pos + 0.5 + w);
					m_f_zScreen			= z_Pos;
				}
				NormalizeRect(m_rcScreen);
				return;
			}
*/
			const double fRadius = 0.0000003*rc.bottom*rc.right;
			double w = m_fRadius>fRadius? m_fRadius:fRadius;
			double xPos = x_Pos+w/2;
			double yPos = y_Pos+w/2;
			double zPos = z_Pos+w/2;
			x_Pos -= w/2;
			y_Pos -= w/2;
			z_Pos -= w/2;
		//	int hrL = pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		//	int hrR = pGLView->ComputeScreenCoords(&xPos, &yPos, &zPos);
			int hrL = pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			int hrR = pGLView->ComputeScreenCoords(xPos, yPos, zPos, false);
			if (!(hrL||hrR))
			{
		//		w = xPos - x_Pos;
		//		if (w < 8.0) w = 8;
		//		m_rcScreen.left		= (long) (x_Pos - w + 0.5);
		//		m_rcScreen.right	= (long) (x_Pos + w + 0.5);
		//		m_rcScreen.top		= (long) (y_Pos - w + 0.5);
		//		m_rcScreen.bottom	= (long) (y_Pos + w + 0.5);
		//		m_f_zScreen			= z_Pos;
				m_rcScreen.left		= (long) (x_Pos);//	+ XYZ.x);//
				m_rcScreen.right	= (long) (xPos);//	+ XYZ.x);//

				m_rcScreen.top		= (long) (y_Pos);//	+ XYZ.y);//
				m_rcScreen.bottom	= (long) (yPos);//	+ XYZ.y);//
			//	m_rcScreen.top		= (long) (rc.bottom-y_Pos);
			//	m_rcScreen.bottom	= (long) (rc.bottom-yPos);



	const double w = 8;
		m_rcScreen.left		= (long) (x_Pos + 0.5 - w);
		m_rcScreen.right	= (long) (x_Pos + 0.5 + w);
		m_rcScreen.top		= (long) (y_Pos + 0.5 - w);
		m_rcScreen.bottom	= (long) (y_Pos + 0.5 + w);



				m_f_zScreen			= z_Pos;
			}
			else throw SolveIt_ERROR(L"CRigidBody::SetScreenRect");
		}
		break;
		case T_BLOCK:
		{
			x_Pos -= m_fWidth/2;
			y_Pos -= m_fHeight/2;
			z_Pos -= m_fDepth/2;
	//		pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
			pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			double xPos = x.x+m_fWidth/2;
			double yPos = x.y+m_fHeight/2;
			double zPos = x.z+m_fDepth/2;
		//	pGLView->ComputeScreenCoords(&xPos, &yPos, &zPos);
			int hr = pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			m_rcScreen.left		= (long) (x_Pos);
			m_rcScreen.right	= (long) (xPos);
			m_rcScreen.top		= (long) (y_Pos);
			m_rcScreen.bottom	= (long) (yPos);
			m_f_zScreen			= z_Pos;
		}
		break;
	//	case T_CYLINDER:
	//	{
	//		( m_fRadius, m_fDepth );
	//	}
	//	break;
		default:
			return;
	}

	NormalizeRect(m_rcScreen);
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#if 0
void CRigidBody::SetScreenRect()
{
	double x_Pos = x.x;
	double y_Pos = x.y;
	double z_Pos = x.z;
	switch(type)
	{
		case T_SPHERE:
		case T_CYLINDER:
		case T_VECTOR_ANALYSIS_OBJECT:
		{
			HRESULT hr = theApp.GetGLView()->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
/*
			if (m_fRadius <= 0.1)
			{
				const double w = 8;
				if (!hr)
				{
					m_rcScreen.left		= (long) (x_Pos + 0.5 - w);
					m_rcScreen.right	= (long) (x_Pos + 0.5 + w);
					m_rcScreen.top		= (long) (y_Pos + 0.5 - w);
					m_rcScreen.bottom	= (long) (y_Pos + 0.5 + w);
					m_f_zScreen			= z_Pos;
				}
				NormalizeRect(m_rcScreen);
				return;
			}
*/
			double w = 8;
			double xPos = x.x+m_fRadius;
			double yPos = x.y;
			double zPos = x.z;
			theApp.GetGLView()->ComputeScreenCoords(&xPos, &yPos, &zPos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			if (!hr)
			{
				w = xPos - x_Pos;
				if (w < 8.0) w = 8;
				m_rcScreen.left		= (long) (x_Pos - w + 0.5);
				m_rcScreen.right	= (long) (x_Pos + w + 0.5);
				m_rcScreen.top		= (long) (y_Pos - w + 0.5);
				m_rcScreen.bottom	= (long) (y_Pos + w + 0.5);
				m_f_zScreen			= z_Pos;
			}
		}
		break;
		case T_BLOCK:
		{
			x_Pos -= m_fWidth/2;
			y_Pos -= m_fHeight/2;
			z_Pos -= m_fDepth/2;
			HRESULT hr = theApp.GetGLView()->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			double xPos = x.x+m_fWidth/2;
			double yPos = x.y+m_fHeight/2;
			double zPos = x.z+m_fDepth/2;
			hr = theApp.GetGLView()->ComputeScreenCoords(&xPos, &yPos, &zPos);
	pGLView->ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
			m_rcScreen.left		= (long) (x_Pos);
			m_rcScreen.right	= (long) (xPos);
			m_rcScreen.top		= (long) (y_Pos);
			m_rcScreen.bottom	= (long) (yPos);
			m_f_zScreen			= z_Pos;
		}
		break;
	//	case cylindricalBody:
	//	{
	//		( m_fRadius, m_fDepth );
	//	}
	//	break;
		default:
			return;
	}

	NormalizeRect(m_rcScreen);
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////

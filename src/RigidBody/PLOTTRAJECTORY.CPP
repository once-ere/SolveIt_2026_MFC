// PlotTrajectory.cpp: implementation of the CTrajectoryRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//
#include "RigidBody.h"
#include "TrajectoryRecord.h"
#include "parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// CTrajectoryRecord
long CTrajectoryRecord::PlotTrajectory(const Vector3D& color)
{
	void DrawCone(double coneRadius, double x, double y, double z, bool bTranslate);
	if (m_data.empty()) return -1;
//	const Vector3D& sc = theSystem->m_fScalePosition;

	wchar_t buf[2056];
	/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf));
try {
		int n = static_cast<int>(m_data.size());
		int j;
		if (n==0) return -1;
//		vector<Vector3D> x;
//		vector<Vector3D> p;
//		x.reserve(n);
//		p.reserve(n);
		GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
		if (bEnableLighting==GL_TRUE)
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);     
		}

		glLineWidth(1.0f);
		glColor3dv(color.v);
		glBegin(GL_LINE_STRIP);
		for (j=0;j<n; ++j)
		{
//			glVertex3d(m_data[j].x.x / sc.x,m_data[j].x.y / sc.y,m_data[j].x.z / sc.z);
			glVertex3d( m_data[j].x.x , m_data[j].x.y , m_data[j].x.z );
//			x.push_back(m_data[j].x);
//			p.push_back(m_data[j].p);CRigidBody* pThis
		}
		glEnd();
m_pCRigidBody = reinterpret_cast<CRigidBody*>((BYTE*)this - offsetof(CRigidBody, m_trajectoryRecord));

		long& offset = m_pCRigidBody->m_lShowPositionEveryNumSteps;
		if (offset>0 && n > offset)
		{
			Point3D p1 = m_data[0].x;
			Vector3D& col = m_pCRigidBody->m_vecColor;
		//	glColor3d(col.x*(0.5),col.y*(0.5),col.z*(0.5));
			int N=1;
			for (j=n-offset-1;j>=0; j-=offset)//=
			{
//				j=n-offset-1;
				p1 = m_data[j].x;
//				p1.x /= sc.x; 
//				p1.y /= sc.y; 
//				p1.z /= sc.z; 
			//	Point3D& p2 = m_data[n-1].x;
				Point3D p2 = m_data[j+offset].x;
//				p2.x /= sc.x; 
//				p2.y /= sc.y; 
//				p2.z /= sc.z; 
				glPushMatrix();
//#define SCC(x,a,n) ((x)+(N-1)*(1.0 - (x))/(N+(n)))*(a)
#define SCC(x,a,n) ((x)+sqrt(double(N-1)/double(N+(n)))*(1.0 - (x)))*(a)
				glColor3d(SCC(col.x,0.91, 1), SCC(col.y,0.91, 1), SCC(col.z,0.91, 1));
				++N;
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(p1.x, p1.y, p1.z);
				glEnd ();
				DrawCone(0.025, p1.x, p1.y, p1.z, true);

				Vector3D r = p2-p1;
				glTranslated(p1.x, p1.y, p1.z);
				glColor3d(SCC(col.x,0.65, 1), SCC(col.y,0.65, 1), SCC(col.z,0.65, 1));
#undef SCC
		glLineWidth(2.0f);
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(r.x, r.y, r.z);
				glEnd ();
		glLineWidth(1.0f);
				DrawCone(0.025, r.x, r.y, r.z, true);
				glPopMatrix();
			}
		}

		if (m_pCRigidBody->m_lShowVelocityEveryNumSteps>0)
		{
			Vector3D& col = m_pCRigidBody->m_vecColor;
		//	glColor3d(col.x*(1-color.x),col.y*(1-color.y),col.z*(1-color.z));
			glColor3d(col.x*(color.x),col.y*(color.y),col.z*(color.z));
			TrajectoryRecordData::iterator i;
			double sc=m_pCRigidBody->m_fScaleMomentum*m_pCRigidBody->m_fMassInverse;
			for (i=m_data.begin();i!=m_data.end();++i)
			{
				if	(m_pCRigidBody->m_lShowVelocityEveryNumSteps != 1 &&
					(distance(m_data.begin(),i)%m_pCRigidBody->m_lShowVelocityEveryNumSteps)
					) continue;
				glPushMatrix();
				glTranslated(i->x.x, i->x.y, i->x.z);
				glBegin (GL_LINES);
				glVertex3d( 0, 0, 0 );
				glVertex3d(sc*i->p.x, sc*i->p.y, sc*i->p.z);
				glEnd ();
				DrawCone(0.025, sc*i->p.x, sc*i->p.y, sc*i->p.z, true);
				glPopMatrix();
			}
		}
/*
*/
		if (bEnableLighting==GL_TRUE)
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
		}
		glLineWidth(1.0f);
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	TimedMessageBox(NULL,L"CTrajectoryRecord::PlotTrajectory",L"Exception",MB_OK);
	throw E_FAIL;
}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////

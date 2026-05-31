// DrawEquipotential.cpp : Implementation of CElectroStatics2D

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "FieldsManager.h"
#include "RigidBodyManager.h"
//#include "FieldLineODE.h"
//typedef Lock Lock;
// go figure

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElectroStatics2D

struct sortV {
	double d;
	int i;
};

int compare( const void *arg1, const void *arg2 ) {
	double a1 = (*(sortV**)arg1)->d;
	double a2 = (*(sortV**)arg2)->d;
	if (a1==a2) return 0;
	return a1-a2>0.0? 1:-1;
}
const int KMAX = 512;
const int JMAX = 16;

void CFieldsManager::DrawEquipotential2D(const Vector2D& fPt, int niter, bool bRecord)
{
	try
	{
	CSystem* m_system = theApp.GetSystem();
//	if (!m_bShowFieldLines) return;
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	if (niter>KMAX) niter=KMAX;
	double R = 1.0/JMAX;
	double potential = V (fPt);
	static sortV spot[KMAX];
	static Vector2D q[KMAX];
	ZeroMemory(spot, sizeof spot);
	ZeroMemory(q, sizeof q);
	sortV* pspot[KMAX];
	double Theta = 2.0* PI/(niter-1);
	int j, k;

	if (bRecord)
	{
		m_currentEquipotential.clear();
		m_currentEquipotentialPoint = Vector3D(fPt.x,fPt.y,0);
	}


	Vector2D Q1, Q2, pt(fPt), prev(fPt), tmp;
//	glColor3d(0,0.2,1);
	glColor3d(0x33/255., 0xff/255., 0x33/255.);


	glBegin(GL_LINES);
	glVertex2d(fPt.x, fPt.y);
	for (j=0;j<=JMAX;j++) {
		for (k=0;k<niter;k++) {
			q[k].x = pt.x + R*cos(k*Theta);
			q[k].y = pt.y + R*sin(k*Theta);
			spot[k].d = fabs(potential - V (q[k]));//f.V(q[k], pV));//ok to reuse f
			spot[k].i = k;
			pspot[k] = &spot[k];
		}
		qsort( (void *)&pspot, (size_t)niter, sizeof( sortV* ), compare );
		if (j) {
			for (k=0;k<niter;k++) {
				Q1 = q[pspot[k]->i];
				tmp = prev - Q1;
				if (tmp.Norm() > R) break;
			}
			prev = pt;
		}
		else {
			Q1 = q[pspot[0]->i];
			for (k=1;k<niter;k++) {
				Q2 = q[pspot[k]->i];
				tmp = Q2 - Q1;
				if (tmp.Norm() > R) break;
			}
		}
		pt = Q1;
		glVertex2d(pt.x, pt.y);
		if (bRecord)
		{
			m_currentEquipotentialPoint = Vector3D(pt.x, pt.y, 0);
		//	for (int h=0;h<3;++h)
				m_currentEquipotential.push_back(m_currentEquipotentialPoint);//[h]);
		}
	}
	glEnd();

	if (bRecord)
	{
//		vector_of_double* currentFieldLine = new vector_of_double;
		vector_of_Vector3D* currentFieldLine = new vector_of_Vector3D(0x99/255., 0x66/255., 0x33/255.);
		currentFieldLine->clear();
//		vector_of_double::const_iterator i_data;
		vector_of_Vector3D::const_iterator i_data;
		{
			Lock lock(protectFieldLines);
			for (i_data=m_currentEquipotential.begin();i_data!=m_currentEquipotential.end(); ++i_data) currentFieldLine->push_back( *i_data );
				push_back(currentFieldLine);
		}
	}
	pt=Q2;
	prev=fPt;

	if (bRecord)
	{
		m_currentEquipotential.clear();
		m_currentEquipotentialPoint = Vector3D(fPt.x,fPt.y,0);
	}
	glBegin(GL_LINES);
	glVertex2d(fPt.x, fPt.y);
	for (j=0;j<=JMAX;j++) {
		for (k=0;k<niter;k++) {
			q[k].x = pt.x + R*cos(k*Theta);
			q[k].y = pt.y + R*sin(k*Theta);
			spot[k].d = fabs(potential - V (q[k]));// f.V(q[k], pV));//ok to reuse f
			spot[k].i = k;
			pspot[k] = &spot[k];
		}
		qsort( (void *)&pspot, (size_t)niter, sizeof( sortV* ), compare );
		if (j) {
			for (k=0;k<niter;k++) {
				Q1 = q[pspot[k]->i];
				tmp = prev - Q1;
				if (tmp.Norm() > R) break;
			}
			prev = pt;
		}
		else {
			Q1 = q[pspot[0]->i];
		}
		pt = Q1;
		glVertex2d(pt.x, pt.y);
		if (bRecord)
		{
			m_currentEquipotentialPoint = Vector3D(pt.x, pt.y, 0);
		//	for (int h=0;h<3;++h)
				m_currentEquipotential.push_back(m_currentEquipotentialPoint);//[h]);
		}
	}
	glEnd();

	if (0&&bRecord)
	{
//		vector_of_double* currentFieldLine = new vector_of_double;
		vector_of_Vector3D* currentFieldLine = new vector_of_Vector3D(0,0.4,0);
		currentFieldLine->clear();
//		vector_of_double::const_iterator i_data;
		vector_of_Vector3D::const_iterator i_data;
		{
			Lock lock(protectFieldLines);
			for (i_data=m_currentEquipotential.begin();i_data!=m_currentEquipotential.end(); ++i_data) currentFieldLine->push_back( *i_data );
				push_back(currentFieldLine);
		}
	}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		Clear();
		TimedMessageBox(NULL,L"Out of Memory",L"CFieldsManager",MB_ICONWARNING);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double CFieldsManager::V(const Vector2D& pt) {
	const double smallR = 1.0e-6;
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	double potential = 0;
	try {
	if (rigidBodyManager->empty()) return potential;
	ObjectsList::iterator it;
	for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
	{
		CRigidBody* c = dynamic_cast<CRigidBody*>(dynamic_cast<CRigidBody *>((*it).rbPtr));
		if (c!=NULL) {
			Point3D p = c->x;
			Vector2D m_fPt(p.x, p.y);
			Vector2D q=pt - m_fPt;
			double R = q.Norm();
			if (R < smallR) R=smallR;
			potential += c->m_fCharge/R;
		}
		else throw SolveIt_ERROR(L"pbody == 0");//
	}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
	return potential;
}
///////////////////////////////////////////////////////////////////////////////
void CFieldsManager::DrawVector2D(const Vector2D& Pt) {
//	CSystem* psys = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theFieldsManager));
//	CRigidBodyManager* pThis = reinterpret_cast<CRigidBodyManager*>((BYTE*)psys - offsetof(CRigidBodyManager, theSystem));
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	try {
	if (!rigidBodyManager->empty())
	{
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}
	glDisable(GL_DITHER);
	glDisable(GL_CULL_FACE);
	Vector2D pt(Pt);
	Vector2D e(E(pt));
///////////////////////////////////////////////////////////////////////////////
	TCHAR buf[256];
	std::wstring bstr=L"";
	_stprintf_s(buf, _countof(buf), L"( %lg , %lg ) E=( %lg , %lg )", pt.x, pt.y, e.x, e.y);
	bstr = buf;
	Fire_WriteStatus( bstr.c_str() );
///////////////////////////////////////////////////////////////////////////////
//	e *= m_fScaleElectricField;
	glLineWidth(2.0f);
	glColor3d(0,0,0);
	glBegin(GL_LINES);
	glVertex2d(pt.x,pt.y);
	pt += e;
	glVertex2d(pt.x,pt.y);
	glEnd();

		double X	= pt.x;
		double Y	= pt.y;
		double dx	= e.x;
		double dy	= e.y;
		double R	= sqrt(dx*dx+dy*dy);
		if ( R > 0.0 ) {
			dx /= R;
			dy /= R;
		}
		const double asize	= 0.1;
		double c	= asize*cos(PI/6.0);
		double s	= asize*sin(PI/6.0);
	Vector2D Arrow[3];
	Arrow[0].x= ( X );
	Arrow[0].y= ( Y );
	Arrow[1].x= ( X - ( c*dx-s*dy ));
	Arrow[1].y= ( Y - ( s*dx+c*dy ));
	Arrow[2].x= ( X - ( c*dx+s*dy ));
	Arrow[2].y= ( Y - (-s*dx+c*dy ));

	glLineWidth(1.0f); 
	glBegin(GL_POLYGON);
	for (int j=0;j<3;++j) glVertex2d(Arrow[j].x, Arrow[j].y);
	glEnd();
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);     
	}
	}
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
// 



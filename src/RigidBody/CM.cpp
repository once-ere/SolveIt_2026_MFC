// CM.cpp : Implementation of CConstraintManager

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "PtToPtConstraint.h"
//#include "RigidBody.h"
#include "ConstraintManager.h"
#include "RigidBodyManager.h"
#include "parser.h" /* for token definitions and yylval */
#include "MainFrm.h"
///////////////////////////////////////////////////////////////////////////////
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::Remove(const string& name)
{
	try
	{
		if (Empty()) return;
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		CRigidBody *body = rigidBodyManager->get_Body(name);
		if (!body) return;

		RemoveBodyFromVecList(body);
		Fire_ViewChange();
		return;

		string n ( constraint_prefix+body->name );
		vector<ConstraintNameToKey::iterator> vecit;
		vector<ConstraintNameToKey::iterator>::iterator ivecit;
		ConstraintNameToKey::iterator i;
		for (i=m_nameToKey.begin();i!=m_nameToKey.end();++i)
		{
			if (i->first.find(name) > 0)
			{
				long Key = i->second;
				vector<KeyToConstraint_List*>::iterator ik;
				for (ik = begin(); ik != end(); ++ik)
				{
					KeyToConstraint_List& k = *(*ik);
					map< unsigned long, CConstraint *>::iterator it = k.find(Key);
					if (it != k.end())
					{
						if (it->second) {
							delete it->second;
						}
						it->second = 0;
						k.erase(it);
//						map< unsigned long, string>::iterator its = m_keyToName.find(Key);
//						if ( its != m_keyToName.end() ) m_keyToName.erase(its);
//						m_nameToKey.erase(i);
						vecit.push_back(i);
					}
				}
			}
		}
		for (ivecit=vecit.begin();ivecit!=vecit.end();++ivecit)
			m_nameToKey.erase(*ivecit);

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->UpdateTree();
	Fire_ViewChange();
		
		return;
	}
	catch( ... )
	{
		::MessageBox(NULL,"CConstraintManager::Remove","exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddPointToPointConstraint(
													CRigidBody* body1,
													const Point3D& point1,
													CRigidBody* body2,
													const Point3D& point2,
													double _separation,
													const string& option
													)
{
	//	TimedMessageBox(NULL, CVOdeErrors[0], "CConstraintManager::AddPointToPointConstraint", MB_OK, 1000);
	try
	{
		CSystem* m_system = theApp.GetSystem();
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		m_system->Stop();

		bool bDeactivateIfPositive	= false;
		bool bDeactivateIfNegative	= false;
		bool bZeroVelocity			= false;
		if (option == _positive) bDeactivateIfNegative = true;
		if (option == _negative) bDeactivateIfPositive = true;
		if (option == _zeroVelocity) bZeroVelocity = true;


		string n1 = "";
		if (body1)
		{
			n1 = constraint_prefix+body1->name;
			AddBodyToVecList(body1);
		}

		string n2 = "";
		if (body2)
		{
			n2 = constraint_prefix+body2->name;
			AddBodyToVecList(body2);
		}

	char s[256];
	ostrstream oss(s, sizeof s);
	oss << n1;
	oss << '[' << point1.x << "|" << point1.y << "|" << point1.z << ']' ;
	oss << n2;
	oss << '[' << point2.x << "|" << point2.y << "|" << point2.z << ']' ;
	oss << '=' << _separation << ends ;
	string strName = s;
	map< string, unsigned long>::iterator is = m_nameToKey.find(strName);
	if ( is != m_nameToKey.end() ) return;

	CPointToPointConstraint * c = 
							new CPointToPointConstraint(	body1, point1,
															body2, point2,
															_separation);

	c->m_Key = m_keyCount++;

	if (bDeactivateIfPositive) c->m_bDeactivateIfPositive = true;
	if (bDeactivateIfNegative) c->m_bDeactivateIfNegative = true;


	c->name = strName;
	m_nameToKey[c->name]	= c->m_Key;
//	m_keyToName[c->m_Key]		= c->name;
	AddConstraintToVecList(c);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool ps1 = false;
	bool ps2 = false;

	bool pb1 = false;
	bool pb2 = false;

	bool pc1 = false;
	bool pc2 = false;

	bool pv1 = false;
	bool pv2 = false;
///////////////////////////////////////////////////////////////////////////////
	if (body1)  ps1 = body1->type==T_SPHERE;
	if (body2)  ps2 = body2->type==T_SPHERE;

	if (body1)  pb1 = body1->type==T_BLOCK;
	if (body2)  pb2 = body2->type==T_BLOCK;

	if (body1)  pc1 = body1->type==T_CYLINDER;
	if (body2)  pc2 = body2->type==T_CYLINDER;

	if (body1)  pv1 = body1->type==T_VECTOR_ANALYSIS_OBJECT;
	if (body2)  pv2 = body2->type==T_VECTOR_ANALYSIS_OBJECT;
///////////////////////////////////////////////////////////////////////////////
	if (bZeroVelocity)
	{
		m_listZeroVelocityAtFixedPoint[c->m_Key] = c;
	}
	else if (pb1 || pb2 || pc1 || pc2)
	{
		m_listNonPointMassConstraints[c->m_Key] = c;
	}
	else
	{
		m_listPointMassConstraints[c->m_Key] = c;
	}

	CalculateConsistentVelocity();
	ListConstraints();
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->UpdateTree();
	Fire_ViewChange();

/*
int    deg = 2;
Jet::Setup( 7, deg, 6 );
coord  x1( body1->x.x ),  y1( body1->x.y ),  z1( body1->x.z );
coord  x2( body2->x.x ),  y2( body2->x.y ),  z2( body2->x.z );
V.SetComponent( c->m_Key, sqrt( pow( x1-x2, 2 ) + pow( y1-y2, 2 ) + pow( z1-z2, 2 ) ) );
*/
	}
	catch( ... )
	{
		::MessageBox(NULL,"AddPointToPointConstraint","exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddPointToPointConstraint(
													CRigidBody* body1,
													const Point3D& point1,
													CRigidBody* body2,
													const Point3D& point2,
													double _separation,
													bool _bDeactivateIfPositive,
													bool _bDeactivateIfNegative
													)
{
	try
	{
		CSystem* m_system = theApp.GetSystem();
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		m_system->Stop();

		string n1 = "";
		if (body1)
		{
			n1 = constraint_prefix+body1->name;
			AddBodyToVecList(body1);
		}

		string n2 = "";
		if (body2)
		{
			n2 = constraint_prefix+body2->name;
			AddBodyToVecList(body2);
		}

	char s[256];
	ostrstream oss(s, sizeof s);
	oss << n1;//(body1? body1->name :"") ; 
	oss << '[' << point1.x << "|" << point1.y << "|" << point1.z << ']' ;
	oss << n2;//(body2? body2->name :"") ; 
	oss << '[' << point2.x << "|" << point2.y << "|" << point2.z << ']' ;
	oss << '=' << _separation << ends ;
	string strName = s;
	map< string, unsigned long>::iterator is = m_nameToKey.find(strName);
	if ( is != m_nameToKey.end() ) return;

	CPointToPointConstraint * c = 
							new CPointToPointConstraint(	body1, point1,
															body2, point2,
															_separation);
	m_vecTwoPointConstraints.push_back(c);

	c->m_Key = m_keyCount++;

	if (_bDeactivateIfPositive) c->m_bDeactivateIfPositive = true;
	if (_bDeactivateIfNegative) c->m_bDeactivateIfNegative = true;


	c->name = strName;
	m_nameToKey[c->name]	= c->m_Key;
//	m_keyToName[c->m_Key]		= c->name;
//	if (body1) AddBodyToVecList(body1, c);
//	if (body2) AddBodyToVecList(body2, c);
	AddConstraintToVecList(c);


///////////////////////////////////////////////////////////////////////////////
	bool ps1 = false;
	bool ps2 = false;

	bool pb1 = false;
	bool pb2 = false;

	bool pc1 = false;
	bool pc2 = false;

	bool pv1 = false;
	bool pv2 = false;
///////////////////////////////////////////////////////////////////////////////
	if (body1)  ps1 = body1->type==T_SPHERE;
	if (body2)  ps2 = body2->type==T_SPHERE;

	if (body1)  pb1 = body1->type==T_BLOCK;
	if (body2)  pb2 = body2->type==T_BLOCK;

	if (body1)  pc1 = body1->type==T_CYLINDER;
	if (body2)  pc2 = body2->type==T_CYLINDER;

	if (body1)  pv1 = body1->type==T_VECTOR_ANALYSIS_OBJECT;
	if (body2)  pv2 = body2->type==T_VECTOR_ANALYSIS_OBJECT;
///////////////////////////////////////////////////////////////////////////////
	if (pb1 || pb2 || pc1 || pc2)
	{
		m_listNonPointMassConstraints[c->m_Key] = c;
	}
	else
	{
		m_listPointMassConstraints[c->m_Key] = c;
	}

//	push_back(c);
	
	CalculateConsistentVelocity();//body2
		ListConstraints();
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->UpdateTree();
		Fire_ViewChange();

/*
int    deg = 2;
Jet::Setup( 7, deg, 6 );
coord  x1( body1->x.x ),  y1( body1->x.y ),  z1( body1->x.z );
coord  x2( body2->x.x ),  y2( body2->x.y ),  z2( body2->x.z );
V.SetComponent( c->m_Key, sqrt( pow( x1-x2, 2 ) + pow( y1-y2, 2 ) + pow( z1-z2, 2 ) ) );
*/

	}
	catch( ... )
	{
		::MessageBox(NULL,"AddPointToPointConstraint","exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//call after m_system->Compute_Force_and_Torque(m_system->m__t);
//call after AssignVarsToColumns;
void CConstraintManager::GetUnconstrainedResultantForce(vector<double>& F) {
	if (Empty()) return;
//	vector<double> b(m_vecUnconstrainedResultantForce.size(), &m_vecUnconstrainedResultantForce.front());
	F = m_vecUnconstrainedResultantForce;
}
//////////////////////////////////////////////////////////////////////
void CConstraintManager::GetNumericalDamping(vector<double>& pos, vector<double>& vel) {
	m_vecNumericalDampingPosition.clear();
	m_vecNumericalDampingVelocity.clear();
	vector<CConstraint*>::iterator i;
	for (i=m_vecUniqueConstraints.begin();i != m_vecUniqueConstraints.end();++i)
	{
		CConstraint* pc = dynamic_cast<CConstraint*>(*i);
		if (pc)
		{
			m_vecNumericalDampingPosition.push_back(pc->m_fNumericalPositionDamping);
			m_vecNumericalDampingVelocity.push_back(pc->m_fNumericalVelocityDamping);
		}
	}
/*
	if (Empty()) return;
	vector<CConstraint*> m_vecFoundConstraints;

	vector<CRigidBody*>::iterator it;
	for(it=m_vecBodiesCurrentTimeStep.begin();it != m_vecBodiesCurrentTimeStep.end(); it++)
	{
		CRigidBody* body = dynamic_cast<CRigidBody*>(*it);
		if (body)
		{
//			long n = 3*distance(m_vecBodiesCurrentTimeStep.begin(), it);
			vector<CConstraint*>::iterator j;
			for (j=body->m_ListOfConstraints.begin();j != body->m_ListOfConstraints.end();++j)
			{
				CConstraint* pc = dynamic_cast<CConstraint*>(*j);
				if (pc)
				{
					CompareConstraintToMe compareConstraintToMe(pc);
					vector<CConstraint*>::iterator ic = find_if(m_vecFoundConstraints.begin(), m_vecFoundConstraints.end(), compareConstraintToMe);
					if (ic == m_vecFoundConstraints.end())
					{
						m_vecFoundConstraints.push_back(pc);
						ic = find_if(m_vecUniqueConstraints.begin(), m_vecUniqueConstraints.end(), compareConstraintToMe);
						if (ic != m_vecUniqueConstraints.end())
						{
							long nc = distance(m_vecUniqueConstraints.begin(), ic);
							ASSERT(nc < m_vecUniqueConstraints.size());
							m_vecNumericalPositionDamping.push_back(pc->m_fNumericalPositionDamping);
							m_vecNumericalVelocityDamping.push_back(pc->m_fNumericalVelocityDamping);
						}
					}
				}
			}
		}
	}
*/
	ASSERT(m_vecUniqueConstraints.size()==m_vecNumericalDampingPosition.size());
	ASSERT(m_vecUniqueConstraints.size()==m_vecNumericalDampingVelocity.size());

//	vector<double> p(m_vecNumericalDampingPosition.size(), &m_vecNumericalDampingPosition.front());
	pos = m_vecNumericalDampingPosition;
//	vector<double> v(m_vecNumericalDampingVelocity.size(), &m_vecNumericalDampingVelocity.front());
	vel = m_vecNumericalDampingVelocity;
}
//////////////////////////////////////////////////////////////////////
	double m_fNumericalPositionDamping;
	double m_fNumericalVelocityDamping;
//////////////////////////////////////////////////////////////////////
void CConstraintManager::GetSystemVelocity(vector<double>& V) {
	if (Empty()) return;
//	vector<double> b(m_vecSystemVelocity.size(), &m_vecSystemVelocity.front());
	V = m_vecSystemVelocity;
//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, constraintManager));
//	rigidBodyManager->GetSystemVelocity(V);
}
//////////////////////////////////////////////////////////////////////
void CConstraintManager::GetSystemInverseMassMatrix(double **& W) {
	if (Empty()) return;
	int N = m_vecSystemInverseMassMatrix.size();
	double **m = CVODE::denalloc(N);
		if (m == NULL) throw SolveIt_ERROR("CConstraintManager::GetSystemInverseMassMatrix: denalloc memory request failed");
	W = m;
	vector<double>::iterator j;
	int i=0;
	for (	j=m_vecSystemInverseMassMatrix.begin();
			j!=m_vecSystemInverseMassMatrix.end();++j, ++i)
				W[i][i] = *j;
//	CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, constraintManager));
//	rigidBodyManager->GetSystemInverseMassMatrix(W);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CConstraintManager::Jacobian(double t, double **& A, int nSize) {
	if (Empty()) return;
	ASSERT(nSize > 0);
	if ( nSize<= 0 ) return;
	long& N = m_NumberOfIndependentVariables;//must have called AssignVarsToColumns
//	double ** a(nSize, N);
	int n	= (nSize<N?N:nSize);
	double **a = CVODE::denalloc(n);
	if (a == NULL) throw SolveIt_ERROR("CConstraintManager::GetSystemInverseMassMatrix: denalloc memory request failed");
	CVODE::denzero(a,n);


	A = a;
	for (int j=0;j<nSize;++j) 
		for (int k=0;k<N;++k)  A[j][k]=0.0;
	vector<double> vecJ;
/*
	vector<int> rows;
	vector<int> cols;
	map< unsigned long,  CConstraint*>::iterator row;
	vector<KeyToConstraint_List*>::iterator i;
	for (i = begin(); i != end(); ++i)
	{
		KeyToConstraint_List& k = *(*i);
	for (row = k.begin(); row != k.end(); ++row)
	{
		int Row = distance(k.begin(), row);
		vecJ.clear();
		cols.clear();
		CPointToPointConstraint* ppm = dynamic_cast<CPointToPointConstraint*>(row->second);
		if (ppm) {
			ppm->Jacobian( t, vecJ, cols, var2col);//must have called AssignVarsToColumns
			vector<int>::iterator j;
			vector<double>::iterator J = vecJ.begin();
			for (j=cols.begin();j!=cols.end();++j, ++J) A[Row][*j] = *J;
		}
	}
	}
*/
	vector<CConstraint*>::iterator row;
	for (row = m_vecUniqueConstraints.begin(); row != m_vecUniqueConstraints.end(); ++row)
	{
		int Row = distance(m_vecUniqueConstraints.begin(), row);
		vecJ.clear();
		vecJ.reserve(N);
		for (j=0;j<N;++j) vecJ[j]=0;
		CConstraint* pc = dynamic_cast<CConstraint*>(*row);
		if (pc) {
			pc->Jacobian( t, vecJ);//must have called AssignVarsToColumns
			vector<double>::iterator J = vecJ.begin();
			for (j=0;j<N;++j, ++J)
			{
				A[Row][j] = *J;
		//		double xxx=A[Row][j];
		//		A[Row][j] = vecJ[j];
		//		double yyy=A[Row][j];
		//		ASSERT(xxx==yyy);
			}
		}
	}

}
//////////////////////////////////////////////////////////////////////
void CConstraintManager::NormalizedJacobian(double t, double **& A, int nSize) {
	if (Empty()) return;
	ASSERT(nSize > 0);
	if ( nSize<= 0 ) return;
	long& N = m_NumberOfIndependentVariables;//must have called AssignVarsToColumns
	double **a = CVODE::denalloc(N);
	if (a == NULL) throw SolveIt_ERROR("CConstraintManager::GetSystemInverseMassMatrix: denalloc memory request failed");
	CVODE::denzero(a,N);
	A = a;
	for (int j=0;j<nSize;++j) 
		for (int k=0;k<N;++k)  A[j][k]=0.0;
	vector<double> vecJ;
	vector<CConstraint*>::iterator row;
	for (row = m_vecUniqueConstraints.begin(); row != m_vecUniqueConstraints.end(); ++row)
	{
		int Row = distance(m_vecUniqueConstraints.begin(), row);
		vecJ.clear();
		vecJ.reserve(N);
		for (j=0;j<N;++j) vecJ[j]=0;
		CConstraint* pc = dynamic_cast<CConstraint*>(*row);
		if (pc) {
			pc->NormalizedJacobian( t, vecJ);//must have called AssignVarsToColumns
			vector<double>::iterator J = vecJ.begin();
			for (j=0;j<N;++j, ++J)
			{
				A[Row][j] = *J;
			//	double xxx=A[Row][j];
			//	A[Row][j] = vecJ[j];
			//	double yyy=A[Row][j];
			//	ASSERT(xxx==yyy);
			}
		}
	}

}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::HessianDotVelocity(double t, double **& A, int nSize) {
	if (Empty()) return;
	long& N = m_NumberOfIndependentVariables;//must have called AssignVarsToColumns
//	double ** a(nSize, N);
	int n	= (nSize<N?N:nSize);
	double **a = CVODE::denalloc(n);
	if (a == NULL) throw SolveIt_ERROR("CConstraintManager::GetSystemInverseMassMatrix: denalloc memory request failed");
	CVODE::denzero(a,n);
	A = a;
	for (int j=0;j<nSize;++j) 
		for (int k=0;k<N;++k)  A[j][k]=0.0;
	vector<double> vecJ;
	vector<int> cols;
	vector<CConstraint*>::iterator row;
	for (row = m_vecUniqueConstraints.begin(); row != m_vecUniqueConstraints.end(); ++row)
	{
		int Row = distance(m_vecUniqueConstraints.begin(), row);
		vecJ.clear();
		vecJ.reserve(N);
		for (j=0;j<N;++j) vecJ[j]=0;
		CConstraint* pc = dynamic_cast<CConstraint*>(*row);
		if (pc) {
			pc->HessianDotVelocity( t, vecJ);//must have called AssignVarsToColumns
			vector<double>::iterator J = vecJ.begin();
			for (j=0;j<N;++j, ++J)
			{
				A[Row][j] = *J;
		//		double xxx=A[Row][j];
		//		A[Row][j] = vecJ[j];
		//		double yyy=A[Row][j];
		//		ASSERT(xxx==yyy);
			}
		}
	}

}
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::D_Jacobian_dt(double t, double **& A, int nSize) {
	return;
	if (Empty()) return;
	long& N = m_NumberOfIndependentVariables;//must have called AssignVarsToColumns
//	double ** a(nSize, N);
	int n	= (nSize<N?N:nSize);
	double **a = CVODE::denalloc(n);
	if (a == NULL) throw SolveIt_ERROR("CConstraintManager::GetSystemInverseMassMatrix: denalloc memory request failed");
	CVODE::denzero(a,n);
	A = a;
	vector<double> vecJ;
	vector<int> rows;
	vector<int> cols;
	int col=0;
//	map< unsigned long,  CPointMassConstraint*>::iterator row;
	map< unsigned long,  CConstraint*>::iterator row;
	vector<KeyToConstraint_List*>::iterator i;
	for (i = begin(); i != end(); ++i)
	{
		KeyToConstraint_List& k = *(*i);
	for (row = k.begin(); row != k.end(); ++row, col=0)
	{
		int Row = distance(k.begin(), row);
		vecJ.clear();
		cols.clear();
		CPointToPointConstraint* ppm = dynamic_cast<CPointToPointConstraint*>(row->second);
		if (ppm) {
			ppm->D_Jacobian_dt( t, vecJ, cols, var2col);//must have called AssignVarsToColumns
			vector<int>::iterator j;
			vector<double>::iterator J = vecJ.begin();
			for (j=cols.begin();j!=cols.end();++j, ++J) A[Row][*j] = *J;
		}
	}
	}
	vector<CConstraint*>::iterator it;
	for (it = m_vecUniqueConstraints.begin(); it != m_vecUniqueConstraints.end(); ++it)
	{
	}
}
//////////////////////////////////////////////////////////////////////
void CConstraintManager::Error(double t, vector<double>& A) {
	if (Empty()) return;
	vector<double> vecError;

	vector<CConstraint*>::iterator it;
	for (it = m_vecUniqueConstraints.begin(); it != m_vecUniqueConstraints.end(); ++it)
	{
		CConstraint* ppm = dynamic_cast<CConstraint*>(*it);
		if (ppm) ppm->Error( t, vecError);
	}
//	vector<double> b(vecError.size(), &vecError.front());
	ASSERT(m_vecUniqueConstraints.size()==vecError.size());
	A = vecError;
}
//////////////////////////////////////////////////////////////////////
void CConstraintManager::AddConstraintForces(vector<double>& f, bool bDuringCollision)
{
	if (Empty()) return;
	vector<CConstraint*>::iterator it;
	for (it = m_vecUniqueConstraints.begin(); it != m_vecUniqueConstraints.end(); ++it)
	{
		CConstraint* pc = dynamic_cast<CConstraint*>(*it);
		if (pc)
		{
			pc->AddConstraintForce(f, bDuringCollision);
		//	if (bDuringCollision) pc->AddConstraintImpulse(f);
		//	else pc->AddConstraintForce(f);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
long CConstraintManager::GetNumberOfIndependentVariables() {
	if (Empty()) return 0;
	return m_NumberOfIndependentVariables;
}
//////////////////////////////////////////////////////////////////////
// AssignVarsToColumns:
//////////////////////////////////////////////////////////////////////
//sets ud.body?->m_bConstraintForceShouldBeUpdated = true;
//defines m_vecSystemInverseMassMatrix
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void CConstraintManager::Remove(long Key)
{
// (Could just use erase on the key, but it's not clear what erase does if the item
//  isn't present. Our method is safer and allows us to inform the client of failure.)
	try
	{
		if (Empty()) return;
		CSystem* pThis = reinterpret_cast<CSystem*>((BYTE*)this - offsetof(CSystem, theConstraintManager));
CRigidBodyManager* rigidBodyManager = m_system->m_rigidBodyManager;
		m_system->Stop();
		map< unsigned long, CPointMassConstraint *>::iterator it = find(Key);
		if (it == end()) return;
//		if (it->second) (*it)->Destroy(), delete it->second;
//		it->second = 0;
//		erase(it);
		map< unsigned long, string>::iterator its = m_keyToName.find(Key);
		if ( its == m_keyToName.end() ) return;
		Remove(its->second);

	//	NameToKey::iterator i = m_nameToKey.find(its->second);
	////	m_keyToName.erase(its);
	//	if (i == m_nameToKey.end()) return;
	//	m_nameToKey.erase(i);

	//	Fire_ViewChange();
	}
	catch( ... )
	{
		::MessageBox(NULL,"CConstraintManager::Remove","exception",MB_OK);
	}
}
*/
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//void CConstraintManager::FindConstraints(const string& name, vector<CPointMassConstraint*>& vec)
void CConstraintManager::FindConstraints(const string& name, vector<CConstraint*>& vec)
{
	if (Empty()) return;
	try
	{
		CSystem* m_system = theApp.GetSystem();
		CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
		CRigidBody *body = rigidBodyManager->get_Body(name);
		if (!body) return;

		string n ( constraint_prefix+body->name );
		ConstraintNameToKey::iterator i;
		for (i=m_nameToKey.begin();i!=m_nameToKey.end();++i)
		{
			if (i->first.find(name) > 0)
			{
				long Key = i->second;
				vector<KeyToConstraint_List*>::iterator i;
				for (i = begin(); i != end(); ++i)
				{
					KeyToConstraint_List& k = *(*i);
				//	map< unsigned long, CPointMassConstraint *>::iterator it = k.find(Key);
					map< unsigned long, CConstraint *>::iterator it = k.find(Key);
					if (it != k.end())
					{
						if (it->second) vec.push_back(it->second);
					}
				}
	vector<CConstraint*>::iterator it;
	for (it = m_vecUniqueConstraints.begin(); it != m_vecUniqueConstraints.end(); ++it)
	{
	}
			}
		}
	}
	catch( ... )
	{
		::MessageBox(NULL,"CConstraintManager::FindConstraints","exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////

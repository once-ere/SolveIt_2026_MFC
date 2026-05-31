//Tension.cpp
#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"
#include "ConstraintManager.h"
#include "PulleyConstraint.h"
#include "System.h"
#include "RigidBodyManager.h"

#include "FieldsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double TensionsLeft11(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);
double TensionsLeft12(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);
double TensionsLeft21(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);
double TensionsLeft22(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);

double TensionsRight11(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);

double TensionsRight12(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);

double TensionsRight21(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);

double TensionsRight22(double R, double g, double Inertia, double w,
					 double m1, double x1, double y1, double v1x, double v1y,
					 double m2, double x2, double y2, double v2x, double v2y);


#define Power(a,b) pow(fabs(a),(b))
#define Sqrt(a) sqrt(fabs(a))

///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::AddPulleyConstraint(
									CRigidBody*  leftBody,
									CRigidBody*  pulley,
									CRigidBody*  rightBody,
									int nLeftBodyConnectedInQuadrant,
									int nRightBodyConnectedInQuadrant
									)
{
	try
	{
		vector< CPulleyConstraint*>::iterator i;
		for (i = m_vecPulleyConstraints.begin(); i != m_vecPulleyConstraints.end(); ++i)
		{
			CPulleyConstraint* ppc = *i;//dynamic_cast<CPulleyConstraint*>(row->second);
			if (ppc)
			{
				if (ppc->m_pulley==pulley)
				{
					if (rightBody)	ppc->m_rightBody	= rightBody;
					if (leftBody)	ppc->m_leftBody		= leftBody;
	if (leftBody)
	{
		leftBody ->InitializePulleyConnection(pulley->m_fRadius, nLeftBodyConnectedInQuadrant );
	//	leftBody ->set_DisableSelect(true);
	}
	if (rightBody)
	{
		rightBody->InitializePulleyConnection(pulley->m_fRadius, nRightBodyConnectedInQuadrant);
	//	rightBody->set_DisableSelect(true);
	}
					if (ppc->m_leftBody && ppc->m_rightBody)
					{
						InitializePulleyConstraints(ppc);
					}
					Fire_ViewChange();
					return;
				}
			}
		}


		CPulleyConstraint * c = 
							new CPulleyConstraint(leftBody, pulley, rightBody,
													nLeftBodyConnectedInQuadrant,
													nRightBodyConnectedInQuadrant);

		m_vecPulleyConstraints.push_back(c);

/*
*/
		if (leftBody && rightBody)
		{
			InitializePulleyConstraints(c);
		}
		Fire_ViewChange();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch( ... )
	{
		::MessageBox(NULL,L"AddPulleyConstraint",L"exception",MB_OK);
	}
}
///////////////////////////////////////////////////////////////////////////////

/*
-++++++++�_���a�GpSs�tFTOd8fen=�==()�����vn���� � � � � �
????????????????????????

FOR POINT MASSES:

C_�			= 0
C_�,j x_j'	= 0

Let	C_�,j = C_�j = Transpose{C_j�}

C_�,jk x_j' x_k' + C_�,j x_j''		= 0
W = 1 / M
C_�,jk x_j' x_k' + C_�,j W_jk F_k	= 0

Introduce constraint-enforcing Q_j:
C_�,jk x_j' x_k' + C_�,j W_jk { F_k + Q_k }			= 0
Apply the 'Principle of Virtual Work':	Q_j x_j'	= 0 :
=> 
Q_j = �_� C_�,j
Whence
C_�,jk x_j' x_k' + C_�,j W_jk { F_k + �_�' C_�',k }	= 0

Generalized constraint forces = �_�

Let	T_��'	= C_�,j W_jk C_�',k = C_�j W_jk C_k�'	(	=	W_��')

T_��' �_�'	= -C_�,jk x_j' x_k' - C_�,j W_jk F_k
--OR--
W_�	= W_��' �_�'	= -C_�,jk x_j' x_k' - C_�,j W_jk F_k
W_�	+ C_�,jk x_j' x_k' + C_�,j W_jk F_k = 0

Collision:
C_�,jk �[x_j' dx_k + x_k' dx_j ] + C_�,j W_jk { F_k + �_�' C_�',k } dt	= 0

Let	J_k = collision impulse
	H_k = constraint impulse = h_� C_�,k

IF  Integral[C_�,jk x_j'] dx_k = 0 THEN
C_�,j W_jk { J_k + h_�' C_�',k } 	= 0
T_��' h_�'	=  - C_�,j W_jk J_k

*/
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CConstraintManager::PulleyConstraintForce(const double& fRadius,
											   const double& x_right, const double& y_right,
											   const double& Vx_right, const double& Vy_right,
											   const double& Fx_right, const double& Fy_right,
											   const double& x_left, const double& y_left,
											   const double& Vx_left, const double& Vy_left,
											   const double& Fx_left, const double& Fy_left,
											   const double& mass_right, const double& mass_left,
											   double& Jx, double& Jy, double& JX, double& JY, double& lambda
											   )
{
	const double mu	=  (mass_right * mass_left)/(mass_right + mass_left);
	const double a2	=  SQR(fRadius);
	const double r2	=  SQR(x_right) + SQR(y_right);
	const double R2	=  SQR(x_left) + SQR(y_left);
	if (r2 <= a2) throw SolveIt_ERROR(L"r2 <= fRadius^2");
	if (R2 <= a2) throw SolveIt_ERROR(L"R2 <= fRadius^2");
	const double ar	=  sqrt(r2 - a2);
	const double aR	=  sqrt(R2 - a2);
	const double w	=  x_right*Fx_right + y_right*Fy_right;
	const double t	=  x_right*Fy_right - y_right*Fx_right;
	const double W	=  x_left*Fx_left + y_left*Fy_left;
	const double T	=  x_left*Fy_left - y_left*Fx_left;
	const double f	=  (mass_left*R2*(ar*w-fRadius*t) + mass_right*r2*(aR*W+fRadius*T))/(mass_right*mass_left*r2*R2);


	const double s	=  x_right*Vx_right + y_right*Vy_right;
	const double l	=  x_right*Vy_right - y_right*Vx_right;
	const double S	=  x_left*Vx_left + y_left*Vy_left;
	const double L	=  x_left*Vy_left - y_left*Vx_left;

	const double h	=  SQR(l)/(r2*ar)+SQR(L)/(R2*aR)+2*fRadius*(s*l/SQR(r2)-S*L/SQR(R2))
		+a2*((s+l)*(s-l)/(ar*SQR(r2)) + (S+L)*(S-L)/(aR*SQR(R2)));

	lambda	= -mu*(f+h);
	Jx=( fRadius*y_right	+x_right*ar)/r2;
	Jy=(-fRadius*x_right	+y_right*ar)/r2;
	JX=(-fRadius*y_left		+x_left*aR)/R2;
	JY=( fRadius*x_left		+y_left*aR)/R2;
}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CConstraintManager::ImposePulleyConstraints(double t, bool bDuringCollision) {
	if (bDuringCollision) return;//not yet implemented 
	if (m_vecPulleyConstraints.empty()) return;
//	if (!Active()) return;
	if (m_vecPulleyConstraints.size() == 0) return;

	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	double& g	= fieldsManager->m_ExternalGravitationalField.y;
	vector< CPulleyConstraint*>::iterator i;
//	vector<KeyToConstraint_List*>::iterator i;
	for (i = m_vecPulleyConstraints.begin(); i != m_vecPulleyConstraints.end(); ++i)
	{
	//	KeyToConstraint_List& k = *(*i);
	//	for (row = k.begin(); row != k.end(); ++row)
		{
			CPulleyConstraint* ppc = *i;//dynamic_cast<CPulleyConstraint*>(row->second);
			if (ppc)
			{
try {

			double &R=ppc->m_pulley->m_fRadius;
			Vector3D w;
			ppc->m_pulley->get_AngularVelocity(w);
			Mat3 ii;
			ppc->m_pulley->get_InverseInertiaTensor(ii);
			double InertiaInv = ii.zz;

			if (0==ppc->m_rightBody) {
					if (ppc->m_leftBody->m_Tension && ppc->m_leftBody->m_fMassInverse>0.0)
					{
						Vector3D tension = ppc->m_leftBody->Tension(g, R, InertiaInv, w.z )/ppc->m_leftBody->m_fMassInverse;
						ppc->m_leftBody->m_vecForce += tension;
						if (ppc->m_pulley->m_fMassInverse != 0.0) {
							double torque = R*tension.norm();
							ppc->m_pulley->m_vecTorque.z += torque;
						}
						if (1)
						{
//							HRESULT hr = pThis->m_pIUI_ClassicalPhysics->get_Window((long *)&hwnd);
							TCHAR* sz = new TCHAR[128];
							ZeroMemory(sz, 128*sizeof TCHAR);
							_stprintf_s(sz, 128, L"tension = < %lg, %lg, %lg >", tension.x, tension.y, tension.z);

/*
	int cb = _tcslen(sz) + 1;
	HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
	std::wstring    lpszNew = (std::wstring )GlobalLock(hgCF_TEXTData);
	if (lpszNew != NULL) _tcscpy(lpszNew, sz);
	GlobalUnlock(hgCF_TEXTData);
*/
//							::PostMessage(hwnd, WM_TASK_STATUS,(WPARAM) 0, (LPARAM) (LPCTSTR) sz);
						}
					}
				}
				else if (0==ppc->m_leftBody) {
					if (ppc->m_rightBody->m_Tension && ppc->m_rightBody->m_fMassInverse>0.0)
					{
						Vector3D tension = ppc->m_rightBody->Tension(g, R, InertiaInv, w.z )/ppc->m_rightBody->m_fMassInverse;
						ppc->m_rightBody->m_vecForce += tension;
						if (ppc->m_pulley->m_fMassInverse != 0.0) {
							double torque = R*tension.norm();
							ppc->m_pulley->m_vecTorque.z -= torque;
						}
						if (1)
						{
//							HRESULT hr = pThis->m_pIUI_ClassicalPhysics->get_Window((long *)&hwnd);
							TCHAR* sz = new TCHAR[128];
							ZeroMemory(sz, 128*sizeof TCHAR);
							/*ostrstream*/ std::wstringstream oss(sz, sizeof(sz)/sizeof(sz[0]));
							oss << "tension = " << tension << ends;
					//		::PostMessage(hwnd, WM_TASK_STATUS,(WPARAM) 0, (LPARAM) (LPCTSTR) sz);
						}
					}
				}
				else
				{
					double &x1=ppc->m_leftBody->x.x,	&y1=ppc->m_leftBody->x.y;
					double &x2=ppc->m_rightBody->x.x,	&y2=ppc->m_rightBody->x.y;
					double m1 = 1.0/ppc->m_leftBody->m_fMassInverse;
					double m2 = 1.0/ppc->m_rightBody->m_fMassInverse;

			//	Vector3D w1 = ppc->m_leftBody->GetAngularVelocity();
			//	Vector3D w2 = ppc->m_rightBody->GetAngularVelocity();

					Vector3D v1;
					ppc->m_leftBody->get_Velocity(v1);
					Vector3D v2;
					ppc->m_rightBody->get_Velocity(v2);
					double& v1x=v1.x, &v1y=v1.y;
					double& v2x=v2.x, &v2y=v2.y;

///////////////////////////////////////////////////////////////////////////////
#if 0
	try
	{
					double Jx=0,  Jy=0,  JX=0,  JY=0,  lambda=0;
					Vector3D& f1=ppc->m_leftBody->m_vecForce;
					Vector3D& f2=ppc->m_rightBody->m_vecForce;


					PulleyConstraintForce(R,
											   x2, y2,
											   v2x, v2y,
											   f2.x, f2.y,
											   x1, y1,
											   v1x, v1y,
											   f1.x, f1.y,
											   m2, m1,
											   Jx, Jy, JX, JY, lambda
											   );

					ppc->m_leftBody->m_vecForce += lambda*Vector3D(Jx, Jy, 0);
					ppc->m_rightBody->m_vecForce += lambda*Vector3D(JX, JY, 0);
					if (ppc->m_pulley->m_fMassInverse != 0.0)
					{
				//		double torque = R*lambda*Sqrt2;
				//		ppc->m_pulley->m_vecTorque.z -= torque;
					}

	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)
	{
		AfxMessageBox(L"CConstraintManager::PulleyConstraintForce");
	}


#else
///////////////////////////////////////////////////////////////////////////////
					Point3D& p1 = ppc->m_leftBody->m_PulleyContactPoint;
					Point3D& p2 = ppc->m_rightBody->m_PulleyContactPoint;


					double t1 = (*ppc->m_leftBody->m_Tension2)(	R, g, InertiaInv, w.z,
															m1, x1, y1, v1x, v1y,
															m2, x2, y2, v2x, v2y
															);
					double t2 = (*ppc->m_rightBody->m_Tension2)(	R, g, InertiaInv, w.z,
															m1, x1, y1, v1x, v1y,
															m2, x2, y2, v2x, v2y
															);

					double Cos1 = (*ppc->m_leftBody->m_Cos)(x1, y1, R);
					double Sin1 = (*ppc->m_leftBody->m_Sin)(x1, y1, R);
					double dx1 = R*Cos1;// - X;
					double dy1 = R*Sin1;// - Y;
					double Cos2 = (*ppc->m_rightBody->m_Cos)(x2, y2, R);
					double Sin2 = (*ppc->m_rightBody->m_Sin)(x2, y2, R);
					double dx2 = R*Cos2;// - X;
					double dy2 = R*Sin2;// - Y;
					p1 = Point3D(dx1,dy1,0);
					p2 = Point3D(dx2,dy2,0);
					dx1 -= x1;
					dy1 -= y1;
					dx2 -= x2;
					dy2 -= y2;
					double L1 = sqrt(dx1*dx1 + dy1*dy1);
					double L2 = sqrt(dx2*dx2 + dy2*dy2);
					Vector3D T1(dx1/L1, dy1/L1, 0);
					Vector3D T2(dx2/L2, dy2/L2, 0);
					T1 *= m1*t1;
					T2 *= m2*t2;

					ppc->m_leftBody->m_vecForce += T1;
					ppc->m_rightBody->m_vecForce += T2;
			//	double torque1 = Vector3D::K*(p1^T1);
			//	double torque2 = Vector3D::K*(p2^T2);
					double torque1 = R*T1.norm();
					double torque2 = R*T2.norm();
					if (ppc->m_pulley->m_fMassInverse != 0.0) ppc->m_pulley->m_vecTorque.z += (torque1 - torque2);

#endif
				}
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
	}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch(...)// there was an error
	{
		return;
	}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CConstraintManager::InitializePulleyConstraints(CPulleyConstraint *const c, bool bDuringCollision)
{
	CRigidBody* const &body1 = c->m_leftBody;
	CRigidBody* const &body2 = c->m_rightBody;
	if (body1==NULL) throw SolveIt_ERROR(L"InitializePulleyConstraints: body1==NULL");
	if (body2==NULL) throw SolveIt_ERROR(L"InitializePulleyConstraints: body2==NULL");

	double CosTheta1 = 0, SinTheta1 = 0, CosPsi1 = 0, SinPsi1 = 0, Theta1 = 0 , Psi1 = 0;
	double CosTheta2 = 0, SinTheta2 = 0, CosPsi2 = 0, SinPsi2 = 0, Theta2 = 0 , Psi2 = 0;

	body1->FirstPossibleAngle( CosTheta1, SinTheta1, Theta1);
	body1->SecondPossibleAngle( CosPsi1, SinPsi1, Psi1);
	body2->FirstPossibleAngle( CosTheta2, SinTheta2, Theta2);
	body2->SecondPossibleAngle( CosPsi2, SinPsi2, Psi2);

//int j=body1->m_nConnectedInQuadrant;

	CRigidBody::InitialAngleInQuadrant val1;
	CRigidBody::InitialAngleInQuadrant val2;
	body1->get_InitialAngleInQuadrant(val1);
	body2->get_InitialAngleInQuadrant(val2);

	switch (val1)//body1->m_nConnectedInQuadrant)
	{
	case CRigidBody::___Quadrant_Two:
		switch (val2)//body2->m_nConnectedInQuadrant)
		{
			case CRigidBody::___Quadrant_One:
				if (SinTheta1 >= 0.0 && CosTheta1 <= 0.0 && SinTheta2 >= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft11,
					body2->m_Tension2 = TensionsRight11;
				if (SinTheta1 >= 0.0 && CosTheta1 <= 0.0 && SinPsi2 >= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft12,
					body2->m_Tension2 = TensionsRight12;
				if (SinPsi1 >= 0.0 && CosPsi1 <= 0.0 && SinTheta2 >= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft21,
					body2->m_Tension2 = TensionsRight21;
				if (SinPsi1 >= 0.0 && CosPsi1 <= 0.0 && SinPsi2 >= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft22,
					body2->m_Tension2 = TensionsRight22;
				break;
			case CRigidBody::___Quadrant_Four:
				if (SinTheta1 >= 0.0 && CosTheta1 <= 0.0 && SinTheta2 <= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft11,
					body2->m_Tension2 = TensionsRight11;
				if (SinTheta1 >= 0.0 && CosTheta1 <= 0.0 && SinPsi2 <= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft12,
					body2->m_Tension2 = TensionsRight12;
				if (SinPsi1 >= 0.0 && CosPsi1 <= 0.0 && SinTheta2 <= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft21,
					body2->m_Tension2 = TensionsRight21;
				if (SinPsi1 >= 0.0 && CosPsi1 <= 0.0 && SinPsi2 <= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft22,
					body2->m_Tension2 = TensionsRight22;
				break;
			case CRigidBody::___Quadrant_Two:
			case CRigidBody::___Quadrant_Three:
			default:
				throw SolveIt_ERROR(L"InitializePulleyConstraints: Wrong Quadrant");
				return;
		}
		break;
	case CRigidBody::___Quadrant_Three:
		switch (val2)//body2->m_nConnectedInQuadrant)
		{
			case CRigidBody::___Quadrant_One:
				if (SinTheta1 <= 0.0 && CosTheta1 <= 0.0 && SinTheta2 >= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft11,
					body2->m_Tension2 = TensionsRight11;
				if (SinTheta1 <= 0.0 && CosTheta1 <= 0.0 && SinPsi2 >= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft12,
					body2->m_Tension2 = TensionsRight12;
				if (SinPsi1 <= 0.0 && CosPsi1 <= 0.0 && SinTheta2 >= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft21,
					body2->m_Tension2 = TensionsRight21;
				if (SinPsi1 <= 0.0 && CosPsi1 <= 0.0 && SinPsi2 >= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft22,
					body2->m_Tension2 = TensionsRight22;
				break;
			case CRigidBody::___Quadrant_Four:
				if (SinTheta1 <= 0.0 && CosTheta1 <= 0.0 && SinTheta2 <= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft11,
					body2->m_Tension2 = TensionsRight11;
				if (SinTheta1 <= 0.0 && CosTheta1 <= 0.0 && SinPsi2 <= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft12,
					body2->m_Tension2 = TensionsRight12;
				if (SinPsi1 <= 0.0 && CosPsi1 <= 0.0 && SinTheta2 <= 0.0 && CosTheta2 >= 0.0)
					body1->m_Tension2 = TensionsLeft21,
					body2->m_Tension2 = TensionsRight21;
				if (SinPsi1 <= 0.0 && CosPsi1 <= 0.0 && SinPsi2 <= 0.0 && CosPsi2 >= 0.0)
					body1->m_Tension2 = TensionsLeft22,
					body2->m_Tension2 = TensionsRight22;
				break;
			case CRigidBody::___Quadrant_Two:
			case CRigidBody::___Quadrant_Three:
			default:
				throw SolveIt_ERROR(L"InitializePulleyConstraints: Wrong Quadrant");
				return;
		}
		break;
	case CRigidBody::___Quadrant_One:
		throw SolveIt_ERROR(L"InitializePulleyConstraints: ___Quadrant_One is Wrong Quadrant");
	case CRigidBody::___Quadrant_Four:
		throw SolveIt_ERROR(L"InitializePulleyConstraints: ___Quadrant_Four is Wrong Quadrant");
	default:
		throw SolveIt_ERROR(L"InitializePulleyConstraints: Unknown Quadrant");
		return;
	}

	const double &R=c->m_pulley->m_fRadius;
	body1->SetPulleyContactPoint(R);
	body2->SetPulleyContactPoint(R);
}

///////////////////////////////////////////////////////////////////////////////


#undef LENGTH
//#undef SQR
#undef Power
#undef Sqrt


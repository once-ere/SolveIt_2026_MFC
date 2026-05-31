// RigidLink.cpp : Implementation of CRigidBody

#include "stdafx.h"
#include "SolveIt.h"
#include "RigidBody.h"

#include "System.h"
#include "RigidBodyManager.h"
#include "../SolveIt.Lib/lib/cvode/llnltyps.h"	/* definitions of CVODE::real, CVODE::integer, bool, TRUE, FALSE */
#include "../SolveIt.Lib/lib/cvode/cvode.h"		 /* main CVODE header file													*/
#include "../SolveIt.Lib/lib/cvode/iterativ.h"	/* contains the enum for types of preconditioning	*/
#include "../SolveIt.Lib/lib/cvode/cvspgmr.h"	 /* use CVSPGMR linear solver each internal step		*/
#include "../SolveIt.Lib/lib/cvode/dense.h"		 /* use generic DENSE solver for preconditioning		*/
#include "../SolveIt.Lib/lib/cvode/cvdense.h"  /* use CVDENSE linear solver each internal step */
#include "Matherror.h"
#include "Types.h"
#include "GLView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
	int ToIntSize(size_t value)
	{
		return static_cast<int>(value);
	}

	long ToLongSize(size_t value)
	{
		return static_cast<long>(value);
	}
}


///////////////////////////////////////////////////////////////////////////////
//vector<double>& _state,
//						map<long, double>& Joint1DOFAccelerations,
///////////////////////////////////////////////////////////////////////////////
/*
*************************WARNING*************************
*************************WARNING*************************
	A^B+C == A^(B+C)
*************************WARNING*************************
*************************WARNING*************************
const PAIR_Vector3D PAIR_Vector3D::ZERO(ZERO_3D,ZERO_3D);
///////////////////////////////////////////////////////////////////////////////
//const Mat6 Mat6::G(Mat3::ID, Mat3::ID);
const Mat6 Mat6::ZERO;
const Mat6 Mat6::ID(Mat3::ID, Mat3::ZERO, Mat3::ZERO, Mat3::ID);
const Mat6 Mat6::G(Mat3::ZERO, Mat3::ID, Mat3::ID, Mat3::ZERO);
*/
///////////////////////////////////////////////////////////////////////////////
// e1' = R e1
// e2' = R e2
// e3' = R e3
// R = exp[Phi rotAxis_j Sj]
// R = e1' @ ~e1 + e2' @ ~e2 + e3' @ ~e3; @ = tensor product, ~ = tilde =  transpose
//tr[SjR] = -2 sin(Phi) rotAxis_j = - [Cross(e1,e1')+Cross(e2,e2')+Cross(e3,e3')]_j
//trace(R) = 1 + 2 cosPhi = e1*e1' + e2*e2' + e3*e3'
/*
	double trace=X*x+Y*y+Z*z;
	double cosPhi = (trace-1.)/2.;
	double Phi = acos(cosPhi);
	Vector3D rotAxis(Vector3D::K);
	double sinPhi = sin(Phi);
	if (fabs(sinPhi)>0.1e-63) {
		rotAxis =  (x^X) + ((y^Y)+(z^Z));
		rotAxis /= 2*sinPhi;
	}
//	R.set( z, displacement.angle);
	((Mat3::AfRRAY3 &) R.m[0]) = x.v;
	for (int j=0;j<3;++j) {
		R.m[0][j] =x[j];
		R.m[1][j] =y[j];
		R.m[2][j] =z[j];
	}
*/
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::connect(
			const Point3D &attachMeToInboundJointHere,//present coordinates of point of attachment
			Link* parentLink,
			const Point3D &inboundJointLocation,
			Joint1DOF::JointType jointType,
			const Vector3D &jointAxis
			)
{
	if (jointType == Joint1DOF::SPRING)
	{
		m_inboundJoint.jointType = jointType;
		m_parentLink = parentLink;

		PAIR_springConstant_springLength springConstant_springLength(jointAxis.x, jointAxis.y);
		m_ListOf_springConstants_springLengths.push_back(springConstant_springLength);

		PairPoint3D ptpt(attachMeToInboundJointHere, inboundJointLocation);
		m_PairPoint3DVector.push_back(ptpt);

		m_bodiesConnectedTo.push_back(m_parentLink);

		if (m_parentLink) {
			m_parentLink->m_bodiesConnectedTo.push_back(this);
			m_parentLink->m_ListOf_springConstants_springLengths.push_back(springConstant_springLength);
			PairPoint3D ptpt(inboundJointLocation, attachMeToInboundJointHere);
			m_parentLink->m_PairPoint3DVector.push_back(ptpt);
		}
		return;
	}
	else
	{
		set_DisableSelect(true);
	}
	m_inboundJoint.outboundLink = this;
	m_parentLink = parentLink;


	if (m_parentLink) {
		m_parentLink->outbounds.push_back(this);
		m_parentLink->set_DisableSelect(true);
	}
	else {
		m_baseJoint = &m_inboundJoint;
	}
	m_inboundJoint.jointType = jointType;
	m_Kd = m_Ks = m_Ls = 0;

	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	svIdx = ToIntSize(rigidBodyManager->RigidLinkJointStates.size());// initialize svIdx, id of this link flexible joint
	switch(jointType) {//m_inboundJoint.jointType
		case Joint1DOF::REV:
		case Joint1DOF::PRI:
			rigidBodyManager->RigidLinkJointStates.insert(rigidBodyManager->RigidLinkJointStates.end(), 2, 0.0);
			rigidBodyManager->JointForcesTorques.insert(rigidBodyManager->JointForcesTorques.end(), 2, 0.0);
			rigidBodyManager->RigidLinkJointStates[svIdx]		= 0;
			rigidBodyManager->RigidLinkJointStates[svIdx+1]	= 0;
		break;
		case Joint1DOF::FLT:
			rigidBodyManager->RigidLinkJointStates.insert(rigidBodyManager->RigidLinkJointStates.end(), sizeof(RBConfig)/sizeof(double), 0.0);
			rigidBodyManager->JointForcesTorques.insert(rigidBodyManager->JointForcesTorques.end(), sizeof(RBConfig)/sizeof(double), 0.0);
			((RBConfig *) &*(rigidBodyManager->RigidLinkJointStates.end() - 13))->Tbi = Se3::ID;
		break;
	}
	rigidBodyManager->RigidLinkJointStatesInitial = 	rigidBodyManager->RigidLinkJointStates;

	rigidBodyManager->Joint1DOFAccelerations[svIdx]				= 0;
	rigidBodyManager->partialJoint1DOFAccelerations[svIdx]		= 0;
	rigidBodyManager->partialVect6[svIdx]							= PAIR_Vector3D::ZERO;
	rigidBodyManager->map_isIs[svIdx]								= 0;

		const Point3D &outLoc=attachMeToInboundJointHere;
		const Point3D &inLoc=inboundJointLocation;

/*
Xij.R = Mat3::ID;
Xij.d = -inLoc;

Xjo.R = Mat3::ID;
Xjo.d = outLoc;
*/

	Vector3D m_ThisLinkCenterOfMassWithRespectToJointOrigin = -outLoc;
	m_ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass = inLoc + m_ThisLinkCenterOfMassWithRespectToJointOrigin;
parentLinkToThisLink = new ThisLinkCenterOfMassWithRespectToParentLinkCenterOfMass(inLoc, m_ThisLinkCenterOfMassWithRespectToJointOrigin);

		
		m_jointAxis = jointAxis;
		m_jointAxis.normalize();

	switch(jointType) {
		case Joint1DOF::PRI:
			m_inboundJoint.S = Vect6(ZERO_3D, m_jointAxis);
		break;
		case Joint1DOF::REV:
	{
			m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin= m_jointAxis  .vectorProd (m_ThisLinkCenterOfMassWithRespectToJointOrigin) ;
			m_ThisLinkCenterOfMassWithRespectToJointOriginPerpJointAxis	= m_jointAxis  .vectorProd (m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin) ;
			m_inboundJoint.S											= Vect6(m_jointAxis, m_jointAxisCROSSThisLinkCenterOfMassWithRespectToJointOrigin);
	}
		break;
	default:
		break;
	}
	
//	compXf(&rigidBodyManager->RigidLinkJointStates.front());

	rigidBodyManager->compXf(&rigidBodyManager->RigidLinkJointStates.front());
//	CRigidBody::m_baseJoint->outboundLink->updatePoses();
	rigidBodyManager->updatePoses();
	set_InitialState(*this);

	CSystem* m_system = theApp.GetSystem();
	m_system->UpdateTree();
	m_system->DisplayData();
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return;
//	pGLView->ResetView();
	pGLView->Invalidate();
}
///////////////////////////////////////////////////////////////////////////////
void CRigidBodyManager::SetMultibodyState(const vector<double>& state)//const double state[])
{
	if (CRigidBody::m_LastLink==0) return;
	if (CRigidBody::m_baseJoint==0) return;
	if (CRigidBody::m_baseJoint->outboundLink==0) return;
/*	
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	int i=0;
	while (pb)
	{
		double& x = RigidLinkJointStates[pb->svIdx];
		double& v = RigidLinkJointStates[pb->svIdx+1];
		double& a = Joint1DOFAccelerations[pb->svIdx];

		x	= state[i++];
		v	= state[i++];

		if (pb->outbounds.size()>0)	pb = pb->outbounds[0];
		else pb = 0;
	}
*/
	RigidLinkJointStates = state;
//	ComputeLinkFramesAndComputeVelocities(0, &state.front(), true);
//	RigidBodyList::iterator it = begin();
//	CRigidBody* pbody = *it;
//	ASSERT(pbody == CRigidBody::m_baseJoint->outboundLink);
//	if (pbody != CRigidBody::m_baseJoint->outboundLink) return;
//	CSystem* m_system = theApp.GetSystem();
	CRigidBody::m_baseJoint->outboundLink->updatePoses();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef const void * OdePointer;
typedef void (*deriv_func)(double t,						 // time					 
				 const double y[],		 // state vector
				 double ydot[],			// derivative vector
				 void * cd);		 // client data
//////////////////////////////////////////////////////////////////////
// OdeSolver : class for solving Ordinary Differential Equations
//////////////////////////////////////////////////////////////////////
class OdeSolver {
public:
	OdeSolver();
	~OdeSolver();
// state integration methods
	void Euler(double y0[], double yend[], int len,
			 double t0, double t1,
			 deriv_func dydt, void * clientData);

	void RungeKutta4(double y0[], double yend[], int len,
			 double t0, double t1,
			 deriv_func dydt, void * clientData);
private:
// methods
	void _ReAllocate(int newSize);
 //data
	int		_size;	//the size of the state variable array allocated

	double* _ydot;	//the derivatives
	double* _yint;

	double* _k1;		//the iterations of Runge-Kutta
	double* _k2;
	double* _k3;
	double* _k4;

};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// default size for state array
const int DEFAULT_SIZE = 200;
// useful fractions
const double ONE_HALF	= (1.0/2.0);
const double ONE_THIRD = (1.0/3.0);
const double ONE_SIXTH = (1.0/6.0);
//////////////////////////////////////////////////////////////////////////
OdeSolver::OdeSolver() :
	_size(0),_ydot(0),_yint(0),	_k1(0),	_k2(0),	_k3(0),	_k4(0)
{
	_ReAllocate(DEFAULT_SIZE);
}
//////////////////////////////////////////////////////////////////////////
OdeSolver::~OdeSolver()
{
	if (_ydot) delete [] _ydot;
	if (_yint) delete [] _yint;
	if (_k1)	 delete [] _k1;
	if (_k2)	 delete [] _k2;
	if (_k3)	 delete [] _k3;
	if (_k4)	 delete [] _k4;
}
//////////////////////////////////////////////////////////////////////////
// Euler : perform one step of Euler's method for solving ODEs
//////////////////////////////////////////////////////////////////////////
void OdeSolver::Euler(double y0[], double yend[], int len,
					double t0, double t1,
					deriv_func dydt, void * clientData)

{
// reallocate if necessary
	if (len > _size)_ReAllocate(len);
	double h = t1 - t0;
	dydt(t0, y0, _ydot, clientData);
	for (int i = 0; i < len; i++) {
		yend[i] = y0[i] + h * _ydot[i];
	}
}
//////////////////////////////////////////////////////////////////////////
// RungeKutta : perform one step of the Fourth-order Runge-Kutta 
//	 method for solving ODEs.
//////////////////////////////////////////////////////////////////////////
void OdeSolver::RungeKutta4(double y0[], double yend[], int len,
					double t0, double t1,
					deriv_func dydt, void * clientData)
{
// reallocate if necessary
	if (len > _size)_ReAllocate(len);

	double h = t1 - t0;
	double h_2 = h * 0.5;

	dydt(t0, y0, _ydot, clientData);
	for (int i = 0; i < len; i++) {		// first iteration
		_k1[i] = h*_ydot[i];
	}

	for (auto i = 0; i < len; i++) {
		_yint[i] = y0[i] + _k1[i] * ONE_HALF;
	}

	dydt(t0 + h_2, _yint, _ydot, clientData);
	for (auto i = 0; i < len; i++) {		// second iteration
		_k2[i] = h*_ydot[i];
	}

	for (auto i = 0; i < len; i++) {
		_yint[i] = y0[i] + _k2[i] * ONE_HALF;
	}

	dydt(t0 + h_2, _yint, _ydot, clientData);
	for (auto i = 0; i < len; i++) {		// third iteration
		_k3[i] = h*_ydot[i];
	}

	for (auto i = 0; i < len; i++) {
		_yint[i] = y0[i] + _k3[i];
	}

	dydt(t0 + h, _yint, _ydot, clientData);
	for (auto i = 0; i < len; i++) {		// fourth iteration
		_k4[i] = h*_ydot[i];
	}

	for (auto i = 0; i < len; i++) {
		yend[i] = y0[i] + (_k1[i] * ONE_SIXTH) + (_k2[i] * ONE_THIRD)
										+ (_k3[i] * ONE_THIRD) + (_k4[i] * ONE_SIXTH);
	}
}
//////////////////////////////////////////////////////////////////////////
// _ReAllocate : reallocate storage for intermediate values
//////////////////////////////////////////////////////////////////////////
void OdeSolver::_ReAllocate(int newSize)
{
	if (_ydot) delete [] _ydot;
	if (_yint) delete [] _yint;
	if (_k1)	 delete [] _k1;
	if (_k2)	 delete [] _k2;
	if (_k3)	 delete [] _k3;
	if (_k4)	 delete [] _k4;

	_size = newSize;

	_ydot = new double[_size];
	_yint = new double[_size];
	_k1 = new double[_size];
	_k2 = new double[_size];
	_k3 = new double[_size];
	_k4 = new double[_size];
}

struct _OdeSolver {
	OdeSolver* _odeint;
	_OdeSolver() : _odeint(0) {
		_odeint = new OdeSolver();
	}
	~_OdeSolver() {
		if (_odeint) delete _odeint;
	}
} g__OdeSolver;
///////////////////////////////////////////////////////////////////////////////
void MultibodyDerivative(
							  double t,
							  const double stateVector[],
							  double dx[],
							  void *f_data);
///////////////////////////////////////////////////////////////////////////////
#define RTOL  1e-5
#define ATOL  1e-5
///////////////////////////////////////////////////////////////////////////////

#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 


//static wchar_t workingDirectory[] = L"\\work\\";
//static std::wstring bstrWorkingDirectory = L"\\work\\";
//extern wstring		g_szWorkDir;		//no terminal '\'
//extern wstring		g_szWorkDir2;		//with terminal '\'
//extern wstring		g_szWorkDirFileName;
//extern "C" wchar_t		DATA_PASS_mp_WORK_DIR[_MAX_PATH];
///////////////////////////////////////////////////////////////////////////////
//wstring		g_szWorkDir;		//no terminal '\'
//wstring		g_szWorkDir2;		//with terminal '\'
//wstring		g_szWorkDirFileName;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CRigidBodyManager::StepMultibodyState()
{
	ASSERT(0);
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	long flag = CVODE::ERR_FAILURE;
	if (CRigidBody::m_LastLink==0) return -5;
	if (CRigidBody::m_baseJoint==0) return -4;
	double t=0, tout=0.02;
//	std::wstring bbuf(g_szWorkDir.c_str());
//	bbuf += L"\\cvode_err.txt";
//	freopen( bbuf, "w", stdout );
try
{
	vector<double> state(RigidLinkJointStates);
#if 0
	g__OdeSolver._odeint->RungeKutta4(&RigidLinkJointStates[0], &state[0], RigidLinkJointStates.size(), t, tout, MultibodyDerivative, this);
//	SetMultibodyState(state);//&state.front()
	double vv[12] = {state[0],state[1],state[2],state[3],
		state[4],state[5],state[6],state[7],
		state[8],state[9],state[10],state[11]
	};
	RigidLinkJointStates = state;
//	RigidBodyList::iterator it = begin();
//	CRigidBody* pbody = *it;//
//	ASSERT(pbody == CRigidBody::m_baseJoint->outboundLink);//
//	if (pbody != CRigidBody::m_baseJoint->outboundLink) return -3;//
	CRigidBody::m_baseJoint->outboundLink->updatePoses();//
#else

	long N = ToLongSize(state.size());
	CVODE::N_Vector x = CVODE::N_VNew(N, 0);
	if (x == 0) return flag;
	if (x->data == 0) return flag;
	x->length = N;
	CopyMemory(x->data, &state.front(), N * sizeof(double) );
	double abstol=1*ATOL, reltol=1*RTOL;
	void *cvode_mem = 
			CVODE::CVodeMalloc(	x->length, CVodeMultibodyDerivative, t, x, 
								CVODE::BDF, CVODE::NEWTON, CVODE::SS,
								&reltol, &abstol, (void*) this, 0, FALSE, iopt, ropt, 0);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, 0, 0);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(&state.front(), x->data, N * sizeof(double) );
		SetMultibodyState(state);//&state.front()
	}
	else
	{
		TimedMessageBox(0, GetWC(CVOdeErrors[-flag]), L"CRigidBodyManager::StepMultibodyState", MB_OK, 1000);
		throw SolveIt_ERROR(L"-1");
	}

	CVODE::N_VFree(x);
#endif
	CSystem* m_system = theApp.GetSystem();
	SetTime(m_system->m__t+1);
	m_system->UpdateTree();
	m_system->DisplayData();
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return flag;
//	pGLView->ResetView();
	pGLView->Invalidate();
}
catch(...)
{
	CSystem* m_system = theApp.GetSystem();
	m_system->Stop();
	SetToPreviousState();
}
	return flag;
}
#undef RTOL
#undef ATOL
///////////////////////////////////////////////////////////////////////////////
//	mb->updatePoses();
//	for (int i = 0; i < mb->getNumLinks(); i++)		mb->getLink(i).getGlobalPose().toSoTransform(th->sceneGraphXforms[i]);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
void MultibodyDerivative(
							  double t,
							  const double stateVector[],
							  double dx[],
							  void *f_data)
{
	if (CRigidBody::m_baseJoint==0) return;
//	CRigidBodyManager* pRigidBodyManager = (CRigidBodyManager * ) f_data;
	CRigidBodyManager* pRigidBodyManager = reinterpret_cast<CRigidBodyManager *>(f_data);
	ASSERT(pRigidBodyManager != 0);
	if (pRigidBodyManager == 0) {
		TimedMessageBox(0,CVOdeErrors[2], "MultibodyDerivative:pRigidBodyManager == 0", MB_OK, 1000);
		return;
	}

	SpatialVect dummyVect;
	SpatialMat dummyMat;

//	RigidBodyList::iterator it = pRigidBodyManager->begin();
//	CRigidBody* pbody = dynamic_cast<CRigidBody*>(*it);
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pbody = CRigidBody::m_baseJoint->outboundLink;
	ASSERT(pbody != 0);
	if (pbody == 0) {
		TimedMessageBox(0,CVOdeErrors[2], "MultibodyDerivative:pbody == 0", MB_OK, 1000);
		return;
	}

	pbody->featherstonePass1(stateVector, MatX::ID, ZERO_3D, ZERO_3D, dummyMat, dummyVect);

	pbody->featherstonePass2(stateVector, dx, SpatialVect::ZERO);
}
#endif
///////////////////////////////////////////////////////////////////////////////
void SolveIt::CVodeMultibodyDerivative(CVODE::integer N,
							  CVODE::real t,
							  CVODE::N_Vector stateVector,
							  CVODE::N_Vector dx,
							  void *f_data)
{
	if (CRigidBody::m_baseJoint==0) return;
//	CRigidBodyManager* pRigidBodyManager = (CRigidBodyManager * ) f_data;
//	CRigidBodyManager* pRigidBodyManager = reinterpret_cast<CRigidBodyManager *>(f_data);
//	ASSERT(pRigidBodyManager != 0);
//	if (pRigidBodyManager == 0) return;
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();

	SpatialVect dummyVect;
	SpatialMat dummyMat;

//	RigidBodyList::iterator it = pRigidBodyManager->begin();
//	CRigidBody* pbody = dynamic_cast<CRigidBody*>(*it);
//	ASSERT(pbody != 0);
//	if (pbody == 0) return;
	if (CRigidBody::m_baseJoint==0) return;
	CRigidBody* pb = CRigidBody::m_baseJoint->outboundLink;
	if (pb==0) return;

//	pbody->featherstonePass1(stateVector->data, MatX::ID, ZERO_3D, ZERO_3D, dummyMat, dummyVect);
//	CRigidBody::m_baseJoint->outboundLink->Xbi = MatX::ID;
//	CRigidBody::m_baseJoint->outboundLink->m_linkV = ZERO_3D;
//	CRigidBody::m_baseJoint->outboundLink->m_linkW = ZERO_3D;

/*
*/
//	pRigidBodyManager->ComputeLinkIAndZs(stateVector->data);


#if 1
	rigidBodyManager->compXf(stateVector->data);
	rigidBodyManager->ComputeLinkAbsoluteVelocities(stateVector->data);
	rigidBodyManager->InitializeLinkIAndZs();
	rigidBodyManager->ForwardDynamics(stateVector->data, dummyMat, dummyVect);
#else
	pb->featherstonePass1(stateVector->data, dummyMat, dummyVect);
#endif
	pb->featherstonePass2(stateVector->data, dx->data, SpatialVect::ZERO);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
MatX jointFrame(const Point3D &translate, 
				const Vector3D &xIn, const Vector3D &zIn, 
				const Vector3D &xOut, const Vector3D &zOut
				)
{
	Vector3D x(xIn), z(zIn), X(xOut), Z(zOut);

	z.normalize();
	x -= (x*z) * z;
	x.normalize();

	Z.normalize();
	X -= (X*Z) * Z;
	X.normalize();

	Vector3D y(z .vectorProd (x)), Y(Z .vectorProd (X));

	Mat3 Xx(X,x);
	ASSERT(Xx*x==X);
	Mat3 Yy(Y,y);
	ASSERT(Yy*y==Y);
	Mat3 Zz(Z,z);
	ASSERT(Zz*z==Z);
	Mat3 R(Xx+(Yy+Zz));
	ASSERT(R*x==X);
	ASSERT(R*y==Y);
	ASSERT(R*z==Z);

//	if (bInvert) return MatX(R, R*translate);,bool bInvert
	return MatX(R, translate);
}
//xform: joint to outboard link coords
///////////////////////////////////////////////////////////////////////////////
MatX jointColFrame(const Point3D &translate, const Vector3D &axis_, const Vector3D &ref_)
{
	Vector3D z(axis_), x(ref_), y;
	z.normalize();

	x -= (x*z) * z;
	x.normalize();

	y = z  .vectorProd (x) ;

	Mat3 R;
	R.setXcol(x);
	R.setYcol(y);
	R.setZcol(z);

	return MatX(R, translate);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
MatX StandardFrame(const Point3D &translate, 
				const Vector3D &xIn, const Vector3D &zIn, 
				const Vector3D &xOut, const Vector3D &zOut
				)
{
	Vector3D x(xIn), z(zIn), X(xOut), Z(zOut);
	x.normalize();
	z.normalize();
	X.normalize();
	Z.normalize();
	Vector3D y(z .vectorProd (x)), Y(Z .vectorProd (X));

	Mat3 Xx(X,x);
	ASSERT(Xx*x==X);
	Mat3 Yy(Y,y);
	ASSERT(Yy*y==Y);
	Mat3 Zz(Z,z);
	ASSERT(Zz*z==Z);
	Mat3 R(Xx+(Yy+Zz));
	ASSERT(R*x==X);
	ASSERT(R*y==Y);
	ASSERT(R*z==Z);

	return MatX(R, (R)*(-translate));
}
///////////////////////////////////////////////////////////////////////////////

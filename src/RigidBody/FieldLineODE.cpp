// FieldLineODE.cpp: implementation of the CFieldLineODEThread class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolveIt.h"

#include "FieldLineODE.h"
#include "FieldsManager.h"
#include "RigidBodyManager.h"
#include "System.h"
#include "MainFrm.h"
#include "GLView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//	PostThreadMessage(WM_USER_START_PASS, 0, 0); 

#define IMPLEMENT_USER_THREAD(class_name) \
IMPLEMENT_DYNCREATE(class_name, CWinThread) \
BOOL class_name::InitInstance() \
{ \
	return TRUE; \
} \
int class_name::ExitInstance() \
{ \
	return CWinThread::ExitInstance(); \
} \
BEGIN_MESSAGE_MAP(class_name, CWinThread) \
ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep) \
ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone) \
ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt) \
END_MESSAGE_MAP() \
void class_name::OnStepDone(WPARAM wParam, LPARAM lParam) \
{ \
	m_bStepCompleted=true; \
	Fire_ViewChange(); \
} \
void class_name::OnStep(WPARAM wParam, LPARAM lParam) \
{ \
	m_bStepCompleted=false; \
	MakeTimeStep(); \
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); \
} \
void class_name::OnHalt(WPARAM wParam, LPARAM lParam) \
{ \
	PostQuitMessage(0); \
}

//	PostThreadMessage(WM_USER_START_PASS, 0, 0); \
///////////////////////////////////////////////////////////////////////////////
//IMPLEMENT_USER_THREAD(FieldLineElectric2D)
IMPLEMENT_USER_THREAD(FieldLineElectric3D)
IMPLEMENT_USER_THREAD(FieldLineMagnetostatic)
//IMPLEMENT_USER_THREAD(FieldLineMagnetic_current_loop)
//IMPLEMENT_USER_THREAD(FieldLineMagnetic)
IMPLEMENT_USER_THREAD(FieldLineGravitational)
IMPLEMENT_USER_THREAD(FieldLine_Equipotential)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineGravitational::MakeTimeStep(void)
{
	CSystem* m_system	= theApp.GetSystem();
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	try
	{
	fieldsManager->m_gravitational_currentFieldLine.clear();
// set in CGLView::OnLButtonDown:	fieldsManager->m_selectedPointForMagneticField
	const long num_points_to_compute = 128;
	long num_points = num_points_to_compute;
	CGLView* pGLView = theApp.GetGLView();
	while (num_points-- > 0)
	{
		long flag = CVodeStep();
		if (flag == 0)//SUCCESS
		{
fieldsManager->m_gravitational_currentFieldLine.push_back( fieldsManager->m_gravitational_currentFieldLinePoint );
		}
	}
	vector_of_Vector3D* currentFieldLine = new vector_of_Vector3D();
	currentFieldLine->clear();
	vector_of_Vector3D::const_iterator i_data;
	for (i_data=fieldsManager->m_gravitational_currentFieldLine.begin();i_data!=fieldsManager->m_gravitational_currentFieldLine.end(); ++i_data)
		currentFieldLine->push_back( *i_data );
	{
		Lock lock (fieldsManager->protectFieldLines);//_mutex);
		fieldsManager->push_back(currentFieldLine);
	}
//	Fire_ViewChange();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox(L"ERROR: FieldLineGravitational::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void GravitationalFieldLine_Tangent(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
	Point3D p(stateVector->data);
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	Vector3D tangent;
	Vector3D g = fieldsManager->Static_GravitationalField(p);
	tangent += g;
	double G=tangent.norm();
	if (G < 1.0e-32)
	{
		throw SolveIt_ERROR(L"ERROR: GravitationalFieldLine_Tangent::   G = 0");
		dx->data[0] = 0;
		dx->data[1] = 0;
		dx->data[2] = 0;
		return;
	}
	dx->data[0] = tangent.x/G;
	dx->data[1] = tangent.y/G;
	dx->data[2] = tangent.z/G;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineGravitational::CVodeStep(void)
{
try {
	double abstol=1e-3;
	double reltol=1e-3;
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	struct {
		long	length;
		double	*data;
	} X;
	double t=0, tout=0, dt = 0.05;
	long flag = CVODE::ERR_FAILURE;

//	Lock lock (_mutex);
	X.length	= 3;
	t			= 0;
	tout		= dt;
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	X.data =  fieldsManager->m_selectedFieldPoint.v;
	long cnt = 0;
tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(X.length, NULL);
	if (x == 0) throw SolveIt_ERROR(L"ERROR: FieldLineGravitational::CVodeStep:N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"ERROR: FieldLineGravitational:x->data==0");
	x->length = X.length;
	CopyMemory(x->data, X.data, X.length * sizeof(double) );
	t			= 0;
	tout		= dt;

	void *cvode_mem = CVODE::CVodeMalloc(x->length, GravitationalFieldLine_Tangent, t, x, 
		CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) this, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, NULL, NULL);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(X.data, x->data, X.length * sizeof(double) );
	}
	CVODE::N_VFree(x);
	if (flag == CVODE::SUCCESS)
fieldsManager->m_gravitational_currentFieldLinePoint= Vector3D(X.data);

	if (flag != CVODE::SUCCESS)
	{
		reltol *= 10;
		abstol *= 10;
		if (cnt++<3) goto tryAgain;
		throw SolveIt_ERROR(L"ERROR: FieldLineMagnetostatic::CVodeStep: failed");
	}

	return flag;
}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			return 0;
		}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineElectric3D::MakeTimeStep(void)
{
	CSystem* m_system	= theApp.GetSystem();
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	try
	{
	fieldsManager->m_electric_3D_currentFieldLine.clear();
// set in CGLView::OnMouseMove
//	fieldsManager->m_electrostatic_currentFieldLinePoint = Vector3D(pt.x,pt.y,0);
	const long num_points_to_compute = 128;
	long num_points = num_points_to_compute;
	CGLView* pGLView = theApp.GetGLView();
	while (num_points-- > 0)
	{
		long flag = CVodeStep();
		if (flag == 0)//SUCCESS
		{
fieldsManager->m_electric_3D_currentFieldLine.push_back( fieldsManager->m_electrostatic_currentFieldLinePoint );
		//	double x_Pos = fieldsManager->m_electric_currentFieldLinePoint[0];
		//	double y_Pos = fieldsManager->m_electric_currentFieldLinePoint[1];
		//	double z_Pos = fieldsManager->m_electric_currentFieldLinePoint[2];
		//	pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		//	fieldsManager->m_electric_3D_currentFieldLine.push_back( x_Pos );
		//	fieldsManager->m_electric_3D_currentFieldLine.push_back( y_Pos );
		//	fieldsManager->m_electric_3D_currentFieldLine.push_back( z_Pos );
		}
	}
//	vector_of_double* currentFieldLine = new vector_of_double(fieldsManager->m_electric_3D_currentFieldLine.n_group_by, fieldsManager->m_electric_3D_currentFieldLine.vecPlotTrajectoryColor);
vector_of_Vector3D* currentFieldLine = new vector_of_Vector3D();
	currentFieldLine->clear();
	vector_of_Vector3D::const_iterator i_data;
	for (i_data=fieldsManager->m_electric_3D_currentFieldLine.begin();i_data!=fieldsManager->m_electric_3D_currentFieldLine.end(); ++i_data)
		currentFieldLine->push_back( *i_data );
	{
		Lock lock (fieldsManager->protectFieldLines);//_mutex);
		fieldsManager->push_back(currentFieldLine);
	}
//	Fire_ViewChange();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox(L"ERROR: FieldLineElectric2D::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void FieldLine_3Tangent(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();

	Point3D p(stateVector->data);
	Vector3D tangent	= fieldsManager->ElectrostaticField(p);//N_Body_ElectrostaticField(p);
	double E=tangent.norm();
	if (E < 1.0e-32)
	{
		throw SolveIt_ERROR(L"ERROR: FieldLine_3Tangent::   E = 0");
		dx->data[0] = 0;
		dx->data[1] = 0;
		dx->data[2] = 0;
		return;
	}
	dx->data[0] = tangent.x/E;
	dx->data[1] = tangent.y/E;
	dx->data[2] = tangent.z/E;
}
///////////////////////////////////////////////////////////////////////////////
long FieldLineElectric3D::CVodeStep(void)
{
	double abstol=1e-3;
	double reltol=1e-3;
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	struct {
		long	length;
		double	*data;
	} X;
	double t=0, tout=0, dt = 0.05;
	long flag = CVODE::ERR_FAILURE;

//	Lock lock (_mutex);
	X.length	= 3;
	t			= 0;
	tout		= dt;
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	X.data =  fieldsManager->m_selectedFieldPoint.v;
	long cnt = 0;
tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(X.length, NULL);
	if (x == 0) throw SolveIt_ERROR(L"ERROR: FieldLineElectric3D::CVodeStep:N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"ERROR: FieldLineElectric3D:x->data==0");
	x->length = X.length;
	CopyMemory(x->data, X.data, X.length * sizeof(double) );
	t			= 0;
	tout		= dt;

	void *cvode_mem = CVODE::CVodeMalloc(x->length, FieldLine_3Tangent, t, x, 
		CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) this, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, NULL, NULL);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(X.data, x->data, X.length * sizeof(double) );
	}
	CVODE::N_VFree(x);
//	if (flag == CVODE::SUCCESS)
//	fieldsManager->m_electric_currentFieldLinePoint == Vector3D(X.data);

	if (flag != CVODE::SUCCESS)
	{
		reltol *= 10;
		abstol *= 10;
		if (cnt++<3) goto tryAgain;
		throw SolveIt_ERROR(L"ERROR: FieldLineElectric3D::CVodeStep: failed");
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineMagnetostatic::MakeTimeStep(void)
{
	CSystem* m_system	= theApp.GetSystem();
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	try
	{
	fieldsManager->m_magnetostatic_currentFieldLine.clear();
// set in CGLView::OnLButtonDown:	fieldsManager->m_selectedPointForMagneticField
	const long num_points_to_compute = 128;
	long num_points = num_points_to_compute;
	CGLView* pGLView = theApp.GetGLView();
	while (num_points-- > 0)
	{
		long flag = CVodeStep();
		if (flag == 0)//SUCCESS
		{
fieldsManager->m_magnetostatic_currentFieldLine.push_back( fieldsManager->m_magnetostatic_currentFieldLinePoint );
		}
	}
	vector_of_Vector3D* currentFieldLine = new vector_of_Vector3D();
	currentFieldLine->clear();
	vector_of_Vector3D::const_iterator i_data;
	for (i_data=fieldsManager->m_magnetostatic_currentFieldLine.begin();i_data!=fieldsManager->m_magnetostatic_currentFieldLine.end(); ++i_data)
		currentFieldLine->push_back( *i_data );
	{
		Lock lock (fieldsManager->protectFieldLines);//_mutex);
		fieldsManager->push_back(currentFieldLine);
	}
//	Fire_ViewChange();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox(L"ERROR: FieldLineMagnetostatic::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void MagneticFieldLine_Tangent(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
	Point3D p(stateVector->data);
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	Vector3D tangent;
	Vector3D b = fieldsManager->MagneticField(p);
	tangent += b;
	double B=tangent.norm();
	if (B < 1.0e-32)
	{
		throw SolveIt_ERROR(L"ERROR: MagneticFieldLine_Tangent::   B = 0");
		dx->data[0] = 0;
		dx->data[1] = 0;
		dx->data[2] = 0;
		return;
	}
	dx->data[0] = tangent.x/B;
	dx->data[1] = tangent.y/B;
	dx->data[2] = tangent.z/B;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineMagnetostatic::CVodeStep(void)
{
try {
	double abstol=1e-3;
	double reltol=1e-3;
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	struct {
		long	length;
		double	*data;
	} X;
	double t=0, tout=0, dt = 0.05;
	long flag = CVODE::ERR_FAILURE;

//	Lock lock (_mutex);
	X.length	= 3;
	t			= 0;
	tout		= dt;
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
//	X.data =  fieldsManager->m_selectedPointForMagneticField.v;
	X.data =  fieldsManager->m_selectedFieldPoint.v;
	long cnt = 0;
tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(X.length, NULL);
	if (x == 0) throw SolveIt_ERROR(L"ERROR: FieldLineMagnetostatic::CVodeStep:N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"ERROR: FieldLineMagnetostatic:x->data==0");
	x->length = X.length;
	CopyMemory(x->data, X.data, X.length * sizeof(double) );
	t			= 0;
	tout		= dt;

	void *cvode_mem = CVODE::CVodeMalloc(x->length, MagneticFieldLine_Tangent, t, x, 
		CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) this, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, NULL, NULL);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(X.data, x->data, X.length * sizeof(double) );
	}
	CVODE::N_VFree(x);
	if (flag == CVODE::SUCCESS)
fieldsManager->m_magnetostatic_currentFieldLinePoint= Vector3D(X.data);
////	fieldsManager->m_electric_currentFieldLinePoint == Vector3D(X.data);

	if (flag != CVODE::SUCCESS)
	{
		reltol *= 10;
		abstol *= 10;
		if (cnt++<3) goto tryAgain;
		throw SolveIt_ERROR(L"ERROR: FieldLineMagnetostatic::CVodeStep: failed");
	}

	return flag;
}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			return 0;
		}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
long FieldLineMagnetostatic::MakeTimeStep(void){return 0;}
long FieldLineMagnetostatic::CVodeStep(void){return 0;}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLineMagnetic::MakeTimeStep(void){return 0;}
long FieldLineMagnetic::CVodeStep(void){return 0;}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
long FieldLine_Equipotential::MakeTimeStep(void)
{
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	Vector2D pt;
	int niter=0;
	{
		Lock lock (fieldsManager->protectFieldLines);//_mutex);
		pt		= fieldsManager->m_CurrentMousePosition;
		niter	= fieldsManager->m_RecordPotentialNumSteps;
	}
	fieldsManager->DrawEquipotential2D(pt, niter, true);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long FieldLine_Equipotential::CVodeStep(void)
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Electric2D
IMPLEMENT_DYNCREATE(FieldLineElectric2D, CWinThread) 
BOOL FieldLineElectric2D::InitInstance() 
{ 
	return TRUE; 
} 
int FieldLineElectric2D::ExitInstance() 
{ 
	return CWinThread::ExitInstance(); 
} 
BEGIN_MESSAGE_MAP(FieldLineElectric2D, CWinThread) 
ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep) 
ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone) 
ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt) 
END_MESSAGE_MAP() 
void FieldLineElectric2D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{ 
	m_bStepCompleted=true; 
//	return 0; 
} 
void FieldLineElectric2D::OnStep(WPARAM wParam, LPARAM lParam) 
{ 
	m_bStepCompleted=false; 
	MakeTimeStep(); 
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
//	return 0; 
} 
void FieldLineElectric2D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0); 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
long FieldLineElectric2D::MakeTimeStep(void)
{
	CSystem* m_system	= theApp.GetSystem();
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
	try
	{
	fieldsManager->m_electric_2D_currentFieldLine.clear();
// set in CGLView::OnMouseMove
//	fieldsManager->m_electric_currentFieldLinePoint = Vector2D(pt.x,pt.y);


// calculate 'num_points_to_compute' fieldline points from current Mouse position:
	const long num_points_to_compute = 64;
	long num_points = num_points_to_compute;
	CGLView* pGLView = theApp.GetGLView();
	while (num_points-- > 0)
	{
		long flag = CVodeStep();
		if (flag == 0)//SUCCESS
		{
			bool add=true;
#if 10
			ObjectsList::const_iterator it;
			for (it = rigidBodyManager->begin(); it != rigidBodyManager->end(); ++it)
			{
				CRigidBody *  pbody = dynamic_cast<CRigidBody *>((*it).rbPtr);
				ASSERT(pbody != 0);
				if (pbody == 0) throw SolveIt_ERROR(L"pbody == 0");//return -1;
				Vector2D x(pbody->x);
				x -= fieldsManager->m_electrostatic_currentFieldLinePoint2D;
			//	if (fieldsManager->m_electrostatic_currentFieldLinePoint2D == pbody->x) {add=false;break;}
				if (x.Norm() < 0.1) {add=false;break;}
			}
#endif
			{
				vector_of_Vector2D::const_iterator i_data;
				for (i_data=fieldsManager->m_electric_2D_currentFieldLine.begin();i_data!=fieldsManager->m_electric_2D_currentFieldLine.end(); ++i_data)
				{
					Vector2D x(*i_data);
					if (fieldsManager->m_electrostatic_currentFieldLinePoint2D == x) {add=false;break;}
					x -= fieldsManager->m_electrostatic_currentFieldLinePoint2D;
					if (x.Norm() < 0.1) {add=false;break;}
				}
			}
			if (add)
				fieldsManager->m_electric_2D_currentFieldLine.push_back( fieldsManager->m_electrostatic_currentFieldLinePoint2D );
		//	double x_Pos = fieldsManager->m_electric_currentFieldLinePoint[0];
		//	double y_Pos = fieldsManager->m_electric_currentFieldLinePoint[1];
		//	double z_Pos = 0;//z == 0 => z == gluPerspective_zNear
		//	pGLView->ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		//	fieldsManager->m_electric_2D_currentFieldLine.push_back( x_Pos );
		//	fieldsManager->m_electric_2D_currentFieldLine.push_back( y_Pos );
		}
	}
//	vector_of_double* currentFieldLine = new vector_of_double(fieldsManager->m_electric_2D_currentFieldLine.n_group_by, fieldsManager->m_electric_2D_currentFieldLine.vecPlotTrajectoryColor);
//vector_of_Vector2D* currentFieldLine = new vector_of_Vector2D(0xff/255., 0x66/255., 0x33/255.);
//vector_of_Vector2D* currentFieldLine = new vector_of_Vector2D(0x99/255., 0x00/255., 0x33/255.);
vector_of_Vector2D* currentFieldLine = new vector_of_Vector2D(0xff/255., 0xcc/255., 0x33/255.);
	currentFieldLine->clear();
	vector_of_Vector2D::const_iterator i_data;
	for (i_data=fieldsManager->m_electric_2D_currentFieldLine.begin();i_data!=fieldsManager->m_electric_2D_currentFieldLine.end(); ++i_data)
		currentFieldLine->push_back( *i_data );
	{
		Lock lock (fieldsManager->protectFieldLines);//_mutex);
		fieldsManager->push_back(currentFieldLine);
	}
//	Fire_ViewChange();
	}
	catch (wchar_t * str)
	{
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		AfxMessageBox(L"ERROR: FieldLineElectric2D::MakeTimeStep");
		PostQuitMessage(0);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void FieldLine_2Tangent(CVODE::integer N, CVODE::real t, CVODE::N_Vector stateVector, CVODE::N_Vector dx, void *f_data)
{
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();

	Vector2D p(stateVector->data);
	Vector2D e	= fieldsManager->E(p);
	double E=sqrt(e.x*e.x+e.y*e.y);
	if (E < 1.0e-32)
	{
		throw SolveIt_ERROR(L"ERROR: FieldLine_2Tangent::   E = 0");
		dx->data[0] = 0;
		dx->data[1] = 0;
		return;
	}
	dx->data[0] = e.x/E;
	dx->data[1] = e.y/E;
}
///////////////////////////////////////////////////////////////////////////////
long FieldLineElectric2D::CVodeStep(void)
{
	double abstol=1e-3;
	double reltol=1e-3;
	static CVODE::real ropt[OPT_SIZE];
	static long iopt[OPT_SIZE];
	struct {
		long	length;
		double	*data;
	} X;
	double t=0, tout=0, dt = 0.05;
	long flag = CVODE::ERR_FAILURE;

//	Lock lock (_mutex);
	X.length	= 2;
	t			= 0;
	tout		= dt;
	CFieldsManager* fieldsManager = theApp.GetFieldsManager();
	X.data =  &fieldsManager->m_electrostatic_currentFieldLinePoint2D.x;
	long cnt = 0;
tryAgain:
	CVODE::N_Vector x = CVODE::N_VNew(X.length, NULL);
	if (x == 0) throw SolveIt_ERROR(L"ERROR: FieldLineElectric2D::CVodeStep:N_VNew failed");
	if (x->data == 0) throw SolveIt_ERROR(L"ERROR: FieldLineElectric2D:x->data==0");
	x->length = X.length;
	CopyMemory(x->data, X.data, X.length * sizeof(double) );
	t			= 0;
	tout		= dt;

	void *cvode_mem = CVODE::CVodeMalloc(x->length, FieldLine_2Tangent, t, x, 
		CVODE::BDF, CVODE::NEWTON, CVODE::SS,
							&reltol, &abstol, (void*) this, NULL, FALSE, iopt, ropt, NULL);
	if (cvode_mem)
	{
		CVODE::CVDense(cvode_mem, NULL, NULL);
		flag = CVODE::CVode(cvode_mem, tout, x, &t, CVODE::NORMAL);
		CVODE::CVodeFree(cvode_mem);
	}
	else return (CVODE::N_VFree(x) , -1);
	if (flag == CVODE::SUCCESS)
	{
		CopyMemory(X.data, x->data, X.length * sizeof(double) );
	}
	CVODE::N_VFree(x);
//	if (flag == CVODE::SUCCESS)
//	fieldsManager->m_electric_currentFieldLinePoint == Vector3D(X.data);

	if (flag != CVODE::SUCCESS)
	{
		reltol *= 10;
		abstol *= 10;
		if (cnt++<2) goto tryAgain;
		throw SolveIt_ERROR(L"ERROR: FieldLineElectric2D::CVodeStep: failed");
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

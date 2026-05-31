// OdeSolverCollision.cpp: implementation of the COdeSolverCollision class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OdeSolverCollision.h"
#include "Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COdeSolverCollision::COdeSolverCollision()
{

}

COdeSolverCollision::~COdeSolverCollision()
{

}

//#define Field(jth_Particle,xyz)			field [3*(jth_Particle) + (xyz)]
#define UX 0
#define UY 1
#define JZ 2
#define WZ 3
#define UZ 3

#define X 0
#define Y 1
#define Z 2

//transverse speed tolerance:
#define U_TR_TOL 1e-14


void SolveIt::CollisionDerivs(CVODE::integer N, CVODE::real t, CVODE::N_Vector x, CVODE::N_Vector xdot, void *f_data)
{
try
{
	UserDataCollision f_Data = *(UserDataCollision*)f_data;
	double coefficientOfKineticFriction = f_Data.coefficientOfKineticFriction;
	double (*tK)[3] = f_Data.tK;	// collision matrix transformed to local frame
	long bCompression = f_Data.bCompression;

	double c = 1;						// cosine theta 
	double s = 0;						// sine theta
	double ux = N_VIth(x,UX);
	double uy = N_VIth(x,UY);
	double uz = t;
	double Jz = N_VIth(x,JZ);
	double Wz = N_VIth(x,WZ);
	double k1 = tK[X][Z];
	double k2 = tK[Y][Z];
	double k3 = tK[Z][Z];
	double u = sqrt(ux*ux+uy*uy);	// transverse speed
	if (u>U_TR_TOL)
	{
		c = ux/u, s = uy/u;
		k1 -=  coefficientOfKineticFriction*(tK[X][X]*c + tK[X][Y]*s);
		k2 -=  coefficientOfKineticFriction*(tK[Y][X]*c + tK[Y][Y]*s);
		k3 -=  coefficientOfKineticFriction*(tK[Z][X]*c + tK[Z][Y]*s);
	}
	if (bCompression)
	{
// dependent variables: Ux, Uy, Jz, Wz
// dUx/dUz, dUy/dUz, dJz/dUz, dWz/dUz 
// independent variable: t = Uz
		double k = 1.0/k3;
		N_VIth(xdot,UX) = k*k1;
		N_VIth(xdot,UY) = k*k2;
		N_VIth(xdot,JZ) = k;
		N_VIth(xdot,WZ) = k*t;
	}
	else {
// dependent variables: Ux, Uy, Jz, Uz
// dUx/dWz, dUy/dWz, dJz/dWz, dUz/dWz 
// independent variable: t = Wz
		double Uz = N_VIth(x,UZ);

		if (Uz<=0.0)
		{
		N_VIth(xdot,UX) = 0;
		N_VIth(xdot,UY) = 0;
		N_VIth(xdot,JZ) = 0;
		N_VIth(xdot,UZ) = 0;
			throw SolveIt_ERROR(L"Uz < 0");
//	TimedMessageBox(NULL,L"ERROR: Uz<=0.0 during restitution phase","CollisionDerivs",MB_OK);
		//	return;
		//	return;
		}
		double r = 1.0/Uz;
		N_VIth(xdot,UX) = r*k1;
		N_VIth(xdot,UY) = r*k2;
		N_VIth(xdot,JZ) = r;
		N_VIth(xdot,UZ) = r*k3;
	}

}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	TimedMessageBox(NULL,L"ERROR: Uz<=0.0 during restitution phase",L"CollisionDerivs",MB_OK);
//	TimedMessageBox(NULL,L"CollisionDerivs",L"Exception",MB_OK);
		N_VIth(xdot,UX) = 0;
		N_VIth(xdot,UY) = 0;
		N_VIth(xdot,JZ) = 0;
		N_VIth(xdot,UZ) = 0;
		//	return;
	throw SolveIt_ERROR(L"-1");
}
}



#define Ith(v,i)    N_VIth(v,i)         /* Ith numbers components 0..NEQ-1 */
#define IJth(A,i,j) DENSE_ELEM(A,i,j) /* IJth numbers rows,cols 0..NEQ-1 */
 
void SolveIt::CollisionJac1(CVODE::integer N, CVODE::DenseMat J, CVODE::RhsFn f, void *f_data, CVODE::real tn,
		 CVODE::N_Vector _x, CVODE::N_Vector fy, CVODE::N_Vector ewt, CVODE::real h, CVODE::real uround,
		 void *jac_data, long int *nfePtr, CVODE::N_Vector vtemp1,
		 CVODE::N_Vector vtemp2, CVODE::N_Vector vtemp3)
{
try
{
	CVODE::integer j, k;

	for (j=0; j< N; ++j)
	{
		for (k=0; k< N; ++k) IJth(J,j,k) = 0;
	}

	UserDataCollision f_Data = *(UserDataCollision*)f_data;
	double mu = f_Data.coefficientOfKineticFriction;
	double (*tK)[3] = f_Data.tK;	// collision matrix transformed to local frame
	long bCompression = f_Data.bCompression;

	double k11 = tK[X][X];
	double k12 = tK[X][Y];
	double k13 = tK[X][Z];
	double k21 = tK[Y][X];
	double k22 = tK[Y][Y];
	double k23 = tK[Y][Z];
	double k31 = tK[Z][X];
	double k32 = tK[Z][Y];
	double k33 = tK[Z][Z];

	double x = N_VIth(_x,UX);	// Ux
	double y = N_VIth(_x,UY);	// Uy
	double R = sqrt(x*x+y*y);	// transverse speed
	double t = tn;

	if (bCompression)
	{
		if (R<=U_TR_TOL) return;
// dependent variables: Ux, Uy, Jz, Wz
// dUx/dUz, dUy/dUz, dJz/dUz, dWz/dUz 
// independent variable: t = Uz
		double k3= -k33*R + mu*(k31*x + k32*y);
		double g = mu /(R*k3*k3);

		double a = g*( k12*k31*mu*R - k11*k32*mu*R + k13*k32*x - k12*k33*x - k13*k31*y + k11*k33*y);
		double b = g*( k22*k31*mu*R - k21*k32*mu*R + k23*k32*x - k22*k33*x - k23*k31*y + k21*k33*y);
		double c = g*( k32*x - k31*y);

		IJth(J,UX,UX)=-y*a;//g*mu*y*(-k12*k31*mu*R + k11*k32*mu*R - k13*k32*x + k12*k33*x + k13*k31*y - k11*k33*y);
		IJth(J,UY,UX)=-y*b;//g*mu*y*(-k22*k31*mu*R + k21*k32*mu*R - k23*k32*x + k22*k33*x + k23*k31*y - k21*k33*y);
		IJth(J,JZ,UX)=-y*c;//-g*mu*  y*(k32*x - k31*y);
		IJth(J,WZ,UX)= t*IJth(J,JZ,UX);//-g*mu*t*y*(k32*x - k31*y);

		IJth(J,UX,UY)= x*a;//g*mu*x*( k12*k31*mu*R - k11*k32*mu*R + k13*k32*x - k12*k33*x - k13*k31*y + k11*k33*y);
		IJth(J,UY,UY)= x*b;//g*mu*x*( k22*k31*mu*R - k21*k32*mu*R + k23*k32*x - k22*k33*x - k23*k31*y + k21*k33*y);
		IJth(J,JZ,UY)= x*c;//g*mu*  x*(k32*x - k31*y);
		IJth(J,WZ,UY)= t*IJth(J,JZ,UY);//g*mu*t*x*(k32*x - k31*y);


/*
y*(-(k12*k31*mu*R) + k11*k32*mu*R - k13*k32*x + k12*k33*x + k13*k31*y - k11*k33*y);
y*(-(k22*k31*mu*R) + k21*k32*mu*R - k23*k32*x + k22*k33*x + k23*k31*y - k21*k33*y);
y*(-(k32*x) + k31*y);
y*(-(k32*x) + k31*y)*z;


x*(k12*k31*mu*R - k11*k32*mu*R + k13*k32*x - k12*k33*x - k13*k31*y + k11*k33*y);
x*(k22*k31*mu*R - k21*k32*mu*R + k23*k32*x - k22*k33*x - k23*k31*y + k21*k33*y);
x*(k32*x - k31*y);
x*(k32*x - k31*y)*z
*/

	}
	else
	{
// dependent variables: Ux, Uy, Jz, Uz
// dUx/dWz, dUy/dWz, dJz/dWz, dUz/dWz 
// independent variable: t = Wz
		double z = N_VIth(_x,UZ);			// Uz
		double g = 0;
		if (R > U_TR_TOL) g = mu/(z*R*R*R);
//expansion:
		double a = g*(k11*y - k12*x);
		double b = g*(k22*x - k21*y);
		double c = g*(k32*x - k31*y);

		IJth(J,UX,UX)=-y*a;
		IJth(J,UY,UX)= y*b;
		IJth(J,JZ,UX)= 0;
		IJth(J,UZ,UX)= y*c;

		IJth(J,UX,UY)= x*a;
		IJth(J,UY,UY)=-x*b;
		IJth(J,JZ,UY)= 0;
		IJth(J,UZ,UY)=-x*c;

		if (R > U_TR_TOL) g = 1.0/(R*z*z);

		IJth(J,UX,UZ) = g*(-k13*R + k11*mu*x + k12*mu*y);
		IJth(J,UY,UZ) = g*(-k23*R + k21*mu*x + k22*mu*y);
		IJth(J,JZ,UZ) = g*(-R);
		IJth(J,UZ,UZ) = g*(-k33*R + k31*mu*x + k32*mu*y);

/*
y*(k12*x - k11*y),y*(k22*x - k21*y),0,y*(k32*x - k31*y);
-(x*(k12*x - k11*y)),-(x*(k22*x - k21*y)) 0,-(x*(k32*x - k31*y)));
-(k13*R) + k11*mu*x + k12*mu*y,-(k23*R) + k21*mu*x + k22*mu*y,-1,-(k33*R) + k31*mu*x + k32*mu*y)
*/
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	TimedMessageBox(NULL,L"CollisionJac1",L"Exception",MB_OK);
	throw E_FAIL;
}
}


#undef Ith
#undef IJth

#undef UX
#undef UY
#undef JZ
#undef WZ
#undef UZ

#undef X
#undef Y
#undef Z


/* CVODE::CVodeMalloc Constants */

//#define RTOL	1.0e-7			/* scalar relative tolerance */
#define FLOOR	1.0				/* value of C1 or C2 at which tolerances */
/*																	//* change from relative to absolute			*
#define ATOL	(RTOL*FLOOR)	//* scalar absolute tolerance *

#define RTOL  1e-4         //* scalar relative tolerance            *

#define ATOL1 1e-6         //* vector absolute tolerance components *
#define ATOL2 1e-6
#define ATOL3 1e-14
#define ATOL4 1e-8
*/

#define RTOL  1e-5
#define ATOL  1e-5

long COdeSolverCollision::CollisionCVODE(	double& ux, double& uy, double& uz,
											double& Wz, double& Jz,
											double coefficientOfKineticFriction,
											double coefficientOfRestitution,
											double (*tK)[3]
)
{
	long flag = -12;
	wchar_t buf[1024];
	/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf));
	if (uz>=0.0)
	{
//	ATLASSERT(uz<0.0);
		TimedMessageBox(NULL,L"Uz >= 0.0",L"CollisionCVODE ERROR:", MB_OK);
		ux =  uy =  Jz =  uz = Wz = 0;
		return flag;
	}
// 'force' during restitution phase is inversely proportional to uz
//  extend interval so that uz > 0 :
	double extendUz = -uz*0.1;
	double usav = uz;

	CVODE::N_Vector y = 0;
	try
	{
	const long NEQ = 4;

	UserDataCollision f_Data, *f_data=&f_Data;

	f_Data.coefficientOfKineticFriction = coefficientOfKineticFriction;
	f_Data.tK = tK;
	f_Data.bCompression = 1;

	Jz = 0;//  initial Jz
	Wz = 0;//  initial Wz
	double dt	= -uz*0.01;
	CVODE::real t = uz, tout = 0;


	y = CVODE::N_VNew(NEQ, NULL);
	N_VIth(y,0) = ux, N_VIth(y,1) = uy, N_VIth(y,2) = Jz, N_VIth(y,3) = Wz;

	CVODE::real reltol=RTOL;

	CVODE::real abstol=ATOL;
//	CVODE::N_Vector abstol = CVODE::N_VNew(NEQ, NULL); 

	CVODE::real ropt[OPT_SIZE];
	long int iopt[OPT_SIZE];
	iopt[CVODE::MXSTEP] = 5000;
//* Allocate memory, and set problem data, initial values, tolerances *
//	N_VIth(abstol,0) = ATOL1, N_VIth(abstol,1) = ATOL2, N_VIth(abstol,2) = ATOL3, N_VIth(abstol,3) = ATOL4;

/* Call CVODE::CVodeMalloc to initialize CVODE: 

		 NEQ		 is the problem size = number of equations
		 f			 is the user's right hand side function in y'=f(t,y)
		 T0			is the initial time
		 y			 is the initial dependent variable vector
		 BDF		 specifies the Backward Differentiation Formula
		 NEWTON	specifies a Newton iteration
		 SS			specifies scalar relative and absolute tolerances
		 &reltol and &abstol are pointers to the scalar tolerances
		 f_data		is the pointer to the user-defined block of coefficients
		 FALSE	 indicates there are no optional inputs in iopt and ropt
		 iopt		and ropt arrays communicate optional CVODE::integer and CVODE::real input/output

		 A pointer to CVODE problem memory is returned and stored in cvode_mem.	*/

	void *cvode_mem = 0;

//compression phase: t = uz
	long cnt = 0;
	reltol /= 2;
	abstol /= 2;


	double sav_ux = N_VIth(y,0);
	double sav_uy = N_VIth(y,1);
	double sav_Jz = N_VIth(y,2);
	double sav_Wz = N_VIth(y,3);
	double sav_uz = uz;


try_compressionAgain:
	reltol *= 2;
	abstol *= 2;

	N_VIth(y,0) = sav_ux;
	N_VIth(y,1) = sav_uy;
	N_VIth(y,2) = sav_Jz;
	N_VIth(y,3) = sav_Wz;
	uz = sav_uz;

	t = uz, tout = 0;


	cvode_mem = CVODE::CVodeMalloc(NEQ, CollisionDerivs, uz, y, 
		CVODE::BDF/*ADAMS*/, /*FUNCTIONAL*/CVODE::NEWTON, CVODE::SS, &reltol,
							&abstol, (void*) f_data, NULL, FALSE, iopt, ropt, NULL);

	if (cvode_mem == NULL)
	{
		CVODE::N_VFree(y);
		TimedMessageBox(NULL,L"CVODE::CVodeMalloc failed.",L"COdeSolverCollision::compression phase", MB_OK);
		return flag;
	}

//	CVODE::CVDense(cvode_mem, NULL, NULL);
	CVODE::CVDense(cvode_mem, CollisionJac1, NULL);
//	CVSpgmr(cvode_mem, NONE, MODIFIED_GS, 0, 0.0, NULL, NULL, NULL);
	tout = 0;//uz = 0 at end of compression phase
	flag = CVODE::CVode(cvode_mem, tout, y, &uz, CVODE::NORMAL);
	CVODE::CVodeFree(cvode_mem);
	if (flag != CVODE::SUCCESS)
	{
		if (cnt++<16) goto try_compressionAgain;
		CVODE::N_VFree(y);
		TimedMessageBox(NULL, GetWC(CVOdeErrors[-flag]), L"COdeSolverCollision::compression phase", MB_OK);
		return flag;
//		ux =  uy =  Jz =  uz = 0;
//		throw -1;
	}

//work done by the normal component of the impulse from the beginning
//of the collision to the time of maximum compression:
	double Winitial = N_VIth(y,3);
	if (Winitial>=0.0) {
		CVODE::N_VFree(y);
		TimedMessageBox(NULL,L"Error: Wz > 0 at end of compression phase.",L"COdeSolverCollision::CollisionCVODE", MB_OK);
		return flag;
//		ux =  uy =  Jz =  uz = 0;
//		throw -1;
	}
// Wfinal <= 0
// Wfinal == 0 for an elastic collision
	double Wfinal = (1.0 - coefficientOfRestitution*coefficientOfRestitution)*Winitial;//Stronge

//'force' during restitution phase is inversely proportional to uz
//extend interval so that uz > 0 :
	double Wr =  - coefficientOfRestitution*coefficientOfRestitution*Winitial;//Mirtich
	if (Wr>0.0)
		extendUz = .05*sqrt(2*Wr*fabs(tK[2][2] - coefficientOfKineticFriction*sqrt(tK[2][0]*tK[2][0]+tK[2][1]*tK[2][1])));
/*
	oss.seekp(0);
	oss << "Wfinal = "  << Wfinal << endl << "extendUz = "  << extendUz << endl
		<< "Wmax comp = "  << Winitial << endl << "u0 "  << usav   << endl << "uz "  << uz  << endl << "Wr "  << Wr
		<< ends;
		::MessageBox(NULL, buf, L"about to extend interval", MB_OK);
*/

	if (extendUz != uz)
	{


	cnt = 0;
	reltol /= 2;
	abstol /= 2;


	sav_ux = N_VIth(y,0);
	sav_uy = N_VIth(y,1);
	sav_Jz = N_VIth(y,2);
	sav_Wz = N_VIth(y,3);
	sav_uz = uz;


try_extendAgain:
	reltol *= 2;
	abstol *= 2;

	N_VIth(y,0) = sav_ux;
	N_VIth(y,1) = sav_uy;
	N_VIth(y,2) = sav_Jz;
	N_VIth(y,3) = sav_Wz;
	uz = sav_uz;

	t = uz, tout = 0;


	iopt[CVODE::MXSTEP] = 5000;
	cvode_mem = CVODE::CVodeMalloc(NEQ, CollisionDerivs, uz, y, CVODE::BDF, CVODE::NEWTON, CVODE::SS, &reltol,
							&abstol, (void*) f_data, NULL, FALSE, iopt, ropt, NULL);

	if (cvode_mem == NULL) {
		CVODE::N_VFree(y);
		TimedMessageBox(NULL,L"CVODE::CVodeMalloc failed.",L"COdeSolverCollision::extend interval", MB_OK);
		return -1;
	}

//	CVODE::CVDense(cvode_mem, NULL, NULL);
	CVODE::CVDense(cvode_mem, CollisionJac1, NULL);
	flag = CVODE::CVode(cvode_mem, extendUz, y, &uz, CVODE::NORMAL);
	CVODE::CVodeFree(cvode_mem);

	if (flag != CVODE::SUCCESS)
	{
		if (cnt++<16) goto try_extendAgain;
		CVODE::N_VFree(y);
		TimedMessageBox(NULL, GetWC(CVOdeErrors[-flag]), L"COdeSolverCollision::extend interval", MB_OK);
		return flag;
//		ux =  uy =  Jz =  uz = 0;
//		throw -1;
	}
	}
//restitution phase: t = Wz
	cnt = 0;
	reltol /= 2;
	abstol /= 2;


	sav_ux = N_VIth(y,0);
	sav_uy = N_VIth(y,1);
	sav_Jz = N_VIth(y,2);
	sav_uz = uz;
	sav_Wz = N_VIth(y,3);


tryAgain:
	reltol *= 2;
	abstol *= 2;

	N_VIth(y,0) = sav_ux;
	N_VIth(y,1) = sav_uy;
	N_VIth(y,2) = sav_Jz;
	uz = sav_uz;
	N_VIth(y,3) = sav_Wz;

	f_Data.bCompression = 0;
	Wz = N_VIth(y,3);
	N_VIth(y,3) = uz;
	/*N_VIth(y,0) = ux, N_VIth(y,1) = uy, N_VIth(y,2) = Jz,*/
	t=Wz;
	if (Wfinal > Wz)
	{
	iopt[CVODE::MXSTEP] = 5000;
	cvode_mem = CVODE::CVodeMalloc(NEQ, CollisionDerivs, Wz, y, CVODE::BDF, CVODE::NEWTON, CVODE::SS, &reltol,
							&abstol, (void*) f_data, NULL, FALSE, iopt, ropt, NULL);

	if (cvode_mem == NULL) {
		CVODE::N_VFree(y);
		TimedMessageBox(NULL,L"CVODE::CVodeMalloc failed.",L"COdeSolverCollision::restitution phase", MB_OK);
		return -1;
	}

//	CVODE::CVDense(cvode_mem, NULL, NULL);
	CVODE::CVDense(cvode_mem, CollisionJac1, NULL);
	flag = CVODE::CVode(cvode_mem, Wfinal, y, &Wz, CVODE::NORMAL);
	CVODE::CVodeFree(cvode_mem);
	if (flag != CVODE::SUCCESS)
	{
		if (cnt++<16) goto tryAgain;
		CVODE::N_VFree(y);
		TimedMessageBox(NULL, GetWC(CVOdeErrors[-flag]), L"COdeSolverCollision::restitution phase", MB_OK);
		return flag;
//		ux =  uy =  Jz =  uz = 0;
//		throw -1;
	}
	else
	{
	}
	}

	ux = N_VIth(y,0);
	uy = N_VIth(y,1);
	Jz = N_VIth(y,2);
	uz = N_VIth(y,3);

//		oss.seekp(0);
//		oss << ux << " "  << uy << " "  << uz << " "  << Jz << " "  << Wz << " " << ends;
//		::MessageBox(NULL, buf, L"ux uy uz Jz Wz", MB_OK);

	CVODE::N_VFree(y);
//	CVODE::N_VFree(abstol);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		CVODE::N_VFree(y);
//		TimedMessageBox(NULL, "CollisionCVODE failed","General Exception", MB_OK);
		throw SolveIt_ERROR(L"-1");
	}
	return flag;
}

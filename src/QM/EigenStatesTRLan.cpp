//EigenStatesTRLan.cpp
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//!!!!!!!!!!observe:
// _imp____data_2d_MOD_nev;in cpp code; compiler prepends '_' ; leading underscore
//__imp____data_2d_MOD_nev in dll


#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <iostream>
#include <random> // Requires C++ 11

#include <Spectra/GenEigsRealShiftSolver.h>
#include <Spectra/MatOp/DenseGenRealShiftSolve.h>
#include <Spectra/MatOp/SparseGenRealShiftSolve.h>

using namespace Spectra;
//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"

typedef Eigen::MatrixXd Matrix;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXcd ComplexMatrix;
typedef Eigen::VectorXcd ComplexVector;
typedef Eigen::SparseMatrix<double> SpMatrix;

// Traits to obtain operation type from matrix type
template <typename MatType>
struct OpTypeTrait
{
	typedef DenseGenRealShiftSolve<double> OpType;
};

template <>
struct OpTypeTrait<SpMatrix>
{
	typedef SparseGenRealShiftSolve<double> OpType;
};

// Generate random sparse matrix
SpMatrix gen_sparse_data(int n, double prob = 0.5)
{
	SpMatrix mat(n, n);
	std::default_random_engine gen;
	gen.seed(0);
	std::uniform_real_distribution<double> distr(0.0, 1.0);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (distr(gen) < prob)
				mat.insert(i, j) = distr(gen) - 0.5;
		}
	}
	return mat;
}



#include "stdafx.h"

//#pragma optimize( "", off )

#include <cassert>

#include "SolveIt.h"
#include "QMStationary2D.h"
#include "System.h"
#include "MainFrm.h"
#include "GLView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern SMutex g_SMutex_1;
//extern "C" void _stdcall	DATA_2D_mp_CALL_DSYEV( long*N, double*A, double*W, long*exitINFO );

///////////////////////////////////////////////////////////////////////////////
extern "C" void _stdcall	DATA_2D_mp_INIT();
extern "C" void _stdcall	DATA_2D_mp_TERM();
extern "C" void _stdcall	DATA_2D_mp_CALL_DSYEV();
extern "C"  double			DATA_2D_mp_LAMBDA;
extern "C" long				DATA_2D_mp_PPOTENTIAL;
extern "C" long				DATA_2D_mp_PPSI__XY;
extern "C" long				DATA_2D_mp_PEIGENVALUES;
extern "C" long				DATA_2D_mp_PWRK;
extern "C" long				DATA_2D_mp_PIPARAM;
extern "C" long				DATA_2D_mp_EXITINFO;
extern "C"  double _stdcall	DATA_2D_mp_GETDSYEVEIGENVALUE(long* j);
extern "C"  double _stdcall	DATA_2D_mp_GETDSYEVEIGENVECTOR(long* j, long* x);
extern "C"  void _stdcall	DATA_2D_mp_SETDSYEV_DIAG(long* x, long* y, double* val);
extern "C"  void _stdcall	DATA_2D_mp_SETDSYEV_A(long* x, long* y, double* val);
extern "C"  void _stdcall	DATA_2D_mp_SETDSYEV_UPPER();

extern "C"  double _stdcall	DATA_2D_mp_GETEIGENVALUE(long* j);
extern "C"  double _stdcall	DATA_2D_mp_GETEIGENVECTOR(long* j, long* x);
extern "C" void _stdcall	DATA_2D_mp_DSSIMP();
	const long maxncv = 25;
//	typedef long (*MATRIX_2)[2];
//	typedef double DataXY[_N2*_N2];
//	typedef double (**MATRIX_2)[2];
//	typedef double (*MATRIX_2x[2])[maxncv];

//	typedef double (MATRIX_2xy[2])[maxncv];
//	typedef double (MATRIX_2xy[2])[maxncv];
	typedef double (MATRIX_2xy)[2][maxncv];
//	typedef double (MATRIX_2xy)[maxncv][2];

//extern "C" long				DATA_2D_mp_PVEC;
extern "C" MATRIX_2xy				DATA_2D_mp_PVEC;
///////////////////////////////////////////////////////////////////////////////
extern "C" void __stdcall	TRLAN77(void(*op)(int*, int*, double*, int*, double*, int*),
		   int*, int*, int*, double*, double*, int*, double*, int*);
///////////////////////////////////////////////////////////////////////////////
extern "C" void _stdcall DATA_2D_mp_AV( long*nx, double*v, double*w);

void twoDim_op(int *nrow, int *ncol, double *xin, int *ldx, double *yout, int *ldy) {
	long nx=65;
//	ASSERT(nx*nx==*nrow);
//	ASSERT(1==*ncol);
//	ASSERT(nx*nx==*ldx);
//	ASSERT(nx*nx==*ldy);
	// // //DATA_2D_mp_AV( &nx, xin, yout );
}
///////////////////////////////////////////////////////////////////////////////
extern	int *__ipar;

extern	double *__potential;
extern	double *__EigenValues;
extern	double *__psi_xy;
extern	double *__wrk;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::EigenStatesTRLan()
{
#if 0
	Lock lock(mutex);
//	CWaitCursor();
	SetCursor(LoadCursor(NULL, IDC_WAIT));
/*
				CGLView *pView = theApp.GetGLView();
				HDC hDC = pView->GetDC()->GetSafeHdc();
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
				//wglMakeCurrent(hDC, pView->m_hRC);

		if (listEigenStates)
		{
			glDeleteLists(listEigenStates[0], m_nNumFound+1);

		GLenum e=GL_NO_ERROR;
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			ASSERT(0);
			return;
		}
			delete[] listEigenStates;
			listEigenStates = 0;
		}
*/

/* global variables */
//	double *const &__psi_xy			= (double *)DATA_2D_mp_PPSI__XY;
//	double *const &__EigenValues	= (double *)DATA_2D_mp_PEIGENVALUES;

#define NROW	(65*65)
#define LOHI	-1
#define NED		8
#define MEV		12
//#define MAXLAN	(4*MEV)
#define MAXLAN	(30*1)
	int my_pe = 0;
	int i, lwrk, nrow, mev;
//	double tt, to, tp, tr;

	int * ipar	= __ipar;//(int *)DATA_2D_mp_PIPARAM;
	double* eval	= __EigenValues;
	double* evec	= __psi_xy;//new double[NROW*MEV];
	double* wrk		= __wrk;
/*
	double eval[MEV], evec[NROW*MEV], wrk[MAXLAN*(MAXLAN+10)];
	int ipar[32];
*/


//	double* eval	= new double[MEV];
//	double* evec	= new double[NROW*MEV];
//	double*const & eval	= &EigenValues[0];//new double[MEV];

//	double* eval	= (double *)DATA_2D_mp_PEIGENVALUES;
	ASSERT(eval);
//	double*const & eval	= __EigenValues;
//	double*const & evec	= &psi_xy[0];//new double[NROW*MEV];
//	double*const & evec	= __psi_xy;//new double[NROW*MEV];
//	double* evec	= (double *)DATA_2D_mp_PPSI__XY;
	ASSERT(evec);
//	double* wrk		= (double *)DATA_2D_mp_PWRK;//new double[MAXLAN*(MAXLAN+10)];
	ASSERT(wrk);

	nrow = NROW;
	mev = MEV;
	lwrk = MAXLAN*(MAXLAN+10);
	ipar[0] = 0;
	ipar[1] = LOHI;//which end of the spectrum to compute.
	ipar[2] = m_nSeekEigenvalues;//     Integer :: ned     ! number of eigenpairs wanted

	ipar[3] = 0;	 //* nec = 0 *
	ipar[4] = MAXLAN;
//Scheme 5 attempts to mimic the restarting strategy in ARPACK,
	ipar[5] = 5;	 //* restarting scheme 1 *
	ipar[6] = NROW*20;  //* maximum number of MATVECs *
	ipar[7] = 0;	 //* MPI_COMM *
//	ipar[7] = -6;	 //* MPI_COMM *
	ipar[8] = -15;   //* verboseness *
	ipar[9] = 99;	//* Fortran IO unit number used to write log messages *
//	ipar[9] = 6;	//* Fortran IO unit number used to write log messages *
	ipar[10] = 1;	//* iguess -- 1 = use supply the starting vector *
	ipar[11] = -5;   //* checkpointing flag -- write about -5 times *
	ipar[12] = 98;   //* Fortran IO unit number to write checkpoint files *
//	ipar[12] = 6;   //* Fortran IO unit number to write checkpoint files *
	ipar[13] = 3*NROW;//* floating-point operations per MATVEC per PE *
	for (i=0; i<MEV; ++i) eval[i]=0.0;
	for (i=0; i<NROW; ++i) evec[i] = 1.0;
	wrk[0] = 1.4901E-8; //* relative tolerance on residual norms *
try{

/*
	SetCursor(LoadCursor(NULL, IDC_WAIT));

//DSYEV computes all eigenvalues and, optionally, eigenvectors of a real symmetric matrix A.
	DATA_2D_mp_SETDSYEV_UPPER();
///////////////////////////////////////////////////////////////////////////////
#define potential(x, y) v[(x)*NDATA+(y)]
///////////////////////////////////////////////////////////////////////////////
	double *v	= (double *)DATA_2D_mp_PPOTENTIAL;
	double val = 0;
	for (long  x = 0; x < NDATA; ++x)
	for (long  x2 = 0; x2 < NDATA; ++x2)
	{
		for (long  y = 0; y < NDATA; ++y)
		for (long  y2 = 0; y2 < NDATA; ++y2)
		{
			long i = x*NDATA+y;
			long j = x2*NDATA+y2;
			if (x2==x && y2==y)
				val = -4 + potential(x, y);
			else
			if (x2==x && y2==y+1)
				val = 1;
			else
			if (x2==x && y2==y-1)
				val = 1;
			else
			if (y2==y && x2==x+1)
				val = 1;
			else
			if (y2==y && x2==x-1)
				val = 1;
			else
				val = 0;
			DATA_2D_mp_SETDSYEV_A(& i, & j, & val);
	//		DATA_2D_mp_SETDSYEV_DIAG(& x, & y, & val);
		}
	}
#undef potential
///////////////////////////////////////////////////////////////////////////////

	DATA_2D_mp_CALL_DSYEV();
*/
	DWORD res = 0;
	res = GetFileAttributes("fail.txt");
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile("fail.txt");
	}
	res = GetFileAttributes("log_io.txt");
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile("log_io.txt");
	}

	TRLAN77(twoDim_op, ipar, &nrow, &mev, eval, evec, &nrow, wrk, &lwrk);
	if (my_pe == 0)
	{
		wchar_t msg[256];
		for (i=0; i<ipar[3]; i++)
		{
			sprintf(msg,"E(%i) = %25.17g   [ %16.4e]\n", i+1, eval[i], wrk[i]);
			cout << msg;
		}
	}


	SetCursor(LoadCursor(NULL, IDC_ARROW));

//	CWaitCursor();
	SetCursor(LoadCursor(NULL, IDC_WAIT));

//	if (::IsWindow(QMStationary2DhWnd)) ::SendMessage(QMStationary2DhWnd, WM_CLOSE,0,0);
	const long N = NDATA*NDATA;


//	long *iparam	= (long *)DATA_2D_mp_PIPARAM;

	if (m_nNumFound)
	{
	//	if (psi_xy)
	//	{
	//		delete[]psi_xy;
	//		psi_xy = 0;
	//	}
	//	if (psi_x)
	//	{
		//	for (int j = 0; j < m_nNumFound; ++j)
		//	{
		//		delete[]psi_x[j];
		//	}
		//	delete[]psi_x;
	//		psi_x = 0;
	//	}
	//	firstInlistEigenStates = 0;
	}

	m_nNumFound = ipar[3];
	if (m_nNumFound>max_NumFound) m_nNumFound = max_NumFound;
//	m_nNumFound = DATA_2D_mp_EXITINFO==0? NDATA/16:0;//NDATA*NDATA
		cout << "m_nNumFound = " << m_nNumFound << endl << endl;

	FindMinMaxPotential();
		cout << "minPotential = " << m_fMinPotential << "\t\t\t";
		cout << "maxPotential = " << m_fMaxPotential << endl;

	if (m_nNumFound>0)
	{
//	psi_xy = new double[m_nNumFound*N];
//	psi_x = new double*[m_nNumFound];

//	listEigenStates = new GLuint[m_nNumFound+1];
//	for (long j = 0; j <= m_nNumFound; ++j)
//	{
//		listEigenStates[j]	= 0;
//	}
//	firstInlistEigenStates = glGenLists(m_nNumFound);
//	//wglMakeCurrent(0, 0);
//	::ReleaseDC(pView->m_hWnd, hDC);

	for ( int j = 0; j < m_nNumFound; ++j)
	{
	//	psi_x[j]	= new double[N];

		double val = eval[j];
	//	double val = DATA_2D_mp_GETDSYEVEIGENVALUE(& j);
		cout << "EigenValue[" << j << "] = " << val << "\t\t\t";
	//	for (long  x = 0; x < N; ++x)
	//	{
	//		psi_xy[x+j*N]	= evec[x+j*N];
		//	psi_x[j][x]	= evec[x+j*N];
		//	psi_x[j][x]	= DATA_2D_mp_GETDSYEVEIGENVECTOR(& j, & x);
	//	}

	for (long  x = 0; x < _N2a; ++x)
	for (long  y = 0; y < _N2a; ++y)
	{
	//	psi_x[j][(x)*_N2a+(y)]	= evec[(x)*_N2a+(y)+j*N];
		psi_xy(j,x,y)	= evec[(j)*_N2aN2a+(x)*_N2a+(y)];//
	//	psi_xy(j,x,y)	= fabs(evec[(j)*_N2aN2a+(x)*_N2a+(y)]);
	}
		FindMinMaxPsi(j);
	//	DrawList(j);
		cout << j << ":: "  << "min psi = " << m_fMinPsi << "\t\t\t";
		cout << j << ":: "  << "max psi = " << m_fMaxPsi << endl;
		if(10)
		{
			for (long  x = 0; x < _N2a; ++x)
			for (long  y = 0; y < _N2a; ++y)
			{
			//	psi_x[j][(x)*_N2a+(y)]	= evec[(x)*_N2a+(y)+j*N]*evec[(x)*_N2a+(y)+j*N];
	//			psi_x[j][(x)*_N2a+(y)]	= fabs(psi_x[j][(x)*_N2a+(y)]);
	//			psi_xy(j,x,y)	= evec[(j)*_N2aN2a+(x)*_N2a+(y)];
		//		psi_xy(j,x,y)	= fabs(evec[(j)*_N2aN2a+(x)*_N2a+(y)]);
				psi_xy(j,x,y)	= evec[(x)*_N2a+(y)+j*N]*evec[(x)*_N2a+(y)+j*N];//
			}
		}
	}
	}
	DATA_2D_mp_TERM();
///////////////////////////////////////////////////////////////////////////////
//	ofstream ofs(_T("QMStationary2D.htm"));
//	DWORD res = GetFileAttributes(_T("QMStationary2D.htm"));
//	if (res!=-2)//FILE_ATTRIBUTE_ARCHIVE
//	{
//		DeleteFile(_T("QMStationary2D.htm"));
//	}

	std::wstring QMStationary2Dhtm(g_szWorkDir.c_str());
	QMStationary2Dhtm += L"\\QMStationary2D.htm";
	res = GetFileAttributes(QMStationary2Dhtm);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(QMStationary2Dhtm);
	}


	std::ofstream ofs(std::wstring(QMStationary2Dhtm), std::ios::binary);//_T("QMStationary2D.htm")

	//ofstream ofs(QMStationary2Dhtm);//_T("QMStationary2D.htm")

/*
	ofs << "<HTML><HEAD></HEAD><BODY><P>" << endl;
	ofs << "<HR>"
"<H5>Visualization Summary:</h5>"
"<ul>"
"  <li>Press the 'Run' button to calculate eigenvalues and eigenvectors [you may have just done this]</li>"
"  <li>double-click in the 'System Window' to display the next stationary state wave function</li>"
"  <li>press the 'PNG' [portable network graphics] button to save an image in .png format</li>"
"  <li>press the 'EPS' [encapsulated postscript] button to save an image in .eps format</li>"
"</ul><HR>" << endl;
*/

	const constexpr wchar_t* dat = R"(
<HTML><HEAD></HEAD><BODY><P>\n
<HR>\n
<H5>Visualization Summary:</h5>\n
<ul>\n
  <li>Press the 'Run' button to calculate eigenvalues and eigenvectors [you may have just done this]</li>\n
  <li>double-click in the 'System Window' to display the next stationary state wave function</li>\n
  <li>press the 'PNG' [portable network graphics] button to save an image in .png format</li>\n
  <li>press the 'EPS' [encapsulated postscript] button to save an image in .eps format</li>\n
</ul><HR>\n
)";


	std::wstring s;
	std::wstring rr{ dat };
	ofs << rr.c_str() << endl;
/*
	ifstream ifs1( _T("duh.txt"));
	if (ifs1)
	{
		while (!(getline(ifs1, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
		ofs << "<hr>" << endl;
	}
	ifstream ifs2( _T("duh2.txt"));
	if (ifs2)
	{
		while (!(getline(ifs2, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
		ofs << "<hr>" << endl;
	}
*/
	ifstream ifs3(g_szWorkDirFileName.c_str());// _T("CONOUT.txt"));
	if (ifs3)
	{
		while (!(getline(ifs3, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
		ofs << "<hr>" << endl;
	}

	//assert(QMStationary2Dtxt != nullptr);
	//std::wstring ws(QMStationary2Dtxt, SysStringLen(QMStationary2Dtxt));
	wstring basicstring((wchar_t*)QMStationary2Dtxt);
	ifstream ifs(basicstring);//_T("QMStationary2D.txt")
	if (ifs)
	{
		while (!(getline(ifs, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
	}
	ifstream ifs_fail("fail.txt");
	if (ifs_fail)
	{
		while (!(getline(ifs_fail, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
		ifstream ifs_log_io("log_io.txt");
		if (ifs_log_io)
		{
			while (!(getline(ifs_log_io, s, '\n')).fail())
			{
				ofs << s << " ";
				ofs << "<P>" << endl;
			}
		}
	}
	ofs << "<P></BODY>" << endl;

	if (QMStationary2Dhtm.length() > 0)
	{
		CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
//		CInfoDialog* pCQMStationary2DDialog = new CInfoDialog(0, _T("Stationary QM 2D"), 0, QMStationary2Dhtm);//_T("QMStationary2D.htm")
//		pFrame->m_CleanupOnInitialize.Add(pCQMStationary2DDialog->m_hWnd);
		pFrame->PostMessage(WM_USER_AddCInfoDialog2, (WPARAM) GetWC((const wchar_t*) QMStationary2Dhtm), (LPARAM) GetWC(_T("Stationary QM 2D")));//
	//	pFrame->SendMessage(WM_USER_AddCInfoDialog, (WPARAM) html_file, (LPARAM)title);//
	}
}
catch (...) {
//	delete[] wrk;
//	delete[] evec;
//	delete[] eval;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	ASSERT(0);
	return;
}

//	delete[] wrk;
//	delete[] evec;
//	delete[] eval;
#endif
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
///////////////////////////////////////////////////////////////////////////////
#if 0
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::EigenStates()
{
	DWORD res = GetFileAttributes(g_szWorkDirFileName.c_str());//_T("CONOUT.txt"));
	if (res!=INVALID_FILE_ATTRIBUTES)//res!=-1//FILE_ATTRIBUTE_ARCHIVE_T("CONOUT.txt")
	{
		DeleteFile(g_szWorkDirFileName.c_str());
	}
	Lock lock(mutex);
try{
//	CWaitCursor();
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	DATA_2D_mp_DSSIMP();

//	if (::IsWindow(QMStationary2DhWnd)) ::SendMessage(QMStationary2DhWnd, WM_CLOSE,0,0);
	const long N = NDATA*NDATA;


	long *iparam	= (long *)DATA_2D_mp_PIPARAM;

	if (m_nNumFound)
	{
		if (psi_x)
		{
			for (int j = 0; j < m_nNumFound; ++j)
			{
				delete[]psi_x[j];
			}
			delete[]psi_x;
			psi_x = 0;
		}
		firstInlistEigenStates = 0;
		if (listEigenStates)
		{
			glDeleteLists(listEigenStates[0], m_nNumFound+1);

		GLenum e=GL_NO_ERROR;
		if ( GL_NO_ERROR !=  (e=glGetError()) )
		{
			ASSERT(0);
			return;
		}
			delete[] listEigenStates;
			listEigenStates = 0;
		}
	}

	m_nNumFound	= iparam[4];
	if (m_nNumFound==0) return;
	
	psi_x = new double*[m_nNumFound];

	listEigenStates = new GLuint[m_nNumFound+1];
	for (long j = 0; j <= m_nNumFound; ++j)
	{
		listEigenStates[j]	= 0;
	}
//	firstInlistEigenStates = glGenLists(m_nNumFound);


	FindMinMaxPotential();
		cout << "minPotential = " << m_fMinPotential << "\t\t\t";
		cout << "maxPotential = " << m_fMaxPotential << endl;
	for ( j = 0; j < m_nNumFound; ++j)
	{
		psi_x[j]	= new double[N];

		double val = DATA_2D_mp_GETEIGENVALUE(&j);
		cout << "EigenValue[" << j << "] = " << val << "\t\t\t";
		for (long  x = 0; x < NDATA*NDATA; ++x)
			psi_x[j][x]	= DATA_2D_mp_GETEIGENVECTOR(&j, &x);

		FindMinMaxPsi(j);
	//	DrawList(j);
		cout << "min psi = " << m_fMinPsi << "\t\t\t";
		cout << "max psi = " << m_fMaxPsi << endl;
		for (x = 0; x < NDATA*NDATA; ++x)
			psi_x[j][x]	= fabs(psi_x[j][x]);
	}


	DATA_2D_mp_TERM();
///////////////////////////////////////////////////////////////////////////////
	std::wstring QMStationary2Dhtm(g_szWorkDir.c_str());//buf
	QMStationary2Dhtm += L"\\QMStationary2D.htm";

	
	res = GetFileAttributes(QMStationary2Dhtm);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(QMStationary2Dhtm);
	}
	ofstream ofs(QMStationary2Dhtm);//_T("QMStationary2D.htm")
	wstring s;
	ofs << "<HTML><HEAD></HEAD><BODY><P>" << endl;
	ifstream ifs2( _T("duh.txt"));
	if (ifs2)
	{
		while (!(getline(ifs2, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
		ofs << "<hr>" << endl;
	}
	{
	ifstream ifs(g_szWorkDirFileName.c_str());// _T("CONOUT.txt")
	if (ifs)
	{
		while (!(getline(ifs, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
	}
	}

	ifstream ifs(QMStationary2Dtxt);// _T("QMStationary2D.txt"));
	if (ifs)
	{
		while (!(getline(ifs, s, '\n')).fail())
		{
			ofs << s << " ";
			ofs << "<P>" << endl;
		}
	}
	ofs << "<P></BODY>" << endl;
//	CQMStationary2DDialog* pCQMStationary2DDialog=new CQMStationary2DDialog();//this);0;//
	CInfoDialog* pCQMStationary2DDialog = new CInfoDialog(0, _T("Stationary QM 2D"), 0, QMStationary2Dhtm);//_T("QMStationary2D.htm")
//	QMStationary2DhWnd  = pCQMStationary2DDialog->m_hWnd;//0;//
//	CSystem* m_system	= theApp.GetSystem();
//	m_system->m_CleanupOnInitialize.Add(pCQMStationary2DDialog->m_hWnd);
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->m_CleanupOnInitialize.Add(pCQMStationary2DDialog->m_hWnd);
}
catch (...) {
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	ASSERT(0);
}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
#endif
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::FindMinMaxPotential()
{
///////////////////////////////////////////////////////////////////////////////
//#undef potential
//#define potential(x, y) __potential[(x)*NDATA+(y)]
///////////////////////////////////////////////////////////////////////////////
//	double *v		= (double *)DATA_2D_mp_PPOTENTIAL;
//	double *__potential		= (double *)DATA_2D_mp_PPOTENTIAL;
	m_fMinPotential = m_fMaxPotential = potential(0, 0);
	for (long x=0;x<NDATA;x++)
	{
		for (long y=0;y<NDATA;y++)
		{
			if (m_fMinPotential > potential(x, y)) m_fMinPotential = potential(x, y);
			if (m_fMaxPotential < potential(x, y)) m_fMaxPotential = potential(x, y);
		}
	}
	double X=m_fMaxPotential;
	double Y=m_fMinPotential;
	double Z=m_fMinPotential;
}
//#undef potential
///////////////////////////////////////////////////////////////////////////////

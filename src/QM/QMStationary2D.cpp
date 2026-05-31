// QMStationary2D.cpp: implementation of the CQMStationary2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//#pragma optimize( "", off )

#include "SolveIt.h"
#include "System.h"
#include "QMStationary2D.h"
//#include "..\ScannerParser\parser_2020.h"
//#include "Help/HtmlCtrl.h"
//#include "Help/TraceWin.h"
//#include "GL_Thread.h"
#include "GLView.h"
//#include "SMutex.h"
#include "instruction.h"
#include "StackMachine.h"
#include "MainFrm.h"
#include "DataQM_Stationary2D.h"
#include "Cruncher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern SMutex g_SMutex_1;
///////////////////////////////////////////////////////////////////////////////
extern "C" void _stdcall	DATA_2D_mp_INIT();
extern "C" void _stdcall	DATA_2D_mp_TERM();
extern "C"  double			DATA_2D_mp_LAMBDA;
extern "C" long				DATA_2D_mp_PPOTENTIAL;
extern "C" long				DATA_2D_mp_PPSI__XY;
extern "C" long				DATA_2D_mp_PWRK;
extern "C" long				DATA_2D_mp_PEIGENVALUES;
extern "C" long				DATA_2D_mp_PIPARAM;
extern "C" long				DATA_2D_mp_NEV;//# eigenvalues to seek

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

//double CQMStationary2D::psi_xy[2*max_Num*_N2aN2a];
//double **CQMStationary2D::psi_x				= 0;
//GLuint *CQMStationary2D::listEigenStates	= 0;
//double *CQMStationary2D::eigenvalues		= 0;
//double *CQMStationary2D::eigenvalueErrors	= 0;
///////////////////////////////////////////////////////////////////////////////
//extern "C" double _stdcall	DATA_PASS_mp_DBSJ0(double* X);
extern "C" void __stdcall	TRLAN77(void(*op)(int*, int*, double*, int*, double*, int*),
			int*, int*, int*, double*, double*, int*, double*, int*);
extern FILE *stream_stdout;
/* prototype of operator (matrix-vector multiplication routine 
void ndiag_op2(int*, int*, double*, int*, double*, int*);
double	g_fLambda	= 0;//m_system->m_pCQMStationary1D->m_fLambda;
double*g_v			= 0;//m_system->m_pCQMStationary1D->v;

void ndiag_op2(int *nrow, int *ncol, double *xin, int *ldx, double *yout, int *ldy) {
	int i, j;
	double  *xtmp, *ytmp;

	for (j=0; j<*ncol; ++j) {
		xtmp = xin + *ldx * j;
		ytmp = yout + *ldy * j;
		i=1;
		*ytmp = *xtmp * (2*g_fLambda + g_v[i-1]) - g_fLambda * ( *(xtmp+1) + *(xtmp+*ncol-1) );
		++xtmp, ++ytmp;
		for (i=1+1; i<=-1+*nrow; ++i, ++xtmp, ++ytmp)
			*ytmp = *xtmp * (2*g_fLambda + g_v[i-1]) - g_fLambda * ( *(xtmp+1) + *(xtmp-1));
		*ytmp = *xtmp * (2*g_fLambda + g_v[i-1]) - g_fLambda * ( *(xtmp-1) + *(xtmp-*ncol+1) );
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
static CQMStationary2D* pCQMStationary2D=0;
	double *__potential		= 0;//(double *)DATA_2D_mp_PPOTENTIAL;
	double *__psi_xy		= 0;//(double *)DATA_2D_mp_PPSI__XY;
	double *__EigenValues	= 0;//(double *)DATA_2D_mp_PEIGENVALUES;
	double *__wrk			= 0;//(double *)DATA_2D_mp_PWRK;//new double[MAXLAN*(MAXLAN+10)];
	int *__ipar	= 0;//(int *)DATA_2D_mp_PIPARAM;

//HWND QMStationary2DhWnd = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

	IMPLEMENT_DYNCREATE(CQMStationary2D, CWinThread)

double DATA_2D_mp_LAMBDA{1.0};
long DATA_2D_mp_NEV{ 1 };

CQMStationary2D::CQMStationary2D()://long _NDATA, double& fScaleNurbs) :
	m_fLambda(DATA_2D_mp_LAMBDA),
	m_nSeekEigenvalues(DATA_2D_mp_NEV),
	m_fScaleNurbs(theApp.GetGLView()->m_fScaleWave),

	NDATA(65),				//_NDATA + (_NDATA%2? 0:1)),//force odd

//	eigenvalueIndex(theApp.GetSystem()->data__QM_Stationary1D->eigenvalueIndex),
//	theNurbs(0),//gluNewNurbsRenderer()),
	m_bstrQHullOptions(L"d QJ Qx"),
	firstInlistEigenStates(0),
//	listEigenStates(0),
	psi_x(0),
//	psi_xy(0),
//	eigenvalues(new double[NDATA]),
//	eigenvalueErrors(new double[NDATA]),
//	eigenvectors_block(new double[NDATA*NDATA]),
//	eigenvectors(new double*[NDATA]),
	m_fMinPotential(0),
	m_fMaxPotential(0),
	m_fMinPsi(0),
	m_fMaxPsi(0),
	m_fScalePsi(1),
	m_fScalePotential(1),
	m_nSelectedEigenstate(-1),
	m_nNumFound(0),

	QMStationary2DhWnd(0)

{
//		AfxMessageBox(L"1");
	ASSERT(NDATA==_N2a);

#if 0
	DATA_2D_mp_INIT();
	__potential		= (double *)DATA_2D_mp_PPOTENTIAL;
	__psi_xy		= (double *)DATA_2D_mp_PPSI__XY;
	__EigenValues	= (double *)DATA_2D_mp_PEIGENVALUES;
	__wrk			= (double *)DATA_2D_mp_PWRK;//new double[MAXLAN*(MAXLAN+10)];
	__ipar			= (int *)DATA_2D_mp_PIPARAM;
#endif



//#pragma warning(disable: 4355) // 'this' used before initialized
	pCQMStationary2D	= this;
//	psi_x				= 0;
//#pragma warning(default: 4355)
	m_fScaleNurbs		= 80.0;
	m_nSeekEigenvalues	= 8;
	m_fLambda			= 0.92;
//Redirect stdout to a File 
//	TCHAR buf[_MAX_PATH];
//	GetCurrentDirectory(_MAX_PATH, buf);
	QMStationary2Dtxt	= std::wstring(g_szWorkDir.c_str());//buf
	QMStationary2Dtxt += L"\\QMStationary2D.txt";
	FILE* redirected_stdout = 0;
	freopen_s(&redirected_stdout, GetC(QMStationary2Dtxt.c_str()), "w", stdout);
//	SetEnvironmentVariable(_T("FORT6"), QMStationary2Dtxt);
//	SetEnvironmentVariable(_T("FOR_TYPE"), QMStationary2Dtxt);
//	SetEnvironmentVariable(_T("FOR_PRINT"), QMStationary2Dtxt);

///////////////////////////////////////////////////////////////////////////////
//	double *__psi_xy		= (double *)DATA_2D_mp_PPSI__XY;
//	for (long  x = 0; x < sizeof(psi_xy)/sizeof(psi_xy[0]); ++x)
	for (long  j = 0; j < max_Num; ++j)
	for (long  x = 0; x < _N2a; ++x)
	for (long  y = 0; y < _N2a; ++y)
	{
	//	psi_xy[x]	= 0;
		psi_xy(j,x,y)	= 0;
	}
	for (long  x = 0; x < sizeof(listEigenStates)/sizeof(listEigenStates[0]); ++x)
	{
		listEigenStates[x]	= 0;
	}
}
///////////////////////////////////////////////////////////////////////////////
void __cdecl CQMStationary2D::GLViewRenderCallback(void)
{
	return;//

	GLenum e=GL_NO_ERROR;
//	if (g_SMutex_1.wait(500))
	{
		if (pCQMStationary2D && pCQMStationary2D->m_nNumFound>0)
		{
			CGLView *pGLView = theApp.GetGLView();
//			HDC hDC = pGLView->GetDC()->GetSafeHdc();
//		HDC hDC = pGLView->m_pDC->GetSafeHdc();
//			if(FALSE == ::wglMakeCurrent(hDC, pGLView->m_hRC))
//			{
//					::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
//					throw SolveIt_ERROR(L"Unable to //wglMakeCurrent");
//			}
			pCQMStationary2D->firstInlistEigenStates = glGenLists(pCQMStationary2D->m_nNumFound);
			if ( GL_NO_ERROR !=  (e=glGetError()) )
			{
					//wglMakeCurrent(0, 0);
//					::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
					throw SolveIt_ERROR(L"Unable to glGenLists");
//					ASSERT(0);
//					return;
			}
			//wglMakeCurrent(0, 0);
//			::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
			ASSERT(pCQMStationary2D->firstInlistEigenStates>0);
		}

		pCQMStationary2D->m_nSelectedEigenstate = 0;//pCQMStationary2D->m_nNumFound;
		for (long j = 0; j < pCQMStationary2D->m_nNumFound; ++j)
		{
			if (pCQMStationary2D->listEigenStates[j]) continue;
				pCQMStationary2D->listEigenStates[j]	= pCQMStationary2D->firstInlistEigenStates + j;
				pCQMStationary2D->DrawList(j);
			//	pCQMStationary2D->BuildHull(j);
		}
//		g_SMutex_1.Release();
	}
}
///////////////////////////////////////////////////////////////////////////////
//if (theApp.GetGLView()->notifications_mutex.wait(500))
//{
//	theApp.GetGLView()->notifications_mutex.Release();
//}
//if (theApp.GetGLView()->render_actions_mutex.wait(500))
//{
//	theApp.GetGLView()->render_actions_mutex.Release();
//}

void CQMStationary2D::GLViewNotificationCallback(void)
{
	++pCQMStationary2D->m_nSelectedEigenstate;
	if (pCQMStationary2D->m_nSelectedEigenstate == pCQMStationary2D->m_nNumFound+1)
	{
		pCQMStationary2D->m_nSelectedEigenstate=0; 
	}
}
///////////////////////////////////////////////////////////////////////////////
CQMStationary2D::~CQMStationary2D()
{
	freopen_s(&stream_stdout, "stdout.txt", "w", stdout);
//	if (psi_xy)
//	{
//		delete[]psi_xy;
//		psi_xy = 0;
//	}
	pCQMStationary2D = 0;
//	g_SMutex_1.Release();
//	if (::IsWindow(QMStationary2DhWnd)) ::SendMessage(QMStationary2DhWnd, WM_CLOSE,0,0);

	CGLView *pGLView = theApp.GetGLView();
	ASSERT( pGLView );
if (pGLView && pGLView->notifications_mutex.wait(500))
{
	pGLView->notifications.clear();
	pGLView->notifications_mutex.Release();
}

if (pGLView && pGLView->render_actions_mutex.wait(500))
{
	pGLView->render_actions.clear();
	pGLView->render_actions_mutex.Release();
}

//	if (theNurbs) gluDeleteNurbsRenderer(theNurbs);
//	freopen( "CON", "w", stdout );
//	if (eigenvalues) delete[]eigenvalues;
//	if (eigenvalueErrors) delete[]eigenvalueErrors;
	for (int j = 0; j < NDATA; ++j)
	{
//		if (eigenvectors[j])
//		delete[]eigenvectors[j];
	}
//	delete[]eigenvectors;
//	delete[]eigenvectors_block;
	if (psi_x)
	{
		for (int j = 0; j < m_nNumFound; ++j)
		{
			delete[]psi_x[j];
		}
		delete[]psi_x;
	}
	psi_x	= 0;
/*

	if (listEigenStates)
	{
		ASSERT(listEigenStates[0] == firstInlistEigenStates);
		glDeleteLists(listEigenStates[0], m_nNumFound);
	//	delete[] listEigenStates;
	//	listEigenStates = 0;
	}
*/
	for (auto j = 0; j < m_nNumFound; ++j)
	{
		if (listEigenStates[j]) glDeleteLists(listEigenStates[j], 1);
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::Draw(const long& j)
{
	if (m_nNumFound==0) return;
//	if (psi_xy==0) return;
	if (listEigenStates[j])
		glCallList(listEigenStates[j]);
	else
		DrawHull(j);

///////////////////////////////////////////////////////////////////////////////
	{
		std::wstring bstr =std::to_wstring( long(j) ) ;
		wstring s(bstr);
	glRasterPos2d(-4+0.4*j, 4);

	glListBase(2000);
	glCallLists(static_cast<GLsizei>(s.size()), GL_BYTE, (GLbyte *)s.c_str());

#if 0
	CGLView *pGLView = theApp.GetGLView();
	wchar_t szNumber[64];
	wsprintf(szNumber,"%04u",j) ;
	CString strCaption(szNumber);
		HDC			hDC		= pGLView->GetDC()->GetSafeHdc();
//		HDC hDC = GetDC()->GetSafeHdc();
		//HDC hDC = m_pDC->GetSafeHdc();
	SetBkMode(hDC, TRANSPARENT);
	CRect rc;
	pGLView->GetClientRect(rc);
	int cx = rc.Width()/2, cy = rc.Height()/2;
//	CFont* pOldFont = (class CFont *) SelectObject(hDC, &m_Font);
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	SetTextAlign(hDC,	TA_LEFT | TA_TOP);//TA_CENTER
	SetTextColor(hDC, RGB(0, 0, 128));
	CRect r(6, cy-2*tm.tmHeight, cx, tm.tmHeight);//
//	ExtTextOut(cx-2-tm.tmAveCharWidth*m_strCaption.GetLength() , cy - tm.tmHeight, ETO_OPAQUE , NULL, m_strCaption, m_strCaption.GetLength(), NULL);//ETO_CLIPPED
	ExtTextOut(hDC, cx-2-tm.tmAveCharWidth*strCaption.GetLength() , cy - tm.tmHeight, ETO_OPAQUE , NULL, strCaption, strCaption.GetLength(), NULL);//ETO_CLIPPED
//	pGLView->SelectObject(hDC, pOldFont);
	::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
#endif
	}
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::Draw()
{
	if (m_nNumFound==0) return;
//	if (psi_xy==0) return;

	if (m_nSelectedEigenstate == m_nNumFound)
	{
//		Draw(0);
		for (long k=0; k < m_nNumFound; ++k) Draw(k);
	}
	else
	Draw(m_nSelectedEigenstate);
}
///////////////////////////////////////////////////////////////////////////////
//#undef potential
//#define potential(x, y) v[(x)*NDATA+(y)]
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void __stdcall nurbsError(GLenum errorCode)
{
const GLubyte *estring;
estring = gluErrorString(errorCode);
	MessageBox (0, (const wchar_t *) estring, L"Nurbs Error:", MB_OK);
//exit (0);
}
///////////////////////////////////////////////////////////////////////////////
#define ON_ERROR_THROW(msg) \
	if ( GL_NO_ERROR !=  (e=glGetError()) )\
	{\
		glPopAttrib();\
		glPopMatrix();\
		throw SolveIt_ERROR(msg);\
	}
		//::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
		//wglMakeCurrent(0, 0);
///////////////////////////////////////////////////////////////////////////////
#define ON_ERROR_RETURN \
	if ( GL_NO_ERROR !=  (e=glGetError()) )\
	{\
		glPopAttrib();\
		glPopMatrix();\
		if (listEigenStates[j]) glDeleteLists(listEigenStates[j], 1);listEigenStates[j]=0;\
		throw SolveIt_ERROR(L"CQMStationary2D");\
	}
		//::ReleaseDC(pGLView->GetSafeHwnd(), hDC)
		//wglMakeCurrent(0, 0);

/*		ASSERT(0);\
		return;\
*/
///////////////////////////////////////////////////////////////////////////////
	const long N			= _N2;//16;//_N2a;
	const long S_NUMPOINTS	= N;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= N;
	const long T_ORDER		= S_ORDER;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;

///////////////////////////////////////////////////////////////////////////////
void FindMinMax(GLfloat	ctlpoints[N][N][3], float& fMinPotential, float& fMaxPotential)
{
	long x=0;
	long y=0;
	fMinPotential = fMaxPotential = ctlpoints[x][y][2];
	for ( x=0;x<N;x++)
		for ( y=0;y<N;y++)
		{
			if (fMinPotential > ctlpoints[x][y][2]) fMinPotential = ctlpoints[x][y][2];
			if (fMaxPotential < ctlpoints[x][y][2]) fMaxPotential = ctlpoints[x][y][2];
		}
}

/*
	static GLfloat	ctlpoints2[N][N][4];

	GLfloat	ctlpoints[N][N][3];
	float	sknots[S_NUMKNOTS];
	float	tknots[T_NUMKNOTS];

	static GLfloat	ctlpoints[N][N][3];
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];
*/

///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::DrawList(const long& j)
{
	return;//
	Lock lock(mutex);
	if (m_nNumFound==0) return;
//	if (psi_xy==0) return;
	if (listEigenStates==0) return;

	ASSERT(firstInlistEigenStates);
	if (firstInlistEigenStates==0) return;
//	ASSERT(listEigenStates[j]==0);
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	CGLView *pGLView = theApp.GetGLView();


# define use_NURBS
#if defined(use_NURBS)
	double	amplitude	= 0;
	double	iamplitude	= 0;

//	double *v	= (double *)DATA_2D_mp_PPOTENTIAL;
//	double *__potential		= (double *)DATA_2D_mp_PPOTENTIAL;

	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	const double	&y1	= pGLView->glFrustum_bottom;
	const double	&y2	= pGLView->glFrustum_top;
//	const double	&Z	= pGLView->m_fTransZ;
//	const long N			= _N2;//+1;
//	ASSERT(NDATA==N+1);
//	const long NDATA2 = NDATA*NDATA;
/*
	const long S_NUMPOINTS	= _N2;//+1;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= _N2;//+1;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;

	GLfloat	ctlpoints2[N][N][4];


	static GLfloat	ctlpoints[N][N][3];
	static float	sknots[S_NUMKNOTS];
	static float	tknots[T_NUMKNOTS];
*/

	GLfloat	ctlpoints[N][N][3];
	float	sknots[S_NUMKNOTS];
	float	tknots[T_NUMKNOTS];

	long x, y;
	long _x, _y;

#if 0
	for (x=0;x<S_NUMKNOTS;x++)
	{
	//	sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	//	sknots[x] = (x<S_ORDER)? 0.0f:((x<2*S_ORDER)? 1.0f:float(x-2*S_ORDER+2));
		long n = 1+S_NUMKNOTS/S_ORDER;
		for (long j=1;j<=n;j++)
		{
			if (x<=j*S_ORDER)
			{
				sknots[x] = float(j);
				break;
			}
		}
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
	//	tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	//	tknots[x] = (x<T_ORDER)? 0.0f:((x<2*T_ORDER)? 1.0f:float(x-2*T_ORDER+2));
		long n = 1+T_NUMKNOTS/T_ORDER;
		for (long j=1;j<=n;j++)
		{
			if (x<=j*T_ORDER)
			{
				tknots[x] = float(j);
				break;
			}
		}
	}
#endif

	for (x=0;x<S_NUMKNOTS;x++)
	{
		sknots[x] = (x<S_ORDER)? 0.0f:float(x-S_ORDER+1);
	//	sknots[x] = (x<S_ORDER)? 0.0f:((x<2*S_ORDER)? 1.0f:float(x-2*S_ORDER+2));
	}
	for (x=0;x<T_NUMKNOTS;x++)
	{
		tknots[x] = (x<T_ORDER)? 0.0f:float(x-T_ORDER+1);
	//	tknots[x] = (x<T_ORDER)? 0.0f:((x<2*T_ORDER)? 1.0f:float(x-2*T_ORDER+2));
	}

	double X=0, Y=0;

	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);
//	double _dx = w/(_N2a-1);
//	double _dy = h/(_N2a-1);
	double r = (_N2a-1)/(N-1);
//	double r = (_N2a)/(N);

	if (j < m_nNumFound)
	{
		FindMinMaxPsi(j);
		amplitude = m_fMaxPsi - m_fMinPsi;
		if (fabs(amplitude) < 0.01e-4)
		{
	//		//wglMakeCurrent(0, 0);
	//		::ReleaseDC(pGLView->m_hWnd, hDC);
			throw SolveIt_ERROR(L"fabs(amplitude) < 0.01e-4");
	//		return;
		}
	}
	else if (j == m_nNumFound)
	{
			throw SolveIt_ERROR(L"j == m_nNumFound");
		amplitude = m_fMaxPotential - m_fMinPotential;
	}
	iamplitude = 1.0/amplitude;

//	m_fScaleNurbs		= w*h;
//	m_fScaleNurbs		= 54.150375;
//	m_fScaleNurbs		= 12;
//	double *__psi_xy		= (double *)DATA_2D_mp_PPSI__XY;
	for (x=0;x<N;x++)
	{
		X = x1 + x*dx;
		_x=(N==_N2a)? x:long(r*x);
		for (y=0;y<N;y++)
		{
			Y = y1 + y*dy;
			_y=(N==_N2a)? y:long(r*y);
			ctlpoints[x][y][0] = float(X);
			ctlpoints[x][y][1] = float(Y);
		//	if (j < m_nNumFound)//
			{
				ctlpoints[x][y][2] = float(iamplitude*(psi_x[j][_x*NDATA+_y]-m_fMinPsi) );
			//	ctlpoints[x][y][2] = float(iamplitude*(psi_xy(j, _x, _y)-m_fMinPsi) );
//				ctlpoints[x][y][2] = float(iamplitude*(psi_xy[j*NDATA2+x*NDATA+y]-m_fMinPsi) );
			//	ctlpoints[x][y][2] = float(iamplitude*(psi_xy(j, _x, _y)-m_fMinPsi) );
			//	ctlpoints[x][y][2] = float(2*(iamplitude*(psi_xy(j, x, y)-m_fMinPsi)-0.5) );

			//	ctlpoints[x][y][2] = float(m_fScaleNurbs*fabs(psi_xy(j, _x, _y)-m_fMinPsi) );

			//	ctlpoints[x][y][2] = float(1*fabs(psi_xy(j, _x, _y)) );
			//	ctlpoints[x][y][2] = float( m_fScaleNurbs*(psi_xy(j, _x, _y)) );

			//		ctlpoints[x][y][2] = float(m_fScaleNurbs*fabs(psi_xy(j, x, y)-m_fMinPsi) );

//				ctlpoints[x][y][2] = float(1*fabs(iamplitude*(psi_xy(j, _x, _y)-m_fMinPsi)) );
			//	ctlpoints[x][y][2] = float(m_fScaleNurbs*((psi_xy(j, _x, _y)-m_fMinPsi)) );
			//	ctlpoints[x][y][2] = float(m_fScaleNurbs*((psi_xy(j, _x, _y))) );

			//	ctlpoints[x][y][2] = float(m_fScaleNurbs*2*(iamplitude*(psi_xy(j, x, y)-m_fMinPsi) - 0.5) );
			//	ctlpoints[x][y][2] = float(iamplitude*(psi_xy[(j)*_N2aN2a+(x)*_N2a+(y)]-m_fMinPsi) );
			}
#if 0
			else if (j == m_nNumFound)
			{
				ctlpoints[x][y][2] = float((potential(x, y)));
/*
				ctlpoints[x][y][2] = float(iamplitude*(potential(x, y) - m_fMinPotential));
				ctlpoints2[x][y][0] = 1.0f - ctlpoints[x][y][2];
				ctlpoints2[x][y][2] = float(X);
				ctlpoints2[x][y][3] = float(Y);
				ctlpoints2[x][y][1] = ctlpoints[x][y][2];//float(fabs(sqrt(X*X+Y*Y)));//float(fabs(cos(X*Y)));

				ctlpoints2[x][y][0] = ctlpoints[x][y][2];
				ctlpoints2[x][y][2] = float(X); - float(2*Y)
				ctlpoints2[x][y][3] = float(Y);
				ctlpoints2[x][y][1] = 1.0f - ctlpoints[x][y][2];//float(fabs(sqrt(X*X+Y*Y)));//float(fabs(cos(X*Y)));
*/
				ctlpoints2[x][y][0] = 1.0f - ctlpoints[x][y][2];
				ctlpoints2[x][y][2] = float(X);
				ctlpoints2[x][y][3] = 1.0f;
				ctlpoints2[x][y][1] = ctlpoints[x][y][2];//float(fabs(sqrt(X*X+Y*Y)));//float(fabs(cos(X*Y)));
			}
#endif
		}
	}
//m_fScaleNurbs*
#endif	// defined(use_NURBS)



//	HDC hDC = ::GetDC(pGLView->m_hWnd);
//	HDC hDC = pGLView->GetDC()->GetSafeHdc();//pGLView->m_hdcMem
//	HDC hDC = pGLView->m_pDC->GetSafeHdc();
//	HDC hDC = CreateCompatibleDC(pGLView->GetDC()->GetSafeHdc());
	//wglMakeCurrent(hDC, pGLView->m_hRC);
//	if(FALSE == ::wglMakeCurrent(hDC, pGLView->m_hRC))
//	{
//		::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
//		throw SolveIt_ERROR(L"Unable to //wglMakeCurrent");
//	//	return;
//	}
//	//wglMakeCurrent(0, 0);
//	::ReleaseDC(pGLView->m_hWnd, hDC);

	GLenum e=GL_NO_ERROR;
	ASSERT(listEigenStates[j]	== firstInlistEigenStates + j);
	glNewList(listEigenStates[j], GL_COMPILE);
//	glNewList(listEigenStates[j], GL_COMPILE_AND_EXECUTE);

	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		//wglMakeCurrent(0, 0);
	//	::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
		throw SolveIt_ERROR(L"Unable to glNewList");
	//	ASSERT(0);
	//	return;
	}

//	DrawHull(j);

	
#if defined(use_NURBS)

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);


	const float f=0.50f;

	if (j < m_nNumFound)
		glColor3d(0.50*fabs(sin(2*j+1)),0.50*fabs(cos(j)),0.50*fabs(sin(j+1)));
	else if (j == m_nNumFound)
		glColor3f( f, f, f);

	GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	ON_ERROR_RETURN
///////////////////////////////////////////////////////////////////////////////
	glDisable(GL_TEXTURE_2D);
	ON_ERROR_RETURN

	GLUnurbsObj* aNurbs = gluNewNurbsRenderer();
	ON_ERROR_RETURN
	gluNurbsProperty(aNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	ON_ERROR_RETURN
	gluNurbsProperty(aNurbs, GLU_DISPLAY_MODE, GLU_FILL);
	ON_ERROR_RETURN
//	gluNurbsProperty(aNurbs, GLU_CULLING , GL_TRUE);

	gluNurbsCallback(aNurbs, GLU_ERROR,(GLvoid (__stdcall *)()) nurbsError);
	ON_ERROR_RETURN
	gluBeginSurface(aNurbs);
	ON_ERROR_RETURN
#if 10
	gluNurbsSurface(aNurbs,
					S_NUMKNOTS, sknots,
					T_NUMKNOTS, tknots,
					T_NUMPOINTS*3, 3,
					&ctlpoints[0][0][0],//reinterpret_cast<float *>(ctlpoints),
					S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);
#else
	{
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLfloat ctlpoints[4][4][3];
{
for (int u = 0; u < 4; u++) {
for (int v = 0; v < 4; v++) {
ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);
if ( (u == 1 || u == 2) && (v == 1 || v == 2))
ctlpoints[u][v][2] = 3.0;
else
ctlpoints[u][v][2] = -3.0;
}
}
}

gluNurbsSurface(aNurbs,
8, knots, 8, knots,
4 * 3, 3, &ctlpoints[0][0][0],
4, 4, GL_MAP2_VERTEX_3);
	}
#endif
	ON_ERROR_RETURN
#if 0
	if (j == m_nNumFound)
	{
/*			glEnable(GL_NORMALIZE);

			gluNurbsSurface(aNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*3, 3,
							&ctlpoints[0][0][0], S_ORDER, T_ORDER, GL_MAP2_NORMAL);
			if ( GL_NO_ERROR !=  (e=glGetError()) )
			{
				ASSERT(0);
				return;
			}
*/
	//	gluNurbsSurface(aNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*3, 3,
	//					&ctlpoints[0][0][0], S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_3);
		gluNurbsSurface(aNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*4, 4,
							&ctlpoints2[0][0][0], S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_4);
		ON_ERROR_RETURN
	}
#endif
//	gluBeginTrim (aNurbs);
//	gluPwlCurve (aNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (aNurbs);

	float fMinPotential=0.0f,  fMaxPotential=0.0f;
	FindMinMax(	ctlpoints,  fMinPotential,  fMaxPotential);

try{
	gluEndSurface(aNurbs);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		throw SolveIt_ERROR(L"Unable to gluEndSurface(aNurbs)");
	//	throw SolveIt_ERROR(L"Unable to gluEndSurface(aNurbs)", THIS_FILE, __LINE__);
	}
	ON_ERROR_RETURN
	gluDeleteNurbsRenderer(aNurbs);
	ON_ERROR_RETURN

#if 0
	{
	const float f=0.50f;
	glColor3f( f, f, f);
	GLUnurbsObj*	theNurbs	= gluNewNurbsRenderer();

	gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurbs, GLU_DISPLAY_MODE, GLU_FILL);

	gluBeginSurface(theNurbs);
	ON_ERROR_RETURN

	gluNurbsSurface(theNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*3, 3,
					&ctlpoints2[0][0][0], S_ORDER, T_ORDER, GL_MAP2_TEXTURE_COORD_4);
	ON_ERROR_RETURN
	gluNurbsSurface(theNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*3, 3,
					&ctlpoints2[0][0][0], S_ORDER, T_ORDER, GL_MAP2_NORMAL);
	ON_ERROR_RETURN
	gluNurbsSurface(theNurbs, S_NUMKNOTS, sknots, T_NUMKNOTS, tknots, T_NUMPOINTS*3, 3,
					&ctlpoints2[0][0][0], S_ORDER, T_ORDER, GL_MAP2_VERTEX_3);
	ON_ERROR_RETURN
//	gluBeginTrim (theNurbs);
//	gluPwlCurve (theNurbs, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
//	gluEndTrim (theNurbs);

	gluEndSurface(theNurbs);
	ON_ERROR_RETURN
	gluDeleteNurbsRenderer(theNurbs);
	ON_ERROR_RETURN
	}
#endif
///////////////////////////////////////////////////////////////////////////////

#if defined(__use_GL_TEXTURE_2D)
	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
#endif

	glPopAttrib();
	glPopMatrix();

#endif	// defined(use_NURBS)

	glFlush();
	glEndList();
	ON_ERROR_RETURN
	//wglMakeCurrent(0, 0);
//	::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
///////////////////////////////////////////////////////////////////////////////
#undef ON_ERROR_RETURN
///////////////////////////////////////////////////////////////////////////////
double CQMStationary2D::getPotential(int x, int y)
{
//	double *v	= (double *)DATA_2D_mp_PPOTENTIAL;
//	double *__potential		= (double *)DATA_2D_mp_PPOTENTIAL;
	return potential(x, y);
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::DefinePotential()
{
	Lock lock(mutex);
try
{
//	double *v	= (double *)DATA_2D_mp_PPOTENTIAL;
//	double *__potential		= (double *)DATA_2D_mp_PPOTENTIAL;
//	double*const & __potential		= (double *)DATA_2D_mp_PPOTENTIAL;
//	ZeroMemory(v, NDATA*NDATA*sizeof(double));
	for (long x=0;x<NDATA;x++)
		for (long y=0;y<NDATA;y++)
		{
			potential(x, y) = 0;
		}
	CGLView *pGLView = theApp.GetGLView();
	const double	&x1	= pGLView->glFrustum_left;
	const double	&x2	= pGLView->glFrustum_right;
	const double	&y1	= pGLView->glFrustum_bottom;
	const double	&y2	= pGLView->glFrustum_top;
	double Width	= x2 - x1;
	double Height	= y2 - y1;
	double dx		= Width/(NDATA-1);
	double dy		= Height/(NDATA-1);
	double X=0, Y=0, Z=0;


	CSystem* m_system	= theApp.GetSystem();
	vector<double> args;
			{
				args.push_back(0.0);args.push_back(5.0);//args.push_back(Z);
				LoadAndCallRealFunc(&m_system->m_uUserDefinedPotential, args);
				Z = CStackMachine::receiveStackTopDoubleValue;//163.5
				ASSERT(Z==163.5);
				args.push_back(4.0);args.push_back(0.0);//args.push_back(Z);
				LoadAndCallRealFunc(&m_system->m_uUserDefinedPotential, args);
				Z = CStackMachine::receiveStackTopDoubleValue;//0.0
				ASSERT(Z==0.0);
				args.push_back(0.0);args.push_back(0.0);//args.push_back(Z);
				LoadAndCallRealFunc(&m_system->m_uUserDefinedPotential, args);
				Z = CStackMachine::receiveStackTopDoubleValue;//1
				ASSERT(Z==1.0);
			}


if (1)//m_system->m_bUserDefinedPotentialIsValid)
{
	for(long x=0;x<NDATA;x++)
	{
		X = x1 + x*dx;
		for(long y=0;y<NDATA;y++)
		{
			Y = y1 + y*dy;
			args.clear();
			if (m_system->m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				args.push_back(m_system->m__t);
				args.push_back(X);args.push_back(Y);//args.push_back(Z);
				if (m_system->m_uUserDefinedPotential.nargs==3) args.push_back(Z);
				LoadAndCallRealFunc(&m_system->m_uUserDefinedPotential, args);
				potential(x, y) = CStackMachine::receiveStackTopDoubleValue;
			}
			else
			{
				args.push_back(X);args.push_back(Y);//args.push_back(Z);
				if (m_system->m_uUserDefinedPotential.nargs==3) args.push_back(Z);
				LoadAndCallRealFunc(&m_system->m_uUserDefinedPotential, args);
			//	Z = CStackMachine::receiveStackTopDoubleValue;
				potential(x, y) = CStackMachine::receiveStackTopDoubleValue;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
	else
	{
#define SCALE(y)	(2*(((double(y))/NDATA - 0.5)))
		try
		{
			for (long x=0;x<NDATA;x++)
			{
				for (long y=0;y<NDATA;y++)
				{
					double X = SCALE(x)/2/0.135;
					double Y = SCALE(y)/2/0.135;
					potential(x, y) = m_fScalePotential*exp(-(X*X+Y*Y))*cos(X*X+Y*Y);//*cos(X*Y)*sin(X*X+Y*Y);
				}
			}
		}
		catch( SolveIt_Error& e){e.why();}catch(...)
		{
			::MessageBox(NULL,_T("CQMStationary2D::DefinePotential"),_T("General Exception"),MB_OK);
			return;
		}
#undef SCALE
	}
//	for ( x=0;x<NDATA;x++)
//		for (long y=0;y<NDATA;y++)
//			potential(x, y) *= m_fScalePotential;
//	FindMinMaxPotential();
//	g_fLambda	= m_fLambda;
//	g_v			= v;
//	m_system->CreateTexture();
	m_fMinPotential = m_fMaxPotential = potential(0, 0);
	for (auto x=0;x<NDATA;x++)
	{
		for (long y=0;y<NDATA;y++)
		{
			if (m_fMinPotential > potential(x, y)) m_fMinPotential = potential(x, y);
			if (m_fMaxPotential < potential(x, y)) m_fMaxPotential = potential(x, y);
		}
	}
	X=m_fMaxPotential;
	Y=m_fMinPotential;
	Z=m_fMinPotential;
}
catch( SolveIt_Error& e){e.why();}catch(...) {
	ASSERT(0);
}
}
///////////////////////////////////////////////////////////////////////////////
//#undef potential
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::FindMinMaxPsi(long j)
{
//	const long NDATA2 = NDATA*NDATA;
//	double *__psi_xy		= (double *)DATA_2D_mp_PPSI__XY;
	if (m_nNumFound==0) return;
	if (j >= m_nNumFound) return;
//	if (psi_x==0) return;
//	if (psi_xy==0) return;
//	m_fMinPsi = m_fMaxPsi = psi_x[j][0];
	m_fMinPsi = m_fMaxPsi = psi_xy(j, 0, 0);//psi_xy[j*NDATA2+0];
	long index=0;
	for (long x=0;x<NDATA-1;x++)
		for (long y=0;y<NDATA-1;y++)
		{
		//	index=x*NDATA+y;
		//	if (m_fMinPsi > psi_x[j][index]) m_fMinPsi = psi_x[j][index];
		//	if (m_fMaxPsi < psi_x[j][index]) m_fMaxPsi = psi_x[j][index];
		//	index=j*NDATA2+x*NDATA+y;
		//	if (m_fMinPsi > psi_xy[index]) m_fMinPsi = psi_xy[index];
		//	if (m_fMaxPsi < psi_xy[index]) m_fMaxPsi = psi_xy[index];
			if (m_fMinPsi > psi_xy(j, x, y)) m_fMinPsi = psi_xy(j, x, y);
			if (m_fMaxPsi < psi_xy(j, x, y)) m_fMaxPsi = psi_xy(j, x, y);
		}
	return;
	if (psi_x==0) return;
	{
	m_fMinPsi = m_fMaxPsi = psi_x[j][0];
	long index=0;
	for (long x=0;x<NDATA-1;x++)
		for (long y=0;y<NDATA-1;y++)
		{
			index=x*NDATA+y;
			if (m_fMinPsi > psi_x[j][index]) m_fMinPsi = psi_x[j][index];
			if (m_fMaxPsi < psi_x[j][index]) m_fMaxPsi = psi_x[j][index];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
BOOL CQMStationary2D::InitInstance()
{
//	SetEvent(g_EventHandle);
//	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int CQMStationary2D::ExitInstance()
{
	// TODO:  perform any per-thread cleanup herem_system->m_pCQMStationary1D->m_system->m_pCQMStationary1D->
	CSystem* m_system	= theApp.GetSystem();
	CloseHandle (m_hThread);
	m_hThread		= 0;
	m_system->data__QM_Stationary2D->m_pCQMStationary2D		= 0;
	return CWinThread::ExitInstance();
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CQMStationary2D, CWinThread)
	//{{AFX_MSG_MAP(CQMStationary2D)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::OnStepDone(WPARAM wParam, LPARAM lParam) 
{ 
	CSystem* m_system	= theApp.GetSystem();
	m_system->UpdateTree();
}
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::OnSetParam(WPARAM wParam, LPARAM lParam)
{
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::OnStep(WPARAM wParam, LPARAM lParam) 
{ 
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	pFrame->PostMessage(WM_USER_Message, 0, (LPARAM) GetWC("Running ..."));//SetStatusBarText("Running ...", 2);

	DefinePotential();
//	EigenStates();//arpack
	EigenStatesTRLan();
///////////////////////////////////////////////////////////////////////////////
	CGLView *pGLView = theApp.GetGLView();
	ASSERT( pGLView );
	ASSERT( GLViewRenderCallback==&GLViewRenderCallback );

if (pGLView && pGLView->notifications_mutex.wait(500))
{
	pGLView->notifications.clear();
	pGLView->notifications.push_back(&CQMStationary2D::GLViewNotificationCallback);
	pGLView->notifications_mutex.Release();
}

if (pGLView && pGLView->render_actions_mutex.wait(500))
{
	pGLView->render_actions.clear();
	pGLView->render_actions.push_back(&CQMStationary2D::GLViewRenderCallback);
	pGLView->render_actions_mutex.Release();
}
	Fire_ViewChange();

	PostThreadMessage(WM_USER_PASS_DONE, 0, 0); 
	SetCursor(LoadCursor(NULL, IDC_ARROW));
//	return 0; 
} 
///////////////////////////////////////////////////////////////////////////////
void CQMStationary2D::OnHalt(WPARAM wParam, LPARAM lParam) 
{ 
	PostQuitMessage(0); 
//	return 0; 
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

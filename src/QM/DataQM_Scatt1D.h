// DataQM_Scatt1D.h: interface for the CDataQM_Scatt1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAQM_SCATT1D_H__AED1C4EF_88CE_4206_8EE0_487A83632AC8__INCLUDED_)
#define AFX_DATAQM_SCATT1D_H__AED1C4EF_88CE_4206_8EE0_487A83632AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "vfw.h"

#include "QM_1DScatt.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
typedef complex<double> cdouble;
typedef vector<cdouble> ComplexData;

class CDataQM_Scatt1D  
{
public:
	CDataQM_Scatt1D();
	virtual ~CDataQM_Scatt1D();
	void CreateQM_Scatt1DThread(bool bCREATE_SUSPENDED = false);
	BOOL StopQM_Scatt1D(void);
///////////////////////////////////////////////////////////////////////////////
	int get_FourierTransformOfWavePacketAtK(double k0, double k, /*[out, retval]*/ double *pVal);
	int get_MaximumWaveNumber(/*[out, retval]*/ double *pVal);
	int get_CenterOfGaussian(/*[out, retval]*/ double *pVal);
	int put_CenterOfGaussian(/*[in]*/ double newVal);
	int get_InitialSpread(/*[out, retval]*/ double *pVal);
	int put_InitialSpread(/*[in]*/ double newVal);
	int get_ScalePsi(/*[out, retval]*/ double *pVal);
	int put_ScalePsi(/*[in]*/ double newVal);
	int get_ScalePotential(/*[out, retval]*/ double *pVal);
	int put_ScalePotential(/*[in]*/ double newVal);
	int get_Lambda(/*[out, retval]*/ double *pVal);
	int put_Lambda(/*[in]*/ double newVal);
	int get_WaveNumber(/*[out, retval]*/ double *pVal);
	int put_WaveNumber(/*[in]*/ double newVal);
	int get_EvolutionMethod(/*[out, retval]*/ long *pVal);
	int put_EvolutionMethod(/*[in]*/ long newVal);
	int get_Renormalize(/*[out, retval]*/ long *pVal);
	int put_Renormalize(/*[in]*/ long newVal);
	int get_NumberOfFrames(/*[out, retval]*/ long *pVal);
	int put_NumberOfFrames(/*[in]*/ long newVal);

	int get_NumberOfIntermediateSteps(/*[out, retval]*/ long *pVal);
	int put_NumberOfIntermediateSteps(/*[in]*/ long newVal);
	
///////////////////////////////////////////////////////////////////////////////
//	enum EvolutionMethod { Nash, Cayley, PeriodicCayley, LastEvolutionMethod } m_EvolutionMethod;
//	int& m_EvolutionMethod;
	enum DrawSource { Initial, Current, External } m_DrawSource;
///////////////////////////////////////////////////////////////////////////////
	long  mu(long i, long N);
/////////////////////////////////////////////////////////////////////////////
double bessj0(double x)
{
	double ax,z;
	double xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {
		y=x*x;
		ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7
			+y*(-11214424.18+y*(77392.33017+y*(-184.9052456)))));
		ans2=57568490411.0+y*(1029532985.0+y*(9494680.718
			+y*(59272.64853+y*(267.8532712+y*1.0))));
		ans=ans1/ans2;
	} else {
		z=8.0/ax;
		y=z*z;
		xx=ax-0.785398164;
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4
			+y*(-0.2073370639e-5+y*0.2093887211e-6)));
		ans2 = -0.1562499995e-1+y*(0.1430488765e-3
			+y*(-0.6911147651e-5+y*(0.7621095161e-6
			-y*0.934935152e-7)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
	}
	return ans;
}
/////////////////////////////////////////////////////////////////////////////
double bessj1(double x)
{
	double ax,z;
	double xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {
		y=x*x;
		ans1=x*(72362614232.0+y*(-7895059235.0+y*(242396853.1
			+y*(-2972611.439+y*(15704.48260+y*(-30.16036606))))));
		ans2=144725228442.0+y*(2300535178.0+y*(18583304.74
			+y*(99447.43394+y*(376.9991397+y*1.0))));
		ans=ans1/ans2;
	} else {
		z=8.0/ax;
		y=z*z;
		xx=ax-2.356194491;
		ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4
			+y*(0.2457520174e-5+y*(-0.240337019e-6))));
		ans2=0.04687499995+y*(-0.2002690873e-3
			+y*(0.8449199096e-5+y*(-0.88228987e-6
			+y*0.105787412e-6)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
		if (x < 0.0) ans = -ans;
	}
	return ans;
}
/////////////////////////////////////////////////////////////////////////////

#define ACC 40.0
#define BIGNO 1.0e10
#define BIGNI 1.0e-10

double bessj(int n, double x)
{
	int j,jsum,m;
	double ax,bj,bjm,bjp,sum,tox,ans;

	if (n == 0) return bessj0(x);
	if (n == 1) return bessj1(x);
	ax=fabs(x);
	if (ax == 0.0)
		return 0.0;
	else if (ax > (double) n) {
		tox=2.0/ax;
		bjm=bessj0(ax);
		bj=bessj1(ax);
		for (j=1;j<n;j++) {
			bjp=j*tox*bj-bjm;
			bjm=bj;
			bj=bjp;
		}
		ans=bj;
	} else {
		tox=2.0/ax;
		m=2*((n+(int) sqrt(ACC*n))/2);
		jsum=0;
		bjp=ans=sum=0.0;
		bj=1.0;
		for (j=m;j>0;j--) {
			bjm=j*tox*bj-bjp;
			bjp=bj;
			bj=bjm;
			if (fabs(bj) > BIGNO) {
				bj *= BIGNI;
				bjp *= BIGNI;
				ans *= BIGNI;
				sum *= BIGNI;
			}
			if (jsum) sum += bj;
			jsum=!jsum;
			if (j == n) ans=bjp;
		}
		sum=2.0*sum-bj;
		ans /= sum;
	}
	return  x < 0.0 && n%2 == 1 ? -ans : ans;
}

#undef ACC
#undef BIGNO
#undef BIGNI
/////////////////////////////////////////////////////////////////////////////
// f(x) = (1/TwoPiToTheOneFourth)*[1/sqrt(a)]*e^(i k0 x)*e^[- ((x-x0)/2a)^2]
cdouble GaussianWavePacket(double a, double x0, double k0, double x);
/////////////////////////////////////////////////////////////////////////////
// F(k) = [2/Pi]^(1/4) sqrt(a)*e^[-((k0+k)*a)^2]*e^(i k x0)
cdouble FourierTransformGaussianWavePacket(double a, double x0, double k0, double k);
///////////////////////////////////////////////////////////////////////////////
/* for description of method see [Engeln-Mullges + Uhlig, p. 92]
 *
 *     diag[0]  offdiag[0]             0   .....
 *  offdiag[0]     diag[1]    offdiag[1]   .....
 *           0  offdiag[1]       diag[2]
 *           0           0    offdiag[2]   .....
 */
int solve_tridiag(
  const cdouble diag[], size_t d_stride,
  const cdouble offdiag[], size_t o_stride,
  const cdouble b[], size_t b_stride,
  cdouble x[], size_t x_stride,
  size_t N);
/////////////////////////////////////////////////////////////////////////////
/* for description of method see [Engeln-Mullges + Uhlig, p. 96]
 *
 *      diag[0]  offdiag[0]             0   .....  offdiag[N-1]
 *   offdiag[0]     diag[1]    offdiag[1]   .....
 *            0  offdiag[1]       diag[2]
 *            0           0    offdiag[2]   .....
 *          ...         ...
 * offdiag[N-1]         ...
 *
 */
int solve_cyc_tridiag(
  const cdouble diag[], size_t d_stride,
  const cdouble offdiag[], size_t o_stride,
  const cdouble b[], size_t b_stride,
  cdouble x[], size_t x_stride,
  size_t N);
/////////////////////////////////////////////////////////////////////////////
	void InitialWaveFunction();
	void INIT(void);
//	void SETUP(void);
	void OnResetViewSource(void);
	void DefineInitialWavefunction();
	void DefinePotential();
	void DefinePotentialDependentParameters();
	void Draw(int nFrameNum=0);
	
	int OnCloseMovie();
	int OnCompute();
	int get_HDF5FileName(/*[out, retval]*/ wchar_t* *pVal);
	int put_HDF5FileName(/*[in]*/ wchar_t* newVal);
	int OnLButtonUP(/*[in]*/ long wParam, /*[in]*/ long lParam, const RECT& _rc);
	int OnLButtonDown(/*[in]*/ long wParam, /*[in]*/ long lParam);
	int OnMouseMove(/*[in]*/ long wParam, /*[in]*/ long lParam);
	int Destroy();
	int Step();
/////////////////////////////////////////////////////////////////////////////
	const int NumOrder;
//	long NDATA;
//	long RENORMALIZE;

	double	*	Jn;//[NumOrder+1];
	cdouble	*	imM;//[NumOrder+1];
	cdouble	*	im;
	CQM_Scatt1D*	m_pCQM_Scatt1D;
	double			m_fWaveNumber_x;
	double			m_fScalePsi;
	double			m_fScalePotential;
	double			m_fInitialGaussianSpread;
	double			m_fLambda;
	double			m_fInitialGaussianCenter_x;
//	long& m_NDATA;
	long			m_NDATA;
	long			m_quantumModeSuchThatKEequalsPE;
	long			m_nNumberOfIntermediateSteps;
	long			m_nNumFrames;
	long			m_nRenormalize;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	double  * v;
	cdouble * u;
	cdouble * psi_xt;
	cdouble * psi_xt0;
	cdouble * ev;
	cdouble * evc;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void Clear(void);
	void Reset(void);
///////////////////////////////////////////////////////////////////////////////
	void EVOLVE_NASH(long StepsPerIteration);
///////////////////////////////////////////////////////////////////////////////
	void EVOLVE_CAYLEY(long StepsPerIteration);
	void EVOLVE_CAYLEY_PERIODIC(long StepsPerIteration);
///////////////////////////////////////////////////////////////////////////////
	void Renormalize(cdouble*psi);
///////////////////////////////////////////////////////////////////////////////
	typedef long (CQM_Scatt1D::*DataPump) (long& k);
	typedef long (CQM_Scatt1D::*ProcessData) (long& k, void* dataAttribute);
	void WriteData1D(const wchar_t* HDF5FileName, const wchar_t* szGroupTag, const wchar_t* szAttributeTag,
			  long NX, void* dataAttribute,
			  const void* data,
			  DataPump CallMeToGetMoreData
			  );
	void WriteData1D(const wchar_t* HDF5FileName, long NX,
			  const void* data,
			  DataPump CallMeToGetMoreData
			  );
	long PumpData(long& k);
///////////////////////////////////////////////////////////////////////////////
//	long DrawData(long& k, cdouble* psi);//, int nFrameNum
	long DoSomething(long& k, void* dataAttribute);
///////////////////////////////////////////////////////////////////////////////
	void ReadData1D(const wchar_t* HDF5FileName, long NX, ProcessData doSomething);
	std::wstring m_bstrHDF5FileName;
//typedef int hid_t;
	int	file_idHDF5;
	pair<int, std::wstring> PAIR_fileId_FileName;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool				g_bInitCalled;
	IAVIStream*			g_pavi;	
	IAVIStream*			g_psCompressed;	
	IAVIFile*			g_pfile;

	AVICOMPRESSOPTIONS	g_opts;
	AVICOMPRESSOPTIONS* g_aopts[1];//= {&opts};

//	void*	g_pavi;	
//	void*	g_psCompressed;	
//	void*	g_pfile;

	void AVI_Term();
	long AVI_Init(const LPBITMAPINFO pbmi, std::wstring bstr_AVI_FileName);
///////////////////////////////////////////////////////////////////////////////
	void AVIStreamWrite();
///////////////////////////////////////////////////////////////////////////////

	HDC m_hdcMemAVI;
	HBITMAP m_hBitmapAVI;
	HBITMAP m_hBitmapOldAVI;
	BYTE* m_pBits;
	std::wstring m_bstrAVIFileName;
	bool m_bMakingAVI;
	void* m_psCompressed;		// stream pointer
	LONG m_timeOfThisFrame;		// time of this frame
	LONG m_nImageSize;			// size of this frame
///////////////////////////////////////////////////////////////////////////////
};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DATAQM_SCATT1D_H__AED1C4EF_88CE_4206_8EE0_487A83632AC8__INCLUDED_)

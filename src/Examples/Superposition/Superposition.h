// Superposition.h : Declaration of the CSuperposition

#ifndef __SUPERPOSITION_H_
#define __SUPERPOSITION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
/*
#include "QMEvolve.h"
for
typedef complex<double> cdouble;
typedef vector<cdouble> ComplexData;
*/
/////////////////////////////////////////////////////////////////////////////
#include <complex>

///////////////////////////////////////////////////////////////////////////////
namespace SolveIt {
///////////////////////////////////////////////////////////////////////////////
typedef vector<double> RealData;
typedef complex<double> cdouble;
typedef vector<cdouble> ComplexData;

struct _RealVertex
{
	double x, y, z;
};
typedef vector<_RealVertex> VertexData;
typedef vector<VertexData*> FamilyOfCurves;

class CGLView;
class CWaveNumbers;
/////////////////////////////////////////////////////////////////////////////
// CSuperposition
class CSuperposition
{
public:
	CSuperposition(CGLView*);
	virtual ~CSuperposition();


// ISuperposition
public:
	int OnUpdate(CWaveNumbers* pwn);
	int SetViewPointer(long pointer);
	int Hide();
	int Show();
	int Destroy();
	int Initialize();
//	int AdviseSource();
	int Calculate();

	int SetParameters(
							long newNumberOfWaveNumbers,
							double newWavelength,
							double newRatio,
							double newDeltaTime	);


	int get_DeltaTime(double *pVal);
	int put_DeltaTime(double newVal);
	int get_Ratio(double *pVal);
	int put_Ratio(double newVal);
	int get_Wavelength( double *pVal);
	int put_Wavelength(double newVal);
	int get_NumberOfWaveNumbers(long *pVal);
	int put_NumberOfWaveNumbers(long newVal);
	int OnResetParameters();
	int OnViewReset();
	int OnEditNumberOfWaveNumbers();
	bool m_bMouseDown;

	double m_Wavelength; 
	double m_dt; 
	double m_Ratio; 
	long m_NumberOfWaveNumbers; 
	CGLView* m_pCView;
	
	RealData m_WaveNumbers;
	ComplexData m_PlaneWaves;
	RealData P;
//typedef vector<vertex> VertexData;
//typedef vector<VertexData*> FamilyOfCurves;
	FamilyOfCurves m_curves;

// Operations
public:
	void Draw();

	cdouble PlaneWave(double k, double x, double t)
	{
		const cdouble i(0,1);
		cdouble c = exp(i*(k*x)) * exp( i*(-0.5*k*k*t) );
		return c;
	}
	void ClearCurves();
	void CalculateCurves();
	void Reset();
	void OnNewWaveNumbers();
};

} // namespace SolveIt

#endif //__SUPERPOSITION_H_

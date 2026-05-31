// QMEvolve1D.h: interface for the CQMEvolve1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMEVOLVE1D_H__E700D72B_6F15_4604_9023_CBCD6C6F9180__INCLUDED_)
#define AFX_QMEVOLVE1D_H__E700D72B_6F15_4604_9023_CBCD6C6F9180__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//const int NumOrder=8;
#include "QMEvolve.h"
/////////////////////////////////////////////////////////////////////////////

namespace SolveIt {

class CQMEvolve1D : public CQMEvolve
{
public:
	CQMEvolve1D(long _NDATA) :
		CQMEvolve(_NDATA),
		v(new double[NDATA]),
		ev(new cdouble[NDATA]),
		evc(new cdouble[NDATA]),
		u(new cdouble[NDATA]),
		psi_xt(new cdouble[NDATA]),
		psi_xt0(new cdouble[NDATA]),
	m_fScalePotential(0.1)
	{
try
{
		INIT();
}
catch( SolveIt_Error& e){e.why();}catch(...)      // Handle all exceptions
{
	ASSERT(0);
	throw;      // Pass exception to some other handler
}
	}
	virtual ~CQMEvolve1D()
	{
try
{
		delete[]ev;
		delete[]evc;
		delete[]v;
		delete[]u;
		delete[]psi_xt;
		delete[]psi_xt0;
}
catch( SolveIt_Error& e){e.why();}catch(...)      // Handle all exceptions
{
	ASSERT(0);
}
	}

	double  * v;
	cdouble * u;
	cdouble * psi_xt;
	cdouble * psi_xt0;
	cdouble * ev;
	cdouble * evc;
	double		m_fScalePotential;
/////////////////////////////////////////////////////////////////////////////
	void INIT(void);
//	void SETUP(void);
	void OnResetViewSource(void);
	void DefineInitialWavefunction();
	void DefinePotential();
	void DefinePotentialDependentParameters();
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	void EVOLVE_NASH(long StepsPerIteration);
/////////////////////////////////////////////////////////////////////////////
	void EVOLVE_CAYLEY(long StepsPerIteration);
	void EVOLVE_CAYLEY_PERIODIC(long StepsPerIteration);
/////////////////////////////////////////////////////////////////////////////

};

} // namespace SolveIt

#endif // !defined(AFX_QMEVOLVE1D_H__E700D72B_6F15_4604_9023_CBCD6C6F9180__INCLUDED_)

// DataQM_Stationary1D.h: interface for the CDataQM_Stationary1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAQM_STATIONARY1D_H__483F654F_0372_489C_B92E_A706EDCA2441__INCLUDED_)
#define AFX_DATAQM_STATIONARY1D_H__483F654F_0372_489C_B92E_A706EDCA2441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "qmstationary1d.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

class CDataQM_Stationary1D  
{
public:
	CDataQM_Stationary1D();
	virtual ~CDataQM_Stationary1D();
	void CreateQMStationary1DThread(bool bCREATE_SUSPENDED = false);
	BOOL StopQMStationary1D(void);
///////////////////////////////////////////////////////////////////////////////
	long eigenvalueIndex;
	double energy_eigenvalue;
	CQMStationary1D*	m_pCQMStationary1D;
	double M;

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DATAQM_STATIONARY1D_H__483F654F_0372_489C_B92E_A706EDCA2441__INCLUDED_)

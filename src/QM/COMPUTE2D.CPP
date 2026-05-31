// Compute2D.cpp : implementation of the CQM_2DScatt class
//
#include "stdafx.h"
#include "Action.h"
#include "GLView.h"
#include "QM_2DScatt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "SMutex.h"
//winmm.lib 
//#define WIDTHBYTES(bits) ((((bits) + 31) / 32) * 4)
//extern SMutex g_SMutex_1;
/////////////////////////////////////////////////////////////////////////////
// CQM_2DScatt
void CQM_2DScatt::Compute2D()
{
//	Lock lock(mutex);
	ASSERT( m_nNumNodes > 0);
	QMEvolve2D.DefinePotential();
	QMEvolve2D.DefinePotentialDependentParameters();
	QMEvolve2D.DefineInitialWavefunction();
///////////////////////////////////////////////////////////////////////////////
	QMEvolve2D.m_DrawSource = QMEvolve2D.Current;

	for (int k=0; k<m_nNumFrames; k++ )
	{
///////////////////////////////////////////////////////////////////////////////
		Draw(k);
	//	QMEvolve2D.EVOLVE_NASH(&m_nIterationsPerFrame);
	//	QMEvolve2D.EVOLVE_CAYLEY(&m_nIterationsPerFrame);
	//	QMEvolve2D.EVOLVE_NASH_APPROX(&m_nIterationsPerFrame);
		switch (QMEvolve2D.m_EvolutionMethod)
		{
		case QMEvolve2D.Nash: QMEvolve2D.EVOLVE_NASH(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.Cayley: QMEvolve2D.EVOLVE_CAYLEY(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.PeriodicCayley: QMEvolve2D.EVOLVE_CAYLEY(m_nIterationsPerFrame);
			break;
		}

		TCHAR buf[256];
		_stprintf(buf, "Frame[%d]", k);
		_bstr_t bstr(buf);
		Fire_WriteStatus(bstr);
	}
	Draw(m_nNumFrames);
//	m_DrawSource = Initial;
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CQM_2DScatt::Step()
{
	QMEvolve2D.m_DrawSource = QMEvolve2D.Current;

//if (g_SMutex_1.wait(500))
{
	Lock lock(mutex);
	++m_pCView->m_timeOfThisFrame;
	switch (QMEvolve2D.m_EvolutionMethod)
	{
		case QMEvolve2D.Nash: QMEvolve2D.EVOLVE_NASH(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.Cayley: QMEvolve2D.EVOLVE_CAYLEY(m_nIterationsPerFrame);
			break;
		case QMEvolve2D.PeriodicCayley: QMEvolve2D.EVOLVE_CAYLEY_PERIODIC(m_nIterationsPerFrame);
			break;
	}
//		g_SMutex_1.Release();
}
//	theApp.theSystem.PopulateTree();
	theApp.theSystem.DisplayData();
	Fire_ViewChange();
//	m_bDrawUsingInitial = true;

	return S_OK;
}

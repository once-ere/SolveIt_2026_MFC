// Box.cpp: implementation of the CBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "Box.h"
#include "GLView.h"
#include "MainFrm.h"

#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
#include "ChangeCharge.h"
#include "LeftView.h"
#include "PlayMap.h"
#include "QM/qm_1dscatt.h"
#include "QM/qm_2dscatt.h"
#include "QM/qm_ground_mc_1d.h"
#include "QM/qmstationary1d.h"
#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Scatt1D.h"
#include "System.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
CBox* CGLView::HitTestRectangleItems(const POINT& point)
{
	if (vecBoxes.empty()) return 0;
	vector<CBox*>::iterator iBox;
	for (iBox=vecBoxes.begin();iBox!=vecBoxes.end();++iBox)
	{
		CBox*pBox = *iBox;
		if ( pBox->HitTest(point) ) return  pBox;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
#define Y(r) (r.top>=h? h-r.bottom:(r.bottom<=h? h-r.top:0) )

#define ScaledSign(r) (r.top>=h? -1.0/h:(r.bottom<=h? 1.0/h:0.0) )

//#define Y(r) (r.top==h? r.top-r.bottom:(r.bottom==h? r.bottom-r.top:0) )
///////////////////////////////////////////////////////////////////////////////
//long ComputeScreenCoords(double *x, double *y, double *z);
//long ComputeWorldCoords(double *x, double *y, double *z);
/////////////////////////////////////////////////////////////////////////////
void CGLView::DefinePotential()
{
	CSystem* m_system	= theApp.GetSystem();
//-1 <= x <= +1
//	ZeroMemory(theApp.GetSystem().data__QM_Scatt1D->v, m_NDATA*sizeof(double));
	m_system->data__QM_Scatt1D->DefinePotential();
	if (vecBoxes.empty())
	{
		return;
	}

	double* pot = m_system->data__QM_Scatt1D->v;

//	double x_Pos = 0;
//	double y_Pos = 0;
//	double z_Pos = 0;
//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);

	long h = viewHeight/2;

	sort(vecBoxes.begin(),vecBoxes.end(), boxCompare);
	vector<CBox*>::iterator iBox=vecBoxes.begin();
	CBox*pBox = *iBox;
	double V = 0;
	bool notDefined = true;
	if (pBox->rc.left==0)
	{
		notDefined = false;
		V = ScaledSign(pBox->rc)*fabs(pBox->rc.bottom-pBox->rc.top)*m_system->data__QM_Scatt1D->m_fScalePotential;
	}
	for (long j=0;j<m_NDATA;j++)
	{
		double x = double(j*viewWidth)/(m_NDATA-1);
		if (pBox->rc.right < long(x))
		{
			notDefined = true;
			V = 0;
			++iBox;
			pBox = *iBox;
			if (iBox==vecBoxes.end())
			{
			//	for (long jj=j;jj<m_NDATA;jj++) pot[jj] += 0;
				m_system->data__QM_Scatt1D->DefinePotentialDependentParameters();
				return;
			}
		}
		if (notDefined && pBox->rc.left <= long(x) && pBox->rc.right >= long(x))
		{
			notDefined = false;
			V = ScaledSign(pBox->rc)*fabs(pBox->rc.bottom-pBox->rc.top)*m_system->data__QM_Scatt1D->m_fScalePotential;
		//	double y = pBox->rc.top<h? pBox->rc.top:pBox->rc.bottom;
		//	double z = z_Pos;
		//	ComputeWorldCoords(&x, &y, &z);
		//	V = y;
		}
		pot[j] += V;
	}
	m_system->data__QM_Scatt1D->DefinePotentialDependentParameters();
}
#undef Y
#undef ScaledSign
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CBox* CGLView::AddPot(const RECT& r)
{
	CBox*pbox = new CBox(r, m_numRectangles++);
	ASSERT(pbox!=0);
	if (pbox==0) return pbox;
	vecBoxes.push_back( pbox );
//	sort(begin(),end(), boxCompare);
//##	if (m_pCView->m_hBitmapBackground) DeleteObject(m_pCView->m_hBitmapBackground);
//##	m_pCView->m_hBitmapBackground=0;
	DefinePotential();
	return pbox;
}


void CGLView::RemovePot(CBox* pbox)
{
	if (vecBoxes.empty()) return;
	m_pSelectedBox=0;
	ASSERT(pbox!=0);
	if(pbox == 0) return;
	vector<CBox*>::iterator iBox;
	for (iBox = vecBoxes.begin(); iBox != vecBoxes.end(); ++iBox)
	{
		if(pbox==*iBox)
		{
			break;
		//	delete pbox;
		//	vecBoxes.erase(iBox);
		//	--m_numRectangles;
		//	ASSERT(m_numRectangles==vecBoxes.size());
//##	if (m_pCView->m_hBitmapBackground) DeleteObject(m_pCView->m_hBitmapBackground);
//##	m_pCView->m_hBitmapBackground=0;
		//	DefinePotential();
		//	return;
		}
	}
	delete pbox;
	if(iBox != vecBoxes.end())
	{
		vecBoxes.erase(iBox);
		--m_numRectangles;
		ASSERT(m_numRectangles==vecBoxes.size());
		DefinePotential();
	}
	else
	{
		ASSERT(0);
	}
}
///////////////////////////////////////////////////////////////////////////////

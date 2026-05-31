// TrajectoryRecord.h: interface for the CTrajectoryRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAJECTORYRECORD_H__B0BE4FA1_28DE_11D4_A704_0050045B99C4__INCLUDED_)
#define AFX_TRAJECTORYRECORD_H__B0BE4FA1_28DE_11D4_A704_0050045B99C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "State.h"
//#include "parse.h"

#include <map>

namespace SolveIt {

typedef vector<State> TrajectoryRecordData;

struct AnalyzeTrajectory;
struct TrajectoryPeriod;
///////////////////////////////////////////////////////////////////////////////
class CRigidBody;
///////////////////////////////////////////////////////////////////////////////

struct AnalyzeTrajectory
{
	long option;
};

struct TrajectoryPeriod : AnalyzeTrajectory
{
	Vector3D alongThisAxis;
	double period;
	double amplitude;
	TrajectoryPeriod();
};



class CTrajectoryRecord  
{
public:
	CTrajectoryRecord(CRigidBody* pCRigidBody) :
		m_pCRigidBody(pCRigidBody)
		{Clear();}

//&	CTrajectoryRecord(double startTime, double endTime) :	m_startTime(startTime), m_endTime(endTime)	{}

	CRigidBody* m_pCRigidBody;
	virtual ~CTrajectoryRecord() {Clear();}
	double m_startTime;
	double m_endTime;
	TrajectoryRecordData m_data;
//	enum Measurement {PERIOD} ;

	vector<double> ta;
	vector<double> ya;
	vector<double>y2a;
	double terminalValues[4];
	double initialguess[4];
	long splintN;
///////////////////////////////////////////////////////////////////////////////
//
	void SetTimes(double startTime, double endTime)
	{
		m_startTime=startTime, m_endTime=endTime;
	}

	void Clear()
	{
		m_data.clear();
		ta.clear();
		ya.clear();
		y2a.clear();
		m_startTime = m_endTime = 0;
		splintN = 0;long j=0;
		for (j=0; j<sizeof(terminalValues)/sizeof(terminalValues[0]);++j)
		{
			terminalValues[j] = 0;
		}
		for (j=0; j<sizeof(initialguess)/sizeof(initialguess[0]);++j)
		{
			initialguess[j] = 0;
		}
	}

	long AnalyzeData(AnalyzeTrajectory* specify);
	long AnalyzePeriod(TrajectoryPeriod* specify);
	long PlotTrajectory(const Vector3D& color);

	void AddDataPoint(const State& state)
	{
		if (state.t>=m_startTime && state.t<=m_endTime)
			m_data.push_back(state);
	}

	double splint(double x);

	typedef double (CTrajectoryRecord::*func)(double);

	double zbrent(  func f, double x1, double x2, double tol);

};
///////////////////////////////////////////////////////////////////////////////
//typedef vector<TrajectoryRecord*> TrajectoryRecords;
typedef map<long, CTrajectoryRecord*> TrajectoryRecords;

///////////////////////////////////////////////////////////////////////////////


} // namespace SolveIt

#endif // !defined(AFX_TRAJECTORYRECORD_H__B0BE4FA1_28DE_11D4_A704_0050045B99C4__INCLUDED_)

// PlayMap.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "PlayMap.h"
#include "Cruncher.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
/////////////////////////////////////////////////////////////////////////////
// CPlayMap

IMPLEMENT_DYNCREATE(CPlayMap, CWinThread)

CPlayMap::CPlayMap():
	m_hStream(0),
	delay(200),
	bPaused(false),
	bg_RateChanged(false),
	g_Rate(3.59254)

{
}

///////////////////////////////////////////////////////////////////////////////
CPlayMap::~CPlayMap()
{
}

BOOL CPlayMap::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CPlayMap::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlayMap, CWinThread)
	//{{AFX_MSG_MAP(CPlayMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayMap message handlers
///////////////////////////////////////////////////////////////////////////////
LRESULT CPlayMap::OnStepDone(WPARAM wParam, LPARAM lParam)
{
	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CPlayMap::OnStep(WPARAM wParam, LPARAM lParam)
{
	MakeTimeStep();
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
LRESULT CPlayMap::OnHalt(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
long CPlayMap::MakeTimeStep()
{
	CSystem* m_system = theApp.GetSystem();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
UINT sendMIDIEvent(HMIDIOUT hmo, BYTE bStatus, BYTE bData1, BYTE bData2) 
{ 
    union { 
        DWORD dwData; 
        BYTE bData[4]; 
    } u; 
 
    // Construct the MIDI message. 
 
    u.bData[0] = bStatus;  // MIDI status byte 
    u.bData[1] = bData1;   // first MIDI data byte 
    u.bData[2] = bData2;   // second MIDI data byte 
    u.bData[3] = 0; 
 
    // Send the message. 
    return midiOutShortMsg(hmo, u.dwData); 
} 
///////////////////////////////////////////////////////////////////////////////
//	UINT uDeviceID	= -1;      
void CPlayMap::PlayMap(int& j, int& sleep, double& delay)
{
#define M_2PI 6.283185307179586476925286766559
	static int s_len = (sizeof(s_y)/sizeof(s_y[0]));
//#define M_ON  (0x7F0090 | (( (char (0x7F*s_y[j]))& 0xFF) << 8) )
//#define M_OFF (0x90     | (( (char (0xFF*s_y[j]))& 0xFF) << 8) )
//HMIDISTRM m_hStream=0;  
	DWORD cMidi		= 1;
	DWORD dwCallback= 0;       
	DWORD dwInstance= 0;       
	DWORD fdwOpen	= 0;        

	if (m_hStream==0)
	{
{
	Lock lock(mutexPlayData);
		s_y[0] =  0.2;
		s_x[0] = M_ON;
		s_x[1] = M_OFF;

		for (j=1;j<s_len;j++)
		{
			s_y[j] = g_Rate * s_y[j-1] * (1 - s_y[j-1]);
			s_x[2*j+0] = M_ON;
			s_x[2*j+1] = M_OFF;
		}
}

		MMRESULT mhr = midiStreamOpen(
							&m_hStream,  
							&m_uDeviceID,      
							cMidi,            
							dwCallback,       
							dwInstance,       
							fdwOpen           
						);
	}

	if (MMSYSERR_NOERROR != midiStreamRestart(m_hStream))
	{
		midiStreamClose(m_hStream);
		if (MMSYSERR_NOERROR != midiStreamOpen(&m_hStream,&m_uDeviceID,cMidi,dwCallback,dwInstance,fdwOpen))
		{
			return;
		}
		if (MMSYSERR_NOERROR != midiStreamRestart(m_hStream))
		{
		return;
		}
	}
	{
		Lock lock(mutexPlayData);
		midiOutShortMsg((HMIDIOUT) m_hStream, s_x[2*j+0]  );
		if (j%2) sleep = int(delay*s_y[j]);
		else sleep = int(delay*(1-s_y[j]));
		if (sleep < int(delay/2)) sleep = int(delay/2);
		Sleep(sleep);
		midiOutShortMsg((HMIDIOUT) m_hStream, s_x[2*j+1] );
		++j;
		if (j>=s_len) j=0;
		if (j)
		{
			s_y[j] = g_Rate * s_y[j-1] * (1 - s_y[j-1]);
		}
		else
		{
			s_y[j] =  g_Rate * s_y[s_len-1] * (1 - s_y[s_len-1]);
		}
		if (s_y[j] <= 0.0001) s_y[j] = 0.005; 
		if (s_y[j] >= 1) s_y[j] = 0.95; 
		s_x[2*j+0] = M_ON;
		s_x[2*j+1] = M_OFF;
	}
}
///////////////////////////////////////////////////////////////////////////////

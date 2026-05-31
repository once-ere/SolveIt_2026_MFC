// PlayMap.cpp : implementation file
//

#include "stdafx.h"
#include "SolveIt.h"
#include "PlayMap.h"
#include "Cruncher.h"
#include "System.h"
#include "GLView.h"
#include "RigidBody/shapes.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GL2PScontext *gl2ps;
///////////////////////////////////////////////////////////////////////////////
Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
/////////////////////////////////////////////////////////////////////////////
// CPlayMap

IMPLEMENT_DYNCREATE(CPlayMap, CWinThread)

CPlayMap::CPlayMap():
	m_hStream(0),
	m_uDeviceID(0),
	delay(200),
	bPaused(false),
	bg_RateChanged(false),
	g_Rate(3.59254),
	j(0),
	sleep(55)

{
}

///////////////////////////////////////////////////////////////////////////////
CPlayMap::~CPlayMap()
{
}
//
BOOL CPlayMap::InitInstance()
{
	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return TRUE;
}

int CPlayMap::ExitInstance()
{
	bPaused = true;
//	midiStreamStop(m_hStream);
	if (MMSYSERR_NOERROR != midiStreamStop(m_hStream))
	{
		AfxMessageBox(L"ERROR: midiStreamStop");
	}
//	if (MMSYSERR_NOERROR != midiOutUnprepareHeader(m_hStream, lpmh, sizeof(MIDIHDR))))
//	{
//		AfxMessageBox(L"ERROR: midiOutUnprepareHeader");
//	}
	mciSendCommand(m_uDeviceID, MCI_CLOSE, 0, NULL);
	mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, NULL);
	midiStreamClose(m_hStream);
	m_hStream = 0;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlayMap, CWinThread)
	//{{AFX_MSG_MAP(CPlayMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
ON_REGISTERED_THREAD_MESSAGE(WM_USER_START_PASS, OnStep)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_PASS_DONE, OnStepDone)
ON_REGISTERED_THREAD_MESSAGE(WM_USER_TERM, OnHalt)
	ON_REGISTERED_THREAD_MESSAGE(WM_USER_SetParam, OnSetParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayMap message handlers
///////////////////////////////////////////////////////////////////////////////
void CPlayMap::OnStepDone(WPARAM wParam, LPARAM lParam)
{
	CGLView* pGLView = theApp.GetGLView();
	if (pGLView == 0) return ;
	::PostMessage(pGLView->m_hWnd, WM_USER_Redraw, 0, 1);
	bool bSingleStep=false;
	bool bKill=false;
	{
		Lock lock (theApp.protectBodies);
		CSystem* m_system = theApp.GetSystem();
		bSingleStep = m_system->m_bSingleStepping;
		if ((m_system->m_fStopTime>0.0) && (m_system->m_fStopTime <= m_system->m__t) )
		{
			m_system->m_bStopIterations = bKill = true;
		}
	}
//	++m_lNumStepsTaken;
	if (bKill)
	{
		PostQuitMessage(0);
	}
	else
	if (!bSingleStep)
	{
		PostThreadMessage(WM_USER_START_PASS, 0, 0);
	}
	else
	{
//0	1	m_lNumStepsTaken = 0;
	}
	return ;
	if (bPaused) return ;
	PostThreadMessage(WM_USER_START_PASS, 0, 0);
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void CPlayMap::OnStep(WPARAM wParam, LPARAM lParam)
{
try {
	MakeTimeStep();
	PostThreadMessage(WM_USER_PASS_DONE, 0, 0);
	}
	catch (wchar_t * str)
	{
		CSystem* m_system = theApp.GetSystem();
		m_system->m_bStopIterations = true;
		AfxMessageBox(str);
		PostQuitMessage(0);
//		throw str;
	}
	catch( SolveIt_Error& e){e.why();}catch(...)// there was an error
	{
		CSystem* m_system = theApp.GetSystem();
		m_system->m_bStopIterations = true;
		AfxMessageBox(L"no stream");
		PostQuitMessage(0);
	//	throw CVOdeErrors[-flag];
	}
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void CPlayMap::OnHalt(WPARAM wParam, LPARAM lParam)
{
	bPaused = true;
	PostQuitMessage(0);
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz);
///////////////////////////////////////////////////////////////////////////////
long CPlayMap::MakeTimeStep()
{
	if (bPaused) return 1;
	PlayMap(j, sleep, delay);
	{
		Lock lock(mutexPlayData);
		point = Point3D(s_y[j],g_Rate,0);
//		color = Vector3D(s_y[j],double(j)/lSize,1-s_y[j]);
	}
//	CGLView* pGLView = theApp.GetGLView();
//	if (pGLView == 0) return 0;
//			::PostMessage(pGLView->m_hWnd, WM_USER_Redraw, 0, 1);
	return 0;

/*
	HDC hDC = ::GetDC(pGLView->m_hWnd);

	Point3D point;
	Vector3D color;
	if(FALSE == :://wglMakeCurrent(hDC , pGLView->m_hRC)){
		ASSERT(0);
		throw SolveIt_ERROR(L"//wglMakeCurrent failed");
	}
	GLenum err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	glViewport(0, 0, pGLView->viewWidth, pGLView->viewHeight);
	glClear(GL_DEPTH_BUFFER_BIT	| GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	{
//		Lock lock(mutexPlayData);
		glTranslated(0, 0, pGLView->m_fTransZ);
		static wchar_t buf[64];
		std::wstringstream oss(buf, sizeof buf);//ostrstream
		oss << "birth rate = " << (float)g_Rate  << ends;
		wstring str(buf);

//	GLfloat	fX = 0.65f*(pGLView->glFrustum_right + pGLView->glFrustum_left);
//	GLfloat	fY = 0.45f*(pGLView->glFrustum_top + pGLView->glFrustum_bottom);

	GLfloat	fX = 0.65f*(pGLView->viewWidth);
	GLfloat	fY = 0.45f*(pGLView->viewHeight);

				glRasterPos3f(fX , fY,0);
		if (gl2ps)
		{
			gl2psText(str.c_str(), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);//
//	pFrame->SetStatusBarText(str, 2);
	pFrame->PostMessage(WM_USER_Message, 3, (LPARAM) GetWC((const wchar_t*)buf));
	}
	if ( GL_NO_ERROR !=  (err=glGetError()) )
	{
		ASSERT(0);
	}
///////////////////////////////////////////////////////////////////////////////
//			glEnable(GL_TEXTURE_2D);
//	glCallList(pGLView->listBackground);
	if ( GL_NO_ERROR !=  (err=glGetError()) )
	{
		ASSERT(0);
	}
///////////////////////////////////////////////////////////////////////////////
	glColor3dv(color.v);
	glPushMatrix();
	glTranslated(point[0],point[1],point[2]);
	shapes::auxSolidSphere(0.0125);
	//		glutSolidSphere(0.0125, 64, 64);
	glPopMatrix();
	glPopMatrix();
	glFlush();//
	glFinish();
	if(FALSE == SwapBuffers( hDC )){
			throw SolveIt_ERROR(L"SwapBuffers failed");
	}

//		if(FALSE == wglSwapLayerBuffers(hDC , WGL_SWAP_MAIN_PLANE))
//			throw SolveIt_ERROR(L"SwapBuffers failed");
//
	
	pGLView->GetMatrices();//
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	//wglMakeCurrent(0, 0);
	::ReleaseDC(pGLView->m_hWnd, hDC);

	return 0;
*/
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
	if (bPaused) return;
#define M_2PI 6.283185307179586476925286766559
	static int s_len = (sizeof(s_y)/sizeof(s_y[0]));
//#define M_ON  (0x7F0090 | (( (wchar_t (0x7F*s_y[j]))& 0xFF) << 8) )
//#define M_OFF (0x90	 | (( (wchar_t (0xFF*s_y[j]))& 0xFF) << 8) )
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
			throw SolveIt_ERROR(L"ERROR: midiStreamOpen");
		}
		if (MMSYSERR_NOERROR != midiStreamRestart(m_hStream))
		{
			throw SolveIt_ERROR(L"ERROR: midiStreamRestart");
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
//	CSystem* m_system = theApp.GetSystem();
//wchar_t* hData = m_system->player->SendMessage(WM_USER_SetParam, 'f', 0);
////GlobalFree(hData);
///////////////////////////////////////////////////////////////////////////////
void CPlayMap::OnSetParam(WPARAM wParam, LPARAM lParam)
{
	try
	{
	switch (int(wParam))
	{
		case 'p':
			bPaused = !bPaused;
			break;
		case 'f'://faster
			delay -= 4*imsl_rand();
			if (delay < 8) delay = 8;
			break;
		case 's'://slower
			delay += 4*imsl_rand();
			break;
		case 'u':
		{
			CGLView* pGLView = theApp.GetGLView();
			if (pGLView == 0) return ;
			Lock lock(mutexPlayData);
			
			g_Rate += pGLView->m_fdelta*imsl_rand();
			if (g_Rate > max_Rate) g_Rate = max_Rate;
		}
			break;
		case 'd':
		{
			CGLView* pGLView = theApp.GetGLView();
			if (pGLView == 0) return ;
			Lock lock(mutexPlayData);
			
			g_Rate -= pGLView->m_fdelta*imsl_rand();
			if (g_Rate <= 0) g_Rate = min_Rate;
		}
			break;
			
	}
	if (bPaused) return ;
	PostThreadMessage(WM_USER_START_PASS, 0, 0);
	return ;

/*
	wchar_t buf[64];
	std::wstringstream oss(buf, sizeof buf);//ostrstream
	oss << g_Rate  << ends;
	int cb = _tcslen(buf) + 1;
	HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, cb);   
	std::wstring    lpszNew = (std::wstring )GlobalLock(hgCF_TEXTData);
	if (_tcslen(buf)) _tcscpy(lpszNew, buf);
	GlobalUnlock(hgCF_TEXTData);
	return (long)lpszNew; 
*/
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(exc->what()), MB_OK);
	}
	catch(struct _exception *except)
	{
		emap::_emap.OnError(except);
		::MessageBox(0,L"OnSetParam Error: ",  GetWC(except->name), MB_OK);
	}
	catch (wchar_t * str)
	{
		::MessageBox(0,L"OnSetParam Error: ", str, MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		::MessageBox(0,L"Unknown error.", L"OnGetParam", MB_OK);
	}
//	return 0;
}
///////////////////////////////////////////////////////////////////////////////

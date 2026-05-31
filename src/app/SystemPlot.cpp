// SystemPlot.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma optimize( "", off )

#include "SolveIt.h"
#include "System.h"

#include "MainFrm.h"
#include "instruction.h"
#include "StackMachine.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "parser.h" /* for token definitions and yylval */

#include "LeftView.h"
#include "SolveItDoc.h"
#include "SolveItView.h"
#include "RigidBody/RigidBodyManager.h"
#include "RigidBody/FieldsManager.h"
//#include "Worker.h"
#include "Cruncher.h"
#include "GLView.h"
#include "PlayMap.h"
#include "QM/qm_1dscatt.h"
#include "QM/qm_2dscatt.h"
#include "QM/qm_ground_mc_1d.h"
#include "QM/qmstationary1d.h"
#include "QM/DataQM_Scatt1D.h"
#include "QM/DataQM_Scatt2D.h"
//#include "QM/DataQM_Stationary1D.h"
#include "QM/DataQM_Stationary2D.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "RigidBody/shapes.h"
#include "RigidBody/ConstraintManager.h"
#include "RigidBody/ExamineCollisionTimes.h"
#include "CalcMap.h"
#include <functional>
#include <algorithm>
#include <sstream>
#include "QM/QMStationary2D.h"

#include "Examples/Superposition/Superposition.h"
#include "Examples/Superposition/WaveNumbers.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
extern GL2PScontext *gl2ps;
extern Mutex mutexPlayData;//worse than useless until PlayMap is moved to its own thread
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void __stdcall PerformPlot(void)//CSystem::
{
//	glFinish();
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CLeftView*	tree = pFrame->GetTreeView();
	CSystem* m_system = theApp.GetSystem();
	if (tree == 0) return;
	GLenum e=GL_NO_ERROR;
//	SetCursor(LoadCursor(NULL, IDC_WAIT));
///////////////////////////////////////////////////////////////////////////////
	const long		NDATA			= tree->GetNDATA();
	ASSERT( NDATA > 1);
	if (NDATA <= 1) return;
	const double	x1				= tree->GetglFrustum_left();
	const double	x2				= tree->GetglFrustum_right();
	const double	y1				= tree->GetglFrustum_bottom();
	const double	y2				= tree->GetglFrustum_top();
	const double	m_fZoomScale	= tree->GetZoomScale();
	const double	m_fScaleWave	= tree->GetScaleWave();
	double x = 0;
	double Width	= x2 - x1;
	double Height	= y2 - y1;
	double dx	= Width/(NDATA-1);
	long j=0;
try
{
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);	 
	}

	glLineWidth(2.0f);
	glColor3d(0,0,1);
	glBegin(GL_LINE_STRIP);
	vector<double> args;
	args.push_back(0);
	for (j=0;j<NDATA; ++j)
	{
		x = x1 + j*dx;
		args[0]=x;
//		LoadAndCallRealFunc(&m_system.m_uUserDefinedExternal1, args);
//		glVertex2d(x, m_fScaleWave*(CStackMachine::receiveStackTopDoubleValue));

//		glVertex2d(x, m_fScaleWave*m_system->GetUserDefinedPotential(x, CSystem::__External1));
		glVertex2d(x, m_system->GetUserDefinedPotential(x, CSystem::__External1));
	}
	glEnd();
///////////////////////////////////////////////////////////////////////////////
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
//	glPopAttrib();
//	glFinish();
	glFlush();//With multithread applications, flush the drawing commands in any other threads drawing to the same window before calling SwapBuffers
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
//		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ASSERT(0);
		return;
	}
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
//	SetCursor(LoadCursor(NULL, IDC_ARROW));
//	::MessageBox(NULL,_T("System:Plot"),_T("General Exception"),MB_OK);
	return;
}
}
///////////////////////////////////////////////////////////////////////////////
//DoThis jPerformPlot=CSystem::PerformPlot;
DoThis jPerformPlot	= PerformPlot;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CSystem::Plot(const char* name, const char* Options)
{
//	CStackMachine& stackMachine	= theApp.GetStackMachine();
///////////////////////////////////////////////////////////////////////////////
	long		NDATA	= 0;
	long		red		= 0;
	long		green	= 0;
	long		blue	= 0;
	double	x1		= 0;
	double	x2		= 0;
	double	y1		= 0;
	double	y2		= 0;
	const char*	xStr	= 0;
	const char*	yStr	= 0;
///////////////////////////////////////////////////////////////////////////////
	struct KEY{
		LPCSTR sKey;
		int nToken;
		enum {UNDEFINED=0, LONG=1, DOUBLE=2, QSTRING=3} type;
		ULONG_PTR data;
	} keys[]={
		"x",		1,		KEY::DOUBLE,	reinterpret_cast<ULONG_PTR>(&x2),
		"x",		0,		KEY::DOUBLE,	reinterpret_cast<ULONG_PTR>(&x1),
		"y",		1,		KEY::DOUBLE,	reinterpret_cast<ULONG_PTR>(&y2),
		"y",		0,		KEY::DOUBLE,	reinterpret_cast<ULONG_PTR>(&y1),
		"#pts",		0,		KEY::LONG,		reinterpret_cast<ULONG_PTR>(&NDATA),
		"color",	0,		KEY::LONG,		reinterpret_cast<ULONG_PTR>(&red),
		"color",	1,		KEY::LONG,		reinterpret_cast<ULONG_PTR>(&green),
		"color",	2,		KEY::LONG,		reinterpret_cast<ULONG_PTR>(&blue),
		"x-Label",	0,		KEY::QSTRING,	reinterpret_cast<ULONG_PTR>(&xStr),
		"y-Label",	0,		KEY::QSTRING,	reinterpret_cast<ULONG_PTR>(&yStr),
		0,0,KEY::UNDEFINED,0
	};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	char _options[256];
	char options[256];
	char sbuf[128];
	char vbuf[128];
//	char seps[]   = " ,\t\n";
	char seps[]   = ",\t\n";
	char *token=0;
	char *nextToken = 0;
///////////////////////////////////////////////////////////////////////////////
try
{
	ZeroMemory( options, sizeof(options) );
	strncpy_s(_options, Options, _TRUNCATE);
	char*	c	=_options;
	char*	p	= options;
	bool bLiteral = false;
	do {
		if (p >= options + sizeof(options) - 1) throw Options;
		if (*c == '\'') bLiteral = !bLiteral;
		if (bLiteral || *c != ' ') *p++ = *c;
	} while (*c++);
	ASSERT(bLiteral == false);
	char*	c1	= options;
	char*	c2	= options;
	int		nc1	= 0;
	int		nc2	= 0;

	int		nCh	= static_cast<int>(strlen(options));
	if (nCh == 0) {throw SolveIt_ERROR("");return false;};
	int j = 0;
	for (j=0; j<nCh; ++j)
	{
		if (*c1++ == '[') ++nc1;
		if (*c2++ == ']') ++nc2;
	}
	if (nc1 == 0) {throw SolveIt_ERROR("");return false;};
	if (nc2 != nc1) {throw SolveIt_ERROR("");return false;};

	CMainFrame*		pFrame	= dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);
	CLeftView*		tree	= pFrame->GetTreeView();
	CSolveItView*	ed		= pFrame->GetEditView();
	if (tree == 0) {throw SolveIt_ERROR("");return false;};
	if (ed == 0) {throw SolveIt_ERROR("");return false;};

	NDATA	= tree->GetNDATA();
	x1		= tree->GetglFrustum_left();
	x2		= tree->GetglFrustum_right();
	y1		= tree->GetglFrustum_bottom();
	y2		= tree->GetglFrustum_top();
//	xStr	= const_cast<char*>(tree->GetxStr());
//	yStr	= const_cast<char*>(tree->GetyStr());
	xStr	= _strdup(m_xStr.c_str());
	yStr	= _strdup(m_yStr.c_str());

	c	= options;
//	SaveState();
	for (j=0; j<nCh; ++j)
	{
		if (c[j] == '[')
		{
//get key
			--nc1;
			++j;
			if (!(isalpha(c[j]) || c[j] == '_')) {throw SolveIt_ERROR("");{throw SolveIt_ERROR("");return false;};}
			p = sbuf;
			do {
				if (p >= sbuf + sizeof(sbuf) - 1) throw Options;
				*p++ = c[j];
			} while (c[++j] != ',' && (isalnum(c[j]) || c[j] == '_' || c[j] == '-'));
			*p = '\0';
			ASSERT(c[j++] == ',');

//get list of values
			p = vbuf;
			do {
				if (p >= vbuf + sizeof(vbuf) - 1) throw Options;
				*p++ = c[j];
			} while (c[++j] != ']');
			*p = '\0';
			if (c[j] == ']')
			{
				--nc2;
			}
			ASSERT(c[j++] == ']');
			int nTok = 0;
//Establish string and get the first token
			token = strtok_s(vbuf, seps, &nextToken);//_tcstok
			while( token != NULL )
			{
				int i = 0;
				for (; keys[i].sKey; ++i)
				{
					if (nTok == keys[i].nToken && !strcmp(sbuf, keys[i].sKey)) break;//_tcscmp
				}

				std::string rendered;
				if (keys[i].type == KEY::QSTRING) rendered = token;
				else
				{
					string out;
					string s(token);
					s += ";\n";
	std::string::size_type pos = 0;
	const string CrLf("\r\n");
	const string r("\n");
// find all occurences of 'CrLf' and replace them by 'r'
	while ( ( pos = s.find( CrLf, pos ) ) != std::string::npos ) {
		s.replace( pos, CrLf.length(), r );
		pos += r.length(); // new starting point of search is just after 'b'
	}
try {
///////////////////////////////////////////////////////////////////////////////
int	top	= stackMachine->top;

stackMachine->initialize_cycle();
top	= stackMachine->top;
					if (false == stackMachine->Compile_and_Run_and_check_for_reset(s, out))
					{
						throw Options;
					}
//	if (CSolveItApp::g_boolDoReset) Initialize();
///////////////////////////////////////////////////////////////////////////////
top	= stackMachine->top;
stackMachine->terminate_cycle();
top	= stackMachine->top;
}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
	catch (std::runtime_error* exc)
	{
		::MessageBox(0, GetWC(exc->what()), L"Error", MB_OK);
		stackMachine->terminate_cycle();
		{throw SolveIt_ERROR("");return false;};
	}
	catch(struct _exception *except)
	{
		::MessageBox(0, GetWC(except->name), L"Error", MB_OK);
		stackMachine->terminate_cycle();
		{throw SolveIt_ERROR("");return false;};
	}
	catch (char * str)
	{
		::MessageBox(0, GetWC(str), L"Error", MB_OK);
		stackMachine->terminate_cycle();
		{throw SolveIt_ERROR("");return false;};
	}
	catch(...)
	{
		stackMachine->terminate_cycle();
		throw Options;
		{throw SolveIt_ERROR("");return false;};
	}
///////////////////////////////////////////////////////////////////////////////
					if (out.size() == 0) out = "0";
					rendered = out;
//					const char* s = ed->OnCompile(token);
//					if (s==0) throw Options;
//					rendered = s;
//					free((void *)s);
				}
				std::istringstream iss(rendered);
				switch(keys[i].type)
				{
					case KEY::LONG:
					{
						long x=0;
						if(!(iss>>x).fail())
						{
							*reinterpret_cast<long*>(keys[i].data) = x;
						}
					}
					break;
					case KEY::DOUBLE:
					{
						double x=0;
						if(!(iss>>x).fail())
						{
							*reinterpret_cast<double*>(keys[i].data) = x;
						}
					}
					break;
					case KEY::QSTRING:
					{
						free((void*)*reinterpret_cast<char**>(keys[i].data));
						*reinterpret_cast<char**>(keys[i].data) = _strdup(rendered.c_str());
					}
					break;
				}
				token = strtok_s(NULL, seps, &nextToken);
				++nTok;
			}
		}
	}
//	RestoreState();Potential
	if (nc1 != 0) {throw SolveIt_ERROR("");return false;};
	if (nc2 != 0) {throw SolveIt_ERROR("");return false;};

	tree->SetNDATA(NDATA);
	tree->SetglFrustum_left(x1);
	tree->SetglFrustum_right(x2);
	tree->SetglFrustum_top(y2);
	tree->SetglFrustum_bottom(y1);
	tree->SetxStr(GetWC(xStr));
	tree->SetyStr(GetWC(yStr));
	free((void *)xStr);
	free((void *)yStr);


	x1		= tree->GetglFrustum_left();

	tree->UpdateTree();

	if (!SetUserDefinedName(string(name), __External1))
	{
		throw SolveIt_ERROR("SetUserDefinedName");
		return false;
	};
	SetUserDefined_IsTimeDependent(false, __External1);

	doit.push_back(jPerformPlot);
}
	catch( SolveIt_Error& e)
	{
		e.why();
	}
catch(...)
{
	::MessageBox(NULL, L"System:Plot", L"General Exception", MB_OK);
	{throw SolveIt_ERROR("");return false;};
}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::DoIt()
{
//vector<DoThis>
	doVector::iterator it;
	for (it = doit.begin(); it != doit.end(); ++it)
	{
		DoThis fn = *it;
		(*fn)();
	}
	if (m_pCQMStationary1D)
	{
	////////	m_pCQMStationary1D->PostThreadMessage(WM_USER_SetParam, 0, WM_PAINT);
		if (m_pCQMStationary1D->m_bStepCompleted)
		{
			m_pCQMStationary1D->DrawSystem();
		}
	}
///////////////////////////////////////////////////////////////////////////////
	if (data__QM_Stationary2D->m_pCQMStationary2D)
	{
		Lock lock(data__QM_Stationary2D->m_pCQMStationary2D->mutex);
		data__QM_Stationary2D->m_pCQMStationary2D->Draw();
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	if (m_pCSuperposition != NULL)
	{
		if ( m_pCSuperposition ) m_pCSuperposition->Draw();
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bDoQM_Scatt1D)
	{
		data__QM_Scatt1D->Draw(data__QM_Scatt1D->m_timeOfThisFrame);
//		return S_OK;data__QM_Scatt1D->m_pCQM_Scatt1D
	}
///////////////////////////////////////////////////////////////////////////////
	if (m_bDoMapper && mapper)
	{
//		mapper->OnDraw();
	}
///////////////////////////////////////////////////////////////////////////////
	if (player != NULL )
	{
		CGLView* pGLView = theApp.GetGLView();
		if (pGLView == 0) return;
		Lock lock(mutexPlayData);
		glPushMatrix();
		{
			static char buf[128];
			std::ostringstream oss;
			oss << "birth rate = " << (float)player->g_Rate;
			string str(oss.str());

//			GLfloat	fX = 0.65f*(pGLView->glFrustum_right + pGLView->glFrustum_left);
//			GLfloat	fY = 0.45f*(pGLView->glFrustum_top + pGLView->glFrustum_bottom);

			GLfloat	fX = 0.165f*(pGLView->viewWidth);
			GLfloat	fY = 0.245f*(pGLView->viewHeight);

			glRasterPos3f(fX , fY,0);
			if (gl2ps)
			{
				gl2psText(str.c_str(), const_cast<char*>("Courier"), 12);
			}
			else
			{
				glPushAttrib(GL_LIST_BIT);
				glListBase(2000);
				glCallLists(static_cast<GLsizei>(str.size()), GL_BYTE, (GLbyte *)str.c_str());
				glPopAttrib();
			}
		}
///////////////////////////////////////////////////////////////////////////////
//		glCallList(pGLView->listBackground);
///////////////////////////////////////////////////////////////////////////////
	//	glColor3dv(player->color.v);
		glColor3ub(0,0,0);
		glPushMatrix();//
		double x=player->point[0];
		double y=player->point[1];
		double z=player->point[2];
	//	pGLView->ComputeScreenCoords(&x, &y, &z);
		pGLView->ComputeScreenCoords(x, y, z, false);
	//	glTranslated(player->point[0],player->point[1],player->point[2]);
		glTranslated(x,y,z);
#if 0
1	//	glDisable(GL_TEXTURE_2D);
	//	glDisable(GL_COLOR_MATERIAL);
	//	glDisable(GL_CULL_FACE);
	//	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DITHER);
	glClearDepth(10.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
#endif
	//	shapes::auxSolidSphere(pGLView->m_fdelta);
	//	shapes::auxSolidSphere(pGLView->viewWidth/64);
	glBegin(GL_POLYGON);
	glVertex2d( 0, 0 );
	glVertex2d( pGLView->viewWidth/42, pGLView->viewHeight/22 );
	glVertex2d(						0, pGLView->viewHeight/54 );
	glVertex2d(-pGLView->viewWidth/42, pGLView->viewHeight/22);
	glEnd();
		glPopMatrix();//
///////////////////////////////////////////////////////////////////////////////
		glPopMatrix();
	}
}
///////////////////////////////////////////////////////////////////////////////
/*
struct _state_data
{
	instruction* code;
	objects* objects_stack;
	objects* objects_data;
	int  errors;
	int progbase;
	instruction* codePtr;
	int fetch_execute_cycle_Level;
	int returning;
	int	top;
	int	pc;
	int indexInDataArray;
	int nInstructionCounter;
} state_data;
///////////////////////////////////////////////////////////////////////////////
void CSystem::SaveState()
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	state_data.top							= stackMachine->top;
	state_data.pc							= stackMachine->pc;
	state_data.indexInDataArray				= stackMachine->indexInDataArray;
	state_data.progbase						= stackMachine->progbase;
	state_data.codePtr						= stackMachine->codePtr;

	state_data.code				= new instruction[stackMachine->next_free_spot_for_code_generation];
	state_data.objects_stack	= new objects[NPROG+1-stackMachine->top];
	state_data.objects_data		= new objects[stackMachine->indexInDataArray];
//	CopyMemory(state_data.code			, stackMachine->code,		stackMachine->next_free_spot_for_code_generation*sizeof(instruction));
//	CopyMemory(state_data.objects_stack	, stackMachine->stack+stackMachine->top,	(NPROG+2-stackMachine->top)*	sizeof(objects));
//	CopyMemory(state_data.objects_data	, stackMachine->data,		(stackMachine->indexInDataArray)*sizeof(objects));
	for (int j=0; j<stackMachine->next_free_spot_for_code_generation; ++j)	state_data.code[j]			= stackMachine->code[j];
	for ( j=0; j<NPROG+1-stackMachine->top; ++j)			state_data.objects_stack[j]	= stackMachine->stack[j+stackMachine->top];
	for ( j=0; j<stackMachine->indexInDataArray; ++j)		state_data.objects_data[j]	= stackMachine->data[j];

	state_data.errors						= stackMachine->errors;
	state_data.fetch_execute_cycle_Level	= stackMachine->fetch_execute_cycle_Level;
	state_data.returning					= stackMachine->returning;

	state_data.nInstructionCounter			= stackMachine->next_free_spot_for_code_generation;
}
///////////////////////////////////////////////////////////////////////////////
void CSystem::RestoreState()
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->indexInDataArray				= state_data.indexInDataArray;
	stackMachine->next_free_spot_for_code_generation			= state_data.nInstructionCounter;
	stackMachine->top							= state_data.top;
	stackMachine->pc							= state_data.pc;
	stackMachine->progbase						= state_data.progbase;
	stackMachine->codePtr						= state_data.codePtr;
	stackMachine->errors						= state_data.errors;
	stackMachine->fetch_execute_cycle_Level	= state_data.fetch_execute_cycle_Level;
	stackMachine->returning					= state_data.returning;

//	CopyMemory(stackMachine->code,					state_data.code,	stackMachine->next_free_spot_for_code_generation*sizeof(instruction));
//	CopyMemory(stackMachine->stack+stackMachine->top	,state_data.objects_stack ,	(NPROG+1-stackMachine->top)*	sizeof(objects));
//	CopyMemory(stackMachine->data,					state_data.objects_data,	(stackMachine->indexInDataArray)*sizeof(objects));
	for (int j=0; j<stackMachine->next_free_spot_for_code_generation; ++j)
	{
		stackMachine->code[j]	= state_data.code[j];
		state_data.code[j].obj	= 0;
	}
	for ( j=0; j<NPROG+1-stackMachine->top; ++j)
	{
		stackMachine->stack[j+stackMachine->top]	= state_data.objects_stack[j];
		state_data.objects_stack[j]	= 0;
	}
	for ( j=0; j<stackMachine->indexInDataArray; ++j)
	{
		stackMachine->data[j]	= state_data.objects_data[j];
		state_data.objects_data[j]	= 0;
	}

	delete[]state_data.objects_data;
	delete[]state_data.objects_stack;
	delete[]state_data.code;
	state_data.code			= 0;
	state_data.objects_stack= 0;
	state_data.objects_data	= 0;
}
*/
///////////////////////////////////////////////////////////////////////////////

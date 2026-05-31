/*
IN "parser_2020.h"
YOU MUST PERFORM THIS EDIT:
/# Tokens.  #/
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/# Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  #/
#include <cstdint>
enum yytokentype : std::uint64_t {
	...
*/

// SolveIt.h : main header file for the SOLVEIT application
//

#if !defined(AFX_SOLVEIT_H__5F98B96D_11CD_44B2_B0C0_316325D11159__INCLUDED_)
#define AFX_SOLVEIT_H__5F98B96D_11CD_44B2_B0C0_316325D11159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "Types.h"
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
class CStackMachine;
class CSymbolTable;
class CSystem;
class CRigidBodyManager;
class CFieldsManager;
class CConstraintManager;
///////////////////////////////////////////////////////////////////////////////
class CGLView;
class CLeftView;
class CSolveItView;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CSolveItApp:
// See SolveIt.cpp for the implementation of this class
//

class CSolveItApp : public CWinApp
{
public:
	CSolveItApp();
//	HINSTANCE m_hLibRichEdit;

	CMultiDocTemplate* m_pEditViewTemplate;
	CMultiDocTemplate* m_pGLViewTemplate;

	CStackMachine*		GetStackMachine();
	CSymbolTable*		GetSymbolTable();
	CRigidBodyManager*	GetRigidBodyManager();
	CFieldsManager*		GetFieldsManager();
	CConstraintManager*	GetConstraintManager();

	CSystem*			GetSystem();
	bool				GetSystem(CSystem* sys);
///////////////////////////////////////////////////////////////////////////////
	CLeftView*			GetTreeView();
	CSolveItView*		GetEditView();
	CGLView*			GetGLView();
///////////////////////////////////////////////////////////////////////////////
	Mutex protectWorker;
	Mutex protectBodies;
	Mutex protectMapper;
///////////////////////////////////////////////////////////////////////////////
	static bool g_boolDoReset;
	void Initialize(const wchar_t *html_file =NULL, const wchar_t *title =NULL);

///////////////////////////////////////////////////////////////////////////////

long WriteResourceToFile(	LPCTSTR filename, 
							LPCTSTR lpName,  // resource name
							LPCTSTR lpType   // resource type
						);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolveItApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
//	virtual int ExitInstance();

// Implementation
//	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CSolveItApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFileOpen();
	afx_msg LRESULT OnUSER_Initialize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

extern CSolveItApp theApp;
///////////////////////////////////////////////////////////////////////////////
extern HANDLE	g_EventStop;
extern HANDLE	g_EventStep;
extern HANDLE	g_EventRun;
extern HANDLE	g_EventKill;
extern HANDLE	g_EventHandle;
extern HANDLE	g_EventInitialize;
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_SOLVEIT_H__5F98B96D_11CD_44B2_B0C0_316325D11159__INCLUDED_)

// SolveItView.cpp : implementation of the CSolveItView class
//

#include "stdafx.h"
#include "SolveIt.h"

#include "SolveItDoc.h"
#include "SolveItView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CSolveItView
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::WriteFileScript()
{
	CFileDialog fDlg(FALSE);
	fDlg.m_ofn.lpstrFilter = L"Script Files\0*.act\0? Text files\0*.txt\0All Files\0*.*\0\0";
	TCHAR filename[_MAX_PATH];
	const std::wstring scriptFileName = ToWideString(m_ScriptFileName.c_str());
	_tcscpy_s(filename, _countof(filename), scriptFileName.c_str());
	fDlg.m_ofn.lpstrFile = filename;//newVal.bstrVal;
// Set the current directory
	wchar_t dirBuffer[1024];
// If the buffer is not big enought, DON'T set the current directory
	if(GetCurrentDirectory(1023, dirBuffer) != 0)
		fDlg.m_ofn.lpstrInitialDir = dirBuffer;
	if(fDlg.DoModal() == IDOK)
	{
// Make sure the user entered a file name
		if(fDlg.GetPathName() == _T(""))
		{
			AfxMessageBox(L"Please enter a file name");
			return;
		}
			m_ScriptFileName = GetC(fDlg.GetPathName());
	}
	else return;

	ofstream ofs(m_ScriptFileName.c_str());
	if (ofs)
	{
		ofs << m_Script.c_str();
		ofs.close();
//		return S_OK;
	}
//	return -1;
}
///////////////////////////////////////////////////////////////////////////////
#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CSolveItView::ReadFileScript()
{
	try {
	std::wstring bstr = ToWideString(m_ScriptFileName.c_str());
	DWORD res = GetFileAttributes(bstr.c_str());
	if (res!=INVALID_FILE_ATTRIBUTES)
	{
		wifstream ifs((const wchar_t *)bstr.c_str());
		if (!ifs) return;// S_OK;
		wstring str;
		while (!(std::getline(ifs, str/*, '\n'*/)).fail())
		{
			WriteCommand(const_cast<BSTR>(str.c_str()));
		}
	}
//	return -1;
	}
	catch(wchar_t * s)
	{
		::MessageBox(0,s,L"CSolveItView::ReadFileScript",MB_OK);
	}
	catch( SolveIt_Error& e){e.why();}catch(...) {
		::MessageBox(0,L"General exception caught.",L"CSolveItView::ReadFileScript",MB_OK);
	}
//	CParser::globalIO = 0;
}

///////////////////////////////////////////////////////////////////////////////

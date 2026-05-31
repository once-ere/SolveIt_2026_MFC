// KeyWords.cpp: implementation of the KeyWords class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SolveIt.h"
////#include "parser_2020.h" /* for token definitions and yylval */
#include "KeyWords.h"
#include "System.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const wchar_t sSolveIt_First_System_Token[]=L"SolveIt_First_System_Token";
const wchar_t sSolveIt_Last_System_Token[]	=L"SolveIt_Last_System_Token";
const wchar_t s__define[]	=L"#define";
const wchar_t s_define[]	=L"define";


//KeyWords g_k;
///////////////////////////////////////////////////////////////////////////////
KeyWords::KeyWords()
{
	wchar_t buf[256];
	wchar_t token[256];
	wifstream ifs(L"parser_2020.h");//, ios::in|ios::nocreate, filebuf::sh_none);
	bool bBegin	= false;
	bool bEnd	= false;

	while ((!bEnd) && (!(ifs.getline(buf, sizeof(buf)/sizeof(buf[0]))).fail()))
	{
		bool bFoundDefine	= false;
		/*istrstream*/ std::wistringstream iss(buf);
	//	iss.seekg(0);
		if (!iss.fail())
		{
			if (bBegin	== false)
			{
				while (!(iss >> token).fail())
				{
					if (!wcscmp(sSolveIt_First_System_Token, token)) bBegin = true;
				}
			}
			else
			{
				while (!(iss >> token).fail())
				{
					if (!wcscmp(s__define, token))	bFoundDefine = true;
					if (!wcscmp(s_define, token))	bFoundDefine = true;
					if (bFoundDefine)
					{
						int key = 0;
						wchar_t key_word[256];
						if (!(iss>> key_word >> key).fail())
						{
							insert(map< int, const wchar_t*>::value_type(key, _wcsdup(key_word)));
							break;
						}
					}
					if (!wcscmp(sSolveIt_Last_System_Token, token)) bEnd = true;
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
KeyWords::~KeyWords()
{
	ofstream ofs("keys.txt");//, ios_base::out | ios_base::trunc);
	ASSERT(!ofs.fail());
	ASSERT(ofs.is_open());
//	wchar_t buf[2560];
//	/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf)/sizeof(buf[0]));
	map< int, const wchar_t*>::iterator it;
	for (it = begin(); it != end() ; ++it)
	{
		const int& i = it->first;
		std::wstring bstr = std::to_wstring((long)i);
		const wchar_t* s=it->second;
		ofs << GetC(bstr.c_str()) << "\t\t" << GetC(s) << endl;//"\r\n";
		free((void*) s);
	//	ASSERT(!ofs.fail());
	//	ASSERT(ofs.is_open());
	}
//	oss  << ends;
//	AfxMessageBox(buf);
	ofs.close();
	clear();
}



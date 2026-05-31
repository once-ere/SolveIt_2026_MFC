// KeyWords.h: interface for the KeyWords class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDS_H__C9C5288D_D28B_4C29_9643_5D62FD0D9677__INCLUDED_)
#define AFX_KEYWORDS_H__C9C5288D_D28B_4C29_9643_5D62FD0D9677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <map>
#include <sstream>

namespace SolveIt {

///////////////////////////////////////////////////////////////////////////////
struct KeyWord {
	int key;
	const wchar_t* s;
};

///////////////////////////////////////////////////////////////////////////////

class KeyWords  : public map<int, const wchar_t*>
{
public:
	KeyWords();
	KeyWords(KeyWord data[])
	{
		for (int i = 0; data[i].s; i++)
		{
			insert(map< int, const wchar_t*>::value_type(data[i].key, data[i].s));
		}
	}
	virtual ~KeyWords();

///////////////////////////////////////////////////////////////////////////////
	const wchar_t* operator[](const int& key)
	{
		map< int, const wchar_t*>::iterator i = find(key);
		if (i != end()) return _wcsdup(i->second);
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////
	const wchar_t* operator[](const wchar_t* skey)
	{
		std::wstring result(L"");
		wchar_t seps[]   = L":,\t\n \r";
		wchar_t *token=0;
		wchar_t *ctx=0;
		wchar_t *mutable_skey = _wcsdup(skey);
		token = wcstok_s( mutable_skey, seps, &ctx );
		while( token != NULL )
		{
			std::wstringstream iss(token);
			int key=0;
			if(!(iss>>key).fail())
			{
				const wchar_t* s = this->operator[](key);
				if (s) {result += s; free((void*) s);}
			}
			token = wcstok_s( NULL, seps, &ctx );
		}
		free(mutable_skey);
		return _wcsdup(result.c_str());
	}
///////////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////////
/*
static KeyWord keyWords[] = {
CYLINDER,"CYLINDER",
WHILE,"WHILE",
0,0
};
static KeyWords keyWordList(keyWords);
*/
///////////////////////////////////////////////////////////////////////////////


} // namespace SolveIt

#endif // !defined(AFX_KEYWORDS_H__C9C5288D_D28B_4C29_9643_5D62FD0D9677__INCLUDED_)

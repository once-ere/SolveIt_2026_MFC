// Se3.cpp: implementation of the Se3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector3D.h"
//#include "Quat.h"
#include "Mat3.h"
#include "Se3.h"

#include "MatX.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define STR_LENGTH 1024
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const Se3	Se3::ID		(Quat::ID, ZERO_3D);

Se3::Se3(const MatX& s) {
	Mat3 m(s.rot());
	q=Quat(m);
	d=s.trans();
}

wistream& Se3::read(wistream &is)
{
	wchar_t c;
	int i;
	double x;
	Vector3D vect;
	Quat quat;
	Se3 op;
	wchar_t buffer[STR_LENGTH];
	wchar_t tok[STR_LENGTH];
	
	*this = Se3::ID;

	is >> ws;
	if (is.peek() == '{') is.get(c);
	else {
		wcerr << L"Se3::read : didn't find '{' \a" << endl;
		return is;
	}
		
	// read until closing '}'
	i = 0;
	while (1) {
		is.get(c);
		if (c == '}') {
			buffer[i++] = 0;
			break;
		}
		buffer[i++] = c;
		if (i == STR_LENGTH || is.eof()) {
			wcerr << L"Se3::read : didn't find '}' or specification too long\a" << endl;
			return is;
		}
	}
	/*istrstream*/ std::wstringstream iss(buffer);

	
	while (!((iss >> tok).fail())) {

		if (!wcscmp(tok, L"trans")) {
			iss >> vect;
			op.set(Quat::ID, vect);
		}
		else if (!wcscmp(tok, L"rot")) {
			iss >> tok >> vect;
			x = atof(GetC(tok)) * DEG_TO_RAD;
			quat.set(x, vect);
			op.set(quat, ZERO_3D);
		}
		else {
			wcerr << L"Se3::read : unknown token " << tok << L" \a" << endl;
			break;
		}

		postmult(op);
		q.normalize();	// just to be sure.
	}
	return is;
}


#undef STR_LENGTH


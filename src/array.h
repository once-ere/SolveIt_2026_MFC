// vector_of_double.h: interface for the vector_of_double class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARRAY_H__6E88BE23_13F8_4DBB_BF62_4A22F11027FD__INCLUDED_)
#define AFX_ARRAY_H__6E88BE23_13F8_4DBB_BF62_4A22F11027FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static const wchar_t msg1[] = L"NORMALIZERADIAL: ";
static const wchar_t msg2[] = L"vector_of_double.norm: ";
static const wchar_t msg3[] = L"vector_of_double.dot: ";
///////////////////////////////////////////////////////////////////////////////
class vector_of_Vector3D  : public vector<Vector3D>
{
public:
	vector_of_Vector3D():vecTrajectoryColor(0.7, 0.0, 0.5){}
	vector_of_Vector3D(const double& Red, const double& Green, const double& Blue):vecTrajectoryColor(Red,Green,Blue){}
	virtual ~vector_of_Vector3D(){}

	Vector3D& operator()(const int i) {return operator[](i);}
	const Vector3D& operator()(const int i) const {return operator[](i);}

	vector_of_Vector3D& operator+=(const vector_of_Vector3D& a)
	{
		size_type n = a.size();
		if (n != size())
		{
			throw SolveIt_ERROR(L"vector_of_Vector3D::+=: invalid");
		}
		for (unsigned long j=0; j< size(); ++j)
		{
			operator[](j) += ( a[j] );
		}
		return *this;
	}

	vector_of_Vector3D& operator*=(const double& a)
	{
		for (unsigned long j=0; j< size(); ++j)
		{
			operator[](j) *= a;
		}
		return *this;
	}

	void OnDraw();
	Vector3D vecTrajectoryColor;

	double dot(const vector_of_Vector3D& a) const{
		size_type n = a.size();
		if (n != size())
		{
			throw SolveIt_ERROR(L"vector_of_Vector3D::dot: invalid");
		}
		if (empty()) return 0;
		double sum = 0;
//		for (const_iterator it = begin(); it != end(); ++it)
//			const Vector3D& v1 = (*it);
//			long n = distance(begin(), it);
		for (unsigned long j=0; j< size(); ++j)
		{
			sum += operator[](j) .dot( a[j] );
		}
		return sum;
	}

	vector_of_Vector3D times(const vector_of_Vector3D& a) const{
		size_type n = a.size();
		if (n != size())
		{
			throw SolveIt_ERROR(L"vector_of_Vector3D::times: invalid");
		}
		vector_of_Vector3D res;
		if (empty()) return res;
		for (unsigned long j=0; j< size(); ++j)
		{
			res.push_back(operator[](j) .times( a[j] ));
		}
		return res;
	}
};
///////////////////////////////////////////////////////////////////////////////
class vector_of_Vector2D  : public vector<Vector2D>
{
public:
	vector_of_Vector2D():vecTrajectoryColor(0.5, 0.0, 0.7){}
	vector_of_Vector2D(const double& Red, const double& Green, const double& Blue):vecTrajectoryColor(Red,Green,Blue){}
	virtual ~vector_of_Vector2D(){}
	void OnDraw();
	Vector3D vecTrajectoryColor;
};
///////////////////////////////////////////////////////////////////////////////
class vector_of_double  : public vector<double>
{
public:
	vector_of_double():n_group_by(3), vecPlotTrajectoryColor(0.5608, 0.3372, 0.2667){}
	vector_of_double(const unsigned long& n, const Vector3D& color):n_group_by(n), vecPlotTrajectoryColor(color){}
	virtual ~vector_of_double();
	unsigned long n_group_by;
	Vector3D vecPlotTrajectoryColor;
///////////////////////////////////////////////////////////////////////////////
	void OnDraw(const double& Red, const double& Green, const double& Blue);
	void OnDraw(const Vector3D& vecPlotTrajectoryColor);
	void OnDraw();


	double& operator()(const int i) {return operator[](i);}
	const double& operator()(const int i) const {return operator[](i);}
/*
	vector_of_double(){}
	vector_of_double(const long n) {
		for (long j=0; j< n; ++j) push_back(0);
	}

	vector_of_double(const vector_of_double& a) {
		operator=(a);
	}

	vector_of_double& operator =(const vector_of_double& a) {
		if (capacity()<a.size()) reserve(a.size());
		for (long j=0; j< a.size(); ++j) push_back(a[j]);
//		CopyMemory((void*)&front(), (void*)&a.front(), a.size()*sizeof(a.front()));
		return *this;
	}
*/

	void is(const double& a, const long n) {
		reserve(n);
		for (long j=0; j< n; ++j) push_back(a);
	}

	friend vector_of_double operator * (const vector_of_double& a, const double& b) {
		vector_of_double c = a;
		for (unsigned long j=0; j< c.size(); ++j) c[j] *= b;
		return c;
	}

	friend vector_of_double operator * (const double& b, const vector_of_double& a) {
		vector_of_double c = a*b;
		return c;
	}

	friend vector_of_double operator * (const vector_of_double& a, const vector_of_double& b) {
		vector_of_double c = a;
		for (unsigned long j=0; j< c.size(); ++j) c[j] *= b[j];
		return c;
	}

	double dot(const vector_of_double& a) {
		size_type n = a.size();
		if (n != size())
		{
			throw SolveIt_ERROR(L"size error: array dot");
	//		_exception *except = new _exception;
	//		except->type = SW_INVALID;
	//		except->name = const_cast<wchar_t*>(msg3);
	//		except->arg1=size();    /* first argument to function */
	//		except->arg2=n;    /* second argument (if any) to function */
	//		except->retval=0;  /* value to be returned by function */
	//		throw except;
			return 0;
		}
		double sum = 0;
		for (unsigned long j=0; j< size(); ++j) sum += operator[](j) * a[j];
		return sum;
	}

	double norm() {
		double x = 0;
	try {
	//	size_type n = size();
	//	double* v= new double[n];
	//	for (long j=0; j< n; ++j) v[j]= operator[](j);
	//	for (j=0; j< n; ++j) x += v[j] * v[j];
	//	delete[] v;
		x = dot(*this);
	}
	catch (wchar_t* str)
	{
		::MessageBoxW(0, str, L"Error: array norm", MB_OK);
		return 0;
	}
//	catch(struct _exception *except)
//	{
//		wstring s(_T("vector_of_double.norm: "));//
//		s += except->name;//
//		except->name = _tcsdup(s.c_str());
//		throw except;
//		return 0;
//	}
	catch( SolveIt_Error& e){e.why();}catch(...) {}
		if (x >= 0.0) return sqrt(x);
		throw SolveIt_ERROR(L"imaginary array norm");
//		_exception *except = new _exception;
//		except->type = SW_INVALID;
//		except->name = const_cast<wchar_t*>(msg2);//DupString(_T("vector_of_double.norm"));
//		except->arg1=x;    /* first argument to function */
//		except->arg2=0;    /* second argument (if any) to function */
//		except->retval=0;  /* value to be returned by function */
//		throw except;
		return x;
	}

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ARRAY_H__6E88BE23_13F8_4DBB_BF62_4A22F11027FD__INCLUDED_)

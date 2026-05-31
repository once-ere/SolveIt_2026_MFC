// AnalyzePeriod.cpp: implementation of the CTrajectoryRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//
#include "TrajectoryRecord.h"
#include "parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// CTrajectoryRecord
//////////////////////////////////////////////////////////////////////
TrajectoryPeriod::TrajectoryPeriod() :
	period(0), amplitude(0) {
	option= /*yy::parser::token::*/T_PERIOD;
}
void spline( double x[] , double y[], long n, double yp1, double ypn, double y2[]);
/////////////////////////////////////////////////////////////////////////////

#if 0
struct InitOle {
	  InitOle()  { ::CoInitialize(NULL); }
	  ~InitOle() { ::CoUninitialize();   }
	} _init_InitOle_;

//		IOpenGLObjPtr m_glObj;
//		HRESULT hr = m_glObj.CreateInstance( __uuidof( OpenGLObj ) );
//		IOpenGLObjPtr m_glObj(__uuidof(OpenGLObj));
#endif


/////////////////////////////////////////////////////////////////////////////
// CTrajectoryRecord
long CTrajectoryRecord::AnalyzePeriod(TrajectoryPeriod* specify)
{
	if (m_data.empty()) return -1;
	wchar_t buf[2056];
	/*ostrstream*/ std::wstringstream oss(buf, sizeof(buf));
//		oss.seekp(0);
try {
		TrajectoryPeriod* tp = specify;
		Vector3D alongThisAxis	= tp->alongThisAxis;
		double& periodV			= tp->period;
		double& amplitude		= tp->amplitude;
		double periodX = 0, x1 = 0, x0 = 0;
		periodV = 0;
		amplitude = 0;
		const long N = 10;//data block size
	//	splintN = m_data.size()/N;
		splintN = static_cast<long>(m_data.size());
		if (splintN==0) return -1;
		long& n = splintN, j;
		vector<double> x;
		vector<double> v;
		vector<double> TapproxZero;//approximate zero crossing time
		vector<double> tMin;//min value bracketing interval
		vector<double> tMax;//max value bracketing interval
		vector<double> tZeroV;
		vector<double> tZeroX;
		x.reserve(n);
		v.reserve(n);
		ta.clear();
		ya.clear();
		y2a.clear();
		ta.reserve(n);
		ya.reserve(n);
		y2a.reserve(n);
		alongThisAxis.normalize();
		double eps = 1.0e-9;
		double Eps = 1.0e-2;
		for (j=0;j<n; ++j)
		{
/*
				ta.push_back(m_data[j*N+0]);
				Vector3D X(m_data[j*N+1],m_data[j*N+2],m_data[j*N+3]);
				Vector3D V(m_data[j*N+4],m_data[j*N+5],m_data[j*N+6]);
*/
				ta.push_back(m_data[j].t);
				Vector3D X(m_data[j].x.x,m_data[j].x.y,m_data[j].x.z);
				Vector3D V(m_data[j].p.x,m_data[j].p.y,m_data[j].p.z);
				double ux = alongThisAxis*X;
				double uv = alongThisAxis*V;
				x.push_back(ux);
				v.push_back(uv);
//		oss << ux << "\r\n";
		}
//		oss << ends;
//	TimedMessageBox(NULL, buf, L"CTrajectoryRecord::AnalyzeData", MB_OK);
///////////////////////////////////////////////////////////////////////////////
// find roots of velocity component:
		for (j=0;j<n; ++j)
		{
			ya[j] = v[j];
			if (ya[j] == 0.0)
				TapproxZero.push_back(ta[j]);
			else if (j && ya[j]*ya[j-1] < 0.0)
				TapproxZero.push_back(ta[j]);
		}


		if (TapproxZero.size()<3)
		{
			TapproxZero.clear();//just to be certain
			ya.clear();
			vector<double>::iterator i;
			double max = v[0];
			double min = v[0];
			for (i=v.begin(); i!=v.end();++i)
			{
				if (max<*i) max = *i;
				if (min>*i) min = *i;
			}
			if (max==min) return -5;
			double ave = 0.5*(max + min);
			for (j=0;j<n; ++j)
			{
				ya[j] = v[j]-ave;
				if (ya[j] == 0.0)
					TapproxZero.push_back(ta[j]);
				else if (j && ya[j]*ya[j-1] < 0.0)
					TapproxZero.push_back(ta[j]);
			}
		}

		if (TapproxZero.size()<3)
		{
	throw SolveIt_ERROR(L"Insufficient data to determine the period.\r\nTry increasing the recording time.");
		}

		spline( &ta.front() , &ya.front(), n, 0.99e40, 0.99e40, &y2a.front());
		for (j=0; j<TapproxZero.size(); ++j)
		{
			double max = TapproxZero[j];
			double min = TapproxZero[j];
			double b = splint(max);
			double a = splint(min);
			while (a*b > 0.0)
			{
				max += Eps;
				min -= Eps;
				b = splint(max);
				a = splint(min);
				if (a*b < 0.0 ) break;
				if (ta[0]>min+Eps && ta[n-1]<max-Eps)
				{
					throw SolveIt_ERROR(L"No zeros found on interval.");
				}
			}
			tMax.push_back(max);
			tMin.push_back(min);
		}

			for (j=0; j<TapproxZero.size(); ++j)
			{
				double zero = -1;
				double a = splint(tMin[j]);
				double b = splint(tMax[j]);
				if (b==0.0) zero = tMin[j];
				else if (a==0.0) zero = tMax[j];
				else
				{
					if (a*b>0.0)
					{
						oss.seekp(0);
						oss << b << "=b;  ERROR  ; a= " << a << "\r\n";
						oss << ends;
						throw buf;
					}
					zero = zbrent(&CTrajectoryRecord::splint, tMin[j], tMax[j], eps);
				}
				tZeroV.push_back(zero);
			}
		
		if (tZeroV[0]<1.e-4) tZeroV[0] = 0;

		periodV = tZeroV[2] - tZeroV[0];
///////////////////////////////////////////////////////////////////////////////
// find roots of displacement component:
		TapproxZero.clear();
		tMax.clear();
		tMin.clear();
		ya.clear();
		y2a.clear();
		for (j=0;j<n; ++j)
		{
			ya[j] = x[j];//-x[0];
			if (ya[j] == 0.0)
				TapproxZero.push_back(ta[j]);
			else if (j && ya[j]*ya[j-1] < 0.0)
				TapproxZero.push_back(ta[j]);
		}

		if (TapproxZero.size()<3)
		{
			TapproxZero.clear();//just to be certain
			ya.clear();
			vector<double>::iterator i;
			double max = x[0];
			double min = x[0];
			for (i=x.begin(); i!=x.end();++i)
			{
				if (max<*i) max = *i;
				if (min>*i) min = *i;
			}
			if (max==min) return -5;
			double ave = 0.5*(max + min);
			for (j=0;j<n; ++j)
			{
				ya[j] = x[j]-ave;
				if (ya[j] == 0.0)
					TapproxZero.push_back(ta[j]);
				else if (j && ya[j]*ya[j-1] < 0.0)
					TapproxZero.push_back(ta[j]);
			}
		}

		if (TapproxZero.size())
		{
		if (TapproxZero.size()<3)
		{
	oss<< 
"WARNING: Insufficient data to determine the displacement component period.\r\nTry increasing the recording time.\r\n";
		//	return -2;
		}

		spline( &ta.front() , &ya.front(), n, 0.99e40, 0.99e40, &y2a.front());
		x1 = splint(tZeroV[1]);//EVALSPLINE(&t1);
		x0 = splint(tZeroV[0]);//EVALSPLINE(&t0);
		amplitude = 0.5*fabs(x1 - x0);
		for (j=0; j<TapproxZero.size(); ++j)
		{
			double max = TapproxZero[j];
			double min = TapproxZero[j];
			double b = splint(max);
			double a = splint(min);
			while (a*b > 0.0)
			{
				max += Eps;
				min -= Eps;
				b = splint(max);
				a = splint(min);
				if ( a*b < 0.0 ) break;
				if (ta[0]>min+Eps && ta[n-1]<max-Eps)
				{
					throw SolveIt_ERROR(L"No zeros found on interval.");
				}
			}
	//		if (!tMax.size() || ( max > tMax.back()+Eps && min > tMax.back()+Eps ) )
	//		{
				tMax.push_back(max);
				tMin.push_back(min);
	//		}
		}

/*2.2479407139330324458100398858256
*/

			for (j=0; j<TapproxZero.size(); ++j)
			{
				double zero = -1;
				double a = splint(tMin[j]);
				double b = splint(tMax[j]);
				if (b==0.0) zero = tMin[j];
				else if (a==0.0) zero = tMax[j];
				else
				{
					if (a*b>0.0)
					{
						oss.seekp(0);
						oss << b << "=b;  ERROR  ; a= " << a << endl;
						oss << ends;
						throw buf;
					}
					zero = zbrent(&CTrajectoryRecord::splint, tMin[j], tMax[j], eps);
				}
			if (!tZeroX.size() || zero > tZeroX.back()+Eps )
				tZeroX.push_back(zero);
			}
		
		if (tZeroX[0]<1.e-4) tZeroX[0] = 0;

		periodX = tZeroX[2] - tZeroX[0];

		}
	//	MYFREE();

		double omegaX=0;
		double omegaV=0;
		if (periodV!=0.0) omegaV = 6.283185307179586476925286766559/periodV;
		if (periodX!=0.0) omegaX = 6.283185307179586476925286766559/periodX;

	//	oss.seekp(0);
	//	oss << info[0] << "  info[0]    " << info[1]  << "\r\n" ;
	//	oss << roots[0] << "  roots[0]    " << roots[1]  << "\r\n" ;
		oss << "zeros of velocity component:" << "\r\n" ;
		for (j=0; j<tZeroV.size(); ++j)
			oss << tZeroV[j] << ' ';
		oss << "\r\n\r\n";
		if (tZeroX.size())
		{
			oss << x0 << " = displacement at t0; displacement at t1 = " << x1 << "\r\n" ;
			oss << "amplitude = " << amplitude << "\r\n" << "\r\n" ;
			oss << "zeros of displacement component:" << "\r\n" ;
			for (j=0; j<tZeroX.size(); ++j) oss << tZeroX[j] << ' ';
		}
		oss << "\r\n\r\n" << "period calculated with zeros of velocity component = "
			<< periodV << " (omega = " << omegaV << ")" << "\r\n" ;
		if (tZeroX.size()>2) oss << "period calculated with zeros of displacement component = "
			<< periodX << " (omega = " << omegaX << ")" << "\r\n" ;
		oss << ends;
//	::MessageBox(NULL, buf, L"CTrajectoryRecord::AnalyzeData", MB_OK);

	{
		std::wstring bstr(buf);
		Fire_WriteResponse(bstr.c_str());
	}
//		AfxMessageBox(bstr);
//	tmpbstr += L"\r\n";
//	tmpbstr += bstr;
/*
		std::wstring bstr(CParser::parserPrintBuffer);
		Fire_WriteResponse(bstr.c_str());
		theSystem->m_pIUserIO->put_Response(bstr);//FireCollisionInfo(bstr);
	IWebBrowser2* pIWebBrowser2=0;
	IServiceProvider* pISP=0;

	HRESULT hr = m_spClientSite->QueryInterface(IID_IServiceProvider, (void **)&pISP);
	if (SUCCEEDED(hr))
	{
		pISP->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void **)&pIWebBrowser2);
		pISP->Release();
	}
	std::wstring bstr(buf);
	if (pIWebBrowser2) pIWebBrowser2->put_StatusText(bstr);
	pIWebBrowser2->Release();
*/
}
catch(wchar_t*str)
{
//	TimedMessageBox(NULL,str,"AnalyzeData:Exception",MB_OK);
		std::wstring bstr(str);
		Fire_WriteResponse(bstr.c_str());
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
	TimedMessageBox(NULL,L"AnalyzeData",L"Exception",MB_OK);
}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////

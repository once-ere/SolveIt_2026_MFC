//BuildHull.cpp
//


#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
#include "QMStationary2D.h"
#include "GLView.h"

#include <type_traits>
#include <cstdint>
template<typename Type>
using remove_cv_ref_t = typename std::remove_cv_t<std::remove_reference_t<Type>>;

extern "C" {
#include "../SolveIt.Lib/lib/qhull/qhull_a.h"
}
//extern GLuint texture;
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
int ToQhullPointCount(size_t value)
{
	ASSERT(value <= static_cast<size_t>(INT_MAX));
	return static_cast<int>(value);
}
}

extern	double *__psi_xy;
///////////////////////////////////////////////////////////////////////////////
	const long N			= _N2a;//16;//;
	const long S_NUMPOINTS	= N;
	const long S_ORDER		= 4;
	const long S_NUMKNOTS	= S_NUMPOINTS + S_ORDER;
	const long T_NUMPOINTS	= N;
	const long T_ORDER		= 4;
	const long T_NUMKNOTS	= T_NUMPOINTS + T_ORDER;

///////////////////////////////////////////////////////////////////////////////

void CQMStationary2D::DrawHull(const long& j)
{
//	return;
try
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
//		double X1=-1,Y1=-1,Z1=0;
//		double X2=-1,Y2=-1,Z2=0;
//		double X3=-1,Y3=-1,Z3=0;
//		double X4=-1,Y4=-1,Z4=0;

	double max = 1;


	CGLView* pGLView = theApp.GetGLView();
	double& x1= pGLView->glFrustum_left;
	double& x2= pGLView->glFrustum_right;
	double& y1= pGLView->glFrustum_bottom;
	double& y2= pGLView->glFrustum_top;
	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);

	double X=0, Y=0;
	list<Vertex> verts;
	Vertex vx;

	for (long x=0;x<N;x++)
	{
		X = x1 + x*dx;//
		for (long y=0;y<N;y++)
		{
			Y = y1 + y*dy;
			vx.x = X;
			vx.y = Y;
			vx.z = m_fScaleNurbs*psi_xy(j, x, y);
			verts.push_back(vx);
		}
	}
/*
		for (long x=0;x<N;x+=2)
		{
			for (long y=0;y<N;y+=2)
			{
				cdouble& C1 = Y[x][y];
				if (fabs(C1.real())>1.e100) {glEnd();return;}
				if (fabs(C1.imag())>1.e100) {glEnd();return;}
				cdouble& C2 = Y[(x+1)%QMEvolve2D.NDATA][(y+1)%QMEvolve2D.NDATA];
				cdouble& C3 = Y[x][(y+1)%QMEvolve2D.NDATA];
				cdouble& C4 = Y[(x+1)%QMEvolve2D.NDATA][y];

				Z1 = m_fScalePsi*log ( 1.0 + C1.real()*C1.real()+C1.imag()*C1.imag() );
				Z2 = m_fScalePsi*log ( 1.0 + C2.real()*C2.real()+C2.imag()*C2.imag() );
				Z3 = m_fScalePsi*log ( 1.0 + C3.real()*C3.real()+C3.imag()*C3.imag() );
				Z4 = m_fScalePsi*log ( 1.0 + C4.real()*C4.real()+C4.imag()*C4.imag() );
				X1= SCALE(x);
				Y1= SCALE(y);
				X2= SCALE(((x+1)%QMEvolve2D.NDATA));
				Y2= SCALE(((y+1)%QMEvolve2D.NDATA));
				X3= X1;
				Y3= Y2;
				X4= X2;
				Y4= Y1;

				vx.real() = X1,vx.imag() = Y1,vx.z = Z1;verts.push_back(vx);
				vx.real() = X3,vx.imag() = Y3,vx.z = Z3;verts.push_back(vx);
				vx.real() = X2,vx.imag() = Y2,vx.z = Z2;verts.push_back(vx);
				vx.real() = X4,vx.imag() = Y4,vx.z = Z4;verts.push_back(vx);
			}
		}
*/
#undef SCALE

//	BuildHull(j, verts, 10*max);
	int nf,  idx, i;//, nv
	int npts=0;
	Vertex *v;

//	wchar_t err[128];
//	/*ostrstream*/ std::wstringstream cerr(err,128);

//	std::wstring bOptions(L"qhull ");
	std::wstring bOptions(L" ");

	boolT ismalloc;
	int curlong, totlong, exitcode;
	facetT *facet;
	vertexT *vertex;
	vertexT **vertexp;
	setT *vertices;
	coordT *qhullData, *qhd;
	list<Vertex>::iterator vi, vi0;
	wchar_t options [200];
//	wchar_t name[1024];

	static vector<Vertex *> facelist;//(MAX_VERTS_PER_FACE);
//	vector<Vertex *>::iterator it;
// initial size; exceeding it breaks nothing
#define MAX_VERTS_PER_HULL 1000
	static vector<coordT> array;//			(MAX_VERTS_PER_HULL * 3);
	static vector<Vertex *> hullVerts;//(MAX_VERTS_PER_HULL);
	static vector<int> vertUsed;//			(MAX_VERTS_PER_HULL);
#undef MAX_VERTS_PER_HULL

	//cout << "invoking qhull...	 " << flush;
		facelist.clear();
		array.clear();
		hullVerts.clear();
		vertUsed.clear();

	const size_t pointCount = verts.size();
	npts = ToQhullPointCount(pointCount);


	if (pointCount > hullVerts.capacity()) {
		array.reserve(3 * pointCount);
		hullVerts.reserve(pointCount);
		vertUsed.reserve(pointCount);
	}
	array.resize(3 * pointCount);
	hullVerts.resize(pointCount);
	vertUsed.resize(pointCount);
	qhullData = array.data();
		

// build array of vertex coordinates
	i = 0;
	qhd = qhullData;
	for(vi=verts.begin();vi!=verts.end();++vi)
	{
		v = &*vi;
		*qhd++ = v->x;
		*qhd++ = v->y;
		*qhd++ = v->z;

		vertUsed[i] = 0;
		hullVerts[i] = v;
		i++;
	}

	ismalloc= False; 	// True if qh_freeqhull should 'free(qhullData)'
	qh_init_A (stdin, stdout, stderr, 0, NULL);
	exitcode= setjmp (qh errexit);
	if (exitcode)
	{
		//goto error;
		std::wstring msg(L"error building convex hull of Polyhedron \nexitcode: ");
		
		std::wstring bstr = std::to_wstring( long(exitcode) )  ;
		msg+=bstr;

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);
		::MessageBox(NULL,msg.c_str(),L"CQMStationary2D::BuildHull",MB_OK);
		throw SolveIt_ERROR(msg.c_str());

	}
//	sprintf(options, "qhull Qx i s Tcv C-0");//bounding
	//sprintf(options, "qhull A0.9999 i s Tcv C-0");
//	sprintf(options, "qhull d QJ i");
//	sprintf(options, "qhull d QJ i QVn");
//	sprintf(options, "qhull d QJ s i TO");
//	sprintf(options, "qhull d Qbb s Fv TO");//bounding
//	sprintf(options, "qhull d QJ Qz s i TO");
//	sprintf(options, "qhull d Qbb Qz s i TO");//bounding
//	sprintf(options, "qhull d QJ s i TO Fx");
//	std::wstring bOptions(L"qhull d ");
	bOptions += m_bstrQHullOptions;
	_tcscpy_s(options, _countof(options), /* (const wchar_t*)*/ bOptions.c_str());//sprintf
	auto xxx = GetC((const wchar_t*)options);
//	template<typename Type>
//	using remove_cv_ref_t = typename std::remove_cv_t<std::remove_reference_t<Type>>;

	qh_initflags((char*)xxx);
	qh_init_B (qhullData, npts, 3, ismalloc);
	qh_qhull();
	qh_check_output();

	// build hull Polyhedron
		double X1,Y1,Z1;
		double X2,Y2,Z2;
		double X3,Y3,Z3;

		double nx, ny, nz, n2;
	nf = 0;
///////////////////////////////////////////////////////////////////////////////
	GLenum e=GL_NO_ERROR;
//	ASSERT(listEigenStates[j]	== firstInlistEigenStates + j);

	glPushMatrix();
	glColor3d(0.50*fabs(sin(2*j+1)),0.50*fabs(cos(j)),0.50*fabs(sin(j+1)));

	glDisable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
//	glBegin(GL_TRIANGLE_STRIP);
{

	FORALLfacets
	{
//		sprintf(name, "f%d", nf++);
		vertices= qh_facet3vertex (facet);
		facelist.clear();
		FOREACHvertex_(vertices) {
			idx = qh_pointid(vertex->point);
			vertUsed[idx] = 1;
			facelist.push_back(hullVerts[idx]);
		}
//		addFace(name, facelist, 1); // qhull generates a clockwise list

		if (!facelist.empty() && facelist.size()>2)
		{
			vector<Vertex *>::iterator it3=facelist.begin();
			for(/*it3=facelist.begin()*/;it3!=facelist.end();/*++it3*/)
			{
				vector<Vertex *>::iterator it1=it3++;
				if ( it1 == facelist.end() ) break;
				vector<Vertex *>::iterator it2=it3++;
				if ( it2 == facelist.end() ) break;
				if ( it3 == facelist.end() ) break;
//				glVertex3d(it->x,it->y,it->z);
			//	X1= x1 + w*(*it1)->x;
			//	Y1= y1 + h*(*it1)->y;
				X1= (*it1)->x;
				Y1= (*it1)->y;
				Z1= (*it1)->z;
				X2= (*it2)->x;
				Y2= (*it2)->y;
			//	X2= x1 + w*(*it2)->x;
			//	Y2= y1 + h*(*it2)->y;
				Z2= (*it2)->z;
				X3= (*it3)->x;
				Y3= (*it3)->y;
				Z3= (*it3)->z;
/*
				if (
					(Z1<max && Z2<max && Z3<max) &&
					(0<=Z1 && 0<=Z2 && 0<=Z3) &&
					(X1<=x2 && x1<=X1) &&
					(X2<=x2 && x1<=X2) &&
					(X3<=x2 && x1<=X3) &&
					(Y1<=y2 && y1<=Y1) &&
					(Y2<=y2 && y1<=Y2) &&
					(Y3<=y2 && y1<=Y3)
					)
*/
				{
					nx=(Y1-Y2)*(Z3-Z2)-(Z1-Z2)*(Y3-Y2);
					ny=(Z1-Z2)*(X3-X2)-(X1-X2)*(Z3-Z2);
					nz=(X1-X2)*(Y3-Y2)-(Y1-Y2)*(X3-X2);
					n2 = nx*nx + ny*ny + nz*nz;
					if ( n2 < 0.1e-12) continue;
			//		if (         ny*ny + nz*nz < 1.0e-8*n2 ) continue;//omit if nx = �1
			//		if ( nx*nx +         nz*nz < 1.0e-8*n2 ) continue;//omit if ny = �1
			//		if ( nx*nx + ny*ny         < 1.0e-6*n2 ) continue;//omit if nz = �1
					glNormal3d(-nx,-ny,-nz);
					glVertex3d(X1,Y1,Z1);
					glVertex3d(X2,Y2,Z2);
					glVertex3d(X3,Y3,Z3);
				}
			}
		}
		qh_settempfree(&vertices);
	}//end FORALLfacets
}

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);
	glEnd();
	glPopMatrix();
	glFlush();
	glFinish();
//	SwapBuffers(hDC);
//	//wglMakeCurrent(0, 0);
//	::ReleaseDC(pGLView->GetSafeHwnd(), hDC);

	if (0 && listEigenStates[j]==0)
	{
		GLuint texture;
		pGLView->PixelsToTexture(pGLView->PixelsSnapshot(), texture);
		pGLView->TextureToDisplayList(listEigenStates[j], texture);
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
catch( SolveIt_Error& e){e.why();}catch(...)
{
}
}

//typedef Vector3D Vertex;

//#if QHULL
// Given the Vertex list of a Polyhedron, compute the convex hull.
// Use this to determine and build the Edges and Faces lists.	If a
// Vertex does not lie on the convex hull, it will be removed
// from the Vertex list.
long CQMStationary2D::BuildHull(const long& j)//, list<Vertex>& vertss, double maxx)
{
return -1;
	CGLView* pGLView = theApp.GetGLView();

	const double& x1= pGLView->glFrustum_left;
	const double& x2= pGLView->glFrustum_right;
	const double& y1= pGLView->glFrustum_bottom;
	const double& y2= pGLView->glFrustum_top;
	double	w = x2 - x1;
	double	h = y2 - y1;
	double dx = w/(N-1);
	double dy = h/(N-1);
	double X=0, Y=0;
	list<Vertex> verts;
	Vertex vx;

	for (long x=0;x<N;x++)
	{
		X = x1 + x*dx;//
		for (long y=0;y<N;y++)
		{
			Y = y1 + y*dy;
			vx.x = X;
			vx.y = Y;
			vx.z = m_fScaleNurbs*psi_xy(j, x, y);
			verts.push_back(vx);
		}
	}
	double max = 10;
	int nf,  idx, i;//, nv
	int npts=0;
	Vertex *v;

	wchar_t err[128];
	/*ostrstream*/ std::wstringstream cerr(err,128);

//	std::wstring bOptions(L"qhull ");
	std::wstring bOptions(L" ");

	boolT ismalloc;
	int curlong, totlong, exitcode;
	facetT *facet;
	vertexT *vertex;
	vertexT **vertexp;
	setT *vertices;
	coordT *qhullData, *qhd;
	list<Vertex>::iterator vi, vi0;
	wchar_t options [200];
//	wchar_t name[1024];

	static vector<Vertex *> facelist;//(MAX_VERTS_PER_FACE);
//	vector<Vertex *>::iterator it;
// initial size; exceeding it breaks nothing
#define MAX_VERTS_PER_HULL 1000
	static vector<coordT> array;//			(MAX_VERTS_PER_HULL * 3);
	static vector<Vertex *> hullVerts;//(MAX_VERTS_PER_HULL);
	static vector<int> vertUsed;//			(MAX_VERTS_PER_HULL);
#undef MAX_VERTS_PER_HULL

	//cout << "invoking qhull...	 " << flush;

	const size_t pointCount = verts.size();
	npts = ToQhullPointCount(pointCount);


	if (pointCount > hullVerts.capacity()) {
		array.reserve(3 * pointCount);
		hullVerts.reserve(pointCount);
		vertUsed.reserve(pointCount);
	}
	array.resize(3 * pointCount);
	hullVerts.resize(pointCount);
	vertUsed.resize(pointCount);
	qhullData = array.data();
		


	// build array of vertex coordinates
	i = 0;
	qhd = qhullData;
	for(vi=verts.begin();vi!=verts.end();++vi)
	{
		v = &*vi;
		*qhd++ = v->x;
		*qhd++ = v->y;
		*qhd++ = v->z;

		vertUsed[i] = 0;
		hullVerts[i] = v;
		i++;
	}

	ismalloc= False; 	// True if qh_freeqhull should 'free(qhullData)'
	qh_init_A (stdin, stdout, stderr, 0, NULL);
	exitcode= setjmp (qh errexit);
/*
	if (exitcode)
	{
		//goto error;
		_bstr_t msg(L"error building convex hull of Polyhedron \nexitcode: ");
		_variant_t xit=long(exitcode);
		_bstr_t bstr(xit);
		msg+=bstr;

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);
		::MessageBox(NULL,msg,"CQMStationary2D::BuildHull",MB_OK);
		throw SolveIt_ERROR(msg);

	return 1;
	}
*/
	if (exitcode)
	{
		//goto error;
		std::wstring msg(L"error building convex hull of Polyhedron \nexitcode: ");
		std::wstring bstr = std::to_wstring( long(exitcode) ) ;
		msg+=bstr;

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);
		::MessageBox(NULL,msg.c_str(),L"CQMStationary2D::BuildHull",MB_OK);
		throw SolveIt_ERROR(msg.c_str());

	return 1;
	}
//	sprintf(options, "qhull Qx i s Tcv C-0");//bounding
	//sprintf(options, "qhull A0.9999 i s Tcv C-0");
//	sprintf(options, "qhull d QJ i");
//	sprintf(options, "qhull d QJ i QVn");
//	sprintf(options, "qhull d QJ s i TO");
//	sprintf(options, "qhull d Qbb s Fv TO");//bounding
//	sprintf(options, "qhull d QJ Qz s i TO");
//	sprintf(options, "qhull d Qbb Qz s i TO");//bounding
//	sprintf(options, "qhull d QJ s i TO Fx");
//	std::wstring bOptions(L"qhull d ");
	bOptions += m_bstrQHullOptions;
	_tcscpy_s(options, _countof(options), /*(const wchar_t*)*/ bOptions.c_str());//sprintf

	auto xoptionsx = GetC((const wchar_t*)options);
	//	template<typename Type>
	//	using remove_cv_ref_t = typename std::remove_cv_t<std::remove_reference_t<Type>>;

	qh_initflags((char*)xoptionsx);
	//qh_initflags (options);
	qh_init_B (qhullData, npts, 3, ismalloc);
	qh_qhull();
	qh_check_output();

	// build hull Polyhedron
		double X1,Y1,Z1;
		double X2,Y2,Z2;
		double X3,Y3,Z3;

		double nx, ny, nz, n2;
	nf = 0;
///////////////////////////////////////////////////////////////////////////////

//	HDC hDC = pGLView->GetDC()->GetSafeHdc();//pGLView->m_hdcMem
//	HDC hDC = pGLView->m_pDC->GetSafeHdc();

	//wglMakeCurrent(hDC, pGLView->m_hRC);
//	if(FALSE == ::wglMakeCurrent(hDC, pGLView->m_hRC))
//	{
//		::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
//		throw SolveIt_ERROR(L"Unable to //wglMakeCurrent");
//	//	return;
//	}

	GLenum e=GL_NO_ERROR;
	ASSERT(listEigenStates[j]	== firstInlistEigenStates + j);

	glPushMatrix();
	glColor3d(0.50*fabs(sin(2*j+1)),0.50*fabs(cos(j)),0.50*fabs(sin(j+1)));

	glDisable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
//	glBegin(GL_TRIANGLE_STRIP);
{

	FORALLfacets
	{
//		sprintf(name, "f%d", nf++);
		vertices= qh_facet3vertex (facet);
		facelist.clear();
		FOREACHvertex_(vertices) {
			idx = qh_pointid(vertex->point);
			vertUsed[idx] = 1;
			facelist.push_back(hullVerts[idx]);
		}
//		addFace(name, facelist, 1); // qhull generates a clockwise list

		if (!facelist.empty() && facelist.size()>2)
		{
			vector<Vertex *>::iterator it3=facelist.begin();
			for(/*it3=facelist.begin()*/;it3!=facelist.end();/*++it3*/)
			{
				vector<Vertex *>::iterator it1=it3++;
				if ( it1 == facelist.end() ) break;
				vector<Vertex *>::iterator it2=it3++;
				if ( it2 == facelist.end() ) break;
				if ( it3 == facelist.end() ) break;
//				glVertex3d(it->x,it->y,it->z);
			//	X1= x1 + w*(*it1)->x;
			//	Y1= y1 + h*(*it1)->y;
				X1= (*it1)->x;
				Y1= (*it1)->y;
				Z1= (*it1)->z;
				X2= (*it2)->x;
				Y2= (*it2)->y;
			//	X2= x1 + w*(*it2)->x;
			//	Y2= y1 + h*(*it2)->y;
				Z2= (*it2)->z;
				X3= (*it3)->x;
				Y3= (*it3)->y;
				Z3= (*it3)->z;
				if (
					(Z1<max && Z2<max && Z3<max) &&
					(0<=Z1 && 0<=Z2 && 0<=Z3) &&
					(X1<=x2 && x1<=X1) &&
					(X2<=x2 && x1<=X2) &&
					(X3<=x2 && x1<=X3) &&
					(Y1<=y2 && y1<=Y1) &&
					(Y2<=y2 && y1<=Y2) &&
					(Y3<=y2 && y1<=Y3)
					)
				{
					nx=(Y1-Y2)*(Z3-Z2)-(Z1-Z2)*(Y3-Y2);
					ny=(Z1-Z2)*(X3-X2)-(X1-X2)*(Z3-Z2);
					nz=(X1-X2)*(Y3-Y2)-(Y1-Y2)*(X3-X2);
					n2 = nx*nx + ny*ny + nz*nz;
					if ( n2 < 0.1e-12) continue;
			//		if (         ny*ny + nz*nz < 1.0e-8*n2 ) continue;//omit if nx = �1
			//		if ( nx*nx +         nz*nz < 1.0e-8*n2 ) continue;//omit if ny = �1
			//		if ( nx*nx + ny*ny         < 1.0e-6*n2 ) continue;//omit if nz = �1
					glNormal3d(-nx,-ny,-nz);
					glVertex3d(X1,Y1,Z1);
					glVertex3d(X2,Y2,Z2);
					glVertex3d(X3,Y3,Z3);
				}
			}
		}
		qh_settempfree(&vertices);
	}//end FORALLfacets
}
	glEnd();
	glPopMatrix();
	glFlush();
	glFinish();
//	SwapBuffers(hDC);
	//wglMakeCurrent(0, 0);
//	::ReleaseDC(pGLView->GetSafeHwnd(), hDC);

/*
	glNewList(listEigenStates[j], GL_COMPILE);
//	glNewList(listEigenStates[j], GL_COMPILE_AND_EXECUTE);

	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		//wglMakeCurrent(0, 0);
		::ReleaseDC(pGLView->GetSafeHwnd(), hDC);
		throw SolveIt_ERROR(L"Unable to glNewList");
	//	ASSERT(0);
	//	return;
	}

		GLubyte image[3*imageWidth*imageHeight];
*/
	GLuint texture;
	pGLView->PixelsToTexture(pGLView->PixelsSnapshot(), texture);
	pGLView->TextureToDisplayList(listEigenStates[j], texture);

/*
	// strip out unused vertices, count remaining ones
	for (vi = verts.begin(), i = 0; vi != verts.end(); i++)
		if (vertUsed[i]) vi++;
		else {
			vi0 = vi;
			vi++;
			verts.erase(vi0);
		}
	nv = verts.size();
*/
	//cout << nf << " faces, " << nv << " vertices " << endl;

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);

	return 0;
/*
error:
	cerr << "error building convex hull of Polyhedron \n" << endl;
	cerr << "exitcode: " << exitcode << ends;
		::MessageBox(NULL,err,"CQMStationary2D::BuildHull",MB_OK);

	qh NOerrexit= True;
	qh_freeqhull (!qh_ALL);
	qh_memfreeshort (&curlong, &totlong);

	return 1;
*/
}




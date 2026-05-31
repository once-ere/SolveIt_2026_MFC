// Draw3DAxes.cpp : Helper implementation of COpenGLObj

#include "stdafx.h"

#include "SolveIt.h"
#include "GLView.h"
#include "SolveIt.Lib/lib/Lib3D/gl2ps.h"
#include "System.h"

#include <limits>

#define gl2psText(text, font, size) gl2psText((text), const_cast<char*>(font), (size))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern GL2PScontext *gl2ps;

namespace {

GLsizei ToGlListCount(size_t count)
{
	ASSERT(count <= static_cast<size_t>((std::numeric_limits<GLsizei>::max)()));
	return static_cast<GLsizei>(count);
}

template <size_t N, typename... Args>
void FormatAxisText(wchar_t (&buffer)[N], const wchar_t* format, Args... args)
{
	_snwprintf_s(buffer, N, _TRUNCATE, format, args...);
}

}

//////////////////////////////////////////////////////////////////////

void renderVerticalBitmapString(
			double x, 
			double y, 
			double bitmapHeight, 
			int base,
			const wchar_t *wstring)
{
	const wchar_t *c=0;
	int i;
	for (c=wstring,i=0; *c != '\0'; i++,c++)
	{
		glRasterPos2d(x, y-bitmapHeight*i);
		glCallList(base + __toascii(*c)) ;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CView
/*
void InitGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LESS);			
	glClearColor(1, 1, 1, 0);
}
*/
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CGLView::Draw3DAxes(const wchar_t* xStr, const wchar_t* yStr)
{
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper)
	{
		return;
	}
	if (m_system->m_bDoVerhulstDynamics)
	{
		return;
	}
	const float fTicHt = 0.023f/2;
	const float fTicHt1= 0.028f/2;
	const float fTicHt2= 0.028f/2;
	wchar_t s[256];
	wstring str;
	double x, y, z, fTicH=0;
	long i=0, nTicks=101;
///////////////////////////////////////////////////////////////////////////////
	double	DX = glFrustum_right - glFrustum_left;
	double	DY = glFrustum_top - glFrustum_bottom;
	double	DZ = 2*glFrustum_znear;//glFrustum_zfar - glFrustum_znear;
	double	dTransX = viewWidth/DX;
	double	dTransY = viewHeight/DY;
	double	dTransZ = DZ/2;
////
	GLfloat txtColor1[3] = { 0, 0, 0 };
	GLfloat ticColor1[3] = { 0.30f, 0.15f, 0 };
	GLfloat ticColor2[3] = { 0.15f, 0.08f, 0.03f };

//	const double mr[4]={2,2,2.5,2};
	double range[3*22];
	range[0] = 0.00001e-5;
	range[1] = 0.00002e-5;
	range[2] = 0.00005e-5;
	for (int j=1;j< (sizeof(range)/sizeof(range[0]))/3;++j)
	{
		range[3*j+0] = 10*range[3*(j-1)+0];
		range[3*j+1] = 10*range[3*(j-1)+1];
		range[3*j+2] = 10*range[3*(j-1)+2];
	}

	double dx = DX/(nTicks-1);
	double dy = DY/(nTicks-1);
	double dz = DZ/(nTicks-1);

///////////////////////////////////////////////////////////////////////////////
	bool xDone=false;
	bool yDone=false;
	bool zDone=false;
	if ( dx < range[0] ) dx	= range[0];
	if ( dy < range[0] ) dy	= range[0];
	if ( dz < range[0] ) dz	= range[0];
	for ( auto j=0;j< sizeof(range)/sizeof(range[0])-1;++j)
	{
		if (!xDone && (range[j] < dx) && (dx < range[j+1]))
		{
		//	ASSERT(range[j] == dx);float FAILS!
			ASSERT(range[j] < dx);
			ASSERT(dx < range[j+1]);
			dx	= range[j+1], xDone=true;
		}
		if (!yDone && (range[j] < dy) && (dy < range[j+1])) dy	= range[j+1], yDone=true;
		if (!zDone && (range[j] < dz) && (dz < range[j+1])) dz	= range[j+1], zDone=true;
	}

	GLenum err = glGetError();
	glShadeModel(GL_FLAT);
	glDisable(GL_DITHER);
	ASSERT(err == GL_NO_ERROR);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	if (gl2ps==0) glListBase(2000);
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);		 
	}
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
	GLfloat XnegativeColor[3] = { 0.7f, 0, 0 };
	GLfloat XpositiveColor[3] = { 1, 0.2f, 0.2f };
	GLfloat YnegativeColor[3] = { 0, 0.3f, 0 };
	GLfloat YpositiveColor[3] = { 0, 0.6f, 0 };
	GLfloat ZnegativeColor[3] = { 0, 0, 0.37f };
	GLfloat ZpositiveColor[3] = { 0.5f, 0.4f, 1 };
///////////////////////////////////////////////////////////////////////////////
// draw the two lines that make up the x axis
	glLineWidth(0.7f);
	glBegin(GL_LINES);
	if (glFrustum_left<0.0)
	{
		glColor3fv( XnegativeColor );
		glVertex3d(	glFrustum_left,	0, 0);
		glVertex3d( 0, 0, 0);
	}
	glColor3fv( XpositiveColor );
	glVertex3d(	glFrustum_left,		0, 0);
	glVertex3d(	glFrustum_right,	0, 0);
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
// draw the two lines that make up the y axis
	glLineWidth(0.7f);
	glBegin(GL_LINES);
	glColor3fv( YnegativeColor );
	glVertex3d( 0,	glFrustum_bottom, 0);
	glVertex3d( 0,	0, 0);

	glColor3fv( YpositiveColor );
	glVertex3d( 0,	0, 0);
	glVertex3d( 0,	glFrustum_top, 0);
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
// draw the two lines that make up the z axis
	glBegin(GL_LINES);
	glColor3fv( ZnegativeColor );//
	glVertex3d( 0, 0,  -glFrustum_znear);//-dTransZ//
	glVertex3d( 0, 0, 0 );//

	glColor3fv( ZpositiveColor );
	glVertex3d( 0, 0, 0 );
	glVertex3d( 0, 0,  glFrustum_znear);//dTransZ
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
// draw the tick marks
	glLineWidth(1);
	glBegin(GL_LINES);
	for (i=0,x = 0; x <= glFrustum_right; x += dx,i++ )
	{
		fTicH = ( fTicHt*DY );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DY ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DY ), glColor3fv( ticColor1 );
		}
		else if ( x < 0 )
		{
			glColor3fv( XnegativeColor );
		}
		else
		{
			glColor3fv( XpositiveColor );
		}
		glVertex3d( x, 0, 0);
		glVertex3d( x, fTicH, 0);
	}
	for (i=0,x = glFrustum_left; x <= 0; x += dx,i++ )
	{
		fTicH = ( fTicHt*DY );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DY ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DY ), glColor3fv( ticColor1 );
		}
		else if ( x < 0 )
		{
			glColor3fv( XnegativeColor );
		}
		else
		{
			glColor3fv( XpositiveColor );
		}
		glVertex3d( x, 0, 0);
		glVertex3d( x, fTicH, 0);
	}
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// draw the tick marks
	glLineWidth(1);
	glBegin(GL_LINES);
	for (i = 0, y=0; y <= glFrustum_top; y += dy, i++ )
	{
		fTicH = ( fTicHt*DX );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DX ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DX ), glColor3fv( ticColor1 );
		}
		else if ( y < 0 )
		{
			glColor3fv( YnegativeColor );
		}
		else
		{
			glColor3fv( YpositiveColor );
		}
		glVertex3d( 0, y, 0);
		glVertex3d( fTicH, y, 0);
	}
	for (i = 0, y=glFrustum_bottom; y<=0; y += dy, i++ )
	{
		fTicH = ( fTicHt*DX );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DX ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DX ), glColor3fv( ticColor1 );
		}
		else if ( y < 0 )
		{
			glColor3fv( YnegativeColor );
		}
		else
		{
			glColor3fv( YpositiveColor );
		}
		glVertex3d( 0, y, 0);
		glVertex3d( fTicH, y, 0);
	}
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// draw the tick marks
	glLineWidth(1);
	glBegin(GL_LINES);
	for (i = 0, z=0; z <= glFrustum_znear	; z+=dz, i++ )//glFrustum_zfar
	{
		fTicH = ( fTicHt*DY );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DY ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DY ), glColor3fv( ticColor1 );
		}
		else if ( z < 0 )
		{
			glColor3fv( ZnegativeColor );
		}
		else
		{
			glColor3fv( ZpositiveColor );
		}
//		fTicH *= dTransY*1.5;
		glVertex3d( 0, 0, z);
		glVertex3d( 0, fTicH, z);
	}
	for (i = 0, z=0; z>=-glFrustum_znear; z-=dz, i++ )//glFrustum_znear
	{
		fTicH = ( fTicHt*DY );
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = ( fTicHt2*DY ), glColor3fv( ticColor2 );
			else fTicH = ( fTicHt1*DY ), glColor3fv( ticColor1 );
		}
		else if ( z < 0 )
		{
			glColor3fv( ZnegativeColor );
		}
		else
		{
			glColor3fv( ZpositiveColor );
		}
//		fTicH *= dTransY*1.5;
		glVertex3d( 0, 0, z);
		glVertex3d( 0, fTicH, z);
	}
/*
*/
	glEnd();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
//Label
	const float fLabel= 0.17f;

	glColor3fv( txtColor1 );
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	m_min_tic_mark_spacing_x=((glFrustum_right - glFrustum_left)/m_num_tic_marks_x);
	m_min_tic_mark_spacing_y=((glFrustum_top - glFrustum_bottom)/m_num_tic_marks_y);
//	const wchar_t format[] = L"%4.2f";
//	const wchar_t formatX[] = L"%7.4g";
	const wchar_t formatX[] = L"%6.3g";
	const wchar_t format[] = L"%8.5g";
// now Label the x axis tick marks
	const int iScale = 10;
if (m_min_tic_mark_spacing_x > 0.0)
{

	
	double m_last_tic_mark_x	= -m_min_tic_mark_spacing_x;
//	double m_last_tic_mark_y	= -m_min_tic_mark_spacing_y;
	double m_last_tic_mark_y	= -m_min_tic_mark_spacing_x;

	
	for (i=0,x = 0; x <= glFrustum_right; x += dx,i++ )
	{
		if (x-m_last_tic_mark_x	< m_min_tic_mark_spacing_x) continue;
		m_last_tic_mark_x	= x;
		FormatAxisText(s, formatX, x);
		str = s;
		glRasterPos3d( x, -4*dy, 0);

		if (gl2ps)
		{
			gl2psText(GetC(str.c_str()), const_cast<char*>("Courier"), 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	m_last_tic_mark_x	= glFrustum_left-m_min_tic_mark_spacing_x;
	for (i=0,x = glFrustum_left; x <= 0; x += dx,i++ )
	{
		if (i==0) continue;
		if (x-m_last_tic_mark_x	< m_min_tic_mark_spacing_x) continue;
		m_last_tic_mark_x	= x;
		FormatAxisText(s, formatX, x);
		str = s;
		glRasterPos3d( x, -4*dy, 0);
		if (gl2ps)
		{
			gl2psText(GetC(str.c_str()), const_cast<char*>("Courier"), 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}	
	
	
	for (i = 0, y=0; y <= glFrustum_top; y += dy, i++ )
	{
		if (i==0) continue;
	//	if (y-m_last_tic_mark_y	< m_min_tic_mark_spacing_y) continue;
		if (y-m_last_tic_mark_y	< m_min_tic_mark_spacing_x) continue;
		m_last_tic_mark_y	= y;
		FormatAxisText(s, formatX, y);
		str = s;
		glRasterPos3d( -8*dx, y, 0);
		if (gl2ps)
		{
			gl2psText(GetC(str.c_str()), const_cast<char*>("Courier"), 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
//	m_last_tic_mark_y	= glFrustum_bottom-m_min_tic_mark_spacing_y;
	m_last_tic_mark_y	= glFrustum_bottom-m_min_tic_mark_spacing_x;
	for (i = 0, y=glFrustum_bottom; y<=0; y += dy, i++ )
	{
		if (i==0) continue;
//		if (y-m_last_tic_mark_y	< m_min_tic_mark_spacing_y) continue;
		if (y-m_last_tic_mark_y	< m_min_tic_mark_spacing_x) continue;
		m_last_tic_mark_y	= y;
		FormatAxisText(s, formatX, y);
		str = s;
		glRasterPos3d( -8*dx, y, 0);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}

	
#if 0
	
	for (i=0; i <= nTicks*glFrustum_right/DX; i+=10)//x = (x+10*dx) )
	{
	//	_stprintf(s,formatX, double(x*dTransX));
	//	if (x%DX) continue;*dTransX
		_stprintf(s,L"%d", i/iScale);
		str = s;
		glRasterPos3d( i*dx, -4*dy, 0);
	//	glRasterPos2i( (x-12.5*dx), (iY0-3*dy));

		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	}
	for (i=0; i >= nTicks*glFrustum_left/DX; i-=10)
	{
		if (i==0) continue;
		_stprintf(s,L"%d", i/iScale);
		str = s;
		glRasterPos3d( i*dx, -4*dy, 0);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	}
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
// now Label the y axis tick marks
//	double	DX = glFrustum_right - glFrustum_left;
//	double	DY = glFrustum_top - glFrustum_bottom;
	for (i=0; i <= nTicks*glFrustum_top/DY; i+=10)
	{
		if (i==0) continue;
		_stprintf(s,L"%d", i/iScale);
		_stprintf(s,L"%g", double(i)/iScale);
		str = s;
		glRasterPos3d( -4*dx, i*dy, 0);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	}
	for (i=0; i >= nTicks*glFrustum_bottom/DY; i-=10)
	{
		if (i==0) continue;
		_stprintf(s,L"%d", i/iScale);
		_stprintf(s,L"%g", double(i)/iScale);
		str = s;
		glRasterPos3d( -4*dx, i*dy, 0);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	}
#endif
/*
	for (y=0; y<=iY0; y=(y+10*dy) )
	{
		if (y==iY0) continue;
	//	_stprintf(s,format, double((y-iY0)*dTransY));
		if ((iY0-y)%DY) continue;
		_stprintf(s,L"%d", y-iY0);
		str = s;
		glRasterPos2i( 0/2, y);

		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
			glPopAttrib();
		}
	}
*/
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
///////////////////////////////////////////////////////////////////////////////
// now Label the z axis tick marks
	for (i=0; i <= nTicks*glFrustum_znear/DZ; i+=10)
	{
		if (i==0) continue;
		FormatAxisText(s, L"%d", i/iScale);
		str = s;
		glRasterPos3d( 0, -4*dy, i*dz);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	for (i=0; i >= -nTicks*glFrustum_znear/DZ; i-=10)
	{
		if (i==0) continue;
		FormatAxisText(s, L"%d", i/iScale);
		str = s;
		glRasterPos3d( 0, -4*dy, i*dz);
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		double x_Pos = 0;
		double y_Pos = 0;
		double z_Pos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeScreenCoords(x_Pos, y_Pos, z_Pos, false);
	//	ComputeWorldCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeWorldCoords(x_Pos, y_Pos, z_Pos, false);
	if (xStr)
	{
		FormatAxisText(s, xStr, float(dx*iScale));
		glRasterPos3d( x_Pos+5*dx, y_Pos-2*dy, 0);
	//	glRasterPos3d( 15*dx, -12*dy, 0);
		if (gl2ps)
		{
			gl2psText( GetC(s), "Arial", 14);//xStr
		}
		else
		{
			const char* axisText = GetC(s);
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(strlen(axisText)), GL_BYTE, reinterpret_cast<const GLbyte *>(axisText));//xStr
			glPopAttrib();
		}
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	}
///////////////////////////////////////////////////////////////////////////////
	if (yStr)
	{
		FormatAxisText(s, yStr, float(dy*iScale));
renderVerticalBitmapString(x_Pos-5*dx, y_Pos+2*dy+strlen(GetC(s))*DY/50, DY/50, 2000, s);
		glRasterPos3d( 3*dx, glFrustum_top - 6*dy, 0);
		if (gl2ps)
		{
			gl2psText( GetC(s), "Arial", 14);//yStr
		}
		else
		{
			const char* axisText = GetC(s);
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(strlen(axisText)), GL_BYTE, reinterpret_cast<const GLbyte *>(axisText));//yStr
			glPopAttrib();
		}
/*
		x=-18*dx;
		y= glFrustum_top - 6*dy;
		const wchar_t *c=yStr;
		float bitmapHeight = 14/100;
		for (int i=0; *c != '\0'; i++,c++)
		{
			glRasterPos2f(x, y-bitmapHeight*i);
			wchar_t ch[2]={*c,0};
			if (gl2ps)
			{
				gl2psText(ch, "Arial", 14);
			}
			else
			{
				glPushAttrib(GL_LIST_BIT);
				glListBase(2000);
				glCallLists(1, GL_BYTE, (GLbyte *)ch);
				glPopAttrib();
			}
		}
*/
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	}


	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
	glPopAttrib();
//			glFinish();
//			glFlush();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
	{
		glPushMatrix();
		glRotatef(90, 0.0f, 0.0f, 1.0f);
		glPopMatrix();
	}
// Pixel format and rendering context already set up and initialized.

// Get the DC for the current rendering context.
HDC hdc = wglGetCurrentDC();
CDC* pdc = CDC::FromHandle(hdc) ;

// Select the font you want to use.
CFont* pOldFont = (CFont*)pdc->SelectObject(m_pFontSelected) ;

// Allocate structure for character metrics.
GLYPHMETRICSFLOAT agmf;

// Build the display lists.
wglUseFontOutlines(hdc,               // DC with font
				   __toascii('A'),    // Character to generate
				   1,                 // Number of characters
				   1000,              // Display-list number
				   0.0,               // Deviation
				   0.1,               // Extrusion
				   WGL_FONT_POLYGONS, // Format
				   &agmf) ;           // Metrics pointer

// Set up transformation.
glLoadIdentity();
glTranslated(0.5, 0.0, -2.0);

// Display the 3-D letter 'A'
glCallList(1000) ;



// Allocate structure for character metrics.
GLYPHMETRICSFLOAT agmf[256];

// Build the display lists.
wglUseFontOutlines(hdc,               // DC with font
				   0,                 // Starting character index
				   256,               // Number of characters
				   1000,              // Display-list number
				   0.0,               // Deviation
				   0.1,               // Extrusion
				   WGL_FONT_POLYGONS, // Format
				   agmf) ;            // Metrics pointer

// Set up transformation.
glLoadIdentity();
glTranslated(0.5, 0.0, -2.0);

// Display the 3-D letters 'Hello!'
glCallList(1000 + __toascii('H')) ;
glCallList(1000 + __toascii('e')) ;
glCallList(1000 + __toascii('l')) ;
glCallList(1000 + __toascii('l')) ;
glCallList(1000 + __toascii('o')) ;
glCallList(1000 + __toascii('!')) ;

void renderVerticalBitmapString(
			float x, 
			float y, 
			int bitmapHeight, 
			int base,
			const wchar_t *wstring)
{
	wchar_t *c=0;
	int i;
	for (c=wstring,i=0; *c != '\0'; i++,c++)
	{
		glRasterPos2f(x, y+bitmapHeight*i);
		glCallList(base + __toascii(*c)) ;
	}
}


void renderVerticalBitmapString(
			float x, 
			float y, 
			int bitmapHeight, 
			void *font,
			wchar_t *wstring)
{
  
  wchar_t *c;
  int i;
  for (c=wstring,i=0; *c != '\0'; i++,c++) {
	glRasterPos2f(x, y+bitmapHeight*i);
	glutBitmapCharacter(font, *c);
  }
}

*/
//////////////////////////////////////////////////////////////////////
//START-UP FOUND BY PLN 03FEB2020 after days of searching
void CGLView::Draw3DAxes(long nTicks)
{
	CSystem* m_system = theApp.GetSystem();
	if (m_system->m_bDoMapper)
	{
		return;
	}
	if (m_system->m_bDoVerhulstDynamics)
	{
		return;
	}
	const float fTicHt = 0.07f;
	const float fTicHt1= 0.10f;
	const float fTicHt2= 0.14f;
	GLfloat  dTransX = static_cast<GLfloat>((glFrustum_right - glFrustum_left) / 8.0);
	GLfloat  dTransY = static_cast<GLfloat>((glFrustum_top - glFrustum_bottom) / 8.0);
	GLfloat  dTransZ = static_cast<GLfloat>((glFrustum_zfar - glFrustum_znear) / 2.0);

	wchar_t s[8];
	wstring str;
	float x,y,z, fTicH;
	GLfloat txtColor1[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat ticColor1[3] = { 0.30f, 0.15f, 0.00f };
	GLfloat ticColor2[3] = { 0.15f, 0.08f, 0.03f };

	if (nTicks<1) return;
	glListBase(1000);
	GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
	if (bEnableLighting==GL_TRUE)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);     
	}
	GLfloat XnegativeColor[3] = { 0.7f, 0.0f, 0.0f };
	GLfloat XpositiveColor[3] = { 1.0f, 0.2f, 0.2f };
	GLfloat YnegativeColor[3] = { 0.0f, 0.3f, 0.0f };
	GLfloat YpositiveColor[3] = { 0.0f, 0.6f, 0.0f };
	GLfloat ZnegativeColor[3] = { 0.0f, 0.0f, 0.7f };
	GLfloat ZpositiveColor[3] = { 0.4f, 0.4f, 1.0f };
//	GLfloat w = (glFrustum_right - glFrustum_left)/2;
//	GLfloat h = (glFrustum_top - glFrustum_bottom)/2;
//	GLfloat d = (glFrustum_zfar - glFrustum_znear)/2;
///////////////////////////////////////////////////////////////////////////////
//	m_min_tic_mark_spacing_x=((glFrustum_right - glFrustum_left)/m_num_tic_marks_x);
//	m_min_tic_mark_spacing_y=((glFrustum_top - glFrustum_bottom)/m_num_tic_marks_y);
	GLfloat dx = static_cast<GLfloat>((glFrustum_right - glFrustum_left) / (m_num_tic_marks_x - 1));
	GLfloat dy = static_cast<GLfloat>((glFrustum_top - glFrustum_bottom) / (m_num_tic_marks_y - 1));
///////////////////////////////////////////////////////////////////////////////
//	GLfloat dx = (glFrustum_right - glFrustum_left)/(nTicks-1);
//	GLfloat dy = (glFrustum_top - glFrustum_bottom)/(nTicks-1);
	GLfloat dz = static_cast<GLfloat>((glFrustum_zfar - glFrustum_znear) / (nTicks - 1));
///////////////////////////////////////////////////////////////////////////////
//	long n = (nTicks-1)/2;
	GLenum err = glGetError();
	glShadeModel(GL_FLAT);
	glDisable(GL_DITHER);
	ASSERT(err == GL_NO_ERROR);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	if (gl2ps==0) glListBase(2000);
			
///////////////////////////////////////////////////////////////////////////////
// now draw the two lines that make up the z axis
	glBegin(GL_LINES);
	glColor3fv( ZnegativeColor );
	glVertex3f( 0.0f, 0.0f, -dTransZ );//glFrustum_znear
	glVertex3f( 0.0f, 0.0f, 0.0f );

	glColor3fv( ZpositiveColor );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, dTransZ );//glFrustum_zfar
///////////////////////////////////////////////////////////////////////////////
// now draw the tick marks
///////////////////////////////////////////////////////////////////////////////
	int i;
	for (i = 0, z=0; z <= dTransZ  ; z+=dz, i++ )//glFrustum_zfar
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( z < 0.0f )
		{
			glColor3fv( ZnegativeColor );
		}
		else
		{
			glColor3fv( ZpositiveColor );
		}
		fTicH *= dTransY * 1.5f;
		glVertex3f( 0.00f, 0.00f, z);
//		glVertex3f( fTicH, fTicH, z);
		glVertex3f( 0.00f, fTicH, z);
	}
	for (i = 0, z=0; z>=-dTransZ; z-=dz, i++ )//glFrustum_znear
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( z < 0.0f )
		{
			glColor3fv( ZnegativeColor );
		}
		else
		{
			glColor3fv( ZpositiveColor );
		}
		fTicH *= dTransY * 1.5f;
		glVertex3f( 0.00f, 0.00f, z);
//		glVertex3f( fTicH, fTicH, z);
		glVertex3f( 0.00f, fTicH, z);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// now draw the two lines that make up the x axis
	glColor3fv( XnegativeColor );
	glVertex3f(  static_cast<GLfloat>(glFrustum_left),  0.0f, 0.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f );

	glColor3fv( XpositiveColor );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( static_cast<GLfloat>(glFrustum_right),  0.0f, 0.0f);
///////////////////////////////////////////////////////////////////////////////
// now draw the tick marks
///////////////////////////////////////////////////////////////////////////////
	for (i=0,x = 0; x <= glFrustum_right  ; x+=dx ,i++ )
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( x < 0.0f )
		{
			glColor3fv( XnegativeColor );
		}
		else
		{
			glColor3fv( XpositiveColor );
		}
		fTicH *= dTransY;
		glVertex3f( x, 0.00f, 0.00f);
	//	glVertex3f( x, fTicH, fTicH);
		glVertex3f( x, fTicH, 0.00f);
	}
	for (i=0,x = 0;x >= glFrustum_left; x-=dx ,i++ )
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( x < 0.0f )
		{
			glColor3fv( XnegativeColor );
		}
		else
		{
			glColor3fv( XpositiveColor );
		}
		fTicH *= dTransY;
		glVertex3f( x, 0.00f, 0.00f);
	//	glVertex3f( x, fTicH, fTicH);
		glVertex3f( x, fTicH, 0.00f);
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// now draw the two lines that make up the y axis
	glColor3fv( YnegativeColor );
	glVertex3f( 0.0f,   static_cast<GLfloat>(glFrustum_bottom), 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );

	glColor3fv( YpositiveColor );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, static_cast<GLfloat>(glFrustum_top), 0.0f );
///////////////////////////////////////////////////////////////////////////////
// now draw the tick marks
///////////////////////////////////////////////////////////////////////////////
	for (i = 0, y=0; y <= glFrustum_top  ; y+= dy, i++ )
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( y < 0.0f )
		{
			glColor3fv( YnegativeColor );
		}
		else
		{
			glColor3fv( YpositiveColor );
		}
		fTicH *= dTransX;
		glVertex3f( 0.00f, y, 0.00f);
//		glVertex3f( fTicH, y, fTicH);
		glVertex3f( fTicH, y, 0.00f);
	}
	for (i = 0, y=0; y>=glFrustum_bottom; y-= dy, i++ )
	{
		fTicH = fTicHt;
		if ( !(i%5) ) {
			if ( !(i%10) ) fTicH = fTicHt2, glColor3fv( ticColor2 );
			else fTicH = fTicHt1, glColor3fv( ticColor1 );
		}
		else if ( y < 0.0f )
		{
			glColor3fv( YnegativeColor );
		}
		else
		{
			glColor3fv( YpositiveColor );
		}
		fTicH *= dTransX;
		glVertex3f( 0.00f, y, 0.00f);
//		glVertex3f( fTicH, y, fTicH);
		glVertex3f( fTicH, y, 0.00f);
	}
	glEnd();
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Label
///////////////////////////////////////////////////////////////////////////////
	const float fLabel= 0.17f;

	glColor3fv( txtColor1 );
// now draw the two lines that make up the z axis
///////////////////////////////////////////////////////////////////////////////
// now Label the tick marks
	for (z=0; z <=  dTransZ ; z+=10*dz )//glFrustum_zfar
	{
		FormatAxisText(s, L"%4.2f", z);
		str = s;
		glRasterPos3f( 0, 0, z-fLabel);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	for (z=0; z>=-dTransZ; z-=10*dz )//glFrustum_znear
	{
		FormatAxisText(s, L"%4.2f", z);
		str = s;
		glRasterPos3f( 0, 0, z-fLabel);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// now Label the x axis tick marks
	for (x=0; x <= glFrustum_right  ; x+=10*dx )
	{
		FormatAxisText(s, L"%4.2f", x);
		str = s;
		if (fabs(x)<=0.01)
			glRasterPos3f( -3*dx, -2*dy, 0);
		else
			glRasterPos3f( x-1*dx, -2*dy, 0);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	for (x=0; x>=glFrustum_left; x-=10*dx )
	{
		FormatAxisText(s, L"%4.2f", x);
		str = s;
		if (fabs(x)<=0.01)
			glRasterPos3f( -3*dx, -2*dy, 0);
		else
			glRasterPos3f( x-1*dx, -2*dy, 0);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// now Label the y axis tick marks
	for (y=0; y <= glFrustum_top  ; y+=10*dy )
	{
		if (fabs(y)<=0.01) continue;
		FormatAxisText(s, L"%4.2f", y);
		str = s;
		glRasterPos3f( -4*dx, y-1*dy, 0);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
	for (y=0; y>=glFrustum_bottom; y-=10*dy )
	{
		if (fabs(y)<=0.01) continue;
		FormatAxisText(s, L"%4.2f", y);
		str = s;
		glRasterPos3f( -4*dx, y-1*dy, 0);
	//	glPushAttrib(GL_LIST_BIT);
	//	glListBase(2000);
	//	glCallLists(str.size(), GL_BYTE, (GLbyte *)str.c_str());
	//	glPopAttrib();
		if (gl2ps)
		{
			gl2psText( GetC(str.c_str()), "Courier", 12);
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(str.size()), GL_BYTE, reinterpret_cast<const GLbyte *>(str.c_str()));
			glPopAttrib();
		}
	}
///////////////////////////////////////////////////////////////////////////////
// axis labels/titles
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	const wchar_t* xStr	= GetWC(m_system->m_xStr.c_str());
	const wchar_t* yStr	= GetWC(m_system->m_yStr.c_str());
///////////////////////////////////////////////////////////////////////////////
	const int iScale = 10;

		double xPos = 0;
		double yPos = 0;
		double zPos = 0;//z == 0 => z == m_fNearPlane
	//	ComputeScreenCoords(&xPos, &yPos, &zPos);
		ComputeScreenCoords(xPos, yPos, zPos, false);
	if (xStr)
	{
		double x_Pos = xPos;
		double y_Pos = yPos;
		double z_Pos = zPos;
		y_Pos += 2*14;
	//	ComputeWorldCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeWorldCoords(x_Pos, y_Pos, z_Pos, false);
	//	const double	DX = glFrustum_right - glFrustum_left;
	//	const double	DY = 0.5*(glFrustum_top + glFrustum_bottom);
		FormatAxisText(s, xStr, float(dx*iScale));
	//	glRasterPos3d( x_Pos+5*dx, y_Pos-2*dy, 0);
		glRasterPos3d( x_Pos+5*dx, y_Pos-2*dy, 0);
	//	glRasterPos3d( 15*dx, -12*dy, 0);
		if (gl2ps)
		{
			gl2psText(GetC(s), const_cast<char*>("Arial"), 14);//xStr
		}
		else
		{
			const char* axisText = GetC(s);
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(strlen(axisText)), GL_BYTE, reinterpret_cast<const GLbyte *>(axisText));//xStr
			glPopAttrib();
		}
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	}
///////////////////////////////////////////////////////////////////////////////
	if (yStr)
	{
	//	const double	DX = glFrustum_right - glFrustum_left;
		const double	DY = glFrustum_top - glFrustum_bottom;
		double x_Pos = xPos;
		double y_Pos = yPos;
		double z_Pos = zPos;
		x_Pos -= 4*14;
		y_Pos -= 4*14;
	//	ComputeWorldCoords(&x_Pos, &y_Pos, &z_Pos);
		ComputeWorldCoords(x_Pos, y_Pos, z_Pos, false);

		FormatAxisText(s, yStr, float(dy*iScale));

//renderVerticalBitmapString(x_Pos-5*dx, y_Pos+2*dy+strlen(s)*DY/50, DY/50, 2000, s);
renderVerticalBitmapString(x_Pos, y_Pos+ strlen(GetC(s))*DY/50, DY/50, 2000, s);

		glRasterPos3d( 3*dx, glFrustum_top - 6*dy, 0);
		if (gl2ps)
		{
			gl2psText(GetC(s), const_cast<char*>("Arial"), 14);//yStr
		}
		else
		{
			const char* axisText = GetC(s);
			glPushAttrib(GL_LIST_BIT);
			glListBase(2000);
			glCallLists(ToGlListCount(strlen(axisText)), GL_BYTE, reinterpret_cast<const GLbyte *>(axisText));//yStr
			glPopAttrib();
		}
/*
		x=-18*dx;
		y= glFrustum_top - 6*dy;
		const wchar_t *c=yStr;
		float bitmapHeight = 14/100;
		for (int i=0; *c != '\0'; i++,c++)
		{
			glRasterPos2f(x, y-bitmapHeight*i);
			wchar_t ch[2]={*c,0};
			if (gl2ps)
			{
				gl2psText(ch, "Arial", 14);
			}
			else
			{
				glPushAttrib(GL_LIST_BIT);
				glListBase(2000);
				glCallLists(1, GL_BYTE, (GLbyte *)ch);
				glPopAttrib();
			}
		}
*/
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
	}



///////////////////////////////////////////////////////////////////////////////
	if (bEnableLighting==GL_TRUE)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
	glPopAttrib();
//			glFinish();
//			glFlush();
	err = glGetError();
	ASSERT(err == GL_NO_ERROR);
}


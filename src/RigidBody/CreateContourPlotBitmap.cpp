// RigidBodyCnstr.cpp: implementation of the CRigidBody class.
//

#include "stdafx.h"
#include "SolveIt.h"
#include "System.h"
////#include "parser_2020.h"
#include "Types.h"
#include "GLView.h"
#include "instruction.h"
#include "StackMachine.h"

#include "DibSectn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 


///////////////////////////////////////////////////////////////////////////////
#define SWAPBYTESW(w) (w)
#define SWAPBYTESL(l) (l)
//speed things up:
// **	imagic		unsigned short	 2 bytes	imagic = 0x01da,
// **					      /4Dgifts/iristools/libimage/open.c
// **						  uses this to determine the
// **						  byte ordering and revision
// **						  number (see dorev)
//Inventor will swap bytes if necessary; do this for Inventor:
/*
#undef SWAPBYTESW
#undef SWAPBYTESL

#define SWAPBYTESW(w) MAKEWORD(HIBYTE(w), LOBYTE(w))
#define SWAPBYTESL(l) MAKELONG(SWAPBYTESW(LOWORD(l)), SWAPBYTESW(HIWORD(l)))
*/

// RGB file header
struct imRgbHeaderInfo
{
	unsigned short rgb_imagic;	/* Magic number (0x01da)	*/
	unsigned short rgb_stype;	/* Storage type			*/
	unsigned short rgb_dim;		/* Number of dimension of image	*/
	unsigned short rgb_width;	/* Image width			*/
	unsigned short rgb_height;	/* Image height			*/
	unsigned short rgb_zsize;	/* # channels is 3 or 4 in RGB images*/
	long	rgb_min;		/* Minimun pixel value			*/
	long	rgb_max;		/* Maximun pixel value			*/
	long	rgb_wastebytes;	/* Number of bytes wasted by image	*/
	wchar_t	rgb_name[80];	/* Image name				*/
	long	rgb_colormap;	/* Color map type			*/

	wchar_t	rgb_dummy[32];	/* Unused dummy field			*/

	unsigned long	rgb_rleend;	/* File location of end of rle data	*/
	unsigned long	rgb_rowstart;	/* File location of row start table	*/
	unsigned long	rgb_rowsize;	/* File location of row size table	*/
	wchar_t	rgb_pad[360];	/* Unused dummy field 			*/
};

#define IMRGB_ITYPE_VERBATIM	0x0000
#define IMRGB_BYTES_PER_PLANE	0x01

FILE * writeSgiRgbHeader(imRgbHeaderInfo& header, const int Width, const int Height, const wchar_t* outfn)
{
	FILE *of;
	if (fopen_s(&of, GetC(outfn), "w") != 0 || !of) return 0;
	ZeroMemory(&header, sizeof imRgbHeaderInfo);

	header.rgb_imagic		= SWAPBYTESW(0x01da);
	header.rgb_stype		= SWAPBYTESW(IMRGB_ITYPE_VERBATIM | IMRGB_BYTES_PER_PLANE);
	header.rgb_dim			= SWAPBYTESW(2);
	header.rgb_width		= SWAPBYTESW(Width);
	header.rgb_height		= SWAPBYTESW(Height);
	header.rgb_zsize		= SWAPBYTESW(3);
	header.rgb_min			= SWAPBYTESL(0);
	header.rgb_max			= SWAPBYTESL(255);
	header.rgb_wastebytes	= SWAPBYTESL(3);//or 1?
	_tcscpy_s(header.rgb_name, _countof(header.rgb_name), L"dynamic");

	header.rgb_colormap		= SWAPBYTESL(0);
	header.rgb_rleend		= 0;
	header.rgb_rowstart		= 0;
	header.rgb_rowsize		= 0;
	fwrite(&header, sizeof imRgbHeaderInfo,1,of);
//	for(int j=0; j< sizeof imRgbHeaderInfo;j++)
//		fwrite((wchar_t*)&header+j,1,1,of);
	return of;
}

// Macro to determine the number of bytes per line in the DIB bits. This
//  accounts for DWORD alignment by adding 31 bits, then dividing by 32,
//  then rounding up to the next highest count of 4-bytes. Then, we 
//  multiply by 4 to get the total byte count.
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)

extern const wchar_t scratchFileName[];
extern const wchar_t scratchBmpName[];
const wchar_t scratchFileName[] = L"xxx.rgb";
const wchar_t scratchBmpName[] = L"xxx.bmp";
///////////////////////////////////////////////////////////////////////////////
//GLuint texture=0;
#define	imageWidth 64
#define	imageHeight 64
//extern GLubyte image[3*imageWidth*imageHeight];
//bool bTextureDefined = false;
//bool m_bNewBackgroundTextureDefined = false;
///////////////////////////////////////////////////////////////////////////////
#if 0
void CreateContourPlotBitmap(_CallUserDefined& m_uUserDefinedPotential,
							 _CallUserDefined& m_uUserDefinedRed, 
							 _CallUserDefined& m_uUserDefinedGreen,
							 _CallUserDefined& m_uUserDefinedBlue) 
#endif
void CSystem::CreateContourPlotBitmap() 
{
	if (m_uUserDefinedPotential.func == 0) return;
	SetCursor(LoadCursor(NULL, IDC_WAIT));
//	CSystem* m_system = theApp.GetSystem();
try {
//	typedef int duh;
	typedef BYTE duh;
//	typedef long duh;
//	typedef short duh;
//	typedef float duh;
	DWORD res = GetFileAttributes(scratchFileName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}
/*
	if ( m_hBitmapPotential )
		DeleteObject( m_hBitmapPotential );
	if ( m_hPalettePotential )
		DeleteObject( m_hPalettePotential );
	m_hBitmapPotential	= 0;
	m_hPalettePotential	= 0;
*/
///////////////////////////////////////////////////////////////////////////////
	const int N = 64;
	CGLView* gl = theApp.GetGLView();
	if (!gl) return;
	GLubyte *d=gl->image;
	int i=0,j=0,k=0;
	double energy[N][N];
	double X=0, Y=0, Z=0;
	double r=0, g=0, b=0, rgb=0;
	const double &x1 = gl->glFrustum_left;
	const double &x2 = gl->glFrustum_right;
	const double &y1 = gl->glFrustum_bottom;
	const double &y2 = gl->glFrustum_top;

	double Width	= x2 - x1;
	double Height	= y2 - y1;
	double dx		= Width/(N-1);
	double dy		= Height/(N-1);

	vector<double> args;

	for(i=0; i<N; ++i)
	{
		X = x1 + i*dx;
		for(j=0; j<N; ++j)
		{
			args.clear();
			Y = y1 + j*dy;
			if (m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				args.push_back(m__t);
				args.push_back(X);args.push_back(Y);//args.push_back(Z);
				if (m_uUserDefinedPotential.nargs==3) args.push_back(Z);
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				energy[i][j] = CStackMachine::receiveStackTopDoubleValue;
			}
			else
			{
				args.push_back(X);args.push_back(Y);
				if (m_uUserDefinedPotential.nargs==3) args.push_back(Z);
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				energy[i][j] = CStackMachine::receiveStackTopDoubleValue;
			}
		}
	}
	double max = energy[0][0];
	double min = energy[0][0];
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; ++j)
		{
			if (max<energy[i][j])max=energy[i][j];
			if (min>energy[i][j])min=energy[i][j];
		}
	}
	max -= min;
	if (max==0.0)
	{
		TimedMessageBox(0,L"No data.",L"CreateContourPlotBitmap", MB_OK);
		return;
	}

	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; ++j)
		{
			energy[i][j] = (energy[i][j]-min)/max;
		}
	}
///////////////////////////////////////////////////////////////////////////////
{
/*
	imRgbHeaderInfo header;
	FILE *of = writeSgiRgbHeader( header, 4* N,  N, scratchFileName);//=> width in bytes, not pixels
	ASSERT(of);
	if(!of) return;
*/

	for(j=N-1, k=0; j>=0; --j, k=0, d=gl->image)//d=data)
	{
	//	Y = y1 + j*dy;
		for(i=0; i<N; ++i, ++k)
		{
	//		X = x1 + i*dx;
			rgb = 0.5;//pow ( energy[i][j] , energy[i][j] );
			if (m_uUserDefinedRed.func){
				LoadAndCallRealFunc(&m_uUserDefinedRed, energy[i][j]);
				r = CStackMachine::receiveStackTopDoubleValue;
			}
			else r = rgb;//1-energy[i][j]*(1-energy[i][j]/2);
			if (m_uUserDefinedGreen.func){
				LoadAndCallRealFunc(&m_uUserDefinedGreen, energy[i][j]);
				g = CStackMachine::receiveStackTopDoubleValue;
			}
			else g = rgb;//1-energy[i][j]*(1-energy[i][j]/4);
			if (m_uUserDefinedBlue.func){
				LoadAndCallRealFunc(&m_uUserDefinedBlue, energy[i][j]);
				b = CStackMachine::receiveStackTopDoubleValue;
			}
			else b = rgb;//1-energy[i][j]*(1-energy[i][j]/3);


	gl->image[3*(imageWidth*j+i)+0] = GLubyte(255*r);
	gl->image[3*(imageWidth*j+i)+1] = GLubyte(255*g);
	gl->image[3*(imageWidth*j+i)+2] = GLubyte(255*b);
	//		fillSolidRect( i*dx, j*dy, dx, dy, data[0+4*k],data[1+4*k],data[2+4*k] );
// swap bytes:
		//	data[0+4*k]=duh(255*g);
		//	data[1+4*k]=duh(255*r);
		//	data[2+4*k]=0;
		//	data[3+4*k]=duh(255*b);
		//	data[2+4*k]=duh(255*b);
		}
//		fwrite(data,4 * N , sizeof duh, of);
//	//	for( i=0; i< 3 * N;i++) fwrite(data+i,1,sizeof duh,of);
	}
//#undef fillSolidRect
/*
	fclose(of);
	DSStoreDIBSectionInBMPFile( (wchar_t *) scratchBmpName, m_hBitmapPotential );
*/

	gl->m_bNewBackgroundTextureDefined = true;
}
/*
	gl->ReleaseDC( pDC );
	SelectObject( memDC.m_hDC, hOldBitmap );
	DeleteObject( m_hBitmapPotential );
	m_hBitmapPotential = 0;
//	DeleteDC( memDC.m_hDC );
	memDC.DeleteDC();
*/
///////////////////////////////////////////////////////////////////////////////
#if 0
	GLenum e=GL_NO_ERROR;
	CGL::gl->MakeCurrent();

	glEnable(GL_TEXTURE_2D);//GL_INVALID_OPERATION
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// allocate a texture name
	glGenTextures( 1, &texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// select our current texture
	glBindTexture( GL_TEXTURE_2D, texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// when texture area is small, bilinear filter the closest MIP map
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// when texture area is large, bilinear filter the first MIP map
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );//GL_NEAREST
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// if wrap is true, the texture wraps over at the edges (repeat)
//			 ... false, the texture ends at the edges (clamp)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );//GL_CLAMP
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );//GL_CLAMP
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
// build our texture MIP maps//GL_UNSIGNED_BYTE
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, N,	N, GL_RGB, GL_FLOAT, data );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, N, N, 0, GL_RGB, GL_FLOAT, data);//GL_RGB
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
//GL_UNSIGNED_BYTE//GL_BGR_EXT
	glDisable(GL_TEXTURE_2D);
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
	}
#endif
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	gl->Invalidate();
}
catch( SolveIt_Error& e){e.why();}catch(...) {
	DWORD res = GetFileAttributes(scratchFileName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchFileName);
	}
	res = GetFileAttributes(scratchBmpName);
	if (res!=INVALID_FILE_ATTRIBUTES)//FILE_ATTRIBUTE_ARCHIVE
	{
		DeleteFile(scratchBmpName);
	}
/*
	if ( m_hBitmapPotential )
		DeleteObject( m_hBitmapPotential );
	if ( m_hPalettePotential )
		DeleteObject( m_hPalettePotential );
	m_hBitmapPotential	= 0;
	m_hPalettePotential	= 0;
*/
}
}
///////////////////////////////////////////////////////////////////////////////
int CSystem::CreateTexture() 
{
	if (!m_uUserDefinedRed.func) return 0;
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	BOOL bMutexAcquired = FALSE;
try {
	typedef BYTE duh;
///////////////////////////////////////////////////////////////////////////////
	const int N = 64;
//	duh data[3*N*N], *d=data;
//	duh data[4*N], *d=data;
	CGLView* gl = theApp.GetGLView();
	if (!gl) return -1;
	GLubyte *d=gl->image;
	int i=0,j=0,k=0;
	double energy[N][N];
	ZeroMemory(energy, sizeof energy);
	double X=0, Y=0, Z=0;
	double r=0, g=0, b=0;
	{
	const double &x1 = gl->glFrustum_left;
	const double &x2 = gl->glFrustum_right;
	const double &y1 = gl->glFrustum_bottom;
	const double &y2 = gl->glFrustum_top;

	double Width	= x2 - x1;
	double Height	= y2 - y1;
	double dx		= Width/(N-1);
	double dy		= Height/(N-1);

	vector<double> args;

	for(i=0; i<N; ++i)
	{
		X = x1 + i*dx;
		for(j=0; j<N; ++j)
		{
			Y = y1 + j*dy;
			if (m_uUserDefinedPotential.m_bIsTimeDependent)
			{
				args.push_back(m__t);
				args.push_back(X);args.push_back(Y);args.push_back(Z);
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				energy[i][j] = CStackMachine::receiveStackTopDoubleValue;
			}
			else
			{
				args.push_back(X);args.push_back(Y);args.push_back(Z);
				LoadAndCallRealFunc(&m_uUserDefinedPotential, args);
				energy[i][j] = CStackMachine::receiveStackTopDoubleValue;
			}
		}
	}
	}
	double max = energy[0][0];
	double min = energy[0][0];
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; ++j)
		{
			if (max<energy[i][j])max=energy[i][j];
			if (min>energy[i][j])min=energy[i][j];
		}
	}
	max -= min;
	if (max==0.0)
	{
		TimedMessageBox(0,L"No data.",L"CreateContourPlotBitmap", MB_OK);
		return -1;
	}

	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; ++j)
		{
			energy[i][j] = (energy[i][j]-min)/max;
		}
	}
///////////////////////////////////////////////////////////////////////////////
#define SCALE(x)		2.0*((((double)x)/64)-0.5)

{
	gl->ClearBackground();
	double a=0;
	for(j=N-1, k=0; j>=0; --j, k=0, d=gl->image)//d=data)
	{
		double Y1= SCALE(j);
	//	Y = y1 + j*dy;
		for(i=0; i<N; ++i, ++k)
		{
			double X1= SCALE(i);
			double V = fabs(energy[i][j]);
	//		X = x1 + i*dx;

			if (m_uUserDefinedRed.func){
				LoadAndCallRealFunc(&m_uUserDefinedRed, energy[i][j]);
				r = CStackMachine::receiveStackTopDoubleValue;
			}
		//	else r = fabs(X1*Y1);
		//	else r = sin(PI_DIVIDED_BY_2*sqrt(X1*X1+Y1*Y1));
		//	else a = sin(PI_DIVIDED_BY_2*sqrt(X1*X1+Y1*Y1)), r = sqrt(a);
			else a = sin(PI_DIVIDED_BY_2*sqrt(X1*X1+Y1*Y1)), r = a*a;
		//	else r = cos(PI_DIVIDED_BY_2*sqrt(X1*X1+Y1*Y1));

			if (m_uUserDefinedGreen.func){
				LoadAndCallRealFunc(&m_uUserDefinedGreen, energy[i][j]);
				g = CStackMachine::receiveStackTopDoubleValue;
			}
			else a = sin(PI_DIVIDED_BY_2*V), g = a*a;
			if (m_uUserDefinedBlue.func){
				LoadAndCallRealFunc(&m_uUserDefinedBlue, energy[i][j]);
				b = CStackMachine::receiveStackTopDoubleValue;
			}
			else a = sin(PI_DIVIDED_BY_2*(1.0 - V)), b = a;//sqrt(a);

			gl->image[3*(imageWidth*j+i)+0] = GLubyte(255*r);
			gl->image[3*(imageWidth*j+i)+1] = GLubyte(255*g);
			gl->image[3*(imageWidth*j+i)+2] = GLubyte(255*b);
		}
	}
	gl->m_bNewBackgroundTextureDefined = true;
}
///////////////////////////////////////////////////////////////////////////////
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	gl->Invalidate();
}
catch( SolveIt_Error& e){e.why();}catch(...) {
}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//GLuint listBackground=0;
int RGBImageRender()
{
	CGLView* gl = theApp.GetGLView();
	if (!gl) return 1;
	if (!gl->texture) return 1;
	const double &x1 = gl->glFrustum_left;
	const double &x2 = gl->glFrustum_right;
	const double &y1 = gl->glFrustum_bottom;
	const double &y2 = gl->glFrustum_top;

	if(gl->listBackground == 0) gl->listBackground = glGenLists(1);
	glNewList(gl->listBackground, GL_COMPILE);
	glPushMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	GLenum e=GL_NO_ERROR;
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, gl->texture );
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
		return -2;
	}

	const float f=1.0f;
	glColor3f(f,f,f);
	glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);

		glNormal3d(0, 0, -1);
		glTexCoord2d(0,1);glVertex3d(x1,y2, 0);
		glTexCoord2d(0,0);glVertex3d(x1,y1, 0);
		glTexCoord2d(1,0);glVertex3d(x2,y1, 0);
		glTexCoord2d(1,1);glVertex3d(x2,y2, 0);
	glEnd();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if ( GL_NO_ERROR !=  (e=glGetError()) )
	{
		ASSERT(0);
		return -3;
	}
	glEndList();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////

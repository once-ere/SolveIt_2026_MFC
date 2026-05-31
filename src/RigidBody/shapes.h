//shapes.h

#if !defined(________shapes_h__)
#define ________shapes_h__
/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "aux.h"
#include "3d.h"
 */
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
** ToolKit Window Types
** In the future, AUX_RGBA may be a combination of both RGB and ALPHA
*/
namespace shapes 
{
#define AUX_RGB		0
#define AUX_RGBA	AUX_RGB
#define AUX_INDEX	1
#define AUX_SINGLE	0
#define AUX_DOUBLE	2
#define AUX_DIRECT	0
#define AUX_INDIRECT	4

#define AUX_ACCUM	8
#define AUX_ALPHA	16

#undef AUX_DEPTH
#define AUX_DEPTH	32
#define AUX_STENCIL	64
#define AUX_AUX		128

/* 
** Window Masks
*/

#define AUX_WIND_IS_RGB(x)	(((x) & AUX_INDEX) == 0)
#define AUX_WIND_IS_INDEX(x)	(((x) & AUX_INDEX) != 0)
#define AUX_WIND_IS_SINGLE(x)	(((x) & AUX_DOUBLE) == 0)
#define AUX_WIND_IS_DOUBLE(x)	(((x) & AUX_DOUBLE) != 0)
#define AUX_WIND_IS_INDIRECT(x)	(((x) & AUX_INDIRECT) != 0)
#define AUX_WIND_IS_DIRECT(x)	(((x) & AUX_INDIRECT) == 0)
#define AUX_WIND_HAS_ACCUM(x)	(((x) & AUX_ACCUM) != 0)
#define AUX_WIND_HAS_ALPHA(x)	(((x) & AUX_ALPHA) != 0)

#undef AUX_WIND_HAS_DEPTH
#define AUX_WIND_HAS_DEPTH(x)	(((x) & AUX_DEPTH) != 0)
#define AUX_WIND_HAS_STENCIL(x)	(((x) & AUX_STENCIL) != 0)

/*
** ToolKit Event Structure
*/

 struct AUX_EVENTREC {
	GLint event;
	GLint data[4];
} ;

/* 
** ToolKit Event Types
*/
#define AUX_EXPOSE	1
#define AUX_CONFIG	2
#define AUX_DRAW	4
#define AUX_KEYEVENT	8
#define AUX_MOUSEDOWN	16
#define AUX_MOUSEUP	32	
#define AUX_MOUSELOC	64

/*
** Toolkit Event Data Indices
*/
#define AUX_WINDOWX		0
#define AUX_WINDOWY		1
#define AUX_MOUSEX		0
#define AUX_MOUSEY		1
#define AUX_MOUSESTATUS		3
#define AUX_KEY			0
#define AUX_KEYSTATUS		1

/*
** ToolKit Event Status Messages
*/
#define	AUX_LEFTBUTTON		1
#define	AUX_RIGHTBUTTON		2
#define	AUX_MIDDLEBUTTON	4
#define	AUX_SHIFT		1
#define	AUX_CONTROL		2

/* 
** ToolKit Key Codes
*/
#define AUX_RETURN		0x0D
#define AUX_ESCAPE		0x1B
#define AUX_SPACE		0x20
#define AUX_LEFT		0x25
#define AUX_UP			0x26
#define AUX_RIGHT		0x27
#define AUX_DOWN		0x28
#define AUX_A			'A'
#define AUX_B			'B'
#define AUX_C			'C'
#define AUX_D			'D'
#define AUX_E			'E'
#define AUX_F			'F'
#define AUX_G			'G'
#define AUX_H			'H'
#define AUX_I			'I'
#define AUX_J			'J'
#define AUX_K			'K'
#define AUX_L			'L'
#define AUX_M			'M'
#define AUX_N			'N'
#define AUX_O			'O'
#define AUX_P			'P'
#define AUX_Q			'Q'
#define AUX_R			'R'
#define AUX_S			'S'
#define AUX_T			'T'
#define AUX_U			'U'
#define AUX_V			'V'
#define AUX_W			'W'
#define AUX_X			'X'
#define AUX_Y			'Y'
#define AUX_Z			'Z'
#define AUX_a			'a'
#define AUX_b			'b'
#define AUX_c			'c'
#define AUX_d			'd'
#define AUX_e			'e'
#define AUX_f			'f'
#define AUX_g			'g'
#define AUX_h			'h'
#define AUX_i			'i'
#define AUX_j			'j'
#define AUX_k			'k'
#define AUX_l			'l'
#define AUX_m			'm'
#define AUX_n			'n'
#define AUX_o			'o'
#define AUX_p			'p'
#define AUX_q			'q'
#define AUX_r			'r'
#define AUX_s			's'
#define AUX_t			't'
#define AUX_u			'u'
#define AUX_v			'v'
#define AUX_w			'w'
#define AUX_x			'x'
#define AUX_y			'y'
#define AUX_z			'z'
#define AUX_0			'0'
#define AUX_1			'1'
#define AUX_2			'2'
#define AUX_3			'3'
#define AUX_4			'4'
#define AUX_5			'5'
#define AUX_6			'6'
#define AUX_7			'7'
#define AUX_8			'8'
#define AUX_9			'9'

/*
** ToolKit Gets and Sets
*/
#define AUX_FD			1  /* return fd (long) */
#define AUX_COLORMAP		3  /* pass buf of r, g and b (unsigned char) */
#define AUX_GREYSCALEMAP	4
#define AUX_FOGMAP		5  /* pass fog and color bits (long) */
#define AUX_ONECOLOR		6  /* pass index, r, g, and b (long) */

/*
** Color Macros

enum {
	AUX_BLACK = 0,
	AUX_RED,
	AUX_GREEN,
	AUX_YELLOW,
	AUX_BLUE,
	AUX_MAGENTA,
	AUX_CYAN,
	AUX_WHITE
};
*/

extern float auxRGBMap[8][3];

#define AUX_SETCOLOR(x, y) (AUX_WIND_IS_RGB((x)) ? \
				   glColor3fv(auxRGBMap[(y)]) : glIndexf((y)))

/*
** RGB Image Structure
*/

 struct AUX_RGBImageRec {
	GLint sizeX, sizeY;
	unsigned char *data;
} ;

/*
** Prototypes
*/

void auxInitDisplayMode(GLenum);
void auxInitPosition(int, int, int, int);
//extern GLenum auxInitWindow(char *);
void auxCloseWindow(void);
void auxQuit(void);
void auxSwapBuffers(void);

////extern Display *auxXDisplay(GLvoid);
////extern Window auxXWindow(GLvoid);

void auxMainLoop(void (*)());
void auxExposeFunc(void (*)(int, int));
void auxReshapeFunc(void (*)(GLsizei, GLsizei));
void auxIdleFunc(void (*)());
void auxKeyFunc(int, void (*)());
void auxMouseFunc(int, int, void (*)(AUX_EVENTREC *));

//extern int auxGetColorMapSize(void);
void auxGetMouseLoc(int *, int *);
void auxSetOneColor(int, float, float, float);
void auxSetFogRamp(int, int);
void auxSetGreyRamp(void);
void auxSetRGBMap(int, float *);

//extern AUX_RGBImageRec *auxRGBImageLoad(char *);

void auxCreateFont(void);
void auxDrawStr(char *);

void auxWireSphere(GLdouble);
void auxSolidSphere(GLdouble);
void auxWireCube(GLdouble);
void auxSolidCube(GLdouble);
void auxWireBox(GLdouble, GLdouble, GLdouble);
void auxSolidBox(GLdouble, GLdouble, GLdouble);
void auxWireTorus(GLdouble, GLdouble);
void auxSolidTorus(GLdouble, GLdouble);
void auxWireCylinder(GLdouble, GLdouble);
void auxSolidCylinder(GLdouble, GLdouble);
void auxWireIcosahedron(GLdouble);
void auxSolidIcosahedron(GLdouble);
void auxWireOctahedron(GLdouble);
void auxSolidOctahedron(GLdouble);
void auxWireTetrahedron(GLdouble);
void auxSolidTetrahedron(GLdouble);
void auxWireDodecahedron(GLdouble);
void auxSolidDodecahedron(GLdouble);
void auxWireCone(GLdouble, GLdouble);
void auxSolidCone(GLdouble, GLdouble);
void auxWireTeapot(GLdouble);
void auxSolidTeapot(GLdouble);
///////////////////////////////////////////////////////////////////////////////
	void	error(const char *);
void	diff3(GLdouble [3], GLdouble [3], GLdouble [3]);
void	add3(GLdouble [3], GLdouble [3], GLdouble [3]);
void	scalarmult(GLdouble, GLdouble [3], GLdouble [3]);
GLdouble	dot3(GLdouble [3], GLdouble [3]);
GLdouble	length3(GLdouble [3]);
GLdouble	dist3(GLdouble [3], GLdouble [3]);
void	copy3(GLdouble [3], GLdouble [3]);
void	crossprod(GLdouble [3], GLdouble [3], GLdouble [3]);
void	normalize(GLdouble [3]);
void	print3(GLdouble [3]);
void	printmat3(GLdouble [3][3]);
void	identifymat3(GLdouble [3][3]);
void	copymat3(GLdouble *, GLdouble *);
void	xformvec3(GLdouble [3], GLdouble [3][3], GLdouble [3]);

void m_resetmatrixstack();
void m_xformpt(GLdouble [3], GLdouble [3], GLdouble [3], GLdouble [3]);
void m_xformptonly(GLdouble [3], GLdouble [3]);
void m_pushmatrix();
void m_popmatrix();
void m_shear(GLdouble, GLdouble, GLdouble);
void m_translate(GLdouble, GLdouble, GLdouble);
void m_scale(GLdouble, GLdouble, GLdouble);
void m_rotate(GLdouble, char);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define SPHEREWIRE	0
#define CUBEWIRE	1
#define BOXWIRE		2
#define TORUSWIRE	3
#define CYLINDERWIRE	4
#define ICOSAWIRE	5
#define OCTAWIRE	6
#define TETRAWIRE	7
#define DODECAWIRE	8
#define CONEWIRE	9
#define SPHERESOLID	10
#define CUBESOLID	11
#define BOXSOLID	12
#define TORUSSOLID	13
#define CYLINDERSOLID	14
#define ICOSASOLID	15
#define OCTASOLID	16
#define TETRASOLID	17
#define DODECASOLID	18
#define CONESOLID	19

#define PI 3.1415926535897

/*	structure for each geometric object	*/
 struct MODEL {
	GLuint list;	/*  display list to render object   */
	MODEL *ptr;	/*  pointer to next object	*/
	int numParam;	/*  # of parameters		*/
	GLdouble *params;	/*  array with parameters	*/
} ;
typedef MODEL *MODELPTR;

/*	array of linked lists--used to keep track of display lists 
 *	for each different type of geometric object.
 */
static MODELPTR lists[25] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};

GLuint findList (int index, GLdouble *paramArray, int size);
int compareParams (GLdouble *oneArray, GLdouble *twoArray, int size);
GLuint makeModelPtr (int index, GLdouble *sizeArray, int count);

void drawbox(GLdouble, GLdouble, GLdouble, 
	GLdouble, GLdouble, GLdouble, GLenum);
void doughnut(GLdouble, GLdouble, GLint, GLint, GLenum);
void icosahedron(GLdouble *, GLdouble, GLenum);
void octahedron(GLdouble *, GLdouble, GLenum);
void tetrahedron(GLdouble *, GLdouble, GLenum);
void subdivide(int, GLdouble *, GLdouble *, GLdouble *,
	GLdouble *, GLdouble, GLenum, int);
void drawtriangle(int, int, int,
	GLdouble *, GLdouble, GLenum, int);
void recorditem(GLdouble *, GLdouble *, GLdouble *,
	GLdouble *, GLdouble, GLenum, int);
void initdodec(void);
void dodecahedron(GLdouble *, GLdouble, GLenum);
void pentagon(int, int, int, int, int, GLenum);

void  auxDrawStrA(LPCSTR);
void  auxDrawStrW(LPCWSTR);

void  auxWireSphere(GLdouble);
void  auxSolidSphere(GLdouble);
void  auxWireCube(GLdouble);
void  auxSolidCube(GLdouble);
void  auxWireBox(GLdouble, GLdouble, GLdouble);
void  auxSolidBox(GLdouble, GLdouble, GLdouble);
void  auxWireTorus(GLdouble, GLdouble);
void  auxSolidTorus(GLdouble, GLdouble);
void  auxWireCylinder(GLdouble, GLdouble);
void  auxSolidCylinder(GLdouble, GLdouble);
void  auxWireIcosahedron(GLdouble);
void  auxSolidIcosahedron(GLdouble);
void  auxWireOctahedron(GLdouble);
void  auxSolidOctahedron(GLdouble);
void  auxWireTetrahedron(GLdouble);
void  auxSolidTetrahedron(GLdouble);
void  auxWireDodecahedron(GLdouble);
void  auxSolidDodecahedron(GLdouble);
void  auxWireCone(GLdouble, GLdouble);
void  auxSolidCone(GLdouble, GLdouble);
void  auxWireTeapot(GLdouble);
void  auxSolidTeapot(GLdouble);


GLenum  auxInitWindowAW(LPCSTR title, BOOL bUnicode);
void auxInitDisplayModePolicy(GLenum type);
GLenum auxInitDisplayModeID(GLint id);
GLenum auxInitWindowA(LPCSTR title);
GLenum  auxInitWindowW(LPCWSTR title);
GLenum  auxInitWindowAW(LPCSTR title, BOOL bUnicode);
GLenum auxGetDisplayModePolicy(void);
GLint auxGetDisplayModeID(void);
GLenum auxGetDisplayMode(void);
void auxSetOneColor(int index, float r, float g, float b);
HWND auxGetHWND(void);
HDC auxGetHDC(void);
HGLRC auxGetHGLRC(void);
GLenum auxGetDisplayModePolicy(void);
GLint auxGetDisplayModeID(void);
int auxGetColorMapSize(void);
void auxGetMouseLoc(int *x, int *y);

void  auxDrawStrAW(LPCSTR str, BOOL bUnicode);

void seterrorfunc(void (*func)(const char *));
void perpnorm(GLdouble p1[3], GLdouble p2[3], GLdouble p3[3], GLdouble n[3]);

void PrintMessage( const char *Format, ... );
void DelayPaletteRealization( void );

BOOL GrabStaticEntries(HDC hdc);
BOOL FindPixelFormat(HDC hdc, GLenum type);
int FindBestPixelFormat(HDC hdc, GLenum type, PIXELFORMATDESCRIPTOR *ppfd);
long RealizePaletteNow( HDC Dc, HPALETTE Palette, BOOL bForceBackground );
void ForceRedraw( HWND Window );
long tkWndProc(HWND hWnd, UINT message, DWORD wParam, LONG lParam);

HPALETTE CreateCIPalette( HDC Dc );
HPALETTE CreateRGBPalette( HDC Dc );
void CleanUp( void );
void DestroyThisWindow( HWND Window );
int PixelFormatDescriptorFromDc( HDC Dc, PIXELFORMATDESCRIPTOR *Pfd );

AUX_RGBImageRec *  auxRGBImageLoadA(LPCSTR filename);
AUX_RGBImageRec *  auxRGBImageLoadW(LPCWSTR filename);
AUX_RGBImageRec *  auxDIBImageLoadA(LPCSTR filename);
AUX_RGBImageRec *  auxDIBImageLoadW(LPCWSTR filename);

void solidTeapot(long grid, GLdouble scale);

};



#endif

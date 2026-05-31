//	(c) 2000 J. Gans (jdg9@cornell.edu), Shalloway Lab, 
//	Cornell University. You can modify and freely distribute 
//	this code, but please keep this header intact.
//
//	Look-and-feel inspired by Xmol 
//	(http://www.networkcs.com/msc/docs/xmol/).
//
//	OpenGL selection code and quaterion rotation matrix
//	formulation from molview demo program by Mark Kilgard
//	(http://reality.sgi.com/opengl/OpenGLforX.html).
//
//	The printing routines are based on code from
//	Craig Fahrnbach <craignan@home.com> via
//	Uwe Kotyczka <kotyczka@bnm-gmbh.de>.
//
//	Covalent bond determination code based on VMD
//	http://www.ks.uiuc.edu/Research/vmd/
//
//	Stide (http://www.embl-heidelberg.de/stride/stride.html)
//	is used for secondary structure determination and has been 
//	included in Qmol with the kind permission of 
//
//	Dmitrij Frishman, PhD
//	Institute for Bioinformatics
//	GSF - Forschungszentrum f�r Umwelt und Gesundheit, GmbH
//	Ingolst�dter Landstra�e 1,
//	D-85764 Neuherberg, Germany
// 
//	Telephone: +49-89-3187-4201
//	Fax: +49-89-31873585
//	e-mail: d.frishman@gsf.de
//	WWW: http://mips.gsf.de/mips/staff/frishman/
// 
// A win32/MSDEV thing ...
#include "stdafx.h"

#include "trackball2.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


VirtualTrackBall::VirtualTrackBall()
{
	curquat[0] = lastquat[0] = 0.0;
	curquat[1] = lastquat[1] = 0.0;
	curquat[2] = lastquat[2] = 0.0;
	curquat[3] = lastquat[3] = 1.0;

	// The default value
	trackballsize = DEFAULT_TRACKBALLSIZE;
}


void VirtualTrackBall::trackball(double p1x, double p1y, 
								 double p2x, double p2y)
{
	// Axis of rotation.
	Vector3D a; 

	// How much to rotate about axis.
	double phi;            
	static Vector3D p1, p2, d;
	double t;

	if (p1x == p2x && p1y == p2y) {
		// Zero rotation
	lastquat[0] = 0.0;
	lastquat[1] = 0.0;
	lastquat[2] = 0.0;
	lastquat[3] = 1.0;
		return;
	}

	// First, figure out z-coordinates for projection of P1 and
	// P2 to deformed sphere.
	p1[0] = p1x;
	p1[1] = p1y;
	p1[2] = tb_project_to_sphere(trackballsize, p1x, p1y);
	
	p2[0] = p2x;
	p2[1] = p2y;
	p2[2] = tb_project_to_sphere(trackballsize, p2x, p2y);

	// Now, we want the cross product of P1 and P2.
	a = p2.vectorProd(p1); // a = p2 X p1

	// Figure out how much to rotate around that axis. 
	d = p1 - p2;

	t = (double)(sqrt(d*d)/(2.0 * trackballsize));

	// Avoid problems with out-of-control values.
	if (t > 1.0)
		t = 1.0;
	if (t < -1.0)
		t = -1.0;
	phi = (double)(2.0 * asin(t));

	axis_to_quat(a, phi);
}

// Given an axis and angle, compute quaternion.
void VirtualTrackBall::axis_to_quat(Vector3D &a, double phi)
{
	// Normalize a
	a.normalize();
double x=sin(phi/2.0);
	lastquat[0] = x*a[0];
	lastquat[1] = x*a[1];
	lastquat[2] = x*a[2];
	
	lastquat[3] = (double)(cos(phi/2.0));
}

// Project an x,y pair onto a sphere of radius r OR a
// hyperbolic sheet if we are away from the center of the
// sphere.
double VirtualTrackBall::tb_project_to_sphere(double r, double x, double y)
{
	double d, t, z;

	d = (double)(sqrt(x * x + y * y));
	if (d < r * 0.70710678118654752440f) {
		// Inside sphere
		z = (double)(sqrt(r * r - d * d));
	} 
	else {
		// On hyperbola
		t = r / 1.41421356237309504880f;
		z = t * t / d;
	}
	
	return z;
}

void VirtualTrackBall::update()
{
	static int count = 0;
	static double t1[4]={0,0,0,1};
	static double t2[4]={0,0,0,1};
	static double t3[4]={0,0,0,1};
	static double tf[4]={0,0,0,1};
	double three_d_dot;

	t1[0] = lastquat[0]*curquat[3];
	t1[1] = lastquat[1]*curquat[3];
	t1[2] = lastquat[2]*curquat[3];
	t1[3] = lastquat[3]*curquat[3];
	t2[0] = curquat[0]*lastquat[3];
	t2[1] = curquat[1]*lastquat[3];
	t2[2] = curquat[2]*lastquat[3];
	t2[3] = curquat[3]*lastquat[3];
	
	// Cross Product
	t3[0] = curquat[1]*lastquat[2] - curquat[2]*lastquat[1];
	t3[1] = curquat[2]*lastquat[0] - curquat[0]*lastquat[2];
	t3[2] = curquat[0]*lastquat[1] - curquat[1]*lastquat[0];
	
	for (int j=0;j<4;j++) tf[j] = t1[j] + t2[j] + t3[j];
	
	// Take the dot product in 3D
	three_d_dot = lastquat[0]*curquat[0] + lastquat[1]*curquat[1] +
				  lastquat[2]*curquat[2];

	tf[3] = lastquat[3] * curquat[3] - three_d_dot;

	CopyMemory(curquat, tf, sizeof tf);

double x=1.0/sqrt(curquat[0]*curquat[0] + curquat[1]*curquat[1] +
				  curquat[2]*curquat[2] + curquat[3]*curquat[3]);
	if (++count > RENORMCOUNT) {
		count = 0;
//		curquat = curquat/(curquat*curquat);
		for (int j=0;j<4;j++) curquat[j]*=x;
	}
}


/* Build a rotation matrix, given a quaternion rotation. GLfloat*/
void VirtualTrackBall::build_rotmatrix(double m[4][4])
{
	m[0][0] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[2] * curquat[2]);
	m[0][1] = 2.0 * (curquat[0] * curquat[1] - curquat[2] * curquat[3]);
	m[0][2] = 2.0 * (curquat[2] * curquat[0] + curquat[1] * curquat[3]);
	m[0][3] = 0.0;

	m[1][0] = 2.0 * (curquat[0] * curquat[1] + curquat[2] * curquat[3]);
	m[1][1] = 1.0 - 2.0 * (curquat[2] * curquat[2] + 
							 curquat[0] * curquat[0]);
	m[1][2] = 2.0 * (curquat[1] * curquat[2] - curquat[0] * curquat[3]);
	m[1][3] = 0.0;

	m[2][0] = 2.0 * (curquat[2] * curquat[0] - curquat[1] * curquat[3]);
	m[2][1] = 2.0 * (curquat[1] * curquat[2] + curquat[0] * curquat[3]);
	m[2][2] = 1.0 - 2.0 * (curquat[1] * curquat[1] + 
							 curquat[0] * curquat[0]);
	m[2][3] = 0.0;

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
///////////////////////////////////////////////////////////////////////////////
/*
void CALLBACK
AnimateTimer(HWND hwnd, UINT uiMsg, UINT idEvent, DWORD dwTime)
{
	add_quats(lastquat, curquat, curquat);
	Fire_ViewChange();
}
*/
///////////////////////////////////////////////////////////////////////////////

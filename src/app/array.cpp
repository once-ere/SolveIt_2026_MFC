// vector_of_double.cpp: implementation of the vector_of_double class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "array.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
vector_of_double::~vector_of_double()
{
}
///////////////////////////////////////////////////////////////////////////////
void vector_of_double::OnDraw()
{
	if (empty()) return;
	OnDraw(vecPlotTrajectoryColor);
}
///////////////////////////////////////////////////////////////////////////////
void vector_of_double::OnDraw(const double& Red, const double& Green, const double& Blue)
{
	if (empty()) return;
	Vector3D vecColor(Red,Green,Blue);
	OnDraw(vecColor);
}
///////////////////////////////////////////////////////////////////////////////
void vector_of_double::OnDraw(const Vector3D& vecColor)
{
	if (empty()) return;
	glPushMatrix();
	glColor3dv(vecColor.v);
//	Lock lock (protectFieldLines);
	glBegin (GL_LINE_STRIP);
	for (long j=0; j< size()/n_group_by; ++j)
	{
		switch(n_group_by)
		{
			case 2:
				glVertex2d(operator[](3*j),operator[](3*j+1));
				break;
			case 3:
				glVertex3d(operator[](3*j),operator[](3*j+1),operator[](3*j+2));
				break;
		}
	}
	glEnd ();
	glPopMatrix();
}
///////////////////////////////////////////////////////////////////////////////
void vector_of_Vector3D::OnDraw(void) 
{
	if (empty()) return;
	vector_of_Vector3D::const_iterator it;
	glColor3dv(vecTrajectoryColor.v);
	glBegin (GL_LINE_STRIP);
	for (it = begin(); it != end(); ++it)
	{
		(*it).OnDraw();
	}
	glEnd ();
}
///////////////////////////////////////////////////////////////////////////////
void vector_of_Vector2D::OnDraw(void) 
{
	if (empty()) return;
	vector_of_Vector2D::const_iterator it = begin();
	glColor3dv(vecTrajectoryColor.v);
	glBegin (GL_LINE_STRIP);
//	glBegin (GL_LINES);
//	const long num_points_to_compute = 64;
//	long num_points = num_points_to_compute;
//	if (num_points-- > 2)
	 
	for (it = begin(); it != end(); ++it)
	{
		(*it).OnDraw();
	}
	glEnd ();
}

// RigidLink.cpp : Implementation of CRigidBody

#include "stdafx.h"
#include "MatX.h"
#include "Se3.h"
#include "SixDim.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
*************************WARNING*************************
*************************WARNING*************************
	A^B+C == A^(B+C)
*************************WARNING*************************
*************************WARNING*************************
*/
const PAIR_Vector3D PAIR_Vector3D::ZERO(ZERO_3D,ZERO_3D);
///////////////////////////////////////////////////////////////////////////////
//const Mat6 Mat6::G(Mat3::ID, Mat3::ID);
const Mat6 Mat6::ZERO;
const Mat6 Mat6::ID(Mat3::ID, Mat3::ZERO, Mat3::ZERO, Mat3::ID);
const Mat6 Mat6::G(Mat3::ZERO, Mat3::ID, Mat3::ID, Mat3::ZERO);
///////////////////////////////////////////////////////////////////////////////

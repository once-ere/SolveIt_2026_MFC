//State.cpp
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "State.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const State SolveIt::Unit_State(0, Point3D(0,0,0), Quat::ID, ZERO_3D,ZERO_3D);
const State SolveIt::Zero_State(0, Point3D(0,0,0), Quat::ZERO, ZERO_3D,ZERO_3D);


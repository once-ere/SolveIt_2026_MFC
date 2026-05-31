// StackDatum.cpp: implementation of the objects class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolveIt.h"
#include "objects.h"
#include "parser.h" /* for T_INFINITY, T_MINUS_INFINITY token definitions */
//#include "parser_2020.h" /* for token definitions and yylval */
#include "RigidBody/RigidBodyManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double power(double x, double y)
{
//	static emap _emap;
	if ( y == 0.0) return 1.0;
	if ( x == 1.0) return 1.0;
	try {
	return pow(x,y);
//	return errcheck(pow(x,y), "exponentiation");struct _exception *except
	}
	catch( SolveIt_Error& e){e.why();}catch(...)
	{
		return 0;
	}
}
///////////////////////////////////////////////////////////////////////////////
	objects duh(const long& a, const objects& b)
	{
		if (b.type==__long)			return objects(a*b.lVal);
		if (b.type==__int)			return objects(a*b.iVal);
		if (b.type == __double)		return objects(a*b.dVal);
		if (b.type == __vector3)	return objects(double(a)**b.vPtr);
		if (b.type == __point3)		return objects(double(a)**b.ptPtr);
		throw new std::runtime_error("objects.*");
	}
///////////////////////////////////////////////////////////////////////////////
	objects duuh(const double& a, const objects& b)
	{
		if (b.type==__long)			return objects(a*b.lVal);
		if (b.type==__int)			return objects(a*b.iVal);
		if (b.type == __double)		return objects(a*b.dVal);
		if (b.type == __vector3)	return objects(a**b.vPtr);
		if (b.type == __point3)		return objects(a**b.ptPtr);
		throw new std::runtime_error("objects.*");
	}

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{

///////////////////////////////////////////////////////////////////////////////
	objects operator*(const double& a, const objects& b)
	{
		return duuh(a, b);
	}
	objects operator*(const objects& b, const double& a)
	{
		return duuh(a, b);
	}

	objects operator*(const long& a, const objects& b)
	{
		return duh(a, b);
	}
	objects operator*(const objects& b, const long& a)
	{
		return duh(a, b);
	}


///////////////////////////////////////////////////////////////////////////////
objects objects::operator-() const //{return objects(-1)*(*this);}
{
	if (type == __long)		return objects(-lVal);
	if (type == __int)
	{
		if (iVal == T_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
		if (iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		return objects(-iVal);
	}
	if (type == __double)	return objects(-dVal);
	if (type == __vector3)	return objects(-*vPtr);
	if (type == __point3)	return objects(-*ptPtr);
	throw new std::runtime_error("objects.*");
}
///////////////////////////////////////////////////////////////////////////////
double objects::norm() const
{
	if (type == __long)			return fabs(lVal);
	if (type == __int)			return fabs(iVal);
	if (type == __double)		return fabs(dVal);
	if (type == __vector3)		return vPtr?vPtr->norm():0;
	if (type == __point3)		return ptPtr?ptPtr->norm():0;
	if (type == __vector4)		return v4Ptr->v?sqrt(v4Ptr->v[0]*v4Ptr->v[0]+v4Ptr->v[1]*v4Ptr->v[1]+v4Ptr->v[2]*v4Ptr->v[2]+v4Ptr->v[3]*v4Ptr->v[3]):0;
	if (type == __vector5)		return v5Ptr->v?sqrt(v5Ptr->v[0]*v5Ptr->v[0]+v5Ptr->v[1]*v5Ptr->v[1]+v5Ptr->v[2]*v5Ptr->v[2]+v5Ptr->v[3]*v5Ptr->v[3]+v5Ptr->v[4]*v5Ptr->v[4]):0;
	if (type == __vector6)		return v6Ptr->v?sqrt(v6Ptr->v[0]*v6Ptr->v[0]+v6Ptr->v[1]*v6Ptr->v[1]+v6Ptr->v[2]*v6Ptr->v[2]+v6Ptr->v[3]*v6Ptr->v[3]+v6Ptr->v[4]*v6Ptr->v[4]+v6Ptr->v[5]*v6Ptr->v[5]):0;

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
// arithmetic operations
//
objects operator+(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects.+: __undefined ");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(a.lVal + b.lVal);
				case __int:		return objects(a.lVal + b.iVal);
				case __double:	return objects(a.lVal + b.dVal);
				case __point3:	return objects(double(a.lVal) + *b.ptPtr);
				case __vector3:	return objects(double(a.lVal) + *b.vPtr);
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(a.iVal + b.lVal);
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
	return objects(a.iVal + b.iVal);
				case __double:	return objects(a.iVal + b.dVal);
				case __point3:	return objects(double(a.iVal) + *b.ptPtr);
				case __vector3:	return objects(double(a.iVal) + *b.vPtr);
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(a.dVal + b.lVal);
				case __int:		return objects(a.dVal + b.iVal);
				case __double:	return objects(a.dVal + b.dVal);
				case __point3:	return objects(a.dVal + *b.ptPtr);
				case __vector3:	return objects(a.dVal + *b.vPtr);
			}
		case __vector3:
			switch (b.type)
			{
				case __long:		return objects(*a.vPtr + double(b.lVal));
				case __int:		return objects(*a.vPtr + double(b.iVal));
				case __double:	return objects(*a.vPtr + b.dVal);
				case __vector3:
					{
						return objects(*a.vPtr + *b.vPtr);
					}
				case __point3:
					{
						return objects(*a.ptPtr + *b.ptPtr);
					}
			}
		case __point3:
			switch (b.type)
			{
				case __long:	return objects(*a.ptPtr + double(b.lVal));
				case __int:		return objects(*a.ptPtr + double(b.iVal));
				case __double:	return objects(*a.ptPtr + b.dVal);
				case __vector3:
					{
						return objects(*a.ptPtr + *b.vPtr);
					}
				case __point3:
					{
						return objects(*a.ptPtr + *b.ptPtr);
					}
			}
		case __RigidBodyPtr:
			switch (b.type)
			{
				case __RigidBodyPtr:
					{
				//		CRigidBody body(wstring("sum"), T_VECTOR_ANALYSIS_OBJECT);
				//		objects c(body);
						CRigidBody* body = new CRigidBody(wstring(L"sum"), T_VECTOR_ANALYSIS_OBJECT);
						objects c(body);
						CRigidBody* A = a.rbPtr;
						CRigidBody* B = b.rbPtr;
						CRigidBody* C = c.rbPtr;
	C->m_bDisableSelect = true;
	C->m_role = CRigidBody::_sum;

	C->m_bodiesDependentOn.push_back(A);
	C->m_bodiesDependentOn.push_back(B);
	A->m_bodiesToAdvise.push_back(C);
	B->m_bodiesToAdvise.push_back(C);
	C->set_Position(A->x + B->x);
				CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
				rigidBodyManager->add(c);
						return c;
					}
				default:
					throw new std::runtime_error("objects.+: not a rigid body");
			}
		default:
			{
			throw new std::runtime_error("objects.+__undefined__undefined");
			}
	}
}
///////////////////////////////////////////////////////////////////////////////
objects operator-(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects.-");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(a.lVal - b.lVal);
				case __int:		return objects(a.lVal - b.iVal);
				case __double:	return objects(a.lVal - b.dVal);
				case __point3:	return objects(double(a.lVal) - *b.ptPtr);
				case __vector3:	return objects(double(a.lVal) - *b.vPtr);
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(a.iVal - b.lVal);
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
		return objects(a.iVal - b.iVal);
				case __double:	return objects(a.iVal - b.dVal);
				case __point3:	return objects(double(a.iVal) - *b.ptPtr);
				case __vector3:	return objects(double(a.iVal) - *b.vPtr);
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(a.dVal - b.lVal);
				case __int:		return objects(a.dVal - b.iVal);
				case __double:	return objects(a.dVal - b.dVal);
				case __point3:
				case __vector3:	return objects(a.dVal - *b.vPtr);
			}
		case __point3:
			switch (b.type)
			{
				case __long:	return objects(*a.ptPtr - double(b.lVal));
				case __int:		return objects(*a.ptPtr - double(b.iVal));
				case __double:	return objects(*a.ptPtr - b.dVal);
				case __point3:	return objects(*a.ptPtr - *b.ptPtr);
				case __vector3:	return objects(*a.ptPtr - *b.vPtr);
			}
		case __vector3:
			switch (b.type)
			{
				case __long:	return objects(*a.vPtr - double(b.lVal));
				case __int:		return objects(*a.vPtr - double(b.iVal));
				case __double:	return objects(*a.vPtr - b.dVal);
				case __point3:	return objects(*a.vPtr - *b.ptPtr);
				case __vector3:	return objects(*a.vPtr - *b.vPtr);
			}

		case __RigidBodyPtr:
			switch (b.type)
			{
				case __RigidBodyPtr:
					{
					//	CRigidBody body(wstring("diff"), T_VECTOR_ANALYSIS_OBJECT);
					//	objects c(body);
						CRigidBody* body = new CRigidBody(wstring(L"diff"), T_VECTOR_ANALYSIS_OBJECT);
						objects c(body);
						CRigidBody* A = a.rbPtr;
						CRigidBody* B = b.rbPtr;
						CRigidBody* C = c.rbPtr;
	C->m_bDisableSelect = true;
	C->m_role = CRigidBody::_difference;

	C->m_bodiesDependentOn.push_back(A);
	C->m_bodiesDependentOn.push_back(B);
	A->m_bodiesToAdvise.push_back(C);
	B->m_bodiesToAdvise.push_back(C);
	C->set_Position(A->x - B->x);
				CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
				rigidBodyManager->add(c);
						return c;
					}
				default:
					throw new std::runtime_error("objects.+: not a rigid body");
			}


		default:
			throw new std::runtime_error("objects.-");
	}
}
///////////////////////////////////////////////////////////////////////////////
objects operator*(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects.*");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(a.lVal * b.lVal);
				case __int:		return objects(a.lVal * b.iVal);
				case __double:	return objects(a.lVal * b.dVal);
				case __point3:	return objects(double(a.lVal) * *b.ptPtr);
				case __vector3:	return objects(double(a.lVal) * *b.vPtr);
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(a.iVal * b.lVal);
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
		return objects(a.iVal * b.iVal);
				case __double:	return objects(a.iVal * b.dVal);
				case __point3:	return objects(double(a.iVal) * *b.ptPtr);
				case __vector3:	return objects(double(a.iVal) * *b.vPtr);
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(a.dVal * b.lVal);
				case __int:		return objects(a.dVal * b.iVal);
				case __double:	return objects(a.dVal * b.dVal);
				case __point3:	return objects(a.dVal * *b.ptPtr);
				case __vector3:	return objects(a.dVal * *b.vPtr);
			}
		case __point3:
			switch (b.type)
			{
				case __long:	return objects(*a.ptPtr * double(b.lVal));
				case __int:		return objects(*a.ptPtr * double(b.iVal));
				case __double:	return objects(*a.ptPtr * b.dVal);
				case __point3:	return objects(*a.ptPtr * *b.ptPtr);//scalar product
				case __vector3:	return objects(*a.ptPtr * *b.vPtr);//scalar product
			}
		case __vector3:
			switch (b.type)
			{
				case __long:	return objects(*a.vPtr * double(b.lVal));
				case __int:		return objects(*a.vPtr * double(b.iVal));
				case __double:	return objects(*a.vPtr * b.dVal);
				case __point3:	return objects(*a.vPtr * *b.ptPtr);//scalar product
				case __vector3:	return objects(*a.vPtr * *b.vPtr);//scalar product
			}

		case __RigidBodyPtr:
			switch (b.type)
			{
				case __RigidBodyPtr:
					{
					//	CRigidBody body(wstring("vprod"), T_VECTOR_ANALYSIS_OBJECT);
					//	objects c(body);
						CRigidBody* body = new CRigidBody(wstring(L"vprod"), T_VECTOR_ANALYSIS_OBJECT);
						objects c(body);
						CRigidBody* A = a.rbPtr;
						CRigidBody* B = b.rbPtr;
						CRigidBody* C = c.rbPtr;
	C->m_bDisableSelect = true;
	C->m_role = CRigidBody::_crossProduct;

	C->m_bodiesDependentOn.push_back(A);
	C->m_bodiesDependentOn.push_back(B);
	A->m_bodiesToAdvise.push_back(C);
	B->m_bodiesToAdvise.push_back(C);
	C->set_Position(A->x. vectorProd ( B->x ));
				CRigidBodyManager*	rigidBodyManager = theApp.GetRigidBodyManager();
				rigidBodyManager->add(c);
						return c;
					}
				default:
					throw new std::runtime_error("objects.+: not a rigid body");
			}


		default:
			throw new std::runtime_error("objects.*");
	}
}
///////////////////////////////////////////////////////////////////////////////
objects operator/(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects./");
	}
	if ((b.type==__long) && b.lVal==0)
	{
		throw SolveIt_ERROR(L"ERROR: division by 0");
	}
	if ((b.type==__int) && b.iVal==0)
	{
		throw SolveIt_ERROR(L"ERROR: division by 0");
	}
	else if (b.type==__double && b.dVal==0.0)
	{
		throw SolveIt_ERROR(L"ERROR: division by 0.0");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(a.lVal / b.lVal);
				case __int:		return objects(a.lVal / b.iVal);
				case __double:	return objects(a.lVal / b.dVal);
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: division by vector");
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(a.iVal / b.lVal);
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
		return objects(a.iVal / b.iVal);
				case __double:	return objects(a.iVal / b.dVal);
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: division by vector");
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(a.dVal / b.lVal);
				case __int:		return objects(a.dVal / b.iVal);
				case __double:	return objects(a.dVal / b.dVal);
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: division by vector");
			}
		case __point3:
		case __vector3:
			switch (b.type)
			{
				case __long:		return objects(*a.vPtr / double(b.lVal));
				case __int:		return objects(*a.vPtr / double(b.iVal));
				case __double:	return objects(*a.vPtr / b.dVal);
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: division by vector");
			}
		default:
			throw new std::runtime_error("objects./");
	}
}
///////////////////////////////////////////////////////////////////////////////
objects operator^(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects.^");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(power(double(a.lVal) , double(b.lVal)));
				case __int:		return objects(power(double(a.lVal) , double(b.iVal)));
				case __double:	return objects(power(double(a.lVal) , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: exponentiation by vector");
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(power(double(a.iVal) , double(b.lVal)));
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
		return objects(power(double(a.iVal) , double(b.iVal)));
				case __double:	return objects(power(double(a.iVal) , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: exponentiation by vector");
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(power(a.dVal , double(b.lVal)));
				case __int:		return objects(power(a.dVal , double(b.iVal)));
				case __double:	return objects(power(a.dVal , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: exponentiation by vector");
			}
		case __point3:
		case __vector3:
			switch (b.type)
			{
				case __long:		return objects(Vector3D(power(a.vPtr->v[0], double(b.lVal)), power(a.vPtr->v[1], double(b.lVal)), power(a.vPtr->v[2], double(b.lVal))) );
				case __int:		return objects(Vector3D(power(a.vPtr->v[0], double(b.iVal)), power(a.vPtr->v[1], double(b.iVal)), power(a.vPtr->v[2], double(b.iVal))) );
				case __double:	return objects(Vector3D(power(a.vPtr->v[0], b.dVal), power(a.vPtr->v[1], b.dVal), power(a.vPtr->v[2], b.dVal)) );
				case __point3:
				case __vector3:	return objects( a.vPtr->vectorProd(*b.vPtr));
			}
		default:
			throw new std::runtime_error("objects.^");
	}
}
///////////////////////////////////////////////////////////////////////////////
objects operator%(const objects& a, const objects& b)
{
	if (a.type==__undefined || b.type==__undefined)
	{
		throw new std::runtime_error("objects.%");
	}
	switch (a.type)
	{
		case __long:
			switch (b.type)
			{
				case __long:		return objects(a.lVal % b.lVal);
				case __int:		return objects(a.lVal % b.iVal);
				case __double:	return objects(fmod(double(a.lVal) , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: mod by vector");
			}
		case __int:
			switch (b.type)
			{
				case __long:		return objects(a.iVal % b.lVal);
				case __int:
	{
		if (a.iVal == T_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_INFINITY) );
		if (a.iVal == T_MINUS_INFINITY && b.iVal == T_MINUS_INFINITY)	return objects( static_cast<int>(T_MINUS_INFINITY) );
	}
		return objects(a.iVal % b.iVal);
				case __double:	return objects(fmod(double(a.iVal) , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: mod by vector");
			}
		case __double:
			switch (b.type)
			{
				case __long:		return objects(fmod(a.dVal , double(b.lVal)));
				case __int:		return objects(fmod(a.dVal , double(b.iVal)));
				case __double:	return objects(fmod(a.dVal , b.dVal));
				case __point3:
				case __vector3:	throw SolveIt_ERROR(L"ERROR: mod by vector");
			}
		case __point3:
		case __vector3:
			switch (b.type)
			{
				case __long:		return objects(Vector3D(fmod(a.vPtr->v[0], double(b.lVal)), fmod(a.vPtr->v[1], double(b.lVal)), fmod(a.vPtr->v[2], double(b.lVal))) );
				case __int:		return objects(Vector3D(fmod(a.vPtr->v[0], double(b.iVal)), fmod(a.vPtr->v[1], double(b.iVal)), fmod(a.vPtr->v[2], double(b.iVal))) );
				case __double:	return objects(Vector3D(fmod(a.vPtr->v[0], b.dVal), fmod(a.vPtr->v[1], b.dVal), fmod(a.vPtr->v[2], b.dVal)) );
				case __point3:
				case __vector3:	return objects(*a.vPtr % *b.vPtr);
			}
		default:
			throw new std::runtime_error("objects.%");
	}
}
///////////////////////////////////////////////////////////////////////////////
int operator!(const objects& a)
{
	if (a.type == __long)		return (!a.lVal);
	if (a.type == __int)		return (!a.iVal);
	if (a.type == __double)		return !(a.dVal == 0.0);
	if (a.type == __vector3)	return !(*a.vPtr);
	throw SolveIt_ERROR(L"ERROR: !");
}
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Comparison operations
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int objects::operator==	(const objects& a) const
{
//	if (type == __double && a.type == __int)		return (dVal == double(a.iVal));
//	if (type == __int && a.type == __double)		return (a.dVal == double(iVal));

	if (type == __double && a.type == __long)		return (dVal == double(a.lVal));
	if (type == __long && a.type == __double)		return (a.dVal == double(lVal));

	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)		return (lVal == a.lVal);
	if (type == __int)		return (iVal == a.iVal);
	if (type == __double)	return (dVal == a.dVal);
	if (type == __vector3)	return (*vPtr == *a.vPtr);
	if (type == __point3)	return (*ptPtr == *a.ptPtr);
	if (type == __string)	return (*sPtr == *a.sPtr);//!(strcmp(sPtr, a.sPtr));
	if (type == __F_0)		return (fn == a.fn);
	if (type == __F_L)		return (fnL == a.fnL);
	if (type == __F_D)		return (fnX == a.fnX);
	if (type == __F_DD)		return (fnXY == a.fnXY);
	if (type == __F_DDD)	return (fnXYZ == a.fnXYZ);
	if (type == __F_LD)		return (fnLX == a.fnLX);
	if (type == __F_DL)		return (fnXL == a.fnXL);

	if (type == __instructionPtr)	return (iPtr == a.iPtr);
	if (type == __symbolPtr)		return (sym == a.sym);
	if (type == __objectPtr)		return (oPtr == a.oPtr);
	if (type == __RigidBodyPtr)		return (rbPtr==a.rbPtr);

	if (ref_count)	return (ref_count == a.ref_count);

	throw SolveIt_ERROR(L"ERROR ==: undefined");
}
int objects::operator!=	(const objects& a) const
{
	return	!(operator==(a));
}
int objects::operator>=	(const objects& a) const
{
//	if (type == __double && a.type == __int)		return (dVal >= double(a.iVal));
//	if (type == __int && a.type == __double)		return (double(iVal) >= a.dVal);
	if (type == __double && a.type == __long)		return (dVal >= double(a.lVal));
	if (type == __long && a.type == __double)		return (double(lVal) >= a.dVal);
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal >= a.lVal);
	if (type == __int)			return (iVal >= a.iVal);
	if (type == __double)		return (dVal >= a.dVal);
	throw SolveIt_ERROR(L"ERROR >=: undefined");
}
int objects::operator<=	(const objects& a) const
{
//	if (type == __double && a.type == __int)		return (dVal <= double(a.iVal));
//	if (type == __int && a.type == __double)		return (double(iVal) <= a.dVal);
	if (type == __double && a.type == __long)		return (dVal <= double(a.lVal));
	if (type == __long && a.type == __double)		return (double(lVal) <= a.dVal);
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal <= a.lVal);
	if (type == __int)			return (iVal <= a.iVal);
	if (type == __double)		return (dVal <= a.dVal);
	throw SolveIt_ERROR(L"ERROR <=: undefined");
}
int objects::operator>		(const objects& a) const
{
//	if (type == __double && a.type == __int)		return (dVal > double(a.iVal));
//	if (type == __int && a.type == __double)		return (double(iVal) > a.dVal);
	if (type == __double && a.type == __long)		return (dVal > double(a.lVal));
	if (type == __long && a.type == __double)		return (double(lVal) > a.dVal);
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal > a.lVal);
	if (type == __int)			return (iVal > a.iVal);
	if (type == __double)		return (dVal > a.dVal);
	throw SolveIt_ERROR(L"ERROR >: undefined");
}
int objects::operator<		(const objects& a) const
{
	if (type == __double && a.type == __long)		return (dVal < double(a.lVal));
	if (type == __long && a.type == __double)		return (double(lVal) < a.dVal);
//	if (type == __double && a.type == __int)		return (dVal < double(a.iVal));
//	if (type == __int && a.type == __double)		return (double(iVal) < a.dVal);
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal < a.lVal);
	if (type == __int)			return (iVal < a.iVal);
	if (type == __double)		return (dVal < a.dVal);
	throw SolveIt_ERROR(L"ERROR <: undefined");
}
int objects::operator&&	(const objects& a) const
{
	if (type == __double && a.type == __long)		return ((dVal != 0.0) && a.lVal);
	if (type == __long && a.type == __double)		return ( lVal  && (a.dVal != 0.0));
//	if (type == __double && a.type == __int)		return ((dVal != 0.0) && a.iVal);
//	if (type == __int && a.type == __double)		return ( iVal  && (a.dVal != 0.0));
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal && a.lVal);
	if (type == __int)			return (iVal && a.iVal);
	if (type == __double)		return (dVal != 0.0 && a.dVal != 0.0);
	throw SolveIt_ERROR(L"ERROR &&: undefined");
}
int objects::operator||	(const objects& a) const
{
//	if (type == __double && a.type == __int)		return ((dVal != 0.0) || a.iVal);
//	if (type == __int && a.type == __double)		return ( iVal  || (a.dVal != 0.0));
	if (type == __double && a.type == __long)		return ((dVal != 0.0) || a.lVal);
	if (type == __long && a.type == __double)		return ( lVal  || (a.dVal != 0.0));
	if (type != a.type)								throw SolveIt_ERROR(L"ERROR ==: incompatible types");
	if (type == __long)			return (lVal || a.lVal);
	if (type == __int)			return (iVal || a.iVal);
	if (type == __double)		return (dVal != 0.0 || a.dVal != 0.0);
	throw SolveIt_ERROR(L"ERROR ||: undefined");
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double objects::Dble(void) const
{
	if (type == __long)		return double(lVal);
	if (type == __int)		return double(iVal);
	if (type == __double)	return dVal;
	throw SolveIt_ERROR(L"ERROR Dble: undefined on type ");
}
///////////////////////////////////////////////////////////////////////////////
float objects::Float(void) const
{
	return float(Dble());
}
///////////////////////////////////////////////////////////////////////////////
int objects::Long() const
{
	if (type == __long)		return lVal;
	if (type == __int)		return iVal;
	if (type == __double)	return int(dVal);
	if (type == __string)	return int(_wtol( sPtr->c_str() ));//wcstol
	//if (type == __string)	return int(atol(GetC(sPtr->c_str())));
	throw SolveIt_ERROR(L"ERROR Long: undefined on type ");
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//std::wstring  GetWC(std::wstring const &  lpsz); //Action::DupString
//requires a call to	//GlobalFree((void*)lParam);
///////////////////////////////////////////////////////////////////////////////
//std::wstring  DupString(std::wstring const &  lpsz); //_tcsdup; memory leak
///////////////////////////////////////////////////////////////////////////////
const wchar_t* objects::String(void) const
{
	if (type == __string)
	{
		if (sPtr == 0) return 0;
		return (sPtr->c_str());
	}
	if (type == __long)
	{
		 ;
		std::wstring bstr = std::to_wstring( lVal ) ;
		return _wcsdup(bstr.c_str());
	}
	if (type == __int)
	{
		//long();
		std::wstring bstr = std::to_wstring( iVal );
		return _wcsdup(bstr.c_str());
	}
	if (type == __double)
	{
		  ;
		std::wstring bstr = std::to_wstring( dVal );
		return _wcsdup(bstr.c_str());
	}
	if (type == __vector3)
	{
		const Vector3D&a=*vPtr;
		/*ostrstream*/ std::wostringstream os;
		os << a.v[0] << L", " << a.v[1] << L", " << a.v[2];
		std::wstring bstr = os.str();
		return _wcsdup(bstr.c_str());
	}
	if (type == __point3)
	{
		const Point3D&a=*ptPtr;
		/*ostrstream*/ std::wostringstream os;
		os << a.v[0] << L", " << a.v[1] << L", " << a.v[2];
		std::wstring bstr = os.str();
		return _wcsdup(bstr.c_str());
	}

	throw SolveIt_ERROR(L"ERROR String: undefined on ");
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	bool objects::True() const {
		switch (type)
		{
			case __long:	if (lVal != 0) return true;return false;
				break;
			case __int:	if (iVal != 0) return true;return false;
				break;
			case __double:	if (dVal != 0.0) return true;return false;
				break;
			case __point3:	if (ptPtr && ptPtr->norm2() != 0.0) return true;return false;
				break;
			case __vector3:	if (vPtr && vPtr->norm2() != 0.0) return true;return false;
				break;
			case __string:	if (sPtr && sPtr->size() >= 0) return true;return false;
				break;

		case __F_0:		if (fn != 0) return true;return false;
			break;
		case __F_L:		if (fnL != 0) return true;return false;
			break;
		case __F_D:		if (fnX != 0) return true;return false;
			break;
		case __F_DD:	if (fnXY != 0) return true;return false;
			break;
		case __F_DDD:	if (fnXYZ != 0) return true;return false;
			break;
		case __F_LD:	if (fnLX != 0) return true;return false;
			break;
		case __F_DL:	if (fnXL != 0) return true;return false;
			break;
		
		case __complex: 
		//	ref_count=a.ref_count;
		//	(*ref_count)++;
		//	cPtr=a.cPtr;
			break;
		
		case __objectPtr:
			if (oPtr != 0) return true;return false;
			break;
		case __instructionPtr:
			if (iPtr != 0) return true;return false;
			break;
		
		case __symbolPtr: 
		//	ref_count=a.ref_count;
		//	(*ref_count)++;
			if (sym != 0) return true;return false;
			break;


			default:
				throw new std::runtime_error("objects.True.type");//
				break;
		}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
 wistream& SolveIt::operator>>(wistream& _I, objects& b)
	{
//		_I.eatwhite();
		_I >> ws;
		if (b.type == __long)
		{
			long a=0;
			if (!(_I >> a).fail())
			{
				b = objects(a);
			}
		}
		else
		if (b.type == __int)
		{
			int a=0;
			if (!(_I >> a).fail())
			{
				b = objects(a);
			}
		}
		else
		if (b.type == __double)
		{
			double a=0;
			if (!(_I >> a).fail())
			{
				b = objects(a);
			}
		}
		else
		if (b.type == __vector3)
		{
			Vector3D a;
			if (!(_I >> a).fail())
			{
				b = objects(a);
			}
		}
		else
		if (b.type == __point3)
		{
			Point3D a;
			if (!(_I >> a).fail())
			{
				b = objects(a);
			}
		}
		else
		if (b.type == __string)
		{
			wstring a;
			if (!(_I >> a).fail())
			{
				b = objects(a);
//	//			b.sPtr = _tcsdup(b.c_str());//DupString(b.c_str());
			}
		}
		else
		if (b.type == __undefined)//fix this
		{
			wchar_t buf[1256];
			_I.getline( buf, sizeof(buf)/sizeof(buf[0]) );
			if ( _I.fail() ) return (_I);

			{
				long a=0;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}
			{
				int a=0;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}
			{
				double a=0;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}
			{
				wstring a;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}
			{
				Vector3D a;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}
			{
				Point3D a;
				/*istrstream*/ std::wistringstream is(buf);
				if (!(is >> a).fail())
				{
					/*istrstream*/ std::wistringstream iss(buf);
					if (!(iss >> a).fail())
					{
						b=objects(a);
						return (_I);
					}
				}
			}

		}
		return (_I);
	}
///////////////////////////////////////////////////////////////////////////////

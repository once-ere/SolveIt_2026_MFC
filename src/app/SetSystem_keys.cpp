#include <iostream>
#include <cstring>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>
#include <cstring>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
#define INVALID_FILE_ATTRIBUTES             ((DWORD)-1) 
///////////////////////////////////////////////////////////////////////////////
/*
32-bit uint
4294967295
42 94 96 72 95
4294 967 295
*/

#define xstr(a) str(a)
#define str(a) #a


#define CONCAT2(s1,s2) s1 ## s2
#define new_case2(s1,s2) \
	ostrm << xstr(CONCAT2(s1,s2)) << ",\n";cntr++;


#define CONCAT3(s1,s2,s3) s1 ## s2 ## s3
#define new_case3(s1,s2,s3) \
	ostrm << xstr(CONCAT3(s1,s2,s3)) << ",\n";cntr++;


//SetSystem.cpp:
///////////////////////////////////////////////////////////////////////////////
#define pos_case(X, w) \
	new_case2( T_POSITION, T_ ## X ## COMP)

#define pos_cases \
		pos_case(X,x) \
		pos_case(Y,y) \
		pos_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define mom_case(X, w) \
	new_case2( T_MOMENTUM, T_ ## X ## COMP)

#define mom_cases \
		mom_case(X,x) \
		mom_case(Y,y) \
		mom_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define vel_case(X, w) \
	new_case2( T_VELOCITY, T_ ## X ## COMP)

#define vel_cases \
		vel_case(X,x) \
		vel_case(Y,y) \
		vel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define color_case(X, w) \
	new_case2( T_COLOR, T_ ## X ## COMP)

#define color_cases \
		color_case(X,x) \
		color_case(Y,y) \
		color_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define amom_case(X, w) \
	new_case2( T_ANGULARMOMENTUM, T_ ## X ## COMP)

#define amom_cases \
		amom_case(X,x) \
		amom_case(Y,y) \
		amom_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define avel_case(X, w) \
	new_case2( T_ANGULARVELOCITY, T_ ## X ## COMP)

#define avel_cases \
		avel_case(X,x) \
		avel_case(Y,y) \
		avel_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define ext_grav_case(X, w) \
	new_case2( T_GRAVITATIONAL_FIELD, T_ ## X ## COMP)

#define ext_grav_cases \
		ext_grav_case(X,x) \
		ext_grav_case(Y,y) \
		ext_grav_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
#define lennardjones_case(X, w) \
	new_case2( T_LENNARD_JONES, T_ ## X ## COMP)

#define lennardjones_cases \
		lennardjones_case(X,x) \
		lennardjones_case(Y,y) \
		lennardjones_case(Z,z)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define forcetime_case(X) \
	new_case3( T_BODY_FORCE_EXTERNAL, T_ ## X ## COMP, T_SYSTEM_TIME) 

#define forcetime_cases \
		forcetime_case(X) \
		forcetime_case(Y) \
		forcetime_case(Z)
///////////////////////////////////////////////////////////////////////////////
#define force_case(X, w) \
	new_case2( T_BODY_FORCE_EXTERNAL, T_ ## X ## COMP) 

#define force_cases \
		force_case(X,x) \
		force_case(Y,y) \
		force_case(Z,z)
/*
// cl.exe /std:c++17 /EHsc SetSystem_keys.cpp /Fe: SetSystem_keys.exe

//tree.cpp:
new_case2(T_STATE , T_EigenNumber)
new_case2(T_STATE , T_ENERGY)
new_case2(T_STATE , T_SCALE)
new_case2( T_SCENE, T_SYSTEM_RIGHT)
new_case2( T_SCENE, T_SYSTEM_LEFT)
new_case2( T_SCENE, T_SYSTEM_TOP)
new_case2( T_SCENE, T_SYSTEM_BOTTOM)
new_case2( T_SCENE, T_SYSTEM_FAR)
new_case2( T_SCENE, T_SYSTEM_NEAR)
new_case2( T_TRANSLATE, T_XCOMP)
new_case2( T_TRANSLATE, T_YCOMP)
new_case2( T_TRANSLATE, T_ZCOMP)
new_case2( T_TITLE, T_XCOMP)
new_case2( T_TITLE, T_YCOMP)
new_case2( T_TICMARK, T_XCOMP)
new_case2( T_TICMARK, T_YCOMP)
*/
///////////////////////////////////////////////////////////////////////////////
int main()
{
	int cntr{0};
    std::string filename = "extended_yytokens.h";
    
        std::ofstream ostrm(filename);//, std::ios::binary);
		pos_cases
		mom_cases
		amom_cases
		force_cases
		forcetime_cases
		vel_cases
		avel_cases
	ext_grav_cases
	lennardjones_cases
		color_cases

#define RECORD_LINE_NUMBER00(X) X=__LINE__;
#define RECORD_LINE_NUMBER(X) RECORD_LINE_NUMBER00(X)
static constexpr const std::uint32_t RECORD_LINE_NUMBER(LOCATIONS_begin)
	new_case3(T_DOMAIN, T_XCOMP, T_BEGIN)
	new_case3(T_DOMAIN, T_XCOMP, T_END)
	new_case3(T_DOMAIN, T_YCOMP, T_BEGIN)
	new_case3(T_DOMAIN, T_YCOMP, T_END)
	new_case3(T_DOMAIN, T_ZCOMP, T_BEGIN)
	new_case3(T_DOMAIN, T_ZCOMP, T_END)
	new_case2(T_STATE , T_SCALE)
	new_case3(T_STATE , T_POTENTIAL, T_SCALE)
	new_case2(T_STATE , T_Stationary)
	new_case2(T_STATE , T_Scattering)
	new_case2(T_STATE , T_ENERGY)
	new_case2(T_STATE , T_EigenNumber)
	new_case2(T_STATE , T_MASS)
	new_case2( T_STATE, T_Renormalize)
	new_case2( T_Scattering, T_Renormalize)
	new_case2( T_Scattering, T_Incident)
	new_case2( T_Scattering, T_FourierTransform)
	new_case2( T_Scattering, T_WaveNumber)
	new_case2( T_Scattering, T_Method)
	new_case2( T_Stationary, T_Method)
	new_case2( T_DATA, T_FILE)
	new_case2( T_TITLE, T_XCOMP )
	new_case2( T_TITLE, T_YCOMP )
	new_case2( T_TICMARK, T_XCOMP )
	new_case2( T_TICMARK, T_YCOMP )
	new_case2( T_TRANSLATE, T_XCOMP)
	new_case2( T_TRANSLATE, T_YCOMP)
	new_case2( T_TRANSLATE, T_ZCOMP)
	new_case2( T_ROTATE, T_XCOMP)
	new_case2( T_ROTATE, T_YCOMP)
	new_case2( T_ROTATE, T_ZCOMP)
	new_case2( T_ELECTRIC_FIELD, T_XCOMP)
	new_case2( T_ELECTRIC_FIELD, T_YCOMP)
	new_case2( T_ELECTRIC_FIELD, T_ZCOMP)
	new_case2( T_MAGNETIC_FIELD, T_XCOMP)
	new_case2( T_MAGNETIC_FIELD, T_YCOMP)
	new_case2( T_MAGNETIC_FIELD, T_ZCOMP)
	new_case2( T_SYSTEM_Background, T_SHOW)
	new_case2( T_SYSTEM_VectorFields, T_SHOW)
	new_case2( T_SYSTEM_FieldLines, T_SHOW)
	new_case2( T_SYSTEM_Equipotentials, T_SHOW)
	new_case2( T_DEFERRED_COMMAND, T_ONRESET)
	new_case2( T_SCENE, T_SYSTEM_LEFT)
	new_case2( T_SCENE, T_SYSTEM_RIGHT)
	new_case2( T_SCENE, T_SYSTEM_TOP)
	new_case2( T_SCENE, T_SYSTEM_BOTTOM)
	new_case2( T_SCENE, T_SYSTEM_NEAR)
	new_case2( T_SCENE, T_SYSTEM_FAR)
	new_case2( T_POTENTIAL, T_SYSTEM_TIME)
	new_case2( T_POTENTIAL, T_SHOW)
	new_case2( T_CONTACTS, T_SHOW)
	new_case3(T_RECORD, T_SYSTEM_TIME, T_BEGIN)
	new_case3(T_RECORD, T_SYSTEM_TIME, T_END)
	new_case2(T_RECORD, T_PERIOD)
	new_case2(T_RECORD, T_TRAJECTORY)
	new_case2(T_TRAJECTORY, T_COLOR)
	new_case2( T_PULLEY, T_SYSTEM_LEFT)
	new_case2( T_PULLEY, T_SYSTEM_RIGHT)
	new_case2( T_PLANE, T_ANGLE)
	new_case2( T_PLANE, T_POINT_IN_PLANE)
	new_case2( T_POSITION, T_SCALE)
	new_case2( T_VELOCITY, T_SCALE)
	new_case2( T_NORMAL, T_SCALE)
	new_case2( T_MOMENTUM, T_SCALE)
	new_case2( T_ANGULARMOMENTUM, T_SCALE)
	new_case2( T_FORCE, T_SCALE)
	new_case2( T_ACCELERATION, T_SCALE)
	new_case2( T_contactFORCE, T_SCALE)
	new_case2( T_TORQUE, T_SCALE)
	new_case2( T_contactTORQUE, T_SCALE)
	new_case2( T_POSITION, T_SHOW)
	new_case2( T_VELOCITY, T_SHOW)
	new_case2( T_NORMAL, T_SHOW)
	new_case2( T_MOMENTUM, T_SHOW)
	new_case2( T_ANGULARMOMENTUM, T_SHOW)
	new_case2( T_FORCE, T_SHOW)
	new_case2( T_ACCELERATION, T_SHOW)
	new_case2( T_contactFORCE, T_SHOW)
	new_case2( T_Constraint, T_SHOW)
	new_case2( T_TORQUE, T_SHOW)
	new_case2( T_contactTORQUE, T_SHOW)
	new_case2( T_TRAJECTORY, T_SHOW)
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_SPRING_LENGTH)
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_SPRING_CONSTANT)
	new_case2(T_MultiBody_Link_Joint,T_MultiBody_CONTROL)		
	new_case2(T_MultiBody_Link_Joint,T_DISPLACEMENT)
	new_case2(T_MultiBody_Link_Joint,T_VELOCITY)
static constexpr const std::uint32_t RECORD_LINE_NUMBER(LOCATIONS_end)
    std::cout<<cntr<<std::endl;
    std::cout<<LOCATIONS_end-LOCATIONS_begin<<std::endl;
    std::cout<<30+LOCATIONS_end-LOCATIONS_begin<<std::endl;
    return 0;
}

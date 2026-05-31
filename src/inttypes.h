#ifndef inttypes_h
#define inttypes_h

typedef	signed int int32_t;
typedef	unsigned int uint32_t;
#ifdef WIN32
typedef	signed __int64	int64_t;
#else
typedef	signed long long int int64_t;
#endif

#ifdef WIN32
#include <winsock.h> //For u_long used in SbDict.h 
#endif

#if defined(__sun) || defined(__GNUG__) || defined(__sgi)
#define DEFINE_U_LONG
#define DEFINE_U_SHORT
#endif

#ifdef DEFINE_U_LONG
typedef unsigned long u_long;
#endif

#ifdef DEFINE_U_SHORT
typedef unsigned short u_short;
#endif

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;

//typedef long long int int64_t;

/* Unsigned.  */

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long long int uint64_t;


#endif

/* D:\projects\cpp\F2C\test\test.f -- translated by f2c (version of 23 April 1993  18:34:30).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Main program */ MAIN__()
{
    /* Builtin functions */
    /* Subroutine */ int s_copy();

    /* Local variables */
    static integer i;
    static char str[10];

    for (i = 1; i <= 10; ++i) {
	s_copy(str, "Hello world", 10L, 11L);
/* 	write(6,*) str//"-",i,i*i */
    }
} /* MAIN__ */

/* Main program alias */ int test_ () { MAIN__ (); }

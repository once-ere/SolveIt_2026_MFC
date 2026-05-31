/* fprota.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Subroutine */ int fprota_(doublereal *cos__, doublereal *sin__, doublereal 
	*a, doublereal *b)
{
    static doublereal stor1, stor2;

/*  subroutine fprota applies a givens rotation to a and b. */
/*  .. */
/*  ..scalar arguments.. */
/* ..local scalars.. */
/*  .. */
    stor1 = *a;
    stor2 = *b;
    *b = *cos__ * stor2 + *sin__ * stor1;
    *a = *cos__ * stor1 - *sin__ * stor2;
    return 0;
} /* fprota_ */

#ifdef __cplusplus
	}
#endif

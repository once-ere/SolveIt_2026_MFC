/* fpgivs.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Subroutine */ int fpgivs_(doublereal *piv, doublereal *ww, doublereal *
	cos__, doublereal *sin__)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal dd, one, store;

/*  subroutine fpgivs calculates the parameters of a givens */
/*  transformation . */
/*  .. */
/*  ..scalar arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  .. */
    one = 1.;
    store = abs(*piv);
    if (store >= *ww) {
/* Computing 2nd power */
	d__1 = *ww / *piv;
	dd = store * sqrt(one + d__1 * d__1);
    }
    if (store < *ww) {
/* Computing 2nd power */
	d__1 = *piv / *ww;
	dd = *ww * sqrt(one + d__1 * d__1);
    }
    *cos__ = *ww / dd;
    *sin__ = *piv / dd;
    *ww = dd;
    return 0;
} /* fpgivs_ */

#ifdef __cplusplus
	}
#endif

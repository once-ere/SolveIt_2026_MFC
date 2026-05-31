/* fpbspl.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <f2c/f2c.h>
///////////////////////////////////////////////////////////////////////////////

/* Subroutine */ int fpbspl_(doublereal *t, integer *n, integer *k, 
	doublereal *x, integer *l, doublereal *h__)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static doublereal f;
    static integer i__, j;
    static doublereal hh[5];
    static integer li, lj;
    static doublereal one;

/*  subroutine fpbspl evaluates the (k+1) non-zero b-splines of */
/*  degree k at t(l) <= x < t(l+1) using the stable recurrence */
/*  relation of de boor and cox. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  .. */
    /* Parameter adjustments */
    --t;
    --h__;

    /* Function Body */
    one = 1.;
    h__[1] = one;
    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    hh[i__ - 1] = h__[i__];
/* L10: */
	}
	h__[1] = 0.;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    li = *l + i__;
	    lj = li - j;
	    f = hh[i__ - 1] / (t[li] - t[lj]);
	    h__[i__] += f * (t[li] - *x);
	    h__[i__ + 1] = f * (*x - t[lj]);
/* L20: */
	}
    }
    return 0;
} /* fpbspl_ */

#ifdef __cplusplus
	}
#endif

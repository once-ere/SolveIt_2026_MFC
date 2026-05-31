/* drf.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/


#include "stdafx.h"
extern "C" {

#undef min
#undef max

#include <f2c/f2c.h>



/* Table of constant values */

static integer c__3 = 3;
static doublereal c_b3 = .16666666666666666;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__6 = 6;
static integer c__8 = 8;

/* DECK DRF */
doublereal drf_(doublereal *x, doublereal *y, doublereal *z__, integer *ier)
{
    /* Initialized data */

    static logical first = TRUE_;

    /* System generated locals */
    address a__1[6], a__2[8];
    integer i__1[6], i__2[8];
    doublereal ret_val, d__1, d__2;
    char ch__1[88], ch__2[117], ch__3[123];

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    ;
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal s, c1, c2, c3, e2, e3, mu, xn, yn, zn;
    static char xern3[16], xern4[16], xern5[16], xern6[16];
    static doublereal lamda, lolim, xndev, yndev, uplim, zndev;
    extern doublereal d1mach_(integer *);
    static doublereal epslon;
    extern /* Subroutine */ int xermsg_(char *, char *, char *, integer *, 
	    integer *, ftnlen, ftnlen, ftnlen);
    static doublereal errtol, xnroot, ynroot, znroot;

    /* Fortran I/O blocks */
    static icilist io___9 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___11 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___13 = { 0, xern5, 0, "(1PE15.6)", 16, 1 };
    static icilist io___14 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___15 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___16 = { 0, xern5, 0, "(1PE15.6)", 16, 1 };
    static icilist io___18 = { 0, xern6, 0, "(1PE15.6)", 16, 1 };
    static icilist io___19 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___20 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___21 = { 0, xern5, 0, "(1PE15.6)", 16, 1 };
    static icilist io___22 = { 0, xern6, 0, "(1PE15.6)", 16, 1 };


/* ***BEGIN PROLOGUE  DRF */
/* ***PURPOSE  Compute the incomplete or complete elliptic integral of the */
/*            1st kind.  For X, Y, and Z non-negative and at most one of */
/*            them zero, RF(X,Y,Z) = Integral from zero to infinity of */
/*                                -1/2     -1/2     -1/2 */
/*                      (1/2)(t+X)    (t+Y)    (t+Z)    dt. */
/*            If X, Y or Z is zero, the integral is complete. */
/* ***LIBRARY   SLATEC */
/* ***CATEGORY  C14 */
/* ***TYPE      DOUBLE PRECISION (RF-S, DRF-D) */
/* ***KEYWORDS  COMPLETE ELLIPTIC INTEGRAL, DUPLICATION THEOREM, */
/*             INCOMPLETE ELLIPTIC INTEGRAL, INTEGRAL OF THE FIRST KIND, */
/*             TAYLOR SERIES */
/* ***AUTHOR  Carlson, B. C. */
/*             Ames Laboratory-DOE */
/*             Iowa State University */
/*             Ames, IA  50011 */
/*           Notis, E. M. */
/*             Ames Laboratory-DOE */
/*             Iowa State University */
/*             Ames, IA  50011 */
/*           Pexton, R. L. */
/*             Lawrence Livermore National Laboratory */
/*             Livermore, CA  94550 */
/* ***DESCRIPTION */

/*   1.     DRF */
/*          Evaluate an INCOMPLETE (or COMPLETE) ELLIPTIC INTEGRAL */
/*          of the first kind */
/*          Standard FORTRAN function routine */
/*          Double precision version */
/*          The routine calculates an approximation result to */
/*          DRF(X,Y,Z) = Integral from zero to infinity of */

/*                               -1/2     -1/2     -1/2 */
/*                     (1/2)(t+X)    (t+Y)    (t+Z)    dt, */

/*          where X, Y, and Z are nonnegative and at most one of them */
/*          is zero.  If one of them  is zero, the integral is COMPLETE. */
/*          The duplication theorem is iterated until the variables are */
/*          nearly equal, and the function is then expanded in Taylor */
/*          series to fifth order. */

/*   2.     Calling sequence */
/*          DRF( X, Y, Z, IER ) */

/*          Parameters On entry */
/*          Values assigned by the calling routine */

/*          X      - Double precision, nonnegative variable */

/*          Y      - Double precision, nonnegative variable */

/*          Z      - Double precision, nonnegative variable */



/*          On Return    (values assigned by the DRF routine) */

/*          DRF     - Double precision approximation to the integral */

/*          IER    - Integer */

/*                   IER = 0 Normal and reliable termination of the */
/*                           routine. It is assumed that the requested */
/*                           accuracy has been achieved. */

/*                   IER >  0 Abnormal termination of the routine */

/*          X, Y, Z are unaltered. */


/*   3.    Error Messages */


/*         Value of IER assigned by the DRF routine */

/*                  Value assigned         Error Message Printed */
/*                  IER = 1                MIN(X,Y,Z) .LT. 0.0D0 */
/*                      = 2                MIN(X+Y,X+Z,Y+Z) .LT. LOLIM */
/*                      = 3                MAX(X,Y,Z) .GT. UPLIM */



/*   4.     Control Parameters */

/*                  Values of LOLIM, UPLIM, and ERRTOL are set by the */
/*                  routine. */

/*          LOLIM and UPLIM determine the valid range of X, Y and Z */

/*          LOLIM  - Lower limit of valid arguments */

/*                   Not less than 5 * (machine minimum). */

/*          UPLIM  - Upper limit of valid arguments */

/*                   Not greater than (machine maximum) / 5. */


/*                     Acceptable values for:   LOLIM      UPLIM */
/*                     IBM 360/370 SERIES   :   3.0D-78     1.0D+75 */
/*                     CDC 6000/7000 SERIES :   1.0D-292    1.0D+321 */
/*                     UNIVAC 1100 SERIES   :   1.0D-307    1.0D+307 */
/*                     CRAY                 :   2.3D-2466   1.09D+2465 */
/*                     VAX 11 SERIES        :   1.5D-38     3.0D+37 */



/*          ERRTOL determines the accuracy of the answer */

/*                 The value assigned by the routine will result */
/*                 in solution precision within 1-2 decimals of */
/*                 "machine precision". */



/*          ERRTOL - Relative error due to truncation is less than */
/*                   ERRTOL ** 6 / (4 * (1-ERRTOL)  . */



/*        The accuracy of the computed approximation to the integral */
/*        can be controlled by choosing the value of ERRTOL. */
/*        Truncation of a Taylor series after terms of fifth order */
/*        introduces an error less than the amount shown in the */
/*        second column of the following table for each value of */
/*        ERRTOL in the first column.  In addition to the truncation */
/*        error there will be round-off error, but in practice the */
/*        total error from both sources is usually less than the */
/*        amount given in the table. */





/*          Sample choices:  ERRTOL   Relative Truncation */
/*                                    error less than */
/*                           1.0D-3    3.0D-19 */
/*                           3.0D-3    2.0D-16 */
/*                           1.0D-2    3.0D-13 */
/*                           3.0D-2    2.0D-10 */
/*                           1.0D-1    3.0D-7 */


/*                    Decreasing ERRTOL by a factor of 10 yields six more */
/*                    decimal digits of accuracy at the expense of one or */
/*                    two more iterations of the duplication theorem. */

/* *Long Description: */

/*   DRF Special Comments */



/*          Check by addition theorem: DRF(X,X+Z,X+W) + DRF(Y,Y+Z,Y+W) */
/*          = DRF(0,Z,W), where X,Y,Z,W are positive and X * Y = Z * W. */


/*          On Input: */

/*          X, Y, and Z are the variables in the integral DRF(X,Y,Z). */


/*          On Output: */


/*          X, Y, Z are unaltered. */



/*          ******************************************************** */

/*          WARNING: Changes in the program may improve speed at the */
/*                   expense of robustness. */



/*   Special double precision functions via DRF */




/*                  Legendre form of ELLIPTIC INTEGRAL of 1st kind */

/*                  ----------------------------------------- */



/*                                             2         2   2 */
/*                  F(PHI,K) = SIN(PHI) DRF(COS (PHI),1-K SIN (PHI),1) */


/*                                  2 */
/*                  K(K) = DRF(0,1-K ,1) */


/*                         PI/2     2   2      -1/2 */
/*                       = INT  (1-K SIN (PHI) )   D PHI */
/*                          0 */



/*                  Bulirsch form of ELLIPTIC INTEGRAL of 1st kind */

/*                  ----------------------------------------- */


/*                                          2 2    2 */
/*                  EL1(X,KC) = X DRF(1,1+KC X ,1+X ) */


/*                  Lemniscate constant A */

/*                  ----------------------------------------- */


/*                       1      4 -1/2 */
/*                  A = INT (1-S )    DS = DRF(0,1,2) = DRF(0,2,1) */
/*                       0 */



/*    ------------------------------------------------------------------- */

/* ***REFERENCES  B. C. Carlson and E. M. Notis, Algorithms for incomplete */
/*                 elliptic integrals, ACM Transactions on Mathematical */
/*                 Software 7, 3 (September 1981), pp. 398-403. */
/*               B. C. Carlson, Computing elliptic integrals by */
/*                 duplication, Numerische Mathematik 33, (1979), */
/*                 pp. 1-16. */
/*               B. C. Carlson, Elliptic integrals of the first kind, */
/*                 SIAM Journal of Mathematical Analysis 8, (1977), */
/*                 pp. 231-242. */
/* ***ROUTINES CALLED  D1MACH, XERMSG */
/* ***REVISION HISTORY  (YYMMDD) */
/*   790801  DATE WRITTEN */
/*   890531  Changed all specific intrinsics to generic.  (WRB) */
/*   891009  Removed unreferenced statement labels.  (WRB) */
/*   891009  REVISION DATE from Version 3.2 */
/*   891214  Prologue converted to Version 4.0 format.  (BAB) */
/*   900315  CALLs to XERROR changed to CALLs to XERMSG.  (THJ) */
/*   900326  Removed duplicate information from DESCRIPTION section. */
/*           (WRB) */
/*   900510  Changed calls to XERMSG to standard form, and some */
/*           editorial changes.  (RWC)) */
/*   920501  Reformatted the REFERENCES section.  (WRB) */
/* ***END PROLOGUE  DRF */

/* ***FIRST EXECUTABLE STATEMENT  DRF */

    if (first) {
	d__1 = d1mach_(&c__3) * 4.;
	errtol = pow_dd(&d__1, &c_b3);
	lolim = d1mach_(&c__1) * 5.;
	uplim = d1mach_(&c__2) / 5.;

	c1 = .041666666666666664;
	c2 = .068181818181818177;
	c3 = .071428571428571425;
    }
    first = FALSE_;

/*         CALL ERROR HANDLER IF NECESSARY. */

    ret_val = 0.;
/* Computing MIN */
    d__1 = min(*x,*y);
    if (min(d__1,*z__) < 0.) {
	*ier = 1;
	s_wsfi(&io___9);
	do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___11);
	do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___13);
	do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
	e_wsfi();
/* Writing concatenation */
	i__1[0] = 26, a__1[0] = "MIN(X,Y,Z).LT.0 WHERE X = ";
	i__1[1] = 16, a__1[1] = xern3;
	i__1[2] = 5, a__1[2] = " Y = ";
	i__1[3] = 16, a__1[3] = xern4;
	i__1[4] = 9, a__1[4] = " AND Z = ";
	i__1[5] = 16, a__1[5] = xern5;
	s_cat(ch__1, a__1, i__1, &c__6, (ftnlen)88);
	xermsg_("SLATEC", "DRF", ch__1, &c__1, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)88);
	return ret_val;
    }

/* Computing MAX */
    d__1 = max(*x,*y);
    if (max(d__1,*z__) > uplim) {
	*ier = 3;
	s_wsfi(&io___14);
	do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___15);
	do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___16);
	do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___18);
	do_fio(&c__1, (char *)&uplim, (ftnlen)sizeof(doublereal));
	e_wsfi();
/* Writing concatenation */
	i__2[0] = 30, a__2[0] = "MAX(X,Y,Z).GT.UPLIM WHERE X = ";
	i__2[1] = 16, a__2[1] = xern3;
	i__2[2] = 5, a__2[2] = " Y = ";
	i__2[3] = 16, a__2[3] = xern4;
	i__2[4] = 5, a__2[4] = " Z = ";
	i__2[5] = 16, a__2[5] = xern5;
	i__2[6] = 13, a__2[6] = " AND UPLIM = ";
	i__2[7] = 16, a__2[7] = xern6;
	s_cat(ch__2, a__2, i__2, &c__8, (ftnlen)117);
	xermsg_("SLATEC", "DRF", ch__2, &c__3, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)117);
	return ret_val;
    }

/* Computing MIN */
    d__1 = *x + *y, d__2 = *x + *z__, d__1 = min(d__1,d__2), d__2 = *y + *z__;
    if (min(d__1,d__2) < lolim) {
	*ier = 2;
	s_wsfi(&io___19);
	do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___20);
	do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___21);
	do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___22);
	do_fio(&c__1, (char *)&lolim, (ftnlen)sizeof(doublereal));
	e_wsfi();
/* Writing concatenation */
	i__2[0] = 36, a__2[0] = "MIN(X+Y,X+Z,Y+Z).LT.LOLIM WHERE X = ";
	i__2[1] = 16, a__2[1] = xern3;
	i__2[2] = 5, a__2[2] = " Y = ";
	i__2[3] = 16, a__2[3] = xern4;
	i__2[4] = 5, a__2[4] = " Z = ";
	i__2[5] = 16, a__2[5] = xern5;
	i__2[6] = 13, a__2[6] = " AND LOLIM = ";
	i__2[7] = 16, a__2[7] = xern6;
	s_cat(ch__3, a__2, i__2, &c__8, (ftnlen)123);
	xermsg_("SLATEC", "DRF", ch__3, &c__2, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)123);
	return ret_val;
    }

    *ier = 0;
    xn = *x;
    yn = *y;
    zn = *z__;

L30:
    mu = (xn + yn + zn) / 3.;
    xndev = 2. - (mu + xn) / mu;
    yndev = 2. - (mu + yn) / mu;
    zndev = 2. - (mu + zn) / mu;
/* Computing MAX */
    d__1 = abs(xndev), d__2 = abs(yndev), d__1 = max(d__1,d__2), d__2 = abs(
	    zndev);
    epslon = max(d__1,d__2);
    if (epslon < errtol) {
	goto L40;
    }
    xnroot = sqrt(xn);
    ynroot = sqrt(yn);
    znroot = sqrt(zn);
    lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    zn = (zn + lamda) * .25;
    goto L30;

L40:
    e2 = xndev * yndev - zndev * zndev;
    e3 = xndev * yndev * zndev;
    s = (c1 * e2 - .1 - c2 * e3) * e2 + 1. + c3 * e3;
    ret_val = s / sqrt(mu);

    return ret_val;
} /* drf_ */

}

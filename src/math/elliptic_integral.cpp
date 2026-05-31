/* elliptic_integral.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "stdafx.h"
extern "C" {

#undef min
#undef max

#include <f2c/f2c.h>

/* Table of constant values */

static integer c__1 = 1;

/* ACM ALGORITHM 577 */

/* ALGORITHMS FOR INCOMPLETE ELLIPTIC INTEGRALS */

/* BY B.C. CARLSON AND E.M. NOTIS */

/* ACM TRANSACTIONS ON MATHEMATICAL SOFTWARE, SEPTEMBER, 1981. */


/*     THIS FILE CONTAINS FOUR SUBROUTINES FOR COMPUTING INCOMPLETE */
/*     ELLIPTIC INTEGRALS, FOLLOWED BY SIX DRIVERS FOR TESTING THE */
/*     SUBROUTINES.  EACH SUBROUTINE AND EACH DRIVER IS PRECEDED BY */
/*     A COMMENT CARD WITH A LINE OF DOLLAR SIGNS, AND EACH DRIVER IS */
/*     FOLLOWED BY ITS INPUT DATA IF ANY.  THE FOUR SUBROUTINES HAVE */
/*     THE NAMES RC, RF, RD, RJ IN THAT ORDER.  THE DRIVERS HAVE NO */
/*     NAMES BUT BEGIN WITH DESCRIPTIVE COMMENTS.  THE FIRST FOUR */
/*     DRIVERS TEST RC, RF, RD, RJ IN THAT ORDER.  THE FIFTH DRIVER */
/*     TESTS RC AGAINST LIBRARY ROUTINES.  THE SIXTH DRIVER TESTS RF */
/*     AGAINST THE FUNPACK SUBROUTINE DELIKM. */


/*     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */


/*          ******************************************************** */

doublereal rc_(doublereal *x, doublereal *y, doublereal *errtol, integer *
	ierr)
{
    /* Initialized data */

    static integer printr = 6;
    static doublereal lolim = 3e-78;
    static doublereal uplim = 1e75;

    /* Format strings */
    static char fmt_104[] = "(\0020\002,\002*** ERROR - INVALID ARGUMENTS PA"
	    "SSED TO RC\002)";
    static char fmt_108[] = "(\002 \002,\002X = \002,d23.16,4x,\002Y = \002,"
	    "d23.16)";

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal s, c1, c2, sn, mu, xn, yn, lamda;

    /* Fortran I/O blocks */
    static cilist io___4 = { 0, 0, 0, fmt_104, 0 };
    static cilist io___5 = { 0, 0, 0, fmt_108, 0 };



/*          THIS FUNCTION SUBROUTINE COMPUTES THE ELEMENTARY INTEGRAL */
/*          RC(X,Y) = INTEGRAL FROM ZERO TO INFINITY OF */

/*                              -1/2     -1 */
/*                    (1/2)(T+X)    (T+Y)  DT, */

/*          WHERE X IS NONNEGATIVE AND Y IS POSITIVE.  THE DUPLICATION */
/*          THEOREM IS ITERATED UNTIL THE VARIABLES ARE NEARLY EQUAL, */
/*          AND THE FUNCTION IS THEN EXPANDED IN TAYLOR SERIES TO FIFTH */
/*          ORDER.  LOGARITHMIC, INVERSE CIRCULAR, AND INVERSE HYPER- */
/*          BOLIC FUNCTIONS CAN BE EXPRESSED IN TERMS OF RC.  REFERENCE: */
/*          B. C. CARLSON, COMPUTING ELLIPTIC INTEGRALS BY DUPLICATION, */
/*          NUMER. MATH. 33 (1979), 1-16.  CODED BY B. C. CARLSON AND */
/*          ELAINE M. NOTIS, AMES LABORATORY-DOE, IOWA STATE UNIVERSITY, */
/*          AMES, IOWA 50011.  MARCH 1, 1980. */

/*          CHECK BY ADDITION THEOREM: RC(X,X+Z) + RC(Y,Y+Z) = RC(0,Z), */
/*          WHERE X, Y, AND Z ARE POSITIVE AND  X * Y = Z * Z. */

/*          INTRINSIC FUNCTIONS USED: DABS,DMAX1,DSQRT */

/*          PRINTR IS THE UNIT NUMBER OF THE PRINTER. */

/*          LOLIM AND UPLIM DETERMINE THE RANGE OF VALID ARGUMENTS. */
/*          LOLIM IS NOT LESS THAN THE MACHINE MINIMUM MULTIPLIED BY 5. */
/*          UPLIM IS NOT GREATER THAN THE MACHINE MAXIMUM DIVIDED BY 5. */

/*          ACCEPTABLE VALUES FOR:   LOLIM      UPLIM */
/*          IBM 360/370 SERIES   :   3.D-78     1.D+75 */
/*          CDC 6000/7000 SERIES :   1.D-292    1.D+321 */
/*          UNIVAC 1100 SERIES   :   1.D-307    1.D+307 */

/*          WARNING: IF THIS PROGRAM IS CONVERTED TO SINGLE PRECISION, */
/*          THE VALUES FOR THE UNIVAC 1100 SERIES SHOULD BE CHANGED TO */
/*          LOLIM = 1.E-37 AND UPLIM = 1.E+37 BECAUSE THE MACHINE */
/*          EXTREMA CHANGE WITH THE PRECISION. */


/*          ON INPUT: */

/*          X AND Y ARE THE VARIABLES IN THE INTEGRAL RC(X,Y). */

/*          ERRTOL IS SET TO THE DESIRED ERROR TOLERANCE. */
/*          RELATIVE ERROR DUE TO TRUNCATION IS LESS THAN */
/*          16 * ERRTOL ** 6 / (1 - 2 * ERRTOL). */

/*          SAMPLE CHOICES:  ERRTOL   RELATIVE TRUNCATION */
/*                                    ERROR LESS THAN */
/*                           1.D-3    2.D-17 */
/*                           3.D-3    2.D-14 */
/*                           1.D-2    2.D-11 */
/*                           3.D-2    2.D-8 */
/*                           1.D-1    2.D-5 */

/*          ON OUTPUT: */

/*          X, Y, AND ERRTOL ARE UNALTERED. */

/*          IERR IS THE RETURN ERROR CODE: */
/*               IERR = 0 FOR NORMAL COMPLETION OF THE SUBROUTINE, */
/*               IERR = 1 FOR ABNORMAL TERMINATION. */

/*          ******************************************************** */
/*          WARNING: CHANGES IN THE PROGRAM MAY IMPROVE SPEED AT THE */
/*          EXPENSE OF ROBUSTNESS. */

    if (*x < 0. || *y <= 0.) {
	goto L100;
    }
    if (*x + *y < lolim) {
	goto L100;
    }
    if (max(*x,*y) <= uplim) {
	goto L112;
    }
L100:
    io___4.ciunit = printr;
    s_wsfe(&io___4);
    e_wsfe();
    io___5.ciunit = printr;
    s_wsfe(&io___5);
    do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
    e_wsfe();
    *ierr = 1;
    goto L124;

L112:
    *ierr = 0;
    xn = *x;
    yn = *y;

L116:
    mu = (xn + yn + yn) / 3.;
    sn = (yn + mu) / mu - 2.;
    if (abs(sn) < *errtol) {
	goto L120;
    }
    lamda = sqrt(xn) * 2. * sqrt(yn) + yn;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    goto L116;

L120:
    c1 = .14285714285714285;
    c2 = .40909090909090912;
    s = sn * sn * (sn * (c1 + sn * (sn * c2 + .375)) + .3);
    ret_val = (s + 1.) / sqrt(mu);

L124:
    return ret_val;
} /* rc_ */



/*     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */


/*          ******************************************************** */

doublereal rf_(doublereal *x, doublereal *y, doublereal *z__, doublereal *
	errtol, integer *ierr)
{
    /* Initialized data */

    static integer printr = 6;
    static doublereal lolim = 3e-78;
    static doublereal uplim = 1e75;

    /* Format strings */
    static char fmt_104[] = "(\0020\002,\002*** ERROR - INVALID ARGUMENTS PA"
	    "SSED TO RF\002)";
    static char fmt_108[] = "(\002 \002,\002X = \002,d23.16,4x,\002Y = \002,"
	    "d23.16,4x,\002Z = \002,d23.16)";

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal s, c1, c2, c3, e2, e3, mu, xn, yn, zn, lamda, xndev, 
	    yndev, zndev, epslon, xnroot, ynroot, znroot;

    /* Fortran I/O blocks */
    static cilist io___17 = { 0, 0, 0, fmt_104, 0 };
    static cilist io___18 = { 0, 0, 0, fmt_108, 0 };



/*          THIS FUNCTION SUBROUTINE COMPUTES THE INCOMPLETE ELLIPTIC */
/*          INTEGRAL OF THE FIRST KIND, */
/*          RF(X,Y,Z) = INTEGRAL FROM ZERO TO INFINITY OF */

/*                                -1/2     -1/2     -1/2 */
/*                      (1/2)(T+X)    (T+Y)    (T+Z)    DT, */

/*          WHERE X, Y, AND Z ARE NONNEGATIVE AND AT MOST ONE OF THEM */
/*          IS ZERO.  IF ONE OF THEM IS ZERO, THE INTEGRAL IS COMPLETE. */
/*          THE DUPLICATION THEOREM IS ITERATED UNTIL THE VARIABLES ARE */
/*          NEARLY EQUAL, AND THE FUNCTION IS THEN EXPANDED IN TAYLOR */
/*          SERIES TO FIFTH ORDER.  REFERENCE: B. C. CARLSON, COMPUTING */
/*          ELLIPTIC INTEGRALS BY DUPLICATION, NUMER. MATH. 33 (1979), */
/*          1-16.  CODED BY B. C. CARLSON AND ELAINE M. NOTIS, AMES */
/*          LABORATORY-DOE, IOWA STATE UNIVERSITY, AMES, IOWA 50011. */
/*          MARCH 1, 1980. */

/*          CHECK BY ADDITION THEOREM: RF(X,X+Z,X+W) + RF(Y,Y+Z,Y+W) */
/*          = RF(0,Z,W), WHERE X,Y,Z,W ARE POSITIVE AND X * Y = Z * W. */

/*          INTRINSIC FUNCTIONS USED: DABS,DMAX1,DMIN1,DSQRT */

/*          PRINTR IS THE UNIT NUMBER OF THE PRINTER. */

/*          LOLIM AND UPLIM DETERMINE THE RANGE OF VALID ARGUMENTS. */
/*          LOLIM IS NOT LESS THAN THE MACHINE MINIMUM MULTIPLIED BY 5. */
/*          UPLIM IS NOT GREATER THAN THE MACHINE MAXIMUM DIVIDED BY 5. */

/*          ACCEPTABLE VALUES FOR:   LOLIM      UPLIM */
/*          IBM 360/370 SERIES   :   3.D-78     1.D+75 */
/*          CDC 6000/7000 SERIES :   1.D-292    1.D+321 */
/*          UNIVAC 1100 SERIES   :   1.D-307    1.D+307 */

/*          WARNING: IF THIS PROGRAM IS CONVERTED TO SINGLE PRECISION, */
/*          THE VALUES FOR THE UNIVAC 1100 SERIES SHOULD BE CHANGED TO */
/*          LOLIM = 1.E-37 AND UPLIM = 1.E+37 BECAUSE THE MACHINE */
/*          EXTREMA CHANGE WITH THE PRECISION. */


/*          ON INPUT: */

/*          X, Y, AND Z ARE THE VARIABLES IN THE INTEGRAL RF(X,Y,Z). */

/*          ERRTOL IS SET TO THE DESIRED ERROR TOLERANCE. */
/*          RELATIVE ERROR DUE TO TRUNCATION IS LESS THAN */
/*          ERRTOL ** 6 / (4 * (1 - ERRTOL)). */

/*          SAMPLE CHOICES:  ERRTOL   RELATIVE TRUNCATION */
/*                                    ERROR LESS THAN */
/*                           1.D-3    3.D-19 */
/*                           3.D-3    2.D-16 */
/*                           1.D-2    3.D-13 */
/*                           3.D-2    2.D-10 */
/*                           1.D-1    3.D-7 */

/*          ON OUTPUT: */

/*          X, Y, Z, AND ERRTOL ARE UNALTERED. */

/*          IERR IS THE RETURN ERROR CODE: */
/*               IERR = 0 FOR NORMAL COMPLETION OF THE SUBROUTINE, */
/*               IERR = 1 FOR ABNORMAL TERMINATION. */

/*          ******************************************************** */
/*          WARNING: CHANGES IN THE PROGRAM MAY IMPROVE SPEED AT THE */
/*          EXPENSE OF ROBUSTNESS. */

/* Computing MIN */
    d__1 = min(*x,*y);
    if (min(d__1,*z__) < 0.) {
	goto L100;
    }
/* Computing MIN */
    d__1 = *x + *y, d__2 = *x + *z__, d__1 = min(d__1,d__2), d__2 = *y + *z__;
    if (min(d__1,d__2) < lolim) {
	goto L100;
    }
/* Computing MAX */
    d__1 = max(*x,*y);
    if (max(d__1,*z__) <= uplim) {
	goto L112;
    }
L100:
    io___17.ciunit = printr;
    s_wsfe(&io___17);
    e_wsfe();
    io___18.ciunit = printr;
    s_wsfe(&io___18);
    do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
    e_wsfe();
    *ierr = 1;
    goto L124;

L112:
    *ierr = 0;
    xn = *x;
    yn = *y;
    zn = *z__;

L116:
    mu = (xn + yn + zn) / 3.;
    xndev = 2. - (mu + xn) / mu;
    yndev = 2. - (mu + yn) / mu;
    zndev = 2. - (mu + zn) / mu;
/* Computing MAX */
    d__1 = abs(xndev), d__2 = abs(yndev), d__1 = max(d__1,d__2), d__2 = abs(
	    zndev);
    epslon = max(d__1,d__2);
    if (epslon < *errtol) {
	goto L120;
    }
    xnroot = sqrt(xn);
    ynroot = sqrt(yn);
    znroot = sqrt(zn);
    lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    zn = (zn + lamda) * .25;
    goto L116;

L120:
    c1 = .041666666666666664;
    c2 = .068181818181818177;
    c3 = .071428571428571425;
    e2 = xndev * yndev - zndev * zndev;
    e3 = xndev * yndev * zndev;
    s = (c1 * e2 - .1 - c2 * e3) * e2 + 1. + c3 * e3;
    ret_val = s / sqrt(mu);

L124:
    return ret_val;
} /* rf_ */



/*     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */


/*          ******************************************************** */

doublereal rd_(doublereal *x, doublereal *y, doublereal *z__, doublereal *
	errtol, integer *ierr)
{
    /* Initialized data */

    static integer printr = 6;
    static doublereal lolim = 6e-51;
    static doublereal uplim = 1e48;

    /* Format strings */
    static char fmt_104[] = "(\0020\002,\002*** ERROR - INVALID ARGUMENTS PA"
	    "SSED TO RD\002)";
    static char fmt_108[] = "(\002 \002,\002X = \002,d23.16,4x,\002Y = \002,"
	    "d23.16,4x,\002Z = \002,d23.16)";

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal c1, c2, c3, c4, s1, s2, ea, eb, ec, ed, ef, mu, xn, yn, 
	    zn, lamda, sigma, xndev, yndev, zndev, power4, epslon, xnroot, 
	    ynroot, znroot;

    /* Fortran I/O blocks */
    static cilist io___40 = { 0, 0, 0, fmt_104, 0 };
    static cilist io___41 = { 0, 0, 0, fmt_108, 0 };



/*          THIS FUNCTION SUBROUTINE COMPUTES AN INCOMPLETE ELLIPTIC */
/*          INTEGRAL OF THE SECOND KIND, */
/*          RD(X,Y,Z) = INTEGRAL FROM ZERO TO INFINITY OF */

/*                                -1/2     -1/2     -3/2 */
/*                      (3/2)(T+X)    (T+Y)    (T+Z)    DT, */

/*          WHERE X AND Y ARE NONNEGATIVE, X + Y IS POSITIVE, AND Z IS */
/*          POSITIVE.  IF X OR Y IS ZERO, THE INTEGRAL IS COMPLETE. */
/*          THE DUPLICATION THEOREM IS ITERATED UNTIL THE VARIABLES ARE */
/*          NEARLY EQUAL, AND THE FUNCTION IS THEN EXPANDED IN TAYLOR */
/*          SERIES TO FIFTH ORDER.  REFERENCE: B. C. CARLSON, COMPUTING */
/*          ELLIPTIC INTEGRALS BY DUPLICATION, NUMER. MATH. 33 (1979), */
/*          1-16.  CODED BY B. C. CARLSON AND ELAINE M. NOTIS, AMES */
/*          LABORATORY-DOE, IOWA STATE UNIVERSITY, AMES, IOWA 50011. */
/*          MARCH 1, 1980.. */

/*          CHECK: RD(X,Y,Z) + RD(Y,Z,X) + RD(Z,X,Y) */
/*          = 3 / DSQRT(X * Y * Z), WHERE X, Y, AND Z ARE POSITIVE. */

/*          INTRINSIC FUNCTIONS USED: DABS,DMAX1,DMIN1,DSQRT */

/*          PRINTR IS THE UNIT NUMBER OF THE PRINTER. */

/*          LOLIM AND UPLIM DETERMINE THE RANGE OF VALID ARGUMENTS. */
/*          LOLIM IS NOT LESS THAN 2 / (MACHINE MAXIMUM) ** (2/3). */
/*          UPLIM IS NOT GREATER THAN (0.1 * ERRTOL / MACHINE */
/*          MINIMUM) ** (2/3), WHERE ERRTOL IS DESCRIBED BELOW. */
/*          IN THE FOLLOWING TABLE IT IS ASSUMED THAT ERRTOL WILL */
/*          NEVER BE CHOSEN SMALLER THAN 1.D-5. */

/*          ACCEPTABLE VALUES FOR:   LOLIM      UPLIM */
/*          IBM 360/370 SERIES   :   6.D-51     1.D+48 */
/*          CDC 6000/7000 SERIES :   5.D-215    2.D+191 */
/*          UNIVAC 1100 SERIES   :   1.D-205    2.D+201 */

/*          WARNING: IF THIS PROGRAM IS CONVERTED TO SINGLE PRECISION, */
/*          THE VALUES FOR THE UNIVAC 1100 SERIES SHOULD BE CHANGED TO */
/*          LOLIM = 1.E-25 AND UPLIM = 2.E+21 BECAUSE THE MACHINE */
/*          EXTREMA CHANGE WITH THE PRECISION. */


/*          ON INPUT: */

/*          X, Y, AND Z ARE THE VARIABLES IN THE INTEGRAL RD(X,Y,Z). */

/*          ERRTOL IS SET TO THE DESIRED ERROR TOLERANCE. */
/*          RELATIVE ERROR DUE TO TRUNCATION IS LESS THAN */
/*          3 * ERRTOL ** 6 / (1-ERRTOL) ** 3/2. */

/*          SAMPLE CHOICES:  ERRTOL   RELATIVE TRUNCATION */
/*                                    ERROR LESS THAN */
/*                           1.D-3    4.D-18 */
/*                           3.D-3    3.D-15 */
/*                           1.D-2    4.D-12 */
/*                           3.D-2    3.D-9 */
/*                           1.D-1    4.D-6 */

/*          ON OUTPUT: */

/*          X, Y, Z, AND ERRTOL ARE UNALTERED. */

/*          IERR IS THE RETURN ERROR CODE: */
/*               IERR = 0 FOR NORMAL COMPLETION OF THE SUBROUTINE, */
/*               IERR = 1 FOR ABNORMAL TERMINATION. */

/*          ******************************************************** */
/*          WARNING: CHANGES IN THE PROGRAM MAY IMPROVE SPEED AT THE */
/*          EXPENSE OF ROBUSTNESS. */

    if (min(*x,*y) < 0.) {
	goto L100;
    }
/* Computing MIN */
    d__1 = *x + *y;
    if (min(d__1,*z__) < lolim) {
	goto L100;
    }
/* Computing MAX */
    d__1 = max(*x,*y);
    if (max(d__1,*z__) <= uplim) {
	goto L112;
    }
L100:
    io___40.ciunit = printr;
    s_wsfe(&io___40);
    e_wsfe();
    io___41.ciunit = printr;
    s_wsfe(&io___41);
    do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
    e_wsfe();
    *ierr = 1;
    goto L124;

L112:
    *ierr = 0;
    xn = *x;
    yn = *y;
    zn = *z__;
    sigma = 0.;
    power4 = 1.;

L116:
    mu = (xn + yn + zn * 3.) * .2;
    xndev = (mu - xn) / mu;
    yndev = (mu - yn) / mu;
    zndev = (mu - zn) / mu;
/* Computing MAX */
    d__1 = abs(xndev), d__2 = abs(yndev), d__1 = max(d__1,d__2), d__2 = abs(
	    zndev);
    epslon = max(d__1,d__2);
    if (epslon < *errtol) {
	goto L120;
    }
    xnroot = sqrt(xn);
    ynroot = sqrt(yn);
    znroot = sqrt(zn);
    lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
    sigma += power4 / (znroot * (zn + lamda));
    power4 *= .25;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    zn = (zn + lamda) * .25;
    goto L116;

L120:
    c1 = .21428571428571427;
    c2 = .16666666666666666;
    c3 = .40909090909090912;
    c4 = .11538461538461539;
    ea = xndev * yndev;
    eb = zndev * zndev;
    ec = ea - eb;
    ed = ea - eb * 6.;
    ef = ed + ec + ec;
    s1 = ed * (-c1 + c3 * .25 * ed - c4 * 1.5 * zndev * ef);
    s2 = zndev * (c2 * ef + zndev * (-c3 * ec + zndev * c4 * ea));
    ret_val = sigma * 3. + power4 * (s1 + 1. + s2) / (mu * sqrt(mu));

L124:
    return ret_val;
} /* rd_ */



/*     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */


/*          ******************************************************** */

doublereal rj_(doublereal *x, doublereal *y, doublereal *z__, doublereal *p, 
	doublereal *errtol, integer *ierr)
{
    /* Initialized data */

    static integer printr = 6;
    static doublereal lolim = 2e-26;
    static doublereal uplim = 3e24;

    /* Format strings */
    static char fmt_104[] = "(\0020\002,\002*** ERROR - INVALID ARGUMENTS PA"
	    "SSED TO RJ\002)";
    static char fmt_108[] = "(\002 \002,\002X = \002,d23.16,4x,\002Y = \002,"
	    "d23.16,4x,\002Z = \002,d23.16,4x,\002P = \002,d23.16)";

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal c1, c2, c3, c4, e2, e3, s1, s2, s3, ea, eb, ec;
    extern doublereal rc_(doublereal *, doublereal *, doublereal *, integer *)
	    ;
    static doublereal pn, mu, xn, yn, zn, alfa, beta, lamda, sigma, pndev, 
	    xndev, yndev, zndev, power4, etolrc, epslon, xnroot, ynroot, 
	    znroot;

    /* Fortran I/O blocks */
    static cilist io___70 = { 0, 0, 0, fmt_104, 0 };
    static cilist io___71 = { 0, 0, 0, fmt_108, 0 };



/*          THIS FUNCTION SUBROUTINE COMPUTES AN INCOMPLETE ELLIPTIC */
/*          INTEGRAL OF THE THIRD KIND, */
/*          RJ(X,Y,Z,P) = INTEGRAL FROM ZERO TO INFINITY OF */

/*                                  -1/2     -1/2     -1/2     -1 */
/*                        (3/2)(T+X)    (T+Y)    (T+Z)    (T+P)  DT, */

/*          WHERE X, Y, AND Z ARE NONNEGATIVE, AT MOST ONE OF THEM IS */
/*          ZERO, AND P IS POSITIVE.  IF X OR Y OR Z IS ZERO, THE */
/*          INTEGRAL IS COMPLETE.  THE DUPLICATION THEOREM IS ITERATED */
/*          UNTIL THE VARIABLES ARE NEARLY EQUAL, AND THE FUNCTION IS */
/*          THEN EXPANDED IN TAYLOR SERIES TO FIFTH ORDER.  REFERENCE: */
/*          B. C. CARLSON, COMPUTING ELLIPTIC INTEGRALS BY DUPLICATION, */
/*          NUMER. MATH. 33 (1979), 1-16.  CODED BY B. C. CARLSON AND */
/*          ELAINE M. NOTIS, AMES LABORATORY-DOE, IOWA STATE UNIVERSITY, */
/*          AMES, IOWA 50011.  MARCH 1, 1980. */

/*          CHECK BY ADDITION THEOREM: RJ(X,X+Z,X+W,X+P) */
/*          + RJ(Y,Y+Z,Y+W,Y+P) + (A-B) * RJ(A,B,B,A) + 3 / DSQRT(A) */
/*          = RJ(0,Z,W,P), WHERE X,Y,Z,W,P ARE POSITIVE AND X * Y */
/*          = Z * W,  A = P * P * (X+Y+Z+W),  B = P * (P+X) * (P+Y), */
/*          AND B - A = P * (P-Z) * (P-W).  THE SUM OF THE THIRD AND */
/*          FOURTH TERMS ON THE LEFT SIDE IS 3 * RC(A,B). */

/*          INTRINSIC FUNCTIONS USED: DABS,DMAX1,DMIN1,DSQRT */

/*          PRINTR IS THE UNIT NUMBER OF THE PRINTER. */

/*          RC IS A FUNCTION COMPUTED BY AN EXTERNAL SUBROUTINE. */

/*          LOLIM AND UPLIM DETERMINE THE RANGE OF VALID ARGUMENTS. */
/*          LOLIM IS NOT LESS THAN THE CUBE ROOT OF THE VALUE */
/*          OF LOLIM USED IN THE SUBROUTINE FOR RC. */
/*          UPLIM IS NOT GREATER THAN 0.3 TIMES THE CUBE ROOT OF */
/*          THE VALUE OF UPLIM USED IN THE SUBROUTINE FOR RC. */

/*          ACCEPTABLE VALUES FOR:   LOLIM      UPLIM */
/*          IBM 360/370 SERIES   :   2.D-26     3.D+24 */
/*          CDC 6000/7000 SERIES :   5.D-98     3.D+106 */
/*          UNIVAC 1100 SERIES   :   5.D-103    6.D+101 */

/*          WARNING: IF THIS PROGRAM IS CONVERTED TO SINGLE PRECISION, */
/*          THE VALUES FOR THE UNIVAC 1100 SERIES SHOULD BE CHANGED TO */
/*          LOLIM = 5.E-13 AND UPLIM = 6.E+11 BECAUSE THE MACHINE */
/*          EXTREMA CHANGE WITH THE PRECISION. */


/*          ON INPUT: */

/*          X, Y, Z, AND P ARE THE VARIABLES IN THE INTEGRAL RJ(X,Y,Z,P). */

/*          ERRTOL IS SET TO THE DESIRED ERROR TOLERANCE. */
/*          RELATIVE ERROR DUE TO TRUNCATION OF THE SERIES FOR RJ */
/*          IS LESS THAN 3 * ERRTOL ** 6 / (1 - ERRTOL) ** 3/2. */
/*          AN ERROR TOLERANCE (ETOLRC) WILL BE PASSED TO THE SUBROUTINE */
/*          FOR RC TO MAKE THE TRUNCATION ERROR FOR RC LESS THAN FOR RJ. */

/*          SAMPLE CHOICES:  ERRTOL   RELATIVE TRUNCATION */
/*                                    ERROR LESS THAN */
/*                           1.D-3    4.D-18 */
/*                           3.D-3    3.D-15 */
/*                           1.D-2    4.D-12 */
/*                           3.D-2    3.D-9 */
/*                           1.D-1    4.D-6 */

/*          ON OUTPUT: */

/*          X, Y, Z, P, AND ERRTOL ARE UNALTERED. */

/*          IERR IS THE RETURN ERROR CODE: */
/*               IERR = 0 FOR NORMAL COMPLETION OF THE SUBROUTINE, */
/*               IERR = 1 FOR ABNORMAL TERMINATION. */

/*          ******************************************************** */
/*          WARNING: CHANGES IN THE PROGRAM MAY IMPROVE SPEED AT THE */
/*          EXPENSE OF ROBUSTNESS. */

/* Computing MIN */
    d__1 = min(*x,*y);
    if (min(d__1,*z__) < 0.) {
	goto L100;
    }
/* Computing MIN */
    d__1 = *x + *y, d__2 = *x + *z__, d__1 = min(d__1,d__2), d__2 = *y + *z__,
	     d__1 = min(d__1,d__2);
    if (min(d__1,*p) < lolim) {
	goto L100;
    }
/* Computing MAX */
    d__1 = max(*x,*y), d__1 = max(d__1,*z__);
    if (max(d__1,*p) <= uplim) {
	goto L112;
    }
L100:
    io___70.ciunit = printr;
    s_wsfe(&io___70);
    e_wsfe();
    io___71.ciunit = printr;
    s_wsfe(&io___71);
    do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*p), (ftnlen)sizeof(doublereal));
    e_wsfe();
    *ierr = 1;
    goto L124;

L112:
    *ierr = 0;
    xn = *x;
    yn = *y;
    zn = *z__;
    pn = *p;
    sigma = 0.;
    power4 = 1.;
    etolrc = *errtol * .5;

L116:
    mu = (xn + yn + zn + pn + pn) * .2;
    xndev = (mu - xn) / mu;
    yndev = (mu - yn) / mu;
    zndev = (mu - zn) / mu;
    pndev = (mu - pn) / mu;
/* Computing MAX */
    d__1 = abs(xndev), d__2 = abs(yndev), d__1 = max(d__1,d__2), d__2 = abs(
	    zndev), d__1 = max(d__1,d__2), d__2 = abs(pndev);
    epslon = max(d__1,d__2);
    if (epslon < *errtol) {
	goto L120;
    }
    xnroot = sqrt(xn);
    ynroot = sqrt(yn);
    znroot = sqrt(zn);
    lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
    alfa = pn * (xnroot + ynroot + znroot) + xnroot * ynroot * znroot;
    alfa *= alfa;
    beta = pn * (pn + lamda) * (pn + lamda);
    sigma += power4 * rc_(&alfa, &beta, &etolrc, ierr);
    if (*ierr != 0) {
	goto L124;
    }
    power4 *= .25;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    zn = (zn + lamda) * .25;
    pn = (pn + lamda) * .25;
    goto L116;

L120:
    c1 = .21428571428571427;
    c2 = .33333333333333331;
    c3 = .13636363636363635;
    c4 = .11538461538461539;
    ea = xndev * (yndev + zndev) + yndev * zndev;
    eb = xndev * yndev * zndev;
    ec = pndev * pndev;
    e2 = ea - ec * 3.;
    e3 = eb + pndev * 2. * (ea - ec);
    s1 = e2 * (-c1 + c3 * .75 * e2 - c4 * 1.5 * e3) + 1.;
    s2 = eb * (c2 * .5 + pndev * (-c3 - c3 + pndev * c4));
    s3 = pndev * ea * (c2 - pndev * c3) - c2 * pndev * ec;
    ret_val = sigma * 3. + power4 * (s1 + s2 + s3) / (mu * sqrt(mu));

L124:
    return ret_val;
} /* rj_ */

}

/* drd.f -- translated by f2c (version 20020621).
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
static integer c__2 = 2;
static doublereal c_b5 = .66666666666666663;
static integer c__1 = 1;
static doublereal c_b7 = .33333333333333331;
static doublereal c_b9 = 2.;
static integer c__4 = 4;
static integer c__8 = 8;

/* DECK DRD */
doublereal drd_(doublereal *x, doublereal *y, doublereal *z__, integer *ier)
{
    /* Initialized data */

    static logical first = TRUE_;

    /* System generated locals */
    address a__1[4], a__2[8];
    integer i__1[4], i__2[8];
    doublereal ret_val, d__1, d__2;
    char ch__1[65], ch__2[117];

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    ;
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal c1, c2, c3, c4, s1, s2, ea, eb, ec, ed, ef, mu, xn, yn, 
	    zn;
    static char xern3[16], xern4[16], xern5[16], xern6[16];
    static doublereal lamda, sigma, lolim, xndev, yndev, uplim, zndev;
    extern doublereal d1mach_(integer *);
    static doublereal power4, epslon;
    extern /* Subroutine */ int xermsg_(char *, char *, char *, integer *, 
	    integer *, ftnlen, ftnlen, ftnlen);
    static doublereal errtol, tuplim, xnroot, ynroot, znroot;

    /* Fortran I/O blocks */
    static icilist io___11 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___13 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___14 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___15 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___17 = { 0, xern5, 0, "(1PE15.6)", 16, 1 };
    static icilist io___19 = { 0, xern6, 0, "(1PE15.6)", 16, 1 };
    static icilist io___20 = { 0, xern3, 0, "(1PE15.6)", 16, 1 };
    static icilist io___21 = { 0, xern4, 0, "(1PE15.6)", 16, 1 };
    static icilist io___22 = { 0, xern5, 0, "(1PE15.6)", 16, 1 };
    static icilist io___23 = { 0, xern6, 0, "(1PE15.6)", 16, 1 };


/* ***BEGIN PROLOGUE  DRD */
/* ***PURPOSE  Compute the incomplete or complete elliptic integral of */
/*            the 2nd kind. For X and Y nonnegative, X+Y and Z positive, */
/*            DRD(X,Y,Z) = Integral from zero to infinity of */
/*                                -1/2     -1/2     -3/2 */
/*                      (3/2)(t+X)    (t+Y)    (t+Z)    dt. */
/*            If X or Y is zero, the integral is complete. */
/* ***LIBRARY   SLATEC */
/* ***CATEGORY  C14 */
/* ***TYPE      DOUBLE PRECISION (RD-S, DRD-D) */
/* ***KEYWORDS  COMPLETE ELLIPTIC INTEGRAL, DUPLICATION THEOREM, */
/*             INCOMPLETE ELLIPTIC INTEGRAL, INTEGRAL OF THE SECOND KIND, */
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

/*   1.     DRD */
/*          Evaluate an INCOMPLETE (or COMPLETE) ELLIPTIC INTEGRAL */
/*          of the second kind */
/*          Standard FORTRAN function routine */
/*          Double precision version */
/*          The routine calculates an approximation result to */
/*          DRD(X,Y,Z) = Integral from zero to infinity of */
/*                              -1/2     -1/2     -3/2 */
/*                    (3/2)(t+X)    (t+Y)    (t+Z)    dt, */
/*          where X and Y are nonnegative, X + Y is positive, and Z is */
/*          positive.  If X or Y is zero, the integral is COMPLETE. */
/*          The duplication theorem is iterated until the variables are */
/*          nearly equal, and the function is then expanded in Taylor */
/*          series to fifth order. */

/*   2.     Calling Sequence */

/*          DRD( X, Y, Z, IER ) */

/*          Parameters On Entry */
/*          Values assigned by the calling routine */

/*          X      - Double precision, nonnegative variable */

/*          Y      - Double precision, nonnegative variable */

/*                   X + Y is positive */

/*          Z      - Double precision, positive variable */



/*          On Return    (values assigned by the DRD routine) */

/*          DRD     - Double precision approximation to the integral */


/*          IER    - Integer */

/*                   IER = 0 Normal and reliable termination of the */
/*                           routine. It is assumed that the requested */
/*                           accuracy has been achieved. */

/*                   IER >  0 Abnormal termination of the routine */


/*          X, Y, Z are unaltered. */

/*   3.    Error Messages */

/*         Value of IER assigned by the DRD routine */

/*                  Value assigned         Error message printed */
/*                  IER = 1                MIN(X,Y) .LT. 0.0D0 */
/*                      = 2                MIN(X + Y, Z ) .LT. LOLIM */
/*                      = 3                MAX(X,Y,Z) .GT. UPLIM */


/*   4.     Control Parameters */

/*                  Values of LOLIM, UPLIM, and ERRTOL are set by the */
/*                  routine. */

/*          LOLIM and UPLIM determine the valid range of X, Y, and Z */

/*          LOLIM  - Lower limit of valid arguments */

/*                    Not less  than 2 / (machine maximum) ** (2/3). */

/*          UPLIM  - Upper limit of valid arguments */

/*                 Not greater than (0.1D0 * ERRTOL / machine */
/*                 minimum) ** (2/3), where ERRTOL is described below. */
/*                 In the following table it is assumed that ERRTOL will */
/*                 never be chosen smaller than 1.0D-5. */


/*                    Acceptable values for:   LOLIM      UPLIM */
/*                    IBM 360/370 SERIES   :   6.0D-51     1.0D+48 */
/*                    CDC 6000/7000 SERIES :   5.0D-215    2.0D+191 */
/*                    UNIVAC 1100 SERIES   :   1.0D-205    2.0D+201 */
/*                    CRAY                 :   3.0D-1644   1.69D+1640 */
/*                    VAX 11 SERIES        :   1.0D-25     4.5D+21 */


/*          ERRTOL determines the accuracy of the answer */

/*                 The value assigned by the routine will result */
/*                 in solution precision within 1-2 decimals of */
/*                 "machine precision". */

/*          ERRTOL    Relative error due to truncation is less than */
/*                    3 * ERRTOL ** 6 / (1-ERRTOL) ** 3/2. */



/*        The accuracy of the computed approximation to the integral */
/*        can be controlled by choosing the value of ERRTOL. */
/*        Truncation of a Taylor series after terms of fifth order */
/*        introduces an error less than the amount shown in the */
/*        second column of the following table for each value of */
/*        ERRTOL in the first column.  In addition to the truncation */
/*        error there will be round-off error, but in practice the */
/*        total error from both sources is usually less than the */
/*        amount given in the table. */




/*          Sample choices:  ERRTOL   Relative truncation */
/*                                    error less than */
/*                           1.0D-3    4.0D-18 */
/*                           3.0D-3    3.0D-15 */
/*                           1.0D-2    4.0D-12 */
/*                           3.0D-2    3.0D-9 */
/*                           1.0D-1    4.0D-6 */


/*                    Decreasing ERRTOL by a factor of 10 yields six more */
/*                    decimal digits of accuracy at the expense of one or */
/*                    two more iterations of the duplication theorem. */

/* *Long Description: */

/*   DRD Special Comments */



/*          Check: DRD(X,Y,Z) + DRD(Y,Z,X) + DRD(Z,X,Y) */
/*          = 3 / SQRT(X * Y * Z), where X, Y, and Z are positive. */


/*          On Input: */

/*          X, Y, and Z are the variables in the integral DRD(X,Y,Z). */


/*          On Output: */


/*          X, Y, Z are unaltered. */



/*          ******************************************************** */

/*          WARNING: Changes in the program may improve speed at the */
/*                   expense of robustness. */



/*    ------------------------------------------------------------------- */


/*   Special double precision functions via DRD and DRF */


/*                  Legendre form of ELLIPTIC INTEGRAL of 2nd kind */

/*                  ----------------------------------------- */


/*                                             2         2   2 */
/*                  E(PHI,K) = SIN(PHI) DRF(COS (PHI),1-K SIN (PHI),1) - */

/*                     2      3             2         2   2 */
/*                  -(K/3) SIN (PHI) DRD(COS (PHI),1-K SIN (PHI),1) */


/*                                  2        2            2 */
/*                  E(K) = DRF(0,1-K ,1) - (K/3) DRD(0,1-K ,1) */

/*                         PI/2     2   2      1/2 */
/*                       = INT  (1-K SIN (PHI) )  D PHI */
/*                          0 */

/*                  Bulirsch form of ELLIPTIC INTEGRAL of 2nd kind */

/*                  ----------------------------------------- */

/*                                               2 2    2 */
/*                  EL2(X,KC,A,B) = AX DRF(1,1+KC X ,1+X ) + */

/*                                              3          2 2    2 */
/*                                 +(1/3)(B-A) X DRD(1,1+KC X ,1+X ) */




/*                  Legendre form of alternative ELLIPTIC INTEGRAL */
/*                  of 2nd kind */

/*                  ----------------------------------------- */



/*                            Q     2       2   2  -1/2 */
/*                  D(Q,K) = INT SIN P  (1-K SIN P)     DP */
/*                            0 */



/*                                     3          2     2   2 */
/*                  D(Q,K) = (1/3) (SIN Q) DRD(COS Q,1-K SIN Q,1) */




/*                  Lemniscate constant  B */

/*                  ----------------------------------------- */




/*                       1    2    4 -1/2 */
/*                  B = INT  S (1-S )    DS */
/*                       0 */


/*                  B = (1/3) DRD (0,2,1) */


/*                  Heuman's LAMBDA function */

/*                  ----------------------------------------- */



/*                  (PI/2) LAMBDA0(A,B) = */

/*                                    2                2 */
/*                 = SIN(B) (DRF(0,COS (A),1)-(1/3) SIN (A) * */

/*                            2               2         2       2 */
/*                  *DRD(0,COS (A),1)) DRF(COS (B),1-COS (A) SIN (B),1) */

/*                            2       3             2 */
/*                  -(1/3) COS (A) SIN (B) DRF(0,COS (A),1) * */

/*                           2         2       2 */
/*                   *DRD(COS (B),1-COS (A) SIN (B),1) */



/*                  Jacobi ZETA function */

/*                  ----------------------------------------- */

/*                             2                 2       2   2 */
/*                  Z(B,K) = (K/3) SIN(B) DRF(COS (B),1-K SIN (B),1) */


/*                                       2             2 */
/*                             *DRD(0,1-K ,1)/DRF(0,1-K ,1) */

/*                               2       3           2       2   2 */
/*                            -(K /3) SIN (B) DRD(COS (B),1-K SIN (B),1) */


/* --------------------------------------------------------------------- */

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
/*   890531  REVISION DATE from Version 3.2 */
/*   891214  Prologue converted to Version 4.0 format.  (BAB) */
/*   900315  CALLs to XERROR changed to CALLs to XERMSG.  (THJ) */
/*   900326  Removed duplicate information from DESCRIPTION section. */
/*           (WRB) */
/*   900510  Modify calls to XERMSG to put in standard form.  (RWC) */
/*   920501  Reformatted the REFERENCES section.  (WRB) */
/* ***END PROLOGUE  DRD */

/* ***FIRST EXECUTABLE STATEMENT  DRD */
    if (first) {
	d__1 = d1mach_(&c__3) / 3.;
	errtol = pow_dd(&d__1, &c_b3);
	d__1 = d1mach_(&c__2);
	lolim = 2. / pow_dd(&d__1, &c_b5);
	d__1 = d1mach_(&c__1);
	tuplim = pow_dd(&d__1, &c_b7);
	d__1 = errtol * .1;
	tuplim = pow_dd(&d__1, &c_b7) / tuplim;
	uplim = pow_dd(&tuplim, &c_b9);

	c1 = .21428571428571427;
	c2 = .16666666666666666;
	c3 = .40909090909090912;
	c4 = .11538461538461539;
    }
    first = FALSE_;

/*         CALL ERROR HANDLER IF NECESSARY. */

    ret_val = 0.;
    if (min(*x,*y) < 0.) {
	*ier = 1;
	s_wsfi(&io___11);
	do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___13);
	do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
	e_wsfi();
/* Writing concatenation */
	i__1[0] = 24, a__1[0] = "MIN(X,Y).LT.0 WHERE X = ";
	i__1[1] = 16, a__1[1] = xern3;
	i__1[2] = 9, a__1[2] = " AND Y = ";
	i__1[3] = 16, a__1[3] = xern4;
	s_cat(ch__1, a__1, i__1, &c__4, (ftnlen)65);
	xermsg_("SLATEC", "DRD", ch__1, &c__1, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)65);
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
	s_wsfi(&io___17);
	do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___19);
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
	xermsg_("SLATEC", "DRD", ch__2, &c__3, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)117);
	return ret_val;
    }

/* Computing MIN */
    d__1 = *x + *y;
    if (min(d__1,*z__) < lolim) {
	*ier = 2;
	s_wsfi(&io___20);
	do_fio(&c__1, (char *)&(*x), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___21);
	do_fio(&c__1, (char *)&(*y), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___22);
	do_fio(&c__1, (char *)&(*z__), (ftnlen)sizeof(doublereal));
	e_wsfi();
	s_wsfi(&io___23);
	do_fio(&c__1, (char *)&lolim, (ftnlen)sizeof(doublereal));
	e_wsfi();
/* Writing concatenation */
	i__2[0] = 30, a__2[0] = "MIN(X+Y,Z).LT.LOLIM WHERE X = ";
	i__2[1] = 16, a__2[1] = xern3;
	i__2[2] = 5, a__2[2] = " Y = ";
	i__2[3] = 16, a__2[3] = xern4;
	i__2[4] = 5, a__2[4] = " Z = ";
	i__2[5] = 16, a__2[5] = xern5;
	i__2[6] = 13, a__2[6] = " AND LOLIM = ";
	i__2[7] = 16, a__2[7] = xern6;
	s_cat(ch__2, a__2, i__2, &c__8, (ftnlen)117);
	xermsg_("SLATEC", "DRD", ch__2, &c__2, &c__1, (ftnlen)6, (ftnlen)3, (
		ftnlen)117);
	return ret_val;
    }

    *ier = 0;
    xn = *x;
    yn = *y;
    zn = *z__;
    sigma = 0.;
    power4 = 1.;

L30:
    mu = (xn + yn + zn * 3.) * .2;
    xndev = (mu - xn) / mu;
    yndev = (mu - yn) / mu;
    zndev = (mu - zn) / mu;
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
    sigma += power4 / (znroot * (zn + lamda));
    power4 *= .25;
    xn = (xn + lamda) * .25;
    yn = (yn + lamda) * .25;
    zn = (zn + lamda) * .25;
    goto L30;

L40:
    ea = xndev * yndev;
    eb = zndev * zndev;
    ec = ea - eb;
    ed = ea - eb * 6.;
    ef = ed + ec + ec;
    s1 = ed * (-c1 + c3 * .25 * ed - c4 * 1.5 * zndev * ef);
    s2 = zndev * (c2 * ef + zndev * (-c3 * ec + zndev * c4 * ea));
    ret_val = sigma * 3. + power4 * (s1 + 1. + s2) / (mu * sqrt(mu));

    return ret_val;
} /* drd_ */

}

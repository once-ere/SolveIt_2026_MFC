/* sledge.f -- translated by f2c (version 20020621).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

//#pragma optimize( "", off )

#include "stdafx.h"
#include "SolveIt.h"

extern "C" long _stdcall COEFF(double *x, double *px, double *qx, double *rx);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


//#include "f2c.h"
#include <f2c/f2c.h>

/* Common Block Declarations */

union {
    struct {
	doublereal cp[2], cr[2], cutoff, d__[8]	/* was [4][2] */, emu[2], ep[
		2], eqlnf[2], er[2], eta[4]	/* was [2][2] */, pnu[2];
	integer kclass[2];
    } _1;
    struct {
	doublereal cpt[2], crt[2], cutoff, d__[8]	/* was [4][2] */, emu[
		2], ept[2], eqlnf[2], ert[2], eta[4]	/* was [2][2] */, pnu[
		2];
	integer kclass[2];
    } _2;
    struct {
	doublereal cp[2], cr[2], cutoff, d__[8]	/* was [4][2] */, emu[2], ep[
		2], eqlnf[2], er[2], etat[4]	/* was [2][2] */, pnu[2];
	integer kclass[2];
    } _3;
} slclss_;

#define slclss_1 (slclss_._1)
#define slclss_2 (slclss_._2)
#define slclss_3 (slclss_._3)

struct {
    integer flag__, level, maxext, maxint, maxlvl, ncoeff, nsgnf, nxinit;
} slint_;

#define slint_1 slint_

struct {
    logical afin, bfin, countz, lflag[6], lnf, lc[2], osc[2], reg[2];
} sllog_;

#define sllog_1 sllog_

union {
    struct {
	doublereal a1, a1p, a2, a2p, b1, b2, a, b, u, under;
    } _1;
    struct {
	doublereal a1, a1p, a2, a2p, b1, b2, a, b, urn, under;
    } _2;
} slreal_;

#define slreal_1 (slreal_._1)
#define slreal_2 (slreal_._2)

/* Table of constant values */

static doublereal c_b8 = 1.;
static integer c_n1 = -1;
static integer c__1 = 1;
static logical c_true = TRUE_;
static integer c__0 = 0;
static doublereal c_b123 = .5;
static doublereal c_b133 = 5.;
static logical c_false = FALSE_;
static integer c__2 = 2;
static integer c__3 = 3;
static integer c__4 = 4;
static integer c__5 = 5;
static integer c__6 = 6;
static integer c__7 = 7;
static integer c__8 = 8;
static integer c__9 = 9;
static integer c__10 = 10;
static doublereal c_b453 = .1;
static doublereal c_b455 = .4;
static doublereal c_b457 = .333;
static real c_b458 = (float).1;

/* ********************************************************************** */
/*                                                                     * */
/*                     Routines for SLEDGE                             * */
/*     (Sturm-Liouville Estimates Determined by Global Errors)         * */
/*                                                                     * */
/* ********************************************************************** */

/*     Release 2.2     04/12/93 */

/*     Steven Pruess,  Colorado School of Mines */
/*                     spruess@mines.colorado.edu */
/*     Charles Fulton, Florida Institute of Technology */
/*                     fulton@zach.fit.edu */

/* ********************************************************************** */
/*      These routines estimate eigenvalues, eigenfunctions and/or     * */
/*      spectral density functions for Sturm-Liouville problems.       * */
/*      The differential equation has the form:                        * */
/*                                                                     * */
/*           -(p(x)u')' + q(x)u  =  EV*r(x)u       for x in [A,B]      * */
/*                                                                     * */
/*      with boundary conditions (at regular points)                   * */
/*                                                                     * */
/*           A1*u - A2*(pu')  =  EV*(A1'*u - A2'*(pu'))    at A        * */
/*           B1*u + B2*(pu')  =  0                         at B .      * */
/*                                                                     * */
/*      The functions p(x) and r(x) are assumed to be positive in      * */
/*      the open interval (A,B).                                       * */
/* ********************************************************************** */

/*      Possible outputs are: */
/*         a set of eigenvalues; */
/*         a set of eigenvalues and tables of values for their eigen- */
/*            functions; */
/*         a table of the spectral density function (for cases with */
/*            continuous spectrum). */
/*         a classification of the problem (regular or singular; if */
/*            singular then limit point or limit circle, oscillatory */
/*            or nonoscillatory). */

/*      The code can find eigenvalues and eigenfunctions for problems */
/*      in spectral category 1 (both endpoints NONOSC), spectral */
/*      category 2 (one endpoint NONOSC and the other O-NO), and */
/*      those discrete eigenvalues below the essential spectrum in */
/*      spectral category 10 (both endpoints O-NO).  Here OSC at an */
/*      endpoint means the Sturm-Liouville equation is oscillatory for */
/*      all real values of EV at that endpoint, NONOSC at an endpoint */
/*      means the equation is nonoscillatory for all real values of EV */
/*      at that endpoint, and O-NO means there is a `cutoff' value EV' */
/*      such that the equation is nonoscillatory for real values of */
/*      EV < EV' and oscillatory for real values of EV > EV'.  For */
/*      problems in other spectral categories an error return will */
/*      be generated.  The manner in which SLEDGE classifies singular */
/*      endpoints of Sturm-Liouville problems as LP/LC (Limit Point/ */
/*      Limit Circle), OSC/NONOSC/O-NO, and uses this information to */
/*      determine the spectral category is explained in detail in */
/*      reference [2]. */

/*      There is one subroutine called SLEDGE of direct interest to the */
/*      user; additionally, a secondary routine INTERV is available which */
/*      determines the indices of eigenvalues located in a specified */
/*      subinterval of the real line. */

/*      The names of other routines in this package are AITKEN, ASYMEV, */
/*      ASYMR, BRCKET, CLASS, CLSEND, DENSEF, DSCRIP, EXTRAP, GETEF, */
/*      GETRN, MESH, POWER, PQRINT, REGULR, SHOOT, START, STEP, and */
/*      ZZERO. */

/*      There are 4 blocks of labeled COMMON with the names SLREAL, */
/*      SLINT, SLLOG, and SLCLSS. */

/*      This is the double precision version of the code; all floating */
/*      point variables should be declared DOUBLE PRECISION in the */
/*      calling program.  In these subprograms all such local */
/*      variables and constants have been explicitly declared; also, */
/*      FORTRAN77 generic intrinsic functions have been used, so */
/*      conversion to single precision should be straightforward, if */
/*      desired. */

/*      ACKNOWLEDGMENT:  This work was partially supported by the */
/*      National Science Foundation under grants DMS-8813113 and DMS- */
/*      8905202 to Florida Institute of Technology and DMS-8800839 and */
/*      DMS-8905232 to the Colorado School of Mines. */

/*      References */

/*      The following papers are available from the authors on request: */

/*      [1]. Pruess & Fulton, Mathematical software for Sturm-Liouville */
/*           problems, ACM Trans. on Math. Software, to appear, 1993. */

/*      [2]. Fulton, Pruess & Xie, The automatic classification of Sturm- */
/*           Liouville problems, submitted, 1992. */

/*      [3]. Pruess, Fulton & Xie, An asymptotic numerical method for a */
/*           class of singular Sturm-Liouville problems, submitted, 1992. */

/*      [4]  Fulton and Pruess, Eigenvalue and eigenfunction asymptotics */
/*           for regular Sturm-Liouville problems, Jour. Math. Anal. and */
/*           Appls., to appear. */

/*      [5]  Fulton and Pruess, Numerical Approximation of singular */
/*           spectral functions arising from the Fourier-Jacobi problem */
/*           on a half line with continuous spectra, Sixth International */
/*           Workshop in Analysis and its Applications, June, 1992. */
/*      [6]. Pruess, Fulton & Xie, Performance of the Sturm-Liouville */
/*           software package SLEDGE, Colo. School of Mines, Dept. of */
/*           Math. and Comp. Sci., MCS-91-19, 1991. Revision 12/92. */
/* ----------------------------------------------------------------------- */
/*      Brief overview of algorithms: */

/*         The code constructs (or takes from input) an initial mesh, */
/*      called the level 0 mesh.  Subsequent meshes (for level 1,2,...) */
/*      are unions of the previous level's mesh with its midpoints.  A */
/*      sequence of estimates for desired eigenvalues and eigenfunctions */
/*      is constructed, one set for each level.  These estimates (the */
/*      eigenvalue is called EvHat) are exact solutions (up to the */
/*      requested tolerance) of a Sturm-Liouville problem which is an */
/*      approximation to the original one; this approximation results */
/*      from replacing the given coefficient functions with step function */
/*      approximations relative to the current level's mesh.  The eigen- */
/*      functions of the resulting ODE's are piecewise trigonometric */
/*      (circular or hyperbolic) functions. */
/*         If estimates for the spectral density function are reqested, */
/*      these are computed as limits of a sequence of spectral density */
/*      functions of approximating regular problems.  For these regular */
/*      problems the spectral density function is a step function, and */
/*      is computed directly from the definition making use of computed */
/*      eigenvalues and the norm reciprocals of the corresponding eigen- */
/*      functions.  If verbose output is rquested by the user, there */
/*      will be displayed iterations (corresponding to the sequence of */
/*      approximating regular intervals which the code automatically */
/*      selects) and within each iteration there will be levels */
/*      (corresponding to increasingly finer meshes as described above). */
/*      A step spectral density function will be printed at each level */
/*      of each iteration.  The spectral density function displayed at */
/*      the end of each iteration is the result of an h-squared extra- */
/*      polation over the regular step functions generated at each level */
/*      of this iteration.  The condition for stopping at a given iter- */
/*      ation is a straightforward comparison of the spectral function */
/*      data for the current iteration with the previous iteration. There */
/*      is no extrapolation over the sequence of regular approximating */
/*      intervals as no extrapolation theory for the approximation of */
/*      the singular spectral function by regular step spectral functions */
/*      is known.  (To achieve closer approximation of the regular step */
/*      spectral functions to the singular spectral function, it is */
/*      actually the piecewise linear function obtained by joining the */
/*      midpoints of successive steps by a straight line which is used as */
/*      the `regular' spectral function for the purpose of generating the */
/*      actual data used for the h-squared extrapolation.) */
/*         The classification is determined by applying standard theory */
/*      to an approximating problem, each of whose coefficient functions, */
/*      in a small neighborhood of each endpoint, consists of the leading */
/*      term in a power-like asymptotic development.  For this reason */
/*      there are many problems, particularly those with oscillatory */
/*      coefficient functions, for which the code's output for the */
/*      classification information is labelled `uncertain'.  For further */
/*      information on the theory used by the code to generate endpoint */
/*      classifications and spectral category information see [2] above. */
/* ----------------------------------------------------------------------- */
/*  Usage (simple explanation) - */
/*      The subroutine SLEDGE is called in the following manner: */

/*      SUBROUTINE SLEDGE(JOB,CONS,ENDFIN,INVEC,TOL,TYPE,EV,NUMX,XEF,EF, */
/*                        PDEF,T,RHO,IFLAG,STORE) */

/*      If k eigenvalues (no eigenvectors) are sought then set */
/*      (a) the logical 5-vector JOB to (True, False, False, False, True); */
/*      (b) the real 8-vector CONS to the values of A1,A1',A2,A2',B1,B2, */
/*          A,B for the boundary condition information.  It does not */
/*          matter what values are used for infinite endpoints, nor for */
/*          the boundary constants at a singular endpoint; the code */
/*          automatically selects the Friedrichs' boundary condition at */
/*          NONOSC singular endpoints, overriding user input for the */
/*          boundary condition constants; for infinite endpoints the code */
/*          also automatically selects these constants. */
/*      (c) the logical 2-vector ENDFIN to (True, True) if both endpoints */
/*          are finite, (True, False) if A is finite but B infinite, etc.; */
/*      (d) the integer vector INVEC should have */
/*              INVEC(1) = 0 (no internal printing) */
/*              INVEC(2) = 0 */
/*              INVEC(3) = k, the number of eigenvalues sought */
/*              INVEC(3+i) = index of ith eigenvalue sought,i = 1,...,k; */
/*      (e) the real 6-vector TOL should have */
/*              TOL(1) = absolute error tolerance desired, */
/*              TOL(2) = relative error tolerance desired, */
/*          the remaining 4 entries of TOL are ignored; */
/*      (f) the output estimate for the ith eigenvalue is returned */
/*          in EV(i), i = 1,...,k; */
/*      (g) the output integer k-vector IFLAG(*) should have all entries */
/*          zero; nonzero values indicate warnings or error returns and */
/*          are explained in the detailed usage section below; */
/*      (h) the auxiliary vector STORE(*) should be dimensioned at least */
/*          155 in the calling program; */
/*      (i) the logical 4 by 2 vector TYPE, the real vectors XEF(1), */
/*          EF(1), PDEF(1), T(1), and RHO(1) can be ignored except that */
/*          they need to be declared in the calling program.  The integer */
/*          scalar NUMX can also be ignored. */

/*      If k eigenfunctions are also desired, then follow the above */
/*      pattern except make JOB(1) False and JOB(2) True.  The values of */
/*      TOL(3) and TOL(4) control the absolute and relative errors in */
/*      each u(x); TOL(5) and TOL(6) control the absolute and relative */
/*      errors in each (pu')(x).  It is usually appropriate to set TOL(5) */
/*      = TOL(3) = TOL(1) and TOL(6) = TOL(4) = TOL(2), but the user has */
/*      the option of entering all six tolerance parameters as desired. */
/*      The output eigenfunction information is returned in the three */
/*      real vectors X(*) for the independent variable x , EF(*) for u(x), */
/*      PDEF(*) for (pu')(x).  The code automatically chooses the x */
/*      values; the number of values is returned in NUMX.  If you prefer */
/*      another choice of output points, see the detailed explanation */
/*      below on usage of the code.  The values for the first requested */
/*      u(x) are returned in the first NUMX locations of EF(*), those for */
/*      the second are in the next NUMX locations, etc.  PDEF(*) is part- */
/*      itioned similarly; X(*) must be dimensioned at least 31 in the */
/*      calling program while EF(*) and PDEF(*) must be dimensioned at */
/*      least 31*k.  The auxiliary vector STORE(*) should be dimensioned */
/*      at least 420. */

/*      For other possibilities, see the detailed description which */
/*      follows. */
/* ----------------------------------------------------------------------- */
/*  Usage (detailed explanation) - */

/*  SUBROUTINE SLEDGE(JOB,CONS,ENDFIN,INVEC,TOL,TYPE,EV,NUMX,XEF,EF,PDEF, */
/*                    T,RHO,IFLAG,STORE) */

/*  Input parameters; */
/*     JOB(*)    = logical 5-vector, */
/*                 JOB(1) = .True. iff a set of eigenvalues are to be */
/*                                 computed but not their eigenfunctions. */
/*                 JOB(2) = .True. iff a set of eigenvalue and eigenfunc- */
/*                                 tion pairs are to be calculated. */
/*                 JOB(3) = .True. iff the spectral function is to be */
/*                                 computed over some subinterval of the */
/*                                 essential spectrum. */
/*                 JOB(4) = .True. iff the normal call to the routines for */
/*                                 classification (regular/singular, etc.) */
/*                                 is OVERRIDDEN.  If JOB(4) is True then */
/*                                 TYPE(*,*) discussed below must be */
/*                                 INPUT correctly!  Most users will not */
/*                                 want to override the classification */
/*                                 routines, but it would, of course, be */
/*                                 appropriate for users experimenting with */
/*                                 problems for which the coefficient */
/*                                 functions do not have power-like */
/*                                 behavior near the singular endpoints. */
/*                                 Note: the code may perform poorly if */
/*                                 the classification information is */
/*                                 incorrect; since the cost is usually */
/*                                 negligible, it is strongly recommended */
/*                                 that JOB(4) be False.  The classifica- */
/*                                 tion is deemed sufficiently important */
/*                                 for spectral density function calcul- */
/*                                 ations that JOB(4) is ignored when */
/*                                 the input JOB(3) is True. */
/*                 JOB(5) = .True. iff mesh distribution is to be chosen */
/*                                 by SLEDGE.  If JOB(5) is True and NUMX */
/*                                 is zero, then the number of mesh */
/*                                 points is also chosen by SLEDGE; if */
/*                                 NUMX > 0 then NUMX mesh points will be */
/*                                 used.  If JOB(5) is False, then the */
/*                                 number (NUMX) and distribution */
/*                                 (XEF(*)) must be input by the user. */
/*                                 If JOB(3) is True and JOB(5) False */
/*                                 then the user must set BOTH the number */
/*                                 NUMX and distribution.  In this case, */
/*                                 NO global error estimates are made. */
/*     CONS(*)    = real vector of length 8, values are the boundary */
/*                  condition constants A1, A1', A2, A2', B1, B2, A, B. */
/*                  In the case of a NONOSC singular endpoint, the class- */
/*                  ification routine uses the Friedrichs' boundary */
/*                  condition constants.  The code cannot automatically */
/*                  choose a non-Friedrichs' boundary condition; however, */
/*                  interval truncation in the user's calling program can */
/*                  be used, together with many calls to SLEDGE, to */
/*                  compute singular eigenvalues associated with a non- */
/*                  Friedrichs' boundary condition at a NONOSC endpoint */
/*                  (see remark 12 below). */
/*     ENDFIN(*) = logical 2-vector, values are */
/*        ENDFIN(1) = .True. iff endpoint A is finite. */
/*        ENDFIN(2) = .True. iff endpoint B is finite. */
/*     INVEC(*)  = integer vector of length 3+(number of eigenvalues */
/*                 desired).  This vector contains a variety of input */
/*                 information. */
/*        INVEC(1) controls the amount of internal printing: values are */
/*                 from 0 (no printing) to 5 (much printing). */
/*                 For INVEC(1) > 0 much of the output will be to a file */
/*                 attached to unit #21 which should be named in the */
/*                 user's calling program via an OPEN statement. */
/*                 Output for the various cases is, when INVEC(1) = */
/*                    0   no printing. */
/*                 When JOB(1) or JOB(2) is True */
/*                    1   initial mesh (the first 51 or fewer points), */
/*                        eigenvalue estimate at each level, */
/*                    4   the above, */
/*                        at each level */
/*                          matching point for eigenfunction shooting, */
/*                          X(*), EF(*), PDE(*) values, */
/*                    5   all the above, */
/*                        at each level */
/*                          brackets for the eigenvalue search, */
/*                          intermediate shooting info for the eigen- */
/*                          function and eigenfunction norm. */
/*                 When JOB(3) is True */
/*                    1   the actual (a,b) used at each iteration, */
/*                        the total number of eigenvalues computed, */
/*                    2   the above, */
/*                        switchover points to the asymptotic formulas, */
/*                        some intermediate Rho(t) approximations, */
/*                    3   all the above, */
/*                        initial meshes for each iteration, */
/*                        index of the largest EV which may be computed, */
/*                        various Ev and RsubN values, */
/*                    4   all of the above, */
/*                        RhoHat values at each level, */
/*                    5   all of the above, */
/*                        all Ev and RsubN values below switchover point. */
/*                 When JOB(4) is False */
/*                    2   output a description of the spectrum, */
/*                    3   the above plus the constants for the */
/*                        Friedrichs' boundary condition(s), */
/*                    5   all the above plus intermediate details of */
/*                        classification calculation. */
/*                 Some of the output may go to the default output device */
/*                 (screen or printer), but all information requested is */
/*                 also directed to the file attached to unit #21. */
/*        INVEC(2) gives the number (positive) of output values desired */
/*                 for the array RHO(*) (not referenced if JOB(3) is */
/*                 False). */
/*        INVEC(3) is the total number of eigenvalues to be output in */
/*                 EV(*). */
/*        INVEC(J) for J = 4, 5, ..., 3+INVEC(3) contains the indices for */
/*                 the eigenvalues sought. If JOB(1) and JOB(2) are */
/*                 False, this part of INVEC(*) is not referenced. */
/*     TOL(*)    = real vector of from 2 to 6 tolerances. */
/*                 If JOB(1) or JOB(2) is True then */
/*                   TOL(1) is the absolute error tolerance for e-values, */
/*                   TOL(2) is the relative error tolerance for e-values, */
/*                   TOL(3) is the abs. error tolerance for e-functions, */
/*                   TOL(4) is the rel. error tolerance for e-functions, */
/*                   TOL(5) is the abs. error tolerance for eigenfunction */
/*                          derivatives, */
/*                   TOL(6) is the rel. error tolerance for eigenfunction */
/*                          derivatives. */
/*                   Eigenfunction tolerances need not be set if JOB(2) */
/*                   is False. */
/*                 If JOB(3) is True then */
/*                   TOL(1) is the absolute error tolerance, */
/*                   TOL(2) is the relative error tolerance; */
/*                   the output RHO values are NOT required to satisfy */
/*                   these tolerances when JOB(5) is False. */
/*                 All absolute error tolerances must be positive; all */
/*                 relative error tolerances must be at least 100 times */
/*                 the unit roundoff. */
/*     NUMX      = integer whose value is */
/*                    the number of output points where each eigen- */
/*                    function is to be evaluated (the number of entries */
/*                    in XEF(*)) when JOB(2) is True, */
/*                 or */
/*                    the number of points in the initial mesh used when */
/*                    JOB(5) is False and NUMX>0. */
/*                 If JOB(5) is False, the points in XEF(*) should be */
/*                 chosen to have a reasonable distribution.  Since the */
/*                 endpoints A and B must be part of any mesh, NUMX */
/*                 cannot be 1 in this case.  If JOB(5) is FALSE and */
/*                 JOB(3) is True, then NUMX must be positive. */
/*     XEF(*)    = real vector of points where */
/*                    eigenfunction estimates are desired (JOB(2) True) */
/*                 or */
/*                    where user's initial mesh is entered (JOB(5) False */
/*                    and NUMX>0). */
/*                 The values must satisfy */
/*                      A = XEF(1) < XEF(2) < ... < XEF(NUMX) = B . */
/*                 When JOB(2) is True the initial mesh corresponds to */
/*                 the set of points where eigenfunction output is */
/*                 desired. If JOB(2) is False and NUMX = 0, then this */
/*                 vector is not referenced.  When A and/or B are */
/*                 infinite (as indicated through ENDFIN(*)), the */
/*                 entries XEF(1) and/or XEF(NUMX) are ignored; however, */
/*                 it is required that XEF(2) be negative when ENDFIN(1) */
/*                 is False, and XEF(NUMX-1) be positive when ENDFIN(2) */
/*                 is False (otherwise, IFLAG = -39 will result). */
/*     T(*)      = real vector of INVEC(2) values where the spectral */
/*                 function RHO(*) is desired (the existence and location */
/*                 of continuous spectrum can be found by first calling */
/*                 SLEDGE with JOB(J) False, J=1,...,4 and INVEC(1) = 1). */
/*                 Vector T(*) is not referenced if JOB(3) is False.  Its */
/*                 entries must be in increasing order. */

/*   Output parameters: */
/*     TYPE(*,*) = 4 by 2 logical array; column 1 carries information */
/*                 about endpoint A while column 2 refers to B. */
/*                 TYPE(1,*) = True  iff the endpoint is regular, */
/*                 TYPE(2,*) = True  iff it is limit circle, */
/*                 TYPE(3,*) = True  iff it is nonoscillatory for all EV, */
/*                 TYPE(4,*) = True  iff it is oscillatory for all EV, */
/*                 Important note: all of these must be correctly INPUT */
/*                 if JOB(4) is True! */
/*     EV(*)     = real vector containing the computed approximations to */
/*                 the eigenvalues whose indices are specified in */
/*                 INVEC(*); if JOB(1) and JOB(2) are False, then the */
/*                 output has no meaning. */
/*     NUMX      = the number of output points for eigenfunctions when */
/*                 input NUMX = 0, and JOB(2) or JOB(5) is True. */
/*     XEF(*)    = input values (if any) are changed only if JOB(2) and */
/*                 JOB(5) are True; in this case, the output values */
/*                 are chosen by the code.  If JOB(2) is False then this */
/*                 vector is not referenced; if JOB(2) is True and NUMX>0 */
/*                 on input then XEF(*) should be dimensioned at least */
/*                 NUMX+16 in the calling program.  If JOB(2) is True and */
/*                 NUMX=0 on input (so that the code chooses NUMX), then */
/*                 dimension XEF(*) at least 31 in the calling program. */
/*     EF(*)     = real vector of eigenfunction values: EF((k-1)*NUMX+i) */
/*                 is the estimate of u(XEF(i)) corresponding to the */
/*                 eigenvalue in EV(k).  If JOB(2) is False then this */
/*                 vector is not referenced.  Otherwise, if JOB(2) is */
/*                 True and NUMX>0 on input then EF(*) should be */
/*                 dimensioned at least NUMX*INVEC(3) in the calling */
/*                 program.  If JOB(2) is True and NUMX=0 on input (so */
/*                 that the code chooses NUMX), then dimension XEF(*) */
/*                 at least 31*INVEC(3) in the calling program. */
/*     PDEF(*)   = real vector of eigenfunction derivative values: */
/*                 PDEF((k-1)*NUMX+i) is the estimate of (pu')(XEF(i)) */
/*                 corresponding to the eigenvalue in EV(k).  If JOB(2) */
/*                 is False then this vector is not referenced; otherwise, */
/*                 it must be dimensioned as is EF(*). */
/*     RHO(*)    = real vector of values for the spectral density */
/*                 function rho(t), RHO(I) = rho(T(I)).  RHO(*) must be */
/*                 dimensioned at least INVEC(2); this vector is not */
/*                 referenced if JOB(3) is False. */
/*     IFLAG(*)   = integer vector carrying information about the output. */
/*       Declared length must be at least max(1,INVEC(3)).  For the Kth */
/*       requested eigenvalue (when JOB(1) or JOB(2) is true; otherwise, */
/*       only IFLAG(1) is used): */
/*       IFLAG(K) =  0, normal return, output should be reliable. */
/*                <  0, fatal error, calculations ceased: if */
/*                = -1, too many levels needed for the eigenvalue */
/*                      calculation; problem seems too difficult for */
/*                      this algorithm at this tolerance. Are the */
/*                      coefficient functions nonsmooth? */
/*                = -2, too many levels needed for the eigenfunction */
/*                      calculation; problem seems too difficult for */
/*                      this algorithm at this tolerance.  Are the */
/*                      eigenfunctions ill-conditioned? */
/*                = -3, too many levels needed for the spectral density */
/*                      calculation; problem seems too difficult for */
/*                      this algorithm at this tolerance. */
/*                = -4, the user has requested the spectral density */
/*                      function for a problem which has no continuous */
/*                      spectrum. */
/*                = -5, the user has requested the spectral density */
/*                      function for a problem with both endpoints */
/*                      generating essential spectrum, i.e., both */
/*                      endpoints being either OSC or O-NO.  The spectral */
/*                      density function calculation has not been */
/*                      implemented for such cases.  For spectral */
/*                      category 10 (both endpoints O-NO) the spectral */
/*                      multiplicity is generally two, proper normal- */
/*                      izations for the solutions against which the */
/*                      spectral functions will be normalized will */
/*                      depend on how the user wants to express the */
/*                      eigenfunction expansion.  Users having problems */
/*                      in spectral category 10 are encouraged to supply */
/*                      them to the authors, and if possible, recommend */
/*                      normalizations of the two solutions to be used in */
/*                      writing the associated eigenfunction expansion. */
/*                = -6, the user has requested the spectral density */
/*                      function for a problem in spectral category 2 for */
/*                      which a proper normalization of solution at the */
/*                      NONOSC endpoint is not known; for example, */
/*                      problems with an irregular singular point or */
/*                      infinite endpoint at one end and continuous */
/*                      spectrum generated at the other.  Users with */
/*                      problems of this type are encouraged to supply */
/*                      them to the authors, and if possible, recommend a */
/*                      normalization of solution at the NONOSC endpoint */
/*                      which they would like to see implemented. As a */
/*                      rule it is best to pick a normalization which */
/*                      ensures that the solution is uniquely fixed and */
/*                      entire in the eigenvalue parameter EV for all x */
/*                      in the Sturm-Liouville interval; for further */
/*                      mathematical information on NONOSC endpoints we */
/*                      refer to paper [2] above. */
/*                = -7, problems encountered in obtaining a bracket. */
/*                = -8, too small a step used in the integration; */
/*                      TOL(*) values may be too small for this problem. */
/*                = -9, too small a step used in a spectral density */
/*                      function calculation for which the continuous */
/*                      spectrum is generated by a finite endpoint.  Try */
/*                      transforming to Liouville (or some other) form. */
/*                = -10, an argument to the circular trig functions is */
/*                       too large.  Try rerunning with a finer initial */
/*                       mesh, or, on singular problems, use interval */
/*                       truncation (see remark (12)). */
/*                = -15, p(x) and r(x) not positive in (A,B). */
/*                = -20, eigenvalues/functions were requested for a */
/*                       problem with an OSC singular endpoint. */
/*                       Interval truncation (see remark (12)) must be */
/*                       used on such problems. */
/*                = -3?, illegal input, viz. */
/*                  -30,  NUMX = 1 when JOB(5) is True, */
/*                        or NUMX = 0 when JOB(3) is True and JOB(5) is */
/*                        False, */
/*                  -31,  B1 = B2 = 0 (at a regular endpoint), */
/*                  -32,  A1'*A2-A1*A2' .le. 0 when A1' or A2' nonzero, */
/*                  -33,  A1 = A2 = A1'= A2'= 0 (at a regular endpoint), */
/*                  -34,  A .ge. B (when both are finite), */
/*                  -35,  TOL(odd) .le. 0 , */
/*                  -36,  TOL(even)  <  100*unit roundoff, */
/*                  -37,  INVEC(k) < 0  for some k>3 when INVEC(3)>0, */
/*                  -38,  INVEC(2) .le. 0 when JOB(3) is True , */
/*                  -39,  XEF(*) entries out of order or not in [A,B]. */
/*                        or XEF(2), XEF(NUMX-1) have the wrong sign in */
/*                           infinite interval cases, */
/*                        or T(*) entries are out of order. */
/*                >  0,  indicates some kind of warning, in this case the */
/*                       value may contain ANY of the following digits: */
/*                =  1,  failure in routine BRCKET probably due to a */
/*                       cluster of eigenvalues which the code cannot */
/*                       separate.  Calculations have continued as best */
/*                       as possible, but any eigenfunction results are */
/*                       suspect.  Try rerunning with tighter input */
/*                       tolerances to separate the cluster. */
/*                =  2,  there is uncertainty in the classification for */
/*                       this problem.  Because of the limitations of the */
/*                       floating point arithmetic on the computer used, */
/*                       and the nature of the finite sampling, the */
/*                       routine is cannot be decisive about the */
/*                       classification information at the requested */
/*                       tolerance. */
/*                =  3,  there may be some eigenvalues imbedded in the */
/*                       essential spectrum; using IPRINT greater than */
/*                       zero will result in additional output giving */
/*                       the location of the approximating eigenvalues */
/*                       for the step function problem.  These could be */
/*                       extrapolated to estimate the actual eigenvalue */
/*                       embedded in the essential spectrum. */
/*                =  5,  a change of variables was made to avoid poten- */
/*                       tial slow convergence; however, the global */
/*                       error estimates may not be as reliable.  Some */
/*                       experimentation using different tolerances is */
/*                       recommended. */
/*                =  6,  there were problems with eigenfunction conver- */
/*                       gence in a spectral density calculation; the */
/*                       output Rho(t) may not be accurate. */

/*   Auxiliary storage: */
/*     STORE(*) = real vector of auxiliary storage, must be dimensioned */
/*                at least */
/*             max(155,NUMX+16)     in general; */
/*               26*(NUMX+16)       for any eigenfunction calculation; */
/*             2400+13*INVEC(2)     for any spectral density calculation. */
/* ----------------------------------------------------------------------- */
/*  SUBROUTINE INTERV(FIRST,ALPHA,BETA,CONS,ENDFIN,NFIRST,NTOTAL, */
/*                    IFLAG,STORE) */

/*    Input parameters: */
/*     FIRST      = logical; value is True if various internal variables */
/*                  have not yet been set.  If a prior call has been made */
/*                  to INTERV with FIRST True, then a little time can */
/*                  be saved by letting FIRST be False. */
/*                  IMPORTANT NOTE: setting FIRST = True will clobber any */
/*                  initial mesh the user has input (when NUMX > 0 or */
/*                  JOB(5) is False);  also, INTERV will classify the */
/*                  problem irregardless of what JOB(4) is set to */
/*                  for SLEDGE. */
/*      ALPHA     = real value of left end point of search interval. */
/*      BETA      = real value of right end point of search interval. */
/*      CONS(* )  = real vector of 8 input constants: A1, A1', A2, A2', */
/*                  B1, B2, A, B. */
/*      ENDFIN(*) = logical 2-vector, same meaning as in SLEDGE. */
/*      STORE(*)  = real vector holding initial mesh. */

/*    Output parameters: */
/*      NFIRST = index of first eigenvalue > ALPHA. */
/*      NTOTAL = total number of eigenvalues in the interval. */
/*      IFLAG  = integer status indicator. */
/*               IFLAG =   0 , normal return, output should be reliable, */
/*                     =  11 , there are no eigenvalues in [alpha, beta], */
/*                     =  12 , low confidence in NFIRST or NTOTAL or both, */
/*                     =  13 , BETA and/or ALPHA exceed the cutoff for */
/*                             the continuous spectrum.  If only BETA */
/*                             is too big then NFIRST may be OK, but */
/*                             NTOTAL is meaningless. */
/*                     = -11 , ALPHA .ge. BETA, */
/*                     = -25 , oscillatory endpoint, output meaningless, */
/*                     = -3? , illegal CONS(*) values (see above comments */
/*                             on SLEDGE for an explanation). */
/* -------------------------------------------------------------------------- */
/*         In addition, a subroutine subprogram must be provided for the */
/*      coefficient functions p(x), q(x), and r(x); the form of this */
/*      routine is */

/*          SUBROUTINE COEFF(X,PX,QX,RX) */
/*          DOUBLE PRECISION X,PX,QX,RX */
/*               ... */
/*          PX = ... */
/*          QX = ... */
/*          RX = ... */
/*          RETURN */
/*          END */

/*      The subroutine name MUST be COEFF, though of course the names of */
/*      arguments only need follow the usual FORTRAN77 rules. X is the */
/*      independent variable; PX, QX, and RX are the output values of the */
/*      respective coefficient functions p(x), q(x), and r(x) at X. */
/* ----------------------------------------------------------------------- */
/*     This is a simple sample driver for SLEDGE. */
/* C */
/* C     Declare all variables: */
/* C */
/*      INTEGER IFLAG(1),INVEC(4),NUMX, I,J,K */
/*      LOGICAL JOB(5),TYPE(4,2),ENDFIN(2) */
/*      DOUBLE PRECISION CONS(8),TOL(6),EV(1),T(3),RHO(3),STORE(2450), */
/*     &                 XEF(5),EF(5),PDEF(5) */
/* C */
/* C     Load the boundary condition information into CONS(*). */
/* C     This example has a Neumann condition at A = 1, and a */
/* C     singular point at B = +infinity. */
/* C */
/*      DATA CONS/0.0, 0.0, 1.0, 0.0,   0.0, 0.0,   1.0, 0.0/ */
/*      DATA ENDFIN/.TRUE., .FALSE./ */
/* C */
/* C     The eigenfunctions will be estimated at 5 points. */
/* C */
/*      DATA NUMX,XEF/5, 1.0, 1.5D0, 2.0, 4.0, 100.0/ */
/* C */
/* C     Initialize the vector INVEC(*): */
/* C        little printing, */
/* C        3 output points for the density function Rho(t), */
/* C        estimates for the first (index 0) eigenvalue/function. */
/* C */
/*      DATA INVEC/1, 3, 1, 0/ */
/* C */
/* C     Set the JOB(*) vector: */
/* C        estimate both eigenvalues and eigenvectors, */
/* C        estimate the spectral density function, */
/* C        classify, */
/* C        force the initial mesh to be the output points. */
/* C */
/*      DATA JOB/.FALSE.,.TRUE.,.TRUE.,.FALSE.,.FALSE./ */
/* C */
/* C     Set the tolerances: */
/* C */
/*      DATA TOL/1.D-5,1.D-4,  1.D-5,1.D-4,  1.D-5,1.D-4/ */
/* C */
/* C     Initialize the 3 output points for the density function. */
/* C */
/*      DATA T/0.0, 0.5, 2.0/ */
/* C */
/* C     Open file for output. */
/* C */
/*      OPEN(21,FILE = 'sample.out') */
/*      CALL SLEDGE(JOB,CONS,ENDFIN,INVEC,TOL,TYPE,EV,NUMX,XEF,EF,PDEF, */
/*     &            T,RHO,IFLAG,STORE) */
/* C */
/* C     Print results: */
/* C */
/*      DO 30 I = 1,INVEC(3) */
/*         WRITE (*,10) INVEC(3+I),EV(I),IFLAG(I) */
/*         WRITE (21,10) INVEC(3+I),EV(I),IFLAG(I) */
/*   10    FORMAT(' Nev =',I6,';   Ev =',D25.15,';     Flag = ',I3) */
/*         IF (IFLAG(I) .GT. -10) THEN */
/*            WRITE (*,15) */
/*            WRITE (21,15) */
/*   15       FORMAT(13X,'x',23X,'u(x)',18X,'(pu`)(x)') */
/*            K = NUMX*(I-1) */
/*            DO 25 J = 1,NUMX */
/*               WRITE (21,20) XEF(J),EF(J+K),PDEF(J+K) */
/*   20          FORMAT(3D25.15) */
/*   25       CONTINUE */
/*         ENDIF */
/*   30    CONTINUE */
/*      WRITE (*,35) */
/*      WRITE (21,35) */
/*   35 FORMAT(/,8X,'t',21X,'Rho(t)') */
/*      DO 45 I = 1,INVEC(2) */
/*         WRITE (*,40) T(I),RHO(I) */
/*         WRITE (21,40) T(I),RHO(I) */
/*   40    FORMAT(F11.3,D32.15) */
/*   45 CONTINUE */
/*      CLOSE(21) */
/*      STOP */
/*      END */
/* C */
/*      SUBROUTINE COEFF(X,PX,QX,RX) */
/* C */
/* C     Define the coefficient functions; here a Yukawa potential. */
/* C */
/*      DOUBLE PRECISION X,PX,QX,RX, T */
/* C */
/* C     Be careful with potential over/underflows; here we assume the */
/* C     IEEE double precision exponent range. */
/* C */
/*      IF (X .LT. 650.0) THEN */
/*         T = EXP(-X) */
/*      ELSE */
/*         T = 0.0 */
/*      ENDIF */
/*      PX = 1.0 */
/*      QX = -T/X */
/*      RX = PX */
/*      RETURN */
/*      END */
/* C */
/* C     End of sample driver for SLEDGE. */
/* ----------------------------------------------------------------------- */
/*      General remarks: */
/*      (1) Two machine dependent constants must be set in a DATA */
/*          statement in routine START (in part 4 of the package): */
/*          URN   -  an estimate of the unit roundoff; infinite output */
/*                   values are assigned the value 1/URN. */
/*          UFLOW -  a number somewhat smaller than -ln(underflow level). */
/*                   Values of certain variables z for which */
/*                             ln(abs(z)) < -under */
/*                   will be set to zero. */
/*      (2) A value of IFLAG = -1, -2, or -3 may be the result of a */
/*          lack of smoothness in the coefficient functions.  In such */
/*          cases a user input mesh may perform better (see (4) below). */
/*      (3) The heuristics for generating the initial mesh distribution */
/*          work reasonably well over a wide range of examples, but */
/*          occasionally they are far from optimal.  The code's choice */
/*          can be over-ridden by setting JOB(5) False, setting NUMX */
/*          appropriately and supplying a mesh in XEF(*). */
/*      (4) If any of the coefficient functions p,q, or r (or their first */
/*          few derivatives) have finite jump discontinuities at points */
/*          in the interior of (A,B), then it is advantageous to have */
/*          these points in SLEDGE's mesh.  Currently, this can only be */
/*          accomplished by setting JOB(5) False and supplying an */
/*          appropriate mesh using NUMX and XEF(*). */
/*      (5) In general, eigenvalue convergence is observed to be more */
/*          rapid than eigenfunction convergence; hence, it is */
/*          recommended that JOB(2) be False unless eigenfunction */
/*          information really is necessary. */
/*      (6) When eigenfunction output is sought, unless some knowledge */
/*          of the eigenfunction is known in advance, it is recommended */
/*          that JOB(5) be True so that the code will attempt to choose */
/*          a reasonable distribution for the initial mesh points. */
/*      (7) Computing the spectral density function for problems having */
/*          continuous spectrum can be very expensive; it is recommended */
/*          that initially, relatively crude tolerances (0.001 or so) be */
/*          used to get some idea of the effort required. */
/*      (8) It is recommended that every problem be classified (JOB(4) */
/*          False) by the code before any calculation of spectral */
/*          quantities occurs.  Only if the user is certain as to what */
/*          the classification is (and describes it correctly through */
/*          INVEC and TYPE) should the classification option be bypassed. */
/*      (9) If the code does the classification of singular problems, it */
/*          will automatically choose the Friedrichs' boundary condition */
/*          at NONOSC endpoints.  If another boundary condition is */
/*          desired, the user must use interval truncation in the */
/*          calling program (see remark (12)). */
/*     (10) While all parts of the code should function on machines */
/*          with a fairly narrow exponent range (such as IEEE single */
/*          precision), it is better to have a relatively wide exponent */
/*          range (IEEE double precision).  The classification algorithm, */
/*          in particular, is far more reliable if done on a machine with */
/*          a fairly wide exponent range. */
/*     (11) Care must be taken in writing the subroutine COEFF for the */
/*          evaluation of p(x), q(x), and r(x) to avoid arithmetic */
/*          exceptions such as overflow and underflow (or trig function */
/*          arguments too large).  This can be especially delicate on */
/*          machines with a small exponent range. */
/*     (12) In some cases `interval truncation' is recommended.  By this */
/*          is meant the user should call SLEDGE several times using a */
/*          sequence of regular endpoints (with appropriate boundary */
/*          conditions) converging to the singular endpoint.  The eigen- */
/*          values of the regular problems selected by the user should be */
/*          arranged so as to converge to those of the desired singular */
/*          problem. For example, if the user wishes to compute eigen- */
/*          values associated with a non-Friedrichs' boundary condition */
/*          for problems in spectral category 1, the user can experiment */
/*          with choosing a sequence of regular approximating intervals, */
/*          and vary the boundary conditions appropriately by means of a */
/*          `boundary condition function' or known solution of the */
/*          equation for a real value of EV on the sequence of regular */
/*          intervals until convergence of the regular eigenvalues to */
/*          the desired singular one is observed.  Similarly, for */
/*          problems in spectral category 3 or 5 which involve one or two */
/*          endpoints which are LC and OSC, the (necessarily discrete) */
/*          spectrum is known to be unbounded below and above. To */
/*          implement a given LC boundary condition at a singular LC */
/*          endpoint one may choose a `boundary condition function' or */
/*          known solution of the equation for a real value of EV and */
/*          make use of it on a sequence of regular approximating */
/*          intervals to vary the boundary condition on successive calls */
/*          to SLEDGE for the sequence of regular intervals until */
/*          convergence to the desired singular eigenvalue is observed. */
/*          At present these methods are highly experimental and problem- */
/*          dependent as good heuristics for the choice of the rate of */
/*          convergence of the regular intervals to the singular one */
/*          which work well over a wide class of problems are not known. */
/*          (The only case in which SLEDGE automatically selects regular */
/*          approximating subintervals is for spectral density function */
/*          calculations for problems in spectral category 2; but */
/*          here the singular endpoint is of LP type, so no singular */
/*          boundary condition is required to be implemented.) */
/*     (13) Problems of slow convergence can sometimes be avoided by a */
/*          judicious change of either dependent or independent variable */
/*          (or both). */
/*     (14) If the Liouville normal form potential Q(t) has a minimum */
/*          far from zero, then the heuristics for generating the initial */
/*          mesh may well miss it.  In this case, it is advisable to */
/*          shift the independent variable. */
/*     (15) The determination of the total number of eigenvalues is the */
/*          most difficult part of the classification process.  When the */
/*          theory provides this number, of course, there is no problem; */
/*          otherwise, it should be viewed with some skepticism.  A more */
/*          reliable count of the eigenvalues below the cutoff point of */
/*          the essential spectrum can be gained (at some expense) by */
/*          trying to compute many eigenvalues near that point. */
/* /////////////////////////////////////////////////////////////////////// */
/* Subroutine */ int sledge_(logical *job, doublereal *cons, logical *endfin, 
	integer *invec, doublereal *tol, logical *type__, doublereal *ev, 
	integer *numx, doublereal *xef, doublereal *ef, doublereal *pdef, 
	doublereal *t, doublereal *rho, integer *iflag, doublereal *store)
{
    /* Initialized data */

    static doublereal denslo = 4.;
    static doublereal densop = 6.;
    static doublereal denshi = 12.;
    static doublereal endi[5] = { 12.,20.,85.,240.,500. };
    static doublereal zetai[5] = { 2.2,2.,1.5,1.4,1.3 };

    /* Format strings */
    static char fmt_30[] = "(\002 This problem is oscillatory, you must use\
 \002,\002interval truncation.\002)";
    static char fmt_35[] = "(\002 Level 0 mesh:\002,/,(5g15.6))";
    static char fmt_40[] = "(\002 WARNING: Requested eigenvalue \002,i6,\002\
 may not be below the continuous spectrum.\002)";
    static char fmt_50[] = "(\002 This problem has continuous spectrum gener\
ated by\002,\002 both endpoints.  The\002,/,\002 calculation of the spectral\
 density\002,\002 function has not yet been implemented\002,/,\002 for such \
cases.\002,/)";
    static char fmt_55[] = "(\002 This problem has no continuous spectrum\
.\002)";
    static char fmt_60[] = "(\002 The normalization of the spectral density \
function\002,\002 is unknown for this problem.\002)";
    static char fmt_65[] = "(60(\002-\002),/,\002 Iteration \002,i2)";
    static char fmt_70[] = "(/,\002 For a, b =\002,2f15.8,/,\002 Nxinit =\
 \002,i4,/)";
    static char fmt_75[] = "(\002 Level 0 mesh:\002,/,(5g15.6))";
    static char fmt_85[] = "(9x,\002t\002,15x,\002Truncated Rho(t)\002)";
    static char fmt_90[] = "(f12.4,d31.15)";
    static char fmt_115[] = "(\002 The total number of eigenvalues computed \
was \002,i10)";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double d_sign(doublereal *, doublereal *);
    integer s_wsfe(cilist *), e_wsfe(), do_fio(integer *, char *, ftnlen);
    double d_lg10(doublereal *);

    /* Local variables */
    static integer i__, j, k;
    static doublereal aa, bb, fz;
    static integer mu1, mu2;
    static doublereal cev[2];
    static integer iev;
    static doublereal sgn;
    static integer nev, kcl1, kcl2;
    static doublereal tol1;
    static logical done;
    static doublereal dens;
    static logical lplc[2];
    static doublereal hmin;
    extern /* Subroutine */ int mesh_(logical *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal zeta;
    static integer jtol, maxt, numt;
    static doublereal xtol;
    static logical aafin, bbfin;
    static integer ibase;
    static doublereal alpha;
    static logical lbase, edone, cspec[2];
    extern /* Subroutine */ int class_(integer *, doublereal *, logical *, 
	    logical *, doublereal *, integer *, logical *, doublereal *, 
	    logical *, doublereal *, logical *);
    static logical lmesh, jobst[3];
    static doublereal error;
    static integer numev;
    extern /* Subroutine */ int start_(logical *, doublereal *, doublereal *, 
	    integer *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *), shoot_(doublereal *, 
	    doublereal *, integer *, doublereal *);
    static doublereal endfac;
    extern /* Subroutine */ int densef_(doublereal *, logical *, integer *, 
	    integer *, integer *, integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *, doublereal *);
    static logical domesh, oscill;
    extern /* Subroutine */ int dscrip_(logical *, logical *, logical *, 
	    logical *, logical *, doublereal *, doublereal *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer lastev;
    extern /* Subroutine */ int regulr_(logical *, logical *, doublereal *, 
	    integer *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer iprint, maxits;
    static doublereal rhotol;
    static integer nextrp;

    /* Fortran I/O blocks */
    static cilist io___26 = { 0, 6, 0, fmt_30, 0 };
    static cilist io___27 = { 0, 21, 0, fmt_30, 0 };
    static cilist io___28 = { 0, 6, 0, fmt_35, 0 };
    static cilist io___29 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___30 = { 0, 6, 0, fmt_40, 0 };
    static cilist io___31 = { 0, 21, 0, fmt_40, 0 };
    static cilist io___32 = { 0, 6, 0, fmt_50, 0 };
    static cilist io___33 = { 0, 6, 0, fmt_55, 0 };
    static cilist io___34 = { 0, 6, 0, fmt_60, 0 };
    static cilist io___35 = { 0, 6, 0, fmt_60, 0 };
    static cilist io___36 = { 0, 6, 0, fmt_30, 0 };
    static cilist io___37 = { 0, 21, 0, fmt_30, 0 };
    static cilist io___50 = { 0, 6, 0, fmt_65, 0 };
    static cilist io___51 = { 0, 21, 0, fmt_65, 0 };
    static cilist io___52 = { 0, 21, 0, fmt_70, 0 };
    static cilist io___53 = { 0, 6, 0, fmt_75, 0 };
    static cilist io___54 = { 0, 21, 0, fmt_75, 0 };
    static cilist io___62 = { 0, 6, 0, fmt_85, 0 };
    static cilist io___63 = { 0, 21, 0, fmt_85, 0 };
    static cilist io___64 = { 0, 6, 0, fmt_90, 0 };
    static cilist io___65 = { 0, 21, 0, fmt_90, 0 };
    static cilist io___70 = { 0, 21, 0, fmt_115, 0 };



/*     This is the interface routine between the user and other routines */
/*     which carry out most of the actual calculations. */

    /* Parameter adjustments */
    --store;
    --iflag;
    --rho;
    --t;
    --pdef;
    --ef;
    --xef;
    --ev;
    type__ -= 5;
    --tol;
    --invec;
    --endfin;
    --cons;
    --job;

    /* Function Body */

/*     Initialize. */

    sllog_1.afin = endfin[1];
    sllog_1.bfin = endfin[2];
    iprint = invec[1];
    numt = invec[2];
    nev = invec[3];
    sllog_1.lnf = FALSE_;
    domesh = TRUE_;
    lmesh = FALSE_;
    slint_1.flag__ = 0;
    iflag[1] = 0;
    ibase = 1;
    lbase = FALSE_;
    for (i__ = 1; i__ <= 6; ++i__) {
	sllog_1.lflag[i__ - 1] = FALSE_;
/* L5: */
    }
/* Computing MIN */
    d__1 = tol[1] + tol[2];
    tol1 = min(d__1,1e-4);
    jobst[0] = job[2];
    if (*numx > 0 && ! job[5]) {
	jobst[1] = TRUE_;
    } else {
	jobst[1] = FALSE_;
    }
    jobst[2] = job[3];
    if (! job[1] && ! job[2]) {
	nev = 0;
    }
    start_(jobst, &cons[1], &tol[1], &nev, &invec[4], numx, &xef[1], &numt, &
	    t[1], &nextrp, &store[1]);
    if (job[4]) {
	alpha = slreal_1.a2 * slreal_1.a1p - slreal_1.a1 * slreal_1.a2p;
	if (slreal_1.a1p != 0. || slreal_1.a2p != 0.) {
	    if (alpha <= 0.) {
		slint_1.flag__ = -32;
	    }
	} else {
	    if (slreal_1.a1 == 0. && slreal_1.a2 == 0.) {
		slint_1.flag__ = -33;
	    }
	}
	if (slreal_1.b1 == 0. && slreal_1.b2 == 0.) {
	    slint_1.flag__ = -31;
	}
    }
    if (slint_1.flag__ < 0) {
	goto L120;
    }
    if (job[1] || job[2]) {
	i__1 = nev;
	for (k = 1; k <= i__1; ++k) {
	    ev[k] = 0.;
/* L10: */
	}
    }
    if (job[3]) {
	i__1 = numt;
	for (k = 1; k <= i__1; ++k) {
	    rho[k] = 0.;
/* L15: */
	}
    }
    if (! job[4] || job[3]) {
	class_(&iprint, &tol1, &jobst[1], cspec, cev, &lastev, lplc, &store[1]
		, &job[5], &hmin, &domesh);
	alpha = slreal_1.a2 * slreal_1.a1p - slreal_1.a1 * slreal_1.a2p;
	if (slreal_1.a1p != 0. || slreal_1.a2p != 0.) {
	    if (alpha <= 0.) {
		slint_1.flag__ = -32;
	    }
	} else {
	    if (slreal_1.a1 == 0. && slreal_1.a2 == 0.) {
		slint_1.flag__ = -33;
	    }
	}
	if (slreal_1.b1 == 0. && slreal_1.b2 == 0.) {
	    slint_1.flag__ = -31;
	}
	if (slint_1.flag__ < 0) {
	    goto L120;
	}
	for (k = 1; k <= 2; ++k) {
	    type__[(k << 2) + 1] = sllog_1.reg[k - 1];
	    type__[(k << 2) + 2] = sllog_1.lc[k - 1];
	    type__[(k << 2) + 3] = ! sllog_1.osc[k - 1];
	    type__[(k << 2) + 4] = sllog_1.osc[k - 1];
	    if (cspec[k - 1]) {
		type__[(k << 2) + 3] = FALSE_;
		type__[(k << 2) + 4] = FALSE_;
	    }
/* L20: */
	}
	if (iprint > 2) {
	    dscrip_(sllog_1.lc, lplc, &type__[5], sllog_1.reg, cspec, cev, &
		    slclss_1.cutoff, &lastev, &slreal_1.a1, &slreal_1.a1p, &
		    slreal_1.a2, &slreal_1.a2p, &slreal_1.b1, &slreal_1.b2);
	}
    } else {
	sllog_1.lnf = FALSE_;
	slclss_1.kclass[0] = 0;
	slclss_1.kclass[1] = 0;
	for (k = 1; k <= 2; ++k) {
	    sllog_1.reg[k - 1] = type__[(k << 2) + 1];
	    sllog_1.lc[k - 1] = type__[(k << 2) + 2];
	    sllog_1.osc[k - 1] = type__[(k << 2) + 4];
	    cspec[k - 1] = ! (type__[(k << 2) + 3] || type__[(k << 2) + 4]);
/* L25: */
	}
	if (! sllog_1.afin) {
	    store[1] = (float)-99999.;
	}
	if (! sllog_1.bfin) {
	    store[slint_1.nxinit] = (float)99999.;
	}
    }

/*     Use NSGNF to hold the sign of F when EV is large negative. */

    sgn = slreal_1.a2p * slreal_1.b2;
    if (sgn != 0.) {
	slint_1.nsgnf = (integer) d_sign(&c_b8, &sgn);
    } else {
	sgn = slreal_1.a1p * slreal_1.b2 + slreal_1.a2p * slreal_1.b1;
	if (sgn != 0.) {
	    slint_1.nsgnf = (integer) d_sign(&c_b8, &sgn);
	} else {
	    sgn = slreal_1.a1p * slreal_1.b1 + slreal_1.a2 * slreal_1.b2;
	    if (sgn != 0.) {
		slint_1.nsgnf = (integer) d_sign(&c_b8, &sgn);
	    } else {
		sgn = slreal_1.a1 * slreal_1.b2 + slreal_1.a2 * slreal_1.b1;
		if (sgn != 0.) {
		    slint_1.nsgnf = (integer) d_sign(&c_b8, &sgn);
		} else {
		    d__1 = slreal_1.a1 * slreal_1.b1;
		    slint_1.nsgnf = (integer) d_sign(&c_b8, &d__1);
		}
	    }
	}
    }
    oscill = ! type__[5] && type__[8] || ! type__[9] && type__[12];
    tol1 = tol[1] + tol[2];
    if (job[1] || job[2]) {

/*        Set up approximating regular problems for eigenvalues. */
//throw SolveIt_ERROR("")

	if (oscill) {
	    if (iprint >= 1) {
		s_wsfe(&io___26);
		e_wsfe();
		s_wsfe(&io___27);
		e_wsfe();
	    }
	    slint_1.flag__ = -20;
	    goto L120;
	}
	if (domesh) {

/*           Calculate the initial mesh. */

	    k = slint_1.nxinit + 16;
	    mesh_(&job[5], &c_n1, &store[1], &store[k], &store[(k << 1) + 1], 
		    &store[k * 3 + 1], &store[(k << 2) + 1], &tol1, &hmin);
	    if (slint_1.flag__ < 0) {
		goto L120;
	    }
	}
	if ((slclss_1.kclass[0] == 3 || slclss_1.kclass[1] == 3) && job[5]) {
	    lmesh = TRUE_;
	}
	if (! lmesh && iprint >= 1) {
	    s_wsfe(&io___28);
	    i__1 = slint_1.nxinit;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(doublereal))
			;
	    }
	    e_wsfe();
	    s_wsfe(&io___29);
	    i__1 = slint_1.nxinit;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(doublereal))
			;
	    }
	    e_wsfe();
	}
	if (job[5]) {
	    *numx = slint_1.nxinit;
	}

/*        Set MAXLVL, the maximum number of levels (mesh bisections). */

/*        IMPORTANT NOTE: the size of various fixed arrays in this */
/*        package depends on the value of MAXLVL in this FORTRAN77 */
/*        implementation.  If MAXLVL is increased, then more storage */
/*        may have to be allocated to these arrays.  In particular, */
/*        check RATIO(*), R(*,*), and W(*,*) in EXTRAP; EVEXT(*) */
/*        in REGULR. */

	slint_1.maxlvl = 10;
//throw SolveIt_ERROR("")

	i__1 = nev;
	for (k = 1; k <= i__1; ++k) {
	    ev[k] = 0.;
	    iflag[k] = 0;
	    slint_1.flag__ = 0;
	    regulr_(&job[2], &lmesh, &tol[1], &invec[k + 3], &ev[k], &iprint, 
		    &nextrp, &xef[1], &ef[*numx * (k - 1) + 1], &pdef[*numx * 
		    (k - 1) + 1], &hmin, &store[1]);
	    if ((cspec[0] || cspec[1]) && iprint >= 1 && ! job[4] && 
		    slint_1.flag__ > -5) {
		if (ev[k] >= slclss_1.cutoff || lastev != -5 && invec[k + 3] 
			>= lastev) {
		    s_wsfe(&io___30);
		    do_fio(&c__1, (char *)&invec[k + 3], (ftnlen)sizeof(
			    integer));
		    e_wsfe();
		    s_wsfe(&io___31);
		    do_fio(&c__1, (char *)&invec[k + 3], (ftnlen)sizeof(
			    integer));
		    e_wsfe();
		}
	    }
	    if (sllog_1.lflag[0]) {
		iflag[k] += ibase;
		sllog_1.lflag[0] = FALSE_;
		lbase = TRUE_;
	    }
	    if (slint_1.flag__ < 0) {
		iflag[k] = slint_1.flag__;
	    }
/* L45: */
	}
	if (lbase) {
	    ibase *= 10;
	    lbase = FALSE_;
	}
    }
    if (job[3]) {
	if (cspec[0] && cspec[1]) {
	    iflag[1] = -5;
	    if (iprint > 0) {
		s_wsfe(&io___32);
		e_wsfe();
	    }
	    goto L120;
	}
	if (! (cspec[0] || cspec[1])) {
	    iflag[1] = -4;
	    if (iprint > 0) {
		s_wsfe(&io___33);
		e_wsfe();
	    }
	    goto L120;
	}
	if (cspec[0] && (slclss_1.kclass[1] == 5 || slclss_1.kclass[1] == 9) 
		|| cspec[1] && (slclss_1.kclass[0] == 5 || slclss_1.kclass[0] 
		== 9)) {
	    iflag[1] = -6;
	    if (iprint > 0) {
		s_wsfe(&io___34);
		e_wsfe();
	    }
	    goto L120;
	}
	if (cspec[0] && ! sllog_1.bfin || cspec[1] && ! sllog_1.afin) {
	    iflag[1] = -6;
	    if (iprint > 0) {
		s_wsfe(&io___35);
		e_wsfe();
	    }
	    goto L120;
	}
	if (oscill) {
	    slint_1.flag__ = -25;
	    if (iprint > 0) {
		s_wsfe(&io___36);
		e_wsfe();
		s_wsfe(&io___37);
		e_wsfe();
	    }
	    goto L120;
	}
	d__1 = max(tol[1],tol[2]);
	xtol = -d_lg10(&d__1);
	jtol = (integer) (xtol - .5);
/* Computing MIN */
	i__1 = max(jtol,1);
	jtol = min(i__1,5);
	densop = (doublereal) (jtol * 3);
	maxits = (15 - jtol) / 3;

/*        Set Maxlvl for the density function calculation; see above */
/*        "IMPORTANT NOTE" if this is to be increased. */

	slint_1.maxlvl = (jtol + 7) / 2;
	aafin = sllog_1.afin;
	aa = slreal_1.a;
	kcl1 = slclss_1.kclass[0];
	bbfin = sllog_1.bfin;
	bb = slreal_1.b;
	kcl2 = slclss_1.kclass[1];
	if (job[5]) {

/*           Use interval truncation in this oscillatory regime. */

	    oscill = FALSE_;
	    if (! job[4] && (slclss_1.kclass[0] == 1 || slclss_1.kclass[1] == 
		    1)) {
		oscill = TRUE_;
	    }
	    if (! oscill) {
		slint_1.nxinit = (jtol << 2) + 5;
		endfac = endi[jtol - 1];
	    } else {
		slint_1.nxinit = jtol * 24 + 36;
		endfac = (float)48.;
	    }
	    if (cspec[0]) {
		if (sllog_1.afin) {
		    slclss_1.kclass[0] = 7;
		    endfac *= (float)4.;
		    if (sllog_1.bfin) {
			slreal_1.a = aa + (bb - aa) / endfac;
		    } else {
			slreal_1.a = aa + abs(aa) / endfac;
		    }
		} else {
		    sllog_1.afin = TRUE_;
		    slclss_1.kclass[0] = 0;
		    if (sllog_1.bfin) {
/* Computing MIN */
			d__1 = -slreal_1.b;
			slreal_1.a = -endfac - min(d__1,0.);
		    } else {
			slreal_1.a = -endfac;
		    }
		}
	    } else {
		if (sllog_1.bfin) {
		    slclss_1.kclass[1] = 7;
		    endfac *= (float)4.;
		    if (sllog_1.afin) {
			slreal_1.b = bb - (bb - aa) / endfac;
		    } else {
			slreal_1.b = bb - abs(bb) / endfac;
		    }
		} else {
		    sllog_1.bfin = TRUE_;
		    slclss_1.kclass[1] = 0;
		    if (sllog_1.afin) {
			slreal_1.b = endfac + max(slreal_1.a,0.);
		    } else {
			slreal_1.b = endfac;
		    }
		}
	    }
	} else {
	    if (cspec[0]) {
		sllog_1.afin = TRUE_;
	    }
	    if (cspec[1]) {
		sllog_1.bfin = TRUE_;
	    }
	    if (*numx == 0) {
		iflag[1] = -30;
		return 0;
	    }
	}
	maxt = numt;

/*        Loop over the choices of intervals. */

	numev = 0;
	i__1 = maxits;
	for (k = 1; k <= i__1; ++k) {
	    store[1] = slreal_1.a;
	    store[slint_1.nxinit] = slreal_1.b;
	    sllog_1.lflag[2] = FALSE_;
	    slint_1.flag__ = 0;
	    if (iprint >= 1) {
		s_wsfe(&io___50);
		do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
		e_wsfe();
		s_wsfe(&io___51);
		do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
		e_wsfe();
		s_wsfe(&io___52);
		do_fio(&c__1, (char *)&slreal_1.a, (ftnlen)sizeof(doublereal))
			;
		do_fio(&c__1, (char *)&slreal_1.b, (ftnlen)sizeof(doublereal))
			;
		do_fio(&c__1, (char *)&slint_1.nxinit, (ftnlen)sizeof(integer)
			);
		e_wsfe();
	    }
	    if (job[5]) {
		i__ = slint_1.nxinit + 16;
		mesh_(&c_true, &c_n1, &store[1], &store[i__ + 1], &store[(i__ 
			<< 1) + 1], &store[i__ * 3 + 1], &store[(i__ << 2) + 
			1], &tol1, &hmin);
	    }
	    if (iprint >= 3) {
		s_wsfe(&io___53);
		i__2 = slint_1.nxinit;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(
			    doublereal));
		}
		e_wsfe();
		s_wsfe(&io___54);
		i__2 = slint_1.nxinit;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(
			    doublereal));
		}
		e_wsfe();
	    }
	    densef_(&tol[1], cspec, &iprint, &k, &nextrp, &maxt, &t[1], &rho[
		    1], &iev, &hmin, &numev, &store[1]);
	    if (slint_1.flag__ == -3) {
		sllog_1.lflag[5] = FALSE_;
		slint_1.flag__ = 0;
	    }
	    if (slint_1.flag__ < 0) {
		goto L120;
	    }
	    if (! job[5]) {
		goto L110;
	    }
	    if (k > 1) {
		done = TRUE_;
		j = maxt;
		i__2 = j;
		for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
		    d__1 = tol[1], d__2 = tol[2] * rho[i__];
		    rhotol = zeta * 2. * max(d__1,d__2);
		    error = rho[i__] - store[(slint_1.maxlvl + 2) * numt + 
			    2320 + i__];
		    if (abs(error) <= rhotol) {
			edone = TRUE_;
		    } else {
			edone = FALSE_;
			maxt = i__;
		    }
		    done = done && edone;
/* L80: */
		}
		if (done) {
		    goto L110;
		}
	    }
	    if (iprint >= 2) {
		s_wsfe(&io___62);
		e_wsfe();
		s_wsfe(&io___63);
		e_wsfe();
		i__2 = numt;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    s_wsfe(&io___64);
		    do_fio(&c__1, (char *)&t[i__], (ftnlen)sizeof(doublereal))
			    ;
		    do_fio(&c__1, (char *)&rho[i__], (ftnlen)sizeof(
			    doublereal));
		    e_wsfe();
		    s_wsfe(&io___65);
		    do_fio(&c__1, (char *)&t[i__], (ftnlen)sizeof(doublereal))
			    ;
		    do_fio(&c__1, (char *)&rho[i__], (ftnlen)sizeof(
			    doublereal));
		    e_wsfe();
/* L95: */
		}
	    }
	    i__2 = maxt;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		store[(slint_1.maxlvl + 2) * numt + 2320 + i__] = rho[i__];
/* L100: */
	    }
	    sllog_1.countz = TRUE_;
	    shoot_(&slclss_1.cutoff, &store[1], &mu1, &fz);
	    shoot_(&t[numt], &store[1], &mu2, &fz);
	    sllog_1.countz = FALSE_;
	    if (t[numt] > slclss_1.cutoff) {
		dens = (mu2 - mu1) / (k * (t[numt] - slclss_1.cutoff));
	    } else {
		dens = densop;
	    }
	    if (! oscill) {
		slint_1.nxinit += 10;
		zeta = zetai[jtol - 1];
	    } else {
		zeta = (float)2.;
		slint_1.nxinit = (integer) (zeta * slint_1.nxinit);
	    }
	    if (cspec[0]) {
		if (aafin) {
		    endfac = zeta * (float)5.;
		    if (dens < denslo) {
			endfac = (float)75.;
		    }
		    if (dens > denshi && ! oscill) {
			endfac = (float)8.;
		    }
		    slreal_1.a = aa + (slreal_1.a - aa) / endfac;
/* Computing 2nd power */
		    d__1 = aa - slreal_1.a;
		    if (d__1 * d__1 < slreal_1.u) {
			slint_1.flag__ = -9;
			goto L110;
		    }
		} else {
		    if (dens < denslo) {
			zeta = (float)2.;
		    }
		    if (dens > denshi && ! oscill) {
			zeta = (float)1.4;
		    }
		    slreal_1.a = zeta * slreal_1.a;
		}
	    }
	    if (cspec[1]) {
		if (bbfin) {
		    endfac = zeta * (float)5.;
		    if (dens < denslo) {
			endfac = (float)75.;
		    }
		    if (dens > denshi && ! oscill) {
			endfac = (float)8.;
		    }
		    slreal_1.b = bb - (bb - slreal_1.b) / endfac;
/* Computing 2nd power */
		    d__1 = slreal_1.b - bb;
		    if (d__1 * d__1 < slreal_1.u) {
			slint_1.flag__ = -9;
			goto L110;
		    }
		} else {
		    if (dens < denslo) {
			zeta = (float)2.;
		    }
		    if (dens > denshi && ! oscill) {
			zeta = (float)1.4;
		    }
		    slreal_1.b = zeta * slreal_1.b;
		}
	    }
	    if (slint_1.nxinit % 2 == 0) {
		++slint_1.nxinit;
	    }
	    slint_1.nxinit = min(464,slint_1.nxinit);
/* L105: */
	}
	slint_1.flag__ = -3;
L110:
	if (iprint >= 1) {
	    s_wsfe(&io___70);
	    do_fio(&c__1, (char *)&numev, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	if (cspec[0]) {
	    if (aafin) {
		slreal_1.a = aa;
		slclss_1.kclass[0] = kcl1;
	    } else {
		sllog_1.afin = FALSE_;
	    }
	}
	if (cspec[1]) {
	    if (bbfin) {
		slreal_1.b = bb;
		slclss_1.kclass[1] = kcl2;
	    } else {
		sllog_1.bfin = FALSE_;
	    }
	}
    }

/*     Set fatal output flags. */

L120:
    if (slint_1.flag__ < -9) {
	i__1 = max(nev,1);
	for (k = 1; k <= i__1; ++k) {
	    iflag[k] = slint_1.flag__;
/* L125: */
	}
	return 0;
    } else {
	if (slint_1.flag__ < 0 && ! (job[1] || job[2])) {
	    iflag[1] = slint_1.flag__;
	}
    }

/*     Set warning flags. */

    for (i__ = 2; i__ <= 5; ++i__) {
	i__1 = max(nev,1);
	for (k = 1; k <= i__1; ++k) {
	    if (sllog_1.lflag[i__ - 1] && iflag[k] >= 0) {
		iflag[k] += i__ * ibase;
	    }
/* L130: */
	}
	if (sllog_1.lflag[i__ - 1]) {
	    ibase *= 10;
	}
/* L135: */
    }
    return 0;
} /* sledge_ */

/* ======================================================================= */
/* Subroutine */ int interv_(logical *first, doublereal *alpha, doublereal *
	beta, doublereal *cons, logical *endfin, integer *nfirst, integer *
	ntotal, integer *iflag, doublereal *x)
{
    static integer k;
    static doublereal v;
    static integer i1, i2, i3, j1, j2, j3, mu;
    static doublereal cev[2], tol[6];
    static logical lplc[2];
    static doublereal hmin;
    extern /* Subroutine */ int mesh_(logical *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer numx;
    static logical cspec[2];
    extern /* Subroutine */ int class_(integer *, doublereal *, logical *, 
	    logical *, doublereal *, integer *, logical *, doublereal *, 
	    logical *, doublereal *, logical *);
    static integer nlast;
    static logical jobst[3];
    extern /* Subroutine */ int shoot_(doublereal *, doublereal *, integer *, 
	    doublereal *), start_(logical *, doublereal *, doublereal *, 
	    integer *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *);
    static integer level0;
    static logical domesh;
    static doublereal cutoff;
    static integer lastev, idummy[1], nextrp;

/* ********************************************************************** */
/*                                                                     * */
/*     INTERV calculates the indices of eigenvalues found in a         * */
/*     specified interval.                                             * */
/*                                                                     * */
/* ********************************************************************** */
/*     Local variables: */


    /* Parameter adjustments */
    --x;
    --endfin;
    --cons;

    /* Function Body */
    *nfirst = -5;
    nlast = -5;
    *iflag = 0;
    if (*alpha >= *beta) {
	*iflag = -9;
	return 0;
    }
    tol[0] = (float)1e-6;
    tol[1] = (float)1e-6;
    if (*first) {
	jobst[0] = FALSE_;
	jobst[1] = FALSE_;
	jobst[2] = FALSE_;
	sllog_1.afin = endfin[1];
	sllog_1.bfin = endfin[2];
	slint_1.maxlvl = 10;
	numx = 0;
	start_(jobst, &cons[1], tol, &c__0, idummy, &numx, &x[1], &c__0, &x[1]
		, &nextrp, &x[1]);
	class_(&c__0, tol, &jobst[1], cspec, cev, &lastev, lplc, &x[1], &
		c_true, &hmin, &domesh);
	cutoff = min(cev[0],cev[1]);
	if (slint_1.flag__ < 0) {
	    *iflag = slint_1.flag__;
	    return 0;
	}
	if (sllog_1.osc[0] || sllog_1.osc[1]) {
	    *iflag = -25;
	    return 0;
	}
	if (domesh) {
	    k = numx + 16;
	    mesh_(&c_true, &c_n1, &x[1], &x[k], &x[(k << 1) + 1], &x[k * 3 + 
		    1], &x[(k << 2) + 1], tol, &hmin);
	}
    }
    if (slint_1.flag__ < 0) {
	*iflag = slint_1.flag__;
	return 0;
    }
    level0 = slint_1.level;
    sllog_1.countz = TRUE_;
    slint_1.level = 3;
    shoot_(alpha, &x[1], &mu, &v);
    i1 = mu;
    shoot_(beta, &x[1], &mu, &v);
    j1 = mu;
    ++slint_1.level;
    shoot_(alpha, &x[1], &mu, &v);
    i2 = mu;
    shoot_(beta, &x[1], &mu, &v);
    j2 = mu;
L10:
    ++slint_1.level;
    if (*nfirst == -5) {
	shoot_(alpha, &x[1], &mu, &v);
	i3 = mu;
	if (i1 == i2 && i2 == i3) {
	    *nfirst = i1;
	    goto L15;
	}
	i1 = i2;
	i2 = i3;
    }
L15:
    if (nlast == -5) {
	shoot_(beta, &x[1], &mu, &v);
	j3 = mu;
	if (j1 == j2 && j2 == j3) {
	    nlast = j1 - 1;
	    if (*nfirst != -5) {
		goto L20;
	    }
	}
	j1 = j2;
	j2 = j3;
    }
    if (slint_1.level < slint_1.maxlvl) {
	goto L10;
    }
L20:
    if (*nfirst == -5) {
	*nfirst = i3;
	*iflag = 12;
    }
    if (nlast == -5) {
	nlast = j3;
	*iflag = 12;
    }
    *ntotal = nlast + 1 - *nfirst;
    if (*ntotal == 0) {
	*iflag = 11;
    }
    if (*beta > cutoff) {
	*iflag = 13;
    }
    sllog_1.countz = FALSE_;
    slint_1.level = level0;
    return 0;
} /* interv_ */

/* /////////////////////////////////////////////////////////////////////// */
/* Subroutine */ int aitken_(doublereal *xlim, doublereal *tol, integer *n, 
	doublereal *x, doublereal *error)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Local variables */
    static integer i__;
    static doublereal xold, denom;


/*     Use Aitken's algorithm to accelerate convergence of the sequence */
/*     in X(*). */


    /* Parameter adjustments */
    --x;

    /* Function Body */
    if (*n <= 2) {
	*xlim = x[*n];
	*error = 0.;
	return 0;
    }
    xold = 1e30;
    i__1 = *n - 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	denom = x[i__ + 2] - x[i__ + 1] * 2. + x[i__];
	if (denom != 0.) {
/* Computing 2nd power */
	    d__1 = x[i__ + 1] - x[i__];
	    *xlim = x[i__] - d__1 * d__1 / denom;
	    *error = *xlim - xold;
	} else {
	    *error = x[i__ + 2] - x[i__ + 1];
	    *xlim = x[i__ + 2];
	}
/* Computing MAX */
	d__1 = 1., d__2 = abs(*xlim);
	if (abs(*error) < max(d__1,d__2) * *tol) {
	    return 0;
	}
	xold = *xlim;
/* L10: */
    }
    return 0;
} /* aitken_ */

/* ---------------------------------------------------------------------- */
doublereal asymev_(integer *nev, doublereal *qint, doublereal *rpint, 
	doublereal *alpha1, doublereal *alpha2, doublereal *beta1, doublereal 
	*beta2)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Local variables */
    static doublereal fnev;



/*     Evaluate the asymptotic formula for eigenvalue NEV. */
/*        Note: not all cases have been implemented yet. */

    ret_val = (float)-999999.;
    fnev = (doublereal) (*nev);
    if (sllog_1.reg[0]) {
	if (slreal_1.a1p != 0. || slreal_1.a2p != 0.) {
	    if (slreal_1.a2p != 0.) {
		ret_val = (slreal_1.a1p * 2. / slreal_1.a2p + *qint) / *rpint;
		if (slreal_1.b2 != 0.) {
/*                 Case 1 */
/* Computing 2nd power */
		    d__1 = (fnev - 1.) * 3.14159265358979324 / *rpint;
		    ret_val = ret_val + slreal_1.b1 * 2. / (slreal_1.b2 * *
			    rpint) + d__1 * d__1;
		} else {
/*                 Case 2 */
/* Computing 2nd power */
		    d__1 = (fnev - .5) * 3.14159265358979324 / *rpint;
		    ret_val += d__1 * d__1;
		}
	    } else {
		ret_val = (slreal_1.a2 * 2. / slreal_1.a1p + *qint) / *rpint;
		if (slreal_1.b2 != 0.) {
/*                 Case 3 */
/* Computing 2nd power */
		    d__1 = (fnev - .5) * 3.14159265358979324 / *rpint;
		    ret_val = ret_val + slreal_1.b1 * 2. / (slreal_1.b2 * *
			    rpint) + d__1 * d__1;
		} else {
/*                 Case 4 */
/* Computing 2nd power */
		    d__1 = fnev * 3.14159265358979324 / *rpint;
		    ret_val += d__1 * d__1;
		}
	    }
	} else {
	    if (slreal_1.a2 != 0.) {
		if (slreal_1.b2 != 0.) {
/*                 Case 1 */
/* Computing 2nd power */
		    d__1 = fnev * 3.14159265358979324 / *rpint;
		    ret_val = d__1 * d__1 + ((*beta1 / *beta2 + *alpha1 / *
			    alpha2) * 2. + *qint) / *rpint;
		} else {
/*                 Case 2   (Dirichlet at B) */
/* Computing 2nd power */
		    d__1 = (fnev + .5) * 3.14159265358979324 / *rpint;
		    ret_val = d__1 * d__1 + (*alpha1 * 2. / *alpha2 + *qint) /
			     *rpint;
		}
	    } else {
		if (slreal_1.b2 != 0.) {
/*                 Case 3   (Dirichlet at A) */
/* Computing 2nd power */
		    d__1 = (fnev + .5) * 3.14159265358979324 / *rpint;
		    ret_val = d__1 * d__1 + (*beta1 * 2. / *beta2 + *qint) / *
			    rpint;
		} else {
/*                 Case 4   (Dirichlet at A and at B) */
/* Computing 2nd power */
		    d__1 = (fnev + 1.) * 3.14159265358979324 / *rpint;
		    ret_val = d__1 * d__1 + *qint / *rpint;
		}
	    }
	}
	return ret_val;
    }
    if (sllog_1.reg[1]) {
	if (slreal_1.b2 != 0.) {
	    if (slreal_1.a2 != 0.) {
/*              Case 1 */
/* Computing 2nd power */
		d__1 = fnev * 3.14159265358979324 / *rpint;
		ret_val = d__1 * d__1 + ((*alpha1 / *alpha2 + *beta1 / *beta2)
			 * 2. + *qint) / *rpint;
	    } else {
/*              Case 2   (Dirichlet at A) */
/* Computing 2nd power */
		d__1 = (fnev + .5) * 3.14159265358979324 / *rpint;
		ret_val = d__1 * d__1 + (*beta1 * 2. / *beta2 + *qint) / *
			rpint;
	    }
	} else {
	    if (slreal_1.b2 != 0.) {
/*              Case 3   (Dirichlet at B) */
/* Computing 2nd power */
		d__1 = (fnev + .5) * 3.14159265358979324 / *rpint;
		ret_val = d__1 * d__1 + (*alpha1 * 2. / *alpha2 + *qint) / *
			rpint;
	    } else {
/*              Case 4   (Dirichlet at A and at B) */
/* Computing 2nd power */
		d__1 = (fnev + 1.) * 3.14159265358979324 / *rpint;
		ret_val = d__1 * d__1 + *qint / *rpint;
	    }
	}
    }
    return ret_val;
} /* asymev_ */

/* ---------------------------------------------------------------------- */
doublereal asymr_(integer *nev, doublereal *rpint, doublereal *rpata, 
	doublereal *rpatb, doublereal *scale)
{
    /* System generated locals */
    doublereal ret_val, d__1, d__2, d__3;

    /* Local variables */
    static doublereal fnev;



/*     Evaluate the asymptotic formula for RsubNEV. */
/*        Note: not all cases have been implemented yet.  See the note */
/*        above in ASYMEV. */

    ret_val = 0.;
    fnev = (doublereal) max(*nev,2);
    if (sllog_1.reg[0]) {
	if (slreal_1.a1p != 0. || slreal_1.a2p != 0.) {
	    if (slreal_1.a2p != 0.) {
		if (slreal_1.b2 != 0.) {
/* Computing 3rd power */
		    d__1 = *rpint;
/* Computing 2nd power */
		    d__2 = slreal_1.a2p;
/* Computing 4th power */
		    d__3 = (fnev - 1.) * 3.14159265358979324, d__3 *= d__3;
		    ret_val = d__1 * (d__1 * d__1) * 2. / (*rpata * (d__2 * 
			    d__2) * (d__3 * d__3));
		} else {
/* Computing 3rd power */
		    d__1 = *rpint;
/* Computing 2nd power */
		    d__2 = slreal_1.a2p;
/* Computing 4th power */
		    d__3 = (fnev - .5) * 3.14159265358979324, d__3 *= d__3;
		    ret_val = d__1 * (d__1 * d__1) * 2. / (*rpata * (d__2 * 
			    d__2) * (d__3 * d__3));
		}
	    } else {
		if (slreal_1.b2 != 0.) {
/* Computing 2nd power */
		    d__1 = slreal_1.a1p * (fnev - .5) * 3.14159265358979324;
		    ret_val = *rpata * 2. * *rpint / (d__1 * d__1);
		} else {
/* Computing 2nd power */
		    d__1 = slreal_1.a1p * fnev * 3.14159265358979324;
		    ret_val = *rpata * 2. * *rpint / (d__1 * d__1);
		}
	    }
	} else {
	    if (slreal_1.a2 != 0.) {
		ret_val = 2. / (*rpata * slreal_1.a2 * slreal_1.a2 * *rpint);
	    } else {
		if (slreal_1.b2 != 0.) {
/* Computing 2nd power */
		    d__1 = (fnev + .5) * 3.14159265358979324 / slreal_1.a1;
/* Computing 3rd power */
		    d__2 = *rpint;
		    ret_val = *rpata * 2. * (d__1 * d__1) / (d__2 * (d__2 * 
			    d__2));
		} else {
/* Computing 2nd power */
		    d__1 = (fnev + 1.) * 3.14159265358979324 / slreal_1.a1;
/* Computing 3rd power */
		    d__2 = *rpint;
		    ret_val = *rpata * 2. * (d__1 * d__1) / (d__2 * (d__2 * 
			    d__2));
		}
	    }
	}
	return ret_val;
    }
    if (sllog_1.reg[1]) {
	if (slreal_1.a2 != 0.) {
	    ret_val = 2. / (*rpatb * slreal_1.b2 * slreal_1.b2 * *rpint);
	} else {
	    if (slreal_1.b2 != 0.) {
/* Computing 2nd power */
		d__1 = (fnev + .5) * 3.14159265358979324 / slreal_1.b1;
/* Computing 3rd power */
		d__2 = *rpint;
		ret_val = *rpatb * 2. * (d__1 * d__1) / (d__2 * (d__2 * d__2))
			;
	    } else {
/* Computing 2nd power */
		d__1 = (fnev + 1.) * 3.14159265358979324 / slreal_1.b1;
/* Computing 3rd power */
		d__2 = *rpint;
		ret_val = *rpatb * 2. * (d__1 * d__1) / (d__2 * (d__2 * d__2))
			;
	    }
	}
    }
/* Computing 2nd power */
    d__1 = *scale;
    ret_val *= d__1 * d__1;
    return ret_val;
} /* asymr_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int brcket_(integer *n, doublereal *evlow, doublereal *
	evhigh, doublereal *flow, doublereal *fhigh, doublereal *abserr, 
	doublereal *relerr, doublereal *x)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    /* Local variables */
    static integer k;
    static doublereal ev;
    static integer mu;
    static doublereal fev;
    static logical low;
    static doublereal diff;
    static logical high;
    extern /* Subroutine */ int shoot_(doublereal *, doublereal *, integer *, 
	    doublereal *);
    static doublereal evsign;


/*     Find values for EVLOW and EVHIGH which bracket the Nth eigenvalue; */
/*     in particular, */
/*           EV(N-1) < EVLOW < EV(N) < EVHIGH < EV(N+1)  . */
/*     It is assumed that if U(X,LAMBDA) has NZ zeros in (A,B) then */
/*           EV(MU-1) < LAMBDA < EV(MU) */
/*     where MU is a function of NZ, LAMBDA, and the constants in the */
/*     boundary conditions.  The value of MU for a given LAMBDA is */
/*     returned by a call to subprogram SHOOT. */


/*     Set COUNTZ so that zeros are counted in SHOOT. */

    /* Parameter adjustments */
    --x;

    /* Function Body */
    sllog_1.countz = TRUE_;
    evsign = (doublereal) slint_1.nsgnf;

/*     SHOOT with Ev = Evlow should return FEV having sign EVSIGN. */

    if (*n != *n / 2 << 1) {
	evsign = -evsign;
    }
    low = FALSE_;
    high = FALSE_;

/*     Make EVLOW a lower bound for EV(N). */

    ev = *evlow;
    diff = (d__1 = *evhigh - *evlow, abs(d__1));
    if (diff == 0.) {
	diff = *abserr + *relerr;
    }
L10:
    shoot_(&ev, &x[1], &mu, &fev);
    if (slint_1.flag__ < 0) {
	sllog_1.countz = FALSE_;
	return 0;
    }
    if (mu > *n) {
	*evhigh = ev;
	*fhigh = fev;
	ev -= diff;
	diff *= 2.;
	if (mu == *n + 1 && evsign * fev <= 0.) {
	    high = TRUE_;
	}
	goto L10;
    } else {
	*evlow = ev;
	*flow = fev;
	if (mu == *n && evsign * fev >= 0.) {
	    low = TRUE_;
	}
    }

/*     Make EVHIGH an upper bound for EV(N). */

    if (! high) {
	ev = *evhigh;
	diff = (d__1 = *evhigh - *evlow, abs(d__1));
L20:
	shoot_(&ev, &x[1], &mu, &fev);
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	i__1 = mu % 2;
	k = slint_1.nsgnf * pow_ii(&c_n1, &i__1);
	if (k * fev < 0.) {
	    ev += diff;
	    diff *= 2.;
	    goto L20;
	} else {
	    if (mu <= *n) {
		*evlow = ev;
		*flow = fev;
		ev += diff;
		diff *= 2.;
		goto L20;
	    } else {
		*evhigh = ev;
		*fhigh = fev;
		if (mu == *n + 1) {
		    high = TRUE_;
		}
	    }
	}
    }

/*     Refine the interval [EVLOW,EVHIGH] to include only the Nth */
/*     eigenvalue. */

L30:
    if (! low || ! high) {
	diff = *evhigh - *evlow;
	ev = *evlow + diff / 2.;

/*        Check for a cluster of eigenvalues within user's tolerance. */

/* Computing MAX */
/* Computing MAX */
	d__3 = abs(*evlow), d__4 = abs(*evhigh);
	d__1 = *abserr, d__2 = *relerr * max(d__3,d__4);
	if (diff * 2. < max(d__1,d__2)) {
	    sllog_1.lflag[0] = TRUE_;
	    sllog_1.countz = FALSE_;
	    return 0;
	}
	shoot_(&ev, &x[1], &mu, &fev);
	if (slint_1.flag__ < 0) {
	    sllog_1.countz = FALSE_;
	    return 0;
	}

/*        Update EVLOW and EVHIGH. */

	if (mu == *n) {
	    *evlow = ev;
	    low = TRUE_;
	    *flow = fev;
	} else {
	    if (mu == *n + 1) {
		*evhigh = ev;
		high = TRUE_;
		*fhigh = fev;
	    } else {
		if (mu < *n) {
		    *evlow = ev;
		    *flow = fev;
		} else {
		    *evhigh = ev;
		    *fhigh = fev;
		}
	    }
	}
	goto L30;
    }
    sllog_1.countz = FALSE_;
    return 0;
} /* brcket_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int class_(integer *iprint, doublereal *tol, logical *job, 
	logical *cspec, doublereal *cev, integer *lastev, logical *lplc, 
	doublereal *x, logical *jmesh, doublereal *hmin, logical *domesh)
{
    /* Format strings */
    static char fmt_70[] = "(\002 When level = \002,i2,\002, Ev index at cut\
off is \002,i12)";
    static char fmt_80[] = "(\002 The eigenvalue count is uncertain.\002)";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double d_lg10(doublereal *), pow_di(doublereal *, integer *), log(
	    doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();

    /* Local variables */
    static integer j, k, m;
    static doublereal s, y[40], z__[80]	/* was [40][2] */, bc[2], ev, pz[80]	
	    /* was [40][2] */, qz[80]	/* was [40][2] */, rz[80]	/* 
	    was [40][2] */, end, fev, sgn, base;
    extern /* Subroutine */ int mesh_(logical *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer kmax, last[3];
    static doublereal over;
    static integer iflag;


//extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal *, doublereal *);
//extern "C" long _stdcall COEFF(double *x, double *px, double *qx, double *rx);
#define coeff_ COEFF


    static integer kused[2];
    extern /* Subroutine */ int shoot_(doublereal *, doublereal *, integer *, 
	    doublereal *);
    static logical endfin[2];
    extern /* Subroutine */ int clsend_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    logical *, integer *, doublereal *, doublereal *, logical *, 
	    doublereal *, doublereal *, logical *, integer *);

    /* Fortran I/O blocks */
    static cilist io___128 = { 0, 21, 0, fmt_70, 0 };
    static cilist io___129 = { 0, 21, 0, fmt_80, 0 };



/*     This routine classifies the Sturm-Liouville problem.  Note: */
/*     (1) any computational algorithm must be based on a finite */
/*     amount of information; hence, there will always be cases that */
/*     any algorithm misclassifies.  In addition, some problems are */
/*     inherently ill-conditioned, in that a small change in the */
/*     coefficients can produce a totally different classification. */
/*     (2)  The maximum number of points sampled for singular problems */
/*     is given by the variable KMAX.  By increasing this number, the */
/*     reliability of the classification may increase; however, the */
/*     computing time may also increase.  The values we have chosen */
/*     seem to be a reasonable balance for most problems. */
/*     (3) The algorithms apply standard theorems involving limits of */
/*     the Liouville normal form potential.  When this is not available, */
/*     each coefficient function is approximated by a power function */
/*     (c*x^r) and classified according to the properties of the */
/*     resulting Liouville approximation. */


/*     Sample the coefficient functions; determine if the problem as */
/*     given is in Liouville normal form. */

    /* Parameter adjustments */
    --x;
    --lplc;
    --cev;
    --cspec;

    /* Function Body */
    sllog_1.lnf = TRUE_;
    *domesh = TRUE_;
    for (j = 1; j <= 2; ++j) {
	if (j == 1) {
	    end = slreal_1.a;
	    endfin[0] = sllog_1.afin;
	    sgn = 1.;
	} else {
	    end = slreal_1.b;
	    endfin[1] = sllog_1.bfin;
	    sgn = -1.;
	}
	k = (integer) (.1 - d_lg10(tol));
/* Computing MIN */
/* Computing MAX */
	i__2 = k << 2;
	i__1 = max(i__2,10);
	kmax = min(i__1,40);
	m = 0;
/* Computing MIN */
	i__1 = max(k,4);
	base = 1. / min(i__1,8);
	if (endfin[j - 1]) {
	    if (end != 0.) {
/* Computing MIN */
		i__1 = -((integer) d_lg10(&slreal_1.u)) - 1;
		kmax = min(i__1,kmax);
	    }
	} else {
	    kmax = min(kmax,20);
	    base = 8.;
	}
	i__1 = kmax;
	for (k = 1; k <= i__1; ++k) {
	    z__[k + j * 40 - 41] = pow_di(&base, &k);
/* L10: */
	}
	over = slreal_1.under / 2.;
	i__1 = kmax;
	for (k = 1; k <= i__1; ++k) {
	    if (endfin[j - 1]) {
		s = end + sgn * z__[k + j * 40 - 41];
	    } else {
		s = -sgn * z__[k + j * 40 - 41];
	    }
	    coeff_(&s, &pz[k + j * 40 - 41], &qz[k + j * 40 - 41], &rz[k + j *
		     40 - 41]);
	    ++slint_1.ncoeff;
	    if (pz[k + j * 40 - 41] <= 0. || rz[k + j * 40 - 41] <= 0.) {
		slint_1.flag__ = -15;
		return 0;
	    }
	    if (sllog_1.lnf && k > 1) {
		if (pz[k + j * 40 - 41] != pz[k - 1 + j * 40 - 41]) {
		    sllog_1.lnf = FALSE_;
		}
		if (rz[k + j * 40 - 41] != rz[k - 1 + j * 40 - 41]) {
		    sllog_1.lnf = FALSE_;
		}
	    }
	    s = log(pz[k + j * 40 - 41]);
	    if (abs(s) > over) {
		m = 1;
	    }
	    s = log((d__1 = qz[k + j * 40 - 41], abs(d__1)) + 1.);
	    if (abs(s) > over) {
		m = 1;
	    }
	    s = log(rz[k + j * 40 - 41]);
	    if (abs(s) > over) {
		m = 1;
	    }
	    if (m != 0) {
		goto L35;
	    }
/* L30: */
	}
	k = kmax;
L35:
	kused[j - 1] = k - 1;
/* L40: */
    }
    for (j = 1; j <= 2; ++j) {
	clsend_(&z__[j * 40 - 40], &pz[j * 40 - 40], &qz[j * 40 - 40], &rz[j *
		 40 - 40], &kused[j - 1], iprint, &end, &endfin[j - 1], &j, 
		tol, &cev[j], &cspec[j], bc, y, &lplc[1], &iflag);
	if (! (*job)) {
	    if (! sllog_1.afin && j == 1) {
		x[1] = -end;
	    }
	    if (! sllog_1.bfin && j == 2) {
		x[slint_1.nxinit] = end;
	    }
	}
	if ((cspec[j] || ! sllog_1.osc[j - 1]) && ! sllog_1.reg[j - 1]) {
	    if (j == 1) {
		slreal_1.a1 = bc[0];
		slreal_1.a1p = 0.;
		slreal_1.a2 = bc[1];
		slreal_1.a2p = 0.;
	    } else {
		slreal_1.b1 = bc[0];
		slreal_1.b2 = bc[1];
	    }
	}
	if (iflag == 1) {
	    sllog_1.lflag[1] = TRUE_;
	}
/* L50: */
    }
    slclss_1.cutoff = min(cev[1],cev[2]);

/*     Find the number of eigenvalues below the start of the */
/*     continuous spectrum. */

    *lastev = -5;
    if (! sllog_1.osc[0] && ! sllog_1.lc[1] && sllog_1.osc[1]) {
	*lastev = 0;
    }
    if (! sllog_1.osc[1] && ! sllog_1.lc[0] && sllog_1.osc[0]) {
	*lastev = 0;
    }
    if (sllog_1.osc[0] && ! sllog_1.lc[0] && sllog_1.osc[1] && sllog_1.lc[1]) 
	    {
	*lastev = 0;
    }
    if (sllog_1.osc[1] && ! sllog_1.lc[1] && sllog_1.osc[0] && sllog_1.lc[0]) 
	    {
	*lastev = 0;
    }
    if (cspec[1] && sllog_1.osc[1] || cspec[2] && sllog_1.osc[0]) {
	*lastev = 0;
    }
    if (cspec[1] && ! sllog_1.osc[1] || cspec[2] && ! sllog_1.osc[0] || cspec[
	    1] && cspec[2]) {
	k = slint_1.nxinit + 16;
	mesh_(jmesh, &c_n1, &x[1], &x[k], &x[(k << 1) + 1], &x[k * 3 + 1], &x[
		(k << 2) + 1], tol, hmin);
	*domesh = FALSE_;
	sllog_1.countz = TRUE_;
	for (j = 1; j <= 2; ++j) {
	    slint_1.level = j * 3;
	    ev = slclss_1.cutoff;
	    shoot_(&ev, &x[1], &last[j - 1], &fev);
	    if (slint_1.flag__ < 0) {
		return 0;
	    }
	    if (*iprint >= 5) {
		s_wsfe(&io___128);
		do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(integer))
			;
		do_fio(&c__1, (char *)&last[j - 1], (ftnlen)sizeof(integer));
		e_wsfe();
	    }
/* L75: */
	}
	sllog_1.countz = FALSE_;
	if (last[0] >= last[1]) {
	    *lastev = last[1];
	    if (last[0] != last[1]) {
		sllog_1.lflag[1] = TRUE_;
		if (*iprint >= 3) {
		    s_wsfe(&io___129);
		    e_wsfe();
		}
		if (last[0] > last[1] << 1) {
		    *lastev = 0;
		}
	    }
	} else {
	    *lastev = -5;
	}
    }
    return 0;
} /* class_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int clsend_(doublereal *z__, doublereal *pz, doublereal *qz, 
	doublereal *rz, integer *kmax, integer *iprint, doublereal *end, 
	logical *endfin, integer *iend, doublereal *tol, doublereal *cev, 
	logical *cspec, doublereal *bc, doublereal *y, logical *lplc, integer 
	*iflag)
{
    /* Format strings */
    static char fmt_5[] = "(/,\002 For endpoint A\002)";
    static char fmt_6[] = "(/,\002 For endpoint B:\002)";
    static char fmt_7[] = "(\002  Kmax = \002,i3)";
    static char fmt_10[] = "(\002  WARNING: p(x) or r(x) is not well-approxi\
mated by\002,\002 a power potential.\002,/,\002  Classification is uncertain.\
\002)";
    static char fmt_20[] = "(\002  WARNING: q(x) is not well-approximated by\
 a power \002,\002potential.\002,/,\002  Classification is uncertain.\002)";
    static char fmt_40[] = "(\002  Cp, Ep; Cq, Eq; Cr, Er =\002,/,3(2d25.12,\
/))";
    static char fmt_50[] = "(\002  C1, C2      =\002,2d20.10,/,\002  Eqlnf, \
Eta1 =\002,2d20.10)";
    static char fmt_60[] = "(\002  C3 = \002,d19.10)";
    static char fmt_70[] = "(\002  This is an irregular singular point.\002)";
    static char fmt_75[] = "(\002  The principal Frobenius root is \002,e20.\
8)";
    static char fmt_80[] = "(\002  WARNING: borderline nonoscillatory/oscill\
ato\002,\002ry classification.\002)";
    static char fmt_85[] = "(\002  NO INFORMATION on osc/nonosc class.\002)";
    static char fmt_90[] = "(\002  WARNING: borderline Lc/Lp classification\
.\002)";
    static char fmt_95[] = "(\002  NO INFORMATION on Lp/Lc class.\002)";
    static char fmt_100[] = "(\002  NO INFORMATION on Osc/Nonosc class.\002)";
    static char fmt_105[] = "(\002  NO INFORMATION on Lp/Lc class.\002)";
    static char fmt_110[] = "(\002  Mu =\002,d20.6,\002;  Eta2 =\002,d20.6)";
    static char fmt_115[] = "(\002  D3 =\002,d20.6,\002;    D4 =\002,d20.6)";
    static char fmt_120[] = "(\002  This problem has unbounded \002,\002[Ev*\
r(x)-q(x)]/p(x).\002)";
    static char fmt_125[] = "(\002  A change of variables will be used nea\
r\002,\002 this endpoint.\002)";
    static char fmt_130[] = "(\002  Classification type (KCLASS) is: \002,i2)"
	    ;

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(), do_fio(integer *, char *, ftnlen);
    double d_sign(doublereal *, doublereal *), sqrt(doublereal), pow_dd(
	    doublereal *, doublereal *);

    /* Local variables */
    static integer i__, k;
    static doublereal c1, c2, c3, cp, cq, cr, ep, eq, er;
    static logical ex;
    static doublereal zz, sgn, tol4;
    static logical posc, qosc, rosc;
    static doublereal gamma;
    //extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal delta;
    static logical exact, irreg;
    static integer iqlnf;
    extern /* Subroutine */ int power_(doublereal *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, logical *, 
	    logical *, doublereal *, integer *);

    /* Fortran I/O blocks */
    static cilist io___130 = { 0, 21, 0, fmt_5, 0 };
    static cilist io___131 = { 0, 21, 0, fmt_6, 0 };
    static cilist io___132 = { 0, 21, 0, fmt_7, 0 };
    static cilist io___148 = { 0, 21, 0, fmt_10, 0 };
    static cilist io___149 = { 0, 21, 0, fmt_20, 0 };
    static cilist io___154 = { 0, 21, 0, fmt_40, 0 };
    static cilist io___156 = { 0, 21, 0, fmt_50, 0 };
    static cilist io___158 = { 0, 21, 0, fmt_60, 0 };
    static cilist io___161 = { 0, 21, 0, fmt_70, 0 };
    static cilist io___162 = { 0, 21, 0, fmt_75, 0 };
    static cilist io___163 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___164 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___165 = { 0, 21, 0, fmt_85, 0 };
    static cilist io___166 = { 0, 21, 0, fmt_90, 0 };
    static cilist io___167 = { 0, 21, 0, fmt_90, 0 };
    static cilist io___168 = { 0, 21, 0, fmt_95, 0 };
    static cilist io___169 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___170 = { 0, 21, 0, fmt_105, 0 };
    static cilist io___171 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___172 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___173 = { 0, 21, 0, fmt_120, 0 };
    static cilist io___174 = { 0, 21, 0, fmt_125, 0 };
    static cilist io___175 = { 0, 21, 0, fmt_130, 0 };



/*     Iflag = 0    if reasonably certain of the classification; */
/*           = 1    if not sure. */

/*     Information about the nature of the problem at singular point */
/*     IEND is passed through the variable KCLASS(IEND): */
/*         KCLASS(*) = 0    normal; */
/*                   = 1    oscillatory coefficient function; */
/*                   = 2    regular, but 1/p, q, or r unbounded; */
/*                   = 3    infinite endpoint, Eqlnf = -1 ; */
/*                   = 4    finite singular endpoint, Tau unbounded, */
/*                          (not 8-10); */
/*                   = 5    not "hard", irregular; */
/*                   = 6    "hard" irregular with Eta(1) < 0; */
/*                   = 7    finite end which generates Cspectrum; */
/*                   = 8    Q is unbounded (< 1/t^2) near a nonoscill- */
/*                          atory finite end; */
/*                   = 9    Q is unbounded (like 1/t^2) near a nonosc- */
/*                          illatory finite end; */
/*                   = 10   "hard", irregular, Eta(1) > 0. */
/*                    Note: "hard" means Tau goes to +infinity at a */
/*                          finite nonoscillatory endpoint. */
/*         REG(*) = .True.  iff endpoint is regular. */
/*         LC(*)  = .True.  iff endpoint is limit circle. */
/*         OSC(*) = .True.  iff endpoint is oscillatory for all Ev. */
/*         CSPEC  = .True.  iff endpoint generates continuous spectrum. */
/*         LPLC(*)= .True.  iff theory yields Lp/Lc classification. */


    /* Parameter adjustments */
    --lplc;
    --y;
    --bc;
    --rz;
    --qz;
    --pz;
    --z__;

    /* Function Body */
    *iflag = 0;
    if (*iprint >= 3) {
	if (*iend == 1) {
	    s_wsfe(&io___130);
	    e_wsfe();
	}
	if (*iend == 2) {
	    s_wsfe(&io___131);
	    e_wsfe();
	}
	s_wsfe(&io___132);
	do_fio(&c__1, (char *)&(*kmax), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    *cspec = FALSE_;
    irreg = FALSE_;
    lplc[*iend] = TRUE_;
    slclss_2.kclass[*iend - 1] = 0;
    slclss_2.pnu[*iend - 1] = 0.;
    *cev = 1. / slreal_1.u;
    ex = TRUE_;
    c1 = 0.;
    c2 = 0.;
    tol4 = *tol * 4.;

/*     Seek monomial approximations to each coefficient function. */

    power_(&z__[1], &qz[1], kmax, tol, iprint, &eq, &cq, &qosc, &exact, &y[1],
	     iflag);
    if (abs(cq) <= *tol) {
	cq = 0.;
    }
    if (cq == 0.) {
	eq = 0.;
    }
    if (sllog_1.lnf) {
	ep = 0.;
	cp = pz[1];
	er = 0.;
	cr = rz[1];
	posc = FALSE_;
	rosc = FALSE_;
    } else {
	power_(&z__[1], &pz[1], kmax, tol, iprint, &ep, &cp, &posc, &exact, &
		y[1], iflag);
	if (abs(cp) <= *tol) {
	    ep = 0.;
	}
	ex = ex && exact;
	power_(&z__[1], &rz[1], kmax, tol, iprint, &er, &cr, &rosc, &exact, &
		y[1], iflag);
	if (abs(cr) <= *tol) {
	    er = 0.;
	}
    }
    if (posc || rosc) {
	if (*endfin) {
	    sllog_1.reg[*iend - 1] = TRUE_;
	} else {
	    *iflag = 1;
	    if (*iprint >= 3) {
		s_wsfe(&io___148);
		e_wsfe();
	    }
	    sllog_1.reg[*iend - 1] = FALSE_;
	    slclss_2.kclass[*iend - 1] = 1;
	}
	sllog_1.lc[*iend - 1] = TRUE_;
	sllog_1.osc[*iend - 1] = FALSE_;
    }
    if (qosc) {
	*iflag = 1;
	if (*iprint >= 3) {
	    s_wsfe(&io___149);
	    e_wsfe();
	}
	if (*endfin) {
	    sllog_1.reg[*iend - 1] = TRUE_;
	    sllog_1.lc[*iend - 1] = TRUE_;
	    sllog_1.osc[*iend - 1] = FALSE_;
	} else {
	    slclss_2.kclass[*iend - 1] = 1;
	    sllog_1.reg[*iend - 1] = FALSE_;
	    sllog_1.lc[*iend - 1] = FALSE_;
	    *cspec = TRUE_;
	    sllog_1.osc[*iend - 1] = FALSE_;
	    bc[1] = 1.;
	    bc[2] = 0.;
	    *cev = qz[*kmax - 1];
	    k = 40;
	    delta = (z__[*kmax] - z__[*kmax - 1]) / (k + 1);
	    i__1 = k;
	    for (i__ = 0; i__ <= i__1; ++i__) {
		zz = z__[*kmax] - i__ * delta;
		coeff_(&zz, &cp, &cq, &cr);
		++slint_1.ncoeff;
		*cev = min(*cev,cq);
/* L30: */
	    }
	    if (abs(*cev) < tol4) {
		*cev = 0.;
	    }
	    if (slreal_1.u * abs(*cev) >= 1.) {
		*cspec = FALSE_;
	    }
	}
	slclss_2.eqlnf[*iend - 1] = 0.;
    }
    if (*iprint >= 3) {
	s_wsfe(&io___154);
	do_fio(&c__1, (char *)&cp, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ep, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&cq, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&eq, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&cr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&er, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    slclss_2.cpt[*iend - 1] = cp;
    slclss_2.ept[*iend - 1] = ep;

/*     Analyze this endpoint. */

    if (ep < 1. && eq > -1. && er > -1. && *endfin) {
	sllog_1.reg[*iend - 1] = TRUE_;
	sllog_1.lc[*iend - 1] = TRUE_;
	sllog_1.osc[*iend - 1] = FALSE_;
	*cspec = FALSE_;
	if (ep > 0. || eq < 0. || er < 0.) {
	    slclss_2.kclass[*iend - 1] = 2;
	}
	return 0;
    }
    sllog_1.reg[*iend - 1] = FALSE_;
    slclss_2.eta[(*iend << 1) - 2] = (er - ep + 2.) * .5;
    if ((d__1 = slclss_2.eta[(*iend << 1) - 2], abs(d__1)) <= *tol) {
	slclss_2.eta[(*iend << 1) - 2] = 0.;
    }
    if (slclss_2.eta[(*iend << 1) - 2] != 0.) {
	slclss_2.eqlnf[*iend - 1] = (eq - er) / slclss_2.eta[(*iend << 1) - 2]
		;
	iqlnf = (integer) (slclss_2.eqlnf[*iend - 1] + d_sign(&c_b123, &
		slclss_2.eqlnf[*iend - 1]));
	if ((d__1 = iqlnf - slclss_2.eqlnf[*iend - 1], abs(d__1)) < tol4) {
	    slclss_2.eqlnf[*iend - 1] = (doublereal) iqlnf;
	}
	d__2 = (d__1 = slclss_2.eta[(*iend << 1) - 2], abs(d__1)) * sqrt(cp / 
		cr);
	c1 = cq / cr * pow_dd(&d__2, &slclss_2.eqlnf[*iend - 1]);
	if (c1 == 0.) {
	    slclss_2.eqlnf[*iend - 1] = 0.;
	}
	c2 = (ep + er) / (slclss_2.eta[(*iend << 1) - 2] * 4.);
	c2 *= c2 - 1.;
	if (*iprint >= 5) {
	    s_wsfe(&io___156);
	    do_fio(&c__1, (char *)&c1, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&c2, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&slclss_2.eqlnf[*iend - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&slclss_2.eta[(*iend << 1) - 2], (ftnlen)
		    sizeof(doublereal));
	    e_wsfe();
	}
    } else {
/* Computing 2nd power */
	d__1 = (ep + er) * .25;
	c3 = cp / cr * (d__1 * d__1);
	if (*iprint >= 5) {
	    s_wsfe(&io___158);
	    do_fio(&c__1, (char *)&c3, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
    }
    if (! (*endfin)) {

/*        Make an initial estimate for "infinity" (used in MESH). */

	if (er > eq || cq == 0.) {
	    if (er != ep) {
		gamma = er - ep;
		delta = cr / cp;
	    } else {
		gamma = eq - ep;
		delta = abs(cq) / cp;
		if (delta == 0.) {
		    delta = 1.;
		}
	    }
	} else {
	    if (eq > er) {
		gamma = eq - ep;
		delta = abs(cq) / cp;
	    } else {
		if (er > ep) {
		    gamma = er - ep;
		    delta = cr / cp;
		} else {
		    gamma = 0.;
		    if (er == ep) {
			delta = (d__1 = cr - cq, abs(d__1)) / cp;
		    } else {
			delta = abs(cq) / cp;
		    }
		}
	    }
	}
	if (gamma > .5) {
	    if (gamma < 2.) {
		*end = (float)80.;
	    } else {
/* Computing MIN */
/* Computing MAX */
		d__3 = 1. / (gamma + 2.);
		d__2 = (float)64. / ((gamma * 2. - (float)3.) * pow_dd(&delta,
			 &d__3));
		d__1 = max(d__2,1.);
		*end = min(d__1,80.);
		if (gamma > (float)24.) {
		    *end = (float)12.;
		}
	    }
	} else {
	    if (gamma < -.5) {
/* Computing MAX */
/* Computing MIN */
		d__3 = 1. / gamma;
		d__2 = pow_dd(&delta, &d__3) * (float)600. * pow_dd(&c_b133, &
			gamma);
		d__1 = min(d__2,120.);
		*end = max(d__1,1.);
	    } else {
		*end = (float)12.;
	    }
	    if (gamma == 0. && cq != 0.) {
		*end = (float)40.;
	    }
	}
    }

/*     Test for finite irregular singular points. */

    if (*endfin) {
	sgn = 1.;
	d__1 = er - ep;
	i__ = (integer) (er - ep + d_sign(&c_b123, &d__1));
	d__1 = eq - ep;
	k = (integer) (eq - ep + d_sign(&c_b123, &d__1));
	irreg = TRUE_;
	if (cq == 0.) {
	    if (i__ >= -2 && (d__1 = er - ep - i__, abs(d__1)) <= tol4) {
		irreg = FALSE_;
	    }
	} else {
	    if (er <= eq && i__ >= -2 && (d__1 = er - ep - i__, abs(d__1)) <= 
		    tol4) {
		irreg = FALSE_;
	    }
	    if (er > eq && k >= -2 && (d__1 = eq - ep - k, abs(d__1)) <= tol4)
		     {
		irreg = FALSE_;
	    }
	}
	slclss_2.emu[*iend - 1] = (1. - ep) * .5;
	if (irreg) {
	    if (*iprint >= 3) {
		s_wsfe(&io___161);
		e_wsfe();
	    }
	    slclss_2.kclass[*iend - 1] = 5;
	} else {

/*           Compute the principal Frobenius root. */

	    if (slclss_2.eta[(*iend << 1) - 2] != 0.) {
		if (cq != 0. && er > eq && k == -2) {
/* Computing 2nd power */
		    d__1 = slclss_2.emu[*iend - 1];
		    slclss_2.pnu[*iend - 1] = d__1 * d__1 + cq / cp;
		    if ((d__1 = slclss_2.pnu[*iend - 1], abs(d__1)) <= tol4) {
			slclss_2.pnu[*iend - 1] = 0.;
		    }
		    if (slclss_2.pnu[*iend - 1] >= 0.) {
			slclss_2.pnu[*iend - 1] = slclss_2.emu[*iend - 1] + 
				sqrt(slclss_2.pnu[*iend - 1]);
		    } else {
			slclss_2.pnu[*iend - 1] = -ep;
		    }
		} else {
/* Computing MAX */
		    d__1 = 1. - ep;
		    slclss_2.pnu[*iend - 1] = max(d__1,0.);
		}
		if (slclss_2.pnu[*iend - 1] > -ep) {
		    if (*iprint >= 5) {
			s_wsfe(&io___162);
			do_fio(&c__1, (char *)&slclss_2.pnu[*iend - 1], (
				ftnlen)sizeof(doublereal));
			e_wsfe();
		    }
		}
	    }
	}
    } else {
	sgn = -1.;
    }
    if (sgn * slclss_2.eta[(*iend << 1) - 2] > 0.) {

/*        Carry out the Case 1 tests. */

	k = 0;
	if (slclss_2.eqlnf[*iend - 1] < -2.) {
	    if (cq < 0.) {
		k = 1;
	    }
	    if (cq > 0.) {
		k = -1;
	    }
	}
	if (slclss_2.eqlnf[*iend - 1] == -2.) {
	    if ((d__1 = c1 + c2 + .25, abs(d__1)) <= tol4) {
		if (*iprint >= 3) {
		    s_wsfe(&io___163);
		    e_wsfe();
		}
		k = -1;
		*iflag = 1;
	    } else {
		if (c1 + c2 < -.25 - tol4) {
		    k = 1;
		}
		if (c1 + c2 > -.25) {
		    k = -1;
		}
	    }
	}
	if (slclss_2.eqlnf[*iend - 1] > -2.) {
	    if ((d__1 = c2 + .25, abs(d__1)) <= tol4) {
		c2 = -.25;
		if (*iprint >= 3) {
		    s_wsfe(&io___164);
		    e_wsfe();
		}
		*iflag = 1;
	    }
	    if (c2 >= -.25) {
		k = -1;
	    }
	}
	if (k == 1) {
	    sllog_1.osc[*iend - 1] = TRUE_;
	} else {
	    if (k == -1) {
		sllog_1.osc[*iend - 1] = FALSE_;
	    } else {
		if (*iprint >= 3) {
		    s_wsfe(&io___165);
		    e_wsfe();
		}
	    }
	}
	k = 0;
	if (slclss_2.eqlnf[*iend - 1] < -2.) {
	    if (cq > 0.) {
		k = -1;
	    }
	    if (cq < 0.) {
		k = 1;
	    }
	}
	if (slclss_2.eqlnf[*iend - 1] == -2.) {
	    if ((d__1 = c1 + c2 - .75, abs(d__1)) <= tol4) {
		k = -1;
		if (*iprint >= 3) {
		    s_wsfe(&io___166);
		    e_wsfe();
		}
		*iflag = 1;
	    }
	    if (c1 + c2 >= .75) {
		k = -1;
	    }
	    if ((d__1 = c1 + c2, abs(d__1)) < .75 - tol4) {
		k = 1;
	    }
	    if (c1 + c2 < -tol4) {
		k = 1;
	    }
	}
	if (slclss_2.eqlnf[*iend - 1] > -2.) {
	    if ((d__1 = c2 - .75, abs(d__1)) <= tol4) {
		k = -1;
		if (*iprint >= 3) {
		    s_wsfe(&io___167);
		    e_wsfe();
		}
		*iflag = 1;
	    }
	    if (c2 >= .75) {
		k = -1;
	    }
	    if (abs(c2) < .75 - tol4) {
		k = 1;
	    }
	    if (c2 < -tol4) {
		k = 1;
	    }
	}
	if (k == 1) {
	    sllog_1.lc[*iend - 1] = TRUE_;
	} else {
	    if (k == -1) {
		sllog_1.lc[*iend - 1] = FALSE_;
	    } else {
		s_wsfe(&io___168);
		e_wsfe();
	    }
	}
    }
    if (sgn * slclss_2.eta[(*iend << 1) - 2] < 0.) {

/*        Carry out the Case 2 tests. */

	k = 0;
	if (slclss_2.eqlnf[*iend - 1] > 0. && cq < 0.) {
	    k = 1;
	}
	if (slclss_2.eqlnf[*iend - 1] > 0. && cq > 0.) {
	    k = -1;
	}
	if (slclss_2.eqlnf[*iend - 1] == 0.) {
	    k = -1;
	    *cev = cq / cr;
	    *cspec = TRUE_;
	    if (slreal_1.u * abs(*cev) >= 1.) {
		*cspec = FALSE_;
	    }
	}
	if (slclss_2.eqlnf[*iend - 1] < 0.) {
	    k = -1;
	    *cev = 0.;
	    *cspec = TRUE_;
	}
	if (k == 1) {
	    sllog_1.osc[*iend - 1] = TRUE_;
	} else {
	    if (k == -1) {
		sllog_1.osc[*iend - 1] = FALSE_;
	    } else {
		s_wsfe(&io___169);
		e_wsfe();
	    }
	}
	k = 0;
	if (slclss_2.eqlnf[*iend - 1] > 2. && cq > 0.) {
	    k = -1;
	}
	if (slclss_2.eqlnf[*iend - 1] <= 2.) {
	    k = -1;
	}
	if (slclss_2.eqlnf[*iend - 1] > 2. && cq < 0.) {
	    k = 1;
	}
	if (k == 1) {
	    sllog_1.lc[*iend - 1] = TRUE_;
	} else {
	    if (k == -1) {
		sllog_1.lc[*iend - 1] = FALSE_;
	    } else {
		s_wsfe(&io___170);
		e_wsfe();
	    }
	}
    }
    if (slclss_2.eta[(*iend << 1) - 2] == 0.) {

/*        Carry out the Case 3 and 4 tests. */

	if (sgn * (eq - er) < 0. && cq < 0.) {
	    sllog_1.osc[*iend - 1] = TRUE_;
	    sllog_1.lc[*iend - 1] = TRUE_;
	}
	if (sgn * (eq - er) < 0. && cq > 0.) {
	    sllog_1.osc[*iend - 1] = FALSE_;
	    sllog_1.lc[*iend - 1] = FALSE_;
	}
	if (eq == er) {
	    sllog_1.osc[*iend - 1] = FALSE_;
	    sllog_1.lc[*iend - 1] = FALSE_;
	    *cev = cq / cr + c3;
	    *cspec = TRUE_;
	    if (slreal_1.u * abs(*cev) >= 1.) {
		*cspec = FALSE_;
	    }
	}
	if (sgn * (eq - er) > 0. || cq == 0.) {
	    sllog_1.osc[*iend - 1] = FALSE_;
	    sllog_1.lc[*iend - 1] = FALSE_;
	    *cev = c3;
	    *cspec = TRUE_;
	    if (slreal_1.u * abs(*cev) >= 1.) {
		*cspec = FALSE_;
	    }
	}
    }
    if (abs(*cev) <= tol4) {
	*cev = 0.;
    }

/*     Calculate the Friedrichs boundary condition (if appropriate). */

    if (*cspec) {
	bc[1] = 1.;
	bc[2] = 0.;
    }
    if (! (*cspec) && ! sllog_1.osc[*iend - 1]) {
	if (sgn * (er + ep) > 0. && sgn * (eq + ep) > 0.) {
	    bc[1] = 0.;
	    bc[2] = 1.;
	} else {
	    if (sgn * (er + ep) > 0. && eq + ep == 0.) {
		bc[1] = sqrt(cp * abs(cq));
		bc[2] = 1.;
		if (bc[1] > 1.) {
		    bc[2] = 1. / bc[1];
		    bc[1] = 1.;
		}
	    } else {
		if (sgn * (er + ep) < 0. || sgn * (eq + ep) < 0.) {
		    bc[1] = 1.;
		    bc[2] = 0.;
		}
	    }
	}
    }
    if (! sllog_1.osc[*iend - 1]) {
	if (! (*endfin) && slclss_2.eqlnf[*iend - 1] == -1.) {
	    slclss_2.kclass[*iend - 1] = 3;
	}
	i__ = (integer) (er - ep);
	if (cq != 0.) {
	    k = (integer) (eq - ep);
	    if (cq > 0.) {
		if (k < i__) {
		    i__ = 0;
		}
	    } else {
		i__ = min(i__,k);
	    }
	}
	if (*endfin && i__ < 0) {
	    if (irreg) {
		slclss_2.kclass[*iend - 1] = 6;
	    }
	    if (slclss_2.eta[(*iend << 1) - 2] > 0.) {

/*              Transform some nonoscillatory problems for which Tau */
/*              is unbounded near a finite singular endpoint. */

		if (irreg) {
		    slclss_2.kclass[*iend - 1] = 10;
		}
		slclss_2.cpt[*iend - 1] = cp;
		slclss_2.crt[*iend - 1] = cr;
		slclss_2.ept[*iend - 1] = ep;
		slclss_2.ert[*iend - 1] = er;
		slclss_2.emu[*iend - 1] = 0.;
		d__1 = slclss_2.eta[(*iend << 1) - 2] * sqrt(cp / cr);
		d__2 = 1. / slclss_2.eta[(*iend << 1) - 2];
		slclss_2.d__[(*iend << 2) - 4] = pow_dd(&d__1, &d__2);
		d__1 = ep + er;
		slclss_2.d__[(*iend << 2) - 3] = 1. / sqrt(sqrt(cp * cr * 
			pow_dd(&slclss_2.d__[(*iend << 2) - 4], &d__1)));
		if (slclss_2.eqlnf[*iend - 1] == -2. || c1 == 0.) {
		    if (! irreg) {
			slclss_2.kclass[*iend - 1] = 9;
		    }
		    slclss_2.emu[*iend - 1] = (d__1 = c1 + .25 + c2, abs(d__1)
			    );
		    if (slclss_2.emu[*iend - 1] < tol4) {
			slclss_2.emu[*iend - 1] = .5;
		    } else {
			slclss_2.emu[*iend - 1] = sqrt(slclss_2.emu[*iend - 1]
				) + .5;
		    }
		} else {
		    if (! irreg) {
			slclss_2.kclass[*iend - 1] = 8;
		    }
		}
		slclss_2.eta[(*iend << 1) - 1] = slclss_2.emu[*iend - 1] - (
			ep + er) * .25 / slclss_2.eta[(*iend << 1) - 2];
		if (slclss_2.kclass[*iend - 1] == 10 && slclss_2.emu[*iend - 
			1] == 0.) {
		    slclss_2.eta[(*iend << 1) - 1] = (1. - ep) * .5 / 
			    slclss_2.eta[(*iend << 1) - 2];
		    slclss_2.emu[*iend - 1] = slclss_2.eta[(*iend << 1) - 1] 
			    + (ep + er) * .25 / slclss_2.eta[(*iend << 1) - 2]
			    ;
		}
		slclss_2.d__[(*iend << 2) - 2] = slclss_2.eta[(*iend << 1) - 
			1] * (slclss_2.eta[(*iend << 1) - 1] + (ep - 1.) / 
			slclss_2.eta[(*iend << 1) - 2]);
		slclss_2.d__[(*iend << 2) - 1] = slclss_2.d__[(*iend << 2) - 
			2];
		if (slclss_2.eqlnf[*iend - 1] == -2.) {
		    slclss_2.d__[(*iend << 2) - 1] -= c1;
		}
		if ((d__1 = slclss_2.d__[(*iend << 2) - 1], abs(d__1)) <= 
			tol4) {
		    slclss_2.d__[(*iend << 2) - 1] = 0.;
		}
		slclss_2.d__[(*iend << 2) - 1] = sqrt((d__1 = slclss_2.d__[(*
			iend << 2) - 1], abs(d__1)));
		if (*iprint >= 5) {
		    s_wsfe(&io___171);
		    do_fio(&c__1, (char *)&slclss_2.emu[*iend - 1], (ftnlen)
			    sizeof(doublereal));
		    do_fio(&c__1, (char *)&slclss_2.eta[(*iend << 1) - 1], (
			    ftnlen)sizeof(doublereal));
		    e_wsfe();
		    s_wsfe(&io___172);
		    do_fio(&c__1, (char *)&slclss_2.d__[(*iend << 2) - 2], (
			    ftnlen)sizeof(doublereal));
		    do_fio(&c__1, (char *)&slclss_2.d__[(*iend << 2) - 1], (
			    ftnlen)sizeof(doublereal));
		    e_wsfe();
		}
	    }
	    if (slclss_2.kclass[*iend - 1] >= 9) {
		if (! ex) {
		    sllog_1.lflag[4] = TRUE_;
		}
		if (*iprint >= 3) {
		    s_wsfe(&io___173);
		    e_wsfe();
		    if (slclss_2.emu[*iend - 1] > 0. || ep + er != 0.) {
			s_wsfe(&io___174);
			e_wsfe();
		    }
		}
	    }
	}
	if (*endfin && ! sllog_1.reg[*iend - 1] && slclss_2.kclass[*iend - 1] 
		== 0) {
	    slclss_2.kclass[*iend - 1] = 4;
	}
    }
    if ((posc || qosc || rosc) && ! (*endfin)) {
	*end = (float)99.;
    }
    if (*iprint >= 5) {
	s_wsfe(&io___175);
	do_fio(&c__1, (char *)&slclss_2.kclass[*iend - 1], (ftnlen)sizeof(
		integer));
	e_wsfe();
    }
    return 0;
} /* clsend_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int densef_(doublereal *tol, logical *cspec, integer *iprint,
	 integer *iter, integer *nextrp, integer *numt, doublereal *t, 
	doublereal *rho, integer *nev, doublereal *hmin, integer *numev, 
	doublereal *store)
{
    /* Format strings */
    static char fmt_10[] = "(\002 Level\002,i3,\002 of iteration\002,i3)";
    static char fmt_30[] = "(\002 Switchover to asymptotic eigenvalues at\
\002,\002 Nev =\002,i8)";
    static char fmt_40[] = "(\002 Switchover to asymptotic RsubN at \002,\
\002Nev = \002,i8)";
    static char fmt_75[] = "(\002 Large jump in the step spectral density\
\002,\002 function at\002,d17.10)";
    static char fmt_76[] = "(18x,\002Iteration =\002,i2,\002, level = \002,i\
2,\002, jump = \002,d17.10)";
    static char fmt_80[] = "(\002 Nev =\002,i7,\002, EvHat =\002,d15.6,\002,\
 RHat =\002,d15.6)";
    static char fmt_90[] = "(\002 MaxNev = \002,i8)";
    static char fmt_95[] = "(9x,\002t\002,18x,\002RhoHat(t)\002)";
    static char fmt_100[] = "(f12.4,e31.15)";

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal z__, ev, dx;
    static integer js;
    static doublereal px, qx, rx, ux, big, eta, aev, arn, pdu, tol1, tol2;
    static logical efin[4]	/* was [2][2] */, done;
    static doublereal zabs, qlnf, rold;
    static integer krho, nrho;
    static doublereal flow;
    static integer klvl;
    static logical jump;
    static doublereal qint, tenu, zrel, xtol, beta1, beta2, halfh;
    //extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal 	    *, doublereal *);
    static doublereal alpha, fhigh, scale, delta;
    extern /* Subroutine */ int getef_(doublereal *, doublereal *, integer *, 
	    doublereal *, logical *);
    static doublereal denom;
    static logical rdone;
    static doublereal evhat, evold[200];
    static integer nsave;
    static doublereal rpata, rpatb, rsave[5], xleft;
    extern /* Subroutine */ int getrn_(doublereal *, doublereal *, logical *, 
	    doublereal *, doublereal *, doublereal *);
    static doublereal error;
    extern doublereal asymr_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal evlow, psrho, rpint, rsubn, alpha1, alpha2;
    extern /* Subroutine */ int zzero_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, integer *, 
	    doublereal *);
    static doublereal etaold, evhigh;
    extern /* Subroutine */ int brcket_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    static doublereal abserr, avgrho, efnorm, evsave, relerr;
    static integer maxnev, iasymp;
    extern doublereal asymev_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal tolmax;
    static integer lprint;
    extern /* Subroutine */ int extrap_(doublereal *, doublereal *, integer *,
	     integer *, logical *, logical *, integer *, doublereal *, 
	    integer *, doublereal *, logical *);
    static doublereal rhosum;
    extern /* Subroutine */ int pqrint_(doublereal *, doublereal *, 
	    doublereal *);
    static doublereal sqrtrp;

    /* Fortran I/O blocks */
    static cilist io___202 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___203 = { 0, 21, 0, fmt_10, 0 };
    static cilist io___239 = { 0, 6, 0, fmt_30, 0 };
    static cilist io___240 = { 0, 21, 0, fmt_30, 0 };
    static cilist io___242 = { 0, 6, 0, fmt_40, 0 };
    static cilist io___243 = { 0, 21, 0, fmt_40, 0 };
    static cilist io___245 = { 0, 6, 0, fmt_75, 0 };
    static cilist io___246 = { 0, 21, 0, fmt_75, 0 };
    static cilist io___247 = { 0, 6, 0, fmt_76, 0 };
    static cilist io___248 = { 0, 21, 0, fmt_76, 0 };
    static cilist io___249 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___250 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___251 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___252 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___253 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___254 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___255 = { 0, 21, 0, fmt_80, 0 };
    static cilist io___256 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___258 = { 0, 6, 0, fmt_90, 0 };
    static cilist io___259 = { 0, 21, 0, fmt_90, 0 };
    static cilist io___260 = { 0, 6, 0, fmt_95, 0 };
    static cilist io___261 = { 0, 21, 0, fmt_95, 0 };
    static cilist io___262 = { 0, 6, 0, fmt_100, 0 };
    static cilist io___263 = { 0, 21, 0, fmt_100, 0 };


/* ********************************************************************** */
/*                                                                     * */
/*     This routine computes the spectral density function rho(t).     * */
/*                                                                     * */
/* ********************************************************************** */

/*    Input parameters: */
/*      TOL(*) as in SLEDGE. */
/*      CSPEC(*)  = logical 2-vector; CSPEC(i) = .true. iff endpoint i */
/*                  (1 = A, 2 = B) generates continuous spectrum. */
/*      IPRINT    = integer controlling printing. */
/*      ITER      = iteration from SLEDGE. */
/*      NEXTRP    = integer giving maximum no. of extrapolations. */
/*      NUMT      = integer equalling number of T(*) points. */
/*      T(*)      = real vector of abcissae for spectral function rho(t). */
/*      HMIN      = minimum stepsize in Level 0 mesh. */

/*    Output parameters: */
/*      RHO(*)    = real vector of values for spectral density function */
/*                  rho(t),  RHO(I) = rho(T(I)). */
/*      NEV       = integer pointer to eigenvalue.  On a normal return */
/*                  (FLAG = 0) this is set to the index of the last */
/*                  eigenvalue computed; if FLAG is not zero, then NEV */
/*                  gives the index of the eigenvalue where the problem */
/*                  occurred. */
/*      NUMEV     = cumulative number of eigenvalues computed. */

/*    Auxiliary storage: */
/*      STORE(*) = real vector of auxiliary storage, must be dimensioned */
/*                 at least 5*Nxinit+(Maxlvl+2)*NUMT.  The value of */
/*                 Nxinit is either the input NUMX or Maxint.  Currently, */
/*                 Maxlvl = 8 and Maxint = 235. */
/*            1    ->     Nxinit         vector of mesh points X(*), */
/*       Nxinit+1  ->    5*Nxinit        intermediate RsubN calculations, */
/*     5*Nxinit+1  -> 5*Nxinit+10*NUMT   intermediate RHO values. */
/* ----------------------------------------------------------------------- */
/*     The definition of a spectral density function assumes a certain */
/*     normalization on the eigenfunctions.  For the case when x = b */
/*     generates the continuous spectrum, the normalization used here */
/*     (and in routine GETRN below) is: */
/*     (1) when x = a is regular  u(a) = (A2-A2P*Ev)/SCALE */
/*                            (pu')(a) = (A1-A1P*Ev)/SCALE, */
/*         with SCALE = sqrt(A1**2+A2**2) when A1' = A2' = 0, and */
/*              SCALE = sqrt(ALPHA) otherwise. */
/*     (2) When x = a is a regular singular point then u(x) is taken to */
/*         be asymptotic to the principal Frobenius solution, i.e., */
/*         near x = a   u(x) ~ (x-a)**Nu    with Nu the larger */
/*         root of the indicial equation. */
/*     Analogous normalizations hold at x = b when the endpoint x = a */
/*     generates the continuous spectrum. */
/* ---------------------------------------------------------------------- */
/*     Local variables: */


/*     Initialization: */

    /* Parameter adjustments */
    --store;
    --rho;
    --t;
    --cspec;
    --tol;

    /* Function Body */
    nsave = 200;
    js = slint_1.nxinit * 5;
    tenu = slreal_1.u * 10.;
    big = 1. / slreal_1.u;
    avgrho = big;
    lprint = min(*iprint,3);
    maxnev = 0;
    if (sllog_1.reg[0]) {
/* Computing MAX */
	d__1 = 1., d__2 = abs(slreal_1.a);
	dx = sqrt(slreal_1.u) * max(d__1,d__2);
	d__1 = slreal_1.a + dx;
	coeff_(&d__1, &px, &qx, &rx);
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	rpata = rx * px;
	alpha1 = 1. / rx;
	d__1 = slreal_1.a + dx * 2.;
	coeff_(&d__1, &px, &qx, &rx);
	alpha1 = alpha1 * (rpata - px * rx) / (dx * 4.);
	rpata = sqrt(rpata);
	alpha2 = sqrt(rpata);
	alpha1 = (slreal_1.a1 + slreal_1.a2 * alpha1) / alpha2;
	alpha2 *= slreal_1.a2;
	slint_1.ncoeff += 2;
    } else {
	alpha1 = 0.;
	alpha2 = 1.;
	rpata = 1.;
    }
    if (sllog_1.reg[1]) {
/* Computing MAX */
	d__1 = 1., d__2 = abs(slreal_1.b);
	dx = sqrt(slreal_1.u) * max(d__1,d__2);
	d__1 = slreal_1.b - dx;
	coeff_(&d__1, &px, &qx, &rx);
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	rpatb = rx * px;
	beta1 = 1. / rx;
	d__1 = slreal_1.b - dx * 2.;
	coeff_(&d__1, &px, &qx, &rx);
	beta1 = beta1 * (rpatb - px * rx) / (dx * 4.);
	rpatb = sqrt(rpatb);
	beta2 = sqrt(rpatb);
	beta1 = (slreal_1.b1 - slreal_1.b2 * beta1) / beta2;
	beta2 *= slreal_1.b2;
	slint_1.ncoeff += 2;
    } else {
	beta1 = 0.;
	beta2 = 1.;
    }
    alpha = slreal_1.a1p * slreal_1.a2 - slreal_1.a1 * slreal_1.a2p;
    if (cspec[2]) {
	if (alpha == 0.) {
/* Computing 2nd power */
	    d__1 = slreal_1.a1;
/* Computing 2nd power */
	    d__2 = slreal_1.a2;
	    scale = sqrt(d__1 * d__1 + d__2 * d__2);
	} else {
	    scale = sqrt(alpha);
	}
    }
    if (cspec[1]) {
/* Computing 2nd power */
	d__1 = slreal_1.b1;
/* Computing 2nd power */
	d__2 = slreal_1.b2;
	scale = sqrt(d__1 * d__1 + d__2 * d__2);
    }
    tolmax = max(tol[1],tol[2]);
    tol1 = min(tol[1],.005);
    tol2 = min(tol[2],.005);
    abserr = tol1;
    relerr = tol2;
    klvl = 1;
    delta = .5;

/*     Begin the Main loop over LEVEL. */

    i__1 = slint_1.maxlvl;
    for (slint_1.level = 0; slint_1.level <= i__1; ++slint_1.level) {
	if (*iprint >= 2) {
	    s_wsfe(&io___202);
	    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&(*iter), (ftnlen)sizeof(integer));
	    e_wsfe();
	    s_wsfe(&io___203);
	    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&(*iter), (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	nrho = 1;
	krho = 0;
	psrho = 0.;
	rhosum = 0.;
	rold = 0.;
	iasymp = 0;
	evsave = -big;
	*nev = 0;
	jump = FALSE_;

/*        Compute integrals needed in asymptotic formulas. */

	qint = 0.;
	rpint = 0.;
	i__2 = slint_1.nxinit;
	for (i__ = 2; i__ <= i__2; ++i__) {
	    xleft = store[i__ - 1];
	    h__ = (store[i__] - xleft) / klvl;
	    halfh = h__ * .5;
	    i__3 = klvl;
	    for (j = 1; j <= i__3; ++j) {
		z__ = xleft + halfh;
		xleft += h__;
		pqrint_(&z__, &sqrtrp, &qlnf);
		if (slint_1.flag__ < 0) {
		    return 0;
		}
		qint += h__ * qlnf;
		rpint += h__ * sqrtrp;
/* L15: */
	    }
/* L20: */
	}
	if (qint > 1. / slreal_1.u) {
	    qint = 0.;
	}
/* Computing MAX */
/* Computing MIN */
	d__2 = abserr / (float)100.;
	d__1 = min(d__2,relerr) / (float)10.;
	zabs = max(d__1,tenu);
	zrel = relerr / (float)10.;
/* Computing MAX */
	d__1 = delta / 6., d__2 = tol1 + tol2;
	delta = max(d__1,d__2);

/*        Begin the secondary loop over NEV. */

L25:
	if (iasymp >= 2) {
	    aev = asymev_(nev, &qint, &rpint, &alpha1, &alpha2, &beta1, &
		    beta2);
	    evhat = aev;
	    if (iasymp <= 3) {
		goto L35;
	    }
	    rsubn = asymr_(nev, &rpint, &rpata, &rpatb, &scale);
	    goto L45;
	}
	if (*hmin / klvl <= tenu) {
	    slint_1.flag__ = -8;
	}
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	if (slint_1.level > 0 && *nev < min(maxnev,nsave)) {
/* Computing MAX */
	    d__2 = tol1 * .5, d__2 = max(d__2,delta), d__3 = tol2 * .5 * (
		    d__1 = evold[*nev], abs(d__1));
	    ev = max(d__2,d__3);
	    evlow = evold[*nev] - ev;
	    evhigh = evold[*nev] + ev;
	} else {
	    if (slint_1.level == 0) {
		ev = 0.;
	    } else {
		ev = asymev_(nev, &qint, &rpint, &alpha1, &alpha2, &beta1, &
			beta2);
	    }
/* Computing MAX */
	    d__1 = tol1 * .5, d__1 = max(d__1,delta), d__2 = tol2 * .5 * abs(
		    ev);
	    eta = max(d__1,d__2);
	    evlow = ev - eta;
	    evhigh = ev + eta;
	}
	brcket_(nev, &evlow, &evhigh, &flow, &fhigh, &zabs, &zrel, &store[1]);
	if (slint_1.level == 0 && *nev == 0) {
	    delta = evhigh - evlow;
	}
	if (slint_1.flag__ < 0) {
	    return 0;
	}
/* Computing MAX */
	d__2 = zabs, d__3 = zrel * abs(evhigh);
	if ((d__1 = evhigh - evlow, abs(d__1)) > max(d__2,d__3)) {
	    zzero_(&evlow, &evhigh, &flow, &fhigh, &zabs, &zrel, &j, &store[1]
		    );
	}
	evhat = min(evlow,evhigh);
	getef_(&evhat, &efnorm, &lprint, &store[1], efin);
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	if (cspec[2]) {
	    if (sllog_1.reg[0] || slclss_3.pnu[0] == 0. || slclss_3.pnu[0] == 
		    1. - slclss_3.ep[0]) {
		ux = (d__1 = store[slint_1.nxinit + 1], abs(d__1));
		pdu = (d__1 = store[(slint_1.nxinit << 1) + 1], abs(d__1));
		if (slreal_1.a2 - slreal_1.a2p * evhat != 0.) {
		    denom = scale * ux / (d__1 = slreal_1.a2 - slreal_1.a2p * 
			    evhat, abs(d__1));
		} else {
		    denom = scale * pdu / (d__1 = slreal_1.a1 - slreal_1.a1p *
			     evhat, abs(d__1));
		}
	    } else {
		h__ = store[2] - store[1];
		ux = (d__1 = store[slint_1.nxinit + 2], abs(d__1));
		pdu = (d__1 = store[(slint_1.nxinit << 1) + 2], abs(d__1));
		if (ux >= pdu) {
		    denom = ux / pow_dd(&h__, slclss_3.pnu);
		} else {
		    d__1 = slclss_3.ep[0] + slclss_3.pnu[0] - 1.;
		    denom = pdu / (slclss_3.cp[0] * abs(slclss_3.pnu[0]) * 
			    pow_dd(&h__, &d__1));
		}
	    }
	} else {
	    if (sllog_1.reg[1] || slclss_3.pnu[1] == 0. || slclss_3.pnu[1] == 
		    1. - slclss_3.ep[1]) {
		ux = (d__1 = store[slint_1.nxinit * 2], abs(d__1));
		pdu = (d__1 = store[slint_1.nxinit * 3], abs(d__1));
		if (slreal_1.b2 != 0.) {
		    denom = scale * ux / abs(slreal_1.b2);
		} else {
		    denom = scale * pdu / abs(slreal_1.b1);
		}
	    } else {
		h__ = store[slint_1.nxinit] - store[slint_1.nxinit - 1];
		ux = (d__1 = store[(slint_1.nxinit << 1) - 1], abs(d__1));
		pdu = (d__1 = store[slint_1.nxinit * 3 - 1], abs(d__1));
		if (ux >= pdu) {
		    denom = ux / pow_dd(&h__, &slclss_3.pnu[1]);
		} else {
		    d__1 = slclss_3.ep[1] + slclss_3.pnu[1] - 1.;
		    denom = pdu / (slclss_3.cp[1] * abs(slclss_3.pnu[1]) * 
			    pow_dd(&h__, &d__1));
		}
	    }
	}
	if (big * denom >= 1.) {
/* Computing 2nd power */
	    d__1 = denom;
	    efnorm = 1. / (d__1 * d__1);
	} else {
/* Computing 2nd power */
	    d__1 = slreal_1.u;
	    efnorm = 1. / (d__1 * d__1);
	}

/*           Test for asymptotic EV. */

	aev = asymev_(nev, &qint, &rpint, &alpha1, &alpha2, &beta1, &beta2);
/* Computing MAX */
	d__2 = abserr, d__3 = relerr * abs(evhat);
	if ((d__1 = aev - evhat, abs(d__1)) * 10. > max(d__2,d__3)) {
	    iasymp = 0;
	} else {
	    if (iasymp < 1) {
		iasymp = 1;
	    } else {
		if (*iprint >= 2) {
		    s_wsfe(&io___239);
		    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		    e_wsfe();
		    s_wsfe(&io___240);
		    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		    e_wsfe();
		}
		iasymp = 2;
	    }
	}
	if (efnorm < big) {
	    rsubn = 1. / (alpha + efnorm);
	} else {
	    rsubn = 0.;
	}

/*           Test for asymptotic RsubN. */

L35:
	arn = asymr_(nev, &rpint, &rpata, &rpatb, &scale);
	if (iasymp >= 2) {

/*           Eigenvalues from asymptotic formulas; produce current RsubN. */

	    getrn_(&aev, &alpha, &cspec[1], &scale, &rsubn, &store[1]);
	    if (slint_1.flag__ < 0) {
		return 0;
	    }
/* Computing MAX */
	    d__2 = abserr / (float)100., d__3 = relerr * arn;
	    if ((d__1 = arn - rsubn, abs(d__1)) > max(d__2,d__3)) {
		iasymp = 2;
	    } else {
		if (iasymp < 3) {
		    iasymp = 3;
		} else {
		    if (*iprint >= 2) {
			s_wsfe(&io___242);
			do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer)
				);
			e_wsfe();
			s_wsfe(&io___243);
			do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer)
				);
			e_wsfe();
		    }
		    iasymp = 4;
		}
	    }
	}
L45:
	if (*nev < nsave) {
	    evold[*nev] = evhat;
	}
	if (*nev > 0) {
	    eta = (evhat + evsave) * .5;
	}
	if (evhat < slclss_3.cutoff) {
	    psrho += rsubn;
	}
L50:
	if (t[nrho] <= slclss_3.cutoff) {

/*              Use step functions for Rho(t). */

	    if (t[nrho] <= evhat) {
		rho[nrho] = rhosum;
		++nrho;
		if (nrho <= *numt) {
		    goto L50;
		} else {
		    goto L85;
		}
	    }
	} else {
	    if (*nev == 0) {
		goto L78;
	    }

/*              Use linear interpolation for Rho(t). */

L55:
	    if (t[nrho] <= eta) {
		if (jump) {
L60:
		    if (t[nrho] <= evsave) {
			rho[nrho] = rhosum - rold;
			++nrho;
			if (nrho <= *numt) {
			    goto L60;
			} else {
			    jump = FALSE_;
			    goto L85;
			}
		    } else {
			jump = FALSE_;
L65:
			if (t[nrho] <= eta) {
			    rho[nrho] = rhosum;
			    ++nrho;
			    if (nrho > *numt) {
				goto L85;
			    }
			    goto L65;
			} else {
			    goto L70;
			}
		    }
		}
		if (*nev <= 1 || slclss_3.cutoff > evsave) {
		    ux = slclss_3.cutoff;
		} else {
		    ux = etaold;
		}
		rho[nrho] = rhosum - rold * (eta - t[nrho]) / (eta - ux);
		++nrho;
		if (nrho <= *numt) {
		    goto L55;
		} else {
		    goto L85;
		}
	    }
L70:
/* Computing MAX */
	    d__1 = rold * 6., d__2 = tolmax * 10., d__1 = max(d__1,d__2), 
		    d__2 = avgrho * 4.;
	    if (rsubn > max(d__1,d__2) && evhat > slclss_3.cutoff && krho > 4)
		     {

/*                 Possible eigenvalue in the continuous spectrum. */

		sllog_1.lflag[2] = TRUE_;
		if (*iprint >= 1) {
		    s_wsfe(&io___245);
		    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		    s_wsfe(&io___246);
		    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		    s_wsfe(&io___247);
		    do_fio(&c__1, (char *)&(*iter), (ftnlen)sizeof(integer));
		    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(
			    integer));
		    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		    s_wsfe(&io___248);
		    do_fio(&c__1, (char *)&(*iter), (ftnlen)sizeof(integer));
		    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(
			    integer));
		    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		}
		jump = TRUE_;
	    }
	}
	if (*nev > 0) {
	    etaold = eta;
	}
L78:
	rhosum += rsubn;
	rold = rsubn;
	evsave = evhat;

/*           Output requested information. */

	if (*iprint >= 3) {
	    if (*nev <= 25 || iasymp <= 1 && *iprint >= 5) {
		s_wsfe(&io___249);
		do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		e_wsfe();
		s_wsfe(&io___250);
		do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		e_wsfe();
	    } else {
		if (*nev < 100 && *nev / 10 * 10 == *nev) {
		    s_wsfe(&io___251);
		    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		    s_wsfe(&io___252);
		    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer));
		    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
		    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(doublereal));
		    e_wsfe();
		} else {
		    if (*nev < 1000 && *nev / 100 * 100 == *nev) {
			s_wsfe(&io___253);
			do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer)
				);
			do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(
				doublereal));
			do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(
				doublereal));
			e_wsfe();
			s_wsfe(&io___254);
			do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(integer)
				);
			do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(
				doublereal));
			do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(
				doublereal));
			e_wsfe();
		    } else {
			if (*nev / 1000 * 1000 == *nev) {
			    s_wsfe(&io___255);
			    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(
				    integer));
			    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(
				    doublereal));
			    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(
				    doublereal));
			    e_wsfe();
			    s_wsfe(&io___256);
			    do_fio(&c__1, (char *)&(*nev), (ftnlen)sizeof(
				    integer));
			    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(
				    doublereal));
			    do_fio(&c__1, (char *)&rsubn, (ftnlen)sizeof(
				    doublereal));
			    e_wsfe();
			}
		    }
		}
	    }
	}
	++(*nev);
	if (evhat >= slclss_3.cutoff) {
	    if (krho == 5) {
		rsave[0] = rsave[1];
		rsave[1] = rsave[2];
		rsave[2] = rsave[3];
		rsave[3] = rsave[4];
	    } else {
		++krho;
	    }
	    rsave[krho - 1] = rsubn;
	    avgrho = 0.;
	    i__2 = krho;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		avgrho += rsave[i__ - 1];
/* L84: */
	    }
	    if (krho > 0) {
		avgrho /= krho;
	    }
	}
	goto L25;

/*        End of Nev loop ------------------ */

L85:
	maxnev = *nev;
	*numev += maxnev;
	if (*iprint >= 3) {
	    s_wsfe(&io___258);
	    do_fio(&c__1, (char *)&maxnev, (ftnlen)sizeof(integer));
	    e_wsfe();
	    s_wsfe(&io___259);
	    do_fio(&c__1, (char *)&maxnev, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	if (*iprint >= 4) {
	    s_wsfe(&io___260);
	    e_wsfe();
	    s_wsfe(&io___261);
	    e_wsfe();
	    i__2 = *numt;
	    for (j = 1; j <= i__2; ++j) {
		s_wsfe(&io___262);
		do_fio(&c__1, (char *)&t[j], (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&rho[j], (ftnlen)sizeof(doublereal));
		e_wsfe();
		s_wsfe(&io___263);
		do_fio(&c__1, (char *)&t[j], (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&rho[j], (ftnlen)sizeof(doublereal));
		e_wsfe();
/* L105: */
	    }
	}

/*        Extrapolate interpolated approximations. */

	done = TRUE_;
	i__2 = *numt;
	for (j = 1; j <= i__2; ++j) {
/* Computing MAX */
	    d__2 = tol1, d__3 = (d__1 = rho[j], abs(d__1)) * tol2;
	    xtol = max(d__2,d__3);
	    i__3 = slint_1.level + 1;
	    extrap_(&rho[j], &xtol, &i__3, nextrp, &c_true, &c_false, &c__1, &
		    store[(slint_1.maxlvl + 1) * j + js], iprint, &error, &
		    rdone);
	    if (rho[j] < 0.) {
		rho[j] = 0.;
	    }
	    if (j > 1) {
/* Computing MAX */
		d__1 = rho[j], d__2 = rho[j - 1];
		rho[j] = max(d__1,d__2);
	    }
	    if (error <= xtol * .5) {
		rdone = TRUE_;
	    }
	    done = rdone && done;
/* L110: */
	}
	if (done) {
	    return 0;
	}
/* Computing MAX */
	d__1 = abserr * .5;
	abserr = max(d__1,tenu);
/* Computing MAX */
	d__1 = relerr * .5;
	relerr = max(d__1,tenu);
/* L120: */
    }
    slint_1.flag__ = -3;
    return 0;
} /* densef_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int dscrip_(logical *lc, logical *lplc, logical *type__, 
	logical *reg, logical *cspec, doublereal *cev, doublereal *cutoff, 
	integer *lastev, doublereal *a1, doublereal *a1p, doublereal *a2, 
	doublereal *a2p, doublereal *b1, doublereal *b2)
{
    /* Format strings */
    static char fmt_123[] = "(\002 The spectral category is\002,i3,\002.\002)"
	    ;
    static char fmt_100[] = "(\002 This problem has simple spectrum.\002)";
    static char fmt_121[] = "(\002 There are infinitely many eigenvalues, bo\
unded below.\002)";
    static char fmt_102[] = "(\002 There is no continuous spectrum.\002)";
    static char fmt_110[] = "(\002 At endpoint A\002)";
    static char fmt_112[] = "(\002    the problem is regular;\002)";
    static char fmt_113[] = "(\002    the problem is singular;\002)";
    static char fmt_114[] = "(\002    it is nonoscillatory for all Ev.\002)";
    static char fmt_117[] = "(\002    It is limit circle.\002)";
    static char fmt_118[] = "(\002    It is limit point.\002)";
    static char fmt_119[] = "(\002    The constants for the Friedrichs bound\
ary conditions\002,\002 are\002,/,4e18.8)";
    static char fmt_111[] = "(\002 At endpoint B\002)";
    static char fmt_103[] = "(\002 There is continuous spectrum in [Ev, infi\
nity) where\002,\002 Ev =\002,g15.6)";
    static char fmt_105[] = "(\002 The set of eigenvalues is bounded below\
.\002)";
    static char fmt_120[] = "(\002 There appear to be infinitely many eigenv\
alues below\002,\002 the start\002,/,\002    of the continuous spectrum.\002)"
	    ;
    static char fmt_107[] = "(\002 There appear to be no eigenvalues below t\
he start of\002,\002 the continuous spectrum.\002)";
    static char fmt_108[] = "(\002 There appears to be 1 eigenvalue below th\
e start of the\002,\002 continuous spectrum.\002)";
    static char fmt_109[] = "(\002 There appear to be \002,i12,\002 eigenval\
ues below the start\002,/,\002    of the continuous spectrum.\002)";
    static char fmt_116[] = "(\002    it is nonoscillatory for Ev <\002,g15.\
6,\002 and oscillatory otherwise.\002)";
    static char fmt_106[] = "(\002 There are infinitely many negative and in\
finitely many\002,\002 positive\002,/,\002    eigenvalues (unbounded in eith\
er\002,\002 direction).\002)";
    static char fmt_115[] = "(\002    it is oscillatory for all Ev.\002)";
    static char fmt_104[] = "(\002 There is continuous spectrum consisting o\
f the entire \002,\002real line.\002)";
    static char fmt_122[] = "(\002 The nature of the spectrum is unknown; th\
ere is likely\002,\002 to be \002,/,\002 continuous spectrum.\002)";
    static char fmt_101[] = "(\002 This problem may have non-simple spectr\
um.\002)";

    /* Builtin functions */
    integer s_wsle(cilist *), e_wsle(), s_wsfe(cilist *), do_fio(integer *, 
	    char *, ftnlen), e_wsfe();

    /* Fortran I/O blocks */
    static cilist io___268 = { 0, 21, 0, 0, 0 };
    static cilist io___269 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___270 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___271 = { 0, 21, 0, fmt_121, 0 };
    static cilist io___272 = { 0, 21, 0, fmt_102, 0 };
    static cilist io___273 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___274 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___275 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___276 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___277 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___278 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___279 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___280 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___281 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___282 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___283 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___284 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___285 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___286 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___287 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___288 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___289 = { 0, 21, 0, fmt_103, 0 };
    static cilist io___290 = { 0, 21, 0, fmt_105, 0 };
    static cilist io___291 = { 0, 21, 0, fmt_120, 0 };
    static cilist io___292 = { 0, 21, 0, fmt_107, 0 };
    static cilist io___293 = { 0, 21, 0, fmt_108, 0 };
    static cilist io___294 = { 0, 21, 0, fmt_109, 0 };
    static cilist io___295 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___296 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___297 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___298 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___299 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___300 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___301 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___302 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___303 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___304 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___305 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___306 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___307 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___308 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___309 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___310 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___311 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___312 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___313 = { 0, 21, 0, fmt_106, 0 };
    static cilist io___314 = { 0, 21, 0, fmt_102, 0 };
    static cilist io___315 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___316 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___317 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___318 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___319 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___320 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___321 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___322 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___323 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___324 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___325 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___326 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___327 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___328 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___329 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___330 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___331 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___332 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___333 = { 0, 21, 0, fmt_104, 0 };
    static cilist io___334 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___335 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___336 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___337 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___338 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___339 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___340 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___341 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___342 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___343 = { 0, 21, 0, fmt_112, 0 };
    static cilist io___344 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___345 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___346 = { 0, 21, 0, fmt_114, 0 };
    static cilist io___347 = { 0, 21, 0, fmt_119, 0 };
    static cilist io___348 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___349 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___350 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___351 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___352 = { 0, 21, 0, fmt_106, 0 };
    static cilist io___353 = { 0, 21, 0, fmt_102, 0 };
    static cilist io___354 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___355 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___356 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___357 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___358 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___359 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___360 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___361 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___362 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___363 = { 0, 21, 0, fmt_122, 0 };
    static cilist io___364 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___365 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___366 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___367 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___368 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___369 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___370 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___371 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___372 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___373 = { 0, 21, 0, fmt_104, 0 };
    static cilist io___374 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___375 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___376 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___377 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___378 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___379 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___380 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___381 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___382 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___383 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___384 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___385 = { 0, 21, 0, fmt_100, 0 };
    static cilist io___386 = { 0, 21, 0, fmt_103, 0 };
    static cilist io___387 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___388 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___389 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___390 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___391 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___392 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___393 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___394 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___395 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___396 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___397 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___398 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___399 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___400 = { 0, 21, 0, fmt_101, 0 };
    static cilist io___401 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___402 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___403 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___404 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___405 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___406 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___407 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___408 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___409 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___410 = { 0, 21, 0, fmt_115, 0 };
    static cilist io___411 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___412 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___413 = { 0, 21, 0, fmt_123, 0 };
    static cilist io___414 = { 0, 21, 0, fmt_101, 0 };
    static cilist io___415 = { 0, 21, 0, fmt_103, 0 };
    static cilist io___416 = { 0, 21, 0, fmt_120, 0 };
    static cilist io___417 = { 0, 21, 0, fmt_107, 0 };
    static cilist io___418 = { 0, 21, 0, fmt_108, 0 };
    static cilist io___419 = { 0, 21, 0, fmt_109, 0 };
    static cilist io___420 = { 0, 21, 0, fmt_110, 0 };
    static cilist io___421 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___422 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___423 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___424 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___425 = { 0, 21, 0, fmt_111, 0 };
    static cilist io___426 = { 0, 21, 0, fmt_113, 0 };
    static cilist io___427 = { 0, 21, 0, fmt_116, 0 };
    static cilist io___428 = { 0, 21, 0, fmt_117, 0 };
    static cilist io___429 = { 0, 21, 0, fmt_118, 0 };
    static cilist io___430 = { 0, 21, 0, 0, 0 };



/*     Output (if requested) a description of the spectrum. */

    /* Parameter adjustments */
    --cev;
    --cspec;
    --reg;
    type__ -= 5;
    --lplc;
    --lc;

    /* Function Body */
    s_wsle(&io___268);
    e_wsle();
    if (type__[7] && type__[11]) {

/*     Category 1 */

	s_wsfe(&io___269);
	do_fio(&c__1, (char *)&c__1, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___270);
	e_wsfe();
	s_wsfe(&io___271);
	e_wsfe();
	s_wsfe(&io___272);
	e_wsfe();
	s_wsfe(&io___273);
	e_wsfe();
	if (reg[1]) {
	    s_wsfe(&io___274);
	    e_wsfe();
	} else {
	    s_wsfe(&io___275);
	    e_wsfe();
	    s_wsfe(&io___276);
	    e_wsfe();
	    if (lc[1]) {
		if (lplc[1]) {
		    s_wsfe(&io___277);
		    e_wsfe();
		}
	    } else {
		if (lplc[1]) {
		    s_wsfe(&io___278);
		    e_wsfe();
		}
	    }
	    s_wsfe(&io___279);
	    do_fio(&c__1, (char *)&(*a1), (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&(*a1p), (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&(*a2), (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&(*a2p), (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	s_wsfe(&io___280);
	e_wsfe();
	if (reg[2]) {
	    s_wsfe(&io___281);
	    e_wsfe();
	} else {
	    s_wsfe(&io___282);
	    e_wsfe();
	    s_wsfe(&io___283);
	    e_wsfe();
	    if (lc[2]) {
		if (lplc[2]) {
		    s_wsfe(&io___284);
		    e_wsfe();
		}
	    } else {
		if (lplc[2]) {
		    s_wsfe(&io___285);
		    e_wsfe();
		}
	    }
	    s_wsfe(&io___286);
	    do_fio(&c__1, (char *)&(*b1), (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&(*b2), (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
    }

    if (type__[7] && cspec[2] || type__[11] && cspec[1]) {

/*        Category 2 */

	s_wsfe(&io___287);
	do_fio(&c__1, (char *)&c__2, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___288);
	e_wsfe();
	s_wsfe(&io___289);
	do_fio(&c__1, (char *)&(*cutoff), (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (*lastev == -5) {
	    s_wsfe(&io___290);
	    e_wsfe();
	    s_wsfe(&io___291);
	    e_wsfe();
	} else {
	    if (*lastev == 0) {
		s_wsfe(&io___292);
		e_wsfe();
	    } else {
		if (*lastev == 1) {
		    s_wsfe(&io___293);
		    e_wsfe();
		} else {
		    s_wsfe(&io___294);
		    do_fio(&c__1, (char *)&(*lastev), (ftnlen)sizeof(integer))
			    ;
		    e_wsfe();
		}
	    }
	}
	s_wsfe(&io___295);
	e_wsfe();
	if (reg[1]) {
	    s_wsfe(&io___296);
	    e_wsfe();
	} else {
	    s_wsfe(&io___297);
	    e_wsfe();
	    if (cspec[1]) {
		s_wsfe(&io___298);
		do_fio(&c__1, (char *)&cev[1], (ftnlen)sizeof(doublereal));
		e_wsfe();
	    } else {
		s_wsfe(&io___299);
		e_wsfe();
		s_wsfe(&io___300);
		do_fio(&c__1, (char *)&(*a1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a1p), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2p), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[1]) {
		if (lplc[1]) {
		    s_wsfe(&io___301);
		    e_wsfe();
		}
	    } else {
		if (lplc[1]) {
		    s_wsfe(&io___302);
		    e_wsfe();
		}
	    }
	}
	s_wsfe(&io___303);
	e_wsfe();
	if (reg[2]) {
	    s_wsfe(&io___304);
	    e_wsfe();
	} else {
	    s_wsfe(&io___305);
	    e_wsfe();
	    if (cspec[2]) {
		s_wsfe(&io___306);
		do_fio(&c__1, (char *)&cev[2], (ftnlen)sizeof(doublereal));
		e_wsfe();
	    } else {
		s_wsfe(&io___307);
		e_wsfe();
		s_wsfe(&io___308);
		do_fio(&c__1, (char *)&(*b1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*b2), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[2]) {
		if (lplc[2]) {
		    s_wsfe(&io___309);
		    e_wsfe();
		}
	    } else {
		if (lplc[2]) {
		    s_wsfe(&io___310);
		    e_wsfe();
		}
	    }
	}
    }

    if (type__[7] && (type__[12] && lc[2] && ! cspec[2]) || type__[11] && (
	    type__[8] && lc[1] && ! cspec[1])) {

/*        Category 3 */

	s_wsfe(&io___311);
	do_fio(&c__1, (char *)&c__3, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___312);
	e_wsfe();
	s_wsfe(&io___313);
	e_wsfe();
	s_wsfe(&io___314);
	e_wsfe();
	s_wsfe(&io___315);
	e_wsfe();
	if (reg[1]) {
	    s_wsfe(&io___316);
	    e_wsfe();
	} else {
	    s_wsfe(&io___317);
	    e_wsfe();
	    if (type__[8]) {
		s_wsfe(&io___318);
		e_wsfe();
	    } else {
		s_wsfe(&io___319);
		e_wsfe();
		s_wsfe(&io___320);
		do_fio(&c__1, (char *)&(*a1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a1p), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2p), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[1]) {
		if (lplc[1]) {
		    s_wsfe(&io___321);
		    e_wsfe();
		}
	    } else {
		if (lplc[1]) {
		    s_wsfe(&io___322);
		    e_wsfe();
		}
	    }
	}
	s_wsfe(&io___323);
	e_wsfe();
	if (reg[2]) {
	    s_wsfe(&io___324);
	    e_wsfe();
	} else {
	    s_wsfe(&io___325);
	    e_wsfe();
	    if (type__[12]) {
		s_wsfe(&io___326);
		e_wsfe();
	    } else {
		s_wsfe(&io___327);
		e_wsfe();
		s_wsfe(&io___328);
		do_fio(&c__1, (char *)&(*b1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*b2), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[2]) {
		if (lplc[2]) {
		    s_wsfe(&io___329);
		    e_wsfe();
		}
	    } else {
		if (lplc[2]) {
		    s_wsfe(&io___330);
		    e_wsfe();
		}
	    }
	}
    }

    if (type__[7] && (! lc[2] && type__[12] && ! cspec[2]) || type__[11] && (!
	     lc[1] && type__[8] && ! cspec[1])) {

/*        Category 4 */

	s_wsfe(&io___331);
	do_fio(&c__1, (char *)&c__4, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___332);
	e_wsfe();
	s_wsfe(&io___333);
	e_wsfe();
	s_wsfe(&io___334);
	e_wsfe();
	if (reg[1]) {
	    s_wsfe(&io___335);
	    e_wsfe();
	} else {
	    s_wsfe(&io___336);
	    e_wsfe();
	    if (type__[8]) {
		s_wsfe(&io___337);
		e_wsfe();
	    } else {
		s_wsfe(&io___338);
		e_wsfe();
		s_wsfe(&io___339);
		do_fio(&c__1, (char *)&(*a1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a1p), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*a2p), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[1]) {
		if (lplc[1]) {
		    s_wsfe(&io___340);
		    e_wsfe();
		}
	    } else {
		if (lplc[1]) {
		    s_wsfe(&io___341);
		    e_wsfe();
		}
	    }
	}
	s_wsfe(&io___342);
	e_wsfe();
	if (reg[2]) {
	    s_wsfe(&io___343);
	    e_wsfe();
	} else {
	    s_wsfe(&io___344);
	    e_wsfe();
	    if (type__[12]) {
		s_wsfe(&io___345);
		e_wsfe();
	    } else {
		s_wsfe(&io___346);
		e_wsfe();
		s_wsfe(&io___347);
		do_fio(&c__1, (char *)&(*b1), (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&(*b2), (ftnlen)sizeof(doublereal));
		e_wsfe();
	    }
	    if (lc[2]) {
		if (lplc[2]) {
		    s_wsfe(&io___348);
		    e_wsfe();
		}
	    } else {
		if (lplc[2]) {
		    s_wsfe(&io___349);
		    e_wsfe();
		}
	    }
	}
    }

    if (lc[1] && type__[8] && (lc[2] && type__[12])) {

/*        Category 5 */

	s_wsfe(&io___350);
	do_fio(&c__1, (char *)&c__5, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___351);
	e_wsfe();
	s_wsfe(&io___352);
	e_wsfe();
	s_wsfe(&io___353);
	e_wsfe();
	s_wsfe(&io___354);
	e_wsfe();
	s_wsfe(&io___355);
	e_wsfe();
	s_wsfe(&io___356);
	e_wsfe();
	s_wsfe(&io___357);
	e_wsfe();
	s_wsfe(&io___358);
	e_wsfe();
	s_wsfe(&io___359);
	e_wsfe();
	s_wsfe(&io___360);
	e_wsfe();
	s_wsfe(&io___361);
	e_wsfe();
    }

    if (type__[8] && ! lc[1] && ! cspec[1] && (type__[12] && ! lc[2] && ! 
	    cspec[2])) {

/*        Category 6 */

	s_wsfe(&io___362);
	do_fio(&c__1, (char *)&c__6, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___363);
	e_wsfe();
	s_wsfe(&io___364);
	e_wsfe();
	s_wsfe(&io___365);
	e_wsfe();
	s_wsfe(&io___366);
	e_wsfe();
	s_wsfe(&io___367);
	e_wsfe();
	s_wsfe(&io___368);
	e_wsfe();
	s_wsfe(&io___369);
	e_wsfe();
	s_wsfe(&io___370);
	e_wsfe();
	s_wsfe(&io___371);
	e_wsfe();
    }

    if (type__[8] && type__[12] && ! (cspec[1] || cspec[2]) && (lc[1] && ! lc[
	    2] || ! lc[1] && lc[2])) {

/*        Category 7 */

	s_wsfe(&io___372);
	do_fio(&c__1, (char *)&c__7, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___373);
	e_wsfe();
	s_wsfe(&io___374);
	e_wsfe();
	s_wsfe(&io___375);
	e_wsfe();
	s_wsfe(&io___376);
	e_wsfe();
	if (lc[1]) {
	    if (lplc[1]) {
		s_wsfe(&io___377);
		e_wsfe();
	    }
	} else {
	    if (lplc[1]) {
		s_wsfe(&io___378);
		e_wsfe();
	    }
	}
	s_wsfe(&io___379);
	e_wsfe();
	s_wsfe(&io___380);
	e_wsfe();
	s_wsfe(&io___381);
	e_wsfe();
	if (lc[2]) {
	    if (lplc[2]) {
		s_wsfe(&io___382);
		e_wsfe();
	    }
	} else {
	    if (lplc[2]) {
		s_wsfe(&io___383);
		e_wsfe();
	    }
	}
    }

    if (lc[1] && type__[8] && cspec[2] || lc[2] && type__[12] && cspec[1]) {

/*        Category 8 */

	s_wsfe(&io___384);
	do_fio(&c__1, (char *)&c__8, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___385);
	e_wsfe();
	s_wsfe(&io___386);
	do_fio(&c__1, (char *)&(*cutoff), (ftnlen)sizeof(doublereal));
	e_wsfe();
	s_wsfe(&io___387);
	e_wsfe();
	s_wsfe(&io___388);
	e_wsfe();
	if (cspec[1]) {
	    s_wsfe(&io___389);
	    do_fio(&c__1, (char *)&cev[1], (ftnlen)sizeof(doublereal));
	    e_wsfe();
	} else {
	    s_wsfe(&io___390);
	    e_wsfe();
	}
	if (lc[1]) {
	    if (lplc[1]) {
		s_wsfe(&io___391);
		e_wsfe();
	    }
	} else {
	    if (lplc[1]) {
		s_wsfe(&io___392);
		e_wsfe();
	    }
	}
	s_wsfe(&io___393);
	e_wsfe();
	s_wsfe(&io___394);
	e_wsfe();
	if (cspec[2]) {
	    s_wsfe(&io___395);
	    do_fio(&c__1, (char *)&cev[2], (ftnlen)sizeof(doublereal));
	    e_wsfe();
	} else {
	    s_wsfe(&io___396);
	    e_wsfe();
	}
	if (lc[2]) {
	    if (lplc[2]) {
		s_wsfe(&io___397);
		e_wsfe();
	    }
	} else {
	    if (lplc[2]) {
		s_wsfe(&io___398);
		e_wsfe();
	    }
	}
    }

    if (! lc[1] && type__[8] && ! cspec[1] && cspec[2] || ! lc[2] && type__[
	    12] && ! cspec[2] && cspec[1]) {

/*        Category 9 */

	s_wsfe(&io___399);
	do_fio(&c__1, (char *)&c__9, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___400);
	e_wsfe();
	s_wsfe(&io___401);
	e_wsfe();
	s_wsfe(&io___402);
	e_wsfe();
	if (cspec[1]) {
	    s_wsfe(&io___403);
	    do_fio(&c__1, (char *)&cev[1], (ftnlen)sizeof(doublereal));
	    e_wsfe();
	} else {
	    s_wsfe(&io___404);
	    e_wsfe();
	}
	if (lc[1]) {
	    if (lplc[1]) {
		s_wsfe(&io___405);
		e_wsfe();
	    }
	} else {
	    if (lplc[1]) {
		s_wsfe(&io___406);
		e_wsfe();
	    }
	}
	s_wsfe(&io___407);
	e_wsfe();
	s_wsfe(&io___408);
	e_wsfe();
	if (cspec[2]) {
	    s_wsfe(&io___409);
	    do_fio(&c__1, (char *)&cev[2], (ftnlen)sizeof(doublereal));
	    e_wsfe();
	} else {
	    s_wsfe(&io___410);
	    e_wsfe();
	}
	if (lc[2]) {
	    if (lplc[2]) {
		s_wsfe(&io___411);
		e_wsfe();
	    }
	} else {
	    if (lplc[2]) {
		s_wsfe(&io___412);
		e_wsfe();
	    }
	}
    }

    if (cspec[1] && cspec[2]) {

/*        Category 10 */

	s_wsfe(&io___413);
	do_fio(&c__1, (char *)&c__10, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___414);
	e_wsfe();
	s_wsfe(&io___415);
	do_fio(&c__1, (char *)&(*cutoff), (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (*lastev == -5) {
	    s_wsfe(&io___416);
	    e_wsfe();
	} else {
	    if (*lastev == 0) {
		s_wsfe(&io___417);
		e_wsfe();
	    } else {
		if (*lastev == 1) {
		    s_wsfe(&io___418);
		    e_wsfe();
		} else {
		    s_wsfe(&io___419);
		    do_fio(&c__1, (char *)&(*lastev), (ftnlen)sizeof(integer))
			    ;
		    e_wsfe();
		}
	    }
	}
	s_wsfe(&io___420);
	e_wsfe();
	s_wsfe(&io___421);
	e_wsfe();
	s_wsfe(&io___422);
	do_fio(&c__1, (char *)&cev[1], (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (lc[1]) {
	    if (lplc[1]) {
		s_wsfe(&io___423);
		e_wsfe();
	    }
	} else {
	    if (lplc[1]) {
		s_wsfe(&io___424);
		e_wsfe();
	    }
	}
	s_wsfe(&io___425);
	e_wsfe();
	s_wsfe(&io___426);
	e_wsfe();
	s_wsfe(&io___427);
	do_fio(&c__1, (char *)&cev[2], (ftnlen)sizeof(doublereal));
	e_wsfe();
	if (lc[2]) {
	    if (lplc[2]) {
		s_wsfe(&io___428);
		e_wsfe();
	    }
	} else {
	    if (lplc[2]) {
		s_wsfe(&io___429);
		e_wsfe();
	    }
	}
    }
    s_wsle(&io___430);
    e_wsle();
    return 0;
} /* dscrip_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int extrap_(doublereal *v, doublereal *tol, integer *irow, 
	integer *maxcol, logical *full, logical *tight, integer *mode, 
	doublereal *vsave, integer *iprint, doublereal *error, logical *done)
{
    /* Format strings */
    static char fmt_50[] = "(\002 In EXTRAP: using Wynn`s acceleration; rate\
 = \002,f8.5)";

    /* System generated locals */
    integer i__1, i__2, i__3;
    real r__1, r__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);
    double log(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();

    /* Local variables */
    static integer i__, j;
    static doublereal r__[96]	/* was [12][8] */, t, w[440]	/* was [40][
	    11] */, eps, tol1, tol2, diff;
    static integer ncol, imin, maxj;
    static doublereal rtol, rlow, rhigh, etemp, ratio[12], vtemp;

    /* Fortran I/O blocks */
    static cilist io___448 = { 0, 21, 0, fmt_50, 0 };



/*     Use Richardson's h**2 extrapolation (based on doubling) when */
/*     suitable, otherwise use Wynn's acceleration scheme. */

/*     Input: */
/*        V       = real value at current level. */
/*        TOL     = real tolerance. */
/*        IROW    = integer giving current row index (1 .le. IROW). */
/*        MAXCOL  = integer giving maximum number of columns in table. */
/*        FULL    = logical, True iff entire table is to be computed. */
/*        TIGHT   = logical, True for conservative convergence tests. */
/*        MODE    = integer, value is */
/*                  0   both Richardson and Wynn algorithms can be used; */
/*                  1   only Richardson is used; */
/*                  2   only Wynn is used. */
/*        IPRINT  = integer controlling amount of printing. */
/*     Output: */
/*        V       = real, best output estimate. */
/*        VSAVE   = real vector, holds previous level values. */
/*        DONE    = logical, True iff Error is sufficiently small. */

/*     If FULL is True, then the entire acceleration array is produced */
/*     (through row IROW); if False, then only the next row is appended. */
/*     Hence, the choice of FULL = True requires more work, but it may */
/*     save some global storage.  The vector VSAVE contains the V values */
/*     for levels 0 through max(IROW-1,MAXCOL). */


/*     The local arrays RATIO(*), R(*,*), and W(*,*) must be declared to */
/*     have at least as many rows as the value of MAXLVL initialized in */
/*     routine START. */


    /* Parameter adjustments */
    --vsave;

    /* Function Body */
    eps = (float).2;
    *done = FALSE_;
    etemp = 1. / slreal_1.u;
    vtemp = *v;
    vsave[*irow] = *v;
    tol1 = *tol;
    tol2 = *tol / (float)3.;
    if (*mode == 2) {
	maxj = *maxcol;
	goto L40;
    } else {
	maxj = 11;
    }
/* Computing MAX */
    r__1 = (float)3., r__2 = (float)4.2 - (*irow - 1) * (float).2;
    rlow = dmax(r__1,r__2);
/* Computing MIN */
    r__1 = (float)5., r__2 = (*irow - 1) * (float).2 + (float)3.8;
    rhigh = dmin(r__1,r__2);
    rtol = slreal_1.u;

/*     Analyze the rate of convergence to determine NCOL and tolerances. */

    ncol = 2;
    i__1 = *irow;
    for (i__ = 1; i__ <= i__1; ++i__) {
	r__[i__ - 1] = vsave[i__];
	ratio[i__ - 1] = 1. / slreal_1.u;
	if (i__ >= 3) {
	    t = r__[i__ - 1] - r__[i__ - 2];
	    if (t != 0.) {
		ratio[i__ - 1] = (r__[i__ - 2] - r__[i__ - 3]) / t;
	    } else {
		*v = r__[i__ - 1];
		*done = TRUE_;
		return 0;
	    }
	    if (ratio[i__ - 1] >= rlow && ratio[i__ - 1] <= rhigh || ! (*
		    tight)) {
		rtol = tol1;
/* Computing MIN */
		i__2 = *maxcol, i__3 = ncol + 1;
		ncol = min(i__2,i__3);
	    } else {
		rtol = tol2;
		if (ratio[i__ - 1] < 0.) {
		    rtol = tol2 * .1;
		}
		if (ratio[i__ - 1] < 2.) {
		    ncol = 2;
		}
	    }
	}
/* L10: */
    }
    if (*full) {
	imin = 2;
    } else {
	imin = *irow;
    }

/*     Use Richardson's h^2 extrapolation.  The number of columns used */
/*     is a function of the amount of data (IROW), the requested order */
/*     (MAXCOL), the observed rate of convergence (NCOL), and the amount */
/*     of storage allocated to R(*,*). */

    i__1 = *irow;
    for (i__ = imin; i__ <= i__1; ++i__) {
/* Computing MIN */
	i__3 = min(i__,ncol);
	i__2 = min(i__3,8);
	for (j = 2; j <= i__2; ++j) {
	    i__3 = j - 1;
	    diff = (r__[i__ + (j - 1) * 12 - 13] - r__[i__ - 1 + (j - 1) * 12 
		    - 13]) / (pow_ii(&c__4, &i__3) - 1);
	    r__[i__ + j * 12 - 13] = r__[i__ + (j - 1) * 12 - 13] + diff;
	    if (! (*full) || i__ == *irow) {
		t = abs(diff);
		if (t <= etemp) {
		    etemp = t;
		    vtemp = r__[i__ + j * 12 - 13];
		    if (t <= rtol) {
			*done = TRUE_;
			*v = vtemp;
			*error = etemp;
			return 0;
		    }
		}
	    }
/* L20: */
	}
/* L30: */
    }
    *v = vtemp;
    *error = etemp;
    if (*irow < 4) {
	return 0;
    }

/*     Test for rate of convergence other than second order. */

    if ((d__1 = ratio[*irow - 1] - ratio[*irow - 2], abs(d__1)) + (d__2 = 
	    ratio[*irow - 2] - ratio[*irow - 3], abs(d__2)) > eps) {
	return 0;
    }
    if ((float)3.5 < ratio[*irow - 1] && ratio[*irow - 1] < (float)4.5) {
	return 0;
    }
    if (ratio[*irow - 1] < 1.) {
	return 0;
    }
    if (*mode != 0) {
	return 0;
    }

/*     Use Wynn's algorithm. */

L40:
    if (*iprint >= 4) {
	diff = log((d__1 = ratio[*irow - 1], abs(d__1))) / log(2.);
	s_wsfe(&io___448);
	do_fio(&c__1, (char *)&diff, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    w[0] = vsave[1];
    etemp = 1. / slreal_1.u;
    i__1 = *irow;
    for (i__ = 2; i__ <= i__1; ++i__) {
	w[i__ - 1] = vsave[i__];
	diff = w[i__ - 1] - w[i__ - 2];
	if (i__ == *irow || *mode == 2) {
	    t = abs(diff);
	    if (t <= etemp) {
		etemp = t;
		vtemp = w[i__ - 1];
		if (t <= tol2) {
		    *v = vtemp;
		    *error = etemp;
		    *done = TRUE_;
		    return 0;
		}
	    }
	}
	if (diff != 0.) {
	    w[i__ + 39] = 1. / diff;
	} else {
	    *v = w[i__ - 1];
	    *error = 0.;
	    *done = TRUE_;
	    return 0;
	}
/* L60: */
    }
    i__1 = min(*irow,maxj);
    for (j = 3; j <= i__1; ++j) {
	i__2 = *irow;
	for (i__ = j; i__ <= i__2; ++i__) {
	    diff = w[i__ + (j - 1) * 40 - 41] - w[i__ - 1 + (j - 1) * 40 - 41]
		    ;
	    if (diff != 0.) {
		diff = 1. / diff;
	    } else {
		if (j % 2 == 0) {
		    *v = w[i__ + (j - 1) * 40 - 41];
		    *error = 0.;
		    *done = TRUE_;
		    return 0;
		} else {
/* Computing 2nd power */
		    d__1 = slreal_1.u;
		    diff = 1. / (d__1 * d__1);
		}
	    }
	    w[i__ + j * 40 - 41] = w[i__ - 1 + (j - 2) * 40 - 41] + diff;
	    if (j % 2 == 1 && (i__ == *irow || *mode == 2)) {
		t = abs(diff);
		if (t <= etemp) {
		    etemp = t;
		    vtemp = w[i__ + j * 40 - 41];
		    if (t <= tol2) {
			*v = vtemp;
			*error = etemp;
			*done = TRUE_;
			return 0;
		    }
		}
	    }
/* L70: */
	}
/* L80: */
    }
    *v = vtemp;
    *error = etemp;
    return 0;
} /* extrap_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int getef_(doublereal *ev, doublereal *efnorm, integer *
	iprint, doublereal *x, logical *efin)
{
    /* Format strings */
    static char fmt_21[] = "(\002 Coordinates of matching point =\002,2i6)";
    static char fmt_35[] = "(g16.6,3d15.6)";
    static char fmt_61[] = "(\002  DuHat jump, ratio =\002,2d24.15)";
    static char fmt_62[] = "(\002  UHat jump, ratio =\002,2d24.15)";
    static char fmt_65[] = "(\002  EFnorm =\002,d24.15)";
    static char fmt_75[] = "(10x,\002x\002,15x,\002Uhat(x)\002,13x,\002PUhat\
`(x)\002)";
    static char fmt_80[] = "(g16.6,2d20.8)";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer pow_ii(integer *, integer *), s_wsfe(cilist *), do_fio(integer *, 
	    char *, ftnlen), e_wsfe();
    double pow_dd(doublereal *, doublereal *), exp(doublereal), d_sign(
	    doublereal *, doublereal *), sqrt(doublereal), log(doublereal);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal t, v, w, z__, dv, dw;
    static integer js;
    static doublereal om;
    static integer ju;
    static doublereal pn, rn;
    static integer jx;
    static doublereal chi;
    static integer jdu;
    static doublereal hom, pdv, pdw, psi, tau;
    static integer mode;
    static doublereal dpsi, prod;
    static integer klvl;
    static doublereal fsum;
    extern /* Subroutine */ int step_(doublereal *, doublereal *, doublereal *
	    , doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    ;
    static doublereal vnew, wnew;
    static logical symm;
    static doublereal rsum, halfh, scale;
    static logical allok;
    static doublereal tauhh;
    static integer jlast;
    static doublereal ratio, fnorm, xleft, rnorm, fscale;
    static integer middle[2];
    static doublereal oscale, rscale, taumax, xright;

    /* Fortran I/O blocks */
    static cilist io___474 = { 0, 21, 0, fmt_21, 0 };
    static cilist io___482 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___489 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___490 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___495 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___499 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___500 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___501 = { 0, 21, 0, fmt_35, 0 };
    static cilist io___503 = { 0, 21, 0, fmt_61, 0 };
    static cilist io___504 = { 0, 21, 0, fmt_62, 0 };
    static cilist io___505 = { 0, 21, 0, fmt_65, 0 };
    static cilist io___507 = { 0, 21, 0, fmt_75, 0 };
    static cilist io___508 = { 0, 21, 0, fmt_80, 0 };



/*     Compute an eigenfunction for one fixed mesh. */


    /* Parameter adjustments */
    efin -= 3;
    --x;

    /* Function Body */
    klvl = pow_ii(&c__2, &slint_1.level);

/*     For this EV and mesh, calculate FSCALE, RSCALE, and MIDDLE(*). */
/*         FSCALE = sum of logs of scale factors 1 through m */
/*         RSCALE = sum of logs of scale factors m+1 through N */
/*         MIDDLE(1), MIDDLE(2) describe the coordinates of the matching */
/*            point M for the shooting; in particular, */
/*            M = X(MIDDLE(1)-1) + MIDDLE(2)*H(MIDDLE(1)-1)/2**LEVEL */
/*     The matching point is chosen to be roughly (a+b)/2 if either */
/*     a = -b and Tau(x) > 0 near 0, or if Tau(x) > 0 for all x; */
/*     otherwise, it is chosen to roughly maximize Tau(x). */

    fscale = 0.;
    rscale = 0.;
    taumax = -1. / slreal_1.u;
    allok = TRUE_;
    symm = FALSE_;
    if (slreal_1.a == -slreal_1.b) {
	symm = TRUE_;
    }
    efin[3] = TRUE_;
    efin[4] = TRUE_;
    efin[5] = TRUE_;
    efin[6] = TRUE_;
    jx = (slint_1.nxinit + 1) / 2;
    i__1 = slint_1.nxinit;
    for (i__ = 2; i__ <= i__1; ++i__) {
	mode = 0;
	xleft = x[i__ - 1];
	h__ = x[i__] - xleft;
	if (i__ == 2) {
	    if (slclss_1.kclass[0] >= 9) {
		mode = 1;
	    }
	    if (! sllog_1.afin) {
		mode = 3;
		xleft = 0.;
		h__ = -1. / x[2];
	    }
	}
	if (i__ == slint_1.nxinit) {
	    if (slclss_1.kclass[1] >= 9) {
		mode = 2;
	    }
	    if (! sllog_1.bfin) {
		mode = 4;
		h__ = 1. / x[i__ - 1];
		xleft = -h__;
	    }
	}
	h__ /= klvl;
	halfh = h__ * .5;
	i__2 = klvl;
	for (j = 1; j <= i__2; ++j) {
	    z__ = xleft + halfh;
	    xleft += h__;
	    step_(&z__, &h__, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi, &
		    scale, &mode);
	    if (tau < 0.) {
		allok = FALSE_;
	    }
	    if (tau > taumax) {
		taumax = tau;
		middle[0] = i__;
		middle[1] = j;
		fscale = fscale + rscale + scale;
		oscale = scale;
		rscale = 0.;
	    } else {
		rscale += scale;
	    }
/* L10: */
	}
	if (i__ == jx && tau < 0.) {
	    symm = FALSE_;
	}
/* L20: */
    }
    if (allok || symm) {
	middle[0] = jx;
/* Computing MAX */
	i__1 = klvl - 1;
	middle[1] = max(i__1,1);
    }
    if (! sllog_1.afin || ! sllog_1.bfin) {

/*        Don't split near infinity! */

	if (! sllog_1.afin && middle[0] == 2) {
	    if (sllog_1.reg[1]) {
		middle[0] = slint_1.nxinit;
	    } else {
		middle[0] = jx;
	    }
/* Computing MAX */
	    i__1 = klvl - 1;
	    middle[1] = max(i__1,1);
	}
	if (! sllog_1.bfin && middle[0] == slint_1.nxinit) {
	    if (sllog_1.reg[0]) {
		middle[0] = 2;
	    } else {
		middle[0] = jx;
	    }
	    middle[1] = 1;
	}
    }
    if (slint_1.level > 1 && middle[1] == klvl) {
	middle[1] = klvl - 1;
	fscale -= oscale;
	rscale += oscale;
    }
    if (*iprint >= 4) {
	s_wsfe(&io___474);
	do_fio(&c__1, (char *)&middle[0], (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&middle[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    ju = slint_1.nxinit;
    jdu = slint_1.nxinit << 1;
    js = slint_1.nxinit * 3;

/*     Shoot from x=A to the middle. */

    v = slreal_1.a2 - slreal_1.a2p * *ev;
    pdv = slreal_1.a1 - slreal_1.a1p * *ev;
    fnorm = 0.;
    if (slclss_1.kclass[0] < 9) {
/* Computing MAX */
	d__1 = abs(v), d__2 = abs(pdv);
	scale = max(d__1,d__2);
	v /= scale;
	pdv /= scale;
	mode = 0;
    } else {
	v = 1.;
	pdv = slclss_1.d__[3];
	mode = 1;
    }
    if (! sllog_1.afin) {
	mode = 3;
    }
    fsum = -fscale;
    if (*iprint >= 5 && mode == 0) {
	s_wsfe(&io___482);
	do_fio(&c__1, (char *)&x[1], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&v, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&pdv, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fsum, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    i__1 = middle[0];
    for (i__ = 2; i__ <= i__1; ++i__) {
	x[ju + i__ - 1] = v;
	x[jdu + i__ - 1] = pdv;
	x[js + i__ - 1] = fsum;
	if (mode == 0) {
	    xleft = x[i__ - 1];
	    h__ = x[i__] - xleft;
	} else {
	    xleft = 0.;
	    if (mode == 1) {
		d__1 = (x[2] - x[1]) / slclss_1.d__[0];
		h__ = pow_dd(&d__1, slclss_1.eta);
	    } else {
		h__ = -1. / x[2];
	    }
	}
	h__ /= klvl;
	halfh = h__ * .5;
	jlast = klvl;
	if (i__ == middle[0]) {
	    jlast = middle[1];
	}
	i__2 = jlast;
	for (j = 1; j <= i__2; ++j) {
	    z__ = xleft + halfh;
	    xleft += h__;
	    step_(&z__, &h__, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi, &
		    scale, &mode);
	    dv = pdv / pn;
	    if (abs(tau) * h__ * h__ >= 1e-4) {
		if ((fsum + scale) * 2. > -slreal_1.under) {
		    fnorm += rn * (psi * dpsi * (v * v - dv * dv / tau) / 2. 
			    + v * psi * dv * psi) * exp((fsum + scale) * 2.);
		    if (fsum * 2. > -slreal_1.under) {
			fnorm += rn * exp(fsum * 2.) * h__ * (v * v + dv * dv 
				/ tau) / 2.;
		    }
		}
	    } else {
		if (fsum * 2. > -slreal_1.under) {
		    tauhh = tau * h__ * h__;
/* Computing 2nd power */
		    d__1 = h__ * dv;
		    fnorm += rn * h__ * exp(fsum * 2.) * (v * v * (tauhh * (
			    tauhh / 5. - 1.) / 3. + 1.) + h__ * v * dv * (
			    tauhh * (tauhh * 2. / 15. - 1.) / 3. + 1.) + d__1 
			    * d__1 * (tauhh * (tauhh * 2. / 21. - 1.) / 5. + 
			    1.) / 3.);
		}
	    }
	    fsum += scale;
	    vnew = dpsi * v + psi * dv;
	    pdv = -pn * tau * psi * v + dpsi * pdv;
	    v = vnew;
/* L30: */
	}
	if (mode == 1) {

/*           Convert from V(t) to u(x). */

	    if (slclss_1.eta[1] < 0.) {
		x[ju + 1] = 1. / slreal_1.u;
		efin[3] = FALSE_;
	    } else {
		if (slclss_1.eta[1] == 0.) {
		    x[ju + 1] = slclss_1.d__[1];
		} else {
		    x[ju + 1] = 0.;
		}
	    }
	    z__ = slclss_1.eta[0] * slclss_1.eta[1] + slclss_1.ep[0] - 1.;
	    if (z__ < 0.) {
		d__1 = 1. / slreal_1.u;
		x[jdu + 1] = d_sign(&d__1, &slclss_1.eta[1]);
		efin[4] = FALSE_;
	    } else {
		if (z__ > 0.) {
		    x[jdu + 1] = 0.;
		} else {
		    d__1 = slclss_1.eta[0] * slclss_1.eta[1];
		    x[jdu + 1] = slclss_1.d__[1] * slclss_1.cp[0] * 
			    slclss_1.eta[0] * slclss_1.eta[1] / pow_dd(
			    slclss_1.d__, &d__1);
		}
	    }
	    h__ = x[2] - slreal_1.a;
	    d__1 = slclss_1.ep[0] - 1.;
	    pn = slclss_1.cp[0] * pow_dd(&h__, &d__1);
	    d__1 = h__ / slclss_1.d__[0];
	    t = pow_dd(&d__1, slclss_1.eta);
	    chi = slclss_1.d__[1] * pow_dd(&t, &slclss_1.eta[1]);
	    v *= chi;
	    d__1 = 1. - slclss_1.emu[0] * 2.;
	    pdv = pn * slclss_1.eta[0] * (slclss_1.eta[1] * v + chi * pdv * 
		    pow_dd(&t, &d__1));
	    if (*iprint >= 5) {
		s_wsfe(&io___489);
		do_fio(&c__1, (char *)&x[1], (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&x[ju + 1], (ftnlen)sizeof(doublereal));
		do_fio(&c__1, (char *)&x[jdu + 1], (ftnlen)sizeof(doublereal))
			;
		e_wsfe();
	    }
	}
	mode = 0;
	if (*iprint >= 5) {
	    s_wsfe(&io___490);
	    do_fio(&c__1, (char *)&xleft, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&v, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&pdv, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&fsum, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/* L40: */
    }

/*     Shoot from x=B to the middle. */

    rnorm = 0.;
    mode = 0;
    if (slclss_1.kclass[1] < 9) {
/* Computing MAX */
	d__1 = abs(slreal_1.b1), d__2 = abs(slreal_1.b2);
	scale = max(d__1,d__2);
	w = -slreal_1.b2 / scale;
	pdw = slreal_1.b1 / scale;
	mode = 0;
    } else {
	w = -1.;
	pdw = -slclss_1.d__[7];
	mode = 2;
    }
    if (! sllog_1.bfin) {
	mode = 4;
    }
    rsum = -rscale;
    if (*iprint >= 5 && mode == 0) {
	s_wsfe(&io___495);
	do_fio(&c__1, (char *)&x[slint_1.nxinit], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&pdw, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&rsum, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    i__1 = middle[0];
    for (i__ = slint_1.nxinit; i__ >= i__1; --i__) {
	x[ju + i__] = w;
	x[jdu + i__] = pdw;
	x[js + i__] = rsum;
	if (mode == 0) {
	    xright = x[i__];
	    h__ = xright - x[i__ - 1];
	} else {
	    xright = 0.;
	    if (mode == 2) {
		d__1 = (x[slint_1.nxinit] - x[slint_1.nxinit - 1]) / 
			slclss_1.d__[4];
		h__ = pow_dd(&d__1, &slclss_1.eta[2]);
	    } else {
		h__ = 1. / x[i__ - 1];
	    }
	}
	h__ /= klvl;
	halfh = h__ * .5;
	jlast = klvl;
	if (i__ == middle[0]) {
	    jlast -= middle[1];
	}
	i__2 = jlast;
	for (j = 1; j <= i__2; ++j) {
	    z__ = xright - halfh;
	    xright -= h__;
	    step_(&z__, &h__, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi, &
		    scale, &mode);
	    dw = pdw / pn;
	    if (abs(tau) * h__ * h__ >= 1e-4) {
		if ((rsum + scale) * 2. > -slreal_1.under) {
		    rnorm += rn * (psi * dpsi * (w * w - dw * dw / tau) / 2. 
			    - w * psi * dw * psi) * exp((rsum + scale) * 2.);
		    if (rsum * 2. > -slreal_1.under) {
			rnorm += rn * exp(rsum * 2.) * h__ * (w * w + dw * dw 
				/ tau) / 2.;
		    }
		}
	    } else {
		if (rsum * 2. > -slreal_1.under) {
		    tauhh = tau * h__ * h__;
/* Computing 2nd power */
		    d__1 = h__ * dw;
		    rnorm += rn * h__ * exp(rsum * 2.) * (w * w * (tauhh * (
			    tauhh / 5. - 1.) / 3. + 1.) - h__ * w * dw * (
			    tauhh * (tauhh * 2. / 15. - 1.) / 3. + 1.) + d__1 
			    * d__1 * (tauhh * (tauhh * 2. / 21. - 1.) / 5. + 
			    1.) / 3.);
		}
	    }
	    rsum += scale;
	    wnew = dpsi * w - psi * dw;
	    pdw = pn * tau * psi * w + dpsi * pdw;
	    w = wnew;
/* L50: */
	}
	if (mode == 2) {

/*           Convert from V(t) to u(x). */

	    if (slclss_1.eta[3] < 0.) {
		x[ju + slint_1.nxinit] = -1. / slreal_1.u;
		efin[5] = FALSE_;
	    } else {
		if (slclss_1.eta[3] == 0.) {
		    x[ju + slint_1.nxinit] = -slclss_1.d__[5];
		} else {
		    x[ju + slint_1.nxinit] = 0.;
		}
	    }
	    z__ = slclss_1.eta[2] * slclss_1.eta[3] + slclss_1.ep[1] - 1.;
	    if (z__ < 0.) {
		d__1 = 1. / slreal_1.u;
		x[jdu + slint_1.nxinit] = d_sign(&d__1, &slclss_1.eta[3]);
		efin[6] = FALSE_;
	    } else {
		if (z__ > 0.) {
		    x[jdu + slint_1.nxinit] = 0.;
		} else {
		    d__1 = slclss_1.eta[2] * slclss_1.eta[3];
		    x[jdu + slint_1.nxinit] = slclss_1.d__[5] * slclss_1.cp[1]
			     * slclss_1.eta[2] * slclss_1.eta[3] / pow_dd(&
			    slclss_1.d__[4], &d__1);
		}
	    }
	    if (*iprint >= 5) {
		s_wsfe(&io___499);
		do_fio(&c__1, (char *)&x[slint_1.nxinit], (ftnlen)sizeof(
			doublereal));
		do_fio(&c__1, (char *)&x[ju + slint_1.nxinit], (ftnlen)sizeof(
			doublereal));
		do_fio(&c__1, (char *)&x[jdu + slint_1.nxinit], (ftnlen)
			sizeof(doublereal));
		e_wsfe();
	    }
	    h__ = x[slint_1.nxinit] - x[slint_1.nxinit - 1];
	    d__1 = slclss_1.ep[1] - 1.;
	    pn = slclss_1.cp[1] * pow_dd(&h__, &d__1);
	    d__1 = h__ / slclss_1.d__[4];
	    t = pow_dd(&d__1, &slclss_1.eta[2]);
	    chi = slclss_1.d__[5] * pow_dd(&t, &slclss_1.eta[3]);
	    w = chi * w;
	    d__1 = 1. - slclss_1.emu[1] * 2.;
	    pdw = pn * slclss_1.eta[2] * (chi * pdw * pow_dd(&t, &d__1) - 
		    slclss_1.eta[3] * w);
	}
	if (mode == 4) {
	    x[ju + slint_1.nxinit] = 0.;
	    x[jdu + slint_1.nxinit] = 1.;
	    if (*iprint >= 5) {
		s_wsfe(&io___500);
		do_fio(&c__1, (char *)&x[slint_1.nxinit], (ftnlen)sizeof(
			doublereal));
		do_fio(&c__1, (char *)&x[ju + slint_1.nxinit], (ftnlen)sizeof(
			doublereal));
		do_fio(&c__1, (char *)&x[jdu + slint_1.nxinit], (ftnlen)
			sizeof(doublereal));
		e_wsfe();
	    }
	}
	mode = 0;
	if (jlast != 0 && *iprint >= 5) {
	    s_wsfe(&io___501);
	    do_fio(&c__1, (char *)&xright, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&w, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&pdw, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&rsum, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/* L60: */
    }
    if (abs(w) >= abs(pdw)) {
	ratio = v / w;
	if (*iprint >= 5) {
	    s_wsfe(&io___503);
	    d__1 = ratio * pdw - pdv;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&ratio, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
    } else {
	ratio = pdv / pdw;
	if (v * w * ratio < 0.) {
	    ratio = -ratio;
	}
	if (*iprint >= 5) {
	    s_wsfe(&io___504);
	    d__1 = ratio * w - v;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&ratio, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
    }

/*     Calculate weighted 2-norm and scale approximate eigenfunction. */

    fscale = exp(-fsum);
    rscale = exp(-rsum);
/* Computing 2nd power */
    d__1 = fscale;
/* Computing 2nd power */
    d__2 = ratio * rscale;
    *efnorm = sqrt(fnorm * (d__1 * d__1) + rnorm * (d__2 * d__2));
    scale = log(*efnorm);
    if (*iprint >= 5) {
	s_wsfe(&io___505);
	do_fio(&c__1, (char *)&(*efnorm), (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    i__1 = slint_1.nxinit;
    for (i__ = 1; i__ <= i__1; ++i__) {
	tau = x[js + i__] - scale;
	if (tau <= -slreal_1.under) {
	    x[ju + i__] = 0.;
	    x[jdu + i__] = 0.;
	} else {
	    prod = exp(tau);
	    if (i__ >= middle[0]) {
		prod *= ratio;
	    }
	    x[ju + i__] *= prod;
	    x[jdu + i__] *= prod;
	}
/* L70: */
    }
    if (*iprint >= 4) {
	s_wsfe(&io___507);
	e_wsfe();
	i__1 = slint_1.nxinit;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    s_wsfe(&io___508);
	    do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&x[ju + i__], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&x[jdu + i__], (ftnlen)sizeof(doublereal));
	    e_wsfe();
/* L85: */
	}
    }
    return 0;
} /* getef_ */

/* ---------------------------------------------------------------------- */
/* Subroutine */ int getrn_(doublereal *ev, doublereal *alpha, logical *cspec,
	 doublereal *denom, doublereal *rsubn, doublereal *x)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);
    double exp(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal u, z__, du, om, pn, rn, phi, hom, pdu, tau, psi, dphi, 
	    dpsi;
    static integer klvl;
    extern /* Subroutine */ int step_(doublereal *, doublereal *, doublereal *
	    , doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    ;
    static doublereal unew, halfh, scale, hsave, tauhh, fnorm, xleft, usave, 
	    dusave;


/*     Compute the RsubN value from the weighted eigenfunction 2-norm */
/*     when standard shooting is stable and an accurate eigenvalue is */
/*     available (from the asymptotic formulas). */


    /* Parameter adjustments */
    --x;
    --cspec;

    /* Function Body */
    u = slreal_2.a2 - slreal_2.a2p * *ev;
    pdu = slreal_2.a1 - slreal_2.a1p * *ev;
    fnorm = 0.;
    klvl = pow_ii(&c__2, &slint_1.level);

/*     Shoot from x=A to x=B. */

    i__1 = slint_1.nxinit;
    for (i__ = 2; i__ <= i__1; ++i__) {
	xleft = x[i__ - 1];
	h__ = (x[i__] - xleft) / klvl;
	halfh = h__ * .5;
	i__2 = klvl;
	for (j = 1; j <= i__2; ++j) {
	    z__ = xleft + halfh;
	    xleft += h__;
	    step_(&z__, &h__, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi, &
		    scale, &c__0);
	    scale = exp(scale);
	    phi = psi * scale;
	    dphi = dpsi * scale;
	    du = pdu / pn;
	    if (abs(tau) * h__ * h__ >= 1e-4) {
		fnorm += rn * (phi * dphi * (u * u - du * du / tau) / 2. + u *
			 phi * du * phi + h__ * (u * u + du * du / tau) / 2.);
	    } else {
		tauhh = tau * h__ * h__;
/* Computing 2nd power */
		d__1 = h__ * du;
		fnorm += rn * h__ * (u * u * (tauhh * (tauhh / 5. - 1.) / 3. 
			+ 1.) + h__ * u * du * (tauhh * (tauhh * 2. / 15. - 
			1.) / 3. + 1.) + d__1 * d__1 * (tauhh * (tauhh * 2. / 
			21. - 1.) / 5. + 1.) / 3.);
	    }
	    if (i__ == slint_1.nxinit && j == klvl && cspec[1]) {
		hsave = h__;
		usave = abs(u);
		dusave = abs(pdu);
	    }
	    unew = dphi * u + phi * du;
	    pdu = -pn * tau * phi * u + dphi * pdu;
	    u = unew;
	    if (i__ == 2 && j == 1 && cspec[2]) {
		hsave = h__;
		usave = abs(u);
		dusave = abs(pdu);
	    }
/* L10: */
	}
/* L20: */
    }
    if (cspec[2]) {
	if (sllog_1.reg[0] || slclss_1.pnu[0] == 0. || slclss_1.pnu[0] == 1. 
		- slclss_1.ep[0]) {
	    phi = *denom;
	} else {
	    if (usave >= dusave) {
		phi = usave / pow_dd(&hsave, slclss_1.pnu);
	    } else {
		d__1 = slclss_1.ep[0] + slclss_1.pnu[0] - 1.;
		phi = dusave / (slclss_1.cp[0] * abs(slclss_1.pnu[0]) * 
			pow_dd(&hsave, &d__1));
	    }
	}
    } else {
	if (sllog_1.reg[1] || slclss_1.pnu[1] == 0. || slclss_1.pnu[1] == 1. 
		- slclss_1.ep[1]) {
	    phi = *denom;
	} else {
	    if (usave >= dusave) {
		phi = usave / pow_dd(&hsave, &slclss_1.pnu[1]);
	    } else {
		d__1 = slclss_1.ep[1] + slclss_1.pnu[1] - 1.;
		phi = dusave / (slclss_1.cp[1] * abs(slclss_1.pnu[1]) * 
			pow_dd(&hsave, &d__1));
	    }
	}
    }
/* Computing 2nd power */
    d__1 = phi;
    *rsubn = 1. / (*alpha + fnorm / (d__1 * d__1));
    return 0;
} /* getrn_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int mesh_(logical *job, integer *nev, doublereal *x, 
	doublereal *g, doublereal *h__, doublereal *qlnf, doublereal *z__, 
	doublereal *tol, doublereal *hmin)
{
    /* Initialized data */

    static doublereal eps = 1e-4;

    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2;
    doublereal d__1, d__2, d__3, d__4, d__5;

    /* Builtin functions */
    double d_lg10(doublereal *), sqrt(doublereal), exp(doublereal), pow_di(
	    doublereal *, integer *);
    integer pow_ii(integer *, integer *);
    double pow_dd(doublereal *, doublereal *), pow_ri(real *, integer *);

    /* Local variables */
    static integer i__, j, k, n;
    static doublereal y, p1, p2, p3, q1, q2, q3, r1, r2, r3, y1, y2, y3, ev, 
	    dx;
    static integer its, nadd;
    static doublereal enda, endb;
    static logical done;
    static doublereal qmin, qmax;
    static integer jtol;
    static doublereal gamma;
    //extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal 	    *, doublereal *);
    static doublereal eqmin, eqmax, weight;
    static integer maxits;


/*     If JOB = True then calculate the initial mesh; redistribute so */
/*     that H(*) is approximately equidistributed.  If JOB = False */
/*     then use the mesh input by the user. */


    /* Parameter adjustments */
    --z__;
    --qlnf;
    --h__;
    --g;
    --x;

    /* Function Body */

    if (! (*job)) {
	*hmin = slreal_1.b - slreal_1.a;
	i__1 = slint_1.nxinit;
	for (i__ = 2; i__ <= i__1; ++i__) {
/* Computing MIN */
	    d__1 = *hmin, d__2 = x[i__] - x[i__ - 1];
	    *hmin = min(d__1,d__2);
/* L5: */
	}
	return 0;
    }
    n = slint_1.nxinit - 1;
    ev = (doublereal) (*nev);

/*     Find an appropriate initial mesh. */

    if (sllog_1.afin) {
	x[1] = slreal_1.a;
	if (sllog_1.bfin) {
	    x[slint_1.nxinit] = slreal_1.b;
	    dx = (slreal_1.b - slreal_1.a) / n;
	    i__1 = n;
	    for (i__ = 2; i__ <= i__1; ++i__) {
		x[i__] = x[1] + (i__ - 1) * dx;
/* L10: */
	    }
	} else {
	    if (*nev < 0) {
		endb = x[slint_1.nxinit];
	    } else {
		x[slint_1.nxinit] = ((*nev << 2) + 1) * endb;
	    }
	    y1 = x[1] / (abs(x[1]) + 1.);
	    dx = (x[slint_1.nxinit] / (x[slint_1.nxinit] + 1.) - y1) / n;
	    i__1 = n;
	    for (i__ = 2; i__ <= i__1; ++i__) {
		y = y1 + (i__ - 1) * dx;
		x[i__] = y / (1. - abs(y));
/* L11: */
	    }
	}
    } else {
	if (*nev < 0) {
	    enda = x[1];
	} else {
	    x[1] = ((*nev << 2) + 1) * enda;
	}
	if (sllog_1.bfin) {
	    x[slint_1.nxinit] = slreal_1.b;
	    y1 = x[slint_1.nxinit] / ((d__1 = x[slint_1.nxinit], abs(d__1)) + 
		    1.);
	    dx = (y1 - x[1] / (1. - x[1])) / n;
	    i__1 = n;
	    for (i__ = 2; i__ <= i__1; ++i__) {
		y = y1 - (i__ - 1) * dx;
		x[slint_1.nxinit + 1 - i__] = y / (1. - abs(y));
/* L12: */
	    }
	} else {
	    y1 = x[1] / (1. - x[1]);
	    if (*nev < 0) {
		endb = x[slint_1.nxinit];
	    } else {
		x[slint_1.nxinit] = ((*nev << 2) + 1) * endb;
	    }
	    y2 = x[slint_1.nxinit] / (x[slint_1.nxinit] + 1.);
	    dx = (y2 - y1) / n;
	    i__1 = n;
	    for (i__ = 2; i__ <= i__1; ++i__) {
		y = y1 + (i__ - 1) * dx;
		x[i__] = y / (1. - abs(y));
/* L13: */
	    }
	    if ((d__1 = x[(slint_1.nxinit + 1) / 2], abs(d__1)) < *tol) {
		x[(slint_1.nxinit + 1) / 2] = 0.;
	    }
	}
    }
    jtol = (integer) (-d_lg10(tol) + .5);
    if (sllog_1.reg[0] && sllog_1.reg[1]) {
	maxits = 6;
    } else {
	maxits = 3;
    }

/*     Calculate H(*) and G(*). */

    its = 1;
    if (! sllog_1.afin) {
	if (x[2] >= 0.) {
/* Computing MAX */
	    d__1 = x[1] * .5;
	    x[2] = max(d__1,-1.);
	}
    }
    if (! sllog_1.bfin) {
	if (x[n] <= 0.) {
/* Computing MIN */
	    d__1 = x[slint_1.nxinit] * .5;
	    x[n] = min(d__1,1.);
	}
    }
    qmin = (float)1e31;
    qmax = -qmin;
L20:
    gamma = 0.;

/*    Equidistribute { [Qmax - Q]^2 * max[abs(p') , abs(q') , abs(r')] }. */

    eqmax = 0.;
    eqmin = (float)1e31;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
	dx = x[j + 1] - x[j];
	y2 = x[j] + dx * .5;
	coeff_(&y2, &p2, &q2, &r2);
	if (p2 == 0. || r2 == 0. || p2 * r2 < 0.) {
	    slint_1.flag__ = -15;
	    return 0;
	}
/* Computing MAX */
	d__1 = y2 - eps, d__2 = x[1] + slreal_1.u * 2. * abs(x[1]);
	y1 = max(d__1,d__2);
	coeff_(&y1, &p1, &q1, &r1);
/* Computing MIN */
	d__2 = y2 + eps, d__3 = x[slint_1.nxinit] - slreal_1.u * 2. * (d__1 = 
		x[slint_1.nxinit], abs(d__1));
	y3 = min(d__2,d__3);
	coeff_(&y3, &p3, &q3, &r3);
	if (sllog_1.lnf) {
	    h__[j] = (d__1 = q3 - q1, abs(d__1)) / (y3 - y1);
	    qlnf[j] = q2 / r2;
	} else {
/* Computing MAX */
	    d__4 = (d__1 = p3 - p1, abs(d__1)), d__5 = (d__2 = q3 - q1, abs(
		    d__2)), d__4 = max(d__4,d__5), d__5 = (d__3 = r3 - r1, 
		    abs(d__3));
	    h__[j] = max(d__4,d__5) / (y3 - y1);
	    y1 = sqrt(sqrt(r1 * p1));
	    y2 = sqrt(sqrt(r2 * p2));
	    y3 = sqrt(sqrt(r3 * p3));
	    y = sqrt(p2 / r2);
/* Computing 2nd power */
	    d__1 = eps;
	    qlnf[j] = q2 / r2 + y * ((y3 - y1) * (sqrt(p3 / r3) - sqrt(p1 / 
		    r1)) / 4. + (y3 - y2 * 2. + y1) * y) / (y2 * (d__1 * d__1)
		    );
	    if ((d__1 = qlnf[j], abs(d__1)) <= eps) {
		qlnf[j] = 0.;
	    }
	}
/* Computing MAX */
	d__1 = qmax, d__2 = qlnf[j];
	qmax = max(d__1,d__2);
/* Computing MIN */
	d__1 = qmin, d__2 = qlnf[j];
	qmin = min(d__1,d__2);
/* L25: */
    }
/* Computing MAX */
    d__1 = qmax - qmin;
    y = max(d__1,1.);
    ev = max(0.,qmin) + (float)100.;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
	dx = x[j + 1] - x[j];
	if (qlnf[j] <= ev) {
/* Computing 2nd power */
	    d__1 = (qmax - qlnf[j]) / y;
	    weight = d__1 * d__1 * (float)3. + 1.;
/* Computing MAX */
	    d__1 = h__[j] * weight;
	    h__[j] = max(d__1,slreal_1.u);
	} else {
	    y2 = dx * 2. * sqrt(qlnf[j] - ev);
	    if (y2 <= slreal_1.under) {
		weight = exp(-y2);
/* Computing MAX */
		d__1 = weight * h__[j];
		h__[j] = max(d__1,slreal_1.u);
	    } else {
		h__[j] = slreal_1.u;
	    }
	}
	if (! sllog_1.afin && x[j + 1] < 0.) {
/* Computing MAX */
	    d__1 = h__[j] * exp(x[j + 1]);
	    h__[j] = max(d__1,slreal_1.u);
	    if (j == 1 && h__[1] == slreal_1.u) {
		x[1] = (x[1] + x[2]) * .5;
	    }
	}
	if (! sllog_1.bfin && x[j] > 0.) {
/* Computing MAX */
	    d__1 = h__[j] * exp(-x[j]);
	    h__[j] = max(d__1,slreal_1.u);
	    if (j == n && h__[n] == slreal_1.u) {
		x[slint_1.nxinit] = (x[n] + x[slint_1.nxinit]) * .5;
	    }
	}
/* Computing MIN */
	d__1 = eqmin, d__2 = h__[j] * dx;
	eqmin = min(d__1,d__2);
/* Computing MAX */
	d__1 = eqmax, d__2 = h__[j] * dx;
	eqmax = max(d__1,d__2);
/* L30: */
    }
    slint_1.ncoeff += n * 3;
/* Computing MAX */
    d__1 = eqmax * .1, d__2 = slreal_1.u / .1;
    if (eqmax - eqmin <= max(d__1,d__2)) {
	goto L75;
    }

/*     Use a roughly locally quasi-uniform mesh. */

    gamma = 0.;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	gamma += h__[i__];
/* L35: */
    }
    gamma = 1. / gamma;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	y = 0.;
	i__2 = n;
	for (j = 1; j <= i__2; ++j) {
/* Computing MAX */
	    d__2 = y, d__3 = h__[j] / (gamma * (d__1 = x[i__] + x[i__ + 1] - 
		    x[j] - x[j + 1], abs(d__1)) * h__[j] + 1.);
	    y = max(d__2,d__3);
/* L40: */
	}
	z__[i__] = y;
/* L45: */
    }
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	h__[i__] = z__[i__];
/* L50: */
    }
    g[1] = 0.;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
	g[j + 1] = g[j] + h__[j] * (x[j + 1] - x[j]);
/* L55: */
    }
    gamma = g[n + 1] / n;

/*     Redistribution algorithm: */

    y = gamma;
    i__ = 1;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
L60:
	if (y <= g[j + 1]) {
	    ++i__;
	    z__[i__] = x[j] + (y - g[j]) / h__[j];
	    y += gamma;
	    goto L60;
	}
/* L65: */
    }
    z__[1] = x[1];
    z__[slint_1.nxinit] = x[slint_1.nxinit];
    done = TRUE_;
    i__1 = n;
    for (j = 2; j <= i__1; ++j) {
	if ((d__1 = z__[j] - x[j], abs(d__1)) > (z__[j + 1] - z__[j - 1]) * 
		.1) {
	    done = FALSE_;
	}
	x[j] = z__[j];
/* L70: */
    }
    if (! done) {
	if (its < maxits) {
	    ++its;
	    goto L20;
	}
    }
L75:
    if (! sllog_1.afin) {
	if (x[2] >= 0.) {
	    x[2] = -1.;
	}
    }
    if (! sllog_1.bfin) {
	if (x[n] <= 0.) {
	    x[n] = 1.;
	}
    }
    for (k = 1; k <= 2; ++k) {
	nadd = 0;
	if (slclss_1.kclass[k - 1] > 0) {

/*           Add Nadd extra points near endpoint K. */

	    if (slclss_1.kclass[k - 1] == 1) {
/* Computing MIN */
/* Computing MAX */
		i__2 = (jtol + 2) / 3;
		i__1 = max(i__2,1);
		nadd = min(i__1,4);
		y = .1;
	    }
	    if (slclss_1.kclass[k - 1] == 2) {
/* Computing MIN */
		i__1 = max(jtol,3);
		nadd = min(i__1,4);
/* Computing MIN */
/* Computing MAX */
		i__1 = jtol / 3;
		d__2 = pow_di(&c_b453, &i__1);
		d__1 = max(d__2,.001);
		y = min(d__1,.01);
	    }
	    if (slclss_1.kclass[k - 1] == 3) {
/* Computing MIN */
/* Computing MAX */
		i__2 = jtol / 2;
		i__1 = max(i__2,2);
		nadd = min(i__1,4);
	    }
	    if (slclss_1.kclass[k - 1] == 4) {
/* Computing MIN */
/* Computing MAX */
		i__2 = (jtol + 5) / 3;
		i__1 = max(i__2,2);
		nadd = min(i__1,5);
		i__1 = nadd - 1;
		y = pow_di(&c_b453, &i__1);
	    }
	    if (slclss_1.kclass[k - 1] == 5) {
		d__1 = (doublereal) pow_ii(&c__2, &jtol);
		nadd = pow_dd(&d__1, &c_b455);
/* Computing MIN */
		i__1 = max(nadd,3);
		nadd = min(i__1,6);
/* Computing MIN */
/* Computing MAX */
		d__1 = (doublereal) pow_ri(&c_b458, &jtol);
		r__2 = pow_dd(&d__1, &c_b457);
		r__1 = dmax(r__2,(float).001);
		y = dmin(r__1,(float).1);
	    }
	    if (slclss_1.kclass[k - 1] == 6) {
/* Computing MIN */
		i__1 = max(jtol,2);
		nadd = min(i__1,8);
		y = (float).005;
	    }
	    if (slclss_1.kclass[k - 1] == 7 || slclss_1.kclass[k - 1] == 10) {
/* Computing MIN */
/* Computing MAX */
		i__2 = ((jtol << 1) + 6) / 3;
		i__1 = max(i__2,3);
		nadd = min(i__1,8);
/* Computing MIN */
/* Computing MAX */
		d__2 = sqrt(*tol * .1);
		d__1 = max(d__2,1e-6);
		y = min(d__1,.01);
	    }
	    if (slclss_1.kclass[k - 1] == 8) {
		d__1 = (doublereal) pow_ii(&c__2, &jtol);
		nadd = pow_dd(&d__1, &c_b455);
/* Computing MIN */
		i__1 = max(nadd,2);
		nadd = min(i__1,6);
/* Computing MIN */
/* Computing MAX */
		d__1 = (doublereal) pow_ri(&c_b458, &jtol);
		r__2 = pow_dd(&d__1, &c_b457);
		r__1 = dmax(r__2,(float).001);
		y = dmin(r__1,(float).05);
	    }
	    if (slclss_1.kclass[k - 1] == 9) {
		if (sllog_1.lflag[4]) {
/* Computing MIN */
/* Computing MAX */
		    i__2 = (jtol + 4) / 3;
		    i__1 = max(i__2,2);
		    nadd = min(i__1,5);
		    i__1 = nadd - 1;
		    y = pow_di(&c_b453, &i__1);
		} else {
/* Computing MIN */
/* Computing MAX */
		    i__2 = jtol * (jtol - 3) / 40 + 2;
		    i__1 = max(i__2,2);
		    nadd = min(i__1,4);
		    y = (float).25;
		}
	    }
	    if (k == 1) {
		for (i__ = slint_1.nxinit; i__ >= 2; --i__) {
		    x[i__ + nadd] = x[i__];
/* L80: */
		}
		slint_1.nxinit += nadd;
		if (sllog_1.afin) {
		    dx = x[2] - slreal_1.a;
		}
		i__1 = nadd;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    if (sllog_1.afin) {
			d__1 = (nadd - i__ + 1.) / nadd;
			x[i__ + 1] = slreal_1.a + dx * pow_dd(&y, &d__1);
		    } else {
			if (slclss_1.kclass[0] != 1) {
			    x[nadd + 2 - i__] = x[nadd + 3 - i__] - (x[nadd + 
				    4 - i__] - x[nadd + 3 - i__]) * (float)
				    2.4;
			} else {
			    x[nadd + 2 - i__] = x[nadd + 3 - i__] - (x[nadd + 
				    4 - i__] - x[nadd + 3 - i__]);
			}
		    }
/* L85: */
		}
	    } else {
		if (sllog_1.bfin) {
		    dx = slreal_1.b - x[slint_1.nxinit - 1];
		}
		n = slint_1.nxinit - 1;
		slint_1.nxinit += nadd;
		x[slint_1.nxinit] = slreal_1.b;
		i__1 = nadd;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    if (sllog_1.bfin) {
			d__1 = (nadd - i__ + 1.) / nadd;
			x[slint_1.nxinit - i__] = slreal_1.b - dx * pow_dd(&y,
				 &d__1);
		    } else {
			if (slclss_1.kclass[1] != 1) {
			    x[n + i__] = x[n + i__ - 1] + (x[n + i__ - 1] - x[
				    n + i__ - 2]) * (float)2.4;
			} else {
			    x[n + i__] = x[n + i__ - 1] + (x[n + i__ - 1] - x[
				    n + i__ - 2]);
			}
		    }
/* L90: */
		}
	    }
	}
/* L95: */
    }
    if (! sllog_1.afin) {
	x[1] = -1. / slreal_1.u;
    }
    if (! sllog_1.bfin) {
	x[slint_1.nxinit] = 1. / slreal_1.u;
    }
    *hmin = x[slint_1.nxinit] - x[1];
    i__1 = slint_1.nxinit;
    for (i__ = 2; i__ <= i__1; ++i__) {
/* Computing MIN */
	d__1 = *hmin, d__2 = x[i__] - x[i__ - 1];
	*hmin = min(d__1,d__2);
/* L100: */
    }
    return 0;
} /* mesh_ */

/* ----------------------------------------------------------------------------- */
/* Subroutine */ int power_(doublereal *x, doublereal *f, integer *n, 
	doublereal *tol, integer *iprint, doublereal *ef, doublereal *cf, 
	logical *osc, logical *exact, doublereal *y, integer *iflag)
{
    /* Initialized data */

    static doublereal tolmin = 1e-6;

    /* Format strings */
    static char fmt_15[] = "(4d19.10)";

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double log(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(), s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), 
	    e_wsfe();
    double d_sign(doublereal *, doublereal *), sqrt(doublereal), pow_dd(
	    doublereal *, doublereal *);

    /* Local variables */
    static integer k, ny;
    static doublereal error;
    extern /* Subroutine */ int aitken_(doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *);
    static doublereal tolait;

    /* Fortran I/O blocks */
    static cilist io___573 = { 0, 21, 0, 0, 0 };
    static cilist io___574 = { 0, 21, 0, fmt_15, 0 };
    static cilist io___575 = { 0, 21, 0, 0, 0 };
    static cilist io___576 = { 0, 21, 0, fmt_15, 0 };



/*     Find the power function which "dominates" the tabled */
/*     coefficient function.  The output is Cf and Ef such that */
/*           f(x)  is asymptotic to  Cf*x^Ef . */
/*     The vectors X(*) and F(*) hold the N input points: */
/*           F(I) = f(X(I)) I = 1,...,N. */
/*     Set IFLAG = 0 for normal return; 1 for uncertainty in Ef; */
/*     2 if uncertain about Cf (oscillatory). */

    /* Parameter adjustments */
    --y;
    --f;
    --x;

    /* Function Body */

/*     Estimate the exponent. */

    *osc = FALSE_;
    ny = *n - 1;
    error = (float)1e30;
    tolait = min(tolmin,*tol);
    i__1 = ny;
    for (k = 1; k <= i__1; ++k) {
	if (f[k] != 0. && f[k + 1] != 0.) {
	    y[k] = log((d__1 = f[k + 1] / f[k], abs(d__1))) / log((d__2 = x[k 
		    + 1] / x[k], abs(d__2)));
	} else {
	    y[k] = 0.;
	}
/* L10: */
    }
    *ef = y[ny];
    if (*iprint >= 5) {
	s_wsle(&io___573);
	do_lio(&c__9, &c__1, " From POWER; E_k and c_k sequences:", (ftnlen)
		35);
	e_wsle();
	s_wsfe(&io___574);
	i__1 = ny;
	for (k = 1; k <= i__1; ++k) {
	    do_fio(&c__1, (char *)&y[k], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
	s_wsle(&io___575);
	e_wsle();
    }
    aitken_(ef, &tolait, &ny, &y[1], &error);
    k = (integer) (*ef + d_sign(&c_b123, ef));
    if ((d__1 = k - *ef, abs(d__1)) <= sqrt(*tol)) {
	*ef = (doublereal) k;
    }
/* Computing MAX */
    d__1 = 1., d__2 = abs(*ef);
    if (abs(error) > *tol * max(d__1,d__2)) {

/*        There is uncertainty in the exponent. */

	*iflag = 1;
    }
    if (abs(*ef) <= *tol) {
	*ef = 0.;
    }

/*     Estimate the coefficient. */

    i__1 = *n - 1;
    for (k = 1; k <= i__1; ++k) {
	d__2 = (d__1 = x[k], abs(d__1));
	y[k] = f[k] / pow_dd(&d__2, ef);
/* L20: */
    }
    *cf = y[*n - 1];
    if (*iprint >= 5) {
	s_wsfe(&io___576);
	i__1 = *n - 1;
	for (k = 1; k <= i__1; ++k) {
	    do_fio(&c__1, (char *)&y[k], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    i__1 = *n - 1;
    aitken_(cf, &tolait, &i__1, &y[1], &error);
    if (*ef > (float)20. && abs(*cf) <= *tol) {

/*        Coefficient probably has exponential behavior. */

	*cf = d_sign(&c_b8, &y[*n - 1]);
    } else {
/* Computing MAX */
	d__3 = 1., d__4 = abs(*cf);
	if (abs(error) > *tol * max(d__3,d__4) || (d__2 = f[*n] - *cf * 
		pow_dd(&x[*n], ef), abs(d__2)) > *tol * (float)20. * (d__1 = 
		f[*n], abs(d__1)) && *ef != 0.) {

/*           There is uncertainty in the coefficient; call such */
/*           cases oscillatory. */

	    *iflag = 2;
	    *osc = TRUE_;
	}
    }
    if (abs(*cf) > 1e7) {
	*exact = FALSE_;
	return 0;
    }
    k = (integer) (*cf + .5);
    if ((d__1 = k - *cf, abs(d__1)) <= sqrt(*tol) && k != 0) {
	*cf = (doublereal) k;
    }
    *exact = TRUE_;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if ((d__2 = f[k] - *cf * pow_dd(&x[k], ef), abs(d__2)) > *tol * (d__1 
		= f[k], abs(d__1))) {
	    *exact = FALSE_;
	}
/* L30: */
    }
    return 0;
} /* power_ */

/* ---------------------------------------------------------------------- */
/* Subroutine */ int pqrint_(doublereal *x, doublereal *sqrtrp, doublereal *
	qlnf)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4, d__5, d__6;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal z__, fl, fm, fr, px, qx, rx, eps;
    //extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal 	    *, doublereal *);
    static doublereal xdotl, xdotr;


/*     Evaluate the integrands needed for the asymptotic formulas. */
/*       (1) The Liouville normal form potential Qlnf: */
/*             Qlnf(t) =  q/r + f"(t)/f   with   f = (pr)**.25  . */
/*       (2) The term in the change of independent variable: */
/*                        sqrt(r/p) . */

    if (sllog_1.lnf) {
	coeff_(x, &px, &qx, &rx);
	if (px == 0. || rx == 0. || px * rx < 0.) {
	    slint_1.flag__ = -15;
	    return 0;
	}
	++slint_1.ncoeff;
	*qlnf = qx / rx;
	*sqrtrp = sqrt(rx / px);
    } else {
/* Computing MIN */
/* Computing MIN */
	d__5 = (d__1 = slreal_1.b - *x, abs(d__1)), d__6 = (d__2 = *x - 
		slreal_1.a, abs(d__2));
	d__3 = 1e-4, d__4 = min(d__5,d__6) / 2.;
	eps = min(d__3,d__4);
	z__ = *x - eps;
	coeff_(&z__, &px, &qx, &rx);
	if (px == 0. || rx == 0. || px * rx < 0.) {
	    slint_1.flag__ = -15;
	    return 0;
	}
	xdotl = sqrt(px / rx);
	fl = sqrt(sqrt(px * rx));
	z__ = *x + eps;
	coeff_(&z__, &px, &qx, &rx);
	xdotr = sqrt(px / rx);
	fr = sqrt(sqrt(px * rx));
	coeff_(x, &px, &qx, &rx);
	*sqrtrp = sqrt(rx / px);
	fm = sqrt(sqrt(px * rx));
	slint_1.ncoeff += 3;
	*qlnf = qx / rx + ((fr - fm) * (xdotr - xdotl) / 4. + (fr - fm * 2. + 
		fl) / *sqrtrp) / (eps * eps * fm * *sqrtrp);
	if (abs(*qlnf) <= eps) {
	    *qlnf = 0.;
	}
    }
    return 0;
} /* pqrint_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int regulr_(logical *job, logical *jobmsh, doublereal *tol, 
	integer *nev, doublereal *ev, integer *iprint, integer *nextrp, 
	doublereal *xef, doublereal *ef, doublereal *pdef, doublereal *hmin, 
	doublereal *store)
{
    /* Format strings */
    static char fmt_15[] = "(\002 Level 0 mesh:\002,/,(5g15.6))";
    static char fmt_25[] = "(\002      In  bracket:\002,2d24.15)";
    static char fmt_30[] = "(\002      Out bracket:\002,2d24.15)";
    static char fmt_40[] = "(\002 Level \002,i3,\002 ;    EvHat = \002,d24.1\
5)";

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(), 
	    pow_ii(integer *, integer *);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal z__;
    static integer ii, kk, ju, ku, jdu, kdu;
    static doublereal tol1, tol2, tol3, tol4, tol5, tol6;
    static logical efin[4]	/* was [2][2] */, done;
    extern /* Subroutine */ int mesh_(logical *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal qlnf, flow, umax, qint, beta1, beta2, fhigh, delta;
    extern /* Subroutine */ int getef_(doublereal *, doublereal *, integer *, 
	    doublereal *, logical *);
    static doublereal evhat, error, evext[20], evlow, rpint, alpha1, alpha2;
    extern /* Subroutine */ int zzero_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, integer *, 
	    doublereal *);
    static logical efdone;
    static doublereal evhigh;
    extern /* Subroutine */ int brcket_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    static doublereal abserr;
    static logical evdone;
    static doublereal efnorm, relerr, pdumax;
    static logical exfull;
    extern doublereal asymev_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal tolpdu;
    extern /* Subroutine */ int extrap_(doublereal *, doublereal *, integer *,
	     integer *, logical *, logical *, integer *, doublereal *, 
	    integer *, doublereal *, logical *), pqrint_(doublereal *, 
	    doublereal *, doublereal *);
    static doublereal tolext, tolsum, sqrtrp;

    /* Fortran I/O blocks */
    static cilist io___604 = { 0, 6, 0, fmt_15, 0 };
    static cilist io___606 = { 0, 21, 0, fmt_15, 0 };
    static cilist io___619 = { 0, 21, 0, fmt_25, 0 };
    static cilist io___622 = { 0, 21, 0, fmt_30, 0 };
    static cilist io___624 = { 0, 6, 0, fmt_40, 0 };
    static cilist io___625 = { 0, 21, 0, fmt_40, 0 };


/* ********************************************************************** */
/*                                                                     * */
/*     REGULR calculates Sturm-Liouville eigenvalue and (optionally)   * */
/*     eigenfunction estimates for the problem described initially.    * */
/*                                                                     * */
/* ********************************************************************** */

/*    Input parameters: */
/*      JOB       = logical variable describing tasks to be carried out. */
/*                  JOB = .True. iff an eigenfunction is to be calculated. */
/*      JOBMSH    = logical variable, JOBMSH = .True. iff initial mesh */
/*                  is a function of the eigenvalue index. */
/*      CONS(*)   = real vector of 8 input constants: A1, A1', A2, A2', */
/*                  B1, B2, A, B. */
/*      TOL(*)    = real vector of 6 tolerances. */
/*                  TOL(1) is the absolute error tolerance for e-values, */
/*                  TOL(2) is the relative error tolerance for e-values, */
/*                  TOL(3) is the abs. error tolerance for e-functions, */
/*                  TOL(4) is the rel. error tolerance for e-functions, */
/*                  TOL(5) is the abs. error tolerance for e-function */
/*                         derivatives, */
/*                  TOL(6) is the rel. error tolerance for e-function */
/*                         derivatives. */
/*                  Eigenfunction tolerances need not be set if JOB is */
/*                  False.  All absolute error tolerances must be */
/*                  positive; all relative must be at least 100 times */
/*                  the unit roundoff. */
/*      NEV       = integer index for the eigenvalue sought; NEV .GE. 0 . */
/*      EV        = real initial guess for eigenvalue NEV; accuracy is */
/*                  not at all critical, but if a good estimate is */
/*                  available some time may be saved. */
/*      IPRINT    = integer controlling amount of internal printing done. */

/*    Output parameters: */
/*      EV        = real computed approximation to NEVth eigenvalue. */
/*      XEF(*)    = real vector of points for eigenfunction output. */
/*      EF(*)     = real vector of eigenfunction values: EF(i) is the */
/*                  estimate of u(XEF(i)).  If JOB is False then this */
/*                  vector is not referenced. */
/*      PDEF(*)   = real vector of eigenfunction derivative values: */
/*                  PDEF(i) is the estimate of (pu')(XEF(i)).  If JOB is */
/*                  False then this vector is not referenced. */

/*    Auxiliary storage: */
/*      STORE(*) = real vector of auxiliary storage, must be dimensioned */
/*                 at least max[100,26N]. (N the number of mesh points) */

/*     Storage allocation in auxiliary vector (currently Maxlvl = 10): */
/*         STORE(*) */
/*       1   ->      N           vector of mesh points X(*), */
/*     N+1   ->     2N           best current eigenfunction values, */
/*    2N+1   ->     3N           best current derivative values, */
/*    3N+1   ->     4N           scale factors in GETEF, */
/*    4N+1   ->  (6+2*Maxlvl)N   intermediate eigenfunction values. */
/* ----------------------------------------------------------------------- */
/*     Local variables: */


    /* Parameter adjustments */
    --store;
    --pdef;
    --ef;
    --xef;
    --tol;

    /* Function Body */
    alpha1 = 0.;
    alpha2 = 1.;
    beta1 = 0.;
    beta2 = 1.;
    if (*nev < 0) {
	slint_1.flag__ = -37;
	return 0;
    }
    evdone = FALSE_;
    tol1 = min(tol[1],.001);
    tol2 = min(tol[2],.001);
    if (! (sllog_1.reg[0] && sllog_1.reg[1])) {
	tol1 /= 3.;
	tol2 /= 3.;
    }
    if (*job) {
	tol3 = min(tol[3],.001);
	tol4 = min(tol[4],.001);
	tol5 = min(tol[5],.001);
	tol6 = min(tol[6],.001);
	abserr = tol1 / 5.;
	relerr = tol2 / 5.;
	exfull = TRUE_;
    } else {
	efdone = TRUE_;
	abserr = tol1 / 10.;
	relerr = tol2 / 10.;
	exfull = FALSE_;
    }
    tolsum = tol1 + tol2;
    if (*jobmsh) {
	if (*nev >= 0) {
	    ii = slint_1.nxinit + 16;
	    mesh_(&c_true, nev, &store[1], &store[ii], &store[(ii << 1) + 1], 
		    &store[ii * 3 + 1], &store[(ii << 2) + 1], &tolsum, hmin);
	}
	if (*iprint >= 1) {
	    s_wsfe(&io___604);
	    i__1 = slint_1.nxinit;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(doublereal))
			;
	    }
	    e_wsfe();
	    s_wsfe(&io___606);
	    i__1 = slint_1.nxinit;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&store[i__], (ftnlen)sizeof(doublereal))
			;
	    }
	    e_wsfe();
	}
    }

/*     Compute estimates for integrals in asymptotic formulas (accuracy */
/*     is not all that critical). */

    qint = 0.;
    rpint = 0.;
    i__1 = slint_1.nxinit;
    for (i__ = 2; i__ <= i__1; ++i__) {
	h__ = store[i__] - store[i__ - 1];
	z__ = store[i__ - 1] + h__ * .5;
	if (! sllog_1.afin && i__ == 2) {
	    h__ = store[3] - store[2];
	    z__ = store[2];
	}
	if (! sllog_1.bfin && i__ == slint_1.nxinit) {
	    h__ = store[slint_1.nxinit - 1] - store[slint_1.nxinit - 2];
	    z__ = store[slint_1.nxinit - 1];
	}
	pqrint_(&z__, &sqrtrp, &qlnf);
	if (slint_1.flag__ < 0) {
	    return 0;
	}
	qint += h__ * qlnf;
	rpint += h__ * sqrtrp;
/* L20: */
    }
    if (qint > 1. / slreal_1.u) {
	qint = 0.;
    }
    if (rpint > 1. / slreal_1.u) {
	rpint = 0.;
    }

/*     Loop over the levels. */

    i__1 = slint_1.maxlvl;
    for (slint_1.level = 0; slint_1.level <= i__1; ++slint_1.level) {
	if (*hmin / pow_ii(&c__2, &slint_1.level) <= slreal_1.u * 10.) {
	    slint_1.flag__ = -8;
	    goto L70;
	}

/*        Find a bracket for the Nevth eigenvalue. */

	if (slint_1.level == 0) {
	    *ev = asymev_(nev, &qint, &rpint, &alpha1, &alpha2, &beta1, &
		    beta2);
	    *ev = max(*ev,0.);
	    delta = .5;
	} else {
/* Computing MAX */
	    d__1 = tolsum * abs(evhat), d__2 = delta * .25;
	    delta = max(d__1,d__2);
	    if (slint_1.level > 1) {
		error = (evext[slint_1.level - 1] - evext[slint_1.level - 2]) 
			/ 3.;
		if (abs(error) <= (float)100.) {
		    *ev = evhat + error;
		} else {
		    delta = 1.;
		    *ev = evhat;
		}
	    } else {
		*ev = evhat;
	    }
	}
	evlow = *ev - delta;
	evhigh = *ev + delta;
	if (*iprint >= 4) {
	    s_wsfe(&io___619);
	    do_fio(&c__1, (char *)&evlow, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&evhigh, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	brcket_(nev, &evlow, &evhigh, &flow, &fhigh, &abserr, &relerr, &store[
		1]);
	if (*iprint >= 4) {
	    s_wsfe(&io___622);
	    do_fio(&c__1, (char *)&evlow, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&evhigh, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	delta = (evhigh - evlow) * .5;
	if (slint_1.flag__ < 0) {
	    return 0;
	}
/* Computing MAX */
	d__2 = abserr, d__3 = relerr * abs(evhigh);
	if ((d__1 = evhigh - evlow, abs(d__1)) > max(d__2,d__3)) {
	    zzero_(&evlow, &evhigh, &flow, &fhigh, &abserr, &relerr, &j, &
		    store[1]);
	    if (j != 0) {
		slint_1.flag__ = -7;
		return 0;
	    }
	}
	evhat = min(evlow,evhigh);
	if (*iprint >= 1) {
	    s_wsfe(&io___624);
	    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	    s_wsfe(&io___625);
	    do_fio(&c__1, (char *)&slint_1.level, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&evhat, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	*ev = evhat;
/* Computing MAX */
	d__1 = tol1, d__2 = abs(*ev) * tol2;
	tolext = max(d__1,d__2);
	i__2 = slint_1.level + 1;
	extrap_(ev, &tolext, &i__2, nextrp, &exfull, &c_true, &c__0, evext, 
		iprint, &error, &evdone);
	if (*job) {
	    getef_(&evhat, &efnorm, iprint, &store[1], efin);
	    if (slint_1.level == 0) {
		umax = 1.;
		pdumax = 1.;

/*              Set pointers to STORE(*). */

		ju = slint_1.nxinit;
		jdu = slint_1.nxinit << 1;
		ku = slint_1.nxinit << 2;
		kdu = (slint_1.maxlvl + 5) * slint_1.nxinit;
		kk = slint_1.maxlvl + 1;
	    }

/*           Extrapolate eigenfunction values. */

/* Computing MAX */
	    d__1 = tol3, d__2 = umax * tol4;
	    tolext = max(d__1,d__2);
/* Computing MAX */
	    d__1 = tol5, d__2 = pdumax * tol6;
	    tolpdu = max(d__1,d__2);
	    efdone = TRUE_;
	    if (sllog_1.afin) {
		umax = (d__1 = store[ju + 1], abs(d__1));
		pdumax = (d__1 = store[jdu + 1], abs(d__1));
	    } else {
		umax = 0.;
		pdumax = 0.;
	    }
	    if (efin[0]) {
		i__2 = slint_1.level + 1;
		extrap_(&store[ju + 1], &tolext, &i__2, nextrp, &exfull, &
			c_true, &c__0, &store[ku + 1], &c__0, &error, &done);
		efdone = efdone && done;
	    }
	    if (efin[1]) {
		i__2 = slint_1.level + 1;
		extrap_(&store[jdu + 1], &tolpdu, &i__2, nextrp, &exfull, &
			c_true, &c__0, &store[kdu + 1], &c__0, &error, &done);
		efdone = efdone && done;
	    }
	    i__2 = slint_1.nxinit - 1;
	    for (i__ = 2; i__ <= i__2; ++i__) {
		ii = kk * (i__ - 1) + 1;
		i__3 = slint_1.level + 1;
		extrap_(&store[ju + i__], &tolext, &i__3, nextrp, &exfull, &
			c_true, &c__0, &store[ku + ii], &c__0, &error, &done);
		efdone = efdone && done;
		i__3 = slint_1.level + 1;
		extrap_(&store[jdu + i__], &tolpdu, &i__3, nextrp, &exfull, &
			c_true, &c__0, &store[kdu + ii], &c__0, &error, &done)
			;
		efdone = efdone && done;
/* Computing MAX */
		d__2 = umax, d__3 = (d__1 = store[ju + i__], abs(d__1));
		umax = max(d__2,d__3);
/* Computing MAX */
		d__2 = pdumax, d__3 = (d__1 = store[jdu + i__], abs(d__1));
		pdumax = max(d__2,d__3);
/* L50: */
	    }
	    ii = kk * (slint_1.nxinit - 1) + 1;
	    if (efin[2]) {
		i__2 = slint_1.level + 1;
		extrap_(&store[ju + slint_1.nxinit], &tolext, &i__2, nextrp, &
			exfull, &c_true, &c__0, &store[ku + ii], &c__0, &
			error, &done);
		efdone = efdone && done;
	    }
	    if (efin[3]) {
		i__2 = slint_1.level + 1;
		extrap_(&store[jdu + slint_1.nxinit], &tolpdu, &i__2, nextrp, 
			&exfull, &c_true, &c__0, &store[kdu + ii], &c__0, &
			error, &done);
		efdone = efdone && done;
	    }
	    if (sllog_1.bfin) {
/* Computing MAX */
		d__2 = umax, d__3 = (d__1 = store[ju + slint_1.nxinit], abs(
			d__1));
		umax = max(d__2,d__3);
/* Computing MAX */
		d__2 = pdumax, d__3 = (d__1 = store[jdu + slint_1.nxinit], 
			abs(d__1));
		pdumax = max(d__2,d__3);
	    }
/* Computing MAX */
	    d__1 = abserr * .5, d__2 = slreal_1.u * 10.;
	    abserr = max(d__1,d__2);
/* Computing MAX */
	    d__1 = relerr * .5, d__2 = slreal_1.u * 10.;
	    relerr = max(d__1,d__2);
	}
	if (evdone && slint_1.level >= 2 && efdone) {
	    goto L70;
	}
/* L60: */
    }
    if (! evdone) {
	slint_1.flag__ = -1;
	return 0;
    }

/*     Unload eigenfunction values. */

L70:
    if (*job) {
	i__1 = slint_1.nxinit;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    xef[i__] = store[i__];
	    ef[i__] = store[ju + i__];
	    pdef[i__] = store[jdu + i__];
/* L80: */
	}
	if (slint_1.flag__ >= 0 && ! efdone) {
	    slint_1.flag__ = -2;
	}
    }
    return 0;
} /* regulr_ */

/* --------------------------------------------------------------------- */
/* Subroutine */ int shoot_(doublereal *ev, doublereal *x, integer *mu, 
	doublereal *fev)
{
    /* Initialized data */

    static integer imax = 1000000;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);
    double d_sign(doublereal *, doublereal *), pow_dd(doublereal *, 
	    doublereal *), atan(doublereal);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal t, v, z__;
    static integer k1, k2;
    static doublereal x2, dv, pn, rn, sa1, sa2, sb1, sb2, chi, sgn, pdv, tau, 
	    psi;
    static integer mode;
    static doublereal dpsi;
    static integer klvl;
    extern /* Subroutine */ int step_(doublereal *, doublereal *, doublereal *
	    , doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    ;
    static doublereal vnew, halfh, scale, omega, phase;
    static integer nsave;
    static doublereal xleft;
    static integer nzero;
    static doublereal homega;


    /* Parameter adjustments */
    --x;

    /* Function Body */

/*     Make one shot across (A,B) for the current mesh using the scaled */
/*     variable v(x). Count zeros if COUNTZ is True. */

/*     Shoot from x=A to x=B. */

    v = slreal_1.a2 - slreal_1.a2p * *ev;
    pdv = slreal_1.a1 - slreal_1.a1p * *ev;
    nzero = 0;
    nsave = slint_1.nsgnf;
    sa1 = slreal_1.a1;
    sa2 = slreal_1.a2;
    sb1 = slreal_1.b1;
    sb2 = slreal_1.b2;
    klvl = pow_ii(&c__2, &slint_1.level);
    mode = 0;

/*     Modify base sign count if necessary. */

    if ((slclss_1.kclass[0] >= 9 || slclss_1.kclass[1] >= 9) && *ev < 
	    slclss_1.cutoff) {
	if (slclss_1.kclass[0] >= 9) {
	    sa1 = slclss_1.d__[3];
	    sa2 = 1.;
	    v = sa2;
	    pdv = sa1;
	    mode = 1;
	}
	if (slclss_1.kclass[1] >= 9) {
	    sb1 = slclss_1.d__[7];
	    sb2 = 1.;
	}
	sgn = slreal_1.a2 * slreal_1.b2;
	if (sgn == 0.) {
	    sgn = sa1 * sb2 + sa2 * sb1;
	    if (sgn == 0.) {
		sgn = slreal_1.a1 * slreal_1.b1;
	    }
	}
	slint_1.nsgnf = (integer) d_sign(&c_b8, &sgn);
    }
    if (! sllog_1.afin) {
	mode = 3;
    }
/* Computing MAX */
    d__1 = abs(v), d__2 = abs(pdv);
    scale = max(d__1,d__2);
    v /= scale;
    pdv /= scale;
    i__1 = slint_1.nxinit;
    for (i__ = 2; i__ <= i__1; ++i__) {
	xleft = x[i__ - 1];
	h__ = x[i__] - xleft;
	if (mode == 1) {
	    d__1 = h__ / slclss_1.d__[0];
	    h__ = pow_dd(&d__1, slclss_1.eta);
	    xleft = 0.;
	}
	if (mode == 3) {
	    xleft = 0.;
	    h__ = -1. / x[2];
	}
	if (slclss_1.kclass[1] >= 9 && i__ == slint_1.nxinit && *ev < 
		slclss_1.cutoff) {

/*           Convert from u(x) to V(t) near x=b. */

	    mode = 2;
	    d__1 = h__ / slclss_1.d__[4];
	    t = pow_dd(&d__1, &slclss_1.eta[2]);
	    chi = slclss_1.d__[5] * pow_dd(&t, &slclss_1.eta[3]);
	    v /= chi;
	    d__1 = slclss_1.ep[1] - 1.;
	    pn = slclss_1.cp[1] * pow_dd(&h__, &d__1);
	    d__1 = slclss_1.emu[1] * 2. - 1.;
	    pdv = (pdv / (pn * chi * slclss_1.eta[2]) + slclss_1.eta[3] * v) *
		     pow_dd(&t, &d__1);
	    h__ = t;
	    xleft = -h__;
	}
	if (! sllog_1.bfin && i__ == slint_1.nxinit) {
	    mode = 4;
	    h__ = 1. / x[i__ - 1];
	    xleft = -h__;
	}
	h__ /= klvl;
	halfh = h__ * .5;
	i__2 = klvl;
	for (j = 1; j <= i__2; ++j) {
	    z__ = xleft + halfh;
	    step_(&z__, &h__, ev, &pn, &rn, &tau, &omega, &homega, &psi, &
		    dpsi, &scale, &mode);
	    if (slint_1.flag__ < 0) {
		*fev = 0.;
		return 0;
	    }
	    dv = pdv / pn;
	    vnew = dpsi * v + psi * dv;
	    xleft += h__;
	    if (sllog_1.countz) {

/*              Count zeros of v(x). */

		if (tau <= 0.) {
		    if (vnew * v < 0.) {
			++nzero;
		    }
		} else {
		    if (dv == 0.) {
			nzero += (integer) (homega / 3.141592653589793 + .5);
		    } else {
			phase = atan(v * omega / dv);
			k1 = (integer) (phase / 3.141592653589793);
			x2 = (phase + homega) / 3.141592653589793;
			if (x2 < (doublereal) imax) {
			    k2 = (integer) x2;
			    nzero = nzero + k2 - k1;
			} else {
			    nzero = imax;
			}
			if (phase * (phase + homega) < 0.) {
			    ++nzero;
			}
		    }
		    nzero = min(imax,nzero);
		}
	    }
	    pdv = -pn * tau * psi * v + dpsi * pdv;
	    v = vnew;
/* L10: */
	}
	if (mode == 1) {

/*           Convert from V(t) back to u(x) near x=a. */

	    d__1 = x[2] - slreal_1.a;
	    d__2 = slclss_1.ep[0] - 1.;
	    pn = slclss_1.cp[0] * pow_dd(&d__1, &d__2);
	    d__1 = (x[2] - slreal_1.a) / slclss_1.d__[0];
	    t = pow_dd(&d__1, slclss_1.eta);
	    chi = slclss_1.d__[1] * pow_dd(&t, &slclss_1.eta[1]);
	    d__1 = 1. - slclss_1.emu[0] * 2.;
	    pdv = pn * slclss_1.eta[0] * chi * (slclss_1.eta[1] * v + pdv * 
		    pow_dd(&t, &d__1));
	    v = chi * v;
	}
	mode = 0;
/* L20: */
    }
    *fev = sb1 * v + sb2 * pdv;
    if (sllog_1.countz) {

/*        Adjust zero count. */

	*mu = nzero;
	if (slreal_1.a2p != 0.) {
	    if (*ev >= sa2 / slreal_1.a2p) {
		*mu = nzero + 1;
	    }
	}
	if (sb2 != 0.) {
	    d__1 = slint_1.nsgnf * *fev;
	    sgn = d_sign(&c_b8, &d__1);
	    if (*mu % 2 == 1) {
		sgn = -sgn;
	    }
	    if (sgn < 0.) {
		++(*mu);
	    }
	}
    }
    slint_1.nsgnf = nsave;
    return 0;
} /* shoot_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int start_(logical *job, doublereal *cons, doublereal *tol, 
	integer *nev, integer *indxev, integer *n, doublereal *xef, integer *
	numt, doublereal *t, integer *nextrp, doublereal *x)
{
    /* Initialized data */

    static doublereal urn = 2e-16;
    static doublereal uflow = 650.;

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double d_lg10(doublereal *);

    /* Local variables */
    static integer i__, k;


/*     This routine tests the input data, initializes the labeled */
/*     common blocks, and generates the first mesh.  Check */
/*        eigenfunction tolerances iff JOB(1) is True , */
/*        XEF(*) iff JOB(2) is True, */
/*        NUMT, T(*) iff JOB(3) is True , */

/* *********************************************************************** */
/*   In the following DATA statement, initialize URN to an estimate for * */
/*   the unit roundoff and UFLOW to a value somewhat less than          * */
/*   -ln(underflow).  E.g.,                                             * */
/*      for IEEE double precision use URN = 2.D-16, UFLOW = 650;        * */
/*      for VAXen double precision use URN = 1.D-17, UFLOW = 85;        * */
/*      for Crays (single precision) use URN = 7.D-15, UFLOW = 5000.    * */
/*   Exact values are not at all critical.                              * */
/*   Here, we assume IEEE double precision:                             * */
/*                                                                      * */
    /* Parameter adjustments */
    --x;
    --t;
    --xef;
    --indxev;
    --tol;
    --cons;
    --job;

    /* Function Body */
/* *********************************************************************** */
    slreal_1.u = urn;
    slreal_1.under = uflow;

/*     Initialize. */

    slreal_1.a1 = cons[1];
    slreal_1.a1p = cons[2];
    slreal_1.a2 = cons[3];
    slreal_1.a2p = cons[4];
    slreal_1.a = cons[7];
    slreal_1.b1 = cons[5];
    slreal_1.b2 = cons[6];
    slreal_1.b = cons[8];
    slint_1.ncoeff = 0;
    slint_1.flag__ = 0;

/*     Test input. */

    if (sllog_1.afin && sllog_1.bfin && slreal_1.a >= slreal_1.b) {
	slint_1.flag__ = -34;
    }
    if (tol[1] <= 0.) {
	slint_1.flag__ = -35;
    }
    if (tol[2] < slreal_1.u * 100.) {
	slint_1.flag__ = -36;
    }
    if (job[1]) {
	if (tol[3] <= 0.) {
	    slint_1.flag__ = -35;
	}
	if (tol[4] < slreal_1.u * 100.) {
	    slint_1.flag__ = -36;
	}
	if (tol[5] <= 0.) {
	    slint_1.flag__ = -35;
	}
	if (tol[6] < slreal_1.u * 100.) {
	    slint_1.flag__ = -36;
	}
    }
    if (job[2]) {
	if (*n == 1) {
	    slint_1.flag__ = -30;
	} else {
	    if (sllog_1.afin && xef[2] <= slreal_1.a) {
		slint_1.flag__ = -39;
	    }
	    i__1 = *n - 1;
	    for (i__ = 3; i__ <= i__1; ++i__) {
		if (xef[i__ - 1] >= xef[i__]) {
		    slint_1.flag__ = -39;
		}
/* L10: */
	    }
	    if (sllog_1.bfin && xef[*n - 1] > slreal_1.b) {
		slint_1.flag__ = -39;
	    }
	}
	if (! sllog_1.afin && xef[2] >= 0.) {
	    slint_1.flag__ = -39;
	}
	if (! sllog_1.bfin && xef[*n - 1] <= 0.) {
	    slint_1.flag__ = -39;
	}
    }
    if ((job[2] || job[3]) && *nev > 0) {
	i__1 = *nev;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (indxev[i__] < 0) {
		slint_1.flag__ = -37;
	    }
/* L20: */
	}
    }
    if (job[3]) {
	if (*numt <= 0) {
	    slint_1.flag__ = -38;
	}
	i__1 = *numt;
	for (i__ = 2; i__ <= i__1; ++i__) {
	    if (t[i__] <= t[i__ - 1]) {
		slint_1.flag__ = -39;
	    }
/* L30: */
	}
    }
    if (slint_1.flag__ < 0) {
	return 0;
    }

/*     Set MAXEXT, the maximum number of extrapolations allowed, and */
/*         MAXINT, the maximum number of intervals in X(*) allowed when */
/*         mesh is chosen by START. */

/*     IMPORTANT NOTE: the size of various fixed arrays in this package */
/*     depends on the value of MAXEXT in this FORTRAN77 implementation. */
/*     If MAXEXT is increased, then more storage may have to be allocated */
/*     to the columns of R(*,*) in EXTRAP. */

    slint_1.maxext = 6;
    slint_1.maxint = 31;

/*     Calculate maximum number of columns in extrapolation table */
/*     and the maximum number of levels allowed. */

    k = (integer) (-d_lg10(&tol[2]));
/* Computing MAX */
    i__1 = k + 3;
    i__ = max(i__1,0);
/* Computing MIN */
/* Computing MAX */
    i__2 = 3, i__3 = i__ / 2;
    i__1 = max(i__2,i__3);
    *nextrp = min(i__1,slint_1.maxext);

/*     Calculate the initial mesh. */

    if (*n > 0) {
	slint_1.nxinit = *n;
    } else {
/* Computing MIN */
	i__1 = (*nextrp << 1) + 3;
	slint_1.nxinit = min(i__1,slint_1.maxint);
	if (job[1]) {
	    *n = slint_1.nxinit;
	}
    }
    if (job[2]) {
	slreal_1.a = xef[1];
	slreal_1.b = xef[slint_1.nxinit];
	i__1 = slint_1.nxinit;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = xef[i__];
/* L40: */
	}
    }
    return 0;
} /* start_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int step_(doublereal *x, doublereal *h__, doublereal *ev, 
	doublereal *px, doublereal *rx, doublereal *tau, doublereal *omega, 
	doublereal *homega, doublereal *psi, doublereal *dpsi, doublereal *
	sclog, integer *mode)
{
    /* Initialized data */

    static doublereal over = 1e8;

    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal), cos(
	    doublereal), sin(doublereal), tanh(doublereal), log(doublereal);

    /* Local variables */
    static doublereal t, z__, fp, fr, dx, qx, tmu;
    //extern /* Subroutine */ int coeff_(doublereal *, doublereal *, doublereal 	    *, doublereal *);


/*    Evaluate the coefficient functions, the scaled basis function PSI, */
/*    its derivative DPSI, and the log of the scale factor SCLOG. */



/*    Evaluate the coefficient functions at X and calculate TAU.  The */
/*    error flag FLAG is zero for a successful calculation; if p(x) */
/*    or r(x) are zero, then FLAG is set to -15.  If the argument for */
/*    a trig function exceeds OVER then FLAG is set to -10. */
/*         Proceed normally when Mode = 0; when Mode = 1 or 2 use the */
/*    change of variable for "hard" problems; when Mode = 3 or 4 use */
/*    the change of variable t = -1/x near infinity. */

    if (*mode == 0) {
	coeff_(x, px, &qx, rx);
    } else {
	if (*mode >= 3) {
	    t = *x;
	    *x = -1. / t;
	    coeff_(x, px, &qx, rx);
	    t *= t;
	    *px = t * *px;
	    qx /= t;
	    *rx /= t;
	} else {
	    t = *x;
	    if (slclss_1.eta[(*mode << 1) - 2] == 1.) {
		dx = slclss_1.d__[(*mode << 2) - 4] * abs(t);
	    } else {
		d__1 = abs(t);
		d__2 = 1. / slclss_1.eta[(*mode << 1) - 2];
		dx = slclss_1.d__[(*mode << 2) - 4] * pow_dd(&d__1, &d__2);
	    }
	    if (*mode == 1) {
		z__ = slreal_1.a + dx;
	    } else {
		z__ = slreal_1.b - dx;
	    }
	    coeff_(&z__, px, &qx, rx);
	}
    }
    ++slint_1.ncoeff;
    if (*px == 0. || *rx == 0.) {
	slint_1.flag__ = -15;
	return 0;
    }
    if (*mode == 1 || *mode == 2) {
	if (slclss_1.emu[*mode - 1] == .5) {
	    tmu = abs(t);
	} else {
	    d__1 = t * t;
	    tmu = pow_dd(&d__1, &slclss_1.emu[*mode - 1]);
	}
	fp = slclss_1.cp[*mode - 1];
	if (slclss_1.ep[*mode - 1] != 0.) {
	    fp *= pow_dd(&dx, &slclss_1.ep[*mode - 1]);
	}
	fr = slclss_1.cr[*mode - 1];
	if (slclss_1.er[*mode - 1] != 0.) {
	    fr *= pow_dd(&dx, &slclss_1.er[*mode - 1]);
	}
	*px = *px * tmu / fp;
/* Computing 2nd power */
	d__1 = t;
	qx = (qx / fr - slclss_1.d__[(*mode << 2) - 2] / (d__1 * d__1)) * tmu;
	*rx = *rx * tmu / fr;
    }
    *tau = (*ev * *rx - qx) / *px;
    *omega = sqrt((abs(*tau)));
    *homega = *h__ * *omega;
    *sclog = 0.;

/*     Evaluate the scaled basis functions. */

    if (*homega > .01) {
	if (*tau > 0.) {
	    if (*homega > over) {
		slint_1.flag__ = -10;
		return 0;
	    }
	    *dpsi = cos(*homega);
	    *psi = sin(*homega) / *omega;
	} else {
	    *sclog = *homega;
	    if (*homega < slreal_1.under) {
		t = tanh(*homega);
		*dpsi = 1. / (t + 1.);
		*psi = t * *dpsi / *omega;
	    } else {
/* Computing MIN */
		d__1 = *sclog, d__2 = slreal_1.under * 2.;
		*sclog = min(d__1,d__2);
		*dpsi = .5;
		*psi = *dpsi / *omega;
	    }
	}
    } else {
	t = *tau * *h__ * *h__;
	*dpsi = t * (t / 12. - 1.) / 2. + 1.;
	*psi = *h__ * (t * (t / 20. - 1.) / 6. + 1.);
	if (t < 0.) {
/* Computing MAX */
	    d__1 = abs(*psi), d__2 = abs(*dpsi);
	    t = max(d__1,d__2);
	    *sclog = log(t);
	    *psi /= t;
	    *dpsi /= t;
	}
    }
    return 0;
} /* step_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int zzero_(doublereal *b, doublereal *c__, doublereal *fb, 
	doublereal *fc, doublereal *abserr, doublereal *relerr, integer *
	iflag, doublereal *x)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double d_sign(doublereal *, doublereal *);

    /* Local variables */
    static doublereal a, p, q, fa;
    static integer nf, mu;
    static doublereal cmb, tol, acmb, width;
    extern /* Subroutine */ int shoot_(doublereal *, doublereal *, integer *, 
	    doublereal *);
    static integer kount;



/*  ZZERO computes a root of F.  The method used is a combination of */
/*  bisection and the secant rule.  This code is adapted from one in */
/*  the text "Foundations of Numerical Computing" written by Allen, */
/*  Pruess, and Shampine. */

/*  Input parameters: */
/*     B,C   = values of X such that F(B)*F(C) .LE. 0. */
/*     FB,FC = values of F at input B and C, resp. */
/*     ABSERR,RELERR = absolute and relative error tolerances.  The */
/*             stopping criterion is: */
/*               ABS(B-C) .LE. 2.0*MAX(ABSERR,ABS(B)*RELERR). */
/*  Output parameters: */
/*     B,C   = see IFLAG returns. */
/*     FB    = value of final residual F(B). */
/*     IFLAG = 0 for normal return; F(B)*F(C) .LT. 0 and the */
/*               stopping criterion is met (or F(B)=0).  B always */
/*               satisfies ABS(F(B)) .LE. ABS(F(C)). */
/*           = 1 if too many function evaluations were made; in this version */
/*               200 are allowed. */
/*           =-2 if F(B)*F(C) is positive on input. */

/*  Local variables: */

/*  Internal constants */


/*  Initialization. */

    /* Parameter adjustments */
    --x;

    /* Function Body */
    kount = 0;
    width = (d__1 = *b - *c__, abs(d__1));
    a = *c__;
    fa = *fc;
    if (d_sign(&c_b8, &fa) == d_sign(&c_b8, fb)) {
	*iflag = -2;
	return 0;
    }
    *fc = fa;
    nf = 2;
L20:
    if (abs(*fc) < abs(*fb)) {

/*  Interchange B and C so that ABS(F(B)) .LE. ABS(F(C)). */

	a = *b;
	fa = *fb;
	*b = *c__;
	*fb = *fc;
	*c__ = a;
	*fc = fa;
    }
    cmb = (*c__ - *b) / 2.;
    acmb = abs(cmb);
/* Computing MAX */
    d__1 = *abserr, d__2 = abs(*b) * *relerr;
    tol = max(d__1,d__2);

/*  Test stopping criterion and function count. */

    if (acmb <= tol) {
	*iflag = 0;
	return 0;
    }
    if (nf >= 200) {
	*iflag = 1;
	return 0;
    }

/*  Calculate new iterate implicitly as B+P/Q where we arrange */
/*     P .GE. 0.  The implicit form is used to prevent overflow. */

    p = (*b - a) * *fb;
    q = fa - *fb;
    if (p < 0.) {
	p = -p;
	q = -q;
    }

/*  Update A; check if reduction in the size of bracketing interval is */
/*     satisfactory.  If not, bisect until it is. */

    a = *b;
    fa = *fb;
    ++kount;
    if (kount >= 4) {
	if (acmb * 8. >= width) {
	    *b += cmb;
	    goto L30;
	}
	kount = 0;
	width = acmb;
    }

/*  Test for too small a change. */

    if (p <= abs(q) * tol) {

/*  Increment by tolerance. */

	*b += d_sign(&tol, &cmb);
    } else {

/*  Root ought to be between B and (C+B)/2. */

	if (p < cmb * q) {

/*  Use secant rule. */

	    *b += p / q;
	} else {

/*  Use bisection. */

	    *b += cmb;
	}
    }

/*  Have completed computation for new iterate B. */

L30:
    shoot_(b, &x[1], &mu, fb);
    ++nf;
    if (abs(*fb) == 0.) {
	*iflag = 0;
	*c__ = *b;
	*fc = *fb;
	return 0;
    }
    if (d_sign(&c_b8, fb) == d_sign(&c_b8, fc)) {
	*c__ = a;
	*fc = fa;
    }
    goto L20;
} /* zzero_ */

#ifdef __cplusplus
	}
#endif

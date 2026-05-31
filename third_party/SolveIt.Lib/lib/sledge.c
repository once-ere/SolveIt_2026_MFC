/*                            SLEDGE files                             */
/*                  (see sledge.doc for information)                   */
#include "sledge.h"
#include "sl_gv.h"

/*********************** C version 2 ************************************
                         December  2, 1991
                revised: December 23, 1994                             */

void sledge(logical *job, double *cons, logical *endfin, int *invec,
            double *tol, double *ev, int *numx, double *t,
            double *rho, int *iflag)
{
/*   input: job[], cons[], endfin[], invec[], tol[], *numx, xef[], t[];
     output: ev[], rho[], iflag[];

     This is the interface routine between the user and other routines
     which carry out most of the actual calculations.

     Local variables:                                                  */

     int i, ibase, j, k, nev, numt, flag;
     long int lastev;
     double alpha, cev[2], sgn, tol1, tolmax = 1.e-4;
     logical cspec[2], domesh, jobst[3], lbase, lmesh, lplc[2], oscill;

/* Initialize                                                          */

     g_afin = endfin[0];
     g_bfin = endfin[1];
     nev = invec[0];
     numt = invec[1];
     g_lnf = FALSE;
     domesh = TRUE;
     lmesh = FALSE;
     flag = 0;
     iflag[0] = 0;
     ibase = 1;
     lbase = FALSE;
     for (i = 0; i < 6; i++) g_lflag[i] = FALSE;
     tol1 = min_d(tol[0] + tol[1], tolmax);
     jobst[0] = job[1];
     if ((*numx > 0) && (!job[4]))
        jobst[1] = TRUE;
     else
        jobst[1] = FALSE;
     jobst[2] = job[2];
     if ((!job[0]) && (!job[1])) nev = 0;
     flag = start(jobst, cons, tol, nev, invec+1, numx, numt, t);
     if (job[3])
     {
        alpha = g_a2 * g_a1p - g_a1 * g_a2p;
        if ((g_a1p != 0.0) || (g_a2p != 0.0))
        {
           if (alpha <= 0.0) flag = -32;
        }
        else
        {
           if ((g_a1 == 0.0) && (g_a2 ==  0.0)) flag = -33;
        }
        if ((g_b1 == 0.0) && (g_b2 == 0.0)) flag = -31;
     }
     if (flag < 0) goto exit;
     if (job[0] || job[1])
     for (i = 0; i < nev; i++) ev[i] = 0;
     if ((!job[3]) || job[2])
     {
        flag = classify(tol1, jobst[1], cspec, cev, &lastev, lplc, 
                        job[4], &domesh);
        alpha = g_a2 * g_a1p - g_a1 * g_a2p;
        if ((g_a1p != 0.0) || (g_a2p != 0.0))
          if (alpha <= 0.0) flag = -32;
        else
          if ((g_a1 == 0.0) && (g_a2 == 0.0)) flag = -33;
        if ((g_b1 == 0.0) && (g_b2 == 0.0)) flag = -31;
        if (flag < 0) goto exit;
        for (k = 0; k < 2; k++)
        {
           ctype[k][0] = g_reg[k];
           ctype[k][1] = g_lc[k];
           if (g_osc[k])
              ctype[k][2] = FALSE;
           else
              ctype[k][2] = TRUE;
           ctype[k][3] = g_osc[k];
           if (cspec[k])
           {
              ctype[k][2] = FALSE;
              ctype[k][3] = FALSE;
           }
        }
        if (g_print > 2) description(lplc, cspec, cev, lastev);
     }
     else
     {
        g_lnf = FALSE;
        kclass[0] = 0;
        kclass[1] = 0;
        for (k = 0; k < 2; k++)
        {
           g_reg[k] = ctype[k][0];
           g_lc[k]  = ctype[k][1];
           g_osc[k] = ctype[k][3];
           if (ctype[k][2] || ctype[k][3])
              cspec[k] = FALSE;
           else
              cspec[k] = TRUE;
        }
        if (!g_afin)
           x[0] = -99999.0;
        if (!g_bfin)
           x[g_nxinit-1] = 99999.0;
     }

/*   Use NSGNF to hold the sign of F when EV is large negative.   */

     sgn = g_a2p * g_b2;
     if (sgn != 0.0)
        g_nsgnf = (int) sign(1.0, sgn);
     else
     {
        sgn = g_a1p * g_b2 + g_a2p * g_b1;
        if (sgn != 0.0)
           g_nsgnf = (int) sign(1.0, sgn);
        else
        {
           sgn = g_a1p * g_b1 + g_a2 * g_b2;
           if (sgn != 0.0)
              g_nsgnf = (int) sign(1.0, sgn);
           else
           {
              sgn = g_a1 * g_b2 + g_a2 * g_b1;
              if (sgn != 0.0)
                 g_nsgnf = (int) sign(1.0, sgn);
              else
                 g_nsgnf = (int) sign(1.0, g_a1 * g_b1);
           }
        }
     }

     if (fabs(g_cutoff) < tol1 * tol1) g_cutoff = 0.0;  /* temporary fix */

     if (((!ctype[0][0]) && ctype[0][3]) || ((!ctype[1][0]) && ctype[1][3]))
        oscill = TRUE;
     else
        oscill = FALSE;
     tol1 = tol[0] + tol[1];
     if (job[0] || job[1])
     {

/*    Set up approximating regular problems for eigenvalues.        */

        if (oscill)
        {
           if (g_print > 0)
           {
              printf(
    " This problem is oscillatory, you must use interval truncation.\n");
              fprintf(output_file,
    " This problem is oscillatory, you must use interval truncation.\n");
           }
           flag = -20;
           goto exit;
        }
        if (domesh)
        {

/*         Calculate the initial mesh.                         */

           flag = mesh(job[4], -1, tol1);
           if (flag < 0) goto exit;
        }
        if (((kclass[0] == 3) || (kclass[1] == 3)) && job[4])
           lmesh = TRUE;
        if ((!lmesh) && (g_print > 0))
        {
           printf(" Level 0 mesh:\n");
           fprintf(output_file, "Level 0 mesh:\n");
           for (i = 0; i < g_nxinit; i = i+5)
           {
              for (j = 0; (j < 5) && (i+j < g_nxinit); j++)
              {
                 printf("%13.6e  ", x[i+j]);
                 fprintf(output_file, "%13.6e  ", x[i+j]);
              }
              printf("\n");
              fprintf(output_file, "\n");
           }
           fprintf(output_file, "\n");
           printf("\n");
        }
        if (job[1])
        {
           ef   = matrix_d(nev, g_nxinit);
           pdef = matrix_d(nev, g_nxinit);
        }
        else
        {
           ef   = matrix_d(nev, 1);
           pdef = matrix_d(nev, 1);
        }
        if (job[4])
        {
           *numx = g_nxinit;
           if (job[1])
              xef = vector_d(g_nxinit);
           for (i = 0; i < *numx; i++) xef[i] = x[i];
        }

/*      Set g_maxlvl, the maximum number of levels (mesh bisections).

        IMPORTANT NOTE: the size of various fixed arrays in this
        package depends on the value of g_maxlvl in this implementation.
        If g_maxlvl is increased, then more storage may have to be
        allocated to these arrays.  In particular,  check ratio[*],
        r[*][*], and w[*][*] in extrap; evext[*] in regular.            */

        g_maxlvl = 10;

        for (k = 0; k < nev; k++)
        {
           ev[k] = 0.0;
           iflag[k] = 0;
           flag = regular(job[1], lmesh, tol, invec[k+2], &ev[k],
                          &ef[k][0], &pdef[k][0]);
           if ((cspec[0] || cspec[1]) && (g_print > 0) &&
               (!job[3]) && (flag > -5) && ((ev[k] >= g_cutoff) ||
               ((lastev != -5) && (invec[k+2] >= lastev))))
           {
              printf(" WARNING: Requested eigenvalue %6d ", invec[k+2]);
              printf(" may not be below the continuous spectrum.\n");
              fprintf(output_file,
                      " WARNING: Requested eigenvalue %6d may not",
                      invec[k+2]);
              fprintf(output_file," be below the continuous spectrum.\n");
           }
           if (g_lflag[0])
           {
              iflag[k] += ibase;
              g_lflag[0] = FALSE;
              lbase = TRUE;
           }
           if (flag < 0) iflag[k] = flag;
        }
        if (lbase)
        {
           ibase *= 10;
           lbase = FALSE;
        }
     }
     if (job[2])
     {
        for (k = 0; k < numt; k++) rho[k] = 0.0;
        if (!(cspec[0] || cspec[1])) iflag[0] = -4;
        if (cspec[0] && cspec[1]) iflag[0] = -5;
        if ((cspec[0] && ((kclass[1] == 5) || (kclass[1] == 9))) ||
           (cspec[1] && ((kclass[0] == 5) || (kclass[0] == 9))) ||
           (cspec[0] && (!g_bfin)) || (cspec[1] && (!g_afin)))
           iflag[0] = -6;
        if (oscill) iflag[0] = -25;
        if ((g_print > 0) && (iflag[0] < 0))
        {
           switch (iflag[0])
           {
              case -4:
                 printf(" This problem has no continuous spectrum.\n");
                 break;
              case -5:
                 printf(
   " This problem has continuous spectrum generated by both endpoints.\n");
                 printf(
   " The calculation of the spectral density function has not yet been\n");
                 printf(" implemented for such cases.\n");
                 goto exit;
              case -6:
                 printf(" The normalization of the spectral density\n");
                 printf(" function is unknown\n for this problem.\n");
                 goto exit;
              case -25:
                 printf(
   " This problem is oscillatory, you must use interval truncation\n.");
                 goto exit;
              default:
                 goto exit;
           }
           goto exit;
        }
        flag = get_rho(cspec, job, numx, numt, t, rho, tol);
     }

/*   Set fatal output flags.                           */

     exit:
     if (flag < -9)
        for (k = 0; k < nev+1; k++) iflag[k] = flag;
     else
     {
        if ((flag < 0) && (!(job[0] || job[1]))) iflag[nev] = flag;

   /*   Set warning flags.                             */

        for (i = 1; i < 5; i++)
        {
           for (k = 0; k < nev+1; k++)
              if (g_lflag[i] && (iflag[k] >= 0)) iflag[k] += (i+1) * ibase;
           if (g_lflag[i]) ibase *= 10;
        }
     }
     if (g_print > 0)
     {
        printf(" Number of COEFF calls = %ld\n", g_ncoeff);
        fprintf(output_file, " Number of COEFF calls = %ld\n", g_ncoeff);
     }
     free(x);
}

/***********************************************************************/

int interv(logical first, double alpha, double beta, double *cons,
           logical *endfin, long int *nfirst, long int *ntotal)
{
/* INTERV calculates the indices of eigenvalues found in a specified
     interval.
     input: first,alpha,beta,cons[],endfin[];
     output: *nfirst,*ntotal;
     return flag =   0 , normal return, output should be reliable,
                 =  11 , there are no eigenvalues in [alpha, beta],
                 =  12 , low confidence in *nfirst or *ntotal or both,
                 =  13 , beta and/or alpha exceed the cutoff for the
                         continuous spectrum.  If only beta is too big
                         then *nfirst may be OK, but *ntotal is meaningless.
                 = -11 , alpha >= beta,
                 = -25 , oscillatory endpoint, output meaningless,
                 = -3? , illegal cons[] values (see above comments on sledge
                         for an explanation).

     Local variables:                                                  */

     int flag, i, k, level0;
     long int i1, i2, i3, j1, j2, j3, lastev, nlast;
     double cev[2], tol[6], v;
     logical cspec[2], domesh, jobst[3], lplc[2];

     *nfirst = -5l;
     nlast = -5l;
     flag = 0;
     if (alpha >= beta) return(-11);
     tol[0] = 0.000001;
     tol[1] = 0.000001;
     if (first)
     {
        jobst[0] = FALSE;
        jobst[1] = FALSE;
        jobst[2] = FALSE;
        g_afin = endfin[0];
        g_bfin = endfin[1];
        g_maxlvl = 10;
        flag = start(jobst, cons, tol, 0, &k, &i, 0, &v);
        if (flag >= 0)
        {
           k = g_print;
           g_print = 0;
           flag = classify(tol[0], jobst[1], cspec, cev, &lastev,
                           lplc, TRUE, &domesh);
           g_print = k;
           if (flag < 0) return (flag);
           if (g_osc[0] || g_osc[1]) return(-25);
           if (domesh) flag = mesh(TRUE, -1, tol[0]);
        }
     }
     if (flag < 0) return (flag);
     level0 = g_level;
     g_countz = TRUE;
     g_level = 3;
     flag = shoot(alpha, &i1, &v);
     if (flag < 0) return (flag);
     flag = shoot(beta, &j1, &v);
     if (flag < 0) return (flag);
     ++g_level;
     flag = shoot(alpha, &i2, &v);
     if (flag < 0) return (flag);
     flag = shoot(beta, &j2, &v);
     if (flag < 0) return (flag);
     while (g_level < g_maxlvl)
     {
        ++g_level;
        if (*nfirst == -5l)
        {
           flag = shoot(alpha, &i3, &v);
           if (flag < 0) return (flag);
           if ((i1 == i2) && (i2 == i3))
              *nfirst = i1;
           else
           {
              i1 = i2;
              i2 = i3;
           }
        }
        if (nlast == -5l)
        {
           flag = shoot(beta, &j3, &v);
           if (flag < 0) return (flag);
           if ((j1 == j2) && (j2 == j3))
           {
              nlast = j1-1;
              if (*nfirst != -5l) break;
           }
           j1 = j2;
           j2 = j3;
        }
     }
     if (*nfirst == -5l)
     {
        *nfirst = i3;
        flag = 12;
     }
     if (nlast == -5l)
     {
        nlast = j3;
        flag = 12;
     }
     *ntotal = nlast + 1 - (*nfirst);
     if (*ntotal == 0l) flag = 11;
     if (beta > g_cutoff) flag = 13;
     g_countz = FALSE;
     g_level = level0;
     return(flag);
}

/***********************************************************************/

double aitken(double* xlimit, double tol, int n, double* x)
{
     double denom, error, xold;
     int i;

/*   Use Aitken's algorithm to accelerate convergence of the sequence in x[*].

     input: tol, n, x[];
     output: *xlimit;                                                     */

     if (n > 2)
     {
        xold = 1.0 / g_u;
        error = xold;
        for (i = 0; i < n-2; i++)
        {
            denom = x[i+2] - 2.0 * x[i+1] + x[i];
            if (denom != 0.0)
            {
               *xlimit = x[i] - (x[i+1] - x[i]) * (x[i+1] - x[i]) / denom;
               error = *xlimit - xold;
            }
            else
            {
               error = x[i+2] - x[i+1];
               *xlimit = x[i+2];
            }
            if (fabs(error) < max_d(1.0, fabs(*xlimit)) * tol) return(error);
            xold = *xlimit;
        }
     }
     else
     {
        *xlimit = x[n-1];
        error = 0.0;
     }
     return(error);
}

/***********************************************************************/

double asymp_ev(int nev, double qint, double rpint, double alpha1,
                double alpha2, double beta1, double beta2)
{
/*   input: nev, qint, rpint, alpha1, alpha2, beta1, beta2;
     return function value: value                                     */

     double value = 0.0, fnev;

/*   Evaluate the asymptotic formula for eigenvalue nev.
     Note: not all cases have been implemented yet.                   */

     value = -1.0 / g_u;
     fnev = (double) nev;
     if (g_reg[0])
     {
        if ((g_a1p != 0.0) || (g_a2p != 0.0))
        {
           if (g_a2p != 0.0)
           {
              value = (2.0 * g_a1p / g_a2p + qint) / rpint;
              if (g_b2 != 0.0)

/*               Case 1                                              */

                 value += 2.0 * g_b1 / (g_b2 * rpint) + pow((fnev - 1.0)
                          * PI / rpint, 2);
              else

/*               Case 2                                               */

                 value += pow((fnev - 0.5) * PI / rpint, 2);
           }
           else
           {
              value = (2.0 * g_a2 / g_a1p + qint) / rpint;
              if (g_b2 != 0.0)

/*               Case 3                                              */

                 value += 2.0 * g_b1 / (g_b2 * rpint) + pow((fnev - 0.5)
                          * PI / rpint, 2);
              else

/*               Case 4                                              */

                 value += pow(fnev * PI / rpint, 2);
           }
        }
        else
        {
           if (g_a2 != 0.0)
           {
              if (g_b2 != 0.0)

/*               Case 1                                              */

                 value = pow(fnev * PI / rpint, 2) + (2.0 * (beta1 / beta2
                         + alpha1 / alpha2) + qint) / rpint;
              else

/*               Case 2   (Dirichlet at B)                             */

                 value = pow((fnev + 0.5) * PI / rpint, 2)+(2.0 * alpha1
                         / alpha2 + qint) / rpint;
           }
           else
           {
              if (g_b2 != 0.0)

/*               Case 3   (Dirichlet at A)                             */

                 value = pow((fnev + 0.5) * PI / rpint, 2) + (2.0 * beta1
                         / beta2 + qint) / rpint;
              else

/*               Case 4   (Dirichlet at A and at B)                    */

                 value = pow((fnev + 1.0) * PI / rpint, 2) + qint / rpint;
           }
        }
        return(value);
     }
     if (g_reg[1])
     {
        if (g_b2 != 0.0)
        {
           if (g_a2 != 0.0)

/*            Case 1                                                 */

              value = pow(fnev * PI / rpint, 2) + (2.0 * (alpha1 / alpha2
                      + beta1 / beta2) + qint) / rpint;
           else

/*            Case 2   (Dirichlet at A)                              */

              value = pow((fnev + 0.5) * PI / rpint, 2) + (2.0 * beta1 /
                      beta2 + qint) / rpint;
        }
        else
        {
           if (g_b2 != 0.0)

/*            Case 3   (Dirichlet at B)                              */

              value = pow((fnev + 0.5) * PI / rpint, 2) + (2.0 * alpha1 /
                      alpha2 + qint) / rpint;
           else

/*            Case 4   (Dirichlet at A and at B)                     */

              value = pow((fnev + 1.0) * PI / rpint, 2) + qint / rpint;
        }
     }
     return(value);
}

/***********************************************************************/

double asymp_r(int nev, double rpint, double rpata, double rpatb, double scale)
{
/*   Evaluate the asymptotic formula for RsubNEV.
     Note: not all cases have been implemented yet.  See the note
           above in asymp_ev.
     input: nev, rpint, rpata, rpatb, scale; return: value.             */

     double fnev, value;

     value = 0.0;
     fnev = (double) max_i(nev, 2);
     if (g_reg[0])
     {
        if ((g_a1p != 0.0) || (g_a2p != 0.0))
        {
           if (g_a2p != 0.0)
           {
              if (g_b2 != 0.0)
                 value = 2.0 * pow(rpint, 3) / (rpata * g_a2p * g_a2p *
                         pow((fnev - 1.0) * PI, 4));
              else
                 value = 2.0 * pow(rpint, 3) / (rpata * g_a2p * g_a2p *
                         pow((fnev - 0.5) * PI, 4));
           }
           else
           {
              if (g_b2 != 0.0)
                 value = rpata * 2.0 * rpint / pow(g_a1p * (fnev - 0.5) 
                         * PI, 2);
              else
                 value = rpata * 2.0 * rpint / pow(g_a1p * fnev * PI, 2);
           }
        }
        else
        {
           if (g_a2 != 0.0)
              value = 2.0 / (rpata * g_a2 * g_a2 * rpint);
           else
           {
              if (g_b2 != 0.0)
                 value = 2.0 * rpata * pow((fnev + 0.5) * PI / g_a1, 2)
                         / pow(rpint, 3);
              else
                 value = 2.0 * rpata * pow((fnev + 1.0) * PI / g_a1, 2)
                         / pow(rpint, 3);
           }
        }
        return(value);
     }
     if (g_reg[1])
        if (g_a2 != 0.0)
           value = 2.0 / (rpatb * g_b2 * g_b2 * rpint);
        else
        {
           if (g_b2 != 0.0)
              value = 2.0 * rpatb * pow((fnev + 0.5) * PI / g_b1, 2)
                      / pow(rpint, 3);
           else
              value = 2.0 * rpatb * pow((fnev + 1.0) * PI / g_b1, 2)
                      / pow(rpint, 3);
        }
     value *= scale * scale;
     return(value);
}

/***********************************************************************/

int bracket(int n, double *evlow, double *evhigh, double *flow, 
            double *fhigh, double abserr, double relerr)
{
/*   input: n, *evlow, *evhigh, *flow, *fhigh, abserr, relerr;
     output: *evlow, *evhigh, *flow, *fhigh.
     return flag: 0 -- success, < 0 -- failure.

     Find values for evlow and evhigh which bracket the nth eigenvalue;
     in particular,
           ev(n-1) < evlow < ev(n) < evhigh < ev(n+1)  .
     It is assumed that if u(x,lambda) has nz zeros in (A,B) then
           ev(mu-1) < lambda < ev(mu)
     where mu is a function of nz, lambda, and the constants in the
     boundary conditions.  The value of mu for a given lambda is
     returned by the shoot function.                                  */

     int flag, k;
     long int mu;
     double diff, ev, evsign, fev;
     logical low, high, sht;

/*   Set countz so that zeros are counted in shoot.                   */

     g_countz = TRUE;
     evsign = (double) g_nsgnf;

/*   SHOOT with ev = evlow should return fev having sign evsign.      */

     if (n != 2 * (n / 2)) evsign = -evsign;
     low  = FALSE;
     high = FALSE;

/*   Make evlow a lower bound for ev(n).                              */

     ev   = *evlow;
     diff = fabs((*evhigh) - (*evlow));
     if (diff == 0.0) diff = abserr + relerr;
     do 
     {
        flag = shoot(ev, &mu, &fev);
        sht = FALSE;
        if (flag < 0)
        {
           g_countz = FALSE;
           return(flag);
        }
        if (mu > n)
        {
           *evhigh = ev;
           *fhigh  = fev;
           ev      = ev - diff;
           diff    = 2.0 * diff;
           if ((mu == n+1) && (evsign*fev <= 0.0)) high = TRUE;
           sht = TRUE;
        }
        else
        {
           *evlow = ev;
           *flow = fev;
           if ((mu == n) && (evsign*fev >= 0.0)) low = TRUE;
        }
     } while (sht);

/*   Make evhigh an upper bound for ev(n).                             */

     if (!high)
     {
        ev = *evhigh;
        diff = fabs((*evhigh) - (*evlow));
        do 
        {
           flag = shoot(ev, &mu, &fev);
           sht = FALSE;
           if (flag < 0) return(flag);
           k = g_nsgnf * pow(-1, fmod(mu, 2));
           if (k*fev < 0.0)
           {
              ev   += diff;
              diff *= 2.0;
              sht   = TRUE;
           }
           else
           {
              if (mu <= n)
              {
                 *evlow = ev;
                 *flow  = fev;
                 ev    += diff;
                 diff  *= 2.0;
                 sht    = TRUE;
              }
              else
              {
                 *evhigh = ev;
                 *fhigh  = fev;
                 if (mu == n+1) high = TRUE;
              }
           }
        } while (sht);
     }

/*   Refine the interval [evlow,evhigh] to include only the nth
     eigenvalue.                                                       */

     while ((!low) || (!high))
     {
        diff = (*evhigh) - (*evlow);
        ev   = (*evlow) + diff / 2.0;

/*      Check for a cluster of eigenvalues within user's tolerance.    */

        if (2.0 * diff < max_d(abserr, relerr * (max_d(fabs(*evlow),
                               fabs(*evhigh)))))
        {
           g_lflag[0]= TRUE;
           g_countz = FALSE;
           return(0);
        }
        flag = shoot(ev, &mu, &fev);
        if (flag < 0)
        {
           g_countz = FALSE;
           return(flag);
        }

/*    Update evlow and evhigh.                                         */

        if (mu == n)
        {
           *evlow = ev;
           low    = TRUE;
           *flow  = fev;
        }
        else
           if (mu == n+1)
           {
              *evhigh = ev;
              high    = TRUE;
              *fhigh  = fev;
           }
           else
              if (mu < n)
              {
                 *evlow = ev;
                 *flow  = fev;
              }
              else
              {
                 *evhigh = ev;
                 *fhigh  = fev;
              }
     }
     g_countz = FALSE;
     return(0);
}

/***********************************************************************/

int classify(double tol, logical job, logical *cspec, double *cev,
             long int *lastev, logical *lplc, logical jmesh, logical *domesh)
{
/*   input: tol, job, jmesh;
     output: cspec[], cev[], *lastev, lplc[], *domesh;
     return flag: 0 -- success; > 0 -- warning; < 0 -- failure;

     This routine classifies the Sturm-Liouville problem.  Note:
     (1) any computational algorithm must be based on a finite
       amount of information; hence, there will always be cases that
       any algorithm misclassifies.  In addition, some problems are
       inherently ill-conditioned, in that a small change in the
       coefficients can produce a totally different classification.
     (2) The maximum number of points sampled for singular problems
       is given by the variable kmax which is less than maxk defined in
       sledge.h.  By increasing this number, the reliability of the
       classification may increase; however, the computing time may also
       increase.  The values we have chosen seem to be a reasonable
       balance for most problems.
     (3) The algorithms apply standard theorems involving limits of
       the Liouville normal form potential.  When this is not available,
       each coefficient function is approximated by a power function
       (c*x^r) and classified according to the properties of the
       resulting Liouville approximation.                             */

     int flag = 0, j, k, kmax, m;
     long int last[3];
     double base, bc[2], end, ev, fev, clsover, *z, *pz, *qz, *rz, *y, s, sgn;
     logical endfin[2];

/*   Sample the coefficient functions; determine if the problem as
     given is in Liouville normal form.                               */

     g_lnf = TRUE;
     *domesh = TRUE;
     for (j = 0; j < 2; j++)
     {
        if (j == 0)
        {
           end = g_a;
           endfin[0] = g_afin;
           sgn = 1.0;
        }
        else
        {
           end = g_b;
           endfin[1] = g_bfin;
           sgn = -1.0;
        }
        k = (int) (0.1 - log10(tol));
        kmax = min_i(max_i(4 * k, 10), MAXK);
        m = 0;
        base = 1.0 / min_i(max_i(k, 4), 8);
        if (endfin[j])
        {
           if (end != 0.0)
              kmax = min_i((int)(-log10(g_u) + 0.5) - 2, kmax);
        }
        else
        {
           kmax = min_i(kmax, MAXK/2);
           base = 8.0;
        }
        z  = vector_d(kmax);
        pz = vector_d(kmax);
        qz = vector_d(kmax);
        rz = vector_d(kmax);
        y  = vector_d(kmax);
        for (k = 0; k < kmax; k++) z[k] = pow(base, k + 1);
        clsover = g_under / 2.0;
        for (k = 0; k < kmax; k++)
        {
           if (endfin[j])
              s = end + sgn * z[k];
           else
              s = -sgn * z[k];
           coeff(s, pz + k, qz + k, rz + k);
	   ++g_ncoeff;
           if ((pz[k] <= 0.0) || (rz[k] <= 0.0)) return(-15);
           if ((g_lnf && (k > 0)) &&
              ((pz[k] != pz[k-1]) || (rz[k] != rz[k-1])))
	      g_lnf = FALSE;
           s = log(pz[k]);
           if (fabs(s) > clsover) m = 1;
           s = log(1.0+fabs(qz[k]));
           if (fabs(s) > clsover) m = 1;
           s = log(rz[k]);
           if (fabs(s) > clsover) m = 1;
           if (m != 0) break;
        }
        if (m == 0) k = kmax-1;
        flag = classify_end(z, pz, qz, rz, y, k, &end, endfin[j], j,
                            tol, cev + j, cspec + j, bc, lplc);
        if (!job)
        {
          if ((!g_afin) && (j == 0)) x[0] = -end;
          if ((!g_bfin) && (j == 1)) x[g_nxinit-1] = end;
        }
        if ((cspec[j] || (!g_osc[j])) && (!g_reg[j]))
        if (j == 0)
        {
           g_a1 = bc[0];
           g_a1p = 0.0;
           g_a2 = bc[1];
           g_a2p = 0.0;
        }
        else
        {
           g_b1 = bc[0];
           g_b2 = bc[1];
        }
        if (flag == 1) g_lflag[1] = TRUE;
        free(z);
        free(pz);
        free(qz);
        free(rz);
        free(y);
     }
     g_cutoff = min_d(cev[0], cev[1]);

/*   Find the number of eigenvalues below the start of the continuous
     spectrum.                                                         */

     *lastev = -5;
     if (((!g_osc[0]) && (!g_lc[1]) && g_osc[1]) ||
        ((!g_osc[1]) && (!g_lc[0]) && g_osc[0]) ||
        (g_osc[0] && (!g_lc[0]) && g_osc[1] && g_lc[1]) ||
        (g_osc[1] && (!g_lc[1]) && g_osc[0] && g_lc[0]) ||
        ((cspec[0] && g_osc[1]) || (cspec[1] && g_osc[0])))
     *lastev = 0;
     if ((cspec[0] && (!g_osc[1])) || (cspec[1] && (!g_osc[0])) ||
        (cspec[0] && cspec[1]))
     {
        k = g_nxinit + 16;
        flag = mesh(jmesh, -1, tol);
        *domesh = FALSE;
        for (j = 0; j < 2; j++)
        {
            g_level = 3 * (j + 1);
            g_countz = TRUE;
            ev = g_cutoff;
            flag = shoot(ev, &last[j], &fev);
            if (flag < 0) return(flag);
            if (g_print > 4)
               fprintf(output_file,
                       " When level = %2d, Ev index at cutoff is %d\n",
                       g_level, last[j]);
        }
        g_countz = FALSE;
        if (last[0] >= last[1])
        {
           *lastev = last[1];
           if (last[0] != last[1])
           {
              g_lflag[1] = TRUE;
              if (g_print > 2)
                 fprintf(output_file," The eigenvalue count is uncertain.\n");
              if (last[0] > 2 * last[1])
                 *lastev = 0 ;
           }
        }
        else
           *lastev = -5;
     }
     return(flag);
}

/***********************************************************************/

int classify_end(double *z, double *pz, double *qz, double *rz, double *y,
    int kmax, double *end, logical endfin, int iend, double tol,
    double *cev, logical *cspec, double *bc, logical *lplc)
{
/*   input: z[], pz[], qz[], rz[], y[], kmax, endfin, iend, tol;
     output: *end, *cev, *cspec, bc[], lplc[];
     return: flag = 0    if reasonably certain of the classification;
                  = 1    if not sure.

     Information about the nature of the problem at singular point
     iend is passed through the variable kclass[iend]:
     kclass[*] = 0   normal;
               = 1   oscillatory coefficient function;
               = 2   regular, but 1/p, q, or r unbounded;
               = 3   infinite endpoint, Eqlnf = -1 ;
               = 4   finite singular endpoint, Tau unbounded, (not 8-10);
               = 5   not "hard", irregular;
               = 6   "hard" irregular with Eta(1) < 0;
               = 7   finite end which generates Cspectrum;
               = 8   Q is unbounded (< 1/t^2) near a nonoscillatory finite end;
               = 9   Q is unbounded (like 1/t^2) near a nonoscillatory finite
                     end;
               = 10  "hard", irregular, Eta(1) > 0.
                 Note: "hard" means Tau goes to +infinity at a finite
                 nonoscillatory endpoint.
     g_reg[*] = TRUE  iff endpoint is regular.
     g_lc[*]  = TRUE  iff endpoint is limit circle.
     g_osc[*] = TRUE  iff endpoint is oscillatory for all Ev.
     *cspec   = TRUE  iff endpoint generates continuous spectrum.
     lplc[*]  = TRUE  iff theory yields Lp/Lc classification.          */

     int flag = 0, i, iqlnf, k;
     logical ex, exact, irreg, posc, qosc, rosc;
     double  cp, cq, cr, c1, c2, c3, delta, ep, eq, er, gamma, sgn, tol4, zz;

     if (g_print > 2)
     {
        if (iend == 0)
           fprintf(output_file, "\n For endpoint A:\n");
        if (iend == 1)
           fprintf(output_file, "\n For endpoint B:\n");
        fprintf(output_file, "  Kmax = %3d\n", kmax);
     }
     *cspec = FALSE;
     irreg = FALSE;
     lplc[iend] = TRUE;
     kclass[iend] = 0;
     g_pnu[iend] = 0.0;
     *cev = 1.0 / g_u;
     ex = TRUE;
     c1 = 0.0;
     c2 = 0.0;
     tol4 = 4.0 * tol;

/*   Seek monomial approximations to each coefficient function.        */

     flag = power(z, qz, y, kmax, tol, &eq, &cq, &qosc, &exact);
     if (fabs(cq) <= tol) cq = 0.0;
     if (cq == 0.0) eq = 0.0;
     if (g_lnf)
     {
        ep = 0.0;
        cp = pz[0];
        er = 0.0;
        cr = rz[0];
        posc = FALSE;
        rosc = FALSE;
     }
     else
     {
        flag = power(z, pz, y, kmax, tol, &ep, &cp, &posc, &exact);
        if (fabs(cp) <= tol) ep = 0.0;
        if (ex && exact)
           ex = TRUE;
        else
           ex = FALSE;
        flag = power(z, rz, y, kmax, tol, &er, &cr, &rosc, &exact);
        if (fabs(cr) <= tol) er = 0.0;
     }
     if (posc || rosc)
     {
        if (endfin)
           g_reg[iend] = TRUE;
        else
        {
           flag = 1;
           if (g_print > 2)
           {
              fprintf(output_file,
                 " WARNING: p(x) or r(x) is not well-approximated by");
              fprintf(output_file,
                 " a power potential.\n");
              fprintf(output_file,
                 " Classification is uncertain.\n");
           }
           g_reg[iend] = FALSE;
           kclass[iend] = 1;
        }
        g_lc[iend] = TRUE;
        g_osc[iend] = FALSE;
     }
     if (qosc)
     {
        flag = 1;
        if (g_print > 2)
           fprintf(output_file,
         " WARNING: q(x) is not well-approximated by a power potential.\n");
           fprintf(output_file, "Classification is uncertain.\n");
        if (endfin)
        {
           g_reg[iend] = TRUE;
           g_lc[iend]  = TRUE;
           g_osc[iend] = FALSE;
        }
        else
        {
           kclass[iend] = 1;
           g_reg[iend] = FALSE;
           g_lc[iend]  = FALSE;
           *cspec      = TRUE;
           g_osc[iend] = FALSE;
           bc[0] = 1.0;
           bc[1] = 0.0;
           *cev = qz[kmax-2];
           k = 40;
           delta = (z[kmax-1] - z[kmax-2]) / (k+1);
           for (i = 0; i < k; i++)
               zz = z[kmax] - i * delta;
           coeff(zz, &cp, &cq, &cr);
           ++g_ncoeff;
           *cev = min_d(*cev, qz[i]);
           if (fabs(*cev) < tol4) *cev = 0.0;
           if (g_u * fabs(*cev) >= 1.0) *cspec = FALSE;
        }
        g_eqlnf[iend] = 0.0;
     }
     if (g_print > 2)
     {
        fprintf(output_file, "\n  Cp = %19.12e   Ep =  %19.12e \n",
                cp, ep);
        fprintf(output_file, "  Cq = %19.12e   Eq =  %19.12e \n", 
                cq, eq);
        fprintf(output_file, "  Cr = %19.12e   Er =  %19.12e \n\n",
                cr, er);
     }
     g_cp[iend] = cp;
     g_ep[iend] = ep;

/*   Analyze this endpoint.                                            */

     if ((ep < 1.0) && (eq > -1.0) && (er > -1.0) && endfin)
     {
        g_reg[iend] = TRUE;
        g_lc[iend]  = TRUE;
        g_osc[iend] = FALSE;
        *cspec      = FALSE;
        if ((ep > 0.0) || (eq < 0.0) || (er < 0.0)) kclass[iend] = 2;
        return(flag);
     }
     g_reg[iend] = FALSE;
     g_eta[iend][0] = 0.5 * (er - ep + 2.0);
     if (fabs(g_eta[iend][0]) <= tol) g_eta[iend][0] = 0.0;
     if (g_eta[iend][0] != 0.0)
     {
        g_eqlnf[iend] = (eq - er) / g_eta[iend][0];
        iqlnf = (int) (g_eqlnf[iend] + sign(0.5, g_eqlnf[iend]));
        if (fabs(iqlnf - g_eqlnf[iend]) < tol4) 
            g_eqlnf[iend] = (double) iqlnf;
        c1 = (cq / cr) * pow(fabs(g_eta[iend][0]) * sqrt(cp / cr),
              g_eqlnf[iend]);
        if (c1 == 0.0) g_eqlnf[iend] = 0.0;
        c2 = (ep + er) / (4.0 * g_eta[iend][0]);
        c2 *= (c2 - 1.0);
        if (g_print > 4)
        {
           fprintf(output_file, "  C1 = %17.10e     C2 =  %17.10e \n",
                   c1, c2);
           fprintf(output_file, "  Eqlnf = %17.10e  Eta1 = %17.10e\n\n",
                   g_eqlnf[iend], g_eta[iend][0]);
        }
     }
     else
     {
        c3 = (cp / cr) * pow(0.25 * (ep + er), 2);
        if (g_print > 4)
           fprintf(output_file, "  C3 = %17.10e \n\n", c3);
     }
     sgn = -1.0;
     if (!endfin)
     {

/*   Make an initial estimate for "infinity" (used in MESH).          */

        if ((er > eq) || (cq == 0.0))
        {
           if (er != ep)
           {
              gamma = er - ep;
              delta = cr / cp;
           }
           else
           {
              gamma = eq - ep;
              delta = fabs(cq) / cp;
              if (delta == 0.0) delta = 1.0;
           }
        }
        else
        {
           if (eq > er)
           {
              gamma = eq - ep;
              delta = fabs(cq) / cp;
           }
           else
           {
              if (er > ep)
              {
                 gamma = er - ep;
                 delta = cr / cp;
              }
              else
              {
                 gamma = 0.0;
                 if (er == ep)
                    delta = fabs(cr - cq) / cp;
                 else
                    delta = fabs(cq) / cp;
              }
           }
        }
        if (gamma > 0.5)
        {
           if (gamma < 2.0)
              *end = 80.0;
           else
           {
              *end = min_d(max_d(64.0 / ((2.0 * gamma - 3.0) *
                     pow(delta, 1.0 / (gamma + 2.0))), 1.0), 80.0);
              if (gamma > 24.0) *end = 12.0;
           }
        }
        else
        {
           if (gamma < -0.5)
              *end = max_d(min_d(600.0 * pow(delta, 1.0 / gamma) *
                     pow(5.0, gamma), 120.0), 1.0);
           else
              *end = 12.0;
           if ((gamma == 0.0) && (cq != 0.0)) *end = 40.0;
        }
     }
     else
     {

/*   Test for finite irregular singular points.                        */

        sgn = 1.0;
        i = (int) (er - ep + sign(0.5, er - ep));
        k = (int) (eq - ep + sign(0.5, eq - ep));
        irreg = TRUE;
        if (cq == 0.0)
        {
           if ((i > -3) && (fabs(er - ep - i) <= tol4)) irreg = FALSE;
        }
        else
        {
           if (((er <= eq) && (i > -3) && (fabs(er - ep - i) <= tol4)) ||
              ((er > eq) && (k > -3) && (fabs(eq - ep - k) <= tol4))) 
              irreg = FALSE;
        }
        g_emu[iend] = 0.5 * (1.0 - ep);
        if (irreg)
        {
           if (g_print > 2)
              fprintf(output_file,
                      " This is an irregular singular point.\n");
           kclass[iend] = 5;
        }
        else
        {

/*         Compute the principal Frobenius root.                       */

           if (g_eta[iend][0] != 0.0)
           {
              if ((cq != 0.0) && (er > eq) && (k == -2))
              {
                 g_pnu[iend] = g_emu[iend] * g_emu[iend] + cq / cp;
                 if (fabs(g_pnu[iend]) <= tol4) g_pnu[iend] = 0.0;
                 if (g_pnu[iend] >= 0.0)
                    g_pnu[iend] = g_emu[iend] + sqrt(g_pnu[iend]);
                 else
                    g_pnu[iend] = -ep;
              }
              else
                 g_pnu[iend] = max_d(1.0 - ep,0.0);
           }
           if ((g_pnu[iend] > -ep) && (g_print > 4))
           fprintf(output_file,
                   " The principal Frobenius root is %20.8e\n", 
                   g_pnu[iend]);
        }
     }
     if (sgn * g_eta[iend][0] > 0.0)
     {

/*   Carry out the Case 1 tests.                                      */

        k = 0;
        if (g_eqlnf[iend] < -2.0) k = (int) (sign(1.0, -cq));
        if (g_eqlnf[iend] == -2.0)
        {
           if (fabs(c1 + c2 + 0.25) <= tol4)
           {
              if (g_print > 2)
                 fprintf(output_file,
   " WARNING: borderline nonoscillatory/oscillatory classification.\n");
              k = -1;
              flag = 1;
           }
           else
           {
              if (c1 + c2 < -0.25 - tol4) k = 1;
              if (c1 + c2 > -0.25) k = -1;
           }
        }
        if (g_eqlnf[iend] > -2.0)
        {
           if (fabs(c2 + 0.25) <= tol4)
           {
              c2 = -0.25;
              if (g_print > 2)
                 fprintf(output_file,
    " WARNING: borderline nonoscillatory/oscillatory classification.\n");
              flag = 1;
           }
           if (c2 >= -0.25) k = -1;
        }
        if (k == 1)
           g_osc[iend] = TRUE;
        else
        {
           if (k == -1)
              g_osc[iend] = FALSE;
           else
              if (g_print > 2)
              fprintf(output_file,
              " NO INFORMATION on osc/nonosc class.\n");
        }
        k = 0;
        if (g_eqlnf[iend] < -2.0) k = (int) (sign(1.0, -cq));
        if (g_eqlnf[iend] == -2.0)
        {
           if (fabs(c1 + c2 - 0.75) <= tol4)
           {
              k = -1;
              if (g_print > 3)
                 fprintf(output_file,
                 " WARNING: borderline Lc/Lp classification\n");
              flag = 1;
           }
           if (c1 + c2 >= 0.75) k = -1;
           if (fabs(c1 + c2) < 0.75 - tol4) k = 1;
           if (c1 + c2 < -tol4) k = 1;
        }
        if (g_eqlnf[iend] > -2.0)
        {
           if (fabs(c2 - 0.75) <= tol4)
           {
              k = -1;
              if (g_print > 2)
                 fprintf(output_file,
                 " WARNING: borderline Lc/Lp classification\n");
              flag = 1;
           }
           if (c2 >= 0.75) k = -1;
           if (fabs(c2) < 0.75 - tol4) k = 1;
           if (c2 < -tol4) k = 1;
        }
        if (k == 1)
           g_lc[iend] = TRUE;
        else
           if (k == -1)
              g_lc[iend] = FALSE;
           else
              fprintf(output_file,
              " NO INFORMATION on Lp/Lc class.\n");
     }
     if (sgn*g_eta[iend][0] < 0.0)
     {

/*   Carry out the Case 2 tests.                                      */

        k = 0;
        if ((g_eqlnf[iend] > 0.0) && (cq < 0.0)) k = 1;
        if ((g_eqlnf[iend] > 0.0) && (cq > 0.0)) k = -1;
        if (g_eqlnf[iend] == 0.0)
        {
           k      = -1;
           *cev   = cq / cr;
           *cspec = TRUE;
           if (g_u * fabs(*cev) >= 1.0) *cspec = FALSE;
        }
        if (g_eqlnf[iend] < 0.0)
        {
           k      = -1;
           *cev   = 0.0;
           *cspec = TRUE;
        }
        if (k == 1)
           g_osc[iend] = TRUE;
        else
        {
           if (k == -1)
              g_osc[iend] = FALSE;
           else
              fprintf(output_file, " NO INFORMATION on Osc/Nonosc class.\n");
        }
        k = 0;
        if ((g_eqlnf[iend] > 2.0) && (cq > 0.0)) k = -1;
        if (g_eqlnf[iend] <= 2.0) k = -1;
        if ((g_eqlnf[iend] > 2.0) && (cq < 0.0)) k = 1;
        if (k == 1)
           g_lc[iend] = TRUE;
        else
        {
           if (k == -1)
              g_lc[iend] = FALSE;
           else
              fprintf(output_file, " NO INFORMATION on Lp/Lc class.\n");
        }
     }
     if (g_eta[iend][0] == 0.0)
     {

/*   Carry out the Case 3 and 4 tests.                                 */

        if ((sgn * (eq - er) < 0.0) && (cq < 0.0))
        {
           g_osc[iend] = TRUE;
           g_lc[iend]  = TRUE;
        }
        if ((sgn * (eq - er) < 0.0) && (cq > 0.0))
        {
           g_osc[iend] = FALSE;
           g_lc[iend]  = FALSE;
        }
        if (eq == er)
        {
           g_osc[iend] = FALSE;
           g_lc[iend]  = FALSE;
           *cev      = cq / cr + c3;
           *cspec    = TRUE;
           if (g_u * fabs(*cev) >= 1.0) *cspec = FALSE;
        }
        if ((sgn * (eq - er) > 0.0) || (cq == 0.0))
        {
           g_osc[iend] = FALSE;
           g_lc[iend]  = FALSE;
           *cev = c3;
           *cspec = TRUE;
           if (g_u * fabs(*cev) >= 1.0) *cspec = FALSE;
        }
     }
     if (fabs(*cev) <= tol4) *cev = 0.0;

/*   Calculate the Friedrichs boundary condition (if appropriate).     */

     if (*cspec)
     {
        bc[0] = 1.0;
        bc[1] = 0.0;
     }
     if ((!(*cspec)) && (!g_osc[iend]))
     {
        if ((sgn * (er + ep) > 0.0) && (sgn * (eq + ep) > 0.0))
        {
           bc[0] = 0.0;
           bc[1] = 1.0;
        }
        else
        {
           if ((sgn * (er + ep) > 0.0) && (eq + ep == 0.0))
           {
              bc[0] = sqrt(cp * fabs(cq));
              bc[1] = 1.0;
              if (bc[0] > 1.0)
              {
                 bc[1] = 1.0 / bc[0];
                 bc[0] = 1.0;
              }
           }
           else
           {
              if ((sgn * (er + ep) < 0.0) || (sgn * (eq + ep) < 0.0))
              {
                 bc[0] = 1.0;
                 bc[1] = 0.0;
              }
           }
        }
     }
     if (!g_osc[iend])
     {
        if ((!endfin) && (g_eqlnf[iend] == -1.0)) kclass[iend] = 3;
        i = (int) (er - ep);
        if (cq != 0.0)
        {
           k = (int) (eq - ep);
           if (cq > 0.0)
           {
              if (k < i) i = 0;
           }
           else
              i = min_i(i, k);
        }
        if (endfin && (i < 0))
        {
           if (irreg) kclass[iend] = 6;
           if (g_eta[iend][0] > 0.0)
           {

/*         Transform some nonoscillatory problems for which Tau
           is unbounded near a finite singular endpoint.               */

              if (irreg) kclass[iend] = 10;
              g_cp[iend] = cp;
              g_cr[iend] = cr;
              g_ep[iend] = ep;
              g_er[iend] = er;
              g_emu[iend] = 0.0;
              g_d[iend][0] = pow(g_eta[iend][0] * sqrt(cp / cr),
                             1.0 / g_eta[iend][0]);
              g_d[iend][1] = 1.0 / sqrt(sqrt(cp * cr * pow(g_d[iend][0],
                             ep + er)));
              if ((g_eqlnf[iend] == -2.0) || (c1 == 0.0))
              {
                 if (!irreg) kclass[iend] = 9;
                 g_emu[iend] = fabs(0.25 + c1 + c2);
                 if (g_emu[iend] < tol4)
                    g_emu[iend] = 0.5;
                 else
                    g_emu[iend] = 0.5 + sqrt(g_emu[iend]);
              }
              else
              {
                 if (!irreg) kclass[iend] = 8;
              }
              if (kclass[iend] < 10)
              {
                 g_eta[iend][1] = g_emu[iend] - 0.25 * (ep + er) /
                                  g_eta[iend][0];
              }
              else
              {
                 if (g_emu[iend] == 0.0)
                 {
                    g_eta[iend][1] = 0.5 * (1.0 - ep) / g_eta[iend][0];
                    g_emu[iend] = g_eta[iend][1] + 0.25 * (ep + er) /
                                  g_eta[iend][0];
                 }
              }
              g_d[iend][2] = g_eta[iend][1] * (g_eta[iend][1] + (ep - 1.0)
                             / g_eta[iend][0]);
              g_d[iend][3] = g_d[iend][2];
              if (g_eqlnf[iend] == -2.0) g_d[iend][3] -= c1;
              if (fabs(g_d[iend][3]) <= tol4) g_d[iend][3] = 0.0;
              g_d[iend][3] = sqrt(fabs(g_d[iend][3]));
              if (g_print > 4)
              {
                 fprintf(output_file, " Mu = %13.6e  Eta2 = %13.6e\n",
                         g_emu[iend], g_eta[iend][1]);
                 fprintf(output_file, " D3 = %13.6e  D4 = %13.6e\n",
                         g_d[iend][2], g_d[iend][3]);
              }
           }
           if (kclass[iend] > 8)
           {
              if (!ex)
                 g_lflag[4] = TRUE;
              if (g_print > 2)
              {
                 fprintf(output_file,
                   " This problem has unbounded [Ev*r(x)-q(x)]/p(x).\n");
                 if ((g_emu[iend] > 0.0) || (ep + er != 0.0))
                 fprintf(output_file,
            " A change of variables will be used near this endpoint.\n");
              }
           }
        }
        if (endfin && (!g_reg[iend]) && (kclass[iend] == 0)) 
           kclass[iend] = 4;
     }
     if ((posc || qosc || rosc) && (!endfin)) *end = 99.0;
     if (g_print > 4)
        fprintf(output_file, " Classification type (KCLASS) is: %2d\n",
                kclass[iend]);
     return(flag);
}

/***********************************************************************/

int density_function(double *tol, logical *cspec, int iter, int ndim,
                     int numt, double *t, double *rho, int *nev,
                     long int *numev)
{
/************************************************************************
*                                                                       *
*     This routine computes the spectral density function Rho(t).       *
*                                                                       *
*************************************************************************

    Input parameters:
      tol[] as in SLEDGE.
      cspec[]   = logical 2-vector; cspec[i] = TRUE iff endpoint i
                  (0 = A, 1 = B) generates continuous spectrum.
      iter      = iteration number from get_rho.
      ndim      = upper bound on the total number of computed evhat.
      numt      = integer equalling number of t[] points.
      t[]       = real vector of abcissae for spectral function Rho(t).

    Output parameters:
      rho[]     = real vector of values for spectral density function
                  Rho(t),  rho[i] = Rho(t[i]).
     *nev       = integer pointer to eigenvalue.  On a normal return
                  (flag = 0) this is set to the index of the last
                  eigenvalue computed; if flag is not zero, then nev
                  gives the index of the eigenvalue where the problem
                  occurred.
     *numev     = cumulative number of eigenvalues computed.
-----------------------------------------------------------------------
     The definition of a spectral density function assumes a certain
     normalization on the eigenfunctions.  For the case when x = b
     generates the continuous spectrum, the normalization used here
     (and in function get_rn below) is:
     (1) when x = a is regular  u(a) = (a2-a2p*Ev)/scale
                                (pu')(a) = (a1-a1p*Ev)/scale,
         with scale = sqrt(a1**2+a2**2) when a1' = a2' = 0, and
              scale = sqrt(alpha) otherwise.
     (2) When x = a is a regular singular point then u(x) is taken to
         be asymptotic to the principal Frobenius solution, i.e.,
         near x = a   u(x) ~ (x-a)**Nu    with Nu the larger
         root of the indicial equation.
     Analogous normalizations hold at x = b when the endpoint x = a
     generates the continuous spectrum.
------------------------------------------------------------------------
     Local variables:                                                  */

     int flag, i, iasymp, icut, j, klvl, loop, lprint, nevmax, nrho,
         nsave = 200;
     logical done, ef_fin[2], gap, jump, pdef_fin[2], rdone, test_gap,
         test_jump;
     double abserr, alpha, alpha1, alpha2, aev, arn, avgdev, avgrho, 
         beta1, beta2, big, delta, denom, dl, dr, dx, efnorm, error, 
         eta, ev, evhat, evhigh, evlow, evsave, flow, fhigh, h, halfh,
         pdu, px, qint, qlnf, qx, relerr, rhat, rhocut, rhosum, rpata,
         rpatb, rpint, rx, scale, sqrtrp, tenu, tol1, tol2, tolmax,
         tolmin = 5.e-3, ux, xleft, xtol, z, zabs, zrel,
/*       *allev,*allrn,*drho,**drhoex,*evold,*lef,*lpdef,**rhoex,*scal;    */
         allev[500], allrn[500], *drho, **drhoex, evold[200], *lef,
         *lpdef, **rhoex, *scal;

/*   Initialization:                                                   */

     ndim = 500;   /*  remove this!   */
/*   allev = vector_d(ndim);     */
/*   allrn = vector_d(ndim);     */
     drho = vector_d(numt);
     drhoex = matrix_d(numt,g_maxlvl+2);
/*   evold = vector_d(nsave);   */
     lef   = vector_d(g_nxinit);
     lpdef = vector_d(g_nxinit);
     rhoex = matrix_d(numt,g_maxlvl+2);
     scal  = vector_d(g_nxinit);

/*     If you want to omit the test for gaps, set TGAP = FALSE;    */
/*     if you want to omit the test for jumps, set TJUMP = FALSE   */

     test_gap = TRUE;
     test_jump = TRUE;

     tenu = 10.0 * g_u;
     big = 1.0 / g_u;
     avgrho = big;
     lprint = min_i(g_print, 3);
     nevmax = 0;
     if (g_reg[0])
     {
        dx = sqrt(g_u) * max_d(1.0, fabs(g_a));
        coeff(g_a + dx, &px, &qx, &rx);
        rpata = rx * px;
        alpha1 = 1.0 / rx;
        coeff(g_a + 2.0 * dx, &px, &qx, &rx);
        alpha1 = alpha1 * (rpata - px * rx) / (dx * 4.0);
        rpata = sqrt(rpata);
        alpha2 = sqrt(rpata);
        alpha1 = (g_a1 + g_a2 * alpha1) / alpha2;
        alpha2 *= g_a2;
        g_ncoeff += 2;
     }
     else
     {
        alpha1 = 0.0;
        alpha2 = 1.0;
        rpata = 1.0;
     }
     if (g_reg[1])
     {
        dx = sqrt(g_u) * max_d(1.0, fabs(g_b));
        coeff(g_b - dx, &px, &qx, &rx);
        rpatb = rx * px;
        beta1 = 1.0 / rx;
        coeff(g_b - 2.0 * dx, &px, &qx, &rx);
        beta1 *= (rpatb - px * rx) / (dx * 4.0);
        rpatb = sqrt(rpatb);
        beta2 = sqrt(rpatb);
        beta1 = (g_b1 - g_b2 * beta1) / beta2;
        beta2 *= g_b2;
        g_ncoeff += 2;
     }
     else
     {
        beta1 = 0.0;
        beta2 = 1.0;
     }
     alpha = g_a1p * g_a2 - g_a1 * g_a2p;
     if (cspec[1])
     {
        if (alpha == 0.0)
           scale = sqrt(g_a1 * g_a1 + g_a2 * g_a2);
        else
           scale = sqrt(alpha);
     }
     if (cspec[0]) scale = sqrt(g_b1 * g_b1 + g_b2 * g_b2);
     tolmax = max_d(tol[0], tol[1]);
     tol1 = min_d(tol[0], tolmin);
     tol2 = min_d(tol[1], tolmin);
     abserr = tol1;
     relerr = tol2;
     klvl = 1;
     delta = 0.5;
     eta = t[numt] + 1.0;
     for (g_level = 0; g_level <= g_maxlvl; g_level++)
     {

/*   Begin the Main loop over LEVEL.                                  */

        if (g_print > 1)
        {
           fprintf(output_file, " Level = %3d, Iteration = %3d\n",
                   g_level, iter);
           printf(" Level = %3d, Iteration = %3d\n", g_level,iter);
        }
        iasymp = 0;
        *nev = 0;
        evsave = -big;

/*      Compute integrals needed in asymptotic formulas.              */

        qint = 0.0;
        rpint = 0.0;
        for (i = 1; i < g_nxinit; i++)
        {
            xleft = x[i-1];
            h = (x[i] - xleft) / klvl;
            halfh = 0.5 * h;
            for (j = 0; j < klvl; j++)
            {
                z = xleft + halfh;
                xleft += h;
                flag = pqrint(z, &sqrtrp, &qlnf);
                if (flag < 0) goto exit;
                qint += h * qlnf;
                rpint += h * sqrtrp;
            }
        }
        if (qint > 1.0 / g_u) qint = 0.0;
        zabs = max_d(min_d(abserr / 100.0, relerr) / 10.0, tenu);
        zrel = relerr / 10.0;
        delta = max_d(delta / 6.0, tol1 + tol2);
        loop = 0;

/*      Begin the secondary loop over nev.                            */

        while (loop < 2)
        {
           if (iasymp > 1)
           {
              aev = asymp_ev(*nev, qint, rpint, alpha1, alpha2,
                    beta1, beta2);
              evhat = aev;
              if (iasymp > 3)
                 rhat = asymp_r(*nev, rpint, rpata, rpatb, scale);
           }
           else
           {
              if (g_hmin / klvl <= tenu) return(-8);
              if ((g_level > 0) && (*nev < min_i(nevmax, nsave)))
              {
                 ev = max_d(0.5 * tol1, max_d(delta, 0.5 * tol2 *
                      fabs(evold[*nev])));
                 evlow = evold[*nev] - ev;
                 evhigh = evold[*nev] + ev;
              }
              else
              {
                 if (g_level == 0)
                    ev = 0.0;
                 else
                    ev = asymp_ev(*nev, qint, rpint, alpha1, alpha2, beta1,
                         beta2);
                 eta = max_d(0.5 * tol1, max_d(delta, 0.5 * tol2 
                       * fabs(ev)));
                 evlow = ev - eta;
                 evhigh = ev + eta;
              }
              flag = bracket(*nev, &evlow, &evhigh, &flow, &fhigh,
                     zabs, zrel);
              if ((g_level == 0) && (*nev == 0)) delta = evhigh - evlow;
              if (flag < 0) goto exit;
              if (fabs(evhigh - evlow) > max_d(zabs, zrel * fabs(evhigh)))
                 flag = zero_calc(&evlow, &evhigh, &flow, &fhigh, 
                        zabs, zrel);
              evhat = min_d(evlow, evhigh);
              if (flag < 0) goto exit;
              i = g_print;
              g_print = lprint;
              flag = get_ef(evhat, &efnorm, x, ef_fin, pdef_fin, lef,
                     lpdef, scal);
              g_print = i;
              if (flag < 0) goto exit;
              if (cspec[1])
              {
                 if (g_reg[0] || (g_pnu[0] == 0.0) || (g_pnu[0] == 
                                  1.0 - g_ep[0]))
                 {
                    ux = fabs(lef[0]);
                    pdu = fabs(lpdef[0]);
                    if (g_a2 - g_a2p * evhat != 0.0)
                       denom = scale * ux / fabs(g_a2 - g_a2p * evhat);
                    else
                       denom = scale * pdu/fabs(g_a1 - g_a1p * evhat);
                 }
                 else
                 {
                    h = x[1] - x[0];
                    ux = fabs(lef[1]);
                    pdu = fabs(lpdef[1]);
                    if (ux >= pdu)
                       denom = ux / pow(h, g_pnu[0]);
                    else
                       denom = pdu / (g_cp[0] * fabs(g_pnu[0]) *
                               pow(h, g_ep[0] + g_pnu[0] - 1.0));
                 }
              }
              else
              {
                 if (g_reg[1] || (g_pnu[1] == 0.0) ||
                     (g_pnu[1] == 1.0 - g_ep[1]))
                 {
                    ux = fabs(lef[g_nxinit-1]);
                    pdu = fabs(lpdef[g_nxinit-1]);
                    if (g_b2 != 0.0)
                       denom = scale * ux / fabs(g_b2);
                    else
                       denom = scale * pdu / fabs(g_b1);
                 }
                 else
                 {
                    h = x[g_nxinit-1] - x[g_nxinit-2];
                    ux = fabs(lef[g_nxinit-2]);
                    pdu = fabs(lpdef[g_nxinit-2]);
                    if (ux >= pdu)
                       denom = ux / pow(h, g_pnu[1]);
                    else
                       denom = pdu / (g_cp[1] * fabs(g_pnu[1]) *
                               pow(h, g_ep[1] + g_pnu[1] - 1.0));
                 }
              }
              if (big * denom >= 1.0)
                 efnorm = 1.0 / (denom * denom);
              else
                 efnorm = 1.0 / (g_u * g_u);

/*            Test for asymptotic ev.                                  */

              aev = asymp_ev(*nev, qint, rpint, alpha1, alpha2,
                    beta1, beta2) ;
              if (10.0 * fabs(aev - evhat) > max_d(abserr, relerr *
                                                   fabs(evhat)))
                 iasymp = 0;
              else
              {
                 if (iasymp < 1)
                    iasymp = 1;
                 else
                 {
                    if (g_print > 1)
                    {
                       printf(
            " Switchover to asymptotic eigenvalues at Nev = %8d\n", *nev);
                       fprintf(output_file,
            " Switchover to asymptotic eigenvalues at Nev = %8d\n", *nev);
                    }
                    iasymp = 2;
                 }
              }
              if (efnorm < big)
                 rhat = 1.0 / (alpha + efnorm);
              else
                 rhat = 0.0;
           }
           if (iasymp < 4)
           {

/*            Test for asymptotic rhat.                               */

              arn = asymp_r(*nev, rpint, rpata, rpatb, scale);
              if (iasymp > 1)
              {

/*            Eigenvalues from asymptotic formulas; produce current rhat. */

                 flag = get_r(aev, alpha, cspec, scale, &rhat);
                 if (flag < 0) goto exit;
                 if (fabs(arn - rhat) > max_d(abserr / 100.0, relerr * arn))
                    iasymp = 2;
                 else
                 {
                    if (iasymp < 3)
                       iasymp = 3;
                    else
                    {
                       if (g_print > 1)
                       {
                          printf(
                " Switchover to asymptotic Rhat at Nev = %8d\n",*nev);
                          fprintf(output_file,
                " Switchover to asymptotic Rhat at Nev = %8d\n",*nev);
                       }
                       iasymp = 4;
                    }
                 }
              }
           }
           if (*nev < nsave) evold[*nev] = evhat;

/*         Output requested information.                               */

           if (g_print > 2)
           {
              if (((*nev < 26) || ((iasymp < 2) && (g_print > 4))) ||
                 ((*nev < 100) && (10*(*nev/10) == *nev)) ||
                 ((*nev < 1000) && (100*(*nev/100) == *nev)) ||
                 ((1000*(*nev/1000) == *nev)))
              {
                 printf(" Nev = %7d, EvHat =%14.6e, RHat =%14.6e\n",
                 *nev, evhat, rhat);
                 fprintf(output_file, 
                 " Nev = %7d, EvHat =%14.6e, RHat = %14.6e\n",
                 *nev, evhat, rhat);
              }
           }

/*         Store the evhat and rhat values for later interpolation.    */

           if (*nev >= ndim)
           {
              flag = -50;
              goto exit;
           }
           allev[*nev] = evhat;
           allrn[*nev] = rhat;
           ++(*nev);
           eta = 0.5 * (evsave + evhat);
           evsave = evhat;
           if (eta > t[numt-1]) ++loop;

/*  End of secondary loop over nev ----------------------------------  */

        }
        nevmax = *nev;
        *numev += *nev;
        if (g_print > 2)
        {
           fprintf(output_file, " MaxNev = %8d\n",nevmax);
           printf(" MaxNev = %8d\n", nevmax);
        }
        rhocut = 0.0;
        icut = -1;
        i = 0;
        while ((i < nevmax) && (g_cutoff > allev[i]))
        {
           rhocut += allrn[i];
           icut = i;
           ++i;
        }

/*      Scan for gaps and jumps in the continuous spectrum.     */

        if (g_print > 0)
        {
           if (test_gap)
           {
              gap = FALSE;
              for (i = 0; i < nevmax - 1; i++)
              {
                 if ((allev[i] >= g_cutoff) && (i > 2))
                 {
                    avgdev = (allev[i] - allev[i-3]) / 3.0;
                    if (!gap)
                    {
                       if (allev[i+1] - allev[i] > 4.0 * avgdev)
                       {
                          gap = TRUE;
                          fprintf(output_file,
             "Possible gap in continuous spectrum %16.10e to %16.10e\n",
                          allev[i], allev[i+1]);
                          printf(
             "Possible gap in continuous spectrum %16.10e to %16.10e\n",
                          allev[i], allev[i+1]);
                       }
                       else
                       {
                          if ((i > 2) && test_jump)
                          {
                             avgrho = (allrn[i-1] + allrn[i-2] + 
                                      allrn[i-3]) / 3.0;
                             rhat = allrn[i];
                             if (rhat > max_d(10.0 * tolmax, 6.0 * avgrho))
                             {
                                g_lflag[2] = TRUE;
                                fprintf(output_file,
    " Large jump in the step spectral density function at %17.10e\n",evhat);
                                printf(
    " Large jump in the step spectral density function at %17.10e\n",evhat);
                                fprintf(output_file,
    "                  Iteration = %2d; Level = %2d; Jump = %17.10e\n",
                                iter, g_level, rhat);
                                printf(
    "                  Iteration = %2d; Level = %2d; Jump = %17.10e\n",
                                iter, g_level, rhat);
                             }
                          }
                       }
                    }
                    else
                       gap = FALSE;
                 }
              }
           }
        }
        gap = FALSE;
        jump = FALSE;

/*      Interpolation block ---------------------------------------- */

        rhosum = 0.0;
        nrho = 0;
        dl = 0.0;
        for (i = 0; i <nevmax-1; i++)
        {
next:      if (t[nrho] <= g_cutoff)
           {

/*            Use step functions for rho[t] below the cutoff.      */

              if (t[nrho] <= allev[i])
              {
                 drho[nrho] = 0.0;
                 rho[nrho] = rhosum;
                 ++nrho;
                 if (nrho == numt) break;
                 goto next;
              }
              else
                 rhosum += allrn[i];
           }
           else
           {
              while ((t[nrho] <= allev[i]) && (nrho <= numt))
              {
                 if (i != icut + 1)
                 {
/*                  General: Ev(i-1) .le. T(NRho) .le. Ev(i)
                    Use piecewise linear approx. with breakpoints at
                    eigenvalues for each of rho and rho' (unless there
                    is a gap or a jump).                            */
 
                    if (!test_gap || !gap)
                    {
                       dr = 2.0 * allrn[i] / (allev[i+1] - allev[i-1]);
                       denom = allev[i] - allev[i-1];
                       drho[nrho] = (dr * (t[nrho] - allev[i-1])+
                                     dl * (allev[i] - t[nrho])) / denom;
                       rho[nrho] = rhosum + drho[nrho] * (t[nrho] 
                                   - allev[i-1]);
                    }
                    else
                    {
                       if (gap)
                       {
                          dr = 0.0;
                          drho[nrho] = 0.0;
                          rho[nrho] = rhosum + 0.5 * (allrn[i-1] + allrn[i]);
                       }
                    }
                 }
                 else
                 {

/*                  Transition: Cutoff <= t[nrho] <= Ev[i]          */

                    denom = allev[i+1] - g_cutoff;
                    dr = 2.0 * allrn[i] / denom;
                    drho[nrho] = dr * (t[nrho] - g_cutoff) / 
                                 (allev[i] - g_cutoff);
                    rho[nrho] = rhocut + drho[nrho] * (t[nrho] - g_cutoff);
                 }
                 ++nrho;
                 if (nrho == numt) break;
              }
              if (nrho == numt) break;
              if (t[nrho] > allev[i])
              {
                 if (i != icut+1)
                    dr = 2.0 * allrn[i] / (allev[i+1] - allev[i-1]);
                 else
                    dr = 2.0 * allrn[i] / (allev[i+1] - g_cutoff);
              }
              if (i != icut+1)
              {
                 if (!jump)
                    rhosum += 0.5 * (allrn[i-1] + allrn[i]);
                 else
                    rhosum += rhat;
              }
              else
                 rhosum = rhocut + 0.5 * allrn[i];
              dl = dr;
              gap = FALSE;
              jump = FALSE;
              if (i > 2)
              {
                 if (test_gap)
                 {
                    avgdev = (allev[i] - allev[i-3]) / 3.0;
                    if (allev[i+1] - allev[i] > 4.0 * avgdev)
                       gap = TRUE;
                    else
                       gap = FALSE;
                 }
                 if (test_jump)
                 {
                    avgrho = (allrn[i] + allrn[i-1] + allrn[i-2]) / 3.0;
                    rhat = allrn[i+1];
                    if (rhat > max_d(10.0 * tolmax, 6.0 * avgrho))
                    {
                       jump = TRUE;
                       allrn[i+1] = 0.5 * (allrn[i] + allrn[i+2]);
                    }
                 }
              }
           }
        }
        if (g_print > 2)
        {
           fprintf(output_file,
"         t               Interp. Rhohat(t)              Interp. Rhohat'(t)\n");
           for (j = 0; j < numt; j++)
               fprintf(output_file, " %12.4f %31.15e %31.15e\n",
               t[j], rho[j], drho[j]);
        }

/*     End of interpolation block -----------------------------------  */

/*     Extrapolate interpolated approximations.                        */

        done = TRUE;
        for (j = 0; j < numt; j++)
        {
           xtol = max_d(tol1, fabs(rho[j]) * tol2);
           rdone = extrapolation(&rho[j], xtol, g_level + 1, g_nextrp,
                   TRUE, FALSE, 1, &rhoex[j][0], &error);
           if (rho[j] < 0.0) rho[j] = 0.0;
           if (j > 0) rho[j] = max_d(rho[j], rho[j-1]) ;
           if ((g_level > 1) && (error <= 0.5 * xtol)) rdone = TRUE;
           if (done && rdone)
              done = TRUE;
           else
              done = FALSE;
           rdone = extrapolation(&drho[j], xtol, g_level + 1, g_nextrp,
                   TRUE, FALSE, 1, &drhoex[j][0], &error);
           if (drho[j] < 0.0) drho[j] = 0.0;
        }
        if (g_print > 1)
        {
           fprintf(output_file,
"         t               Truncated Rho(t)               Truncated Rho'(t)\n");
           printf(
"         t               Truncated Rho(t)               Truncated Rho'(t)\n");
           for (j = 0; j < numt; j++)
           {
               fprintf(output_file, " %12.4f %31.15e %31.15e\n",
                    t[j], rho[j], drho[j]);
               printf(" %12.4f %31.15e %31.15e\n", t[j], rho[j], drho[j]);
           }
        }
        if (done) goto exit;
        abserr = max_d(0.5 * abserr, tenu);
        relerr = max_d(0.5 * relerr, tenu);
     }
     flag = -3;
exit:
/*   free(allev);  */
/*   free(allrn);  */
     free(drho);
     free(drhoex);
/*   free(evold);  */
     free(rhoex);
     free(lef);
     free(lpdef);
     free(scal);
     return(flag);
}

/***********************************************************************/

void description(logical *lplc, logical *cspec, double *cev, 
     long int lastev)
{
/*   input: lplc[], cspec[], cev[], lastev;
     output (if requested) a description of the spectrum.              */

     char s0[] = {"The spectral category is"},
     s1[] = {"This problem has simple spectrum."},
     s2[] = {"This problem may have non-simple spectrum."},
     s3[] = {"There is no continuous spectrum."},
     s4[] = {"There is continuous spectrum in [Ev, infinity) where Ev ="},
s5[] = {"There is continuous spectrum consisting of the entire real line."},
     s6[] = {"The set of eigenvalues is bounded below."},
s7[] = {"There are infinitely many negative and infinitely many positive"},
     s8[] = {"eigenvalues (unbounded in either direction)."},
     s9[] ={"There appear to be no "},
s10[] = {"There appears to be 1 eigenvalue below the start of the continuous spectrum."},
     s11[] = {"There appear to be"},
     s12[] = {"eigenvalues below the start of the continuous spectrum."},
     s13[] = {"At endpoint A"},
     s14[] = {"At endpoint B"},
     s15[] = {"   The problem is regular,"},
     s16[] = {"   The problem is singular,"},
     s17[] = {"   It is nonoscillatory for all Ev."},
     s18[] = {"   It is oscillatory for all Ev."},
     s19[] = {"   It is nonoscillatory for Ev <"},
     s20[] = {"and oscillatory otherwise."},
     s21[] = {"   It is limit circle."},
     s22[] = {"   It is limit point."},
s23[] = {"   The constants for the Friedrichs boundary conditions are:"},
     s24[] = {"There appear to be infinitely many eigenvalues below the "},
     s25[] = {"start of the continuous spectrum."},
     s26[] = {"There are infinitely many eigenvalues, bounded below."},
s27[] = {"The nature of the spectrum is unknown, there is likely to be"};
     fprintf(output_file, "\n");
 
     if (ctype[0][2] && ctype[1][2])
     {

/*      Category 1                                                     */

        fprintf(output_file, " %s 1\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s\n", s26);
        fprintf(output_file, "  %s\n", s3);
        fprintf(output_file, " %s\n", s13);
        if (g_reg[0])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           fprintf(output_file, " %s\n", s17);
           if (lplc[0])
           {
              if (g_lc[0])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
           fprintf(output_file, " %s\n%18.8e %18.8e %18.8e %18.8e\n", s23,
                   g_a1, g_a1p, g_a2, g_a2p);
        }
        fprintf(output_file, " %s\n", s14);
        if (g_reg[1])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           fprintf(output_file, " %s\n", s17);
           if (lplc[1])
           {
              if (g_lc[1])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
           fprintf(output_file, " %s\n%18.8e %18.8e\n", s23, g_b1, g_b2);
        }
     }
     if ((ctype[0][2] && cspec[1]) || (ctype[1][2] && cspec[0]))
     {

/*      Category 2                                                    */

        fprintf(output_file, " %s 2\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s %13.6e\n", s4, g_cutoff);
        if (lastev == -5)
        {
           fprintf(output_file, " %s\n", s6);
           fprintf(output_file, " %s\n %s\n", s24, s25);
        }
        else
        {
           if (lastev == 0)
              fprintf(output_file, " %s %s\n", s9, s12);
           else
           {
              if (lastev == 1)
                 fprintf(output_file, " %s\n", s10);
              else
                 fprintf(output_file, " %s %ld %s\n", s11, lastev, s12);
           }
        }
        fprintf(output_file, " %s\n", s13);
        if (g_reg[0])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (cspec[0])
              fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, " %s\n%18.8e %18.8e %18.8e %18.8e\n",
                      s23, g_a1, g_a1p, g_a2, g_a2p);
           }
           if (lplc[0])
           {
              if (g_lc[0])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
        fprintf(output_file, " %s\n", s14);
        if (g_reg[1])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (cspec[1])
              fprintf(output_file, " %s %13.6e %s\n", s19, cev[1], s20);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, " %s\n%18.8e %18.8e\n", s23, g_b1, g_b2);
           }
           if (lplc[1])
           {
              if (g_lc[1])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
     }
     if ((ctype[0][2] && ctype[1][3] && g_lc[1] && (!cspec[1])) ||
        (ctype[1][2] && ctype[0][3] && g_lc[0] && (!cspec[0])))
     {

/*      Category 3                                                    */

        fprintf(output_file, " %s 3\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s\n %s\n", s5, s6);
        fprintf(output_file, " %s\n", s3);
        fprintf(output_file, " %s\n", s13);
        if (g_reg[0])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (ctype[0][3])
              fprintf(output_file, " %s\n", s18);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, "%s\n%18.8e %18.8e %18.8e %18.8e\n",
                      s23, g_a1, g_a1p, g_a2, g_a2p);
           }
           if (lplc[0])
           {
              if (g_lc[0])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
        fprintf(output_file, " %s\n", s14);
        if (g_reg[1])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (ctype[1][3])
              fprintf(output_file, " %s\n", s18);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, " %s\n%18.8e %18.8e\n", s23,
                      g_b1, g_b2);
           }
           if (lplc[1])
           {
              if (g_lc[1])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
     }
     if ((ctype[0][2] && (!g_lc[1]) && ctype[1][3] && (!cspec[1])) ||
        (ctype[1][2] && (!g_lc[0]) && ctype[0][3] && (!cspec[0])))
     {

/*      Category 4                                                    */

        fprintf(output_file, " %s 4\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s\n", s5);
        fprintf(output_file, " %s\n", s13);
        if (g_reg[0])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (ctype[0][3])
              fprintf(output_file, " %s\n", s18);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, " %s\n%18.8e %18.8e %18.8e %18.8e\n",
                      s23, g_a1, g_a1p, g_a2, g_a2p);
           }
           if (lplc[0])
           {
              if (g_lc[0])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
        fprintf(output_file, " %s\n", s14);
        if (g_reg[1])
           fprintf(output_file, " %s\n", s15);
        else
        {
           fprintf(output_file, " %s\n", s16);
           if (ctype[1][3])
              fprintf(output_file, " %s\n", s18);
           else
           {
              fprintf(output_file, " %s\n", s17);
              fprintf(output_file, " %s\n%18.8e %18.8e\n", s23,
                      g_b1, g_b2);
           }
           if (lplc[1])
           {
              if (g_lc[1])
                 fprintf(output_file, " %s\n", s21);
              else
                 fprintf(output_file, " %s\n", s22);
           }
        }
     }
     if (g_lc[0] && ctype[0][3] && g_lc[1] && ctype[1][3])
     {

/*      Category 5                                                    */

        fprintf(output_file, " %s 5\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s\n %s\n", s7, s8);
        fprintf(output_file, " %s\n", s3);
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        fprintf(output_file, " %s\n", s21);
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        fprintf(output_file, " %s\n", s21);
     }
     if (ctype[0][3] && (!g_lc[0]) && (!cspec[0]) && ctype[1][3] &&
        (!g_lc[1]) && (!cspec[1]))
     {

/*      Category 6                                                     */

        fprintf(output_file, " %s 6\n", s0);
        fprintf(output_file, " %s\n %s\n", s27, s25);
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        fprintf(output_file, " %s\n", s22);
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        fprintf(output_file, " %s\n", s22);
     }
     if (ctype[0][3] && ctype[1][3] && (!(cspec[0]) || cspec[1]) &&
        ((g_lc[0] && (!g_lc[1])) || ((!g_lc[0]) && g_lc[1])))
     {

/*      Category 7                                                     */

        fprintf(output_file, " %s 7\n", s0);
        fprintf(output_file, " %s\n", s5);
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        if (lplc[0])
        {
           if (g_lc[0])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s\n", s18);
        if (lplc[1])
        {
           if (g_lc[1])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
     }
     if ((g_lc[0] && ctype[0][3] && cspec[1]) ||
        (g_lc[1] && ctype[1][3] && cspec[0]))
     {

/*      Category 8                                                     */

        fprintf(output_file, " %s 8\n", s0);
        fprintf(output_file, " %s\n", s1);
        fprintf(output_file, " %s %13.6e\n", s4, g_cutoff);
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        if (cspec[0])
           fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        else
           fprintf(output_file, " %s\n", s18);
        if (lplc[0])
        {
           if (g_lc[0])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        if (cspec[1])
           fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        else
           fprintf(output_file, " %s\n", s18);
        if (lplc[1])
        {
           if (g_lc[1])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
     }
     if (((!g_lc[0]) && ctype[0][3] && (!cspec[0]) && cspec[1]) ||
        ((!g_lc[1]) && ctype[1][3] && (!cspec[1]) && cspec[0]))
     {

/*      Category 9                                                     */

        fprintf(output_file, " %s 9\n", s0);
        fprintf(output_file, " %s\n", s2);
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        if (cspec[0])
           fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        else
           fprintf(output_file, " %s\n", s18);
        if (lplc[0])
        {
           if (g_lc[0])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        if (cspec[1])
           fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        else
           fprintf(output_file, " %s\n", s18);
        if (lplc[1])
        {
           if (g_lc[1])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
     }
     if (cspec[0] && cspec[1])
     {

/*      Category 10                                                   */

        fprintf(output_file, " %s 10\n", s0);
        fprintf(output_file, " %s\n", s2);
        fprintf(output_file, " %s %13.6e\n", s4, g_cutoff);
        if (lastev == -5)
           fprintf(output_file, " %s\n %s\n", s24, s25);
        else
        {
           if (lastev == 0)
              fprintf(output_file, " %s %s \n", s9, s12);
           else
           {
              if (lastev == 1)
                 fprintf(output_file, " %s\n", s10);
              else
                 fprintf(output_file, " %s %ld %s", s11, lastev, s12);
           }
        }
        fprintf(output_file, " %s\n", s13);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        if (lplc[0])
        {
           if (g_lc[0])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
        fprintf(output_file, " %s\n", s14);
        fprintf(output_file, " %s\n", s16);
        fprintf(output_file, " %s %13.6e %s\n", s19, cev[0], s20);
        if (lplc[1])
        {
           if (g_lc[1])
              fprintf(output_file, " %s\n", s21);
           else
              fprintf(output_file, " %s\n", s22);
        }
     }
     fprintf(output_file, "\n");
}

/***********************************************************************/

logical extrapolation(double *v, double tol, int jcol, int maxrow,
        logical full, logical tight, int mode, double *vsave, 
        double *error)
{
/*    Use Richardson's h**2 extrapolation (based on doubling) when
      suitable, otherwise use Wynn's acceleration scheme.

      Input:
         v       = double value at current level.
         tol     = double tolerance.
         jcol    = integer giving current column index (0 .le. JCOL).
         maxrow  = integer giving maximum number of rows in table.
         full    = logical, True iff entire table is to be computed.
         tight   = logical, True for conservative convergence tests.
         mode    = integer, value is
                   0   both Richardson and Wynn algorithms can be used;
                   1   only Richardson is used;
                   2   only Wynn is used.
      Output:
         v       = double, best output estimate.
         vsave   = double vector, holds previous level values.
         error   = double, error estimate for v.
         done    = logical, True iff Error is sufficiently small.
                   (return value)

      If full is TRUE, then the entire acceleration array is produced
      (through column jcol); if False, then only the next row is appended.
      Hence, the choice of full = TRUE requires more work, but it may
      save some global storage.  The vector vsave contains the v values
      for levels 0 through max(jcol-1,maxrow).                         */

     int i, j, jmin, maxi, nrow;
     double diff, eps, etemp, ratio[12], colhigh, collow, coltol, t, 
            tol1, tol2, vtemp;
     double static r[12][12], w[11][40];

/*   The local arrays ratio[*], r[*][*], and w[*][*] must be declared to
     have at least as many rows as the value of maxlvl+1 initialized in
     start function.                                                    */

     eps = 0.2;
     etemp = 1.0 / g_u;
     vtemp = *v;
     vsave[jcol-1] = *v;
     tol1 = tol;
     tol2 = tol / 3.0;
     if (mode != 2)
     {
        maxi = 11;
        collow = max_d(3.0, 4.4 - 0.2 * jcol);
        colhigh = min_d(5.0, 3.6 + 0.2 * jcol);
        coltol = g_u;

/* Analyze the rate of convergence to determine NROW and tolerances.   */

        nrow = 2;
        for (j = 0; j < jcol; j++)
        {
           r[0][j] = vsave[j];
           ratio[j] = 1.0 / g_u;
           if (j > 1)
           {
              t = r[0][j] - r[0][j-1];
              if (t != 0.0)
                 ratio[j] = (r[0][j-1] - r[0][j-2]) / t;
              else
              {
                 *v = r[0][j];
                 return(TRUE);
              }
              if (((ratio[j] >= collow) && (ratio[j] <= colhigh)) ||
                  (!tight))
              {
                 coltol = tol1;
                 nrow = min_i(maxrow, nrow + 1);
              }
              else
              {
                 coltol = tol2;
                 if (ratio[j] < 0.0) coltol = 0.1 * tol2;
                 if (ratio[j] < 2.0) nrow = 2;
              }
           }
        }
        if (full)
          jmin = 1;
        else
          jmin = jcol - 1;

/* Use Richardson's h^2 extrapolation.  The number of columns used
   is a function of the amount of data (jcol), the requested order
   (maxrow), the observed rate of convergence (nrow), and the amount
   of storage allocated to r[*][*].                                    */

        for (j = jmin; j < jcol; j++)
        {
           for (i = 1; i < min_i(j+1, min_i(nrow, 8)); i++)
           {
               diff = (r[i-1][j] - r[i-1][j-1]) / (pow(4.0, i) - 1.0);
               r[i][j] = r[i-1][j] + diff;
               if ((!full) || (j == jcol - 1))
               {
                  t = fabs(diff);
                  if (t <= etemp)
                  {
                     etemp = t;
                     vtemp = r[i][j];
                     if (t <= coltol)
                     {
                        *v = vtemp;
                        *error = etemp;
                        return(TRUE);
                     }
                  }
               }
            }
        }
        *v = vtemp;
        *error = etemp;
        if (jcol < 4) return(FALSE);

/* Test for rate of convergence other than second order.               */

        if (fabs(ratio[jcol-1] - ratio[jcol-2]) + fabs(ratio[jcol-2] -
            ratio[jcol-3]) > eps) return(FALSE);
        if ((3.5 < ratio[jcol-1]) && (ratio[jcol-1] < 4.5)) return(FALSE);
        if (ratio[jcol-1] < 1.0) return(FALSE);
        if (mode != 0) return(FALSE);
     }
     else
        maxi = maxrow;

/*      Use Wynn's algorithm.                                          */

     if (g_print > 3)
     {
        diff = log(fabs(ratio[jcol-1])) / log(2.0);
        fprintf(output_file, 
                " In extrap: using Wynn's acceleration; rate =%8.5e\n",
                diff);
     }
     w[0][0] = vsave[0];
     etemp = 1.0 / g_u;
     for (j = 1; j < jcol; j++)
     {
        w[0][j] = vsave[j];
        diff = w[0][j] - w[0][j-1];
        if ((j == jcol - 1) || (mode == 2))
        {
           t = fabs(diff);
           if (t <= etemp)
           {
              etemp = t;
              vtemp = w[0][j];
              if (t <= tol2)
              {
                 *v = vtemp;
                 *error = etemp;
                 return(TRUE);
              }
           }
        }
        if (diff != 0.0)
           w[1][j] = 1.0 / diff;
        else
        {
           *v = w[0][j];
           *error = 0.0;
           return(TRUE);
        }
     }
     for (i = 2; i < min_i(jcol,maxi); i++)
     {
        for (j = i; j < jcol; j++)
        {
           diff = w[i-1][j] - w[i-1][j-1];
           if (diff != 0.0)
              diff = 1.0 / diff;
           else
           {
              if (fmod(i,2) == 1)
              {
                 *v = w[i-1][j];
                 *error = 0.0;
                 return(TRUE);
              }
              else
                 diff = 1.0 / (g_u * g_u);
           }
           w[i][j] = w[i-2][j-1] + diff;
           if (fmod(i,2) == 0 && (j == jcol-1 || mode == 2))
           {
              t = fabs(diff);
              if (t <= etemp)
              {
                 etemp = t;
                 vtemp = w[i][j];
                 if (t <= tol2)
                 {
                    *v = vtemp;
                    *error = etemp;
                    return(TRUE);
                 }
              }
           }
        }
     }
     *v = vtemp;
     *error = etemp;
     return(FALSE);
}

/***********************************************************************/

int get_ef(double ev, double *efnorm, double *x, logical *ef_fin,
           logical *pdef_fin, double *lef, double *lpdef, double *scal)
{
/*      Compute an eigenfunction for one fixed mesh.
        input: ev, *efnorm, x[];
        output: *efnorm, ef_fin[], pdef_fin[], lef[], lpdef[], scal[];
        return flag: 0 -- success, > 0 -- warning, < 0 -- failure.     */

     int flag, i, j, jlast, jx, klvl, middle[2], mode;
     logical allok, symm;
     double chi, dpsi, dv, dw, fnorm, fscale, fsum, h, halfh, hom, om,
        oscale, pdv, pdw, pn, prod, psi, ratio, rn, rnorm, rscale, rsum,
        scale, t, tau, tauhh, taumax, v, vnew, w, wnew, xleft, xright, z;

     klvl = (int) pow(2, g_level);

/*   For this ev and mesh, calculate fscale, rscale, and middle[*].
         fscale = sum of logs of scale factors 1 through m
         rscale = sum of logs of scale factors m+1 through n
         middle[0], middle[1] describe the coordinates of the matching
            point M for the shooting; in particular,
            m = x[middle[0]-1] + middle[1]*h(middle[0]-1)/2**level
     The matching point is chosen to be roughly (a+b)/2 if either
     a = -b and Tau(x) > 0 near 0, or if Tau(x) > 0 for all x;
     otherwise, it is chosen to roughly maximize Tau(x).               */

     fscale = 0.0;
     rscale = 0.0;
     taumax = -1.0 / g_u;
     allok = TRUE;
     if (g_a == -g_b)
        symm = TRUE;
     else
        symm = FALSE;
     ef_fin[0] = TRUE;
     ef_fin[1] = TRUE;
     pdef_fin[0] = TRUE;
     pdef_fin[1] = TRUE;
     jx = (g_nxinit - 1) / 2;
     for (i = 1; i < g_nxinit; i++)
     {
        mode = 0;
        xleft = x[i-1];
        h = x[i] - xleft;
        if (i == 1)
        {
           if (kclass[0] > 8) mode = 1;
           if (!g_afin)
           {
              mode = 3;
              xleft = 0.0;
              h = -1.0 / x[1];
           }
        }
        if (i == g_nxinit-1)
        {
           if (kclass[1] > 8) mode = 2;
           if (!g_bfin)
           {
              mode = 4;
              h = 1.0 / x[i-1];
              xleft = -h;
           }
        }
        h /= klvl;
        halfh = 0.5*h;
        for (j = 0; j < klvl; j++)
        {
           z = xleft + halfh;
           xleft = xleft + h;
           flag = step(z, h, ev, &pn, &rn, &tau, &om, &hom, &psi, 
                       &dpsi, &scale, mode);
           if (tau < 0.0) allok = FALSE;
           if (tau > taumax)
           {
              taumax = tau;
              middle[0] = i;
              middle[1] = j;
              fscale += (rscale + scale);
              oscale = scale;
              rscale = 0.0;
           }
           else
              rscale += scale;
        }
        if ((i == jx) && (tau < 0.0)) symm = FALSE;
     }
     if (allok || symm)
     {
        middle[0] = jx;
        middle[1] = max_i(klvl - 2, 0);
     }
     if ((!g_afin) || (!g_bfin))
     {

/*      Don't split near infinity!                                    */

        if ((!g_afin) && (middle[0] == 1))
        {
           if (g_reg[1])
              middle[0] = g_nxinit - 1;
           else
              middle[0] = jx;
           middle[1] = max_i(klvl - 2, 0);
        }
        if ((!g_bfin) && (middle[0] == g_nxinit-1))
        {
           if (g_reg[0])
              middle[0] = 1;
           else
              middle[0] = jx;
           middle[1] = 0;
        }
     }
     if ((g_level > 1) && (middle[1] == klvl-1))
     {
        middle[1] = max_i(klvl - 2, 0);
        fscale -= oscale;
        rscale += oscale;
     }
     if (g_print > 3)
        fprintf(output_file,
           " Coordinates of matching point %4d %4d\n", middle[0],
           middle[1]);

/*   Shoot from x = A to the middle.                                   */

     v = g_a2 - g_a2p * ev;
     pdv = g_a1 - g_a1p * ev;
     fnorm = 0.0;
     if (kclass[0] < 9)
     {
        scale = max_d(fabs(v), fabs(pdv));
        v /= scale;
        pdv /= scale;
        mode = 0;
     }
     else
     {
        v = 1.0;
        pdv = g_d[0][3];
        mode = 1;
     }
     if (!g_afin) mode = 3;
     fsum = -fscale;
     if ((g_print > 4) && (mode == 0))
        fprintf(output_file, " %16.6e %15.6e %15.6e %15.6e\n",
                x[0], v, pdv, fsum);
     for (i = 0; i < middle[0]; i++)
     {
        lef[i] = v;
        lpdef[i] = pdv;
        scal[i] = fsum;
        if (mode == 0)
        {
           xleft = x[i];
           h = x[i+1] - xleft;
        }
        else
        {
           xleft = 0.0;
           if (mode == 1)
              h = pow((x[1] - x[0]) / g_d[0][0], g_eta[0][0]);
           else
              h = -1.0 / x[1];
        }
        h /= klvl;
        halfh = 0.5 * h;
        jlast = klvl;
        if (i == middle[0] - 1) jlast = middle[1] + 1;
        for (j = 0; j < jlast; j++)
        {
           z = xleft + halfh;
           xleft += h;
           flag = step(z, h, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi,
                       &scale, mode);
           dv = pdv / pn;
           if (fabs(tau) * h * h >= 1.e-4)
           {
              if (2.0 * (fsum + scale) > -g_under)
              {
                 fnorm += rn * (psi * dpsi * (v * v - dv * dv / tau)
                          / 2.0 + v * psi * dv * psi) * exp(2.0 * 
                          (fsum + scale));
                 if (2.0 * fsum > -g_under)
                    fnorm += rn * exp(2.0 * fsum) * h * (v * v +
                             dv * dv / tau) / 2.0;
              }
           }
           else
           {
              if (2.0 * fsum > -g_under)
              {
                 tauhh = tau * h * h;
                 fnorm += rn * h * exp(2.0 * fsum) * (v * v * (1.0 +
                          tauhh * (tauhh / 5.0 - 1.0) / 3.0) + h * v * 
                          dv * (1.0 + tauhh * (2.0 * tauhh / 15.0 - 1.0)
                          / 3.0) + (h * dv) * (h * dv) * (1.0 + tauhh
                          * (2.0 * tauhh / 21.0 - 1.0) / 5.0) / 3.0);
              }
           }
           fsum += scale;
           vnew = dpsi * v + psi * dv;
           pdv = -pn * tau * psi * v + dpsi * pdv;
           v = vnew;
        }
        if (mode == 1)
        {

/*         Convert from V(t) to u(x).                                 */

           if (g_eta[0][1] < 0.0)
           {
              lef[0] = 1.0 / g_u;
              ef_fin[0] = FALSE;
           }
           else
           {
              if (g_eta[0][1] == 0.0)
                 lef[0] = g_d[0][1];
              else
                 lef[0] = 0.0;
           }
           z = g_eta[0][0] * g_eta[0][1] + g_ep[0] - 1.0;
           if (z < 0.0)
           {
              lpdef[0] = sign(1.0 / g_u, g_eta[0][1]);
              pdef_fin[0] = FALSE;
           }
           else
           {
           if (z > 0.0)
              lpdef[0] = 0.0;
           else
              lpdef[0] = g_d[0][1] * g_cp[0] * g_eta[0][0] *
                        g_eta[0][1] / pow(g_d[0][0], g_eta[0][0] *
                        g_eta[0][1]);
           }
           h = x[1] - g_a;
           pn = g_cp[0] * pow(h, g_ep[0] - 1.0);
           t = pow(h / g_d[0][0], g_eta[0][0]);
           chi = g_d[0][1] * pow(t, g_eta[0][1]);
           v *= chi;
           pdv = pn * g_eta[0][0] * (g_eta[0][1] * v + chi * pdv *
                 pow(t, (1.0 - 2.0 * g_emu[0])));
           if (g_print > 4)
              fprintf(output_file, " %16.6e %15.6e %15.6e\n", x[0],
                      lef[0], lpdef[0]);
        }
        mode = 0;
        if (g_print > 4)
           fprintf(output_file, " %16.6e %15.6e %15.6e %15.6e\n", xleft,
                   v, pdv, fsum);
     }

/*   Shoot from x = B to the middle.                                  */

     rnorm = 0.0;
     mode =  0;
     if (kclass[1] < 9)
     {
        scale = max_d(fabs(g_b1), fabs(g_b2));
        w = -g_b2 / scale;
        pdw = g_b1 / scale;
        mode = 0;
     }
    else
    {
        w = -1.0;
        pdw = -g_d[1][3];
        mode = 2;
    }
    if (!g_bfin) mode = 4;
    rsum = -rscale;
    if ((g_print > 4) && (mode == 0))
       fprintf(output_file, " %16.6e %15.6e %15.6e %15.6e\n",
               x[g_nxinit-1], w, pdw, rsum);
    for (i = g_nxinit - 1; i >= middle[0]; i--)
    {
        lef[i] = w;
        lpdef[i] = pdw;
        scal[i] = rsum;
        if (mode == 0)
        {
           xright = x[i];
           h = xright - x[i-1];
        }
        else
        {
           xright = 0.0;
           if (mode == 2)
              h = pow((x[g_nxinit-1] - x[g_nxinit-2]) / g_d[1][0],
                       g_eta[1][0]);
           else
              h = 1.0 / x[i-1];
        }
        h /= klvl;
        halfh = 0.5 * h;
        jlast = klvl;
        if (i == middle[0])
           jlast -= (middle[1]+1);
        for (j = 0; j < jlast; j++)
        {
           z = xright - halfh;
           xright = xright - h;
           flag = step(z, h, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi,
                       &scale, mode);
           dw = pdw / pn;
           if (fabs(tau) * h * h >= 1.e-4)
           {
              if (2.0 * (rsum + scale) > -g_under)
              {
                 rnorm += rn * (psi * dpsi * (w * w - dw * dw / tau) /
                          2.0 - w * psi * dw * psi) * exp(2.0 * 
                          (rsum + scale));
                 if (2.0 * rsum > -g_under)
                    rnorm += rn * exp(2.0 * rsum) * h * (w * w + dw
                             * dw / tau) / 2.0;
              }
           }
           else
           {
              if (2.0 * rsum > -g_under)
              {
                 tauhh = tau * h * h;
                 rnorm += rn * h * exp(2.0 * rsum) * (w * w * (1.0 +
                          tauhh * (tauhh / 5.0 - 1.0) / 3.0) - h * w * 
                          dw * (1.0 + tauhh * (2.0 * tauhh / 15.0 - 1.0)
                          / 3.0) + (h * dw) * (h * dw) * (1.0 + tauhh
                          * (2.0 * tauhh / 21.0 - 1.0) / 5.0) / 3.0);
              }
           }
           rsum += scale;
           wnew = dpsi * w - psi * dw;
           pdw = pn * tau * psi * w + dpsi * pdw;
           w = wnew;
        }
        if (mode == 2)
        {

/*         Convert from V(t) to u(x).                                   */

           if (g_eta[1][1] < 0.0)
           {
              lef[g_nxinit-1] = -1.0 / g_u;
              ef_fin[1] = FALSE;
           }
           else
           {
              if (g_eta[1][1] == 0.0)
                 lef[g_nxinit-1] = -g_d[1][1];
              else
                 lef[g_nxinit-1] = 0.0;
           }
           z = g_eta[1][0] * g_eta[1][1] + g_ep[1] - 1.0;
           if (z < 0.0)
           {
              lpdef[g_nxinit-1] = sign(1.0 / g_u, g_eta[1][1]);
              pdef_fin[1] = FALSE;
           }
           else
           {
              if (z > 0.0)
                 lpdef[g_nxinit-1] = 0.0;
              else
                 lpdef[g_nxinit-1] = g_d[1][1] * g_cp[1] *
                                     g_eta[1][0] * g_eta[1][1] /
                                     pow(g_d[1][0], g_eta[1][0] *
                                     g_eta[1][1]);
           }
           if (g_print > 4)
              fprintf(output_file, " %16.6e %15.6e %15.6e\n", 
                      x[g_nxinit-1], lef[g_nxinit-1], lpdef[g_nxinit-1]);
           h = x[g_nxinit-1] - x[g_nxinit-2];
           pn = g_cp[1] * pow(h, g_ep[1] - 1.0);
           t = pow(h / g_d[1][0], g_eta[1][0]);
           chi = g_d[1][1] * pow(t, g_eta[1][1]);
           w *= chi;
           pdw = pn * g_eta[1][0] * (chi * pdw * pow(t, 1.0 - 2.0 * 
                 g_emu[1]) - g_eta[1][1] * w);
        }
        if (mode == 4)
        {
           lef[g_nxinit-1] = 0.0;
           lpdef[g_nxinit-1] = 1.0;
           if (g_print > 4)
              fprintf(output_file, " %16.6e %15.6e %15.6e\n", 
                      x[g_nxinit-1], lef[g_nxinit-1], lpdef[g_nxinit-1]);
        }
        mode = 0;
        if ((jlast != 0) && (g_print > 4))
           fprintf(output_file, " %16.6e %15.6e %15.6e %15.6e\n",
                   xright, w, pdw, rsum);
     }
     if (fabs(w) >= fabs(pdw))
     {
        ratio = v/w;
        if (g_print > 4)
           fprintf(output_file, " DuHat jump = %22.15e, ratio = %22.15e\n",
                   ratio * pdw - pdv, ratio);
     }
     else
     {
        ratio = pdv / pdw;
        if (v * w * ratio < 0.0)
        {
           ratio = -ratio;
           if (g_print > 4)
              fprintf(output_file, " UHat jump = %22.15e, ratio = %22.15e\n",
                      ratio * w - v, ratio);
        }
     }

/*   Calculate weighted 2-norm and scale approximate eigenfunction.    */

     fscale = exp(-fsum);
     rscale = exp(-rsum);
     *efnorm = sqrt(fnorm * pow(fscale, 2) + rnorm * pow(ratio * rscale, 2));
     scale = log(*efnorm);
     if (g_print > 4)
        fprintf(output_file, "   EFnorm = %22.15e\n", *efnorm);
     for (i = 0; i < g_nxinit; i++)
     {
        tau = scal[i] - scale;
        if (tau <= -g_under)
        {
           lef[i] = 0.0;
           lpdef[i] = 0.0;
        }
        else
        {
           prod = exp(tau);
           if (i >= middle[0]) prod *= ratio;
           lef[i] *= prod;
           lpdef[i] *= prod;
        }
     }
     if (g_print > 3)
     {
        fprintf(output_file,
        "         x                Uhat(x)             PUhat`(x)\n");
        for (i = 0; i < g_nxinit; i++)
           fprintf(output_file, "%16.6e %20.8e %20.8e\n", x[i],
                   lef[i], lpdef[i]);
     }
     return(flag);
}

/***********************************************************************/

int get_r(double ev, double alpha, logical *cspec, double denom,
          double *rsubn)
{
/*   Compute the RsubN value from the weighted eigenfunction 2-norm
     when standard shooting is stable and an accurate eigenvalue is
     available (from the asymptotic formulas).
     input: ev, alpha, cspec[], denom; output: *rsubn.
     return: flag                                                      */

     int flag, i, j, klvl;
     double dphi, dpsi, du, dusave, fnorm, h, halfh, hom, hsave, om, pdu, 
       phi, pn, psi, rn, scale, tau, tauhh, u, unew, usave, xleft, z;

     u = g_a2 - g_a2p * ev;
     pdu = g_a1 - g_a1p * ev;
     fnorm = 0.0;
     klvl = (int) pow(2, g_level);

/*   Shoot from x = A to x = B.                                       */

     for (i = 1; i < g_nxinit; i++)
     {
        xleft = x[i-1];
        h = (x[i] - xleft) / klvl;
        halfh = 0.5 * h;
        for (j = 0; j < klvl; j++)
        {
           z = xleft + halfh;
           xleft += h;
           flag = step(z, h, ev, &pn, &rn, &tau, &om, &hom, &psi, &dpsi,
                       &scale, 0);
           if (flag < 0) return (flag);
           scale = exp(scale);
           phi = psi * scale;
           dphi = dpsi * scale;
           du = pdu / pn;
           if (fabs(tau) * h * h >= 1.e-4)
              fnorm += rn * (phi * dphi * (u * u - du * du / tau) / 2.0
                       + u * phi * du * phi + h * (u * u + du * du / tau)
                       / 2.0);
           else
           {
              tauhh = tau * h * h;
              fnorm += rn * h * (u * u * (1.0 + tauhh * (tauhh / 5.0
                       - 1.0) / 3.0) + h * u * du * (1.0 + tauhh * 
                       (2.0 * tauhh / 15.0 - 1.0) / 3.0) + h * h * du
                       * du * (1.0 + tauhh * (2.0 * tauhh / 21.0 - 1.0)
                       / 5.0) / 3.0);
           }
           if ((i == g_nxinit-1) && (j == klvl-1) && cspec[0])
           {
              hsave = h;
              usave = fabs(u);
              dusave = fabs(pdu);
           }
           unew = dphi * u + phi * du;
           pdu = -pn * tau * phi * u + dphi * pdu;
           u = unew;
           if ((i == 1) && (j == 0) && cspec[1])
           {
              hsave = h;
              usave = fabs(u);
              dusave = fabs(pdu);
           }
        }
     }
     if (cspec[1])
     {
        if (g_reg[0] || (g_pnu[0] == 0.0) || (g_pnu[0] == 1.0 - g_ep[0]))
           phi = denom;
        else
        {
           if (usave >= dusave)
              phi = usave / pow(hsave, g_pnu[0]);
           else
              phi = dusave / (g_cp[0] * fabs(g_pnu[0]) * 
                    pow(hsave, g_ep[0] + g_pnu[0] - 1.0));
        }
     }
     else
     {
        if (g_reg[1] || (g_pnu[1] == 0.0) || (g_pnu[1] == 1.0 - g_ep[1]))
           phi = denom;
        else
        {
           if (usave >= dusave)
              phi = usave / pow(hsave, g_pnu[1]);
           else
              phi = dusave / (g_cp[1] * fabs(g_pnu[1]) * pow(hsave,
                    g_ep[1] + g_pnu[1] - 1.0));
        }
     }
     *rsubn = 1.0 / (alpha + fnorm / phi / phi);
     return(0);
}

/***********************************************************************/

int get_rho(logical *cspec, logical *job, int *numx, int numt, double *t,
            double *rho, double *tol)
{
/*   input: cspec[], job[], *numx, numt, t[], tol[];
     output: rho[];

     Local variables:                                               */

     int flag, i, iev, j, jtol, k, kcl1, kcl2, maxits, maxt, ndim;
     long int mu1, mu2, numev;
     double aa, bb, dens, denshi = 12.0, denslo = 4.0, densop = 6.0,
        endfac, endi[] = {12.0, 20.0, 85.0, 240.0, 500.0}, error, fz,
        rhotol, tol1, xtol, zeta, zetai[] = {2.2, 2.0, 1.5, 1.4, 1.3},
       *oldrho;

     logical aafin, bbfin, done, edone, oscill;

/*   Initialize:                                                    */

     oldrho = vector_d(numt);
     tol1 = tol[1] + tol[2];
     xtol = -log10(max_d(tol[0], tol[1]));
     jtol = (int) (xtol - 0.5);
     jtol = min_i(max_i(jtol, 1), 5);
     densop = 3 * jtol;
     maxits = (15 - jtol) / 3;
     ndim = 100;

/*   Set g_maxlvl for the density function calculation; if this is to be
     increased, see remarks in "IMPORTANT NOTE" in function sledge above. */

     g_maxlvl = (7 + jtol) / 2;
     aafin = g_afin;
     aa = g_a;
     kcl1 = kclass[0];
     bbfin = g_bfin;
     bb = g_b;
     kcl2 = kclass[1];
     g_nxinit = *numx;
     if (job[4])
     {

/*      Use interval truncation in this oscillatory regime.         */

        oscill = FALSE;
        if ((!job[3]) && ((kclass[0] == 1) || (kclass[1] == 1))) 
           oscill = TRUE;
        if (!oscill)
        {
           if (*numx == 0) g_nxinit = 4 * jtol + 5;
           endfac = endi[jtol-1];
        }
        else
        {
           if (*numx == 0) g_nxinit = 24 * jtol + 11;
           endfac = 48.0;
        }
        if (cspec[0])
        {
           if (g_afin)
           {
              kclass[0] = 7;
              endfac = 4.0 * endfac;
              if (g_bfin)
                 g_a = aa + (bb - aa) / endfac;
              else
                 g_a = aa + fabs(aa) / endfac;
           }
           else
           {
              g_afin = TRUE;
              kclass[0] = 0;
              if (g_bfin)
                 g_a = -endfac - min_d(-g_b, 0.0);
              else
                 g_a = -endfac;
           }
        }
        else
        {
           if (g_bfin)
           {
              kclass[1] = 7;
              endfac = 4.0 * endfac;
              if (g_afin)
                 g_b = bb - (bb - aa) / endfac;
              else
                 g_b = bb - fabs(bb) / endfac;
           }
           else
           {
              g_bfin = TRUE;
              kclass[1] = 0;
              if (g_afin)
                 g_b = endfac + max_d(g_a, 0.0);
              else
                 g_b = endfac;
           }
        }
     }
     else
     {
        if (cspec[0]) g_afin = TRUE;
        if (cspec[1]) g_bfin = TRUE;
        if (*numx == 0) return(-30);
     }
     maxt = numt;

/*   Loop over the choices of intervals.                        */

     numev = 0;
     for (k = 0; k < maxits; k++)
     {
        g_lflag[2] = FALSE;
        flag = 0;
        if (g_print > 0)
        {
            printf(" ---------------- Iteration %2d ----------------\n",
                   k + 1);
            fprintf(output_file,
             " ---------------- Iteration %2d ----------------\n", k + 1);
            fprintf(output_file, " For a = %13.6e,  B = %13.6e\n",
                    g_a, g_b);
            fprintf(output_file, " Nxinit = %4d\n", g_nxinit);
        }
        if (job[4])
        {
           free(x);
           x = vector_d(g_nxinit + 16);        /*   over-dimension */
           x[0] = g_a;
           x[g_nxinit-1] = g_b;
           flag = mesh(TRUE, -1, tol1);
        }
        if (g_print > 2)
        {
           printf(" Level 0 mesh:\n");
           fprintf(output_file, "Level 0 mesh:\n");
           for (i = 0; i < g_nxinit; i = i + 5)
           {
              for (j = 0; (j < 5) && (i+j < g_nxinit); j++)
              {
                 printf("%13.6e  ", x[i+j]);
                 fprintf(output_file, "%13.6e  ", x[i+j]);
              }
              printf("\n");
              fprintf(output_file, "\n");
           }
           fprintf(output_file, "\n");
           printf("\n");
        }
        flag = density_function(tol, cspec, k + 1, ndim, maxt, t, rho,
                                &iev, &numev);
        if (flag == -3)
        {
          g_lflag[5] = FALSE;
          flag = 0;
        }
        if ((flag < 0) || (!job[4])) break;

/*      Global error testing:                          */

        done = FALSE;
        if (k > 0)
        {
           done = TRUE;
           for (i = 0; i < maxt; i++)
           {
              rhotol = 2.0 * zeta * max_d(tol[0], tol[1] * rho[i]);
              error = rho[i] - oldrho[i];
              if (fabs(error) <= rhotol)
                 edone = TRUE;
              else
              {
                 edone = FALSE;
                 maxt = i + 1;
              }
              if (done && edone)
                 done = TRUE;
              else
                done = FALSE;
           }
           if (done) break;
        }
        if (g_print > 1)
        {
           printf("         t               Truncated Rho(t)\n");
           fprintf(output_file,
                 "         t               Truncated Rho(t)\n");
           for (i = 0; i < numt; i++)
           {
              printf(" %12.4f %31.15e\n", t[i], rho[i]);
              fprintf(output_file, " %12.4f %31.15e\n", t[i], rho[i]);
           }
        }
        for (i = 0; i < numt; i++) oldrho[i] = rho[i];
        g_countz = TRUE;
        flag = shoot(g_cutoff, &mu1, &fz);
        flag = shoot(t[numt-1], &mu2, &fz);
        g_countz = FALSE;
        if (t[numt-1] > g_cutoff)
           dens = (mu2 - mu1) / ((k + 1.0) * (t[numt-1] - g_cutoff));
        else
           dens = densop;
        if (!oscill)
        {
           g_nxinit += 10;
           zeta = zetai[jtol-1];
        }
        else
        {
           zeta = 2.0;
           g_nxinit *= zeta;
        }
        if (cspec[0])
        {
           if (aafin)
           {
              endfac = 5.0 * zeta;
              if (dens < denslo) endfac = 75.0;
              if ((dens > denshi) && (!oscill)) endfac = 8.0;
              g_a = aa + (g_a - aa) / endfac;
              if ((aa - g_a) * (aa - g_a) < g_u)
              {
                 flag = -9;
                 break;
              }
           }
           else
           {
              if (dens < denslo) zeta = 2.0;
              if ((dens > denshi) && (!oscill)) zeta = 1.4;
              g_a *= zeta;
           }
        }
        if (cspec[1])
        {
           if (bbfin)
           {
              endfac = 5.0 * zeta;
              if (dens < denslo) endfac = 75.0;
              if ((dens > denshi) && (!oscill)) endfac = 8.0;
              g_b = bb-(bb-g_b)/endfac;
              if ((g_b-bb)*(g_b-bb) < g_u)
              {
                 flag = -9;
                 break;
              }
           }
           else
           {
              if (dens < denslo) zeta = 2.0;
              if ((dens > denshi) && (!oscill)) zeta = 1.4;
              g_b *= zeta;
           }
        }
        if (fmod(g_nxinit,2) == 0) ++g_nxinit;
        if (g_nxinit > 464) g_nxinit = 464;
        ndim *= zeta;
     }

/*   End of loop over endpoints.                       */

     if (k == maxits) flag = -3;
     if (g_print > 0) fprintf(output_file,
            " The total number of eigenvalues computed was %10d\n",
            numev);
     if (cspec[0])
     {
        if (aafin)
        {
           g_a = aa;
           kclass[0] = kcl1;
        }
        else
           g_afin = FALSE;
     }
     if (cspec[1])
     {
        if (bbfin)
        {
           g_b = bb;
           kclass[1] = kcl2;
        }
        else
           g_bfin = FALSE;
     }
     free(oldrho);
     return(0);
}

/***********************************************************************/

int mesh(logical job, int nev, double tol)
{
/*   input: job, nev, tol;
     return flag: 0 -- success; other failure.
     Calculate the initial mesh; redistribute so that h[*] is
     approximately equidistributed.  (return flag)                    */

     int i, its, j, jtol, k, maxits, n, nadd[2];
     double *g, *h, *qlnf, *z, ya[2], dx, eps = 0.0001, eqmax, eqmin,
        ev, gamma, p1, p2, p3, qmax, qmin, q1, q2, q3, r1, r2, r3, 
        weight, y, y1, y2, y3;
     static double enda, endb;
     logical done;

     if (!job)
     {

/*       Mesh has been pre-defined by the user.                       */

         g_hmin = g_b - g_a;
         for (i = 1; i < g_nxinit; i++)
             g_hmin = min_d(g_hmin, x[i] - x[i-1]);
         return(0);
     }
     n = g_nxinit - 1;
     ev = nev;

     g    = vector_d(g_nxinit);
     h    = vector_d(g_nxinit);
     qlnf = vector_d(g_nxinit);
     z    = vector_d(g_nxinit);

/*   Find an appropriate initial mesh.                                */

     if (g_afin)
     {
        x[0] = g_a;
        if (g_bfin)
        {
           x[n] = g_b;
           dx = (g_b - g_a) / n;
           for (i = 1; i < n; i++)
               x[i] = x[0] + i * dx;
        }
        else
        {
           if (nev < 0)
              endb = x[n];
           else
              x[n] = (1 + 4 * nev) * endb;
           y1 = x[0] / (1.0 + fabs(x[0]));
           dx = (x[n] / (1.0 + x[n]) - y1) / n;
           for (i = 1; i < n; i++)
           {
               y = y1 + i * dx;
               x[i] = y / (1.0 - fabs(y));
           }
        }
     }
     else
     {
        if (nev < 0)
           enda = x[0];
        else
           x[0] = (1 + 4 * nev) * enda;
        if (g_bfin)
        {
           x[n] = g_b;
           y1 = x[n] / (1.0 + fabs(x[n]));
           dx = (y1 - x[0] / (1.0 - x[0])) / n;
           for (i = 1; i < n; i++)
           {
               y = y1 - i * dx;
               x[n-i] = y / (1.0 - fabs(y));
           }
        }
        else
        {
           y1 = x[0] / (1.0 - x[0]);
           if (nev < 0)
              endb = x[n];
           else
              x[n] = (1 + 4 * nev) * endb;
           y2 = x[n] / (1.0 + x[n]);
           dx = (y2 - y1) / n;
           for (i = 1; i < n; i++)
           {
               y = y1 + i * dx;
               x[i] = y / (1.0 - fabs(y));
           }
           if (fabs(x[n/2]) < tol) x[n/2] = 0.0;
        }
     }
     jtol = (int)(-log10(tol) + 0.5);
     if (g_reg[0] && g_reg[1])
        maxits = 6;
     else
        maxits = 3;

/*   Calculate H(*) and G(*).                              */

     if ((!g_afin) && (x[1] >= 0.0)) x[1] = max_d(0.5 * x[0], -1.0);
     if ((!g_bfin) && (x[n] <= 0.0)) x[n-1] = min_d(0.5 * x[n], 1.0);
     qmin = 1.0e31;
     qmax = -qmin;
     done = FALSE;
     for (its = 0; ((its < maxits) && (!done)); its++)
     {
        gamma = 0.0;

/*      Equidistribute {[Qmax - Q]^2 * max[abs(p'), abs(q'), abs(r')]}.*/

        eqmax = 0.0;
        eqmin = 1.0e31;
        for (j = 0; j < n; j++)
        {
           dx = x[j+1] - x[j];
           y2 = x[j] + 0.5 * dx;
           coeff(y2, &p2, &q2, &r2);
           if ((p2 == 0.0) || (r2 == 0.0) || (p2 * r2 < 0.0)) return(-15);
           y1 = max_d(y2 - eps, x[0] + 2.0 * g_u * fabs(x[0]));
           coeff(y1, &p1, &q1, &r1);
           y3 = min_d(y2 + eps, x[n] - 2.0 * g_u * fabs(x[n]));
           coeff(y3, &p3, &q3, &r3);
           if (g_lnf)
           {
              h[j] = fabs(q3 - q1) / (y3 - y1);
              qlnf[j] = q2 / r2;
           }
           else
           {
              h[j] = max_d(fabs(p3 - p1), fabs(q3 - q1));
              h[j] = max_d(h[j], fabs(r3 - r1)) / (y3 - y1);
              y1 = sqrt(sqrt(r1 * p1));
              y2 = sqrt(sqrt(r2 * p2));
              y3 = sqrt(sqrt(r3 * p3));
              y = sqrt(p2 / r2);
              qlnf[j] = q2 / r2 + y * ((y3 - y1) * (sqrt(p3 / r3) -
                        sqrt(p1 / r1)) / 4.0 + (y3 - 2.0 * y2 + y1) * y)
                        / (y2 * eps * eps);
              if (fabs(qlnf[j]) <= eps) qlnf[j] = 0.0;
           }
           qmax = max_d(qmax, qlnf[j]);
           qmin = min_d(qmin, qlnf[j]);
        }
        y = max_d(qmax - qmin, 1.0);
        ev = 100.0 + max_d(0.0, qmin);
        for (j = 0; j < n; j++)
        {
           dx = x[j+1] - x[j];
           if (qlnf[j] <= ev)
           {
              weight = 3.0 * pow((qmax - qlnf[j]) / y, 2) + 1.0;
              h[j] = max_d(h[j] * weight, g_u);
           }
           else
           {
              y2 = 2.0 * dx * sqrt(qlnf[j] - ev);
              if (y2 <= g_under)
              {
                 weight = exp(-y2);
                 h[j] = max_d(weight * h[j], g_u);
              }
              else
                 h[j] = g_u;
           }
           if ((!g_afin) && (x[j+1] < 0.0))
           {
              h[j] = max_d(h[j] * exp(x[j+1]), g_u);
              if ((j == 0) && (h[0] == g_u)) x[0] = 0.5 * (x[0] + x[1]);
           }
           if ((!g_bfin) && (x[j] > 0.0))
           {
              h[j] = max_d(h[j] * exp(-x[j]), g_u);
              if ((j == n-1) && (h[n-1] == g_u)) x[n] = 0.5 * 
                                                (x[n-1] + x[n]);
           }
           eqmin = min_d(eqmin, h[j] * dx);
           eqmax = max_d(eqmax, h[j] * dx);
        }
        g_ncoeff += 3 * n;
        if (eqmax - eqmin <= max_d(0.1 * eqmax, g_u / 0.1)) break;

/*      Use a roughly locally quasi-uniform mesh.                      */

        gamma = 0.0;
        for (i = 0; i < n; i++) gamma += h[i];
        gamma = 1.0 / gamma;
        for (i = 0; i < n; i++)
        {
            y = 0.0;
            for (j = 0; j < n; j++)
                y = max_d(y, h[j] / (1.0 + gamma * fabs(x[i] + x[i+1] 
                    - x[j] - x[j+1]) * h[j]));
            z[i] = y;
        }
        for (i = 0; i < n; i++)
            h[i] = z[i];
        g[0] = 0.0;
        for (j = 0; j < n; j++)
            g[j+1] = g[j] + h[j] * (x[j+1] - x[j]);
        gamma = g[n] / n;

/*      Redistribution algorithm:                                          */

        y = gamma;
        i = 0;
        for (j = 0; j < n; j++)
           while (y <= g[j+1])
           {
              ++i;
              z[i] = x[j] + (y - g[j]) / h[j];
              y += gamma;
           }
        z[0] = x[0];
        z[n] = x[n];
        done = TRUE;
        for (j = 1; j < n; j++)
        {
           if (fabs(z[j] - x[j]) > 0.1 * (z[j+1] - z[j-1])) done = FALSE;
           x[j] = z[j];
	}
     }
     if ((!g_afin) && (x[1] >= 0.0)) x[1] = -1.0;
     if ((!g_bfin) && (x[n] <= 0.0)) x[n-1] = 1.0;
     for (k = 0; k < 2; k++)
        switch (kclass[k])
        {

/*      Add nadd[k] extra points near endpoint k.                   */

        case 1:
              nadd[k] = min_i(max_i((jtol + 2) / 3, 1), 4);
              ya[k] = 0.1;
              break;
        case 2:
              nadd[k] = min_i(max_i(jtol, 3), 4);
              ya[k] = min_d(max_d(pow(0.1, jtol / 3), 1.e-3), 1.e-2);
              break;
        case 3:
              nadd[k] = min_i(max_i(jtol / 2, 2), 4);
              break;
        case 4:
              nadd[k] = min_i(max_i((5 + jtol) / 3, 2), 5);
              ya[k] = pow(0.1, nadd[k] - 1);
              break;
        case 5:
              nadd[k] = pow(pow(2, jtol), 0.4);
              nadd[k] = min_i(max_i(nadd[k], 3), 6);
              ya[k] = min_d(max_d(pow(pow(0.1, jtol), 0.333), 0.001), 0.1);
              break;
        case 6:
              nadd[k] = min_i(max_i(jtol, 2), 8);
              ya[k] = 0.005;
              break;
        case 7:
              nadd[k] = min_i(max_i((2 * jtol + 6) / 3,3), 8);
              ya[k] = min_d(max_d(sqrt(0.1 * tol), 1.e-6), 1.e-2);
              break;
        case 8:
              nadd[k] = pow(pow(2, jtol), 0.4);
              nadd[k] = min_i(max_i(nadd[k], 2), 6);
              ya[k] = min_d(max_d(pow(pow(0.1, jtol), 0.333), 0.001), 0.05);
              break;
        case 9:
              if (g_lflag[4])
              {
                 nadd[k] = min_i(max_i((jtol + 4) / 3, 2), 5);
                 ya[k] = pow(0.1,nadd[k] - 1);
              }
              else
              {
                 nadd[k] = min_i(max_i(2 + jtol * (jtol - 3) / 40, 2), 4);
                 ya[k] = 0.25;
              }
              break;
        case 10:
              nadd[k] = min_i(max_i((2 * jtol + 6) / 3, 3), 8);
              ya[k] = min_d(max_d(sqrt(0.1 * tol), 1.e-6), 1.e-2);
              break;
        default:
              nadd[k] = 0;
              break;
        }
     if (nadd[0] + nadd[1] > 0)
     {

/*      Add additional points in the mesh, change the dimension of x here.*/

        for (k = 0; k < g_nxinit; k++) z[k] = x[k];
        if (nadd[0] > 0)
        {
           for (i = 1; i < g_nxinit ; i++)
               x[i+nadd[0]] = z[i];
           g_nxinit += nadd[0];
           if (g_afin)
           {
              dx = z[1] - g_a;
              x[0] = z[0];
           }
           for (i = 0; i < nadd[0]; i++)
               if (g_afin)
                  x[i+1] = g_a + dx * pow(ya[0], (double) (nadd[0] - i)
                           / nadd[0]);
               else
               {
                  if (kclass[0] != 1)
                     x[nadd[0]-i] = x[nadd[0]+1-i] - (x[nadd[0]+2-i] -
		                    x[nadd[0]+1-i]) * 2.4;
                  else
                     x[nadd[0]-i] = 2.0 * x[nadd[0]+1-i] - x[nadd[0]+2-i];
               }
        }
        if (nadd[1] > 0)
        {
           if (g_bfin) dx = g_b - z[n-1];
           n = g_nxinit - 1;
           if (nadd[0] == 0)
              for (i = 0; i < g_nxinit; i++)
                  x[i] = z[i];
           g_nxinit += nadd[1];
           x[g_nxinit-1] = g_b;
           for (i = 0; i < nadd[1]; i++)
               if (g_bfin)
                  x[g_nxinit-i-2] = g_b - dx * pow(ya[1], (double)
                                    (nadd[1] - i) / nadd[1]);
               else
               {
                  if (kclass[1] != 1)
                     x[n+i] = x[n+i-1] + (x[n+i-1] - x[n+i-2]) *2.4;
                  else
                     x[n+i] = 2.0 * x[n+i-1] - x[n+i-2];
               }
        }
     }
     if (!g_afin) x[0] = -1.0 / g_u;
     if (!g_bfin) x[g_nxinit-1] = 1.0 / g_u;
     g_hmin = x[g_nxinit-1] - x[0];
     for (i = 1; i < g_nxinit; i++)
         g_hmin = min_d(g_hmin, x[i] - x[i-1]);
     free(g);
     free(h);
     free(qlnf);
     free(z);
     return(0);
}

/***********************************************************************/

int power(double *z, double *f, double *y, int n, double tol, double *ef,
          double *cf, logical *osc, logical *exact)
{
/*   input: z[],f[],y[],n,tol;  output: *ef,*cf,*osc,*exact;
     return flag: 0 -- success; > 0 -- warning.

     Find the power function which "dominates" the tabled
     coefficient function.  The output is Cf and Ef such that
          f(z)  is asymptotic to  Cf*z^Ef .
     The vectors Z(*) and F(*) hold the N input points:
          F(I) = f(Z(I)) I = 1,...,N.
     Set flag = 0 for a normal return; 1 for uncertainty in Ef;
     2 if uncertain about Cf (oscillatory).              */

     double error, tolait, tolmin = 1.e-6;
     int flag = 0, i, j, k, ny;

/*   Estimate the exponent.                                           */

     *osc = FALSE;
     ny = n - 1;
     error = 1.e20;
     tolait = min_d(tolmin, tol);
     for ( k = 0; k < ny; k++)
     {
         if ((f[k] != 0.0) && (f[k+1] != 0.0))
            y[k] = log(fabs(f[k+1] / f[k])) / log(fabs(z[k+1] / z[k]));
         else
            y[k] = 0.0;
     }
     *ef = y[ny-1];
     if (g_print > 4)
     {
        fprintf(output_file, " From POWER; E_k and c_k sequences:\n");
        for (i = 0; i < ny; i = i + 4)
        {
            for (j = 0; (j < 4) && (i + j < ny); j++)
               fprintf(output_file, "%19.10e", y[i+j]);
               fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n");
     }
     error = aitken(ef, tolait, ny, y);
     k = (int) (*ef + sign(0.5, *ef));
     if (fabs(k - (*ef)) <= sqrt(tol)) *ef = k;
     if (fabs(error) > tol * max_d(1.0, fabs(*ef)))

/*   There is uncertainty in the exponent.                 */

     flag = 1;
     if (fabs(*ef) <= tol) *ef = 0.0;

/*   Estimate the coefficient.                             */

     for (k = 0; k < n; k++)
        y[k] = f[k] / pow(fabs(z[k]), *ef);
     *cf = y[n-1];
     if (g_print > 4)
     {
        for (i = 0; i < n-1; i = i + 4)
        {
           for (j = 0; (j < 4) && (i + j < n - 1); j++)
              fprintf(output_file, "%19.10e", y[i+j]);
              fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n");
     }
     error = aitken(cf,tolait,n,y);
     if ((*ef > 20.0) && (fabs(*cf) <= tol))

/*      Coefficient probably has exponential behavior.     */

        *cf = sign(1.0, y[n-1]);
     else
     {
        if ((fabs(error) > tol * max_d(1.0, fabs(*cf))) ||
           ((fabs(f[n] - *cf * pow(z[n], *ef)) > 20.0 * tol * fabs(f[n])) 
           && (*ef != 0.0)))
        {

/*         There is uncertainty in the coefficient; call such
           cases oscillatory.                              */

           flag = 2;
           *osc = TRUE;
        }
     }
     if (fabs(*cf) > 1.e7)
     {
        *exact = FALSE;
        return(flag);
     }
     k = (int) (*cf + 0.5);
     if ((fabs(k - (*cf)) <= sqrt(tol))  && (k != 0)) *cf = k;
     *exact = TRUE;
     for (k = 0; k < n; k++)
        if (fabs(f[k] - (*cf) * pow(z[k], *ef)) > tol * fabs(f[k]))
           *exact = FALSE;
     return (flag);
}

/***********************************************************************/

int pqrint(double z, double *sqrtrp, double *qlnf)
{
/*   input: z; output: *sqrtrp, *qlnf.
     return flag: = 0, success; < 0, failure.

     Evaluate the integrands needed for the asymptotic formulas.
     (1) The Liouville normal form potential Qlnf:
         Qlnf(t) =  q/r + f"(t)/f   with   f = (pr)**.25  .
     (2) The term in the change of independent variable: sqrt(r/p).  */

     double eps, fl, fm, fr, px, qx, rx, t, xdotl, xdotr;

     if (g_lnf)
     {
        coeff(z, &px, &qx, &rx);
        if (px == 0.0 || rx == 0.0 || px * rx < 0.0) return(-15);
        ++g_ncoeff;
        *qlnf = qx / rx;
        *sqrtrp = sqrt(rx / px);
     }
     else
     {
        eps = min_d(1.0e-4, min_d(fabs(g_b - z), fabs(z - g_a)) / 2.0);
        t = z - eps;
        coeff(t, &px, &qx, &rx);
        if (px == 0.0 || rx == 0.0 || px * rx < 0.0) return(-15);
        xdotl = sqrt(px / rx);
        fl = sqrt(sqrt(px * rx));
        t = z + eps;
        coeff(t, &px, &qx, &rx);
        xdotr = sqrt(px / rx);
        fr = sqrt(sqrt(px * rx));
        coeff(z, &px, &qx, &rx);
        *sqrtrp = sqrt(rx / px);
        fm = sqrt(sqrt(px * rx));
        g_ncoeff += 3;
        *qlnf = qx / rx + ((fr - fm) * (xdotr - xdotl) / 4.0 + (fr - 
                2.0 * fm + fl) / (*sqrtrp)) / (eps * eps * fm * (*sqrtrp));
        if (fabs(*qlnf) <= eps) *qlnf = 0.0;
     }
     return(0);
}

/***********************************************************************/

int regular(logical job, logical jobmesh, double *tol, int nev, double *ev,
            double *lef, double *lpdef)

/************************************************************************
*                                                                       *
*     regular calculates Sturm-Liouville eigenvalue and (optionally)    *
*     eigenfunction estimates for the problem described initially.      *
*                                                                       *
*************************************************************************

     Input parameters:
       job       = logical variable describing tasks to be carried out.
                   job = TRUE iff an eigenfunction is to be calculated.
       jobmesh   = logical variable, jobmesh = TRUE iff initial mesh
                   is a function of the eigenvalue index.
       tol[*]    = double vector of 6 tolerances.
                   tol[0] is the absolute error tolerance for e-values,
                   tol[1] is the relative error tolerance for e-values,
                   tol[2] is the abs. error tolerance for e-functions,
                   tol[3] is the rel. error tolerance for e-functions,
                   tol[4] is the abs. error tolerance for e-function
                          derivatives,
                   tol[5] is the rel. error tolerance for e-function
                          derivatives.
                   Eigenfunction tolerances need not be set if JOB is
                   False.  All absolute error tolerances must be
                   positive; all relative must be at least 100 times
                   the unit roundoff.
       nev       = integer index for the eigenvalue sought; NEV .GE. 0 .
       ev        = double initial guess for eigenvalue NEV; accuracy is
                   not at all critical, but if a good estimate is
                   available some time may be saved.

     Output parameters:
       ev        = double computed approximation to NEVth eigenvalue.
       x[]       = double vector of points for eigenfunction output.
       lef[]     = double vector of eigenfunction values: lef[i] is the
                   estimate of u(x[i]).  If job is FALSE then this
                   vector is not referenced.
       lpdef[]   = double vector of eigenfunction derivative values:
                   lpdef[i] is the estimate of (pu')(x[i]).  If job is
                   FALSE then this vector is not referenced.
-----------------------------------------------------------------------
     Local variables:                                      */
{
     int flag = 0, i, j;
     double *evext, **exef, **expdef, *scal, abserr, alpha1, alpha2, 
        beta1, beta2, delta, efnorm, error, evhat, evhigh, evlow, fhigh,
        flow, h, pdumax, qint, qlnf, relerr, rpint, sqrtrp, tolext,
        tolmin = 1.e-3, tolpdu, tolsum, tol1, tol2, tol3, tol4, tol5,
        tol6, umax, z;
     logical done, efdone, ef_fin[1], evdone, exfull, pdef_fin[1];

     evext  = vector_d(g_maxlvl + 2);
     if (job)
     {
        scal   = vector_d(g_nxinit);
        exef   = matrix_d(g_nxinit, g_maxlvl + 2);
        expdef = matrix_d(g_nxinit, g_maxlvl + 2);
     }

     alpha1 = 0.0;
     alpha2 = 1.0;
     beta1 = 0.0;
     beta2 = 1.0;
     if (nev < 0) return(-37);
     evdone = FALSE;
     tol1 = min_d(tol[0], tolmin);
     tol2 = min_d(tol[1], tolmin);
     if (!(g_reg[0] && g_reg[1]))
     {
        tol1 = tol1 / 3.0;
        tol2 = tol2 / 3.0;
     }
     if (job)
     {
        tol3 = min_d(tol[2], tolmin);
        tol4 = min_d(tol[3], tolmin);
        tol5 = min_d(tol[4], tolmin);
        tol6 = min_d(tol[5], tolmin);
        abserr = tol1 / 5.0;
        relerr = tol2 / 5.0;
        exfull = TRUE;
     }
     else
     {
        efdone = TRUE;
        abserr = tol1 / 10.0;
        relerr = tol2 / 10.0;
        exfull = FALSE;
     }
     tolsum = tol1 + tol2;
     if (jobmesh)
     {
        if (nev >= 0)
        {
           flag = mesh(TRUE, nev, tolsum);
           if (g_print > 0)
           {
              printf(" Level 0 mesh:\n");
              fprintf(output_file, "Level 0 mesh:\n");
              for (i = 0; i < g_nxinit; i = i + 5)
              {
                 for (j = 0; (j < 5) && (i + j < g_nxinit); j++)
                 {
                    printf("%13.6e  ", x[i+j]);
                    fprintf(output_file, "%13.6e  ", x[i+j]);
                 }
                 printf("\n");
                 fprintf(output_file, "\n");
              }
              fprintf(output_file, "\n");
              printf("\n");
           }
        }
     }

/*   Compute estimates for integrals in asymptotic formulas (accuracy
     is not all that critical).                                    */

     qint = 0.0;
     rpint = 0.0;
     for (i = 1; i < g_nxinit; i++)
     {
        h = x[i] - x[i-1];
        z = x[i-1] + 0.5 * h;
        if ((!g_afin) && (i == 1))
        {
           h = x[2] - x[1];
           z = x[1];
        }
        if ((!g_bfin) && (i == g_nxinit - 1))
        {
           h = x[g_nxinit-2] - x[g_nxinit-3];
           z = x[g_nxinit-2];
        }
        flag = pqrint(z, &sqrtrp, &qlnf);
        if (flag < 0) return(flag);
        qint = qint + h * qlnf;
        rpint = rpint + h * sqrtrp;
     }
     if (qint > 1.0 / g_u) qint = 0.0;
     if (rpint > 1.0 / g_u) rpint = 0.0;

/*   Loop over the levels.                                 */

     for (g_level = 0; g_level <= g_maxlvl; g_level++)
     {
        if (g_hmin / pow(2, g_level) <= 10.0 * g_u)
        {
           flag = -8;
           break;
        }

/*      Find a bracket for the Nevth eigenvalue.           */

        if (g_level == 0)
        {
           *ev = asymp_ev(nev, qint, rpint, alpha1, alpha2, beta1, beta2);
           *ev = max_d(*ev, 0.0);
           delta = 0.5;
        }
        else
        {
           delta = max_d(tolsum * fabs(evhat), 0.25 * delta);
           if (g_level > 1)
           {
              error = (evext[g_level-1] - evext[g_level-2]) / 3.0;
              if (fabs(error) <= 100.0)
                 *ev = evhat + error;
              else
              {
                 delta = 1.0;
                 *ev = evhat;
              }
           }
           else
              *ev = evhat;
        }
        evlow  = *ev - delta;
        evhigh = *ev + delta;
        if (g_print > 3)
           fprintf(output_file,
                   " In bracket:\n evlow = %22.15e evhigh = %22.15e\n",
                   evlow, evhigh);
        flag = bracket(nev, &evlow, &evhigh, &flow, &fhigh, abserr, relerr);
        if (g_print > 3)
           fprintf(output_file,
                   " Out bracket:\n evlow = %22.15e  evhigh = %22.15e\n",
                   evlow, evhigh);
        delta = 0.5 * (evhigh - evlow);
        if (flag < 0) return(flag);
        if (fabs(evhigh - evlow) > max_d(abserr, relerr * fabs(evhigh)))
        {
           flag = zero_calc(&evlow, &evhigh, &flow, &fhigh, abserr, relerr);
           if (flag != 0) return(-7);
        }
        evhat = min_d(evlow, evhigh);
        if (g_print > 0)
        {
           printf(" Level %3d ;    EvHat = %24.15e\n", g_level, evhat);
           fprintf(output_file, " Level %3d ;    EvHat = %24.15e\n",
                   g_level, evhat);
        }
        *ev = evhat;
        tolext = max_d(tol1, fabs(*ev) * tol2);
        evdone = extrapolation(ev, tolext, g_level + 1, g_nextrp, exfull,
                     TRUE, 0, evext, &error);
        if (job)
        {
           get_ef(evhat, &efnorm, x, ef_fin, pdef_fin, lef, lpdef, scal);
           if (g_level == 0)
           {
              umax = 1.0;
              pdumax = 1.0;
           }

/*         Extrapolate eigenfunction values.               */

           tolext = max_d(tol3, umax * tol4);
           tolpdu = max_d(tol5, pdumax * tol6);
           if (g_afin)
           {
              umax = fabs(lef[0]);
              pdumax = fabs(lpdef[0]);
           }
           else
           {
              umax = 0.0;
              pdumax = 0.0;
           }
           efdone = TRUE;
           if (ef_fin[0])
              done = extrapolation(&lef[0], tolext, g_level + 1, g_nextrp,
                         exfull, TRUE, 0, &exef[0][0], &error);
           if (pdef_fin)
              done = extrapolation(&lpdef[0], tolpdu, g_level + 1, 
                         g_nextrp, exfull, TRUE, 0, &expdef[0][0], &error);
           if (efdone && done)
              efdone = TRUE;
           else
              efdone = FALSE;
           for (i = 1; i < g_nxinit - 1; i++)
           {
               done = extrapolation(&lef[i], tolext, g_level + 1, g_nextrp,
                          exfull, TRUE, 0, &exef[i][0], &error);
               if (efdone && done)
                  efdone = TRUE;
               else
                  efdone = FALSE;
               done = extrapolation(&lpdef[i], tolpdu, g_level + 1,
                          g_nextrp, exfull, TRUE, 0, &expdef[i][0], &error);
               if (efdone && done)
                  efdone = TRUE;
               else
                  efdone = FALSE;
               umax = max_d(umax, fabs(exef[i][0]));
               pdumax = max_d(pdumax, fabs(expdef[i][0]));
           }
           if (ef_fin[1])
              done = extrapolation(&lef[g_nxinit-1], tolext, g_level + 1,
                         g_nextrp, exfull, TRUE, 0, &exef[g_nxinit-1][0],
                         &error);
           if (efdone && done)
              efdone = TRUE;
           else
              efdone = FALSE;
           if (pdef_fin[1])
              done = extrapolation(&lpdef[g_nxinit-1], tolpdu, g_level + 1,
                         g_nextrp, exfull, TRUE, 0, &expdef[g_nxinit-1][0],
                         &error);
           if (efdone && done)
              efdone = TRUE;
           else
              efdone = FALSE;
           if (g_bfin)
           {
              umax = max_d(umax, fabs(exef[g_nxinit-1][0]));
              pdumax = max_d(pdumax, fabs(expdef[g_nxinit-1][0]));
           }
           abserr = max_d(0.5 * abserr, 10.0 * g_u);
           relerr = max_d(0.5 * relerr, 10.0 * g_u);
        }
        if (evdone && (g_level > 1) && efdone)
           break;
     }
     free(evext);
     if (job)
     {
        free(scal);
        free(exef);
        free(expdef);
     }
     if (!evdone)
        return(-1);
     if (job && (flag >= 0) && (!efdone))
        flag = -2;
     return(flag);
}

/***********************************************************************/

int shoot(double ev, long int *mu, double *fev)
{
/*   input: ev; output: *mu, *fev;
     return flag: 0 -- success; < 0 -- failure.            */

     int flag = 0, i, j, klvl, mode, nsave;
     long int k1, k2, nzero = 0l;
     static long int imax = 1000000l;
     double chi, dpsi, dv, h, halfh, homega, omega, pdv, phase, pn, psi,
        rn, sa1, sa2, sb1, sb2, scale, sgn, t, tau, v, vnew, xleft, z, x2;

/*   Make one shot across (A,B) for the current mesh using the scaled
     variable v(x). Count zeros if countz is TRUE.

     Shoot from x = A to x = B.                            */

     v = g_a2 - g_a2p * ev;
     pdv = g_a1 - g_a1p * ev;
     nsave = g_nsgnf;
     sa1 = g_a1;
     sa2 = g_a2;
     sb1 = g_b1;
     sb2 = g_b2;
     klvl = (int) pow(2, g_level);
     mode = 0;

/*   Modify base sign count if necessary.                  */

     if (((kclass[0] > 8) || (kclass[1] > 8)) && (ev < g_cutoff))
     {
        if (kclass[0] > 8)
        {
           sa1  = g_d[0][3];
           sa2  = 1.0;
           v    = sa2;
           pdv  = sa1;
           mode = 1;
        }
        if (kclass[1] > 8)
        {
           sb1 = g_d[1][3];
           sb2 = 1.0;
        }
        sgn = g_a2 * g_b2;
        if (sgn == 0.0)
        {
           sgn = sa1 * sb2 + sa2 * sb1;
           if (sgn == 0.0) sgn = g_a1 * g_b1;
        }
        g_nsgnf = (int) sign(1.0, sgn);
     }
     if (!g_afin) mode = 3;
     scale = max_d(fabs(v), fabs(pdv));
     v     = v / scale;
     pdv   = pdv / scale;
     for (i = 1; i < g_nxinit; i++)
     {
        xleft = x[i-1];
        h = x[i] - xleft;
        if (mode == 1)
        {
           h = pow(h / g_d[0][0], g_eta[0][0]);
           xleft = 0.0;
        }
        if (mode == 3)
        {
           xleft = 0.0;
           h = -1.0 / x[1];
        }
        if ((kclass[1] > 8) && (i == g_nxinit - 1) &&
            (ev < g_cutoff))
        {

/*         Convert from u(x) to V(t) near x = b.           */

           mode = 2;
           t = pow(h / g_d[1][0], g_eta[1][0]);
           chi = g_d[1][1] * pow(t, g_eta[1][1]);
           v /= chi;
           pn = g_cp[1] * pow(h, (g_ep[1] - 1.0));
           pdv = (pdv / (pn * chi * g_eta[1][0]) + g_eta[1][1] * v) *
                  pow(t, (2.0 * g_emu[1] - 1.0));
           h = t;
           xleft = -h;
        }
        if ((!g_bfin) && (i == g_nxinit - 1))
        {
           mode = 4;
           h = 1.0 / x[i-1];
           xleft = -h;
        }
        h /= klvl;
        halfh = 0.5 * h;
        for (j = 0; j < klvl; j++)
        {
            z = xleft + halfh;
            flag = step(z, h, ev, &pn, &rn, &tau, &omega, &homega, &psi,
                        &dpsi, &scale, mode);
            if (flag < 0)
            {
               *fev = 0.0;
               return(flag);
            }
            dv = pdv / pn;
            vnew = dpsi * v + psi * dv;
            xleft += h;
            if (g_countz)
            {

/*            Count zeros of v(x).                         */

              if (tau > 0.0)
              {
                 if (dv == 0.0)
                    nzero += (long int) (0.5 + homega / PI);
                 else
                 {
                    phase = atan(v * omega / dv);
                    k1 = (long int) (phase / PI);
                    x2 = (phase + homega) / PI;
                    if (x2 < (double) imax)
                    {
                       k2 = (long int) x2;
                       nzero += k2 - k1;
                    }
                    else
                       nzero = imax;
                    if (phase * (phase + homega) < 0.0) ++nzero;
                 }
                 if (nzero > imax) nzero = imax;
              }
              else
              {
                 if (vnew * v < 0.0) ++nzero;
              }
           }
           pdv = -pn * tau * psi * v + dpsi * pdv;
           v = vnew;
        }
       if (mode == 1)
       {

/*        Convert from V(t) back to u(x) near x = a.       */

          pn  = g_cp[0] * pow(x[1] - g_a, g_ep[0] - 1.0);
          t   = pow((x[1] - g_a) / g_d[0][0], g_eta[0][0]);
          chi = g_d[0][1] * pow(t, g_eta[0][1]);
          pdv = pn * g_eta[0][0] * chi * (g_eta[0][1] * v + pdv *
                pow(t, (1.0 - 2.0 * g_emu[0])));
          v *= chi;
       }
       mode = 0;
     }
     *fev = sb1 * v + sb2 * pdv;
     if (g_countz)
     {

/*       Adjust zero count.                                */

         *mu = nzero;
         if ((g_a2p != 0.0) && (ev >= sa2 / g_a2p)) ++(*mu);
         if (sb2 != 0.0)
         {
            sgn = sign(1.0, g_nsgnf * (*fev));
            if (fmod(*mu, 2) == 1l) sgn = -sgn;
            if (sgn < 0.0) ++(*mu);
         }
     }
     g_nsgnf = nsave;
     return(flag);
}

/***********************************************************************/

int start(logical *job, double *cons, double *tol, int nev, int *indxev,
          int *n, int numt, double *t)
{
/*   input: job[], cons[], tol[], nev, indxev[], *n, numt, t[];
     output: *n;
     return flag: 0 -- success, < 0 -- failure.

     This routine tests the input data, initializes the global variables.
     Check eigenfunction tolerances iff job[0] is TRUE , x[]; numt, t(*)
     iff job[2] is TRUE                                                */

     int flag = 0, i, k;
     double uflow, urn;

/************************************************************************
*   In the following assign statements, initialize urn to an estimate   *
*   for the unit roundoff and uflow to a value somewhat less than       *
*   -ln(underflow).  E.g.,                                              *
*      for IEEE double precision use urn = 2.e-16, uflow = 650;         *
*      for VAXen double precision use urn = 1.e-17, uflow = 85;         *
*      for Crays (single precision) use urn = 7.e-15, uflow = 5000.     *
*   Exact values are not at all critical.                               *
*   Here, we assume IEEE double precision:                              *
************************************************************************/
     urn = 2.0e-16;
     uflow = 650.0;

/*   Initialize.                                                       */

     g_u = urn;
     g_under = uflow;
     g_a1 = cons[0];
     g_a1p = cons[1];
     g_a2 = cons[2];
     g_a2p = cons[3];
     g_b1 = cons[4];
     g_b2 = cons[5];
     g_a = cons[6];
     g_b = cons[7];
     g_ncoeff = 0;
     flag = 0;

/*   Test input.                                           */

     if (g_afin && g_bfin && (g_a >= g_b)) flag = -34;
     if (tol[0] <= 0.0) flag = -35;
     if (tol[1] < 100.0 * g_u) flag = -36;
     if (job[0])
     {
        if (tol[2] <= 0.0) flag = -35;
        if (tol[3] < 100.0 * g_u) flag = -36;
        if (tol[4] <= 0.0) flag = -35;
        if (tol[5] < 100.0 * g_u) flag = -36;
     }
     if (job[1])
     {
        if (*n == 1) flag = -30;
        else
        {
           if (g_afin && (xef[1] <= g_a)) flag = -39;
           for (i = 2; i < *n - 1; i++)
             if (xef[i-1] >= xef[i]) flag = -39;
           if (g_bfin && (xef[*n-2] > g_b)) flag = -39;
        }
        if (( !g_afin) && (xef[1] >= 0.0)) flag = -39;
        if (( !g_bfin) && (xef[*n-2] <= 0.0)) flag = -39;
     }
     if ((job[1] || job[2]) && (nev > 0))
        for (i = 0; i < nev; i++)
            if (indxev[i] < 0) flag = -37;
     if (job[2])
     {
        if (numt <= 0) flag = -38;
        for (i = 1; i < numt; i++)
           if (t[i] <= t[i-1]) flag = -39;
     }
     if (flag >= 0)
     {

/*      Set maxext, the maximum number of extrapolations allowed, and 
        maxint, the maximum number of intervals in x[*] allowed when 
        mesh is chosen by start.

        IMPORTANT NOTE: the size of various fixed arrays in this package
        depends on the value of maxext in this implementation. If maxext
        is increased, then more storage may have to be allocated to the
        columns of r[*][*] in extrap function.             */

        g_maxext = 6;
        g_maxint = 31;

/*      Calculate maximum number of columns in extrapolation table
        and the maximum number of levels allowed.          */

        k = (int) (-log10(tol[1]));
        i = max_i(k + 3,0);
        g_nextrp = min_i(max_i(3, i/2), g_maxext);

/*      Set the g_nxinit and initial mesh                  */

        if (*n > 0)
           g_nxinit = *n;
        else
        {
           g_nxinit = min_i(g_nextrp*2 + 3, g_maxint);
           if (job[0]) *n = g_nxinit;
        }
        x = vector_d(g_nxinit+16);  /* Allow for later added mesh points */
        if (job[1])
        {
           g_a = xef[0];
           g_b = xef[g_nxinit-1];
           for (i = 0; i < g_nxinit; i++)
              x[i] = xef[i];
        }
        else
           xef = vector_d(g_nxinit+16);
     }
     return(flag);
}

/***********************************************************************/

int step(double x, double h, double ev, double *px, double *rx, 
         double *tau, double *omega, double *homega, double *psi, 
         double *dpsi, double *sclog, int mode)
{
/*   input: x, h, ev, mode;
     output: *px, *rx, *tau, *omega, *homega, *psi, *dpsi, *sclog,
     return the flag: 0 -- success, < 0 -- failure.

     Evaluate the coefficient functions, the scaled basis function psi,
     its derivative dpsi, and the log of the scale factor sclog.       */

     double dx, fp, fr, qx, t, tmu, z;
     static double over = 1.e8;

/*   Evaluate the coefficient functions at x and calculate tau.  The
     error flag flag is zero for a successful calculation; if p(x)
     or r(x) are zero, then flag is set to -15.  If the argument for a
     trig function exceeds OVER then flag is set to -10.  Proceed normally
     when Mode = 0; when Mode = 1 or 2 use the change of variable for "hard"
     problems; when Mode = 3 or 4 use the change of variable t = -1/x near
     infinity.                                             */

     if (mode == 0)
        coeff(x, px, &qx, rx);
     else
     {
        if (mode > 2)
        {
           t = x;
           x = -1.0 / t;
           coeff(x, px, &qx, rx);
           t *= t;
           *px *= t;
           qx /= t;
           *rx /= t;
        }
        else
        {
           t = x;
           if (g_eta[mode-1][0] == 1.0)
              dx = g_d[mode-1][0] * fabs(t);
           else
              dx = g_d[mode-1][0] * pow(fabs(t), 1.0 / g_eta[mode-1][0]);
           if (mode == 1)
              z = g_a + dx;
           else
              z = g_b - dx;
             coeff(z, px, &qx, rx);
        }
     }
     ++g_ncoeff;
     if ((*px == 0.0) || (*rx == 0.0)) return(-15);
     if ((mode == 1) || (mode == 2))
     {
        if (g_emu[mode-1] == 0.5)
           tmu = fabs(t);
        else
           tmu = pow(t * t, g_emu[mode-1]);
        fp = g_cp[mode-1];
        if (g_ep[mode-1] != 0.0) fp *= pow(dx, g_ep[mode-1]);
        fr = g_cr[mode-1];
        if (g_er[mode-1] != 0.0) fr *= pow(dx, g_er[mode-1]);
        *tau = ((ev * (*rx) / fr) + (-qx / fr + g_d[mode-1][2] / t / t))
               * fp / (*px);
        *px *= tmu / fp;
        qx = (qx / fr - g_d[mode-1][2] / t / t) * tmu;
        *rx *= tmu / fr;
     }
     *tau = (*rx * ev - qx) / (*px);
     *omega = sqrt(fabs(*tau));
     *homega = *omega * h;
     *sclog = 0.0;

/*   Evaluate the scaled basis functions.                  */

     if (*homega > 0.01)
     {
        if (*tau > 0.0)
        {
           if (*homega > over) return(-10);
           *dpsi = cos(*homega);
           *psi = sin(*homega) / (*omega);
        }
        else
        {
           *sclog = *homega;
           if (*homega < g_under)
           {
              t = tanh(*homega);
              *dpsi = 1.0 / (1.0 + t);
              *psi = t * (*dpsi) / (*omega);
           }
           else
           {
              *sclog = min_d(*sclog, 2.0 * g_under);
              *dpsi = 0.5;
              *psi = *dpsi / (*omega);
           }
        }
     }
     else
     {
        t = *tau * h * h;
        *dpsi = 1.0 + t * (t / 12.0 - 1.0) / 2.0;
        *psi = h * (1.0 + t * (t / 20.0 - 1.0) / 6.0);
        if (t < 0.0)
        {
           t = max_d(fabs(*psi), fabs(*dpsi));
           *sclog = log(t);
           *psi = *psi / t;
           *dpsi = *dpsi / t;
        }
     }
     return(0);
}

/***********************************************************************/

int zero_calc(double *b, double *c, double *fb, double *fc, double abserr,
              double relerr)
{
/*   input:  *b,*c,*fb,*fc,abserr,relerr; output: *b,*c,*fb,*fc;
     return flag: 0 -- success; < 0 -- failure, > 0 -- warning.

     zero_calc computes a root of f.  The method used is a combination of
     bisection and the secant rule.  This code is adapted from one in
     the text "Foundations of Numerical Computing" written by Allen,
     Pruess, and Shampine.

     Input parameters:
           B,C   = values of X such that F(B)*F(C) .LE. 0.
           FB,FC = values of F at input B and C, resp.
           ABSERR,RELERR = absolute and relative error tolerances.
     The stopping criterion is:
           ABS(B-C) .LE. 2.0*MAX(ABSERR,ABS(B)*RELERR).
     Output parameters:
            B,C   =  see IFLAG returns.
            FB    =  value of final residual F(B).
     RETURN(IFLAG) =  0 for normal return; F(B)*F(C) < 0 and the
                        stopping criterion is met (or F(B)=0).  B always
                        satisfies ABS(F(B)) .LE. ABS(F(C)).
                   =  1 if too many function evaluations were made; in this
                        version 200 (MAXF) are allowed.
                   = -2 if F(B)*F(C) is positive on input.             */

     int count, k, nf;
     long int mu;
     double a, acmb, cmb, fa, p, q, tol, width;
     logical bisect;

/*   Initialization.                                       */

     count = 0;
     width = fabs(*b - (*c));
     a = *c;
     fa = *fc;
     if (sign(1.0, fa) == sign(1.0, *fb)) return(-2);
     *fc = fa;
     nf  = 2;
     while (nf < MAXF)
     {
        if (fabs(*fc) < fabs(*fb))
        {

/*         Interchange B and C so that fabs(f(b)) <= fabs(f(c)).     */

           a   = *b;
           fa  = *fb;
           *b  = *c;
           *fb = *fc;
           *c  = a;
           *fc = fa;
        }
        cmb  = (*c - *b) / 2.0;
        acmb = fabs(cmb);
        tol  = max_d(abserr, fabs(*b) * relerr);

/*      Test stopping criterion.                           */

        if (acmb <= tol) return(0);

/*      Calculate new iterate implicitly as B+P/Q where we arrange
        p >= 0.  The implicit form is used to prevent overflow.      */

        p = (*b-a) * (*fb);
        q = fa - (*fb);
        if (p < 0.0)
        {
           p = -p;
           q = -q;
        }

/*      Update A; check if reduction in the size of bracketing interval
        is satisfactory.  If not, bisect until it is.                */

        a  = *b;
        fa = *fb;
        ++count;
        bisect = FALSE;
        if (count >= 4)
        {
           if (8.0 * acmb >= width)
              bisect = TRUE;
           else
           {
              count = 0;
              width = acmb;
           }
        }

/*      Test for too small a change.                       */

        if (p <= fabs(q) * tol)

/*         Increment by tolerance.                         */

           *b += sign(tol, cmb);
        else
        {

/*         Root ought to be between B and (C+B)/2.         */

           if (p < cmb * q)

/*            Use secant rule.                             */

              *b += p / q;
           else

/*            Use bisection.                               */

              bisect = TRUE;
        }
        if (bisect)
           *b += cmb;

/*      Have completed computation for new iterate B.      */

        k = shoot(*b, &mu, fb);
        if (k < 0) return(k);
        ++nf;
        if (fabs(*fb) == 0.0)
        {
           *c  = *b;
           *fc = *fb;
           return(0);
        }
        if (sign(1.0, *fb) == sign(1.0, *fc))
        {
           *c  = a;
           *fc = fa;
        }
     }
     return(1);          /*     nf >= MAXF                 */
}

/***********************************************************************/

double max_d(double x, double y)
{
     if (x > y)
        return(x);
     else
        return(y);
}

/***********************************************************************/

double min_d(double x, double y)
{
     if (x > y)
        return(y);
     else
        return(x);
}

/***********************************************************************/

int max_i(int x, int y)
{
     if (x > y)
        return(x);
     else
        return(y);
}

/***********************************************************************/

int min_i(int x, int y)
{
     if (x > y)
        return(y);
     else
        return(x);
}

/***********************************************************************/

double sign(double x, double y)
{
     if (y > 0)
        return(fabs(x));
     else
        if (y < 0)
           return(-fabs(x));
        else
           return(0.0);
}

/***********************************************************************/

double *vector_d(int n)
{
    double *x;
 
    if ( n > 0)
    {
        x = (double *) calloc(n, sizeof(double));
        if (!x)
        {
           printf(" Memory ran out! Allocation failure in vector.\n");
           exit(EXIT_FAILURE);
        }
     }
     else
        printf(" Warning: The vector has dimension less or equal to zero.\n");
     return(x);
}

/***********************************************************************/

int *vector_i(int n)
{
     int *x;

     if ( n > 0)
     {
        x = (int *) calloc(n, sizeof(int));
        if (!x)
        {
           printf(" Memory ran out! Allocation failure in vector.\n");
           exit(EXIT_FAILURE);
        }
   }
   else
      printf(" Warning: The vector has dimension less or equal to zero.\n");
   return(x);
}

/***********************************************************************/

double **matrix_d(int n, int m)
{
     int i;
     double **x;

     if ((n > 0) && (m > 0))
     {
        x = (double **) calloc(n, sizeof(double*));
        if (!x)
        {
           printf(" Memory ran out! Allocation failure in matrix.\n");
           exit(EXIT_FAILURE);
        }
        for (i = 0; i < n; i++)
        {
            x[i] = (double *) calloc(m, sizeof(double));
            if (!x[i])
            {
               printf(" Memory ran out! Allocation failure in matrix.\n");
               exit(EXIT_FAILURE);
            }
        }
     }
     else
        printf(
        " Warning: One of the matrix dimension is less or equal to zero.\n");
     return(x);
}

/***********************************************************************/

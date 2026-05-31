/* Common Block Declarations */

struct klotz0_1_ {
    double buff[607];
    int ptr;
};

#define klotz0_1 (*(struct klotz0_1_ *) &klotz0_)
#define min(a,b) (a<b)?a:b

struct klotz1_1_ {
    double xbuff[1024];
    int first, xptr;
};

#define klotz1_1 (*(struct klotz1_1_ *) &klotz1_)

/* Initialized data */

struct {
    int fill_1[1214];
    int e_2;
    } klotz0_ = { {0}, 0 };

struct {
    double fill_1[1024];
    int e_2[2];
    double e_3;
    } klotz1_ = { {0}, 0, 0, 0. };


/* Table of constant values */

#define NPTS 5000

main()
{
    int seed;
    double a[NPTS];
    int p[NPTS];
    float t1, t2, tt[2];
    float etime_();
    extern int fischet_(), zufalli_(), normalt_(), zufallt_();


/* this program tests three random number generators: */

/*      zufallt = a uniform distribution of r.v.'s test */

/*      normalt  = a gaussian distribution of r. v.'s test */

/*      fischet  = poisson distribution of random numbers test */


/*  initialize the seeds for the uniform random number generator */

    seed = 0;
    t1 = etime_(tt);
    zufalli_(seed);
    t2 = etime_(tt);
    t1 = t2 - t1;

    printf("\n               ========================\n");
    printf("               ===== BEGIN TESTS  =====\n");
    printf("               ========================\n");
    printf("\n --------------------------------------\n");
    printf("  ***** Initialization of zufall *****\n");
    printf(" --------------------------------------\n");
    printf("    Initialization takes %e seconds\n",t1); 

/*  first do the uniform distribution test */

    printf("\n --------------------------------------\n");
    printf("  ***** Uniform distribution test ****\n");
    printf(" --------------------------------------\n");
    zufallt_(NPTS, a);

/*  next, the box-muller gaussian generator test */

    printf("\n --------------------------------------\n");
    printf("  **** Gaussian distribution test ****\n");
    printf(" --------------------------------------\n");
    normalt_(NPTS, a);

/*  finally, the poisson distribution test yields ints */

    printf("\n --------------------------------------\n");
    printf("  ***** Poisson distribution test ****\n");
    printf(" --------------------------------------\n");
    fischet_(NPTS, p);

    printf("\n               ========================\n");
    printf("               ===== END OF TESTS =====\n");
    printf("               ========================\n");

} /* MAIN__ */


int zufallt_(n, a)
int n;
double *a;
{
    /* Initialized data */

/* cycle time for Sun-4 */
    float cycle = 4.0e-8; 
/* cycle time for NEC SX-3 */
/*  float cycle = 2.9e-9; */
/* cycle time for Cray Y-MP */
/*  float cycle = 6.0E-9; */
/* cycle time for VP2200 */
/*  float cycle = 3.2E-9; */

    double diff;
    int nits;
    extern int zufallrs_(), zufallsv_();
    double b[607];
    int i, k;
    double svblk[608]; 
    float etime_();
    float t1, t2, t3, tt[2];
    int ia[20], ii;
    extern int zufall_();


/* number of iterations of test: nits */

    nits = 128;

    for (k = 0; k < 20; ++k) {
	ia[k] = 0;
    }

    t1 = 100.;
    for (k = 0; k < nits; ++k) {
	t2 = etime_(tt);
	zufall_(n, a);
	t3 = etime_(tt);
	t2 = t3 - t2;
	t1 = min(t2,t1);
	for (i = 0; i < n; ++i) {
	    ii = (int) (a[i] * (double)20.);
	    ++ia[ii];
	}

/*  last time, save klotz0 for save/resore test */

	if (k == nits - 2) {
	    zufallsv_(svblk);
	}
    }

/*  test save/restore sequence */

    zufallrs_(svblk);
    zufall_(607, b);
    diff = 0.;
    for (i = 0; i < (min(n,607)); ++i) {
	diff += (b[i] - a[i])*(b[i] - a[i]);
    }

    if (diff != 0.) {
	printf("ERROR in start/restart: diff = %e\n", diff);
    } else {
	printf("    zufall save/restore test OK \n");
    }

    t1 = t1 / ((float)n);
    printf("\n    time/uniform = %e \n",t1);
    t1 = t1 / cycle;
    printf("    Cps./uniform = %e \n",t1);
    printf("\n    Histogram of uniform distribution:\n");
    printf("    --------- -- ------- ------------ \n");
    for (k = 0; k < 20; ++k) {
        if(k<9) printf("    bin[%d]  = %d\n",k+1,ia[k]);
        else    printf("    bin[%d] = %d\n",k+1,ia[k]);
    }
    return 0;
} /* zufallt_ */


int normalt_(n, x)
int n;
double *x;
{
    double diff;
    int nits;
    extern int normalen_(), normalrs_(), normalsv_();
    int i, k;
    double y[128], boxsv[1634]; 
    float etime_();
    float t1, t2, t3; 
    float tt[2];
    double  x1, x2, x3, x4, x5, x6;
    int kk;
    double xx2, xx4;
    int bin[21];

/* cycle time for Sun-4 */
    float cycle = 4.0e-8; 
/* cycle for SX-3: */
/*  float cycle = 2.9e-9; */
/* cycle for Y-MP: */
/*  float cycle = 6.0E-9;  */
/* cycle for VP2200 */
/*  float cycle = 3.2E-9; */

/* number of iterations of test */

    nits = 128;

/* initialize moments */

    x1 = 0.;
    x2 = 0.;
    x3 = 0.;
    x4 = 0.;
    x5 = 0.;
    x6 = 0.;

    normalen_(n, x);
    for (i = 0; i < 21; ++i) {
	bin[i] = 0;
    }

    t1 = 100.;
    for (k = 0; k < nits; ++k) {

/*  save seeds and pointers for save/restore test */

	if (k == nits-1) {
	    normalsv_(boxsv);
	}

	t2 = etime_(tt);
	normalen_(n, x);
	t3 = etime_(tt);
	/* t2 = t3 - t2; */
	t1 = min(t1,t3-t2);

	for (i = 0; i < n; ++i) {
	    kk = (int) ((x[i] + 5.25) * 2.);
	    ++bin[kk];
	}
	for (i = 0; i < n; ++i) {
	    x1 += x[i];
	    xx2 = x[i] * x[i];
	    x2 += xx2;
	    x3 += xx2 * x[i];
	    xx4 = xx2 * xx2;
	    x4 += xx4;
	    x5 += xx4 * x[i];
	    x6 += xx4 * xx2;
	}

/*  restore previous seeds and pointers for save/restore test */

	if (k == nits-1) {
	    normalrs_(boxsv);
	    normalen_(128, y);
	}
    }

/*  save/restore check: */

    diff = 0.;
    for (i = 0; i < (min(n,128)); ++i) {
	diff += (y[i] - x[i])*(y[i] - x[i]);
    }
    if (diff != 0.) {
	printf("ERROR in normalsv/normalrs: diff = %e\n",diff);
    } else {
	printf("    normalen save/restore test OK\n");
    }

    x1 /= (double) (n * nits);
    x2 /= (double) (n * nits);
    x3 /= (double) (n * nits);
    x4 /= (double) (n * nits);
    x5 /= (double) (n * nits);
    x6 /= (double) (n * nits);

    t1 = t1 / ((float) n);
    printf("\n    Time/normal = %e seconds \n",t1);
    t1 = t1 / cycle;
    printf("    Cps./normal = %e \n\n",t1);
    printf("    Moments: \n");
    printf("      Compare to (0.0)               (1.0) \n");
    printf("              %e       %e \n",x1,x2);
    printf("      Compare to (0.0)               (3.0) \n");
    printf("              %e       %e \n",x3,x4);
    printf("      Compare to (0.0)              (15.0) \n");
    printf("              %e       %e \n",x5,x6);
    printf("\n    Histogram of gaussian distribution:\n");
    printf("    --------- -- --------- ------------ \n");
    for (k = 0; k < 21; ++k) {
        if(k<9) printf("    bin[%d]  = %d \n",k+1,bin[k]);
	else     printf("    bin[%d] = %d \n",k+1,bin[k]);
    }


    return 0;
} /* normalt_ */


int fischet_(n, p)
int n, *p;
{
    int nits, i, k;
    double p1,p2,p3,p4,x1,x2,x3,x4,fp;
    float t1,t2,t3;
    float tt[2];
    int kk;
    extern int fische_();
    double mu;
    int bin[20];

/* cycle time for Sun-4 */
    float cycle = 4.0e-8;
/* cycle for NEC SX-3: */
/*  float cycle = 2.9e-9; */
/* cycle for Y-MP: */
/*  float cycle = 6.0E-9; */
/* cycle for VP2200 */
/*  float cycle = 3.2E-9; */

    mu = 2.;
    nits = 128;

    for (k = 0; k < 20; ++k) {
	bin[k] = 0;
    }

/* moment comparison values */

    p1 = mu;
    p2 = mu + mu * mu;
    p3 = mu + mu * (double)3. * mu + mu * mu * mu;
    p4 = mu + 7.*mu*mu + 6.*mu*mu*mu + mu*mu*mu*mu;

    x1 = 0.;
    x2 = 0.;
    x3 = 0.;
    x4 = 0.;

    t1 = 10.;
    for (k = 0; k < nits; ++k) {

	t2 = etime_(tt);
	fische_(n, mu, p);
	t3 = etime_(tt);
	t2 = t3 - t2;
	t1 = min(t1,t2);

	for (i = 0; i < n; ++i) {
	    kk = p[i];
	    ++bin[kk];
	}

	for (i = 0; i < n; ++i) {
	    fp = (double) p[i];
	    x1 += fp;
	    x2 += fp * fp;
	    x3 += fp * fp * fp;
	    x4 += fp * fp * fp * fp;
	}

    }

    x1 /= (double) (n * nits);
    x2 /= (double) (n * nits);
    x3 /= (double) (n * nits);
    x4 /= (double) (n * nits);

    t1 = t1 / (float) n;
    printf("\n    Time/poisson = %e seconds \n",t1);
    t1 = t1 / cycle;
    printf("    Cps./poisson = %e \n\n",t1);
    printf("    Moments: \n"); 
    printf("       Compare: (%e)           (%e)\n",p1,p2);
    printf("                 %e             %e \n",x1,x2);
    printf("       Compare: (%e)           (%e)\n",p3,p4);
    printf("                 %e             %e \n",x3,x4);
    printf("\n    Histogram of Poisson distribution: mu = %e\n",mu);
    printf("    --------- -- ------- ------------ \n");
    for (k = 0; k < 20; ++k) {
        if(k<9) printf("    bin[%d]  = %d \n",k+1,bin[k]);
        else     printf("    bin[%d] = %d \n",k+1,bin[k]);
    }

    return 0;
} /* fischet_ */

/* ---------------- end of test programs ------------- */

int zufall_(n, a)
int n;
double *a;
{
    int buffsz = 607;

    int left, aptr, bptr, aptr0, i, k, q;
    double t;
    int nn, vl, qq, k273, k607, kptr;


/* portable lagged Fibonacci series uniform random number */
/* generator with "lags" -273 und -607: */
/* W.P. Petersen, IPS, ETH Zuerich, 19 Mar. 92 */


    aptr = 0;
    nn = n;

L1:

    if (nn <= 0) {
	return 0;
    }

/* factor nn = q*607 + r */

    q = (nn - 1) / 607;
    left = buffsz - klotz0_1.ptr;

    if (q <= 1) {

/* only one or fewer full segments */

	if (nn < left) {
            kptr = klotz0_1.ptr;
	    for (i = 0; i < nn; ++i) {
		a[i + aptr] = klotz0_1.buff[kptr + i];
	    }
	    klotz0_1.ptr += nn;
	    return 0;
	} else {
            kptr = klotz0_1.ptr;
#pragma _CRI ivdep
	    for (i = 0; i < left; ++i) {
		a[i + aptr] = klotz0_1.buff[kptr + i];
	    }
	    klotz0_1.ptr = 0;
	    aptr += left;
	    nn -= left;
/*  buff -> buff case */
	    vl = 273;
	    k273 = 334;
	    k607 = 0;
	    for (k = 0; k < 3; ++k) {
#pragma _CRI ivdep
		for (i = 0; i < vl; ++i) {
		   t = klotz0_1.buff[k273+i]+klotz0_1.buff[k607+i];
		   klotz0_1.buff[k607+i] = t - (double) ((int) t);
		}
		k607 += vl;
		k273 += vl;
		vl = 167;
		if (k == 0) {
		    k273 = 0;
		}
	    }
	    goto L1;
	}
    } else {

/* more than 1 full segment */

        kptr = klotz0_1.ptr;
#pragma _CRI ivdep
	for (i = 0; i < left; ++i) {
	    a[i + aptr] = klotz0_1.buff[kptr + i];
	}
	nn -= left;
	klotz0_1.ptr = 0;
	aptr += left;

/* buff -> a(aptr0) */

	vl = 273;
	k273 = 334;
	k607 = 0;
	for (k = 0; k < 3; ++k) {
	    if (k == 0) {
#pragma _CRI ivdep
		for (i = 0; i < vl; ++i) {
		    t = klotz0_1.buff[k273+i]+klotz0_1.buff[k607+i];
		    a[aptr + i] = t - (double) ((int) t);
		}
		k273 = aptr;
		k607 += vl;
		aptr += vl;
		vl = 167;
	    } else {
#pragma _CRI ivdep
		for (i = 0; i < vl; ++i) {
		    t = a[k273 + i] + klotz0_1.buff[k607 + i];
		    a[aptr + i] = t - (double) ((int) t);
		}
		k607 += vl;
		k273 += vl;
		aptr += vl;
	    }
	}
	nn += -607;

/* a(aptr-607) -> a(aptr) for last of the q-1 segments */

	aptr0 = aptr - 607;
	vl = 607;

	for (qq = 0; qq < q-2; ++qq) {
	    k273 = aptr0 + 334;
#pragma _CRI ivdep
	    for (i = 0; i < vl; ++i) {
		t = a[k273 + i] + a[aptr0 + i];
		a[aptr + i] = t - (double) ((int) t);
	    }
	    nn += -607;
	    aptr += vl;
	    aptr0 += vl;
	}

/* a(aptr0) -> buff, last segment before residual */

	vl = 273;
	k273 = aptr0 + 334;
	k607 = aptr0;
	bptr = 0;
	for (k = 0; k < 3; ++k) {
	    if (k == 0) {
#pragma _CRI ivdep
		for (i = 0; i < vl; ++i) {
		    t = a[k273 + i] + a[k607 + i];
		    klotz0_1.buff[bptr + i] = t - (double) ((int) t);
		}
		k273 = 0;
		k607 += vl;
		bptr += vl;
		vl = 167;
	    } else {
#pragma _CRI ivdep
		for (i = 0; i < vl; ++i) {
		    t = klotz0_1.buff[k273 + i] + a[k607 + i];
		    klotz0_1.buff[bptr + i] = t - (double) ((int) t);
		}
		k607 += vl;
		k273 += vl;
		bptr += vl;
	    }
	}
	goto L1;
    }
} /* zufall_ */


int zufalli_(seed)
int seed;
{
    /* Initialized data */

    int kl = 9373;
    int ij = 1802;

    /* Local variables */
    int i, j, k, l, m;
    double s, t;
    int ii, jj;


/*  generates initial seed buffer by linear congruential */
/*  method. Taken from Marsaglia, FSU report FSU-SCRI-87-50 */
/*  variable seed should be 0 < seed <31328 */


    if (seed != 0) {
	ij = seed;
    }

    i = ij / 177 % 177 + 2;
    j = ij % 177 + 2;
    k = kl / 169 % 178 + 1;
    l = kl % 169;
    for (ii = 0; ii < 607; ++ii) {
	s = 0.;
	t = .5;
	for (jj = 1; jj <= 24; ++jj) {
	    m = i * j % 179 * k % 179;
	    i = j;
	    j = k;
	    k = m;
	    l = (l * 53 + 1) % 169;
	    if (l * m % 64 >= 32) {
		s += t;
	    }
	    t *= (double).5;
	}
	klotz0_1.buff[ii] = s;
    }
    return 0;
} /* zufalli_ */


int zufallsv_(svblk)
double *svblk;
{
    int i;


/*  saves common blocks klotz0, containing seeds and */
/*  pointer to position in seed block. IMPORTANT: svblk must be */
/*  dimensioned at least 608 in driver. The entire contents */
/*  of klotz0 (pointer in buff, and buff) must be saved. */


    /* Function Body */
    svblk[0] = (double) klotz0_1.ptr;
#pragma _CRI ivdep
    for (i = 0; i < 607; ++i) {
	svblk[i + 1] = klotz0_1.buff[i];
    }

    return 0;
} /* zufallsv_ */

int zufallrs_(svblk)
double *svblk;
{
    int i;


/*  restores common block klotz0, containing seeds and pointer */
/*  to position in seed block. IMPORTANT: svblk must be */
/*  dimensioned at least 608 in driver. The entire contents */
/*  of klotz0 must be restored. */


    klotz0_1.ptr = (int) svblk[0];
#pragma _CRI ivdep
    for (i = 0; i < 607; ++i) {
	klotz0_1.buff[i] = svblk[i + 1];
    }

    return 0;
} /* zufallrs_ */

int normalen_(n, x)
int n;
double *x;
{
    int buffsz = 1024;

    /* Local variables */
    int left, i, nn, ptr, kptr;
    extern int normal00_();
/* Box-Muller method for Gaussian random numbers */

    nn = n;
    if (nn <= 0) {
	return 0;
    }
    if (klotz1_1.first == 0) {
	normal00_();
	klotz1_1.first = 1;
    }
    ptr = 0;

L1:
    left = buffsz - klotz1_1.xptr;
    if (nn < left) {
        kptr = klotz1_1.xptr;
#pragma _CRI ivdep
	for (i = 0; i < nn; ++i) {
	    x[i + ptr] = klotz1_1.xbuff[kptr + i];
	}
	klotz1_1.xptr += nn;
	return 0;
    } else {
        kptr = klotz1_1.xptr;
#pragma _CRI ivdep
	for (i = 0; i < left; ++i) {
	    x[i + ptr] = klotz1_1.xbuff[kptr + i];
	}
	klotz1_1.xptr = 0;
	ptr += left;
	nn -= left;
	normal00_();
	goto L1;
    }
} /* normalen_ */

int normal00_()
{
    /* Builtin functions */
    double cos(), sin(), log(), sqrt();

    /* Local variables */
    int i;
    double twopi, r1, r2, t1, t2;
    extern int zufall_();

    twopi = 6.2831853071795862;
    zufall_(1024, klotz1_1.xbuff);
#pragma _CRI ivdep
    for (i = 0; i < 1023; i += 2) {
	r1 = twopi * klotz1_1.xbuff[i];
	t1 = cos(r1);
	t2 = sin(r1);
	r2 = sqrt(-2.*(log(1. - klotz1_1.xbuff[i+1])));
	klotz1_1.xbuff[i]   = t1 * r2;
	klotz1_1.xbuff[i+1] = t2 * r2;
    }

    return 0;
} /* normal00_ */

int normalsv_(svbox)
double *svbox;
{
    extern int zufallsv_();
    int i, k;


/*  IMPORTANT: svbox must be dimensioned at */
/*  least 1634 in driver. The entire contents of blocks */
/*  klotz0 (via zufallsv) and klotz1 must be saved. */

    if (klotz1_1.first == 0) {
	printf("ERROR in normalsv, save of uninitialized block\n");
    }

/*  save zufall block klotz0 */

    zufallsv_(svbox);

    svbox[608] = (double) klotz1_1.first;
    svbox[609] = (double) klotz1_1.xptr;
    k = 610;
#pragma _CRI ivdep
    for (i = 0; i < 1024; ++i) {
	svbox[i + k] = klotz1_1.xbuff[i];
    }

    return 0;
} /* normalsv_ */

int normalrs_(svbox)
double *svbox;
{
    /* Local variables */
    extern int zufallrs_();
    int i, k;

/*  IMPORTANT: svbox must be dimensioned at */
/*  least 1634 in driver. The entire contents */
/*  of klotz0 and klotz1 must be restored. */

/* restore zufall blocks klotz0 and klotz1 */

    zufallrs_(svbox);
    klotz1_1.first = (int) svbox[608];
    if (klotz1_1.first == 0) {
      printf("ERROR in normalrs, restoration of uninitialized block\n");
    }
    klotz1_1.xptr = (int) svbox[609];
    k = 610;
#pragma _CRI ivdep
    for (i = 0; i < 1024; ++i) {
	klotz1_1.xbuff[i] = svbox[i + k];
    }

    return 0;
} /* normalrs_ */

int fische_(n, mu, p)
int n;
double mu;
int *p;
{

    /* Builtin functions */
    double exp();

    /* Local variables */
    int left, indx[1024], i, k;
    double q[1024], u[1024];
    int nsegs, p0;
    double q0;
    int ii, jj;
    extern /* Subroutine */ int zufall_();
    int nl0;
    double pmu;

/* Poisson generator for distribution function of p's: */

/*    q(mu,p) = exp(-mu) mu**p/p! */

/* initialize arrays, pointers */


    /* Function Body */
    if (n <= 0) {
	return 0;
    }

    pmu = exp(-mu);
    p0 = 0;

    nsegs = (n - 1) / 1024;
    left = n - (nsegs << 10);
    ++nsegs;
    nl0 = left;

    for (k = 0; k < nsegs; ++k) {

	for (i = 0; i < left; ++i) {
	    indx[i] = i;
	    p[p0 + i] = 0;
	    q[i] = 1.;
	}

/* Begin iterative loop on segment of p's */

L1:

/* Get the needed uniforms */

	zufall_(left, u);

	jj = 0;

	for (i = 0; i < left; ++i) {
	    ii = indx[i];
	    q0 = q[ii] * u[i];
	    q[ii] = q0;
	    if (q0 > pmu) {
		indx[jj++] = ii;
		++p[p0 + ii];
	    }
	}

/* any left in this segment? */

	left = jj;
	if (left > 0) {
	    goto L1;
	}

	p0  += nl0;
	nl0  = 1024;
	left = 1024;

    }

    return 0;
} /* fische_ */

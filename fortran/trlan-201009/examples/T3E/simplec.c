/* a C version of simple77.f */
#include <stdio.h>
#include "mpi.h"

/* This set of parameters tell TRLAN to compute 5 (NED) smallest
   (LOHI=-1) of a 100 x 100 (NROW) matrix by using a Krylov subspace
   basis size of 30 (MAXLAN).
   MEV defines the size of the arrays used to store computed solutions,
   eval, evec.
*/
#define NROW	100
#define LOHI	-1
#define NED	5
#define MAXLAN	30
#define MEV	10

/* global variables */
int my_pe;

/* prototype of operator (matrix-vector multiplication routine */
void diag_op (int *, int *, double *, int *, double *, int *);

main (int argc, char **argv) {
  int i, lwrk, nrow, mev, ipar[32];
  double tt, to, tp, tr;
  double eval[MEV], evec[NROW*MEV], wrk[MAXLAN*(MAXLAN+10)];

  nrow = NROW; mev = MEV;
  lwrk = MAXLAN*(MAXLAN+10);
  i = MPI_Init(&argc, &argv);
  i = MPI_Comm_rank(MPI_COMM_WORLD, &my_pe);
  ipar[0] = 0;
  ipar[1] = LOHI;
  ipar[2] = NED;
  ipar[3] = 0;     /* nec = 0 */
  ipar[4] = MAXLAN;
  ipar[5] = 1;     /* restarting scheme 1 */
  ipar[6] = 2000;  /* maximum number of MATVECs */
  ipar[7] = (int)MPI_COMM_WORLD;
  ipar[8] = -15;   /* verboseness */
  ipar[9] = 99;    /* Fortran IO unit number used to write log messages */
  ipar[10] = 1;    /* iguess -- 1 = use supply the starting vector */
  ipar[11] = -5;   /* checkpointing flag -- write about 5 times */
  ipar[12] = 98;   /* Fortran IO unit number used to write checkpoint files */
  ipar[13] = 3*NROW;/* number of floating-point operations per MATVEC per PE */
  for (i=0; i<MEV; ++i) eval[i]=0.0;
  for (i=0; i<NROW; ++i) evec[i] = 1.0;
  wrk[0] = 1.4901e-8; /* relative tolerance on residual norms */
  /* call Fortran function TRLAN77 */
#ifdef CRAY
  TRLAN77(diag_op, ipar, &nrow, &mev, eval, evec, &nrow, wrk, &lwrk);
#else
  trlan77_(diag_op, ipar, &nrow, &mev, eval, evec, &nrow, wrk, &lwrk);
#endif
  /* convert the time from clock ticks to seconds */
  tt = ((double) ipar[28]) * 1e-3;
  tp = ((double) ipar[29]) * 1e-3;
  to = ((double) ipar[30]) * 1e-3;
  tr = ((double) ipar[31]) * 1e-3;
  /* printing the result -- this is replaced with a trl_print_info call in trlan77
  if (my_pe == 0) {
    printf("MAXLAN:%10d, Restart:%10d,   NED:  -%7d,     NEC:%10d\n",
	   ipar[4], ipar[5], ipar[2], ipar[3]);
    printf("MATVEC:%10d,  Reorth:%10d, Nloop:%10d, Nlocked:%10d\n",
	   ipar[24], ipar[26], ipar[25], ipar[23]);
    printf("Ttotal:%10.6f,    T_op:%10.6f, Torth:%10.6f,  Tstart:%10.6f\n",
	   tt, tp, to, tr);
    for (i=0; i<ipar[3]; i++)
      printf("E(%i) = %25.17g %16.4e\n", i+1, eval[i], wrk[i]);
  }
  */
  i = MPI_Finalize();
}	/* end of main */
/*
  The operator (matrix-vector multiplication routine)
  the matrix is a diagonal matrix with values (1, 4, 9, 16, 25, ....)
  */
void diag_op(int *nrow, int *ncol, double *xin, int *ldx, double *yout,
	     int *ldy)
{
  int i, j, doff;
  double  *xtmp, *ytmp;

  doff = my_pe * NROW;
  for (j=0; j<*ncol; ++j) {
    xtmp = xin + *ldx * j;
    ytmp = yout + *ldy * j;
    for (i=1; i<=*nrow; ++i, ++xtmp, ++ytmp)
      *ytmp = *xtmp * ((doff+i)*(doff+i));
  }
} /* end of function diag_op */

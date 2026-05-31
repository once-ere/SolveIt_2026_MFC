C $Id: hbpsp.f,v 1.1.1.1 1998/08/17 19:03:53 kewu Exp $
C-----------------------------------------------------------------------
C This file has been primarily used on Cray MPP machines by the author.
C It used Cray Fortran Pointers to do dynamic memory allocation
C (actually many Fortran compilers support this) and MPI to perform
C inter-processor communication (MPI is also support on many distributed
C platforms).
C
C This is a test program that reads a Harwell-Boeing matrix and compute
C a few extreme eigenvalues and the corresponding eigenvectors using
C TRLAN program.
C-----------------------------------------------------------------------
      program hbpsp
      implicit none
c     parameters that defines the eigenvale problem and the resources
c     given to TRLAN
c     LOHI     -- which end of the spectrum to compute
c     NED      -- number of eigenvalues
c     MAXLAN   -- the basis size used by TRLAN
c     IRESTART -- the restart scheme (1, 2, 3, 4)
c     MAXMV    -- the maximum number of MATVEC allowed
c     VERBOSE  -- verboseness. <=0, log nothing
c     LOG_IO   -- the IO unit number used for loggin
c     CPFLAG   -- the flag used to control checkpointing.
c                 <=0, no checkpointing
c     CPIO     -- the IO unit number used for writing checkpoint files
c     tol      -- the relative tolerance on residual norms
      integer lohi, ned, maxlan, irestart, maxmv, verbose, log_io,
     &     pflag, cpio
      double precision tol
      parameter (lohi=-1, ned=5, maxlan=30, irestart=1)
      parameter (verbose=10, log_io=99, cpflag=5, cpio = 98)
      parameter (maxmv=ned*1000, tol=1.0D-8)
      include 'mpif.h'
      include 'simphb.h'
c
c     local variables
c
      character*128 hbfile
      integer ierr, mpicom, totprocs, myproc
      integer ipar(32)
c
c     start MPI
c
      call mpi_init(ierr)
      IF (IERR .NE. MPI_SUCCESS) STOP 'FAILED TO INIT MPI'
      MPICOM = MPI_COMM_WORLD
      call mpi_comm_size(mpicom, totprocs, ierr)
      call mpi_comm_rank(mpicom, myproc, ierr)
      if (totprocs.gt.MAX_PROCS) then
         print *, 'Increase MAX_PROCS in file simphb.h and PSPARSLIB.'
         call mpi_abort(mpicom, ierr)
      endif
      if (myproc.eq.0) print *, 'SIMPHB'
c
c     the file name can be given as a command line argument
c
      if (iargc().gt.0) then
c         call getarg(1, hbfile)    ! NON-CRAY
         call pxfgetarg(1, hbfile, i, ierr)  ! Cray/SGI
      else if (myproc.eq.0) then
         print *, 'Please input the Boeing-Harwell file name:'
         read *, hbfile
      endif
c
c     read the given file and distribute the matrix
c
      setup_time = mpi_wtime()
      call hbread1(hbfile, 0, myproc, totprocs, nloc, split,
     &     paloc, pjaloc, pialoc, maxnzloc,
     &     maxnloc, iou, mpicom, ierr, msglvl)
      if (ierr.ne.0) then
         print *, 'HBREAD1 failed with error code ', ierr
         call mpi_abort(mpicom, ierr)
      endif
c
c     prepare the matrix for distributed matrix-vector multiplication
c
      call psp_setup(maxnloc, maxnzloc, nloc, myproc, totprocs, split,
     &     aloc, jaloc, ialoc, pord, nbnd, nproc, proc, pix, ipr,
     &     mpicom, ierr)
      if (ierr.ne.0) then
         print *, 'PSP_SETUP failed with error code ', ierr
         call mpi_abort(mpicom, ierr)
      endif

      program shbpsp
      implicit none
c
c     This program performs the following tasks:
c     (1) Read a symmetric matrix stored in Harwell-Boeing RUA format
c     and distributed the matrix to aprocessors in MPI_COMM_WORLD.
c     (2) Call TRLAN77 to compute a few largest (or smallest)
c     eigenvalues and their corresponding eigenvectors.
c
c     The matrix-vector multiplication routine from PSPARSLIB is used.
c     Dynamic memory allocation is used through-out the preprocessing
c     stages.
c
      include 'mpif.h'
c
c     the arrays for the distrubited MATVEC (amxdis) is stored in
c     the following header file
c
      include 'shbpsp.h'
c
c     parameters defining the eigenvalue problem
c
      integer lohi, ned, maxlan, maxmatvec, lwrk
      parameter (lohi=-1, ned=5, maxlan=ned+20, maxmatvec=5000)
      parameter (lwrk = maxlan*(maxlan+ned+14))
      integer iou, msglvl, pspamxtype, psptrn
      parameter (iou=17, pspamxtype=29, psptrn=1, msglvl=0)
      real*8 one, tol
      parameter(one=1.0D0, tol=1.0D-8)
c
c     locally declared variables
c
      external opsp
      character*128 hbfile
      integer iargc,mpicom,ierr
      integer i,j,maxnzloc,maxnloc,nloc,nzloc,mev,ldq
      real*8 setup_time, trlan_time, tt, tp, to, tr
      integer ord(*),split(MAX_PROCS+1),ipar(32)
      real*8 wrk(lwrk), ritz(maxlan+1), qq(*)
      pointer (pord,ord), (pqq, qq)
c
c.... clear the pointers
      paloc = 0
      pjaloc = 0
      pialoc = 0
      pix = 0
      pord = 0
      pqq = 0
      amxtype = pspamxtype
      ptrn = psptrn
c
c     start MPI
c
      call mpi_init(ierr)
      IF (IERR .NE. MPI_SUCCESS) STOP 'FAILED TO INIT MPI'
      MPICOM = MPI_COMM_WORLD
      call mpi_comm_size(mpicom, totprocs, ierr)
      call mpi_comm_rank(mpicom, myproc, ierr)
      if (totprocs.gt.MAX_PROCS) then
         print *, 'Increase MAX_PROCS in file shbpsp.h and PSPARSLIB.'
         call mpi_abort(mpicom, ierr)
      endif
      if (myproc.eq.0) print *, 'SHBPSP'
c
c     the file name can be given as a command line argument
c
      if (iargc().gt.0) then
         call getarg(1, hbfile)
      else
         print *, 'the Boeing-Harwell file name must be provided'
         call MPI_abort(MPICOM, ierr)
      endif
c
c     read the given file and distribute the matrix
c
      setup_time = mpi_wtime()
      call hbread3(hbfile, 0, myproc, totprocs, nloc, split,
     &     paloc, pjaloc, pialoc, maxnzloc,
     &     maxnloc, iou, mpicom, ierr, msglvl)
      if (ierr.ne.0) then
         print *, 'HBREAD3 failed with error code ', ierr
         call mpi_abort(mpicom, ierr)
      endif
      nzloc = ialoc(nloc+1) - ialoc(1)
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
c
c     the workspace for LANSO is allocated to fill up to
c     the maximum memory size available
c
      ldq = ((nloc+3)/4)*4
      mev = maxlan+1
      call dmalloc(pqq, ldq*mev, 8)
      if (pqq .eq. 0) then
         print *, 'SHBPSP: failed to allocate workspace.'
         call mpi_abort(mpicom, ierr)
      endif
c
c     the initial guess for the eigenvector is always [1,1,...,1]^T
c
      do 10 i = 1, nloc
         qq(i) = one
 10   continue
      ipar(1) = 0
      ipar(2) = lohi
      ipar(3) = ned
      ipar(4) = 0               ! nec = 0
      ipar(5) = maxlan
      ipar(6) = 3               ! restarting scheme
      ipar(7) = maxmatvec
      call MPI_COMM_DUP(MPI_COMM_WORLD, ipar(8), ierr)
      if (ierr .ne. MPI_SUCCESS) call MPI_ABORT(MPI_COMM_WORLD, ierr)
      ipar(9) = msglvl          ! verboseness
      ipar(10) = iou            ! log IO unit number
      ipar(11) = 1              ! iguess
      ipar(12) = -5              ! checkpoint flag, write 5 times
      ipar(13) = iou+1          ! unit number used to write checkpoint files
      ipar(14) = nzloc+nzloc    ! number of FLOPS per MATVEC on this PE
      wrk(1) = tol              ! relative tolerance on residual norms
      if (myproc.eq.0 .and. msglvl.gt.0) then
         write (*, '(/A$)') 'SHBPSP: '
         print *, 'NPROCS = ', totprocs, '	MAXLAN = ', maxlan
         if (msglvl.gt.5) then
            print *, 'Matrix distribution:', split(1),
     &           ('--', split(i), i=2,totprocs+1)
         endif
      endif
      trlan_time = mpi_wtime()
      setup_time = trlan_time - setup_time
c
c     call TRLAN77 to compute the eigenvalues
c
      call trlan77(opsp, ipar, nloc, mev, ritz, qq, ldq, wrk, lwrk)
      trlan_time = mpi_wtime() - trlan_time
c
      tt = dble(ipar(29))*1D-3
      tp = dble(ipar(30))*1D-3
      to = dble(ipar(31))*1D-3
      tr = dble(ipar(32))*1D-3
      if (myproc .eq. 0) then
         if (ipar(1).ne.0) print *, 'TRLAN77 exist flag = ', ipar(1)
         print *, 'Setup time:', setup_time,
     &        ',  TRLAN77 time:', trlan_time
         if (ipar(2) .lt. 0) then
            print 100, ipar(5), ipar(6), '-', ipar(3), ipar(4)
         else if (ipar(2) .gt. 0) then
            print 100, ipar(5), ipar(6), '+', ipar(3), ipar(4)
         else
            print 100, ipar(5), ipar(6), '0', ipar(3), ipar(4)
         end if
         print 200, ipar(25), ipar(27), ipar(26), ipar(24)
         if (tt .gt. 1D-3 .and. tt.lt.1D3) then
            print 400, tt, tp, to, tr
         else
            print 300, tt, tp, to, tr
         end if
         if (ipar(4).le.0) ipar(4) = min(ned, mev-1)
         print 500, (i, ritz(i), wrk(i), i=1,ipar(4))
      end if
 100  Format('MAXLAN:', I10, ', Restart:', I10, ',   NED:', 2X, A1,
     &     I7, ',     NEC:', I10)
 200  Format('MATVEC:', I10, ',  Reorth:', I10, ', Nloop:', I10,
     &     ', Nlocked:', I10)
 300  Format('Ttotal:', 1P, E10.2, ',    T_op:', E10.2, ', Torth:',
     &        E10.2, ',  Tstart:', E10.2)
 400  Format('Ttotal:', F10.6, ',    T_op:', F10.6, ', Torth:',
     &        F10.6, ',  Tstart:', F10.6)
 500  Format('E(', I2, ') = ', 1P, G25.17, E16.4)
c
      call mpi_finalize(ierr)
c$$$      stop 'DONE'
      end
c-----------------------------------------------------------------------
      subroutine opsp(nrow, ncol, xin, ldx, yout, ldy)
      implicit none
      integer nrow, ncol, ldx, ldy, i
      real*8 xin(ldx,ncol), yout(ldy,ncol)
      do i = 1, ncol
         call op1(nrow, xin(1,i), yout(1,i))
      end do
      return
      end subroutine opsp
      subroutine op1(n,q,r)
      implicit none
      integer n, length, lentmp, mpicom
      real*8 p(n), q(n), r(n), xtmp(*), ytmp(*)
      pointer (pxtmp, xtmp), (pytmp, ytmp)
      include 'mpif.h'
      include 'shbpsp.h'
      save pxtmp, pytmp, length
      data pxtmp, pytmp, length/3*0/
c
      if (totprocs.gt.1) then
c
c     allocte enough temporary workspace before calling amxdis
c
         lentmp = n+max(ipr(nproc+1), ipr(nproc+2))
         if (length.lt.lentmp) then
            if (length.gt.0) then
               call dmfree(pxtmp)
               call dmfree(pytmp)
            endif
            call dmalloc(pxtmp,lentmp,8)
            call dmalloc(pytmp,lentmp,8)
            if (pxtmp.eq.0 .or. pytmp.eq.0)
     &           call MPI_abort(MPI_COMM_WORLD,n)
            length = lentmp
         endif
         call dcopy(n,q,1,xtmp,1)
         call amxdis(n,nbnd,xtmp,ytmp,aloc,jaloc, ialoc, nproc, proc,
     &     ix, ipr, amxtype, length, ptrn)
         call dcopy(n,ytmp,1,r,1)
         if (xtmp(length+1).ne.0 .or. ytmp(length+1).ne.0) then
            print *, 'Guard elements of xtmp and ytmp are:',
     &           xtmp(length+1), ytmp(length+1)
         endif
      else
         call amux(n,q,r,aloc,jaloc,ialoc)
      endif
      return
c     end subroutine op1
      end

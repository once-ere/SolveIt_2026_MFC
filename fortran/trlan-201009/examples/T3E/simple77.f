                                !!! a really simple test for trlanf77
      Program simple
      implicit none
      Include 'mpif.h'
      ! This set of parameters tell TRLAN to compute 5 (NED) smallest
      !(LOHI=-1) of a 100 x 100 (NROW) matrix by using a Krylov subspace
      ! basis size of 30 (MAXLAN).
      ! MEV defines the size of the arrays used to store computed
      ! solutions, eval, evec.
      Integer, Parameter :: nrow=100, lohi=-1, ned=5, maxlan=30
      Integer, Parameter :: mev=10, lwrk=maxlan*(maxlan+10)
                                ! local variable declaration
      double precision :: eval(mev), evec(nrow, mev), wrk(lwrk)
      double precision :: tt, tp, tr, to
      integer :: my_pe, i, ipar(32)
      external diag_op
                                ! initialize MPI
      Call MPI_INIT(i)
      ipar(1) = 0
      ipar(2) = lohi
      ipar(3) = ned
      ipar(4) = 0               ! nec = 0
      ipar(5) = maxlan
      ipar(6) = 1               ! restarting scheme 1
      ipar(7) = 2000
      ipar(8) = MPI_COMM_WORLD
      ipar(9) = -15             ! verboseness
      ipar(10) = 99             ! log IO unit number
      ipar(11) = 1              ! iguess
      ipar(12) = -5             ! checkpoint flag, write 5 times
      ipar(13) = 98             ! unit number used to write checkpoint files
      ipar(14) = 3*nrow         ! number of FLOPS per MATVEC on this PE
      wrk(1) = 1.4901D-8        ! relative tolerance on residual norms
      eval = 0.0D0
      evec(1:nrow,1) = 1.0D0
                                ! call TRLAN to compute the eigenvalues
      Call trlan77(diag_op, ipar, nrow, mev, eval, evec, nrow, wrk,
     &     lwrk)
c
c     the following printing statements mimics trl_terse_info
c     they have been replaced with a call to trl_print_info inside
c     trlan77
c
c$$$      call MPI_COMM_RANK(MPI_COMM_WORLD, my_pe, i)
c$$$      tt = dble(ipar(29))*1D-3
c$$$      tp = dble(ipar(30))*1D-3
c$$$      to = dble(ipar(31))*1D-3
c$$$      tr = dble(ipar(32))*1D-3
c$$$      if (my_pe .eq. 0) then
c$$$         if (ipar(2) .lt. 0) then
c$$$            print 100, ipar(5), ipar(6), '-', ipar(3), ipar(4)
c$$$         else if (ipar(2) .gt. 0) then
c$$$            print 100, ipar(5), ipar(6), '+', ipar(3), ipar(4)
c$$$         else
c$$$            print 100, ipar(5), ipar(6), '0', ipar(3), ipar(4)
c$$$         end if
c$$$         print 200, ipar(25), ipar(27), ipar(26), ipar(24)
c$$$         if (tt .gt. 1D-3 .and. tt.lt.1D3) then
c$$$            print 400, tt, tp, to, tr
c$$$         else
c$$$            print 300, tt, tp, to, tr
c$$$         end if
c$$$         if (ipar(4).le.0) ipar(4) = min(ned, mev-1)
c$$$         print 500, (i, eval(i), wrk(i), i=1,ipar(4))
c$$$      end if
c$$$ 100  Format('MAXLAN:', I10, ', Restart:', I10, ',   NED:', 2X, A1,
c$$$     &     I7, ',     NEC:', I10)
c$$$ 200  Format('MATVEC:', I10, ',  Reorth:', I10, ', Nloop:', I10,
c$$$     &     ', Nlocked:', I10)
c$$$ 300  Format('Ttotal:', 1PE10.2, ',    T_op:', 1PE10.2, ', Torth:',
c$$$     &        1PE10.2, ',  Tstart:', 1PE10.2)
c$$$ 400  Format('Ttotal:', F10.6, ',    T_op:', F10.6, ', Torth:',
c$$$     &        F10.6, ',  Tstart:', F10.6)
c$$$ 500  Format('E(', I2, ') = ', 1P, G25.17, E16.4)
      Call MPI_finalize(i)
      End
c!!
c a simple matrix-vector multiplications routine
c defines a diagonal matrix with values (1, 4, 9, 16, 25, ...)
c!!
      Subroutine diag_op(nrow, ncol, xin, ldx, yout, ldy)
      Implicit None
      Integer, Intent(in) :: nrow, ncol, ldx, ldy
      Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
      Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
      include 'mpif.h'
                                ! local variables
      integer :: i, j, ioff, joff, doff
      call MPI_COMM_RANK(MPI_COMM_WORLD, i, j)
      doff = nrow*i
      Do j = 1, ncol
         ioff = (j-1)*ldx
         joff = (j-1)*ldy
         Do i = 1, nrow
            yout(joff+i) = (doff+i)**2*xin(ioff+i)
         End Do
      End Do
      End

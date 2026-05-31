! $Id: trl_comm_mpi.f90,v 1.7 2001/09/06 17:13:38 kewu Exp $
!!!
! This file contains all routines that performs data communication. 
! MPI functions are called to perform the actual data exchange.
!!!
! Initialization routine -- initializes a TRL_INFO variable.
! *********
! MUST BE CALLED before calling any other user level routine in TRLAN
! package
! *********
!
! Arguments:
! info   -- the information package to be used by TRLAN
! nrow   -- local dimension of the problem
! mxlan  -- maximum number of basis vectors to be used
! lh     -- which end of the spectrum to compute
!           <0 : lower end, the smallest eigenvalues
!           >0 : high end, the largest eigenvalues
!            0 : either lower and or high end, whoever converges
!                first 
! ned    -- number of wanted eigenvalues and eigenvectors
! tol    -- (optional) tolerance on residual norm,
!           default: sqrt(epsilon)
! trestart-- (optional) thick-restarting scheme, 1-4,
!           default: 1
! mxmv   -- (optional) maximum number of matrix-vector multiplications
!           allowed
!           default: info%ntot*info%ned
! mpicom -- (optional) the MPI communicator,
!           default: a duplicate of MPI_COMM_WORLD
!           in sequential case, this is set to 0.
!!!
Subroutine trl_init_info(info, nrow, mxlan, lh, ned, tol, trestart,&
     & mxmv, mpicom)
  Use trl_info
  Implicit None 
  Integer, Intent(in) :: nrow, mxlan, lh, ned
  Integer, Intent(in), Optional :: mpicom, mxmv, trestart
  Real(8), Intent(in), Optional :: tol
  Type(TRL_INFO_T), Intent(out) :: info
  Include 'mpif.h'
  Integer :: id, ie
  info%maxlan = mxlan
  If (mxlan <= ned) Then
     info%maxlan = ned + Max(ned, 6)
  End If
  info%lohi = lh
  info%ned = ned
  If (Present(mpicom)) Then
     info%mpicom = mpicom
  Else
     Call MPI_COMM_DUP(MPI_COMM_WORLD, info%mpicom, info%stat)
     If (info%stat .Eq. MPI_SUCCESS) Then
        info%stat = 0
     Else
        Print *, 'TRL_INIT_INFO: unable to duplication',&
             & ' MPI_COMM_WORLD.'
        Print *, 'This probably indicate MPI_INIT is not called.'
        Stop
     End If
  End If
  info%nloc = nrow
  Call MPI_Allreduce(nrow, info%ntot, 1, MPI_INTEGER, MPI_SUM,&
       & info%mpicom, info%stat)
  If (info%stat .Eq. MPI_SUCCESS) Then
     info%stat = 0
  Else
     Print *, 'TRL_INIT_INFO: failed to perform MPI_ALLREDUCE, ierr=',&
          & info%stat
     info%stat = -1
     Return
  End If
  info%guess = 0
  If (Present(mxmv)) Then
     info%maxmv = mxmv
  Else
     info%maxmv = Min(Max(info%ntot, 1000), 1000*info%ned)
  End If
  If (Present(trestart)) Then
     info%restart = trestart
  Else
     info%restart = 0
  End If
  If (Present(tol)) Then
     If (tol .Le. Tiny(tol)) Then
        info%tol = Epsilon(tol)
     Else If (tol .Ge. 1D0) Then
        info%tol = Min(0.1D0, 1D0/tol)
     Else
        info%tol = tol
     End If
  Else
     info%tol = Sqrt(Epsilon(info%tol))
  End If
  info%nec = 0
  info%locked = info%nec
  info%matvec = 0
  info%nloop = 0
  info%north = 0
  info%nrand = 0
  info%flop = 0
  info%rflp = 0
  info%flop_h = 0
  info%rflp_h = 0
  info%flop_r = 0
  info%rflp_r = 0
  info%clk_max = -1
  info%clk_rate = 0
  info%clk_tot = 0
  info%clk_op = 0
  info%clk_orth = 0
  info%clk_res = 0
  info%tick_t = 0
  info%tick_o = 0
  info%tick_h = 0
  info%tick_r = 0
  info%clk_in = 0
  info%clk_out = 0
  info%wrds_in = 0
  info%wrds_out = 0
  info%verbose = 0
  info%log_io = 99
  info%log_file = 'TRL_LOG_'
  info%stat = 0
  info%anrm = 0
  info%tmv = -1
  info%trgt = - Huge(info%anrm)
  info%tres = 0.0D0
  info%crat = 1.0D0
  info%cpflag = 0
  info%cpio = 98
  info%cpfile = 'TRL_CHECKPOINT_'
  info%oldcpf = ''
  Call MPI_COMM_RANK(info%mpicom, info%my_pe, info%stat)
  If (info%stat .Ne. MPI_SUCCESS) Then
     Print *, 'TRL_INIT_INFO: failed to perform MPI_COMM_RANK, ierr=',&
          & info%stat
     info%stat = -2
     Return
  End If
  Call MPI_COMM_SIZE(info%mpicom, info%npes, info%stat)
  If (info%stat .Ne. MPI_SUCCESS) Then
     Print *, 'TRL_INIT_INFO: failed to perform MPI_COMM_SIZE, ierr=',&
          & info%stat
     info%stat = -3
     Return
  End If
  Call MPI_Allreduce(info%stat, id, 1, MPI_INTEGER, MPI_MIN,&
       & info%mpicom, ie)
  If (ie .Eq. MPI_SUCCESS) Then
     info%stat = id
  Else
     Print *, 'TRL_INIT_INFO: failed to perform MPI_ALLREDUCE, ierr=',&
          & info%stat
     info%stat = -1
  End If
  Return
End Subroutine trl_init_info
!****** The rest are lower level working routines ******
!!!
! trl_g_sum performs global sum in the parallel environment.
! Calls MPI_ALLREDUCE to perform the actual task.
! need this wrapper to avoid actually linking with MPI library
! when MPI is not used.
!!!
Subroutine trl_g_sum(mpicom, nelm, x, y)
  Implicit None
  Integer, Intent(in) :: mpicom, nelm
  Real(8) :: x(nelm), y(nelm)
  Include 'mpif.h'
  ! local variables
  Integer :: mpitype, ierr
  If (MPI_REAL8.Ne.0) Then
     mpitype = MPI_REAL8
  Else
     mpitype = MPI_DOUBLE_PRECISION
  End If
  Call MPI_Allreduce(x, y, nelm, mpitype, MPI_SUM, mpicom, ierr)
  x(1:nelm) = y(1:nelm)
  If (ierr .Ne. MPI_SUCCESS) Then
     Print *, 'TRL_G_SUM: MPI_ALLREDUCE failed with error code ', ierr
     Call MPI_ABORT(mpicom, ierr)
  End If
  Return
End Subroutine trl_g_sum
!!!
! trl_sync_flag
! given an integer value, this function returns the minimum value of
! all the PEs
!!!
Function trl_sync_flag(mpicom, inflag) Result(outflag)
  Implicit None
  Integer :: outflag, ierr
  Integer, Intent(in) :: mpicom, inflag
  Include 'mpif.h'
  Call MPI_Allreduce(inflag, outflag, 1, MPI_INTEGER, MPI_MIN, mpicom,&
       & ierr)
  If (ierr .Ne. MPI_SUCCESS) Then
     Print *, 'TRL_SYNC_FLAG: MPI_ALLREDUCE failed with error code ', ierr
     Call MPI_ABORT(mpicom, ierr)
  End If
End Function trl_sync_flag
!!!
! trl_g_dot implements a distributed version of BLAS routine dgemv
! which is used to compute dot-products by TRLAN
! this function performs (in matlab notation) wrk = [V1, V2]'*rr
!!!
! Arguments:
! mpicom  -- MPI communicator
! nrow    -- number of rows on the local processor
! v1      -- the first part of the matrix
! ld1     -- the leading dimension of the array v1
! m1      -- number of columns in v1
! v2      -- the second part of the matrix
! ld2     -- the leading dimension of v2
! m2      -- the number of columns in v2
! rr      -- the vector to be multiplied
! wrk     -- results of this operation, partial summs are stored at the
!            end of this array.
Subroutine trl_g_dot(mpicom, nrow, v1, ld1, m1, v2, ld2, m2, rr, wrk)
  Implicit None
  Integer, Intent(in) :: mpicom, nrow, ld1, ld2, m1, m2
  Real(8), Intent(in) :: v1(ld1,m1), v2(ld2,m2), rr(nrow)
  Real(8), Intent(out) :: wrk(m1+m2+m1+m2)
  Include 'mpif.h'
  !
  ! local variables
  Character, Parameter :: trans='T'
  Real(8), Parameter :: zero=0.0D0, one=1.0D0
  Integer :: i, npe, nd, mpitype, m1p1
  !
  ! BLAS routines used here
  External dgemv
  !
  nd = m1+m2
  If (nd .Le. 0) Return
  If (ld1.Lt.nrow .Or. ld2.Lt.nrow) Then
     Stop 'trl_g_dot incorrect array sizes'
  End If
  m1p1 = m1 + 1
  If (mpi_real8 .Ne. 0) Then
     mpitype = MPI_REAL8
  Else
     mpitype = MPI_DOUBLE_PRECISION
  End If
  If (m1 .Gt. 2) Then
!****NOTE****
! Due to some strange reason, the first one of these three variantion
! work correctly on T3E, however not the last two -- KW, July 1, 1998
!!$     wrk(nd+1:nd+m1) = Matmul(Transpose(v1(1:nrow,1:m1)), rr)
     Call dgemv(trans, nrow, m1, one, v1, ld1, rr, 1, zero, &
          & wrk(nd+1), 1) 
!!$     Call dgemv(trans, nrow, m1, one, v1(1:ld1,1:m1), ld1, rr(1:nrow),&
!!$          & 1, zero, wrk(nd+1:nd+m1), 1)
  Else If (m1 .Eq. 2) Then
     wrk(nd+1) = zero
     wrk(nd+2) = zero
     Do i = 1, nrow
        wrk(nd+1) = wrk(nd+1) + v1(i,1)*rr(i)
        wrk(nd+2) = wrk(nd+2) + v1(i,2)*rr(i)
     End Do
  Else If (m1 .Eq. 1) Then
     wrk(nd+1) = Dot_product(v1(1:nrow,1), rr(1:nrow))
  End If
  If (m2 .Gt. 2) Then
     Call dgemv(trans, nrow, m2, one, v2, ld2, rr, 1, zero,&
          & wrk(nd+m1p1:nd+nd), 1)
  Else If (m2 .Eq. 2) Then
     wrk(nd+m1p1) = zero
     wrk(nd+nd) = zero
     Do i = 1, nrow
        wrk(nd+m1p1) = wrk(nd+m1p1) + v2(i,1) * rr(i)
        wrk(nd+nd) = wrk(nd+nd) + v2(i,2) * rr(i)
     End Do
  Else If (m2 .Eq. 1) Then
     wrk(nd+m1p1) = Dot_product(v2(1:nrow,1), rr(1:nrow))
  End If
  If (mpicom.Eq.MPI_COMM_SELF) Then
     npe = 1
  Else
     Call MPI_COMM_SIZE(mpicom, npe, i)
  End If
  If (npe.Gt.1) Then
     Call MPI_ALLREDUCE(wrk(nd+1:nd+nd), wrk(1:nd), nd, mpitype,&
          & mpi_sum, mpicom, i)
     If (i .Ne. MPI_SUCCESS) Then
        Print *, 'TRL_G_DOT: MPI_ALLREDUCE failed with error code ',i
        Call MPI_ABORT(mpicom, i)
     End If
  Else
     wrk(1:nd) = wrk(nd+1:nd+nd)
  End If
End Subroutine trl_g_dot

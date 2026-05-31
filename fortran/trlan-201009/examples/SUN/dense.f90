!!! a really simple example using TRLAN on a simple matrix
Module SampleMatrix
!   Integer, Parameter :: ndim = 8
!   !!! the sample matrix to be used in this test
!   Real(8), Dimension(ndim, ndim) :: matrix = Reshape(&
!        &(/0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &
!        &  1.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &
!        &  0.0, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 1.0, &
!        &  0.0, 0.0, 0.0, 0.2, 1.0, 0.0, 0.0, 0.0, &
!        &  0.0, 0.0, 0.0, 1.0, 0.2, 0.0, 0.0, 0.0, &
!        &  0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 1.0, 0.0, &
!        &  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.2, 0.0, &
!        &  0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.2 /), (/ndim, ndim/))

!   Integer, Parameter :: ndim = 5
!   Real(8), Dimension(ndim, ndim) :: matrix = Reshape(&
!        &(/0.2, 1.0, 0.2, 0.2, 0.2, &
!        &  1.0, 1.0, 1.0, 1.0, 1.0, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2 /), (/ndim, ndim/))

!   Integer, Parameter :: ndim = 9
!   Real(8), Dimension(ndim, ndim) :: matrix = Reshape(&
!        &(/0.2, 1.0, 0.2, 0.2, 0.2, 0.2, 0.4, 0.0, 0.0, &
!        &  1.0, 2.0, 1.4, 1.0, 1.0, 1.0, 2.2, 0.4, 0.4, &
!        &  0.2, 1.4, 0.4, 0.2, 0.2, 0.2, 1.4, 0.2, 0.2, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2, 0.2, 0.4, 0.0, 0.0, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2, 0.2, 0.4, 0.0, 0.0, &
!        &  0.2, 1.0, 0.2, 0.2, 0.2, 0.2, 0.4, 0.0, 0.0, &
!        &  0.4, 2.2, 1.4, 0.4, 0.4, 0.4, 1.4, 1.0, 1.0, &
!        &  0.0, 0.4, 0.2, 0.0, 0.0, 0.0, 1.0, 0.2, 0.2, &
!        &  0.0, 0.4, 0.2, 0.0, 0.0, 0.0, 1.0, 0.2, 0.2 /), (/ndim, ndim/))

  Integer, Parameter :: ndim = 3
  Real(8), Dimension(ndim, ndim) :: matrix = Reshape(&
       &(/0.6, 1.0, 1.0, &
       &  1.0, 0.2, 0.2, &
       &  1.0, 0.2, 0.2 /), (/ndim, ndim/))

Contains
  !!!
  ! The matrix-vector multiplication routine associated with the matrix
  !!!
  Subroutine op(nrow, ncol, xin, ldx, yout, ldy)
    Implicit None
    Integer, Intent(in) :: nrow, ncol, ldx, ldy
    Real(8), Dimension(ldx, ncol), Intent(in) :: xin
    Real(8), Dimension(ldy, ncol), Intent(out) :: yout
    ! local variables
    Integer :: i, j
    If (nrow .Ne. ndim) Then
       Print *, 'SampleMatrix::op is only designed for nrow=', ndim
       Stop
    End If
    Do j = 1, ncol
       Do i = 1, nrow
          yout(i, j) = Dot_product(matrix(i,1:ndim), xin(1:ndim,j))
       End Do
    End Do
  End Subroutine op
End Module SampleMatrix
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! the main entry point of the program
Program simple
  Use trl_info
  Use trl_interface
  Use SampleMatrix
  Implicit None
  ! This set of parameters tell TRLAN to compute 3 (NED) smallest
  !(LOHI=-1) of a 100 x 100 (NROW) matrix by using a Krylov subspace
  ! basis size of 30 (MAXLAN).
  ! MEV defines the size of the arrays used to store computed
  ! solutions, eval, evec.
  Integer, Parameter :: nrow=ndim, lohi=-1, ned=3, maxlan=30, mev=10
  Integer, Parameter :: lwrk=maxlan*(maxlan+10)
  ! local variable declaration
  Real(8) :: eval(mev), evec(nrow, mev)
  Real(8) :: res(lwrk)
  Type(trl_info_t) :: info
  Integer :: i
  ! initialize info -- tell TRLAN to compute NEV smallest eigenvalues
  ! of the op
  Call trl_init_info(info, nrow, maxlan, lohi, ned, 1.4901D-8, 1, 2000)
  Call trl_set_iguess(info, 0, 1)
  Call trl_set_debug(info, +15, 6) ! use IO unit 6
  Call trl_set_checkpoint(info, -5, 98, 'SIMP2')
  ! the Lanczos recurrence is set to start with [1,1,...,1]^T
  eval = 0.0D0
  evec(1:nrow,1) = 1.0D0
  ! call TRLAN to compute the eigenvalues
  Call trlan(op, info, nrow, mev, eval, evec, nrow, res, lwrk)
  Call trl_print_info(info, 2*nrow*nrow)
  If (info%nec.Gt.0) Then
     i = info%nec
  Else
     i = mev - 1
  End If
  Call trl_check_ritz(op, info, nrow, evec(:,1:i), eval(1:i),&
       & res)
!!$  Call trl_terse_info(info, 6)
!!  Call trl_ritz_projection(op, info, evec, res)
!!$  i = info%nec+1
  Call trl_rayleigh_quotients(op, info, evec(:,1:i), res)
  Call trl_check_ritz(op, info, nrow, evec(:,1:i), res(1:i),&
       & res(i+1:i+i))
     If (info%nec .Eq. 0) info%nec = Min(info%ned, mev-1)
!!$  Write (6, FMT=100) (i, eval(i), res(i), i=1,info%nec)
!!$  Write (6, FMT=100) (i, res(i), res(i+info%nec), i=1,info%nec)
100 Format('E(', I2, ') = ', 1PG25.17, 1PE16.4)
End Program simple

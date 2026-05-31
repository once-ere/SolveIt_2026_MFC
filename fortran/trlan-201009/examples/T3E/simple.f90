!!! a really simple example of how to use TRLAN
Program simple
  Use trl_info
  Use trl_interface
  Implicit None
  Include 'mpif.h'
  Interface
     Subroutine diag_op(nrow, ncol, xin, ldx, yout, ldy)
       Implicit None
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Real(8), Dimension(ldx*ncol), Intent(in) :: xin
       Real(8), Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine diag_op
  End Interface
  ! This set of parameters tell TRLAN to compute 5 (NED) smallest
  !(LOHI=-1) of a 100 x 100 (NROW) matrix by using a Krylov subspace
  ! basis size of 30 (MAXLAN).
  ! MEV defines the size of the arrays used to store computed
  ! solutions, eval, evec.
  Integer, Parameter :: nrow=100, lohi=+1, ned=5, maxlan=30, mev=10
  Integer, Parameter :: lwrk=maxlan*(maxlan+10)
  ! local variable declaration
  Real(8) :: eval(mev), evec(nrow, mev), exact(mev)
  Real(8) :: res(lwrk)
  Type(trl_info_t) :: info
  Integer :: i
  ! initialize MPI
  Call MPI_INIT(i)
  ! initialize info -- tell TRLAN to compute NEV smallest eigenvalues
  ! of the diag_op
  Call trl_init_info(info, nrow, maxlan, lohi, ned, 1.4901D-8, 1, 2000)
  Call trl_set_iguess(info, 0, 1)
  Call trl_set_debug(info, -15, 99, 'SIMP1')
  Call trl_set_checkpoint(info, -5, 98, 'SIMP2')
  ! the Lanczos recurrence is set to start with [1,1,...,1]^T
  eval = 0.0D0
  evec(1:nrow,1) = 1.0D0
  ! call TRLAN to compute the eigenvalues
  Call trlan(diag_op, info, nrow, mev, eval, evec, nrow, res, lwrk)
  Call trl_print_info(info, 3*nrow)
  Do i = 1, mev
     exact(i) = i*i
  End Do
  If (info%nec.Gt.0) Then
     i = info%nec
  Else
     i = mev - 1
  End If
  Call trl_check_ritz(diag_op, info, nrow, evec(:,1:i), eval(1:i),&
       & res, exact)
  If (info%my_pe .Eq. 0) Then
     Call trl_terse_info(info, 6)
     If (info%nec .Eq. 0) info%nec = Min(info%ned, mev-1)
     Write (6, FMT=100) (i, eval(i), res(i), i=1,info%nec)
  End If
100 Format('E(', I1, ') = ', 1P, G25.17, E16.4)
  Call MPI_finalize(i)
End Program simple
!!!
! a simple matrix-vector multiplications routine
! defines a diagonal matrix with values (1, 4, 9, 16, 25, ....)
!!!
Subroutine diag_op(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Real(8), Dimension(ldx*ncol), Intent(in) :: xin
  Real(8), Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  ! local variables
  Integer :: i, j, ioff, joff, doff
  Call MPI_COMM_RANK(MPI_COMM_WORLD, i, j)
  doff = nrow*i
  Do j = 1, ncol
     ioff = (j-1)*ldx
     joff = (j-1)*ldy
     Do i = 1, nrow
        yout(joff+i) = (doff+i)**2*xin(ioff+i)
     End Do
  End Do
End Subroutine diag_op

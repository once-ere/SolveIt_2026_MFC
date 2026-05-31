!!! a really simple example of how to use TRLAN
Program simple
  Use trl_info
  Use trl_interface
  Implicit None
  Include 'mpif.h'
  ! local variable declaration
  Integer, Parameter :: nrow=100, lohi=-1, ned=5, maxlan=30, mev=10
  Double Precision :: eval(mev), evec(nrow, mev)
  Type(trl_info_t) :: info
  Integer :: i
  External diag_op
  ! initialize MPI
  Call MPI_INIT(i)
  ! initialize info -- tell TRLAN to compute NEV smallest eigenvalues
  ! of the diag_op
  Call trl_init_info(info, nrow, maxlan, lohi, ned)
  ! call TRLAN to compute the eigenvalues
  Call trlan(diag_op, info, nrow, mev, eval, evec, nrow)
  Call trl_print_info(info, nrow+nrow)
  If (info%my_pe .Eq. 0) Then
!     Call trl_terse_info(info, 6)
     Write (6, FMT=100) (i, eval(i), i=1,info%nec)
  End If
100 Format('E(', I1, ') = ', 1PG25.17)
  Call MPI_finalize(i)
End Program simple
!!!
! a simple matrix-vector multiplications routine
! defines a diagonal matrix with value [1:Nrow].*[1:Nrow]
!!!
Subroutine diag_op(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  ! local variables
  Integer :: i, j, ioff, joff, doff
  Call MPI_COMM_RANK(MPI_COMM_WORLD, i, j)
  doff = nrow*i
  Do j = 1, ncol
     ioff = (j-1)*ldx
     joff = (j-1)*ldy
     Do i = 1, nrow
        yout(joff+i) = (doff+i)*(doff+i)*xin(ioff+i)
     End Do
  End Do
End Subroutine diag_op

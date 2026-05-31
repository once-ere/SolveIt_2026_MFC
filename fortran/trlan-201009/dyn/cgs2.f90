! the Fortran 90 version of the gram-schmidt test program
Program gram_schmidt
  Implicit None
  Integer, Parameter :: nrow=9000, ncol=50
  Real(8), Parameter :: zero=0.0D0, one=1.0D0
  Real(8) :: array(nrow,ncol), wrk(ncol)
  Integer :: i, j, k
  Real(8) :: tmp
  Real(8), External :: dnrm2
  Interface
     Subroutine do_orth(nrow, uu, xx, wrk)
       Implicit None
       Integer, Intent(in) :: nrow
       Real(8), Dimension(:,:), Intent(in) :: uu
       Real(8), Dimension(:), Intent(inout) :: xx, wrk
     End Subroutine do_orth
  End Interface
  External dgemv, dscal
  !     first fill the array with numbers
  k = 13
  Do j = 1, ncol
     Do i = 1, nrow
        k = Mod(31*k+179, 1024)
        array(i, j) = Sqrt(Dble(i+j+k))
     End Do
  End Do
  !     classic Gram-Schmidt loops
  tmp = dnrm2(nrow, array, 1)
  tmp = one / tmp
  Call dscal(nrow, tmp, array, 1)
  Do i = 2, ncol
     Call do_orth(nrow, array(:,1:i-1), array(1:nrow,i), wrk)
     tmp = dnrm2(nrow, array(1,i), 1)
     tmp = one / tmp
     Call dscal(nrow, tmp, array(1,i), 1)
  End Do
  Stop
End Program gram_schmidt
!!!
! the actual GS routine
!!!
Subroutine do_orth(nrow, uu, xx, wrk)
  Implicit None
  Integer, Intent(in) :: nrow
  Real(8), Dimension(:,:), Intent(in) :: uu
  Real(8), Dimension(:), Intent(inout) :: xx, wrk
  Real(8), Parameter :: zero=0.0D0, one=1.0D0
  Integer :: ldu, nu, j, k
  ldu = Size(uu, 1)
  nu = Size(uu, 2)
  Do j = 1, 5
     Call dgemv('T', nrow, nu, one, uu, ldu, xx, 1, zero, wrk, 1)
     Write (*, *) nu+1, j, (wrk(k), k=1,nu)
     Call dgemv('N', nrow, nu, -one, uu, ldu, wrk, 1, one, xx, 1)
  End Do
  Return
End Subroutine do_orth

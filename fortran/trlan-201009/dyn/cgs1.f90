! the Fortran 90 version of the gram-schmidt test program
Program gram_schmidt
  Implicit None
  Integer, Parameter :: nrow=9000, ncol=50
  Real(8), Parameter :: zero=0.0D0, one=1.0D0
  Real(8) :: array(nrow,ncol), wrk(ncol)
  Integer :: i, j, k, im1
  Real(8) :: tmp
  Real(8), External :: dnrm2
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
     im1 = i - 1
     Do j = 1, 5
        Call dgemv('T', nrow, im1, one, array, nrow, &
             &     array(1,i), 1, zero, wrk, 1)
        Write (*, *) i, j, wrk(1:im1)
        Call dgemv('N', nrow, im1, -one, array, nrow, wrk, 1, &
             &     one, array(1,i), 1)
     End Do
     tmp = dnrm2(nrow, array(1,i), 1)
     tmp = one / tmp
     Call dscal(nrow, tmp, array(1,i), 1)
  End Do
  Stop
End Program gram_schmidt

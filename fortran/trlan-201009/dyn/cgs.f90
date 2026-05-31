! the Fortran 90 version of the gram-schmidt test program
!
! this version uses only Fortran 90 intrisic functions
Program gram_schmidt
  Implicit None
  Integer, Parameter :: nrow=9000, ncol=50
  Real(8), Parameter :: one=1.0D0
  Real(8) :: array(nrow,ncol), wrk(ncol)
  Integer :: i, j, k, im1
  Real(8) :: tmp
  !     first fill the array with numbers
  k = 13
  Do j = 1, ncol
     Do i = 1, nrow
        k = Mod(31*k+179, 1024)
        array(i, j) = Sqrt(Dble(i+j+k))
     End Do
  End Do
  !     classic Gram-Schmidt loops
  tmp = Sqrt(Dot_product(array(1:nrow,1), array(1:nrow,1)))
  tmp = one / tmp
  array(1:nrow,1) = tmp*array(1:nrow,1)
  Do i = 2, ncol
     im1 = i - 1
     Do j = 1, 5
        wrk(1:im1) = Matmul(Transpose(array(:,1:im1)),&
             & array(:,i))
        Write (*, *) i, j, wrk(1:im1)
        array(:,i) = array(:,i) - Matmul(array(:,1:im1),&
             & wrk(1:im1))
     End Do
     tmp = Sqrt(Dot_product(array(1:nrow,i), array(1:nrow,i)))
     tmp = one / tmp
     array(1:nrow,i) = tmp*array(1:nrow,i)
  End Do
  Stop
End Program gram_schmidt

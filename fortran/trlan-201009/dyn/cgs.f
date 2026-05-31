c     the Fortran 77 version of the gram-schmidt test program
      Program gram_schmidt
      Implicit None
      Integer nrow, ncol
      real*8 zero, one
      Parameter (nrow=9000, ncol=50, zero=0.0D0, one=1.0D0)
      Real*8 array(nrow, ncol), wrk(ncol)
      integer i, j, k
      real*8 dnrm2, tmp
      external dgemv, dnrm2, dscal
c
c     first fill the array with numbers
c
      k = 13
      do j = 1, ncol
         do i = 1, nrow
            k = mod(31*k+179, 1024)
            array(i, j) = sqrt(dble(i+j+k))
         end do
      end do
c
c     classic Gram-Schmidt loops
c
      tmp = dnrm2(nrow, array, 1)
      tmp = one / tmp
      call dscal(nrow, tmp, array, 1)
      do i = 2, ncol
         do j = 1, 5
            call dgemv('T', nrow, i-1, one, array, nrow,
     &           array(1,i), 1, zero, wrk, 1)
            write (*,*) i, j, (wrk(k), k=1,i-1)
            call dgemv('N', nrow, i-1, -one, array, nrow, wrk, 1,
     &           one, array(1,i), 1)
         end do
         tmp = dnrm2(nrow, array(1,i), 1)
         tmp = one / tmp
         call dscal(nrow, tmp, array(1,i), 1)
      end do
      stop
      End

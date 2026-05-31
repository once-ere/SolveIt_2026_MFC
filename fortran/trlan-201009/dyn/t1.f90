!! a dummy derived type
Module phantom
  Type :: PHANTOM_T
  Integer :: i
  End Type PHANTOM_T
End Module phantom
!!  the main program
Program testing
  Implicit None
  Integer, Parameter :: nrow=300, ncol=10
  Real, Dimension(:, :), Pointer :: array
  Real, Dimension(ncol) :: wrk
  Interface
     Subroutine do_it(array, wrk)
       Implicit None
       Real, Dimension(:,:), Target :: array
       Real, Dimension(:) :: wrk
     End Subroutine do_it
  End Interface
  ! fill the array with random numbers
  Allocate(array(nrow,ncol))
  Call Random_number(array)
  ! orthogonalize the array
  Call do_it(array, wrk)
End Program testing
!! do real work here -- Gram-Schmidt orthogonalization procedure
Subroutine do_it(array, wrk)
  Use PHANTOM
  Implicit None
  Real, Dimension(:,:), Target :: array
  Real, Dimension(:) :: wrk
  Interface
     Subroutine print_array(i,array)
       Use PHANTOM
       Implicit None
       Type(PHANTOM_T), Intent(in) :: i
       Real, Dimension(:,:) :: array
     End Subroutine print_array
     Subroutine print_wrk(i, j, wrk)
       Use PHANTOM
       Implicit None
       Type(PHANTOM_T), Intent(in) :: i, j
       Real, Dimension(:) :: wrk
     End Subroutine print_wrk
  End Interface
  Real, Parameter :: one=1.0, zero=0.0
  Type(PHANTOM_T) :: i, j
  Integer :: nrow, nm1, ncol, ii, jj
  Real :: tmp
  Real, Dimension(:), Pointer :: rr
  nrow = Size(array,1)
  ncol = Size(array,2)
  nm1 = nrow - 1
  tmp = Dot_product(array(2:nrow,1), array(2:nrow,1))
  tmp = one/Sqrt(tmp)
  array(2:nrow,1)=tmp*array(2:nrow,1)
  j%i = 1
  Call dummy_print_array
  Do jj = 1, ncol-1
     j%i = jj+1
     rr => array(2:nrow,j%i)
     Do ii = 1, 5
        i%i = ii
        Call dummy_print_array
        wrk(1:jj) = Matmul(Transpose(array(2:nrow,1:jj)), rr(2:nrow))
        Call print_wrk(i, j, wrk(1:jj))
        Call sgemv('N', nm1, jj, -one, array(2:nrow,1:jj), nm1,&
             & wrk(1:jj), 1, one, rr(2:nrow), 1)
     End Do
     tmp = Dot_product(rr, rr)
     Print *, 'Norm = ', Sqrt(tmp)
     tmp = one/Sqrt(tmp)
     rr = tmp*rr
     Call dummy_print_array
  End Do
Contains
  Subroutine dummy_print_array
    Call print_array(j,array)
  End Subroutine dummy_print_array
End Subroutine do_it
Subroutine print_array(i,array)
  Use PHANTOM
  Implicit None
  Type(PHANTOM_T), Intent(in) :: i
  Real, Dimension(:,:) :: array
  Print *, 'Column: ', i%i
  Print *, array(:,i%i)
End Subroutine print_array
Subroutine print_wrk(i,j,wrk)
  Use PHANTOM
  Implicit None
  Type(PHANTOM_T), Intent(in) :: i, j
  Real, Dimension(:) :: wrk
  Print *, 'Iteration ', j%i, i%i, ',  WRK = ', wrk
End Subroutine print_wrk

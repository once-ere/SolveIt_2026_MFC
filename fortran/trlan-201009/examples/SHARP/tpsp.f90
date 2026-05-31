!!!
! This is a simple test program for pspmatmult.f90
!!!
Program testpspmatmult
  Use pspmatmult
  Implicit None
  Include 'mpif.h'
  Type(pspmat) :: mat
  Integer, Dimension(:), Pointer :: irow, jcol, split, deg
  Real(8), Dimension(:), Pointer :: xx, y1, y2
  Integer :: i, j, mpicom, msgtag, mype, ngol, npe, ierr
  Integer :: ib, is, ie, izs, ize
  Real(8) :: tmp
  !
  msgtag = 1
  Call MPI_INIT(ierr)
  If (ierr .Ne. MPI_SUCCESS) Then
     Print *, 'TPSP: failed to start MPI.'
     Stop
  End If
  Call MPI_comm_dup(mpi_comm_world, mpicom, ierr)
  Call MPI_Comm_size(mpicom, npe, ierr)
  Call MPI_Comm_rank(mpicom, mype, ierr)
  Allocate(split(npe+1))
  Nullify(irow, jcol, deg, xx, y1, y2)
  Do j = 1, 10
     ! generate the global matrix (every PE should generate the same
     ! matrix)
     Call matgen(ngol, irow, jcol, deg)
     If (mype.Eq.0) Print *, 'test run #', j, ', nrow=', ngol
     Allocate(xx(ngol), y1(ngol), y2(ngol))
     Call simple_split(npe, ngol, irow, split, mype)
     is = split(mype+1)
     ie = split(mype+2) - 1
     izs = irow(is)
     ize = irow(ie+1) - 1
     ! perform global matrix-vector multiplication
     Call Random_number(xx)
     tmp = ngol + ngol
     ! use only integer values to avoid floating-point round-off problem
     xx = Real(Nint(tmp*xx) - ngol)
     y1 = 0.0
     y2 = 0.0
     jcol = jcol + 1
     Do i = 1, ngol
	Do ib = irow(i)-irow(1)+1, irow(i+1)-irow(1)
           y2(i) = y2(i) + xx(jcol(ib))
	End Do
        y2(i) = deg(i)*xx(i) - y2(i)
     End Do
     jcol = jcol - 1
     ! set up to do local matrix-vector multiplication
     Call pspmat_init(mat, split, irow(is:ie+1),&
          & jcol(izs:ize), deg(is:ie), mpicom, msgtag)
     ! perform the distributed version of MATVEC
     Call pspmat_mult(mat, xx(is:ie), y1(is:ie))
     Call pspmat_print_stat(mat)
     ! verify the correctness
     ize = 0
     Do i = is, ie
        If (y1(i) .Ne. y2(i)) Then
           ize = ize + 1
           Print *, i, y1(i), y2(i)
        End If
     End Do
     Call MPI_ALLREDUCE(ize, izs, 1, mpi_integer, mpi_sum, mpicom, i)
     If (izs.Eq.0 .And. mype.Eq.0) Then
        Print *, 'No error has been encountered.'
     End If
     Call pspmat_clean(mat)
     Deallocate(xx, y1, y2)
  End Do
  Call MPI_finalize(ierr)
  !! internal procedures
Contains
  ! generate a random matrix
  Subroutine matgen(ngol, irow, jcol, deg)
    Implicit None
    Integer, Intent(out) :: ngol
    Integer, Dimension(:), Pointer :: irow, jcol, deg
    ! parameters for defaul size of prroblem to generate
    Integer, Parameter :: nmax=5000, nzpr=10
    Integer :: i, nnz
    Real(8) :: tmp
    !
    If (Associated(irow)) Deallocate(irow)
    If (Associated(jcol)) Deallocate(jcol)
    If (Associated(deg)) Deallocate(deg)
    ! determine the exact problem size
    Call Random_number(tmp)
    ngol = Nint((0.5 + 0.5*tmp)*nmax)
    Allocate(irow(ngol+1), deg(ngol))
    irow(1) = 1
    Do i = 1, ngol
       Call Random_number(tmp)
       deg(i) = Nint(nzpr*(0.4 + 0.7*tmp))
       irow(i+1) = irow(i) + deg(i)
    End Do
    nnz = irow(ngol+1) - irow(1)
    Allocate(jcol(nnz))
    Do i = 1, nnz
       Call Random_number(tmp)
       jcol(i) = ngol*tmp
    End Do
    Return
  End Subroutine matgen
  ! split the matrix in a straightforward way
  Subroutine simple_split(npe, ngol, irow, split, mype)
    Implicit None
    Integer, Intent(in) :: npe, ngol, mype
    Integer, Dimension(:), Intent(in) :: irow
    Integer, Dimension(npe+1), Intent(out) :: split
    Integer :: i, nav, jrem
    ! first fill the split array
    nav = ngol / npe
    jrem = ngol - nav * npe
    split(1) = 1
    Do i = 1, jrem
       split(i+1) = split(i) + nav + 1
    End Do
    Do i = jrem+1, npe
       split(i+1) = split(i) + nav
    End Do
    Return
  End Subroutine simple_split
End Program testpspmatmult

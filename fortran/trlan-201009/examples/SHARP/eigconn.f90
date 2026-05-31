! $Id: eigconn.f90,v 1.1 2001/02/21 21:39:44 kewu Exp $
!
! This Program Contains accepts a connectivity graph and produce some
! smallest eigenvalues and the corresponding eigenvectors.  The input
! connectivity graph is stored in a file, the output eigenvalues and
! eigenvectors will be put into a file as well.
!
! This file Only does the command line processing, it passes 
! command line arguments
! conneig [conn_file [number_of_eigs [output_file [tolerance [mk]]]]]
!
! conn_file      -- the name of the connectivity file, If not
!                   specified, will attempt to Read from stdin
! number_of_eigs -- number of eigenvalues and eigenvectors to compute
!                   If not specified, it is assumed to be 5.
! output_file    -- the name of the output file, If not specified,
!                   output will be sent to stdout
! tolerance      -- default tolerance is Sqrt(epsilon)
!                   If tolerance read is an integer, it is assumed
!                   to be mk.
! mk             -- the Krylov basis size used by the Lanczos method
!                   the default value is max(30, number_eigs+20).
!
! At the moment, the outputed eigenvalues ane eigenvectors include the
! first one which is expected to have zero as eigenvalue and [1,1,...1]
! as eigenvector.
!
Module connmat
  Use pspmatmult
  Type(pspmat) :: mat
End Module connmat
!
! the operator to be used by TRLAN
Subroutine connop(nrow, ncol, xin, ldx, yout, ldy)
  Use connmat
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Real(8), Intent(in) :: xin(ldx, ncol)
  Real(8), Intent(out) :: yout(ldy, ncol)
  Integer i
  Do i = 1, ncol
     Call pspmat_mult(mat, xin(1:nrow,i), yout(1:nrow,i))
  End Do
End Subroutine connop
!
! to print out the eigenvalues and eigenvectors
Subroutine print_result(mpicom, nloc, neig, eval, evec, lda, filename, iou)
  Implicit None
  Include 'mpif.h'
  Character*(*) filename
  Integer, Intent(in) :: mpicom, nloc, neig, lda, iou
  Real(8) :: eval(neig), evec(lda, neig)
  !
  Integer i, ipe, ierr, j, myid, mpitype, nr, npes, status(MPI_STATUS_SIZE)
  Call MPI_Comm_rank(mpicom, myid, ierr)
  Call MPI_Comm_size(mpicom, npes, ierr)
100 Format(1P,5E16.6)
  If (MPI_REAL8 .Eq. 0) Then
     mpitype = MPI_DOUBLE_PRECISION
  Else
     mpitype = MPI_REAL8
  End If
  If (myid .Eq. 0) Then
     If (filename .Ne. '') Then
        Open(iou, file=filename, iostat=ierr)
        If (ierr.Ne.0) Print *, 'Unable to open ', Trim(filename),&
             & ' for output. iostat=', ierr
     End If
     Write (iou, 100) eval
     Do i = 1, neig
        Write (iou, *)
        Write (iou, 100) evec(1:nloc, i)
        Do ipe = 1, npes-1
           ! how many elements to receive from PE ipe
           Call MPI_Recv(nr, 1, MPI_INTEGER, ipe, 20, mpicom,&
                & status, ierr)
           ! how many can be received at one time
           If (nr .Le. lda) Then
              Call MPI_Send(nr, 1, MPI_INTEGER, ipe, 22, mpicom, ierr)
           Else
              Call MPI_Send(lda, 1, MPI_INTEGER, ipe, 22, mpicom, ierr)
           End If
           ! receive and print
           Do While (nr .Gt. 0)
              Call MPI_Recv(evec(:,i), lda, mpitype, ipe, 21, mpicom,&
                   & status, ierr)
              Call MPI_get_count(status, mpitype, j, ierr)
              Write (iou, 100) evec(1:j,i)
              nr = nr - j
           End Do
        End Do
     End Do
     If (iou.Ne.6) Close(iou)
  Else
     Do i = 1, neig
        ! sends number of elements on this PE
        Call MPI_Send(nloc, 1, MPI_INTEGER, 0, 20, mpicom, ierr)
        ! receives how many can be send at any one time
        Call MPI_Recv(nr, 1, MPI_INTEGER, 0, 22, mpicom, status, ierr)
        ! loop to send local data elements
        Do j = 1, nloc, nr
           Call MPI_Send(evec(j:Min(nloc,j+nr-1),i), Min(nr, nloc-j+1),&
                & mpitype, 0, 21, mpicom, ierr)
        End Do
     End Do
  End If
End Subroutine print_result
!
! the actual main program
Program conneig
  Use trl_info
  Use trl_interface
  Use connmat
  Implicit None
  Include 'mpif.h'
  Integer, Parameter :: lohi=-1, msglvl=0, msgtag=10
  Integer :: mpicom, nrow, iou, lda, lwrk, neig, i, ierr, nop, maxlan
  Real(8) :: tol
  Integer, Dimension(:), Pointer :: split, ia, ja, deg
  Real(8), Dimension(:,:), Pointer :: evec
  Real(8), Dimension(:), Pointer :: eval, wrk
  Character(256) :: str, connf, eigf
  Type(trl_info_t) :: info
  Integer iargc
  External connop, iargc, print_result
  Interface
     Subroutine readconn(mpicom, filename, nrow, split, ia, ja, deg, &
          & iou, msglvl, ierr)
       Implicit None
       Character*(*) filename
       Integer, Intent(in) :: iou, mpicom, msglvl
       Integer, Intent(out) :: ierr, nrow
       Integer, Pointer, Dimension(:) :: ia, ja, deg, split
     End Subroutine readconn
  End Interface
  connf = ''
  eigf  = ''
  ! initialize MPI
  Call MPI_init(ierr)
  ! parse command line arguments
  If (iargc() .Ge. 1) Then
     Call getarg(1, connf)
  End If
  If (iargc() .Ge. 2) Then
     Call getarg(2, str)
     Read(str, *, iostat=ierr) neig
     If (ierr .Ne. 0) Then
	neig = 5
	eigf = str
     End If
  Else
     neig = 5
  End If
  If (iargc() .Ge. 3) Then
     Call getarg(3, eigf)
  End If
  If (iargc() .Ge. 4) Then
     Call getarg(4, str)
     Read(str, *, iostat=ierr) tol
     If (ierr .Ne. 0) Then
        tol = Sqrt(Epsilon(tol))
        Read(str, *, iostat=ierr) maxlan
     Else If (tol .Ge. 1) Then
        tol = Sqrt(Epsilon(tol))
        maxlan = tol
     End If
  Else
     tol = Sqrt(Epsilon(tol))
  End If
  If (iargc() .Ge. 5) Then
     Call getarg(5, str)
     Read(str, *, iostat=ierr) maxlan
     If (ierr .Ne. 0) maxlan = Max(30, neig+20)
  End If
  If (maxlan .Le. neig+3) maxlan = Max(30, neig+20)
  !
  ! read the connectivity graph from file
  mpicom = MPI_COMM_WORLD
  If (connf .Ne. '') Then
     iou = 7
  Else
     iou = 5
  End If
  Call readconn(mpicom, connf, nrow, split, ia, ja, deg, iou, msglvl,&
       & ierr)
  If (ierr .Ne. 0) Then
     Print *, 'CONNEIG: failed to read connectivity file. ierr=', ierr
     Call MPI_Abort(mpicom, ierr)
  End If
  Call pspmat_init(mat, split, ia, ja, deg, mpicom, msgtag)
  !
  ! prepare to call trlan
  lda = ((nrow+3)/4)*4
  lwrk = lda + lda + maxlan * (maxlan + 10)
  Allocate(eval(maxlan), evec(lda, maxlan), wrk(lwrk))
  Call trl_init_info(info, nrow, maxlan, lohi, neig, tol)
  info%verbose = -3
  eval = 0.0
  evec(:,1) = 1.0
  Call trlan(connop, info, nrow, maxlan, eval, evec, lda, wrk, lwrk)
  ! print out some performance information
  nop = nrow + nrow + ia(nrow+1) - ia(1)
!  Call pspmat_print_stat(mat)
  Call pspmat_clean(mat)
  Call trl_print_info(info, nop)
  !
  ! output the eigenvalues and eigenvectors ??
  If (eigf .Eq. '') Then
     If (connf .Ne. '') Then
	Call construct_eigf
     Else
	iou = 6
     End If
  End If
  Call Print_result(mpicom, nrow, neig, eval, evec, lda, eigf, iou)
  Deallocate(eval, evec, wrk, deg, ja, ia, split)
  Call MPI_Finalize(ierr)
Contains
  ! generate the output file name from the input file name by copying
  ! the leading portion (till first '.') of connf and appending '.eig'
  Subroutine construct_eigf
  Integer i, nc, ne
  i = 1
  nc = Len_trim(connf)
  ne = Len(eigf)
  Do While (i.Le.nc .And. i.Le.ne-4 .And. connf(i:i).Ne.'.')
     eigf(i:i) = connf(i:i)
     i = i + 1
  End Do
  eigf(i:i+3) = '.eig'
  Return
  End Subroutine construct_eigf
End Program conneig

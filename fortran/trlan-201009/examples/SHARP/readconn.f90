!!$Id: readconn.f90,v 1.1 2001/02/21 21:39:48 kewu Exp $
!
! This routine reads a connectivity graph from file and
! Distribute the matrix as NPROCS consecutive row blocks,
! Where NPROCS is the number of Processors in the MPICOM
! group.
!
! The matrix is Read on the last processor, as each piece of
! Data is Read, it is sent to its destination.
!
! INPUT:
! mpicom    -- MPI communicator (group)
! filename  -- name of the Boeing-Harwell file
! iou       -- the fortran I/O unit number to be used when Read file
! msglvl    -- more message is printed If msglvl is higher
!
! OUTPUT:
! nrow      -- number rows distributed to this processor
! split     -- the starting point of each row Block distributed
! ja,ia,deg -- the connectivity structure
! nzmax     -- size of ja
! nmax      -- size of ia
! ierr      -- error flag
!!!
Subroutine readconn(mpicom, filename, nrow, split, ia, ja, deg, &
     & iou, msglvl, ierr)
  Implicit None
  Include 'mpif.h'
  ! .. Scalar Arguments ..
  Character*(*) filename
  Integer, Intent(in) :: iou, mpicom, msglvl
  Integer, Intent(out) :: ierr, nrow
  ! ..
  ! .. Array Arguments ..
  Integer, Pointer, Dimension(:) :: ia, ja, deg, split
  ! ..
  ! .. Local Scalars ..
  Logical flag
  Integer i, i0, iroot, j, ncol, nelm, nnz, totedge, totnode, myid, nprocs
  ! ..
  ! .. Local Arrays ..
  Integer  status(mpi_status_size)
  Integer, Dimension(:), Allocatable :: itmp, ireq
  ! ..
  ! .. Executable Statements ..
  ierr = 0
  totedge = 0
  totnode = 0
  Call MPI_Comm_rank(mpicom, myid, ierr)
  Call MPI_Comm_size(mpicom, nprocs, ierr)
  iroot = nprocs - 1
  Allocate(split(Max(2,1+nprocs)), stat=ierr)
  If (ierr .Ne. 0) Then
     Print *, 'PE', myid, ' unable to allocate space for array split'
     Call MPI_Abort(MPI_COMM_WORLD, ierr)
  End If
  !
  If (myid .Eq. iroot) Then
     !
     ! Open the given file
     !
     If (filename .Ne. '') Then
        Open(iou, file = filename, status = 'OLD', iostat = ierr)
        If (ierr.Ne.0) Then
           Print *, 'READCONN: can not open file ', Trim(filename),&
                & '; error code is ', ierr
           Call mpi_abort(mpicom, ierr)
        Endif
     End If
     ! Read the header
     Read (iou, FMT = *)  totnode, totedge
     If (msglvl .Gt. 0) Then
        Write (*, '(A, $)') 'READCONN:'
        If (msglvl.Gt.2) Print *, 'Opened connectivity file ', Trim(filename)
        Print *, totnode, ' vertices, ', totedge, ' number of edges'
     Endif
     split(1) = totnode
     split(2) = totedge
  End If
  ! broadcase the total sizes
  Call MPI_Bcast(split, 2, MPI_INTEGER, iroot, mpicom, ierr)
  totnode = split(1)
  totedge = split(2)
  !
  ! split the matrix so that there are almost the same number of rows
  ! on each PE
  !
  j = totnode / nprocs
  split(1) = 1
  nelm = j + 1
  Do i = 1, totnode - j*nprocs
     split(i+1) = split(i) + nelm
  End Do
  Do i = 1+totnode-j*nprocs, nprocs
     split(i+1) = split(i) + j
  End Do
  !
  ! the number of rows belong to this processor
  nrow = split(myid+2) - split(myid+1)
  !
  ! Allocate required space to store the output quantities
  !
  If (myid .Eq. iroot) Then
     Allocate(ia(nrow+20), deg(nrow), stat=ierr)
  Else
     Allocate(ia(nrow+1), deg(nrow), stat=ierr)
  End If
  If (ierr .Ne. 0) Then
     Print *, 'PE', myid, ' unable to allocate arrays ia or deg'
     Call MPI_Abort(MPI_COMM_WORLD, ierr)
  End If
  ! read the degree and skip over the starting position array
  If (myid .Eq. iroot) Then
     !
     ! the root processor will need to read and send out data
     !
     Allocate(itmp(totnode), ireq(nprocs), stat=ierr)
     If (ierr .Eq. 0) Then
        ! read the whole array into memory (itmp) and use asynchronous
        ! sends
        Read(iou, fmt=*) itmp(1:totnode)
        Do i = 0, iroot-1
           Call MPI_Isend(itmp(split(i+1):split(i+2)-1), &
                & split(i+2)-split(i+1), MPI_INTEGER, i, 101,&
                & mpicom, ireq(i+1), ierr)
        End Do
        deg(1:nrow) = itmp(split(nprocs):totnode)
        Read(iou, fmt=*) (j, i=1,totnode)
        Do i = 1, iroot
           Call MPI_Wait(ireq(i), status, ierr)
        End Do
     Else If (Allocated(itmp)) Then
        ! read the whole array into memory (itmp) but use blocking send
        Read(iou, fmt=*) itmp(1:totnode)
        Do i = 0, iroot-1
           Call MPI_Send(itmp(split(i+1):split(i+2)-1), &
                & split(i+2)-split(i+1), MPI_INTEGER, i, 101, mpicom,&
                & ierr)
        End Do
        deg(1:nrow) = itmp(split(nprocs):totnode)
        Read(iou, fmt=*) itmp(1:totnode)
     Else
        ! read one piece of data at a time
        i0 = 1
        Do i = 1, iroot
           nelm = split(i+1) - split(i)
           j = i0-1+20*((nelm-i0+18)/20)
           If (j .Gt. i0) Read(iou, fmt=*) ia(i0:j)
           Call MPI_Send(ia(1:nelm), nelm, MPI_INTEGER, i-1, 101,&
                & mpicom, ierr)
           i0 = 1
           If (j .Gt. nelm) Then
              ! need to copy the tail to the beginning of array ia
              Do While (i0+nelm .Lt. j)
                 ia(i0) = ia(i0+nelm)
                 i0 = i0 + 1
              End Do
           End If
        End Do
        If (i0 .Gt. 1) deg(1:i0-1) = ia(1:i0-1)
        If (i0 .Le. nrow) Read(iou, fmt=*) deg(i0:nrow)
        Read(iou, fmt=*) (j, i=1,totnode)
     End If
     If (Allocated(ireq)) Deallocate(ireq)
     If (Allocated(itmp)) Deallocate(itmp)
  Else
     ! receive degrees of the edges from the ROOT
     Call MPI_Recv(deg, nrow, MPI_INTEGER, iroot, 101, mpicom, status, ierr)
  End If
  ! generate ia array
  ia(1) = 0
  Do i = 1, nrow
     ia(i+1) = ia(i) + deg(i)
  End Do
  !
  If (myid .Eq. iroot) Then
     Allocate(itmp(nprocs), stat=ierr)
     If (ierr .Ne. 0) Then
        Print *, 'PE', myid, ' unable to allocate itmp(', nprocs, ')'
        Call MPI_Abort(MPI_COMM_WORLD, ierr)
     End If
  End If
  Call MPI_Gather(ia(nrow+1), 1, MPI_INTEGER, itmp, 1, MPI_INTEGER,&
       & iroot, mpicom, ierr)
  If (myid .Ne. iroot) Then
     nelm = ia(nrow+1)
  Else
     nelm = Maxval(itmp(1:nprocs))
  End If
  Allocate(ja(nelm), stat=ierr)
  If (ierr .Ne. 0) Then
     Print *, 'PE', myid, ' unable to allocate array ja(', nelm, ')'
     Call MPI_Abort(MPI_COMM_WORLD, ierr)
  End If
  ! read the connectivity array
  If (myid .Eq. iroot) Then
     Do i = 1, iroot
        nelm = itmp(i)
        Read(iou, fmt=*) ja(1:nelm)
        Call MPI_Send(ja, nelm, MPI_INTEGER, i-1, 102, mpicom, ierr)
     End Do
     nelm = itmp(nprocs)
     Read(iou, fmt=*) ja(1:nelm)
     If (filename.Ne.'') Close(iou)
     Deallocate(itmp)
  Else
     Call MPI_Recv(ja, nelm, MPI_INTEGER, iroot, 102, mpicom, status,&
          & ierr)
  End If
  Return
End Subroutine readconn

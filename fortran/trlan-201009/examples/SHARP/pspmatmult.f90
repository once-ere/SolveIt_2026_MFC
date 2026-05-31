! $Id: pspmatmult.f90,v 1.1 2001/02/21 21:39:48 kewu Exp $
!!!
! This file contains a module for performaning parallel(distributed)
! matrix-vector multiplication with the matrices generated from the
! connectivity graph of SHARP
!
! the input data is in the following format
! nloc  -- the number of nodes whose information is stored on this
!           processor
! deg   -- the degree of the each local node
! ia    -- the starting position in array ja where node connected to
!          node i is stored
! ja    -- the node on the other end of the edges
!!!
Module pspmatmult
  Implicit None
  ! define a Fortran 90 type so that the user may use more than one
  ! matrices in one program
  Type, Public :: pspmat
     Private ! every component is private in this user-defined type
     Integer :: nloc   ! number of rows locally
     Integer :: nzloc  ! nonzero entries on local processor

     Integer :: nglobal! number of rows globally
     Integer :: ready  ! this varible will be set to 1 (one) after
                       ! this data structure is successfully set up
                       ! for matrix-vector multiplication

     Integer :: nzlpsm ! number of nonzeros in the larges principle
                       ! submatrix, i.e., the number of entries that
                       ! can participate in MATVEC without waiting

     Integer :: mpicom ! the MPI communicator number
     Integer :: myrank ! the MPI rank of this PE
     Integer :: npes   ! number of PE's in the group

     ! array SPLIT contains the information about how the matrix is
     ! distributed among the processors
     ! it should contain same information on every PE
     ! PE i has (global) row split(i+1):split(i+2)-1
     ! where the PE number (MPI rank) starts with 0
     ! array split is assumed to start with 1 (one)
     Integer, Dimension(:), Pointer :: split
     Integer, Dimension(:), Pointer :: ia
     Integer, Dimension(:), Pointer :: ja
     Integer, Dimension(:), Pointer :: deg

     ! index array that points to the beginning of the edges that
     ! connects to a node outside of local domain
     Integer, Dimension(:), Pointer :: ib

     ! external column indices (information generated from
     ! preprocessing and may be used to restore the global indices)
     Integer, Dimension(:), Pointer :: extind

     ! information about what local vector components to send and where
     ! to receive from
     Integer :: nsrc, ndest, totsend, totrecv, msgtag
     Integer, Dimension(:), Pointer :: src, dest, nsend, isend, irecv, nrecv
     Integer, Dimension(:), Pointer :: iy, mpireq
     Real(8), Dimension(:), Pointer :: sbufy, rbufx

     ! a small amount of timing/history information
     Integer :: cmax
     Real(8) :: tav, tmath, tcomm, flop, crate, tinit
  End Type pspmat

  ! internal procedures should be private to this module
  Private :: pspmat_principle_submatrix, pspmat_build_send_info
  Private :: pspmat_isort
Contains
!!!
! setup the data structure papmat_symp from input matrix in COO format
!!!
  Subroutine pspmat_init(mat, sp, ia, ja, deg, mpicom, tag)
    Implicit None
    Type(pspmat), Intent(out) :: mat
    Integer, Intent(in) :: mpicom
    Integer, Dimension(:), Intent(in), Target :: sp
    Integer, Dimension(:), Target :: ia, ja, deg
    Integer, Intent(in), Optional :: tag
    ! MPI header file
    Include 'mpif.h'
    ! local variables
    Integer :: i, j, k, ip, npe, mpitype, t0, t1
    Integer, Dimension(:), Pointer :: iwrk
    Real(8) :: tmp
    ! set MAT to not ready for MATVEC operations
    mat%ready = -1
    Call System_clock(count=t0, count_rate=i, count_max=mat%cmax)
    If (i .Gt. 0) Then
       mat%crate = 1.0 / i
    End If
    !
    Call MPI_Comm_size(mpicom, npe, i)
    If (i .Ne. MPI_SUCCESS) Then
       ! probably because mpicom was invalid
       Print *, 'PSPMAT_INIT: ',&
            &'can not find the size of MPI_Comm ', mpicom
       Call MPI_Abort(mpi_comm_world, i)
    End If
    Call pspmat_reset(mat)
    ! change over the indices to be one-based
    ! input ia may start with any number, ja are assumed to start with 0
    k = ia(1) - 1
    ia = ia - k
    ja = ja + 1
    mat%mpicom = mpicom
    mat%npes = npe
    ! the special case of having only one PE
    If (npe .Le. 1) Then
       mat%nloc    = sp(2) - sp(1)
       mat%nglobal = sp(2) - sp(1)
       mat%nzloc   = ia(mat%nloc+1) - ia(1)
       mat%nzlpsm  = mat%nzloc
       mat%myrank  = 0
       mat%ia    => ia(1:mat%nloc+1)
       mat%ja    => ja(1:mat%nzloc)
       mat%deg   => deg(1:mat%nloc)
       mat%split => sp(1:npe+1)
       mat%ready = 1
       go to 99
    End If
    !
    ! continue with norm case where there is more than one PEs
    Call MPI_Comm_rank(mpicom, mat%myrank, i)
    If (i .Ne. MPI_SUCCESS) Then
       ! probably because mpicom is invalid
       Print *, 'PSPMAT_INIT: can not compute my rank ',&
            & ' in MPI_Comm ', mpicom
       Call MPI_Abort(mpi_comm_world, i)
    End If
    If (Size(sp) .Le. npe) Then
       Print *, 'PSPMAT_INIT PE', mat%myrank,&
            & ': size of SP should be ', npe+1, ', but is only ',&
            & Size(sp)
       mat%ready = -2
       Return
    End If
    ! record the matrix size and set the pointers inside mat
    ! we will continue to manipulate ir, jc, and val because their
    ! names are shorter to use
    mat%nglobal = sp(npe+1)-sp(1)
    mat%nloc    = sp(mat%myrank+2) - sp(mat%myrank+1)
    mat%nzloc   = ia(mat%nloc+1) - ia(1)
    mat%ia    => ia(1:mat%nloc+1)
    mat%ja    => ja(1:mat%nzloc)
    mat%deg   => deg(1:mat%nloc)
    mat%split => sp(1:npe+1)
    ! indentify the nonzeros in the principle submatrix
    Call pspmat_principle_submatrix(mat%nloc, ia, ja, sp(mat%myrank+1),&
         & sp(mat%myrank+2), mat%ib, mat%nzlpsm)
    ! continue the normal case, there are external edges in the graph of
    ! the submatrix
    ! gather the node numbers of reached by outgoing edges
    Allocate(iwrk(Max(npe+npe, mat%nglobal)))
    iwrk = 0
    Do i = 1, mat%nloc
       Do j = mat%ib(i), ia(i+1)-1
          k = ja(j)
          iwrk(k) = iwrk(k) + 1
       End Do
    End Do
    ! count the number of elements to receive and number of processors
    ! to receive from
    mat%totrecv = 0
    mat%nsrc    = 0
    Do i = 1, npe
       k = 0
       Do j = sp(i), sp(i+1)-1
          If (iwrk(j) .Gt. 0) k = k + 1
       End Do
       If (k .Gt. 0) Then
          mat%totrecv = mat%totrecv + k
          mat%nsrc    = mat%nsrc + 1
       End If
    End Do
    Allocate(mat%rbufx(mat%totrecv), mat%extind(mat%totrecv),&
         & mat%irecv(mat%nsrc+1), mat%src(mat%nsrc),&
         & mat%nrecv(mat%nsrc), mat%mpireq(npe+npe))
    ! renumber of extern nodes to use the indices to the rbufx
    j = 0
    Do i = 1, mat%nglobal
       If (iwrk(i) .Gt. 0) Then
          j = j + 1
          mat%extind(j) = i
          iwrk(i) = j
       End If
    End Do
    k = sp(mat%myrank+1) - 1
    Do i = 1, mat%nloc
       Do j = mat%ia(i), mat%ib(i)-1
          ja(j) = ja(j) - k
       End Do
       Do j = mat%ib(i), ia(i+1)-1
          ja(j) = iwrk(ja(j))
       End Do
    End Do
    ! where the messages from each processors are stored
    ip = 0
    mat%irecv(1) = 1
    Do i = 1, npe
       k = 0
       Do j = sp(i), sp(i+1)-1
          If (iwrk(j) .Gt. 0) k = k + 1
       End Do
       If (k .Gt. 0) Then
          ip              = ip + 1
          mat%mpireq(i)   = ip
          mat%src(ip)     = i - 1
          mat%nrecv(ip)   = k
          mat%irecv(ip+1) = mat%irecv(ip) + k
       Else
          mat%mpireq(i)   = 0
       End If
    End Do
    If (mat%irecv(mat%nsrc+1) .Ne. mat%totrecv+1) Then
       Print *, 'PE', mat%myrank, ' has inconsistent count of totrecv ',&
            & mat%irecv(mat%nsrc+1)-1, mat%totrecv
    End If
    Deallocate(iwrk)
    !
    ! send information regarding what this processor expects to receive
    ! to the sender -- not assuming the matrix has symmetric
    ! nonzero pattern 
    Call pspmat_build_send_info(mat)
    If (Present(tag)) Then
       mat%msgtag = tag
    Else
       Call Random_number(tmp)
       j = tmp*mat%nloc
       Call MPI_Allreduce(j, mat%msgtag, 1, mpi_integer, mpi_sum,&
            & mpicom, i)
       If (i .Ne. MPI_SUCCESS) mat%msgtag = 1
       mat%msgtag = Mod(mat%msgtag, 65536)
    End If
    mat%ready = 1
    !
    ! the last step of setup --
    ! set up the MPI comminucation pattern for multipling with
    ! one vector; if failed to do so for any reason on any processor,
    ! will nullify mat%mpireq to indicate that persistent communication
    ! is not usable
    If (MPI_REAL8 .Eq. 0) Then
       mpitype = MPI_DOUBLE_PRECISION
    Else
       mpitype = MPI_REAL8
    End If
    ! post send commands
    j = 0
    Do i = 1, mat%ndest
       Call MPI_SEND_INIT(mat%sbufy(mat%isend(i):mat%isend(i+1)-1),&
            & mat%nsend(i), mpitype, mat%dest(i), mat%msgtag,&
            & mpicom, mat%mpireq(i), k)
       If (k .Ne. MPI_SUCCESS) Then
          j = j + 1
          mat%mpireq(i) = MPI_REQUEST_NULL
          Print *, 'PSPMAT_INIT: PE', mat%myrank,&
               & 'failed to initial MPI persistent send', i
       End If
    End Do
    Do i = 1, mat%nsrc
       Call MPI_RECV_INIT(mat%rbufx(mat%irecv(i):mat%irecv(i+1)-1),&
            & mat%nrecv(i), mpitype, mat%src(i), mat%msgtag,&
            & mpicom, mat%mpireq(i+mat%ndest), k)
       If (k .Ne. MPI_SUCCESS) Then
          j = j + 1
          mat%mpireq(i+mat%ndest) = MPI_REQUEST_NULL
          Print *, 'PSPMAT_INIT: PE', mat%myrank,&
               & 'failed to initial MPI persistent receive', i
       End If
    End Do
    ! count of the number of failed persistent request
    Call MPI_Allreduce(j, k, 1, mpi_integer, mpi_min, mpicom, i)
    If (k .Gt. 0) Then
       Do i = 1,mat%ndest + mat%nsrc
          If (mat%mpireq(i) .Ne. MPI_REQUEST_NULL) &
               &Call MPI_Request_free(mat%mpireq(i), j)
       End Do
       Deallocate(mat%mpireq)
       Nullify(mat%mpireq)
    End If
99  Call System_clock(t1)
    If (t1 .Gt. t0) Then
       t1 = t1 - t0
    Else
       t1 = t1 + (mat%cmax - t0)
    End If
    mat%tinit = t1 * mat%crate
    Return
  End Subroutine pspmat_init
!!!
! clean the content of the pspmat
!!!
  Subroutine pspmat_clean(mat)
    Implicit None
    Type(pspmat) :: mat
    Integer i, ierr
    If (Associated(mat%extind)) Deallocate(mat%extind)
    If (Associated(mat%src))    Deallocate(mat%src)
    If (Associated(mat%dest))   Deallocate(mat%dest)
    If (Associated(mat%nsend))  Deallocate(mat%nsend)
    If (Associated(mat%nrecv))  Deallocate(mat%nrecv)
    If (Associated(mat%irecv))  Deallocate(mat%irecv)
    If (Associated(mat%isend))  Deallocate(mat%isend)
    If (Associated(mat%ib))     Deallocate(mat%ib)
    If (Associated(mat%iy))     Deallocate(mat%iy)
    If (Associated(mat%sbufy))  Deallocate(mat%sbufy)
    If (Associated(mat%rbufx))  Deallocate(mat%rbufx)
    If (Associated(mat%mpireq)) Then
       Do i = 1, mat%ndest+mat%nsrc
          Call MPI_Request_free(mat%mpireq(i), ierr)
       End Do
       Deallocate(mat%mpireq)
    End If
    Call pspmat_reset(mat)
  End Subroutine pspmat_clean
!!! set every element of mat to a know (null) value
  Subroutine pspmat_reset(mat)
    Implicit None
    Type(pspmat), Intent(out) :: mat
    Nullify(mat%split, mat%ia, mat%ja, mat%deg, mat%extind,&
         & mat%src, mat%dest, mat%nsend, mat%isend, mat%nrecv,&
         & mat%iy, mat%mpireq, mat%sbufy, mat%rbufx)
    mat%nloc    = 0
    mat%nzloc   = 0
    mat%nzlpsm  = 0
    mat%ready   = -1
    mat%mpicom  = -1
    mat%myrank  = -1
    mat%npes    = 0
    mat%nglobal = 0
    mat%nsrc    = 0
    mat%ndest   = 0
    mat%totsend = 0
    mat%totrecv = 0
    mat%msgtag  = 0
    mat%tav     = 0.0
    mat%tmath   = 0.0
    mat%tcomm   = 0.0
    mat%flop    = 0.0
  End Subroutine pspmat_reset
!!!
! a sweap to locate the principle submatrix
!!!
  Subroutine pspmat_principle_submatrix(nloc, ia, ja, lower, upper, ib, nza)
    Implicit None
    Integer, Dimension(:) :: ia, ja
    Integer, Intent(in) :: nloc, lower, upper
    Integer, Intent(out) :: nza
    Integer, Dimension(:), Pointer :: ib
    ! local variables
    Integer :: i, i0, i1, j, k
    ! the main body of this function is a do while loop
    Allocate(ib(Size(ia)))
    nza = 0
    Do i = 1, nloc
       i0 = ia(i)
       i1 = ia(i+1) - 1
       Do While (i0 .Le. i1)
          j = ja(i0)
          k = ja(i1)
          If (j .Ge. lower .And. j .Lt. upper) Then
             i0 = i0 + 1
          Else If (k.Lt.lower .Or. k.Ge.upper) Then
             i1 = i1 - 1
          Else
             ja(i0) = k
             ja(i1) = j
             i0 = i0 + 1
             i1 = i1 - 1
          End If
       End Do
       ib(i) = i0
       nza = nza + i0 - ia(i)
       i0 = ia(i)
       i1 = ib(i)
       j = ib(i) - i0
       k = ia(i+1) - ib(i)
       ! sort each half in increasing order
       Call pspmat_isort(j, ja(i0:ib(i)-1))
       Call pspmat_isort(k, ja(i1:ia(i+1)-1))
    End Do
  End Subroutine pspmat_principle_submatrix
!!!
! sort an integer array
!!!
  Subroutine pspmat_isort(N, val)
    Implicit None
    Integer N
    Integer :: val(0:N-1)
    !
    Integer :: IGAP,I,J,k
    !
    igap = N/2
    Do While (igap .Gt. 0)
       Do I = igap, N-1
          J = I-IGAP
50        If (val(j) .Gt. val(j+igap)) Then
             k = val(j)
             val(j) = val(j+igap)
             val(j+igap) = k
             If (J .Ge. IGAP) Then
                J = J-IGAP
                Goto 50
             End If
          End If
       End Do
       IGAP = IGAP/2
    End Do
  End Subroutine pspmat_isort
!!!
! send the infromation rgarding what to receive to the expected receiver
! each PE can determine what it expects to receive but it does not know
! how to determine what to send -- this uses a slow pairwise exchange
! which will result in npes*npes messages being sent
!!!
  Subroutine pspmat_build_send_info(mat)
    Implicit None
    Include 'mpif.h'
    Type recv_info
       Integer n, dest
       Integer, Dimension(:), Pointer :: ind
       Type(recv_info), Pointer :: next
    End Type recv_info
    Type(pspmat) :: mat
    Type(recv_info), Pointer :: rinfo, tmp
    Integer i, j, k, n, dest, src, req(2), status(MPI_STATUS_SIZE)
    !
    Nullify(rinfo, tmp)
    mat%ndest = 0
    Do i = 1, mat%npes-1
       dest = mat%myrank + i
       If (dest .Ge. mat%npes) dest = dest - mat%npes
       src = mat%myrank - i
       If (src .Lt. 0) src = src + mat%npes
       j = mat%mpireq(dest+1)
       If (j .Gt. 0) Then
          Call MPI_Isend(mat%nrecv(j), 1, MPI_INTEGER, dest, 301,&
               & mat%mpicom, req(1), k)
          Call MPI_Isend(mat%extind(mat%irecv(j):mat%irecv(j+1)-1),&
               & mat%nrecv(j), MPI_INTEGER, dest, 302, mat%mpicom,&
               & req(2), k)
       Else
          Call MPI_Isend(j, 1, MPI_INTEGER, dest, 301, mat%mpicom,&
               & req(1), k)
          req(2) = MPI_REQUEST_NULL
       End If
       Call MPI_Recv(n, 1, MPI_INTEGER, src, 301, mat%mpicom, status, k)
       If (n .Gt. 0) Then
          Allocate(tmp)
          tmp%dest = src
          tmp%n    = n
          Allocate(tmp%ind(n))
          Call MPI_Recv(tmp%ind, n, MPI_INTEGER, src, 302, mat%mpicom,&
               & status, k)
          mat%ndest   = mat%ndest + 1
          mat%totsend = mat%totsend + n
          tmp%next    => rinfo
          rinfo       => tmp
       End If
       Call MPI_Wait(req(1), status, k)
       If (req(2) .Ne. MPI_REQUEST_NULL) Call MPI_Wait(req(2), status, k)
    End Do
    ! consolidate the information to be sent into a simpler 1D structure
    Allocate(mat%sbufy(mat%totsend), mat%iy(mat%totsend),&
         & mat%dest(mat%ndest), mat%nsend(mat%ndest),&
         & mat%isend(mat%ndest+1), stat=i)
    If (i.Ne.0) Then
       Print *, 'Unable to allocate space to store information about',&
            &' what to send from PE', mat%myrank
       Call MPI_Abort(MPI_COMM_WORLD, i)
    End If
    mat%isend(1) = 1
    j = mat%split(mat%myrank+1)-1
    Do i = 1, mat%ndest
       tmp => rinfo
       mat%dest(i) = tmp%dest
       mat%nsend(i) = tmp%n
       mat%isend(i+1) = mat%isend(i) + tmp%n
       mat%iy(mat%isend(i):mat%isend(i+1)-1) = tmp%ind - j
       rinfo => tmp%next
       Deallocate(tmp)
    End Do
  End Subroutine pspmat_build_send_info
!!!
! print the timing information collected
!!!
  Subroutine pspmat_print_stat(mat)
    Implicit None
    Type(pspmat) :: mat
    ! local variable
    Include 'mpif.h'
    Integer :: ierr
    Real(8) :: rate1, rate2, buf1(5), buf2(5)
    buf1(1) = mat%tinit
    buf1(2) = mat%flop
    buf1(3) = mat%tav
    buf1(4) = mat%tmath
    buf1(5) = mat%tcomm
    If (MPI_REAL8 .Ne. 0) Then
       Call mpi_allreduce(buf1, buf2, 5, MPI_REAL8, mpi_sum,&
            & mat%mpicom, ierr)
    Else
       Call mpi_allreduce(buf1, buf2, 5, MPI_DOUBLE_PRECISION, mpi_sum,&
            & mat%mpicom, ierr)
    End If
    If (ierr .Eq. MPI_SUCCESS) Then
       buf1 = buf2 / mat%npes
    End If
    If (buf1(3).Gt.0.0 .And. mat%myrank.Eq.0) Then
       rate1 = buf1(2) / buf1(3)
       rate2 = buf1(2) / buf1(4)
       Print *, 'PSPMAT_PRINT_STAT: speed of MATVEC'
       Print 100, buf1(3), buf1(4), buf1(5)
       Print 110, rate1, rate2
       If (mat%npes .Gt. 1) Print 120, rate1*mat%npes, rate2*mat%npes
    End If
100 Format(1P, 'Time:  total   ', E12.5, ', arithmetic ', E12.5,&
         & ', comm ', E12.5)
110 Format(1P, 'FLOPS: per PE  ', E12.5, ',        raw ', E12.5)
120 Format(1P, 'FLOPS:aggregate', E12.5, ',        raw ', E12.5)
  End Subroutine pspmat_print_stat
!!!
! multiplying a single vector
! the matrix must has been setup using pspmat_init
!!!
  Subroutine pspmat_mult(mat, xx, yy)
    Implicit None
    Type(pspmat) :: mat
    Real(8), Dimension(:), Intent(in) :: xx
    Real(8), Dimension(:), Intent(out) :: yy
    !
    Include 'mpif.h'
    ! local variables
    Integer i, j, mpitype, ireq, it0, it1, it2, it3, it4, it5
    Integer, Dimension(:), Pointer :: status, req
    Real(8) :: tmp1
    !
    If (mat%ready .Ne. 1) Then
       Print *, 'PSPMAT_MULT: the input matrix is not set up properly,'
       Print *, 'make sure pspmat_init is called correctly'
       Call MPI_Abort(MPI_COMM_WORLD, i)
    End If
    If (Size(xx).Lt.mat%nloc .Or. Size(yy).Lt.mat%nloc) Then
       Print *, 'PSPMAT_MULT: either size of xx or yy is',&
            &' smaller than ', mat%nloc
       Call MPI_Abort(MPI_COMM_WORLD, i)
    End If
    Call System_clock(count=it0)
    If (mat%npes .Gt. 1) Then
       ! real parallel case -- more than one PE
       Nullify(req)
       i = MPI_SUCCESS
       ! first copy input array to send buffer
       mat%sbufy(1:mat%totsend) = xx(mat%iy(1:mat%totsend))
       ! send out the send buffer without waiting for its completion
       If (Associated(mat%mpireq)) Then
          ! start persistent comminucation channels
          Call MPI_startall(mat%nsrc+mat%ndest, mat%mpireq, i)
          If (i.Eq.MPI_SUCCESS) Then
             req => mat%mpireq(1:mat%ndest+mat%ndest)
             ireq = -1
          Else
             Print *, 'MPI_startall failed in PE', mat%myrank,&
                  & ' with error code ', i
             Call MPI_Abort(MPI_COMM_WORLD, i)
          End If
       Else If (mat%ndest+mat%nsrc.Gt.0) Then
          ! post each indivadual send operations
          If (MPI_REAL8 .Ne. 0) Then
             mpitype = MPI_REAL8
          Else
             mpitype = MPI_DOUBLE_PRECISION
          End If
          Allocate(req(mat%ndest+mat%nsrc))
          ireq = 1
          ! post all send operations
          Do i = 1, mat%ndest
             Call MPI_Isend(mat%sbufy(mat%isend(i):mat%isend(i+1)-1),&
                  & mat%nsend(i), mpitype, mat%dest(i), mat%msgtag,&
                  & mat%mpicom, req(i), j)
             If (j .Ne. MPI_SUCCESS) Then
                Print *, 'PE', mat%myrank, ' failed to start ',&
                     & 'MPI_Isend to PE', mat%dest(i)
                Call MPI_Abort(MPI_COMM_WORLD, j)
             End If
          End Do
          ! post all receive operations
          Do i = 1, mat%nsrc
             Call mpi_irecv(mat%rbufx(mat%irecv(i):mat%irecv(i+1)-1),&
                  & mat%nrecv(i), mpitype, mat%src(i), mat%msgtag,&
                  & mat%mpicom, req(mat%ndest+i), j)
             If (j .Ne. MPI_SUCCESS) Then
                Print *, 'PE', mat%myrank, ' failed to start ',&
                     & ' MPI_Irecv from PE', mat%src(i)
                Call MPI_Abort(MPI_COMM_WORLD, j)
             End If
          End Do
       Else
          ireq = 0
       End If
       Call System_clock(it1)
       ! perform multiplication of the principle submatrix
       yy(1:mat%nloc) = mat%deg(1:mat%nloc) * xx(1:mat%nloc)
       Do i = 1, mat%nloc
          tmp1 = 0
          Do j = mat%ia(i), mat%ib(i)-1
             tmp1 = tmp1 + xx(mat%ja(j))
          End Do
          yy(i) = yy(i) - tmp1
       End Do
       Call System_clock(it2)
       ! receive information from others
       ! block till all receive operations are done
       j = Max(mat%nsrc, mat%ndest)
       Allocate(status(mpi_status_size*j), stat=i)
       If (mat%nsrc .Gt. 0) Then
          If (i .Eq. 0) Then
             Call mpi_waitall(mat%nsrc, req(mat%ndest+1:mat%ndest+mat%nsrc),&
                  & status, i)
          Else
             Allocate(status(mpi_status_size), stat=i)
             If (i .Ne. 0) Then
                Print *, 'PE', mat%myrank, ' unable to allocate array',&
                     &' status(', MPI_STATUS_SIZE, ')'
                Call MPI_Abort(MPI_COMM_WORLD, i)
             End If
             Do i = mat%ndest+1, mat%ndest+mat%nsrc
                Call mpi_wait(req(i), status, it4)
             End Do
          End If
       End If
       Call System_clock(it3)
       ! multiply with the rest of the matrix
       If (mat%nsrc .Gt. 0) Then
          Do i = 1, mat%nloc
             tmp1 = 0
             Do j = mat%ib(i), mat%ia(i+1)-1
                tmp1 = tmp1 + mat%rbufx(mat%ja(j))
             End Do
             yy(i) = yy(i) - tmp1
          End Do
       End If
       Call System_clock(it4)
       !
       ! block till all the send operations are complete -- needed for the
       ! sanity of MPI
       If (mat%ndest .Gt. 0) Then
          If (Size(status) .Ge. mat%ndest*MPI_STATUS_SIZE) Then
             Call MPI_Waitall(mat%ndest, req(1:mat%ndest), status, i)
          Else
             Do i = 1, mat%ndest
                Call MPI_Wait(req(i), status, i)
             End Do
          End If
       End If
       Deallocate(status)
       If (ireq .Eq. 1) Deallocate(req)
       Nullify(req)
       Call System_clock(it5)
       j = it5
       Call tdiff(it0, j, mat%cmax)
       mat%tav = mat%tav + mat%crate * j
       Call tdiff(it4, it5, mat%cmax)
       mat%tcomm = mat%tcomm + mat%crate * it5
       Call tdiff(it3, it4, mat%cmax)
       mat%tmath = mat%tmath + mat%crate * it4
       Call tdiff(it2, it3, mat%cmax)
       mat%tcomm = mat%tcomm + mat%crate * it3
       Call tdiff(it1, it2, mat%cmax)
       mat%tmath = mat%tmath + mat%crate * it2
       Call tdiff(it0, it1, mat%cmax)
       mat%tcomm = mat%tcomm + mat%crate * it1
    Else
       ! one-PE special case -- needed so that mat%ib can be null
       Do i = 1, mat%nloc
          tmp1 = 0.0
          Do j = mat%ia(i), mat%ia(i+1)-1
             tmp1 = tmp1 + xx(mat%ja(j))
          End Do
          yy(i) = mat%deg(i)*xx(i) - tmp1
       End Do
       Call System_clock(j)
       Call tdiff(it0, j, mat%cmax)
       mat%tav = mat%tav + mat%crate * j
       mat%tmath = mat%tmath + mat%crate * j
    End If
    mat%flop = mat%flop + mat%nzloc + mat%nloc + mat%nloc
    Return
  Contains
    Subroutine tdiff(t0, t1, tmax)
      Implicit None
      Integer, Intent(in) :: t0, tmax
      Integer :: t1
      If (t1 .Ge. t0) Then
         t1 = t1 - t0
      Else
         t1 = t1 + (tmax - t0)
      End If
      Return
    End Subroutine tdiff
  End Subroutine pspmat_mult
End Module pspmatmult

! $Id: hbtrl.f90,v 1.4 1998/11/05 22:20:01 kewu Exp $
!
! This file contains a test program that reads a Harwell-Boeing matrix
! and computes its extreme eigenvalues
!!!
! A module to store the sparse matrix
!
! NOTE: The matrix is symmetrix and readin as Harwell-Boeing RSA
! matrix.  For efficiency reasons, it is expanded into a complete CSR
! format matrix.
!!!
Module HB_matrix
  Real(8), Dimension(:), Pointer :: aval
  Integer, Dimension(:), Pointer :: ia, ja
  Integer :: nrow

Contains
  !!!
  ! deallocate workspace
  !!!
  Subroutine hb_matrix_clean
    If (Associated(aval)) Deallocate(aval)
    If (Associated(ja)) Deallocate(ja)
    If (Associated(ia)) Deallocate(ia)
    Nullify(aval, ja, ia)
    nrow = 0
  End Subroutine hb_matrix_clean
  !!!
  ! an internal procedure to read the Harwell-Boeing matrix
  ! it also allocate array of appropriate sizes
  !
  ! Arguments:
  ! filename -- the name of the Harwell-Boing fil
  ! iou      -- the I/O unit number to be used.  If it is not passed in
  !             I/O unit 99 is used.
  !!!
!** SUN Fortran compiler does not handle the arrays correctly if the
!** same arguments are used as both input and output varibles on the
!** command line of ssrcsr.  Use two sets of arrays (aval,ja,ia,
!** bval,jb,ib) to work around this.                  -- KW, 980923
  Subroutine HB_matrix_read(filename, iou)
    Implicit None
    Character(*), Intent(in) :: filename
    Integer, Intent(in), Optional :: iou
    ! defaul I/O unit number
    Integer, Parameter :: default_iou = 99;
    ! local variables
    Character*2        guesol
    Character*3        rhstyp, Type
    Character*8        key
    Character*16       indfmt, ptrfmt
    Character*20       rhsfmt, valfmt
    Character*72       title
    Integer            i, ierr, ifmt, indcrd, j, ncol, neltvl, nmax,&
         & nnz, nrhs, nrwindx, nzmax, ptrcrd, rhscrd, totcrd,&
         & valcrd, actio
    Integer, Dimension(:), Pointer :: iau, iwork, ib, jb
    Real(8), Dimension(:), Pointer :: bval
    ! deallocate arrays
    Call hb_matrix_clean
    ! generate the actual I/O unit number to be used
    If (Present(iou)) Then
       actio = iou
    Else
       actio = default_iou
    End If
    ! open Harwell-Boeing file
    Open(actio, file = filename, status = 'OLD', iostat = ierr)
    If (ierr.Ne.0) Then
       Print *, 'HB_MATRIX_READ: can not open file ', Trim(filename),&
            & ', iostat = ', ierr
       Stop
    End If
    ! read header
    Read (actio, FMT = 9999)title, key, totcrd, ptrcrd, indcrd, valcrd,&
         & rhscrd, Type, nrow, ncol, nnz, neltvl, ptrfmt, indfmt,&
         & valfmt, rhsfmt

9999 Format(A72, A8, /5I14, /A3, 11X, 4I14, /2A16, 2A20)

    If (rhscrd.Gt.0)Read (actio, FMT = 9989)rhstyp, nrhs, nrwindx
9989 Format(A3, 11X, I14, I14)
    If (Type.Ne.'rsa' .And. Type.Ne.'RSA') Then
       Print *, 'HB_MATRIX_READ will only expand RSA matrix.'
       Stop
    Else If (valcrd.Le.0) Then
       Print *, 'HB_MATRIX_READ expects nonzero values.'
       Stop
    Else If (nrow.Ne.ncol) Then
       Print *, 'HB_MATRIX_READ expects nrow to the same as ncol.'
       Stop
    End If
    ! allocate arrays
    nzmax = nnz + nnz + nrow
    Allocate(aval(nnz), ja(nnz), ia(nrow+1))
    ! Read the three arrays of CSR Format
    Read (actio, FMT = ptrfmt)(ia(i), i = 1, nrow+1)
    Read (actio, FMT = indfmt)(ja(i), i = 1, nnz)
    Read (actio, FMT = valfmt)(aval(i), i = 1, nnz)
    Close(actio)
    Allocate(bval(nzmax), jb(nzmax), ib(nrow+1), iau(nrow+1), iwork(nrow+1))
    Call ssrcsr(3, 1, nrow, aval,ja,ia, nzmax, bval, jb, ib, iau,&
         & iwork, ierr)
    Deallocate(iau, iwork)
    If (ierr .Ne. 0) Then
       Print *, 'HB_MATRIX_READ ssrcsr failed. Ierr = ', ierr
       Stop
    End If
    Deallocate(aval, ja)
    nzmax = ib(nrow+1) - ib(1)
    Allocate(aval(nzmax), ja(nzmax))
    aval(1:nzmax) = bval(1:nzmax)
    ja(1:nzmax) = jb(1:nzmax)
    ia(1:nrow+1) = ib(1:nrow+1)
    Deallocate(bval, jb, ib)
  End Subroutine HB_matrix_read
End Module HB_matrix
!!!
! the matrix-vector multiplication routine
!!!
Subroutine hbmatmul(ndim, blk, xin, ldx, yout, ldy)
  Use hb_matrix
  Implicit None
  Integer, Intent(in) :: ndim, blk, ldx, ldy
  Real(8), Dimension(ldx,blk), Intent(in) :: xin
  Real(8), Dimension(ldy,blk), Intent(out) :: yout
  ! local variables
  Real(8), Parameter :: zero=0.0
  Integer ir, ib, j
  Real(8) :: tmp
  If (blk .Eq. 1) Then
     Call amux(ndim, xin, yout, aval, ja, ia)
  Else
     Do ir = 1, ndim
        Do ib = 1, blk
           tmp = zero
           Do j = ia(ir), ia(ir+1)-1
              tmp = tmp + aval(j) * xin(ja(j), ib)
           End Do
           yout(ir, ib) = tmp
        End Do
     End Do
  End If
End Subroutine hbmatmul
!!!
! the main program
! parses command line arguments
! sets up data steucture for TRLan
! prints final results
!!!
Program hbtrl
  Use trl_info
  Use trl_interface
  Use HB_matrix
  Implicit None
  Character(128) :: filename, tmp
  Type(TRL_INFO_T) :: info
  Integer :: i, lohi, ned, maxlan, tres, mev, lwrk, lde, mvop
  Real(8), Dimension(:), Allocatable :: eval, wrk
  Real(8), Dimension(:,:), Allocatable :: evec
  Integer, External :: iargc
  Real ,Dimension(2) :: tarray, tm
  Real, External :: etime
  External getarg, hbmatmul
  ! parse command line
  If (iargc() .Gt. 0) Then
     Call getarg(1, filename)
  Else
     Print *, 'HBTRL: must provide a HB file on command line.'
     Print *, 'usage: hbtrl hbfile neig restart_opt maxlan'
     Stop
  End If
  If (iargc() .Gt. 1) Then
     Call getarg(2, tmp)
     Read(tmp, *) lohi
     If (lohi .Ne. 0) Then
        ned = Abs(lohi)
     Else
        lohi = 1
        ned = 5
     End If
  Else
     lohi = 1
     ned = 5
  End If
  If (iargc() .Gt. 2) Then
     Call getarg(3, tmp)
     Read(tmp, *) tres
  Else
     tres = 3
  End If
  If (iargc() .Gt. 3) Then
     Call getarg(4, tmp)
     Read(tmp, *) maxlan
  Else
     maxlan = ned+Min(ned, 10)
  End If
  ! read the HB matrix
  Call HB_matrix_read(filename)
  ! allocate arrays
  mev = maxlan
  lwrk = maxlan*(maxlan + 10)
  lde = ((nrow +3) / 4)*4
  Allocate(evec(lde, mev+1), wrk(lwrk), eval(mev))
  !
  ! set up data structure needed by TRLan
  !
  Call trl_init_info(info, nrow, maxlan, lohi, ned, trestart=tres)
  info%verbose = -5
  Call trl_set_iguess(info, 0, 1)
!!$  Call trl_set_checkpoint(info, 5, 18)
  eval=0
  evec=1
  !
  ! call trlan
  tm(1) = etime(tarray)
  Call trlan(hbmatmul, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  tm(2) = etime(tarray)
  !
  ! print out come information
  mvop = ia(nrow+1) - ia(1)
  mvop = mvop + mvop
  Call trl_print_info(info, mvop)
  Print *, 'Time measured by etime (sec): ', tm(2) - tm(1)
  If (info%nec .Gt. 0) Then
     If (info%lohi .Gt. 0) Then
        Print 100, (i, eval(info%nec-i+1), wrk(info%nec-i+1), i=1,info%nec)
     Else
        Print 100, (i, eval(i), wrk(i), i=1,info%nec)
     End If
  End If
100 Format(I3, 1P, G28.18, E14.4)
End Program hbtrl

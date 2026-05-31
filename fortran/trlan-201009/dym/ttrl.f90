! $Id: ttrl.f90,v 1.3 1998/10/17 00:59:16 kewu Exp $
! 
! this is a simple test procedure for the TRLAN program
Program trlan_test1
  Use trl_info
  Use trl_interface
  Implicit None
  Include 'mpif.h'
  Interface
     Subroutine check_solutions(op, mpicom, nrow, ne, eval, evec, lde,&
          & res, anrm, rr)
       Implicit None
       Integer, Intent(in) :: nrow, ne, lde, mpicom
       Double Precision, Intent(in) :: anrm
       Double Precision, Intent(in) :: eval(ne), evec(lde,ne), res(ne)
       Double Precision, Intent(in), Optional :: rr(nrow)
       Interface
          Subroutine OP(nrow, ncol, xin, ldx, yout, ldy)
            Implicit None
            Integer, Intent(in) :: nrow, ncol, ldx, ldy
            Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
            Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
          End Subroutine OP
       End Interface
     End Subroutine check_solutions
     Subroutine OP1(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP1
     Subroutine OP2(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP2
     Subroutine OP21(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP21
     Subroutine OP3(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP3
     Subroutine OP31(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP31
     Subroutine OP4(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP4
     Subroutine OP5(nrow, ncol, xin, ldx, yout, ldy)
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP5
  End Interface
  Integer, Parameter :: lwrk=1000
  Type(TRL_INFO_T) :: info
  Integer :: nrow, mev, maxlan, lde, ned, lohi, maxmv, optype, tres
  Integer :: i, ierr, nec
  Double Precision :: wrk(lwrk)
  Double Precision, Allocatable, Dimension(:) :: eval
  Double Precision, Allocatable, Dimension(:, :) :: evec
  !
  ! initialize MPI
  Call MPI_INIT(ierr)
  !
  ! read in parameters from file ttrl.param
  Open(2, file='ttrl.param', iostat=ierr)
  If (ierr .Eq. 0) Then
     Read(2, *, iostat=ierr) optype, nrow, lohi, ned, maxlan, maxmv, tres
     Close(2)
  End If
  If (ierr .Ne. 0) Then
     Print *, 'Can not read parameter file!'
     Print *, 'Solve an example problem...'
     optype = 2
     nrow = 1000
     lohi = 0
     ned = 5
     maxlan = 20
     tres = 1
     maxmv = nrow
     lde = nrow
  End If
  lde = 4*Ceiling(nrow/4.0)
  mev = ned + 4
  Allocate(eval(mev), evec(lde, mev), stat=ierr)
  If (ierr.Ne.0) Then
     Stop 'TTRL: unable to allocate eval and evec.'
  End If
  !
  ! set up the initial guesses
  eval=Huge(eval)
  evec(1:nrow,1) = 1.0D0
  Call trl_init_info(info, nrow, maxlan, lohi, ned,&
       & trestart=tres, maxmv=maxmv)
  info%verbose = 8
  Select Case (optype)
  Case (1)
     Call trlan(op1, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  Case (2)
     Call trlan(op21, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  Case (3)
     Call trlan(op31, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  Case (4)
     Call trlan(op4, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  Case (5)
     Call trlan(op5, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  Case default
     Call trlan(op2, info, nrow, mev, eval, evec, lde, wrk, lwrk)
  End Select
  If (info%nec .Gt. 0) Then
     nec = info%nec
  Else
     nec = Min(info%ned, mev-1)
  End If
  If (info%my_pe .Eq. 0) Then
     Call trl_terse_info(info, 6)
     Do i = 1, nec
        Write (6, FMT=100) i, eval(i), wrk(i)
     End Do
  End If
  Call MPI_barrier(MPI_COMM_WORLD, ierr)
100 Format('E(', I3, ') = ', 1PG25.16, ',  r = ', 1PE12.4)
  Select Case (optype)
  Case (1)
     If (nec .Lt. mev) Then
        Call check_solutions(op1, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:, 1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, nec+1))
     Else
        Call check_solutions(op1, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  Case (2)
     If (nec .Lt. mev) Then
        Call check_solutions(op21, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:, 1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, nec+1))
     Else
        Call check_solutions(op21, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  Case (3)
     If (nec .Lt. mev) Then
        Call check_solutions(op31, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:, 1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, 1:nec))
     Else
        Call check_solutions(op31, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  Case (4)
     If (nec .Lt. mev) Then
        Call check_solutions(op4, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:, 1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, nec+1))
     Else
        Call check_solutions(op4, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  Case (5)
     If (nec .Lt. mev) Then
        Call check_solutions(op5, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, nec+1))
     Else
        Call check_solutions(op5, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  Case default
     If (nec .Lt. mev) Then
        Call check_solutions(op2, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec),&
             & info%anrm, evec(1:nrow, nec+1))
     Else
        Call check_solutions(op2, info%mpicom, nrow, nec,&
             & eval(1:nec), evec(:,1:nec), lde, wrk(1:nec), info%anrm)
     End If
  End Select
  If (info%my_pe .Eq. 0) Then
     Write(6, *)
  End If
  Deallocate(eval, evec)
  !
  ! finalize MPI
  Call MPI_finalize(ierr)
End Program trlan_test1
!!!
! verify the solution produced by TRLAN are accuarte by compare
! the return eigenvalues against the Rayleigh quotients and check
! the residual norm as well
!!!
Subroutine check_solutions(op, mpicom, nrow, ne, eval, evec, lde, res,&
     & anrm, rr)
  Implicit None
  Integer, Intent(in) :: nrow, ne, lde, mpicom
  Double Precision, Intent(in) :: anrm
  Double Precision, Intent(in) :: eval(ne), evec(lde,ne), res(ne)
  Double Precision, Intent(in), Optional :: rr(nrow)
  Interface
     Subroutine OP(nrow, ncol, xin, ldx, yout, ldy)
       Implicit None
       Integer, Intent(in) :: nrow, ncol, ldx, ldy
       Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
       Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
     End Subroutine OP
  End Interface
  Include 'mpif.h'
  ! local variables
  Integer :: i, ierr, myid, MPITYPE
  Double Precision :: tol
  Double Precision, Allocatable, Dimension(:) :: wrk, res2, cs, rq, nm
  !
  ierr = 0
  Allocate(wrk(Max(nrow, ne)), res2(ne), cs(ne), rq(ne), nm(ne),&
       & stat=ierr)
  If (ierr.Ne.0) Then
     Write (*,*) 'CHECK_SOLUTIONS: unable to allocate workspace.'
     Call MPI_Abort(MPI_COMM_WORLD, ierr)
  End If
  !
  ! go throught each vector one at a time
  Do i = 1, ne
     Call op(nrow, 1, evec(:, i), lde, wrk, nrow)
     nm(i) = Dot_product(evec(1:nrow, i), evec(1:nrow, i))
     rq(i) = Dot_product(evec(1:nrow, i), wrk(1:nrow))
     wrk(1:nrow) = wrk(1:nrow) - eval(i)*evec(1:nrow,i)
     res2(i) = Dot_product(wrk(1:nrow), wrk(1:nrow))
     If (Present(rr)) Then
        cs(i) = Dot_product(wrk(1:nrow), rr)
     Else
        cs(i) = 0.0D0
     End If
  End Do
  !
  ! perform global sums to generate the quantities
  !
  If (MPI_REAL8.Ne.0) Then
     mpitype = MPI_REAL8
  Else
     mpitype = MPI_DOUBLE_PRECISION
  End If
  Call MPI_Allreduce(nm, wrk, ne, mpitype, MPI_SUM, mpicom, ierr)
  nm = wrk(1:ne)
  Call MPI_Allreduce(rq, wrk, ne, mpitype, MPI_SUM, mpicom, ierr)
  rq = wrk(1:ne)
  Do i = 1, ne
     If (nm(i) .Gt. 0.0D0) Then
        nm(i) = Sqrt(nm(i))
        rq(i) = rq(i) / nm(i)
     End If
  End Do
  rq = rq - eval
  nm = nm - 1.0D0
  Call MPI_Allreduce(res2, wrk, ne, mpitype, MPI_SUM, mpicom, ierr)
  Do i = 1, ne
     If (wrk(i) .Ge. 0.0D0) Then
        res2(i) = Sqrt(wrk(i))
     Else
        res2(i) = wrk(i)
     End If
  End Do
  If (Present(rr)) Then
     Call MPI_Allreduce(cs, wrk, ne, mpitype, MPI_SUM, mpicom, ierr)
     Do i =1, ne
        If (res2(i) .Gt. 0.0D0) Then
           cs(i) = wrk(i)/res2(i)
           If (Abs(cs(i)).Gt.1.0D0) cs(i) = 1.0D0
           cs(i) = Sqrt(1.0D0 - cs(i)*cs(i))
        Else
           cs(i) = 0.0D0
        End If
     End Do
  End If
  res2 = res2 - res
  ! report the comparison results
  Call MPI_COMM_RANK(mpicom, myid, ierr)
  ! count the entries with large values
  tol = Max(anrm, Maxval(Abs(eval)))
  tol = tol * Epsilon(tol) * (lde * ne)
  ierr = 0
  Do i = 1, ne
     If (Abs(rq(i)) .Gt. tol) ierr = ierr + 1
     If (Abs(nm(i)) .Gt. tol) ierr = ierr + 1
     If (Abs(res2(i)) .Gt. tol) ierr = ierr + 1
  End Do
  If (myid.Eq.0) Then
     If (Present(rr)) Then
        Print 200
        Do i = 1, ne
           Print 100, i, rq(i), nm(i), res2(i), cs(i)
        End Do
     Else
        Print 300
        Do i  = 1, ne
           Print 100, i, rq(i), nm(i), res2(i)
        End Do
     End If
     If (ierr .Gt. 0) Print 400, ierr
  End If
  Deallocate(wrk, res2, cs, rq, nm)
100 Format(I6, 4(3X, 1PE12.4))
200 Format('ERRORS in', 2X, 'Ritz value', 4X, 'Vector norm', 2X,&
         & 'Residual norm', 2X, 'Res direction')
300 Format('ERRORS in', 2X, 'Ritz Value', 4X, 'Vector norm', 2X,&
         & 'Residual norm')
400 Format('Number of exceedingly large errors:', I10)
End Subroutine check_solutions
!!!
! the fake operator 1 (all eigenvalues are one)
!!!
Subroutine op1(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Integer :: i
  Do i = 0, ncol-1
     yout(i*ldy+1:i*ldy+nrow) = xin(i*ldx+1:i*ldx+nrow)
  End Do
End Subroutine OP1
!!!
! the second fake operator (uniform linear spectrum)
!!!
Subroutine op2(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  Integer :: i, j, offx, offy, offd, myid
  Call mpi_comm_rank(MPI_COMM_WORLD, myid, i)
  offd = myid*nrow
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = (offd+j)*xin(offx+j)
     End Do
  End Do
End Subroutine OP2
!!!
! linear spectrum, multiplicities equal to the number of PEs
!!!
Subroutine op21(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Integer :: i, j, offx, offy
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = j*xin(offx+j)
     End Do
  End Do
End Subroutine OP21
!!!
! the third fake operator (quadratic spectrum)
!!!
Subroutine op3(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  Integer :: i, j, offx, offy, offd, myid
  Call mpi_comm_rank(MPI_COMM_WORLD, myid, i)
  offd = myid*nrow
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = (offd+j)**2*xin(offx+j)
     End Do
  End Do
End Subroutine OP3
!!!
! quadratic spectrum, multiplicities equations to number of PEs
!!!
Subroutine op31(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  Integer :: i, j, offx, offy
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = j*j*xin(offx+j)
     End Do
  End Do
End Subroutine OP31
!!!
! the forth fake operator (cubic spectrum)
!!!
Subroutine op4(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  Integer :: i, j, offx, offy, offd, myid
  Call mpi_comm_rank(MPI_COMM_WORLD, myid, i)
  offd = myid*nrow
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = (offd+j)**3*xin(offx+j)
     End Do
  End Do
End Subroutine OP4
!!!
! the fifth fake operator (logrithmic spectrum -- dense high end)
!!!
Subroutine op5(nrow, ncol, xin, ldx, yout, ldy)
  Implicit None
  Integer, Intent(in) :: nrow, ncol, ldx, ldy
  Double Precision, Dimension(ldx*ncol), Intent(in) :: xin
  Double Precision, Dimension(ldy*ncol), Intent(out) :: yout
  Include 'mpif.h'
  Integer :: i, j, offx, offy, offd, myid
  Call mpi_comm_rank(MPI_COMM_WORLD, myid, i)
  offd = myid*nrow
  Do i = 0, ncol-1
     offx = i*ldx
     offy = i*ldy
     Do j = 1, nrow
        yout(offy+j) = Log(Dble(offd+j))*xin(offx+j)
     End Do
  End Do
End Subroutine OP5

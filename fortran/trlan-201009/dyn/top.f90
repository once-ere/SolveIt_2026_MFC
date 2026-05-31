! try to identify the problem with using dummy procedure in F90
! the inner-most dummy procedure
Subroutine dummy(ia)
  Implicit None
  Integer, Intent(in) :: ia
  Print *, 'DUMMY: ia = ', ia
End Subroutine dummy
! the second layer dummy procedure
Subroutine dummy1(op, ia, ib)
  Implicit None
  External op
  Integer, Intent(in) :: ia
  Integer, Intent(out) :: ib
  Call op(ia)
  ib = ia + 1
  Call op(ib)
End Subroutine dummy1
! the main program
Program test_dummy_procedure
  Implicit None
  External dummy
  Integer :: ia, ib
  ia = 0; ib=-1;
  Call dummy1(dummy, ia, ib)
End Program test_dummy_procedure

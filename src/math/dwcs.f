      SUBROUTINE DWCS (M, X, CS, N, F, B0, B1, B2)
C***BEGIN PROLOGUE  DWCS
C***PURPOSE  Evaluate a Chebyshev series for a vector of real arguments
C***LIBRARY   VFNLIB
C***CATEGORY  C3A2
C***TYPE      DOUBLE PRECISION (DWCS-S, DWCS-D)
C***KEYWORDS  CHEBYSHEV SERIES EVALUATION, VECTORIZED
C***AUTHOR  SAUNDERS, B. V,. (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWCS evaluates a given Chebyshev series for a vector of real
C   arguments.
C
C
C   P A R A M E T E R S
C
C   M   (Input) Integer 
C       The number of arguments at which the series is to be
C       evaluated.
C
C   X   (Input) Double precision array of length M 
C       The arguments at which the function is to be evaluated are
C       stored in X(1) to X(M) in any order.
C
C   CS  (Input) Double precision array of length .GE. N
C       The N coefficients of the Chebyshev series. (Note -- only half
C       the first coefficient is used in summing the series.)
C
C   N   (Input) Integer (0 .LE. N .LE. 1000)
C       The number of terms in the Chebyshev series.
C
C   F   (Output) Double precision array of length M
C       F(i) contains the value of the series at X(i), i=1,..,M.
C
C   B0  (Work) Double precision array of length M
C
C   B1  (Work) Double precision array of length M
C
C   B2  (Work) Double precision array of length M
C
C
C *********************************************************************
C   This routine is a modification of the function CSEVL  developed by 
C   W. Fullerton of LANL.
C *********************************************************************
C
C***REFERENCES  Ronald F. Boisvert and Bonita V. Saunders, Portable
C               Vectorized Software for Bessel Function Evaluation,
C               ACM Transactions on Mathematical Software 18 (1992),
C               pp 456-469.
C***ROUTINES CALLED  WFERR
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN 
C   930203  Minor modifications to prologue.
C***END PROLOGUE  DWCS
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER N, M
      DOUBLE PRECISION X, CS, F, B0, B1, B2
C
      DIMENSION B0(M), B1(M), B2(M), CS(N), F(M), X(M)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I, IMOD, J, NI
      DOUBLE PRECISION CS1, CS2, CSNI, CSNI1, CSNI2
C
C***FIRST EXECUTABLE STATEMENT  DWCS
C
c      IF (N .LT.    0)  CALL WFERR('DWCS','NUMBER OF TERMS LT 0',2)
c      IF (N .GT. 1000)  CALL WFERR('DWCS','NUMBER OF TERMS GT 1000',2)
C
C     ... INITIALIZATION
C
      DO 10 I=1,M
         B1(I) = 0.0D0
         B2(I) = 0.0D0
         F(I)  = 2.0D0*X(I)
  10  CONTINUE
C
C     ... THREE-TERM RECURSION
C         (DO THREE STEPS AT A TIME TO AVOID VECTOR COPIES)
C
      IMOD = MOD(N,3)
      DO 30 I=1,N-IMOD,3
         NI = N + 1 - I
         CSNI = CS(NI)
         CSNI1 = CS(NI-1)
         CSNI2 = CS(NI-2)
         DO 20 J=1,M
            B0(J) = ( F(J)*B1(J)-B2(J) ) + CSNI
            B2(J) = ( F(J)*B0(J)-B1(J) ) + CSNI1
            B1(J) = ( F(J)*B2(J)-B0(J) ) + CSNI2
  20     CONTINUE
  30  CONTINUE
C
C     ... LAST STEP  
C         (CLEANUP FOR CASE N NOT DIVISIBLE BY 3)
C
      IF (IMOD .EQ. 0) THEN
         DO 40 J=1,M
            F(J) = 0.50D0*(B1(J) - B0(J))
  40     CONTINUE
      ELSEIF (IMOD .EQ. 1) THEN
         CS1 = CS(1)
         DO 50 J=1,M
            B0(J) = ( F(J)*B1(J) - B2(J) ) + CS1
            F(J)  = 0.50D0*(B0(J) - B2(J))
  50     CONTINUE
      ELSE
         CS1 = CS(1)
         CS2 = CS(2)
         DO 60 J=1,M
            B0(J) = ( F(J)*B1(J) - B2(J) ) + CS2
            B2(J) = ( F(J)*B0(J) - B1(J) ) + CS1
            F(J)  = 0.50D0*(B2(J) - B1(J))
  60     CONTINUE
      ENDIF
C
      RETURN
      END 
      INTEGER FUNCTION IDWCS (OS, NOS, ETA)
C***BEGIN PROLOGUE  IDWCS
C***PURPOSE  Determines the number of terms of an orthogonal series  
C            required to meet a specified error tolerance.
C***LIBRARY   VFNLIB
C***CATEGORY  C3A2
C***TYPE      DOUBLE PRECISION (IDWCS-S, IDWCS-D)
C***KEYWORDS  CHEBYSHEV SERIES, INITIALIZATION
C***AUTHOR  BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   IDWCS returns the number of terms of the Chebyshev series OS 
C   required to insure the error in evaluating it is no larger than 
C   ETA.  
C   
C   Ordinarily, ETA is chosen to be one-tenth machine precision.
C
C
C   P A R A M E T E R S
C
C   OS     (Input) Double precision array of length .GE. NOS 
C          Coefficients of the NOS-term orthogonal series.
C
C   NOS    (Input) Integer (.GE. 1) 
C          Number of terms in the orthogonal series.
C
C   ETA    (Input) Double precision
C          Requested accuracy of the series.
C
C
C *********************************************************************
C   This routine is a modification of the function INITS developed by 
C   W. Fullerton of LANL.
C *********************************************************************
C
C***REFERENCES  Ronald F. Boisvert and Bonita V. Saunders, Portable
C               Vectorized Software for Bessel Function Evaluation,
C               ACM Transactions on Mathematical Software 18 (1992),
C               pp 456-469.
C***ROUTINES CALLED  WFERR
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN 
C   930203  Minor modifications to prologue.
C***END PROLOGUE  IDWCS
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER NOS
      DOUBLE PRECISION ETA, OS
C
      DIMENSION OS(NOS)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I, II
      DOUBLE PRECISION ERR
C
C***FIRST EXECUTABLE STATEMENT  IDWCS
C
c      IF (NOS .LT. 1)  
c     +   CALL WFERR('IDWCS','NUMBER OF COEFFICIENTS LT 1',2)
C
      ERR = 0.0D0
      DO 10 II=NOS,1,-1
         I = II
         ERR = ERR + ABS(OS(I))
         IF (ERR .GT. ETA) GO TO 20
 10   CONTINUE
C
 20   CONTINUE
c      IF (I .EQ. NOS) 
c     +   CALL WFERR('IDWCS','TOO MUCH ACCURACY REQUESTED',2)
C
      IDWCS = I
C
      RETURN
      END

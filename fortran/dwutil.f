
      SUBROUTINE DWGT (M, X, SCALR, N, INDX)
C***BEGIN PROLOGUE  DWGT
C***PURPOSE  Builds an array of indices of vector elements that are 
C            greater than a specified scalar
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (DWGT-S, DWGT-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWGT builds an array of indices of vector elements that are 
C   greater than a specified scalar.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer (M .GT. 0)
C          The number of elements of the input vector. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   SCALR  (Input) Double precision
C          The scalar used for comparison with vector elements.
C          Indices i are selected if X(i) .GT. SCALR.
C
C   N      (Output) Integer
C          The number of elements of X that satisfy relationship with
C          scalar.
C
C   INDX   (Output) Integer array of length N
C          Array containing indices of vector elements that satisfy
C          relationship with scalar.
C
C
C   CAUTION : constraints on the input variables are not checked by
C             this routine.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWGT
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INDX, M, N
      DOUBLE PRECISION SCALR, X
C
      DIMENSION X(M), INDX(*)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I
C
C***FIRST EXECUTABLE STATEMENT DWGT
C
      N = 0
C
      DO 10 I=1,M
         IF (X(I) .GT. SCALR) THEN
            N = N + 1
            INDX(N) = I
         ENDIF
  10  CONTINUE
C
      RETURN
      END

      SUBROUTINE DWGTHR (N, X, INDX, Y)
C***BEGIN PROLOGUE  DWGTHR
C***PURPOSE  Performs a gather by index on a given vector
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (WGTHR-S, DWGTHR-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWGTHR performs a gather by index on a given vector based on the
C   indices provided in an array: Y(i) = X(INDX(i)), i=1,...,N.
C
C
C   P A R A M E T E R S
C
C   N      (Input) Integer (N .GT. 0)
C          The number of elements to be gathered from the input vector
C          X. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   INDX   (Input) Integer array of length N
C          Array specifying indices of elements to be gathered from 
C          the input vector.
C
C   Y      (Output) Double precision array of length N
C          The array containing the compressed vector once the gather is
C          completed.  Y(i) = X(INDX(i)), i=1,...,N.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWGTHR
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INDX, N
      DOUBLE PRECISION X, Y
C
      DIMENSION X(*), Y(N), INDX(N)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER J
C
C***FIRST EXECUTABLE STATEMENT DWGTHR
C
      IF (N .LE. 0) RETURN
C
      DO 10 J=1,N
         Y(J) = X(INDX(J))
  10  CONTINUE
C
      RETURN
      END

      SUBROUTINE DWGTLE (M, X, SCALR1, SCALR2, N, INDX)
C***BEGIN PROLOGUE  DWGTLE
C***PURPOSE  Builds an array of indices of vector elements that are 
C            greater than a given scalar and less than or equal to a
C            second given scalar
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (DWGTLE-S, DDWGTLE-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWGTLE builds an array of indices of vector elements that are 
C   greater than a given scalar and less than or equal to a second
C   given scalar.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer (M .GT. 0)
C          The number of elements of the input vector. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   SCALR1,
C   SCALR2 (Input) Double precision
C          The scalars used for comparison with vector elements.
C          Indices i are selected if SCALR1 .LT. X(i) .LE. SCALR2.
C
C   N      (Output) Integer
C          The number of elements of X that satisfy relationship with
C          scalars.
C
C   INDX   (Output) Integer array of length N
C          Array containing indices of vector elements that satisfy
C          relationship with scalars.
C
C
C   CAUTION : constraints on the input variables are not checked by
C             this routine.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWGTLE
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INDX, M, N
      DOUBLE PRECISION SCALR1, SCALR2, X
C
      DIMENSION X(M), INDX(*)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I
      DOUBLE PRECISION ELEMT
C
C***FIRST EXECUTABLE STATEMENT DWGTLE
C
      N = 0
C
      DO 10 I=1,M
         ELEMT = X(I)
         IF (ELEMT .GT. SCALR1 .AND. ELEMT .LE. SCALR2) THEN
            N = N + 1
            INDX(N) = I
         ENDIF
  10  CONTINUE
C
      RETURN
      END

      SUBROUTINE DWLE (M, X, SCALR, N, INDX)
C***BEGIN PROLOGUE  DWLE
C***PURPOSE  Builds an array of indices of vector elements that are 
C            less than  or equal to a given scalar
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (DWLE-S, DWLE-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWLE builds an array of indices of vector elements that are 
C   less than or equal to a given scalar.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer (M .GT. 0)
C          The number of elements of the input vector. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   SCALR  (Input) Double precision
C          The scalar used for comparison with vector elements.
C          Indices i are selected if X(i) .LE. SCALR.
C
C   N      (Output) Integer
C          The number of elements of X that satisfy relationship with
C          scalars.
C
C   INDX   (Output) Integer array of length N
C          Array containing indices of vector elements that satisfy
C          relationship with scalars.
C
C
C   CAUTION : constraints on the input variables are not checked by
C             this routine.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWLE
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INDX, M, N
      DOUBLE PRECISION SCALR, X
C
      DIMENSION X(M), INDX(*)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I
C
C***FIRST EXECUTABLE STATEMENT WGE
C
      N = 0
C
      DO 10 I = 1,M
         IF (X(I) .LE. SCALR) THEN
            N = N + 1
            INDX(N) = I
         ENDIF
  10  CONTINUE
C
      RETURN
      END

      SUBROUTINE DWNGT (M, X, SCALR, KEY)
C***BEGIN PROLOGUE  DWNGT 
C***PURPOSE  Determines whether elements of a given vector are greater
C            than a given scalar
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (DWNGT-S, DWNGT-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWNGT determines whether elements of a given vector are greater
C   than a given scalar.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer (M .GT. 0)
C          The number of elements of the input vector. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   SCALR  (Input) Double precision
C          The scalar used for comparison with vector elements.
C
C   KEY    (Output) Integer
C          If KEY=0 then no vector elements satisfy X(i).GT.SCALR.
C          If KEY>0 then some vector elements satisfy X(i).GT.SCALR;
C          the first to do so is X(KEY).
C
C
C   CAUTION : constraints on the input variables are not checked by
C             this routine.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWNGT
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER KEY, M
      DOUBLE PRECISION SCALR, X
C
      DIMENSION X(M)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I
C
C***FIRST EXECUTABLE STATEMENT  DWNGT
C
C     ... QUICK CHECK (VECTORIZABLE)
C
      KEY = 0
      DO 10 I=1,M
         IF (X(I) .GT. SCALR) KEY = 1
   10 CONTINUE
C
C     ... IF CHECK FAILED THEN FIND INDEX OF FIRST FAILURE
C
      IF (KEY .NE. 0) THEN
         KEY = 0
         DO 20 I=1,M
            IF (X(I) .GT. SCALR) THEN
               KEY = I
               GO TO 30
            ENDIF
   20    CONTINUE
      ENDIF
C
   30 CONTINUE
      RETURN
      END
      SUBROUTINE DWNLE (M, X, SCALR, KEY)
C***BEGIN PROLOGUE  DWNLE 
C***PURPOSE  Determines whether elements of a given vector are less
C            than or equal to a given scalar
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (DWNLE-S, DDWNLE-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWNLE determines whether elements of a given vector are less than
C   or equal to a given scalar.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer (M .GT. 0)
C          The number of elements of the input vector. 
C
C   X      (Input) Double precision array of length M
C          The input vector.
C
C   SCALR  (Input) Double precision
C          The scalar used for comparison with vector elements.
C
C   KEY    (Output) Integer
C          If KEY=0 then no vector elements satisfy X(i).LE.SCALR.
C          If KEY>0 then some vector elements satisfy X(i).LE.SCALR;
C          the first to do so is X(KEY).
C
C
C   CAUTION : constraints on the input variables are not checked by
C             this routine.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWNLE
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER KEY, M
      DOUBLE PRECISION SCALR, X
C
      DIMENSION X(M)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER I
C
C***FIRST EXECUTABLE STATEMENT  DWNLE
C
C     ... QUICK CHECK (VECTORIZABLE)
C
      KEY = 0
      DO 10 I=1,M
         IF (X(I) .LE. SCALR) KEY = 1
   10 CONTINUE
C
C     ... IF CHECK FAILED THEN FIND INDEX OF FIRST FAILURE
C
      IF (KEY .NE. 0) THEN
         KEY = 0
         DO 20 I=1,M
            IF (X(I) .LE. SCALR) THEN
               KEY = I
               GO TO 30
            ENDIF
   20    CONTINUE
      ENDIF
C
   30 CONTINUE
      RETURN
      END

      SUBROUTINE DWSCTR (N, Y, INDX, X)
C***BEGIN PROLOGUE  DWSCTR
C***PURPOSE  Performs a scatter by index on a given vector
C***LIBRARY   VFNLIB
C***TYPE      DOUBLE PRECISION (WSCTR-S, DWSCTR-D)
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***DESCRIPTION
C
C   DWSCTR performs a scatter by index on a given vector based on the
C   indices provided in an array: X(INDX(i)) = Y(i), i=1,...,N.
C
C
C   P A R A M E T E R S
C
C   N      (Input) Integer (N .GT. 0)
C          The number of elements in the input vector Y. 
C
C   Y      (Input) Double precision array of length N
C          The compressed vector.
C
C   INDX   (Input) Integer array of length N
C          Array of indices of positions elements of compressed
C          vector will occupy when scattered into vector X.
C
C   X      (Output) Double precision array 
C          The vector to receive the scattered elements of Y.
C          X(INDX(i)) = Y(i), i=1,...,N.  Only elements of X
C          whose indices are in INDX are changed.
C
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWSCTR
C
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INDX, N
      DOUBLE PRECISION Y, X
C
      DIMENSION Y(N), X(*), INDX(N)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER J
C
C***FIRST EXECUTABLE STATEMENT DWGTHR
C
      IF (N .LE. 0) RETURN
C
      DO 10 J=1,N
         X(INDX(J)) = Y(J)
  10  CONTINUE
C
      RETURN
      END

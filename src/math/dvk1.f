c      SUBROUTINE DVK1 (M, X, F, WORK, IWORK, INFO)
      SUBROUTINE DVK1 (M, X, F, INFO)
C***BEGIN PROLOGUE  DVK1
C***PURPOSE  Computes the hyperbolic Bessel function of the third kind
C            of order one (K1) for a vector of real arguments
C***LIBRARY   VFNLIB
C***CATEGORY  C10B1
C***TYPE      DOUBLE PRECISION (VK1-S, DVK1-D)
C***KEYWORDS  BESSEL FUNCTION,THIRD KIND,HYPERBOLIC BESSEL FUNCTION,
C             MODIFIED BESSEL FUNCTION, ORDER ONE, VECTORIZED
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
C   DVK1 computes the modified (hyperbolic) Bessel function of the
C   third kind of order one (K1) for real arguments using uniform
C   approximation by Chebyshev polynomials.
C
C
C   P A R A M E T E R S
C
C   M      (Input) Integer
C          The number of arguments at which the function is to be
C          evaluated.
C
C   X      (Input) Double precision array of length M
C          The arguments at which the function is to be evaluated are
C          stored in X(1) to X(M) in any order.
C
C   F      (Output) Double precision array of length M
C          F(i) contains the value of the function at X(i), i=1,..,M.
C
C   WORK   (Work) Double precision vector of length 7*M
C
C   IWORK  (Work) Integer vector of length M
C
C   INFO   (Output) Integer
C          Indicates status of computed result. The following table
C          lists possible values and their meanings.  If OK=Yes then
C          all F(i) have been set, otherwise none have been set.
C
C          INFO  OK            Description
C          ------------------------------------------------------------
C           -1   Yes  Warning: Some X(i) so big K1 underflows. 
C                     The corresponding F(i) are set to zero.
C            0   Yes  Successfull execution.
C            1   No   Error: M .LE. 0
C            2   No   Error: Some X(i) is zero or negative.
C                     The index of the first offending argument is
C                     returned in IWORK(1).
C            3   No   Error: Some X(i) so small K1 overflows.
C                     The index of the first offending argument is
C                     returned in IWORK(1).
C
C
C *********************************************************************
C   This routine is a modification of the function DBESK1 developed by
C   W. Fullerton of LANL.
C *********************************************************************
C
C***REFERENCES  Ronald F. Boisvert and Bonita V. Saunders, Portable
C               Vectorized Software for Bessel Function Evaluation,
C               ACM Transactions on Mathematical Software 18 (1992),
C               pp 456-469.
C***ROUTINES CALLED  DWK1
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C   930203  Minor modifications to prologue.
C***END PROLOGUE  DVK1
C
C     ----------
C     PARAMETERS
C     ----------
C
      INTEGER INFO, IWORK, M
      DOUBLE PRECISION F, X, WORK
C
      DIMENSION X(M), F(M), WORK(7*M), IWORK(M)
C
C     ---------------
C     LOCAL VARIABLES
C     ---------------
C
      INTEGER IWB0, IWB1, IWB2, IWTC, IWXC, IWYC, IWZC, JIN
C
C***FIRST EXECUTABLE STATEMENT  DVK1
C
C     ... PARTITION WORK ARRAYS
C
      IWTC   = 1
      IWXC  = IWTC + M
      IWYC  = IWXC + M
      IWZC  = IWYC + M
      IWB0  = IWZC + M
      IWB1  = IWB0 + M
      IWB2  = IWB1 + M
C     Total = IB2  + M
C
      JIN   = 1
C     Total = JIN  + M
C
C     ... DWK1 DOES ALL THE WORK
C
      CALL DWK1(M,X,F,WORK(IWTC),WORK(IWXC),WORK(IWYC),WORK(IWZC),
     +         IWORK(JIN),WORK(IWB0),WORK(IWB1),WORK(IWB2),INFO)
C
      RETURN
      END

      SUBROUTINE DWK1 (M, X, F, TCMP, XCMP, YCMP, ZCMP, INDX, B0, B1,  
     +   B2, INFO)
C***BEGIN PROLOGUE  DWK1
C***SUBSIDIARY
C***PURPOSE  Computes the hyperbolic Bessel function of the third kind
C            of order one (K1) for a vector of arguments
C***LIBRARY   VFNLIB
C***AUTHOR  SAUNDERS, B. V., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C           BOISVERT, R. F., (NIST)
C             COMPUTING AND APPLIED MATHEMATICS LABORATORY
C             NATIONAL INSTITUTE OF STANDARDS AND TECHNOLOGY
C             GAITHERSBURG, MD 20899
C***ROUTINES CALLED  IDWCS, D1MACH, DWNLE, DWGTHR, DWGTLE, DWGT, DWLE,
C                    DWSCTR, DWCS
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C***END PROLOGUE  DWK1
C
C  ----------
C  PARAMETERS
C  ----------
C
      INTEGER INFO, INDX, M
      DOUBLE PRECISION B0, B1, B2, F, X, TCMP, XCMP, YCMP, ZCMP
C
      DIMENSION B0(M), B1(M), B2(M), F(M), INDX(M), X(M), TCMP(M),
     +          XCMP(M), YCMP(M), ZCMP(M)
C
C  ---------------
C  LOCAL VARIABLES
C  ---------------
C
      INTEGER LAK1, LAK12, LBI1, LBK1
      PARAMETER ( LAK1=38, LAK12=33, LBI1=17, LBK1=16 )
C
      INTEGER I, IDWCS, J, KEY, N, NTI1, NTK1, NTAK1, NTAK12, NTOT
      DOUBLE PRECISION AK1CS, AK12CS, BI1CS, BK1CS, EPMACH, EPS, D1MACH,
     +        XMAX, XMIN, XSML
C
      DIMENSION AK1CS(LAK1), AK12CS(LAK12), BI1CS(LBI1), BK1CS(LBK1)
C
      SAVE AK1CS, AK12CS, BI1CS, BK1CS, NTAK1, NTAK12, NTI1, NTK1, 
     +     XMAX, XMIN, XSML
C
C----------------------------------------------------------------------
C
C Series for BK1        on the interval  0.          to  4.00000D+00
C                                        with weighted error   9.16E-32
C                                         log weighted error  31.04
C                               significant figures required  30.61
C                                    decimal places required  31.64
C
      DATA BK1 CS(  1) / +.2530022733 8947770532 5311208685 33 D-1     /
      DATA BK1 CS(  2) / -.3531559607 7654487566 7238316918 01 D+0     /
      DATA BK1 CS(  3) / -.1226111808 2265714823 4790679300 42 D+0     /
      DATA BK1 CS(  4) / -.6975723859 6398643501 8129202960 83 D-2     /
      DATA BK1 CS(  5) / -.1730288957 5130520630 1765073689 79 D-3     /
      DATA BK1 CS(  6) / -.2433406141 5659682349 6007350301 64 D-5     /
      DATA BK1 CS(  7) / -.2213387630 7347258558 3152525451 26 D-7     /
      DATA BK1 CS(  8) / -.1411488392 6335277610 9583302126 08 D-9     /
      DATA BK1 CS(  9) / -.6666901694 1993290060 8537512643 73 D-12    /
      DATA BK1 CS( 10) / -.2427449850 5193659339 2631968648 53 D-14    /
      DATA BK1 CS( 11) / -.7023863479 3862875971 7837971200 00 D-17    /
      DATA BK1 CS( 12) / -.1654327515 5100994675 4910293333 33 D-19    /
      DATA BK1 CS( 13) / -.3233834745 9944491991 8933333333 33 D-22    /
      DATA BK1 CS( 14) / -.5331275052 9265274999 4666666666 66 D-25    /
      DATA BK1 CS( 15) / -.7513040716 2157226666 6666666666 66 D-28    /
      DATA BK1 CS( 16) / -.9155085717 6541866666 6666666666 66 D-31    /
C
C-------------------------------------------------------------------
C
C Series for BI1        ON THE INTERVAL  0.          to  9.00000D+00
C                                        with weighted error   1.44E-32
C                                         log weighted error  31.84
C                               significant figures required  31.45
C                                    decimal places required  32.46
C
      DATA BI1 CS(  1) / -.1971713261 0998597316 1385032181 49 D-2     /
      DATA BI1 CS(  2) / +.4073488766 7546480608 1553936520 14 D+0     /
      DATA BI1 CS(  3) / +.3483899429 9959455866 2450377837 87 D-1     /
      DATA BI1 CS(  4) / +.1545394556 3001236038 5984010584 89 D-2     /
      DATA BI1 CS(  5) / +.4188852109 8377784129 4588320041 20 D-4     /
      DATA BI1 CS(  6) / +.7649026764 8362114741 9597039660 69 D-6     /
      DATA BI1 CS(  7) / +.1004249392 4741178689 1798080372 38 D-7     /
      DATA BI1 CS(  8) / +.9932207791 9238106481 3712980548 63 D-10    /
      DATA BI1 CS(  9) / +.7663801791 8447637275 2001716813 49 D-12    /
      DATA BI1 CS( 10) / +.4741418923 8167394980 3880919481 60 D-14    /
      DATA BI1 CS( 11) / +.2404114404 0745181799 8631720320 00 D-16    /
      DATA BI1 CS( 12) / +.1017150500 7093713649 1211007999 99 D-18    /
      DATA BI1 CS( 13) / +.3645093565 7866949458 4917333333 33 D-21    /
      DATA BI1 CS( 14) / +.1120574950 2562039344 8106666666 66 D-23    /
      DATA BI1 CS( 15) / +.2987544193 4468088832 0000000000 00 D-26    /
      DATA BI1 CS( 16) / +.6973231093 9194709333 3333333333 33 D-29    /
      DATA BI1 CS( 17) / +.1436794822 0620800000 0000000000 00 D-31    /
C
C-------------------------------------------------------------------
C
C Series for AK1        on the interval  1.25000D-01 to  5.00000D-01
C                                        with weighted error   3.07E-32
C                                         log weighted error  31.51
C                               significant figures required  30.71
C                                    decimal places required  32.30
C
      DATA AK1 CS(  1) / +.2744313406 9738829695 2576662272 66 D+0     /
      DATA AK1 CS(  2) / +.7571989953 1993678170 8923781492 90 D-1     /
      DATA AK1 CS(  3) / -.1441051556 4754061229 8531161756 25 D-2     /
      DATA AK1 CS(  4) / +.6650116955 1257479394 2513854770 36 D-4     /
      DATA AK1 CS(  5) / -.4369984709 5201407660 5808450891 67 D-5     /
      DATA AK1 CS(  6) / +.3540277499 7630526799 4171390085 34 D-6     /
      DATA AK1 CS(  7) / -.3311163779 2932920208 9826882457 04 D-7     /
      DATA AK1 CS(  8) / +.3445977581 9010534532 3114997709 92 D-8     /
      DATA AK1 CS(  9) / -.3898932347 4754271048 9819374927 58 D-9     /
      DATA AK1 CS( 10) / +.4720819750 4658356400 9474493390 05 D-10    /
      DATA AK1 CS( 11) / -.6047835662 8753562345 3735915628 90 D-11    /
      DATA AK1 CS( 12) / +.8128494874 8658747888 1938379856 63 D-12    /
      DATA AK1 CS( 13) / -.1138694574 7147891428 9239159510 42 D-12    /
      DATA AK1 CS( 14) / +.1654035840 8462282325 9729482050 90 D-13    /
      DATA AK1 CS( 15) / -.2480902567 7068848221 5160104405 33 D-14    /
      DATA AK1 CS( 16) / +.3829237890 7024096948 4292272991 57 D-15    /
      DATA AK1 CS( 17) / -.6064734104 0012418187 7682103773 86 D-16    /
      DATA AK1 CS( 18) / +.9832425623 2648616038 1940046506 66 D-17    /
      DATA AK1 CS( 19) / -.1628416873 8284380035 6666201156 26 D-17    /
      DATA AK1 CS( 20) / +.2750153649 6752623718 2841203370 66 D-18    /
      DATA AK1 CS( 21) / -.4728966646 3953250924 2810695680 00 D-19    /
      DATA AK1 CS( 22) / +.8268150002 8109932722 3920503466 66 D-20    /
      DATA AK1 CS( 23) / -.1468140513 6624956337 1939648853 33 D-20    /
      DATA AK1 CS( 24) / +.2644763926 9208245978 0858948266 66 D-21    /
      DATA AK1 CS( 25) / -.4829015756 4856387897 9698688000 00 D-22    /
      DATA AK1 CS( 26) / +.8929302074 3610130180 6563327999 99 D-23    /
      DATA AK1 CS( 27) / -.1670839716 8972517176 9977514666 66 D-23    /
      DATA AK1 CS( 28) / +.3161645603 4040694931 3686186666 66 D-24    /
      DATA AK1 CS( 29) / -.6046205531 2274989106 5064106666 66 D-25    /
      DATA AK1 CS( 30) / +.1167879894 2042732700 7184213333 33 D-25    /
      DATA AK1 CS( 31) / -.2277374158 2653996232 8678400000 00 D-26    /
      DATA AK1 CS( 32) / +.4481109730 0773675795 3058133333 33 D-27    /
      DATA AK1 CS( 33) / -.8893288476 9020194062 3360000000 00 D-28    /
      DATA AK1 CS( 34) / +.1779468001 8850275131 3920000000 00 D-28    /
      DATA AK1 CS( 35) / -.3588455596 7329095821 9946666666 66 D-29    /
      DATA AK1 CS( 36) / +.7290629049 2694257991 6799999999 99 D-30    /
      DATA AK1 CS( 37) / -.1491844984 5546227073 0240000000 00 D-30    /
      DATA AK1 CS( 38) / +.3073657387 2934276300 7999999999 99 D-31    /
C
C-------------------------------------------------------------------
C
C Series for AK12       on the interval  0.          to  1.25000D-01
C                                        with weighted error   2.41E-32
C                                         log weighted error  31.62
C                               significant figures required  30.25
C                                    decimal places required  32.38
C
      DATA AK12CS(  1) / +.6379308343 7390010366 0048853410 2 D-1      /
      DATA AK12CS(  2) / +.2832887813 0497209358 3503028470 8 D-1      /
      DATA AK12CS(  3) / -.2475370673 9052503454 1454556673 2 D-3      /
      DATA AK12CS(  4) / +.5771972451 6072488204 7097662576 3 D-5      /
      DATA AK12CS(  5) / -.2068939219 5365483027 4553319655 2 D-6      /
      DATA AK12CS(  6) / +.9739983441 3818041803 0921309788 7 D-8      /
      DATA AK12CS(  7) / -.5585336140 3806249846 8889551112 9 D-9      /
      DATA AK12CS(  8) / +.3732996634 0461852402 2121285473 1 D-10     /
      DATA AK12CS(  9) / -.2825051961 0232254451 3506575492 8 D-11     /
      DATA AK12CS( 10) / +.2372019002 4841441736 4349695548 6 D-12     /
      DATA AK12CS( 11) / -.2176677387 9917539792 6830166793 8 D-13     /
      DATA AK12CS( 12) / +.2157914161 6160324539 3956268970 6 D-14     /
      DATA AK12CS( 13) / -.2290196930 7182692759 9155133815 4 D-15     /
      DATA AK12CS( 14) / +.2582885729 8232749619 1993956522 6 D-16     /
      DATA AK12CS( 15) / -.3076752641 2684631876 2109817344 0 D-17     /
      DATA AK12CS( 16) / +.3851487721 2804915970 9489684479 9 D-18     /
      DATA AK12CS( 17) / -.5044794897 6415289771 1728250880 0 D-19     /
      DATA AK12CS( 18) / +.6888673850 4185442370 1829222399 9 D-20     /
      DATA AK12CS( 19) / -.9775041541 9501183030 0213248000 0 D-21     /
      DATA AK12CS( 20) / +.1437416218 5238364610 0165973333 3 D-21     /
      DATA AK12CS( 21) / -.2185059497 3443473734 9973333333 3 D-22     /
      DATA AK12CS( 22) / +.3426245621 8092206316 4538880000 0 D-23     /
      DATA AK12CS( 23) / -.5531064394 2464082325 0124800000 0 D-24     /
      DATA AK12CS( 24) / +.9176601505 6859954037 8282666666 6 D-25     /
      DATA AK12CS( 25) / -.1562287203 6180249114 4874666666 6 D-25     /
      DATA AK12CS( 26) / +.2725419375 4843331323 4943999999 9 D-26     /
      DATA AK12CS( 27) / -.4865674910 0748279923 7802666666 6 D-27     /
      DATA AK12CS( 28) / +.8879388552 7235025873 5786666666 6 D-28     /
      DATA AK12CS( 29) / -.1654585918 0392575489 3653333333 3 D-28     /
      DATA AK12CS( 30) / +.3145111321 3578486743 0399999999 9 D-29     /
      DATA AK12CS( 31) / -.6092998312 1931276124 1600000000 0 D-30     /
      DATA AK12CS( 32) / +.1202021939 3698158346 2399999999 9 D-30     /
      DATA AK12CS( 33) / -.2412930801 4594088413 8666666666 6 D-31     /
C
C-------------------------------------------------------------------
C
      DATA NTK1 / 0 /
C
C***FIRST EXECUTABLE STATEMENT  DWK1
C
      IF (M .LE. 0)  GO TO 910
C
      IF (NTK1 .EQ. 0) THEN
         EPMACH = D1MACH(3)
         EPS = 0.10D0*EPMACH
         NTK1 = IDWCS(BK1CS, LBK1, EPS)
         NTI1 = IDWCS(BI1CS, LBI1, EPS)
         NTAK1 = IDWCS(AK1CS, LAK1, EPS)
         NTAK12 = IDWCS(AK12CS, LAK12, EPS)
         XMIN = EXP(MAX( LOG(D1MACH(1)), -LOG(D1MACH(2))) + 0.010D0)
         XSML = SQRT(4.0D0*EPMACH)
         XMAX = -LOG(D1MACH(1))
         XMAX = XMAX - 0.50D0*XMAX*LOG(XMAX)/(XMAX + 0.50D0)
      ENDIF
C
      NTOT = 0
C
      CALL DWNLE(M,X,0.0D0,KEY)
      IF (KEY .NE. 0)  GO TO 920
C
      CALL DWNLE(M,X,XMIN,KEY)
      IF (KEY .NE. 0)  GO TO 930
C
C  ------------------
C  CASE   X .GT. XMAX
C  ------------------
C
C     NOTE -- K0 UNDERFLOWS FOR X .GT. XMAX
C
      DO 5 I=1,M
         F(I) = 0.0D0
   5  CONTINUE
C
C  ----------------
C  CASE  X .LE. 2.0
C  ----------------
C
      CALL DWLE(M,X,2.0D0,N,INDX)
      IF (N .GT. 0) THEN
         NTOT = NTOT + N
         CALL DWGTHR(N,X,INDX,XCMP)
C
C        ... COMPUTE I1(X) ... RESULT IN ZCMP
C
         DO 20 J=1,N
            TCMP(J) = XCMP(J)**2/4.50D0 - 1.0D0
   20    CONTINUE
         CALL DWCS(N,TCMP,BI1CS,NTI1,ZCMP,B0,B1,B2)
         DO 30 J=1,N
            ZCMP(J) = XCMP(J)*(0.8750D0 + ZCMP(J))
   30    CONTINUE
C
         DO 40 J=1,N
            TCMP(J) = 0.50D0*XCMP(J)**2 - 1.0D0
   40    CONTINUE
         CALL DWCS(N,TCMP,BK1CS,NTK1,YCMP,B0,B1,B2)
         DO 50 J=1,N
            ZCMP(J) = LOG(0.50D0*XCMP(J))*ZCMP(J) +
     +                (0.750D0 + YCMP(J))/XCMP(J)
   50    CONTINUE
         CALL DWSCTR(N,ZCMP,INDX,F)
      ENDIF
C
C  -------------------------
C  CASE  2.0 .LT. X .LE. 8.0
C  -------------------------
C
      CALL DWGTLE(M,X,2.0D0,8.0D0,N,INDX)
      IF (N .GT. 0) THEN
         NTOT = NTOT + N
         CALL DWGTHR(N,X,INDX,XCMP) 
         DO 60 J=1,N
            TCMP(J) = (16.0D0/XCMP(J) - 5.0D0)/3.0D0
   60    CONTINUE
         CALL DWCS(N,TCMP,AK1CS,NTAK1,YCMP,B0,B1,B2)
         DO 70 J=1,N
            ZCMP(J) = EXP(-XCMP(J))*(1.250D0 + YCMP(J))/SQRT(XCMP(J))
   70    CONTINUE
         CALL DWSCTR(N,ZCMP,INDX,F)
      ENDIF
C
C  --------------------------
C  CASE  8.0 .LT. X .LE. XMAX
C  --------------------------
C
      CALL DWGTLE(M,X,8.0D0,XMAX,N,INDX)
      IF (N .GT. 0) THEN
         NTOT = NTOT + N
         CALL DWGTHR(N,X,INDX,XCMP) 
         DO 80 J=1,N
            TCMP(J) = 16.0D0/XCMP(J) - 1.0D0
   80    CONTINUE
         CALL DWCS(N,TCMP,AK12CS,NTAK12,YCMP,B0,B1,B2)
         DO 90 J=1,N
            ZCMP(J) = EXP(-XCMP(J))*(1.250D0 + YCMP(J))/SQRT(XCMP(J))
   90    CONTINUE
         CALL DWSCTR(N,ZCMP,INDX,F)
      ENDIF
C
C  -----
C  EXITS
C  -----
C
C     ... NORMAL
C
      INFO = 0
      IF (NTOT .NE. M)  INFO = -1
      GO TO 999
C
C     ... M .LE. 0
C
  910 CONTINUE
      INFO = 1
      GO TO 999
C
C     ... X IS ZERO OR NEGATIVE
C
  920 CONTINUE
      INFO = 2
      INDX(1) = KEY
      GO TO 999
C
C     ... X SO SMALL K1 OVERFLOWS
C
  930 CONTINUE
      INFO = 3
      INDX(1) = KEY
      GO TO 999
C
  999 CONTINUE
      RETURN
      END

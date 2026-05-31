c      SUBROUTINE DVK0 (M, X, F, WORK, IWORK, INFO)
      SUBROUTINE DVK0 (M, X, F, INFO)
C***BEGIN PROLOGUE  DVK0
C***PURPOSE  Computes the hyperbolic Bessel function of the third kind
C            of order zero (K0) for a vector of real arguments
C***LIBRARY   VFNLIB
C***CATEGORY  C10B1
C***TYPE      DOUBLE PRECISION (VK0-S, DVK0-D)
C***KEYWORDS  BESSEL FUNCTION,THIRD KIND,HYPERBOLIC BESSEL FUNCTION,
C             MODIFIED BESSEL FUNCTION, ORDER ZERO, VECTORIZED
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
C   DVK0 computes the modified (hyperbolic) Bessel function of the
C   third kind of order zero (K0) for real arguments using uniform
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
C           -1   Yes  Warning: Some X(i) so big K0 underflows. 
C                     The corresponding F(i) are set to zero.
C            0   Yes  Successfull execution.
C            1   No   Error: M .LE. 0
C            2   No   Error: Some X(i) is zero or negative.
C                     The index of the first offending argument is
C                     returned in IWORK(1).
C
C
C *********************************************************************
C   This routine is a modification of the function DBESK0 developed by
C   W. Fullerton of LANL.
C *********************************************************************
C
C***REFERENCES  Ronald F. Boisvert and Bonita V. Saunders, Portable
C               Vectorized Software for Bessel Function Evaluation,
C               ACM Transactions on Mathematical Software 18 (1992),
C               pp 456-469.
C***ROUTINES CALLED  DWK0
C***REVISION HISTORY  (YYMMDD)
C   910226  DATE WRITTEN
C   930203  Minor modifications to prologue.
C***END PROLOGUE  DVK0
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
C***FIRST EXECUTABLE STATEMENT  DVK0
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
C     ... DWK0 DOES ALL THE WORK
C
      CALL DWK0(M,X,F,WORK(IWTC),WORK(IWXC),WORK(IWYC),WORK(IWZC),
     +         IWORK(JIN),WORK(IWB0),WORK(IWB1),WORK(IWB2),INFO)
C
      RETURN
      END


      SUBROUTINE DWK0 (M, X, F, TCMP, XCMP, YCMP, ZCMP, INDX, B0, B1,  
     +   B2, INFO)
C***BEGIN PROLOGUE  DWK0
C***SUBSIDIARY
C***PURPOSE  Computes the hyperbolic Bessel function of the third kind
C            of order zero (K0) for a vector of arguments
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
C***END PROLOGUE  DWK0
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
      INTEGER LAK0, LAK02, LBI0, LBK0
      PARAMETER ( LAK0=38, LAK02=33, LBI0=18, LBK0=16 )
C
      INTEGER I, IDWCS, J, KEY, N, NTI0, NTK0, NTAK0, NTAK02, NTOT
      DOUBLE PRECISION AK0CS, AK02CS, BI0CS, BK0CS, EPMACH, EPS, D1MACH, 
     +        XMAX, XSML
C
      DIMENSION  AK0CS(LAK0), AK02CS(LAK02), BI0CS(LBI0), BK0CS(LBK0)
C
      SAVE BK0CS, AK0CS, AK02CS, BI0CS, N, NTAK0, NTAK02, NTI0, NTK0, 
     +     XMAX, XSML
C
C----------------------------------------------------------------------
C
C Series for BK0        on the interval  0.          to  4.00000D+00
C                                        with weighted error   3.08E-33
C                                         log weighted error  32.51
C                               significant figures required  32.05
C                                    decimal places required  33.11
C
      DATA BK0 CS(  1) / -.3532739323 3902768720 1140060063 153 D-1    /
      DATA BK0 CS(  2) / +.3442898999 2462848688 6344927529 213 D+0    /
      DATA BK0 CS(  3) / +.3597993651 5361501626 5721303687 231 D-1    /
      DATA BK0 CS(  4) / +.1264615411 4469259233 8479508673 447 D-2    /
      DATA BK0 CS(  5) / +.2286212103 1194517860 8269830297 585 D-4    /
      DATA BK0 CS(  6) / +.2534791079 0261494573 0790013428 354 D-6    /
      DATA BK0 CS(  7) / +.1904516377 2202088589 7214059381 366 D-8    /
      DATA BK0 CS(  8) / +.1034969525 7633624585 1008317853 089 D-10   /
      DATA BK0 CS(  9) / +.4259816142 7910825765 2445327170 133 D-13   /
      DATA BK0 CS( 10) / +.1374465435 8807508969 4238325440 000 D-15   /
      DATA BK0 CS( 11) / +.3570896528 5083735909 9688597333 333 D-18   /
      DATA BK0 CS( 12) / +.7631643660 1164373766 7498666666 666 D-21   /
      DATA BK0 CS( 13) / +.1365424988 4407818590 8053333333 333 D-23   /
      DATA BK0 CS( 14) / +.2075275266 9066680831 9999999999 999 D-26   /
      DATA BK0 CS( 15) / +.2712814218 0729856000 0000000000 000 D-29   /
      DATA BK0 CS( 16) / +.3082593887 9146666666 6666666666 666 D-32   /
C
C-------------------------------------------------------------------
C
C Series for AK0        on the interval  1.25000D-01 to  5.00000D-01
C                                        with weighted error   2.85E-32
C                                         log weighted error  31.54
C                               significant figures required  30.19
C                                    decimal places required  32.33
C
      DATA AK0 CS(  1) / -.7643947903 3279414240 8297827008 8 D-1      /
      DATA AK0 CS(  2) / -.2235652605 6998190520 2309555079 1 D-1      /
      DATA AK0 CS(  3) / +.7734181154 6938582353 0061817404 7 D-3      /
      DATA AK0 CS(  4) / -.4281006688 8860994644 5214643541 6 D-4      /
      DATA AK0 CS(  5) / +.3081700173 8629747436 5001482666 0 D-5      /
      DATA AK0 CS(  6) / -.2639367222 0096649740 6744889272 3 D-6      /
      DATA AK0 CS(  7) / +.2563713036 4034692062 9408826574 2 D-7      /
      DATA AK0 CS(  8) / -.2742705549 9002012638 5721191524 4 D-8      /
      DATA AK0 CS(  9) / +.3169429658 0974995920 8083287340 3 D-9      /
      DATA AK0 CS( 10) / -.3902353286 9621841416 0106571796 2 D-10     /
      DATA AK0 CS( 11) / +.5068040698 1885754020 5009212728 6 D-11     /
      DATA AK0 CS( 12) / -.6889574741 0078706795 4171355798 4 D-12     /
      DATA AK0 CS( 13) / +.9744978497 8259176913 8820133683 1 D-13     /
      DATA AK0 CS( 14) / -.1427332841 8845485053 8985534012 2 D-13     /
      DATA AK0 CS( 15) / +.2156412571 0214630395 5806297652 7 D-14     /
      DATA AK0 CS( 16) / -.3349654255 1495627721 8878205853 0 D-15     /
      DATA AK0 CS( 17) / +.5335260216 9529116921 4528039260 1 D-16     /
      DATA AK0 CS( 18) / -.8693669980 8907538076 3962237883 7 D-17     /
      DATA AK0 CS( 19) / +.1446404347 8622122278 8776344234 6 D-17     /
      DATA AK0 CS( 20) / -.2452889825 5001296824 0467875157 3 D-18     /
      DATA AK0 CS( 21) / +.4233754526 2321715728 2170634240 0 D-19     /
      DATA AK0 CS( 22) / -.7427946526 4544641956 9534129493 3 D-20     /
      DATA AK0 CS( 23) / +.1323150529 3926668662 7796746240 0 D-20     /
      DATA AK0 CS( 24) / -.2390587164 7396494513 3598146559 9 D-21     /
      DATA AK0 CS( 25) / +.4376827585 9232261401 6571255466 6 D-22     /
      DATA AK0 CS( 26) / -.8113700607 3451180593 3901141333 3 D-23     /
      DATA AK0 CS( 27) / +.1521819913 8321729583 1037815466 6 D-23     /
      DATA AK0 CS( 28) / -.2886041941 4833977702 3595861333 3 D-24     /
      DATA AK0 CS( 29) / +.5530620667 0547179799 9261013333 3 D-25     /
      DATA AK0 CS( 30) / -.1070377329 2498987285 9163306666 6 D-25     /
      DATA AK0 CS( 31) / +.2091086893 1423843002 9632853333 3 D-26     /
      DATA AK0 CS( 32) / -.4121713723 6462038274 1026133333 3 D-27     /
      DATA AK0 CS( 33) / +.8193483971 1213076401 3568000000 0 D-28     /
      DATA AK0 CS( 34) / -.1642000275 4592977267 8075733333 3 D-28     /
      DATA AK0 CS( 35) / +.3316143281 4802271958 9034666666 6 D-29     /
      DATA AK0 CS( 36) / -.6746863644 1452959410 8586666666 6 D-30     /
      DATA AK0 CS( 37) / +.1382429146 3184246776 3541333333 3 D-30     /
      DATA AK0 CS( 38) / -.2851874167 3598325708 1173333333 3 D-31     /
C
C-------------------------------------------------------------------
C
C Series for AK02       on the interval  0.          to  1.25000D-01
C                                        with weighted error   2.30E-32
C                                         log weighted error  31.64
C                               significant figures required  29.68
C                                    decimal places required  32.40
C
      DATA AK02CS(  1) / -.1201869826 3075922398 3934621245 2 D-1      /
      DATA AK02CS(  2) / -.9174852691 0256953106 5256107571 3 D-2      /
      DATA AK02CS(  3) / +.1444550931 7750058210 4884387805 7 D-3      /
      DATA AK02CS(  4) / -.4013614175 4357097286 7102107787 9 D-5      /
      DATA AK02CS(  5) / +.1567831810 8523106725 9034899033 3 D-6      /
      DATA AK02CS(  6) / -.7770110438 5217377103 1579975446 0 D-8      /
      DATA AK02CS(  7) / +.4611182576 1797178825 3313052958 6 D-9      /
      DATA AK02CS(  8) / -.3158592997 8605657705 2666580330 9 D-10     /
      DATA AK02CS(  9) / +.2435018039 3650411278 3588781432 9 D-11     /
      DATA AK02CS( 10) / -.2074331387 3983478977 0985337350 6 D-12     /
      DATA AK02CS( 11) / +.1925787280 5899170847 4273650469 3 D-13     /
      DATA AK02CS( 12) / -.1927554805 8389561036 0034718221 8 D-14     /
      DATA AK02CS( 13) / +.2062198029 1978182782 8523786964 4 D-15     /
      DATA AK02CS( 14) / -.2341685117 5792424026 0364019507 1 D-16     /
      DATA AK02CS( 15) / +.2805902810 6430422468 1517882845 8 D-17     /
      DATA AK02CS( 16) / -.3530507631 1618079458 1548246357 3 D-18     /
      DATA AK02CS( 17) / +.4645295422 9351082674 2421633706 6 D-19     /
      DATA AK02CS( 18) / -.6368625941 3442664739 2205346133 3 D-20     /
      DATA AK02CS( 19) / +.9069521310 9865155676 2234880000 0 D-21     /
      DATA AK02CS( 20) / -.1337974785 4236907398 4500531199 9 D-21     /
      DATA AK02CS( 21) / +.2039836021 8599523155 2208896000 0 D-22     /
      DATA AK02CS( 22) / -.3207027481 3678405000 6086997333 3 D-23     /
      DATA AK02CS( 23) / +.5189744413 6623099636 2635946666 6 D-24     /
      DATA AK02CS( 24) / -.8629501497 5405721929 6460799999 9 D-25     /
      DATA AK02CS( 25) / +.1472161183 1025598552 0803840000 0 D-25     /
      DATA AK02CS( 26) / -.2573069023 8670112838 1235199999 9 D-26     /
      DATA AK02CS( 27) / +.4601774086 6435165873 7664000000 0 D-27     /
      DATA AK02CS( 28) / -.8411555324 2010937371 3066666666 6 D-28     /
      DATA AK02CS( 29) / +.1569806306 6353689393 0154666666 6 D-28     /
      DATA AK02CS( 30) / -.2988226453 0057577889 7919999999 9 D-29     /
      DATA AK02CS( 31) / +.5796831375 2168365206 1866666666 6 D-30     /
      DATA AK02CS( 32) / -.1145035994 3476813321 5573333333 3 D-30     /
      DATA AK02CS( 33) / +.2301266594 2496828020 0533333333 3 D-31     /
C
C-------------------------------------------------------------------
C
C Series for BI0        on the interval  0.          to  9.00000D+00
C                                        with weighted error   9.51E-34
C                                         log weighted error  33.02
C                               significant figures required  33.31
C                                    decimal places required  33.65
C
      DATA BI0 CS(  1) / -.7660547252 8391449510 8189497624 3285 D-1   /
      DATA BI0 CS(  2) / +.1927337953 9938082699 5240875088 1196 D+1   /
      DATA BI0 CS(  3) / +.2282644586 9203013389 3702929233 0415 D+0   /
      DATA BI0 CS(  4) / +.1304891466 7072904280 7933421069 1888 D-1   /
      DATA BI0 CS(  5) / +.4344270900 8164874513 7868268102 6107 D-3   /
      DATA BI0 CS(  6) / +.9422657686 0019346639 2317174411 8766 D-5   /
      DATA BI0 CS(  7) / +.1434006289 5106910799 6209187817 9957 D-6   /
      DATA BI0 CS(  8) / +.1613849069 6617490699 1541971999 4611 D-8   /
      DATA BI0 CS(  9) / +.1396650044 5356696994 9509270814 2522 D-10  /
      DATA BI0 CS( 10) / +.9579451725 5054453446 2752317189 3333 D-13  /
      DATA BI0 CS( 11) / +.5333981859 8625021310 1510774400 0000 D-15  /
      DATA BI0 CS( 12) / +.2458716088 4374707746 9678591999 9999 D-17  /
      DATA BI0 CS( 13) / +.9535680890 2487700269 4434133333 3333 D-20  /
      DATA BI0 CS( 14) / +.3154382039 7214273367 8933333333 3333 D-22  /
      DATA BI0 CS( 15) / +.9004564101 0946374314 6666666666 6666 D-25  /
      DATA BI0 CS( 16) / +.2240647369 1236700160 0000000000 0000 D-27  /
      DATA BI0 CS( 17) / +.4903034603 2428373333 3333333333 3333 D-30  /
      DATA BI0 CS( 18) / +.9508172606 1226666666 6666666666 6666 D-33  /

C
C-------------------------------------------------------------------
C
      DATA NTK0 / 0 /
C
C***FIRST EXECUTABLE STATEMENT  DWK0
C
      IF (M .LE. 0)  GO TO 910
C
      IF (NTK0 .EQ. 0) THEN
         EPMACH = D1MACH(3)
         EPS = 0.10D0*EPMACH
         NTK0 = IDWCS(BK0CS, LBK0, EPS)
         NTAK0 = IDWCS(AK0CS, LAK0, EPS)
         NTAK02 = IDWCS(AK02CS, LAK02, EPS)
         NTI0 = IDWCS(BI0CS, LBI0, EPS)
         XSML = SQRT (4.0D0*EPMACH)
         XMAX = -LOG(D1MACH(1))
         XMAX = XMAX - 0.50D0*XMAX*LOG(XMAX)/(XMAX+0.50D0) - 0.010D0
      ENDIF
C
      NTOT = 0
C
      CALL DWNLE(M,X,0.0D0,KEY)
      IF (KEY .NE. 0)  GO TO 920
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
C        ... COMPUTE I0(X) ... RESULT IN ZCMP
C
         DO 10 J=1,N
            TCMP(J) = XCMP(J)**2/4.50D0 - 1.0D0
   10    CONTINUE
         CALL DWCS(N,TCMP,BI0CS,NTI0,ZCMP,B0,B1,B2)
         DO 15 J=1,N
            ZCMP(J) = 2.750D0 + ZCMP(J)
   15    CONTINUE
C
         DO 20 J=1,N
            TCMP(J) = 0.50D0*XCMP(J)**2 - 1.0D0
   20    CONTINUE
         CALL DWCS(N,TCMP,BK0CS,NTK0,YCMP,B0,B1,B2)
         DO 30 J=1,N
            YCMP(J) = -LOG(0.50D0*XCMP(J))*ZCMP(J) - 0.250D0 + YCMP(J)
   30    CONTINUE
         CALL DWSCTR(N,YCMP,INDX,F)
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
         DO 40 J=1,N
            TCMP(J) = (16.0D0/XCMP(J) - 5.0D0)/3.0D0
   40    CONTINUE
         CALL DWCS(N,TCMP,AK0CS,NTAK0,ZCMP,B0,B1,B2)
         DO 50 J=1,N
            YCMP(J) = EXP(-XCMP(J))*(1.250D0 + ZCMP(J))/SQRT(XCMP(J))
   50    CONTINUE
         CALL DWSCTR(N,YCMP,INDX,F)
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
         DO 60 J=1,N
            TCMP(J) = 16.0D0/XCMP(J) - 1.0D0
   60    CONTINUE
         CALL DWCS(N,TCMP,AK02CS,NTAK02,ZCMP,B0,B1,B2)
         DO 70 J=1,N
            YCMP(J) = EXP(-XCMP(J))*(1.250D0 + ZCMP(J))/SQRT(XCMP(J))
   70    CONTINUE
         CALL DWSCTR(N,YCMP,INDX,F)
      ENDIF
C
C  -----
C  EXITS
C  -----
C
C     ... NORMAL
C
      INFO = 0
      GO TO 999
C
C     ... M .LE. 0
C
  910 CONTINUE
      INFO = 1
      IF (NTOT .NE. M)  INFO = -1
      GO TO 999
C
C     ... X IS ZERO OR NEGATIVE
C
  920 CONTINUE
      INFO = 2
      INDX(1) = KEY
      GO TO 999
C
  999 CONTINUE
      RETURN
      END

      SUBROUTINE DQK15I(F,BOUN,INF,A,B,RESULT,ABSERR,RESABS,RESASC)
C***BEGIN PROLOGUE  DQK15I
C***DATE WRITTEN   800101   (YYMMDD)
C***REVISION DATE  830518   (YYMMDD)
C***CATEGORY NO.  H2A3A2,H2A4A2
C***KEYWORDS  15-POINT TRANSFORMED GAUSS-KRONROD RULES
C***AUTHOR  PIESSENS,ROBERT,APPL. MATH. & PROGR. DIV. - K.U.LEUVEN
C           DE DONCKER,ELISE,APPL. MATH. & PROGR. DIV. - K.U.LEUVEN
C***PURPOSE  THE ORIGINAL (INFINITE INTEGRATION RANGE IS MAPPED
C            ONTO THE INTERVAL (0,1) AND (A,B) IS A PART OF (0,1).
C            IT IS THE PURPOSE TO COMPUTE
C            I = INTEGRAL OF TRANSFORMED INTEGRAND OVER (A,B),
C            J = INTEGRAL OF ABS(TRANSFORMED INTEGRAND) OVER (A,B).
C***DESCRIPTION
C
C           INTEGRATION RULE
C           STANDARD FORTRAN SUBROUTINE
C           DOUBLE PRECISION VERSION
C
C           PARAMETERS
C            ON ENTRY
C              F      - DOUBLE PRECISION
C                       FUCTION SUBPROGRAM DEFINING THE INTEGRAND
C                       FUNCTION F(X). THE ACTUAL NAME FOR F NEEDS TO BE
C                       DECLARED E X T E R N A L IN THE CALLING PROGRAM.
C
C              BOUN   - DOUBLE PRECISION
C                       FINITE BOUND OF ORIGINAL INTEGRATION
C                       RANGE (SET TO ZERO IF INF = +2)
C
C              INF    - INTEGER
C                       IF INF = -1, THE ORIGINAL INTERVAL IS
C                                   (-INFINITY,BOUND),
C                       IF INF = +1, THE ORIGINAL INTERVAL IS
C                                   (BOUND,+INFINITY),
C                       IF INF = +2, THE ORIGINAL INTERVAL IS
C                                   (-INFINITY,+INFINITY) AND
C                       THE INTEGRAL IS COMPUTED AS THE SUM OF TWO
C                       INTEGRALS, ONE OVER (-INFINITY,0) AND ONE OVER
C                       (0,+INFINITY).
C
C              A      - DOUBLE PRECISION
C                       LOWER LIMIT FOR INTEGRATION OVER SUBRANGE
C                       OF (0,1)
C
C              B      - DOUBLE PRECISION
C                       UPPER LIMIT FOR INTEGRATION OVER SUBRANGE
C                       OF (0,1)
C
C            ON RETURN
C              RESULT - DOUBLE PRECISION
C                       APPROXIMATION TO THE INTEGRAL I
C                       RESULT IS COMPUTED BY APPLYING THE 15-POINT
C                       KRONROD RULE(RESK) OBTAINED BY OPTIMAL ADDITION
C                       OF ABSCISSAE TO THE 7-POINT GAUSS RULE(RESG).
C
C              ABSERR - DOUBLE PRECISION
C                       ESTIMATE OF THE MODULUS OF THE ABSOLUTE ERROR,
C                       WHICH SHOULD EQUAL OR EXCEED ABS(I-RESULT)
C
C              RESABS - DOUBLE PRECISION
C                       APPROXIMATION TO THE INTEGRAL J
C
C              RESASC - DOUBLE PRECISION
C                       APPROXIMATION TO THE INTEGRAL OF
C                       ABS((TRANSFORMED INTEGRAND)-I/(B-A)) OVER (A,B)
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  D1MACH
C***END PROLOGUE  DQK15I
C
      DOUBLE PRECISION A,ABSC,ABSC1,ABSC2,ABSERR,B,BOUN,CENTR,DABS,DINF,
     *  DMAX1,DMIN1,D1MACH,EPMACH,F,FC,FSUM,FVAL1,FVAL2,FV1,FV2,HLGTH,
     *  RESABS,RESASC,RESG,RESK,RESKH,RESULT,TABSC1,TABSC2,UFLOW,WG,WGK,
     *  XGK
      INTEGER INF,J
      EXTERNAL F
C
      DIMENSION FV1(7),FV2(7),XGK(8),WGK(8),WG(8)
C
C           THE ABSCISSAE AND WEIGHTS ARE SUPPLIED FOR THE INTERVAL
C           (-1,1).  BECAUSE OF SYMMETRY ONLY THE POSITIVE ABSCISSAE AND
C           THEIR CORRESPONDING WEIGHTS ARE GIVEN.
C
C           XGK    - ABSCISSAE OF THE 15-POINT KRONROD RULE
C                    XGK(2), XGK(4), ... ABSCISSAE OF THE 7-POINT
C                    GAUSS RULE
C                    XGK(1), XGK(3), ...  ABSCISSAE WHICH ARE OPTIMALLY
C                    ADDED TO THE 7-POINT GAUSS RULE
C
C           WGK    - WEIGHTS OF THE 15-POINT KRONROD RULE
C
C           WG     - WEIGHTS OF THE 7-POINT GAUSS RULE, CORRESPONDING
C                    TO THE ABSCISSAE XGK(2), XGK(4), ...
C                    WG(1), WG(3), ... ARE SET TO ZERO.
C
      DATA WG(1) / 0.0D0 /
      DATA WG(2) / 0.1294849661 6886969327 0611432679 082D0 /
      DATA WG(3) / 0.0D0 /
      DATA WG(4) / 0.2797053914 8927666790 1467771423 780D0 /
      DATA WG(5) / 0.0D0 /
      DATA WG(6) / 0.3818300505 0511894495 0369775488 975D0 /
      DATA WG(7) / 0.0D0 /
      DATA WG(8) / 0.4179591836 7346938775 5102040816 327D0 /
C
      DATA XGK(1) / 0.9914553711 2081263920 6854697526 329D0 /
      DATA XGK(2) / 0.9491079123 4275852452 6189684047 851D0 /
      DATA XGK(3) / 0.8648644233 5976907278 9712788640 926D0 /
      DATA XGK(4) / 0.7415311855 9939443986 3864773280 788D0 /
      DATA XGK(5) / 0.5860872354 6769113029 4144838258 730D0 /
      DATA XGK(6) / 0.4058451513 7739716690 6606412076 961D0 /
      DATA XGK(7) / 0.2077849550 0789846760 0689403773 245D0 /
      DATA XGK(8) / 0.0000000000 0000000000 0000000000 000D0 /
C
      DATA WGK(1) / 0.0229353220 1052922496 3732008058 970D0 /
      DATA WGK(2) / 0.0630920926 2997855329 0700663189 204D0 /
      DATA WGK(3) / 0.1047900103 2225018383 9876322541 518D0 /
      DATA WGK(4) / 0.1406532597 1552591874 5189590510 238D0 /
      DATA WGK(5) / 0.1690047266 3926790282 6583426598 550D0 /
      DATA WGK(6) / 0.1903505780 6478540991 3256402421 014D0 /
      DATA WGK(7) / 0.2044329400 7529889241 4161999234 649D0 /
      DATA WGK(8) / 0.2094821410 8472782801 2999174891 714D0 /
C
C
C           LIST OF MAJOR VARIABLES
C           -----------------------
C
C           CENTR  - MID POINT OF THE INTERVAL
C           HLGTH  - HALF-LENGTH OF THE INTERVAL
C           ABSC*  - ABSCISSA
C           TABSC* - TRANSFORMED ABSCISSA
C           FVAL*  - FUNCTION VALUE
C           RESG   - RESULT OF THE 7-POINT GAUSS FORMULA
C           RESK   - RESULT OF THE 15-POINT KRONROD FORMULA
C           RESKH  - APPROXIMATION TO THE MEAN VALUE OF THE TRANSFORMED
C                    INTEGRAND OVER (A,B), I.E. TO I/(B-A)
C
C           MACHINE DEPENDENT CONSTANTS
C           ---------------------------
C
C           EPMACH IS THE LARGEST RELATIVE SPACING.
C           UFLOW IS THE SMALLEST POSITIVE MAGNITUDE.
C
C***FIRST EXECUTABLE STATEMENT  DQK15I
      EPMACH = D1MACH(4)
      UFLOW = D1MACH(1)
      DINF = MIN0(1,INF)
C
      CENTR = 0.5D+00*(A+B)
      HLGTH = 0.5D+00*(B-A)
      TABSC1 = BOUN+DINF*(0.1D+01-CENTR)/CENTR
      FVAL1 = F(TABSC1)
      IF(INF.EQ.2) FVAL1 = FVAL1+F(-TABSC1)
      FC = (FVAL1/CENTR)/CENTR
C
C           COMPUTE THE 15-POINT KRONROD APPROXIMATION TO
C           THE INTEGRAL, AND ESTIMATE THE ERROR.
C
      RESG = WG(8)*FC
      RESK = WGK(8)*FC
      RESABS = DABS(RESK)
      DO 10 J=1,7
        ABSC = HLGTH*XGK(J)
        ABSC1 = CENTR-ABSC
        ABSC2 = CENTR+ABSC
        TABSC1 = BOUN+DINF*(0.1D+01-ABSC1)/ABSC1
        TABSC2 = BOUN+DINF*(0.1D+01-ABSC2)/ABSC2
        FVAL1 = F(TABSC1)
        FVAL2 = F(TABSC2)
        IF(INF.EQ.2) FVAL1 = FVAL1+F(-TABSC1)
        IF(INF.EQ.2) FVAL2 = FVAL2+F(-TABSC2)
        FVAL1 = (FVAL1/ABSC1)/ABSC1
        FVAL2 = (FVAL2/ABSC2)/ABSC2
        FV1(J) = FVAL1
        FV2(J) = FVAL2
        FSUM = FVAL1+FVAL2
        RESG = RESG+WG(J)*FSUM
        RESK = RESK+WGK(J)*FSUM
        RESABS = RESABS+WGK(J)*(DABS(FVAL1)+DABS(FVAL2))
   10 CONTINUE
      RESKH = RESK*0.5D+00
      RESASC = WGK(8)*DABS(FC-RESKH)
      DO 20 J=1,7
        RESASC = RESASC+WGK(J)*(DABS(FV1(J)-RESKH)+DABS(FV2(J)-RESKH))
   20 CONTINUE
      RESULT = RESK*HLGTH
      RESASC = RESASC*HLGTH
      RESABS = RESABS*HLGTH
      ABSERR = DABS((RESK-RESG)*HLGTH)
      IF(RESASC.NE.0.0D+00.AND.ABSERR.NE.0.D0) ABSERR = RESASC*
     * DMIN1(0.1D+01,(0.2D+03*ABSERR/RESASC)**1.5D+00)
      IF(RESABS.GT.UFLOW/(0.5D+02*EPMACH)) ABSERR = DMAX1
     * ((EPMACH*0.5D+02)*RESABS,ABSERR)
      RETURN
      END
C
C     THIS DRIVER TESTS  EISPACK  FOR THE CLASS OF COMPLEX GENERAL
C     MATRICES SUMMARIZING THE FIGURES OF MERIT FOR ALL PATHS.
C
C     THIS DRIVER IS CATALOGUED AS  EISPDRV4(CGSUMARY).
C
C     THE DIMENSION OF  AR,AI,ZR,ZI,ASAVER,ASAVEI,RM1,  AND  RM2 SHOULD
C     BE  NM  BY  NM.
C     THE DIMENSION OF  WR,WI,WR1,WI1,SELECT,SLHOLD,INT,SCALE,ORTR,ORTI,
C     RV1  AND  RV2  SHOULD BE  NM.
C     THE DIMENSION OF  ARHOLD  AND  AIHOLD  SHOULD BE  NM  BY  NM.
C     HERE NM = 20.
C
      DOUBLE PRECISION AR( 20, 20),AI( 20, 20),ZR( 20, 20),ZI( 20, 20),
     X        ASAVER( 20, 20),ASAVEI( 20, 20),RM1( 20, 20),RM2( 20, 20),
     X        ORTR( 20),ORTI( 20),WR( 20),WI( 20),
     X        WR1( 20),WI1( 20),SCALE( 20),RV1( 20),RV2( 20),
     X        TCRIT( 8),EPSLON,RESDUL,DFL
      DOUBLE PRECISION ARHOLD( 20, 20),AIHOLD( 20, 20)
      INTEGER  INT( 20),IERR( 8),MATCH( 4),LOW,UPP,ERROR,SAME,DIFF
      LOGICAL  SELECT( 20),SLHOLD( 20)
      DATA IREAD1/1/,IREADC/5/,IWRITE/6/
      DATA  SAME,DIFF,LR,QR/4HSAME,4HDIFF,1HL,1HQ/
C
      OPEN(UNIT=IREAD1,FILE='FILE41')
      OPEN(UNIT=IREADC,FILE='FILE42')
      REWIND IREAD1
      REWIND IREADC
C
      NM = 20
      LCOUNT = 0
      WRITE(IWRITE,1)
    1 FORMAT(1H1,19X,57H EXPLANATION OF COLUMN ENTRIES FOR THE SUMMARY S
     XTATISTICS//1H ,95(1H-)/ 26X,9HBALANCING,15X,12HNO BALANCING,10X,
     X16HVECTORS SELECTED/ 18X,3(24H------------------------,1X)/
     X16H ORDER LOW UPP T,2X,
     X2(25H  COM-R2 COM-R   CINVIT     )/1H ,95(1H-)//
     X95H 'BALANCING' IS THE OPTION THAT EMPLOYS SUBROUTINE  CBAL  TO  B
     XALANCE THE MATRIX BEFORE THE    /
     X95H EIGENVALUES ARE COMPUTED AND  CBABK2  TO BACK TRANSFORM THE EI
     XGENVECTORS.                     //
     X95H 'VECTORS COMPUTED' IDENTIFIES BY POSITION, IN THE SET RETURNED
     X BY  COM-R, THOSE EIGENVALUES   /
     X95H FOR WHICH  CINVIT  COMPUTED THE ASSOCIATED EIGENVECTORS.  T  I
     XNDEXES AN EIGENVALUE FOR WHICH  /
     X66H THE EIGENVECTOR WAS COMPUTED AND  F  INDEXES A PASSED EIGENVAL
     XUE.    // 51H UNDER 'ORDER' IS THE ORDER OF EACH TEST MATRIX.   //
     X95H UNDER 'LOW' AND 'UPP' ARE INTEGERS INDICATING THE BOUNDARY IND
     XICES FOR THE BALANCED MATRIX.   //
     X95H UNDER 'T' IS THE LETTER L OR Q INDICATING THE USE OF ELEMENTAR
     XY OR UNITARY TRANSFORMATIONS.   /)
      WRITE(IWRITE,2)
    2 FORMAT(
     X61H PERFORMANCE REPORTED UNDER 'COM-R2 COM-R' IS THAT OF  COMLR2 ,
     X31H  AND  COMLR  ON LINE HEADED BY   /
     X60H 'L' AND THAT OF  COMQR2  AND  COMQR  ON LINE HEADED BY 'Q'.//
     X95H UNDER 'COM-R2 COM-R' ARE TWO NUMBERS AND A KEYWORD.  THE FIRST
     X NUMBER, AN INTEGER, IS THE     /
     X95H ABSOLUTE SUM OF THE ERROR FLAGS RETURNED SEPARATELY FROM  COM-
     XR2  AND  COM-R.  THE SECOND     /
     X95H NUMBER IS THE MEASURE OF PERFORMANCE BASED UPON THE RESIDUAL C
     XOMPUTED FOR THE  COM-R2  PATH.  /
     X95H THE KEYWORD REPORTS THE DUPLICATION OF THE COMPUTED EIGENVALUE
     XS FROM  COM-R2  AND  COM-R.     /
     X95H 'SAME' MEANS THAT THE EIGENVALUES ARE EXACT DUPLICATES.  'DIFF
     X' MEANS THAT FOR AT LEAST ONE   /
     X95H PAIR OF CORRESPONDING EIGENVALUES, THE MEMBERS OF THE PAIR ARE
     X NOT IDENTICAL.                 //
     X95H UNDER 'CINVIT' ARE TWO NUMBERS.  THE FIRST NUMBER, AN INTEGER,
     X IS THE ABSOLUTE SUM OF THE     /
     X62H ERROR FLAGS RETURNED FROM THE PATH.  THE SECOND NUMBER IS THE,
     X29H MEASURE OF PERFORMANCE BASED    )
      WRITE(IWRITE,3)
    3 FORMAT(
     X41H UPON THE RESIDUAL COMPUTED FOR THE PATH.//
     X62H -1.0  AS THE MEASURE OF PERFORMANCE IS PRINTED IF AN ERROR IN,
     X27H THE CORRESPONDING PATH HAS      /
     X47H PREVENTED THE COMPUTATION OF THE EIGENVECTORS.//
     X45H0FOR REDUCTIONS BY ELEMENTARY TRANSFORMATIONS/
     X95H THE  COMLR2   PATH WITH    BALANCING USES THE EISPACK CODES  C
     XBAL  -COMHES-COMLR2-CBABK2.     /
     X95H THE   COMLR   PATH WITH    BALANCING USES THE EISPACK CODES  C
     XBAL  -COMHES-COMLR .            /
     X60H THE  CINVIT   PATH WITH    BALANCING USES THE EISPACK CODES ,
     X44H  CBAL  -COMHES-COMLR -CINVIT-COMBAK-CBABK2. /
     X95H THE  COMLR2   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XOMHES-COMLR2.                   /
     X95H THE   COMLR   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XOMHES-COMLR .                   /
     X95H THE  CINVIT   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XOMHES-COMLR -CINVIT-COMBAK.     )
      WRITE(IWRITE,4)
    4 FORMAT(42H0FOR REDUCTIONS BY UNITARY TRANSFORMATIONS/
     X95H THE  COMQR2   PATH WITH    BALANCING USES THE EISPACK CODES  C
     XBAL  -CORTH -COMQR2-CBABK2,           /
     X38H AS CALLED FROM DRIVER SUBROUTINE  CG. /
     X95H THE   COMQR   PATH WITH    BALANCING USES THE EISPACK CODES  C
     XBAL  -CORTH -COMQR ,            /
     X38H AS CALLED FROM DRIVER SUBROUTINE  CG. /
     X60H THE  CINVIT   PATH WITH    BALANCING USES THE EISPACK CODES ,
     X44H  CBAL  -CORTH -COMQR -CINVIT-CORTB -CBABK2. /
     X95H THE  COMQR2   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XORTH -COMQR2.                   /
     X95H THE   COMQR   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XORTH -COMQR .                   /
     X95H THE  CINVIT   PATH WITH NO BALANCING USES THE EISPACK CODES  C
     XORTH -COMQR -CINVIT-CORTB .     )
      WRITE(IWRITE,15)
   15 FORMAT(1X,21HD.P. VERSION 04/15/83 )
    5 FORMAT( 53H1       TABULATION OF THE ERROR FLAG  ERROR  AND THE ,
     X    31HMEASURE OF PERFORMANCE  Y  FOR /5X,
     X    56HTHE  EISPACK  CODES.  THIS RUN DISPLAYS THESE STATISTICS ,
     X    33H FOR COMPLEX GENERAL MATRICES.      //
     X    26X,9HBALANCING,15X,12HNO BALANCING,10X,16HVECTORS SELECTED/
     X    18X,3(24H------------------------,1X)/
     X    16H ORDER LOW UPP T,2X,2(25H  COM-R2 COM-R   CINVIT       ))
   10 CALL CMATIN(NM,N,AR,AI,ARHOLD,AIHOLD,0)
      READ(IREADC,50) MM,(SLHOLD(I),I=1,N)  
   50 FORMAT(I4/(72L1))
C
C     MM  AND  SELECT  ARE READ FROM SYSIN AFTER THE MATRIX IS
C     GENERATED.  MM  SPECIFIES TO  INVIT  THE MAXIMUM -UMBER OF
C     EIGENVECTORS THAT WILL BE "OMPUTED.  SELECT  CONTAINS INFORMATION
C     ABOUT WHICH EIGENVECTORS ARE DESIRED.
C
      DO  300  ICALL = 1,12
         ICALL2 = MOD(ICALL,3)
         IF( ICALL2 .NE. 0 ) GO TO 80
         DO  70  I = 1,N
   70    SELECT(I) = SLHOLD(I)
   80    IF( ICALL .NE. 1 )  CALL  CMATIN(NM,N,AR,AI,ARHOLD,AIHOLD,1)
         GO TO  (90,100,110,120,130,140,160,165,170,175,180,185), ICALL
C
C     IF  COM-R  PATH IS TAKEN THEN  COM-R2  PATH MUST ALSO BE TAKEN
C     IN ORDER THAT COMPARISON OF EIGENVALUES BE POSSIBLE.
C
C
C     CGEWZ
C
   90    ICT = 1
         CALL  CBAL(NM,N,AR,AI,LOW,UPP,SCALE)
         CALL  COMHES(NM,N,LOW,UPP,AR,AI,INT)
         CALL  COMLR2(NM,N,LOW,UPP,INT,AR,AI,WR,WI,ZR,ZI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 ) GO TO 220
         DO  91  I = 1,N
            WR1(I) = WR(I)
   91       WI1(I) = WI(I)
         CALL  CBABK2(NM,N,LOW,UPP,SCALE,N,ZR,ZI)
         GO TO  190
C
C     CGEW
C
  100    IF( IERR(1) .NE. 0 ) GO TO 300
         CALL  CBAL(NM,N,AR,AI,LOW,UPP,SCALE)
         CALL  COMHES(NM,N,LOW,UPP,AR,AI,INT)
         CALL  COMLR(NM,N,LOW,UPP,AR,AI,WR,WI,ERROR)
         IERR(1) = ERROR
         MATCH(1) = DIFF
         DO  101  I = 1,N
            IF( WR(I) .NE. WR1(I) .OR. WI(I) .NE. WI1(I) )  GO TO  300
  101    CONTINUE
         MATCH(1) = SAME
         GO TO  300
C
C     CGEW1Z
C
  110    ICT = 2
         CALL  CBAL(NM,N,AR,AI,LOW,UPP,SCALE)
         CALL  COMHES(NM,N,LOW,UPP,AR,AI,INT)
         DO  112  I = 1,N
            DO  112  J = 1,N
              ASAVER(I,J) = AR(I,J)
  112         ASAVEI(I,J) = AI(I,J)
         CALL  COMLR(NM,N,LOW,UPP,AR,AI,WR,WI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 )  GO TO  220
         CALL  CINVIT(NM,N,ASAVER,ASAVEI,WR,WI,SELECT,MM,M,ZR,ZI,
     X                ERROR,RM1,RM2,RV1,RV2)
         IERR(ICT) = IABS(ERROR)
         CALL  COMBAK(NM,LOW,UPP,ASAVER,ASAVEI,INT,M,ZR,ZI)
         CALL  CBABK2(NM,N,LOW,UPP,SCALE,M,ZR,ZI)
         GO TO  190
C
C     CGNEWZ
C
  120    ICT = 3
         CALL  COMHES(NM,N,1,N,AR,AI,INT)
         CALL  COMLR2(NM,N,1,N,INT,AR,AI,WR,WI,ZR,ZI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 ) GO TO 220
         DO  121  I = 1,N
            WR1(I) = WR(I)
  121       WI1(I) = WI(I)
         GO TO  190
C
C     CGNEW
C
  130    IF( IERR(3) .NE. 0 ) GO TO 300
         CALL  COMHES(NM,N,1,N,AR,AI,INT)
         CALL  COMLR(NM,N,1,N,AR,AI,WR,WI,ERROR)
         IERR(3) = ERROR
         MATCH(2) = DIFF
         DO  131  I = 1,N
            IF( WR(I) .NE. WR1(I) .OR. WI(I) .NE. WI1(I) )  GO TO  300
  131    CONTINUE
         MATCH(2) = SAME
         GO TO  300
C
C     CGNEW1Z
C
  140    ICT = 4
         CALL  COMHES(NM,N,1,N,AR,AI,INT)
         DO  142  I = 1,N
            DO  142  J = 1,N
              ASAVER(I,J) = AR(I,J)
  142         ASAVEI(I,J) = AI(I,J)
         CALL  COMLR(NM,N,1,N,AR,AI,WR,WI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 )  GO TO  220
         CALL  CINVIT(NM,N,ASAVER,ASAVEI,WR,WI,SELECT,MM,M,ZR,ZI,
     X                ERROR,RM1,RM2,RV1,RV2)
         IERR(ICT) = IABS(ERROR)
         CALL  COMBAK(NM,1,N,ASAVER,ASAVEI,INT,M,ZR,ZI)
         GO TO 190
C
C     CGUWZ
C     INVOKED FROM DRIVER SUBROUTINE  CG.
C
  160    ICT = 5
         CALL  CG(NM,N,AR,AI,WR,WI,1,ZR,ZI,SCALE,ORTR,ORTI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 ) GO TO 220
         GO TO  190
C
C     CGUW
C     INVOKED FROM DRIVER SUBROUTINE  CG.
C
  165    IF( IERR(5) .NE. 0 ) GO TO 300
         CALL  CG(NM,N,AR,AI,WR1,WI1,0,AR,AI,SCALE,ORTR,ORTI,ERROR)
         IERR(5) = ERROR
         MATCH(3) = DIFF
         DO  166  I = 1,N
            IF( WR(I) .NE. WR1(I) .OR. WI(I) .NE. WI1(I) )  GO TO  300
  166    CONTINUE
         MATCH(3) = SAME
         GO TO  300
C
C     CGUW1Z
C
  170    ICT = 6
         CALL  CBAL(NM,N,AR,AI,LOW,UPP,SCALE)
         CALL  CORTH(NM,N,LOW,UPP,AR,AI,ORTR,ORTI)
         DO  172  I = 1,N
            DO  172  J = 1,N
              ASAVER(I,J) = AR(I,J)
  172         ASAVEI(I,J) = AI(I,J)
         CALL  COMQR(NM,N,LOW,UPP,AR,AI,WR,WI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 )  GO TO  220
         CALL  CINVIT(NM,N,ASAVER,ASAVEI,WR,WI,SELECT,MM,M,ZR,ZI,
     X                        ERROR,RM1,RM2,RV1,RV2)
         IERR(ICT) = IABS(ERROR)
         CALL  CORTB(NM,LOW,UPP,ASAVER,ASAVEI,ORTR,ORTI,M,ZR,ZI)
         CALL  CBABK2(NM,N,LOW,UPP,SCALE,M,ZR,ZI)
         GO TO  190
C
C     CGNUWZ
C
  175    ICT = 7
         CALL  CORTH(NM,N,1,N,AR,AI,ORTR,ORTI)
         CALL  COMQR2(NM,N,1,N,ORTR,ORTI,AR,AI,WR,WI,ZR,ZI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 ) GO TO 220
         DO  176  I = 1,N
            WR1(I) = WR(I)
  176       WI1(I) = WI(I)
         GO TO  190
C
C     CGNUW
C
  180    IF( IERR(7) .NE. 0 ) GO TO 300
         CALL  CORTH(NM,N,1,N,AR,AI,ORTR,ORTI)
         CALL  COMQR(NM,N,1,N,AR,AI,WR,WI,ERROR)
         IERR(7) = ERROR
         MATCH(4) = DIFF
         DO  181  I = 1,N
            IF( WR(I) .NE. WR1(I) .OR. WI(I) .NE. WI1(I) )  GO TO  300
  181    CONTINUE
         MATCH(4) = SAME
         GO TO  300
C
C     CGNUW1Z
C
  185    ICT = 8
         CALL  CORTH(NM,N,1,N,AR,AI,ORTR,ORTI)
         DO  187  I = 1,N
            DO  187  J = 1,N
              ASAVER(I,J) = AR(I,J)
  187         ASAVEI(I,J) = AI(I,J)
         CALL  COMQR(NM,N,1,N,AR,AI,WR,WI,ERROR)
         IERR(ICT) = ERROR
         IF( ERROR .NE. 0 )  GO TO  220
         CALL  CINVIT(NM,N,ASAVER,ASAVEI,WR,WI,SELECT,MM,M,ZR,ZI,
     X                ERROR,RM1,RM2,RV1,RV2)
         IERR(ICT) = IABS(ERROR)
         CALL  CORTB(NM,1,N,ASAVER,ASAVEI,ORTR,ORTI,M,ZR,ZI)
C
  190    CALL  CMATIN(NM,N,AR,AI,ARHOLD,AIHOLD,1)
         IF( ICALL2 .EQ. 0 ) CALL CGW1ZR(NM,N,AR,AI,WR,WI,SELECT,
     X                                   M,ZR,ZI,RV1,RESDUL)
         IF( ICALL2 .EQ. 1 ) CALL CGWZR(NM,N,AR,AI,WR,WI,ZR,ZI,
     X                                  RV1,RESDUL)
         DFL = 10 * N
         TCRIT(ICT) = RESDUL/EPSLON(DFL)
         GO TO 300
  220    TCRIT(ICT) = -1.0D0
  300 CONTINUE
C
      IF( MOD(LCOUNT,16) .EQ. 0 ) WRITE(IWRITE,5)
      LCOUNT = LCOUNT + 1
      WRITE(IWRITE,520) N,LOW,UPP,LR,(IERR(2*I-1),TCRIT(2*I-1),MATCH(I),
     X              IERR(2*I),TCRIT(2*I),I=1,2),(SELECT(I),I=1,N)
  520 FORMAT(I4,1X,2I4,2X,A1,2(I4,F6.3,1X,A4,I4,F6.3),
     X       4X,21L1/(72X,20L1))
      WRITE(IWRITE,530) QR,(IERR(2*I-1),TCRIT(2*I-1),MATCH(I),
     X              IERR(2*I),TCRIT(2*I),I=3,4)
  530 FORMAT(15X,A1,2(I4,F6.3,1X,A4,I4,F6.3))
      GO TO  10
      END
      SUBROUTINE CMATIN(NM,N,AR,AI,ARHOLD,AIHOLD,INITIL)
C
C     THIS INPUT SUBROUTINE READS A COMPLEX MATRIX  A = (AR,AI)
C     FROM SYSIN OF ORDER N.
C     TO GENERATE THE MATRIX  A  INITIALLY,  INITIL  IS TO BE 0.
C     TO REGENERATE THE MATRIX  A  FOR THE PURPOSE OF THE RESIDUAL
C     CALCULATION,  INITIL  IS TO BE  1.
C
C     THIS ROUTINE IS CATALOGUED AS  EISPDRV4(CGREADI).
C
      DOUBLE PRECISION AR(NM,NM),AI(NM,NM),ARHOLD(NM,NM),AIHOLD(NM,NM)
      INTEGER  IAR( 20), IAI( 20)
      DATA IREADA/1/,IWRITE/6/
C
      IF( INITIL .EQ. 1 )  GO TO  30
      READ(IREADA,5) N
    5 FORMAT(I6)
      IF( N .EQ. 0 )  GO TO  70
      DO  15  I = 1,N
         READ(IREADA,10) (IAR(J),IAI(J),J=1,N)
   10    FORMAT(2I18)
         DO  15  J = 1,N
           AR(I,J) = IAR(J)
   15      AI(I,J) = IAI(J)
      DO  20  I = 1,N
         DO  20  J = 1,N
           ARHOLD(I,J) = AR(I,J)
   20      AIHOLD(I,J) = AI(I,J)
      RETURN
   30 DO  40  I = 1,N
         DO  40  J = 1,N
           AR(I,J) = ARHOLD(I,J)
   40      AI(I,J) = AIHOLD(I,J)
      RETURN
   70 WRITE(IWRITE,80)
   80 FORMAT(44H0END OF DATA FOR SUBROUTINE CMATIN(CGREADI).  /1H1)
      STOP
      END
      SUBROUTINE CGWZR(NM,N,AR,AI,WR,WI,ZR,ZI,NORM,RESDUL)
C
      DOUBLE PRECISION NORM(N),WR(N),WI(N),AR(NM,N),AI(NM,N),
     X       ZR(NM,N), ZI(NM,N), NORMA, XR, XI, S, SUMA, SUMZ,
     X       SUMR, SUMI, RESDUL, PYTHAG
C
C     THIS SUBROUTINE FORMS THE 1-NORM OF THE RESIDUAL MATRIX
C     A*Z-Z*DIAG(W)  WHERE  A  IS A COMPLEX GENERAL MATRIX,  Z  IS A
C     MATRIX WHOSE COLUMNS ARE APPROXIMATE EIGENVECTORS OF  A,
C     AND  W  IS A VECTOR WHOSE ELEMENTS ARE APPROXIMATE EIGENVALUES
C     CORRESPONDING TO THE VECTORS IN  Z.  ALL NORMS USED IN THE
C     COMMENTS BELOW ARE 1-NORMS.
C
C     THIS SUBROUTINE IS CATALOGUED AS EISPDRV4(CGWZR)
C
C     INPUT.
C
C        NM IS THE ROW DIMENSION OF TWO-DIMENSIONAL ARRAY PARAMETERS
C           AS DECLARED IN THE CALLING PROGRAM DIMENSION STATEMENT.
C
C        N IS THE ORDER OF THE MATRIX  A;
C
C        AR(NM,N), AI(NM,N) ARE ARRAYS CONTAINING THE REAL AND
C           IMAGINARY PARTS OF THE ELEMENTS OF  A;
C
C        ZR(NM,N), ZI(NM,N) ARE ARRAYS CONTAINING THE REAL AND
C           IMAGINARY PARTS OF THE ELEMENTS OF  Z;
C
C        WR(N), WI(N) ARE ARRAYS CONTAINING THE REAL AND IMAGINARY
C           PARTS OF  W.
C
C     OUTPUT.
C
C        ZR(NM,N), ZI(NM,N) ARE ARRAYS WHOSE COLUMNS CONTAIN THE
C           REAL AND IMAGINARY PARTS OF THE NORMALIZED APPROXIMATE
C           EIGENVECTORS OF  A.  THE EIGENVECTORS ARE NORMALIZED BY
C           THE 1-NORM IN SUCH A WAY THAT THE FIRST ELEMENT WHOSE
C           MAGNITUDE IS LARGER THAN THE NORM OF THE EIGENVECTOR
C           DIVIDED BY  N  IS REAL AND POSITIVE;
C
C        NORM(N) IS AN ARRAY SUCH THAT FOR EACH  K,
C           NORM(K) = !!A*Z(K)-Z(K)*(W(K))!!/(!!A!!*!!Z(K)!!)
C           WHERE  Z(K)  IS THE K-TH EIGENVECTOR;
C
C        RESDUL IS THE REAL NUMBER
C           !!A*Z-Z*DIAG(W)!!/(!!A!!*!!Z!!).
C
C     ----------------------------------------------------------------
C
      RESDUL = 0.0D0
      NORMA = 0.0D0
C
      DO 20 I=1,N
         SUMA = 0.0D0
C
         DO 10 L=1,N
   10       SUMA = SUMA + PYTHAG(AR(L,I),AI(L,I))
C
   20    NORMA = DMAX1(NORMA,SUMA)
C
      IF(NORMA .EQ. 0.0D0) NORMA = 1.0D0
C
      DO 80 I=1,N
         SUMZ = 0.0D0
         S = 0.0D0
C
         DO 40 L=1,N
            SUMZ = SUMZ + PYTHAG(ZR(L,I),ZI(L,I))
            SUMR = -WR(I)*ZR(L,I) + WI(I)*ZI(L,I)
            SUMI = -WR(I)*ZI(L,I) - WI(I)*ZR(L,I)
C
            DO 30 K=1,N
               SUMR = SUMR + AR(L,K)*ZR(K,I) - AI(L,K)*ZI(K,I)
   30          SUMI = SUMI + AR(L,K)*ZI(K,I) + AI(L,K)*ZR(K,I)
C
   40       S = S + PYTHAG(SUMR,SUMI)
C
         NORM(I) = SUMZ
C        ..........THIS LOOP WILL NEVER BE COMPLETED SINCE THERE
C                  WILL ALWAYS EXIST AN ELEMENT IN THE VECTOR Z(I)
C                  LARGER THAN !!Z(I)!!/N..........
         DO 50 L=1,N
            IF(PYTHAG(ZR(L,I),ZI(L,I)) .GE. NORM(I)/N) GO
     1      TO 60
   50        CONTINUE
C
   60    XR = NORM(I)*ZR(L,I)/PYTHAG(ZR(L,I),ZI(L,I))
         XI = NORM(I)*ZI(L,I)/PYTHAG(ZR(L,I),ZI(L,I))
C
         DO 70 L=1,N
            CALL CDIV(ZR(L,I),ZI(L,I),XR,XI,ZR(L,I),ZI(L,I))
   70    CONTINUE
C
         NORM(I) = S/(NORM(I)*NORMA)
         RESDUL = DMAX1(NORM(I),RESDUL)
   80    CONTINUE
C
      RETURN
      END
      SUBROUTINE CGW1ZR(NM,N,AR,AI,WR,WI,SELECT,M,ZR,ZI,NORM,RESDUL)
C
      DOUBLE PRECISION NORM(N),WR(N),WI(N),AR(NM,N),AI(NM,N),
     X       ZR(NM,M), ZI(NM,M), NORMA, XR, XI, S, SUMA, SUMZ,
     X       SUMR, SUMI, RESDUL, PYTHAG
      LOGICAL SELECT(N)
C
C     THIS SUBROUTINE FORMS THE 1-NORM OF THE RESIDUAL MATRIX
C     A*Z-Z*DIAG(W)   WHERE  A  IS A COMPLEX GENERAL MATRIX,  Z  IS A
C     MATRIX WHOSE COLUMNS ARE APPROXIMATE EIGENVECTORS OF  A,
C     AND  W  IS A VECTOR WHOSE ELEMENTS ARE APPROXIMATE EIGENVALUES
C     CORRESPONDING TO THE VECTORS IN  Z.  ALL NORMS USED IN THE
C     COMMENTS BELOW ARE 1-NORMS.
C
C     THIS SUBROUTINE IS CATALOGUED AS  EISPDRV4(CGW1ZR).
C
C     INPUT.
C
C        NM  IS THE ROW DIMENSION OF TWO-DIMENSIONAL ARRAY PARAMETERS
C           AS DECLARED IN THE CALLING PROGRAM DIMENSION STATEMENT;
C
C        N  IS THE ORDER OF THE MATRIX A;
C
C        AR(NM,N), AI(NM,N) ARE ARRAYS CONTAINING THE REAL AND
C           IMAGINARY PARTS OF THE ELEMENTS OF  A;
C
C        ZR(NM,M), ZI(NM,M) ARE ARRAYS CONTAINING THE REAL AND
C           IMAGINARY PARTS OF THE ELEMENTS OF  Z;
C
C        WR(N), WI(N) ARE ARRAYS CONTAINING THE REAL AND IMAGINARY
C           PARTS OF  W;
C
C        SELECT(N)  IS A BOOLEAN ARRAY WHICH SPECIFIES THE EIGENVALUES
C           IN THE VECTOR  W  THAT ARE ASSOCIATED WITH EIGENVECTORS
C           IN THE MATRIX  Z.  THE I-TH VALUE IN  W  IS ASSOCIATED WITH
C           A VECTOR IN  Z  IF SELECT(I) = .TRUE..  FURTHERMORE IF
C           SELECT(I) IS THE J-TH  .TRUE.  ELEMENT, THEN THE I-TH
C           ELEMENT OF  W  IS ASSOCIATED WITH THE J-TH VECTOR OF  Z;
C
C        M  IS THE NUMBER OF ELEMENTS IN SELECT WHOSE VALUE IS  .TRUE..
C
C     OUTPUT.
C
C        ZR(NM,M), ZI(NM,M) ARE ARRAYS WHOSE COLUMNS CONTAIN THE
C           REAL AND IMAGINARY PARTS OF THE NORMALIZED APPROXIMATE
C           EIGENVECTORS OF  A.  THE EIGENVECTORS ARE NORMALIZED BY
C           THE 1-NORM IN SUCH A WAY THAT THE FIRST ELEMENT WHOSE
C           MAGNITUDE IS LARGER THAN THE NORM OF THE EIGENVECTOR
C           DIVIDED BY  N  IS REAL AND POSITIVE;
C
C        NORM(N)  IS AN ARRAY SUCH THAT WHENEVER SELECT(K) =  .TRUE.
C           NORM(K) = !!A*Z(K)-Z(K)*W(K)!!/(!!A!!*!!Z(K)!!)
C           WHERE  Z(K)  IS THE EIGENVECTOR CORRESPONDING TO THE
C           K-TH EIGENVALUE (WR(K),WI(K));
C
C        RESDUL  IS A REAL NUMBER DEFINED BY THE FOLLOWING.
C           !!A*Z-Z*DIAG(W)!!/(!!A!!*!!Z!!).
C
C     ----------------------------------------------------------------
C
      RESDUL = 0.0D0
      IF( M .EQ. 0 ) RETURN
      NORMA = 0.0D0
      II=0
C
      DO 20 I=1,N
         SUMA = 0.0D0
C
         DO 10 L=1,N
   10       SUMA = SUMA + PYTHAG(AR(L,I),AI(L,I))
C
         NORMA = DMAX1(NORMA,SUMA)
         IF(NORMA .EQ. 0.0D0) NORMA = 1.0D0
   20    CONTINUE
C
      IF(NORMA .EQ. 0.0D0) NORMA = 1.0D0
C
      DO 80 I=1,N
         SUMZ = 0.0D0
         S = 0.0D0
         IF( .NOT. SELECT(I) ) GO TO 80
         II = II+1
C
         DO 40 L=1,N
            SUMZ = SUMZ + PYTHAG(ZR(L,II),ZI(L,II))
            SUMR = -WR(I)*ZR(L,II) + WI(I)*ZI(L,II)
            SUMI = -WR(I)*ZI(L,II) - WI(I)*ZR(L,II)
C
            DO 30 K=1,N
               SUMR = SUMR + AR(L,K)*ZR(K,II) - AI(L,K)*ZI(K,II)
   30          SUMI = SUMI + AR(L,K)*ZI(K,II) + AI(L,K)*ZR(K,II)
C
   40       S = S + PYTHAG(SUMR,SUMI)
C
         IF(SUMZ .NE. 0.0D0) GO TO 45
         NORM(I) = -1
         GO TO 80
   45    NORM(I) = SUMZ
C        ..........THIS LOOP WILL NEVER BE COMPLETED SINCE THERE
C                  WILL ALWAYS EXIST AN ELEMENT IN THE VECTOR  Z(II)
C                  LARGER THAN !!Z(II)!!/N..........
         DO 50 L=1,N
            IF(PYTHAG(ZR(L,II),ZI(L,II)) .GE. NORM(I)/N)
     1      GO TO 60
   50        CONTINUE
C
   60    XR = NORM(I)*ZR(L,II)/PYTHAG(ZR(L,II),ZI(L,II))
         XI = NORM(I)*ZI(L,II)/PYTHAG(ZR(L,II),ZI(L,II))
C
         DO 70 L=1,N
            CALL CDIV(ZR(L,II),ZI(L,II),XR,XI,ZR(L,II),ZI(L,II))
   70    CONTINUE
C
         NORM(I) = S/(NORM(I)*NORMA)
         RESDUL = DMAX1(NORM(I),RESDUL)
   80    CONTINUE
C
      RETURN
      END
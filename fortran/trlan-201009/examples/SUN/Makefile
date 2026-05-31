#$Id: Makefile,v 1.13 2002/11/26 01:15:12 kewu Exp $
.SUFFIXES: .f90 .f .c .o
include ../../Make.inc

#CC=pgcc
CC=cc
CFLAGS=-g#-O0
#CFLAGS=-O
#CFLAGS=-fast -xtarget=ultra2 -xcache=16/32/1:1024/64/1
#LDFLAGS=-L${HOME}/fortran/LAPACK -llapack -lblas
#LDFLAGS=-L${HOME}/lib -llapack -lblas
LDFLAGS=-L${HOME}/lib -llapack -lblas -lfsu -lf77compat -lsunmath
#LDFLAGS=-L${PGI}/linux86/lib -llapack -lblas -lpgftnrtl

TRLAN=../../libtrlan.a

ALLSRC=../../SRC/dsort2.f ../../SRC/dstqrb.f ../../SRC/trlcore.f90 \
	../../SRC/trlan.f90 ../../SRC/restart.f90 \
	../../SRC/trlaux.f90 ../../SRC/trl_comm_none.f90

dense: dense.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ dense.o ${TRLAN} ${LDFLAGS}

shbeig: shbeig.o shbaux.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ shbeig.o shbaux.o ${TRLAN} ${LDFLAGS}

shbsvd: shbsvd.o shbaux.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ shbsvd.o shbaux.o ${TRLAN} ${LDFLAGS}

simple: simple.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ simple.o ${TRLAN} ${LDFLAGS}

simple77: simple77.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ simple77.o ${TRLAN} ${LDFLAGS}

simplec: simplec.o ${TRLAN}
	${FC} ${FFLAGS} -v -o $@ simplec.o ${TRLAN} ${LDFLAGS}
#	$(CC) $(CFLAGS) -o $@ simplec.o ${TRLAN} ${LDFLAGS} -lpgf90 -lpgf90_rpm1 -lpgf902 -lpgf90rtl -lm

check90: ${ALLSRC} simple.f90 trl_info.f90
	${FC} ${FFLAGS} -Xlist trl_info.f90 simple.f90 ${ALLSRC} \
	-o simple ${LDFLAGS}

check77:${ALLSRC} simple77.f trl_info.f90 ../SRC/trlan77.f90
	${FC} ${FFLAGS} -Xlist simple77.f trl_info.f90 ../SRC/trlan77.f90 \
	${ALLSRC} -o simple77 ${LDFLAGS}

${TRLAN}: ${ALLSRC} ../../SRC/trl_info.f90
	cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan.a

clean:
	/bin/rm -f *.o *.M *.T *.lst core shb??? dense simple simple77 simplec trl_info.f90 SIMP* TRL_* *.mod V*.inc *.vo

shbeig.o shbsvd.o simple.o dense.o: trl_info.o
trl_info.f90:
	ln -s ../../SRC/trl_info.f90 .

.c.o:
	$(CC) ${CFLAGS} -c $<
.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${F90} ${FFLAGS} -c $<

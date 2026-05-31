#$Id: Makefile,v 1.1.1.1 1998/08/17 19:03:44 kewu Exp $
.SUFFIXES: .f90 .f .o

FC=f90
CFLAGS=-O0 -g
FFLAGS=${CFLAGS} -dp -ei -Rabcs -Ca
LDFLAGS=-Wl"segldr.cld"

TRLAN=lapack_aux.o ../../libtrlan.a
ALLSRC=../../SRC/dsort2.f ../../SRC/dstqrb.f ../../SRC/trlcore.f90 ../../SRC/trlan.f90 \
       ../../SRC/restart.f90 ../../SRC/trlaux.f90 ../../SRC/trl_comm_none.f90

simple: simple.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simple.o ${TRLAN} ${LDFLAGS}

simple77: simple77.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simple77.o ${TRLAN} ${LDFLAGS}

simplec: simplec.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simplec.o ${TRLAN} ${LDFLAGS}

../../libtrlan.a:
	cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan.a

check90: ${ALLSRC} simple.f90 trl_info.f90
	cflint -s508 simple.f90 trl_info.f90 ${ALLSRC}
#	NPROC=1; f90 ${FFLAGS} -Ca -c trl_info.f90 simple.f90 ${ALLSRC}

clean:
	/bin/rm -f *.o *.T *.M *.lst core SIMP* TRL_* simple*

simple.o: trl_info.o

simplec.o: simplec.c
	cc ${CFLAGS} -c simplec.c

.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${FC} ${FFLAGS} -c $<
simple.f90:
	ln -s ../SUN/simple.f90 .
simple77.f:
	ln -s ../SUN/simple77.f .
simplec.c:
	ln -s ../SUN/simplec..c

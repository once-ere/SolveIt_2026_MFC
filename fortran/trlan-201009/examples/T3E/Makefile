.SUFFIXES: .f90 .f .c .o
include ../../Make.inc
#CFLAGS=-O
CFLAGS=-O0 -g
LDFLAGS=-Wl"cray.cld"

TRLAN=lapack_aux.o ../../libtrlan_mpi.a

ALLSRC=../../SRC/dsort2.f ../../SRC/dstqrb.f ../../SRC/trlcore.f90 ../../SRC/trlan.f90 \
       ../../SRC/restart.f90 ../../SRC/trlaux.f90 ../../SRC/trl_comm_mpi.f90

s1: s1.o ${TRLAN}
	f90 ${FFLAGS} ${LDFLAGS} -o $@ s1.o ${TRLAN}

simple: simple.o ${TRLAN}
	f90 ${FFLAGS} ${LDFLAGS} -o $@ simple.o ${TRLAN}

simple77: simple77.o ${TRLAN}
	f90 ${FFLAGS} ${LDFLAGS} -o $@ simple77.o ${TRLAN}

simplec: simplec.o ${TRLAN}
	f90 ${FFLAGS} ${LDFLAGS} -o $@ simplec.o ${TRLAN}

check77:
	cflint simple77.f ../../SRC/trlan77.f90 ${ALLSRC}
check90:
	cflint -s25 -s42 -s507 -s508 -s 517-518 simple.f90 ${ALLSRC}
#	cflint simple.f90 ${ALLSRC}

../../libtrlan_mpi.a: ${ALLSRC} ../../SRC/trl_info.f90
	cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan_mpi.a

clean:
	/bin/rm -f *.o *.T *.lst core simple simplec simple77 SIMP* \
	trl_info.f90 s1 TRL_*

s1.o simple.o: trl_info.o
trl_info.f90:
	ln -s ../../SRC/trl_info.f90 .

.c.o:
	cc ${CFLAGS} -c $<
.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${FC} ${FFLAGS} -c $<

#$Id: Makefile,v 1.3 1999/04/21 18:00:33 kewu Exp $
.SUFFIXES: .f90 .f .c .o
include ../../Make.inc
# flags for debugging
CFLAGS=-64 -mips4 -O0 -g
#FFLAGS=-64 -mips4 -O0 -g
LDFLAGS=-L/usr/lib64 -lcomplib.sgimath
# flags for MIP4 based single processor machines
#CFLAGS=-64 -mips4 -Ofast
#FFLAGS=-64 -mips4 -Ofast -OPT:alias=restrict -OPT:Olimit=0
#LDFLAGS=-L/usr/lib64 -lcomplib.sgimath
# flags for SGI SMP machines
#CFLAGS=-64 -mips4 -Ofast -pfa
#CFLAGS=-g -O0 -64 -mips4 -OPT:alias=restrict
#FFLAGS=-64 -mips4 -Ofast -pfa -OPT:alias=restrict -OPT:Olimit=0
#LDFLAGS=-L/usr/lib64 -lcomplib.sgimath_mp

TRLAN=../../libtrlan.a

simple: simple.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simple.o ${TRLAN} ${LDFLAGS}

simple77: simple77.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simple77.o ${TRLAN} ${LDFLAGS}

simplec: simplec.o ${TRLAN}
	f90 ${FFLAGS} -o $@ simplec.o ${TRLAN} ${LDFLAGS}

../../libtrlan.a:
	cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan.a

clean:
	/bin/rm -rf *.o *.kmo core SIMP* TRL_* simple* trl_info.* rii_files

simple.o: trl_info.o

.c.o:
	cc ${CFLAGS} -c $<
.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${FC} ${FFLAGS} -c $<
simple.f90:
	ln -s ../SUN/simple.f90 .
simple77.f:
	ln -s ../SUN/simple77.f .
simplec.c:
	ln -s ../SUN/simplec.c .
trl_info.f90:
	ln -s ../../SRC/trl_info.f90 .


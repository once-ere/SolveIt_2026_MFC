.SUFFIXES: .f90 .f .c .o

FC=xlf90
CFLAGS=-g -C
FFLAGS=-g -C
LDFLAGS=-L/soft/local/lib -llapack -lblas

TRLAN=../../libtrlan.a

simple: simple.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ simple.o ${TRLAN} ${LDFLAGS}

simple77: simple77.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ simple77.o ${TRLAN} ${LDFLAGS}

simplec: simplec.o ${TRLAN}
	${FC} ${FFLAGS} -o $@ simplec.o ${TRLAN} ${LDFLAGS}

../../libtrlan.a:
	cd ../../SRC; ${MAKE} FC="${FC}" OPT="${OPT}" libtrlan.a

clean:
	/bin/rm -f *.o *.kmo core SIMP* TRL_* simple* trl_info.f90

simple.o: trl_info.o simple.f90
	${FC} ${FFLAGS} -c simple.f90
simple77.o: simple77.f
	xlf ${FFLAGS} -c simple77.f
trl_info.o: trl_info.f90
	${FC} ${FFLAGS} -c trl_info.f90
.c.o:
	cc ${CFLAGS} -c $<
simple.f90:
	ln -s ../SUN/simple.f90 .
simple77.f:
	ln -s ../SUN/simple77.f .
simplec.c:
	ln -s ../SUN/simplec..c
trl_info.f90:
	ln -s ../../SRC/trl_info.f90 .


.SUFFIXES: .f90 .f .c .o
include ../Make.inc
#LDFLAGS=-L${HOME}/lib -lskit -llapack -lblas
#LDFLAGS=-L${HOME}/lib -lskit -L/usr/lib64 -lcomplib.sgimath -lfastm
#LDFLAGS=-L/soft/local/lib -llapack -lblas
LDFLAGS=-L${HOME}/lib -lskit -Wl"../dym/cray.cld"

TRLAN=getarg.o lapack_aux.o ../libtrlan.a

ttrl: ttrl.o ${TRLAN}
	$(FC) ${FFLAGS} -o $@ ttrl.o ${TRLAN} ${LDFLAGS}

hbt: hbtrl.o ${TRLAN}
	$(FC) ${FFLAGS} -o $@ hbtrl.o ${TRLAN} ${LDFLAGS}

hbl: hblanso.o
	${FC} ${FFLAGS} -o $@ hblanso.o ../../PLAN/liblanso.a ${LDFLAGS}

hbn: hblan.o
	${FC} ${FFLAGS} -o $@ hblan.o ../../PLAN/liblanso.a ${LDFLAGS}

arp: arphb.o
	$(FC) ${FFLAGS} -o $@ arphb.o -L${HOME}/lib -larpack -lskit ${LDFLAGS}

cgs77: cgs.f
	$(FC) ${FFLAGS} -o cgs77 cgs.f ${LDFLAGS}
cgs90: cgs.f90
	$(FC) ${FFLAGS} -o cgs90 cgs.f90 ${LDFLAGS}
cgs91: cgs1.f90
	$(FC) ${FFLAGS} -o cgs91 cgs1.f90 ${LDFLAGS}
cgs92: cgs2.f90
	$(FC) ${FFLAGS} -o cgs92 cgs2.f90 ${LDFLAGS}
top: top.f90
	$(FC) ${FFLAGS} -o top top.f90 ${LDFLAGS}

getarg.o: getarg.F
	$(FC) ${FFLAGS} -c -DCRAY $<

lapack_aux.o: lapack_aux.f
lapack_aux.f: ../dym/lapack_aux.f
	ln -s ../dym/lapack_aux.f .

../libtrlan.a: ../SRC/trlan.f90 ../SRC/trlcore.f90 ../SRC/trlaux.f90 ../SRC/restart.f90 ../SRC/trl_info.f90
	cd ../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan.a

clean:
	/bin/rm -f *.o *.T *.M *.kmo *.mod core trl_info.f90 TRL_*
clean-all: clean
	/bin/rm -f hbt arp hbl hbn cgs77 cgs9? top ttrl
	cd ..; make clean

ttrl.o hbtrl.o: trl_info.o

.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${FC} ${FFLAGS} -c $<

trl_info.f90:
	ln -s ../SRC/trl_info.f90 .

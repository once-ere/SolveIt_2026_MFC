.SUFFIXES: .f90 .f .c .o
include ../Make.inc
#FC=f90
CFLAGS=-O3
#FFLAGS=${CFLAGS} -dp -Rabcs -g
LDFLAGS=-Wl"cray.cld"

TRLAN=lapack_aux.o ../libtrlan_mpi.a
SRC=../SRC/dsort2.f ../SRC/dstqrb.f ../SRC/restart.f90 \
	../SRC/trl_comm_mpi.f90 ../SRC/trl_info.f90 ../SRC/trlan.f90 \
	../SRC/trlan77.f90 ../SRC/trlaux.f90 ../SRC/trlcore.f90

ttrl: ttrl.o ${TRLAN}
	$(FC) ${FFLAGS} -o $@ ttrl.o ${TRLAN} ${LDFLAGS}
lint:
	cflint -s1,4,12,42 ttrl.f90 ${SRC}

../libtrlan_mpi.a:
	cd ../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan_mpi.a

clean:
	/bin/rm -f *.o *.T core

ttrl.o: trl_info.o

trl_info.o:
	cd ../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" trl_info.o
	ln -s ../SRC/trl_info.o .

.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${FC} ${FFLAGS} -c $<

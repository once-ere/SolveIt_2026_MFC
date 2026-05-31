.SUFFIXES: .f90 .f .F .o .a
#
#FC=mpif90
#FFLAGS=-g
FC=f90
FFLAGS=-dp -G1 -O1 -ei -Rabcs#-Ca
#FFLAGS=-dp -g -O0 -ei -Rabcs
#FFLAGS=-dp -O3,aggress
LDFLAGS=-Wl"cray.cld"
#
eigconn: eigconn.o readconn.o pspmatmult.o getarg.o ../../libtrlan_mpi.a
	${FC} ${FFLAGS} -o $@ eigconn.o readconn.o pspmatmult.o getarg.o ../../libtrlan_mpi.a ${LDFLAGS}
#
clean:
	rm -f *.o *.mod *.T *.kmo trl_info.f90
#
../../libtrlan_mpi.a::
	-cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" plib
getarg.o: getarg.F
	${FC} ${FFLAGS} -c getarg.F
trl_info.f90: ../../SRC/trl_info.f90
	ln -s ../../SRC/trl_info.f90 .
#
eigconn.o: pspmatmult.o trl_info.o
#
.f90.o:
	${FC} ${FFLAGS} -c $<

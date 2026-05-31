#$Id: Makefile,v 1.3 2002/10/02 18:15:25 kewu Exp $
#
# This is the make file in the director of TRLAN/SRC
# It assumes the archive command to build .a file is ar
# and the Fortran 90 compiler can also compile Fortran 77 files
#
.SUFFIXES: .f90 .f .o
%.o : %.mod

include ../Make.inc

TRL=trlcore.o trlan.o restart.o trlaux.o trlan77.o
OBJ=dsort2.o trl_info.o dstqrb.o ${TRL}

lib libtrlan.a: ${OBJ} trl_comm_none.o
	ar -cru ../libtrlan.a ${OBJ} trl_comm_none.o

plib libtrlan_mpi.a: ${OBJ} trl_comm_mpi.o
	ar -cru ../libtrlan_mpi.a ${OBJ} trl_comm_mpi.o

clean:
	/bin/rm -f *.o *.T *.M *.mod *.l* *.L V*.inc V*.f *.vo

trl_comm_mpi.o trl_comm_none.o ${TRL}: trl_info.o

.f.o:
	${FC} ${FFLAGS} -c $<
.f90.o:
	${F90} ${FFLAGS} -c $<

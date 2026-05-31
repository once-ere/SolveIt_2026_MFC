# this Makefile is only tested on Cray T3E
# You will need to locate appropriate compiler and libraries in order
# to run it on a different machine
#
FC=f90
MPIFC=f90
#FFLAGS=-dp -O3,aggress,inline2,pipeline2,unroll2,fusion -e0
FFLAGS=-dp -O3,aggress -e0
NOOPT=-dp -O1 -e0
#FFLAGS=-g -dp -eiI -Rnbc -Ca
#NOOPT=-g -dp -eiI -Rnbc -Ca
LDFLAGS=-Wl"cray.cld"
#
LIBS=-L${HOME}/lib -lpspars -lskit
SHBPSP=shbpsp.o hbread3.o psp_setup.o dmem.o getarg.o
DYNSRC=psp_setup.o shbpsp.o hbread3.o
#
# Cray's libsci is missing some lower level eigenvalue routines
# comment it out if you have a complete set of LAPACK
LAPACK=lapack_aux.o
#
.DEFAULT:
	@$(ECHO) 'Unknown target $@.'
	@$(ECHO) 'The following targets are defined in this Makefile.'
	@$(ECHO) 'shbpsp clean'
#
shbpsp: $(SHBPSP) ${LAPACK} ../../libtrlan_mpi.a cray.cld
	$(MPIFC) -o $@ $(SHBPSP) ${LAPACK} ../../libtrlan_mpi.a $(LIBS) $(LDFLAGS)
#
rsarua: rsarua.o dmem.o
	$(FC) $(FFLAGS) -o $@ rsarua.o dmem.o ${LIBS}
#
clean:
	rm -f *.o *trace core *~ shbpsp  rsarua *.T *.lst TRL_* lapack_aux.f cray.cld
#
# compile all files that uses dynamic memory allocation with
# lower optimization level
#
$(DYNSRC):
	$(MPIFC) $(NOOPT) -c $<
.f.o:
	$(MPIFC) $(FFLAGS) -c $<
lapack_aux.f:
	ln -s ../T3E/lapack_aux.f .
cray.cld:
	ln -s ../T3E/cray.cld .
#
shbpsp.o: shbpsp.h
#
../../libtrlan_mpi.a::
	cd ../../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" libtrlan_mpi.a
#
# special target dmem.o, getarg.o and getarg.o
#
# on a CRAY machine
dmem.o: dmem.F
	$(FC) $(NOOPT) -DCRAY -c dmem.F
getarg.o: getarg.F
	$(FC) $(FFLAGS) -DCRAY -c getarg.F
#
# on Most workstations
#
#dmem.o: dmem.F
#	$(FC) $(NOOPT) -DSUN -c dmem.F
#
# In some special cases where there is no fortran interface to dynamic
# memory allocation routine malloc, the following line is for SGI r10000
#
#dmem.o: dmem.c
#	cc $(NOOPT) -D_SGI -c dmem.c
#
#

#$Id: Makefile,v 1.1.1.1 1998/08/17 19:03:44 kewu Exp $
#
.SUFFIXES: .f90 .f .o
%.o : %.mod
# the fortran compiler name and the flags are defined in Make.inc
include Make.inc
# building library
lib:
	(cd SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" lib)
plib:
	(cd SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" plib)
clean:
	for d in SRC examples; do (cd $$d; ${MAKE} clean;); done
	/bin/rm -f *.a
#

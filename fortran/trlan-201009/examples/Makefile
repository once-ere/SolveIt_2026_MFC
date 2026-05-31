#$Id: Makefile,v 1.2 2001/02/21 21:39:43 kewu Exp $
# the compiler name and compiling flags are defined in Make.inc
include ../Make.inc
# building library
lib:
	(cd ../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" lib)
plib:
	(cd ../SRC; ${MAKE} FC="${FC}" FFLAGS="${FFLAGS}" plib)
clean:
	for d in SUN C90 T3E SGI RS6K SHARP; do (cd $$d; ${MAKE} clean;); done
#

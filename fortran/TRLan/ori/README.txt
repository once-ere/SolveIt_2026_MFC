This directory contains all source code of the TRLAN package.

Makefile          -- the makefile, a template for compiling the souce code
dsort2.f          -- a simple sorting routine
dstqrb.f          -- a program copied from ARPACK to compute the
                     eigenvalues and the last row of the eigenvectors of
		     a tridiagonal matrix
restart.f90       -- a file contain all restarting procedures
trl_comm_mpi.f90  -- all routines that need to invoke MPI to perform
                     data communication
trl_comm_none.f90 -- the same procedures as in trl_comm_mpi.f90 without
                     actually calling MPI (it is used to be used on
		     sequential machines.)
trl_info.f90      -- this file contains two modules one defines the
                     TRL_INFO Fortran 90 derived type and the public
		     interfaces of the pacakge (trl_interface)
trlan.f90         -- all subroutines with public interface are in this
                     file
trlan77.f90       -- the Fortran 77 wrapper for TRLAN (can be called
                     from C)
trlaux.f90        -- some I/O functions and printing functions used in
		     TRLAN
trlcore.f90       -- the bulk of the code that performs arithmetic
		     operations

All compiler flags are defined in ../Make.inc
To generate MPI version of the library file, type

make plib
or
make libtrlan_mpi.a

To generate a sequential version of the library, type

make lib
or
make libtran.a

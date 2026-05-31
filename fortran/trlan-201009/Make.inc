#$Id: Make.inc,v 1.12 2010/09/17 18:10:04 kewu Exp $
# This file deines the Fortran compiler name and the defaul options
#
########
# The generic (unspecified) architecture
#FC=f90
#F90=f90
#FFLAGS=-g -Mbounds -Mchkfpstk -Mdclchk -Minform,inform
#FFLAGS=-O

########
# MPICH on linux
##FC=pgf90
#FC=mpif90
#F90=mpif90
#FFLAGS=-O3
#FFLAGS=-g -O0

########
# SUN/SPARC workstations
#FC=f90
#F90=f90
#FFLAGS=-O
#FFLAGS=-g -ansi -Xlist
#FFLAGS=-fast -xtarget=ultra2 -xcache=16/32/1:1024/64/1

########
# Cray C90/J90/T3E flags
#FC=f90
#F90=f90
#FFLAGS=-g -O0 -dp -eiI -Rnbc -Ca
#FFLAGS=-O3 -dp

########
# SGI/IRIX flags
#FC=f90
#F90=f90
#FFLAGS_SGI=-64 -O3
#FFLAGS=-64 -g -O0 -static -WK,-o=0,-so=0,-r=0 -listing -LIST:all_options=ON:notes=ON:symbols=ON
#FFLAGS=-g -O0 -64 -mips4 -OPT:alias=restrict

########
# IBM RS 6000 and others that use XLF 
#FC=xlf
#F90=xlf90
#FFLAGS=-O3
#FFLAGS=-g -C -qinit=f90ptr -qinitauto

########
# IBM SP2 and others that use MPXLF 
#FC=mpxlf
#F90=mpxlf90 -qsuffix=f=f90 # -qlanglvl=90pure
#FFLAGS=-O3 -qstrict -qmaxmem=-1# -qxref -qextchk -bnoquiet

########
# Portland Group fortran compilers
#FC=pgf77
#F90=pgf90
#FFLAGS=-O

########
# gfortran compiler from GNU
FC=gfortran
F90=gfortran
FFLAGS=-O0 -g -Wall -fbounds-check


########
# INTEL ifort
#FC=ifort
#F90=ifort
#FFLAGS=-O -C

#
SHELL=/bin/sh
#

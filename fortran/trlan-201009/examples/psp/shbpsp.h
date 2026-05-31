c
c     This head file is used to define the common block for storing
c     the matrix information required to perform matrix-vector
c     multiplications with PSPARSLIB amxdis.
c
c     The following is the header block of the matrix-vector
c     multiplication routine
c$$$  subroutine amxdis(nloc,nbnd,x,y,aloc,jaloc,ialoc,nproc,proc,ix,
c$$$ *     ipr,type,xlen,ptrn)
c$$$  implicit none
c$$$  integer nloc,nbnd,nproc,type,xlen,ptrn,myproc
c$$$  integer jaloc(*),ix(*),ialoc(*),ipr(nproc+1),proc(2*nproc)
c$$$  real*8 aloc(*),x(xlen),y(*)
c
      integer MAX_PROCS
      parameter (MAX_PROCS=512)
c
      integer nbnd, nproc, amxtype, ptrn, myproc, totprocs
      integer proc(4*MAX_PROCS+7), IPR(MAX_PROCS+1)
      integer jaloc(*), ialoc(*), ix(*)
      real*8 aloc(*), bloc(*), binv(*)
      pointer (paloc, aloc), (pjaloc, jaloc), (pialoc, ialoc),
     &     (pix, ix), (pbloc, bloc), (pbinv, binv)
c
      common /pspmat/paloc, pjaloc, pialoc, pix, proc, ipr, nbnd, nproc,
     $     amxtype, ptrn, myproc, totprocs, pbloc, pbinv

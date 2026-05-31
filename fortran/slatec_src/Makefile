#cd C:/Users/nsh/dev/lib/slatec_src
#
#I saw similar errors when building a version of slatec.dll from the f77
#source, and solved it by pulling the routines from slatec_chk.
#make lib
#
#nm -gD yourLib.so
#nm -gD libslatec.a
#
#dllwrap --export-all-symbols ./obj/*.o -lgfortran --output-def slatec_win32.def -o slatec_win32.dll
#dllwrap --export-all-symbols ./obj/*.o -lopenblas -llapack -lgfortran --output-def slatec_win32.def -o slatec_win32.dll
#
#
#dllwrap --export-all-symbols ./obj/*.o -larpack -lopenblas -llapack -lgfortran --output-def slatec_win32.def -o slatec_win32.dll
#
#C:\msys64\mingw32\bin\dllwrap.exe: no export definition file provided.
#Creating one, but that may not be what you want
#
#
#
#C:\Users\nsh>"C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2019\Visual Studio Tools\VC\x86 Native Tools Command Prompt for VS 2019 (3).lnk"
#
#cd C:\msys64\home\nsh\trlan-201009
#
#lib /machine:i386 /def:slatec_win32.def
#
#dumpbin.exe /ALL libslatec.a >> libslatec.txt
#
#C:\msys64\home\nsh\trlan-201009>dumpbin.exe /ALL slatec_win32.lib >> trlan77_win32.txt


#------------------------------------------------------------------------------#
# cat -vT -vE Makefile
#cat -e -t -v Makefile
#It shows the presence of tabs with ^I and line endings with $ both are vital to ensure that
#dependencies end properly and tabs mark the action for the rules so that they are easily identifiable to #the make utility 
#------------------------------------------------------------------------------#
# clang++ -static-libgcc -static-stdc++ -std=c++14 test.cpp -o test


ifeq ($(OS),Windows_NT)
	CCFLAGS += -D WIN32
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		CCFLAGS += -D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			CCFLAGS += -D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			CCFLAGS += -D IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
			LIBRARIES	:= -lmpfr  -lgmpxx -lgmp -lstdc++fs -lstdc++ -lm -lpthread
	    CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		LIBS = -framework Metal -framework MetalKit -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore
		LIBS += -L/usr/local/lib -lglfw
			LIBRARIES	:= -framework OpenGl -lpng -lstdc++ -lm -lpthread
# -framework GLUT			
	    CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
	    CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
	    CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
	    CCFLAGS += -D ARM
	endif
endif



WRKDIR = `pwd`


CC = clang
CXX = clang++
F77 = gfortran
F9X = gfortran
FFLAGS= -fdec -O0 -g -w -Wno-error -fbounds-check -fno-range-check
#LD = clang++ -static-libgcc -static-stdc++ -std=c++2a
#LD = clang++ -static -std=c++2a
LD = clang++ -std=c++2a
AR = ar
RANLIB = ranlib
WINDRES = windres

INC = -I"./imgui"
CFLAGS = -std=c++2a -Wall
RESINC = 
RCFLAGS = 
LDFLAG = -L"/usr/lib/x86_64-linux-gnu"
LDFLAGS = -L"/usr/lib/x86_64-linux-gnu" -L/usr/local/boost_1_72_0/lib
LIBDIR = -L"/usr/lib/x86_64-linux-gnu"
LIBc = -lglfw -lGLEW -lGLU -lGL  /usr/local/lib/libsoundio.a -ljack -lpulse -lasound -lportaudio -lrt -lfreeimage -lm -lpthread -lstdc++
#LIB = -lglut -lGLU -lGL -lm -lpthread -lsoundio#-latomic
#LIB2 = /usr/local/lib/libsoundio.a -lfreeimage -ljack -lpulse -lasound -lportaudio -lrt 
#"/usr/local/lib/libsoundio.a /usr/local/lib/libsoundio.so",
#"/usr/lib/x86_64-linux-gnu/libfreeimage.so",
#"/usr/lib/x86_64-linux-gnu/libportaudio.so.2",
#"/usr/lib/x86_64-linux-gnu/libjack.a /usr/lib/x86_64-linux-gnu/libjack.so",
#"/usr/lib/x86_64-linux-gnu/libpulse.so",
#"/usr/lib/x86_64-linux-gnu/libsoundio.a /usr/lib/x86_64-linux-gnu/libasound.so",
#"/usr/lib/x86_64-linux-gnu/librt.a /usr/lib/x86_64-linux-gnu/librt.so",


SOUNDIO_INCLUDE_DIR =-I"/usr/local/include"

SRC	:= src
INC_RELEASE = ${INC}
CFLAGS_RELEASE = ${CFLAGS} -O2
RESINC_RELEASE = ${RESINC}
RCFLAGS_RELEASE = ${RCFLAGS}
LIBDIR_RELEASE = ${LIBDIR}
LIB_RELEASE = ${LIB} #${LIB2}
LDFLAGS_RELEASE = ${LDFLAGS}
LFLAGS		:= "-L$(LDFLAG)"
ODIR = obj
binDIR_RELEASE = lib
BIN		:= bin
EXECUTABLE	:= libslatec.a
DEP_RELEASE = 
EARTH2048BMP = earth2048.bmp
OUT_RELEASE = ${binDIR_RELEASE}/libslatec.a

REMOVE=bvder.f dbvder.f drkfab.f dbvpor.f dexbvp.f rkfab.f bvpor.f bvsup.f dbvsup.f exbvp.f
#	common	sources
#	common	sources APPROX.f hyperelliptic.f
_COMMON_SRC	:=  aaaaaa.f  cdiv.f    cpoir.f   dbndsl.f  dgeco.f   dqage.f   dsvdc.f   hwscsp.f  pchid.f   rc.f      snls1.f   strmm.f \
acosh.f   cdntl.f   cposl.f   dbnfac.f  dgedi.f   dqagi.f   dswap.f   hwscyl.f  pchim.f   rc3jj.f   snls1e.f  strmv.f \
ai.f      cdntp.f   cppco.f   dbnslv.f  dgefa.f   dqagie.f  dsymm.f   hwsplr.f  pchkt.f   rc3jm.f   snrm2.f   strsl.f \
aie.f     cdotc.f   cppdi.f   dbocls.f  dgefs.f   dqagp.f   dsymv.f   hwsss1.f  pchngs.f  rc6j.f    snsq.f    strsm.f \
albeta.f  cdotu.f   cppfa.f   dbols.f   dgemm.f   dqagpe.f  dsyr.f    hwsssp.f  pchsp.f   rd.f      snsqe.f   strsv.f \
algams.f  cdpsc.f   cppsl.f   dbolsm.f  dgemv.f   dqags.f   dsyr2.f   i1mach.f  pchst.f   rebak.f   sods.f    stway.f \
ali.f     cdpst.f   cpqr79.f  dbsgq8.f  dger.f    dqagse.f  dsyr2k.f  i1merg.f  pchsw.f   rebakb.f  somn.f    suds.f \
alngam.f  cdriv1.f  cproc.f   dbsi0e.f  dgesl.f   dqawc.f   dsyrk.f   icamax.f  pcoef.f   reduc.f   sopenm.f  svco.f \
alnrel.f  cdriv2.f  cprocp.f  dbsi1e.f  dglss.f   dqawce.f  dtbmv.f   icopy.f   pfqad.f   reduc2.f  sorth.f   svd.f \
asinh.f   cdriv3.f  cprod.f   dbsk0e.f  dgmres.f  dqawf.f   dtbsv.f   idamax.f  pgsf.f    reort.f   sos.f     svecs.f \
asyik.f   cdscl.f   cprodp.f  dbsk1e.f  dgtsl.f   dqawfe.f  dtin.f    idloc.f   pimach.f  rf.f      soseqs.f  svout.f \
asyjy.f   cdstp.f   cpsi.f    dbskes.f  dh12.f    dqawo.f   dtout.f   imtql1.f  pinitm.f  rfftb.f   sossol.f  swritp.f \
atanh.f   cdzro.f   cptsl.f   dbskin.f  dhels.f   dqawoe.f  dtpmv.f   imtql2.f  pjac.f    rfftb1.f  spbco.f   sxlcal.f \
avint.f   cexprl.f  cpzero.f  dbsknu.f  dheqr.f   dqaws.f   dtpsv.f   imtqlv.f  pnnzrs.f  rfftf.f   spbdi.f   tevlc.f \
bakvec.f  cfftb.f   cqrdc.f   dbspdr.f  dhfti.f   dqawse.f  dtrco.f   indxa.f   poch.f    rfftf1.f  spbfa.f   tevls.f \
balanc.f  cfftb1.f  cqrsl.f   dbspev.f  dhkseq.f  dqc25c.f  dtrdi.f   indxb.f   poch1.f   rffti.f   spbsl.f   tinvit.f \
balbak.f  cfftf.f   crati.f   dbsppp.f  dhstrt.f  dqc25f.f  dtrmm.f   indxc.f   pois3d.f  rffti1.f  speli4.f  tql1.f \
bandr.f   cfftf1.f  crotg.f   dbspvd.f  dhvnrm.f  dqc25s.f  dtrmv.f   initds.f  poisd2.f  rg.f      spelip.f  tql2.f \
bandv.f   cffti.f   cs1s2.f   dbspvn.f  dintp.f   dqcheb.f  dtrsl.f   inits.f   poisn2.f  rgauss.f  spenc.f   tqlrat.f \
bcrh.f    cffti1.f  cscal.f   dbsqad.f  dintrv.f  dqdota.f  dtrsm.f   intrv.f   poisp2.f  rgg.f     spigmr.f  trbak1.f \
bdiff.f   cfod.f    cscale.f  dbsynu.f  dintyd.f  dqdoti.f  dtrsv.f   intyd.f   poistg.f  rj.f      spincw.f  trbak3.f \
besi.f    cg.f      cseri.f   dbvalu.f  dir.f     dqelg.f   du11ls.f  invit.f   polcof.f            spinit.f  tred1.f \
besi0.f   cgamma.f  csevl.f             djairy.f  dqform.f  du11us.f  inxca.f   polfit.f  rpqr79.f  splp.f    tred2.f \
besi0e.f  cgamr.f   cshch.f             dlbeta.f  dqk15.f   du12ls.f  inxcb.f   polint.f  rpzero.f  splpce.f  tred3.f \
besi1.f   cgbco.f   csico.f             dlgams.f  dqk15i.f  du12us.f  inxcc.f   polyvl.f  rs.f      splpdm.f  tri3.f \
besi1e.f  cgbdi.f   csidi.f   dcbrt.f   dli.f     dqk15w.f  dulsia.f  iploc.f   pos3d1.f  rsb.f     splpfe.f  tridib.f \
besj.f    cgbfa.f   csifa.f   dcdot.f   dllsia.f  dqk21.f   dusrmt.f  ipperm.f  postg2.f  rsco.f    splpfl.f  tridq.f \
besj0.f   cgbmv.f   csinh.f   dcfod.f   dllti2.f  dqk31.f   dvecs.f   ipsort.f  ppadd.f   rsg.f     splpmn.f  tris4.f \
besj1.f   cgbsl.f   csisl.f   dcg.f     dlngam.f  dqk41.f   dvnrms.f  isamax.f  ppgq8.f   rsgab.f   splpmu.f  trisp.f \
besk.f    cgeco.f   cspco.f   dcgn.f    dlnrel.f  dqk51.f   dvout.f   isdbcg.f  ppgsf.f   rsgba.f   splpup.f  trix.f \
besk0.f   cgedi.f   cspdi.f   dcgs.f    dlpdoc.f  dqk61.f   dwnlit.f  isdcg.f   pppsf.f   rsp.f     spoco.f   tsturm.f \
besk0e.f  cgeev.f   cspfa.f   dchdc.f   dlpdp.f   dqmomo.f  dwnlsm.f  isdcgn.f  ppqad.f   rst.f     spodi.f   u11ls.f \
besk1.f   cgefa.f   cspsl.f   dchdd.f   dlsei.f   dqnc79.f  dwnlt1.f  isdcgs.f  ppsgf.f   rt.f      spofa.f   u11us.f \
besk1e.f  cgefs.f   csroot.f  dchex.f   dlsi.f    dqng.f    dwnlt2.f  isdgmr.f  ppspf.f   runif.f   spofs.f   u12ls.f \
beskes.f  cgeir.f   csrot.f   dchfcm.f  dlsod.f   dqpsrt.f  dwnlt3.f  isdir.f   ppval.f   rwupdt.f  spoir.f   u12us.f \
besknu.f  cgemm.f   csscal.f  dchfdv.f  dlssud.f  dqrdc.f   dwnnls.f  isdomn.f  proc.f    s1merg.f  spopt.f   ulsia.f \
besks.f   cgemv.f   csvdc.f   dchfev.f  dmacon.f  dqrfac.f  dwritp.f  isort.f   procp.f   sasum.f   sposl.f   usrmat.f \
besy.f    cgerc.f   cswap.f   dchfie.f  dmgsbv.f  dqrsl.f   dwupdt.f  issbcg.f  prod.f    saxpy.f   sppco.f   vnwrms.f \
besy0.f   cgeru.f   csymm.f   dchkw.f   dmout.f   dqrslv.f  dx.f      isscg.f   prodp.f   sbcg.f    sppdi.f   wnlit.f \
besy1.f   cgesl.f   csyr2k.f  dchu.f    dmpar.f   dqwgtc.f  dx4.f     isscgn.f  prvec.f   sbhin.f   spperm.f  wnlsm.f \
besynu.f  cgtsl.f   csyrk.f   dchud.f   dnbco.f   dqwgtf.f  dxadd.f   isscgs.f  prwpge.f  sbocls.f  sppfa.f   wnlt1.f \
beta.f    ch.f      ctan.f    dckder.f  dnbdi.f   dqwgts.f  dxadj.f   issgmr.f  prwvir.f  sbols.f   sppsl.f   wnlt2.f \
betai.f   chbmv.f   ctanh.f   dcoef.f   dnbfa.f   drc.f     dxc210.f  issir.f   psgf.f    sbolsm.f  spsort.f  wnlt3.f \
bfqad.f   chemm.f   ctbmv.f   dcopy.f   dnbfs.f   drc3jj.f  dxcon.f   issomn.f  psi.f     scasum.f  sptsl.f   wnnls.f \
bi.f      chemv.f   ctbsv.f   dcopym.f  dnbsl.f   drc3jm.f  dxlcal.f  iswap.f   psifn.f   scg.f     sqrdc.f   xadd.f \
bie.f     cher.f    ctpmv.f   dcosdg.f  dnls1.f   drc6j.f   dxlegf.f  ivout.f   psixn.f   scgn.f    sqrsl.f   xadj.f \
binom.f   cher2.f   ctpsv.f   dcot.f    dnls1e.f  drd.f     dxnrmp.f  j4save.f  pvalue.f  scgs.f    sreadp.f  xc210.f \
bint4.f   cher2k.f  ctrco.f   dcov.f    dnrm2.f   dreadp.f  dxpmu.f   jairy.f   pythag.f  schdc.f   srlcal.f  xcon.f \
bintk.f   cherk.f   ctrdi.f   dcpplt.f  dnsq.f    dreort.f  dxpmup.f  la05ad.f  qag.f     schdd.f   srot.f    xerbla.f \
bisect.f  chfcm.f   ctrmm.f   dcscal.f  dnsqe.f   drf.f     dxpnrm.f  la05as.f  qage.f    schex.f   srotg.f   xerclr.f \
bkias.f   chfdv.f   ctrmv.f   dcsevl.f  dogleg.f  drj.f     dxpqnu.f  la05bd.f  qagi.f    schkw.f   srotm.f   xercnt.f \
bkisr.f   chfev.f   ctrsl.f   dcv.f     dohtrl.f            dxpsi.f   la05bs.f  qagie.f   schud.f   srotmg.f  xerdmp.f \
bksol.f   chfie.f   ctrsm.f   ddaini.f  domn.f    drkfs.f   dxqmu.f   la05cd.f  qagp.f    sclosm.f  ss2lt.f   xerhlt.f \
blktr1.f  chico.f   ctrsv.f   ddajac.f  dorth.f   drlcal.f  dxqnu.f   la05cs.f  qagpe.f   scnrm2.f  ss2y.f    xermax.f \
blktri.f  chidi.f   cuchk.f   ddanrm.f  dorthr.f  drot.f    dxred.f   la05ed.f  qags.f    scoef.f   ssbmv.f   xermsg.f \
bndacc.f  chiev.f   cunhj.f   ddaslv.f  dp1vlu.f  drotg.f   dxset.f   la05es.f  qagse.f   scopy.f   sscal.f   xerprn.f \
bndsol.f  chifa.f   cuni1.f   ddassl.f  dpbco.f   drotm.f   dy.f      llsia.f   qawc.f    scopym.f  ssd2s.f   xersve.f \
bnfac.f   chisl.f   cuni2.f   ddastp.f  dpbdi.f   drotmg.f  dy4.f     lmpar.f   qawce.f   scov.f    ssdbcg.f  xgetf.f \
bnslv.f   chkder.f  cunik.f   ddatrp.f  dpbfa.f   drsco.f   dyairy.f  lpdp.f    qawf.f    scpplt.f  ssdcg.f   xgetua.f \
bqr.f     chkpr4.f  cunk1.f   ddaws.f   dpbsl.f   ds2lt.f   e1.f      lsame.f   qawfe.f   sdaini.f  ssdcgn.f  xgetun.f \
bsgq8.f   chkprm.f  cunk2.f   ddawts.f  dpchbs.f  ds2y.f    efc.f     lsei.f    qawo.f    sdajac.f  ssdcgs.f  xlegf.f \
bskin.f   chksn4.f  cuoik.f   ddcor.f   dpchce.f  dsbmv.f   efcmn.f   lsi.f     qawoe.f   sdanrm.f  ssdgmr.f  xnrmp.f \
bspdoc.f  chksng.f  cv.f      ddcst.f   dpchci.f  dscal.f   ei.f      lsod.f    qaws.f    sdaslv.f  ssdi.f    xpmu.f \
bspdr.f   chpco.f   cwrsk.f   ddeabm.f  dpchcm.f  dsd2s.f   eisdoc.f  lssods.f  qawse.f   sdassl.f  ssdomn.f  xpmup.f \
bspev.f   chpdi.f   d1mach.f  ddebdf.f  dpchcs.f  dsdbcg.f  elmbak.f  lssuds.f  qc25c.f   sdastp.f  ssds.f    xpnrm.f \
bsplvd.f  chpfa.f   d1merg.f  dderkf.f  dpchdf.f  dsdcg.f   elmhes.f  macon.f   qc25f.f   sdatrp.f  ssdscl.f  xpqnu.f \
bsplvn.f  chpmv.f   d1mpyq.f  ddes.f    dpchfd.f  dsdcgn.f  eltran.f  mc20ad.f  qc25s.f   sdawts.f  ssgs.f    xpsi.f \
bsppp.f   chpr.f    d1updt.f  ddntl.f   dpchfe.f  dsdcgs.f  enorm.f   mc20as.f  qcheb.f   sdcor.f   ssiccg.f  xqmu.f \
bspvd.f   chpr2.f   d9aimp.f  ddntp.f   dpchia.f  dsdgmr.f  erf.f     mgsbv.f   qelg.f    sdcst.f   ssico.f   xqnu.f \
bspvn.f   chpsl.f   d9atn1.f  ddoglg.f  dpchic.f  dsdi.f    erfc.f    minfit.f  qform.f   sdntl.f   ssics.f   xred.f \
bsqad.f   chu.f     d9b0mp.f  ddot.f    dpchid.f  dsdomn.f            minso4.f  qk15.f    sdntp.f   ssidi.f   xset.f \
bsrh.f    cinvit.f  d9b1mp.f  ddpsc.f   dpchim.f  dsdot.f   exint.f   minsol.f  qk15i.f   sdot.f    ssiev.f   xsetf.f \
bvalu.f   ckscl.f   d9chu.f   ddpst.f   dpchkt.f  dsds.f    exprel.f  mpadd.f   qk15w.f   sdpsc.f   ssifa.f   xsetua.f \
          clbeta.f  d9gmic.f  ddriv1.f  dpchng.f  dsdscl.f  ezfft1.f  mpadd2.f  qk21.f    sdpst.f   ssilur.f  xsetun.f \
          clngam.f  d9gmit.f  ddriv2.f  dpchsp.f  dsgs.f    ezfftb.f  mpadd3.f  qk31.f    sdriv1.f  ssilus.f  yairy.f \
          clnrel.f  d9knus.f  ddriv3.f  dpchst.f  dsiccg.f  ezfftf.f  mpblas.f  qk41.f    sdriv2.f  ssisl.f   zabs.f \
c0lgmc.f  clog10.f  d9lgic.f  ddscl.f   dpchsw.f  dsico.f   ezffti.f  mpcdm.f   qk51.f    sdriv3.f  ssjac.f   zacai.f \
c1merg.f  cmgnbn.f  d9lgit.f  ddstp.f   dpcoef.f  dsics.f   fac.f     mpchk.f   qk61.f    sdscl.f   ssli.f    zacon.f \
c9lgmc.f  cmlri.f   d9lgmc.f  ddzro.f   dpfqad.f  dsidi.f   fc.f      mpcmd.f   qmomo.f   sdsdot.f  ssli2.f   zairy.f \
c9ln2r.f  cmpcsg.f  d9ln2r.f  de1.f     dpigmr.f  dsifa.f   fcmn.f    mpdivi.f  qnc79.f   sdstp.f   ssllti.f  zasyi.f \
cacai.f   cmposd.f  d9pak.f   deabm.f   dpincw.f  dsilur.f  fdjac1.f  mperr.f   qng.f     sdzro.f   sslubc.f  zbesh.f \
cacon.f   cmposn.f  d9upak.f  debdf.f   dpinit.f  dsilus.f  fdjac3.f  mpmaxr.f  qpdoc.f   sepeli.f  sslucn.f  zbesi.f \
cacos.f   cmposp.f  dacosh.f  defc.f    dpintm.f  dsindg.f  fdump.f   mpmlp.f   qpsrt.f   sepx4.f   sslucs.f  zbesj.f \
cacosh.f  cmptr3.f  dai.f     defcmn.f  dpjac.f   dsisl.f   fftdoc.f  mpmul.f   qrfac.f   sgbco.f   sslugm.f  zbesk.f \
cairy.f   cmptrx.f  daie.f    defe4.f   dplint.f  dsjac.f   figi.f    mpmul2.f  qrsolv.f  sgbdi.f   sslui.f   zbesy.f \
carg.f    cnbco.f   dasinh.f  defehl.f  dplpce.f  dsli.f    figi2.f   mpmuli.f  qs2i1d.f  sgbfa.f   sslui2.f  zbinu.f \
casin.f   cnbdi.f   dasum.f   defer.f   dplpdm.f  dsli2.f   fulmat.f  mpnzr.f   qs2i1r.f  sgbmv.f   sslui4.f  zbiry.f \
casinh.f  cnbfa.f   dasyik.f  dei.f     dplpfe.f  dsllti.f  fundoc.f  mpovfl.f  qwgtc.f   sgbsl.f   ssluom.f  zbknu.f \
casyi.f   cnbfs.f   dasyjy.f  denorm.f  dplpfl.f  dslubc.f  fzero.f   mpstr.f   qwgtf.f   sgeco.f   ssluti.f  zbuni.f \
catan.f   cnbir.f   datanh.f  derf.f    dplpmn.f  dslucn.f  gami.f    mpunfl.f  qwgts.f   sgedi.f   ssmmi2.f  zbunk.f \
catan2.f  cnbsl.f   davint.f  derfc.f   dplpmu.f  dslucs.f  gamic.f   numxer.f  qzhes.f   sgeev.f   ssmmti.f  zdiv.f \
catanh.f  combak.f  daws.f    derkf.f   dplpup.f  dslugm.f  gamit.f   ohtrol.f  qzit.f    sgefa.f   ssmtv.f   zexp.f \
caxpy.f   comhes.f  daxpy.f   derkfs.f  dpnnzr.f  dslui.f   gamlim.f  ohtror.f  qzval.f   sgefs.f   ssmv.f    zkscl.f \
cbabk2.f  comlr.f   dbcg.f    des.f     dpoch.f   dslui2.f  gamln.f   ortbak.f  qzvec.f   sgeir.f   ssort.f   zlog.f \
cbal.f    comlr2.f  dbdiff.f            dpoch1.f  dslui4.f  gamma.f   orthes.f  r1mach.f  sgemm.f   sspco.f   zmlri.f \
cbesh.f   compb.f   dbesi.f   dexint.f  dpoco.f   dsluom.f  gamr.f    ortho4.f  r1mpyq.f  sgemv.f   sspdi.f   zmlt.f \
cbesi.f   comqr.f   dbesi0.f  dexprl.f  dpodi.f   dsluti.f  gamrn.f   orthog.f  r1updt.f  sger.f    sspev.f   zrati.f \
cbesj.f   comqr2.f  dbesi1.f  dfac.f    dpofa.f   dslvs.f   gaus8.f   orthol.f  r9aimp.f  sgesl.f   sspfa.f   zs1s2.f \
cbesk.f   cortb.f   dbesj.f   dfc.f     dpofs.f   dsmmi2.f  genbun.f  orthor.f  r9atn1.f  sglss.f   sspmv.f   zseri.f \
cbesy.f   corth.f   dbesj0.f  dfcmn.f   dpolcf.f  dsmmti.f  h12.f     ortran.f  r9chu.f   sgmres.f  sspr.f    zshch.f \
cbeta.f   cosdg.f   dbesj1.f  dfdjc1.f  dpolft.f  dsmtv.f   hfti.f    passb.f   r9gmic.f  sgtsl.f   sspr2.f   zsqrt.f \
cbinu.f   cosgen.f  dbesk.f   dfdjc3.f  dpolvl.f  dsmv.f    hkseq.f   passb2.f  r9gmit.f  shels.f   sspsl.f   zuchk.f \
cbiry.f   cosqb.f   dbesk0.f  dfehl.f   dpopt.f   dsort.f   hpperm.f  passb3.f  r9knus.f  sheqr.f   ssvdc.f   zunhj.f \
cbknu.f   cosqb1.f  dbesk1.f  dfspvd.f  dposl.f   dsos.f    hpsort.f  passb4.f  r9lgic.f  sindg.f   sswap.f   zuni1.f \
cblkt1.f  cosqf.f   dbesks.f  dfspvn.f  dppco.f   dsoseq.f  hqr.f     passb5.f  r9lgit.f  sinqb.f   ssymm.f   zuni2.f \
cblktr.f  cosqf1.f  dbesy.f   dfulmt.f  dppdi.f   dsossl.f  hqr2.f    passf.f   r9lgmc.f  sinqf.f   ssymv.f   zunik.f \
cbrt.f    cosqi.f   dbesy0.f  dfzero.f  dpperm.f  dspco.f   hstart.f  passf2.f  r9ln2r.f  sinqi.f   ssyr.f    zunk1.f \
cbuni.f   cost.f    dbesy1.f  dgami.f   dppfa.f   dspdi.f   hstcrt.f  passf3.f  r9pak.f   sint.f    ssyr2.f   zunk2.f \
cbunk.f   costi.f   dbeta.f   dgamic.f  dppgq8.f  dspenc.f  hstcs1.f  passf4.f  r9upak.f  sinti.f   ssyr2k.f  zuoik.f \
ccbrt.f   cot.f     dbetai.f  dgamit.f  dppqad.f  dspfa.f   hstcsp.f  passf5.f  radb2.f   sintrp.f  ssyrk.f   zwrsk.f \
cchdc.f   cpadd.f   dbfqad.f  dgamlm.f  dppsl.f   dsplp.f   hstcyl.f  pchbs.f   radb3.f   sir.f     stbmv.f \
cchdd.f   cpbco.f   dbhin.f   dgamln.f  dppval.f  dspmv.f   hstplr.f  pchce.f   radb4.f   sllti2.f  stbsv.f \
cchex.f   cpbdi.f   dbi.f     dgamma.f  dprvec.f  dspr.f    hstssp.f  pchci.f   radb5.f   slpdoc.f  steps.f \
cchud.f   cpbfa.f   dbie.f    dgamr.f   dprwpg.f  dspr2.f   htrib3.f  pchcm.f   radbg.f   slvs.f    stin.f \
ccmpb.f   cpbsl.f   dbinom.f  dgamrn.f  dprwvr.f  dspsl.f   htribk.f  pchcs.f   radf2.f   smout.f   stod.f \
ccopy.f   cpevl.f   dbint4.f  dgaus8.f  dpsi.f    dsteps.f  htrid3.f  pchdf.f   radf3.f   snbco.f   stor1.f \
ccosh.f   cpevlr.f  dbintk.f  dgbco.f   dpsifn.f  dstod.f   htridi.f  pchdoc.f  radf4.f   snbdi.f   stout.f \
ccot.f    cpoco.f   dbkias.f  dgbdi.f   dpsixn.f  dstor1.f  hvnrm.f   pchfd.f   radf5.f   snbfa.f   stpmv.f \
cdcdot.f  cpodi.f   dbkisr.f  dgbfa.f   dpsort.f  dstway.f  hw3crt.f  pchfe.f   radfg.f   snbfs.f   stpsv.f \
cdcor.f   cpofa.f   dbksol.f  dgbmv.f   dptsl.f   dsuds.f   hwscrt.f  pchia.f   rand.f    snbir.f   strco.f \
cdcst.f   cpofs.f   dbndac.f  dgbsl.f   dqag.f    dsvco.f   hwscs1.f  pchic.f   ratqr.f   snbsl.f   strdi.f 

COMMON_SRC :=  $(patsubst %,$(SRC)/%,$(_COMMON_SRC))

VPATH		:= $(dir $(COMMON_SRC))
COMMON_OBJS	:= $(patsubst %.f, $(ODIR)/%.o, $(notdir $(COMMON_SRC)))  #$(ODIR)/glad.o
#COMMON_C_OBJS	:= $(patsubst %.c, $(ODIR)/%.o, $(notdir $(COMMON_C_SRC))) 

IDIR	=.
_DEPS	:= Bmp.h glext.h glExtension.h LorenzAttr.h \
	Matrices.h Sphere.h vec4.h Vectors.h LMapParameters.h \
	colorWheel.h global.h logisticMapBoundaryPostOnsetOfChaos.h vectorPianoKeyFrequencies.h
DEPS	:=  $(patsubst %,$(IDIR)/%,$(_DEPS))



#OBJ_RELEASE =	${ODIR}/beep.o ${ODIR}/Bmp.o \
#		${ODIR}/chaoticWalk4.o ${ODIR}/glExtension.o \
#		${ODIR}/Matrices.o ${ODIR}/Sphere.o  \
#		${ODIR}/main.o
			 
OBJ_RELEASE	= ${COMMON_OBJS}


#all: $(BIN)/$(EXECUTABLE)
all: release

.SUFFIXES: .f90 .f .o
%.o : %.mod
#include Make.inc

#TRL=top.o
#OBJ=top.o #${TRL}

lib: ${OBJ_RELEASE} #top.o
	ar -cru libslatec.a ${OBJ_RELEASE} 

clean:
	/bin/rm -f *.o *.T *.M *.mod *.l* *.L V*.inc V*.f *.vo

#trl_comm_mpi.o trl_comm_none.o ${TRL}: trl_info.o

.f.o:
	${F77} ${FFLAGS} -c $< -o ${ODIR}/$@
.f90.o:
	${F9X} ${FFLAGS} -c $< -o ${ODIR}/$@


$(ODIR)/%.o: %.f
	${F77} ${FFLAGS} -c $< -o $@

#$(BIN)/$(EXECUTABLE): $(COMMON_OBJS)  
#	@echo building output ...  ${COMMON_SRC} ---   ${DEPS} 
#	$(F9X) -o $(BIN)/$(EXECUTABLE) $(COMMON_OBJS) #$(LIBRARIES)

#$(ODIR)/%.o: %.f # $(DEPS)
##	$(F9X) $(F9X_FLAGS) -I$(INCLUDE) -L$(LIB) -c $< -o $@
#	${F9X} ${CFLAGS_RELEASE} ${INC_RELEASE} -I ${SOUNDIO_INCLUDE_DIR} -c $< -o $@

#$(ODIR)/glad.o: glad.c $(DEPS)
#	$(CC) $(C_FLAGS) -I$(INCLUDE) -I$(INCLUDE2) -L$(LIB) -c $< -o $@


clean: clean_release

before_release: 
	test -d ${binDIR_RELEASE} || mkdir -p ${binDIR_RELEASE}
	test -d ${ODIR} || mkdir -p ${ODIR}

after_release: 
	test -f ${binDIR_RELEASE}/${EARTH2048BMP} || cp ${EARTH2048BMP} ${binDIR_RELEASE}

release: before_release lib #out_release #after_release

out_release: before_release ${OBJ_RELEASE} ${DEP_RELEASE}
	${LD} ${LIBDIR_RELEASE} -o ${OUT_RELEASE} ${OBJ_RELEASE}  ${LDFLAGS_RELEASE} ${LIB_RELEASE}


#${ODIR}/beep.o: beep.cpp
#	${F9X} ${CFLAGS_RELEASE} ${INC_RELEASE} -c beep.cpp -o ${ODIR}/beep.o -I ${SOUNDIO_INCLUDE_DIR}


clean_release: 
	rm -f ${OBJ_RELEASE} ${OUT_RELEASE}
	rm -rf ${binDIR_RELEASE}
	rm -rf ${ODIR}
	#test -d ${binDIR_RELEASE} || mkdir -p ${binDIR_RELEASE}
	#cp earth2048.bmp ${binDIR_RELEASE}

.PHONY: before_release after_release clean_release


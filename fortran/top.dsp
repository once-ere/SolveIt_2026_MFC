# Microsoft Developer Studio Project File - Name="top" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=top - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "top.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "top.mak" CFG="top - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "top - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "top - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "top - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /threads /warn:nofileopt
# SUBTRACT F90 /assume:underscore
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "top - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /threads /warn:argument_checking /warn:nofileopt
# SUBTRACT F90 /assume:underscore
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\topD.lib"

!ENDIF 

# Begin Target

# Name "top - Win32 Release"
# Name "top - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "TRLan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TRLan\dsort2.f
# End Source File
# Begin Source File

SOURCE=.\TRLan\dstqrb.f
# End Source File
# Begin Source File

SOURCE=.\TRLan\restart.f90
DEP_F90_RESTA=\
	".\Release\trl_info.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\TRLan\trl_comm_none.f90
DEP_F90_TRL_C=\
	".\Release\trl_info.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\TRLan\trl_info.f90
DEP_F90_TRL_I=\
	".\Release\DATA_PASS.MOD"\
	

!IF  "$(CFG)" == "top - Win32 Release"

!ELSEIF  "$(CFG)" == "top - Win32 Debug"

!ENDIF 

F90_MODOUT=\
	"trl_info" \
	"trl_interface"

# End Source File
# Begin Source File

SOURCE=.\TRLan\trlan.f90
DEP_F90_TRLAN=\
	".\Release\DATA_PASS.MOD"\
	".\Release\trl_info.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\TRLan\trlan77.f90
DEP_F90_TRLAN7=\
	".\Release\DATA_PASS.MOD"\
	".\Release\trl_info.mod"\
	".\Release\trl_interface.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\TRLan\trlaux.f90
DEP_F90_TRLAU=\
	".\Release\trl_info.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\TRLan\trlcore.f90
DEP_F90_TRLCO=\
	".\Release\DATA_PASS.MOD"\
	".\Release\trl_info.mod"\
	
# End Source File
# End Group
# Begin Group "fit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fit\curfit.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpback.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpbspl.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpchec.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpcurf.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpdisc.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpgivs.f
# End Source File
# Begin Source File

SOURCE=.\fit\fpknot.f
# End Source File
# Begin Source File

SOURCE=.\fit\fprati.f
# End Source File
# Begin Source File

SOURCE=.\fit\fprota.f
# End Source File
# Begin Source File

SOURCE=.\fit\splev.f
# End Source File
# End Group
# Begin Source File

SOURCE=.\APPROX.f
DEP_F90_APPRO=\
	".\Release\DATA_PASS.MOD"\
	
# End Source File
# Begin Source File

SOURCE=.\Bessel.f
# End Source File
# Begin Source File

SOURCE=.\drd.f
DEP_F90_DRD_F=\
	".\Release\DATA_PASS.MOD"\
	
# End Source File
# Begin Source File

SOURCE=.\drf.f
DEP_F90_DRF_F=\
	".\Release\DATA_PASS.MOD"\
	
# End Source File
# Begin Source File

SOURCE=.\dvi0.f
# End Source File
# Begin Source File

SOURCE=.\dvi1.f
# End Source File
# Begin Source File

SOURCE=.\dvj0.f
# End Source File
# Begin Source File

SOURCE=.\dvj1.f
# End Source File
# Begin Source File

SOURCE=.\dvk0.f
# End Source File
# Begin Source File

SOURCE=.\dvk1.f
# End Source File
# Begin Source File

SOURCE=.\dvy0.f
# End Source File
# Begin Source File

SOURCE=.\dvy1.f
# End Source File
# Begin Source File

SOURCE=.\dwcs.f
# End Source File
# Begin Source File

SOURCE=.\dwutil.f
# End Source File
# Begin Source File

SOURCE=.\elliptic_integral.f
# End Source File
# Begin Source File

SOURCE=.\fdump.f
# End Source File
# Begin Source File

SOURCE=.\j4save.f
# End Source File
# Begin Source File

SOURCE=.\machcon.f
# End Source File
# Begin Source File

SOURCE=.\sledge.f
# End Source File
# Begin Source File

SOURCE=.\src_ieee.f
# End Source File
# Begin Source File

SOURCE=.\top.f90

!IF  "$(CFG)" == "top - Win32 Release"

!ELSEIF  "$(CFG)" == "top - Win32 Debug"

!ENDIF 

F90_MODOUT=\
	"DATA_PASS" \
	"DATA_2D"

# End Source File
# Begin Source File

SOURCE=.\xercnt.f
# End Source File
# Begin Source File

SOURCE=.\xerhlt.f
# End Source File
# Begin Source File

SOURCE=.\xermsg.f
# End Source File
# Begin Source File

SOURCE=.\xerprn.f
# End Source File
# Begin Source File

SOURCE=.\xersve.f
# End Source File
# Begin Source File

SOURCE=.\xgetua.f
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project

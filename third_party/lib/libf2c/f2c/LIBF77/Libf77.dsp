# Microsoft Developer Studio Project File - Name="LIBF77" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LIBF77 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Libf77.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Libf77.mak" CFG="LIBF77 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LIBF77 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LIBF77 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "LIBF77 - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# ADD BASE F90 /include:".\WinRel/"
# ADD F90 /include:".\WinRel/"
# ADD BASE CPP /nologo /G3 /MT /W3 /GX /Ox /Ot /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MT /Za /W3 /GX /Ox /Ot /Ob2 /Gy /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LIBF77 - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# ADD BASE F90 /include:".\WinDebug/"
# ADD F90 /include:".\WinDebug/"
# ADD BASE CPP /nologo /MT /W3 /GX /Z7 /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MTd /Za /W3 /GX /Z7 /Od /Gy /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\WinDebug\Libf77D.lib"

!ENDIF 

# Begin Target

# Name "LIBF77 - Win32 Release"
# Name "LIBF77 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ABORT_.C
# End Source File
# Begin Source File

SOURCE=.\C_ABS.C
# End Source File
# Begin Source File

SOURCE=.\C_COS.C
# End Source File
# Begin Source File

SOURCE=.\C_DIV.C
# End Source File
# Begin Source File

SOURCE=.\C_EXP.C
# End Source File
# Begin Source File

SOURCE=.\C_LOG.C
# End Source File
# Begin Source File

SOURCE=.\C_SIN.C
# End Source File
# Begin Source File

SOURCE=.\C_SQRT.C
# End Source File
# Begin Source File

SOURCE=.\CABS.C
# End Source File
# Begin Source File

SOURCE=.\D_ABS.C
# End Source File
# Begin Source File

SOURCE=.\D_ACOS.C
# End Source File
# Begin Source File

SOURCE=.\D_ASIN.C
# End Source File
# Begin Source File

SOURCE=.\D_ATAN.C
# End Source File
# Begin Source File

SOURCE=.\D_ATN2.C
# End Source File
# Begin Source File

SOURCE=.\D_CNJG.C
# End Source File
# Begin Source File

SOURCE=.\D_COS.C
# End Source File
# Begin Source File

SOURCE=.\D_COSH.C
# End Source File
# Begin Source File

SOURCE=.\D_DIM.C
# End Source File
# Begin Source File

SOURCE=.\D_EXP.C
# End Source File
# Begin Source File

SOURCE=.\D_IMAG.C
# End Source File
# Begin Source File

SOURCE=.\D_INT.C
# End Source File
# Begin Source File

SOURCE=.\D_LG10.C
# End Source File
# Begin Source File

SOURCE=.\D_LOG.C
# End Source File
# Begin Source File

SOURCE=.\D_MOD.C
# End Source File
# Begin Source File

SOURCE=.\D_NINT.C
# End Source File
# Begin Source File

SOURCE=.\D_PROD.C
# End Source File
# Begin Source File

SOURCE=.\D_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\D_SIN.C
# End Source File
# Begin Source File

SOURCE=.\D_SINH.C
# End Source File
# Begin Source File

SOURCE=.\D_SQRT.C
# End Source File
# Begin Source File

SOURCE=.\D_TAN.C
# End Source File
# Begin Source File

SOURCE=.\D_TANH.C
# End Source File
# Begin Source File

SOURCE=.\DERF_.C
# End Source File
# Begin Source File

SOURCE=.\DERFC_.C
# End Source File
# Begin Source File

SOURCE=.\EF1ASC_.C
# End Source File
# Begin Source File

SOURCE=.\EF1CMC_.C
# End Source File
# Begin Source File

SOURCE=.\ERF_.C
# End Source File
# Begin Source File

SOURCE=.\ERFC_.C
# End Source File
# Begin Source File

SOURCE=.\GETARG_.C
# End Source File
# Begin Source File

SOURCE=.\GETENV_.C
# End Source File
# Begin Source File

SOURCE=.\H_ABS.C
# End Source File
# Begin Source File

SOURCE=.\H_DIM.C
# End Source File
# Begin Source File

SOURCE=.\H_DNNT.C
# End Source File
# Begin Source File

SOURCE=.\H_INDX.C
# End Source File
# Begin Source File

SOURCE=.\H_LEN.C
# End Source File
# Begin Source File

SOURCE=.\H_MOD.C
# End Source File
# Begin Source File

SOURCE=.\H_NINT.C
# End Source File
# Begin Source File

SOURCE=.\H_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\HL_GE.C
# End Source File
# Begin Source File

SOURCE=.\HL_GT.C
# End Source File
# Begin Source File

SOURCE=.\HL_LE.C
# End Source File
# Begin Source File

SOURCE=.\HL_LT.C
# End Source File
# Begin Source File

SOURCE=.\I_ABS.C
# End Source File
# Begin Source File

SOURCE=.\I_DIM.C
# End Source File
# Begin Source File

SOURCE=.\I_DNNT.C
# End Source File
# Begin Source File

SOURCE=.\I_INDX.C
# End Source File
# Begin Source File

SOURCE=.\I_LEN.C
# End Source File
# Begin Source File

SOURCE=.\I_MOD.C
# End Source File
# Begin Source File

SOURCE=.\I_NINT.C
# End Source File
# Begin Source File

SOURCE=.\I_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\IARGC_.C
# End Source File
# Begin Source File

SOURCE=.\L_GE.C
# End Source File
# Begin Source File

SOURCE=.\L_GT.C
# End Source File
# Begin Source File

SOURCE=.\L_LE.C
# End Source File
# Begin Source File

SOURCE=.\L_LT.C
# End Source File
# Begin Source File

SOURCE=.\MAIN.C
# End Source File
# Begin Source File

SOURCE=.\POW_CI.C
# End Source File
# Begin Source File

SOURCE=.\POW_DD.C
# End Source File
# Begin Source File

SOURCE=.\POW_DI.C
# End Source File
# Begin Source File

SOURCE=.\POW_HH.C
# End Source File
# Begin Source File

SOURCE=.\POW_II.C
# End Source File
# Begin Source File

SOURCE=.\POW_QQ.C
# End Source File
# Begin Source File

SOURCE=.\POW_RI.C
# End Source File
# Begin Source File

SOURCE=.\POW_ZI.C
# End Source File
# Begin Source File

SOURCE=.\POW_ZZ.C
# End Source File
# Begin Source File

SOURCE=.\R_ABS.C
# End Source File
# Begin Source File

SOURCE=.\R_ACOS.C
# End Source File
# Begin Source File

SOURCE=.\R_ASIN.C
# End Source File
# Begin Source File

SOURCE=.\R_ATAN.C
# End Source File
# Begin Source File

SOURCE=.\R_ATN2.C
# End Source File
# Begin Source File

SOURCE=.\R_CNJG.C
# End Source File
# Begin Source File

SOURCE=.\R_COS.C
# End Source File
# Begin Source File

SOURCE=.\R_COSH.C
# End Source File
# Begin Source File

SOURCE=.\R_DIM.C
# End Source File
# Begin Source File

SOURCE=.\R_EXP.C
# End Source File
# Begin Source File

SOURCE=.\R_IMAG.C
# End Source File
# Begin Source File

SOURCE=.\R_INT.C
# End Source File
# Begin Source File

SOURCE=.\R_LG10.C
# End Source File
# Begin Source File

SOURCE=.\R_LOG.C
# End Source File
# Begin Source File

SOURCE=.\R_MOD.C
# End Source File
# Begin Source File

SOURCE=.\R_NINT.C
# End Source File
# Begin Source File

SOURCE=.\R_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\R_SIN.C
# End Source File
# Begin Source File

SOURCE=.\R_SINH.C
# End Source File
# Begin Source File

SOURCE=.\R_SQRT.C
# End Source File
# Begin Source File

SOURCE=.\R_TAN.C
# End Source File
# Begin Source File

SOURCE=.\R_TANH.C
# End Source File
# Begin Source File

SOURCE=.\S_CAT.C
# End Source File
# Begin Source File

SOURCE=.\S_CMP.C
# End Source File
# Begin Source File

SOURCE=.\S_COPY.C
# End Source File
# Begin Source File

SOURCE=.\S_PAUS.C
# End Source File
# Begin Source File

SOURCE=.\S_RNGE.C
# End Source File
# Begin Source File

SOURCE=.\S_STOP.C
# End Source File
# Begin Source File

SOURCE=.\SIG_DIE.C
# End Source File
# Begin Source File

SOURCE=.\SIGNAL_.C
# End Source File
# Begin Source File

SOURCE=.\SYSTEM_.C
# End Source File
# Begin Source File

SOURCE=.\VERSION.C
# End Source File
# Begin Source File

SOURCE=.\Z_ABS.C
# End Source File
# Begin Source File

SOURCE=.\Z_COS.C
# End Source File
# Begin Source File

SOURCE=.\Z_DIV.C
# End Source File
# Begin Source File

SOURCE=.\Z_EXP.C
# End Source File
# Begin Source File

SOURCE=.\Z_LOG.C
# End Source File
# Begin Source File

SOURCE=.\Z_SIN.C
# End Source File
# Begin Source File

SOURCE=.\Z_SQRT.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

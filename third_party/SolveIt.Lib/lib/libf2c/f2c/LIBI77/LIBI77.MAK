# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=LIBI77 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to LIBI77 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "LIBI77 - Win32 Release" && "$(CFG)" != "LIBI77 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "LIBI77.MAK" CFG="LIBI77 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LIBI77 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LIBI77 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "LIBI77 - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "LIBI77 - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\LIBI77.lib"

CLEAN : 
	-@erase "$(INTDIR)\BACKSPAC.OBJ"
	-@erase "$(INTDIR)\CLOSE.OBJ"
	-@erase "$(INTDIR)\DFE.OBJ"
	-@erase "$(INTDIR)\DOLIO.OBJ"
	-@erase "$(INTDIR)\DUE.OBJ"
	-@erase "$(INTDIR)\ENDFILE.OBJ"
	-@erase "$(INTDIR)\ERR.OBJ"
	-@erase "$(INTDIR)\FMT.OBJ"
	-@erase "$(INTDIR)\FMTLIB.OBJ"
	-@erase "$(INTDIR)\IIO.OBJ"
	-@erase "$(INTDIR)\ILNW.OBJ"
	-@erase "$(INTDIR)\INQUIRE.OBJ"
	-@erase "$(INTDIR)\LREAD.OBJ"
	-@erase "$(INTDIR)\LWRITE.OBJ"
	-@erase "$(INTDIR)\OPEN.OBJ"
	-@erase "$(INTDIR)\RDFMT.OBJ"
	-@erase "$(INTDIR)\REWIND.OBJ"
	-@erase "$(INTDIR)\RSFE.OBJ"
	-@erase "$(INTDIR)\RSLI.OBJ"
	-@erase "$(INTDIR)\RSNE.OBJ"
	-@erase "$(INTDIR)\SFE.OBJ"
	-@erase "$(INTDIR)\SUE.OBJ"
	-@erase "$(INTDIR)\TYPESIZE.OBJ"
	-@erase "$(INTDIR)\UIO.OBJ"
	-@erase "$(INTDIR)\UTIL.OBJ"
	-@erase "$(INTDIR)\VERSION.OBJ"
	-@erase "$(INTDIR)\WREF.OBJ"
	-@erase "$(INTDIR)\WRTFMT.OBJ"
	-@erase "$(INTDIR)\WSFE.OBJ"
	-@erase "$(INTDIR)\WSLE.OBJ"
	-@erase "$(INTDIR)\WSNE.OBJ"
	-@erase "$(INTDIR)\XWSNE.OBJ"
	-@erase "$(OUTDIR)\LIBI77.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /G3 /MT /W3 /GX /Ox /Ot /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G3 /MT /W3 /GX /Ox /Ot /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NON_UNIX_STDIO" /D "_MBCS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G3 /MT /W3 /GX /Ox /Ot /Ob2 /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "NON_UNIX_STDIO" /D "_MBCS" /Fp"$(INTDIR)/LIBI77.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/LIBI77.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/LIBI77.lib" 
LIB32_OBJS= \
	"$(INTDIR)\BACKSPAC.OBJ" \
	"$(INTDIR)\CLOSE.OBJ" \
	"$(INTDIR)\DFE.OBJ" \
	"$(INTDIR)\DOLIO.OBJ" \
	"$(INTDIR)\DUE.OBJ" \
	"$(INTDIR)\ENDFILE.OBJ" \
	"$(INTDIR)\ERR.OBJ" \
	"$(INTDIR)\FMT.OBJ" \
	"$(INTDIR)\FMTLIB.OBJ" \
	"$(INTDIR)\IIO.OBJ" \
	"$(INTDIR)\ILNW.OBJ" \
	"$(INTDIR)\INQUIRE.OBJ" \
	"$(INTDIR)\LREAD.OBJ" \
	"$(INTDIR)\LWRITE.OBJ" \
	"$(INTDIR)\OPEN.OBJ" \
	"$(INTDIR)\RDFMT.OBJ" \
	"$(INTDIR)\REWIND.OBJ" \
	"$(INTDIR)\RSFE.OBJ" \
	"$(INTDIR)\RSLI.OBJ" \
	"$(INTDIR)\RSNE.OBJ" \
	"$(INTDIR)\SFE.OBJ" \
	"$(INTDIR)\SUE.OBJ" \
	"$(INTDIR)\TYPESIZE.OBJ" \
	"$(INTDIR)\UIO.OBJ" \
	"$(INTDIR)\UTIL.OBJ" \
	"$(INTDIR)\VERSION.OBJ" \
	"$(INTDIR)\WREF.OBJ" \
	"$(INTDIR)\WRTFMT.OBJ" \
	"$(INTDIR)\WSFE.OBJ" \
	"$(INTDIR)\WSLE.OBJ" \
	"$(INTDIR)\WSNE.OBJ" \
	"$(INTDIR)\XWSNE.OBJ"

"$(OUTDIR)\LIBI77.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "LIBI77 - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\LIBI77.lib"

CLEAN : 
	-@erase "$(INTDIR)\BACKSPAC.OBJ"
	-@erase "$(INTDIR)\CLOSE.OBJ"
	-@erase "$(INTDIR)\DFE.OBJ"
	-@erase "$(INTDIR)\DOLIO.OBJ"
	-@erase "$(INTDIR)\DUE.OBJ"
	-@erase "$(INTDIR)\ENDFILE.OBJ"
	-@erase "$(INTDIR)\ERR.OBJ"
	-@erase "$(INTDIR)\FMT.OBJ"
	-@erase "$(INTDIR)\FMTLIB.OBJ"
	-@erase "$(INTDIR)\IIO.OBJ"
	-@erase "$(INTDIR)\ILNW.OBJ"
	-@erase "$(INTDIR)\INQUIRE.OBJ"
	-@erase "$(INTDIR)\LREAD.OBJ"
	-@erase "$(INTDIR)\LWRITE.OBJ"
	-@erase "$(INTDIR)\OPEN.OBJ"
	-@erase "$(INTDIR)\RDFMT.OBJ"
	-@erase "$(INTDIR)\REWIND.OBJ"
	-@erase "$(INTDIR)\RSFE.OBJ"
	-@erase "$(INTDIR)\RSLI.OBJ"
	-@erase "$(INTDIR)\RSNE.OBJ"
	-@erase "$(INTDIR)\SFE.OBJ"
	-@erase "$(INTDIR)\SUE.OBJ"
	-@erase "$(INTDIR)\TYPESIZE.OBJ"
	-@erase "$(INTDIR)\UIO.OBJ"
	-@erase "$(INTDIR)\UTIL.OBJ"
	-@erase "$(INTDIR)\VERSION.OBJ"
	-@erase "$(INTDIR)\WREF.OBJ"
	-@erase "$(INTDIR)\WRTFMT.OBJ"
	-@erase "$(INTDIR)\WSFE.OBJ"
	-@erase "$(INTDIR)\WSLE.OBJ"
	-@erase "$(INTDIR)\WSNE.OBJ"
	-@erase "$(INTDIR)\XWSNE.OBJ"
	-@erase "$(OUTDIR)\LIBI77.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NON_UNIX_STDIO" /D "_MBCS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "NON_UNIX_STDIO" /D "_MBCS" /Fp"$(INTDIR)/LIBI77.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/LIBI77.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/LIBI77.lib" 
LIB32_OBJS= \
	"$(INTDIR)\BACKSPAC.OBJ" \
	"$(INTDIR)\CLOSE.OBJ" \
	"$(INTDIR)\DFE.OBJ" \
	"$(INTDIR)\DOLIO.OBJ" \
	"$(INTDIR)\DUE.OBJ" \
	"$(INTDIR)\ENDFILE.OBJ" \
	"$(INTDIR)\ERR.OBJ" \
	"$(INTDIR)\FMT.OBJ" \
	"$(INTDIR)\FMTLIB.OBJ" \
	"$(INTDIR)\IIO.OBJ" \
	"$(INTDIR)\ILNW.OBJ" \
	"$(INTDIR)\INQUIRE.OBJ" \
	"$(INTDIR)\LREAD.OBJ" \
	"$(INTDIR)\LWRITE.OBJ" \
	"$(INTDIR)\OPEN.OBJ" \
	"$(INTDIR)\RDFMT.OBJ" \
	"$(INTDIR)\REWIND.OBJ" \
	"$(INTDIR)\RSFE.OBJ" \
	"$(INTDIR)\RSLI.OBJ" \
	"$(INTDIR)\RSNE.OBJ" \
	"$(INTDIR)\SFE.OBJ" \
	"$(INTDIR)\SUE.OBJ" \
	"$(INTDIR)\TYPESIZE.OBJ" \
	"$(INTDIR)\UIO.OBJ" \
	"$(INTDIR)\UTIL.OBJ" \
	"$(INTDIR)\VERSION.OBJ" \
	"$(INTDIR)\WREF.OBJ" \
	"$(INTDIR)\WRTFMT.OBJ" \
	"$(INTDIR)\WSFE.OBJ" \
	"$(INTDIR)\WSLE.OBJ" \
	"$(INTDIR)\WSNE.OBJ" \
	"$(INTDIR)\XWSNE.OBJ"

"$(OUTDIR)\LIBI77.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "LIBI77 - Win32 Release"
# Name "LIBI77 - Win32 Debug"

!IF  "$(CFG)" == "LIBI77 - Win32 Release"

!ELSEIF  "$(CFG)" == "LIBI77 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\BACKSPAC.C
DEP_CPP_BACKS=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\BACKSPAC.OBJ" : $(SOURCE) $(DEP_CPP_BACKS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLOSE.C
DEP_CPP_CLOSE=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\CLOSE.OBJ" : $(SOURCE) $(DEP_CPP_CLOSE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DFE.C
DEP_CPP_DFE_C=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\DFE.OBJ" : $(SOURCE) $(DEP_CPP_DFE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DOLIO.C
DEP_CPP_DOLIO=\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\DOLIO.OBJ" : $(SOURCE) $(DEP_CPP_DOLIO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DUE.C
DEP_CPP_DUE_C=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\DUE.OBJ" : $(SOURCE) $(DEP_CPP_DUE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ENDFILE.C
DEP_CPP_ENDFI=\
	".\fio.h"\
	".\rawio.h"\
	{$(INCLUDE)}"\DOWNHILL\DH_INC.H"\
	{$(INCLUDE)}"\f2c.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ENDFILE.OBJ" : $(SOURCE) $(DEP_CPP_ENDFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ERR.C
DEP_CPP_ERR_C=\
	".\fio.h"\
	".\fmt.h"\
	".\rawio.h"\
	{$(INCLUDE)}"\DOWNHILL\DH_INC.H"\
	{$(INCLUDE)}"\DOWNHILL\DH_POSIX.H"\
	{$(INCLUDE)}"\f2c.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ERR.OBJ" : $(SOURCE) $(DEP_CPP_ERR_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FMT.C
DEP_CPP_FMT_C=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\FMT.OBJ" : $(SOURCE) $(DEP_CPP_FMT_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FMTLIB.C

"$(INTDIR)\FMTLIB.OBJ" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IIO.C
DEP_CPP_IIO_C=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\IIO.OBJ" : $(SOURCE) $(DEP_CPP_IIO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ILNW.C
DEP_CPP_ILNW_=\
	".\fio.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\ILNW.OBJ" : $(SOURCE) $(DEP_CPP_ILNW_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INQUIRE.C
DEP_CPP_INQUI=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\INQUIRE.OBJ" : $(SOURCE) $(DEP_CPP_INQUI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LREAD.C
DEP_CPP_LREAD=\
	".\fio.h"\
	".\fmt.h"\
	".\fp.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\LREAD.OBJ" : $(SOURCE) $(DEP_CPP_LREAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LWRITE.C
DEP_CPP_LWRIT=\
	".\fio.h"\
	".\fmt.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\LWRITE.OBJ" : $(SOURCE) $(DEP_CPP_LWRIT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OPEN.C
DEP_CPP_OPEN_=\
	".\fio.h"\
	".\rawio.h"\
	{$(INCLUDE)}"\DOWNHILL\DH_INC.H"\
	{$(INCLUDE)}"\DOWNHILL\DH_POSIX.H"\
	{$(INCLUDE)}"\f2c.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	{$(INCLUDE)}"\sys\unistd.h"\
	{$(INCLUDE)}"\unistd.h"\
	

"$(INTDIR)\OPEN.OBJ" : $(SOURCE) $(DEP_CPP_OPEN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RDFMT.C
DEP_CPP_RDFMT=\
	".\fio.h"\
	".\fmt.h"\
	".\fp.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\RDFMT.OBJ" : $(SOURCE) $(DEP_CPP_RDFMT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\REWIND.C
DEP_CPP_REWIN=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\REWIND.OBJ" : $(SOURCE) $(DEP_CPP_REWIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RSFE.C
DEP_CPP_RSFE_=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\RSFE.OBJ" : $(SOURCE) $(DEP_CPP_RSFE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RSLI.C
DEP_CPP_RSLI_=\
	".\fio.h"\
	".\fmt.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\RSLI.OBJ" : $(SOURCE) $(DEP_CPP_RSLI_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RSNE.C
DEP_CPP_RSNE_=\
	".\fio.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\RSNE.OBJ" : $(SOURCE) $(DEP_CPP_RSNE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SFE.C
DEP_CPP_SFE_C=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\SFE.OBJ" : $(SOURCE) $(DEP_CPP_SFE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SUE.C
DEP_CPP_SUE_C=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\SUE.OBJ" : $(SOURCE) $(DEP_CPP_SUE_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TYPESIZE.C
DEP_CPP_TYPES=\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\TYPESIZE.OBJ" : $(SOURCE) $(DEP_CPP_TYPES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UIO.C
DEP_CPP_UIO_C=\
	".\fio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\UIO.OBJ" : $(SOURCE) $(DEP_CPP_UIO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UTIL.C
DEP_CPP_UTIL_=\
	".\fio.h"\
	{$(INCLUDE)}"\DOWNHILL\DH_INC.H"\
	{$(INCLUDE)}"\DOWNHILL\DH_POSIX.H"\
	{$(INCLUDE)}"\f2c.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\UTIL.OBJ" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\VERSION.C

"$(INTDIR)\VERSION.OBJ" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WREF.C
DEP_CPP_WREF_=\
	".\fio.h"\
	".\fmt.h"\
	".\fp.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\WREF.OBJ" : $(SOURCE) $(DEP_CPP_WREF_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WRTFMT.C
DEP_CPP_WRTFM=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\WRTFMT.OBJ" : $(SOURCE) $(DEP_CPP_WRTFM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WSFE.C
DEP_CPP_WSFE_=\
	".\fio.h"\
	".\fmt.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\WSFE.OBJ" : $(SOURCE) $(DEP_CPP_WSFE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WSLE.C
DEP_CPP_WSLE_=\
	".\fio.h"\
	".\fmt.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\WSLE.OBJ" : $(SOURCE) $(DEP_CPP_WSLE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WSNE.C
DEP_CPP_WSNE_=\
	".\fio.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\WSNE.OBJ" : $(SOURCE) $(DEP_CPP_WSNE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\XWSNE.C
DEP_CPP_XWSNE=\
	".\fio.h"\
	".\fmt.h"\
	".\lio.h"\
	{$(INCLUDE)}"\f2c.h"\
	

"$(INTDIR)\XWSNE.OBJ" : $(SOURCE) $(DEP_CPP_XWSNE) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################

# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=F2C - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to F2C - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "F2C - Win32 Release" && "$(CFG)" != "F2C - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "F2C.MAK" CFG="F2C - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "F2C - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "F2C - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "F2C - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "F2C - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\F2C.exe" "$(OUTDIR)\F2C.bsc"

CLEAN : 
	-@erase ".\WinRel\F2C.bsc"
	-@erase ".\WinRel\EXPR.SBR"
	-@erase ".\WinRel\VERSION.SBR"
	-@erase ".\WinRel\IO.SBR"
	-@erase ".\WinRel\MAIN.SBR"
	-@erase ".\WinRel\INTR.SBR"
	-@erase ".\WinRel\NICEPRIN.SBR"
	-@erase ".\WinRel\SYSDEP.SBR"
	-@erase ".\WinRel\MISC.SBR"
	-@erase ".\WinRel\EXEC.SBR"
	-@erase ".\WinRel\PUT.SBR"
	-@erase ".\WinRel\DATA.SBR"
	-@erase ".\WinRel\P1OUTPUT.SBR"
	-@erase ".\WinRel\FORMAT.SBR"
	-@erase ".\WinRel\LEX.SBR"
	-@erase ".\WinRel\OUTPUT.SBR"
	-@erase ".\WinRel\PUTPCC.SBR"
	-@erase ".\WinRel\VAX.SBR"
	-@erase ".\WinRel\FORMATDA.SBR"
	-@erase ".\WinRel\PREAD.SBR"
	-@erase ".\WinRel\PROC.SBR"
	-@erase ".\WinRel\PARSE_AR.SBR"
	-@erase ".\WinRel\MEM.SBR"
	-@erase ".\WinRel\CDS.SBR"
	-@erase ".\WinRel\INIT.SBR"
	-@erase ".\WinRel\NAMES.SBR"
	-@erase ".\WinRel\EQUIV.SBR"
	-@erase ".\WinRel\ERROR.SBR"
	-@erase ".\WinRel\gram.sbr"
	-@erase ".\WinRel\F2C.exe"
	-@erase ".\WinRel\INIT.OBJ"
	-@erase ".\WinRel\NAMES.OBJ"
	-@erase ".\WinRel\EQUIV.OBJ"
	-@erase ".\WinRel\ERROR.OBJ"
	-@erase ".\WinRel\gram.obj"
	-@erase ".\WinRel\EXPR.OBJ"
	-@erase ".\WinRel\VERSION.OBJ"
	-@erase ".\WinRel\IO.OBJ"
	-@erase ".\WinRel\MAIN.OBJ"
	-@erase ".\WinRel\INTR.OBJ"
	-@erase ".\WinRel\NICEPRIN.OBJ"
	-@erase ".\WinRel\SYSDEP.OBJ"
	-@erase ".\WinRel\MISC.OBJ"
	-@erase ".\WinRel\EXEC.OBJ"
	-@erase ".\WinRel\PUT.OBJ"
	-@erase ".\WinRel\DATA.OBJ"
	-@erase ".\WinRel\P1OUTPUT.OBJ"
	-@erase ".\WinRel\FORMAT.OBJ"
	-@erase ".\WinRel\LEX.OBJ"
	-@erase ".\WinRel\OUTPUT.OBJ"
	-@erase ".\WinRel\PUTPCC.OBJ"
	-@erase ".\WinRel\VAX.OBJ"
	-@erase ".\WinRel\FORMATDA.OBJ"
	-@erase ".\WinRel\PREAD.OBJ"
	-@erase ".\WinRel\PROC.OBJ"
	-@erase ".\WinRel\PARSE_AR.OBJ"
	-@erase ".\WinRel\MEM.OBJ"
	-@erase ".\WinRel\CDS.OBJ"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_DOS" /D "MSDOS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__" /FR /YX " " /c
CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/F2C.pch" /YX /Fo"$(INTDIR)/" " " /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/F2C.bsc" 
BSC32_SBRS= \
	".\WinRel\EXPR.SBR" \
	".\WinRel\VERSION.SBR" \
	".\WinRel\IO.SBR" \
	".\WinRel\MAIN.SBR" \
	".\WinRel\INTR.SBR" \
	".\WinRel\NICEPRIN.SBR" \
	".\WinRel\SYSDEP.SBR" \
	".\WinRel\MISC.SBR" \
	".\WinRel\EXEC.SBR" \
	".\WinRel\PUT.SBR" \
	".\WinRel\DATA.SBR" \
	".\WinRel\P1OUTPUT.SBR" \
	".\WinRel\FORMAT.SBR" \
	".\WinRel\LEX.SBR" \
	".\WinRel\OUTPUT.SBR" \
	".\WinRel\PUTPCC.SBR" \
	".\WinRel\VAX.SBR" \
	".\WinRel\FORMATDA.SBR" \
	".\WinRel\PREAD.SBR" \
	".\WinRel\PROC.SBR" \
	".\WinRel\PARSE_AR.SBR" \
	".\WinRel\MEM.SBR" \
	".\WinRel\CDS.SBR" \
	".\WinRel\INIT.SBR" \
	".\WinRel\NAMES.SBR" \
	".\WinRel\EQUIV.SBR" \
	".\WinRel\ERROR.SBR" \
	".\WinRel\gram.sbr"

"$(OUTDIR)\F2C.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x8192 /subsystem:console /machine:IX86
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x8192 /subsystem:console /machine:IX86
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /stack:0x8192 /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/F2C.pdb" /machine:IX86 /out:"$(OUTDIR)/F2C.exe" 
LINK32_OBJS= \
	".\WinRel\INIT.OBJ" \
	".\WinRel\NAMES.OBJ" \
	".\WinRel\EQUIV.OBJ" \
	".\WinRel\ERROR.OBJ" \
	".\WinRel\gram.obj" \
	".\WinRel\EXPR.OBJ" \
	".\WinRel\VERSION.OBJ" \
	".\WinRel\IO.OBJ" \
	".\WinRel\MAIN.OBJ" \
	".\WinRel\INTR.OBJ" \
	".\WinRel\NICEPRIN.OBJ" \
	".\WinRel\SYSDEP.OBJ" \
	".\WinRel\MISC.OBJ" \
	".\WinRel\EXEC.OBJ" \
	".\WinRel\PUT.OBJ" \
	".\WinRel\DATA.OBJ" \
	".\WinRel\P1OUTPUT.OBJ" \
	".\WinRel\FORMAT.OBJ" \
	".\WinRel\LEX.OBJ" \
	".\WinRel\OUTPUT.OBJ" \
	".\WinRel\PUTPCC.OBJ" \
	".\WinRel\VAX.OBJ" \
	".\WinRel\FORMATDA.OBJ" \
	".\WinRel\PREAD.OBJ" \
	".\WinRel\PROC.OBJ" \
	".\WinRel\PARSE_AR.OBJ" \
	".\WinRel\MEM.OBJ" \
	".\WinRel\CDS.OBJ" \
	"..\..\DOWNHILL\DOWNHILL.LIB"

"$(OUTDIR)\F2C.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "F2C - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\F2C.exe" "$(OUTDIR)\F2C.bsc"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\F2C.bsc"
	-@erase ".\WinDebug\MAIN.SBR"
	-@erase ".\WinDebug\NAMES.SBR"
	-@erase ".\WinDebug\EQUIV.SBR"
	-@erase ".\WinDebug\ERROR.SBR"
	-@erase ".\WinDebug\LEX.SBR"
	-@erase ".\WinDebug\INIT.SBR"
	-@erase ".\WinDebug\VAX.SBR"
	-@erase ".\WinDebug\MEM.SBR"
	-@erase ".\WinDebug\gram.sbr"
	-@erase ".\WinDebug\NICEPRIN.SBR"
	-@erase ".\WinDebug\CDS.SBR"
	-@erase ".\WinDebug\FORMATDA.SBR"
	-@erase ".\WinDebug\EXPR.SBR"
	-@erase ".\WinDebug\DATA.SBR"
	-@erase ".\WinDebug\VERSION.SBR"
	-@erase ".\WinDebug\SYSDEP.SBR"
	-@erase ".\WinDebug\INTR.SBR"
	-@erase ".\WinDebug\IO.SBR"
	-@erase ".\WinDebug\MISC.SBR"
	-@erase ".\WinDebug\PROC.SBR"
	-@erase ".\WinDebug\P1OUTPUT.SBR"
	-@erase ".\WinDebug\FORMAT.SBR"
	-@erase ".\WinDebug\EXEC.SBR"
	-@erase ".\WinDebug\OUTPUT.SBR"
	-@erase ".\WinDebug\PUTPCC.SBR"
	-@erase ".\WinDebug\PARSE_AR.SBR"
	-@erase ".\WinDebug\PREAD.SBR"
	-@erase ".\WinDebug\PUT.SBR"
	-@erase ".\WinDebug\F2C.exe"
	-@erase ".\WinDebug\PUTPCC.OBJ"
	-@erase ".\WinDebug\PARSE_AR.OBJ"
	-@erase ".\WinDebug\PREAD.OBJ"
	-@erase ".\WinDebug\PUT.OBJ"
	-@erase ".\WinDebug\MAIN.OBJ"
	-@erase ".\WinDebug\NAMES.OBJ"
	-@erase ".\WinDebug\EQUIV.OBJ"
	-@erase ".\WinDebug\ERROR.OBJ"
	-@erase ".\WinDebug\LEX.OBJ"
	-@erase ".\WinDebug\INIT.OBJ"
	-@erase ".\WinDebug\VAX.OBJ"
	-@erase ".\WinDebug\MEM.OBJ"
	-@erase ".\WinDebug\gram.obj"
	-@erase ".\WinDebug\NICEPRIN.OBJ"
	-@erase ".\WinDebug\CDS.OBJ"
	-@erase ".\WinDebug\FORMATDA.OBJ"
	-@erase ".\WinDebug\EXPR.OBJ"
	-@erase ".\WinDebug\DATA.OBJ"
	-@erase ".\WinDebug\VERSION.OBJ"
	-@erase ".\WinDebug\SYSDEP.OBJ"
	-@erase ".\WinDebug\INTR.OBJ"
	-@erase ".\WinDebug\IO.OBJ"
	-@erase ".\WinDebug\MISC.OBJ"
	-@erase ".\WinDebug\PROC.OBJ"
	-@erase ".\WinDebug\P1OUTPUT.OBJ"
	-@erase ".\WinDebug\FORMAT.OBJ"
	-@erase ".\WinDebug\EXEC.OBJ"
	-@erase ".\WinDebug\OUTPUT.OBJ"
	-@erase ".\WinDebug\F2C.ilk"
	-@erase ".\WinDebug\F2C.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_DOS" /D "MSDOS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__" /FR /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D\
 "_CONSOLE" /D "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/F2C.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/F2C.bsc" 
BSC32_SBRS= \
	".\WinDebug\MAIN.SBR" \
	".\WinDebug\NAMES.SBR" \
	".\WinDebug\EQUIV.SBR" \
	".\WinDebug\ERROR.SBR" \
	".\WinDebug\LEX.SBR" \
	".\WinDebug\INIT.SBR" \
	".\WinDebug\VAX.SBR" \
	".\WinDebug\MEM.SBR" \
	".\WinDebug\gram.sbr" \
	".\WinDebug\NICEPRIN.SBR" \
	".\WinDebug\CDS.SBR" \
	".\WinDebug\FORMATDA.SBR" \
	".\WinDebug\EXPR.SBR" \
	".\WinDebug\DATA.SBR" \
	".\WinDebug\VERSION.SBR" \
	".\WinDebug\SYSDEP.SBR" \
	".\WinDebug\INTR.SBR" \
	".\WinDebug\IO.SBR" \
	".\WinDebug\MISC.SBR" \
	".\WinDebug\PROC.SBR" \
	".\WinDebug\P1OUTPUT.SBR" \
	".\WinDebug\FORMAT.SBR" \
	".\WinDebug\EXEC.SBR" \
	".\WinDebug\OUTPUT.SBR" \
	".\WinDebug\PUTPCC.SBR" \
	".\WinDebug\PARSE_AR.SBR" \
	".\WinDebug\PREAD.SBR" \
	".\WinDebug\PUT.SBR"

"$(OUTDIR)\F2C.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x8192 /subsystem:console /debug /machine:IX86
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x8192 /subsystem:console /debug /machine:IX86
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /stack:0x8192 /subsystem:console\
 /incremental:yes /pdb:"$(OUTDIR)/F2C.pdb" /debug /machine:IX86\
 /out:"$(OUTDIR)/F2C.exe" 
LINK32_OBJS= \
	".\WinDebug\PUTPCC.OBJ" \
	".\WinDebug\PARSE_AR.OBJ" \
	".\WinDebug\PREAD.OBJ" \
	".\WinDebug\PUT.OBJ" \
	".\WinDebug\MAIN.OBJ" \
	".\WinDebug\NAMES.OBJ" \
	".\WinDebug\EQUIV.OBJ" \
	".\WinDebug\ERROR.OBJ" \
	".\WinDebug\LEX.OBJ" \
	".\WinDebug\INIT.OBJ" \
	".\WinDebug\VAX.OBJ" \
	".\WinDebug\MEM.OBJ" \
	".\WinDebug\gram.obj" \
	".\WinDebug\NICEPRIN.OBJ" \
	".\WinDebug\CDS.OBJ" \
	".\WinDebug\FORMATDA.OBJ" \
	".\WinDebug\EXPR.OBJ" \
	".\WinDebug\DATA.OBJ" \
	".\WinDebug\VERSION.OBJ" \
	".\WinDebug\SYSDEP.OBJ" \
	".\WinDebug\INTR.OBJ" \
	".\WinDebug\IO.OBJ" \
	".\WinDebug\MISC.OBJ" \
	".\WinDebug\PROC.OBJ" \
	".\WinDebug\P1OUTPUT.OBJ" \
	".\WinDebug\FORMAT.OBJ" \
	".\WinDebug\EXEC.OBJ" \
	".\WinDebug\OUTPUT.OBJ" \
	"..\..\DOWNHILL\DOWNHILL.LIB"

"$(OUTDIR)\F2C.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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

# Name "F2C - Win32 Release"
# Name "F2C - Win32 Debug"

!IF  "$(CFG)" == "F2C - Win32 Release"

!ELSEIF  "$(CFG)" == "F2C - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\EXPR.C
DEP_CPP_EXPR_=\
	".\defs.h"\
	".\output.h"\
	".\names.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\EXPR.OBJ" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"

"$(INTDIR)\EXPR.SBR" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FORMAT.C
DEP_CPP_FORMA=\
	".\defs.h"\
	".\p1defs.h"\
	".\format.h"\
	".\output.h"\
	".\names.h"\
	".\iob.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\FORMAT.OBJ" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"

"$(INTDIR)\FORMAT.SBR" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FORMATDA.C
DEP_CPP_FORMAT=\
	".\defs.h"\
	".\output.h"\
	".\names.h"\
	".\format.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\FORMATDA.OBJ" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"

"$(INTDIR)\FORMATDA.SBR" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gram.c
DEP_CPP_GRAM_=\
	".\defs.h"\
	".\p1defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	
NODEP_CPP_GRAM_=\
	".\y.debug"\
	

"$(INTDIR)\gram.obj" : $(SOURCE) $(DEP_CPP_GRAM_) "$(INTDIR)"

"$(INTDIR)\gram.sbr" : $(SOURCE) $(DEP_CPP_GRAM_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INIT.C
DEP_CPP_INIT_=\
	".\defs.h"\
	".\output.h"\
	".\iob.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\INIT.OBJ" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

"$(INTDIR)\INIT.SBR" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\INTR.C
DEP_CPP_INTR_=\
	".\defs.h"\
	".\names.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\INTR.OBJ" : $(SOURCE) $(DEP_CPP_INTR_) "$(INTDIR)"

"$(INTDIR)\INTR.SBR" : $(SOURCE) $(DEP_CPP_INTR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO.C
DEP_CPP_IO_C7=\
	".\defs.h"\
	".\names.h"\
	".\iob.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\IO.OBJ" : $(SOURCE) $(DEP_CPP_IO_C7) "$(INTDIR)"

"$(INTDIR)\IO.SBR" : $(SOURCE) $(DEP_CPP_IO_C7) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LEX.C
DEP_CPP_LEX_C=\
	".\defs.h"\
	".\tokdefs.h"\
	".\p1defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\LEX.OBJ" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"

"$(INTDIR)\LEX.SBR" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MAIN.C
DEP_CPP_MAIN_=\
	".\defs.h"\
	".\parse.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"

"$(INTDIR)\MAIN.SBR" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MEM.C
DEP_CPP_MEM_C=\
	".\defs.h"\
	".\iob.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\MEM.OBJ" : $(SOURCE) $(DEP_CPP_MEM_C) "$(INTDIR)"

"$(INTDIR)\MEM.SBR" : $(SOURCE) $(DEP_CPP_MEM_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MISC.C
DEP_CPP_MISC_=\
	".\defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\MISC.OBJ" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"

"$(INTDIR)\MISC.SBR" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NAMES.C
DEP_CPP_NAMES=\
	".\defs.h"\
	".\output.h"\
	".\names.h"\
	".\iob.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\NAMES.OBJ" : $(SOURCE) $(DEP_CPP_NAMES) "$(INTDIR)"

"$(INTDIR)\NAMES.SBR" : $(SOURCE) $(DEP_CPP_NAMES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NICEPRIN.C
DEP_CPP_NICEP=\
	".\defs.h"\
	".\names.h"\
	".\output.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\NICEPRIN.OBJ" : $(SOURCE) $(DEP_CPP_NICEP) "$(INTDIR)"

"$(INTDIR)\NICEPRIN.SBR" : $(SOURCE) $(DEP_CPP_NICEP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OUTPUT.C
DEP_CPP_OUTPU=\
	".\defs.h"\
	".\names.h"\
	".\output.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\OUTPUT.OBJ" : $(SOURCE) $(DEP_CPP_OUTPU) "$(INTDIR)"

"$(INTDIR)\OUTPUT.SBR" : $(SOURCE) $(DEP_CPP_OUTPU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\P1OUTPUT.C
DEP_CPP_P1OUT=\
	".\defs.h"\
	".\p1defs.h"\
	".\output.h"\
	".\names.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\P1OUTPUT.OBJ" : $(SOURCE) $(DEP_CPP_P1OUT) "$(INTDIR)"

"$(INTDIR)\P1OUTPUT.SBR" : $(SOURCE) $(DEP_CPP_P1OUT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PARSE_AR.C
DEP_CPP_PARSE=\
	".\parse.h"\
	

"$(INTDIR)\PARSE_AR.OBJ" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"

"$(INTDIR)\PARSE_AR.SBR" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PREAD.C
DEP_CPP_PREAD=\
	".\defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\PREAD.OBJ" : $(SOURCE) $(DEP_CPP_PREAD) "$(INTDIR)"

"$(INTDIR)\PREAD.SBR" : $(SOURCE) $(DEP_CPP_PREAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PROC.C
DEP_CPP_PROC_=\
	".\defs.h"\
	".\names.h"\
	".\output.h"\
	".\p1defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\PROC.OBJ" : $(SOURCE) $(DEP_CPP_PROC_) "$(INTDIR)"

"$(INTDIR)\PROC.SBR" : $(SOURCE) $(DEP_CPP_PROC_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PUT.C
DEP_CPP_PUT_C=\
	".\defs.h"\
	".\names.h"\
	".\pccdefs.h"\
	".\p1defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\PUT.OBJ" : $(SOURCE) $(DEP_CPP_PUT_C) "$(INTDIR)"

"$(INTDIR)\PUT.SBR" : $(SOURCE) $(DEP_CPP_PUT_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PUTPCC.C
DEP_CPP_PUTPC=\
	".\defs.h"\
	".\pccdefs.h"\
	".\output.h"\
	".\names.h"\
	".\p1defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\PUTPCC.OBJ" : $(SOURCE) $(DEP_CPP_PUTPC) "$(INTDIR)"

"$(INTDIR)\PUTPCC.SBR" : $(SOURCE) $(DEP_CPP_PUTPC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SYSDEP.C
DEP_CPP_SYSDE=\
	".\defs.h"\
	".\usignal.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\SYSDEP.OBJ" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"

"$(INTDIR)\SYSDEP.SBR" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\VERSION.C

"$(INTDIR)\VERSION.OBJ" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\VERSION.SBR" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CDS.C
DEP_CPP_CDS_C=\
	".\sysdep.h"\
	

"$(INTDIR)\CDS.OBJ" : $(SOURCE) $(DEP_CPP_CDS_C) "$(INTDIR)"

"$(INTDIR)\CDS.SBR" : $(SOURCE) $(DEP_CPP_CDS_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DATA.C
DEP_CPP_DATA_=\
	".\defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\DATA.OBJ" : $(SOURCE) $(DEP_CPP_DATA_) "$(INTDIR)"

"$(INTDIR)\DATA.SBR" : $(SOURCE) $(DEP_CPP_DATA_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EQUIV.C
DEP_CPP_EQUIV=\
	".\defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\EQUIV.OBJ" : $(SOURCE) $(DEP_CPP_EQUIV) "$(INTDIR)"

"$(INTDIR)\EQUIV.SBR" : $(SOURCE) $(DEP_CPP_EQUIV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ERROR.C
DEP_CPP_ERROR=\
	".\defs.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\ERROR.OBJ" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"

"$(INTDIR)\ERROR.SBR" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EXEC.C
DEP_CPP_EXEC_=\
	".\defs.h"\
	".\p1defs.h"\
	".\names.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	

"$(INTDIR)\EXEC.OBJ" : $(SOURCE) $(DEP_CPP_EXEC_) "$(INTDIR)"

"$(INTDIR)\EXEC.SBR" : $(SOURCE) $(DEP_CPP_EXEC_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\VAX.C
DEP_CPP_VAX_C=\
	".\defs.h"\
	".\pccdefs.h"\
	".\output.h"\
	".\sysdep.h"\
	".\ftypes.h"\
	".\defines.h"\
	".\machdefs.h"\
	".\niceprintf.h"\
	

"$(INTDIR)\VAX.OBJ" : $(SOURCE) $(DEP_CPP_VAX_C) "$(INTDIR)"

"$(INTDIR)\VAX.SBR" : $(SOURCE) $(DEP_CPP_VAX_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\MSDEV\Projects\DOWNHILL\DOWNHILL.LIB

!IF  "$(CFG)" == "F2C - Win32 Release"

!ELSEIF  "$(CFG)" == "F2C - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

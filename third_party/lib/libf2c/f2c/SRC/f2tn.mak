# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "f2tn.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : $(OUTDIR)/f2tn.exe $(OUTDIR)/f2tn.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE CPP /nologo /ML /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /c
# ADD CPP /nologo /ML /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /c
CPP_PROJ=/nologo /ML /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"f2tn.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"f2tn.bsc" 
BSC32_SBRS= \
	$(INTDIR)/EQUIV.SBR \
	$(INTDIR)/INIT.SBR \
	$(INTDIR)/XSUM.SBR \
	$(INTDIR)/VAX.SBR \
	$(INTDIR)/FORMAT.SBR \
	$(INTDIR)/LEX.SBR \
	$(INTDIR)/GRAM.SBR \
	$(INTDIR)/EXPR.SBR \
	$(INTDIR)/PREAD.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/IO.SBR \
	$(INTDIR)/MALLOC.SBR \
	$(INTDIR)/EXEC.SBR \
	$(INTDIR)/OUTPUT.SBR \
	$(INTDIR)/P1OUTPUT.SBR \
	$(INTDIR)/CDS.SBR \
	$(INTDIR)/MEM.SBR \
	$(INTDIR)/PROC.SBR \
	$(INTDIR)/ERROR.SBR \
	$(INTDIR)/MISC.SBR \
	$(INTDIR)/PARSE_AR.SBR \
	$(INTDIR)/SYSDEP.SBR \
	$(INTDIR)/NAMES.SBR \
	$(INTDIR)/MEMSET.SBR \
	$(INTDIR)/VERSION.SBR \
	$(INTDIR)/DATA.SBR \
	$(INTDIR)/PUTPCC.SBR \
	$(INTDIR)/INTR.SBR \
	$(INTDIR)/NICEPRIN.SBR \
	$(INTDIR)/PUT.SBR \
	$(INTDIR)/FORMATDA.SBR

$(OUTDIR)/f2tn.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:console /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:console /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /NOLOGO /SUBSYSTEM:console /INCREMENTAL:no\
 /PDB:$(OUTDIR)/"f2tn.pdb" /MACHINE:I386 /OUT:$(OUTDIR)/"f2tn.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/EQUIV.OBJ \
	$(INTDIR)/INIT.OBJ \
	$(INTDIR)/XSUM.OBJ \
	$(INTDIR)/VAX.OBJ \
	$(INTDIR)/FORMAT.OBJ \
	$(INTDIR)/LEX.OBJ \
	$(INTDIR)/GRAM.OBJ \
	$(INTDIR)/EXPR.OBJ \
	$(INTDIR)/PREAD.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/IO.OBJ \
	$(INTDIR)/MALLOC.OBJ \
	$(INTDIR)/EXEC.OBJ \
	$(INTDIR)/OUTPUT.OBJ \
	$(INTDIR)/P1OUTPUT.OBJ \
	$(INTDIR)/CDS.OBJ \
	$(INTDIR)/MEM.OBJ \
	$(INTDIR)/PROC.OBJ \
	$(INTDIR)/ERROR.OBJ \
	$(INTDIR)/MISC.OBJ \
	$(INTDIR)/PARSE_AR.OBJ \
	$(INTDIR)/SYSDEP.OBJ \
	$(INTDIR)/NAMES.OBJ \
	$(INTDIR)/MEMSET.OBJ \
	$(INTDIR)/VERSION.OBJ \
	$(INTDIR)/DATA.OBJ \
	$(INTDIR)/PUTPCC.OBJ \
	$(INTDIR)/INTR.OBJ \
	$(INTDIR)/NICEPRIN.OBJ \
	$(INTDIR)/PUT.OBJ \
	$(INTDIR)/FORMATDA.OBJ

$(OUTDIR)/f2tn.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : $(OUTDIR)/f2tn.exe $(OUTDIR)/f2tn.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE CPP /nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /c
# ADD CPP /nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /c
CPP_PROJ=/nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"f2tn.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"f2tn.pdb" /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"f2tn.bsc" 
BSC32_SBRS= \
	$(INTDIR)/EQUIV.SBR \
	$(INTDIR)/INIT.SBR \
	$(INTDIR)/XSUM.SBR \
	$(INTDIR)/VAX.SBR \
	$(INTDIR)/FORMAT.SBR \
	$(INTDIR)/LEX.SBR \
	$(INTDIR)/GRAM.SBR \
	$(INTDIR)/EXPR.SBR \
	$(INTDIR)/PREAD.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/IO.SBR \
	$(INTDIR)/MALLOC.SBR \
	$(INTDIR)/EXEC.SBR \
	$(INTDIR)/OUTPUT.SBR \
	$(INTDIR)/P1OUTPUT.SBR \
	$(INTDIR)/CDS.SBR \
	$(INTDIR)/MEM.SBR \
	$(INTDIR)/PROC.SBR \
	$(INTDIR)/ERROR.SBR \
	$(INTDIR)/MISC.SBR \
	$(INTDIR)/PARSE_AR.SBR \
	$(INTDIR)/SYSDEP.SBR \
	$(INTDIR)/NAMES.SBR \
	$(INTDIR)/MEMSET.SBR \
	$(INTDIR)/VERSION.SBR \
	$(INTDIR)/DATA.SBR \
	$(INTDIR)/PUTPCC.SBR \
	$(INTDIR)/INTR.SBR \
	$(INTDIR)/NICEPRIN.SBR \
	$(INTDIR)/PUT.SBR \
	$(INTDIR)/FORMATDA.SBR

$(OUTDIR)/f2tn.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:console /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:console /DEBUG /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /NOLOGO /SUBSYSTEM:console /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"f2tn.pdb" /DEBUG /MACHINE:I386 /OUT:$(OUTDIR)/"f2tn.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/EQUIV.OBJ \
	$(INTDIR)/INIT.OBJ \
	$(INTDIR)/XSUM.OBJ \
	$(INTDIR)/VAX.OBJ \
	$(INTDIR)/FORMAT.OBJ \
	$(INTDIR)/LEX.OBJ \
	$(INTDIR)/GRAM.OBJ \
	$(INTDIR)/EXPR.OBJ \
	$(INTDIR)/PREAD.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/IO.OBJ \
	$(INTDIR)/MALLOC.OBJ \
	$(INTDIR)/EXEC.OBJ \
	$(INTDIR)/OUTPUT.OBJ \
	$(INTDIR)/P1OUTPUT.OBJ \
	$(INTDIR)/CDS.OBJ \
	$(INTDIR)/MEM.OBJ \
	$(INTDIR)/PROC.OBJ \
	$(INTDIR)/ERROR.OBJ \
	$(INTDIR)/MISC.OBJ \
	$(INTDIR)/PARSE_AR.OBJ \
	$(INTDIR)/SYSDEP.OBJ \
	$(INTDIR)/NAMES.OBJ \
	$(INTDIR)/MEMSET.OBJ \
	$(INTDIR)/VERSION.OBJ \
	$(INTDIR)/DATA.OBJ \
	$(INTDIR)/PUTPCC.OBJ \
	$(INTDIR)/INTR.OBJ \
	$(INTDIR)/NICEPRIN.OBJ \
	$(INTDIR)/PUT.OBJ \
	$(INTDIR)/FORMATDA.OBJ

$(OUTDIR)/f2tn.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\EQUIV.C

$(INTDIR)/EQUIV.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\INIT.C

$(INTDIR)/INIT.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XSUM.C

$(INTDIR)/XSUM.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\VAX.C

$(INTDIR)/VAX.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FORMAT.C

$(INTDIR)/FORMAT.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LEX.C

$(INTDIR)/LEX.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GRAM.C

$(INTDIR)/GRAM.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EXPR.C

$(INTDIR)/EXPR.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PREAD.C

$(INTDIR)/PREAD.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MAIN.C

$(INTDIR)/MAIN.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO.C

$(INTDIR)/IO.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MALLOC.C

$(INTDIR)/MALLOC.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EXEC.C

$(INTDIR)/EXEC.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OUTPUT.C

$(INTDIR)/OUTPUT.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\P1OUTPUT.C

$(INTDIR)/P1OUTPUT.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CDS.C

$(INTDIR)/CDS.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MEM.C

$(INTDIR)/MEM.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PROC.C

$(INTDIR)/PROC.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ERROR.C

$(INTDIR)/ERROR.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MISC.C

$(INTDIR)/MISC.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PARSE_AR.C

$(INTDIR)/PARSE_AR.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SYSDEP.C

$(INTDIR)/SYSDEP.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NAMES.C

$(INTDIR)/NAMES.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MEMSET.C

$(INTDIR)/MEMSET.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\VERSION.C

$(INTDIR)/VERSION.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DATA.C

$(INTDIR)/DATA.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PUTPCC.C

$(INTDIR)/PUTPCC.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\INTR.C

$(INTDIR)/INTR.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NICEPRIN.C

$(INTDIR)/NICEPRIN.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PUT.C

$(INTDIR)/PUT.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FORMATDA.C

$(INTDIR)/FORMATDA.OBJ :  $(SOURCE)  $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################

# Microsoft Developer Studio Project File - Name="F2C" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=F2C - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "f2c.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "f2c.mak" CFG="F2C - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "F2C - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "F2C - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "F2C - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_DOS" /D "MSDOS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__" /FR /YX /FD " " /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x8192 /subsystem:console /machine:IX86
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x8192 /subsystem:console /machine:IX86

!ELSEIF  "$(CFG)" == "F2C - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# ADD BASE F90 /include:".\WinDebug/"
# ADD F90 /include:".\WinDebug/"
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_DOS" /D "MSDOS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ANSI_Libraries" /D "ANSI_Prototypes" /D "__NT_VC__" /FR /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x8192 /subsystem:console /debug /machine:IX86
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x8192 /subsystem:console /debug /machine:IX86

!ENDIF 

# Begin Target

# Name "F2C - Win32 Release"
# Name "F2C - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\CDS.C
# End Source File
# Begin Source File

SOURCE=.\DATA.C
# End Source File
# Begin Source File

SOURCE=.\EQUIV.C
# End Source File
# Begin Source File

SOURCE=.\ERROR.C
# End Source File
# Begin Source File

SOURCE=.\EXEC.C
# End Source File
# Begin Source File

SOURCE=.\EXPR.C
# End Source File
# Begin Source File

SOURCE=.\FORMAT.C
# End Source File
# Begin Source File

SOURCE=.\FORMATDA.C
# End Source File
# Begin Source File

SOURCE=.\gram.c
# End Source File
# Begin Source File

SOURCE=.\INIT.C
# End Source File
# Begin Source File

SOURCE=.\INTR.C
# End Source File
# Begin Source File

SOURCE=.\IO.C
# End Source File
# Begin Source File

SOURCE=.\LEX.C
# End Source File
# Begin Source File

SOURCE=.\MAIN.C
# End Source File
# Begin Source File

SOURCE=.\MEM.C
# End Source File
# Begin Source File

SOURCE=.\MISC.C
# End Source File
# Begin Source File

SOURCE=.\NAMES.C
# End Source File
# Begin Source File

SOURCE=.\NICEPRIN.C
# End Source File
# Begin Source File

SOURCE=.\OUTPUT.C
# End Source File
# Begin Source File

SOURCE=.\P1OUTPUT.C
# End Source File
# Begin Source File

SOURCE=.\PARSE_AR.C
# End Source File
# Begin Source File

SOURCE=.\PREAD.C
# End Source File
# Begin Source File

SOURCE=.\PROC.C
# End Source File
# Begin Source File

SOURCE=.\PUT.C
# End Source File
# Begin Source File

SOURCE=.\PUTPCC.C
# End Source File
# Begin Source File

SOURCE=.\SYSDEP.C
# End Source File
# Begin Source File

SOURCE=.\VAX.C
# End Source File
# Begin Source File

SOURCE=.\VERSION.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\defs.h
# End Source File
# Begin Source File

SOURCE=.\format.h
# End Source File
# Begin Source File

SOURCE=.\ftypes.h
# End Source File
# Begin Source File

SOURCE=.\iob.h
# End Source File
# Begin Source File

SOURCE=.\machdefs.h
# End Source File
# Begin Source File

SOURCE=.\names.h
# End Source File
# Begin Source File

SOURCE=.\niceprintf.h
# End Source File
# Begin Source File

SOURCE=.\output.h
# End Source File
# Begin Source File

SOURCE=.\p1defs.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=.\pccdefs.h
# End Source File
# Begin Source File

SOURCE=.\sysdep.h
# End Source File
# Begin Source File

SOURCE=.\tokdefs.h
# End Source File
# Begin Source File

SOURCE=.\usignal.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

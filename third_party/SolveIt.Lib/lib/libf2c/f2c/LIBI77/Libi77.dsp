# Microsoft Developer Studio Project File - Name="LIBI77" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LIBI77 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Libi77.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Libi77.mak" CFG="LIBI77 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LIBI77 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LIBI77 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "LIBI77 - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# ADD BASE F90 /include:".\WinRel/"
# ADD F90 /include:".\WinRel/"
# ADD BASE CPP /nologo /G3 /MT /W3 /GX /Ox /Ot /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /Ox /Ot /Ob2 /I ".." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NON_UNIX_STDIO" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LIBI77 - Win32 Debug"

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
# ADD BASE CPP /nologo /MT /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NON_UNIX_STDIO" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\WinDebug\Libi77D.lib"

!ENDIF 

# Begin Target

# Name "LIBI77 - Win32 Release"
# Name "LIBI77 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\BACKSPAC.C
# End Source File
# Begin Source File

SOURCE=.\CLOSE.C
# End Source File
# Begin Source File

SOURCE=.\DFE.C
# End Source File
# Begin Source File

SOURCE=.\DOLIO.C
# End Source File
# Begin Source File

SOURCE=.\DUE.C
# End Source File
# Begin Source File

SOURCE=.\ENDFILE.C
# End Source File
# Begin Source File

SOURCE=.\ERR.C
# End Source File
# Begin Source File

SOURCE=.\FMT.C
# End Source File
# Begin Source File

SOURCE=.\FMTLIB.C
# End Source File
# Begin Source File

SOURCE=.\IIO.C
# End Source File
# Begin Source File

SOURCE=.\ILNW.C
# End Source File
# Begin Source File

SOURCE=.\INQUIRE.C
# End Source File
# Begin Source File

SOURCE=.\LREAD.C
# End Source File
# Begin Source File

SOURCE=.\LWRITE.C
# End Source File
# Begin Source File

SOURCE=.\OPEN.C
# End Source File
# Begin Source File

SOURCE=.\RDFMT.C
# End Source File
# Begin Source File

SOURCE=.\REWIND.C
# End Source File
# Begin Source File

SOURCE=.\RSFE.C
# End Source File
# Begin Source File

SOURCE=.\RSLI.C
# End Source File
# Begin Source File

SOURCE=.\RSNE.C
# End Source File
# Begin Source File

SOURCE=.\SFE.C
# End Source File
# Begin Source File

SOURCE=.\SUE.C
# End Source File
# Begin Source File

SOURCE=.\TYPESIZE.C
# End Source File
# Begin Source File

SOURCE=.\UIO.C
# End Source File
# Begin Source File

SOURCE=.\UTIL.C
# End Source File
# Begin Source File

SOURCE=.\VERSION.C
# End Source File
# Begin Source File

SOURCE=.\WREF.C
# End Source File
# Begin Source File

SOURCE=.\WRTFMT.C
# End Source File
# Begin Source File

SOURCE=.\WSFE.C
# End Source File
# Begin Source File

SOURCE=.\WSLE.C
# End Source File
# Begin Source File

SOURCE=.\WSNE.C
# End Source File
# Begin Source File

SOURCE=.\XWSNE.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\fio.h
# End Source File
# Begin Source File

SOURCE=.\fmt.h
# End Source File
# Begin Source File

SOURCE=.\fp.h
# End Source File
# Begin Source File

SOURCE=.\lio.h
# End Source File
# Begin Source File

SOURCE=.\rawio.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

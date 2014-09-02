# Microsoft Developer Studio Generated NMAKE File, Based on ShellHook.dsp
!IF "$(CFG)" == ""
CFG=ShellHook - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ShellHook - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ShellHook - Win32 Release" && "$(CFG)" != "ShellHook - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ShellHook.mak" CFG="ShellHook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ShellHook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ShellHook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ShellHook - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ShellHook.dll"


CLEAN :
	-@erase "$(INTDIR)\ShellHook.obj"
	-@erase "$(INTDIR)\ShellHook.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ShellHook.dll"
	-@erase "$(OUTDIR)\ShellHook.exp"
	-@erase "$(OUTDIR)\ShellHook.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHELLHOOK_EXPORTS" /Fp"$(INTDIR)\ShellHook.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ShellHook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ShellHook.pdb" /machine:I386 /out:"$(OUTDIR)\ShellHook.dll" /implib:"$(OUTDIR)\ShellHook.lib" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ShellHook.obj"

"$(OUTDIR)\ShellHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ShellHook - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ShellHook.dll" "$(OUTDIR)\ShellHook.bsc"


CLEAN :
	-@erase "$(INTDIR)\ShellHook.obj"
	-@erase "$(INTDIR)\ShellHook.pch"
	-@erase "$(INTDIR)\ShellHook.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ShellHook.bsc"
	-@erase "$(OUTDIR)\ShellHook.dll"
	-@erase "$(OUTDIR)\ShellHook.exp"
	-@erase "$(OUTDIR)\ShellHook.ilk"
	-@erase "$(OUTDIR)\ShellHook.lib"
	-@erase "$(OUTDIR)\ShellHook.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHELLHOOK_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ShellHook.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ShellHook.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ShellHook.sbr"

"$(OUTDIR)\ShellHook.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:yes /pdb:"$(OUTDIR)\ShellHook.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ShellHook.dll" /implib:"$(OUTDIR)\ShellHook.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ShellHook.obj"

"$(OUTDIR)\ShellHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ShellHook.dep")
!INCLUDE "ShellHook.dep"
!ELSE 
!MESSAGE Warning: cannot find "ShellHook.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ShellHook - Win32 Release" || "$(CFG)" == "ShellHook - Win32 Debug"
SOURCE=.\ShellHook.cpp

!IF  "$(CFG)" == "ShellHook - Win32 Release"


"$(INTDIR)\ShellHook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ShellHook.pch"


!ELSEIF  "$(CFG)" == "ShellHook - Win32 Debug"


"$(INTDIR)\ShellHook.obj"	"$(INTDIR)\ShellHook.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ShellHook.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ShellHook - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHELLHOOK_EXPORTS" /Fp"$(INTDIR)\ShellHook.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ShellHook.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ShellHook - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHELLHOOK_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ShellHook.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ShellHook.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


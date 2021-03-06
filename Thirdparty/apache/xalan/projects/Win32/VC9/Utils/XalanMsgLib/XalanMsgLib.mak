# Microsoft Developer Studio Generated NMAKE File, Based on XalanMsgLib.dsp
!IF "$(CFG)" == ""
CFG=XalanMsgLib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to XalanMsgLib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "XalanMsgLib - Win32 Release" && "$(CFG)" != "XalanMsgLib - Win32 Debug" && "$(CFG)" != "XalanMsgLib - Win32 Release.symbols" && "$(CFG)" != "XalanMsgLib - Win64 Release" && "$(CFG)" != "XalanMsgLib - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XalanMsgLib.mak" CFG="XalanMsgLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XalanMsgLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win32 Release.symbols" (based on "Win64 (x86) Console Application")
!MESSAGE "XalanMsgLib - Win64 Release" (based on "Win64 (x86) Console Application")
!MESSAGE "XalanMsgLib - Win64 Debug" (based on "Win64 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

NO_EXTERNAL_DEPS=1

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IFNDEF DLLNAME
!	ERROR Name of dll should be provided in the command line.
!ENDIF

!IF  "$(CFG)" == "XalanMsgLib - Win32 Release.symbols"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC9\Release.symbols\Util\XalanMsgLib
INTDIR=.\..\..\..\..\..\Build\Win32\VC9\Release.symbols\Util\XalanMsgLib

ALL : "..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC90.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).dll"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).exp"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).lib"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).pdb"	
	-@erase "$(INTDIR)\VC90.pdb"
	-@erase "$(INTDIR)\VC90.idb"	
"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /EHsc /Zi /O2 /I $(XERCESCROOT)\src /I "..\..\..\..\..\src" /I ".\..\..\..\..\..\Build\Win32\VC9\Release.symbols\Nls\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "_CRT_SECURE_NO_WARNINGS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(DLLNAME).bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS= /nologo /dll /incremental:no /pdb:"$(OUTDIR)\XalanMsgLib.pdb" /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).dll" /implib:"..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).lib" 
LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj"

"..\..\..\..\..\Build\Win32\VC9\Release.symbols\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Release"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC9\Release\Util\XalanMsgLib
INTDIR=.\..\..\..\..\..\Build\Win32\VC9\Release\Util\XalanMsgLib

ALL : "..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC90.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).dll"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).exp"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /EHsc /O2 /I $(XERCESCROOT)\src /I "..\..\..\..\..\src" /I ".\..\..\..\..\..\Build\Win32\VC9\Release\Nls\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "_CRT_SECURE_NO_WARNINGS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(DLLNAME).bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:none /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).dll" /implib:"..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).lib" 
LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj"

"..\..\..\..\..\Build\Win32\VC9\Release\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC9\Debug\Util\XalanMsgLib
INTDIR=.\..\..\..\..\..\Build\Win32\VC9\Debug\Util\XalanMsgLib
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win32\VC9\Debug\Util\XalanMsgLib
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win32\VC9\Debug\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC90.idb"
	-@erase "$(INTDIR)\VC90.pdb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\XalanMsgLib.sbr"
	-@erase "$(OUTDIR)\XalanMsgLib.bsc"
	-@erase "$(OUTDIR)\XalanMsgLib.exp"
	-@erase "$(OUTDIR)\$(DLLNAME).lib"
	-@erase "..\..\..\..\..\Build\Win32\VC9\Debug\$(DLLNAME).dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /ZI /Od /I $(XERCESCROOT)\src /I "..\..\..\..\..\src" /I ".\..\..\..\..\..\Build\Win32\VC9\Debug\Nls\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /RTC1 /c 
MTL_PROJ=/D "_DEBUG" /win32 

LINK32=link.exe
LINK32_FLAGS= /nologo /dll /pdb:none /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC9\Debug\$(DLLNAME).dll" /implib:"..\..\..\..\..\Build\Win32\VC9\Debug\$(DLLNAME).lib" 
LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj"

"..\..\..\..\..\Build\Win32\VC9\Debug\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
!ELSEIF  "$(CFG)" == "XalanMsgLib - Win64 Release"

OUTDIR=.\..\..\..\..\..\Build\Win64\VC9\Release\Util\XalanMsgLib
INTDIR=.\..\..\..\..\..\Build\Win64\VC9\Release\Util\XalanMsgLib

ALL : "..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC90.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).dll"
	-@erase "..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).exp"
	-@erase "..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /EHsc /O2 /I $(XERCESCROOT)\src /I "..\..\..\..\..\src" /I ".\..\..\..\..\..\Build\Win64\VC9\Release\Nls\Include" /D "_WIN64" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:none /machine:X64 /out:"..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).dll" /implib:"..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).lib" /machine:X64
LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj"

"..\..\..\..\..\Build\Win64\VC9\Release\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\VC9\Debug\Util\XalanMsgLib
INTDIR=.\..\..\..\..\..\Build\Win64\VC9\Debug\Util\XalanMsgLib
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\VC9\Debug\Util\XalanMsgLib
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win64\VC9\Debug\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC90.idb"
	-@erase "$(INTDIR)\VC90.pdb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(OUTDIR)\XalanMsgLib.exp"
	-@erase "$(OUTDIR)\$(DLLNAME).lib"
	-@erase "..\..\..\..\..\Build\Win64\VC9\Debug\$(DLLNAME).dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /ZI /Od /I $(XERCESCROOT)\src /I "..\..\..\..\..\src" /I ".\..\..\..\..\..\Build\Win64\VC9\Debug\Nls\Include" /D "_WIN64" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /RTC1 /c 
MTL_PROJ=/D "_DEBUG" /win32 

LINK32=link.exe
LINK32_FLAGS= /nologo /dll /pdb:none /debug /machine:X64 /out:"..\..\..\..\..\Build\Win64\VC9\Debug\$(DLLNAME).dll" /implib:"..\..\..\..\..\Build\Win64\VC9\Debug\$(DLLNAME).lib" /machine:X64
LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj"

"..\..\..\..\..\Build\Win64\VC9\Debug\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("XalanMsgLib.dep")
!INCLUDE "XalanMsgLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "XalanMsgLib.dep"
!ENDIF 
!ENDIF 



SOURCE=..\..\..\..\..\src\xalanc\Utils\XalanMsgLib\XalanMsgLib.cpp

!IF  "$(CFG)" == "XalanMsgLib - Win32 Release" || "$(CFG)" == "XalanMsgLib - Win32 Release.symbols" || "$(CFG)" == "XalanMsgLib - Win64 Release"


"$(INTDIR)\XalanMsgLib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Debug" || "$(CFG)" == "XalanMsgLib - Win64 Debug"


"$(INTDIR)\XalanMsgLib.obj"	"$(INTDIR)\XalanMsgLib.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

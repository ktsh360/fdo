@echo off

SET TYPEACTIONWFS=buildinstall
SET TYPEBUILDWFS=release
SET FDOORGPATHWFS=%cd%
SET FDOINSPATHWFS=%cd%\Fdo
SET FDOBINPATHWFS=%cd%\Fdo\Bin
SET FDOINCPATHWFS=%cd%\Fdo\Inc
SET FDOLIBPATHWFS=%cd%\Fdo\Lib
SET FDODOCPATHWFS=%cd%\Fdo\Docs
SET DOCENABLEWFS=skip
SET FDOERROR=0

:study_params
if (%1)==() goto start_build

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-c"       goto get_conf
if "%1"=="-config"  goto get_conf

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

goto custom_error

:get_docs
SET DOCENABLEWFS=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_action
SET TYPEACTIONWFS=%2
if "%2"=="installonly" goto next_param
if "%2"=="buildonly" goto next_param
if "%2"=="buildinstall" goto next_param
goto custom_error

:get_conf
SET TYPEBUILDWFS=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATHWFS=%~2
SET FDOINSPATHWFS=%~2\Fdo
SET FDOBINPATHWFS=%~2\Fdo\Bin
SET FDOINCPATHWFS=%~2\Fdo\Inc
SET FDOLIBPATHWFS=%~2\Fdo\Lib
SET FDODOCPATHWFS=%~2\Fdo\Docs

:next_param
shift
shift
goto study_params

:start_build
if "%TYPEACTIONWFS%"=="buildonly" goto start_exbuild
if not exist "%FDOINSPATHWFS%" mkdir "%FDOINSPATHWFS%"
if not exist "%FDOBINPATHWFS%" mkdir "%FDOBINPATHWFS%"
if not exist "%FDOINCPATHWFS%" mkdir "%FDOINCPATHWFS%"
if not exist "%FDOLIBPATHWFS%" mkdir "%FDOLIBPATHWFS%"
if not exist "%FDODOCPATHWFS%" mkdir "%FDODOCPATHWFS%"

:start_exbuild
time /t
if "%TYPEACTIONWFS%"=="installonly" goto install_files_wfs

echo building %TYPEBUILDWFS% WFS provider dlls
pushd Src
SET FDOACTIVEBUILD=%cd%\WFSOS
cscript //job:prepare ../../../preparebuilds.wsf
msbuild WFSOS_temp.sln /t:Rebuild /p:Configuration=%TYPEBUILDWFS% /p:Platform="Win32" /nologo
SET FDOERROR=%errorlevel%
if exist WFSOS_temp.sln del /Q /F WFSOS_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTIONWFS%"=="buildonly" goto generate_docs

:install_files_wfs
echo copy %TYPEBUILDWFS% WFS provider output files
copy /y "%FDOUTILITIES%\OWS\Bin\Win32\%TYPEBUILDWFS%\OWS.dll" "%FDOBINPATHWFS%"
copy /y "Bin\Win32\%TYPEBUILDWFS%\WFSProvider.dll" "%FDOBINPATHWFS%"
copy /y "Bin\Win32\%TYPEBUILDWFS%\WFSMessage.dll" "%FDOBINPATHWFS%"

echo copy header files
rem none

:generate_docs
if "%DOCENABLEWFS%"=="skip" goto end
echo Creating WFS provider html and chm documentation
if exist "%FDODOCPATHWFS%\HTML\Providers\WFS" rmdir /S /Q "%FDODOCPATHWFS%\HTML\Providers\WFS"
if exist "..\Docs\HTML\Providers\WFS" rmdir /S /Q "..\Docs\HTML\Providers\WFS"
if not exist "..\Docs\HTML\Providers\WFS" mkdir "..\Docs\HTML\Providers\WFS"
if exist ..\Docs\WFS_Provider_API.chm attrib -r ..\Docs\WFS_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_WFS
popd
xcopy/CQEYI ..\Docs\HTML\Providers\WFS\* "%FDODOCPATHWFS%\HTML\Providers\WFS"
copy /y "..\Docs\WFS_Provider_API.chm" "%FDODOCPATHWFS%"

:end
time /t
echo End WFS Build
exit /B 0

:error
echo There was a build error.
time /t
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] [-o=OutFolder] [-c=BuildType] [-a=Action] [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo Action:         -a[ction]=buildinstall(default), buildonly, installonly
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0
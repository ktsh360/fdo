@echo off

rem 
rem Copyright (C) 2004-2006  Autodesk, Inc.
rem 
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of version 2.1 of the GNU Lesser
rem General Public License as published by the Free Software Foundation.
rem 
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Lesser General Public License for more details.
rem 
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, write to the Free Software
rem Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
rem 

SET TYPEACTION=build
SET MSACTION=Build
SET TYPEBUILD=release
SET TYPEPLATFORM=Win32
SET INTERMEDIATEDIR=Win32
SET FDOORGPATH=%cd%
SET FDOINSPATH=%cd%\Fdo
SET FDOBINPATH=%cd%\Fdo\Bin
SET FDOINCPATH=%cd%\Fdo\Inc
SET FDOLIBPATH=%cd%\Fdo\Lib
SET FDODOCPATH=%cd%\Fdo\Docs
SET DOCENABLE=skip
SET FDOERROR=0

:study_params
if (%1)==() goto start_build

if "%1"=="-help"    goto help_show
if "%1"=="-h"       goto help_show

if "%1"=="-o"       goto get_path
if "%1"=="-outpath" goto get_path

if "%1"=="-c"       goto get_conf
if "%1"=="-config"  goto get_conf

if "%1"=="-p"       	goto get_platform
if "%1"=="-platform"    goto get_platform

if "%1"=="-a"       goto get_action
if "%1"=="-action"  goto get_action

if "%1"=="-d"       goto get_docs
if "%1"=="-docs"    goto get_docs

goto custom_error

:get_docs
SET DOCENABLE=%2
if "%2"=="build" goto next_param
if "%2"=="skip" goto next_param
goto custom_error

:get_conf 
SET TYPEBUILD=%2
if "%2"=="release" goto next_param
if "%2"=="debug" goto next_param
goto custom_error

:get_action
SET TYPEACTION=%2
if "%2"=="install" goto next_param
if "%2"=="build" goto next_param
if "%2"=="buildinstall" goto next_param
if "%2"=="clean" goto next_param
goto custom_error 

:get_platform
SET TYPEPLATFORM=%2
if "%2"=="Win32" goto next_param
if "%2"=="x64" goto next_param
goto custom_error

:get_path
if (%2)==() goto custom_error
SET FDOORGPATH=%~2
SET FDOINSPATH=%~2\Fdo
SET FDOBINPATH=%~2\Fdo\Bin
SET FDOINCPATH=%~2\Fdo\Inc
SET FDOLIBPATH=%~2\Fdo\Lib
SET FDODOCPATH=%~2\Fdo\Docs

:next_param
shift
shift
goto study_params

:start_build
SET FDOACTENVSTUDY="FDO"
if ("%FDO%")==("") goto env_error
if not exist "%FDO%" goto env_path_error

SET FDOACTENVSTUDY="FDOUTILITIES"
if ("%FDOUTILITIES%")==("") goto env_error
if not exist "%FDOUTILITIES%" goto env_path_error

if "%TYPEPLATFORM%"=="Win32" SET INTERMEDIATEDIR="Win32"
if "%TYPEPLATFORM%"=="x64" SET INTERMEDIATEDIR="Win64"

if "%TYPEACTION%"=="build" goto start_exbuild
if "%TYPEACTION%"=="clean" goto start_exbuild

if not exist "%FDOINSPATH%" mkdir "%FDOINSPATH%"
if not exist "%FDOBINPATH%" mkdir "%FDOBINPATH%"
if not exist "%FDODOCPATH%" mkdir "%FDODOCPATH%"

:start_exbuild
if "%TYPEACTION%"=="clean" SET MSACTION=Clean
if "%TYPEACTION%"=="install" goto install_files_SQLite

echo %MSACTION% %TYPEBUILD% SQLite provider dlls
SET FDOACTIVEBUILD=%cd%\Src\SQLiteProvider%VCBEXTENSION%
cscript //Nologo //job:prepare preparebuilds.wsf
pushd Src
msbuild SQLiteProvider%VCBEXTENSION%_temp.sln /t:%MSACTION% /p:Configuration=%TYPEBUILD% /p:Platform=%TYPEPLATFORM% /nologo /consoleloggerparameters:NoSummary
SET FDOERROR=%errorlevel%
if exist SQLiteProvider%VCBEXTENSION%_temp.sln del /Q /F SQLiteProvider%VCBEXTENSION%_temp.sln
popd
if "%FDOERROR%"=="1" goto error
if "%TYPEACTION%"=="clean" goto end
if "%TYPEACTION%"=="build" goto generate_docs

:install_files_SQLite
echo copy %TYPEBUILD% SQLite provider output files
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\SQLiteProvider.dll" "%FDOBINPATH%"
copy /y "Bin\%INTERMEDIATEDIR%\%TYPEBUILD%\SQLiteProvider.pdb" "%FDOBINPATH%"

:generate_docs
if "%DOCENABLE%"=="skip" goto install_docs
echo Creating SQLite provider html and chm documentation
if exist "Docs\HTML\SQLite" rmdir /S /Q "Docs\HTML\SQLite"
if not exist "Docs\HTML\SQLite" mkdir "Docs\HTML\SQLite"
copy ..\..\DocResources\geospatial.js Docs\HTML\SQLite
copy ..\..\DocResources\osgeo.css Docs\HTML\SQLite
if exist Docs\SQLite_Provider_API.chm attrib -r Docs\SQLite_Provider_API.chm
pushd Docs\doc_src
doxygen Doxyfile_SQLite
popd

:install_docs
if "%TYPEACTION%"=="build" goto end
if exist "%FDODOCPATH%\HTML\Providers\SQLite" rmdir /S /Q "%FDODOCPATH%\HTML\Providers\SQLite"
if exist Docs\HTML\SQLite xcopy/CQEYI Docs\HTML\SQLite\* "%FDODOCPATH%\HTML\Providers\SQLite"
if exist "Docs\SQLite_Provider_API.chm" copy /y "Docs\SQLite_Provider_API.chm" "%FDODOCPATH%"

:end
echo End SLITE %MSACTION%
exit /B 0

:env_error
echo Environment variable undefined: %FDOACTENVSTUDY%
SET FDOERROR=1
exit /B 1

:env_path_error
echo Invalid path contained in FDO environment variable: %FDOACTENVSTUDY%
SET FDOERROR=1
exit /B 1

:env_path_error_ex
echo Unable to find location of %FDOACTENVSTUDY% in the Windows System PATH
SET FDOERROR=1
exit /B 1

:error
echo There was a build error executing action: %MSACTION%
exit /B 1

:custom_error
echo The command is not recognized.
echo Please use the format:
:help_show
echo **************************************************************************
echo build.bat [-h] 
echo           [-o=OutFolder] 
echo           [-c=BuildType]
echo           [-a=Action] 
echo           [-p=PlatformType]
echo           [-d=BuildDocs]
echo *
echo Help:           -h[elp]
echo OutFolder:      -o[utpath]=destination folder for binaries
echo BuildType:      -c[onfig]=release(default), debug
echo PlatformType:   -p[latform]=Win32(default), x64
echo Action:         -a[ction]=build(default), buildinstall, install, clean
echo BuildDocs:      -d[ocs]=skip(default), build
echo **************************************************************************
exit /B 0

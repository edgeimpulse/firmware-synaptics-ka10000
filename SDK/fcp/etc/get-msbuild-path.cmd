@echo off
REM
REM The batch for getting path of MSBuild tool.
REM 

set VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe

if not exist "%VSWHERE%" (
	echo ""
	EXIT /B 0 
)

set pre=Microsoft.VisualStudio.Product.
set ids=%pre%Professional %pre%Enterprise %pre%Community %pre%BuildTools

for /f "usebackq tokens=1* delims=: " %%i in (`"%VSWHERE%" -latest -products %ids% -requires Microsoft.Component.MSBuild -version 15.9`) do (
if /i "%%i"=="installationPath" set InstallDir=%%j
)

if exist "%InstallDir%\MSBuild\15.0\Bin\MSBuild.exe" (
echo "%InstallDir%\MSBuild\15.0\Bin"
)

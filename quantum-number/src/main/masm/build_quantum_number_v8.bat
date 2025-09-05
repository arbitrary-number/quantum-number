@echo off
echo Building QuantumNumberV8 MASM Implementation...
echo.

REM Set up environment for MASM
set PATH=%PATH%;"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\bin\Hostx64\x64"
set PATH=%PATH%;"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64"

REM Check if ml64.exe exists
where ml64 >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: ml64.exe not found in PATH
    echo Please ensure Microsoft Visual Studio Build Tools are installed
    echo and ml64.exe is in your PATH
    pause
    exit /b 1
)

echo Assembling QuantunNumberV8.masm...
ml64 /c /Zi /Fo"QuantunNumberV8.obj" QuantunNumberV8.masm

if %ERRORLEVEL% neq 0 (
    echo ERROR: Assembly failed
    pause
    exit /b 1
)

echo.
echo Linking executable...
link QuantunNumberV8.obj /subsystem:console /entry:main /out:QuantunNumberV8.exe

if %ERRORLEVEL% neq 0 (
    echo ERROR: Linking failed
    pause
    exit /b 1
)

echo.
echo Build successful! Run QuantunNumberV8.exe to test the implementation.
echo.
pause

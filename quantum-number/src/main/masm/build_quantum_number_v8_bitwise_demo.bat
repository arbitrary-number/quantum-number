@echo off
REM ============================================================================
REM QUANTUM NUMBER V8 BITWISE DEMO BUILD SCRIPT
REM ============================================================================
REM This script builds the bitwise addition demo that uses pure bitwise
REM operations instead of the ADD instruction.
REM ============================================================================

echo Building QuantumNumberV8 Bitwise Demo...

REM Set up environment
set MASM_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64"
set LINK_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64"
set LIB_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.29.30133\lib\x64"
set INCLUDE_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.29.30133\include"

REM Add to PATH
set PATH=%PATH%;%MASM_PATH%;%LINK_PATH%

REM Clean previous build
if exist quantum_number_v8_bitwise_demo.obj del quantum_number_v8_bitwise_demo.obj
if exist quantum_number_v8_bitwise_demo.exe del quantum_number_v8_bitwise_demo.exe

REM Assemble the source file
echo Assembling quantum_number_v8_bitwise_demo.masm...
ml64 /c /Fo quantum_number_v8_bitwise_demo.obj quantum_number_v8_bitwise_demo.masm
if errorlevel 1 (
    echo Assembly failed!
    pause
    exit /b 1
)

REM Link the object file
echo Linking quantum_number_v8_bitwise_demo.exe...
link /SUBSYSTEM:CONSOLE /ENTRY:main /OUT:quantum_number_v8_bitwise_demo.exe quantum_number_v8_bitwise_demo.obj kernel32.lib user32.lib
if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo Run quantum_number_v8_bitwise_demo.exe to test the bitwise addition demo.

REM Run the demo
echo.
echo Running the demo...
quantum_number_v8_bitwise_demo.exe

pause

@echo off
REM Copyright 2025 Arbitrary Number Project Team
REM Licensed under the Apache License, Version 2.0
REM
REM Build script for Quantum Number Electron v2.0 Assembly Tests
REM Windows x64 build using NASM and Microsoft linker

echo Building Quantum Number Electron v2.0 Assembly Tests...
echo.

REM Check if NASM is available
nasm -v >nul 2>&1
if errorlevel 1 (
    echo ERROR: NASM assembler not found in PATH
    echo Please install NASM and add it to your PATH
    echo Download from: https://www.nasm.us/
    pause
    exit /b 1
)

REM Check if Microsoft linker is available
link /? >nul 2>&1
if errorlevel 1 (
    echo ERROR: Microsoft linker not found in PATH
    echo Please run this from a Visual Studio Developer Command Prompt
    echo Or install Visual Studio Build Tools
    pause
    exit /b 1
)

echo Step 1: Assembling quantum_number_electron.asm...
nasm -f win64 -g -o quantum_number_electron.obj quantum_number_electron.asm
if errorlevel 1 (
    echo ERROR: Failed to assemble quantum_number_electron.asm
    pause
    exit /b 1
)
echo   ✓ quantum_number_electron.obj created successfully

echo.
echo Step 2: Assembling test_quantum_electron.asm...
nasm -f win64 -g -o test_quantum_electron.obj test_quantum_electron.asm
if errorlevel 1 (
    echo ERROR: Failed to assemble test_quantum_electron.asm
    pause
    exit /b 1
)
echo   ✓ test_quantum_electron.obj created successfully

echo.
echo Step 3: Linking executable...
link /SUBSYSTEM:CONSOLE /ENTRY:main /OUT:test_quantum_electron.exe quantum_number_electron.obj test_quantum_electron.obj kernel32.lib
if errorlevel 1 (
    echo ERROR: Failed to link executable
    pause
    exit /b 1
)
echo   ✓ test_quantum_electron.exe created successfully

echo.
echo Step 4: Running tests...
echo =====================================
test_quantum_electron.exe
set TEST_RESULT=%errorlevel%

echo.
echo =====================================
if %TEST_RESULT% equ 0 (
    echo BUILD AND TEST SUCCESS!
    echo All assembly code assembled without error and tests passed.
) else (
    echo BUILD SUCCESS, but some tests failed.
    echo Assembly code assembled without error.
)

echo.
echo Files created:
echo   - quantum_number_electron.obj
echo   - test_quantum_electron.obj  
echo   - test_quantum_electron.exe
echo.

pause
exit /b %TEST_RESULT%

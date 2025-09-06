@echo off
REM Build script for demo_addition_1_3_plus_2_3_bitwise.asm (Bitwise Demo)
REM QuantumNumberV8 1/3 + 2/3 = 3/3 Bitwise Demo

echo Building QuantumNumberV8 1/3 + 2/3 Bitwise Demo...
echo.

REM Clean previous builds
if exist demo_addition_1_3_plus_2_3_bitwise.obj del demo_addition_1_3_plus_2_3_bitwise.obj
if exist demo_addition_1_3_plus_2_3_bitwise.exe del demo_addition_1_3_plus_2_3_bitwise.exe

REM Set UTF-8 code page for proper character display
chcp 65001 >nul
echo UTF-8 code page set for Windows console

REM Assemble with MASM
echo Assembling demo_addition_1_3_plus_2_3_bitwise.asm...
ml64 /c /nologo demo_addition_1_3_plus_2_3_bitwise.asm
if errorlevel 1 (
    echo ERROR: Assembly failed
    pause
    exit /b 1
)

REM Link executable
echo Linking demo_addition_1_3_plus_2_3_bitwise.exe...
link /nologo /subsystem:console /entry:main demo_addition_1_3_plus_2_3_bitwise.obj
if errorlevel 1 (
    echo ERROR: Linking failed
    pause
    exit /b 1
)

echo.
echo Build successful! Created demo_addition_1_3_plus_2_3_bitwise.exe
echo.

REM Optionally run the demo
echo Running demo...
echo.
demo_addition_1_3_plus_2_3_bitwise.exe

echo.
echo Demo execution complete.
pause

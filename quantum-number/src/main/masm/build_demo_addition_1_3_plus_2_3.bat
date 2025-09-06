@echo off
REM Build script for demo_addition_1_3_plus_2_3.asm (Conventional Math Demo)
REM QuantumNumberV8 1/3 + 2/3 = 3/3 Conventional Demo

echo Building QuantumNumberV8 1/3 + 2/3 Conventional Demo...
echo.

REM Clean previous builds
if exist demo_addition_1_3_plus_2_3.obj del demo_addition_1_3_plus_2_3.obj
if exist demo_addition_1_3_plus_2_3.exe del demo_addition_1_3_plus_2_3.exe

REM Set UTF-8 code page for proper character display
chcp 65001 >nul
echo UTF-8 code page set for Windows console

REM Assemble with MASM
echo Assembling demo_addition_1_3_plus_2_3.asm...
ml64 /c /nologo demo_addition_1_3_plus_2_3.asm
if errorlevel 1 (
    echo ERROR: Assembly failed
    pause
    exit /b 1
)

REM Link executable
echo Linking demo_addition_1_3_plus_2_3.exe...
link /nologo /subsystem:console /entry:main demo_addition_1_3_plus_2_3.obj
if errorlevel 1 (
    echo ERROR: Linking failed
    pause
    exit /b 1
)

echo.
echo Build successful! Created demo_addition_1_3_plus_2_3.exe
echo.

REM Optionally run the demo
echo Running demo...
echo.
demo_addition_1_3_plus_2_3.exe

echo.
echo Demo execution complete.
pause

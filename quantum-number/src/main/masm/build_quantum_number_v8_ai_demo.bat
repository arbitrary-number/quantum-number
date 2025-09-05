@echo off
REM ============================================================================
REM QUANTUM NUMBER V8 EXPLAINABLE AI DEMO BUILD SCRIPT
REM ============================================================================
REM Builds the explainable AI decision engine demonstration
REM ============================================================================

echo Building Quantum Number V8 Explainable AI Demo...

REM Set up environment
set MASM_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\bin\Hostx64\x64"
set INCLUDE_PATH="C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt"
set LIB_PATH="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64"

REM Create output directory
if not exist "..\build" mkdir ..\build

echo.
echo Assembling quantum_number_v8_core.masm...
ml64 /c /Fo"..\build\quantum_number_v8_core.obj" quantum_number_v8_core.masm
if errorlevel 1 (
    echo ERROR: Failed to assemble core library
    pause
    exit /b 1
)

echo.
echo Assembling quantum_number_v8_ai_demo.masm...
ml64 /c /Fo"..\build\quantum_number_v8_ai_demo.obj" quantum_number_v8_ai_demo.masm
if errorlevel 1 (
    echo ERROR: Failed to assemble AI demo
    pause
    exit /b 1
)

echo.
echo Linking quantum_number_v8_ai_demo.exe...
link /OUT:"..\build\quantum_number_v8_ai_demo.exe" ^
     /SUBSYSTEM:CONSOLE ^
     /ENTRY:main ^
     "..\build\quantum_number_v8_core.obj" ^
     "..\build\quantum_number_v8_ai_demo.obj" ^
     kernel32.lib ^
     user32.lib
if errorlevel 1 (
    echo ERROR: Failed to link executable
    pause
    exit /b 1
)

echo.
echo Build successful! Running Explainable AI Demo...
echo.
"..\build\quantum_number_v8_ai_demo.exe"

echo.
echo AI Demo complete. Decision was made with full transparency!
pause

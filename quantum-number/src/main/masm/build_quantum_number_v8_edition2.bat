@echo off
REM ============================================================================
REM QUANTUM NUMBER V8 EDITION 2 BUILD SCRIPT
REM ============================================================================
REM Builds the advanced quantum mathematics framework for super AI
REM ============================================================================

echo === QUANTUM NUMBER V8 EDITION 2 BUILD SCRIPT ===
echo.

REM Set up environment
set MASM_DIR=quantum-number\src\main\masm
set OUTPUT_DIR=quantum-number\target
set SOURCE_FILE=QuantumNumberV8Edition2.masm
set OUTPUT_EXE=QuantumNumberV8Edition2.exe

REM Create output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

echo Building %SOURCE_FILE%...
echo.

REM Compile with ML64 (x64 assembler)
ml64 /c /Cp /Cx /Zi "%MASM_DIR%\%SOURCE_FILE%" /Fo"%OUTPUT_DIR%\QuantumNumberV8Edition2.obj"

if errorlevel 1 (
    echo.
    echo ERROR: Assembly compilation failed!
    echo.
    pause
    exit /b 1
)

echo Assembly compilation successful.
echo.

REM Link the object file
link "%OUTPUT_DIR%\QuantumNumberV8Edition2.obj" /OUT:"%OUTPUT_DIR%\%OUTPUT_EXE%" /SUBSYSTEM:CONSOLE /ENTRY:main /LARGEADDRESSAWARE

if errorlevel 1 (
    echo.
    echo ERROR: Linking failed!
    echo.
    pause
    exit /b 1
)

echo Linking successful.
echo.

REM Check if executable was created
if exist "%OUTPUT_DIR%\%OUTPUT_EXE%" (
    echo.
    echo === BUILD SUCCESSFUL ===
    echo Executable created: %OUTPUT_DIR%\%OUTPUT_EXE%
    echo.
    echo Run the program with: %OUTPUT_DIR%\%OUTPUT_EXE%
    echo.
) else (
    echo.
    echo ERROR: Executable was not created!
    echo.
    pause
    exit /b 1
)

echo Build completed successfully!
echo.

pause

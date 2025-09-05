@echo off
REM ============================================================================
REM QUANTUM NUMBER V8: 1/3 + 2/3 = 1 TRADITIONAL DEMO BUILD SCRIPT
REM ============================================================================
REM Builds the traditional ASM demo for 1/3 + 2/3 = 1
REM ============================================================================

echo Building QuantumNumberV8 1/3 + 2/3 Traditional Demo...
echo.

REM Set UTF-8 code page for proper character display
chcp 65001 >nul 2>&1
if errorlevel 1 (
    echo Warning: Could not set UTF-8 code page
) else (
    echo UTF-8 code page set successfully
)

REM Clean previous build
if exist quantum_number_v8_1_3_plus_2_3_traditional_demo.obj del quantum_number_v8_1_3_plus_2_3_traditional_demo.obj
if exist quantum_number_v8_1_3_plus_2_3_traditional_demo.exe del quantum_number_v8_1_3_plus_2_3_traditional_demo.exe

REM Assemble the program
echo Assembling quantum_number_v8_1_3_plus_2_3_traditional_demo.masm...
ml64 /c /Cp /Cx /Zi quantum_number_v8_1_3_plus_2_3_traditional_demo.masm
if errorlevel 1 (
    echo Error: Assembly failed
    pause
    exit /b 1
)

REM Link the program
echo Linking quantum_number_v8_1_3_plus_2_3_traditional_demo.exe...
link /subsystem:console /entry:main /out:quantum_number_v8_1_3_plus_2_3_traditional_demo.exe quantum_number_v8_1_3_plus_2_3_traditional_demo.obj kernel32.lib user32.lib
if errorlevel 1 (
    echo Error: Linking failed
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable: quantum_number_v8_1_3_plus_2_3_traditional_demo.exe
echo.

REM Run the program if requested
set /p run_program="Run the program now? (y/n): "
if /i "%run_program%"=="y" (
    echo.
    echo Running QuantumNumberV8 1/3 + 2/3 Traditional Demo...
    echo ======================================================================
    quantum_number_v8_1_3_plus_2_3_traditional_demo.exe
    echo ======================================================================
    echo.
    echo Program execution completed.
)

echo.
pause

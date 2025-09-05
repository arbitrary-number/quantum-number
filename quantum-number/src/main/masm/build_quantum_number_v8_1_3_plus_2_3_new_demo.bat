@echo off
REM Build script for QuantumNumberV8 1/3 + 2/3 New Demo
REM Uses traditional ASM operations

echo Building QuantumNumberV8 1/3 + 2/3 New Demo...

REM Set UTF-8 code page for proper display
chcp 65001 >nul

REM Assemble the main demo file
ml64 /c /Fo"quantum_number_v8_1_3_plus_2_3_new_demo.obj" quantum_number_v8_1_3_plus_2_3_new_demo.masm

if errorlevel 1 (
    echo Assembly failed!
    pause
    exit /b 1
)

REM Link the object file
link /subsystem:console /out:"quantum_number_v8_1_3_plus_2_3_new_demo.exe" quantum_number_v8_1_3_plus_2_3_new_demo.obj

if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo Build successful! Running demo...
echo.

REM Run the demo
quantum_number_v8_1_3_plus_2_3_new_demo.exe

echo.
echo Demo completed.
pause

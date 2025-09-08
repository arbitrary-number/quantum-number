cmake_minimum_required(VERSION 3.20)

# Project name and supported languages
project(QuantumNumberV8 C ASM_MASM)

# Source files for the static library
set(QUANTUM_SOURCES
    QuantumNumberV8.c
    QuantumNumberV8.asm       # <-- Add MASM assembly file here
    PrintUtils.asm
)

# Ensure MASM is used for .asm files (Windows + MSVC only)
set_source_files_properties(QuantumNumberV8.asm PROPERTIES LANGUAGE ASM_MASM)

# Create the static library
add_library(QuantumNumberV8 STATIC ${QUANTUM_SOURCES})

# Include current directory so headers like QuantumNumberV8.h can be found
target_include_directories(QuantumNumberV8 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------------------------
# Optional: Build a test executable using the lib
# -----------------------------------------------

# Test program using the library
add_executable(QuantumNumberV8Test QuantumNumberV8Test.c)

# Link test program with the static library
target_link_libraries(QuantumNumberV8Test PRIVATE QuantumNumberV8)

# Use same include directory
target_include_directories(QuantumNumberV8Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

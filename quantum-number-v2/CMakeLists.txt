# Copyright 2025 Arbitrary Number Project Team
# Licensed under the Apache License, Version 2.0

cmake_minimum_required(VERSION 3.20)
project(quantum-number-electron VERSION 2.0.0 LANGUAGES C ASM_NASM)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform-specific settings for Windows x64
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(STATUS "Building for Windows x64")
        set(ARCH_X64 TRUE)
    else()
        message(FATAL_ERROR "Only x64 architecture is supported")
    endif()
else()
    message(FATAL_ERROR "Only Windows platform is currently supported")
endif()

# Compiler flags for optimization and debugging
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# NASM assembler settings
set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)
set(CMAKE_ASM_NASM_FLAGS "-f win64 -g")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Source files
set(QUANTUM_ELECTRON_SOURCES
    src/quantum_number_electron.asm
    src/quantum_electron_ops.c
    src/quantum_electron_math.c
    src/quantum_electron_utils.c
)

# Header files
set(QUANTUM_ELECTRON_HEADERS
    include/quantum_number_electron.h
    include/quantum_electron_ops.h
    include/quantum_electron_math.h
    include/quantum_electron_utils.h
)

# Create the quantum-number-electron library
add_library(quantum-number-electron STATIC ${QUANTUM_ELECTRON_SOURCES})

# Test executable
set(TEST_SOURCES
    tests/test_quantum_electron.c
    tests/test_basic_ops.c
    tests/test_math_ops.c
    tests/test_performance.c
)

add_executable(quantum-electron-tests ${TEST_SOURCES})
target_link_libraries(quantum-electron-tests quantum-number-electron)

# Benchmark executable
set(BENCHMARK_SOURCES
    benchmarks/benchmark_quantum_electron.c
    benchmarks/benchmark_basic_ops.c
    benchmarks/benchmark_math_ops.c
)

add_executable(quantum-electron-benchmarks ${BENCHMARK_SOURCES})
target_link_libraries(quantum-electron-benchmarks quantum-number-electron)

# Enable testing
enable_testing()
add_test(NAME quantum_electron_tests COMMAND quantum-electron-tests)

# Installation
install(TARGETS quantum-number-electron
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${QUANTUM_ELECTRON_HEADERS}
        DESTINATION include/quantum-number-electron)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "Quantum Number Electron v2.0 Configuration Summary:")
message(STATUS "  Platform: Windows x64")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  ASM Compiler: ${CMAKE_ASM_NASM_COMPILER}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")

# ====================================================================
# QUANTUM NUMBER V8 - SOURCE CODE BUILD CONFIGURATION
# ====================================================================
# Main source directory CMakeLists.txt
# Builds core quantum number libraries and components
# ====================================================================

cmake_minimum_required(VERSION 3.20)

# Include directories for quantum number headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/main/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/main/masm)

# Hint CMake where Java is installed (Corretto)
set(Java_JAVAC_EXECUTABLE "C:/Program Files/Amazon Corretto/jdk17.0.16_8/bin/javac.exe")
set(Java_JAVA_EXECUTABLE  "C:/Program Files/Amazon Corretto/jdk17.0.16_8/bin/java.exe")
set(CMAKE_JAVA_COMPILER "C:/Program Files/Amazon Corretto/jdk17.0.16_8/bin/javac.exe")

# ====================================================================
# MAIN COMPONENT LIBRARIES
# ====================================================================

# Quantum Number V8 MASM64 Assembly Library
if(WIN32 AND MSVC)
    # Assembly source files - exclude problematic UTF-8 macros for now
    set(MASM_SOURCES
        main/masm/QuantumNumberV8.masm
        main/masm/quantum_number_v8_core.masm
        # main/masm/utf8_to_utf16_macros.asm  # Disabled - missing masm32 includes
    )

    # Create static library from MASM assembly
    add_library(QuantumNumberV8 STATIC ${MASM_SOURCES})

    # Set MASM compilation properties
    set_target_properties(QuantumNumberV8 PROPERTIES
        LINKER_LANGUAGE ASM_MASM
        COMPILE_OPTIONS "/c;/nologo"
        PHASE "ASM_MASM"
    )

    target_compile_definitions(QuantumNumberV8 PRIVATE
        _WIN32=1
        QUANTUM_NUMBER_V8_BUILD=1
        MASM64_BUILD=1
    )

    message(STATUS "Quantum Number V8 (ASM) - ENABLED ✓")
else()
    message(STATUS "Quantum Number V8 (ASM) - DISABLED (MSVC/MASM64 only) ✗")
endif()

# ====================================================================
# C/C++ INTERFACE LIBRARY
# ====================================================================

# Quantum Number V8 C Interface
set(C_SOURCES
    main/masm/QuantumNumberV8.c
)

add_library(QuantumNumberV8C SHARED ${C_SOURCES})

# Interface library properties
set_target_properties(QuantumNumberV8C PROPERTIES
    OUTPUT_NAME "QuantumNumberV8"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "main/masm/QuantumNumberV8.h"
    INSTALL_RPATH "$ORIGIN"
)

# Include directories
target_include_directories(QuantumNumberV8C
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/main/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/main/masm>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main/masm
)

# Link with quantum number libraries
if(TARGET QuantumNumberV8)
    target_link_libraries(QuantumNumberV8C
        PRIVATE QuantumNumberV8
    )
endif()

# Windows-specific linking
if(WIN32)
    target_link_libraries(QuantumNumberV8C
        PRIVATE
            kernel32.lib
            user32.lib
            msvcrt.lib
    )
endif()

target_compile_definitions(QuantumNumberV8C
    PRIVATE QUANTUM_NUMBER_V8_C_BUILD=1
    PUBLIC QUANTUM_NUMBER_V8_API=
)

# Generate export def file for Windows
if(WIN32)
    target_sources(QuantumNumberV8C PRIVATE main/masm/QuantumNumberV8.def)
endif()

# ====================================================================
# DEMONSTRATION APPLICATIONS - DISABLED (Missing source files)
# ====================================================================
# Note: Demo applications disabled due to missing quantum_number_v8_demo.c source file
# The core QuantumNumberV8C library is still available for integration

# # Main quantum number demonstration
# set(DEMO_SOURCES main/masm/quantum_number_v8_demo.c)
#
# add_executable(quantum_number_demo ${DEMO_SOURCES})
#
# target_link_libraries(quantum_number_demo
#     PRIVATE QuantumNumberV8C
# )
#
# target_include_directories(quantum_number_demo
#     PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main/masm
# )
#
# target_compile_definitions(quantum_number_demo
#     PRIVATE QUANTUM_NUMBER_V8_DEMO=1
# )

# UTF-8 to UTF-16 test application
if(WIN32)
    set(UTF8_TEST_SOURCES
        main/masm/utf8_to_utf16_macros.asm
        main/masm/utf8_to_utf16_test_harness.asm
        main/masm/quantum_number_v8_core.masm
    )

    add_executable(utf8_test_harness ${UTF8_TEST_SOURCES})

    set_target_properties(utf8_test_harness PROPERTIES
        LINKER_LANGUAGE ASM_MASM
        OUTPUT_NAME "utf8_to_utf16_test"
    )

    message(STATUS "UTF-8 Test Harness - ENABLED ✓")
endif()

# ====================================================================
# SUBDIRECTORY PROCESSING
# ====================================================================

# Add subdirectories as needed
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main/cpp")
    add_subdirectory(main/cpp)
endif()

# Test subdirectory disabled - contains Java tests only (no CMakeLists.txt)
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
#     add_subdirectory(test)
# endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/example")
    add_subdirectory(example)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc")
    add_subdirectory(doc)
endif()

# ====================================================================
# BUILD CONFIGURATION SUMMARY
# ====================================================================

message(STATUS "================================================================")
message(STATUS "Quantum Number V8 Source Build Summary")
message(STATUS "================================================================")

if(TARGET QuantumNumberV8)
    message(STATUS "• QuantumNumberV8 (ASM Library): COMPILED ✓")
else()
    message(STATUS "• QuantumNumberV8 (ASM Library): SKIPPED ✗")
endif()

if(TARGET QuantumNumberV8C)
    message(STATUS "• QuantumNumberV8C (C Interface): COMPILED ✓")
    get_target_property(LIB_VERSION QuantumNumberV8C VERSION)
    message(STATUS "  - Version: ${LIB_VERSION}")
    get_target_property(LIB_OUTPUT QuantumNumberV8C OUTPUT_NAME)
    message(STATUS "  - Output: ${LIB_OUTPUT}")
else()
    message(STATUS "• QuantumNumberV8C (C Interface): FAILED ✗")
endif()

if(TARGET quantum_number_demo)
    message(STATUS "• quantum_number_demo (C Demo): COMPILED ✓")
else()
    message(STATUS "• quantum_number_demo (C Demo): FAILED ✗")
endif()

if(TARGET utf8_test_harness)
    message(STATUS "• utf8_test_harness (UTF-8 Test): COMPILED ✓")
else()
    message(STATUS "• utf8_test_harness (UTF-8 Test): SKIPPED ✗")
endif()

message(STATUS "================================================================")
message(STATUS "Quantum computing mathematics ready!")
message(STATUS "================================================================")

# ====================================================================
# DEVELOPMENT NOTES
# ====================================================================
#
# TARGETS CREATED:
# - QuantumNumberV8      : Core assembly library (Windows MSVC only)
# - QuantumNumberV8C     : C interface shared library
# - quantum_number_demo  : C demonstration application
# - utf8_test_harness    : UTF-8 conversion test (Windows ASM only)
#
# INSTALLATION:
# Libraries will be installed to CMAKE_INSTALL_ROOT as configured
# in the main CMakeLists.txt
#
# BUILDING:
# cmake --build build --target QuantumNumberV8C   (C interface)
# cmake --build build --target quantum_number_demo (Demo app)
#
# ====================================================================

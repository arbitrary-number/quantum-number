# ====================================================================
# QUANTUM NUMBER V8 - TEST SUITE BUILD CONFIGURATION
# ====================================================================
# Test suite directory CMakeLists.txt
# Builds comprehensive quantum number tests
# ====================================================================

cmake_minimum_required(VERSION 3.20)

include(CTest)
include(GoogleTest)

# Test include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/main/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/main/masm)

# ====================================================================
# UNIT TEST LIBRARIES
# ====================================================================

# C Interface Tests
if(BUILD_TESTING)
    enable_testing()

    # Quantum Number C Interface Tests
    add_executable(test_quantum_number_c
        test_quantum_number_c.c
        test_quantum_number_interface.c
        test_quantum_number_operations.c
    )

    target_link_libraries(test_quantum_number_c
        PRIVATE QuantumNumberV8C
    )

    target_include_directories(test_quantum_number_c
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/masm
    )

    target_compile_definitions(test_quantum_number_c
        PRIVATE QUANTUM_NUMBER_V8_TEST_BUILD=1
    )

    add_test(NAME test_quantum_number_c
             COMMAND test_quantum_number_c
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # UTF-8/UTF-16 Conversion Tests
    if(WIN32)
        add_executable(test_utf8_conversion
            test_utf8_conversion.c
            test_unicode_boundary.c
            test_surrogate_pairs.c
        )

        target_link_libraries(test_utf8_conversion
            PRIVATE QuantumNumberV8C
        )

        target_compile_definitions(test_utf8_conversion
            PRIVATE QUANTUM_NUMBER_V8_UTF8_TEST=1
        )

        add_test(NAME test_utf8_conversion
                 COMMAND test_utf8_conversion
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()

    # Mathematical Precision Tests
    add_executable(test_mathematical_precision
        test_fraction_operations.c
        test_arithmetic_precision.c
        test_overflow_handling.c
        test_edge_cases.c
    )

    target_link_libraries(test_mathematical_precision
        PRIVATE QuantumNumberV8C
    )

    target_compile_definitions(test_mathematical_precision
        PRIVATE QUANTUM_NUMBER_V8_PRECISION_TEST=1
    )

    add_test(NAME test_mathematical_precision
             COMMAND test_mathematical_precision
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Performance Benchmark Tests
    add_executable(test_performance
        test_benchmark.c
        test_timing.c
        test_throughput.c
    )

    target_link_libraries(test_performance
        PRIVATE QuantumNumberV8C
    )

    target_compile_definitions(test_performance
        PRIVATE QUANTUM_NUMBER_V8_PERFORMANCE_TEST=1
    )

    add_test(NAME test_performance
             COMMAND test_performance
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Java Integration Tests (if Java available)
    if(Java_FOUND)
        add_test(NAME test_java_integration
                 COMMAND ${Java_JAVA_EXECUTABLE}
                         -cp "${CMAKE_JAVA_TARGET_OUTPUT_DIR}:${CMAKE_CURRENT_BINARY_DIR}"
                         com.github.quantum_number.QuantumNumberTest
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
endif()

# ====================================================================
# TEST DATA AND RESOURCES
# ====================================================================

# Copy test data files
file(COPY test_data/
     DESTINATION ${CMAKE_BINARY_DIR}/test_data
     FILES_MATCHING PATTERN "*.txt" PATTERN "*.csv" PATTERN "*.dat")

# Reset any accidental modifications during testing
set(QUANTUM_NUMBER_OVERRIDE OFF CACHE BOOL "Override quantum number error checking" FORCE)

# ====================================================================
# TEST CONFIGURATION SUMMARY
# ====================================================================

message(STATUS "================================================================")
message(STATUS "Quantum Number V8 Test Suite Configuration")
message(STATUS "================================================================")

if(TARGET test_quantum_number_c)
    message(STATUS "• C Interface Tests: ENABLED ✓")
else()
    message(STATUS "• C Interface Tests: DISABLED ✗")
endif()

if(TARGET test_utf8_conversion)
    message(STATUS "• UTF-8 Conversion Tests: ENABLED ✓")
else()
    message(STATUS "• UTF-8 Conversion Tests: DISABLED ✗")
endif()

if(TARGET test_mathematical_precision)
    message(STATUS "• Mathematical Precision Tests: ENABLED ✓")
else()
    message(STATUS "• Mathematical Precision Tests: DISABLED ✗")
endif()

if(TARGET test_performance)
    message(STATUS "• Performance Tests: ENABLED ✓")
else()
    message(STATUS "• Performance Tests: DISABLED ✗")
endif()

if(Java_FOUND)
    message(STATUS "• Java Integration Tests: ENABLED ✓")
else()
    message(STATUS "• Java Integration Tests: DISABLED ✗")
endif()

message(STATUS "================================================================")

# ====================================================================
# DEVELOPMENT NOTES
# ====================================================================
#
# TEST TARGETS:
# - test_quantum_number_c        : Core C interface functionality
# - test_utf8_conversion         : Unicode conversion accuracy
# - test_mathematical_precision  : Numerical computation precision
# - test_performance            : Execution speed and throughput
# - test_java_integration       : Java JNI interface validation
#
# RUNNING TESTS:
# cmake --build build --target test             (Run all tests)
# ctest -V                                       (Verbose test output)
# ctest --output-on-failure                      (Show failures only)
#
# TEST DATA:
# Test data files are copied to CMAKE_BINARY_DIR/test_data/
# This includes reference values, edge cases, and performance benchmarks
#
# ====================================================================

# ====================================================================
# OPTIONAL: VALGRIND MEMORY TESTING (Linux only)
# ====================================================================

find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE AND UNIX)
    add_test(NAME test_memory_leaks
             COMMAND ${VALGRIND_EXECUTABLE}
                     --leak-check=full
                     --show-leak-kinds=all
                     --errors-for-leak-kinds=all
                     --error-exitcode=1
                     $<TARGET_FILE:test_quantum_number_c>
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

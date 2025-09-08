# ====================================================================
# QUANTUM NUMBER V8 - EXAMPLES BUILD CONFIGURATION
# ====================================================================
# Examples directory CMakeLists.txt
# Builds demonstration and tutorial applications
# ====================================================================

cmake_minimum_required(VERSION 3.20)

# Example include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/main/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/main/masm)

# ====================================================================
# DEMONSTRATION APPLICATIONS
# ====================================================================

# Basic quantum number example
add_executable(example_basic
    basic_usage.c
)

target_link_libraries(example_basic
    PRIVATE QuantumNumberV8C
)

target_include_directories(example_basic
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/masm
)

target_compile_definitions(example_basic
    PRIVATE QUANTUM_NUMBER_V8_EXAMPLE_BUILD=1
)

# Advanced mathematical examples
add_executable(example_advanced_math
    advanced_math.c
    complex_operations.c
    precision_demo.c
)

target_link_libraries(example_advanced_math
    PRIVATE QuantumNumberV8C
)

target_compile_definitions(example_advanced_math
    PRIVATE QUANTUM_NUMBER_V8_ADVANCED_EXAMPLE=1
)

# Quantum algorithm examples
add_executable(example_quantum_algorithms
    quantum_algorithms.c
    superposition_demo.c
    entanglement_demo.c
)

target_link_libraries(example_quantum_algorithms
    PRIVATE QuantumNumberV8C
)

target_compile_definitions(example_quantum_algorithms
    PRIVATE QUANTUM_NUMBER_V8_QUANTUM_EXAMPLE=1
)

# Performance comparison examples
add_executable(example_performance
    performance_comparison.c
    benchmark_demo.c
    optimization_examples.c
)

target_link_libraries(example_performance
    PRIVATE QuantumNumberV8C
)

target_compile_definitions(example_performance
    PRIVATE QUANTUM_NUMBER_V8_PERFORMANCE_EXAMPLE=1
)

# UTF-8/UTF-16 conversion examples
add_executable(example_unicode
    unicode_conversion.c
    utf8_examples.c
    international_demo.c
)

target_link_libraries(example_unicode
    PRIVATE QuantumNumberV8C
)

target_compile_definitions(example_unicode
    PRIVATE QUANTUM_NUMBER_V8_UNICODE_EXAMPLE=1
)

# ====================================================================
# INSTALLATION SETUP FOR EXAMPLES
# ====================================================================

# Install example executables
install(TARGETS
    example_basic
    example_advanced_math
    example_quantum_algorithms
    example_performance
    example_unicode
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
    COMPONENT examples
)

# Install example source code for educational purposes
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/basic_usage.c
    ${CMAKE_CURRENT_SOURCE_DIR}/advanced_math.c
    ${CMAKE_CURRENT_SOURCE_DIR}/complex_operations.c
    ${CMAKE_CURRENT_SOURCE_DIR}/quantum_algorithms.c
    ${CMAKE_CURRENT_SOURCE_DIR}/performance_comparison.c
    ${CMAKE_CURRENT_SOURCE_DIR}/unicode_conversion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples
    COMPONENT examples
)

# ====================================================================
# CUSTOM TARGETS FOR RUNNING EXAMPLES
# ====================================================================

# Add convenience targets for running examples
add_custom_target(run_basic_example
    COMMAND example_basic
    DEPENDS example_basic
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running basic quantum number example..."
)

add_custom_target(run_advanced_example
    COMMAND example_advanced_math
    DEPENDS example_advanced_math
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running advanced mathematical examples..."
)

add_custom_target(run_quantum_example
    COMMAND example_quantum_algorithms
    DEPENDS example_quantum_algorithms
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running quantum algorithm examples..."
)

add_custom_target(run_performance_example
    COMMAND example_performance
    DEPENDS example_performance
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running performance comparison examples..."
)

add_custom_target(run_unicode_example
    COMMAND example_unicode
    DEPENDS example_unicode
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Unicode conversion examples..."
)

# ====================================================================
# EXAMPLE CONFIGURATION SUMMARY
# ====================================================================

message(STATUS "================================================================")
message(STATUS "Quantum Number V8 Example Applications")
message(STATUS "================================================================")

if(TARGET example_basic)
    message(STATUS "• Basic Usage Example: ENABLED ✓")
else()
    message(STATUS "• Basic Usage Example: DISABLED ✗")
endif()

if(TARGET example_advanced_math)
    message(STATUS "• Advanced Math Example: ENABLED ✓")
else()
    message(STATUS "• Advanced Math Example: DISABLED ✗")
endif()

if(TARGET example_quantum_algorithms)
    message(STATUS "• Quantum Algorithms Example: ENABLED ✓")
else()
    message(STATUS "• Quantum Algorithms Example: DISABLED ✗")
endif()

if(TARGET example_performance)
    message(STATUS "• Performance Example: ENABLED ✓")
else()
    message(STATUS "• Performance Example: DISABLED ✗")
endif()

if(TARGET example_unicode)
    message(STATUS "• Unicode Example: ENABLED ✓")
else()
    message(STATUS "• Unicode Example: DISABLED ✗")
endif()

message(STATUS "================================================================")
message(STATUS "Example applications built successfully!")
message(STATUS "================================================================")

# ====================================================================
# DEVELOPMENT NOTES
# ====================================================================
#
# EXAMPLE TARGETS:
# - example_basic              : Basic quantum number usage
# - example_advanced_math      : Complex mathematical operations
# - example_quantum_algorithms : Quantum computing algorithms
# - example_performance       : Performance comparisons and benchmarks
# - example_unicode           : UTF-8/UTF-16 conversion examples
#
# CUSTOM TARGETS:
# - run_basic_example         : Run basic example
# - run_advanced_example      : Run advanced math example
# - run_quantum_example       : Run quantum algorithms example
# - run_performance_example   : Run performance example
# - run_unicode_example       : Run Unicode example
#
# BUILDING:
# cmake --build build --target example_basic       (Build basic example)
# cmake --build build --target run_basic_example   (Build and run)
#
# USAGE IN CODE:
# See the example source files for API usage patterns and best practices
#
# ====================================================================

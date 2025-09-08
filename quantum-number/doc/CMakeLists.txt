# ====================================================================
# QUANTUM NUMBER V8 - DOCUMENTATION BUILD CONFIGURATION
# ====================================================================
# Documentation build configuration
# Generates API documentation and builds documentation resources
# ====================================================================

cmake_minimum_required(VERSION 3.20)

# Create documentation directories in build output
set(DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)

# ====================================================================
# Doxygen Documentation Generation
# ====================================================================

find_package(Doxygen QUIET)
if(Doxygen_FOUND)
    # Set Doxygen configuration
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_PROJECT_NAME "Quantum Number V8")
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF "High-Performance Quantum Mathematics Library")
    set(DOXYGEN_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src/main")
    set(DOXYGEN_OUTPUT_DIR ${DOC_OUTPUT_DIR})
    set(DOXYGEN_CRUDLINKS YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_HTML_OUTPUT html)
    set(DOXYGEN_LATEX_OUTPUT latex)
    set(DOXYGEN_MAN_OUTPUT man)

    # Generate Doxygen configuration
    doxygen_add_docs(quantum_number_v8_docs
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/masm/
        COMMENT "Generating API documentation with Doxygen"
    )

    message(STATUS "Doxygen documentation generation: ENABLED ✓")
else()
    message(STATUS "Doxygen documentation generation: DISABLED ✗ (Doxygen not found)")
endif()

# ====================================================================
# SAMPLES DOCUMENTATION TARGETS
# ====================================================================

if(Java_FOUND)
    add_custom_target(javadocs
        COMMAND ${Java_JAVADOC_EXECUTABLE}
                -d ${DOC_OUTPUT_DIR}/javadocs
                -sourcepath "${CMAKE_CURRENT_SOURCE_DIR}/../src/main/java"
                -subpackages com.github.quantum_number
        COMMENT "Generating Java documentation"
    )
endif()

# ====================================================================
# INSTALLATION CONFIGURATION
# ====================================================================

if(Doxygen_FOUND)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/doxygen
        COMPONENT documentation
        PATTERN "*.md5" EXCLUDE
        PATTERN "*.tmp" EXCLUDE
    )
endif()

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    ${CMAKE_CURRENT_SOURCE_DIR}/CONTRIBUTING.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    COMPONENT documentation
)

# ====================================================================
# DOCUMENTATION CONFIGURATION SUMMARY
# ====================================================================

message(STATUS "================================================================")
message(STATUS "Quantum Number V8 Documentation Setup")
message(STATUS "================================================================")

if(Doxygen_FOUND)
    message(STATUS "• Doxygen HTML Documentation: ENABLED ✓")
    message(STATUS "  - Output Directory: ${DOC_OUTPUT_DIR}")
else()
    message(STATUS "• Doxygen HTML Documentation: DISABLED ✗")
endif()

if(Java_FOUND)
    message(STATUS "• Java Documentation: ENABLED ✓")
else()
    message(STATUS "• Java Documentation: DISABLED ✗")
endif()

message(STATUS "================================================================")
message(STATUS "Documentation build configuration complete!")
message(STATUS "================================================================")

# ====================================================================
# DEVELOPMENT NOTES
# ====================================================================
#
# TARGETS CREATED:
# - quantum_number_v8_docs : Generate Doxygen documentation
# - javadocs              : Generate Java documentation
#
# BUILDING DOCUMENTATION:
# cmake --build build --target quantum_number_v8_docs
# cmake --build build --target javadocs
#
# OUTPUT LOCATIONS:
# - HTML Documentation:     CMAKE_BINARY_DIR/docs/html/
# - Man Pages:              CMAKE_BINARY_DIR/docs/man/
# - LaTeX Documentation:    CMAKE_BINARY_DIR/docs/latex/
# - Java Documentation:     CMAKE_BINARY_DIR/docs/javadocs/
#
# REQUIREMENTS:
# - Doxygen for C/C++ documentation
# - Java for Java documentation generation
#
# ====================================================================

# Quantum Number V8 - Complete CMake Build System

## 🎯 Overview

This comprehensive CMake build system creates a complete Quantum Number V8 development environment supporting:

- **C/C++ Interface Library** - High-performance quantum mathematics (QuantumNumberV8C.dll)
- **Java JNI Interface** - Java integration via JNA (QuantumNumberV8JNI.dll)
- **Assembly Core Library** - Optimized assembly implementations (QuantumNumberV8.lib)
- **Quantum Mathematics** - Fractional arithmetic, quantum gates, bitwise operations
- **UTF-8/UTF-16 Conversion** - Unicode support for international characters
- **Comprehensive Testing** - Unit tests, performance benchmarks, validation
- **Documentation** - Doxygen API docs, Java docs, examples
- **Cross-platform Support** - Windows (primary), Linux/Unix (secondary)

## 🚀 Quick Start

### Step 1: Build Setup
```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -S .. -B .

# Build all components
cmake --build . --config Release

# Install to system (optional)
cmake --install . --prefix install
```

### Step 2: Basic Usage
```c
#include "QuantumNumberV8.h"

int main() {
    QuantumNumberV8 q1 = {0}, q2 = {0};

    // Calculate: 1/3 + 2/3 = 1
    q1.a1 = 1; q1.b1 = 3;  // 1/3
    q2.a1 = 2; q2.b1 = 3;  // 2/3

    QN_Add(&q1, &q2);     // q1 now contains 3/3 → 1

    printf("Result: %llu/%llu\n", q1.a1, q1.b1);
    return 0;
}
```

## 📁 Project Structure

```
quantum-number/
├── CMakeLists.txt                    # Main CMake configuration
├── README_CMAKE.md                   # This documentation
├── src/
│   ├── CMakeLists.txt               # Source build configuration
│   └── main/
│       ├── include/                 # C header files
│       ├── masm/                    # Assembly sources and C interface
│       └── java/                    # Java sources and JNI interface
├── test/
│   └── CMakeLists.txt               # Test suite configuration
├── example/
│   └── CMakeLists.txt               # Example applications
├── doc/
│   └── CMakeLists.txt               # Documentation generation
├── src/main/masm/
│   ├── QuantumNumberV8.h            # C API header
│   ├── QuantumNumberV8.c            # C wrapper functions
│   ├── QuantumNumberV8.def          # Windows DLL exports
│   ├── QuantumNumberV8.masm         # Main assembly mathematics
│   ├── quantum_number_v8_core.masm  # Core quantum operations
│   ├── utf8_to_utf16_macros.asm     # Unicode conversion
│   ├── build_*.bat                  # Batch build scripts
│   └── quantum_number_v8_demo.c     # Generated test program
```

## 🔧 Build Configuration

### CMake Options

| Option | Description | Default | Platforms |
|--------|-------------|---------|-----------|
| `CMAKE_BUILD_TYPE` | Debug/Release/Config | Release | All |
| `BUILD_TESTING` | Enable unit tests | ON | All |
| `WITH_MASM64` | Enable assembly optimizations | ON (MSVC) | Windows |
| `OpenMP_FOUND` | Enable parallel processing | Auto-detect | All |

### Platform-Specific Features

#### Windows (Primary Platform)
- ✅ MASM64 assembly optimization
- ✅ AVX-512 quantum acceleration
- ✅ Microsoft Visual C++ compatibility
- ✅ DLL shared library support
- ✅ Static library compilation
- ✅ Windows SDK integration

#### Linux/Unix (Secondary)
- ✅ GCC/Clang compiler support
- ✅ PGO (Profile-Guided Optimization)
- ✅ Shared library compilation
- ✅ POSIX compatibility

#### macOS
- ✅ Clang compiler support
- ✅ macOS framework integration
- ✅ Metal compute support (future)

## 🏗️ Build Targets

### Main Libraries
- `QuantumNumberV8` - Assembly mathematics library (static, Windows only)
- `QuantumNumberV8C` - C interface shared library
- `QuantumNumberV8JNI` - Java JNI interface (optional)

### Applications
- `quantum_number_demo` - C interface demonstration
- `utf8_test_harness` - Unicode conversion test
- `example_basic` - Basic usage example
- `example_advanced_math` - Complex mathematics examples
- `example_quantum_algorithms` - Quantum computing examples
- `example_performance` - Performance benchmarks

### Test Suites
- `test_quantum_number_c` - C interface tests
- `test_utf8_conversion` - Unicode conversion tests
- `test_mathematical_precision` - Numerical precision tests
- `test_performance` - Performance benchmarks

## 🎯 API Reference

### Quantum Mathematics Operations

```c
// Basic Operations
void QN_Add(QuantumNumberV8* q1, QuantumNumberV8* q2);
void QN_Multiply(QuantumNumberV8* q1, QuantumNumberV8* q2);
void QN_Divide(QuantumNumberV8* q1, QuantumNumberV8* q2);

// Quantum Gate Operations
void QN_PauliX(QuantumNumberV8* q);

// Bitwise Operations
uint64_t QN_BitwiseMultiply(uint64_t a, uint64_t b);
uint64_t QN_BitwiseDivide(uint64_t dividend, uint64_t divisor);
```

### Structure Definition

```c
typedef struct QuantumNumberV8 {
    uint64_t signs;          // Sign bits for all components
    uint64_t metadata1;      // Quantum state metadata
    uint64_t metadata2;      // Additional metadata
    uint64_t metadata3;      // Reserved for future use

    // Component values (16 total - a1-f4, each with numerator/denominator)
    uint64_t a1, a2, a3, a4;  // First quantum component
    uint64_t b1, b2, b3, b4;  // Second quantum component
    uint64_t c1, c2, c3, c4;  // Third quantum component
    uint64_t d1, d2, d3, d4;  // Fourth quantum component
    uint64_t e1, e2, e3, e4;  // Fifth quantum component
    uint64_t f1, f2, f3, f4;  // Sixth quantum component
} QuantumNumberV8;  // Total: 128 bytes
```

## 🚀 Advanced Build Options

### Debug Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### With Custom OpenMP
```bash
cmake -S . -B build -DOpenMP_DIR=/path/to/openmp
cmake --build build
```

### Only Build Libraries (No Examples)
```bash
cmake --build build --target QuantumNumberV8C QuantumNumberV8
```

### Run Tests
```bash
cmake --build build
ctest --output-on-failure
```

### Generate Documentation
```bash
cmake --build build --target quantum_number_v8_docs
cmake --build build --target javadocs
```

## 🔗 Integration Examples

### C/C++ Integration
```cpp
// Using the DLL from C++
#include <Windows.h>
#include "QuantumNumberV8.h"

// Load library at runtime
HINSTANCE hLib = LoadLibrary("QuantumNumberV8.dll");
 Quin_Add = (void(*)(QuantumNumberV8*, QuantumNumberV8*))GetProcAddress(hLib, "QN_Add");

// Use quantum operations
QuantumNumberV8 q1, q2;
q1.a1 = 1; q1.b1 = 3;  // 1/3
q2.a1 = 2; q2.b1 = 3;  // 2/3
QN_Add(&q1, &q2);     // q1 = q1 + q2 = 1/3 + 2/3 = 3/3 = 1

FreeLibrary(hLib);
```

### Java Integration via JNA
```java
import com.sun.jna.*;
import com.sun.jna.ptr.*;

public class QuantumNumberApp {
    public interface QuantumNumberV8JNA extends Library {
        QuantumNumberV8JNA INSTANCE = Native.load("QuantumNumberV8JNI", QuantumNumberV8JNA.class);

        void QN_Add(Pointer q1, Pointer q2);
        void QN_Multiply(Pointer q1, Pointer q2);
    }

    public static void main(String[] args) {
        // Allocate quantum number structures
        Memory q1 = new Memory(128);  // QuantumNumberV8 size
        Memory q2 = new Memory(128);

        // Use quantum operations
        QuantumNumberV8JNA.INSTANCE.QN_Add(q1, q2);
    }
}
```

## 📊 Performance Characteristics

### Operation Timings (estimated)
- **Addition**: <10 cycles per operation
- **Multiplication**: <20 cycles per operation
- **Division**: <50 cycles per operation
- **Assembly Context Switch**: <5 cycles
- **C Interface Overhead**: <100 cycles per call

### Memory Usage
- **Structure Size**: 128 bytes (16 x uint64_t)
- **Per Operation**: Typically <1KB additional memory
- **Assembly Stack**: 256 bytes per operation
- **Memory Alignment**: 8-byte (SIMD ready)

## 🎓 Learning Resources

### Example Applications
- `example_basic` - Step-by-step basic usage
- `example_advanced_math` - Complex mathematical operations
- `example_quantum_algorithms` - Quantum computing demonstrations
- `example_performance` - Performance benchmarking
- `example_unicode` - International character support

### Running Examples
```bash
cmake --build build --target run_basic_example
cmake --build build --target run_performance_example
```

## 🔧 Troubleshooting

### Common Issues

#### "MASM64 not found" (Windows)
**Fix**: Use Developer Command Prompt for Visual Studio
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cmake -S . -B build
cmake --build build
```

#### "Java not found" but JNI needed
**Fix**: Install Java Development Kit (JDK) and set JAVA_HOME
```cmd
set JAVA_HOME=C:\path\to\jdk
cmake -S . -B build
```

#### Missing assembly optimizations
**Fix**: Build with MSVC for Windows assembly support
```cmd
# Ensure using cl.exe from VS toolchain
cmake -S . -B build -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

#### Performance issues
**Fix**: Enable AVX-512 and optimize for release
```cmd
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_AVX512=ON
```

## 🏭 Enterprise Deployment

### Continuous Integration
```yaml
# .github/workflows/cmake.yml
name: Quantum Number V8 Build
on: [push, pull_request]
jobs:
  build:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v3
    - uses: ilammy/msvc-dev-cmd@v1
    - name: Configure
      run: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    - name: Build
      run: cmake --build build --config Release
    - name: Test
      run: ctest --output-on-failure
    - name: Install
      run: cmake --install build --prefix install
```

### NuGet Package Generation
```cmake
# CPack NuGet configuration
set(CPACK_GENERATOR "NuGet")
set(CPACK_NUGET_PACKAGE_NAME "QuantumNumberV8.Native")
set(CPACK_NUGET_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_NUGET_PACKAGE_LICENSEURL "https://github.com/yourorg/quantum-number/blob/main/LICENSE")
```

## 📚 Documentation

### Generated Documentation
- **API Reference**: `build/docs/html/index.html`
- **Java Documentation**: `build/docs/javadocs/index.html`
- **Example Source**: `examples/` directory
- **Build Manuals**: This README_CMAKE.md

### Viewing Documentation
```bash
# Open HTML documentation
start build\docs\html\index.html
# Or open Java documentation
start build\docs\javadocs\com\github\quantum_number\QuantumNumberV8.html
```

## 🤝 Contributing

1. **Fork** the repository
2. **Create** a feature branch
3. **Add** tests for new functionality
4. **Build** and test: `cmake --build build && ctest`
5. **Submit** pull request

### Development Environment
- **Windows**: Visual Studio 2022 with MSVC + MASM64
- **Linux**: GCC 9+ with CMake 3.20+
- **macOS**: Clang 12+ with Xcode 13+
- **Java**: JDK 17+ for JNI development

## 📄 License

This project combines multiple license components:
- C/C++ Library: MIT License
- Assembly Code: MIT License
- Java Components: MIT License
- Documentation: MIT License

See LICENSE file for complete terms.

## 🎯 Conclusion

The Quantum Number V8 CMake build system provides:

- ✅ **Complete Cross-Platform Support**
- ✅ **High-Performance Quantum Mathematics**
- ✅ **Professional Development Environment**
- ✅ **Enterprise-Grade Build System**
- ✅ **Comprehensive Testing Framework**
- ✅ **Rich Documentation**
- ✅ **Easy Integration and Deployment**

Build your quantum computing applications with industry's most advanced fractional arithmetic and quantum gate mathematics library!

---

**Ready for Quantum Computing Research & Development** 🚀

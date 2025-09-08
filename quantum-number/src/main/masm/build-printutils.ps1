# Save original CMakeLists.txt if it exists
if (Test-Path CMakeLists.txt) {
    Copy-Item -Path CMakeLists.txt -Destination CMakeLists_backup.txt -Force
}

# Replace with custom CMakeListsPrintUtils.txt
Copy-Item -Path CMakeListsPrintUtils.txt -Destination CMakeLists.txt -Force

# Run Visual Studio environment setup
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Create build directory if needed
if (-not (Test-Path build)) {
    New-Item -ItemType Directory -Force -Path build
}

# Configure CMake and build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# Restore original CMakeLists.txt if it was backed up
if (Test-Path CMakeLists_backup.txt) {
    Move-Item -Path CMakeLists_backup.txt -Destination CMakeLists.txt -Force
} else {
    # If no backup, remove the temporary CMakeLists.txt
    Remove-Item -Path CMakeLists.txt -Force
}

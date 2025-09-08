"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

New-Item -ItemType Directory -Force -Path build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

ECHO Bootstrap vcpkg and install deps
./external/vcpkg/bootstrap-vcpkg.bat
./external/vcpkg/vcpkg.exe install raylib imgui --triplet x64-windows
ECHO Setting up project structure for windwos
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=".\external\vcpkg\scripts\buildsystems\vcpkg.cmake"
read -p "Press Enter to continue" </dev/tty

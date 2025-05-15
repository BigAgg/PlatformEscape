ECHO Bootstrap vcpkg and install deps
./external/vcpkg/bootstrap-vcpkg.bat
./external/vcpkg/vcpkg.exe install raylib imgui --triplet x64-windows-static
ECHO Setting up project structure for windows
cmake -B build -S . -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
read -p "Press Enter to continue" </dev/tty

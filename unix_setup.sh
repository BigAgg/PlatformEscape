ECHO Bootstrap vcpkg and install deps
./external/vcpkg/bootstrap-vcpkg.sh
./external/vcpkg/vcpkg install raylib imgui --triplet x64-linux
ECHO Setting up project structure for ninja
cmake -B build/Release -S . -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_BUILD_TYPE=Release
cmake -B build/Debug -S . -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_BUILD_TYPE=Debug
read -p "Press Enter to continue" </dev/tty

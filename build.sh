#!/bin/bash
SYSTEM=$1
CONFIG=$2
PLATFORM=$3

if [ "$SYSTEM" == "Ninja" ]; then
  if [ "$PLATFORM" == "win32" ]; then
    echo "Bootstrap vcpkg and install deps"
    ./external/vcpkg/bootstrap-vcpkg.bat
    ./external/vcpkg/vcpkg.exe install raylib imgui --triplet x64-windows-static

    echo "Setting up project structure for Ninja on Windows"
    if [ "$CONFIG" == "Release" ]; then
      cmake -B build/Release -S . -G "Ninja" \
        -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
        -DVCPKG_TARGET_TRIPLET=x64-windows-static \
        -DCMAKE_BUILD_TYPE=Release
      cmake --build build/Release
    elif [ "$CONFIG" == "Debug" ]; then
      cmake -B build/Debug -S . -G "Ninja" \
        -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
        -DVCPKG_TARGET_TRIPLET=x64-windows-static \
        -DCMAKE_BUILD_TYPE=Debug
      cmake --build build/Debug
    else
      echo "Usage: build.sh [Ninja] [Debug|Release] [win32|unix]"
    fi

  elif [ "$PLATFORM" == "unix" ]; then
    echo "Bootstrap vcpkg and install deps"
    ./external/vcpkg/bootstrap-vcpkg.sh
    ./external/vcpkg/vcpkg install raylib imgui --triplet x64-linux

    echo "Setting up project structure for Ninja on Unix"
    if [ "$CONFIG" == "Release" ]; then
      cmake -B build/Release -S . -G "Ninja" \
        -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
        -DVCPKG_TARGET_TRIPLET=x64-linux \
        -DCMAKE_BUILD_TYPE=Release
      cmake --build build/Release
    elif [ "$CONFIG" == "Debug" ]; then
      cmake -B build/Debug -S . -G "Ninja" \
        -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
        -DVCPKG_TARGET_TRIPLET=x64-linux \
        -DCMAKE_BUILD_TYPE=Debug
      cmake --build build/Debug
    else
      echo "Usage: build.sh [Ninja] [Debug|Release] [win32|unix]"
    fi

  else
    echo "Usage: build.sh [Ninja] [Debug|Release] [win32|unix]"
  fi

elif [ "$SYSTEM" == "msvc" ]; then
  echo "Bootstrap vcpkg and install deps"
  ./external/vcpkg/bootstrap-vcpkg.bat
  ./external/vcpkg/vcpkg.exe install raylib imgui --triplet x64-windows-static

  echo "Setting up project structure for MSVC"
  cmake -B build -S . -G "Visual Studio 17 2022" \
    -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=x64-windows-static

  if [ "$CONFIG" == "Debug" ]; then
    cmake --build build --config Debug
  elif [ "$CONFIG" == "Release" ]; then
    cmake --build build --config Release
  else
    echo "Usage: build.sh [msvc] [Debug|Release]"
  fi

else
  echo "Usage: build.sh [Ninja|msvc] [Debug|Release] [win32|unix]"
fi

read -p "Press Enter to continue" </dev/tty

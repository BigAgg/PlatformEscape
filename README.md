# PlatformEscape
**PlatformEscape** is a little Streaming Project for myself on [Twitch](https://www.twitch.tv/atarianc).

## About the Game
The Game aims to be a puzzle jump and run where you have to get through stages to get to the end.\
It is Open Source and can be used and edited by anyone, just mention my Name and dont sell it as your product.

## Technical
It is written in C++ using [Raylib](https://www.raylib.com/), [ImGui](https://github.com/ocornut/imgui) and [rlImGui](https://github.com/raylib-extras/rlImGui).

## Cloning the repo:
```sh
git clone --recurse-submodules https://github.com/BigAgg/PlatformEscape.git
```

## Building the Game
### Windows:
***Requirements***:
- [vcpkg](https://github.com/microsoft/vcpkg)
- [CMake](https://cmake.org/) Version 3.8 or higher
- Any c++ Compiler that supports c++20 or higher like gcc

### Installing Dependencies
```sh
vcpkg install raylib imgui --triplet x64-windows
```

### Preparing the Project:
cd into the cloned repository, create a folder named *build* and cd into the *build* folder.\
**REMEMBER TO SET YOUR PATH_TO_VCPKG INSIDE -DCMAKE_TOOLCHAIN_FILE=**
```sh
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="PATH_TO_VCPKG\scripts\buildsystems\vcpkg.cmake"
```
If you are using **Visual Studio**, it generates a *.sln* inside the build directory that is ready to use.


## Resources
[Raylib](https://www.raylib.com/)
[ImGui](https://github.com/ocornut/imgui)
[rlImGui](https://github.com/raylib-extras/rlImGui)

## License
[MIT](https://mit-license.org/)

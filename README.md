# PlatformEscape
**PlatformEscape** is a little Streaming Project for myself on [Twitch](https://www.twitch.tv/atarianc).\
The Game aims to be a puzzle jump and run where you have to get through stages to get to the end.\
It is Open Source and can be used and edited by anyone, just mention my Name and dont sell it as your product.

## Technical
It is written in C++ using [Raylib](https://www.raylib.com/), [ImGui](https://github.com/ocornut/imgui) and [rlImGui](https://github.com/raylib-extras/rlImGui).

## Cloning the repo:
```sh
git clone --recurse-submodules https://github.com/BigAgg/PlatformEscape.git
cd PlatformEscape
git submodule update --init
```

## Building the Game
### Windows:
***Requirements***:
- [CMake](https://cmake.org/) Version 3.8 or higher
- Any c++ Compiler that supports c++20 or higher like gcc

### Linux:
***Rquirement***:
- [CMake](https://cmake.org/) Version 3.8 or higher
- Any c++ Compiler that supports c++20 or higher like gcc
- [Ninja](https://ninja-build.org/)

### Building
Inside the Projects directory open a Terminal and use the *build.sh* script to build the executable.\
Usage: build.sh [Ninja|msvc] [Debug|Release] [win32|unix]\
Example to build on Windows with msvc in Release mode:
```sh
./build.sh msvc Release win32
```
Example to build on unix with ninja in Debug mode:
```sh
./build.sh Ninja Debug unix
```
Note that the ninja executables are found inside *build/[CONFIG]/bin* while the msvc executables are found inside *build/[CONFIG]/*

## Resources
[Raylib](https://www.raylib.com/)\
[ImGui](https://github.com/ocornut/imgui)\
[rlImGui](https://github.com/raylib-extras/rlImGui)

## License
[MIT](https://mit-license.org/)

# PlatformEscape
**PlatformEscape** is a little Streaming Project for myself on [Twitch](https://www.twitch.tv/atarianc).\
The Game aims to be a puzzle jump and run where you have to get through stages to get to the end.\
It is Open Source and can be used and edited by anyone, just mention my Name and dont sell it as your product.

## Technical
It is written in C++ using [Raylib](https://www.raylib.com/), [ImGui](https://github.com/ocornut/imgui) and [rlImGui](https://github.com/raylib-extras/rlImGui).

## Cloning the repo:
```sh
git clone --recurse-submodules https://github.com/BigAgg/PlatformEscape.git
git submodule update --init
```

## Building the Game
### Windows:
***Requirements***:
- [CMake](https://cmake.org/) Version 3.8 or higher
- Any c++ Compiler that supports c++20 or higher like gcc

#### Preparing the Project:
Simply run the *setup.sh* file. It installs all Dependencies and sets up the Project.\
If you are using **Visual Studio**, it generates a *.sln* inside the build directory that is ready to use.

#### Compiling and Building using Visual Studio:
Enter the *build* directory and type following command:
```sh
cmake --build . --config Release
```
The executable can now be found inside the *Release* directory.

#### Compiling and Building using Ninja:
Enter the *build* directory and type following command:
```sh
ninja
```
The executable can now be found inside the entered *build* directory


## Resources
[Raylib](https://www.raylib.com/)\
[ImGui](https://github.com/ocornut/imgui)\
[rlImGui](https://github.com/raylib-extras/rlImGui)

## License
[MIT](https://mit-license.org/)

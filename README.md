## Uranium Engine
It is a 3D game engine written in C using OpenGL, SDL and etc. Also, if you find a problem or bug, you can tell us about it in [issues](https://github.com/dreadcraftteam/Uranium/issues).


Pull requests are welcome!

## How to Build
1. Clone repository:
```
git clone https://github.com/dreadcraftteam/Uranium.git
```
2. Install [CMake](https://cmake.org/)
3. Inside the cloned directory, enter this command in the terminal:
```
cmake -S . -B build -A Win32
```
4. Then go to the build folder and enter this command in the terminal:
```
cmake --build . --config Release
```
5. Navigate to `dist/`, there will be a compiled build of the engine.
6. Thats All!
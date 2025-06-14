## Uranium
It is an engine written in C using SDL, OpenGL, etc. Also, if you find a problem or bug, you can tell us about it in [issues](https://github.com/dreadcraftteam/Uranium/issues). It is important that at the moment the engine supports only x86 libraries and compiles only for 32-bit. 


Pull requests are welcome!

## How to Build
1. Install [Git](https://git-scm.com/downloads)
2. Clone repository:
```
git clone https://github.com/dreadcraftteam/Uranium.git
```
3. Install [CMake](https://cmake.org/)
4. Inside the cloned directory, enter this command in the terminal:
```
cmake -S . -B build -A Win32
```
5. Then go to the `build` and enter this command in the terminal:
```
cmake --build . --config Release
```
6. Navigate to `dist`, there will be a compiled build of the engine.
7. Thats all!
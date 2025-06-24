## Uranium
This is an engine written in C using SDL, OpenGL, etc. The engine is divided into several parts (launcher, game, engine) for ease of working with it. Also, if you find an error or bug, please report it to the [issues](https://github.com/dreadcraftteam/Uranium/issues) section. Currently, compilation is supported only on Linux and only on x86.


Pull requests are welcome!

## How to Build
1. Install [Git](https://git-scm.com/).
2. Clone repository:
```
git clone https://github.com/dreadcraftteam/Uranium.git
```
3. Install [CMake](https://cmake.org/).
4. Inside the cloned directory, enter this command in the terminal:
```
cmake . -DCMAKE_C_FLAGS="-m32" -DCMAKE_CXX_FLAGS="-m32" -DCMAKE_EXE_LINKER_FLAGS="-m32"
```
5. And then enter this command:
```
cmake --build . --config Release
```
6. Navigate to `dist/`, there will be a compiled build of the engine.
7. Thats All!

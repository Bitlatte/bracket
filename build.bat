if not exist build mkdir build
cd build
"C:/Program Files/CMake/bin/cmake" -S ../ -B . -G "MinGW Makefiles"
mingw32-make.exe && mingw32-make.exe Shaders
cd ..
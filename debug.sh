#!/bin/bash

MARKER_FILE=".first_compile"

if [ ! -f "$MARKER_FILE" ]; then
    echo "Installing Necessary Packages..."
    
    sudo apt update
    sudo apt install -y gcc-multilib g++-multilib cmake aptitude
    sudo aptitude install -y libsdl2-dev:i386
    
    touch "$MARKER_FILE"
    echo "Installation Complete!"
fi

if [ ! -d "build" ]; then
    echo "Build folder Not Found! Creating Project..."
    cmake -B build -DCMAKE_C_FLAGS="-m32" -DCMAKE_CXX_FLAGS="-m32" -DCMAKE_EXE_LINKER_FLAGS="-m32"
else
    echo "Build folder Found! Building Project!"
fi

echo "Building Project..."
cmake --build build --config Debug

echo "Build Complete!"

#!/bin/bash

YELLOW='\033[1;33m'
NC='\033[0m' 

MARKER_FILE=".first_compile"

if [ ! -f "$MARKER_FILE" ]; then
    echo -e  "${YELLOW}Installing Necessary Packages...${NC}"
    
    sudo apt update
    sudo apt install -y gcc-multilib g++-multilib cmake aptitude
    sudo aptitude install -y libsdl2-dev:i386
    
    touch "$MARKER_FILE"
    echo -e "${YELLOW}Installation Complete!${NC}"
fi

if [ ! -d "build" ]; then
    echo -e "${YELLOW}Build folder Not Found! Creating Project...${NC}"
    cmake -B build -DCMAKE_C_FLAGS="-m32" -DCMAKE_CXX_FLAGS="-m32" -DCMAKE_EXE_LINKER_FLAGS="-m32"
else
    echo -e "${YELLOW}Build folder Found! Building Project!${NC}"
fi

echo -e "${YELLOW}Building Project...${NC}"
cmake --build build --config Release

echo "${YELLOW}Build Complete!${NC}"

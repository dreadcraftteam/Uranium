#!/bin/bash

YELLOW='\033[1;33m'
NC='\033[0m' 

echo -e  "${YELLOW}Checking Necessary Packages...${NC}"
    
sudo apt update
sudo apt install -y gcc-multilib g++-multilib cmake libsdl2-dev libglew-dev freeglut3-dev
    
echo -e "${YELLOW}Installation Complete!${NC}"

if [ ! -d "build" ]; then
    echo -e "${YELLOW}Build folder Not Found! Creating Project...${NC}"
    cmake -B build
else
    echo -e "${YELLOW}Build folder Found! Building Project!${NC}"
fi

echo -e "${YELLOW}Building Project...${NC}"
cmake --build build --config Release

echo -e "${YELLOW}Build Complete!${NC}"

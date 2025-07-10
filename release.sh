#!/bin/bash

YELLOW='\033[1;33m'
NC='\033[0m' 

if [ ! -d "build" ]; then
    echo -e "${YELLOW}Build folder Not Found! Configuring Project...${NC}"
    cmake -B build
else
    echo -e "${YELLOW}Build folder Found! Building Project!${NC}"
fi

echo -e "${YELLOW}Building Project...${NC}"
cmake --build build --config Release

echo -e "${YELLOW}Build Complete!${NC}"

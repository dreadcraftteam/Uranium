#!/bin/bash

# Configuring project
cmake -B cmake

# Building project
cmake --build cmake --config Release

# Removing folder with shit
sudo rm -rf cmake

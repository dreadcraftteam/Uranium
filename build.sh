#!/bin/bash

# Yes, its very strange :)
cmake --build cmake --config Release
sudo rm -rf cmake

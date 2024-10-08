#!/bin/bash

# Compile C++ files with g++
g++ -Wall -std=c++11 *.cpp

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! :3"
else
    echo "Compilation failed. :O"
fi

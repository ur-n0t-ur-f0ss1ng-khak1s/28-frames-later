#!/bin/bash

# Compile C++ files with g++
g++ -Wall -w -std=c++11 *.cpp -lSDL -lGL -lGLU

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! :3"
else
    echo "Compilation failed. :O"
fi

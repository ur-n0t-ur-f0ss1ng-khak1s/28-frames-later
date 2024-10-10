#!/bin/bash

# Check if an argument (filename) is passed
if [ -z "$1" ]; then
    # No argument provided, compile all .cpp files
    g++ -Wall -w -std=c++20 *.cpp -lSDL -lGL -lGLU

    # Check if the compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compilation successful! :3"
    else
        echo "Compilation failed. :O"
    fi
else
    # Argument provided, compile the specific file
    g++ -Wall -w -std=c++20 -c "$1"

    # Check if the compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compilation of $1 successful! :3"
    else
        echo "Compilation of $1 failed. :O"
    fi
fi


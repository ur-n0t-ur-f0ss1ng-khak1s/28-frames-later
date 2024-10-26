#!/bin/bash
# For debugging: g++ -g -Wall -w -std=c++20 *.cpp -lSDL2 -lSDL2_image -lGL -lGLU

start=$(date +%s)
# Check if an argument (filename) is passed
if [ -z "$1" ]; then
  echo "removing all files ending in .o"
  rm -iv ./*.o
  rm -v ./objreport.txt # Optional but I want to save storage on my desktop
  # No argument provided, compile all .cpp files
  g++ -Wall -w -std=c++20 *.cpp -I/usr/include/SDL2 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGL -lGLU

  # Check if the compilation was successful
  if [ $? -eq 0 ]; then
    end=$(date +%s)
    echo "succeeded in $((end - start)) seconds :3"
  else
    end=$(date +%s)
    echo "failed in $((end - start)) seconds :O"
  fi
else
  # Argument provided, compile the specific file
  g++ -Wall -w -std=c++20 -c "$1" -I/usr/include/SDL2 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGL -lGLU

  # Check if the compilation was successful
  if [ $? -eq 0 ]; then
    end=$(date +%s)
    echo "Compilation of $1 succeeded in $((end - start)) seconds :3"
  else
    end=$(date +%s)
    echo "Compilation of $1 failed in $((end - start)) seconds :O"
  fi
fi

#by yours truely, ross 

#!/bin/bash

clear
rm -r ./build
mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cppcheck --project=compile_commands.json
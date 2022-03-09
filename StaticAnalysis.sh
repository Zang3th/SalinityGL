#!/bin/bash

clear
mkdir Build_StaticAnalysis
cd Build_StaticAnalysis
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cppcheck --project=compile_commands.json

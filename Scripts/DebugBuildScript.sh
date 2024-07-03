#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/..")
cd "$PROJECT_ROOT" || exit

rm -f -r Debug_Build/
mkdir Debug_Build
cd Debug_Build
cmake .. -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS="-g"
make -j

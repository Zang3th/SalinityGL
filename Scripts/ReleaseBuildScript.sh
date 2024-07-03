#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/..")
cd "$PROJECT_ROOT" || exit

rm -f -r Release_Build/
mkdir Release_Build
cd Release_Build
cmake .. -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_FLAGS="-O3 -Wno-unused-but-set-variable -Wno-unused-result"
make -j

#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/..")
cd "$PROJECT_ROOT" || exit

clear
cppcheck Apps/ Engine/ --enable=all --suppress=cstyleCast --suppress=unusedFunction --suppress=missingInclude --suppress=unknownMacro

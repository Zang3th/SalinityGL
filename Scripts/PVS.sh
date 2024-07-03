#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/..")
cd "$PROJECT_ROOT" || exit

GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${GREEN}Running PVS-Studio analysis...${NC}"
pvs-studio-analyzer analyze -f Debug_Build/compile_commands.json -j -e Vendor/

echo -e "${GREEN}Converting PVS-Studio log to HTML...${NC}"
plog-converter -t fullhtml -o PVS-Report PVS-Studio.log

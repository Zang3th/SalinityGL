#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/..")
cd "$PROJECT_ROOT" || exit

loc_engine=$(find ./Engine -type f | xargs cat | wc -l)
echo "Lines in ./Engine: $loc_engine"
loc_apps=$(find ./Apps -type f | xargs cat | wc -l)
echo "Lines in ./Apps: $loc_apps"

#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
PROJECT_ROOT=$(realpath "$SCRIPT_DIR")
cd "$PROJECT_ROOT" || exit

if [ -z "$1" ]; then
  echo "Fehler: Kein Argument übergeben. Bitte als erstes Argument die PID angeben." >&2
  exit 1
fi

perf record -F 500 -p $1 --call-graph dwarf sleep 5
hotspot perf.data

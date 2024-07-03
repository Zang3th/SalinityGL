#!/bin/bash

loc_engine=$(find ./Engine -type f | xargs cat | wc -l)
echo "Lines in ./Engine: $loc_engine"
loc_apps=$(find ./Apps -type f | xargs cat | wc -l)
echo "Lines in ./Apps: $loc_apps"

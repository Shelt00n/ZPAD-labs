#!/bin/bash
set -e

if [ ! -f build/lab6_opencv ]; then
    echo "Executable not found. Running build first..."
    ./build.sh
fi

./build/lab6_opencv

#!/bin/bash
set -e

if [ ! -f models/deploy.prototxt ] || [ ! -f models/res10_300x300_ssd_iter_140000.caffemodel ]; then
    echo "Model files not found. Run ./preinstall.sh first."
    exit 1
fi

if [ ! -f build/lab7_threads ]; then
    ./build.sh
fi

./build/lab7_threads

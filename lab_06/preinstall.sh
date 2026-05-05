#!/bin/bash
set -e

echo "Installing build tools and OpenCV..."
sudo apt update
sudo apt install -y libopencv-dev cmake g++ make

echo "Done. Now run ./build.sh"

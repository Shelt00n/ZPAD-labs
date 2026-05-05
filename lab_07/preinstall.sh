#!/bin/bash
set -e

sudo apt update
sudo apt install -y libopencv-dev cmake g++ make wget

mkdir -p models
wget -O models/deploy.prototxt \
  https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -O models/res10_300x300_ssd_iter_140000.caffemodel \
  https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "Dependencies and model files are ready."

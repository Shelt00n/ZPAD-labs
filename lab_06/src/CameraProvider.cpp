#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int cameraIndex) {
    capture.open(cameraIndex);
}

bool CameraProvider::isOpened() const {
    return capture.isOpened();
}

bool CameraProvider::getFrame(cv::Mat& frame) {
    return capture.read(frame);
}

#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int cameraIndex) {
    cap.open(cameraIndex);
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

bool CameraProvider::read(cv::Mat& frame) {
    return cap.read(frame);
}

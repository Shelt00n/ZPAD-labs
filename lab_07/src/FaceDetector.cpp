#include "FaceDetector.hpp"
#include <chrono>
#include <iostream>

FaceDetector::FaceDetector(const std::string& modelConfig, const std::string& modelWeights)
    : running(false), frameReady(false) {
    net = cv::dnn::readNetFromCaffe(modelConfig, modelWeights);
}

FaceDetector::~FaceDetector() {
    stop();
}

void FaceDetector::start() {
    running = true;
    worker = std::thread(&FaceDetector::workerLoop, this);
}

void FaceDetector::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}

void FaceDetector::submitFrame(const cv::Mat& frame) {
    if (frame.empty()) return;
    std::lock_guard<std::mutex> lock(frameMutex);
    latestFrame = frame.clone();
    frameReady = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(faceMutex);
    return currentFaces;
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (frameReady && !latestFrame.empty()) {
                frame = latestFrame.clone();
                frameReady = false;
            }
        }

        if (frame.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }

        cv::Mat blob = cv::dnn::blobFromImage(
            frame,
            1.0,
            cv::Size(300, 300),
            cv::Scalar(104.0, 177.0, 123.0),
            false,
            false
        );

        net.setInput(blob);
        cv::Mat detections = net.forward();
        cv::Mat matrix(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

        std::vector<cv::Rect> found;
        for (int i = 0; i < matrix.rows; ++i) {
            float confidence = matrix.at<float>(i, 2);
            if (confidence < 0.50f) continue;

            int x1 = static_cast<int>(matrix.at<float>(i, 3) * frame.cols);
            int y1 = static_cast<int>(matrix.at<float>(i, 4) * frame.rows);
            int x2 = static_cast<int>(matrix.at<float>(i, 5) * frame.cols);
            int y2 = static_cast<int>(matrix.at<float>(i, 6) * frame.rows);

            cv::Rect box(cv::Point(x1, y1), cv::Point(x2, y2));
            found.push_back(box & cv::Rect(0, 0, frame.cols, frame.rows));
        }

        {
            std::lock_guard<std::mutex> lock(faceMutex);
            currentFaces = found;
        }
    }
}

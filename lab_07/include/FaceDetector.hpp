#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class FaceDetector {
public:
    FaceDetector(const std::string& modelConfig, const std::string& modelWeights);
    ~FaceDetector();

    void start();
    void stop();
    void submitFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();

private:
    void workerLoop();

    cv::dnn::Net net;
    std::thread worker;
    std::mutex frameMutex;
    std::mutex faceMutex;
    std::atomic<bool> running;
    std::atomic<bool> frameReady;
    cv::Mat latestFrame;
    std::vector<cv::Rect> currentFaces;
};

#endif

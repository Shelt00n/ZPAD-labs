#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class FrameProcessor {
public:
    cv::Mat draw(const cv::Mat& frame, const std::vector<cv::Rect>& faces, double fps, int frameCounter);
};

#endif

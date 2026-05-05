#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    FrameProcessor();
    cv::Mat process(const cv::Mat& frame, ProcessingMode mode, int blurSize, int frameCounter, double fps);

private:
    cv::Mat addInfoText(const cv::Mat& frame, ProcessingMode mode, int frameCounter, double fps);
    const char* modeName(ProcessingMode mode) const;
};

#endif

#include "FrameProcessor.hpp"
#include <string>

FrameProcessor::FrameProcessor() = default;

cv::Mat FrameProcessor::process(const cv::Mat& frame, ProcessingMode mode, int blurSize, int frameCounter, double fps) {
    cv::Mat result;

    switch (mode) {
        case ProcessingMode::Original:
            result = frame.clone();
            break;
        case ProcessingMode::Invert:
            cv::bitwise_not(frame, result);
            break;
        case ProcessingMode::Gray:
            cv::cvtColor(frame, result, cv::COLOR_BGR2GRAY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        case ProcessingMode::Blur:
            cv::GaussianBlur(frame, result, cv::Size(blurSize, blurSize), 0);
            break;
        case ProcessingMode::Canny: {
            cv::Mat gray, edges;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, edges, 70, 160);
            cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessingMode::Sobel: {
            cv::Mat gray, gradX, gradY, absX, absY;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, gradX, CV_16S, 1, 0, 3);
            cv::Sobel(gray, gradY, CV_16S, 0, 1, 3);
            cv::convertScaleAbs(gradX, absX);
            cv::convertScaleAbs(gradY, absY);
            cv::addWeighted(absX, 0.5, absY, 0.5, 0, result);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessingMode::Binary: {
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::threshold(gray, result, 120, 255, cv::THRESH_BINARY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessingMode::Cartoon: {
            cv::Mat smooth, gray, edges;
            cv::bilateralFilter(frame, smooth, 9, 75, 75);
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::medianBlur(gray, gray, 7);
            cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 2);
            cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
            cv::bitwise_and(smooth, edges, result);
            break;
        }
        case ProcessingMode::TextOverlay:
        case ProcessingMode::SaveFrame:
            result = frame.clone();
            break;
    }

    return addInfoText(result, mode, frameCounter, fps);
}

cv::Mat FrameProcessor::addInfoText(const cv::Mat& frame, ProcessingMode mode, int frameCounter, double fps) {
    cv::Mat output = frame.clone();
    std::string line1 = "Mode: " + std::string(modeName(mode));
    std::string line2 = "Frames: " + std::to_string(frameCounter) + " | FPS: " + std::to_string(static_cast<int>(fps));
    std::string line3 = "Keys: 0-original 1-invert 2-gray 3-blur 4-canny 5-sobel 6-binary 7-cartoon S-save Q/ESC-exit";

    cv::putText(output, line1, cv::Point(15, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    cv::putText(output, line2, cv::Point(15, 60), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
    cv::putText(output, line3, cv::Point(15, output.rows - 20), cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(255, 255, 255), 1);
    return output;
}

const char* FrameProcessor::modeName(ProcessingMode mode) const {
    switch (mode) {
        case ProcessingMode::Original: return "Original";
        case ProcessingMode::Invert: return "Invert colors";
        case ProcessingMode::Gray: return "Grayscale";
        case ProcessingMode::Blur: return "Gaussian blur";
        case ProcessingMode::Canny: return "Canny edges";
        case ProcessingMode::Sobel: return "Sobel filter";
        case ProcessingMode::Binary: return "Binary threshold";
        case ProcessingMode::Cartoon: return "Cartoon effect";
        case ProcessingMode::TextOverlay: return "Text overlay";
        case ProcessingMode::SaveFrame: return "Save frame";
    }
    return "Unknown";
}

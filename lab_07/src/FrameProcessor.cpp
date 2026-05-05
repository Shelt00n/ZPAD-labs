#include "FrameProcessor.hpp"
#include <string>

cv::Mat FrameProcessor::draw(const cv::Mat& frame, const std::vector<cv::Rect>& faces, double fps, int frameCounter) {
    cv::Mat output = frame.clone();

    for (const auto& face : faces) {
        cv::rectangle(output, face, cv::Scalar(0, 255, 0), 2);
        cv::putText(output, "face", cv::Point(face.x, std::max(20, face.y - 8)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    }

    std::string info = "Mode: threaded face detection | FPS: " + std::to_string(static_cast<int>(fps)) +
                       " | frames: " + std::to_string(frameCounter) +
                       " | faces: " + std::to_string(faces.size());
    cv::putText(output, info, cv::Point(15, 30), cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 255, 255), 2);
    cv::putText(output, "Q/ESC - exit | S - save frame", cv::Point(15, output.rows - 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(255, 255, 255), 1);

    return output;
}

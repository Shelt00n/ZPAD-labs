#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>

#include "CameraProvider.hpp"
#include "FaceDetector.hpp"
#include "FrameProcessor.hpp"

int main() {
    const std::string config = "models/deploy.prototxt";
    const std::string weights = "models/res10_300x300_ssd_iter_140000.caffemodel";

    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Camera is not available" << std::endl;
        return 1;
    }

    FaceDetector detector(config, weights);
    detector.start();
    FrameProcessor painter;

    const std::string windowName = "Lab 7 - Threaded Face Detector";
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

    int frameCounter = 0;
    auto start = std::chrono::steady_clock::now();

    while (true) {
        cv::Mat frame;
        if (!camera.read(frame) || frame.empty()) {
            std::cerr << "Cannot read frame from camera" << std::endl;
            break;
        }

        frameCounter++;
        detector.submitFrame(frame);
        std::vector<cv::Rect> faces = detector.getFaces();

        auto now = std::chrono::steady_clock::now();
        double seconds = std::chrono::duration<double>(now - start).count();
        double fps = seconds > 0 ? frameCounter / seconds : 0.0;

        cv::Mat output = painter.draw(frame, faces, fps, frameCounter);
        cv::imshow(windowName, output);

        int key = cv::waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
        if (key == 's' || key == 'S') {
            std::string name = "face_frame_" + std::to_string(frameCounter) + ".png";
            cv::imwrite(name, output);
            std::cout << "Saved: " << name << std::endl;
        }
    }

    detector.stop();
    cv::destroyAllWindows();
    return 0;
}

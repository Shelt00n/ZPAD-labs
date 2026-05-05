#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Error: camera is not available" << std::endl;
        return 1;
    }

    KeyProcessor keys;
    FrameProcessor processor;
    Display display("Laboratory work 6 - OpenCV");

    int frameCounter = 0;
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        cv::Mat frame;
        if (!camera.getFrame(frame) || frame.empty()) {
            std::cerr << "Error: cannot read frame" << std::endl;
            break;
        }

        frameCounter++;
        auto now = std::chrono::steady_clock::now();
        double seconds = std::chrono::duration<double>(now - startTime).count();
        double fps = seconds > 0 ? frameCounter / seconds : 0;

        cv::Mat output = processor.process(frame, keys.getMode(), keys.getBlurSize(), frameCounter, fps);
        display.show(output);

        if (keys.shouldSave()) {
            std::string filename = "frame_" + std::to_string(frameCounter) + ".png";
            cv::imwrite(filename, output);
            std::cout << "Saved: " << filename << std::endl;
            keys.clearSaveFlag();
        }

        int key = cv::waitKey(1);
        keys.handleKey(key);
        if (keys.shouldQuit()) break;
    }

    cv::destroyAllWindows();
    return 0;
}

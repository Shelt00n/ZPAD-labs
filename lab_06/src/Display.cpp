#include "Display.hpp"

Display::Display(const std::string& windowName) : window(windowName) {
    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& image) {
    cv::imshow(window, image);
}

const std::string& Display::name() const {
    return window;
}

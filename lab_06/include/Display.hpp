#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
public:
    explicit Display(const std::string& windowName = "Lab 6 OpenCV");
    void show(const cv::Mat& image);
    const std::string& name() const;

private:
    std::string window;
};

#endif

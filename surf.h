#ifndef SURF_H
#define SURF_H
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

cv::RotatedRect surfTemplateMatching(cv::Mat& img_object, cv::Mat& img_scene);

#endif

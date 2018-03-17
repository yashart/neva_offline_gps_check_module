#ifndef PHOTO_INFO_H
#define PHOTO_INFO_H
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

struct PhotoInfo {
    char name[30];
    int offsetX;
    int offsetY;
    float angle;
    float xOnLatScaling;
};

PhotoInfo calc_photo_info(cv::Mat& im0Init, cv::Mat& im1Init, PhotoInfo photoInfo0, char* photo1name);
void photo_info_dump(PhotoInfo info);

#endif

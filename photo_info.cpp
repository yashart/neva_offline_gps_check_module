#include <stdio.h>
#include <math.h>
#include <string.h>
#include "photo_info.h"
#include "surf.h"

using namespace cv;

PhotoInfo calc_photo_info(cv::Mat& im0Init, cv::Mat& im1Init, PhotoInfo photoInfo0, char* photo1name) {
    Mat im0 = im0Init.clone();
    Mat im1 = im1Init.clone();

    RotatedRect rr = surfTemplateMatching(im1, im0);
    PhotoInfo newPhoto;
    int rrOffsetX = round(rr.center.x - 1920.0/2);
    int rrOffsetY = round(rr.center.y - 1080.0/2);

    strcpy(newPhoto.name, photo1name);
    newPhoto.offsetX = photoInfo0.offsetX + round(
    rrOffsetX * cos(photoInfo0.angle/180.0*M_PI) +
    rrOffsetY * sin(photoInfo0.angle/180.0*M_PI));
    newPhoto.offsetY = photoInfo0.offsetY +round(
    - rrOffsetX * sin(photoInfo0.angle/180.0*M_PI) +
    rrOffsetY * cos(photoInfo0.angle/180.0*M_PI));
    newPhoto.angle = photoInfo0.angle - rr.angle;
    newPhoto.xOnLatScaling = rr.size.width * rr.size.height;

    return newPhoto;
}

void photo_info_dump(PhotoInfo info) {
    printf("name: %s\n", info.name);
    printf("offsetX: %d\n", info.offsetX);
    printf("offsetY: %d\n", info.offsetY);
    printf("angle: %f\n", info.angle);
    printf("xOnLatScaling: %f\n", info.xOnLatScaling);

}

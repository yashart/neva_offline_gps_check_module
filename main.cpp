#include "photo_info.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "math.h"
#include "kor.h"
#include "drone_angle_header.h"

using namespace cv;

void convert_decimal_time_to_text(char* newText, int time);

int main() {
    Kor kor;
    kor_parce_ini(&kor);
    kor_dump(&kor);
    int firstImgCounter = (kor.firstPointTime - kor.startVideoTime)/100; //in 10-th msec
    Mat curr, prev;
    char fileName[120];
    char coordTxtName[120];
    char coordTxtOffsetsName[120];
    char latLonTxtName[120];
    char timeName[100];
    bool isFirstTick = 1;
    sprintf(coordTxtName, "%s/coord.txt", kor.folderName);
    sprintf(coordTxtOffsetsName, "%s/offsets.txt", kor.folderName);
    sprintf(latLonTxtName, "%s/latlon.txt", kor.folderName);

    DroneAngleHeader droneAngleHeader = read_angles();

    FILE* coordTxt = fopen(coordTxtName, "w");
    FILE* coordTxtOffsets = fopen(coordTxtOffsetsName, "w");
    FILE* latLonTxt = fopen(latLonTxtName, "w");

    PhotoInfo photoInfo;
    photoInfo.offsetX = 0;
    photoInfo.offsetY = 0;
    photoInfo.angle = 0;
    photoInfo.xOnLatScaling = 1920 * 1080;

    double mLat = kor.lat;
    double mLon = kor.lon;
    double mYaw = kor.yaw;
    double mHeight = kor.alt;
    double pitch;
    double prevPitch;
    double roll;
    double prevRoll;
    double yaw;
    double prevYaw;

    for (float i = 0; i < kor.duration; i += kor.framePerSeconds) {
        sprintf(fileName, "%s/%06d.jpg", kor.folderName, ( firstImgCounter + (int)(10 * i)));
        printf("fileName %s\n", fileName);
        int currTime = (kor.firstPointTime + (int)(1000 * i));
        photoInfo.angle = -1 * get_angels_by_time(currTime, droneAngleHeader).yaw;
        yaw = photoInfo.angle;
        pitch = get_angels_by_time(currTime, droneAngleHeader).pitch;
        roll = get_angels_by_time(currTime, droneAngleHeader).roll;
        printf("yaw: %lf \n", photoInfo.angle);
        printf("pitch: %lf \n", pitch);
        printf("roll: %lf \n", roll);

        convert_decimal_time_to_text(timeName, currTime);
        if (isFirstTick) {
            isFirstTick = 0;
            prevPitch = pitch;
            prevRoll = roll;
            prevYaw = yaw;
            prev = imread(fileName);
            fprintf(coordTxt, "%s %8lf %8lf %lf\n", timeName, mLat, mLon, mHeight);
            fprintf(coordTxtOffsets, "%s %8lf %8lf %lf 0 0\n",
                    timeName, mLat, mLon, mHeight);

            continue;
        }
        if (abs(yaw - prevYaw) > 1.5) {
            prevYaw = yaw;
            prevRoll = roll;
            prevPitch = pitch;
            continue;
        }
        curr = imread(fileName);
        float oldSquare = photoInfo.xOnLatScaling;
        try {
            photoInfo = calc_photo_info(prev, curr, photoInfo, "0");
        } catch(...) {
            prev = curr.clone();
            continue;
        }
        /*
        mLat -= (kor.calibLat * photoInfo.offsetY +
            sin((pitch - prevPitch)/180.0*M_PI) *
            cos(yaw/180.0*M_PI) * 1080.0 -
            sin((roll - prevRoll)/180.0*M_PI) *
            sin(yaw/180.0*M_PI) * 1080.0) *
            mHeight / 111132.0 / 1080.0;
        */
        mLat -= kor.calibLat * photoInfo.offsetY * mHeight / 111132.0 / 1080.0;
        /*
        mLon += (kor.calibLon * photoInfo.offsetX +
            sin((pitch - prevPitch)/180.0*M_PI) *
            sin(yaw/180.0*M_PI)  * 1920.0 -
            sin((roll - prevRoll)/180.0*M_PI) *
            cos(yaw/180.0*M_PI)  * 1920.0) *
            mHeight / 78847.0 / 1920.0;
        */
        mLon += kor.calibLon * photoInfo.offsetX * mHeight / 78847.0 / 1920.0;
        mHeight = mHeight * sqrt(float(oldSquare)) / sqrt(float(photoInfo.xOnLatScaling));
        mYaw = -1 * photoInfo.angle;

        fprintf(coordTxt, "%s %8lf %8lf %lf\n", timeName, mLat, mLon, mHeight);
        fprintf(coordTxtOffsets, "%s %8lf %8lf %lf %d %d\n",
                timeName, mLat, mLon, mHeight, photoInfo.offsetX, photoInfo.offsetY);
        fprintf(latLonTxt, "%8lf\t%8lf\n", mLat, mLon);


        photoInfo.offsetX = 0;
        photoInfo.offsetY = 0;
        prevPitch = pitch;
        prevRoll = roll;
        prevYaw = yaw;

        prev = curr.clone();
    }

    return 0;
}

void convert_decimal_time_to_text(char* newText, int currTime) {
    int currTime2 = currTime;
    int minutes = currTime/60000;
    currTime2 = currTime2 - floor(currTime2/60000)*60000;
    int seconds = currTime2/1000;
    currTime2 = currTime2 - floor(currTime2/1000)*1000;

    int msec = currTime2;
    sprintf(newText, "0:%d:%d.%d", minutes, seconds, msec);
}

#include "drone_angle_header.h"
#include "drone_angles.h"
#include "stdio.h"
#include <stdlib.h>


DroneAngleHeader read_angles() {
    DroneAngleHeader droneAngleHeader;
    droneAngleHeader.angles = (DroneAngle*) calloc (10000, sizeof(*(droneAngleHeader.angles)));
    FILE* textAngles = fopen("angles.txt", "r");
    int minutes;
    int seconds;
    int mseconds;
    int mtime;
    double roll;
    double pitch;
    double yaw;
    int i;
    for (i = 0; fscanf(textAngles, "00:%d:%d.%d %lf %lf %lf\n",
                            &minutes, &seconds, &mseconds, &roll, &pitch, &yaw) != EOF ;i++) {
        mtime = mseconds * 10 + seconds * 1000 + minutes * 1000 * 60;
        droneAngleHeader.angles[i] = drone_angle_ctor(mtime, roll, pitch, yaw);
    }
    droneAngleHeader.count = i;
    droneAngleHeader.angles = (DroneAngle*) realloc(droneAngleHeader.angles,
                                droneAngleHeader.count * sizeof(*(droneAngleHeader.angles)));
    return droneAngleHeader;
}

DroneAngle get_angels_by_time(int timeMs, DroneAngleHeader droneAngleHeader) {
    int i;
    for (i = 0; droneAngleHeader.angles[i].time_ms < timeMs; i++);
    return droneAngleHeader.angles[i];
}

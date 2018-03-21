#ifndef DRONE_ANGLE_HEADER_H
#define DRONE_ANGLE_HEADER_H

#include "drone_angles.h"

struct DroneAngleHeader {
    DroneAngle* angles;
    int count;
};

DroneAngleHeader read_angles();
DroneAngle get_angels_by_time(int timeMs, DroneAngleHeader droneAngleHeader);

#endif

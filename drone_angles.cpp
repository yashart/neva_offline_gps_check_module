#include "drone_angles.h"

DroneAngle drone_angle_ctor(int time_ms, double roll, double pitch,
                            double yaw) {
    DroneAngle droneAngle;
    droneAngle.time_ms = time_ms;
    droneAngle.roll = roll;
    droneAngle.pitch = pitch;
    droneAngle.yaw = yaw;
    return droneAngle;
}

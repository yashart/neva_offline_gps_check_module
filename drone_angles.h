#ifndef DRONE_ANGLES_H
#define DRONE_ANGLES_H

struct DroneAngle {
    int time_ms;
    double roll;
    double pitch;
    double yaw;
};

DroneAngle drone_angle_ctor(int time_ms, double roll, double pitch,
                            double yaw);

#endif

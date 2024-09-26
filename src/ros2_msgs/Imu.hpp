// This is a message to hold data from an IMU (Inertial Measurement Unit)
//
// Accelerations should be in m/s^2 (not in g's), and rotational velocity should be in rad/sec
//
// If the covariance of the measurement is known, it should be filled in (if all you know is the 
// variance of each measurement, e.g. from the datasheet, just put those along the diagonal)
// A covariance matrix of all zeros will be interpreted as "covariance unknown", and to use the
// data a covariance will have to be assumed or gotten from some other source
//
// If you have no estimate for one of the data elements (e.g. your IMU doesn't produce an orientation 
// estimate), please set element 0 of the associated covariance matrix to -1
// If you are interpreting this message, please check for a value of -1 in the first element of each 
// covariance matrix, and disregard the associated estimate.

// [日本語訳](https://qiita.com/akinami/items/60695bd059071cd10f17)

#pragma once

#include "ROS2_msgs.hpp"

namespace sensor_msgs
{
    namespace msg
    {
        typedef struct 
        {
            std_msgs::Header header;
            geometry_msgs::msg::Quaternion orientation;
            double orientation_covariance[9];

            geometry_msgs::msg::Vector3 angular_velocity;
            double angular_velocity_covariance[9];

            geometry_msgs::msg::Vector3 linear_acceleration;
            double linear_acceleration_covariance[9];
        }Imu;
    } // namespace msg
    
} // namespace sensor_msgs

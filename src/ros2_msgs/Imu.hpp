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
            void set_OrientationCovariance(const double (&a)[9]){
                std::copy(&a[0], &a[0]+9, orientation_covariance);
            }
            
            geometry_msgs::msg::Vector3 angular_velocity;
            double angular_velocity_covariance[9];
            void set_AngleVelCovariance(const double (&a)[9]){
                std::copy(&a[0], &a[0]+9, angular_velocity_covariance);
            }

            geometry_msgs::msg::Vector3 linear_acceleration;
            double linear_acceleration_covariance[9];
            void set_LinearAccelCovariance(const double (&a)[9]){
                std::copy(&a[0], &a[0]+9, linear_acceleration_covariance);
            }

            // シリアライザ
            void serialize(uint8_t* data) {
                header.serialize(data);
                memcpy(data + header.SIZE_HEADER, &orientation, 8U * 4);
                memcpy(data + header.SIZE_HEADER + 32U, &orientation_covariance, 8U * 9);
                memcpy(data + header.SIZE_HEADER + 104U, &angular_velocity, 8U * 3);
                memcpy(data + header.SIZE_HEADER + 128U, &angular_velocity_covariance, 8U * 9);
                memcpy(data + header.SIZE_HEADER + 200U, &linear_acceleration, 8U * 3);
                memcpy(data + header.SIZE_HEADER + 224U, &linear_acceleration_covariance, 8U * 9);
            }

            // デシリアライザ
            void deserialize(const uint8_t* data, size_t* size) {
                // header.deserialize(data, header.SIZE_HEADER);
                // memcpy(&orientation, data+header.SIZE_HEADER, 8U*4);
                // memcpy(&b, data + sizeof(int), sizeof(float));
                // memcpy(&c, data + sizeof(int) + sizeof(float), sizeof(char));
                // *size = header.SIZE_HEADER + 296U;
            };
        }Imu;

        typedef struct 
        {
            std_msgs::Header header;
            geometry_msgs::msg::Quaternion orientation;
            geometry_msgs::msg::Vector3 angular_velocity;
            geometry_msgs::msg::Vector3 linear_acceleration;

            // シリアライザ
            void serialize(uint8_t* data) {
                header.serialize(data);
                memcpy(data + header.SIZE_HEADER, &orientation, 8U * 4);
                memcpy(data + header.SIZE_HEADER + 32U, &angular_velocity, 8U * 3);
                memcpy(data + header.SIZE_HEADER + 56U, &linear_acceleration, 8U * 3);
            }

            // デシリアライザ
            void deserialize(const uint8_t* data, size_t* size) {
                const uint8_t *data_ptr = data;
                header.deserialize(data_ptr, header.SIZE_HEADER);
                memcpy(&orientation, data + header.SIZE_HEADER, 8U * 4);
                memcpy(&angular_velocity, data + header.SIZE_HEADER + 32U, 8U * 3);
                memcpy(&linear_acceleration, data + header.SIZE_HEADER + 56U, 8U * 3);
                *size = header.SIZE_HEADER + 296U;
            };
        }ImuwithoutCovariance;
    } // namespace msg
    
} // namespace sensor_msgs

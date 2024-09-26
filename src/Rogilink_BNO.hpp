#pragma once

#include "mbed.h"
#include "RogiLinkFlex/UartLink.hpp"
#include "ros2_msgs/Imu.hpp"
#include "BNO055.hpp"

#include <vector>
#include <string>


class Rogilink_BNO
{
private:
    UartLink uart;

    UartLinkSubscriber<int, char*> sub;
    UartLinkPublisher<sensor_msgs::msg::Imu> pub;

    std::unique_ptr<IMU::BNO055> bno;

    int rate = 0;// imu output rate [Hz]
    bool reset_flag = false;
    bool is_ready = false;

    void sub_callback(int _length, char* _cmd);

    void pub_callback();

public:
    Rogilink_BNO();
    ~Rogilink_BNO();
};

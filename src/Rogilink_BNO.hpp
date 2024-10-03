#pragma once

#include "mbed.h"
#include "RogiLinkFlex/UartLink.hpp"

#include <memory>
#include "ros2_msgs/Imu.hpp"
#include "BNO055.hpp"

#include <vector>
#include <string>


class Rogilink_BNO
{
private:
    EventQueue queue;

    UartLink uart;

    UartLinkSubscriber<int, char*> sub;
    UartLinkPublisher<int, sensor_msgs::msg::Imu> pub;

    std::unique_ptr<IMU::BNO055> bno;
    uint8_t bno_id;
    PinName tx, rx;

    int rate = 0;// imu output rate [Hz]
    bool reset_flag = false;
    bool is_ready = false;

    void sub_callback(int _length, char* _cmd);

    void pub_callback();

    void reset_watchdog();

public:
    Rogilink_BNO(uint8_t _id, PinName _tx, PinName _rx);
    ~Rogilink_BNO();
};

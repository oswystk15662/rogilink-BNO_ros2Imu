#include <mbed.h>
#include "RogiLinkFlex/UartLink.hpp"

#include "ros2_msgs/Imu.hpp"
#include "BNO055.hpp"

#include <vector>
#include <string>

#if 0

UartLink uart1(USBTX, USBRX, 115200, nullptr, 0);
UartLink uart2(PA_9, PA_10, 115200, nullptr, 0);

UartLinkSubscriber<float, int, double, char*> sub(uart1, 2);
UartLinkPublisher<float, int, double, char*> pub(uart2, 1);

void sub_callback(float a, int b, double c, char* d) {
    pub.publish(a, b, c, d);
}

int main() {
    sub.set_callback(sub_callback);
}

#elif 0

UartLink uart1(USBTX, USBRX, 115200, nullptr, 1);
UartLink uart2(PA_9, PA_10, 115200, nullptr, 1);

UartLinkSubscriber<float, int, double, char*> sub(uart2, 1);
UartLinkPublisher<float, int, double, char*> pub(uart1, 2);

void sub_callback(float a, int b, double c, char* d) {
    pub.publish(a, b, c, d);
}

int main() {
    sub.set_callback(sub_callback);
}

#else

UartLink uart(USBTX, USBRX, 115200, nullptr, 0);

UartLinkSubscriber<int, char*> sub(uart, 2);
UartLinkPublisher<sensor_msgs::msg::Imu> pub(uart, 2);

int rate = 0;// imu output rate [Hz]
bool reset_flag = false;
bool is_ready = false;


void sub_callback(int _length, char* _cmd) {

    vector<string> cmds;
    string item;
    for (int i = 0; i < _length; i++) {
        if (_cmd[i] == ',') {
            if (!item.empty())
                cmds.push_back(item);
            item.clear();
        }
        else {
            item += _cmd[i];
        }
    }
    if (!item.empty())
        cmds.push_back(item);

    if(cmds[0] == "Rate"){
        rate = std::stoi(cmds[1]);
        is_ready = true;
    }
    else if (cmds[0] == "Reset")
    {
        reset_flag = true;
    }
    else{
        return;
    }
    
}

void pub_callback(){
    
}

int main() {
    sub.set_callback(sub_callback);

    while (is_ready);

    IMU::BNO055 bno(PC_10, PC_11, std::chrono::milliseconds(1000 / rate));

    
    
}
#endif
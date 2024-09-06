#include "RogiLinkFlex/Publisher.hpp"
#include "RogiLinkFlex/Subscriber.hpp"
#include "RogiLinkFlex/UartLink.hpp"
#include "mbed.h"

UartLink uart_link(USBTX, USBRX, 115200, nullptr, 0);

UartLinkPublisher<float_t, float_t, float_t> pub(uart_link, 0x01);
UartLinkSubscriber<float_t, float_t, float_t> sub(uart_link, 0x01);

void receive_callback(float_t goal_0, float_t goal_1, float_t goal_2) {
  pub.publish(goal_0, goal_1, goal_2);
}

int main() {
    sub.set_callback(sub_callback);
}
#else
#include <mbed.h>
#include "RogiLinkFlex/UartLink.hpp"

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

#endif

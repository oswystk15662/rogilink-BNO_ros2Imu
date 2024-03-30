#if 0
#include <mbed.h>
#include "RogiLinkFlex/UartLink.hpp"

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

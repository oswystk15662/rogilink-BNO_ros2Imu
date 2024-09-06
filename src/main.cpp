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
  // メッセージ送信
  while (1) {
    sub.set_callback(receive_callback);
    // printf("goal_r: %f, goal_theta: %f, goal_z: %f\n", goal_r, goal_theta, goal_z);
  }
}
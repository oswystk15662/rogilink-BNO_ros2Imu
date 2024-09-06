# rogilinkFlex-f446

現在開発中の汎用通信ライブラリrogilinkFlexのSTM32F446実装です。(おそらくmbed OSならF446じゃなくても動きます。)
UART通信とCAN通信に対応しています。従来のrogilinkに比べて、送受信するデータ型を柔軟に設定できるようになっており、データ変換も自動で行います。マイコン同士の通信、マイコンとros2の通信の両方に対応しています。

## Other Environments

- [rogilinkFlex-ros2](https://github.com/KeioRoboticsAssociation/rogilinkFlex-ros2)
- [rogilinkFlex-esp32](https://github.com/KeioRoboticsAssociation/rogilinkFlex-esp32) (動作未確認)

## 使い方(UART通信)

### インクルード
```cpp
#include "UartLink.hpp"
```

### インスタンスの生成
```cpp
UartLink uart(TXピン, RXピン, ボーレート);
```

### データの送信
```cpp
UartLinkPublisher<データ型1, データ型2, ...> pub(uart, フレームID); // publisherの設定

pub.publish(データ1, データ2, ...);
```

`UartLinkPublisher<char*>`の場合は、`printf`を使うことができます。
```cpp
pub.printf("a: %d, b: %d, c: %d\n", a, b, c);
```

### データの受信
```cpp
UartLinkSubscriber<データ型1, データ型2, ...> sub(uart, フレームID); // subscriberの設定

void callback(データ型1, データ型2, ...) {
    // コールバック関数
}

sub.set_callback(callback); // コールバック関数の設定
```

### 使用例
```cpp
#include <mbed.h>
#include "UartLink.hpp"

UartLink uart(USBTX, USBRX, 115200); // UART通信の設定

UartLinkSubscriber<uint8_t, uint16_t, uint8_t> sub(uart, 1); // Subscriber(受信側)の設定
UartLinkPublisher<char*> pub(uart, 2); // Publisher(送信側)の設定

// Subscriberのコールバック関数
void sub_callback(uint8_t a, uint16_t b, uint8_t c) {
    pub.printf("a: %d, b: %d, c: %d\n", a, b, c);
}

// main関数
int main() {
    sub.set_callback(sub_callback);
}
```

### EventQueueを使う場合

受信処理をすべて割り込みで行うのが好ましくない場合、EventQueueを使って受信処理を行うことができます。

```cpp
#include <mbed.h>
#include "UartLink.hpp"

EventQueue queue(32 * EVENTS_EVENT_SIZE);

UartLink uart(USBTX, USBRX, 115200, &queue); // UART通信の設定

UartLinkSubscriber<uint8_t, uint16_t, uint8_t> sub(uart, 1); // Subscriber(受信側)の設定
UartLinkPublisher<char*> pub(uart, 2); // Publisher(送信側)の設定

// Subscriberのコールバック関数
void sub_callback(uint8_t a, uint16_t b, uint8_t c) {
    pub.printf("a: %d, b: %d, c: %d\n", a, b, c);
}

// main関数
int main() {
    sub.set_callback(sub_callback);

    queue.dispatch_forever();
}
```

## 使い方(CAN通信)

### インスタンスの生成
```cpp
CanLink uart(Rdピン, Tdピン, 周波数);
```

### 受信・送信

UART通信とほとんど同じなので省略します。`UartLinkSubscriber, UartLinkPublisher`を`CanLinkSubscriber, CanLinkPublisher`に置き換えるだけです。



## 高度な使い方

### シリアライザ、デシリアライザの設定

デフォルトでは、送受信するデータ型として使えるのは、
- `int`, `float`, `char`,...などの基本型
- `char*`型(文字列)

のみです。ただし、構造体やクラスなどの複合型を送受信する場合は、シリアライザ(送信時)、デシリアライザ(受信時)を設定することで可能になります。

```cpp
struct Data {
    int a;
    float b;
    char c;

    // シリアライザ
    void serialize(uint8_t* data) {
        memcpy(data, &a, sizeof(int));
        memcpy(data + sizeof(int), &b, sizeof(float));
        memcpy(data + sizeof(int) + sizeof(float), &c, sizeof(char));
    }

    // デシリアライザ
    void deserialize(const uint8_t* data, size_t* size) {
        memcpy(&a, data, sizeof(int));
        memcpy(&b, data + sizeof(int), sizeof(float));
        memcpy(&c, data + sizeof(int) + sizeof(float), sizeof(char));
        *size = sizeof(int) + sizeof(float) + sizeof(char);
    };
};

UartLinkSubscriber<Data> sub(uart, 1); // Subscriber(受信側)の設定
```
シリアライザ・デシリアライザを設定しないとコンパイルエラーになります。
#ifndef __WiFi_H
#define __WiFi_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "gpio.h"
#include "usart.h"
#include <memory.h>
#include <stdbool.h>
#include "tasks.h"

#define WIFI_NAME "MERCURY_1A2C"    // wifi名字
#define WIFI_KEY "14732113"         // wifi密码

#define BROKER_ADDRESS "a1aXiT3oG7V.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define BROKER_PORT "1883"
#define CLIENT_ID "esp01s|securemode=2\\,signmethod=hmacsha1\\,timestamp=1731777824837|"
#define USER_NAME "esp01s&a1aXiT3oG7V"
#define PASSWORD "68187446BD23182D32F64D967F45A50E3D3D6FBF"
#define SUBSCRIBE_SERVICE "/sys/a1aXiT3oG7V/esp01s/thing/service/property/set"
#define SUBSCRIBE_EVENT "/sys/a1aXiT3oG7V/esp01s/thing/event/property/post"

#define WiFi_Rx_LEN 300 // 设置需要吃多少药的json文件的最大长度

// AT
extern const char *buff_at;
// CWMODE
extern const char *buff_CWMODE;
// 连接WiFi
extern const char *buff_CWJAP;
// mqtt用户设置
extern const char *buff_MQTTUSERCFG;
// mqtt连接设置
extern const char *buff_MQTTCONN;
// 订阅服务
extern const char *sub_service;
// 订阅事件
extern const char *sub_event;
// 发布
extern char pub_text[120];
extern char pub_text_time_request[120];
// 接收到的WiFi数据缓冲区
extern char WiFi_RX_BUF[WiFi_Rx_LEN];
extern bool is_wifi_get;

// WiFi使用dma发送
void WifiTransmit(const char buffer[]);
// 启动WiFi dma接收
void WifiReceive();
// WiFi初始化，连接路由器，以及使用mqtt连接阿里云，请求阿里云服务等
void WifiInit();

#endif 


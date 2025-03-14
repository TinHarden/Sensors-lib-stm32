#include "WiFi.h"

// AT
const char *buff_at = "AT+RST\r\n";
// CWMODE
const char *buff_CWMODE = "AT+CWMODE=1\r\n";
// 连接WiFi
const char *buff_CWJAP = "AT+CWJAP=\""WIFI_NAME"\",\""WIFI_KEY"\"\r\n";
// mqtt用户设置
const char *buff_MQTTUSERCFG = "AT+MQTTUSERCFG=0,1,\""CLIENT_ID"\",\""USER_NAME"\",\""PASSWORD"\",0,0,\"\"\r\n";
// mqtt连接设置
const char *buff_MQTTCONN = "AT+MQTTCONN=0,\""BROKER_ADDRESS"\","BROKER_PORT",0\r\n";
// 订阅服务
const char *sub_service = "AT+MQTTSUB=0,\""SUBSCRIBE_SERVICE"\",0\r\n";
// 订阅事件
const char *sub_event = "AT+MQTTSUB=0,\""SUBSCRIBE_EVENT"\",0\r\n";
// 发布
char pub_text[120];
char pub_text_time_request[120];

char WiFi_RX_BUF[WiFi_Rx_LEN];

bool is_wifi_get = false;

uint8_t t[10] = {0};
uint8_t u[10] = {0};

// Wifi发送数据
void WifiTransmit(const char buffer[]) {
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buffer, strlen(buffer));
}
// 重新开启WiFi接收数据
void WifiReceive() {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) WiFi_RX_BUF, WiFi_Rx_LEN);
}

void WifiInit() {
    WifiTransmit(buff_at);
    HAL_Delay(100);
    WifiTransmit(buff_CWMODE);
    HAL_Delay(100);
    WifiTransmit(buff_CWJAP);
    HAL_Delay(2000);
    WifiTransmit(buff_MQTTUSERCFG);
    HAL_Delay(100);
    WifiTransmit(buff_MQTTCONN);
    HAL_Delay(100);
    WifiTransmit(sub_service);
    HAL_Delay(100);
    WifiTransmit(sub_event);
    HAL_Delay(100);
    WifiTransmit("AT+MQTTSUB=0,\"/ext/ntp/a1aXiT3oG7V/esp01s/response\",0\r\n");
    HAL_Delay(200);
    WifiTransmit("AT+MQTTSUB=0,\"/ext/ntp/a1aXiT3oG7V/esp01s/request\",0\r\n");
    HAL_Delay(200);
    WifiReceive();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size) {
    if (huart == &huart1) {
        // 收到消息之后
        if (WiFi_RX_BUF[0] != 'A') {
            const char *target = "eat_";
            const char *target_2 = "is_eat_time";
            const char *target_time_1 = "serverSendTime";
            const char *target_time_2 = "serverRecvTime";
            const char *found = strstr(WiFi_RX_BUF, target);
            const char *found_2 = strstr(WiFi_RX_BUF, target_2);
            const char *found_3 = strstr(WiFi_RX_BUF, target_time_1);
            const char *found_4 = strstr(WiFi_RX_BUF, target_time_2);
            if (found_2 != NULL) {
                if (WiFi_RX_BUF[found_2 - WiFi_RX_BUF] == 'i') {
                    is_time_eat = true;
                }
            } else if (found != NULL) {
                // 对于该吃的药
                if (WiFi_RX_BUF[found - WiFi_RX_BUF + 4] == 'A') {
                    if (WiFi_RX_BUF[found - WiFi_RX_BUF + 11] != ':')
                    {
                        if (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] >= '0' && WiFi_RX_BUF[found - WiFi_RX_BUF + 12] <= '9') {
                            eat_A_Num = (WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0') * 10 +
                                        (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] - '0');
                        } else {
                            eat_A_Num = WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0';
                        }
                    }
                }
                if (WiFi_RX_BUF[found - WiFi_RX_BUF + 4] == 'B') {
                    if (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] >= '0' && WiFi_RX_BUF[found - WiFi_RX_BUF + 12] <= '9') {
                        eat_B_Num = (WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0') * 10 +
                                    (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] - '0');
                    } else {
                        eat_B_Num = WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0';
                    }
                }
                if (WiFi_RX_BUF[found - WiFi_RX_BUF + 4] == 'C') {
                    if (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] >= '0' && WiFi_RX_BUF[found - WiFi_RX_BUF + 12] <= '9') {
                        eat_C_Num = (WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0') * 10 +
                                    (WiFi_RX_BUF[found - WiFi_RX_BUF + 12] - '0');
                    } else {
                        eat_C_Num = WiFi_RX_BUF[found - WiFi_RX_BUF + 11] - '0';
                    }
                }
                is_wifi_get = true;
            }
            if (found_3 != 0) {
                if (WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 6] == 'S') {
                    t[0] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 17] - '0';
                    t[1] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 18] - '0';
                    t[2] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 19] - '0';
                    t[3] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 20] - '0';
                    t[4] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 21] - '0';
                    t[5] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 22] - '0';
                    t[6] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 23] - '0';
                    t[7] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 24] - '0';
                    t[8] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 25] - '0';
                    t[9] = WiFi_RX_BUF[found_3 - WiFi_RX_BUF + 26] - '0';
                    serverSendTime =
                            t[0] * 1000000000 + t[1] * 100000000 + t[2] * 10000000 + t[3] * 1000000 + t[4] * 100000 +
                            t[5] * 10000 +
                            t[6] * 1000 + t[7] * 100 + t[8] * 10 + t[9];
                }
            }
            if (found_4 != 0) {
                if (WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 6] == 'R') {
                    u[0] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 17] - '0';
                    u[1] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 18] - '0';
                    u[2] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 19] - '0';
                    u[3] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 20] - '0';
                    u[4] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 21] - '0';
                    u[5] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 22] - '0';
                    u[6] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 23] - '0';
                    u[7] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 24] - '0';
                    u[8] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 25] - '0';
                    u[9] = WiFi_RX_BUF[found_4 - WiFi_RX_BUF + 26] - '0';
                    serverRecvTime =
                            u[0] * 1000000000 + u[1] * 100000000 + u[2] * 10000000 + u[3] * 1000000 + u[4] * 100000 +
                            u[5] * 10000 +
                            u[6] * 1000 + u[7] * 100 + u[8] * 10 + u[9];
                }
            }
        }
        WifiReceive();
    }
}

//
// Created by heiyt_R9000P on 24-12-14.
//

#include "bluetooth.h"

#include <stdlib.h>

#define LanYa_Rx_LEN 4 // 设置需要吃多少药的json文件的最大长度

uint8_t LanYa_RX_BUF[LanYa_Rx_LEN];

void Bluetooth_init(void)
{
    HAL_UART_Receive_DMA(&huart3, LanYa_RX_BUF, LanYa_Rx_LEN);
}
void LanYaTransmit(uint8_t buffer[]) {
    HAL_UART_Transmit_DMA(&huart3, buffer, 20);
}
void LanYaReceive() {
    HAL_UART_Receive_DMA(&huart3, LanYa_RX_BUF, LanYa_Rx_LEN);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3)
    {
        LanYaReceive();
        if (LanYa_RX_BUF[0] == 0x89&&LanYa_RX_BUF[1] == 0x89&&LanYa_RX_BUF[2] == 0x89)
        {
            is_eat_time = true;
            is_eat_good = false;
            pre_weight = weight;
        }
        if (LanYa_RX_BUF[2] == 0x41 && LanYa_RX_BUF[3] == 0x00)
        {
            eat_yao_num = LanYa_RX_BUF[1] * 256 + LanYa_RX_BUF[0];
        }
        if (LanYa_RX_BUF[0] == 0x60)
        {
            if (LanYa_RX_BUF[1] == 0x01)
            {
                if (is_gaizi_open == 0)
                {
                    is_gaizi_open = 1;
                }
            } else
            {
                // 关盖子的时候如果吃对药
                if (is_eat_time)
                {
                    if (abs(pre_weight-weight-eat_yao_num) < 5)
                    {
                        if (is_gaizi_open == 3)
                        {
                            is_gaizi_open = 2;
                        }
                        is_eat_good = true;
                        is_eat_time = false;
                    } else
                    {
                        if (is_fengmingqi_open == 0)
                        {
                            is_fengmingqi_open = 1;
                        }
                    }
                } else
                {
                    if (is_gaizi_open == 3)
                    {
                        is_gaizi_open = 2;
                    }
                }
            }
        }
        if (LanYa_RX_BUF[0] == 0x70)
        {
            if (LanYa_RX_BUF[1] == 0x01)
            {
                if (is_fengmingqi_open == 0)
                {
                    is_fengmingqi_open = 1;
                }
            } else
            {
                if (is_fengmingqi_open == 3)
                {
                    is_fengmingqi_open = 2;
                }
            }
        }
    }
}
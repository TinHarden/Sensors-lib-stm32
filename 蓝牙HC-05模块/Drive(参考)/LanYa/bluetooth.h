//
// Created by heiyt_R9000P on 24-12-14.
//

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <string.h>
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"
#include "t_tasks.h"

void Bluetooth_init(void);
void LanYaTransmit(uint8_t buffer[]);
void LanYaReceive();

#endif //BLUETOOTH_H

#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"


#define DHT11 GPIO_PIN_1
#define DHT11_PROT GPIOA

#define OUT 1
#define INT 0

#define DHT11_Low HAL_GPIO_WritePin(DHT11_PROT,DHT11,GPIO_PIN_RESET)
#define DHT11_High HAL_GPIO_WritePin(DHT11_PROT,DHT11,GPIO_PIN_SET)

// 读取温度和湿度数据 humi为湿度，temp为温度
void DHT11_Read_Data(uint8_t *humi, uint8_t *temp);

#endif

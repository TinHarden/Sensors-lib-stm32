#ifndef __DHT22_H
#define __DHT22_H

#include "stm32l4xx.h"                  // Device header
#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

#define DHT22_OUT_High HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET)
#define DHT22_OUT_Low HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET)
#define DHT22_IN HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)

uint8_t DHT22_Read_Data(uint16_t* hum, uint16_t* temp);
void DHT22_Rst(void);	

#endif

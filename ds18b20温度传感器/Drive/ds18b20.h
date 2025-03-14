/*
* 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-29     LCKFB-LP    first version
 */
#ifndef _BSP_DS18B20_H_
#define _BSP_DS18B20_H_

#include "stm32f1xx.h"
#include "gpio.h"

//端口移植
#define PORT_DQ         DQ_GPIO_Port
#define GPIO_DQ         DQ_Pin

//设置DQ输出模式
#define DQ_OUT()  {\
GPIO_InitTypeDef  GPIO_InitStructure;\
GPIO_InitStructure.Pin = GPIO_DQ;\
GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;\
GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;\
HAL_GPIO_Init(PORT_DQ, &GPIO_InitStructure);\
}
//设置DQ输入模式
#define DQ_IN()   {\
GPIO_InitTypeDef  GPIO_InitStructure;\
GPIO_InitStructure.Pin = GPIO_DQ;\
GPIO_InitStructure.Mode = GPIO_MODE_INPUT;\
HAL_GPIO_Init(PORT_DQ, &GPIO_InitStructure);\
}
//获取DQ引脚的电平变化
#define DQ_GET()        HAL_GPIO_ReadPin(PORT_DQ, GPIO_DQ)
//DQ输出
#define DQ(x)           HAL_GPIO_WritePin(PORT_DQ, GPIO_DQ, (x?GPIO_PIN_SET:GPIO_PIN_RESET))

void DS18B20_Reset(void);
uint8_t DS18B20_Check(void);
void DS18B20_Start(void);
float DS18B20_GetTemperture(void);

#endif
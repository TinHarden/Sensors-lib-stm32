#ifndef __ds1302_H 
#define __ds1302_H  
 
#include "stm32f1xx.h"
/*
 * pc13 - rst
 * pc14 - dat
 * pc15 - clk
 */
#define CE_L HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)//拉低使能位
#define CE_H HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)//拉高使能位
#define SCLK_L HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET)//拉低时钟线
#define SCLK_H  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET)//拉高时钟线
#define DATA_L  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)//拉低数据线
#define DATA_H  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)//拉高数据线
 
struct TIMEData
{
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  week;
};//创建TIMEData结构体方便存储时间日期数据

struct TIMERAM
{
	uint8_t  hour_kai;
	uint8_t  minute_kai;
	uint8_t  hour_guan;
	uint8_t  minute_guan;
	uint8_t  kai;
	uint8_t  guan;
};//创建TIMEData结构体方便存储时间日期数据

extern struct TIMERAM TimeRAM;
extern struct TIMEData TimeData;//全局变量

// void ds1032_init(void);
void ds1032_read_realTime(void);
void ds1302_write_realTime(struct TIMEData time_data);

#endif







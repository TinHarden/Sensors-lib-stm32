#ifndef __HX711_H
#define __HX711_H

#include "stdint.h"
#include <stdbool.h>
#include "gpio.h"
#include "cmsis_os.h"
#include "t_tasks.h"

extern int32_t weight_mao;

void Weight_Init(void);
uint32_t HX711_Read(void);
int32_t Get_Weight(void);
void Get_Mao();

#endif


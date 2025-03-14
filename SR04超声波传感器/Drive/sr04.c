//
// Created by heiyt on 25-2-2.
//

#include "sr04.h"
#include "cmsis_os2.h"

double getDistance1() {
    uint16_t dis;
    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_SET);
    osDelay(1);
    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(ECHO1_GPIO_Port, ECHO1_Pin) == GPIO_PIN_RESET);
    __HAL_TIM_SetCounter(&htim3, 0);
    __HAL_TIM_ENABLE(&htim3);
    while (HAL_GPIO_ReadPin(ECHO1_GPIO_Port, ECHO1_Pin) == GPIO_PIN_SET);
    dis = __HAL_TIM_GetCounter(&htim3);
    __HAL_TIM_DISABLE(&htim3);
    return (double)dis * 0.034 / 2;
}

double getDistance2() {
    uint16_t dis;
    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_SET);
    osDelay(1);
    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == GPIO_PIN_RESET);
    __HAL_TIM_SetCounter(&htim3, 0);
    __HAL_TIM_ENABLE(&htim3);
    while (HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == GPIO_PIN_SET);
    dis = __HAL_TIM_GetCounter(&htim3);
    __HAL_TIM_DISABLE(&htim3);
    return (double)dis * 0.034 / 2;
}
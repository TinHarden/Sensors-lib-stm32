//
// Created by heiyt_R9000P on 24-11-11.
//

#include "duoji.h"


void GaiZiInit() {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

void GaiZiOpen() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 150);
}
void GaiZiClose() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 250);
}
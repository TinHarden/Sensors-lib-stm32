//
// Created by heiyt_R9000P on 24-11-11.
//

#include "hongwai.h"

bool is_HongWaiGet() {
    GPIO_PinState a;
    a = HAL_GPIO_ReadPin(HW_OUT_GPIO_Port, HW_OUT_Pin);
    if (a == GPIO_PIN_SET) {
        return false;
    } else {
        return true;
    }
}
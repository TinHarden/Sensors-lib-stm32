//
// Created by heiyt on 25-3-6.
//

#include "delay.h"
#include "stm32f1xx.h" // 根据具体芯片型号调整头文件

static uint32_t cycles_per_us = 0;

void delay_init(void) {
    // 启用DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 允许跟踪
    DWT->CYCCNT = 0;                // 重置计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // 启用CYCCNT

    // 计算每微秒的周期数（假设SystemCoreClock已正确设置）
    cycles_per_us = SystemCoreClock / 1000000;
}

void delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * cycles_per_us;

    // 等待直到经过足够的周期数
    while ((DWT->CYCCNT - start) < cycles) {
        // 空循环，依赖硬件计数器
    };
}
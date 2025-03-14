//
// Created by heiyt on 25-3-6.
//

#include "delay.h"
#include "stm32f1xx.h" // ���ݾ���оƬ�ͺŵ���ͷ�ļ�

static uint32_t cycles_per_us = 0;

void delay_init(void) {
    // ����DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // �������
    DWT->CYCCNT = 0;                // ���ü�����
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // ����CYCCNT

    // ����ÿ΢���������������SystemCoreClock����ȷ���ã�
    cycles_per_us = SystemCoreClock / 1000000;
}

void delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * cycles_per_us;

    // �ȴ�ֱ�������㹻��������
    while ((DWT->CYCCNT - start) < cycles) {
        // ��ѭ��������Ӳ��������
    };
}
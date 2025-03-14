//
// Created by heiyt_R9000P on 24-12-29.
//

#ifndef MQ_FUNCTION_H
#define MQ_FUNCTION_H
#include <math.h>
#include "stm32l4xx_hal.h"

//�������崫�����Ŀɵ����ص���ĵ���ֵRL��ͨ�����ñ����A-H�˵ã�
//��λ��K��
#define MQ2_RL    1.0
#define MQ7_RL    1.0
#define MQ135_RL  1.0
#define MP4_RL    1.0
//��λ��K��
//ҪУ׼�Ļ��͸���Щ R0 ��ֵ
#define MQ2_R0    2.0
#define MQ7_R0    2.0
#define MQ135_R0  2.0
#define MP4_R0    2.0

inline float adc_to_Rs(const uint16_t data, const float RL)
{
  const float adc_d = data * 5 / 4096.0;
  return (5 - adc_d) * RL / adc_d;
}
// MQ-7 CO
float MQ7_Data_to_PPM(const uint16_t data) {
  const float RS = adc_to_Rs(data, MQ7_RL);
  const float x = RS / MQ7_R0;
  float ppm = 70.94 * pow(x, -1.67);
  return ppm;
}
// MQ-135 NH3
float MQ135_Data_to_PPM(const uint16_t data) {
  const float RS = adc_to_Rs(data, MQ135_RL);
  const float x = RS / MQ135_R0;
  float ppm = 4.33 * pow(x, -1.94);
  return ppm;
}
// MQ-2 Smog
float MQ2_Data_to_PPM(const uint16_t data) {
  const float RS = adc_to_Rs(data, MQ2_RL);
  const float x = RS / MQ2_R0;
  float ppm = 82.39 * pow(x, -1.28);
  return ppm;
}
// MP4 CH4
float MP4_Data_to_PPM(const uint16_t data) {
  const float RS = adc_to_Rs(data, MP4_RL);
  const float x = RS / MP4_R0;
  float ppm = 86.36 * pow(x, -1.20);
  return ppm;
}
// LM2904 Noise
float Noise_Data_to_dB(const uint16_t data) {
    float db = 56.28 * log1pf(data) - 347.53;
    if (db < 10) {
        db = 10;
    }
    if (db > 90) {
        db = 90;
    }
//    const float db = -70.36 * (pow(log(data), 2)) + 1009.12 * (log(data)) - 3570.89562774;
//  return db;

//    const float db = 108.24 * (pow(log(data), 3)) - 2397.92 * (pow(log(data),2)) + 17706.88 * log(data) - 43542.15;
    return db;
}
// PM2.5
float PM2_5_Data_to_PPM(const uint16_t data)
{
    float adc_d = (float) data * 3.3 / 4096.0;
    float dust = 172 * (adc_d) - 103;
    if (dust < 0) {
        dust = 0;
    }
    if (dust > 500) {
        dust = 500;
    }
    return dust;
}

#endif //MQ_FUNCTION_H

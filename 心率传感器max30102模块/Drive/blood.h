/**
* ************************************************************************
 *
 * @file blood.h
 * @author zxr
 * @brief
 *
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr
 * ************************************************************************
 */
#ifndef _BLOOD_H
#define _BLOOD_H

#include "main.h"
#include "max30102.h"
#include "alg.h"
#include "math.h"

extern int heart;
extern float SpO2;

void blood_data_translate(void);
void blood_data_update(void);
void blood_Loop(void);

#endif





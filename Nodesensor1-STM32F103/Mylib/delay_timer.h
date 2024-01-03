#ifndef DELAY_US_H
#define DELAY_US_H

#include "main.h"

void DELAY_US_Init(TIM_HandleTypeDef *p_htim);
void DELAY_US_Run(uint32_t p_delay_time);
void DELAY_MS_RUN(uint32_t p_delay_time);
#endif

#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x_tim.h"

void TIM2_Init(float UnitTime_ms);
void TIM2_IRQHandler(void);
#endif

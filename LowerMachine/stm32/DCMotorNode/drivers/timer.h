#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x_tim.h"
#include "motor_operate.h"
#include <stdlib.h>
#include <math.h>

#define MaxSpeed 6000

extern float PWM_output;
extern float speed_feedback;


extern int sample_time;
extern int sample_time_unit;
extern int sample_times;
extern int average;
extern int sum;
extern int max;
extern int min;
extern const int max_st;

void TIM1_Init(float UnitTime_ms);
void TIM2_Init(float UnitTime_ms);
void PID_TIM3_Init(float sampleTime_ms);
void TIM4_PWM_Init(u16 arr,u16 psc,u8 mode);
void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void BubbleSort(int *buf, int len);
#endif

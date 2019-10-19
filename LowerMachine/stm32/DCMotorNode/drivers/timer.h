#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x_tim.h"
#include "motor_operate.h"

#define MaxSpeed 6000

extern float currentAngle;
extern float preAngle;
extern float AngleDelta;
extern float PWM_output;
extern float speed_feedback;
extern float sample_time;

void TIM1_Init(float UnitTime_ms);
void TIM2_Init(float UnitTime_ms);
void PID_TIM3_Init(float sampleTime_ms);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
#endif

#ifndef _MOTOR_OPERATE_H_
#define _MOTOR_OPERATE_H_

#include "stm32f10x_gpio.h"
#include "motor_gpio.h"
#include "pid.h"
#include "timer.h"

#define TIM4_ARR 999
#define TIM4_PSC 719

/* public */
extern int motor_state;
extern float wheel_speed_goal;

void motor_init(void);
void motor_run(float Duty_cycle);
void motor_brake(void);
#endif

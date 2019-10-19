/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	电机驱动
 */

#include "motor_operate.h"

int motor_state = 0;	/* 电机运动状态 */
float wheel_speed_goal;	/* 轮子目标速度 */

/**
 *@function 电机资源初始化
 *@param void
 *@return void
 */
void motor_init(void)
{
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC);	/* 初始化TIM4,提供给电机驱动使用 */
	as5600_init();	/* 初始化磁编码器 */
	PID_CONTROLLER(0.0002,0.00001,0.0001,50);	/* 初始化PID控制器，包括定时器3 */
}

/**
 *@function 电机运动
 *@param Duty_cycle占空比
 *@return void
 */
void motor_run(float Duty_cycle)
{
	if(Duty_cycle > 0)
	{
		TIM_SetCompare1(TIM4, Duty_cycle*999.0);	/* IN1 PWM */
		TIM_SetCompare2(TIM4, 0);	/* IN2 LOW */
		motor_state = 1;
	}else if(Duty_cycle < 0)
	{
		TIM_SetCompare1(TIM4, 0);	/* IN1 LOW */
		TIM_SetCompare2(TIM4, -1*Duty_cycle*999.0);	/* IN2 PWM */
		motor_state = -1;
	}
}

/**
 *@function 电机停止
 *@param void
 *@return void
 */
void motor_brake(void)
{
	GPIO_SetBits(Motor.motor_gpio_type[0],Motor.motor_gpio[0].GPIO_Pin);
	GPIO_SetBits(Motor.motor_gpio_type[1],Motor.motor_gpio[1].GPIO_Pin);
	motor_state = 0;
}

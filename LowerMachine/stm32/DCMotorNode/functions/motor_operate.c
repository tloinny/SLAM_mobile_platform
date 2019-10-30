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
 *@function Motor controll resource initialization
 *@param void
 *@return void
 */
void motor_init(void)
{
	VNH5180_io_init();
	as5600_init();	/* 初始化磁编码器 */
	PID_CONTROLLER(0.035,0.01,0.02,25);	/* 初始化PID控制器，包括定时器3 */
}

/**
 *@function A4950 driver motor - Fast Decay mode
 *@param Duty_cycle
 *@return void
 */
void A4950_motor_run_FastDecay(float Duty_cycle)
{
	if(Duty_cycle > 0)
	{
		TIM_SetCompare1(TIM4, Duty_cycle*1000.0);	/* IN1 PWM */
		TIM_SetCompare2(TIM4, 0);	/* IN2 LOW */
		motor_state = 1;
	}else if(Duty_cycle < 0)
	{
		TIM_SetCompare1(TIM4, 0);	/* IN1 LOW */
		TIM_SetCompare2(TIM4, -1*Duty_cycle*1000.0);	/* IN2 PWM */
		motor_state = -1;
	}
}

/**
 *@function A4950 driver motor - Slow Decay mode
 *@param Duty_cycle
 *@return void
 */
void A4950_motor_run_SlowDecay(float Duty_cycle)
{
	if(Duty_cycle > 0)
	{
		TIM_SetCompare2(TIM4, (1-Duty_cycle)*999.0);	/* IN1 PWM */
		TIM_SetCompare1(TIM4, 1000);	/* IN2 HIGH */
		motor_state = 1;
	}else if(Duty_cycle < 0)
	{
		TIM_SetCompare2(TIM4, 1000);	/* IN1 HIGH */
		TIM_SetCompare1(TIM4, -1*(Duty_cycle-1)*999.0);	/* IN2 PWM */
		motor_state = -1;
	}
}

/**
 *@function A4950 motor brake
 *@param void
 *@return void
 */
void A4950_motor_brake(void)
{
	TIM_SetCompare1(TIM4, 1000);	/* IN1 HIGH */
	TIM_SetCompare2(TIM4, 1000);	/* IN2 HIGH */
	motor_state = 0;
}

/**
 *@function VNH5180 driver motor
 *@param Duty_cycle
 *@return void
 */
void VNH5180_motor_run(float Duty_cycle)
{
	if(Duty_cycle > 0)
	{
		INA = 1;
		INB = 0;
		TIM_SetCompare2(TIM4, Duty_cycle*999.0);	/* PWM */
		motor_state = 1;
	}else if(Duty_cycle < 0)
	{
		INA = 0;
		INB = 1;
		TIM_SetCompare2(TIM4, -1*Duty_cycle*999.0);	/* IN2 PWM */
		motor_state = -1;
	}
}

/**
 *@function VNH5180 motor brake
 *@param void
 *@return void
 */
void VNH5180_motor_brake()
{
	INA = 1;
	INB = 1;
	TIM_SetCompare2(TIM4, 0);
}

/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	�������
 */

#include "motor_operate.h"

int motor_state = 0;	/* ����˶�״̬ */
float wheel_speed_goal;	/* ����Ŀ���ٶ� */

/**
 *@function �����Դ��ʼ��
 *@param void
 *@return void
 */
void motor_init(void)
{
	as5600_init();	/* ��ʼ���ű����� */
	PID_CONTROLLER(1,0,0,25);	/* ��ʼ��PID��������������ʱ��3 */
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC);	/* ��ʼ��TIM4,�ṩ���������ʹ�� */
}

/**
 *@function ����˶�
 *@param Duty_cycleռ�ձ�
 *@return void
 */
void motor_run(float Duty_cycle)
{
	if(Duty_cycle > 0)
	{
		TIM_SetCompare1(TIM4, Duty_cycle*999);	/* IN1 PWM */
		GPIO_ResetBits(Motor.motor_gpio_type[1],Motor.motor_gpio[1].GPIO_Pin);	/* IN2 LOW */
		motor_state = 1;
	}else if(Duty_cycle < 0)
	{
		GPIO_ResetBits(Motor.motor_gpio_type[0],Motor.motor_gpio[0].GPIO_Pin);	/* IN1 LOW */
		TIM_SetCompare2(TIM4, -1*Duty_cycle*999);	/* IN2 PWM */
		motor_state = -1;
	}
}

/**
 *@function ���ֹͣ
 *@param void
 *@return void
 */
void motor_brake(void)
{
	GPIO_SetBits(Motor.motor_gpio_type[0],Motor.motor_gpio[0].GPIO_Pin);
	GPIO_SetBits(Motor.motor_gpio_type[1],Motor.motor_gpio[1].GPIO_Pin);
	motor_state = 0;
}

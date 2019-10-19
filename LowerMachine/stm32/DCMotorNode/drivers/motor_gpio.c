/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: ���IO�ڵײ�����
 */

#include "motor_gpio.h"

motor Motor;

/**
 *@function ���IO��ʼ��
 *@param void
 *@return void
 */
void motor_io_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  /* ʹ��GPIO���� */

	/* ����PWM�� */
	Motor.motor_gpio[0].GPIO_Pin = GPIO_Pin_6;	/* IN1 */
	Motor.motor_gpio[1].GPIO_Pin = GPIO_Pin_7;	/* IN2 */
	
	/* ����IOģʽ */
	for(int n=0;n<2;++n)
	{
		Motor.motor_gpio_type[n] = GPIOB;
		Motor.motor_gpio[n].GPIO_Mode = GPIO_Mode_AF_PP;
		Motor.motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;				
	}
		
	/* ��ʼ��IO */	
	for(int n=0;n<2;++n)
		GPIO_Init(Motor.motor_gpio_type[n], &(Motor.motor_gpio[n]));
}

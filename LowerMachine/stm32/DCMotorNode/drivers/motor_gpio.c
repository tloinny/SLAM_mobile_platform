/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 电机IO口底层配置
 */

#include "motor_gpio.h"

motor Motor;

/**
 *@function 电机IO初始化
 *@param void
 *@return void
 */
void A4950_io_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  /* 使能GPIO外设 */

	/* 配置PWM脚 */
	Motor.motor_gpio[0].GPIO_Pin = GPIO_Pin_6;	/* IN1 */
	Motor.motor_gpio[1].GPIO_Pin = GPIO_Pin_7;	/* IN2 */
	
	/* 配置IO模式 */
	for(int n=0;n<2;++n)
	{
		Motor.motor_gpio_type[n] = GPIOB;
		Motor.motor_gpio[n].GPIO_Mode = GPIO_Mode_AF_PP;
		Motor.motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;				
	}
		
	/* 初始化IO */	
	for(int n=0;n<2;++n)
		GPIO_Init(Motor.motor_gpio_type[n], &(Motor.motor_gpio[n]));
	
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC,0);	/* 初始化TIM4,提供给电机驱动使用 */
}

void VNH_io_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  /* 使能GPIO外设 */
	
	/* 配置PWM脚 */
	Motor.motor_gpio[0].GPIO_Pin = GPIO_Pin_7;	/* PWM */
	Motor.motor_gpio_type[0] = GPIOB;
	Motor.motor_gpio[0].GPIO_Mode = GPIO_Mode_AF_PP;
	Motor.motor_gpio[0].GPIO_Speed = GPIO_Speed_50MHz;		

	Motor.motor_gpio[1].GPIO_Pin = GPIO_Pin_6;	/* IN_A */
	Motor.motor_gpio[2].GPIO_Pin = GPIO_Pin_5;	/* IN_B */
	
	for(int n=1;n<3;++n)
	{
		Motor.motor_gpio_type[n] = GPIOB;
		Motor.motor_gpio[n].GPIO_Mode = GPIO_Mode_Out_PP;
		Motor.motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;				
	}		
	
	for(int n=0;n<3;++n)
		GPIO_Init(Motor.motor_gpio_type[n], &(Motor.motor_gpio[n]));
	
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC,1);	/* 初始化TIM4,提供给电机驱动使用 */
}

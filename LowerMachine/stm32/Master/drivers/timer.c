/**l
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 定时器底层配置
 */ 
#include "timer.h"

TIM_ICInitTypeDef encoders_channel[4];
long counter[4];
float speed[4];
double unit_time = 0;

void PID_TIM2_Init(float sampleTime)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* 使能定时器2时钟 */
	
   /* 初始化TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*sampleTime - 1; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* 抢占优先级2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);  /* 使能TIM2 */
}

/**
 *@function TIM3 初始化 
 *@param 
 *			arr：自动重装值
 *			psc：时钟预分频数
 */
void TIM3_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* 使能定时器3时钟 */
	
   /* 初始化TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler = psc; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	unit_time = 72000000.0/((arr+1)*(psc+1));
	unit_time = 1.0/unit_time;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢占优先级2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
}

/**
 *@function TIM4 PWM 初始化 
 *@param 
 *			arr：自动重装值
 *			psc：时钟预分频数
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	motor_io_init();	/* 初始化电机驱动引脚 */

   /* 初始化TIM4 */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* 初始化TIM4 PWM模式 */	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* 选择定时器模式:TIM脉冲宽度调制模式2 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* 输出极性:TIM输出比较极性高 */
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC1 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC2 */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC3 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC4 */

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR1上的预装载寄存器 */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR2上的预装载寄存器 */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR3上的预装载寄存器 */
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR4上的预装载寄存器 */
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

/**
 *@function TIM2 中断服务函数
 *@param void
 */
void TIM2_IRQHandler(void)
{
	for(int i=0;i<4;++i)
	{
		setGoal(wheel_speed[i]);
		PWM[i] += update(speed[i],i);
		if(PWM[i] > 255)
		{
			PWM[i] = 255;
		}else if(PWM[i] < 0)
		{
			PWM[i] = 0;
		}
	}
	motor_run();
}

/**
 *@function TIM3 中断服务函数
 *@param void
 */
void TIM3_IRQHandler(void)
{
	for(int i=0;i<4;++i)
	{
		if(counter_flag[i] == 1)
		{
			counter[i] += 1;
		}else if(counter_flag[i] == 2)
		{
			speed[i] = 30000.0 / (counter[i] * unit_time);
			counter[i] = 0;
		}else if(counter_flag[i] == 0)
		{
			counter[i] = 0;
		}
	}
}

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
#include "led.h"

/**
 *@function TIM2 初始化 用于采集速度反馈
 *@param 
 *			UnitTime_ms: 采样周期，单位ms
 */
void TIM2_Init(float UnitTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* 使能定时器2时钟 */
	
   /* 初始化TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	/* 抢占优先级2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);  /* 使能TIM2 */
}

/**
 *@function TIM2 定时中断服务函数 用于采集速度反馈
 *@param none
 */
void TIM2_IRQHandler(void)
{

}


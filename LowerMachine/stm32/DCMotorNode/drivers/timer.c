/**l
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 定时器底层配置
 */ 
#include "timer.h"
#include "led.h"

float speed_feedback  = -1;
float unit_time = 0;

float PWM_output = 0;

int sample_time_sum = 0;
int sample_time_unit = 0;

int currentAngle = 0;
int preAngle = -1;
int AngleDelta = 0;

const int max_st = 15;
int sample_times = 0;
int AngleDelta_sum = 0;
int AngleDelta_average = -1;
int sample_log[max_st]={0};
int AngleDelta_middle = 0;

/**
 *@function TIM1 initialization, Timer1 is used for only timing
 *@param 
 *			UnitTime_ms
 */
void TIM1_Init(float UnitTime_ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	/* TIM1中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* 抢占优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

/**
 *@function TIM2 initialization. Timer2 is used to collecting the speed of DC motor.
 *@param 
 *			UnitTime_ms
 */
void TIM2_Init(float UnitTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* 使能定时器2时钟 */
	
   /* 初始化TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	unit_time = UnitTime_ms*1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* 抢占优先级2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);  /* 使能TIM2 */
}

/**
 *@function TIM3 initialization. Timer3 is used to do the PID control of DC motor's speed.
 *@param 
 *			UnitTime_ms： PID controlling cycle
 */
void PID_TIM3_Init(float UnitTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* 使能定时器3时钟 */
	
   /* 初始化TIM3 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢占优先级2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	/* 从优先级0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
}

/**
 *@function TIM4 initialization. Timer4 is used to generate PWM signal to control the DC motor.
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

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR1上的预装载寄存器 */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR2上的预装载寄存器 */
	
	TIM_Cmd(TIM4, ENABLE);  /* 使能TIM4 */
}

/**
 *@function TIM2 定时中断服务函数 用于采集速度反馈
 *@param none
 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(preAngle == -1)
		{
			currentAngle = getRawAngle();
			preAngle = currentAngle;
			TIM1->CNT = 0;
		}else
		{
			currentAngle = getRawAngle();
			sample_time_sum += TIM1->CNT;
			TIM1->CNT = 0;
			AngleDelta = currentAngle - preAngle;
			if(abs(AngleDelta)<=1) AngleDelta=0;
			if(AngleDelta >= 0)
			{
				if(AngleDelta <= 1024)
				{
					motor_state = 1;	/* 正转 */
				}else
				{
					motor_state = -1;	/* 反转 */
					AngleDelta -= 4095;
				}
			}else
			{
				if(AngleDelta >= -1024)
				{
					motor_state = -1;
				}else
				{
					motor_state = 1;
					AngleDelta += 4095;
				}
			}
			preAngle = currentAngle;
			if(sample_times>=max_st)
			{
				int noise_num = 0;
				sample_times = 0;
				BubbleSort(sample_log, max_st);
				AngleDelta_middle = sample_log[max_st/2];
				for(int i=0;i<max_st;++i)
				{
					if(abs(sample_log[i]-AngleDelta_middle)<=abs(AngleDelta_middle*0.1))
					{
						AngleDelta_sum += sample_log[i];
					}else
					{
						++noise_num;
						sample_log[i] = 0;
					}
					sample_log[i] = 0;
				}			
				AngleDelta_average = AngleDelta_sum/(max_st-noise_num);
				sample_time_unit = sample_time_sum / max_st;
				sample_time_sum = 0;
				AngleDelta_sum = 0;
			}else
			{
				sample_log[sample_times] = AngleDelta;
			}
			++sample_times;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/**
 *@function TIM3 定时中断服务函数
 *@param none
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		setGoal(wheel_speed_goal);
		if(AngleDelta_average != -1)
		{
			PWM_output += update(AngleDelta_average);	/* 增量式PID */
			if(PWM_output >= 1)
			{
				PWM_output = 1;
			}else if(PWM_output <= -1)
			{
				PWM_output = -1;
			}
		}
		motor_run(PWM_output);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void BubbleSort(int *buf, int len)
{
	for(int i=0;i<len-1;++i) 
		for(int j=0;j<len-i-1;++j)
			if(*(buf+j)<*(buf+j+1))
			{
				int temp = *(buf+j+1);
				*(buf+j+1) = *(buf+j);
				*(buf+j) = temp;
			}
}

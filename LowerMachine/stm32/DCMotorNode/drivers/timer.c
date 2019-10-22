/**l
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: ��ʱ���ײ�����
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
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	/* TIM1�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* ��ռ���ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* ʹ�ܶ�ʱ��2ʱ�� */
	
   /* ��ʼ��TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	unit_time = UnitTime_ms*1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);  /* ʹ��TIM2 */
}

/**
 *@function TIM3 initialization. Timer3 is used to do the PID control of DC motor's speed.
 *@param 
 *			UnitTime_ms�� PID controlling cycle
 */
void PID_TIM3_Init(float UnitTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* ʹ�ܶ�ʱ��3ʱ�� */
	
   /* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);  /* ʹ��TIM3 */
}

/**
 *@function TIM4 initialization. Timer4 is used to generate PWM signal to control the DC motor.
 *@param 
 *			arr���Զ���װֵ
 *			psc��ʱ��Ԥ��Ƶ��
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	motor_io_init();	/* ��ʼ������������� */

   /* ��ʼ��TIM4 */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* ��ʼ��TIM4 PWMģʽ */	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* �������:TIM����Ƚϼ��Ը� */
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC1 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC2 */

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ��� */
	
	TIM_Cmd(TIM4, ENABLE);  /* ʹ��TIM4 */
}

/**
 *@function TIM2 ��ʱ�жϷ����� ���ڲɼ��ٶȷ���
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
					motor_state = 1;	/* ��ת */
				}else
				{
					motor_state = -1;	/* ��ת */
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
 *@function TIM3 ��ʱ�жϷ�����
 *@param none
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		setGoal(wheel_speed_goal);
		if(AngleDelta_average != -1)
		{
			PWM_output += update(AngleDelta_average);	/* ����ʽPID */
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

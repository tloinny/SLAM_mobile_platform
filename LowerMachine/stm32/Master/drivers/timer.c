/**l
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: ��ʱ���ײ�����
 */ 
#include "timer.h"

TIM_ICInitTypeDef encoders_channel[4];
long counter[4];
float speed[4];
double unit_time = 0;

void PID_TIM2_Init(float sampleTime)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* ʹ�ܶ�ʱ��2ʱ�� */
	
   /* ��ʼ��TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*sampleTime - 1; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);  /* ʹ��TIM2 */
}

/**
 *@function TIM3 ��ʼ�� 
 *@param 
 *			arr���Զ���װֵ
 *			psc��ʱ��Ԥ��Ƶ��
 */
void TIM3_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* ʹ�ܶ�ʱ��3ʱ�� */
	
   /* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = psc; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	unit_time = 72000000.0/((arr+1)*(psc+1));
	unit_time = 1.0/unit_time;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);  /* ʹ��TIM3 */
}

/**
 *@function TIM4 PWM ��ʼ�� 
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
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC3 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC4 */

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ��� */
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

/**
 *@function TIM2 �жϷ�����
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
 *@function TIM3 �жϷ�����
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

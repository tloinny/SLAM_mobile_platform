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
#include "led.h"

float currentAngle = 0;
float preAngle = -1;

float speed_feedback  = -1;
double unit_time = 0;
float AngleDelta = 0;

int PWM_output = 0;

float sample_time = 0;
long current_time = 0;
//long pre_time = 0;

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
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	/* TIM1�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	/* ��ռ���ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

/**
 *@function TIM2 ��ʼ�� ���ڲɼ��ٶȷ���
 *@param 
 *			UnitTime_ms: �������ڣ���λms
 */
void TIM2_Init(float UnitTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	/* ʹ�ܶ�ʱ��2ʱ�� */
	
   /* ��ʼ��TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1000*UnitTime_ms - 1; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	unit_time = UnitTime_ms/1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	/* TIM2�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);  /* ʹ��TIM2 */
}

/**
 *@function TIM3 ��ʼ�� ����PID���� 
 *@param 
 *			sampleTime�� PID�������ڣ���λms
 */
void PID_TIM3_Init(float sampleTime_ms)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* ʹ�ܶ�ʱ��3ʱ�� */
	
   /* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period = 1000*sampleTime_ms - 1; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler = 71; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	/* ��ռ���ȼ�2 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	/* �����ȼ�0 */
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

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ��� */
	
	TIM_Cmd(TIM4, ENABLE);  /* ʹ��TIM4 */
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		//printf("TIM1\r\n");
	}
}

/**
 *@function TIM2 ��ʱ�жϷ����� ���ڲɼ��ٶȷ���
 *@param none
 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(preAngle == -1)
		{
			currentAngle = getRawDegrees();
			preAngle = currentAngle;
			
			//TIM_Cmd(TIM1, DISABLE);
			TIM1->CNT = 0;
			//TIM_Cmd(TIM1, ENABLE);
		}else
		{
			currentAngle = getRawDegrees();
			
			//TIM_Cmd(TIM1, DISABLE);
			current_time = TIM1->CNT;
			TIM1->CNT = 0;
			//TIM_Cmd(TIM1, ENABLE);
			sample_time = current_time/1000.0;
			AngleDelta = currentAngle - preAngle;
				if(AngleDelta >= 0)
				{
					if(AngleDelta <= 90)
					{
						motor_state = 1;	/* ��ת */	
					}else
					{
						motor_state = -1;	/* ��ת */
						AngleDelta -= 360;
					}
				}else
				{
					if(AngleDelta >= -90)
					{
						motor_state = -1;
					}else
					{
						AngleDelta += 360;
					}
				}
			speed_feedback = (AngleDelta/sample_time) * 166.666667;
			preAngle = currentAngle;
			//printf("AngleDelta: %f \r\n", AngleDelta);
			//printf("speed_feedback: %f\r\n", speed_feedback);
			//printf("current_time: %d\r\n", current_time);
			//printf("sample_time: %f\r\n", sample_time);
			}
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		//printf("timer3 update------\r\n");
		setGoal(wheel_speed_goal);
		if(speed_feedback != -1)
		{
			PWM_output += update(speed_feedback);	/* ����ʽPID */
		}
		if(PWM_output > 255)
		{
			PWM_output = 255;
		}else if(PWM_output < -255)
		{
			PWM_output = -255;
		}
		motor_run(PWM_output);
	}
}

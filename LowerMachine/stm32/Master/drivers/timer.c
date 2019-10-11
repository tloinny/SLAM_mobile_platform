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
 *@function TIM2 ��ʱ�жϷ����� ���ڲɼ��ٶȷ���
 *@param none
 */
void TIM2_IRQHandler(void)
{

}


/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: �ⲿ�ж�����
 */

#include "sys_conf.h"

u8 home_flag = 0;	/* ��λ���ر�־λ */

/**
 *@function �ⲿ�жϳ�ʼ��
 *@param void
 *@return void
 */
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	/* ʹ�ܸ��ù���ʱ�� */

    /* GPIOA.5 �ж����Լ��жϳ�ʼ������   �½��ش��� */
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}


void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	/* ���� */
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)	home_flag = 1;	/* ��λ���ر�־Ϊ���� */
	EXTI_ClearITPendingBit(EXTI_Line5);  /* ���LINE5�ϵ��жϱ�־λ */  
}
 

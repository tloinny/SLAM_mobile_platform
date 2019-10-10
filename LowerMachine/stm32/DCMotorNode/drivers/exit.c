#include "sys_conf.h"

u8 counter_flag[4];

/**
 *@function exit initializing
 *@param void
 *@return void
 */
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  	encoder_io_init();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		
  	EXTI_Init(&EXTI_InitStructure);	 
		
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);
	
		
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  	NVIC_Init(&NVIC_InitStructure); 
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStructure); 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure); 
}


/**
 *@function exit0 handler function
 *@param void
 *@return void
 */
void EXTI0_IRQHandler(void)
{
	if(counter_flag[0] == 0)
	{
		counter_flag[0] = 1;	
	}else if(counter_flag[0] == 1)
	{
		counter_flag[0] = 2;
	}
}

/**
 *@function exit1 handler function
 *@param void
 *@return void
 */
void EXTI1_IRQHandler(void)
{
	if(counter_flag[1] == 0)
	{
		counter_flag[1] = 1;
	}else if(counter_flag[0] == 1)
	{
		counter_flag[1] = 2;
	}
}

/**
 *@function exit5_9 handler function
 *@param void
 *@return void
 */
void EXTI9_5_IRQHandler(void)
{
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 1)
	{
		if(counter_flag[2] == 0)
		{
			counter_flag[2] = 1;
		}else if(counter_flag[0] == 1)
		{
			counter_flag[2] = 2;
		}
	}else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1)
	{
		if(counter_flag[3] == 0)
		{
			counter_flag[3] = 1;
		}else if(counter_flag[0] == 1)
		{
			counter_flag[3] = 2;
		}
	}
}

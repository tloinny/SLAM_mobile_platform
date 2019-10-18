/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@date on 2019-4-24  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Mecanum Mobile Platform application
 */
#include "delay.h"
#include "led.h"
#include "Master_can_protocol.h"
#include "inverse_solution.h"
#include "USART_Communication.h"


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	DEBUG_USARTx_DMA_Config();
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);	/* 初始化CAN总线 */
	delay_ms(500);
	CAN_Call();	/* CAN广播一次，查看总线上有哪些节点 */
	RUN_LED = 1;
	M1_LED = 1;
	M2_LED = 1;
	M3_LED = 1;
	M4_LED = 1;
		while(1)
		{
			u8 buf[8] = {1,2,3,4};
			DEBUG_USART_DMA_Tx_Start(buf,4);
//			USART_Commuincate();
//			if(Can_Receive_Msg(RecBuf.s) != 0)
//			{
//				match_feedback(RecBuf.s);				
//			}
		}
}


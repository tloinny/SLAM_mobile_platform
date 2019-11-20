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
	float test_speed = 6000;
		while(1)
		{
			//CAN_send_wheel_speed(test_speed, slave_0);
//			for(int i=-6000;i<6000;i+=1)
//			{
//				CAN_send_wheel_speed(i, slave_0);
//				delay_us(100);
//			}
//			for(int i=6000;i>-6000;i-=1)
//			{
//				CAN_send_wheel_speed(i, slave_0);
//				delay_us(100);
//			}
//				CAN_send_wheel_speed(test_speed, slave_0);
//				delay_ms(5000);
//				CAN_send_wheel_speed(-1*test_speed, slave_0);
//				delay_ms(5000);
//			USART_Commuincate();
//			if(Can_Receive_Msg(RecBuf.s) != 0)
//			{
//				match_feedback(RecBuf.s);				
//			}
		}
}


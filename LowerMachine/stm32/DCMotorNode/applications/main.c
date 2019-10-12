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
 #include "usart.h"
 #include "led.h"
 #include "motor_operate.h"
 #include "pid.h"
 #include "as5600.h"
 #include "Slave_can_protocol.h"
 
 #define DelayForRespond delay_ms(6);
 
int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	uart2_init(256000);
	motor_init();
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);	/* 初始化CAN总线 */
	LED0 = 1;   
		while(1)
		{				
			if(Can_Receive_Msg(RecBuf.s) != 0)
			{
				matchMsg(&RecBuf);
			}
		}
}


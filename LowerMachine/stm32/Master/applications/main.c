/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@date on 2019-4-24  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Mecanum Mobile Platform application
 */
 
#include "sys_conf.h"

int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	uart2_init(115200);
	motor_init();
	LED0 = 1;
	
	C_S.Vx = 50;
	C_S.Vy = 500;
	C_S.W = 5;
	speed_inverse_solution();
}


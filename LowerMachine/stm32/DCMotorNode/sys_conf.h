/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Mecanum Mobile Platform firmware config file
 */
 
#ifndef _SYS_CONF_H_
#define _SYS_CONF_H_

/**
 *@description include  
 *在这里定义整个代码include的头文件
 */

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "motor_gpio.h"
#include "led.h"
#include "encoder.h"
#include "motor_operate.h"
#include "pid.h"
#include "exit.h"
#include "SoftwareIIC.h"
#include "usart.h"
#include "as5600.h"
#include "can_protocol.h"

/*
 *配置节点
 */
#define SLAVE0 0
#define SLAVE1 0
#define SLAVE2 0
#define SLAVE3 1

/**
 *@description define 
 *在这里配置用户代码中的define值
 */
#define TIM3_ARR 99
#define TIM3_PSC 71
#define TIM4_ARR 100
#define TIM4_PSC 7199
#define waiting_forever 0
#define error -1

#endif

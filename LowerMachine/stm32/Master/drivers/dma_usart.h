#ifndef __DMA_USART_H
#define __DMA_USART_H

#include "stm32f10x.h"
#include <stdio.h>


// 串口工作参数宏定义
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

// USART 接收中断配置
#define  DEBUG_USART_DMA_NVIC_IRQn 			USART1_IRQn
#define  DEBUG_USART_DMA_NVIC_Pre				3		//抢占优先级
#define  DEBUG_USART_DMA_NVIC_Sub				3		//子优先级
#define  DEBUG_USART_DMA_IRQHandler			USART1_IRQHandler

// 串口对应的DMA请求通道
#define  DEBUG_TX_DMA_CHANNEL     			DMA1_Channel4
#define  DEBUG_RX_DMA_CHANNEL     			DMA1_Channel5

// 外设寄存器地址
#define  USART_DR_ADDRESS        				(USART1_BASE+0x04)

// 一次发送的数据量
#define  DEBUG_TX_BUFF_SIZE							50
#define  DEBUG_RX_BUFF_SIZE							50

// 开启中断接收
#define  DEBUG_RX_Start									USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE)

void USART_Config(void);
void USARTx_DMA_Config(void);
void DEBUG_USART_DMA_Tx_Start(u8 RX_BUFF_SIZE);

#endif

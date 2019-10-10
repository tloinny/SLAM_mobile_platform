#ifndef __DMA_USART1_DEBUG_H
#define __DMA_USART1_DEBUG_H

#include "stm32f10x.h"
#include <stdio.h>


// ���ڹ��������궨��
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

// USART ���տ����ж�����
#define  DEBUG_USART_DMA_NVIC_IRQn 			USART1_IRQn
#define  DEBUG_USART_DMA_NVIC_Pre				3		//��ռ���ȼ�
#define  DEBUG_USART_DMA_NVIC_Sub				3		//�����ȼ�
#define  DEBUG_USART_DMA_IRQHandler			USART1_IRQHandler

// USART DMA��������ж�����
#define  DEBUG_TC_DMA_NVIC_IRQn 				DMA1_Channel4_IRQn
#define  DEBUG_TC_DMA_NVIC_Pre					3		//��ռ���ȼ�
#define  DEBUG_TC_DMA_NVIC_Sub					3		//�����ȼ�
#define  DEBUG_TC_DMA_IRQHandler				DMA1_Channel4_IRQHandler
#define  DEBUG_TC_DMA_IT_FLAG						DMA1_FLAG_TC4

// ���ڶ�Ӧ��DMA����ͨ��
#define  DEBUG_TX_DMA_CHANNEL     			DMA1_Channel4
#define  DEBUG_RX_DMA_CHANNEL     			DMA1_Channel5

// ����Ĵ�����ַ
#define  DEBUG_USART_DR_ADDRESS        	(USART1_BASE+0x04)

// һ�η��͵�������
#define  DEBUG_TX_BUFF_SIZE							50
#define  DEBUG_RX_BUFF_SIZE							50

// �жϽ��տ���
#define  DEBUG_RX_Start									USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE)
#define  DEBUG_RX_Stop									USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, DISABLE)

void DEBUG_USARTx_DMA_Config(void);
void DEBUG_USART_DMA_Tx_Start(u8* DEBUG_Tx_Array, u8 Length);

#endif

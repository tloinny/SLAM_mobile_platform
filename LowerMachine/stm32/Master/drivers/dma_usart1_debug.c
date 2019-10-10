/**
  ******************************************************************************
	* @brief 	�������Ǵ���DMA�շ�����
  * @author Ԭ����
  ******************************************************************************
  * @attention 
  ******************************************************************************
  */
	
#include "dma_usart1_debug.h"

u8 DEBUG_Tx_Buff[DEBUG_TX_BUFF_SIZE];
u8 DEBUG_Rx_Buff[DEBUG_RX_BUFF_SIZE];
__IO u8 DEBUG_Receive_length = 0;//���յ������ݳ���


void DEBUG_USART_Config(void);

/**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void DEBUG_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_DMA_NVIC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_DMA_NVIC_Pre;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DEBUG_USART_DMA_NVIC_Sub;	//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);	

	//�������ڿ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}


/**
  * @brief  USARTx TX DMA ���ã��ڴ浽����(USART1->DR)
  * @param  None
  * @retval None
  */
void DEBUG_USARTx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	DEBUG_USART_Config();
	
	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

/* TX_Init */ 
	// ����DMAԴ��ַ���������ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_ADDRESS;
	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DEBUG_Tx_Buff;
	// ���򣺴��ڴ浽����	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// �����С	
	DMA_InitStructure.DMA_BufferSize = DEBUG_TX_BUFF_SIZE;
	// �����ַ����	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// �������ݵ�λ	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	// DMAģʽ��һ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	// ���ȼ�����	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// ����DMAͨ��		   
	DMA_Init(DEBUG_TX_DMA_CHANNEL, &DMA_InitStructure);
	// DMAͨ��ѡ�񵽴���
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	// DMA��������ж�ʹ��
	DMA_ITConfig(DEBUG_TX_DMA_CHANNEL,DMA_IT_TC,ENABLE);
	// DMA��������ж�����
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_TC_DMA_NVIC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_TC_DMA_NVIC_Pre;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = DEBUG_TC_DMA_NVIC_Sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
  NVIC_Init(&NVIC_InitStructure);
	
/* RX_Init */
	// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_ADDRESS;
	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DEBUG_Rx_Buff;
	// ���򣺴����赽�ڴ�
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	// �����С	
	DMA_InitStructure.DMA_BufferSize = DEBUG_RX_BUFF_SIZE;
	// �����ַ����	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// �������ݵ�λ	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	// DMAģʽ��һ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	// ���ȼ�����	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// ����DMAͨ��		   
	DMA_Init(DEBUG_RX_DMA_CHANNEL, &DMA_InitStructure);		
	// DMAͨ��ѡ�񵽴���
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);
	// ʹ��DMA���������գ�
	DMA_Cmd(DEBUG_RX_DMA_CHANNEL,ENABLE);
}

/**
  * @brief  �����жϷ������������жϣ�
  * @param  None
  * @retval None
	* @note		������һ֡���ݺ�Ҫ������һ�ν��գ����뽫Receive_length���㲢DEBUG_RX_Start
  */
void DEBUG_USART_DMA_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_IDLE) != RESET)
	{
		if(DEBUG_Receive_length == 0)
		{
			DEBUG_RX_Stop;
			DMA_Cmd(DEBUG_RX_DMA_CHANNEL, DISABLE);//��DMA
			DEBUG_Receive_length = DEBUG_RX_BUFF_SIZE - DEBUG_RX_DMA_CHANNEL->CNDTR;//��ȡ���ݳ��ȣ����õĻ������ܳ���-��ǰʣ��Ļ��������ȣ�
			DEBUG_RX_DMA_CHANNEL->CNDTR = DEBUG_RX_BUFF_SIZE;//�������ý��յ���������
			DEBUG_RX_DMA_CHANNEL->CMAR = (u32)DEBUG_Rx_Buff;//�������ý��յ��ڴ��ַ
			DMA_Cmd(DEBUG_RX_DMA_CHANNEL, ENABLE);//������֡���ݽ���
		}
		DEBUG_USARTx->SR; 
		DEBUG_USARTx->DR; //��USART_IT_IDLE��־
	}
}

/**
	* @brief  ����DMA��������жϷ�����
  * @param  None
  * @retval None
  */
void DEBUG_TC_DMA_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DEBUG_TC_DMA_IT_FLAG))
	{
		DEBUG_RX_Start;
		DMA_ClearFlag(DEBUG_TC_DMA_IT_FLAG); //����жϱ�־
	}
}

/**
	* @brief  ����DMA���ͺ���
	* @param  Tx_Array���������ݴ�ŵ�����
						Length  ��Ҫ���͵����ݳ���
  * @retval None
  */
void DEBUG_USART_DMA_Tx_Start(u8* Tx_Array, u8 Length)
{
	DEBUG_RX_Stop;//�ؽ���
	DMA_Cmd(DEBUG_TX_DMA_CHANNEL, DISABLE);
	DEBUG_TX_DMA_CHANNEL->CMAR = (u32)Tx_Array;//���÷��͵��ڴ��ַ
	DEBUG_TX_DMA_CHANNEL->CNDTR = Length;//���÷��͵����ݳ���
	DMA_Cmd(DEBUG_TX_DMA_CHANNEL, ENABLE);
}


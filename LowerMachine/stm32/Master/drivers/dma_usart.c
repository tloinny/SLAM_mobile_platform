/**
  ******************************************************************************
	* @brief:	该驱动是串口DMA收发驱动
  * @attention
	* 1.发送：先将即将发送的数据填入DEBUG_TxBuff，然后调用DEBUG_USART_DMA_Tx_Start
	* 2.接收：初始化后默认开启接收，接收完成后要进行下一次接收必须开启中断接收
	* 3.接收时可能偶尔出现错误：接收到两次的数据连在一起/数据只传一个字节就断掉
  ******************************************************************************
  */
	
#include "dma_usart.h"


char DEBUG_Tx_Buff[DEBUG_TX_BUFF_SIZE];
char DEBUG_Rx_Buff[DEBUG_RX_BUFF_SIZE];
__IO u8 DEBUG_Receive_length = 0;//接收到的数据长度

/**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_DMA_NVIC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DEBUG_USART_DMA_NVIC_Pre;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DEBUG_USART_DMA_NVIC_Sub;	//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);	
	
	//开启串口空闲中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}


/**
  * @brief  USARTx TX DMA 配置，内存到外设(USART1->DR)
  * @param  None
  * @retval None
  */
void USARTx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	USART_Config();
	
	// 开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

/* TX_Init */ 
	// 设置DMA源地址：串口数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;
	// 内存地址(要传输的变量的指针)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DEBUG_Tx_Buff;
	// 方向：从内存到外设	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// 传输大小	
	DMA_InitStructure.DMA_BufferSize = DEBUG_TX_BUFF_SIZE;
	// 外设地址不增	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 内存地址自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 外设数据单位	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// 内存数据单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	// DMA模式，一次模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	// 优先级：中	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	// 禁止内存到内存的传输
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// 配置DMA通道		   
	DMA_Init(DEBUG_TX_DMA_CHANNEL, &DMA_InitStructure);
	// DMA通道选择到串口
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	// 使能DMA（启动传输）
	DMA_Cmd(DEBUG_TX_DMA_CHANNEL,ENABLE); 
	
/* RX_Init */
	// 设置DMA源地址：串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;
	// 内存地址(要传输的变量的指针)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DEBUG_Rx_Buff;
	// 方向：从外设到内存
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	// 传输大小	
	DMA_InitStructure.DMA_BufferSize = DEBUG_RX_BUFF_SIZE;
	// 外设地址不增	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 内存地址自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 外设数据单位	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// 内存数据单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	// DMA模式，一次模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	// 优先级：中	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	// 禁止内存到内存的传输
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// 配置DMA通道		   
	DMA_Init(DEBUG_RX_DMA_CHANNEL, &DMA_InitStructure);		
	// DMA通道选择到串口
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);
	// 使能DMA（启动接收）
	DMA_Cmd(DEBUG_RX_DMA_CHANNEL,ENABLE);
}

/**
  * @brief  串口中断服务函数（空闲中断）
  * @param  None
  * @retval None
  */
void DEBUG_USART_DMA_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USARTx, USART_IT_IDLE) != RESET)
	{
		if(DEBUG_Receive_length == 0)
		{
			USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, DISABLE);
			DMA_Cmd(DEBUG_RX_DMA_CHANNEL, DISABLE);//关DMA
			DEBUG_Receive_length = DEBUG_RX_BUFF_SIZE - DEBUG_RX_DMA_CHANNEL->CNDTR;//获取数据长度（设置的缓冲区总长度-当前剩余的缓冲区长度）
			DEBUG_RX_DMA_CHANNEL->CNDTR = DEBUG_RX_BUFF_SIZE;//重新设置接收的数据数量
			DEBUG_RX_DMA_CHANNEL->CMAR = (u32)DEBUG_Rx_Buff;//重新设置接收的内存地址
			DMA_Cmd(DEBUG_RX_DMA_CHANNEL, ENABLE);//开启下帧数据接收
		}
		DEBUG_USARTx->SR; 
		DEBUG_USARTx->DR; //清USART_IT_IDLE标志
	}
}

/**
	* @brief  串口DMA发送函数，发送前应把要发送的数据填入全局数组DEBUG_TxBuff中
	* @param  Length：要发送的数据长度
  * @retval None
  */
void DEBUG_USART_DMA_Tx_Start(u8 Length)
{
	DMA_Cmd(DEBUG_TX_DMA_CHANNEL, DISABLE);
	DEBUG_TX_DMA_CHANNEL->CMAR = (u32)DEBUG_Tx_Buff;//设置发送的内存地址
	DEBUG_TX_DMA_CHANNEL->CNDTR = Length;//设置发送的数据长度
	DMA_Cmd(DEBUG_TX_DMA_CHANNEL, ENABLE);
}



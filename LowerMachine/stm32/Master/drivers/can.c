/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: CAN�ײ�����
 */
 
#include "sys_conf.h"

/**
 *@function CAN��ʼ��
 *@param	
 *				tsjw:	����ͬ����Ծʱ�䵥Ԫ����Χ:CAN_SJW_1tq~ CAN_SJW_4tq
 *				tbs2:	ʱ���2��ʱ�䵥Ԫ����Χ:CAN_BS2_1tq~CAN_BS2_8tq;
 *				tbs1:	ʱ���1��ʱ�䵥Ԫ����Χ:CAN_BS1_1tq ~CAN_BS1_16tq
 *				brp :	�����ʷ�Ƶ������Χ:1~1024;  tq=(brp)*tpclk1
 *							������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
 *							mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
 *							Fpclk1��ʱ���ڳ�ʼ��ʱ����Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
 *							������:36M/((8+9+1)*4)=500Kbps
 *@return	0,��ʼ���ɹ�;
 *   		 ����,��ʼ��ʧ��; 
 */
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);/* ʹ��PORTAʱ��	*/                   											 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);/* ʹ��CAN1ʱ�� */
	
	/* CAN IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* �������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure);	/* PA12----CAN_TX */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* �������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure);			/* PA11----CAN_RX */

	/* CAN��Ԫ���� */
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode= mode;
	
	/* ���������� */
	CAN_InitStructure.CAN_SJW=tsjw;
	CAN_InitStructure.CAN_BS1=tbs1;
	CAN_InitStructure.CAN_BS2=tbs2;
	CAN_InitStructure.CAN_Prescaler=brp; 
	CAN_Init(CAN1, &CAN_InitStructure);	/* ��ʼ��CAN1 */

	CAN_FilterInitStructure.CAN_FilterNumber=0;	/* ������0 */
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; /* ����λ��ģʽ */
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	/* 32λ�� */ 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x2000;	/* 32λID */
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x2000;	/* 32λMASK */
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;	/* ������������FIFO0 */
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;	/* ��������� */

	CAN_FilterInit(&CAN_FilterInitStructure);			/* ��������ʼ�� */
	
#if CAN_RX0_INT_ENABLE 	/* ���ʹ��RX0�ж� */
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	/* FIFO��Ϣ�Һ��ж����� */

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   /* �����ȼ�Ϊ0 */        
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	/* ���ʹ��RX0�ж� */		    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1, 0, &RxMessage);
	for(i=0;i<8;i++)
	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

/**
 *@function can����һ������
 *�̶���ʽ��IDΪ0x12����׼֡������֡
 *@param 
 *				len:���ݳ���(max 8)		     
 *				msg:����ָ�룬���8���ֽ�
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 Can_Send_Msg(u8* msg,u8 len,u32 ID)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=(ID >> 21);			/* ��׼��ʶ�� */
	TxMessage.ExtId=((ID >> 3)&0x3ffff);			/* ������չ��ʶ�� */
	TxMessage.IDE=CAN_Id_Standard; 	/* ��׼֡ */
	TxMessage.RTR=CAN_RTR_Data;		/* ����֡ */
	TxMessage.DLC=len;				/* Ҫ���͵����ݳ��� */
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	/* �ȴ����ݷ��� */
	if(i>=0XFFF)return 1;
	return 0;	 
}

/**
 *@function	CAN�������ݲ�ѯ
 *@param
 *				buf:���ݻ�����
 *@return
 *				0----������
 *				����----���յ������ݳ���
 */
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
  if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		/* �ж��Ƿ���յ����� */
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	/* ��ȡ���� */
  for(i=0;i<8;i++)
	{
    buf[i]=RxMessage.Data[i];
	}
	return RxMessage.DLC;	
}

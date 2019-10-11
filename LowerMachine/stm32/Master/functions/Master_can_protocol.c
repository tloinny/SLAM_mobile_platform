/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: CANͨѶЭ������
 */
 
#include "Master_can_protocol.h"

//u8 can_send_buf[can_buf_size] = {0};	/* CAN���߷��ͻ����� */
u8 can_rec_buf[can_buf_size] = {0};	/* CAN���߽��ջ����� */
u32 slave[slave_num_max] = {slave_0,slave_1,slave_2,slave_3,slave_4,slave_5};	/* �ڵ�ID�� */
u8 slave_num = 0;	/* ���ýڵ���� */
u8 ready_num = 0;	/* ��׼���õĽڵ���� */
u8 ready_list[slave_num_max] = {0};	/* ׼���õĽڵ��б� */
Buf SendBuf;

/**
 *@function CAN��ӻ������ٶ���Ϣ�ͻ����ƵĽǶ���Ϣ
 *@param 
 *			dir:������Ϣ
 *			speed:�ٶ���Ϣ	
 *				ID:�ӻ���ַ	��:slave_0|slave_1,��ʾ���͸��ӻ�0�ʹӻ�1
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 CAN_send_motion_info(u8 dir, float speed, u32 ID)
{
	u8 result = 0;
	SendBuf.f = speed;
	SendBuf.s[4] = dir;
	result = Can_Send_Msg(SendBuf.s, can_buf_size, ID);
	return result;
}

/**
 *@function CAN��ӻ�����ָ��
 *@param 
 *				cmd:����
 *				ID:�ӻ���ַ	��:slave_0|slave_1,��ʾ���͸��ӻ�0�ʹӻ�1
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 CAN_send_cmd(u8 cmd, u32 ID)
{
	u16 i;
	u8 result;
	for(i = 0; i < can_buf_size; ++i) *(SendBuf.s + i) = cmd;
	result = Can_Send_Msg(SendBuf.s, can_buf_size, ID);
	clean_can_send_buf();
	return result;
}

/**
 *@function CAN��ӻ��������ݷַ�
 *@param 
 *				buf:����Դ
 *				len:����Դ����
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 CAN_distribute(u8 * buf, u8 len)
{
	int i = 1;
	u8 result=0;
	for(;i<len-2;i+=3)	/* ������Ϊ3����buf */
	{
		*can_send_buf=*(buf+i);
		*(can_send_buf+1)=*(buf+i+1);
		*(can_send_buf+3)=0;
		*(can_send_buf+4)=*(buf+i+2);	/* ����buf����can_send_buf�ı�Ҫλ */
		if(slave[(i-1)/3] != 0)	/* ����ڵ���ڣ���ַ����� */
		{
			result += Can_Send_Msg(can_send_buf, can_buf_size, slave[(i-1)/3]);	/* �ַ����� */
			DEBUG_USART_DMA_Tx_Start(can_send_buf, can_buf_size);	/* ����λ������ */
		}
		DelayForRespond
		clean_can_send_buf();
	}
	return result;
}

/**
 *@function CAN��ӻ����к��У�ȷ����Щ�ڵ��Ǵ����������ϵģ�������д��slav[]�С������ڵĽڵ�ID����
 *@param void
 *@return void
 */
void CAN_Call()
{
	u32 time_out = 10;
	u8 temp_buf[8]={0};
	u32 count = 0;
	int i = 0;
	for(;i<slave_num_max;++i)
	{
		CAN_send_cmd(C_CALL,slave[i]);
		for(count=0;(Can_Receive_Msg(temp_buf) == 0) && count < time_out; ++count,delay_ms(6));	/* �����Եȴ��ӻ��ظ� */
		if(count >= time_out || !(temp_buf[0]== 'R'&&temp_buf[1]== 'C')) /* ����ȴ���ʱ���߷�����������Ϊ�ýڵ㲻���� */
		{
			slave[i] = 0;
		}else
		{
			++slave_num;	/* ������ýڵ��� */
		}
	}
}

/**
 *@function ���can_send_buf�е�����
 *@param void
 *@return void
 */
void clean_can_send_buf()
{
	int i = 0;
	for(;i<can_buf_size;++i)
	{
		SendBuf.s[i] = 0;
	}
}

/**
 *@function ���can_rec_buf�е�����
 *@param void
 *@return void
 */
void clean_can_rec_buf()
{
	int i = 0;
	for(;i<can_buf_size;++i)
	{
		can_rec_buf[i] = 0;
	}
}

/**
 *@function ƥ�����Դӻ��ķ�����Ϣ������������Ӧ
 *@param void
 *@return void
 */
void match_feedback(u8* feedback)
{
	u8 i;
	u8 result = 0;
	switch(*feedback)
	{
		case 'R':	/* c_motor_ready:��ʾĳ���ӻ��Ѿ�����׼������ */
				if((*(feedback+1)-'0')>=0 && (*(feedback+1)-'0')<slave_num_max)
				{
					if(ready_list[(*(feedback+1)-'0')] == 0)
					{
						ready_list[(*(feedback+1)-'0')] = 1;	/* ��־Ϊ��׼����� */
						++ready_num;
					}
				}
				for(i=0;i<slave_num_max && ready_num == slave_num;++i) /* ��������Ч�ڵ㶼���׼������ */
				{
					result = 1 && (slave[i]*ready_list[i] == slave[i]);	/* ��ֹһЩ��δ֪ԭ���½������ߵĽڵ�Դ˲������� */
				}
				if(result) CAN_send_cmd(C_ACTION,slave_all);	/* ֪ͨ���дӻ���ʼ������� */
			break;
		default:
			break;		
	}
	clean_can_rec_buf();
}

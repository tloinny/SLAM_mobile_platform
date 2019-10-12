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

float speed_feedback_buf[4] = {0};	/* speed feedback recieve buffer */

u32 slave[slave_num_max] = {slave_0,slave_1,slave_2,slave_3,slave_4,slave_5};	/* �ڵ�ID�� */
u8 slave_num = 0;	/* ���ýڵ���� */
u8 ready_num = 0;	/* ��׼���õĽڵ���� */
u8 ready_list[slave_num_max] = {0};	/* ׼���õĽڵ��б� */

Buf SendBuf;
Buf RecBuf;

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
u8 CAN_send_wheel_speed(float speed, u32 ID)
{
	u8 result = 0;
	SendBuf.f = speed;
	SendBuf.s[4] = 'S';
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
u8 CAN_distribute()
{
	u8 result=0;
		for(int i=0;i<4;++i)
		{
			if(slave[i] != 0)
			{
				result += CAN_send_wheel_speed(wheel_speed[i], slave[i]);
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
		clean_can_rec_buf();
	}
}

/**
 *@function Ask all the lower machine to feedback thire speed right now
 *@param void
 *@return void
 */
void CAN_CallForFeedBack()
{
	u32 time_out = 10;
	u32 count = 0;
	for(int i=0;i<slave_num_max;++i)
	{
		CAN_send_cmd(C_FEEDBACK,slave[i]);
		for(count=0;(Can_Receive_Msg(RecBuf.s) == 0) && count < time_out; ++count,delay_ms(6));	/* �����Եȴ��ӻ��ظ� */
		if(count >= time_out || !(RecBuf.s[4] == 'F' && RecBuf.s[5] == 'S' && RecBuf.s[6] - '0' == i))
		{
			speed_feedback_buf[i] = 0;
		}else
		{
			speed_feedback_buf[i] = RecBuf.f;
		}
		clean_can_rec_buf();
	}
}


/**
 *@function clean the data in SendBuf
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
 *@function clean the data in RecBuf
 *@param void
 *@return void
 */
void clean_can_rec_buf()
{
	int i = 0;
	for(;i<can_buf_size;++i)
	{
		RecBuf.s[i] = 0;
	}
}

/**
 *@function match the feedback from lower machine and do the corresponding action.
 *@param void
 *@return void
 */
void match_feedback(u8* feedback)
{
	u8 i;
	u8 result = 0;
	switch(*(feedback+4))
	{
		case 'R':	/* c_motor_ready: means that there one of the lower machines is ready for action now */
				if((*(feedback+5)-'0')>=0 && (*(feedback+5)-'0')<slave_num_max)
				{
					if(ready_list[(*(feedback+5)-'0')] == 0)
					{
						ready_list[(*(feedback+5)-'0')] = 1;	/* mark as allready */
						++ready_num;
					}
				}
				for(i=0;i<slave_num_max && ready_num == slave_num;++i) /* make sure that all the lower machines are ready */
				{
					result = (1 && (slave[i]*ready_list[i] == slave[i]));	/* In order to avoid that there some unexpect slave node in the CAN bus */
				}
				if(result) CAN_send_cmd(C_ACTION,slave_all);	/* Ask all the slave node to action. */
		break;
		default:
		break;		
	}
	clean_can_rec_buf();
}

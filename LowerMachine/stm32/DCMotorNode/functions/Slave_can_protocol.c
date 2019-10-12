/**
 *@title 
 * Copyright: Copyright (c) 2019 
 *
 *@created on 2019-9-09  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:
 */

#include "Slave_can_protocol.h"

float speed_temp_buf = 0;

Buf SendBuf;
Buf RecBuf;

/**
 *@function CAN向主机发送反馈信息
 *@param 
 *				feedback:反馈信息
 *@return 
 *				0----成功
 *				其他----失败
 */
u8 CAN_send_feedback(u8 *feedback)
{
	u8 result;
	result = Can_Send_Msg(feedback, 8, master);
	return result;
}

u8 CAN_send_speed(float speed_f)
{
	u8 result;
	SendBuf.f = speed_f;
	SendBuf.s[4] = 'F';
	SendBuf.s[5] = 'S';
	#if SLAVE0
	SendBuf.s[6] = '0';
	#endif
	#if SLAVE1
	SendBuf.s[6] = '1';
	#endif
	#if SLAVE2
	SendBuf.s[6] = '2';
	#endif
	#if SLAVE3
	SendBuf.s[6] = '3';
	#endif
	result = Can_Send_Msg(SendBuf.s, can_buf_size, master);
	return result;
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


void matchMsg(Buf *buf)
{
	switch(buf->s[4])
	{
		case ACTION:
			wheel_speed_goal = speed_temp_buf;
		break;
		case SET_SPEED:
			buf->s[4] = 0;
			speed_temp_buf  = buf->f;
			clean_can_rec_buf();
			CAN_send_feedback(c_motor_ready);
		break;
		case CALL:
			CAN_send_feedback(c_receive_call);
		break;
		case STOP:
			motor_brake();
			CAN_send_feedback(c_motor_stoped);
		break;
		case ASK_FOR_SPEED_FEEDNACK:
			CAN_send_speed(speed_feedback);
		break;
		default:
		break;
	}
}

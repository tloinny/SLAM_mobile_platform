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

u8 can_send_buf[can_buf_size] = {0};	/* CAN发送缓存区 */
u8 can_rec_buf[can_buf_size] = {0};	/* CAN接收缓存区 */

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
	result = Can_Send_Msg(feedback, 3, master);
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
 *@function 清除CAN发送缓存区内容
 *@param void
 *@return void
 */
void clean_can_send_buf()
{
	int i = 0;
	for(;i<can_buf_size;++i)
	{
		can_send_buf[i] = 0;
	}
}

void matchMsg(Buf *buf)
{
	switch(buf->s[4])
	{
		case SET_SPEED:
			buf->s[4] = 0;
			wheel_speed_goal  = buf->f;
		break;
		case CALL:
			CAN_send_feedback(received_call);
		break;
		case STOP:
			motor_brake();
			CAN_send_feedback(motor_stoped);
		break;
		case ASK_FOR_SPEED_FEEDNACK:
			CAN_send_speed(speed_feedback);
		break;
		default:
		break;
	}
}

/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: CAN通讯协议驱动
 */
 
#include "Master_can_protocol.h"

float speed_feedback_buf[4] = {0};	/* speed feedback recieve buffer */

u32 slave[slave_num_max] = {slave_0,slave_1,slave_2,slave_3,slave_4,slave_5};	/* 节点ID表 */
u8 slave_num = 0;	/* 可用节点计数 */
u8 ready_num = 0;	/* 已准备好的节点计数 */
u8 ready_list[slave_num_max] = {0};	/* 准备好的节点列表 */

Buf SendBuf;
Buf RecBuf;

/**
 *@function CAN向从机发送速度信息和弧度制的角度信息
 *@param 
 *			dir:方向信息
 *			speed:速度信息	
 *				ID:从机地址	如:slave_0|slave_1,表示发送给从机0和从机1
 *@return 
 *				0----成功
 *				其他----失败
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
 *@function CAN向从机发送指令
 *@param 
 *				cmd:命令
 *				ID:从机地址	如:slave_0|slave_1,表示发送给从机0和从机1
 *@return 
 *				0----成功
 *				其他----失败
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
 *@function CAN向从机进行数据分发
 *@param 
 *				buf:数据源
 *				len:数据源长度
 *@return 
 *				0----成功
 *				其他----失败
 */
u8 CAN_distribute()
{
	u8 result=0;
		for(int i=0;i<4;++i)
		{
			SendBuf.f = wheel_speed[i];
			if(slave[i] != 0)
			{
				result += Can_Send_Msg(SendBuf.s, can_buf_size, slave[i]);
			}
			DelayForRespond
			clean_can_send_buf();
		}
	return result;
}

/**
 *@function CAN向从机进行呼叫，确定哪些节点是存在于总线上的，最终填写在slav[]中。不存在的节点ID置零
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
		for(count=0;(Can_Receive_Msg(temp_buf) == 0) && count < time_out; ++count,delay_ms(6));	/* 阻塞性等待从机回复 */
		if(count >= time_out || !(temp_buf[0]== 'R'&&temp_buf[1]== 'C')) /* 如果等待超时或者反馈出错则认为该节点不存在 */
		{
			slave[i] = 0;
		}else
		{
			++slave_num;	/* 计算可用节点数 */
		}
		clean_can_rec_buf();
	}
}

/**
 *@function 
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
		for(count=0;(Can_Receive_Msg(RecBuf.s) == 0) && count < time_out; ++count,delay_ms(6));	/* 阻塞性等待从机回复 */
		if(count >= time_out || !(RecBuf.s[4] == 'F' && RecBuf.s[5] == 'B' && RecBuf.s[6] == i))
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
 *@function 清除can_send_buf中的数据
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
 *@function 清除can_rec_buf中的数据
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
 *@function 匹配来自从机的反馈信息，并且做出反应
 *@param void
 *@return void
 */
void match_feedback(u8* feedback)
{
	u8 i;
	u8 result = 0;
	switch(*(feedback+4))
	{
		case 'R':	/* c_motor_ready:表示某个从机已经做好准备工作 */
				if((*(feedback+5)-'0')>=0 && (*(feedback+5)-'0')<slave_num_max)
				{
					if(ready_list[(*(feedback+1)-'0')] == 0)
					{
						ready_list[(*(feedback+1)-'0')] = 1;	/* 标志为已准备完成 */
						++ready_num;
					}
				}
				for(i=0;i<slave_num_max && ready_num == slave_num;++i) /* 当所有有效节点都完成准备工作 */
				{
					result = (1 && (slave[i]*ready_list[i] == slave[i]));	/* 防止一些在未知原因下进入总线的节点对此产生干扰 */
				}
				if(result) CAN_send_cmd(C_ACTION,slave_all);	/* 通知所有从机开始驱动电机 */
		break;
		default:
		break;		
	}
	clean_can_rec_buf();
}

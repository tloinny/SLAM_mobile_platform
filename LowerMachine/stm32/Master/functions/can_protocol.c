/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Abot�ؽڽڵ�CANͨѶЭ������
 */
 
#include "can_protocol.h"

u8 can_send_buf[can_buf_size] = {0};	/* CAN���߷��ͻ����� */
u8 can_rec_buf[can_buf_size] = {0};	/* CAN���߽��ջ����� */
u32 slave[slave_num_max] = {slave_0,slave_1,slave_2,slave_3,slave_4,slave_5};	/* �ڵ�ID�� */
u8 slave_num = 0;	/* ���ýڵ���� */
u8 ready_num = 0;	/* ��׼���õĽڵ���� */
u8 ready_list[slave_num_max] = {0};	/* ׼���õĽڵ��б� */
u8 arrive_num = 0;	/* ����Ľڵ���� */
u8 arrive_list[slave_num_max] = {0};	/* ����Ľڵ��б� */
u8 slave_buf_available = 1;	/* ��ʾ�ӻ��������Ƿ���ã����ھ����Ƿ����������湦�� */

/**
 *@function CAN��ӻ������ٶ���Ϣ�ͻ����ƵĽǶ���Ϣ
 *@param 
 *			rad:�Ƕ���Ϣ
 *			speed:�ٶ���Ϣ	
 *				ID:�ӻ���ַ	��:slave_0|slave_1,��ʾ���͸��ӻ�0�ʹӻ�1
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 CAN_send_motion_info(float rad, float speed, u32 ID)
{
	u8 result = 0;
	u16 rad_temp = (u16)(rad * 1000);	/* ����1000���������ȷ��0.001 */
	u16 speed_temp = (u16)(speed * 10);
	*(can_send_buf) = (u8)(rad_temp%254);
	*(can_send_buf + 1) = (u8)(rad_temp/254);
	*(can_send_buf + 3) = 0;	/* '/0'��Ϊ�ָ��� */
	*(can_send_buf + 4) = (u8)(speed_temp%254);
	*(can_send_buf + 5) = (u8)(speed_temp/254);
	result = Can_Send_Msg(can_send_buf, can_buf_size, ID);
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
	for(i = 0; i < can_buf_size; ++i) *(can_send_buf + i) = cmd;
	result = Can_Send_Msg(can_send_buf, can_buf_size, ID);
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
		can_send_buf[i] = 0;
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
 *@function ʹ���нڵ�Ѱ�ҹؽ�ԭ��
 *@param void
 *@return 
 *				1������ԭ��Ѱ�ҳɹ�
 *				0������
 */
u8 home_all()
{
	u8 count;
	u8 i;
		int n = 0;
	u32 rec_history[slave_num_max] = {0};	/* ��¼��Щ�ڵ��Ѿ����͹���Ϣ,�����ظ����͵���� */
	u8 temp_buf[8]={0};
	CAN_send_cmd(C_HOME,slave_all);
	for(count=0;count<slave_num;)	/* �����Եȴ��ظ� */
	{
		DelayForRespond
		if(Can_Receive_Msg(temp_buf))
		{
			if(temp_buf[0] == 'H' && (temp_buf[1]-'0')>=0 && (temp_buf[1]-'0')<slave_num_max)
			{
				if(rec_history[(temp_buf[1]-'0')] == 0 && slave[(temp_buf[1]-'0')] != 0)	/* ��ֹ�ظ� */
				{
					rec_history[(temp_buf[1]-'0')] = 1;
					++count;
				}
			}	
		}	
		for(n=0;n<can_buf_size;++n)
		{
			temp_buf[n] = 0;
		}
	}
	for(i=0;i<slave_num_max;++i)	/* �ж��Ƿ����п��ýڵ㶼�Ѿ�Ѱ�ҵ�ԭ�� */
	{
		if(slave[i]*rec_history[i] != slave[i])
		{
			clean_can_rec_buf();
			return 0;
		}else
		{
			arrive_list[i] = rec_history[i];	/* ��־Ϊ����ָ��λ�� */
		}
	}
	return 1;
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
		case 'Q':	/* c_motion_request:��ʾĳ���ӻ������˶����� */
				if((*(feedback+1)-'0')>=0 && (*(feedback+1)-'0')<slave_num_max)
				{
					if(arrive_list[(*(feedback+1)-'0')] == 1)	/* ֻ�е��ӻ��Ѿ�����ָ��λ��ʱ���ܽ�����һ���˶���׼������ */
						CAN_send_cmd(C_READY,slave[*(feedback+1)-'0']);	/* ֪ͨ�ôӻ�����׼������ */
				}
			break;
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
		case 'H':	/* c_motor_home:��ʾĳ���ӻ��ĵ���Ѿ�����ԭ��λ�� */
			break;
		case 'A':	/* c_motor_arrive����c_motor_action */
				switch(*(feedback+1))
				{
					case 'R':	/* c_motor_arrive */
						if((*(feedback+2)-'0')>=0 && (*(feedback+2)-'0')<slave_num_max)
						{
							if(arrive_list[(*(feedback+2)-'0')] == 0)
							{
								arrive_list[(*(feedback+2)-'0')] = 1;	/* ��־Ϊ�ѵ���ָ��λ�� */
								++arrive_num;
							}
						}
						break;
					case 'C':	/* c_motor_action */
						if((*(feedback+2)-'0')>=0 && (*(feedback+2)-'0')<slave_num_max)
						{
							if(arrive_list[(*(feedback+2)-'0')] == 1)
							{
								arrive_list[(*(feedback+2)-'0')] = 0;	/* ��־Ϊδ����ָ��λ�� */
								--arrive_num;
							}
						}
						break;
					default:
						break;
				}
			break;
		case 'S':	/* c_motor_stop:��ʾĳ���ӻ��ĵ���Ѿ�ֹͣ */
			break;
		case 'D':	/* c_motor_disable:��ʾĳ���ӻ��ĵ���Ѿ�ʧ�� */								
			break;
		case 'E':	/* c_motor_ensable:��ʾĳ���ӻ��ĵ���Ѿ�ʹ�� */
			break;
		case 'F':	/* c_buf_full:��ʾĳ���ӻ����˶���Ϣ���������� */
			slave_buf_available = 0;
		break;
		case 'U':	/* c_buf_usefull:��ʾĳ���ӻ����˶���Ϣ�������ָ����� */
			slave_buf_available = 1;
		break;
		default:
			break;		
	}
	clean_can_rec_buf();
}

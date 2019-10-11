#include "USART_Communication.h"

UsartBuf usart_buf[3];

int Status = FREE;

void Match_CMD(u8* buf)
{
	switch(*(buf+4))
	{
		case SpeedInfo:
			Status = RecSpeed;
		break;
		case CallForSpeedFeedback:
			Status = SpeedFeedback;
		break;
		default:
			Status = FREE;
		break;
	}
}

void SendResponse(u8* buf, int size)
{
	DEBUG_USART_DMA_Tx_Start(buf,size);
}

void SendSpeedFeedBack(u8* buf, int size)
{
}

u8 CheckRecBuf()
{
	u8 result = 0;
	(DEBUG_Receive_length > 0)?(result = 1):(result = 0);
	return result;
}

void ConvertRecInfo2Vector(car_speed* c_s)
{
	for(int i=0;i<3;++i)	/* 上位机一共会发送3组速度信息，分别对应Vx,Vy,W */
	{
		while(CheckRecBuf())
		{
			if(i>=3) break;
			for(int n=0;n<DEBUG_Receive_length;++i)
			{
				usart_buf[i].s[n] = DEBUG_Rx_Buff[n];
			}
		}
	}
	c_s->Vx = usart_buf[0].f;
	c_s->Vy = usart_buf[1].f;
	c_s->W = usart_buf[2].f;
}

void Commuincate()
{
	if(CheckRecBuf())
	{
		Match_CMD(DEBUG_Rx_Buff);
		if(Status != FREE)
		{
			switch(Status)
			{
				case RecSpeed:
					SendResponse(BeginRecSpeedInfo,sizeof(BeginRecSpeedInfo)/sizeof(u8));	/* 通知上位机已经可以开始发送速度信息 */
					ConvertRecInfo2Vector(&C_S);
				break;
				case SpeedFeedback:
				break;
				default:
				break;
			}
		}
	}
}

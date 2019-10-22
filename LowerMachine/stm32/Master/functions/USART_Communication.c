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
		case StopAll:
			Status = AllStop;
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

void SendSpeedFeedBack(float* buf, int size)
{
	for(int i=0;i<size;++i)
	{
		usart_buf[0].f = *(buf+i);
		usart_buf[0].s[4] = 'M';
		usart_buf[0].s[5] = 'S';
		usart_buf[0].s[6] = i;
		DEBUG_USART_DMA_Tx_Start(usart_buf[0].s,8);
	}
}

u8 CheckRecBuf()
{
	u8 result = 0;
	(DEBUG_Receive_length > 0)?(result = 1):(result = 0);
	return result;
}

void ConvertRecInfo2Vector(car_speed* c_s)
{
	for(int i=0;i<3;)	/* 上位机一共会发送3组速度信息，分别对应Vx,Vy,W */
	{
		if(CheckRecBuf())
		{
			for(int n=0;n<DEBUG_Receive_length;++i)
			{
				usart_buf[i].s[n] = DEBUG_Rx_Buff[n];
			}
			++i;
		}
	}
	c_s->Vx = usart_buf[0].f;
	c_s->Vy = usart_buf[1].f;
	c_s->W = usart_buf[2].f;
}

void USART_Commuincate()
{
	if(CheckRecBuf())
	{
		SendResponse(BeginRecSpeedInfo,sizeof(BeginRecSpeedInfo)/sizeof(u8));	
		Match_CMD(DEBUG_Rx_Buff);
		if(Status != FREE)
		{
			switch(Status)
			{
				case RecSpeed:
					SendResponse(BeginRecSpeedInfo,sizeof(BeginRecSpeedInfo)/sizeof(u8));	/* Notice the host machine that is the time to send speed infomation. */
					ConvertRecInfo2Vector(&C_S);	/* Convert the speed infomation from USART to speed vector. */
					speed_inverse_solution();	/* Using the speed vector to do speed inverse solution and get the wheels' target speed. */
					CAN_distribute();	/* Distribute the wheels' to lower machines via CAN bus. */
					Status = FREE;	/* Set the working status to FREE. */
				break;
				case SpeedFeedback: 
					CAN_CallForFeedBack();	/* Ask the lower machines for speed feedback */
					SendResponse(BeginSendFeedback,sizeof(BeginSendFeedback)/sizeof(u8));	/* Notice the host machine that is the time to recieve the speed feedback. */
					SendSpeedFeedBack(speed_feedback_buf, 4);	/* Send the speed feedback to host machine via USART */
					Status = FREE;	/* Set the working status to FREE. */
				break;
				case AllStop:
					CAN_send_cmd(C_STOP,slave_all);
					Status = FREE;	/* Set the working status to FREE. */
				default:
				break;
			}
		}
	}
}

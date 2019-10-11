#include "USART_Communication.h"

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

void ConvertRecInfo2Vector(u8* Rec, car_speed* c_s)
{
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
				break;
				case SpeedFeedback:
				break;
				default:
				break;
			}
		}
	}
}

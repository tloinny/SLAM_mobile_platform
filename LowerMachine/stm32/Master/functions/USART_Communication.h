#ifndef _USART_COMMUNICATION_H_
#define _USART_COMMUNICATION_H_
#include "dma_usart1_debug.h"
#include "inverse_solution.h"
#include "Master_can_protocol.h"

#define buf_size 8

/**
 *CMD
 */
#define SpeedInfo 'S'
#define CallForSpeedFeedback 'C'

/**
 *Response
 */
#define BeginRecSpeedInfo "BRSI"
#define BeginSendFeedback	"BSFB"
#define Online	"ONLINE"

/**
 *STATUS
 */
#define RecSpeed 0
#define SpeedFeedback 1
#define FREE -1

typedef union UsartBuf
{
	u8 s[buf_size];
	float f;
}UsartBuf;

extern int Status;

void Match_CMD(u8* buf);
void SendResponse(u8* buf, int size);
void SendSpeedFeedBack(float* buf, int size);
u8 CheckRecBuf(void);
void ConvertRecInfo2Vector(car_speed* c_s);
void USART_Commuincate(void);

#endif

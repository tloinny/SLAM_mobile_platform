#ifndef _USART_COMMUNICATION_H_
#define _USART_COMMUNICATION_H_
#include "dma_usart1_debug.h"
#include "inverse_solution.h"

/**
 *CMD
 */
#define SpeedInfo 'S'
#define CallForSpeedFeedback 'C'

/**
 *Response
 */
#define BeginRecSpeedInfo "BRSI"
#define Online	"ONLINE"

/**
 *STATUS
 */
#define RecSpeed 0
#define SpeedFeedback 1
#define FREE -1

void Match_CMD(u8* buf);
void SendResponse(u8* buf, int size);
void SendSpeedFeedBack(u8* buf, int size);
u8 CheckRecBuf(void);
void ConvertRecInfo2Vector(u8* Rec, car_speed* c_s);
void Commuincate(void);

#endif

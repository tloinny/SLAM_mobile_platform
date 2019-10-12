#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	   
#include "led.h"

#define SLAVE0 1
#define SLAVE1 0
#define SLAVE2 0
#define SLAVE3 0

/*
 *ID
 */
#define master 			0x20000000	/* mask: 0x20000000 */
#define slave_0 		0x00200000	/* mask: 0x00200000 */
#define slave_1 		0x00400000	/* mask: 0x00400000 */
#define slave_2 		0x00800000	/* mask: 0x00800000 */
#define slave_3 		0x01000000	/* mask: 0x01000000 */
#define slave_4			0x02000000	/* mask: 0x02000000 */	
#define slave_5			0x04000000	/* mask: 0x04000000 */	
#define slave_all 	0x07Ef0000

#define CAN_RX0_INT_ENABLE	0					    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 Can_Send_Msg(u8* msg,u8 len,u32 ID);
u8 Can_Receive_Msg(u8 *buf);
#endif

#ifndef _CAN_PROTOCOL_H_
#define _CAN_PROTOCOL_H_

#include "can.h"
#include "dma_usart1_debug.h"

/*
 *数组容量设置
 */
#define can_buf_size 8
#define slave_num_max 6

/*
 *cmd
 */
#define C_CALL						'C'
#define C_READY						'R'
#define C_ACTION 					'A'
#define C_STOP 						'S'
#define C_HOME 						'H'
#define C_MOTOR_DISABLE		'D'
#define C_MOTOR_ENABLE		'E'


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

#define DelayForRespond delay_ms(6);

typedef union Buf
{
	u8 s[can_buf_size];
	float f;
}Buf;

extern Buf SendBuf;

/*
 *统一声明变量
 */
//extern u8 can_send_buf[];
extern u8 can_rec_buf[];
extern u8 slave_num;
extern u8 slave_buf_available;

u8 CAN_send_motion_info(u8 dir, float speed, u32 ID);
u8 CAN_send_cmd(u8 cmd, u32 ID);
u8 CAN_distribute(void);
void CAN_Call(void);
void clean_can_send_buf(void);
void clean_can_rec_buf(void);
u8 home_all(void);
void match_feedback(u8* feedback);
#endif

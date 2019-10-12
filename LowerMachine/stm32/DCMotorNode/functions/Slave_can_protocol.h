#ifndef _CAN_PROTOCOL_H_
#define _CAN_PROTOCOL_H_
#include "can.h"
#include "motor_operate.h"

/*
 *cmd
 */
#define SET_SPEED								'S'
#define CALL										'C'
#define STOP 										'T'
#define ASK_FOR_SPEED_FEEDNACK	'A'
/*
 *feedback
 */
#define received_call	"RC"
#define motor_stoped	"MS"

#define can_buf_size 8

typedef union Buf
{
	u8 s[can_buf_size];
	float f;
}Buf;

extern Buf SendBuf;
extern Buf RecBuf;

u8 CAN_send_feedback(u8 *feedback);
u8 CAN_send_speed(float speed_f);
void clean_can_send_buf(void);
void matchMsg(Buf *buf);

#endif

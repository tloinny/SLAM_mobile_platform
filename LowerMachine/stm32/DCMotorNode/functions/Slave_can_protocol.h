#ifndef _CAN_PROTOCOL_H_
#define _CAN_PROTOCOL_H_
#include "can.h"
#include "motor_operate.h"

/*
 *cmd
 */
#define ACTION									'A'
#define SET_SPEED								'S'
#define CALL										'C'
#define STOP 										'T'
#define ASK_FOR_SPEED_FEEDNACK	'F'

/*
 *feedback
 */
#if SLAVE0
#define c_receive_call			"RC"
#define c_motor_ready 			"R0"
#define c_motor_stoped			"S0"
#endif

#if SLAVE1
#define c_receive_call			"RC"
#define c_motor_ready 			"R1"
#define c_motor_stoped			"S1"
#endif

#if SLAVE2
#define c_receive_call			"RC"
#define c_motor_ready 			"R2"
#define c_motor_stoped			"S2"
#endif

#if SLAVE3
#define c_receive_call			"RC"
#define c_motor_ready 			"R3"
#define c_motor_stoped			"S3"
#endif

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

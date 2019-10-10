#ifndef _AS5600_H_
#define _AS5600_H_
#include "SoftwareIIC.h"
#include "usart.h"
#include "timer.h"

#define SlaveReceiverMode 0x00
#define SlaveTransmitterMode 0x01

void as5600_init(void);

u8 getAddress(void);       
u16 setMaxAngle(u16 newMaxAngle);
u16 getMaxAngle(void);
u16 setStartPosition(u16 startAngle);
u16 getStartPosition(void);
u16 setEndPosition(u16 endAngle);
u16 getEndPosition(void);
u16 getRawAngle(void);
u16 getScaledAngle(void);
u8 detectMagnet(void);
u8 getMagnetStrength(void);
u8 getAgc(void);
u8 getMagnitude(void);
u8 getBurnCount(void);
int burnAngle(void);
int burnMaxAngleAndConfig(void);

u8 readOneByte(u8 read_adr);
u16 readTwoBytes(u8 read_adr_hi, u8 read_adr_lo);
void writeOneByte(u8 adr_in, u8 dat_in);

float convertRawAngleToDegrees(u16 newAngle);
float convertScaledAngleToDegrees(u16 newAngle);
float getRawDegrees(void);
float getScaledDegrees(void);

#endif

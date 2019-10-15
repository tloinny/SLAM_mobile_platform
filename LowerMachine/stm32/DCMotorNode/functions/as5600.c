#include "as5600.h"

const u8 _ams5600_Address = 0x36;
  
u16 _rawStartAngle;
u16 _zPosition;
u16 _rawEndAngle;
u16 _mPosition;
u16 _maxAngle;

/* Registers */
const u8 _zmco 				= 0x00;
const u8 _zpos_hi 		= 0x01; /*zpos[11:8] high nibble  START POSITION */
const u8 _zpos_lo 		= 0x02; /*zpos[7:0] */
const u8 _mpos_hi 		= 0x03; /*mpos[11:8] high nibble  STOP POSITION */
const u8 _mpos_lo 		= 0x04; /*mpos[7:0] */
const u8 _mang_hi 		= 0x05; /*mang[11:8] high nibble  MAXIMUM ANGLE */
const u8 _mang_lo 		= 0x06; /*mang[7:0] */
const u8 _conf_hi 		= 0x07;     
const u8 _conf_lo 		= 0x08;
const u8 _raw_ang_hi	= 0x0c;
const u8 _raw_ang_lo	= 0x0d;
const u8 _ang_hi 			= 0x0e;
const u8 _ang_lo 			= 0x0f;
const u8 _stat 				= 0x0b;
const u8 _agc 				= 0x1a;
const u8 _mag_hi			= 0x1b;
const u8 _mag_lo			= 0x1c;
const u8 _burn				= 0xff;

void as5600_init()
{
	IIC_Init();
	if(detectMagnet() == 0)
	{
		while(1)
		{
			if(detectMagnet() == 1)
			{
				printf("Current Magnitude: %d\r\n",getMagnitude());
				setStartPosition(0);
				setEndPosition(360);
				delay_ms(500);
				break;
			}else
			{
				printf("Can't detect magnet\r\n");
			}
			delay_ms(1000);
		}
	}
	TIM1_Init(10);
	TIM2_Init(0.1);
}

u8 getAddress()
{
  return _ams5600_Address; 
}

u16 setMaxAngle(u16 newMaxAngle)
{
  u16 retVal;
  _maxAngle = newMaxAngle;
 
  writeOneByte(_mang_hi, (_maxAngle&0xff00)>>8);
  delay_us(2); 
  writeOneByte(_mang_lo, (_maxAngle&0x00ff)<<8); 
  delay_us(2);         

  retVal = readTwoBytes(_mang_hi, _mang_lo);
  return retVal;
}

u16 getMaxAngle(void)
{
	return readTwoBytes(_mang_hi, _mang_lo);
}

u16 setStartPosition(u16 startAngle)
{
  _rawStartAngle = startAngle;

  writeOneByte(_zpos_hi, (_rawStartAngle&0xff00)>>8);
  delay_us(2); 
  writeOneByte(_zpos_lo, (_rawStartAngle&0x00ff)<<8); 
  delay_us(2);                
  _zPosition = readTwoBytes(_zpos_hi, _zpos_lo);
  
  return(_zPosition);
}

u16 getStartPosition(void)
{
	return readTwoBytes(_zpos_hi, _zpos_lo);
}

u16 setEndPosition(u16 endAngle)
{
  _rawEndAngle = endAngle;
 
  writeOneByte(_mpos_hi, (_rawEndAngle&0xff00)>>8);
  delay_us(2); 
  writeOneByte(_mpos_lo, (_rawEndAngle&0x00ff)<<8); 
  delay_us(2);                
  _mPosition = readTwoBytes(_mpos_hi, _mpos_lo);
  
  return(_mPosition);
}

u16 getEndPosition(void)
{
	return readTwoBytes(_mpos_hi, _mpos_lo);
}

u16 getRawAngle(void)
{
	return readTwoBytes(_raw_ang_hi, _raw_ang_lo);
}

/**
 *@function get Scaled Angle
 *@param none
 *@return value of scaled angle register
 */
u16 getScaledAngle(void)
{
	return readTwoBytes(_ang_hi, _ang_lo);
}

/**
 *@function detectMagnet
 *@param none
 *@return 1 if magnet is detected, 0 if not
 */
u8 detectMagnet(void)
{
  u8 magStatus;
  u8 retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = AGC minimum overflow, Magnet to strong */
  /* ML high = AGC Maximum overflow, magnet to weak*/ 
  /* MH high = magnet detected*/ 
  magStatus = readOneByte(_stat);
  
  if(magStatus & 0x20)
    retVal = 1; 
  
  return retVal;
}

/**
 *@function getMagnetStrength
 *@param none
 *@return 0 if no magnet is detected
 *				1 if magnet is to weak
 *				2 if magnet is just right
 *				3 if magnet is to strong
 */
u8 getMagnetStrength(void)
{
  int magStatus;
  int retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = AGC minimum overflow, Magnet to strong */
  /* ML high = AGC Maximum overflow, magnet to weak*/ 
  /* MH high = magnet detected*/ 
  magStatus = readOneByte(_stat);
  if(detectMagnet() ==1)
  {
      retVal = 2; /*just right */
      if(magStatus & 0x10)
        retVal = 1; /*to weak */
      else if(magStatus & 0x08)
        retVal = 3; /*to strong */
  }
  
  return retVal;
}

/**
 *@function get Agc
 *@param none
 *@return value of AGC register
 */
u8 getAgc(void)
{
	return readOneByte(_agc);
}

u8 getMagnitude(void)
{
	return readTwoBytes(_mag_hi, _mag_lo);
}

u8 getBurnCount(void)
{
	return readOneByte(_zmco);
}

int burnAngle(void)
{
  int retVal = 1;
  _zPosition = getStartPosition();
  _mPosition = getEndPosition();
  _maxAngle  = getMaxAngle();
  
  if(detectMagnet() == 1)
  {
    if(getBurnCount() < 3)
    {
      if((_zPosition == 0)&&(_mPosition ==0))
        retVal = -3;
      else
        writeOneByte(_burn, 0x80);
    }
    else
      retVal = -2;
  } 
  else
    retVal = -1;
    
  return retVal;
}

int burnMaxAngleAndConfig(void)
{
  int retVal = 1;
  _maxAngle  = getMaxAngle();
  
  if(getBurnCount() ==0)
  {
    if(_maxAngle*0.087 < 18)
      retVal = -2;
    else
      writeOneByte(_burn, 0x40);    
  }  
  else
    retVal = -1;
    
  return retVal;	
}

u8 readOneByte(u8 read_adr)
{
	u8 RetrunVal = 0;
	
	IIC_Start();
	IIC_Send_Byte(_ams5600_Address<<1|SlaveReceiverMode);	/* set as5600 as slave receiver mode */
	IIC_Wait_Ack();
	IIC_Send_Byte(read_adr);	/* send the register LOW address */
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(_ams5600_Address<<1|SlaveTransmitterMode);	/* set as5600 as slave transmitter mode */
	IIC_Wait_Ack();
	
	RetrunVal = IIC_Read_Byte(1);
	
	return RetrunVal;
}

u16 readTwoBytes(u8 read_adr_hi, u8 read_adr_lo)
{
	u16 RetrunVal = 0;
	u16 DataBuf[3];
	
	for(int i=0;i<8;i++)
	{
    IIC_SCL=0; 
		IIC_SCL=1;
  }	
		
	IIC_Start();
	IIC_Send_Byte(_ams5600_Address<<1|SlaveReceiverMode);	/* set as5600 as slave receiver mode */
	IIC_Wait_Ack();
	IIC_Send_Byte(read_adr_lo);	/* send the register LOW address */
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(_ams5600_Address<<1|SlaveTransmitterMode);	/* set as5600 as slave transmitter mode */
	IIC_Wait_Ack();
	
	for(int i=0;i<2;++i)
	{
		DataBuf[i] = IIC_Read_Byte(1);
	}
	IIC_Stop();
	DataBuf[1] = DataBuf[1] << 8;
	RetrunVal = DataBuf[1]|DataBuf[0];
	
	return RetrunVal;
}

void writeOneByte(u8 adr_in, u8 dat_in)
{
	IIC_Start();
	IIC_Send_Byte(_ams5600_Address<<1|SlaveReceiverMode);	/* set as5600 as slave receiver mode */
	IIC_Wait_Ack();
	IIC_Send_Byte(adr_in);	/* send the register address */
	IIC_Wait_Ack();
	IIC_Send_Byte(dat_in);	/* send date */	
	IIC_Wait_Ack();
	IIC_Stop();
}

float convertRawAngleToDegrees(u16 newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  float retVal = newAngle * 0.087890625;
//	printf("RawRad: %d RawDegress: %f \r\n", newAngle, retVal);
  return retVal;
}

float convertScaledAngleToDegrees(u16 newAngle)
{
  u16 startPos = getStartPosition();
  u16 endPos = getEndPosition();
  u16 maxAngle = getMaxAngle();

  float multipler = 0;

  /* max angle and end position are mutually exclusive*/
  if(maxAngle >0)
  {
    if(startPos == 0)
      multipler = (maxAngle*0.087890625)/4096;
    else  /*startPos is set to something*/
      multipler = ((maxAngle*0.087890625)-(startPos * 0.087890625))/4096;
  }
  else
  {
    if((startPos == 0) && (endPos == 0))
      multipler = 0.087890625;
    else if ((startPos > 0 ) && (endPos == 0))
      multipler = ((360 * 0.087890625) - (startPos * 0.087890625)) / 4096;
    else if ((startPos == 0 ) && (endPos > 0))
      multipler = (endPos*0.087890625) / 4096;
    else if ((startPos > 0 ) && (endPos > 0))
      multipler = ((endPos*0.087890625)-(startPos * 0.087890625))/ 4096;
  }
  return (newAngle * multipler);
}

float getRawDegrees()
{
	return convertRawAngleToDegrees(getRawAngle());
}

float getScaledDegrees()
{
	return convertScaledAngleToDegrees(getScaledAngle());
}

/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/SLAM_mobile_platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	麦克纳姆轮移动平台运动学逆解
 */
#ifndef _INVERSE_SOLUTION_H_
#define _INVERSE_SOLUTION_H_

#define lenghtDistance 0.0824
#define widthDistance 0.0665 /* unit meter */

typedef struct
{
	float Vx;
	float Vy;
	float W;
}car_speed;

extern car_speed C_S;

void speed_inverse_solution(void);

#endif

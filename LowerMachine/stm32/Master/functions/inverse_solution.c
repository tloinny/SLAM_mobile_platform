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
#include "inverse_solution.h"

float wheel_speed[4];
car_speed C_S;

void speed_inverse_solution()
{
	wheel_speed[0] = C_S.Vx - C_S.Vy - C_S.W *(lenghtDistance+widthDistance);
	wheel_speed[1] = C_S.Vx + C_S.Vy + C_S.W *(lenghtDistance+widthDistance);
	wheel_speed[2] = C_S.Vx - C_S.Vy + C_S.W *(lenghtDistance+widthDistance);
	wheel_speed[3] = C_S.Vx + C_S.Vy - C_S.W *(lenghtDistance+widthDistance);
}

#ifndef _MOTOR_GPIO_H_
#define _MOTOR_GPIO_H_

#include "stm32f10x_gpio.h"
#include "time.h"
#include "as5600.h"

#define IN1 PBout(6)
#define IN2 PBout(7)

typedef struct motor
{
	GPIO_TypeDef* motor_gpio_type[2];
	GPIO_InitTypeDef motor_gpio[2];
}motor;
extern motor Motor;

void motor_io_init(void);

#endif

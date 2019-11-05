#ifndef _MOTOR_GPIO_H_
#define _MOTOR_GPIO_H_

#include "stm32f10x_gpio.h"
#include "time.h"
#include "as5600.h"

#define IN1 PBout(6)
#define IN2 PBout(7)

#define INA PBout(6)
#define INB PBout(5)

typedef struct
{
	GPIO_TypeDef* motor_gpio_type[3];
	GPIO_InitTypeDef motor_gpio[3];
}motor;

extern motor Motor;

void A4950_io_init(void);
void VNH_io_init(void);

#endif

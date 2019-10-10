#ifndef _PID_H_
#define _PID_H_
#include "sys_conf.h"

void PID_CONTROLLER(float Kp, float Ki, float Kd, float s_t);
void setPID(float kp, float ki, float kd);
void setSampleTime(float sample_time);
void setGoal(float goal);
float getKp(void);
float getKi(void);
float getKd(void);
float getWindup(void);
void clear(void);
float update(float FeedbackValue, int index);

#endif

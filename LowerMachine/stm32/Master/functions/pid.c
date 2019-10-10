#include "pid.h"

float kp = 0;
float ki = 0;
float kd = 0;
float Goal = 0;
float Output = 0;
float PTerm = 0;
float ITerm = 0;
float DTerm = 0;
float LastError[4];
float LLastError[4];
float SampleTime = 0;

int PWM[4];
	
/**
 *@function
 *@param
 *@return
 */
void PID_CONTROLLER(float Kp, float Ki, float Kd, float s_t)
{
	setPID(Kp, Ki, Kd);
	setSampleTime(s_t);
	clear();
	PID_TIM2_Init(s_t);
}

/**
 *@function
 *@param
 *@return
 */
void setPID(float p, float i, float d)
{
	kp = p;
	ki = i;
	kd = d;
}

/**
 *@function
 *@param
 *@return
 */
void setSampleTime(float sample_time)
{
	SampleTime = sample_time;
}

/**
 *@function
 *@param
 *@return
 */
void setGoal(float goal)
{
	if(goal > 0)
		Goal = goal;
	if(goal < 0)
		Goal = -1 * goal;
}

/**
 *@function
 *@param
 *@return
 */
float getKp()
{
	return kp;
}

/**
 *@function
 *@param
 *@return
 */
float getKi()
{
	return ki;
}

/**
 *@function
 *@param
 *@return
 */
float getKd()
{
	return kd;
}

/**
 *@function
 *@param
 *@return
 */
void clear()
{
	Goal = 0;
	Output = 0;
	PTerm = 0;
	ITerm = 0;
	DTerm = 0;
	LastError = 0;
}

/**
 *@function
 *@param
 *@return
 */
float update(float FeedbackValue, int index)
{
	float Error = Goal - FeedbackValue;
	float delta_error = Error - LastError[index];	

	PTerm = kp * delta_error;
	ITerm += Error;
	DTerm = Error - 2*LastError[index] + LLastError[index];
	LLastError[index] = LastError[index];
	LastError[index] = Error;

	Output = PTerm + ki * ITerm + kd * DTerm;
	return Output;
}

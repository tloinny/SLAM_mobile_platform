#include "pid.h"

float kp = 0;
float ki = 0;
float kd = 0;
float Goal = 0;
float Output = 0;
float PTerm = 0;
float ITerm = 0;
float DTerm = 0;
float LastError;
float LLastError;
float SampleTime = 0;

int PWM;
	
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
	PID_TIM3_Init(s_t);
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
//	Goal = (goal*68.27)/sample_time_unit;
	Goal = goal;
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
float update(float FeedbackValue)
{
	float Error = FeedbackValue-Goal;
	float delta_error = Error - LastError;	

	PTerm = kp * delta_error;
	ITerm = Error;
	DTerm = Error - 2*LastError + LLastError;
	LLastError = LastError;
	LastError = Error;

	Output = PTerm + ki * ITerm + kd * DTerm;
	Output *= -1;
	printf("A:%f, p:%f, Goal:%f, ",FeedbackValue,PWM_output,wheel_speed_goal);
	printf("E:%f, Out:%f\r\n",Error,Output);
	return Output;
}

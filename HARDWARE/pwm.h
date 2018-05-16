#ifndef __PWM_H
#define	__PWM_H

#include "sys.h"

//PWM SET LOW LEVEL
#define SET_PWM1(value)             TIM_SetCompare2(TIM3, value)
#define SET_PWM2(value)             TIM_SetCompare1(TIM3, value)
#define SET_PWM3(value)             TIM_SetCompare1(TIM2, value)
#define SET_PWM4(value)             TIM_SetCompare4(TIM3, value)
#define SET_PWM5(value)             TIM_SetCompare3(TIM3, value)
#define SET_PWM6(value)             TIM_SetCompare4(TIM2, value)
#define SET_PWM7(value)             TIM_SetCompare3(TIM2, value)
#define SET_PWM8(value)             TIM_SetCompare2(TIM2, value)

//PWM GET LOW LEVEL
#define GET_PWM1                    TIM_GetCapture2(TIM3)
#define GET_PWM2                    TIM_GetCapture1(TIM3)
#define GET_PWM3                    TIM_GetCapture1(TIM2)
#define GET_PWM4                    TIM_GetCapture4(TIM3)
#define GET_PWM5                    TIM_GetCapture3(TIM3)
#define GET_PWM6                    TIM_GetCapture4(TIM2)
#define GET_PWM7                    TIM_GetCapture3(TIM2)
#define GET_PWM8                    TIM_GetCapture2(TIM2)

//PWM DEFINE FOR NUM
#define SET_Pitch_cnyh(value)       SET_PWM1(value)
#define SET_Yaw_cnyh(value)         SET_PWM2(value)
#define SET_GimbalSpeed_cnyh(value) SET_PWM3(value)
#define SET_Jiaoju_cnyh(value)      SET_PWM4(value)
#define SET_FocusColor_cnyh(value)  SET_PWM5(value)
#define SET_VedioRecPic_cnyh(value) SET_PWM6(value)
#define SET_MultiTrace_cnyh(value)  SET_PWM7(value)
#define _PWM_SET(NUM, value)        SET_PWM##NUM(value)
#define _PWM_GET(NUM)               GET_PWM##NUM
#define PWM_SET(CH, value)          SET_##CH(value)

void TIM_Init(void);
void pwm_init(void);
#endif


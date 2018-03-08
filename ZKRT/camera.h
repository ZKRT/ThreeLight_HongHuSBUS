#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _TIM_SBUS		       14
#define _TIM_CMD_INTERNEL  1000
#define _TIM_RETURN        500

//PWM1_T3C2����ӦPB5
//PWM2_T3C1����ӦPB4
//PWM3_T2C1����ӦPA15
//PWM4_T3C4����ӦPB1
//PWM5_T3C3����ӦPB0
//PWM6_T2C4����ӦPA3
//PWM7_T2C3����ӦPA2
//PWM8_T2C2����ӦPA1

//#define PWM_HANGXAING(value)      TIM_SetCompare2(TIM3, value);
//#define PWM_MODE(value) 		      TIM_SetCompare1(TIM3, value);
//#define PWM_FUYANG(value)         TIM_SetCompare1(TIM2, value);
//#define PWM_JIAOJU(value)         TIM_SetCompare4(TIM3, value);
//#define PWM_PIC(value)            TIM_SetCompare3(TIM2, value);    //����
//#define PWM_REC(value)            TIM_SetCompare2(TIM2, value);    //¼��

//#define GET_JIAOJU                TIM_GetCapture4(TIM3)
//#define GET_PIC                   TIM_GetCapture3(TIM2)
//#define GET_REC                   TIM_GetCapture2(TIM2)

extern volatile uint32_t cmd_count;

void action_modesw(void);
void action_daynightsw(void);
void action_GyroCalibration(void);
void action_Defog(uint8_t state);
void action_ElectricImageStabilizing(uint8_t state);
void action_GyroCalibration_reset_start(void);
void action_reset_prcs(void);
#endif


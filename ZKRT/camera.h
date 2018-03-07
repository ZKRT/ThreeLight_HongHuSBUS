#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _PWM_2000	2000
#define _PWM_1000	1000

#define _TIM_SBUS		       20
#define _TIM_CMD_INTERNEL  1000
#define _TIM_PAIZHAO       _TIM_CMD_INTERNEL
#define _TIM_RETURN        500

//Â¼Ïñ ÅÄÕÕ macro
#define DONGZUO_MODE_REC					0
#define DONGZUO_MODE_PIC					1

//¿ªÊ¼Â¼Ïñ Í£Ö¹Â¼Ïñ macro
#define START_RECORD              0
#define STOP_RECORD               1
#define RECORD_NONE               2

extern volatile uint32_t cmd_count;
extern volatile uint32_t pic_return_count;

void action_hangxiang(void);
void action_fuyang(void);
void action_henggun(void);
void action_paizhao(void);
void action_shexiang(void);
void action_jiaoju(void);
void action_vediosw(void);
void action_ircolorsw(void);
void action_modesw(void);
void action_daynightsw(void);
void action_mirrorflipsw(void);
void action_irvideorec(void);
void action_irjiaoju(void);
void action_reset_prcs(void);
void action_photo_reset_start(void);
void camera_standby(void);
#endif

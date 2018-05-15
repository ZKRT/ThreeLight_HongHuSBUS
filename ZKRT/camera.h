#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _TIM_SBUS           14
#define _TIM_CMD_INTERNEL   1000
#define _TIM_RETURN         500

extern volatile uint32_t cmd_count;

//void action_hangxiang(void);
//void action_fuyang(void);
//void action_henggun(void);
void action_paizhao(void);
void action_shexiang(void);
void action_jiaoju(void);
void action_ircolorsw(void);
void action_focussw(void);
void action_reset_prcs(void);
void action_photo_reset_start(void);
void action_focus_reset_start(void);
#endif

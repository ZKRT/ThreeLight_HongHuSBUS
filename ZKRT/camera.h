#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _PWM_2000	2000
#define _PWM_1000	1000

#define _TIM_SBUS		       20
#define _TIM_CMD_INTERNEL  1000
#define _TIM_PAIZHAO       _TIM_CMD_INTERNEL
#define _TIM_RETURN        500

//PWM1_T3C2，对应PB5
//PWM2_T3C1，对应PB4
//PWM3_T2C1，对应PA15
//PWM4_T3C4，对应PB1
//PWM5_T3C3，对应PB0
//PWM6_T2C4，对应PA3
//PWM7_T2C3，对应PA2
//PWM8_T2C2，对应PA1

#define PWM_HANGXAING(value)      TIM_SetCompare2(TIM3, value);
#define PWM_MODE(value) 		      TIM_SetCompare1(TIM3, value);
#define PWM_FUYANG(value)         TIM_SetCompare1(TIM2, value);
#define PWM_JIAOJU(value)         TIM_SetCompare4(TIM3, value);
#define PWM_PIC(value)            TIM_SetCompare3(TIM2, value);    //拍照
#define PWM_REC(value)            TIM_SetCompare2(TIM2, value);    //录像

#define GET_JIAOJU                TIM_GetCapture4(TIM3)
#define GET_PIC                   TIM_GetCapture3(TIM2)
#define GET_REC                   TIM_GetCapture2(TIM2)

//录像 拍照 macro
#define DONGZUO_MODE_REC					0
#define DONGZUO_MODE_PIC					1

//开始录像 停止录像 macro
#define START_RECORD              0
#define STOP_RECORD               1
#define RECORD_NONE               2

extern volatile uint32_t cmd_count;

extern volatile uint32_t pic_return_count;
extern volatile uint32_t mirrorflip_return_count; 

extern uint16_t last_paizhao;
extern uint16_t last_shexiang;
//extern uint8_t last_jiaoju_re;
//extern uint8_t last_jiaoju_in;
//extern uint8_t last_three_paizhao;
extern uint8_t last_vediosw;
extern uint8_t last_colorsw;
extern uint8_t last_modesw;
extern uint8_t last_daynightsw;
extern uint8_t last_mirrorflip_sw;
extern uint8_t last_irrec_sw;
extern uint8_t last_ir_zoomout;
extern uint8_t last_ir_zoomin;
extern uint16_t last_zoomout;
extern uint8_t last_zoomin;		
extern uint8_t last_GyroCalibration;

extern uint16_t pwm_hangxiang;	//通道1，航向
extern uint16_t pwm_fuyang;			//通道2，俯仰
//extern uint16_t pwm_henggun;		//通道3，横滚
extern uint16_t pwm_paizhao;		//通道5，拍照
extern uint16_t pwm_shexiang;		//通道6，摄像
//extern uint8_t pwm_jiaoju_re;		//通道7，焦距变小
//extern uint8_t pwm_jiaoju_in;		//通道7，焦距变大		
//extern uint16_t pwm_mode;				//通道8，模式切换
extern uint8_t pwm_vedio_sw;		//PWM通道6，模式（视频）切换
extern uint8_t pwm_color_sw;		//PWM通道7，模式（伪彩）切换
extern uint8_t pwm_mode_sw;		//PWM通道5，模式(锁向/跟随)切换
extern uint8_t pwm_daynight_sw;
extern uint8_t	pwm_mirrorflip_sw;
extern uint8_t	pwm_irrec_sw;
extern uint8_t	pwm_ir_zoomout;
extern uint8_t	pwm_ir_zoomin;
extern uint16_t pwm_zoomout;
extern uint8_t pwm_zoomin;		
extern uint8_t pwm_GyroCalibration;
extern uint8_t pwm_Defog;
extern uint8_t pwm_ElectricImageStabilizing;
//extern uint8_t pwm_three_paizhao;//字节28，三连拍
extern uint8_t pwm_init_flag; 	//字节29，初始化标识

uint8_t sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void);
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
void action_GyroCalibration(void);
void action_Defog(void);
void action_ElectricImageStabilizing(void);

#endif


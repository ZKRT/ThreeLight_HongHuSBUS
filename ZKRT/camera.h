#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _PWM_2000	2000
#define _PWM_1000	1000

#define _TIM_SBUS		       20
#define _TIM_CMD_INTERNEL  1000
#define _TIM_PAIZHAO       _TIM_CMD_INTERNEL
#define _TIM_RETURN        500

//PWM1_T3C2����ӦPB5
//PWM2_T3C1����ӦPB4
//PWM3_T2C1����ӦPA15
//PWM4_T3C4����ӦPB1
//PWM5_T3C3����ӦPB0
//PWM6_T2C4����ӦPA3
//PWM7_T2C3����ӦPA2
//PWM8_T2C2����ӦPA1

#define PWM_HANGXAING(value)      TIM_SetCompare2(TIM3, value);
#define PWM_MODE(value) 		      TIM_SetCompare1(TIM3, value);
#define PWM_FUYANG(value)         TIM_SetCompare1(TIM2, value);
#define PWM_JIAOJU(value)         TIM_SetCompare4(TIM3, value);
#define PWM_PIC(value)            TIM_SetCompare3(TIM2, value);    //����
#define PWM_REC(value)            TIM_SetCompare2(TIM2, value);    //¼��

#define GET_JIAOJU                TIM_GetCapture4(TIM3)
#define GET_PIC                   TIM_GetCapture3(TIM2)
#define GET_REC                   TIM_GetCapture2(TIM2)

//¼�� ���� macro
#define DONGZUO_MODE_REC					0
#define DONGZUO_MODE_PIC					1

//��ʼ¼�� ֹͣ¼�� macro
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

extern uint16_t pwm_hangxiang;	//ͨ��1������
extern uint16_t pwm_fuyang;			//ͨ��2������
//extern uint16_t pwm_henggun;		//ͨ��3�����
extern uint16_t pwm_paizhao;		//ͨ��5������
extern uint16_t pwm_shexiang;		//ͨ��6������
//extern uint8_t pwm_jiaoju_re;		//ͨ��7�������С
//extern uint8_t pwm_jiaoju_in;		//ͨ��7��������		
//extern uint16_t pwm_mode;				//ͨ��8��ģʽ�л�
extern uint8_t pwm_vedio_sw;		//PWMͨ��6��ģʽ����Ƶ���л�
extern uint8_t pwm_color_sw;		//PWMͨ��7��ģʽ��α�ʣ��л�
extern uint8_t pwm_mode_sw;		//PWMͨ��5��ģʽ(����/����)�л�
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
//extern uint8_t pwm_three_paizhao;//�ֽ�28��������
extern uint8_t pwm_init_flag; 	//�ֽ�29����ʼ����ʶ

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


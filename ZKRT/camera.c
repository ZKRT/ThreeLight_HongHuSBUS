#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

//命令控制的时间计数
volatile uint32_t cmd_count = 0XFFFFFFFF;
//重新置位的计数
volatile uint32_t pic_return_count = 0XFFFFFFFF;
volatile uint32_t focus_return_count = 0XFFFFFFFF;
//因为拍照和录像共用同个接口,此标记说明
uint8_t paizhao_enabled = 0;
uint8_t focus_enabled = 0;
//////////////////////////////////////////////////////////////////////////////////
//可见光拍照 //控制完成需要回位
void action_paizhao(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = PIC_VSBUS;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//可见光录像
void action_shexiang(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = REC_VSBUS;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//画中画
void action_focussw(void) {
	if (tx_channel_in[FocusColor_cnyh] == 1500) {
		tx_channel_in[FocusColor_cnyh] = 1900;
	} else {
		tx_channel_in[FocusColor_cnyh] = 1500;
	}
}
//颜色切换
void action_ircolorsw(void) {
	if (tx_channel_in[FocusColor_cnyh] == 1500) {
		tx_channel_in[FocusColor_cnyh] = 1100;
	} else {
		tx_channel_in[FocusColor_cnyh] = 1500;
	}
}
//拍照和录像需要调用重置的控制
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //需回位
}
//画中画和颜色需要调用重置的控制
void action_focus_reset_start(void) {
	focus_enabled = 1;
	focus_enabled = TimingDelay;                   //需回位
}
//控制复位操作
void action_reset_prcs(void) {
	if (paizhao_enabled) {
		if (pic_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
			PWM_SET(VedioRecPic_cnyh, tx_channel_in[VedioRecPic_cnyh]);
			pic_return_count = TimingDelay;
			paizhao_enabled = 0;
		}
	}
	if (focus_enabled) {
		if (focus_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[FocusColor_cnyh] = 1500;
			PWM_SET(FocusColor_cnyh, tx_channel_in[FocusColor_cnyh]);
			focus_return_count = TimingDelay;
			focus_enabled = 0;
		}
	}
}

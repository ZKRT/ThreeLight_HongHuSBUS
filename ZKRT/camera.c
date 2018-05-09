#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

//命令控制的时间计数
volatile uint32_t cmd_count = 0XFFFFFFFF;
//重新置位的计数
volatile uint32_t pic_return_count = 0XFFFFFFFF;
//因为拍照和录像共用同个接口,此标记说明
uint8_t paizhao_enabled = 0;
//////////////////////////////////////////////////////////////////////////////////
//可见光拍照 //控制完成需要回位
void action_paizhao(void) {
	tx_channel_in[Pic_cnyh] = 1900;
}
//录像
void action_shexiang(void) {
	if (tx_channel_in[VedioRec_cnyh] == 1100) {
		tx_channel_in[VedioRec_cnyh] = 1900;
	} else {
		tx_channel_in[VedioRec_cnyh] = 1100;
	}
}
//云台模式切
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}

//拍照需要调用重置的控制
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //需回位
}
//控制复位
void action_reset_prcs(void) {
	if (paizhao_enabled) {
		if (pic_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[Pic_cnyh] = 1100;
			pic_return_count = TimingDelay;
			paizhao_enabled = 0;
		}
	}
}

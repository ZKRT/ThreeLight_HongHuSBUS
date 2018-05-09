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
//void action_hangxiang(void)
//{
//	if (pwm_hangxiang > 1500)
//	{
//		tx_channel_in[Yaw_cnyh] = pwm_hangxiang;
//	}
//	else if (pwm_hangxiang < 1500)
//	{
//		tx_channel_in[Yaw_cnyh] = pwm_hangxiang;
//	}
//	else
//	{
//		tx_channel_in[Yaw_cnyh] = 1500;
//	}
//}

//void action_fuyang(void)
//{
//	if (pwm_fuyang > 1500)
//	{
//		tx_channel_in[Pitch_cnyh] = 3000 - pwm_fuyang;
//	}
//	else if (pwm_fuyang < 1500)
//	{
//		tx_channel_in[Pitch_cnyh] = 3000 - pwm_fuyang;
//	}
//	else
//	{
//		tx_channel_in[Pitch_cnyh] = 1500;
//	}
//}
//1900录像，1510无动作，1100拍照
//可见光拍照 //控制完成需要回位
void action_paizhao(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = 1100;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//可见光录像
void action_shexiang(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = 1900;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//红外颜色切换 1510以下黑热，1520以上白热，1515铁红
void action_ircolorsw(void) {
	if (tx_channel_in[IrColorSw_cnyh] == 1100) {
		tx_channel_in[IrColorSw_cnyh] = 1510;
	} else if (tx_channel_in[IrColorSw_cnyh] == 1510) {
		tx_channel_in[IrColorSw_cnyh] = 1600;
	} else if (tx_channel_in[IrColorSw_cnyh] == 1600) {
		tx_channel_in[IrColorSw_cnyh] = 1100;
	} else {
		tx_channel_in[IrColorSw_cnyh] = 1100;
	}
}

//云台模式切换  低自稳，高跟随
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else if (tx_channel_in[ModeSw_cnyh] == 1900) {
		tx_channel_in[ModeSw_cnyh] = 1500;
	} else if (tx_channel_in[ModeSw_cnyh] == 1500) {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}

//可见光日夜切换 低无增益，中高增益，高黑白夜视
void action_daynightsw(void) {
	if (tx_channel_in[DayNightSw_cnyh] == 1100) {
		tx_channel_in[DayNightSw_cnyh] = 1900;
	} else {
		tx_channel_in[DayNightSw_cnyh] = 1100;
	}
}
//镜面反转
//1510 与1100 切换
void action_mirrorflipsw(void) {
	if (tx_channel_in[MirrorFlip_cnyh] == 1500) {
		tx_channel_in[MirrorFlip_cnyh] = 1900;
	} else {
		tx_channel_in[MirrorFlip_cnyh] = 1500;
	}
}
//红外录像 1100不录像，1900录像
void action_irvideorec(void) {
	if (tx_channel_in[IrVedioRecord_cnyh] == 1100) {
		tx_channel_in[IrVedioRecord_cnyh] = 1900;
	} else {
		tx_channel_in[IrVedioRecord_cnyh] = 1100;
	}
}
//拍照需要调用重置的控制
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //需回位
}
//控制复位操作
void action_reset_prcs(void) {
	if (paizhao_enabled) {
		if (pic_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
			pic_return_count = TimingDelay;
			paizhao_enabled = 0;
		}
	}
}
//上电画面切换准备
int camera_standby_flag = 0;
void camera_standby(void) {
	//10秒内将画面切换成正常
	if (camera_standby_flag > 0) //测试结果
		return;
	if (MAVLINK_TX_INIT_VAL - TimingDelay > 10000) {
		if (camera_standby_cnt - TimingDelay > 1000) {
			camera_standby_cnt = TimingDelay;
			action_mirrorflipsw();
			camera_standby_flag++;
		}
	}
}

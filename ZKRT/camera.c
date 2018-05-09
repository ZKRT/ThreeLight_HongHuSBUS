#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

///////////////////////////////////////////////////////////////////////////////
//命令控制的时间计数
volatile uint32_t cmd_count = 0XFFFFFFFF;
//重新置位的计数
volatile uint32_t GyroCalibration_return_count = 0XFFFFFFFF;
uint8_t GyroCalibration_enabled = 0;
///////////////////////////////////////////////////////////////////////////////
//云台模式切换  低自稳，高跟随
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}
//可见光日夜切换 低无增益，中高增益，高黑白夜视
void action_daynightsw(void) {
	if (tx_channel_in[DayNightSw_cnyh] == 1100) {
		tx_channel_in[DayNightSw_cnyh] = 1500;
	} else if (tx_channel_in[DayNightSw_cnyh] == 1500) {
		tx_channel_in[DayNightSw_cnyh] = 1900;
	} else if (tx_channel_in[DayNightSw_cnyh] == 1900) {
		tx_channel_in[DayNightSw_cnyh] = 1100;
	} else {
		tx_channel_in[DayNightSw_cnyh] = 1100;
	}
}
//陀螺仪校准，低档至高档触发
void action_GyroCalibration(void) {
	tx_channel_in[GyroCalibration_cnyh] = 1900;
}
//透雾，低关，高开
void action_Defog(uint8_t state) {
	if (state == 1)
		tx_channel_in[Defog_cnyh] = 1100;
	else
		tx_channel_in[Defog_cnyh] = 1900;
}
//电子稳像，低关，高开
void action_ElectricImageStabilizing(uint8_t state) {
	if (state == 1)
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1100;
	else
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1900;
}
//控制复位操作
void action_reset_prcs(void) {
	if (GyroCalibration_enabled) {
		if (GyroCalibration_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[GyroCalibration_cnyh] = 1100;
			GyroCalibration_return_count = TimingDelay;
			GyroCalibration_enabled = 0;
		}
	}
}
//拍照需要调用重置的控制
void action_GyroCalibration_reset_start(void) {
	GyroCalibration_enabled = 1;
	GyroCalibration_return_count = TimingDelay;                   //需回位
}

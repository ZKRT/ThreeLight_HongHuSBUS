#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

///////////////////////////////////////////////////////////////////////////////
//������Ƶ�ʱ�����
volatile uint32_t cmd_count = 0XFFFFFFFF;
//������λ�ļ���
volatile uint32_t GyroCalibration_return_count = 0XFFFFFFFF;
uint8_t GyroCalibration_enabled = 0;
///////////////////////////////////////////////////////////////////////////////
//��̨ģʽ�л�  �����ȣ��߸���
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}
//�ɼ�����ҹ�л� �������棬�и����棬�ߺڰ�ҹ��
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
//������У׼���͵����ߵ�����
void action_GyroCalibration(void) {
	tx_channel_in[GyroCalibration_cnyh] = 1900;
}
//͸���͹أ��߿�
void action_Defog(uint8_t state) {
	if (state == 1)
		tx_channel_in[Defog_cnyh] = 1100;
	else
		tx_channel_in[Defog_cnyh] = 1900;
}
//�������񣬵͹أ��߿�
void action_ElectricImageStabilizing(uint8_t state) {
	if (state == 1)
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1100;
	else
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1900;
}
//���Ƹ�λ����
void action_reset_prcs(void) {
	if (GyroCalibration_enabled) {
		if (GyroCalibration_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[GyroCalibration_cnyh] = 1100;
			GyroCalibration_return_count = TimingDelay;
			GyroCalibration_enabled = 0;
		}
	}
}
//������Ҫ�������õĿ���
void action_GyroCalibration_reset_start(void) {
	GyroCalibration_enabled = 1;
	GyroCalibration_return_count = TimingDelay;                   //���λ
}

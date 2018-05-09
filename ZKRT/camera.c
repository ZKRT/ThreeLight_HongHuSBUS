#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

//������Ƶ�ʱ�����
volatile uint32_t cmd_count = 0XFFFFFFFF;
//������λ�ļ���
volatile uint32_t pic_return_count = 0XFFFFFFFF;
//��Ϊ���պ�¼����ͬ���ӿ�,�˱��˵��
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
//1900¼��1510�޶�����1100����
//�ɼ������� //���������Ҫ��λ
void action_paizhao(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = 1100;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//�ɼ���¼��
void action_shexiang(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = 1900;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//������ɫ�л� 1510���º��ȣ�1520���ϰ��ȣ�1515����
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

//��̨ģʽ�л�  �����ȣ��߸���
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else if (tx_channel_in[ModeSw_cnyh] == 1900) {
		tx_channel_in[ModeSw_cnyh] = 1500;
	} else if (tx_channel_in[ModeSw_cnyh] == 1500) {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}

//�ɼ�����ҹ�л� �������棬�и����棬�ߺڰ�ҹ��
void action_daynightsw(void) {
	if (tx_channel_in[DayNightSw_cnyh] == 1100) {
		tx_channel_in[DayNightSw_cnyh] = 1900;
	} else {
		tx_channel_in[DayNightSw_cnyh] = 1100;
	}
}
//���淴ת
//1510 ��1100 �л�
void action_mirrorflipsw(void) {
	if (tx_channel_in[MirrorFlip_cnyh] == 1500) {
		tx_channel_in[MirrorFlip_cnyh] = 1900;
	} else {
		tx_channel_in[MirrorFlip_cnyh] = 1500;
	}
}
//����¼�� 1100��¼��1900¼��
void action_irvideorec(void) {
	if (tx_channel_in[IrVedioRecord_cnyh] == 1100) {
		tx_channel_in[IrVedioRecord_cnyh] = 1900;
	} else {
		tx_channel_in[IrVedioRecord_cnyh] = 1100;
	}
}
//������Ҫ�������õĿ���
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //���λ
}
//���Ƹ�λ����
void action_reset_prcs(void) {
	if (paizhao_enabled) {
		if (pic_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
			pic_return_count = TimingDelay;
			paizhao_enabled = 0;
		}
	}
}
//�ϵ续���л�׼��
int camera_standby_flag = 0;
void camera_standby(void) {
	//10���ڽ������л�������
	if (camera_standby_flag > 0) //���Խ��
		return;
	if (MAVLINK_TX_INIT_VAL - TimingDelay > 10000) {
		if (camera_standby_cnt - TimingDelay > 1000) {
			camera_standby_cnt = TimingDelay;
			action_mirrorflipsw();
			camera_standby_flag++;
		}
	}
}

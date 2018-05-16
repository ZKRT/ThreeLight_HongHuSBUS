#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

//������Ƶ�ʱ�����
volatile uint32_t cmd_count = 0XFFFFFFFF;
//������λ�ļ���
volatile uint32_t pic_return_count = 0XFFFFFFFF;
volatile uint32_t focus_return_count = 0XFFFFFFFF;
//��Ϊ���պ�¼����ͬ���ӿ�,�˱��˵��
uint8_t paizhao_enabled = 0;
uint8_t focus_enabled = 0;
//////////////////////////////////////////////////////////////////////////////////
//�ɼ������� //���������Ҫ��λ
void action_paizhao(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = PIC_VSBUS;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//�ɼ���¼��
void action_shexiang(void) {
	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS) {
		tx_channel_in[VedioRecPic_cnyh] = REC_VSBUS;
	} else {
		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
	}
}
//���л�
void action_focussw(void) {
	if (tx_channel_in[FocusColor_cnyh] == 1500) {
		tx_channel_in[FocusColor_cnyh] = 1900;
	} else {
		tx_channel_in[FocusColor_cnyh] = 1500;
	}
}
//��ɫ�л�
void action_ircolorsw(void) {
	if (tx_channel_in[FocusColor_cnyh] == 1500) {
		tx_channel_in[FocusColor_cnyh] = 1100;
	} else {
		tx_channel_in[FocusColor_cnyh] = 1500;
	}
}
//���պ�¼����Ҫ�������õĿ���
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //���λ
}
//���л�����ɫ��Ҫ�������õĿ���
void action_focus_reset_start(void) {
	focus_enabled = 1;
	focus_enabled = TimingDelay;                   //���λ
}
//���Ƹ�λ����
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

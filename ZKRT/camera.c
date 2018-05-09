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
//�ɼ������� //���������Ҫ��λ
void action_paizhao(void) {
	tx_channel_in[Pic_cnyh] = 1900;
}
//¼��
void action_shexiang(void) {
	if (tx_channel_in[VedioRec_cnyh] == 1100) {
		tx_channel_in[VedioRec_cnyh] = 1900;
	} else {
		tx_channel_in[VedioRec_cnyh] = 1100;
	}
}
//��̨ģʽ��
void action_modesw(void) {
	if (tx_channel_in[ModeSw_cnyh] == 1100) {
		tx_channel_in[ModeSw_cnyh] = 1900;
	} else {
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}

//������Ҫ�������õĿ���
void action_photo_reset_start(void) {
	paizhao_enabled = 1;
	pic_return_count = TimingDelay;                   //���λ
}
//���Ƹ�λ
void action_reset_prcs(void) {
	if (paizhao_enabled) {
		if (pic_return_count - TimingDelay > _TIM_RETURN) {
			tx_channel_in[Pic_cnyh] = 1100;
			pic_return_count = TimingDelay;
			paizhao_enabled = 0;
		}
	}
}

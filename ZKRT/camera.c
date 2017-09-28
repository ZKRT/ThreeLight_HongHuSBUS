#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"
#include "sbus.h"

//volatile uint32_t paizhao_count = 0XFFFFFFFF;
//volatile uint32_t shexiang_count = 0XFFFFFFFF;
//volatile uint32_t jiaoju_count = 0XFFFFFFFF;
//volatile uint32_t vediosw_count = 0XFFFFFFFF;
//volatile uint32_t colorsw_count = 0XFFFFFFFF;
//volatile uint32_t modesw_count = 0XFFFFFFFF;

//������Ƶ�ʱ�����
volatile uint32_t cmd_count = 0XFFFFFFFF;

//������λ�ļ���
volatile uint32_t pic_return_count = 0XFFFFFFFF;      
volatile uint32_t mirrorflip_return_count = 0XFFFFFFFF;   

uint16_t last_paizhao = 0;
uint16_t last_shexiang = 0;
//uint8_t last_jiaoju_re = 0;
//uint8_t last_jiaoju_in = 0;
//uint8_t last_three_paizhao = 0;
uint8_t last_vediosw = 0;
uint8_t last_colorsw = 0;
uint8_t last_modesw = 0;
uint8_t last_daynightsw = 0;
uint8_t last_mirrorflip_sw = 0;
uint8_t last_irrec_sw = 0;
uint8_t last_ir_zoomout = 0;
uint8_t last_ir_zoomin = 0;
uint16_t last_zoomout = 0;
uint8_t last_zoomin = 0;		
uint8_t last_GyroCalibration=0;

uint16_t pwm_hangxiang = 0;		//PWMͨ��1������
uint16_t pwm_fuyang = 0;			//PWMͨ��2������
//uint16_t pwm_henggun = 0;			//
uint16_t pwm_paizhao = 0;			//
uint16_t pwm_shexiang = 0;		//
//uint8_t pwm_jiaoju_re = 0;		//PWMͨ��4�������С
//uint8_t pwm_jiaoju_in = 0;		//PWMͨ��4��������		
uint16_t pwm_zoomout = 1500;		//PWMͨ��4�������С
uint8_t pwm_zoomin = 0;		//PWMͨ��4��������		
uint16_t pwm_mode = 0;				//
uint8_t pwm_vedio_sw = 0;		//PWMͨ��6��ģʽ����Ƶ���л�
uint8_t pwm_color_sw = 0;		//PWMͨ��7��ģʽ��α�ʣ��л�
uint8_t pwm_mode_sw = 0;		//PWMͨ��5��ģʽ(����/����)�л�
uint8_t pwm_daynight_sw;
uint8_t	pwm_mirrorflip_sw;
uint8_t	pwm_irrec_sw;
uint8_t	pwm_ir_zoomout;
uint8_t	pwm_ir_zoomin;
uint8_t pwm_GyroCalibration;
uint8_t pwm_Defog=1;
uint8_t pwm_ElectricImageStabilizing=1;
//uint8_t pwm_three_paizhao = 0;//�ֽ�28��������
uint8_t pwm_init_flag = 1; 		//�ֽ�29����ʼ����ʶ��1��δ��ʼ����0����ʼ����ɣ�������������

uint8_t can_value;

//����ģ���У�����ֽڵ��жϽ��յ����ݣ���zkrtָ������Ȼ�������õľ��������˷�����Ӧ���Ƕ�ȡ�ľͶ�ȡ���˷���ֵ
zkrt_packet_t sub_camera_zkrt_packet_can1_rx;

uint8_t sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	volatile uint8_t rx_enabled =0;
	rx_enabled = CAN1_rx_check();
	while (rx_enabled == 1)//check������ͨ��buffer_get��buffer_store�Ա��Ƿ���ȣ�������buffer���Ƿ������ݡ������ݷ���1�������ݷ���0
	{
		can_value = CAN1_rx_byte();//ȡ��λ��buffer_getλ���ϵ�����
		if (zkrt_decode_char(&sub_camera_zkrt_packet_can1_rx,can_value)==1)//ÿ���ַ����ж�һ�£������յ�����������ʱ����1
		{
			pwm_hangxiang = (sub_camera_zkrt_packet_can1_rx.data[1] << 8) | sub_camera_zkrt_packet_can1_rx.data[0];   //������1-0
			pwm_fuyang = (sub_camera_zkrt_packet_can1_rx.data[3] << 8) | sub_camera_zkrt_packet_can1_rx.data[2];  	  //��������3-2
			//4,5,6,7����
			pwm_paizhao = (sub_camera_zkrt_packet_can1_rx.data[9] << 8) | sub_camera_zkrt_packet_can1_rx.data[8];     //���գ���8-9
			pwm_shexiang = (sub_camera_zkrt_packet_can1_rx.data[11] << 8) | sub_camera_zkrt_packet_can1_rx.data[10];  //¼����10-11
//			pwm_zoomout = sub_camera_zkrt_packet_can1_rx.data[12];  																							  //������С����12
//			pwm_zoomin = sub_camera_zkrt_packet_can1_rx.data[13];   																								//����������13
			pwm_zoomout = (sub_camera_zkrt_packet_can1_rx.data[13] << 8) | sub_camera_zkrt_packet_can1_rx.data[12];   //����
			//14,15����
			pwm_vedio_sw = sub_camera_zkrt_packet_can1_rx.data[16];   																								//��Ƶ�л�
			pwm_color_sw = sub_camera_zkrt_packet_can1_rx.data[17];   																								//α���л�
			pwm_mode_sw = sub_camera_zkrt_packet_can1_rx.data[18];   																								  //ģʽ�л�
			pwm_daynight_sw = sub_camera_zkrt_packet_can1_rx.data[19];
			pwm_mirrorflip_sw = sub_camera_zkrt_packet_can1_rx.data[20];
			pwm_irrec_sw = sub_camera_zkrt_packet_can1_rx.data[21];
			pwm_ir_zoomout = sub_camera_zkrt_packet_can1_rx.data[22];
			pwm_ir_zoomin = sub_camera_zkrt_packet_can1_rx.data[23];
			pwm_GyroCalibration = sub_camera_zkrt_packet_can1_rx.data[24];
			pwm_Defog = sub_camera_zkrt_packet_can1_rx.data[25];
			pwm_ElectricImageStabilizing = sub_camera_zkrt_packet_can1_rx.data[26];			
			
			pwm_init_flag = sub_camera_zkrt_packet_can1_rx.data[29];																									//��ʼ����ʶ
			
			return 1;//�����ɹ���
		}
		rx_enabled = CAN1_rx_check();
	}
	
	return 0;//û�����µĽ����ɹ��Ĳ���
}


void action_hangxiang(void)
{
	if (pwm_hangxiang > 1500)  
	{
		tx_channel_in[Yaw_cnyh] = pwm_hangxiang;
	}
	else if (pwm_hangxiang < 1500)
	{
		tx_channel_in[Yaw_cnyh] = pwm_hangxiang;
	}
	else
	{
		tx_channel_in[Yaw_cnyh] = 1500;
	}
}

void action_fuyang(void)
{
	if (pwm_fuyang > 1500)
	{
		tx_channel_in[Pitch_cnyh] = 3000 - pwm_fuyang;
	}
	else if (pwm_fuyang < 1500)
	{
		tx_channel_in[Pitch_cnyh] = 3000 - pwm_fuyang;
	}
	else
	{
		tx_channel_in[Pitch_cnyh] = 1500;
	}
}
void action_henggun(void)						//����û�ж�����������
{
	
}
////1900¼��1510�޶�����1100����
////�ɼ������� //���������Ҫ��λ
//void action_paizhao(void)
//{
//	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS)
//	{
//		tx_channel_in[VedioRecPic_cnyh] = 1100;
//	}
//	else
//	{
//		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
//	}
//}
////�ɼ���¼��
//void action_shexiang(void)
//{
//	if (tx_channel_in[VedioRecPic_cnyh] == CAMERA_NONE_VSBUS)
//	{
//		tx_channel_in[VedioRecPic_cnyh] = 1900;
//	}
//	else
//	{
//		tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
//	}
//}
////�ɼ��⽹��  //1000~2000 ���ཥ��
//void action_jiaoju(void)
//{
//	int jiaoju_value;
//	jiaoju_value = tx_channel_in[Jiaoju_cnyh];
//	
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) 
//	{
//		jiaoju_value = jiaoju_value-100;	//���ڷ�Χ��1000~2000��ÿ�α仯100�����仯10�ξ͵�����
//		if((jiaoju_value) <=1000)
//			jiaoju_value = 1000;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin))
//	{
//		jiaoju_value = jiaoju_value+100;	//���ڷ�Χ��1000~2000��ÿ�α仯100�����仯10�ξ͵�����
//		if((jiaoju_value) >=2000)
//			jiaoju_value = 2000;
//	}
//	tx_channel_in[Jiaoju_cnyh] = jiaoju_value;
//}
////�ɼ��⽹��  //1000~2000 ���ཥ��  ��������Ϊ10
//void action_jiaoju(void)
//{
//	int jiaoju_value;
//	jiaoju_value = tx_channel_in[Jiaoju_cnyh];
//	
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) 
//	{
//		jiaoju_value = jiaoju_value-10;	//���ڷ�Χ��1000~2000��ÿ�α仯10�����仯100�ξ͵�����
//		if((jiaoju_value) <=1000)
//			jiaoju_value = 1000;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin))
//	{
//		jiaoju_value = jiaoju_value+10;	//���ڷ�Χ��1000~2000��ÿ�α仯10�����仯100�ξ͵�����
//		if((jiaoju_value) >=2000)
//			jiaoju_value = 2000;
//	}
//	tx_channel_in[Jiaoju_cnyh] = jiaoju_value;
//}
//const uint16_t zoom_value[8] = {1000, 1100, 1200, 1400, 1600, 1800, 1900, 2000};
//int zoom_item =0;
////�ɼ��⽹��  //1000~2000 ���ཥ��  ��������Ϊ10
//void action_jiaoju(void)
//{
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) //��С
//	{
//		zoom_item = (zoom_item -1) >=0? (zoom_item -1): 0;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin)) //�Ŵ�
//	{
//		zoom_item = (zoom_item +1) <=7? (zoom_item +1): 7;
//	}
//	
//	tx_channel_in[Jiaoju_cnyh] = zoom_value[zoom_item];
//}
void action_jiaoju(void)
{
	tx_channel_in[Jiaoju_cnyh] = pwm_zoomout;
}
////��Ƶ�л�
//void action_vediosw(void)
//{
//	if (tx_channel_in[DayNightSw_cnyh] == 1500)
//	{
//		tx_channel_in[DayNightSw_cnyh] = 1900;
//	}
//	else
//	{
//		tx_channel_in[DayNightSw_cnyh] = 1500;
//	}
//}	
////������ɫ�л� 1510���º��ȣ�1520���ϰ��ȣ�1515����
//void action_ircolorsw(void)
//{
//	if (tx_channel_in[IrColorSw_cnyh] == 1515)
//	{
//		tx_channel_in[IrColorSw_cnyh] = 1100;
//	}
//	else if (tx_channel_in[IrColorSw_cnyh] == 1100)
//	{
//		tx_channel_in[IrColorSw_cnyh] = 1900;
//	}	
//	else if (tx_channel_in[IrColorSw_cnyh] == 1900)
//	{
//		tx_channel_in[IrColorSw_cnyh] = 1515;
//	}
//	else
//	{
//		tx_channel_in[IrColorSw_cnyh] = 1100;
//	}	
//}

//��̨ģʽ�л�  �����ȣ��߸���
void action_modesw(void)
{
	if (tx_channel_in[ModeSw_cnyh] == 1100)
	{
		tx_channel_in[ModeSw_cnyh] = 1900;
	}
	else
	{
		tx_channel_in[ModeSw_cnyh] = 1100;
	}
}

//�ɼ�����ҹ�л� �������棬�и����棬�ߺڰ�ҹ��
void action_daynightsw(void)
{
	if (tx_channel_in[DayNightSw_cnyh] == 1100)
	{
		tx_channel_in[DayNightSw_cnyh] = 1500;
	}
	else if (tx_channel_in[DayNightSw_cnyh] == 1500)
	{
		tx_channel_in[DayNightSw_cnyh] = 1900;
	}	
	else if (tx_channel_in[DayNightSw_cnyh] == 1900)
	{
		tx_channel_in[DayNightSw_cnyh] = 1100;
	}
	else
	{
		tx_channel_in[DayNightSw_cnyh] = 1100;
	}		
}	
////���淴ת
////1510 ��1100 �л�
//void action_mirrorflipsw(void)
//{
//	if (tx_channel_in[MirrorFlip_cnyh] == 1510)
//	{
//		tx_channel_in[MirrorFlip_cnyh] = 1100;
//	}
//	else
//	{
//		tx_channel_in[MirrorFlip_cnyh] = 1510;
//	}
//}
////����¼�� 1100��¼��1900¼��
//void action_irvideorec(void)
//{
//	if (tx_channel_in[IrVedioRecord_cnyh] == 1100)
//	{
//		tx_channel_in[IrVedioRecord_cnyh] = 1900;
//	}
//	else
//	{
//		tx_channel_in[IrVedioRecord_cnyh] = 1100;
//	}
//}	
//���⽹�� 1210-4x, 1300-2x , 1600-0x
//const uint16_t irzoom_value[3] = {IRZOOM_0X_VSBUS, IRZOOM_2X_VSBUS, IRZOOM_4X_VSBUS};
//int irzoom_item =0;
//void action_irjiaoju(void)
//{
//	if ((last_ir_zoomout != pwm_ir_zoomout)&&(last_ir_zoomin == pwm_ir_zoomin)) //��С
//	{
//		irzoom_item = (irzoom_item -1) >=0? (irzoom_item -1): 0;
//	}
//	else if ((last_ir_zoomout == pwm_ir_zoomout)&&(last_ir_zoomin != pwm_ir_zoomin)) //�Ŵ�
//	{
//		irzoom_item = (irzoom_item +1) <=2? (irzoom_item +1): 2;
//	}
//	
//	tx_channel_in[IrJiaojuSw_cnyh] = irzoom_value[irzoom_item];
//}

//������У׼���͵����ߵ�����
void action_GyroCalibration(void)
{
	tx_channel_in[GyroCalibration_cnyh] = 1900;
}
//͸���͹أ��߿�
void action_Defog(void)
{
	if(pwm_Defog==1)
		tx_channel_in[Defog_cnyh] = 1100;
	else
		tx_channel_in[Defog_cnyh] = 1900;
}
//�������񣬵͹أ��߿�
void action_ElectricImageStabilizing(void)
{
	if(pwm_ElectricImageStabilizing==1)
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1100;
	else
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1900;	
}

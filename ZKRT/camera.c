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

//命令控制的时间计数
volatile uint32_t cmd_count = 0XFFFFFFFF;

//重新置位的计数
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

uint16_t pwm_hangxiang = 0;		//PWM通道1，航向
uint16_t pwm_fuyang = 0;			//PWM通道2，俯仰
//uint16_t pwm_henggun = 0;			//
uint16_t pwm_paizhao = 0;			//
uint16_t pwm_shexiang = 0;		//
//uint8_t pwm_jiaoju_re = 0;		//PWM通道4，焦距变小
//uint8_t pwm_jiaoju_in = 0;		//PWM通道4，焦距变大		
uint16_t pwm_zoomout = 1500;		//PWM通道4，焦距变小
uint8_t pwm_zoomin = 0;		//PWM通道4，焦距变大		
uint16_t pwm_mode = 0;				//
uint8_t pwm_vedio_sw = 0;		//PWM通道6，模式（视频）切换
uint8_t pwm_color_sw = 0;		//PWM通道7，模式（伪彩）切换
uint8_t pwm_mode_sw = 0;		//PWM通道5，模式(锁向/跟随)切换
uint8_t pwm_daynight_sw;
uint8_t	pwm_mirrorflip_sw;
uint8_t	pwm_irrec_sw;
uint8_t	pwm_ir_zoomout;
uint8_t	pwm_ir_zoomin;
uint8_t pwm_GyroCalibration;
uint8_t pwm_Defog=1;
uint8_t pwm_ElectricImageStabilizing=1;
//uint8_t pwm_three_paizhao = 0;//字节28，三连拍
uint8_t pwm_init_flag = 1; 		//字节29，初始化标识：1、未初始化；0、初始化完成，可以正常操作

uint8_t can_value;

//在子模块中，逐个字节的判断接收的数据，对zkrt指令解包，然后是设置的就设置完了返回响应，是读取的就读取完了返回值
zkrt_packet_t sub_camera_zkrt_packet_can1_rx;

uint8_t sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	volatile uint8_t rx_enabled =0;
	rx_enabled = CAN1_rx_check();
	while (rx_enabled == 1)//check函数，通过buffer_get和buffer_store对比是否相等，来检验buffer里是否有数据。有数据返回1，无数据返回0
	{
		can_value = CAN1_rx_byte();//取得位于buffer_get位置上的数据
		if (zkrt_decode_char(&sub_camera_zkrt_packet_can1_rx,can_value)==1)//每个字符都判断一下，当接收到完整的数据时返回1
		{
			pwm_hangxiang = (sub_camera_zkrt_packet_can1_rx.data[1] << 8) | sub_camera_zkrt_packet_can1_rx.data[0];   //航向，字1-0
			pwm_fuyang = (sub_camera_zkrt_packet_can1_rx.data[3] << 8) | sub_camera_zkrt_packet_can1_rx.data[2];  	  //俯仰，字3-2
			//4,5,6,7备用
			pwm_paizhao = (sub_camera_zkrt_packet_can1_rx.data[9] << 8) | sub_camera_zkrt_packet_can1_rx.data[8];     //拍照，字8-9
			pwm_shexiang = (sub_camera_zkrt_packet_can1_rx.data[11] << 8) | sub_camera_zkrt_packet_can1_rx.data[10];  //录像，字10-11
//			pwm_zoomout = sub_camera_zkrt_packet_can1_rx.data[12];  																							  //焦距缩小，字12
//			pwm_zoomin = sub_camera_zkrt_packet_can1_rx.data[13];   																								//焦距增大，字13
			pwm_zoomout = (sub_camera_zkrt_packet_can1_rx.data[13] << 8) | sub_camera_zkrt_packet_can1_rx.data[12];   //焦距
			//14,15备用
			pwm_vedio_sw = sub_camera_zkrt_packet_can1_rx.data[16];   																								//视频切换
			pwm_color_sw = sub_camera_zkrt_packet_can1_rx.data[17];   																								//伪彩切换
			pwm_mode_sw = sub_camera_zkrt_packet_can1_rx.data[18];   																								  //模式切换
			pwm_daynight_sw = sub_camera_zkrt_packet_can1_rx.data[19];
			pwm_mirrorflip_sw = sub_camera_zkrt_packet_can1_rx.data[20];
			pwm_irrec_sw = sub_camera_zkrt_packet_can1_rx.data[21];
			pwm_ir_zoomout = sub_camera_zkrt_packet_can1_rx.data[22];
			pwm_ir_zoomin = sub_camera_zkrt_packet_can1_rx.data[23];
			pwm_GyroCalibration = sub_camera_zkrt_packet_can1_rx.data[24];
			pwm_Defog = sub_camera_zkrt_packet_can1_rx.data[25];
			pwm_ElectricImageStabilizing = sub_camera_zkrt_packet_can1_rx.data[26];			
			
			pwm_init_flag = sub_camera_zkrt_packet_can1_rx.data[29];																									//初始化标识
			
			return 1;//解析成功了
		}
		rx_enabled = CAN1_rx_check();
	}
	
	return 0;//没有最新的解析成功的操作
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
void action_henggun(void)						//这里没有定义横滚！！！
{
	
}
////1900录像，1510无动作，1100拍照
////可见光拍照 //控制完成需要回位
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
////可见光录像
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
////可见光焦距  //1000~2000 焦距渐扩
//void action_jiaoju(void)
//{
//	int jiaoju_value;
//	jiaoju_value = tx_channel_in[Jiaoju_cnyh];
//	
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) 
//	{
//		jiaoju_value = jiaoju_value-100;	//周期范围是1000~2000，每次变化100，最多变化10次就到极限
//		if((jiaoju_value) <=1000)
//			jiaoju_value = 1000;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin))
//	{
//		jiaoju_value = jiaoju_value+100;	//周期范围是1000~2000，每次变化100，最多变化10次就到极限
//		if((jiaoju_value) >=2000)
//			jiaoju_value = 2000;
//	}
//	tx_channel_in[Jiaoju_cnyh] = jiaoju_value;
//}
////可见光焦距  //1000~2000 焦距渐扩  调整渐扩为10
//void action_jiaoju(void)
//{
//	int jiaoju_value;
//	jiaoju_value = tx_channel_in[Jiaoju_cnyh];
//	
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) 
//	{
//		jiaoju_value = jiaoju_value-10;	//周期范围是1000~2000，每次变化10，最多变化100次就到极限
//		if((jiaoju_value) <=1000)
//			jiaoju_value = 1000;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin))
//	{
//		jiaoju_value = jiaoju_value+10;	//周期范围是1000~2000，每次变化10，最多变化100次就到极限
//		if((jiaoju_value) >=2000)
//			jiaoju_value = 2000;
//	}
//	tx_channel_in[Jiaoju_cnyh] = jiaoju_value;
//}
//const uint16_t zoom_value[8] = {1000, 1100, 1200, 1400, 1600, 1800, 1900, 2000};
//int zoom_item =0;
////可见光焦距  //1000~2000 焦距渐扩  调整渐扩为10
//void action_jiaoju(void)
//{
//	if ((last_zoomout != pwm_zoomout)&&(last_zoomin == pwm_zoomin)) //缩小
//	{
//		zoom_item = (zoom_item -1) >=0? (zoom_item -1): 0;
//	}
//	else if ((last_zoomout == pwm_zoomout)&&(last_zoomin != pwm_zoomin)) //放大
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
////视频切换
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
////红外颜色切换 1510以下黑热，1520以上白热，1515铁红
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

//云台模式切换  低自稳，高跟随
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

//可见光日夜切换 低无增益，中高增益，高黑白夜视
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
////镜面反转
////1510 与1100 切换
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
////红外录像 1100不录像，1900录像
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
//红外焦距 1210-4x, 1300-2x , 1600-0x
//const uint16_t irzoom_value[3] = {IRZOOM_0X_VSBUS, IRZOOM_2X_VSBUS, IRZOOM_4X_VSBUS};
//int irzoom_item =0;
//void action_irjiaoju(void)
//{
//	if ((last_ir_zoomout != pwm_ir_zoomout)&&(last_ir_zoomin == pwm_ir_zoomin)) //缩小
//	{
//		irzoom_item = (irzoom_item -1) >=0? (irzoom_item -1): 0;
//	}
//	else if ((last_ir_zoomout == pwm_ir_zoomout)&&(last_ir_zoomin != pwm_ir_zoomin)) //放大
//	{
//		irzoom_item = (irzoom_item +1) <=2? (irzoom_item +1): 2;
//	}
//	
//	tx_channel_in[IrJiaojuSw_cnyh] = irzoom_value[irzoom_item];
//}

//陀螺仪校准，低档至高档触发
void action_GyroCalibration(void)
{
	tx_channel_in[GyroCalibration_cnyh] = 1900;
}
//透雾，低关，高开
void action_Defog(void)
{
	if(pwm_Defog==1)
		tx_channel_in[Defog_cnyh] = 1100;
	else
		tx_channel_in[Defog_cnyh] = 1900;
}
//电子稳像，低关，高开
void action_ElectricImageStabilizing(void)
{
	if(pwm_ElectricImageStabilizing==1)
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1100;
	else
		tx_channel_in[ElectricImageStabilizing_cnyh] = 1900;	
}

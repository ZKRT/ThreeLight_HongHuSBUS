#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "can.h"
#include "pwm.h"
#include "camera.h"
#include "sbus.h"
#include "uart.h"

void bsp_init(void)
{
	SystemInit ();		/*系统初始化*/
	RCC_Configuration();
	SysTick_Init();
	LED_Init();
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN初始化环回模式,波特率1Mbps    	
	TIM_Init();
	sbus_init();
}

uint8_t status_camera[8] = {0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0X04, 0X00, 0X00};
uint8_t three_paizhao_flag = 0;//用于记录三连拍的次数
uint8_t paizhao_enabled = 0;

int main()
{
  bsp_init();
	while (1)
	{		
		if ((sub_camera_zkrt_recv_decode_and_zkrt_encode_ack()) == 1)
		{
			if (pwm_init_flag == 0)   	//初始化标记
			{
				action_hangxiang();
				action_fuyang();
				
				if(cmd_count - TimingDelay > _TIM_CMD_INTERNEL)  //在1秒内多次控制无效
				{
					cmd_count = TimingDelay;
					
					if(last_paizhao != pwm_paizhao)
					{
						paizhao_enabled = 1;
						pic_return_count = TimingDelay;                   //需回位
						action_paizhao();
					}
					
					if(last_shexiang != pwm_shexiang)
					{
						action_shexiang();
					}	
					
					if(((last_zoomout != pwm_zoomout)||(last_zoomin != pwm_zoomin)))
					{
						action_jiaoju();
					}
					
					if(last_vediosw != pwm_vedio_sw)
					{
						action_vediosw();
					}
					
					if(last_colorsw != pwm_color_sw)
					{
						action_ircolorsw();
					}
					
					if(last_modesw != pwm_mode_sw)
					{
						action_modesw();
					}
					
					
					if(last_daynightsw != pwm_daynight_sw)
					{
						action_daynightsw();
					}
					
					if(last_mirrorflip_sw != pwm_mirrorflip_sw)
					{
//						mirrorflip_return_count = TimingDelay;
						action_mirrorflipsw();
					}
					
					if(last_irrec_sw != pwm_irrec_sw)
					{
						action_irvideorec();
					}					
					
					if(((last_ir_zoomout != pwm_ir_zoomout)||(last_ir_zoomin != pwm_ir_zoomin)))
					{
						action_irjiaoju();
					}
	
				}	
			}
			
			//不管有没有初始化过，都要在处理完事后记录上一次操作
			last_paizhao = pwm_paizhao;  		  
			last_shexiang = pwm_shexiang;   	 
			last_zoomout = pwm_zoomout;   
			last_zoomin = pwm_zoomin;   
			last_vediosw = pwm_vedio_sw;
			last_colorsw = pwm_color_sw;
			last_modesw = pwm_mode_sw;
			last_daynightsw = pwm_daynight_sw;
			last_mirrorflip_sw = pwm_mirrorflip_sw;
			last_irrec_sw = pwm_irrec_sw;
			last_ir_zoomout = pwm_ir_zoomout;
			last_ir_zoomin = pwm_ir_zoomin;
			
		}
		
		if (_10ms_count - TimingDelay >= 10)								
		{
			_10ms_count = TimingDelay;
			ADC_StartOfConversion(ADC1);											
			
			if ((_10ms_flag%10) == 0)												
			{				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 2000)	
				{
					bat_read();
					bat_check();
				}
			}
			
			if ((_10ms_flag%100) == 0)												
			{
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 3000)	
				{
					status_camera[7]++;
					if (status_camera[7] == 0XFF)
					{
						status_camera[7] = 0;
					}
					Can_Send_Msg(status_camera, 8);							
				}
			}
			_10ms_flag++;
		}
		
		if (led_rx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
		}
		
		if (led_tx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
		
		if(paizhao_enabled)
		{
			if (pic_return_count - TimingDelay > _TIM_RETURN)
			{
				tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;
				pic_return_count = TimingDelay;
				paizhao_enabled = 0;
			}
		}
		
//		if (mirrorflip_return_count - TimingDelay > 3000)
//		{
//			if (tx_channel_in[IrJiaojuSw_cnyh] == IRZOOM_0X_VSBUS)
//			{
//				tx_channel_in[IrJiaojuSw_cnyh] = 1300;
//			}
//			else if (tx_channel_in[IrJiaojuSw_cnyh] == 1300)
//			{
//				tx_channel_in[IrJiaojuSw_cnyh] = 1210;
//			}
//			else if (tx_channel_in[IrJiaojuSw_cnyh] == 1210)
//			{
//				tx_channel_in[IrJiaojuSw_cnyh] = IRZOOM_0X_VSBUS;
//			}			
//			else
//			{
//				
//				tx_channel_in[IrJiaojuSw_cnyh] = IRZOOM_0X_VSBUS;
//			}			
//			mirrorflip_return_count = TimingDelay;
//		}
		
		if (sbus_send_delay - TimingDelay > 14)
		{
			sbus_send();
			sbus_send_delay = TimingDelay;
		}
	}
}

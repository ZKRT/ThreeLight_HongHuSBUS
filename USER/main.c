#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "can.h"
#include "pwm.h"
#include "camera.h"
#include "sbus.h"
#include "uart.h"
#include "key.h"
#include "appcan.h"

void bsp_init(void)
{
	SystemInit();		/*系统初始化*/
	RCC_Configuration();
	SysTick_Init();
	LED_Init();
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN初始化环回模式,波特率1Mbps    	
	TIM_Init();
	sbus_init();
#ifdef KEY_TEST_FUN
	KEY_Init();
#endif
}

int main()
{
	bsp_init();
	appcan_init();
	while (1)
	{	
#ifdef KEY_TEST_FUN
		KEY_Rock();
#endif		
//		camera_standby();
		appcan_prcs();
		action_reset_prcs();
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
					appcan_hbpacket_send();
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
		if (sbus_send_delay - TimingDelay > 14)
		{
			sbus_send();
			sbus_send_delay = TimingDelay;
		}
	}
}

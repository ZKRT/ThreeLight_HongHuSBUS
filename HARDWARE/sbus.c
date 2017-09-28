#include "sbus.h"
#include "uart.h"

//#define EIGHT_CHANNELS 1
#define ALL_CHANNELS   2
zkrt_sbus  tx_sbus;
zkrt_sbus  rx_sbus;

volatile uint32_t sbus_send_delay = 0XFFFFFFFF;

uint16_t tx_channel_in [16]={1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; 
uint16_t tx_channel_out[16]={0};  																																															

uint16_t rx_channel_in [16]={1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; 
uint16_t rx_channel_out[16]={0};  					  																																								 


//������������Ѿ��������pwmֵ���뵽channel_in��
//���ڽ�ͨ��ֵ����tx_sbus��
uint8_t  sbus_send(void)
{
	uint8_t i=0;
	uint32_t temp = 0;
	uint8_t ch_count = 0;
	uint8_t data_count = 0;
	uint8_t rels_count = 0;
	
	memset((void *)(tx_sbus.data), 0, 22);									
	
  for(i=0;i<16;i++)  																					
	{
		tx_channel_out[i]=(tx_channel_in[i]-874)/5*8;							
	}
	
	tx_sbus.startbyte=0x0f;  																	

	//���ݵĳ�ʼ��
	temp = tx_channel_out[0];																		
	ch_count = 1;																								
	data_count = 0;																							

#if defined EIGHT_CHANNELS
	while (ch_count < 9)																				
#elif defined ALL_CHANNELS
	while (ch_count < 17)																				
#endif		
	{
		if ((rels_count = ch_count*11-data_count*8) < 8)					
																														
		{
			temp = temp | (tx_channel_out[ch_count] << rels_count); 
			ch_count++;																							
		}
	
		tx_sbus.data[data_count] = temp&0XFF;                    
		data_count++;																							
		temp >>= 8;																								
	}
		
	tx_sbus.flags=0x00;
	tx_sbus.endbyte=0x00;
	
	uart1_send((uint8_t *)&tx_sbus, sizeof(tx_sbus));
	
	return 0;
}


//��������������յ�һ��������sbusָ����������rx_sbus��
//�����������෴����һ��sbusת��Ϊch���㷨
uint8_t sbus_recv(void)
{
	uint8_t i=0;
	uint32_t temp = 0;
	uint8_t data_count = 0;
	uint8_t ch_count = 0;
	uint8_t rels_count = 0;
	
	memset((void *)(rx_channel_in), 0, 16);											
	
	//���ݵĳ�ʼ��
	temp = rx_sbus.data[0] | (rx_sbus.data[1]<<8);							
	data_count = 2;																							
	ch_count = 0;																								
	
#if defined EIGHT_CHANNELS
	while (data_count < 12)																			
#elif defined ALL_CHANNELS
	while (data_count < 23)																			
#endif		
	{
		while ((rels_count= data_count*8 - ch_count*11) < 11)			
																															
		{
			temp = temp | (rx_sbus.data[data_count] << rels_count); 
			data_count++;																						
		}
		
		
		
		rx_channel_in[ch_count] = temp&0X7FF;                   
		ch_count++;																							
		temp >>= 11;																						
	}
	
	for(i=0;i<16;i++)  																					
	{
		rx_channel_out[i]=(rx_channel_in[i])*5/8+875;							
	}
	
	return 0;
}

void sbus_init(void)
{
	USART1_Config();	
	/*honghu init value*/
//	tx_channel_in[Yaw_cnyh] = 1500;                            //ok
//	tx_channel_in[Pitch_cnyh] = 1500;	                         //ok
//	tx_channel_in[VedioRecPic_cnyh] = CAMERA_NONE_VSBUS;       //ok, 1900¼��1510�޶�����1100����
//	tx_channel_in[ModeSw_cnyh] = 1100;                         //ok, 1510���棬1900��ͷ��1100����
//	tx_channel_in[IrColorSw_cnyh] = 1100;                      //ok, 1510���º��ȣ�1520���ϰ��ȣ�1515����
//	tx_channel_in[MirrorFlip_cnyh] = 1510;                     //ok, 1510 ��1100 �л�
//	tx_channel_in[Jiaoju_cnyh] = 1000;                         //ok, 1000~2000�����󽹾�
//	tx_channel_in[IrVedioRecord_cnyh] = 1100;                  //ok, 1100��¼��1900¼��
//	tx_channel_in[IrJiaojuSw_cnyh] = IRZOOM_0X_VSBUS;          //ok, 1210-4x, 1300-2x , 1600-0x
//	tx_channel_in[DayNightSw_cnyh] = 1100;                     //ok, 1100���죬1900��ҹ
	
	/*yunhan init value*/
	tx_channel_in[Yaw_cnyh] = 1500;                            //ok
	tx_channel_in[Pitch_cnyh] = 1500;	                         //ok
	tx_channel_in[Jiaoju_cnyh] = 1500;                         //ok
	tx_channel_in[DayNightSw_cnyh] = 1100;                     //ok, �������棬�и����棬�ߺڰ�ҹ��
	tx_channel_in[ModeSw_cnyh] = 1100;                         //ok, �����ȣ��߸���
	tx_channel_in[GyroCalibration_cnyh] = 1100;                //1100, �͵����ߵ�����
	tx_channel_in[Defog_cnyh] = 1100;                          //�͹أ��߿�
	tx_channel_in[ElectricImageStabilizing_cnyh] = 1100;	     //�͹أ��߿�
}

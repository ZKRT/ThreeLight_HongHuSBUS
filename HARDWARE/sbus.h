#ifndef __SBUS_H
#define	__SBUS_H

#include "sys.h"


//sbus value
#define PIC_VSBUS           1100
#define REC_VSBUS           1900
#define CAMERA_NONE_VSBUS   1510
#define IRZOOM_0X_VSBUS     1100 //1600 
#define IRZOOM_2X_VSBUS     1200 //1210
#define IRZOOM_4X_VSBUS     1550 //1300

//enum Channel_num_threelight   //for honghu
//{
//	Yaw_cnyh = 0,
//	Pitch_cnyh,
//	None3_cnyh,
//	None4_cnyh,
//	VedioRecPic_cnyh,
//	ModeSw_cnyh,
//	IrColorSw_cnyh,
//	MirrorFlip_cnyh,
//	Jiaoju_cnyh,
//	IrVedioRecord_cnyh,
//	IrJiaojuSw_cnyh,
//	DayNightSw_cnyh
//};

enum Channel_num_threelight { //for yunhan
	Yaw_cnyh = 0,
	Pitch_cnyh,
	None3_cnyh,
	Jiaoju_cnyh,
	DayNightSw_cnyh,
	ModeSw_cnyh,
	GyroCalibration_cnyh,
	None4_cnyh,
	Defog_cnyh,
	ElectricImageStabilizing_cnyh
};

#pragma pack(push, 1)
typedef struct _zkrt_sbus {
	uint8_t   startbyte;
	uint8_t   data[22];
	uint8_t   flags;
	uint8_t   endbyte;
} zkrt_sbus;
#pragma pack(pop)

extern uint16_t tx_channel_in [16];
extern uint16_t tx_channel_out[16];
extern zkrt_sbus tx_sbus;

extern uint16_t rx_channel_in [16];
extern uint16_t rx_channel_out[16];
extern zkrt_sbus rx_sbus;

extern volatile uint32_t sbus_send_delay;

uint8_t sbus_send(void);
uint8_t sbus_recv(void);
void sbus_init(void);

#endif


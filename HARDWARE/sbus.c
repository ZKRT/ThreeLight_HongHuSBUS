#include "sbus.h"
#include "uart.h"

//#define EIGHT_CHANNELS 1
#define ALL_CHANNELS   2
zkrt_sbus  tx_sbus;
zkrt_sbus  rx_sbus;

////////px4
#define SBUS_FRAME_SIZE			25
#define SBUS_BUFFER_SIZE		(SBUS_FRAME_SIZE + SBUS_FRAME_SIZE / 2)

#define SBUS_START_SYMBOL	0x0f

#define SBUS_INPUT_CHANNELS	16
#define SBUS_FLAGS_BYTE		23
#define SBUS_FAILSAFE_BIT	3
#define SBUS_FRAMELOST_BIT	2

// testing with a SBUS->PWM adapter shows that
// above 300Hz SBUS becomes unreliable. 333 would
// be the theoretical achievable, but at 333Hz some
// frames are lost
#define SBUS1_MAX_RATE_HZ	300
#define SBUS1_MIN_RATE_HZ	50

// this is the rate of the old code
#define SBUS1_DEFAULT_RATE_HZ	72

#define SBUS_SINGLE_CHAR_LEN_US		(1/((100000/10)) * 1000 * 1000)

#define SBUS_FRAME_INTERVAL_US	2500
#define SBUS_MIN_CALL_INTERVAL_US	(SBUS_FRAME_GAP_US / 3)
#define SBUS_EPSILON_US	2500

/*
  Measured values with Futaba FX-30/R6108SB:
    -+100% on TX:  PCM 1.100/1.520/1.950ms -> SBus raw values: 350/1024/1700  (100% ATV)
    -+140% on TX:  PCM 0.930/1.520/2.112ms -> SBus raw values:  78/1024/1964  (140% ATV)
    -+152% on TX:  PCM 0.884/1.520/2.160ms -> SBus raw values:   1/1024/2047  (140% ATV plus dirty tricks)
*/

/* define range mapping here, -+100% -> 1000..2000 */
#define SBUS_RANGE_MIN 200.0f
#define SBUS_RANGE_MAX 1800.0f

#define SBUS_TARGET_MIN 1000.0f
#define SBUS_TARGET_MAX 2000.0f

/* pre-calculate the floating point stuff as far as possible at compile time */
#define SBUS_SCALE_FACTOR ((SBUS_TARGET_MAX - SBUS_TARGET_MIN) / (SBUS_RANGE_MAX - SBUS_RANGE_MIN))
#define SBUS_SCALE_OFFSET (int)(SBUS_TARGET_MIN - (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f))

//static unsigned partial_frame_count;
//static unsigned sbus1_frame_delay = (1000U * 1000U) / SBUS1_DEFAULT_RATE_HZ;
///////////////////px4

volatile uint32_t sbus_send_delay = 0XFFFFFFFF;

uint16_t tx_channel_in [16] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
uint16_t tx_channel_out[16] = {0};

uint16_t rx_channel_in [16] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
uint16_t rx_channel_out[16] = {0};

void delaytemp(uint32_t t) {
	uint32_t i = 0;
	while (t--) {
		i = 12000;
		while (i--) {
		}
	}
}
//被调用情况：已经将所需的pwm值输入到channel_in里
//用于将通道值整理到tx_sbus里
uint8_t endbytecounter = 0;
uint8_t tempflag = 0;
uint8_t tempdata[3] = {0x00, 0x00, 0x00};
//uint8_t  sbus_send(void)
//{
//	uint8_t i=0;
//	uint32_t temp = 0;
//	uint8_t ch_count = 0;
//	uint8_t data_count = 0;
//	uint8_t rels_count = 0;
//
//	memset((void *)(tx_sbus.data), 0, 22);

//  for(i=0;i<16;i++)
//	{
//			if(i<4||i>=5)
//			tx_channel_out[i]=(tx_channel_in[i]-874)/5*8;				 //BY Harry
//			else
//			tx_channel_out[i]=(tx_channel_in[i]);
//	}
//
//	tx_sbus.startbyte=0x0f;

//	//数据的初始化
//	temp = tx_channel_out[0];
//	ch_count = 1;
//	data_count = 0;

//#if defined EIGHT_CHANNELS
//	while (ch_count < 9)
//#elif defined ALL_CHANNELS
//	while (ch_count < 17)
//#endif
//	{
//		if ((rels_count = ch_count*11-data_count*8) < 8)
//
//		{
//			temp = temp | (tx_channel_out[ch_count] << rels_count);
//			ch_count++;
//		}
//
//		tx_sbus.data[data_count] = temp&0XFF;
//		data_count++;
//		temp >>= 8;
//	}
//
//	tx_sbus.flags=0x00;
//	tx_sbus.endbyte = 0x00;
//	/*by Harry*/
//	if(endbytecounter==0)
//	{
//		tx_sbus.endbyte=0x04;
//		endbytecounter = 1;
//	}
//	else if(endbytecounter==1)
//	{
//		tx_sbus.endbyte=0x14;
//		endbytecounter = 2;
//	}
//	else if(endbytecounter==2)
//	{
//		tx_sbus.endbyte=0x24;
//		endbytecounter = 3;
//	}
//	else if(endbytecounter==3)
//	{
//		tx_sbus.endbyte=0x34;
//		endbytecounter = 0;
//	}
//	uart1_send((uint8_t *)&tx_sbus, sizeof(tx_sbus));
////	if(tx_sbus.endbyte==0x04)
////	{
////		delaytemp(2);
////		if(tempflag==0)
////		{
////			tempdata[0]=0x03;
////			tempdata[1]=0xC4;
////			tempdata[2]=0x00;
////			tempflag = 1;
////		}
////		else
////		{
////			tempdata[0]=0x03;
////			tempdata[1]=0xC0;
////			tempdata[2]=0x33;
////			tempflag = 0;
////		}
////		uart1_send((uint8_t *)&tempdata, sizeof(tempdata));
////	}
//	/*end buy Harry*/
//	return 0;
//}
void
sbus1_output(int sbus_fd, uint16_t *values, uint16_t num_values) {
	uint8_t byteindex = 1; /*Data starts one byte into the sbus frame. */
	uint8_t offset = 0;
	uint16_t value;
	unsigned int i;

	uint8_t	oframe[SBUS_FRAME_SIZE] = { 0x0f };

	/* 16 is sbus number of servos/channels minus 2 single bit channels.
	* currently ignoring single bit channels.  */

	for (i = 0; (i < num_values) && (i < 16); ++i) {
		value = (uint16_t)(((values[i] - SBUS_SCALE_OFFSET) / SBUS_SCALE_FACTOR) + .5f);

		/*protect from out of bounds values and limit to 11 bits*/
		if (value > 0x07ff) {
			value = 0x07ff;
		}

		while (offset >= 8) {
			++byteindex;
			offset -= 8;
		}

		oframe[byteindex] |= (value << (offset)) & 0xff;
		oframe[byteindex + 1] |= (value >> (8 - offset)) & 0xff;
		oframe[byteindex + 2] |= (value >> (16 - offset)) & 0xff;
		offset += 11;
	}

	uart1_send(oframe, SBUS_FRAME_SIZE);
}
uint8_t  sbus_send(void) {
	sbus1_output(0, tx_channel_in, 16);
	return 0;
}
//被调用情况：接收到一个完整的sbus指令，并将其存在rx_sbus里
//这个与上面的相反，是一种sbus转化为ch的算法
uint8_t sbus_recv(void) {
	uint8_t i = 0;
	uint32_t temp = 0;
	uint8_t data_count = 0;
	uint8_t ch_count = 0;
	uint8_t rels_count = 0;

	memset((void *)(rx_channel_in), 0, 16);

	//数据的初始化
	temp = rx_sbus.data[0] | (rx_sbus.data[1] << 8);
	data_count = 2;
	ch_count = 0;

#if defined EIGHT_CHANNELS
	while (data_count < 12)
#elif defined ALL_CHANNELS
	while (data_count < 23)
#endif
	{
		while ((rels_count = data_count * 8 - ch_count * 11) < 11)

		{
			temp = temp | (rx_sbus.data[data_count] << rels_count);
			data_count++;
		}



		rx_channel_in[ch_count] = temp & 0X7FF;
		ch_count++;
		temp >>= 11;
	}

	for (i = 0; i < 16; i++) {
		rx_channel_out[i] = (rx_channel_in[i]) * 5 / 8 + 875;
	}

	return 0;
}
void sbus_init(void) {
	USART1_Config();
	tx_channel_in[Yaw_cnyh] = 1500;
	tx_channel_in[Pitch_cnyh] = 1500;
	tx_channel_in[Pic_cnyh] = 1100;
	tx_channel_in[VedioRec_cnyh] = 1100;
	tx_channel_in[Jiaoju_cnyh] = 1500;
	tx_channel_in[ModeSw_cnyh] = 1100;
}

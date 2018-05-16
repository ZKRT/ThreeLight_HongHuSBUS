#include "pwm.h"
#include "sbus.h"
//PWM1_T3C2，对应PB5
//PWM2_T3C1，对应PB4
//PWM3_T2C1，对应PA15
//PWM4_T3C4，对应PB1
//PWM5_T3C3，对应PB0
//PWM6_T2C4，对应PA3
//PWM7_T2C3，对应PA2
//PWM8_T2C2，对应PA1
//PA复用为AF2，用于TIM2；PB复用为AF1，用于TIM3
/**
 * @brief  配置TIM3复用输出PWM时用到的I/O
 * @param  无
 * @retval 无
 */
void TIM_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* GPIOB Clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1;//PWM3_T2C1——PWM6_T2C4——PWM7_T2C3——PWM8_T2C2
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_1 | GPIO_Pin_0;//PWM1_T3C2——PWM2_T3C1——PWM4_T3C4——PWM5_T3C3
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 1500;

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

/**
 * [pwm_init description]
 sequence:
 	Pitch_cnyh,
	Yaw_cnyh,
	GimbalSpeed_cnyh,
	Jiaoju_cnyh,
	FocusColor_cnyh,
	VedioRecPic_cnyh,
	MultiTrace_cnyh,
des:
 * 	tx_channel_in[Yaw_cnyh] = 1500;
//- left, + right, = stop
	tx_channel_in[Pitch_cnyh] = 1500;
//- up, + down, = stop
	tx_channel_in[GimbalSpeed_cnyh] = 1500;
//- slow, + fast, = to + (twice) is 回中
	tx_channel_in[MultiTrace_cnyh] = 1000;
//
	tx_channel_in[Jiaoju_cnyh] = 1500;
//-zoom in, + zoom out, = stop
	tx_channel_in[VedioRecPic_cnyh] = 1500;
//= to + is start record, = to - is photo
	tx_channel_in[FocusColor_cnyh] = 1500;
//= to + is 画中画, = to - is color
 */
void pwm_init(void) {
	PWM_SET(Pitch_cnyh, tx_channel_in[Pitch_cnyh]);
	PWM_SET(Yaw_cnyh, tx_channel_in[Yaw_cnyh]);
	PWM_SET(GimbalSpeed_cnyh, tx_channel_in[GimbalSpeed_cnyh]);
	PWM_SET(Jiaoju_cnyh, tx_channel_in[Jiaoju_cnyh]);
	PWM_SET(FocusColor_cnyh, tx_channel_in[FocusColor_cnyh]);
	PWM_SET(VedioRecPic_cnyh, tx_channel_in[VedioRecPic_cnyh]);
	PWM_SET(MultiTrace_cnyh, tx_channel_in[MultiTrace_cnyh]);
	// THROW_PWM1(1500);
	// THROW_PWM2(1500);
	// THROW_PWM3(1500);
	// THROW_PWM4(1500);
	// THROW_PWM5(1500);
	// THROW_PWM6(1500);
	// THROW_PWM7(1000);
	// THROW_PWM8(1500);
}

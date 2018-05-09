#ifndef _SYS_H
#define _SYS_H

#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
#define MAVLINK_TX_INIT_VAL 0XFFFFFFFF

extern volatile uint32_t TimingDelay;				//设置为全局变量，不断的做减法
extern volatile uint32_t led_rx_count;
extern volatile uint32_t led_tx_count;
extern volatile uint32_t _10ms_count;
extern volatile uint32_t _10ms_flag;
extern volatile uint32_t _key_count;
extern volatile uint32_t camera_standby_cnt;

void RCC_Configuration(void);
void SysTick_Init(void);
void delay_us(uint16_t nus);
void delay_ms(uint16_t nms);

#endif

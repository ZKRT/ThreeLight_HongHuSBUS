#include "uart.h"

void USART1_Config(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*开启GPIOA的外设时钟*/
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	/*配置映射外设管脚*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA.10) as input mode */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void uart1_send(uint8_t * dat, uint16_t len) {
	uint16_t i;
	for (i = 0; i < len; i++) {
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, dat[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}

/*********************************************END OF FILE**********************/

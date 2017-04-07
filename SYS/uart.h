#ifndef __UART_H
#define	__UART_H

#include "sys.h"

void USART1_Config(void);
void uart1_send(uint8_t * dat,uint16_t len);

#endif /* __USART1_H */

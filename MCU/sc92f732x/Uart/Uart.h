
#ifndef __UART_H_
#define __UART_H_


#include "modules.h"




void uartSendByte(uint8_t byte);
void uartSendString(uint8_t *s);
void uart0Init(void);


#endif


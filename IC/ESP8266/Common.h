#ifndef __COMMON_H
#define __COMMON_H



#include "stm32f4xx.h"



/******************************* 宏定义 ***************************/



/********************************** 函数声明 ***************************************/
void USART_printf(UART_HandleTypeDef * USARTx, char * Data, ... );



#endif /* __COMMON_H */


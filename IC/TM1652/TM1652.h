#ifndef __TM16252_H_
#define __TM16252_H_


#include "modules.h"



/* 需要在modules.h定义
#define TM1652_LEADING_ZERO TRUE
#define TM1652_WRITE_DATA(d)   uartSendByte(d)  //串口19200波特率
#define TM1652_WRITE_INIT()    uart0Init()
*/

void TM1652_showDigit(uint8_t pos, uint8_t seg);
void TM1652_showDots(uint8_t dots);
void TM1652_showNumber(int16_t num);
void TM1652_Init(void);
void TM1652_Clear(void);
void TM1652_Display(void);


#endif




#ifndef __XC_LCD12864_H_
#define __XC_LCD12864_H_

#include "modules.h"




void SendByte(uint8_t data);
void Write_Data(uint8_t dat);
void Write_Cmd(uint8_t dat);
void Init_ST7920(void);
void LCD_PutString(uint8_t x,uint8_t y,int8_t *s,uint8_t count);
void ClrScreen(void);
void LCD_Dispaly(int8_t *s);



#endif
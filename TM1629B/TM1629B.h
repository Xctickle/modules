#ifndef __TM1629B_H_
#define __TM1629B_H_

#include "SC92F732x_C.h"

 



void TM1629B_Init(void);
void TM1629B_Clear_Data(void);
void TM1629B_W_byte(uint8_t dat);   

void TM1629B_1_Display(void);
void TM1629B_DataConvert(void);




#endif




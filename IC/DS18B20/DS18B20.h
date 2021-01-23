#ifndef __DS18B20_H_
#define __DS18B20_H_


#include "modules.h"


/* 需要在modules.h定义
#define	DQ2_SET() 	 		P01 = 1           
#define	DQ2_CLR()	 		P01 = 0
#define	DQ2_RED()  	 		P01
#define  DQ2_IN()            P0CON &= ~0x02
#define  DQ2_OUT()           P0CON |= 0x02
*/

uint8_t DS18B20_Init(void);
void DS18B20_SingleConvert(void);
float DS18B20_GetSingelTemp(void);

void DS18B20_GetSingelID(uint8_t *_id);
void DS18B20_Convert(uint8_t *_id);
float DS18B20_GetTemp(uint8_t *_id);



#endif




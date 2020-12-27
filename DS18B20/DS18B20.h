#ifndef __DS18B20_H_
#define __DS18B20_H_


#include "SC92F732x_C.h"


#define SEARCH_ROM     0xF0
#define READ_ROM       0x33
#define MATCH_ROM      0x55
#define SKIP_ROM       0xCC
#define ALARM_SEARCH   0xEC

#define CONVERT_T           0x44
#define WRITE_SCRATCHPAD    0x4E
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCR_EXT       0x77
#define READ_SCR_EXT        0xDD
#define COPY_PAGEO          0x48
#define RECALL_E2           0xB8
#define RECALL_PAGE0_RES    0xBB




#define	DQ2_SET() 	 		P01 = 1
#define	DQ2_CLR()	 		P01 = 0
#define	DQ2_RED()  	 		P01


#define DQ2_IN()            P0CON &= ~0x02
#define DQ2_OUT()           P0CON |= 0x02

// #define	DQ_SET() 	 		(GPIO_WriteHigh(GPIOA, GPIO_PIN_4))
// #define	DQ_CLR()	 		   (GPIO_WriteLow(GPIOA, GPIO_PIN_4))
// #define	DQ_RED()  	 		(GPIO_ReadInputPin(GPIOE, GPIO_PIN_5))


float DS18B20_GetSingelTemp(void);
float DS18B20_GetTemp(uint8_t *_id);

// void DS18B20_GetSingelID(void);
void DS18B20_GetSingelID(uint8_t *_id);
void DS18B20_Convert(uint8_t *_id);
void DS18B20_SingleConvert(void);




#endif




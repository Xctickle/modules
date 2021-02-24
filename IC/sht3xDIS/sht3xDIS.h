#ifndef _SHT3XDIS_H_
#define _SHT3XDIS_H_



#include "modules.h"




void sht3xDIS_Init(void);
uint8_t sht3xDIS_RHMeasure(void);
float sht3xDIS_getHumidity(void);
float sht3xDIS_getTemperature(void);





#endif

#ifndef _SHT3XDIS_H_
#define _SHT3XDIS_H_



#include "modules.h"
#include "c_misc.h"
#include "SoftwareI2C.h"



void sht3xDIS_Init(void);
// uint8_t sht3xDIS_RHMeasure(void);
uint8_t sht3xDIS_RHMeasure(int32_t *temperature, int32_t *humidity);

float sht3xDIS_getHumidity(void);
float sht3xDIS_getTemperature(void);





#endif

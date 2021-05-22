#ifndef _SHT3XDIS_H_
#define _SHT3XDIS_H_



#include "modules.h"
#include "SoftwareI2C.h"



void sht3xDIS_Init(void);
// uint8_t sht3xDIS_RHMeasure(void);
uint8_t sht3xDIS_RHMeasure(int32_t *temperature, int32_t *humidity);

// float sht3xDIS_getHumidity(void);
// float sht3xDIS_getTemperature(void);

unsigned char SGPC3_measure(uint16_t *CO2, uint16_t *TVOC);
unsigned char SGPC3_Init(void);

float get_absolute_humidity(int32_t temperature, int32_t humidity);
int16_t sgp30_set_absolute_humidity(float absolute_humidity);


#endif

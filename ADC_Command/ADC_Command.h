#ifndef _ADC_COMMAND_H_
#define _ADC_COMMAND_H_




#include "modules.h"

// #define ADC_SAMP_COUNT 5
// #define ADC_STARTCONVERSION()       ADC1_StartConversion()
// #define ADC_GETCONVERSIONVALUE()    ADC1_GetConversionValue()


typedef struct ADC
{
    uint16_t adc_value[ADC_SAMP_COUNT];
    uint8_t samp_count;
    void (*ADC_Init_handler)(void);
    struct ADC* next;
} ADC;


void adc_loop(void);
void adc_stop(struct ADC* handle);
int adc_start(struct ADC* handle);
void adc_init(struct ADC* handle, void(*ADC_Init_handler)());
uint16_t adc_getValue(struct ADC* handle);



#endif

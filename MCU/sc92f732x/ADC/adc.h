#ifndef _ADC_H_
#define _ADC_H_




#include "modules.h"

#define SAMP_COUNT 5

typedef struct ADC
{
    uint16_t adc_value[SAMP_COUNT];
    uint8_t samp_count;
    void (*ADC_Init_handler)(void);
    struct ADC* next;
} ADC;

// void InitADC(void);
// uint16_t getADCvalue(uint8_t _channel);

void adc_loop(void);
void adc_stop(struct ADC* handle);
int adc_start(struct ADC* handle);
void adc_init(struct ADC* handle, void(*ADC_Init_handler)());
uint16_t adc_getValue(struct ADC* handle);



#endif

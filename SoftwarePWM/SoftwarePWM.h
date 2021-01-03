
#ifndef __SOFTWAREPWM_H
#define __SOFTWAREPWM_H


#include "modules.h"



#define SOFTPWM_TIMEBASE 50        //50us

typedef struct SoftPWM
{
    volatile uint16_t CC1;
    volatile uint16_t CC2;
    volatile uint16_t Timeout;
    volatile uint16_t Ticks;
    volatile uint8_t  Polarity;
    volatile uint8_t  Status;
    void (*timeout_cb)(void);
    struct SoftPWM* Next;
}SoftPWM;

void bsp_InitSoftPWM(SoftPWM* _handle, void(*timeout_cb)(), uint8_t Polarity);
void bsp_SetSoftPWM(SoftPWM* _handle, uint32_t _cycle, uint32_t _duty);
int bsp_SoftPWM_start(SoftPWM* _handle);
void bsp_SoftPWM_stop(SoftPWM* _handle);
void SoftwarePWM_ISR(void);
void bsp_SetTIM1forPWM(void);



#endif

/******************************************************************************************************/


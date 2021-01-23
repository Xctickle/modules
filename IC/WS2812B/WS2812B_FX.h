#ifndef __WS2812B_FX
#define __WS2812B_FX

#include "ws2812b_LLD.h"
#include "stm8s.h"
#include "modules.h"

RGBColor_t Wheel(unsigned char WheelPos);
void rainbowCycle(uint8_t wait);
#endif
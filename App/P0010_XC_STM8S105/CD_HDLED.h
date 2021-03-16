#ifndef _CD_HDLED_H_
#define _CD_HDLED_H_




#include "modules.h"
#include "sysclk.h"
#include "timer.h"
#include "multi_timer.h"
#include "multi_button.h"
#include "delay.h"
#include "PWM.h"
#include "ws2812B_fx.h"


#define	LED_ON()	 	         (GPIO_WriteHigh(GPIOF, GPIO_PIN_4))
#define	LED_OFF()  	             (GPIO_WriteLow(GPIOF, GPIO_PIN_4))
#define	LED_REV()  	             (GPIO_WriteReverse(GPIOF, GPIO_PIN_4))

#define	LED_1_ON()	 	         (GPIO_WriteHigh(GPIOC, GPIO_PIN_2))
#define	LED_1_OFF()  	         (GPIO_WriteLow(GPIOC, GPIO_PIN_2))
#define	LED_1_REV()  	         (GPIO_WriteReverse(GPIOC, GPIO_PIN_2))

#define ANGLE_0         124		//0.5ms
#define ANGLE_45        249		//1ms
#define ANGLE_90        374		//1.5ms
#define ANGLE_135       499		//2ms
#define ANGLE_180       624		//2.5ms

#define PETAL_CLOSE_ANGLE   313
#define PETAL_OPEN_ANGLE    218
#define PETAL_STEP          1

#define LED_CLOSE_BRIGHTNESS   0
#define LED_OPEN_BRIGHTNESS    1000
#define LED_STEP               10

#define WS2812B_CLOSE_BRIGHTNESS    0
#define WS2812B_OPEN_BRIGHTNESS     110
#define WS2812B_STEP               1


typedef enum
{
	SEVER_STOP = 0,	
	SEVER_RUNING,
}SEVER_STATUS_E;

typedef enum
{
	BREATHLED_STOP = 0,	
	BREATHLED_RUNING,
}BREATHLED_STATUS_E;

typedef enum
{
	WS2812B_STOP = 0,	
	WS2812B_RUNING,
}WS2812B_STATUS_E;

typedef struct tagSevers_T
{
    uint16_t targetAngle;
    uint16_t currentAngle;
    uint8_t step;
    uint8_t status;
    void (*setAngle)(uint16_t);
}Severs_T;

typedef struct tagBreathLED_T
{
    uint16_t targetBrightness;
    uint16_t currentBrightness;
    uint8_t step;
    uint8_t status;
    void (*setBrightness)(uint16_t);
}BreathLED_T;




void InitApp(void);


#endif

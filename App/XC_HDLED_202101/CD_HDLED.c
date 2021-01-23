/**
  ******************************************************************************
  * @file    CD_HDLED.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-1-7
  * @Company 
  * @brief   Main program body
   ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/

#include "sysclk.h"
#include "timer.h"
#include "multi_timer.h"
#include "multi_button.h"
#include "delay.h"
#include "PWM.h"
#include "softwarePWM.h"
#include "ws2812B_fx.h"


#define	LED_ON()	 	         (GPIO_WriteHigh(GPIOF, GPIO_PIN_4))
#define	LED_OFF()  	             (GPIO_WriteLow(GPIOF, GPIO_PIN_4))
#define	LED_REV()  	             (GPIO_WriteReverse(GPIOF, GPIO_PIN_4))

#define	LED_1_ON()	 	         (GPIO_WriteHigh(GPIOC, GPIO_PIN_2))
#define	LED_1_OFF()  	         (GPIO_WriteLow(GPIOC, GPIO_PIN_2))
#define	LED_1_REV()  	         (GPIO_WriteReverse(GPIOC, GPIO_PIN_2))

#define OPEN  0
#define CLOSE 1

extern void Delay_us(uint8_t t);
extern void Delay_ms(uint16_t t);

Timer softwatetimer1;
Timer softwatetimer2;

Button btn_run;

SoftPWM PWM1;

#define BREATHELED_CYCLE 115

uint16_t startLED = 10;
uint16_t stopLED = 300;
uint16_t LEDPosition = 10; 



uint16_t startAngle = 200;
uint16_t stopAngle = 300;

uint16_t servoPosition = 200; 
uint8_t petalsStatus = 0;
uint8_t runStatus = CLOSE;

uint8_t btn_run_GPIO(void);
void SoftwarePWN_Config(void);

void PWM1_callback()
{
    // GPIO_WriteReverse(GPIOF, GPIO_PIN_4);
    LED_1_REV();
}

void openPetals()
{
    if (servoPosition > stopAngle)
    {
        petalsStatus = 2;
        runStatus = OPEN;
        return;
    }
    
    servoPosition++;
    SetPWM(servoPosition);

    LEDPosition+=5;
    TIM1_SetCompare2(LEDPosition);
    // bsp_SetSoftPWM(&PWM1,BREATHELED_CYCLE,LEDPosition);
}

void closePetals()
{
    if (servoPosition < startAngle)
    {
        petalsStatus = 2;
        runStatus = CLOSE;
        return;
    }
    
    servoPosition--;
    SetPWM(servoPosition);

    LEDPosition-=5;
    TIM1_SetCompare2(LEDPosition);
    // bsp_SetSoftPWM(&PWM1,BREATHELED_CYCLE,LEDPosition);
}

// void openBreatheLED()
// {
//     if (LEDPosition > stopLED)
//     {
//         runStatus = OPEN;
//         return;
//     }
    
//     servoPosition++;
//     SetPWM(servoPosition);
// }

// void closeBreatheLED()
// {
//     if (servoPosition < startAngle)
//     {
//         petalsStatus = 2;
//         runStatus = CLOSE;
//         return;
//     }
    
//     servoPosition--;
//     SetPWM(servoPosition);
// }

// void openBreatheLED(void)
// {
//     static uint16_t cnt = 0;
//     bsp_SetSoftPWM(&PWM1,BREATHELED_CYCLE,cnt);

// }


void softwatetimer1_callback()
{
    static uint16_t j = 0;
    uint8_t i;

    if (petalsStatus == 0)
    {
        openPetals();
        
    }
    else if (petalsStatus == 1)
    {
        closePetals();
    }


    if (j < 256 * 5)
    {
        j++;
    }
    
    for(i=0; i < NB_LEDS; i++) 
    {
        rgb_SetColor(i,Wheel(((i * 256 / 10) + j) & 255));
        // rgb_SetColor(i,BLUE);
    }
    // setBrightness(250);

    rgb_SendArray();


}

void softwatetimer2_callback()
{
    button_ticks();

}

uint8_t btn_run_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void BTN_RUN_SINGLE_Click_Handler(void* btn)
{
    if (runStatus == OPEN)
    {
        runStatus = CLOSE;
        petalsStatus = 1;
    }
    else
    {
        runStatus = OPEN;
        petalsStatus = 0;
    }
    // LED_REV();
}


void Button_Config(void)
{

    // GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);

    button_init(&btn_run, btn_run_GPIO, 1);

	button_attach(&btn_run, SINGLE_CLICK,     BTN_RUN_SINGLE_Click_Handler);

	button_start(&btn_run);

}

void SoftwarePWN_Config(void)
{    

    bsp_InitSoftPWM(&PWM1,PWM1_callback,0);
    bsp_SetSoftPWM(&PWM1,BREATHELED_CYCLE,startLED);
    bsp_SoftPWM_start(&PWM1);
    SetTIM2forInt(100);
}

void SoftwareTimer_Config(void)
{    
    SetTIM4forInt(1);

    timer_init(&softwatetimer1, softwatetimer1_callback, 20, 20); //1s loop
    timer_start(&softwatetimer1);

    timer_init(&softwatetimer2, softwatetimer2_callback, 5, 5); //5ms delay
    timer_start(&softwatetimer2);

}


static void GPIO_Config(void)
{

    GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);      //LED
    GPIO_Init(GPIOC, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);      //LED1

    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);      //WS2812b

    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);


    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //LED

}


void InitApp(void)
{

    InitSysclk();
    GPIO_Config();

    Button_Config();

    SetTIM1forPWM();

    SoftwareTimer_Config();
    // SoftwarePWN_Config();
    setBrightness(250);


    enableInterrupts();   

}



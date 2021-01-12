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



extern void Delay_us(uint8_t t);
extern void Delay_ms(uint16_t t);

Timer softwatetimer1;
Timer softwatetimer2;

Button btn_run;

void softwatetimer1_callback()
{
    static uint16_t p = 124;
    // static uint8_t i = 0;

    // switch (i)
    // {
    //     case 0:
    //         TIM1_SetCompare1(124);
    //     break;
    //     case 1:
    //         TIM1_SetCompare1(374);
    //     break;
    //     case 2:
    //         TIM1_SetCompare1(624);
    //     break;
    //     case 3:
    //         TIM1_SetCompare1(374);
    //     break;
    //     case 4:
    //         TIM1_SetCompare1(124);
    //         i = 0;
    //     break;
    // }

    // i++;

    if (p > 625)
    {
        p--;
        TIM1_SetCompare1(p);
    }
    else if (p < 125)
    {
        TIM1_SetCompare1(p++);
    }

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

}


void Button_Config(void)
{

    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);

    button_init(&btn_run, btn_run_GPIO, 0);

	button_attach(&btn_run, SINGLE_CLICK,     BTN_RUN_SINGLE_Click_Handler);

	button_start(&btn_run);

}

void SoftwareTimer_Config(void)
{    

    timer_init(&softwatetimer1, softwatetimer1_callback, 100, 100); //1s loop
    timer_start(&softwatetimer1);

    timer_init(&softwatetimer2, softwatetimer2_callback, 20, 20); //5ms delay
    timer_start(&softwatetimer2);

}

void InitApp(void)
{

    InitSysclk();
    SetTIM4forInt(1);
    SetTIM1forPWM();

    SoftwareTimer_Config();

    enableInterrupts();   

}



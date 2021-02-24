/**
  ******************************************************************************
  * @file    CD_GDJJKG.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-02-19
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
#include "delay.h"


#define	RUN_LED_ON()	 	         (GPIO_WriteHigh(GPIOG, GPIO_PIN_1))
#define	RUN_LED_OFF()  	             (GPIO_WriteLow(GPIOG, GPIO_PIN_1))
#define	RUN_LED_REV()  	             (GPIO_WriteReverse(GPIOG, GPIO_PIN_1))



extern void Delay_us(uint8_t t);
extern void Delay_ms(uint16_t t);


Timer softwatetimer1;
Timer softwatetimer2;


/* Private function prototypes -----------------------------------------------*/

static void GPIO_Config(void);


void softwatetimer1_callback()
{

}

void softwatetimer2_callback()
{
    
}


void SoftwareTimer_Config(void)
{    

    timer_init(&softwatetimer1, softwatetimer1_callback, 200, 200); //1s loop
    timer_start(&softwatetimer1);

    timer_init(&softwatetimer2, softwatetimer2_callback, 20, 20); //5ms delay
    timer_start(&softwatetimer2);

}


void main(void)
{


    InitSysclk();
    SetTIM4forInt(1);

    GPIO_Config();

    SoftwareTimer_Config();

    enableInterrupts();   


    while (1)
    {

        timer_loop();

    }
  
}


/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{

    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);      //relay1
    GPIO_Init(GPIOG, GPIO_PIN_0 | GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);      //relay2£¬runled
}
 


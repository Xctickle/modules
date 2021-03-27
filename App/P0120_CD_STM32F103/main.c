/**
  ******************************************************************************
  * @file    main.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-2-23
  * @Company 
  * @brief   Main program body
   ******************************************************************************
  * @attention

  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/


#include "P0120_CD.h"
#include "LCD_ILI9486.h"


extern void InitApp(void);
// extern void timer_loop(void);
extern void read(void);


int main(void)
{
    InitApp();

	

    while (1)
    {
        // timer_loop();
			read();
    }
  
}

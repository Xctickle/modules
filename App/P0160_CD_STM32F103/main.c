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


#include "modules.h"
#include <stdio.h>


int main(void) 
{
    etError  error;       // error code
    uint16_t id;          // sensor ID
    float    temperature; // temperature
    float    humidity;    // relative humidity

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	
    uart_init(115200);	 	//串口初始化为115200


    // initalize sensor module with the i2c address 0x70
    SHTC3_Init();

    // wake up the sensor from sleep mode
    SHTC3_Wakeup();

    // demonstartion of SoftReset command
    error = SHTC3_SoftReset();

    // wait for sensor to reset
    delay_ms(100);

    // demonstartion of GetId command
    error = SHTC3_GetId(&id);

    while(1) 
    {
        // read temperature and relative humidity
        error = SHTC3_GetTempAndHumiPolling(&temperature, &humidity);
        // error = SHTC3_GetTempAndHumi(&temperature, &humidity);


        printf("Temperature: %0.1fdegC,Relative Humidity: %0.1f%%RH.\r\n", temperature, humidity);

        // activate the sleep mode of the sensor to save energy
        // SHTC3_Sleep();
        error = SHTC3_SoftReset();
        // wait 1 second
        delay_ms(300);
        // wake up the sensor from sleep mode
        // SHTC3_Wakeup();
    }

}


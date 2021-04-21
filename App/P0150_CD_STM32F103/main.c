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
#include "sgp40_voc_index.h"
#include "sensirion_voc_algorithm.h"

#include "sgp40.h"

#include <stdio.h>  // printf

/* TO USE CONSOLE OUTPUT (printf) YOU MAY NEED TO ADAPT THE
 * INCLUDES ABOVE OR DEFINE THEM ACCORDING TO YOUR PLATFORM.
 * #define printf(...)
 */

int main(void) 
{
    int16_t err;
    uint16_t sraw;
    uint16_t ix;

    int32_t voc_index;
    int32_t temperature_celsius;
    int32_t relative_humidity_percent;


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	
    uart_init(115200);	 	//串口初始化为115200


	sht3xDIS_Init();

	// while (1)
	// {
	// 	sht3xDIS_RHMeasure();
	// 	delay_ms(100);
	// }
	
	


    // const char* driver_version = sgp40_get_driver_version();
    // if (driver_version) {
    //     printf("SGP40 driver version %s\n", driver_version);
    // } else {
    //     printf("fatal: Getting driver version failed\n");
    //     return -1;
    // }

    /* Initialize I2C bus */
    sensirion_i2c_init();

    /* Busy loop for initialization. The main loop does not work without
     * a sensor. */
    while (sgp40_probe() != STATUS_OK) {
        printf("\r\nSGP sensor probing failed\r\n");
        // sensirion_sleep_usec(10000);
        delay_ms(100);
    }
    printf("\r\nSGP sensor probing successful\r\n");

    uint8_t serial_id[SGP40_SERIAL_ID_NUM_BYTES];
    err = sgp40_get_serial_id(serial_id);
    if (err == STATUS_OK) {
        printf("\r\nSerialID: ");
        for (ix = 0; ix < SGP40_SERIAL_ID_NUM_BYTES - 1; ix++) {
            printf("%02X:", serial_id[ix]);
        }
        printf("%02X\r\n", serial_id[ix]);
    } else {
        printf("\r\nsgp40_get_serial_id failed!\r\n");
    }

    // /* Run periodic measurements at defined intervals */
    // while (1) {
    //     err = sgp40_measure_raw_blocking_read(&sraw);
    //     if (err == STATUS_OK) {
    //         printf("sraw: %u\n", sraw);
    //     } else {
    //         printf("error reading signal\n");
    //     }

    //     // sensirion_sleep_usec(1000000);
    //     delay_ms(1000);

    // }

    /* Run one measurement per second */
    while (1) {
        err = sensirion_measure_voc_index_with_rh_t(
            &voc_index, &relative_humidity_percent, &temperature_celsius);
        if (err == STATUS_OK) {
            printf("\r\nVOC Index: %i\r\n", voc_index);
            printf("\r\nTemperature: %0.3fdegC\r\n", temperature_celsius * 0.001f);
            printf("\r\nRelative Humidity: %0.3f%%RH\r\n",
                   relative_humidity_percent * 0.001f);
        } else {
            printf("\r\nerror reading signal: %d\r\n", err);
        }
        err = sgp40_measure_raw_blocking_read(&sraw);
        if (err == STATUS_OK) {
            printf("\r\nsraw: %u\r\n", sraw);
        } else {
            printf("\r\nerror reading signal\r\n");
        }
        // sensirion_sleep_usec(1000000); /* wait one second */
        delay_ms(100);

    }


}


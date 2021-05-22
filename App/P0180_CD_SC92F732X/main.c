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


int32_t temperature, humidity;
uint16_t CO2, TVOC;
float absolutehumidity;


void GPIO_Config(void)
{    
	P0CON = 0x08;		//p03输出
	P2CON = 0x03;		//p20、p21输出

	P2CON |= 0x00;		//p22、p23输入
	P2PH |= 0x0C;		//p22、p23上拉输入


	SC92F7321_NIO_Init();
}


void main(void)
{
  	GPIO_Config();
    uart0Init();
    sht3xDIS_Init();
    SGPC3_Init();


    while (1)
    {
		SGPC3_measure(&CO2,&TVOC);

        if(sht3xDIS_RHMeasure(&temperature,&humidity))
        {
            absolutehumidity = get_absolute_humidity(temperature,humidity);
            sgp30_set_absolute_humidity(absolutehumidity);
            printf("temperature: %0.2f C, humidity: %0.2f RH.\r\n", temperature / 1000.0f, humidity / 1000.0f);
            printf("CO2: %d ppm, TVOC: %d ppb.\r\n ", CO2 , TVOC);
        }
        else
        {
            printf("error reading measurement\r\n");
        }
        Delay_ms(100);

    }
    
}

// void main(void) 
// {
//     int8_t ret;
//     int32_t err;
    
//   	GPIO_Config();
//     // uart0Init();

//     /* Initialize the i2c bus for the current platform */
//     sensirion_i2c_init();

//     /* Busy loop for initialization, because the main loop does not work without
//      * a sensor.
//      */
//     while (sht3x_probe() != STATUS_OK) 
//     {
//         /* printf("SHT sensor probing failed\n"); */
//     }
//     /* printf("SHT sensor probing successful\n"); */

//     /* Busy loop for initialization. The main loop does not work without
//      * a sensor. */
//     while (sgp30_probe() != STATUS_OK) 
//     {
//         // printf("SGP sensor probing failed\n");
//     }
//     // printf("SGP sensor probing successful\n");

//     /* Consider the two cases (A) and (B):
//      * (A) If no baseline is available or the most recent baseline is more than
//      *     one week old, it must discarded. A new baseline is found with
//      *     sgp30_iaq_init() */
//     err = sgp30_iaq_init();
//     if (err == STATUS_OK) 
//     {
//         // printf("sgp30_iaq_init done\n");
//     } 
//     else 
//     {
//         // printf("sgp30_iaq_init failed!\n");
//     }
//     /* (B) If a recent baseline is available, set it after sgp30_iaq_init() for
//      * faster start-up */
//     /* IMPLEMENT: retrieve iaq_baseline from presistent storage;
//      * err = sgp30_set_iaq_baseline(iaq_baseline);
//      */

//     while (1) 
//     {
//         /* Measure temperature and relative humidity and store into variables
//          * temperature, humidity (each output multiplied by 1000).
//          */
//         ret = sht3x_measure_blocking_read(&temperature, &humidity);
//         if (ret == STATUS_OK) 
//         {
//             // printf("measured temperature: %0.2f degreeCelsius, "
//             //           "measured humidity: %0.2f percentRH\n",
//             //           temperature / 1000.0f,
//             //           humidity / 1000.0f);
//         }
//         else 
//         {
//             // printf("error reading measurement\n");
//         }

//         err = sgp30_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
//         if (err == STATUS_OK) 
//         {
//             // printf("tVOC  Concentration: %dppb\n", tvoc_ppb);
//             // printf("CO2eq Concentration: %dppm\n", co2_eq_ppm);
//         } 
//         else 
//         {
//             // printf("error reading IAQ values\n");
//         }


//         Delay_ms(500);
//     }


// }


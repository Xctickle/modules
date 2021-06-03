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


#define LEVEL_1()   	PWMDTY2 = 3
#define LEVEL_2()   	PWMDTY2 = 11
#define LEVEL_3()   	PWMDTY2 = 17
#define LEVEL_4()   	PWMDTY2 = 23
#define LEVEL_5()   	PWMDTY2 = 29
#define LEVEL_6()   	PWMDTY2 = 35
#define LEVEL_7()   	PWMDTY2 = 40
#define LEVEL_8()   	PWMDTY2 = 46
#define LEVEL_9()   	PWMDTY2 = 52
#define LEVEL_10()  	PWMDTY2 = 58


int32_t temperature, humidity;
uint16_t CO2, TVOC;
float absolutehumidity;


void GPIO_Config(void)
{    
	P0CON = 0xFF;		//p02输出
	P0PH  = 0x00;
	SC92F7321_NIO_Init();
}

void PWM_Init(void)
{
	PWMCON  = 0x3A;		//PWM输出到IO，PWM时钟为Fsys/4
	PWMPRD  = 59;		//PWM周期=(59+1)*(1/6us)=10us	;
	PWMCFG  = 0x00;		//PWM输出不反向,PWM2输出至P02
 	// PWMDTY0 = 30;		//PWM0的Duty = 30/60 =1/2
	// PWMDTY1 = 15;       //PWM0的Duty = 15/60 =1/4
	PWMDTY2 = 4;       //PWM0的Duty = 10/60 =1/6
	PWMDTYA = 0x00;
	PWMCON |= 0x80;     //开启PWM

	// PWMCON  = 0x38;		//PWM输出到IO，PWM时钟为Fsys/4, 
	// // PWMPRD  = 59;		//PWM周期=(59+1)*(1/6us)=10us
	// // PWMCON  = 0x23;		//PWM2输出到IO，PWM时钟为Fsys/8
	// PWMPRD  = 250;		//PWM周期=(59+1)*(1/256us)=10us
	// PWMCFG  = 0x00;		//PWM输出不反向,PWM2输出至P02
 	// // PWMDTY0 = 30;		//PWM0的Duty = 30/60 =1/2
	// // PWMDTY1 = 15;       //PWM0的Duty = 15/60 =1/4
	// PWMDTY2 = 150;       //PWM0的Duty = 10/60 =1/6
	// PWMDTYA = 0x00;
	// PWMCON |= 0x80;     //开启PWM
}

void main(void)
{
  	GPIO_Config();
    uart0Init();
    sht3xDIS_Init();
    SGPC3_Init();

	PWM_Init();

	ENABLE_INT();



    while (1)
    {
		SGPC3_measure(&CO2,&TVOC);

		if (TVOC > 4001)
		{
			LEVEL_10();	
		}
		else if (TVOC > 3501)
		{
			LEVEL_9();	
		}
		else if (TVOC > 3001)
		{
			LEVEL_8();	
		}		
		else if (TVOC > 2501)
		{
			LEVEL_7();	
		}		
		else if (TVOC > 2001)
		{
			LEVEL_6();	
		}
		else if (TVOC > 1501)
		{
			LEVEL_5();	
		}
		else if (TVOC > 1001)
		{
			LEVEL_4();	
		}
		else if (TVOC > 501)
		{
			LEVEL_3();	
		}
		else if (TVOC > 251)
		{
			LEVEL_2();	
		}
		else
		{
			LEVEL_1();	
		}

        if(sht3xDIS_RHMeasure(&temperature,&humidity))
        {
            absolutehumidity = get_absolute_humidity(temperature,humidity);
            sgp30_set_absolute_humidity(absolutehumidity);
            printf("temperature: %0.2f C, humidity: %0.2f RH.\r\n", temperature / 1000.0f, humidity / 1000.0f);
	        printf("CO2: %u ppm, TVOC: %u ppb.\r\n ", CO2 , TVOC);
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


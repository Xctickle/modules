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
uint16_t level,pwm;


void GPIO_Config(void)
{    
	P0CON = 0xFF;		//p02输出
	P0PH  = 0x00;
	SC92F7321_NIO_Init();
}

void PWM_Init(void)
{
	// PWMCON  = 0x3A;		//PWM输出到IO，PWM时钟为Fsys/4
	// PWMPRD  = 59;		//PWM周期=(59+1)*(1/6us)=10us	;
	// PWMCFG  = 0x00;		//PWM输出不反向,PWM2输出至P02
 	// // PWMDTY0 = 30;		//PWM0的Duty = 30/60 =1/2
	// // PWMDTY1 = 15;       //PWM0的Duty = 15/60 =1/4
	// PWMDTY2 = 4;       //PWM0的Duty = 10/60 =1/6
	// PWMDTYA = 0x00;
	// PWMCON |= 0x80;     //开启PWM

	PWMCON  = 0x38;		//PWM输出到IO，PWM时钟为Fsys/1, 
	PWMPRD  = 249;		//PWM周期=(249+1)*(1/24us)=10.41us， 1/10.41=96kHz
	PWMCFG  = 0x00;		//PWM输出不反向,PWM2输出至P02
	PWMDTY2 = 150;       //PWM0的Duty = 10/60 =1/6
	PWMDTYA = 0x00;
	PWMCON |= 0x80;     //开启PWM
}

/**
  * @brief  将采样值的映射到屏幕的显示范围
  * @param  val：待映射的值
  * @param  rangeMax：当前值的最大值
  * @param  rangeMin：当前值的最小值
  * @param  rangeMaxNew：映射最大值
  * @param  rangeMinNew：映射最小值
  * @retval 映射后的值
  */
uint32_t DataRemap(uint32_t val, uint32_t rangeMax, uint32_t rangeMin, uint32_t rangeMaxNew, uint32_t rangeMinNew)
{
	uint32_t result = 0;
    if (val > rangeMax)
        val = rangeMax;
    else if (val < rangeMin)
        val = rangeMin;

    // result = (uint32_t)(val * (uint16_t)(rangeMaxNew - rangeMinNew));
    // result = 300*100;
    // result = 3000*100;
    // result = 30000*100;

    // result = result / (rangeMax - rangeMin);
    // result = rangeMinNew + result;

    result = rangeMinNew + val * (rangeMaxNew - rangeMinNew) / (rangeMax - rangeMin);

    return result;
}

uint8_t Check_level(uint16_t _tvoc)
{
    if (_tvoc < 100)
    {
        return 1;
    }
    else if (_tvoc < 800)
    {
        return _tvoc/50;
    }
    else if (_tvoc < 1000)
    {
        return _tvoc/100+8;
    }   
    else if (_tvoc < 1400)
    {
        return _tvoc/200+13;
    }
    else if (_tvoc < 2400)
    {
        return _tvoc/100+6;
    }
    else if (_tvoc < 16400)
    {
        return _tvoc/200+18;
    }
    else 
    {
        return 99;
    }
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

		// TVOC = 0;
		// level = DataRemap(TVOC,60000,0,100,0);
        level = Check_level(TVOC);
        // level = 5;
		pwm = DataRemap(level,100,0,163,1);
        if (level < 10)
        {
            pwm += 6;
        }
		else if (level < 25)
        {
            pwm += 5;
        }
   		else if (level > 60)
        {
            pwm -= 5;
        }

		PWMDTY2 = pwm;

	    printf("level: %u , PWMDTY2: %u \r\n ", level , pwm);

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

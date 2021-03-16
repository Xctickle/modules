/*
*********************************************************************************************************
*
*	模块名称 : 主程序
*	文件名称 : main.h
*	版    本 : V1.0
*	说    明 : M1820B\DS18B20数字温度传感器演示程序
*			1：单线通信，读取温度
*			2：TM1629B数码管驱动
*			3：按键控制
*			3：

*	修改记录 :
*		版本号   日期       作者    说明
*		v1.0   2020-12-15  xxc  
*
*
*********************************************************************************************************
*/


#include "modules.h"
#include "sc92f732x_c.h"
#include "SoftwareTimer.h"
#include "multi_button.h"
#include "DS18B20.h"
#include "DS18B20_1.h"

                          // 0    1    2     3    4    5    6    7   8    9     -   |     F    NC   E
uint8_t LED1_Table[15]=  { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x02,0x71,0x00,0x79};             


struct Button btn1;
struct Button btn2;


extern uint8_t CheckSoftwareTimer(uint8_t _id);
extern float DS18B20_GetSingelTemp(void);
extern void DS18B20_GetSingelID(uint8_t *_id);

extern float DS18B20_1_GetSingelTemp(void);
extern void DS18B20_1_GetSingelID(uint8_t *_id);
extern float DS18B20_1_GetTemp(uint8_t *_id);

uint8_t btn1_event_val, btn2_event_val;


uint8_t DS18B20_ID[3][8] = 
{
	{0x28, 0xDB, 0x56, 0x70, 0x89, 0xD4, 0x00, 0x00},
	{0x28, 0xE5, 0xF8, 0x70, 0x89, 0xD4, 0x00, 0x00},
	{0x28, 0x53, 0x07, 0x70, 0x89, 0xD4, 0x00, 0x00},
};
uint8_t read_id_index = 0;
uint8_t display_id_index = 0;

float temperature[3];
// float read_tp;

extern uint8_t TM1629B_buff_1[8][2];
extern uint8_t TM1629B_buff_2[8][2];

uint8_t read_button1_GPIO() 
{
	return P22;
}

uint8_t read_button2_GPIO() 
{
	return P23;
}

void getTemperature(void)
{

}

void displayTemperature(uint8_t _id, float _temp)
{
	uint16_t tp;
    uint16_t n5, n4, temp;
    uint8_t n3, n2, n1, i;


	if (_temp < 0)
	{
		tp = _temp * 10;
		tp = ~tp;
		tp += 1; 
		TM1629B_buff_1[1][1] = LED1_Table[10];
	}	
	else
	{
		// tp  = _temp * 10 + 0.5;      //留1个小数点，并四舍五入
		tp  = _temp * 10.0;      //留1个小数点，并四舍五入
	}
	
    n2 = _id / 10;  
    n1 = _id % 10;  

	TM1629B_buff_1[0][0] = LED1_Table[n2];
	TM1629B_buff_1[0][1] = LED1_Table[n1];
	TM1629B_buff_1[1][0] = LED1_Table[10];

    n5 = tp / 10000; 
    temp = tp % 10000; 
    n4 = temp / 1000;    
    temp = temp % 1000;    
    n3 = temp / 100;
    temp = temp % 100; 
    n2 = temp / 10;  
    n1 = temp % 10;  



	TM1629B_buff_1[2][0] = LED1_Table[n4];
	TM1629B_buff_1[2][1] = LED1_Table[n3];
	TM1629B_buff_1[3][0] = LED1_Table[n2];
	TM1629B_buff_1[3][1] = LED1_Table[n1];

	TM1629B_DataConvert();
	TM1629B_1_Display();
}

static void DelayUs1(uint16_t _us)
{
	uint16_t i,a;

	for (i = 0; i < _us; i++)
	{
		a = 3;
		while (--a);
	}
}

void main(void)
{
	uint8_t i;

	P0CON = 0x08;		//p03输出
	P2CON = 0x03;		//p20、p21输出

	P2CON |= 0x00;		//p22、p23输入
	P2PH |= 0x0C;		//p22、p23上拉输入


	SC92F7321_NIO_Init();

	button_init(&btn1, read_button1_GPIO, 0);
	button_init(&btn2, read_button2_GPIO, 0);
	// WDTCON |= 0x10;

	button_start(&btn1);
	button_start(&btn2);
	
	InitSoftwareTimer();

	StartSoftwareTimer(0,500,TMR_AUTO_MODE);
	StartSoftwareTimer(1,5,TMR_AUTO_MODE);



	displayTemperature(12,345.6);
	DelayUs1(10000);
	displayTemperature(1,345.6);
	DelayUs1(10000);

	displayTemperature(2,345.6);
	DelayUs1(10000);

	displayTemperature(3,345.6);
	DelayUs1(10000);

	displayTemperature(4,345.6);



	

	while (1)
	{
		// WDTCON |= 0x10;
	// DelayUs1(10000);

		if (CheckSoftwareTimer(0))
		{

			// temperature = DS18B20_1_GetSingelTemp();
			// DS18B20_1_SingleConvert();

			temperature[display_id_index] = DS18B20_1_GetTemp(&DS18B20_ID[display_id_index][0]);
			// read_tp = DS18B20_1_GetTemp(&DS18B20_ID[display_id_index][0]);
			// if (read_tp > temperature[display_id_index])
			// {
			// 	if (read_tp - temperature[display_id_index] < 5)
			// 	{
			// 		temperature[display_id_index] = read_tp;
			// 	}
			// }
			// else
			// {
			// 	if (temperature[display_id_index] - read_tp < 5)
			// 	{
			// 		temperature[display_id_index] = read_tp;
			// 	}
			// }
			
			
			DS18B20_1_Convert(&DS18B20_ID[display_id_index][0]);

			// for (i = 0; i < read_id_index; i++)
			// {
			// 	temperature[i] = DS18B20_GetTemp(DS18B20_ID[i][0]);
			// 	DS18B20_Convert(DS18B20_ID[i][0]);
			// }
			if (temperature[display_id_index] > -40)
			{
				displayTemperature(display_id_index+1,temperature[display_id_index]);
			}
			
			// displayTemperature(display_id_index,temperature);
			
		}


		if (CheckSoftwareTimer(1))
		{
			button_ticks();
		}
		


		if(btn1_event_val != get_button_event(&btn1)) 
		{
			btn1_event_val = get_button_event(&btn1);
			
			if(btn1_event_val == SINGLE_CLICK) 
			{
				display_id_index++;
				if (display_id_index > 2)
				{
					display_id_index = 0;
				}
			}
		}
		if(btn2_event_val != get_button_event(&btn2)) 
		{
			btn2_event_val = get_button_event(&btn2);
			
			if(btn2_event_val == DOUBLE_CLICK) 
			{
				DS18B20_GetSingelID(&DS18B20_ID[display_id_index][0]);
				// display_id_index++;
				// if (display_id_index > 2)
				// {
				// 	display_id_index = 0;
				// }
			}
		}
		
	}
	
}


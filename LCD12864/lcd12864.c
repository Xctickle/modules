/******************************************************************************
* @file    xc_lcd12864.c
* @author  xxc
* @version V1.0
* @date    2017-xx-xx
* @brief   串行
******************************************************************************
* @attention
*
*
*
*******************************************************************************/ 
#include "lcd12864.h"
#include "delay.h"




#define	CS_HIGH	 	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_3))
#define	CS_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_3))
#define	CS_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_3))

#define	SID_HIGH	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_2))
#define	SID_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_2))
#define	SID_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_2))

#define	CLK_HIGH	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_1))
#define	CLK_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_1))
#define	CLK_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_1))


#define	SET_CS_OUT       (GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST))
#define	SET_SID_OUT      (GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST))
#define	SET_CLK_OUT      (GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_FAST))


extern void Delay_us(uint8_t t);
extern void Delay_ms(uint16_t t);



void SendByte(uint8_t data) 
{
    uint8_t i;
    uint8_t wd;
    wd = data;
    for (i= 0 ; i< 8; i++)         
    {
        if (wd & 0x80)
        {
            SID_HIGH;
        }
        else
        {
            SID_LOW;
        }
        CLK_HIGH;
        Delay_us(2);
        CLK_LOW;
        wd <<= 1;
    }
    // Delay_us(2);    
}

/**
  * @brief  写入数据
  * @param  -dat 数据
  * @retval None
  */
void Write_Data(uint8_t dat)
{
    uint8_t Hdata, Ldata;

    Hdata = dat & 0xf0;         //取高四位
    Ldata = (dat<<4) & 0xf0;    //取低四位

    SendByte(0xfa);
    SendByte(Hdata);
    SendByte(Ldata);
}

/**
  * @brief  写入命令
  * @param  -cmd 命令值
  * @retval None
  */
void Write_Cmd(uint8_t dat)
{
    uint8_t Hdata, Ldata;

    Hdata = dat & 0xf0;         //取高四位
    Ldata = (dat<<4) & 0xf0;    //取低四位

    SendByte(0xf8);
    SendByte(Hdata);
    SendByte(Ldata);
}

/**
  * @brief  初始化-8BIT串口工作模式
  * @param  None
  * @retval None
  */
void Init_ST7920(void)
{  
    SET_CS_OUT;
    SET_SID_OUT;
    SET_CLK_OUT;

    Delay_ms(40);           //大于40MS的延时程序

    // PSB_LOW;               //设置为8BIT串口工作模式

    Delay_ms(10);

	// Write_Cmd(0x30); 	//DL=1:8-BIT interface
    // Delay_us(110);
	// Write_Cmd(0x30); 	//RE=0:basic instruction
    // Delay_us(110);
	// Write_Cmd(0x06); 	//Entire display shift right by 1
    // Delay_us(110);
	// Write_Cmd(0x08); 	//Display OFF,Cursor OFF,Cursor position blink OFF
    // Delay_us(110);
	// Write_Cmd(0x34); 	//DL=1:8-BIT interface
    // Delay_us(110);
	// Write_Cmd(0x34); 	//RE=0:Extend instruction
    // Delay_us(110);
	// Write_Cmd(0x03);
    // Delay_us(110);

    Write_Cmd(0x30);  //8 位介面，基本指令集
    Write_Cmd(0x0c);  //显示打开，光标关，反白关
    Write_Cmd(0x01);  //清屏，将DDRAM的地址计数器归零 

    // Write_Cmd(0x30);     //基本动作指令
    // Write_Cmd(0x03);     //光标归位
    // Write_Cmd(0x0C);     //现实状态
    // Write_Cmd(0x01);     //清屏
    // Write_Cmd(0x06);
}

/**
  * @brief  显示字符串
  * 
  * @param  -x:横坐标值，范围0~7
  *         -y:纵坐标值，范围1~4 
  *         -*s:字符串
  *         -count:字符串数量
  * @retval None
  */
void LCD_PutString(uint8_t x,uint8_t y,int8_t *s,uint8_t count)
{ 
    uint8_t k;
    k = count;

    //以下3行代码必须添加
	Write_Cmd(0x30); 	//DL=1:8-BIT interface
	// Write_Cmd(0x30); 	//RE=0:basic instruction
	// Write_Cmd(0x0C); 	//Display OFF,Cursor OFF,Cursor position blink OFF
    
    switch(y)
    {
        case 1: Write_Cmd(0x80+x);break;
        case 2: Write_Cmd(0x90+x);break;
        case 3: Write_Cmd(0x88+x);break;
        case 4: Write_Cmd(0x98+x);break;
        default:break;
    }

    while(k)
    { 
        Write_Data(*s);
        s++;
        k--;
    }
}

/**
  * @brief  显示字符串
  * 
  * @param  -*s:字符串
  * @retval None
  */
void LCD_Dispaly(int8_t *s)
{ 
    uint8_t k = 64;

	Write_Cmd(0x30); 	//DL=1:8-BIT interface

    Write_Cmd(0x80);


    while(k)
    { 
        Write_Data(*s);
        s++;
        k--;
    }
}

/**
  * @brief  清屏
  * @param  None
  * @retval None
  */
void ClrScreen(void)
{ 
    Write_Cmd(0x01);
    Delay_ms(15);
}
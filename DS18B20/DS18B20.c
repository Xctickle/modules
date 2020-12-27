/******************************************************************************
* @file    DS18B20.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "modules.h"
#include "DS18B20.h"   

extern void DelayUs(uint16_t _us);


static void DS18B20_WriteByte(uint8_t _data);
static uint8_t DS18B20_ReadByte(void);
static uint8_t DS18B20_Init(void);

    // uint8_t DS18B20_ID[8] = {0x00};


// void Delay500us()		//@24.000MHz
// {
// 	unsigned char i, j;

// 	i = 12;
// 	j = 168;
// 	do
// 	{
// 		while (--j);
// 	} while (--i);
// }

// void Delay68us()		//@24.000MHz
// {
// 	unsigned char i, j;

// 	i = 2;
// 	j = 146;
// 	do
// 	{
// 		while (--j);
// 	} while (--i);
// }

// void Delay48us()		//@24.000MHz
// {
// 	unsigned char i, j;

// 	i = 2;
// 	j = 27;
// 	do
// 	{
// 		while (--j);
// 	} while (--i);
// }

// void Delay1us()		//@24.000MHz
// {
// 	unsigned char i;

// 	i = 2;
// 	while (--i);
// }

/**
  * @brief  初始化传感器.
  * @param  None
  * @retval 是否响应：1响应，0没响应
  */
static uint8_t DS18B20_Init(void)
{
    uint8_t i = 0;

    DQ2_OUT();
    DQ2_SET();
    DelayUs(1);     //稍作延时
    DQ2_CLR();
    DelayUs(640);    //延时480到960us
    DQ2_SET();

    DQ2_IN();
    P0PH = 0x00;
    while(DQ2_RED())    //等待DS18B20拉低总线
    {
        DelayUs(640);
        if(++i>5)         //约等待>5MS
        {
            return 0;   //初始化失败
        }    
    }
    DelayUs(500);
    return 1;
}

void DS18B20_SingleConvert(void)
{

    // DQ2_OUT();
    // DQ2_SET();
    // DelayUs(640);       //500
    // DQ2_CLR();
    // DelayUs(87);        //68
    // DQ2_SET();
    // DelayUs(58);        //48
    // DQ2_CLR();
    // DelayUs(1);
    // DQ2_SET();
    // DelayUs(640);       //500
    // DQ2_CLR();


    DS18B20_Init();
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(CONVERT_T);
    DQ2_OUT();
    DQ2_SET();
}

void DS18B20_Convert(uint8_t *_id)
{
    uint8_t i;
    DS18B20_Init();
    DS18B20_WriteByte(MATCH_ROM);
    for(i = 0; i < 8; i++)
    {
        DS18B20_WriteByte(_id);
    }
    DS18B20_WriteByte(CONVERT_T);
    DQ2_SET();
}

/**
  * @brief  读取单个温度点.
  * @param  None
  * @retval None
  */
float DS18B20_GetSingelTemp(void)
{
    uint8_t vl,vh; 
    int16_t temp; 
    float tp;

    // DS18B20_Init();
    // DS18B20_WriteByte(SKIP_ROM);
    // DS18B20_WriteByte(CONVERT_T);
    // DQ2_SET();
    // delay_us(100); //延时1s

    DS18B20_Init();
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(READ_SCRATCHPAD);
    // DelayUs(500);
    vl = DS18B20_ReadByte();
    vh = DS18B20_ReadByte();
    temp = vh;
    temp <<= 8;

    temp = temp | vl;


    tp = (40.0 + (temp/256.0));

    // if(temp < 0)          //考虑负温度的情况
    // {
    //     temp = temp - 1;
    //     temp = ~temp;
    //     tp = temp * 0.0625;         //16位温度转换成10进制的温度
    //     // temp = tp * 100 + 0.5;      //留两个小数点，并四舍五入
    // }
    // else
    // {
    //     tp = temp * 0.0625;         //16位温度转换成10进制的温度
    //     // temp = tp * 100 + 0.5;      //留两个小数点，并四舍五入
    // }
    // tp = temp * 0.0625;         //16位温度转换成10进制的温度

    return tp;
}


/**
  * @brief  读取单个64位ID.
  * @param  None
  * @retval None
  */
// void DS18B20_GetSingelID(void)
void DS18B20_GetSingelID(uint8_t *_id)
{
    uint8_t i;


    DS18B20_Init();
    DS18B20_WriteByte(READ_ROM);
    for(i = 0; i < 8; i++)
    {
        _id[i] = DS18B20_ReadByte();
        // DS18B20_ID[i] = DS18B20_ReadByte();
    }

}

/**
  * @brief  读取指定温度点.
  * @param  None
  * @retval None
  */
float DS18B20_GetTemp(uint8_t *_id)
{
    uint8_t vl,vh,i; 
    uint16_t temp; 
    float tp;

    // DS18B20_Init();
    // DS18B20_WriteByte(MATCH_ROM);
    // for(i = 0; i < 8; i++)
    // {
    //     DS18B20_WriteByte(_id);
    // }
    // DS18B20_WriteByte(CONVERT_T);
    // DQ2_SET();
    // delay_us(100); //延时1s

    DS18B20_Init();
    DS18B20_WriteByte(MATCH_ROM);
    for(i = 0; i < 8; i++)
    {
        DS18B20_WriteByte(_id);
    }
    DS18B20_WriteByte(READ_SCRATCHPAD);
    vl = DS18B20_ReadByte();
    vh = DS18B20_ReadByte();
    temp = vh << 8;
    temp |= vl;

    if(temp < 0)          //考虑负温度的情况
    {
        temp = temp - 1;
        temp = ~temp;
        tp = temp * 0.0625;         //16位温度转换成10进制的温度
        // temp = tp * 100 + 0.5;      //留两个小数点，并四舍五入
    }
    else
    {
        tp = temp * 0.0625;         //16位温度转换成10进制的温度
        // temp = tp * 100 + 0.5;      //留两个小数点，并四舍五入
    }

    return tp;
}

/**
  * @brief  写入一个字节
  * @param  _data
  * @retval None
  */
static void DS18B20_WriteByte(uint8_t _data)
{
	uint8_t i; 
    DQ2_OUT();

    for(i = 0; i < 8; i++)
    {
        DQ2_CLR();            //每写入一位数据之前先把总线拉低1us
        DelayUs(1);
        if (_data & 0x01)    //取最低位写入
        {
            DQ2_SET();
        }
        else
        {
            DQ2_CLR();
        }
        DelayUs(87);       //延时68us,持续时间最少60us
        DQ2_SET();           //然后释放总线
        _data >>= 1;        //从低位开始写
    }

    DelayUs(58);
}

/**
  * @brief  读取一个字节
  * @param  None
  * @retval 一个字节
  */
static uint8_t DS18B20_ReadByte(void)
{
	uint8_t i, dat; 
    dat = 0;

    for(i = 0; i < 8; i++)
    {
        DQ2_OUT();
        DQ2_CLR();           //先将总线拉低1us
        DelayUs(1);
        dat >>= 1;         //从最低位开始读

        DQ2_SET();           //然后释放总线
        // DelayUs(1);
        DQ2_IN();
        if(DQ2_RED()) 
        {
            dat |= 0x80;   //每次读一位
        }

        DelayUs(63);       //读取完之后等待48us再接着读取下一个数
    }

    return dat;
}


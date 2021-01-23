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

#include "DS18B20.h"   
#include "delay.h"   

#define SEARCH_ROM     0xF0
#define READ_ROM       0x33
#define MATCH_ROM      0x55
#define SKIP_ROM       0xCC
#define ALARM_SEARCH   0xEC

#define CONVERT_T           0x44
#define WRITE_SCRATCHPAD    0x4E
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCR_EXT       0x77
#define READ_SCR_EXT        0xDD
#define COPY_PAGEO          0x48
#define RECALL_E2           0xB8
#define RECALL_PAGE0_RES    0xBB


static void DS18B20_WriteByte(uint8_t _data);
static uint8_t DS18B20_ReadByte(void);


extern void delay500us(void);
extern void delay68us(void);
extern void delay48us(void);
extern void delay1us(void);


/**
  * @brief  初始化传感器.
  * @param  None
  * @retval 是否响应：1响应，0没响应
  */
uint8_t DS18B20_Init(void)
{
    uint16_t i = 0;

    DQ2_OUT();
    DQ2_SET();
    delay1us();     //稍作延时
    DQ2_CLR();
    delay500us();    //延时480到960us
    DQ2_SET();

    DQ2_IN();
    while(DQ2_RED())    //等待DS18B20拉低总线
    {
        delay1us();     //间隔时间不宜过长，错过电平识别机会
        if(++i>10) 
        {
            return 0;   //初始化失败
        }    
    }
    delay500us();       //必须加延时
    return 1;
}

/**
  * @brief  单路传感器启动转换.
  * @param  None
  * @retval 是否响应：1响应，0没响应
  */
void DS18B20_SingleConvert(void)
{
    DS18B20_Init();
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(CONVERT_T);
    DQ2_OUT();
    DQ2_SET();
}

/**
  * @brief  指定传感器启动转换.
  * @param  _id：64位ID
  * @retval 是否响应：1响应，0没响应
  */
void DS18B20_Convert(uint8_t *_id)
{
    uint8_t i;
    DS18B20_Init();
    DS18B20_WriteByte(MATCH_ROM);
    for(i = 0; i < 8; i++)
    {
        DS18B20_WriteByte(_id[i]);
    }
    DS18B20_WriteByte(CONVERT_T);
    DQ2_SET();
}

/**
  * @brief  读取单个温度点.
  * @param  None
  * @retval 返回温度，-99为传感器无响应
  */
float DS18B20_GetSingelTemp(void)
{
    uint8_t vl,vh; 
    int16_t temp; 
    float tp;

    if(!DS18B20_Init())
    {
        return -99;
    }
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(READ_SCRATCHPAD);
    vl = DS18B20_ReadByte();
    vh = DS18B20_ReadByte();
    temp = vh;
    temp <<= 8;

    temp = temp | vl;

    tp = (40.0 + (temp/256.0));

    return tp;
}


/**
  * @brief  读取单个64位ID.
  * @param  _id：64位ID数组
  * @retval None
  */
void DS18B20_GetSingelID(uint8_t *_id)
{
    uint8_t i;

    DS18B20_Init();
    DS18B20_WriteByte(READ_ROM);
    for(i = 0; i < 8; i++)
    {
        _id[i] = DS18B20_ReadByte();
    }
}

/**
  * @brief  读取指定温度点.
  * @param  _id：64位ID
  * @retval 温度
  */
float DS18B20_GetTemp(uint8_t *_id)
{
    uint8_t vl,vh,i; 
    uint16_t temp; 
    float tp;

    DS18B20_Init();
    DS18B20_WriteByte(MATCH_ROM);
    for(i = 0; i < 8; i++)
    {
        DS18B20_WriteByte(_id[i]);
    }
    DS18B20_WriteByte(READ_SCRATCHPAD);
    vl = DS18B20_ReadByte();
    vh = DS18B20_ReadByte();
    temp = vh << 8;
    temp |= vl;
    tp = (40.0 + (temp/256.0));

    return tp;
}

/**
  * @brief  写入一个字节
  * @param  _data
  * @retval None
  */
static void DS18B20_WriteByte(uint8_t _data)
{
	uint8_t i, dat;
    dat = _data; 
    DQ2_OUT();
    for(i = 0; i < 8; i++)
    {
        DQ2_CLR();            //每写入一位数据之前先把总线拉低1us
        delay1us();
        if (dat & 0x01)    //取最低位写入
        {
            DQ2_SET();
        }
        else
        {
            DQ2_CLR();
        }
        delay68us();       //延时68us,持续时间最少60us
        DQ2_SET();           //然后释放总线
        dat >>= 1;        //从低位开始写
    }
    delay68us();
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
        delay1us();
        dat >>= 1;           //从最低位开始读, 先移位再读取！！！
        DQ2_SET();           //然后释放总线
        DQ2_IN();
        if(DQ2_RED()) 
        {
            dat |= 0x80;   //每次读一位
        }
        delay68us();       //读取完之后等待48us再接着读取下一个数
    }
    return dat;
}


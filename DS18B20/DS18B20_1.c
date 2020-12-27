/******************************************************************************
* @file    DS18B20_1.c
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
#include "DS18B20_1.h"   

extern void DelayUs(uint16_t _us);


static void DS18B20_1_WriteByte(uint8_t _data);
static uint8_t DS18B20_1_ReadByte(void);
static uint8_t DS18B20_1_Init(void);

    // uint8_t DS18B20_1_ID[8] = {0x00};


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
  * @brief  ��ʼ��������.
  * @param  None
  * @retval �Ƿ���Ӧ��1��Ӧ��0û��Ӧ
  */
static uint8_t DS18B20_1_Init(void)
{
    uint8_t i = 0;

    DQ1_OUT();
    DQ1_SET();
    DelayUs(1);     //������ʱ
    DQ1_CLR();
    DelayUs(640);    //��ʱ480��960us
    DQ1_SET();

    DQ1_IN();

    while(DQ1_RED())    //�ȴ�DS18B20_1��������
    {
        DelayUs(640);
        if(++i>5)         //Լ�ȴ�>5MS
        {
            return 0;   //��ʼ��ʧ��
        }    
    }
    DelayUs(500);
    return 1;
}

void DS18B20_1_SingleConvert(void)
{

    // DQ1_OUT();
    // DQ1_SET();
    // DelayUs(640);       //500
    // DQ1_CLR();
    // DelayUs(87);        //68
    // DQ1_SET();
    // DelayUs(58);        //48
    // DQ1_CLR();
    // DelayUs(1);
    // DQ1_SET();
    // DelayUs(640);       //500
    // DQ1_CLR();


    DS18B20_1_Init();
    DS18B20_1_WriteByte(SKIP_ROM);
    DS18B20_1_WriteByte(CONVERT_T);
    DQ1_OUT();
    DQ1_SET();
}

void DS18B20_1_Convert(uint8_t *_id)
{
    uint8_t i;
    DS18B20_1_Init();
    DS18B20_1_WriteByte(MATCH_ROM);
    for(i = 0; i < 8; i++)
    {
        DS18B20_1_WriteByte(_id[i]);
    }
    DS18B20_1_WriteByte(CONVERT_T);
    DQ1_SET();
}

/**
  * @brief  ��ȡ�����¶ȵ�.
  * @param  None
  * @retval None
  */
float DS18B20_1_GetSingelTemp(void)
{
    uint8_t vl,vh; 
    int16_t temp; 
    float tp;

    // DS18B20_1_Init();
    // DS18B20_1_WriteByte(SKIP_ROM);
    // DS18B20_1_WriteByte(CONVERT_T);
    // DQ1_SET();
    // delay_us(100); //��ʱ1s

    DS18B20_1_Init();
    DS18B20_1_WriteByte(SKIP_ROM);
    DS18B20_1_WriteByte(READ_SCRATCHPAD);
    // DelayUs(500);
    vl = DS18B20_1_ReadByte();
    vh = DS18B20_1_ReadByte();
    temp = vh;
    temp <<= 8;

    temp = temp | vl;

    tp = (40.0 + (temp/256.0));

    return tp;
}


/**
  * @brief  ��ȡ����64λID.
  * @param  None
  * @retval None
  */
// void DS18B20_1_GetSingelID(void)
void DS18B20_1_GetSingelID(uint8_t *_id)
{
    uint8_t i;

    DS18B20_1_Init();
    DS18B20_1_WriteByte(READ_ROM);
    for(i = 0; i < 8; i++)
    {
        _id[i] = DS18B20_1_ReadByte();
        // DS18B20_1_ID[i] = DS18B20_1_ReadByte();
    }

}

/**
  * @brief  ��ȡָ���¶ȵ�.
  * @param  None
  * @retval None
  */
float DS18B20_1_GetTemp(uint8_t *_id)
{
    uint8_t vl,vh,i; 
    int16_t temp; 
    float tp;

    // DS18B20_1_Init();
    // DS18B20_1_WriteByte(MATCH_ROM);
    // for(i = 0; i < 8; i++)
    // {
    //     DS18B20_1_WriteByte(_id);
    // }
    // DS18B20_1_WriteByte(CONVERT_T);
    // DQ1_SET();
    // delay_us(100); //��ʱ1s

    DS18B20_1_Init();
    DS18B20_1_WriteByte(MATCH_ROM);
    DelayUs(1);
    for(i = 0; i < 8; i++)
    {
        DS18B20_1_WriteByte(_id[i]);
    }
    DS18B20_1_WriteByte(READ_SCRATCHPAD);
    DelayUs(1);
    vl = DS18B20_1_ReadByte();
    vh = DS18B20_1_ReadByte();
    temp = vh;
    temp <<= 8;

    temp = temp | vl;


    tp = (40.0 + (temp/256.0));

    return tp;
}

/**
  * @brief  д��һ���ֽ�
  * @param  _data
  * @retval None
  */
static void DS18B20_1_WriteByte(uint8_t _data)
{
	uint8_t i; 
    DQ1_OUT();

    for(i = 0; i < 8; i++)
    {
        DQ1_CLR();            //ÿд��һλ����֮ǰ�Ȱ���������1us
        // DelayUs(1);
        if (_data & 0x01)    //ȡ���λд��
        {
            DQ1_SET();
        }
        else
        {
            DQ1_CLR();
        }
        DelayUs(87);       //��ʱ68us,����ʱ������60us
        DQ1_SET();           //Ȼ���ͷ�����
        _data >>= 1;        //�ӵ�λ��ʼд
    }

    DelayUs(58);
}

/**
  * @brief  ��ȡһ���ֽ�
  * @param  None
  * @retval һ���ֽ�
  */
static uint8_t DS18B20_1_ReadByte(void)
{
	uint8_t i, dat; 
    dat = 0;

    for(i = 0; i < 8; i++)
    {
        DQ1_OUT();
        DQ1_CLR();           //�Ƚ���������1us
        // DelayUs(1);
        dat >>= 1;         //�����λ��ʼ��

        DQ1_SET();           //Ȼ���ͷ�����
        // DelayUs(1);
        DQ1_IN();
        if(DQ1_RED()) 
        {
            dat |= 0x80;   //ÿ�ζ�һλ
        }

        DelayUs(58);       //��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
    }

    return dat;
}


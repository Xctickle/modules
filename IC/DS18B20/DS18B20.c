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
  * @brief  ��ʼ��������.
  * @param  None
  * @retval �Ƿ���Ӧ��1��Ӧ��0û��Ӧ
  */
uint8_t DS18B20_Init(void)
{
    uint16_t i = 0;

    DQ2_OUT();
    DQ2_SET();
    delay1us();     //������ʱ
    DQ2_CLR();
    delay500us();    //��ʱ480��960us
    DQ2_SET();

    DQ2_IN();
    while(DQ2_RED())    //�ȴ�DS18B20��������
    {
        delay1us();     //���ʱ�䲻�˹����������ƽʶ�����
        if(++i>10) 
        {
            return 0;   //��ʼ��ʧ��
        }    
    }
    delay500us();       //�������ʱ
    return 1;
}

/**
  * @brief  ��·����������ת��.
  * @param  None
  * @retval �Ƿ���Ӧ��1��Ӧ��0û��Ӧ
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
  * @brief  ָ������������ת��.
  * @param  _id��64λID
  * @retval �Ƿ���Ӧ��1��Ӧ��0û��Ӧ
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
  * @brief  ��ȡ�����¶ȵ�.
  * @param  None
  * @retval �����¶ȣ�-99Ϊ����������Ӧ
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
  * @brief  ��ȡ����64λID.
  * @param  _id��64λID����
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
  * @brief  ��ȡָ���¶ȵ�.
  * @param  _id��64λID
  * @retval �¶�
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
  * @brief  д��һ���ֽ�
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
        DQ2_CLR();            //ÿд��һλ����֮ǰ�Ȱ���������1us
        delay1us();
        if (dat & 0x01)    //ȡ���λд��
        {
            DQ2_SET();
        }
        else
        {
            DQ2_CLR();
        }
        delay68us();       //��ʱ68us,����ʱ������60us
        DQ2_SET();           //Ȼ���ͷ�����
        dat >>= 1;        //�ӵ�λ��ʼд
    }
    delay68us();
}

/**
  * @brief  ��ȡһ���ֽ�
  * @param  None
  * @retval һ���ֽ�
  */
static uint8_t DS18B20_ReadByte(void)
{
	uint8_t i, dat; 

    dat = 0;

    for(i = 0; i < 8; i++)
    {
        DQ2_OUT();
        DQ2_CLR();           //�Ƚ���������1us
        delay1us();
        dat >>= 1;           //�����λ��ʼ��, ����λ�ٶ�ȡ������
        DQ2_SET();           //Ȼ���ͷ�����
        DQ2_IN();
        if(DQ2_RED()) 
        {
            dat |= 0x80;   //ÿ�ζ�һλ
        }
        delay68us();       //��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
    }
    return dat;
}


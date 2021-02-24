           
           
#include "sht3xDIS.h"
#include "misc.h"
#include "SoftwareI2C.h"


static float temperature;
static float humidity;

void sht3xDIS_Init(void)
{
	InitI2C();

	temperature = -99;
	humidity = -99;
}

uint8_t sht3xDIS_RHMeasure(void)
{ 
    uint8_t data_temp[2];
    uint8_t data_humi[2];
    uint8_t p_checksum_temp;
    uint8_t p_checksum_humi;
    uint16_t tmp = 0;

	i2c_Start();
	i2c_SendByte(0x89);
	i2c_WaitAck();
	data_temp[0] = i2c_ReadByte();
	i2c_Ack();
	data_temp[1] = i2c_ReadByte();
	i2c_Ack();
	p_checksum_temp = i2c_ReadByte();
	i2c_Ack();
	data_humi[0] = i2c_ReadByte();
	i2c_Ack();
	data_humi[1] = i2c_ReadByte();
	i2c_Ack();
	p_checksum_humi = i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();


	i2c_Start();
	i2c_SendByte(0x88);
	i2c_WaitAck();
	i2c_SendByte(0x2C);
	i2c_WaitAck();
	i2c_SendByte(0x06);
	i2c_WaitAck();
	i2c_Stop();

    if(!CheckCrc(data_temp,2, p_checksum_temp))
    {
        return FALSE;
    }
    if(!CheckCrc(data_humi,2, p_checksum_humi))
    {
        return FALSE;
    }


    tmp = data_temp[0] << 8;
    tmp += data_temp[1];
    humidity = (float)tmp * 1.0;
	humidity = -45 + (175*humidity)/65535;

    tmp = data_humi[0] << 8;
    tmp += data_humi[1];
    temperature = (float)tmp * 1.0;
	temperature = (100*temperature)/65535;

    return TRUE;
}



float sht3xDIS_getHumidity(void)
{
    return humidity;
}

float sht3xDIS_getTemperature(void)
{
    return temperature;
}
           

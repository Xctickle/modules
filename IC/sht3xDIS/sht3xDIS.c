           
#include "modules.h"
           
#include "sht3xDIS.h"
#include <math.h>    //Keil library  



// static float temperature;
// static float humidity;
    uint16_t tmp;


void sht3xDIS_Init(void)
{
	InitI2C();

	// temperature = -99;
	// humidity = -99;
}

unsigned char CalcCrc(unsigned char *data1, unsigned char nbrOfBytes)
{
	unsigned char crc = 0xFF;	
	unsigned char byteCtr;
	char i;
	for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
	{ 
        crc ^= (data1[byteCtr]);
        for ( i = 0; i < 8; i++)   // for ( i = 8; i > 0; i--)
        { 
            if (crc & 0x80) 
                crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
	}

	return crc;
}

unsigned char CheckCrc(unsigned char *data1, unsigned char nbrOfBytes, unsigned char checksum)
{
	unsigned char crc = 0xFF;	
	unsigned char byteCtr;
	char i;
	for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
	{ 
        crc ^= (data1[byteCtr]);
        for ( i = 0; i < 8; i++)   // for ( i = 8; i > 0; i--)
        { 
            if (crc & 0x80) 
                crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
	}

	if (crc != checksum) 
		return 0;
	else 
		return 1;
}

float get_absolute_humidity(int32_t temperature, int32_t humidity)
{
	float temp_f, humi_f;
	float ah;

	temp_f = temperature / 1000.0f,
	humi_f = humidity / 1000.0f,

	ah = exp(17.62 * temp_f / (243.12 + temp_f));
	ah = humi_f / 100 * 6.112 * ah;
	ah = ah / (273.15 + temp_f);
	ah = ah * 216.7;

	return ah;
}

uint8_t sht3xDIS_RHMeasure(int32_t *temperature, int32_t *humidity)
{ 
    uint8_t data_temp[2];
    uint8_t data_humi[2];
    uint8_t p_checksum_temp;
    uint8_t p_checksum_humi;

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

	// tmp = 0;
    // tmp = data_temp[0] << 8;
    // tmp += data_temp[1];
	// tmp = ((175*tmp)/65535) - 45;
	// *temperature = tmp;

	// tmp = 0;
    // tmp = data_humi[0] << 8;
    // tmp += data_humi[1];
	// tmp = (100*tmp)/65535;
	// *humidity = tmp;

	tmp = 0;
    tmp = data_temp[0] << 8;
    tmp += data_temp[1];
    *temperature = ((21875 * (int32_t)tmp) >> 13) - 45000;

	tmp = 0;
	tmp = data_humi[0] << 8;
	tmp += data_humi[1];
    *humidity = ((12500 * (int32_t)tmp) >> 13);


    return TRUE;
}

unsigned char SGPC3_Init(void)
{
    i2c_Start();
    i2c_SendByte(0xB0);
    if(i2c_WaitAck())
    {
        return 0;
    }	   
    i2c_SendByte(0x20);
	i2c_WaitAck();	    
    i2c_SendByte(0x03);    //Init_air_quality
	i2c_WaitAck();	    
    i2c_Stop();
    return 1;
}

unsigned char SGPC3_measure(uint16_t *CO2, uint16_t *TVOC)
{ 
    unsigned char p_checksum_TVOC, p_checksum_CO2;
    unsigned char co2_gas[2];
    unsigned char tvoc_gas[2];


    i2c_Start();
    i2c_SendByte(0xB0);  
	if(i2c_WaitAck())
    {
        return 0;
    }	   
    i2c_SendByte(0x20); 
	i2c_WaitAck();	    
    i2c_SendByte(0x08);
	i2c_WaitAck();	    
    i2c_Stop();


    Delay_ms(50);

    i2c_Start();
    i2c_SendByte(0xB1);
	if(i2c_WaitAck())
    {
        return 0;
    }	    
    Delay_ms(20);

    co2_gas[0] =  i2c_ReadByte(); 
	i2c_Ack();
    co2_gas[1] =  i2c_ReadByte(); 
	i2c_Ack();
    p_checksum_CO2 = i2c_ReadByte();
	i2c_Ack();
	tvoc_gas[0] =  i2c_ReadByte(); 
	i2c_Ack();
    tvoc_gas[1] =  i2c_ReadByte(); 
	i2c_Ack();
    p_checksum_TVOC = i2c_ReadByte();
	i2c_NAck();
    i2c_Stop();

    if(CheckCrc(co2_gas,2, p_checksum_CO2) == 0)
    {
        return 0;
    }
    if(CheckCrc(tvoc_gas,2, p_checksum_TVOC) == 0)
    {
        return 0;
    }

    *CO2 = (uint16_t)(co2_gas[0] << 8) | (uint16_t)(co2_gas[1]);
    *TVOC = (uint16_t)(tvoc_gas[0] << 8) | (uint16_t)(tvoc_gas[1]);

    // TVOC_float = (float)data_hex * 1.0;

    return 1;
}


int16_t sgp30_set_absolute_humidity(float absolute_humidity) 
{
    float ah;
    uint32_t ah_scaled;
	uint8_t crc;
	uint8_t ah_data[2];


    // ah = ah * 256 * 16777;
    // ah_scaled = (uint16_t)(ah >> 24);


	ah = (uint16_t)(absolute_humidity * 100.0);
	ah = (uint16_t)(ah * 256.0 / 100.0);

	ah_scaled = ah;



	ah_data[0] = ah_scaled >> 8;
	ah_data[1] = ah_scaled;


	crc = CalcCrc(ah_data, 2);


    i2c_Start();
    i2c_SendByte(0xB0);  
	if(i2c_WaitAck())
    {
        return 0;
    }	    
    i2c_SendByte(0x20);
	i2c_WaitAck();	     
    i2c_SendByte(0x61);
	i2c_WaitAck();   
    i2c_SendByte(ah_data[0]);
	i2c_WaitAck();	    
    i2c_SendByte(ah_data[1]);
	i2c_WaitAck();	    
    i2c_SendByte(crc);
	i2c_WaitAck();	    
    i2c_Stop();

	return 1;

}
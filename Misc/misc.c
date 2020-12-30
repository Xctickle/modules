

#include "misc.h"


bool CheckCrc(unsigned char *data1, unsigned char nbrOfBytes, unsigned char checksum)
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

void BubleSort(uint16_t a[], uint8_t n)
{
    uint8_t i,j;
    uint16_t temp = 0;
    
    for(j = 0; j < n; j++)   /* ���ݷ�Ҫ����n��*/
    {
        for(i = 0; i < n-j; i++)  /* ֵ�Ƚϴ��Ԫ�س���ȥ��ֻ��ʣ�µ�Ԫ���е����ֵ�ٳ���ȥ�Ϳ����� */
        {
            if(a[i] > a[i+1])  /* ��ֵ�Ƚϴ��Ԫ�س����� */
            {
                temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
            }
        }
    }
}


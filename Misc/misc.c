

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
    
    for(j = 0; j < n; j++)   /* 气泡法要排序n次*/
    {
        for(i = 0; i < n-j; i++)  /* 值比较大的元素沉下去后，只把剩下的元素中的最大值再沉下去就可以啦 */
        {
            if(a[i] > a[i+1])  /* 把值比较大的元素沉到底 */
            {
                temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
            }
        }
    }
}


#ifndef _C_MISC_H_
#define _C_MISC_H_


#include "modules.h"

int str_len(char *_str);
void str_cpy(char *_tar, char *_src);
int str_cmp(char * s1, char * s2);
void mem_set(char *_tar, char _data, int _len);

void int_to_str(int _iNumber, char *_pBuf, unsigned char _len);
int str_to_int(char *_pStr);

uint16_t BEBufToUint16(uint8_t *_pBuf);
uint16_t LEBufToUint16(uint8_t *_pBuf);

uint32_t BEBufToUint32(uint8_t *_pBuf);
uint32_t LEBufToUint32(uint8_t *_pBuf);

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen) ;
int32_t  CaculTwoPoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x);

char BcdToChar(uint8_t _bcd);
void HexToAscll(uint8_t * _pHex, char *_pAscii, uint16_t _BinBytes);
uint32_t AsciiToUint32(char *pAscii);

unsigned char CheckCrc(unsigned char *data1, unsigned char nbrOfBytes, unsigned char checksum);
void BubleSort(uint16_t a[], uint8_t n);
uint16_t DataRemap_reversal(uint16_t val, uint16_t rangeMax, uint16_t rangeMin, uint16_t rangeMaxNew, uint16_t rangeMinNew);
uint16_t DataRemap(uint16_t val, uint16_t rangeMax, uint16_t rangeMin, uint16_t rangeMaxNew, uint16_t rangeMinNew);
void ArrayDataShift(uint16_t newvol, uint16_t *buf, uint16_t len);

#endif

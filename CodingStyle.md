
# ��������
## ����
|����|ǰ׺|����|
|---|---|---|
uint8_t  | uc | ucSum |
uint16_t | us | usParaWord |
uint32_t | ul | ulParaWord |
uint8_t *  | ucp | ucpWrite |
uint16_t *  | usp | ucWrite |
uint32_t *  | ulp | ucWrite |
uint8_t ���� | uca | ucaNum[5] |
uint16_t ���� | usa | usaNum[5] |
uint32_t ���� | ula | ulaNum[5] |
�ṹ�� | t | tParam
---
## ȫ�ֱ���
* uint32_t g_ulParaWord
## ��̬����
* static uint32_t s_ulParaWord

## �ṹ��
```c
/* �ṹ������������ */
typedef struct tagBillQuery_T
{
...
}BillQuery_T;

/* �ṹ��������� */
BillQuery_T tBillQuery;
```
## ö��
```c
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;
```
## ����
* ÿһ��������ǰ׺�����ģ������ģ����ΪСд���뺯�������𿪡�
    > uartReceive(���ڽ���)
* �����ĵ��βζ����»���_��ͷ����ʾ����ͨ�����������֣�����û���β�Ϊ�յĺ���(void)���Ž����������档
    > uint32_t uartConvUartBaud(uint32_t _ulBaud)


## ������д
> �϶̵ĵ��ʿ�ͨ��ȥ����Ԫ�����γ���д���ϳ��ĵ��ʿ�ȡ���ʵ�ͷ������ĸ�γ���д
- temp ����дΪ tmp;
- flag ����дΪ flg;
- statistic ����дΪ stat;
- increment ����дΪ inc;
- message ����дΪ msg;


# 命名规则
## 变量
|类型|前缀|例子|
|---|---|---|
uint8_t  | uc | ucSum |
uint16_t | us | usParaWord |
uint32_t | ul | ulParaWord |
uint8_t *  | ucp | ucpWrite |
uint16_t *  | usp | ucWrite |
uint32_t *  | ulp | ucWrite |
uint8_t 数组 | uca | ucaNum[5] |
uint16_t 数组 | usa | usaNum[5] |
uint32_t 数组 | ula | ulaNum[5] |
结构体 | t | tParam
---
## 全局变量
* uint32_t g_ulParaWord
## 静态变量
* static uint32_t s_ulParaWord

## 结构体
```c
/* 结构体命名类型名 */
typedef struct tagBillQuery_T
{
...
}BillQuery_T;

/* 结构体变量定义 */
BillQuery_T tBillQuery;
```
## 枚举
```c
typedef enum
{
	TMR_ONCE_MODE = 0,		/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
}TMR_MODE_E;
```
## 函数
* 每一个函数名前缀需包含模块名，模块名为小写，与函数名区别开。
    > uartReceive(串口接收)
* 函数的的形参都以下划线_开头，已示与普通变量进行区分，对于没有形参为空的函数(void)括号紧跟函数后面。
    > uint32_t uartConvUartBaud(uint32_t _ulBaud)


## 常见缩写
> 较短的单词可通过去掉“元音”形成缩写；较长的单词可取单词的头几个字母形成缩写
- temp 可缩写为 tmp;
- flag 可缩写为 flg;
- statistic 可缩写为 stat;
- increment 可缩写为 inc;
- message 可缩写为 msg;

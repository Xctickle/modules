
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

## 函数头描述
```c
/**
  * @brief  Enables or Disables the ADC1 peripheral.
  * @param   NewState: specifies the peripheral enabled or disabled state.
  * @retval None
  */
```
## 常见缩写
> 较短的单词可通过去掉“元音”形成缩写；较长的单词可取单词的头几个字母形成缩写
- temp 可缩写为 tmp;
- flag 可缩写为 flg;
- statistic 可缩写为 stat;
- increment 可缩写为 inc;
- message 可缩写为 msg;

## 头文件
```c
/*
	stdlib.h : standard library标准库头文件,包含了C、C++语言的最常用的类型定义和系统函数，比如 malloc()、free()
	
	stdio.h : standard input & output 标准输入输出函数库。常用函数有 printf()、getc()、putc()
	
	string.h : 关于字符数组的函数定义的头文件库，常用函数有strlen、strcmp、strcpy
	
	" " 和 < > 的区别:
	" " 一般表示源代码自带的h文件。编译时，编译器优先在工程配置指定的搜索路径列表中寻找头文件
	< > 一般表示编译系统自带的h文件。编译时，编译器优先在IDE自带的库文件夹寻找头文件。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

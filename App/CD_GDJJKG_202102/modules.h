#ifndef __MODULES_H_
#define __MODULES_H_


#include "HT68F0025.h"


#define CD_GDJJKG

#define ENABLE_INT()	_emi = 1		/* 使能全局中断 */
#define DISABLE_INT()	_emi = 0		/* 禁止全局中断 */
#define FEED_WDG()		GCC_CLRWDT()	/* 喂狗 */

#define NOP()			nop()					


#if 0	/* 1表示使能调试语句， 0 表示不参与编译，不会影响编译结果 */
	#define MODULES_Printf		printf
#else
	#define MODULES_Printf    (...)
#endif

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif


#endif

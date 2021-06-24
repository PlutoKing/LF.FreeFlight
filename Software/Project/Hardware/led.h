#ifndef __LED_H
#define __LED_H

/********** 头 文 件 **********/
#include "sys.h"
#include "delay.h"

/********** 宏 定 义 **********/
#define LED0 PAout(6)		// D2
#define LED1 PAout(7)		// D3	 

/********** 函数声明 **********/
void LED_Init(void);
void LED_Test(void);
#endif

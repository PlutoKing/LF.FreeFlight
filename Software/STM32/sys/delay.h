#ifndef __DELAY_H
#define __DELAY_H

/********** 头 文 件 **********/
#include "sys.h"	  
 
/********** 函数声明 **********/
void Delay_Init(u8 SYSCLK);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);

#endif

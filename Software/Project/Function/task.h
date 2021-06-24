#ifndef __TASK_H
#define __TASK_H

/********** 头 文 件 **********/
#include "sys.h"
#include "stm32f4xx_it.h"
#include "delay.h"
#include "led.h"
#include "usart1.h"
#include "lflink.h"

/********** 宏 定 义 **********/


/********** 函数声明 **********/
void Sys_Init(void);
void Task_500Hz(void);
void Task_200Hz(void);
void Task_100Hz(void);
void Task_50Hz(void);
void Task_20Hz(void);
void Task_Loop(void);
#endif

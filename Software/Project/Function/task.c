/****************************************************
* @File		: task.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: 任务调度
*****************************************************/

/********** 头 文 件 **********/
#include "task.h"

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: Sys_Init
* @Brief	: 系统初始化
* @param	: void
* @return	: void
---------------------------------------------------*/
void Sys_Init(void)
{
	Delay_Init(168);		// 延时函数初始化
	
	LED_Init();				// LED 初始化

	USART1_Init(115200);	// 串口初始化
}



/** --------------------------------------------------
* @Function	: Task_Loop
* @Brief	: 任务循环
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_Loop(void)
{

	if(it_cnt == 2) 		// 2ms，500Hz任务
	{
		Task_500Hz();
	}
	else if(it_cnt == 5) 	// 5ms，200Hz任务
	{
		Task_500Hz();
	}
	else if(it_cnt == 10) 	// 10ms，100Hz任务
	{
		Task_500Hz();
	}
	else if(it_cnt == 20) 	// 20ms，50Hz任务
	{
		Task_50Hz();
	}
	else if(it_cnt == 50) 	// 50ms，20Hz任务
	{
		Task_20Hz();
		it_cnt = 0;
	}
}

/** --------------------------------------------------
* @Function	: Task_500Hz
* @Brief	: 500Hz任务
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_500Hz(void)
{

}

/** --------------------------------------------------
* @Function	: Task_200Hz
* @Brief	: 200Hz任务
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_200Hz(void)
{

}

/** --------------------------------------------------
* @Function	: Task_100Hz
* @Brief	: 100Hz任务
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_100Hz(void)
{

}

/** --------------------------------------------------
* @Function	: Task_50Hz
* @Brief	: 50Hz任务
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_50Hz(void)
{

}

/** --------------------------------------------------
* @Function	: Task_20Hz
* @Brief	: 20Hz任务
* @param	: void
* @return	: void
---------------------------------------------------*/
void Task_20Hz(void)
{
	LFLink_SendMessage(0);	// 发送心跳消息
}
/*********** Copyright © 2021 蓝风实验室 ***********/

/****************************************************
* @File		: delay.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: 延时函数
*****************************************************/

/********** 头 文 件 **********/
#include "delay.h"

/********** 变    量 **********/
static u8  fac_us=0;	// us
static u16 fac_ms=0;	// ms

/********** 方    法 **********/   
/** --------------------------------------------------
* @Function	: Delay_init
* @Brief	: 初始化延时配置
* @param	: SYSCLK : clock of system
* @return	: void
---------------------------------------------------*/
void Delay_Init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK /8 ;		
	fac_ms = (u16)fac_us * 1000;
}	


/** --------------------------------------------------
* @Function	: Delay_us
* @Brief	: 延时微秒
* @param	: nus - 时间，微秒单位， <=798915
* @return	: void
---------------------------------------------------*/
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us;		 
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00; 
}

/** --------------------------------------------------
* @Function	: Delay_xms
* @Brief	:  
* @param	: nms : time in milliseconds, <= 0xffffff*8*1000/SYSCLK
* @return	: void
---------------------------------------------------*/
void Delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms*fac_ms;
	SysTick->VAL = 0x00; 
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;   	    
} 

/** --------------------------------------------------
* @Function	: Delay_ms
* @Brief	: 延时毫秒
* @param	: nms - 时间，毫秒单位
* @return	: void
---------------------------------------------------*/
void Delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;

	u16 remain=nms%540;
	while(repeat)
	{
		Delay_xms(540);
		repeat--;
	}
	if(remain)
		Delay_xms(remain);
} 

/*********** Copyright © 2021 蓝风实验室 ***********/

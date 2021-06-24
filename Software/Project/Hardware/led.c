/****************************************************
* @File		: led.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: 初始化LED灯
*****************************************************/

/********** 头 文 件 **********/
#include "led.h" 

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: LED_Init
* @Brief	: 初始化LED灯的GPIO
* @param	: void
* @return	: void
---------------------------------------------------*/
void LED_Init(void)
{
	/* 1. 定义 */
	GPIO_InitTypeDef  GPIO_Config;

	/* 2. 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// GPIOA时钟使能

	/* 3. GPIO配置 */
	GPIO_Config.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;			// PA6和PA7
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;					// 复用映射
	GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;				// 50M
	GPIO_Config.GPIO_OType = GPIO_OType_PP;					// push-pull
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;					// 上拉高电平
	GPIO_Init(GPIOA,&GPIO_Config);							// 初始化GPIO配置
	
	/* 4. 初始化 */
	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);			// 高电平，灯不亮
}

/** --------------------------------------------------
* @Function	: LED_Test
* @Brief	: 跑马灯测试
* @param	: void
* @return	: void
---------------------------------------------------*/
void LED_Test(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_6); 	// D2 开
	GPIO_SetBits(GPIOA,GPIO_Pin_7);		// D3 关
	Delay_ms(500); 
	GPIO_SetBits(GPIOA,GPIO_Pin_6); 	// D2 关
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	// D3 开
	Delay_ms(500);

}

/*********** Copyright © 2021 蓝风实验室 ***********/

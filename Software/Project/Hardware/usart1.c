/****************************************************
* @File		: usart1.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: USART1串口 PA9和PA10
*****************************************************/

/********** 头 文 件 **********/
#include "usart1.h"

/********** 变    量 **********/
u8 * Send_Buff;
u8 * Rece_Buff;
u16 USART_RX_STA = 0;
u8 USART_RX_BUF[USART_REC_LEN];

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: USART1_Init
* @Brief	: 初始化USART1
* @param	: void
* @return	: void
---------------------------------------------------*/
void USART1_Init(u32 baudrate)
{
	/* 0. 定义 */
	GPIO_InitTypeDef GPIO_Config;	// 定义GPIO配置结构体
	USART_InitTypeDef UART_Config;	// 定义UART配置结构体
	NVIC_InitTypeDef NVIC_Config;	// 定义NVIC配置结构体

	/* 1. 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	// GPIOA时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	// USART1时钟使能

	/* 2. 复用映射 */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	/* 3. GPIO配置 */
	GPIO_Config.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		// PA9和PA10
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;					// 复用映射
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;				// 50M
	GPIO_Config.GPIO_OType = GPIO_OType_PP;					// Push-pull
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;					// High level
	GPIO_Init(GPIOA,&GPIO_Config);							// 初始化GPIO配置

	/* 4. USART配置 */
	UART_Config.USART_BaudRate = baudrate;					// 波特率
	UART_Config.USART_WordLength = USART_WordLength_8b;		// 数据位
	UART_Config.USART_StopBits = USART_StopBits_1;			// 停止位
	UART_Config.USART_Parity = USART_Parity_No;				// 校验位
	UART_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_Config.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	// 收发模式
	USART_Init(USART1,&UART_Config);						// 初始化USART1
	USART_Cmd(USART1,ENABLE);								// USART1使能
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	/* 5. NVIC配置 */
	NVIC_Config.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Config.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Config.NVIC_IRQChannelSubPriority = 2;
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Config);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/** --------------------------------------------------
* @Function	: USART1_SendData
* @Brief	: 发送数据
* @param	: buf    - 发送数据缓存
			  length - 发送数据长度
* @return	: void
---------------------------------------------------*/
void USART1_SendData(unsigned char * buf, int length)
{
	memcpy(Send_Buff,buf,length);
}


/*--------------------------------------------------
* @Function	: USART1_IRQHandler
* @Brief	: USART1终端函数
* @param	: void
* @return	: void
---------------------------------------------------*/
void USART1_IRQHandler(void)
{
	u8 res;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		res = USART_ReceiveData(USART1);
		if((USART_RX_STA & 0X8000) == 0)
		{
			if(USART_RX_STA & 0x4000)
			{
				if(res != 0x0A)
					USART_RX_STA = 0;
				else
					USART_RX_STA |= 0x8000;
			}
			else
			{
				if(res == 0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA = 0;
				}
			}
		}
	}
}

/*********** Copyright © 2021 蓝风实验室 ***********/

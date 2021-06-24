#ifndef __USART1_H
#define __USART1_H

/********** 头 文 件 **********/
#include "sys.h"
#include "string.h"
#include "stm32f4xx_usart.h"

/********** 宏 定 义 **********/
#define USART_REC_LEN		200

/********** 外部变量 **********/
extern u8 USART_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;

/********** 函数声明 **********/
void USART1_Init(u32 baudrate);
void USART1_SendData(unsigned char * buf, int length);
#endif

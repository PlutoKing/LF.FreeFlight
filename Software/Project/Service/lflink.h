#ifndef __LFLink_H
#define __LFLink_H

/********** 头 文 件 **********/
#include "sys.h"
#include "usart1.h"

/********** 宏 定 义 **********/

#define BYTE0(BYTE_TEMP)       ( *( char *)(&BYTE_TEMP))
#define BYTE1(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 1))
#define BYTE2(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 2))
#define BYTE3(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 3))


// 消息相关定义
#define STX 					0xAA	// 消息帧头

// 消息ID定义
#define MSG_HEART_ID			0x00	// 心跳包


/********** 函数声明 **********/
void LFLink_SendMessage(u8 msgID);
u8 * Get_LFLink_Heartbeat(void);
unsigned short CRC_Calculate(u8 * buff, int length);
unsigned short CRC_Accumulate(u8 b, unsigned short crc);


#endif

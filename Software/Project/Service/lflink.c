/****************************************************
* @File		: lflink.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: LFLink蓝风通信协议
*****************************************************/

/********** 头 文 件 **********/
#include "lflink.h"

/********** 变    量 **********/
const int X25_INIT_CRC = 0xffff;
const int X25_VALIDATE_CRC = 0xf0b8;

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: LFLink_SendMessage
* @Brief	: 发送消息
* @param	: msgID - 消息ID
* @return	: void
---------------------------------------------------*/
void LFLink_SendMessage(u8 msgID)
{
	switch(msgID)
	{
		case MSG_HEART_ID:
			USART1_SendData(Get_LFLink_Heartbeat(),11);
			break;
	}
}

/** --------------------------------------------------
* @Function	: Get_LFLink_Heartbeat
* @Brief	: 生成心跳包
* @param	: void
* @return	: 心跳包指针
---------------------------------------------------*/
u8 * Get_LFLink_Heartbeat(void)
{
	const u16 len_msg = 3;
	static u8 data[len_msg+8];
	
	data[0] = STX;
	data[1] = len_msg;
	data[2] = 0;
	data[3] = 1;
	data[4] = 1;
	data[5] = MSG_HEART_ID;
	
	unsigned short crc = CRC_Calculate((void *)data,len_msg+6);
	data[9] = BYTE0(crc);
	data[10] = BYTE1(crc);
	
	return data;
}

/** --------------------------------------------------
* @Function	: CRC_Calculate
* @Brief	: 计算校验值
* @param	: buff   - 数据缓存
			  length - 数据长度
* @return	: 校验值
---------------------------------------------------*/
unsigned short CRC_Calculate(u8 * buff, int length)
{
	unsigned short crcTmp;
	int i;
	crcTmp = X25_VALIDATE_CRC;
	for(i = 1; i< length; i++)
	{
		crcTmp = CRC_Accumulate(buff[i],crcTmp);
	}
	return crcTmp;
	
}
/** --------------------------------------------------
* @Function	: CRC_Accumulate
* @Brief	: 计算当前数据bit的累计校验值
* @param	: b   - 当前数据位
			  crc - 已有累计校验值
* @return	: 当前累计校验值
---------------------------------------------------*/
unsigned short CRC_Accumulate(u8 b, unsigned short crc)
{
	u8 ch = (u8)(b ^ (u8)(crc & 0x00ff));
    ch = (u8)(ch ^ (ch << 4));
	return (unsigned short)((crc >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
}

/*********** Copyright © 2021 蓝风实验室 ***********/

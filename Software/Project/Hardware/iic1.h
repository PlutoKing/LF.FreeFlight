#ifndef __IIC1_H
#define __IIC1_H

/* Head Files */
#include "sys.h"
#include "delay.h"

/* Macro Definition */
#define SDA1_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
#define SDA1_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} 

#define IIC1_SCL PBout(8)		// SCL
#define IIC1_SDA PBout(9)		// SDA	
#define READ1_SDA PBin(9)		// Input

/* Function Declaration */
void IIC1_Init(void);
void IIC1_Start(void);
void IIC1_Stop(void);
u8 IIC1_Wait_Ack(void);
void IIC1_Ack(void);
void IIC1_NAck(void);
void IIC1_Send_Byte(u8 byte);
u8 IIC1_Read_Byte(unsigned char ack);

#endif

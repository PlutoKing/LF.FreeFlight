/****************************************************
* @File		: iic1.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: 使用GPIO（PB6和PB7）模拟IIC1
*****************************************************/

/********** 头 文 件 **********/
#include "iic1.h" 

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: IIC1_Init
* @Brief	: IIC1初始化
* @param	: void
* @return	: void
---------------------------------------------------*/
void IIC1_Init(void)
{
    /* 0. Definitions */
    GPIO_InitTypeDef GPIO_Config;                           // Define the GPIO Config Struct
    /* 1. Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    // GPIOB Clock
    /* 2. GPIO Config */
    GPIO_Config.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         // Pin
    GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;                  // Output
    GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;             // 50M
    GPIO_Config.GPIO_OType = GPIO_OType_PP;
    GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_Config);
    /* 3. Default config */
    IIC1_SCL = 1;
    IIC1_SDA = 1;
}

/** --------------------------------------------------
 * @Function: IIC1_Start
 * @Brief   : 产生开始信号
 * @param   : void
 * @return  : void
 ---------------------------------------------------*/
void IIC1_Start(void)
{
    SDA1_OUT(); 
    IIC1_SDA=1;IIC1_SCL=1;
    Delay_us(4);
    IIC1_SDA=0;
    Delay_us(4);
    IIC1_SCL=0;
}

/** --------------------------------------------------
 * @Function: IIC1_Stop
 * @Brief   : 产生停止信号
 * @param   : void
 * @return  : void
 ---------------------------------------------------*/
void IIC1_Stop(void)
{
    SDA1_OUT(); 
    IIC1_SDA=0;IIC1_SCL=0;
    Delay_us(4);
    IIC1_SDA=1;IIC1_SCL=1;
    Delay_us(4);
}

/** --------------------------------------------------
 * @Function: IIC1_Wait_Ack
 * @Brief   : 等待应答
 * @param   : void
 * @return  : 0 - 成功；1 - 失败
 ---------------------------------------------------*/
u8 IIC1_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA1_IN();
    IIC1_SDA = 1;
	Delay_us(1); 
    IIC1_SCL = 1;
	Delay_us(1); 
    while(READ1_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC1_Stop();
            return 1;
        }
    }
    IIC1_SCL = 0;
    return 0;
}

/** --------------------------------------------------
 * @Function: IIC1_Ack
 * @Brief   : 生产应答信号
 * @param   : void
 * @return  : void
 ---------------------------------------------------*/
void IIC1_Ack(void)
{
    IIC1_SCL=0; SDA1_OUT();
    IIC1_SDA=0; Delay_us(2);
    IIC1_SCL=1; Delay_us(2);
    IIC1_SCL=0;
}

/** --------------------------------------------------
 * @Function: IIC1_NAck
 * @Brief   : 生成不应答信号
 * @param   : void
 * @return  : void
 ---------------------------------------------------*/
void IIC1_NAck(void)
{
    IIC1_SCL=0;
    SDA1_OUT();
    IIC1_SDA=1;Delay_us(2);
    IIC1_SCL=1;Delay_us(2);
    IIC1_SCL=0;
}

/** --------------------------------------------------
 * @Function: IIC1_Send_Byte
 * @Brief   : 发送1字节数据
 * @param   : byte -- 1字节数据
 * @return  : void
 ---------------------------------------------------*/
void IIC1_Send_Byte(u8 byte)
{
    u8 t;
    SDA1_OUT();
    IIC1_SCL=0;
    for(t=0;t<8;t++)
    {
        IIC1_SDA=(byte&0x80)>>7;
        byte<<=1;
        Delay_us(2);
        IIC1_SCL=1;Delay_us(2);
        IIC1_SCL=0;Delay_us(2);
    } 
}

/** --------------------------------------------------
 * @Function: IIC1_Read_Byte
 * @Brief	: 读取1字节数据
 * @param	: ack - 1：产生应答信号；0：不产生应答信号
 * @return	: 读取到的数据
 ---------------------------------------------------*/
u8 IIC1_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA1_IN();
    for(i=0;i<8;i++ )
    {
        IIC1_SCL=0; Delay_us(2);
        IIC1_SCL=1;
        receive<<=1;
        if(READ1_SDA)receive++;
        Delay_us(1);
    } 
    if (!ack) IIC1_NAck();
    else IIC1_Ack();
    return receive;
}

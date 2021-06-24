/****************************************************
* @File		: mpu9250.c
* @Date		: 2021-06-24
* @Author	: Xu Zhe
* @Brief	: MPU9250相关代码
*****************************************************/

/********** 头 文 件 **********/
#include "mpu9250.h"

/********** 变    量 **********/
signed short int gyro_offset_x = 0;	// 陀螺仪校准值
signed short int gyro_offset_y = 0;	// 陀螺仪校准值
signed short int gyro_offset_z = 0;	// 陀螺仪校准值

signed short int acc_old_x,acc_old_y,acc_old_z;	// 加速度计上一次测量值
signed short int mag_old_x,mag_old_y,mag_old_z;	// 磁强计上一次测量值
signed short int gyro_old_x,gyro_old_y,gyro_old_z;	// 陀螺仪上一次测量值

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: MPU9250_Init
* @Brief	: 初始化MPU9250
* @param	: void
* @return	: void
---------------------------------------------------*/
u8 MPU9250_Init(void)
{
	u8 res = 0;
	
	IIC1_Init();										// 初始化IIC
	MPU9250_Write_Byte(MPU9250_ADDR,PWR_MGMT_1,0x80);	// 复位MPU9250
	Delay_ms(100);
	MPU9250_Write_Byte(MPU9250_ADDR,PWR_MGMT_1,0x00);	// 唤醒MPU9250
	Gyro_FS_Sel(0);										// 陀螺仪满量程选择 0 ±250dps
	Accel_FS_Sel(0);									// 加速度计满量程选择 0 ±2g
	MPU_Set_Rate(200);									// 配置采样频率200Hz
	MPU9250_Write_Byte(MPU9250_ADDR,INT_ENABLE,0x00);	// 关闭所有中断
	MPU9250_Write_Byte(MPU9250_ADDR,USER_CTRL,0x00);	// IIC主模式关闭
	MPU9250_Write_Byte(MPU9250_ADDR,FIFO_EN,0x00);		// 关闭FIFO
	MPU9250_Write_Byte(MPU9250_ADDR,INT_PIN_CFG,0x82);	// INT引脚低电平有效，开启旁路模式，直接读取磁力计
	res = MPU9250_Read_Byte(MPU9250_ADDR,WHO_AM_I);		// 读取MPU6500的ID，其默认值为0x71
	if (res == MPU6500_ID1 || res == MPU6500_ID2)		// 器件ID正确
	{
		MPU9250_Write_Byte(MPU9250_ADDR,PWR_MGMT_1, 0x01);	// 设置CLKSEL, PLL X轴为参考
		MPU9250_Write_Byte(MPU9250_ADDR,PWR_MGMT_2, 0x00);	// 加速度计与陀螺仪都工作
		MPU_Set_Rate(200);						      	// 配置采样频率200Hz
	}
	else
		return 1;
	
	res = MPU9250_Read_Byte(AK8963_ADDR,WIA);			// 读取AK8963 ID，默认值为0x48
	if (res == AK8963_ID)								// 器件ID正确
	{
		MPU9250_Write_Byte(AK8963_ADDR,CNTL, 0x11);		// 配置AK8963为单次测量
	}
	else
		return 1;
	
	return res;
}

/** --------------------------------------------------
* @Function	: MPU_Gyro_FS_Sel
* @Brief	: 配置陀螺仪GYRO满量程选择 Full Scale Select
* @param	: fs - 配置参数，详见note
* @return	: 0 - SUCCESS; 1 - ERROR.
* @note		: 00 - ±250dps
			  01 - ±500dps
			  10 - ±1000dps
			  11 - ±2000dps
---------------------------------------------------*/
u8 Gyro_FS_Sel(u8 fs)
{
	return MPU9250_Write_Byte(MPU9250_ADDR,GYRO_CONFIG,(fs << 3)|3);
}


/** --------------------------------------------------
* @Function	: Accel_FS_Sel
* @Brief	: 配置加速度计ACCEL满量程选择 Full Scale Select
* @param	: fs - 配置参数，详见note
* @return	: 0 - 成功；1 - 失败。
* @note		: 00 - ±2g
			  01 - ±4g
			  10 - ±8g
			  11 - ±16g
---------------------------------------------------*/
u8 Accel_FS_Sel(u8 fs)
{
	return MPU9250_Write_Byte(MPU9250_ADDR,ACCEL_CONFIG,fs<<3);
}

/** --------------------------------------------------
* @Function	: 
* @Brief	: 
* @param	: void
* @return	: void
---------------------------------------------------*/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate -1;
	data = MPU9250_Write_Byte(MPU9250_ADDR,SMPLRT_DIV,data);
	return MPU_Set_LPF(rate / 2);
}

/** --------------------------------------------------
* @Function	: MPU_Set_LPF
* @Brief	: 配置数字低通滤波器
* @param	: lpf - 数字低通滤波器频率（Hz）
* @return	: 0 - 成功；1 - 失败。
---------------------------------------------------*/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data = 0;
	if (lpf >=188)
		data = 1;
	else if (lpf >=98)
		data = 2;
	else if (lpf >=42)
		data = 3;
	else if (lpf >=20)
		data = 4;
	else if (lpf >=10)
		data = 5;
	else
		data = 6;
	
	return MPU9250_Write_Byte(MPU9250_ADDR,CONFIG,data);
}

/** --------------------------------------------------
* @Function	: MPU9250_Get_Temperature
* @Brief	: 获取MPU9250的温度值
* @param	: void
* @return	: 温度值的100倍
---------------------------------------------------*/
signed short int MPU9250_Get_Temperature(void)
{
	u8 buf[2];
			
	MPU9250_Read_Len(MPU9250_ADDR, TEMP_OUT_H, 2, buf);		// 读取温度计测量值寄存器中的值
	signed short int raw = ((u16)buf[0]<<8)|buf[1];			// 获得原始数据
	float temp = 21 + ((double)raw)/333.87;					// 温度计算方法
	
	return temp * 100;
}

/** --------------------------------------------------
* @Function	: 
* @Brief	: 
* @param	: gx,gy,gz - 陀螺仪原始数据
* @return	: void
---------------------------------------------------*/
u8 MPU9250_Get_Gyroscope(signed short int *gx, signed short int *gy, signed short int *gz)
{
	u8 buf[6], res;
	res = MPU9250_Read_Len(MPU9250_ADDR,GYRO_XOUT_H, 6, buf);	// 读取陀螺仪测量值寄存器中的值
	if(res == 0)
	{
		*gx = (((u16)buf[0]<<8)|buf[1]);  
		*gy = (((u16)buf[2]<<8)|buf[3]);  
		*gz = (((u16)buf[4]<<8)|buf[5]);

		*gx-= gyro_offset_x;
		*gy-= gyro_offset_y;
		*gz-= gyro_offset_z;

		*gx = (signed short int)(gyro_old_x*0.2+*gx*0.8);
		*gy =( signed short int)(gyro_old_y*0.2+*gy*0.8);
		*gz = (signed short int)(gyro_old_z*0.2+*gz*0.8);
		gyro_old_x = *gx;
		gyro_old_y = *gy;
		gyro_old_z = *gz;
	}
	return res;
}

/** --------------------------------------------------
* @Function	: MPU_Write_Byte
* @Brief	: 向MPU9250写入1位数字
* @param	: addr - device IIC address
			  reg  - register address
			  data - data to be written
* @return	: 0 - SUCCESS; 1 - ERROR.
---------------------------------------------------*/
u8 MPU9250_Write_Byte(u8 addr, u8 reg, u8 data)
{
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|0);	// 发送器件地址和写入命令
	if(IIC1_Wait_Ack())
	{
		IIC1_Stop();
		return 1;
	}
	IIC1_Send_Byte(reg);			// send register add
	IIC1_Wait_Ack();
	IIC1_Send_Byte(data);			// send data
	if(IIC1_Wait_Ack())
	{
		IIC1_Stop();
		return 1;
	}
	IIC1_Stop();
	return 0;
}

/** --------------------------------------------------
* @Function	: MPU_Read_Byte
* @Brief	: Read 1 bit data from MPU9250.
* @param	: addr - 器件地址
			  reg  - 寄存器地址
* @return	: data
---------------------------------------------------*/
u8 MPU9250_Read_Byte(u8 addr, u8 reg)
{
	u8 res;
	
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|0);	// 发送器件地址和写入命令
	IIC1_Wait_Ack();
	IIC1_Send_Byte(reg);			// 发送寄存器地址
	IIC1_Wait_Ack();
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|1);	// 发送器件地址和读取命令
	IIC1_Wait_Ack();
	res = IIC1_Read_Byte(0);		// 读取1位并发送nACK
	IIC1_Stop();
	
	return res;
}

/** --------------------------------------------------
* @Function	: MPU_Write_Len
* @Brief	: 向MPU9250写入数据
* @param	: addr - 器件地址
			  reg  - 寄存器地址
			  len  - 数据长度
			  buf  - 数据缓存器
* @return	: 0 - SUCCESS; 1 - ERROR.
---------------------------------------------------*/
u8 MPU9250_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i;
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|0);	// 发送器件地址和写入命令
	if(IIC1_Wait_Ack())
	{
		IIC1_Stop();
		return 1;
	}
	IIC1_Send_Byte(reg);			// 发送寄存器地址
	IIC1_Wait_Ack();
	for(i = 0; i < len; i++)
	{
		IIC1_Send_Byte(buf[i]);
		if(IIC1_Wait_Ack())
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_Stop();
	return 0;
}


/** --------------------------------------------------
* @Function	: MPU_Read_Len
* @Brief	: Read data from MPU9250.
* @param	: addr - device IIC address
			  reg  - register address
			  len  - data length
			  buf  - data buffer
* @return	: void
---------------------------------------------------*/
u8 MPU9250_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|0);	// send device add and wirte cmd
	if(IIC1_Wait_Ack())
	{
		IIC1_Stop();
		return 1;
	}
	IIC1_Send_Byte(reg);			// send register add
	IIC1_Wait_Ack();
	IIC1_Start();
	IIC1_Send_Byte((addr<<1)|1);	// send device add and read cmd
	IIC1_Wait_Ack();
	while(len)
	{
		if(len == 1)
			*buf = IIC1_Read_Byte(0);	// read data and send nACK
		else
			*buf = IIC1_Read_Byte(1);	// read data and send ACK
		
		len--;
		buf++;
	}
	IIC1_Stop();
	return 0;
}

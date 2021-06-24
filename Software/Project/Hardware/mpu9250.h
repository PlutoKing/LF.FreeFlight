#ifndef __MPU9250_H
#define __MPU9250_H

/********** 头 文 件 **********/
#include "sys.h"
#include "delay.h"
#include "iic1.h"

/********** 宏 定 义 **********/

// 如果AD0（P9）脚接地，IIC地址为0x68。
// 如果接3.3V，IIC地址为0x69。
#define MPU9250_ADDR			0x68	// MPU9250器件地址
#define MPU6500_ID1				0x71  	// MPU6500器件ID1
#define MPU6500_ID2				0x73 	// MPU6500器件ID2

// AK8963磁力计地址和ID
#define AK8963_ADDR				0x0C	// AK8963器件地址
#define AK8963_ID				0x48	// AK8963器件ID

// MPU6500内部寄存器
#define SELF_TEST_X_ACCEL		0x0D	// 13. 加速度计X轴自检寄存器 Accelerometer Self-Test Registers
#define SELF_TEST_Y_ACCEL		0x0E	// 14. 加速度计Y轴自检寄存器 Accelerometer Self-Test Registers
#define SELF_TEST_Z_ACCEL		0x0F	// 15. 加速度计Z轴自检寄存器 Accelerometer Self-Test Registers
 
#define SMPLRT_DIV 				0x19	// 25. 采样率分配寄存器 			Sample Rate Divider
#define CONFIG					0x1A	// 26. 配置寄存器 					Configuration
#define GYRO_CONFIG				0x1B	// 27. 陀螺仪配置寄存器 			Gyroscope Configuration
#define ACCEL_CONFIG			0x1C	// 28. 加速度计配合寄存器 			Accelerometer Configuration

#define FIFO_EN					0x23	// 35. FIFO使能寄存器 				FIFO Enable

#define INT_PIN_CFG				0x37	// 55. INT引脚/旁路启用配置寄存器 	INT Pin / Bypass Enable Configuration
#define INT_ENABLE				0x38	// 56. 中断使能寄存器 				Interrupt Enable

#define TEMP_OUT_H				0x41	// 65. 温度计测量值寄存器高位		Temperature Measurement
#define TEMP_OUT_L				0x42	// 66. 温度计测量值寄存器低位		Temperature Measurement
#define GYRO_XOUT_H				0x43	// 67. 陀螺仪X轴测量值寄存器高位	Gyroscope Measurements
#define GYRO_XOUT_L				0x44	// 68. 陀螺仪X轴测量值寄存器低位	Gyroscope Measurements
#define GYRO_YOUT_H				0x45	// 69. 陀螺仪Y轴测量值寄存器高位	Gyroscope Measurements
#define GYRO_YOUT_L				0x46	// 70. 陀螺仪Y轴测量值寄存器低位	Gyroscope Measurements
#define GYRO_ZOUT_H				0x47	// 71. 陀螺仪Z轴测量值寄存器高位	Gyroscope Measurements
#define GYRO_ZOUT_L				0x48	// 72. 陀螺仪Z轴测量值寄存器低位	Gyroscope Measurements

#define USER_CTRL				0x6A	// 106.用户控制寄存器 				User Control
#define PWR_MGMT_1				0x6B	// 107.电源管理1寄存器 				Power Management 1
#define PWR_MGMT_2				0x6C	// 108.电源管理2寄存器 				Power Management 2


#define WHO_AM_I				0x75	// 117.器件ID寄存器					Who Am I

// AK8963磁力计内部寄存器
#define WIA						0x00	// 器件ID寄存器 					Deviece ID，默认值为0x48
#define CNTL          	  		0X0A    // 控制寄存器 						Control
#define RSV            			0X0B	// 保留 							Reserved DO NOT ACCESS



/********** 函数声明 **********/
u8 MPU9250_Write_Byte(u8 addr, u8 reg, u8 data);
u8 MPU9250_Read_Byte(u8 addr, u8 reg);
u8 MPU9250_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf);
u8 MPU9250_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf);

u8 Gyro_FS_Sel(u8 fs);
u8 Accel_FS_Sel(u8 fs);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_LPF(u16 lpf);
#endif

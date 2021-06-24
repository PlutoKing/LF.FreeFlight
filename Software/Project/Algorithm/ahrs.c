/** --------------------------------------------------
* @File		: ahrs.h
* @Author	: Xu Zhe
* @Date		: 2021-06-24
* @Brief	: Attitude Heading Reference System
			  姿态航向参考系统，结算姿态
---------------------------------------------------*/

/********** 头 文 件 **********/
#include "ahrs.h"

/********** 变    量 **********/
float q0 = 1;		// quaternion e0
float q1 = 0;		// quaternion e1
float q2 = 0;		// quaternion e2
float q3 = 0;     	// quaternion e3

/********** 方    法 **********/
/** --------------------------------------------------
* @Function	: AHRS_Update
* @Brief	: 姿态航向参考系统更新
* @param	: void
* @return	: void
---------------------------------------------------*/
void AHRS_Update(void)
{

}


/*********** Copyright © 2021 蓝风实验室 ***********/

/*************************************************
* 文件名：moter_cntl.h     
* 说明：该模块为2个电机控制的底层处理
*       包括协议组装，协议拆解，底层驱动控制
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    MOTER_CNTL_H_
#define    MOTER_CNTL_H_
#include "msg_output.h"

#define  MOTER_DEBUG_PRINT

/*!    电机状态          */
typedef enum Moter_SW{
	Moter_SW_Off,
	Moter_SW_On
}Moter_SW;

/*!    电机状态          */
typedef enum Moter_State{
	Moter_Stop,
	Moter_Forward,
	Moter_Backward
}Moter_Dir;


/*!    电机方向        
typedef enum Moter_Dirt{
	Mo_Forward,    //0
	Mo_For_Left,   //1
	Mo_For_Rig,    //2
	Mo_Back,       //3
	Mo_Back_Left,  //4
	Mo_Back_Rig,   //5
}Moter_Dirt;
*/

/*!    电机状态          */
typedef struct Moter_Info
{
	Moter_SW  sw;            //开关
	Moter_Dir dir_sta;      //方向状态
	//Moter_Dirt dir;          //方向
	unsigned char angle;      //方向角度
	unsigned char ctnl_mode;  //控制方式  0PWM控制 1速度档位控制   2具体速度值控制
	unsigned char speedLevel;         //速度档位  
	unsigned char leftMoterDuty;         // 左电机pwm  PWM控制模式下有用
	unsigned char rightMoterDuty;         // 右电机pwm PWM控制模式下有用
	int speed;              
}Moter_Info;




int BSP_moter_init(Moter_Info *moInfo);
Moter_Info * get_moterObj(void);
void get_settingMoveAngle(const Moter_Info *moInfo, Moter_Dir *dir, unsigned char *ang);
void set_moterSwitch(Moter_Info *moInfo, Moter_SW sw);

/******************协议部分函数，不需要调用****************************/
int moter_cmdCntl(const unsigned char *data, int size);

void moter_cmdAns(unsigned char *dataBuff, int cmd, unsigned char succ_flg);

#endif




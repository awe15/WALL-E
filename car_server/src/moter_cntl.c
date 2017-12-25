/*************************************************
* 文件名：moter_cntl.c     
* 说明：该模块为电机控制的底层处理
*       包括协议组装，协议拆解，底层驱动控制
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#include "moter_cntl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  

static Moter_Info moter_ctnl;       //控制电机组
static int pwm_fd;
static int dir_io_fd;
#define   PWM_DEV         "/dev/pwmdrv"  
#define   DIRC_IO_DEV     "/dev/cardrv"  

#define   IS_MOTER_CNTL_CLOSE   do {    \
					if (moInfo->sw == Moter_SW_Off)   \
					{\
						printf("moter is not open\n");\
						return;\
					}\
				     }while(0)


/*************************************************
* 函数名: moter_init
* 说明：电机状态初始化，在正式控制电机前需要做的
* 输入参数：moInfo
* 输出参数：
**************************************************/
static void moter_init(Moter_Info *moInfo)
{
	moInfo->sw = Moter_SW_Off;
	moInfo->dir_sta = Moter_Stop;
	moInfo->angle = 90;     //forward
	moInfo->speedLevel = 0;
	moInfo->leftMoterDuty = 0;
	moInfo->rightMoterDuty = 0;	
	moInfo->speed = -1;               //暂时都没用到
	moInfo->ctnl_mode = 1;            //默认档位控制
}



/*************************************************
* 函数名: get_moterObj
* 说明：电机状态初始化，在正式控制电机前需要做的
* 输入参数：
* 输出参数：moInfo
**************************************************/
Moter_Info * get_moterObj(void)
{
	return &moter_ctnl;
}

/*************************************************
* 函数名: moter_speedCtnl
* 说明：改变电机运动
* 输入参数：moInfo
* 输出参数：
**************************************************/
static void moter_speedCtnl(Moter_Info *moInfo)
{
	char buf[4] = {0,0,0,0};
	unsigned char d_val = 0;
	unsigned char base_speed = 0;
	unsigned char slow_speed = 0;
	unsigned char fast_speed = 0;
	unsigned char tmp_angle = 0;
	switch (moInfo->speedLevel)
	{
		case 0: 
			buf[0] = buf[1] = buf[2] = buf[3] = 0;
			write(pwm_fd, buf, sizeof(buf));
			return;
		case 1: 
			base_speed = 10;
			break;
		case 2:
			base_speed = 25;
			break;
		case 3:
			base_speed = 30;
			break;
		case 4:
			base_speed = 45;
			break;
		case 5:
			base_speed = 60;
			break;
		default:break;
	}

	if (moInfo->dir_sta == Moter_Backward)
	{
		buf[0] = buf[2] = 2;
		tmp_angle = 180 - moInfo->angle;
	}
	else if (moInfo->dir_sta == Moter_Forward)
	{
		buf[0] = buf[2] = 1;
		tmp_angle = moInfo->angle;
	}
	
	char  tmp = base_speed * (0.8*(double)((tmp_angle>90)?(tmp_angle-90):(90-tmp_angle))/90);
	#ifdef  MOTER_DEBUG_PRINT
		printf("tmp speed:%d, base speed:%d\n", tmp, base_speed); 
	#endif
	slow_speed = base_speed - tmp;
	fast_speed = base_speed + tmp;    

	if (fast_speed >= 100)
		fast_speed = 99;
	if (tmp_angle > 90)
	{
		buf[3] = slow_speed;
		buf[1] = fast_speed;
	}
	else
	{
		buf[1] = slow_speed;
		buf[3] = fast_speed;
	}
	write(pwm_fd, buf, sizeof(buf));		
}

/*************************************************
* 函数名: moter_change
* 说明：改变电机运动
* 输入参数：moInfo
* 输出参数：
**************************************************/
static void moter_change(Moter_Info *moInfo)
{
	char buf[4] = {0,0,0,0};

	IS_MOTER_CNTL_CLOSE;

	if (moInfo->dir_sta==Moter_Stop)
	{//停止输出
		buf[0] = buf[1] = buf[2] = buf[3] = 0;
		write(pwm_fd, buf, sizeof(buf));
	}
	else
	{//根据速度档位和方向控制电机
		switch (moInfo->ctnl_mode)
		{
			case 0: //0 PWM控制
			{
				buf[0] = buf[2] = ((moInfo->dir_sta==Moter_Backward) ? 2 : 1);
				buf[1] = moInfo->leftMoterDuty;
				buf[3] = moInfo->rightMoterDuty;
				write(pwm_fd, buf, sizeof(buf));
				break;
			}
			case 1: //1 速度档位控制
			{
				moter_speedCtnl(moInfo);
				break;
			}
			case 2: //2 具体速度值控制
			{
				break;
			}
			default:
			#ifdef  MOTER_DEBUG_PRINT
				printf("error control mode\n"); 
			#endif
				break;
		}					
	}
}



/*************************************************
* 函数名: moter_setPWM

* 说明：设置电机占空比
* 输入参数：moInfo
*           level       速度档位
* 输出参数：
**************************************************/
static void moter_setPWM(Moter_Info *moInfo, unsigned char l_pwm, unsigned char r_pwm)
{
	IS_MOTER_CNTL_CLOSE;
	if ((l_pwm > 100) || (r_pwm > 100)) 
	{
	#ifdef  MOTER_DEBUG_PRINT
		printf("moter's setting pwm is out of range\n"); 
	#endif
	}
	moInfo->leftMoterDuty = l_pwm;
	moInfo->rightMoterDuty = r_pwm;
}



/*************************************************
* 函数名: moter_chSpeedLevl
* 说明：设置电机速度档位
* 输入参数：moInfo
*           level       速度档位
* 输出参数：
**************************************************/
static void moter_setSpeedLevel(Moter_Info *moInfo, unsigned char level)
{
	IS_MOTER_CNTL_CLOSE;
	if (level > 6)
		return;

	if (moInfo->dir_sta == Moter_Stop)
	{
	#ifdef  MOTER_DEBUG_PRINT
		printf("car is stop\n");
	#endif 
		return;
	}
	
	//改变占空比
	moInfo->speedLevel = level;
	#ifdef  MOTER_DEBUG_PRINT
		printf("set current speed level: %d\n", moInfo->speedLevel);
	#endif 
}



/*************************************************
* 函数名: get_settingMoveAngle
* 说明：得到方向角度
* 输入参数：方向状态,   角度
* 输出参数：
**************************************************/
void get_settingMoveAngle(const Moter_Info *moInfo, Moter_Dir *dir, unsigned char *ang)
{
	IS_MOTER_CNTL_CLOSE;
	*dir = moInfo->dir_sta;
	*ang = moInfo->angle;
}


/*************************************************
* 函数名: moter_setMoveAngle
* 说明：设置方向角度
* 输入参数：方向状态,   角度
* 输出参数：
**************************************************/
static void moter_setMoveAngle(Moter_Info *moInfo, unsigned char ang)
{
	IS_MOTER_CNTL_CLOSE;
	if (ang > 180)
	{
	#ifdef  MOTER_DEBUG_PRINT
		printf("moter's angle setting value is out of value\n");
	#endif
		return; 
	}
	moInfo->angle = ang;
	#ifdef  MOTER_DEBUG_PRINT
		printf("moter's angle setting value: %d\n", ang);
	#endif
}


/*************************************************
* 函数名: moter_setDirt
* 说明：设置电机控制模式
* 输入参数：moInfo
* 输出参数：
**************************************************/
static void moter_setDirct(Moter_Info *moInfo, unsigned char mode)
{
	IS_MOTER_CNTL_CLOSE;
	moInfo->ctnl_mode = mode;
}


/*************************************************
* 函数名: moter_dirCtnl
* 说明：电机开关控制
* 输入参数：moInfo
*           dir
* 输出参数：
**************************************************/
void moter_dirCtnl(Moter_Info *moInfo, Moter_Dir dir)
{
	IS_MOTER_CNTL_CLOSE;

	if (dir == Moter_Forward)
	{	
		moInfo->dir_sta = Moter_Forward;
#ifdef  MOTER_DEBUG_PRINT
		printf("set car forward\n");
#endif 
	}
	else if (dir == Moter_Backward)
	{
		moInfo->dir_sta = Moter_Backward;
#ifdef  MOTER_DEBUG_PRINT
		printf("set car backward\n");
#endif 
	}
	else if (dir == Moter_Stop)
	{
		moInfo->dir_sta = Moter_Stop;
#ifdef  MOTER_DEBUG_PRINT
		printf("set car off\n");
#endif 
	}

}


/*************************************************
* 函数名: set_moterSwitch
* 说明：电机状态复位
* 输入参数：Moter_Info
* 输出参数：
*************************************************/
void set_moterSwitch(Moter_Info *moInfo, Moter_SW sw)
{
	moter_init(moInfo);
	moInfo->sw = Moter_SW_On;
	moter_change(moInfo);    //开或关都要恢复到初始
	moInfo->sw = sw;

#ifdef  MOTER_DEBUG_PRINT
	if (moInfo->sw != Moter_SW_Off)
		printf("moter control switch is open\n");
	else
		printf("moter control switch is close\n");
#endif
}


/*************************************************
* 函数名: moter_cmdAns
* 说明：电机协议命令应答
* 输入参数：dataBuff   发送数据缓冲区
*                     (这里数据需要足够大，至少能够容纳一帧完整的通信数据)
*           cmd        应答命令
*           succ_flg   是否响应成功
* 输出参数： 
**************************************************/
void moter_cmdAns(unsigned char *dataBuff, int cmd, unsigned char succ_flg)
{
/*
	dataBuff[1] = 0x01;  //命令
	dataBuff[2] = cmd;
	dataBuff[3] = 0x1a;
	dataBuff[4] = (succ_flg==0) ? 0x0f : 0x1f;
	dataBuff[5] = 0;
	dataBuff[6] = 0;  
	commData_pack(dataBuff, 7);
*/
}





/*************************************************
* 函数名: BSP_moter_init
* 说明：电机硬件初始化，处于静止状态，方向默认向前
*       该函数只和底层配置有关，不涉及控制
* 输入参数：
* 输出参数：

**************************************************/
int BSP_moter_init(Moter_Info *moInfo)
{
	static char first_init_flg = 0;
	if (!first_init_flg)
	{
	#ifdef  MOTER_DEBUG_PRINT
		printf("Initialize the moter hardware\n");
	#endif
		//打开PWM驱动
	    	pwm_fd = open(PWM_DEV, O_RDWR);  
	    	if (pwm_fd < 0) 
		{  
			#ifdef  MOTER_DEBUG_PRINT
	       		printf("open the pwm driver failed!\n"); 
			#endif		
	       		return -1;  
	    	}  
		else
		{
			#ifdef  MOTER_DEBUG_PRINT
			printf("open the pwm driver success!\n"); 
			#endif	
		}
		moter_init(moInfo);
		moter_change(moInfo);
		first_init_flg = 1;
	}
	else
	{
		#ifdef  MOTER_DEBUG_PRINT
		printf("moter hardware had been initialized！\n"); 
		#endif	
	}
	return 0;
}





/*************************************************
* 函数名: moter_cmdCntl
* 说明：电机协议命令解析控制
*       该函数只能由上层解析控制。
* 输入参数：data        接收数据
*           size        数据大小 
* 输出参数：命令号
**************************************************/
int moter_cmdCntl(const unsigned char *data, int size)
{
	unsigned char sendbuf[200] = {0};
	if (data[0] != 0x10)
		return 0;
#ifdef  MOTER_DEBUG_PRINT
	printf("moter command number: %d\n", data[1]);
#endif 
	switch (data[1])
	{
	case 0x10:  //开关,方向控制
		if (data[2] == 0x00)     //stop
		{
			moter_dirCtnl(&moter_ctnl, Moter_Stop);
			moter_setSpeedLevel(&moter_ctnl, 0);
		}
		else if (data[2] == 0x01)  //forward
		{
			moter_dirCtnl(&moter_ctnl, Moter_Forward);	
			moter_setSpeedLevel(&moter_ctnl, 0);
		}
		else if (data[2] == 0x02)   //back
		{
			moter_dirCtnl(&moter_ctnl, Moter_Backward);
			moter_setSpeedLevel(&moter_ctnl, 0);
		}
		moter_change(&moter_ctnl);
		break;
	case 0x11:  //方向速度档位控制
		if (data[3] <= 0x06) //&& (data[3] >= 0x00)) 
		{
			moter_setDirct(&moter_ctnl, 1);
			moter_setSpeedLevel(&moter_ctnl, data[3]);  //档位
                        moter_setMoveAngle(&moter_ctnl, data[2]);   //方向
			moter_change(&moter_ctnl);
		}		
		break;
	case 0x12:  break;//方向速度控制（没功能）
	case 0x14:  //左右电机单独PWM控制
		moter_setPWM(&moter_ctnl, data[3], data[5]);
		moter_setDirct(&moter_ctnl, 0);
		break;
	case 0x15:  //速度/档位及方向状态查询

		break;
	default:
#ifdef  MOTER_DEBUG_PRINT
		printf("command number error\n");
#endif 
		return 0;
	}
	return data[1];
}

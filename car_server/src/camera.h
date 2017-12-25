/*************************************************
* 文件名：camera.h     
* 说明：该模块两个摄像头的底层协议处理和命令执行
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef    CAMERA_H_
#define    CAMERA_H_
#include "msg_output.h"

#define  CAMERA_DEBUG_PRINT

#define  CAMERA_MAX_ANS_LEN     20

/*!    开关状态          */
typedef enum Camera_SW_Sta{
	Camera_SW_Off,
	Camera_SW_On

}Camera_SW_Sta;


/*!    摄像头控制信息          */
typedef struct Camera_Info
{
	Camera_SW_Sta  sw;          //开关状态
	volatile char ans_flg;     //应答数据标识
	int camera1_port;          //摄像头1的默认端口为8070
	int camera2_port;          //摄像头2的默认端口为8090
	int camera_pid1;            //摄像头1线程PID
	int camera_pid2;            //摄像头2线程PID
	unsigned char *pbuf;     
}Camera_Info;




Camera_Info *get_cameraObj();                                           //获得对象    
void BSP_camera_init(Camera_Info *cameraInfo, int port1, int port2);    //初始化
void set_cameraSwitch(Camera_Info *cameraInfo, Camera_SW_Sta sw);       //设置开关

//协议处理回调
int camera_cmdCntl(const unsigned char *data, int size);

#endif 


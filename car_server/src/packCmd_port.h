/*************************************************
* 文件名：packCmd_port.h     
* 说明：该模块为手动添加选择协议中要处理的命令
* 版本：V1.0
* 作者：AWE
* 修改日期：2017/06/06
**************************************************/
#ifndef     PACK_CMD_PORT_H
#define     PACK_CMD_PORT_H
#include <stdio.h>
#include "msg_output.h"

#define     PACK_ANALYSIS_PRINT

typedef  int (*Cmd_Analy)(const unsigned char *data, int size);

/*!--------------------自定义命令修改----------------------*/
/*! 命令列表 */
typedef enum Cnt_Cmd{
	Link_Cmd = 0,
	Unlink_Cmd,
	TestLink_Cmd,
	Camera_Cmd,
	Moter_Cmd,
	Max_Cmd
}Cnt_Cmd;

/*! 协议对应命令值列表 */
int cmdList[Max_Cmd]={
	0x01, 
	0x02,
	0x03,
	0x20,
	0x10
};

/*!预包含头文件 */
#include "moter_cntl.h"
#include "camera.h"
#include "car_cntl.h"

/*! 命令处理表 */
Cmd_Analy cmd_anylsTab[Max_Cmd] = {
	&car_sw_cmdCntl,
	&car_sw_cmdCntl,
	&car_link_cmdCntl,
	&camera_cmdCntl,
	&moter_cmdCntl,
};

/*!---------------------------------------------------*/

//void cmdData_analysis(const unsigned char *data, int size);

/*************************************************
* 函数名：cmdData_analysis
* 说明：命令数据解析
* 输入参数：data      数据
*           size      数据长度
* 输出参数：
**************************************************/
void cmdData_analysis(const unsigned char *data, int size)
{
	int i;
	#ifdef PACK_ANALYSIS_PRINT
		printf("cmd code: %d\n", data[0]);
	#endif
	for (i = 0; i < (int)Max_Cmd; i++)
	{
		if (cmdList[i] == data[0])
			break;            
	}
	if (i == (int)Max_Cmd)
	{
	#ifdef PACK_ANALYSIS_PRINT
		printf("not exist current cmd protocol!\n");
	#endif
		return;
	}
	cmd_anylsTab[i](data, size);
}



#endif

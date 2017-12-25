/*************************************************
* �ļ�����packCmd_port.h     
* ˵������ģ��Ϊ�ֶ����ѡ��Э����Ҫ���������
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#ifndef     PACK_CMD_PORT_H
#define     PACK_CMD_PORT_H
#include <stdio.h>
#include "msg_output.h"

#define     PACK_ANALYSIS_PRINT

typedef  int (*Cmd_Analy)(const unsigned char *data, int size);

/*!--------------------�Զ��������޸�----------------------*/
/*! �����б� */
typedef enum Cnt_Cmd{
	Link_Cmd = 0,
	Unlink_Cmd,
	TestLink_Cmd,
	Camera_Cmd,
	Moter_Cmd,
	Max_Cmd
}Cnt_Cmd;

/*! Э���Ӧ����ֵ�б� */
int cmdList[Max_Cmd]={
	0x01, 
	0x02,
	0x03,
	0x20,
	0x10
};

/*!Ԥ����ͷ�ļ� */
#include "moter_cntl.h"
#include "camera.h"
#include "car_cntl.h"

/*! ������ */
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
* ��������cmdData_analysis
* ˵�����������ݽ���
* ���������data      ����
*           size      ���ݳ���
* ���������
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

/*************************************************
* �ļ�����camera.c     
* ˵������ģ����������ͷ�ĵײ�Э�鴦�������ִ��
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#include "camera.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

static Camera_Info camera_obj;
static unsigned char sendbuf[CAMERA_MAX_ANS_LEN] = {0};   //��������

/*************************************************
* ������: int2str
* ˵�������ε��ַ�����ת��
* ���������str         �ַ����
*           num         ת������
* ��������� 
**************************************************/
static void int2str(char *str, int num)
{
	int i = 0, j =0;
	char tmp;
	if (num == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return;
	}
	while (num != 0)
	{
		str[i++] = num % 10 + '0';
		num /= 10;
	}
	str[i--] = '\0';

	while (j <= i)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		j++;
		i--;
	}

}


/*************************************************
* ������: camera_init
* ˵��: ����ͷ��ʼ��
* ���������cameraInfo
*           port1      ����ͷ1
*           port2      ����ͷ2
* ��������� 
**************************************************/
static void camera_init(Camera_Info *cameraInfo, int port1, int port2)
{
	cameraInfo->camera1_port = port1;     
	cameraInfo->camera2_port = port2;  
	cameraInfo->sw = Camera_SW_Off;  
	cameraInfo->camera_pid1 = 0;            //����ͷ1�߳�PID
	cameraInfo->camera_pid2 = 0;            //����ͷ2�߳�PID  
	cameraInfo->ans_flg = 0;
	cameraInfo->pbuf = sendbuf;
}



/*************************************************
* ������: BSP_camera_init
* ˵��: ����ͷ��ʼ��
* ���������cameraInfo
*           port1      ����ͷ1
*           port2      ����ͷ2
* ��������� 
**************************************************/
void BSP_camera_init(Camera_Info *cameraInfo, int port1, int port2)
{
	camera_init(cameraInfo, port1, port2);
	#ifdef  CAMERA_DEBUG_PRINT
		PRINT("camera initalized!\nport: %d, %d\n", camera_obj.camera1_port, camera_obj.camera2_port);
	#endif 
}


/*************************************************
* ������: is_cameraSendData
* ˵�����Ƿ�������Ҫ����
* ���������sendbuff   ���ش����͵�����ָ��
* ��������� 1��Ҫ����,0����Ҫ����
**************************************************/
char is_cameraSendData(Camera_Info *cameraInfo, unsigned char *sendbuff)
{
	sendbuff = cameraInfo->pbuf;
	return cameraInfo->ans_flg;
}


/*************************************************
* ������: set_cameraSendFlg
* ˵�������÷���״̬
* ���������
* ��������� 
**************************************************/
void set_cameraSendFlg(Camera_Info *cameraInfo, char flg)
{
	if (!flg)
	{
		cameraInfo->ans_flg = 0;
		memset(cameraInfo->pbuf, 0 ,CAMERA_MAX_ANS_LEN);
	}
	else
		cameraInfo->ans_flg;
}



/*************************************************
* ������: set_cameraSwitch
* ˵��: ����ͷ���ؿ���
* ���������cameraInfo
*           sw
* ��������� 
**************************************************/
void set_cameraSwitch(Camera_Info *cameraInfo, Camera_SW_Sta sw)
{
	cameraInfo->sw = sw;
	#ifdef  CAMERA_DEBUG_PRINT

	if (sw == Camera_SW_Off)
		PRINT("camera set closed\n");
	else
		PRINT("camera set opened\n");

	#endif 
}


/*************************************************
* ������: get_cameraSwitch
* ˵��: �������ͷ����
* ���������cameraInfo
* ��������� 
**************************************************/
Camera_SW_Sta get_cameraSwitch(Camera_Info *cameraInfo)
{
	return cameraInfo->sw;
}

/*************************************************
* ������: get_cameraObj
* ˵��: �������ͷ��һʵ������
* ���������
* ��������� cameraInfo
**************************************************/
Camera_Info *get_cameraObj()
{
	return &camera_obj;
}




/*************************************************
* ������: start_cameraProc1
* ˵������������ͷ1����
* ���������
* ��������� �Ƿ�ɹ�
**************************************************/
static int start_cameraProc1(Camera_Info *cameraInfo)
{	
	if (cameraInfo->camera_pid1 != 0)
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("camera process 1 been existed!\n");
		#endif 
		return 0;
	}

	char port_str[40] = "output_http.so -p ";
	char * const camera1_argv[] = {"mjpg_streamer", "-i", "input_uvc.so -d /dev/video0", "-o", port_str, NULL};	
	int2str((port_str+strlen(port_str)), cameraInfo->camera1_port);
	cameraInfo->camera_pid1 = getpid();
	if (cameraInfo->camera_pid1 == 0)
	{		
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("start camera process 1 error!\n");
		#endif 
		return 0;
	}
	else
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("start camera process 1!\n");
			PRINT("port info is: %s!\n", port_str);
			PRINT("camera1 process pid is %d\n", cameraInfo->camera_pid1);
		#endif 		
		execv("/usr/bin/mjpg_streamer", camera1_argv);
		return 1;
	}
}

/*************************************************
* ������: start_cameraProc2
* ˵������������ͷ2����
* ���������
* ��������� �Ƿ�ɹ�
**************************************************/
static int start_cameraProc2(Camera_Info *cameraInfo)
{
	PRINT("camera2 process pid is %d\n", cameraInfo->camera_pid2);
	if (cameraInfo->camera_pid2 != 0)
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("camera process 2 had been existed!\n");
		#endif 
		return 0;
	}

	char port_str[40] = "output_http.so -p ";
	char * const camera2_argv[] = {"mjpg_streamer", "-i", "input_uvc.so -d /dev/video1", "-o", port_str, NULL};	
	int2str((port_str+strlen(port_str)), cameraInfo->camera2_port);
	cameraInfo->camera_pid2 = getpid();
	if (cameraInfo->camera_pid2 == 0)
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("start camera process 2 error!\n");
		#endif 
		return 0;
	}
	else
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("start camera process 2 \n");
			PRINT("port info is: %s!\n", port_str);
			PRINT("camera2 process pid is %d\n", cameraInfo->camera_pid2);
		#endif 
		execv("/usr/bin/mjpg_streamer", camera2_argv);
		return 1;
	}
}


/*************************************************
* ������: kill_cameraProc
* ˵�����ر�����ͷ����
* ���������
* ��������� �Ƿ�ɹ�
**************************************************/
static int kill_cameraProc(Camera_Info *cameraInfo)
{
	char str[50] = {0};
	if ((cameraInfo->camera_pid1 == 0) && (cameraInfo->camera_pid2 == 0))
	{
		#ifdef  CAMERA_DEBUG_PRINT
			PRINT("Kill camera process error!\n");
		#endif 
		return 0;
	}
	else
	{
		if (cameraInfo->camera_pid1 != 0)
		{
			sprintf(str, "kill -9 %d", cameraInfo->camera_pid1); 
			system(str);        //ɾ������1
			cameraInfo->camera_pid1 = 0;
		}
		if (cameraInfo->camera_pid2 != 0)
		{
			memset(str, 0, 50);
			sprintf(str, "kill -9 %d", cameraInfo->camera_pid2); 
			system(str);        //ɾ������2
			cameraInfo->camera_pid2 = 0;
		}
		return 1;
	}	
}



/*************************************************
* ������: camera_cmdAns
* ˵��������ͷЭ������Ӧ��
* ���������dataBuff   �������ݻ�����
*                     (����������Ҫ�㹻�������ܹ�����һ֡������ͨ������)
*           cmd        Ӧ������
*           succ_flg   �Ƿ���Ӧ�ɹ�
* ��������� 
**************************************************/
void camera_cmdAns(unsigned char *dataBuff, int cmd, char succ_flg)
{
	dataBuff[0] = 0x20;  
	dataBuff[1] = cmd | 0xa0;
	dataBuff[2] = (succ_flg==0) ? 0x00 : 0x01;
	commData_pack(dataBuff, 3);
}



/*************************************************
* ������: camera_cmdCntl
* ˵��������ͷЭ�������������
*       �ú���ֻ�����ϲ�������ơ�
* ���������data        ��������
*           size        ���ݴ�С 
* ��������������
**************************************************/
int camera_cmdCntl(const unsigned char *data, int size)
{
#ifdef  CAMERA_DEBUG_PRINT
	PRINT("camera command number: %d\n", data[0]);
#endif 
	if (get_cameraSwitch(&camera_obj) == Camera_SW_Off)
	{
	#ifdef  CAMERA_DEBUG_PRINT
		PRINT("camera switch is closed\n");
	#endif 
		return data[1];
	}
	switch (data[1])
	{
		case 0x01://���ؿ���
		{	
			if (data[2] == 0x0f || data[2] == 0x01)
			{
				//��������ͷapp					
				if (camera_obj.camera_pid1 != 0)
					return data[1];
				camera_obj.camera_pid1 = fork();
				switch(camera_obj.camera_pid1)
				{
					case -1:exit(1);break;
					case 0: 
					{
						start_cameraProc1(&camera_obj);
						exit(0);
						camera_cmdAns(sendbuf,data[2], 1);
						set_cameraSendFlg(&camera_obj, 1);
					}
					default: break;
				}
			}

			if (data[2] == 0x0f || data[2] == 0x02)
			{
				if (camera_obj.camera_pid2 != 0)
					return data[1];
				camera_obj.camera_pid2 = fork();
				switch(camera_obj.camera_pid2)
				{
					case -1:exit(1);break;
					case 0: 
					{
						start_cameraProc2(&camera_obj);
						exit(0);
						camera_cmdAns(sendbuf,data[2], 1);
						set_cameraSendFlg(&camera_obj, 1);
					}
					default: break;
				}
			}
			if (data[2] == 0x00)
			{
			#ifdef  CAMERA_DEBUG_PRINT
				PRINT("camera close\n");
			#endif 
				kill_cameraProc(&camera_obj);//�ر�����ͷ
				camera_cmdAns(sendbuf,data[2], 1);
			}
			break;
		}
		case 0x02: //���ö˿�
		{	
			int t_port1 = data[2]*256+data[3];
			int t_port2 = data[3]*256+data[4];
			if ((t_port1 > 1000) && (t_port2 > 1000)) 
			{
				camera_obj.camera1_port = t_port1;
				camera_obj.camera2_port = t_port2;	
			#ifdef  CAMERA_DEBUG_PRINT
				PRINT("camera port 1:%d\n", camera_obj.camera1_port);
				PRINT("camera port 2:%d\n", camera_obj.camera2_port);
			#endif 
				camera_cmdAns(sendbuf,data[2], 1);
				//�ر�ԭ���Ķ˿����¿���
				
			}
			else
			{
			#ifdef  CAMERA_DEBUG_PRINT
				PRINT("camera port para is bad");
			#endif 
			 	camera_cmdAns(sendbuf,data[2], 0);
			}	
			break;
		}
		default:
#ifdef  CAMERA_DEBUG_PRINT
			PRINT("camera command error!\n");
#endif 
			break;
	}
	return data[1];
}


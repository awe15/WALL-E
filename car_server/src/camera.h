/*************************************************
* �ļ�����camera.h     
* ˵������ģ����������ͷ�ĵײ�Э�鴦�������ִ��
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#ifndef    CAMERA_H_
#define    CAMERA_H_
#include "msg_output.h"

#define  CAMERA_DEBUG_PRINT

#define  CAMERA_MAX_ANS_LEN     20

/*!    ����״̬          */
typedef enum Camera_SW_Sta{
	Camera_SW_Off,
	Camera_SW_On

}Camera_SW_Sta;


/*!    ����ͷ������Ϣ          */
typedef struct Camera_Info
{
	Camera_SW_Sta  sw;          //����״̬
	volatile char ans_flg;     //Ӧ�����ݱ�ʶ
	int camera1_port;          //����ͷ1��Ĭ�϶˿�Ϊ8070
	int camera2_port;          //����ͷ2��Ĭ�϶˿�Ϊ8090
	int camera_pid1;            //����ͷ1�߳�PID
	int camera_pid2;            //����ͷ2�߳�PID
	unsigned char *pbuf;     
}Camera_Info;




Camera_Info *get_cameraObj();                                           //��ö���    
void BSP_camera_init(Camera_Info *cameraInfo, int port1, int port2);    //��ʼ��
void set_cameraSwitch(Camera_Info *cameraInfo, Camera_SW_Sta sw);       //���ÿ���

//Э�鴦��ص�
int camera_cmdCntl(const unsigned char *data, int size);

#endif 


/*************************************************
* �ļ�����moter_cntl.h     
* ˵������ģ��Ϊ2��������Ƶĵײ㴦��
*       ����Э����װ��Э���⣬�ײ���������
* �汾��V1.0
* ���ߣ�AWE
* �޸����ڣ�2017/06/06
**************************************************/
#ifndef    MOTER_CNTL_H_
#define    MOTER_CNTL_H_
#include "msg_output.h"

#define  MOTER_DEBUG_PRINT

/*!    ���״̬          */
typedef enum Moter_SW{
	Moter_SW_Off,
	Moter_SW_On
}Moter_SW;

/*!    ���״̬          */
typedef enum Moter_State{
	Moter_Stop,
	Moter_Forward,
	Moter_Backward
}Moter_Dir;


/*!    �������        
typedef enum Moter_Dirt{
	Mo_Forward,    //0
	Mo_For_Left,   //1
	Mo_For_Rig,    //2
	Mo_Back,       //3
	Mo_Back_Left,  //4
	Mo_Back_Rig,   //5
}Moter_Dirt;
*/

/*!    ���״̬          */
typedef struct Moter_Info
{
	Moter_SW  sw;            //����
	Moter_Dir dir_sta;      //����״̬
	//Moter_Dirt dir;          //����
	unsigned char angle;      //����Ƕ�
	unsigned char ctnl_mode;  //���Ʒ�ʽ  0PWM���� 1�ٶȵ�λ����   2�����ٶ�ֵ����
	unsigned char speedLevel;         //�ٶȵ�λ  
	unsigned char leftMoterDuty;         // ����pwm  PWM����ģʽ������
	unsigned char rightMoterDuty;         // �ҵ��pwm PWM����ģʽ������
	int speed;              
}Moter_Info;




int BSP_moter_init(Moter_Info *moInfo);
Moter_Info * get_moterObj(void);
void get_settingMoveAngle(const Moter_Info *moInfo, Moter_Dir *dir, unsigned char *ang);
void set_moterSwitch(Moter_Info *moInfo, Moter_SW sw);

/******************Э�鲿�ֺ���������Ҫ����****************************/
int moter_cmdCntl(const unsigned char *data, int size);

void moter_cmdAns(unsigned char *dataBuff, int cmd, unsigned char succ_flg);

#endif




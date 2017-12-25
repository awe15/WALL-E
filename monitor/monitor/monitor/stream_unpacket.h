/********************************************************
* �ļ�����
* ˵����
* 
* ����ͷ�ļ���
*
*
* ����˵����1.����VIDEO���󣬲���Ҫ��Ӧtcp/ip��ͨ��monConnector
*           2.����init��ʼ��TCP/IPͨ����������HTTP����
*          
* �汾��
*********************************************************/
#ifndef  STREAM_UNPACKET_H
#define  STREAM_UNPACKET_H

#include<stdlib.h>
#include "mon_connector.h"
#include "port.h"
	

class video
{
private:

	int iWidth;   /* ���: һ���ж��ٸ����� */
	int iHeight;  /* �߶�: һ���ж��ٸ����� */
	int iBpp;     /* һ�������ö���λ����ʾ */
	int iLineBytes;  /* һ�������ж����ֽ� */
	int iTotalBytes; /* �����ֽ��� */ 
	unsigned char *aucPixelDatas;  /* �������ݴ洢�ĵط� */

	monConnector *channel;     //��Ӧһ��tcp/ip����ͨ��
	MUTEX read_mutex;         
	Sig   read_fin_sig;

private:
	long int getFileLen(char *FreeBuf, int *FreeLen);
	long int http_recv(char **lpbuff, long int size);

public:
	video(monConnector *connector);
	~video();

	//��ʼ��
	bool init();

	//���������
	int video::get_videoStream(char *retStream);



};






#endif



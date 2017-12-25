#ifndef MJPG_STREAM_H
#define MJPG_STREAM_H

/* ͼƬ���������� */
typedef struct PixelDatas {
	int iWidth;   /* ���: һ���ж��ٸ����� */
	int iHeight;  /* �߶�: һ���ж��ٸ����� */
	int iBpp;     /* һ�������ö���λ����ʾ */
	int iLineBytes;  /* һ�������ж����ֽ� */
	int iTotalBytes; /* �����ֽ��� */ 
	unsigned char *aucPixelDatas;  /* �������ݴ洢�ĵط� */
}T_PixelDatas, *PT_PixelDatas;

typedef struct VideoBuf {
	T_PixelDatas tPixelDatas;
	int iPixelFormat;
	/* signal fresh frames */
	pthread_mutex_t db;
	pthread_cond_t  db_update;
}T_VideoBuf, *PT_VideoBuf;


typedef struct VideoRecv {
	char *name;  //�ṹ������

	int (*Connect_To_Server)(int *SocketClient, const char *ip);//���ӵ�mjpg-streamer�������ϣ�socket�ļ������ָ�룬IP��ַ�ַ�����
	int (*DisConnect_To_Server)(int *SocketClient);//�Ͽ����ӣ�socket�ļ������ָ�룩
	int (*Init)(int *SocketClient)��//��ʼ������
		int (*GetFormat)(void);//�����Ƶ��ʽ
	int (*Get_Video)(int *SocketClient, PT_VideoBuf ptVideoBuf);//�����Ƶ������socket�ļ������ָ�룬PT_VideoBuf �ṹ�壩
	struct VideoRecv *ptNext;  //���ڶ���ṹ�����ϵ
}T_VideoRecv, *PT_VideoRecv;


int VideoRecvInit(void);
void ShowVideoRecv(void);
int RegisterVideoRecv(PT_VideoRecv ptVideoRecv);
int Video_Recv_Init(void);
PT_VideoRecv GetVideoRecv(char *pcName);

#endif 